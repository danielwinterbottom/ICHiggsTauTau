#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/SVFitTestRun2.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "boost/lexical_cast.hpp"
#include <boost/algorithm/string.hpp>
#include "HiggsTauTau/interface/SVFitServiceRun2.h"
#include <stdlib.h>
#include "boost/format.hpp"
#include "boost/filesystem.hpp"

namespace ic {

  SVFitTestRun2::SVFitTestRun2(std::string const& name) : ModuleBase(name), channel_(channel::et) {
  
    out_file_ = NULL;
    out_tree_ = NULL;
    out_cand1_ = NULL;
    out_cand2_ = NULL;
    out_met_ = NULL;

    //out_event_hash_ = 0;
    out_event_ = 0;
    out_lumi_ = 0;
    out_run_ = 0;
    out_objects_hash_ = 0;
    decay_mode_ = 0;
    run_mode_ = 0;
    fail_mode_ = 0;
    require_inputs_match_ = true;
    dm1_ = -1;
    dm2_ = -1;

    file_counter_ = 0;
    event_counter_ = 0;
    split_ = 5000;
    outname_ = "svfit_test";
    fullpath_ = "SVFIT_2012/";

    MC_ = false;
  }

  SVFitTestRun2::~SVFitTestRun2() {
    ;
  }

  int SVFitTestRun2::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "SVFitTestRun2" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
 //decay mode 0 mt 1 em 2 et 3 tt for now
    if (channel_ == channel::et) decay_mode_ = 2;
    if (channel_ == channel::mt) decay_mode_ = 0;
    if (channel_ == channel::tt) decay_mode_ = 3;
    if (channel_ == channel::em) decay_mode_ = 1;
    std::cout << boost::format(param_fmt()) % "decay_mode"              % decay_mode_;
    std::cout << boost::format(param_fmt()) % "run_mode"                % run_mode_;
    std::cout << boost::format(param_fmt()) % "fail_mode"               % fail_mode_;
    std::cout << boost::format(param_fmt()) % "require_inputs_match"    % require_inputs_match_;
    std::cout << boost::format(param_fmt()) % "dilepton_label"          % dilepton_label_;
    std::cout << boost::format(param_fmt()) % "met_label"               % met_label_;

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
    std::cout << boost::format(param_fmt()) % "folder"         % outname_;
    if (fullpath_ == "") {
      fullpath_ = boost::filesystem::current_path().string();
    }
    std::cout << boost::format(param_fmt()) % "directory"      % fullpath_;

    total_path_ = operator/(fullpath_, folder_p);
    boost::filesystem::create_directories(total_path_);
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
          unsigned    event    = 0;
          unsigned    lumi     = 0;
          unsigned    run      = 0;
          ULong64_t    objects_hash  = 0;
          double      svfit_mass    = 0;
          Candidate * svfit_vector  = nullptr;
          otree->SetBranchAddress("event"  , &event);
          otree->SetBranchAddress("lumi"  , &lumi);
          otree->SetBranchAddress("run"  , &run);
          otree->SetBranchAddress("objects_hash", &objects_hash);
          otree->SetBranchAddress("svfit_mass"  , &svfit_mass);
          otree->SetBranchAddress("svfit_vector"  , &svfit_vector);
          for (unsigned evt = 0; evt < otree->GetEntries(); ++evt) {
            otree->GetEntry(evt);
            if(!MC_) mass_map[tri_unsigned(run, lumi, event)] = std::make_pair(objects_hash, svfit_mass);
            else p4_map[tri_unsigned(run, lumi, event)] = std::make_pair(objects_hash, *svfit_vector);
          }
          ofile->Close();
          delete ofile;
        } 
      }
    }

  return 0;
}

int SVFitTestRun2::Execute(TreeEvent *event) {

  if (run_mode_ == 0) return 0;
  EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
  std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(dilepton_label_);
  Candidate c1 = *(dilepton.at(0)->GetCandidate("lepton1"));
  Candidate c2 = *(dilepton.at(0)->GetCandidate("lepton2"));
 //Temporarily extract vector
  //std::vector<Met*> met_vec = event->GetPtrVec<Met>(met_label_);
//  Met met = *(met_vec.at(0));
  Met met = *(event->GetPtr<Met>(met_label_));
  
  //std::size_t event_hash = RunLumiEvtHash(eventInfo->run(), eventInfo->lumi_block(), eventInfo->event());
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
      out_tree_->Branch("event", &out_event_, "event/i");
      out_tree_->Branch("lumi", &out_lumi_, "lumi/i");
      out_tree_->Branch("run", &out_run_, "run/i");
      out_tree_->Branch("objects_hash", &out_objects_hash_, "objects_hash/l");
      out_tree_->Branch("decay_mode", &decay_mode_);
      out_tree_->Branch("lepton1", &out_cand1_);
      out_tree_->Branch("dm1", &dm1_, "dm1/I");
      out_tree_->Branch("lepton2", &out_cand2_);
      out_tree_->Branch("dm2", &dm2_, "dm2/I");
      out_tree_->Branch("met", &out_met_);
      ++file_counter_;
    }
    //out_event_hash_ = event_hash;
    if(channel_==channel::tt){
     Tau * tau1 = dynamic_cast<Tau *>(dilepton.at(0)->GetCandidate("lepton1"));
     dm1_ = tau1->decay_mode();
     }
    if(channel_==channel::tt || channel_==channel::et||channel_==channel::mt){
     Tau * tau2 = dynamic_cast<Tau *>(dilepton.at(0)->GetCandidate("lepton2"));
     dm2_ = tau2->decay_mode();
    }
    out_event_ = eventInfo->event();
    out_lumi_ = eventInfo->lumi_block();
    out_run_ = eventInfo->run();
    out_objects_hash_ = objects_hash;
    out_cand1_ = &c1;
    out_cand2_ = &c2;
    out_met_ = &met;
    out_tree_->Fill();
    ++event_counter_;
  }

  if (run_mode_ == 2) {
    bool fail_state = false;
    //Different actions for Markov-Chain or Vegas integration
     if(!MC_){
        auto it = mass_map.find(tri_unsigned(eventInfo->run(),eventInfo->lumi_block(), eventInfo->event()));
        if (it != mass_map.end()) {
          ;
          if (require_inputs_match_ && it->second.first != objects_hash) {
            fail_state = true;
          } else {
            if (it->second.second < 1.) {
              std::cout << "Warning, SVFit mass is invalid: " << it->second.second << std::endl;
            } else {
              event->Add("svfitMass", it->second.second);
            }
          }
        } else {
          fail_state = true;
        }
     } else {  
        auto it = p4_map.find(tri_unsigned(eventInfo->run(),eventInfo->lumi_block(), eventInfo->event()));
        if (it != p4_map.end()) {
          ;
          if (require_inputs_match_ && it->second.first != objects_hash) {
            fail_state = true;
          } else {
            if (it->second.second.M() < 1.) {
              std::cout << "Warning, SVFit mass is invalid: " << it->second.second.M() << std::endl;
            } else {
              event->Add("svfitMass", it->second.second.M());
              event->Add("svfitHiggs", it->second.second);
            }
          }
        } else {
          fail_state = true;
        }
     }
    if (fail_state) {
      if (fail_mode_ == 0) {
        std::cout << "Warning, SVFitTestRun2 mass not found!" << std::endl;
        event->Add("svfitMass", double(-100.0));
      } else if (fail_mode_ == 1) {
        std::cout << "Error, SVFitTestRun2 mass not found!" << std::endl;
        throw;
      } else {
        std::cout << "Calculating mass on-the-fly" << std::endl;
        if (decay_mode_ == 0) {
          event->Add("svfitMass", SVFitServiceRun2::SVFitMassMuHad(&c1, &c2, dm2_, &met, MC_));
        } else if (decay_mode_ == 1){
          event->Add("svfitMass", SVFitServiceRun2::SVFitMassEleMu(&c1, &c2, &met, MC_));
        } else if (decay_mode_ == 2){
          event->Add("svfitMass", SVFitServiceRun2::SVFitMassEleHad(&c1, &c2, dm2_, &met, MC_));
        } else if (decay_mode_ == 3){
          event->Add("svfitMass", SVFitServiceRun2::SVFitMassHadHad(&c1, dm1_, &c2, dm2_, &met, MC_));
        }
      }
    }
  }

  return 0;
  }
  int SVFitTestRun2::PostAnalysis() {
    if (out_file_) {
      out_file_->Write();
      delete out_tree_;
      out_file_->Close();
      delete out_file_;
      out_file_ = NULL;
    }
    return 0;
  }

  void SVFitTestRun2::PrintInfo() {
    ;
  }

}
