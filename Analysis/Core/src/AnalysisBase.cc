#include "Core/interface/AnalysisBase.h"
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "boost/format.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/bind.hpp"
#include "TFile.h"
#include "TTree.h"
#include "TObject.h"
#include "TDirectory.h"
#include "Core/interface/ModuleBase.h"
#include "Core/interface/TreeEvent.h"

namespace ic {

AnalysisBase::AnalysisBase(std::string const& analysis_name,
                           std::vector<std::string> const& input,
                           std::string const& tree_path, int64_t const& events)
    : analysis_name_(analysis_name),
      input_files_(input),
      tree_path_(tree_path),
      events_to_process_(events),
      events_processed_(0),
      skim_path_(std::string("")),
      print_module_list_(false),
      ttree_caching_(false),
      stop_on_failed_file_(true),
      retry_on_fail_(false),
      retry_pause_(5),
      retry_attempts_(1),
      timings_(false) {}

AnalysisBase::~AnalysisBase() { ; }

void AnalysisBase::AddModule(ic::ModuleBase* module_ptr) {
  AddModule("default", module_ptr);
}

void AnalysisBase::AddModule(std::string const& seq_name,
                             ic::ModuleBase* module_ptr) {
  auto seq_it = std::find_if(
      seqs_.begin(), seqs_.end(), [&](ModuleSequence const& seq) {
        return seq.name == seq_name;
      });
  if (seq_it == seqs_.end()) {
    seq_it = seqs_.insert(seqs_.end(), ModuleSequence(seq_name));
  }
  (*seq_it).modules.push_back(module_ptr);
}

void AnalysisBase::DoEventSetup() {}

bool AnalysisBase::PostModule(int status) {
  if (status > 0) {
    return false;
  } else {
    return true;
  }
}

int AnalysisBase::RunAnalysis() {
  TFile* file_ptr = nullptr;
  TTree* tree_ptr = nullptr;
  // weighted_yields_.resize(modules_.size());
  std::cout << std::string(78, '-') << "\n";
  std::cout << boost::format("%-15s : %-60s\n") % "Analysis" % analysis_name();
  std::cout << boost::format("%-15s : %-60s\n") % "Input Files" %
                   input_files_.size();
  std::cout << boost::format("%-15s : %-60s\n") % "Tree Path" % tree_path();
  std::cout << boost::format("%-15s : %-60s\n") % "Events" % events_to_process_;

  if (print_module_list_) {
    for (auto & seq : seqs_) {
      std::cout << boost::format("%-15s : %-60s\n") % "Sequence" % seq.name;
      std::cout << std::string(78, '-') << "\n";
      for (unsigned i = 0; i < seq.modules.size(); ++i) {
        std::cout << "  - " << (seq.modules)[i]->ModuleName() << "\n";
      }
      std::cout << std::string(78, '-') << "\n";
    }
  }

  // Skimming
  bool do_skim = (skim_path_ != "");
  if (do_skim) {
    std::cout << ">> Skimming enabled to path: " << skim_path_ << "\n";
    std::cout << ">> Skimming triggered by modules in sequences:\n";
    for (auto & seq : seqs_) {
      if (seq.skim_point < 0) seq.skim_point = seq.modules.size() - 1;
      std::cout << boost::format("%-25s : %-40s\n") % seq.name %
                       seq.modules.at(seq.skim_point)->ModuleName();
    }
  }
  if (ttree_caching_) {
    std::cout << ">> TTree caching enabled\n";
  }

  for (auto & seq : seqs_) {
    seq.counters.resize(seq.modules.size());
    seq.proc_counters.resize(seq.modules.size());
    seq.timers.resize(seq.modules.size());
    std::cout << std::string(78, '-') << "\n";
    std::cout << boost::format("%-15s : %-60s\n") % "Pre-analysis" % seq.name;
    std::cout << std::string(78, '-') << "\n";
    std::for_each(seq.modules.begin(), seq.modules.end(),
                  boost::bind(&ModuleBase::PreAnalysis, _1));
  }

  std::cout << std::string(78, '-') << "\n";
  std::cout << "Beginning Analysis Sequence" << std::endl;
  std::cout << std::string(78, '-') << "\n";

  // Timers if we want them
  std::chrono::time_point<std::chrono::system_clock> start, end;

  for (unsigned file = 0; file < input_files_.size(); ++file) {
    // Stop looping through files if user-specified events have
    // been processed
    if (events_processed_ == events_to_process_) break;

    std::vector<std::string> in_name;
    std::string out_name;
    if (do_skim) {
      boost::split(in_name, input_files_[file], boost::is_any_of("/"));
      if (in_name.size() > 0) {
        out_name = in_name[in_name.size() - 1];
      } else {
        std::cerr << ">> Unable to get input file name\n";
        continue;
      }
    }
    file_ptr = TFile::Open(input_files_[file].c_str());
    if (!file_ptr) {
      std::cerr << ">> Error: Unable to open file \"" << input_files_[file]
                << "\"\n";
      if (retry_on_fail_ && retry_attempts_ > 0) {
        for (unsigned att = 0; att < retry_attempts_; ++att) {
          std::cout << ">> Retry attempt " << att + 1 << "/" << retry_attempts_
                    << " in " << retry_pause_ << " seconds\n";
          sleep(retry_pause_);
          file_ptr = TFile::Open(input_files_[file].c_str());
          if (file_ptr) {
            std::cout << ">> File opened successfully\n";
            break;
          } else {
            std::cout << ">> File open failed\n";
          }
        }
      }
      if (stop_on_failed_file_ && !file_ptr) {
        throw std::runtime_error("Input file could not be opened");
      }
    }
    tree_ptr = dynamic_cast<TTree*>(gDirectory->Get(tree_path_.c_str()));
    if (!tree_ptr) {
      std::cerr << ">> Error: Unable to find TTree \"" << tree_path_
                << "\" in file \"" << input_files_[file] << "\"\n";
      if (stop_on_failed_file_) {
        throw std::runtime_error("Error: input tree could not be located");
      } else {
        continue;
      }
    }
    std::cout << ">> File: " << input_files_[file] << "\n";
    TFile* outf = nullptr;
    TTree* outtree = nullptr;
    if (do_skim) {
      outf = new TFile((skim_path_ + out_name).c_str(), "RECREATE");
      if (!outf->IsOpen()) {
        throw std::runtime_error("Skim output file could not be opened");
      }
      outf->cd();
      std::vector<std::string> as_vec;
      boost::split(as_vec, tree_path_, boost::is_any_of("/"));
      if (as_vec.size() >= 1) {
        for (unsigned i = 0; i < as_vec.size() - 1; ++i) {
          if (!gDirectory->GetDirectory(as_vec[i].c_str())) {
            gDirectory->mkdir(as_vec[i].c_str());
          }
          gDirectory->cd(as_vec[i].c_str());
        }
        outtree = tree_ptr->CloneTree(0);
        std::cout << ">> Skim: " << skim_path_ + out_name << std::endl;
      } else {
        throw std::runtime_error(
            "Something went wrong with preparing the skim TTree");
      }
    }

    if (ttree_caching_) {
      tree_ptr->SetCacheSize(100000000);
      tree_ptr->SetCacheLearnEntries(100);
    }

    unsigned tree_events = tree_ptr->GetEntries();
    event_.SetTree(tree_ptr);
    DoEventSetup();
    bool exception_check=false;
    for (unsigned evt = 0; evt < tree_events; ++evt) {
      if(exception_check){
	try{
	  if (ttree_caching_) tree_ptr->LoadTree(evt);
	}
	catch(const std::bad_alloc&){
	  std::cout<<"An exception occured when loading tree "<<std::endl;
	  return -1;
	}
      }
      else if (ttree_caching_) tree_ptr->LoadTree(evt);
      bool skim_event = false;
      for (auto & seq : seqs_) {
        event_.SetEvent(evt);
        bool track_event = false;
        for (unsigned m = 0; m < seq.modules.size(); ++m) {
          if (timings_) start = std::chrono::system_clock::now();
          ++(seq.proc_counters[m]);
	  bool exception_check=true;
	  int status;
	  if(exception_check){
	    try{
	      status = (seq.modules)[m]->Execute(&event_);
	    }
	    catch(const std::bad_alloc&){
	      std::cout<<"An exception occured in module "<<(seq.modules)[m]->ModuleName()<<" of type std::bad_alloc"<<std::endl;
	      return -1;
	    }
	  }
	  else status = (seq.modules)[m]->Execute(&event_);
          if (timings_) {
            end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed = end-start;
            seq.timers[m] += elapsed.count();
          }
          if (!PostModule(status)) {
            if (status == 1) {
              if (track_event)
                std::cout << ">> Event rejected by module "
                          << seq.modules[m]->ModuleName() << " in sequence "
                          << seq.name << "\n";
              break;
            }
          }
          if (status == 0) ++(seq.counters[m]);
          if (status == 3) {
            ++(seq.counters[m]);
            track_event = true;
          }
          if (do_skim && static_cast<int>(m) == seq.skim_point)
            skim_event = true;
        }
      }
      if (skim_event) {
        tree_ptr->GetEntry(evt);
        outtree->Fill();
      }
      ++events_processed_;
      if (events_processed_ % 10000 == 0) {
        std::cout << ">> Processed " << events_processed_ << " events...\r"
                  << std::flush;
      }
      if (events_processed_ == events_to_process_) break;
    }
    file_ptr->Close();
    delete file_ptr;
    if (do_skim) {
      if (outtree) outtree->Write();
      if (outf) outf->Close();
      delete outf;
    }
  }

  std::cout << ">> Processing Complete: " << events_processed_
            << " events processed\n";
  for (auto & seq : seqs_) {
    std::cout << std::string(78, '-') << "\n";
    if (!timings_) {
      std::cout << boost::format("%-13s : %-22s %14s\n") % "Post-analysis" %
                       seq.name % "Events Passed";
    } else {
      std::cout << boost::format("%-13s : %-22s %14s %10s %13s\n") %
                       "Post-analysis" % seq.name % "Evts Passed" %
                       "Time [s]" % "Time/Evt [ms]";
    }
    std::cout << std::string(78, '-') << "\n";
    for (unsigned i = 0; i < (seq.modules).size(); ++i) {
      if (!timings_) {
        std::cout << boost::format("%-38s %14s\n") %
                         seq.modules[i]->ModuleName() % seq.counters[i];
      } else {
        std::cout << boost::format("%-38s %14s %10.3f %13.4f\n") %
                         seq.modules[i]->ModuleName() % seq.counters[i] %
                         seq.timers[i] %
                         (1000. * seq.timers[i] /
                          static_cast<double>((seq.proc_counters[i])));
      }
    }
    std::for_each(seq.modules.begin(), seq.modules.end(),
                  boost::bind(&ModuleBase::PostAnalysis, _1));
  }
  return 0;
}

void AnalysisBase::SetTTreeCaching(bool const& value) {
  ttree_caching_ = value;
}
void AnalysisBase::StopOnFileFailure(bool const& value) {
  stop_on_failed_file_ = value;
}

void AnalysisBase::RetryFileAfterFailure(unsigned pause_in_seconds,
                                         unsigned retry_attempts) {
  retry_on_fail_ = true;
  retry_attempts_ = retry_attempts;
  retry_pause_ = pause_in_seconds;
}

void AnalysisBase::WriteSkimHere() {
  WriteSkimHere("default");
}

void AnalysisBase::WriteSkimHere(std::string const& seq_name) {
  auto seq_it = std::find_if(
      seqs_.begin(), seqs_.end(), [&](ModuleSequence const& seq) {
        return seq.name == seq_name;
      });
  if (seq_it == seqs_.end()) {
    seq_it = seqs_.insert(seqs_.end(), ModuleSequence(seq_name));
  }
  if (seq_it->modules.size() == 0) {
    std::cout << ">> Request to skim before first module is ignored\n";
  } else {
    seq_it->skim_point = seq_it->modules.size() - 1;
  }
}

void AnalysisBase::CalculateTimings(bool const& value) {
  timings_ = value;
}
}
