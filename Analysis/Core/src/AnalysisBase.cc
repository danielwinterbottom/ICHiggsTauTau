#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/AnalysisBase.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include <boost/algorithm/string.hpp>

#include <stdio.h>
#include <thread>
#include "TFile.h"
#include "TTree.h"
#include "boost/format.hpp"
#include "TTreeCache.h"

namespace ic {

  AnalysisBase::AnalysisBase(   std::string const& analysis_name, 
                                std::vector<std::string> const& input,
                                std::string const& tree_path,
                                std::string const& tree_name,
                                unsigned const& events  ) {
    input_file_paths_     = input;
    events_to_process_    = events;
    analysis_name_        = analysis_name;
    tree_path_            = tree_path;
    tree_name_            = tree_name;
    events_processed_     = 0;
    notify_on_fail_       = false;
    notify_evt_on_fail_   = false;
    skim_path_            = "";
    print_module_list_    = false;
    ttree_caching_        = false;
    stop_on_failed_file_  = true;
    retry_on_fail_        = false; 
    retry_pause_          = 5;
    retry_attempts_       = 1;
    skim_after_module_    = -1;
  }

  AnalysisBase::~AnalysisBase() {
      ;
  }

  void AnalysisBase::AddModule(ic::ModuleBase *module_ptr) {
    modules_.push_back(module_ptr);
  }

  void AnalysisBase::DoEventSetup() {
  }

  bool AnalysisBase::PostModule(int status) {
    if (status > 0) {

      return false;
    } else {
      return true;
    }
  }

  int AnalysisBase::RunAnalysis() {
    TFile *file_ptr = NULL;
    TTree *tree_ptr = NULL;
    weighted_yields_.resize(modules_.size());
    bool do_skim = (skim_path_ != "");
    if (do_skim) {
      std::cout << "Info in <ic::AnalysisBase>: Skimming mode enabled" << std::endl;
      if (skim_after_module_ < 0) skim_after_module_ = modules_.size() - 1;
    }
    if (ttree_caching_) std::cout << "Info in <ic::AnalysisBase>: TTree caching enabled" << std::endl;
  
    if (print_module_list_) {
      std::cout << "-------------------------------------" << std::endl;
      std::cout << "Module List" << std::endl;
      std::cout << "-------------------------------------" << std::endl;
      for (unsigned i = 0; i < modules_.size(); ++i) {
        std::cout << modules_[i]->ModuleName() << std::endl;
      }
    }
    //std::for_each(modules_.begin(), modules_.end(), boost::bind(&ModuleBase::PrintInfo,_1));
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Pre-Analysis Module Output" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::for_each(modules_.begin(), modules_.end(), boost::bind(&ModuleBase::PreAnalysis,_1));
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Beginning Main Analysis Sequence" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    for (unsigned file = 0; file < input_file_paths_.size(); ++file) {
        //Stop looping through files if user-specified events have
        //been processed
        if (events_processed_ == events_to_process_) break;

        std::vector<std::string> in_name;
        std::string out_name;
        if (do_skim) {
          boost::split(in_name, input_file_paths_[file], boost::is_any_of("/"));
          if (in_name.size() > 0) {
            out_name = in_name[in_name.size() - 1];
          } else {
            std::cerr << "Unable to get input file name!" << std::endl;
            continue;
          }
        }
        file_ptr = TFile::Open(input_file_paths_[file].c_str());
        if (!file_ptr) {
          std::cerr << "Warning: Unable to open file \"" << input_file_paths_[file] <<
          "\"" << std::endl;
          if (retry_on_fail_ && retry_attempts_ > 0) {
            for (unsigned att = 0; att < retry_attempts_; ++att) {
              std::cout << "Retry attempt " << att+1 << "/" << retry_attempts_ << " in " << retry_pause_ << " seconds" << std::endl;
              sleep(retry_pause_);
              file_ptr = TFile::Open(input_file_paths_[file].c_str());
              if (file_ptr) {
                std::cout << "File opened successfully" << std::endl;
                break;
              } else {
                std::cout << "File open failed" << std::endl;
              }
            }
          }
          if (stop_on_failed_file_ && !file_ptr) {
            throw;
          } 
        }
        gDirectory->Cd(tree_path_.c_str());
        tree_ptr = dynamic_cast<TTree*>(gDirectory->Get(tree_name_.c_str()));
        if (!tree_ptr) {
          std::cerr << "Warning: Unable to find TTree \"" << tree_name_ <<
          "\" in file \"" << input_file_paths_[file] << "\"" << std::endl;
          if (stop_on_failed_file_) {
            throw;
          } else {            
            continue;
          }
        }
        std::cout << "-- " << input_file_paths_[file] << std::endl;
        TFile *outf = NULL;
        TTree *outtree = NULL;
        if (do_skim) {
          outf  = new TFile((skim_path_+out_name).c_str(), "RECREATE");
          if (!outf->IsOpen()) {
            std::cerr << "Error: Could not open output skim file for writing, an exception will be thrown" << std::endl;
            throw;
          }
          outf->cd();
          gDirectory->mkdir(tree_path_.c_str());
          gDirectory->cd(tree_path_.c_str());
          outtree = tree_ptr->CloneTree(0);
          std::cout << "----> " << skim_path_+out_name << std::endl;
        }

        if (ttree_caching_) {
          tree_ptr->SetCacheSize(100000000);
          tree_ptr->SetCacheLearnEntries(100);          
        }

        unsigned tree_events = tree_ptr->GetEntries();
        event_.SetTree(tree_ptr);
        DoEventSetup();
        for (unsigned evt = 0; evt < tree_events; ++evt) {
          if (ttree_caching_) tree_ptr->LoadTree(evt);
          event_.SetEvent(evt);
          EventInfo const* eventInfo = event_.GetPtr<EventInfo>("eventInfo");

          for (unsigned module = 0; module < modules_.size(); ++module) {
            int status = modules_[module]->Execute(&event_);
            if (!PostModule(status)) {
              if (notify_on_fail_) {
                unsigned evt = eventInfo->event();
                unsigned run = eventInfo->run();
                if (notify_run_event_.find(std::make_pair(run,evt)) != notify_run_event_.end()) {
                std::cout << "Run " << run << ", event " << evt << " rejected by module: " << modules_[module]->ModuleName() << std::endl;
                }
              }
              if (notify_evt_on_fail_) {
                unsigned run = eventInfo->run();
                unsigned evt = eventInfo->event();
                if (notify_event_.find(evt) != notify_event_.end()) {
                std::cout << "Run " << run << ", event " << evt << " rejected by module: " << modules_[module]->ModuleName() << std::endl;
                }
              }
              if (status == 1) break;
            }
            if (status == 0) modules_[module]->IncreaseProcessedCount();
            if (status == 0) weighted_yields_[module] += eventInfo->total_weight();

            if (do_skim && module == skim_after_module_) {
              tree_ptr->GetEntry(evt);
              outtree->Fill();
            }
          }
          ++events_processed_;
          if (events_processed_%10000 == 0) {
            std::cout << "Processed " << events_processed_ << " events...\r" << std::flush;
          }
          if (events_processed_ == events_to_process_) break;
        }
        // tree_ptr->PrintCacheStats();
        file_ptr->Close();
        delete file_ptr;
        if (do_skim) {
          if (outtree) outtree->Write();
          if (outf) outf->Close();
          delete outf;
        }
    }
    std::cout << "Processing Complete: " << events_processed_ << " events were processed." << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Post-Analysis Module Output" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    for (unsigned i = 0; i < modules_.size(); ++i) {
      std::cout << boost::format("%-40s %-20s %-20s\n") % modules_[i]->ModuleName() % modules_[i]->EventsProcessed() % weighted_yields_[i];
    }
    std::for_each(modules_.begin(), modules_.end(), boost::bind(&ModuleBase::PostAnalysis,_1));
   return 0; 
  }


  void AnalysisBase::NotifyRunEvent(int const& run, int const& event) {
    notify_on_fail_ = true;
    notify_run_event_.insert(std::make_pair(run,event));
  }

  void AnalysisBase::NotifyEvent(int const& event) {
    notify_evt_on_fail_ = true;
    notify_event_.insert(event);
  }
  void AnalysisBase::SetTTreeCaching(bool const& value) {
    ttree_caching_ = value;
  }
  void AnalysisBase::StopOnFileFailure(bool const& value) {
    stop_on_failed_file_ = value;
  }

  void AnalysisBase::RetryFileAfterFailure(unsigned pause_in_seconds, unsigned retry_attempts) {
    retry_on_fail_ = true;
    retry_attempts_ = retry_attempts;
    retry_pause_ = pause_in_seconds;
  }

  void AnalysisBase::WriteSkimHere() {
    if (modules_.size() == 0) {
      std::cout << "Warning in <ic::AnalysisBase>: Request to write skim before first module is ignored" << std::endl;
    } else {
      std::cout << "Info in <ic::AnalysisBase>: Writing skim output after module \"" << modules_.back()->ModuleName() << "\"" << std::endl;
      skim_after_module_ = modules_.size() - 1;
    }
  }




}
