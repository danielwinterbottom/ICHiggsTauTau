#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/SVFitTest.h"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "boost/lexical_cast.hpp"
#include <boost/algorithm/string.hpp>
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SVFitService.h"
#include <stdlib.h>

#include "boost/filesystem.hpp"



namespace ic {

  SVFitTest::SVFitTest(std::string const& name) : ModuleBase(name), channel_(channel::et) {
  
    out_file_ = NULL;
    out_tree_ = NULL;
    out_cand1_ = NULL;
    out_cand2_ = NULL;
    out_met_ = NULL;

    out_event_hash_ = 0;
    out_objects_hash_ = 0;
    decay_mode_ = 0;
    run_mode_ = 0;
    fail_mode_ = 0;
    require_inputs_match_ = true;

    file_counter_ = 0;
    event_counter_ = 0;
    split_ = 5000;
    outname_ = "svfit_test";
    fullpath_ = "SVFIT_2012/";
  }

  SVFitTest::~SVFitTest() {
    ;
  }

  int SVFitTest::PreAnalysis() {
    std::cout << "***PreAnalysis Info for SVFitTest***" << std::endl;
    if (channel_ == channel::et) decay_mode_ = 0;
    if (channel_ == channel::etmet) decay_mode_ = 0;
    if (channel_ == channel::mt) decay_mode_ = 0;
    if (channel_ == channel::mtmet) decay_mode_ = 0;
    if (channel_ == channel::em) decay_mode_ = 1;
    std::cout << "Decay Mode: " << run_mode_ << std::endl;
    std::cout << "Run Mode: " << run_mode_ << std::endl;
    std::cout << "Fail Mode: " << fail_mode_ << std::endl;
    std::cout << "Require inputs Match: " << require_inputs_match_ << std::endl;
    std::cout << "Dilepton Label: " << dilepton_label_ << std::endl;
    std::cout << "MET Label: " << met_label_ << std::endl;


    // This code first strips the string ".root" from give filename
    // (if found) and appends "_SVIFIT".  This is then taken as the
    // name of the output folder.  If the user has specified a full
    // path (fullpath_) this folder will be created there, otherwise
    // it will be created in the current directory
    std::string sub(".root");
    std::string::size_type foundpos = outname_.find(sub);
    if ( foundpos != std::string::npos )
      outname_.erase(outname_.begin() + foundpos, outname_.begin() + foundpos + sub.length());
    outname_ += "_SVFIT";
    boost::filesystem::path folder_p(outname_);
    std::cout << "Using directory: " << outname_ << std::endl;
    if (fullpath_ == "") {
      std::cout << "No path specified for SVFitTest directories, using the working directory:" << std::endl;
      fullpath_ = boost::filesystem::current_path().string();
      std::cout << "--:" << fullpath_ << std::endl;
    } else {
      std::cout << "Path for SVFitTest directory specifed:" << std::endl;
      std::cout << "--:" << fullpath_ << std::endl;
    }
    total_path_ = operator/(fullpath_, folder_p);
    boost::filesystem::create_directories(total_path_);
    std::cout << "Total path:" << std::endl;
    std::cout << "--:" << total_path_.string() << std::endl;

    if (run_mode_ == 2) {
      boost::filesystem::directory_iterator it(total_path_);
      for (; it != boost::filesystem::directory_iterator(); ++it) {
        std::string path = it->path().string();
        if (path.find("output.root") != path.npos) {
          std::cout << "Reading TFile: " << path << std::endl;
          TFile *ofile = new TFile(path.c_str());
          if (!ofile) {
            std::cout << "Warning, unable to open file " << path << std::endl;
            continue;
          }
          TTree *otree = dynamic_cast<TTree *>(ofile->Get("svfit"));
          if (!otree) {
            std::cout << "Warning, unable to get tree in file " << path << std::endl;
            continue;
          }
          uint64_t    event_hash    = 0;
          uint64_t    objects_hash  = 0;
          double      svfit_mass    = 0;
          otree->SetBranchAddress("event_hash"  , &event_hash);
          otree->SetBranchAddress("objects_hash", &objects_hash);
          otree->SetBranchAddress("svfit_mass"  , &svfit_mass);
          for (unsigned evt = 0; evt < otree->GetEntries(); ++evt) {
            otree->GetEntry(evt);
            mass_map[event_hash] = std::make_pair(objects_hash, svfit_mass);
          }
          ofile->Close();
          delete ofile;
        } 
      }
    }

  return 0;
}

int SVFitTest::Execute(TreeEvent *event) {

  if (run_mode_ == 0) return 0;

  EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
  std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(dilepton_label_);
  Candidate c1 = *(dilepton.at(0)->GetCandidate("lepton1"));
  Candidate c2 = *(dilepton.at(0)->GetCandidate("lepton2"));
  Met met = *(event->GetPtr<Met>(met_label_));
  std::size_t event_hash = RunLumiEvtHash(eventInfo->run(), eventInfo->lumi_block(), eventInfo->event());
  std::size_t objects_hash = ObjectsHash(&c1, &c2, &met);

  if (run_mode_ == 1) {
    if (event_counter_%split_ == 0) {
      if (out_file_) {
        out_file_->Write();
        delete out_tree_;
        out_file_->Close();
        delete out_file_;
        out_file_ = NULL;
      }
      out_file_ = new TFile((total_path_.string()+"/svfit_"+boost::lexical_cast<std::string>(file_counter_)+"_input.root").c_str(),"RECREATE");
      out_tree_ = new TTree("svfit","svfit");
      out_tree_->Branch("event_hash", &out_event_hash_, "event_hash/l");
      out_tree_->Branch("objects_hash", &out_objects_hash_, "objects_hash/l");
      out_tree_->Branch("decay_mode", &decay_mode_);
      out_tree_->Branch("lepton1", &out_cand1_);
      out_tree_->Branch("lepton2", &out_cand2_);
      out_tree_->Branch("met", &out_met_);
      ++file_counter_;
    }
    out_event_hash_ = event_hash;
    out_objects_hash_ = objects_hash;
    out_cand1_ = &c1;
    out_cand2_ = &c2;
    out_met_ = &met;
    out_tree_->Fill();
    ++event_counter_;
  }

  if (run_mode_ == 2) {
    mass_map_const_it it = mass_map.find(event_hash);
    bool fail_state = false;
    if (it != mass_map.end()) {
      if (require_inputs_match_ && it->second.first != objects_hash) {
        std::cout << "Warning, objects hash does not match" << std::endl;
        fail_state = true;
      } else {
        event->Add("svfitMassTest", it->second.second);
      }
    } else {
      fail_state = true;
    }
    if (fail_state) {
      if (fail_mode_ == 0) {
        std::cout << "Warning, SVFitTest mass not found!" << std::endl;
        event->Add("svfitMass", double(-100.0));
      } else if (fail_mode_ == 1) {
        std::cout << "Error, SVFitTest mass not found!" << std::endl;
        throw;
      } else {
        std::cout << "Calculating mass on-the-fly" << std::endl;
        if (decay_mode_ == 0) {
          event->Add("svfitMassTest", SVFitService::SVFitMassLepHad(&c1, &c2, &met));
        } else {
          event->Add("svfitMassTest", SVFitService::SVFitMassLepLep(&c1, &c2, &met));
        }
      }
    }
  }

  return 0;
  }
  int SVFitTest::PostAnalysis() {
    if (out_file_) {
      out_file_->Write();
      delete out_tree_;
      out_file_->Close();
      delete out_file_;
      out_file_ = NULL;
    }
    return 0;
  }

  void SVFitTest::PrintInfo() {
    ;
  }

}
