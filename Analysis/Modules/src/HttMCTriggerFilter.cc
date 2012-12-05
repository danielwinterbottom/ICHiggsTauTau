#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttMCTriggerFilter.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/city.h"



namespace ic {

  HttMCTriggerFilter::HttMCTriggerFilter(std::string const& name) : ModuleBase(name),
    channel_(channel::et), mc_(mc::fall11_42X) {
    do_obj_match_ = true;
  }

  HttMCTriggerFilter::~HttMCTriggerFilter() {
    ;
  }
  int HttMCTriggerFilter::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HttMCTriggerFilter" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Channel: " << Channel2String(channel_) << std::endl;
    std::cout << "MC: " << MC2String(mc_) << std::endl;
    std::cout << "Require match to HLT object: " << do_obj_match_ << std::endl;
    std::cout << "1st Lepton Collection: " << lep1label_ << std::endl;
    std::cout << "2nd Lepton Collection: " << lep2label_ << std::endl;

    if (channel_ == channel::et) {
      if (mc_ == mc::fall11_42X) {
        trig_obj_label_   = "triggerObjectsEle18MediumTau20";
        elmu_filter_      = "hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter";
        tau_filter_       = "hltPFTauMediumIso20TrackMediumIso";
      } else if (mc_ == mc::summer12_53X) {
        trig_obj_label_   = "triggerObjectsEle22WP90RhoLooseTau20";
        elmu_filter_      = "hltEle22WP90RhoTrackIsoFilter";
        tau_filter_       = "hltIsoElePFTau20TrackLooseIso";
      }
    } else if (channel_ == channel::mt) {
      if (mc_ == mc::fall11_42X) {
        trig_obj_label_   = "triggerObjectsIsoMu15LooseTau15";
        elmu_filter_      = "hltSingleMuIsoL3IsoFiltered15";
        tau_filter_       = "hltPFTau15TrackLooseIso";
      } else if (mc_ == mc::summer12_53X) {
        trig_obj_label_   = "triggerObjectsIsoMu17LooseTau20";
        elmu_filter_      = "hltL3crIsoL1sMu14erORMu16erL1f0L2f14QL3f17QL3crIsoRhoFiltered0p15";
        tau_filter_       = "hltIsoMuPFTau20TrackLooseIso";
      }
    } else if (channel_ == channel::em) {
      if (mc_ == mc::fall11_42X) {
        mulow_label_      = "triggerObjectsMu8Ele17";
        muhigh_label_     = "triggerObjectsMu17Ele8";
        mulow_mu_         = "hltL1MuOpenEG12L3Filtered8";
        mulow_el_         = "hltMu8Ele17CaloIdTCaloIsoVLPixelMatchFilter";
        muhigh_mu_        = "hltL1Mu12EG5L3MuFiltered17";
        muhigh_el_        = "hltMu17Ele8CaloIdTPixelMatchFilter";
      } else if (mc_ == mc::summer12_53X) {
        mulow_label_      = "triggerObjectsMu8Ele17";
        muhigh_label_     = "triggerObjectsMu17Ele8";
        mulow_mu_         = "hltL1sL1Mu3p5EG12ORL1MuOpenEG12L3Filtered8";
        mulow_el_         = "hltMu8Ele17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
        muhigh_mu_        = "hltL1Mu12EG7L3MuFiltered17";
        muhigh_el_        = "hltMu17Ele8CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
      }
    }
    return 0;
  }

  int HttMCTriggerFilter::Execute(TreeEvent *event) {

    if (do_obj_match_) {
      if (channel_ == channel::et) {
        std::vector<Electron *> & elmus = event->GetPtrVec<Electron>(lep1label_);
        std::vector<Tau *> & taus = event->GetPtrVec<Tau>(lep2label_);
        std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trig_obj_label_);
        ic::erase_if(elmus, !boost::bind(IsFilterMatched, _1, objs, elmu_filter_, 0.5));
        ic::erase_if(taus, !boost::bind(IsFilterMatched, _1, objs, tau_filter_, 0.5));
      } else if (channel_ == channel::mt) {
        std::vector<Muon *> & elmus = event->GetPtrVec<Muon>(lep1label_);
        std::vector<Tau *> & taus = event->GetPtrVec<Tau>(lep2label_);
        std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trig_obj_label_);
        ic::erase_if(elmus, !boost::bind(IsFilterMatched, _1, objs, elmu_filter_, 0.5));
        ic::erase_if(taus, !boost::bind(IsFilterMatched, _1, objs, tau_filter_, 0.5));
      } else if (channel_ == channel::em) {
        std::vector<Electron *> & elecs = event->GetPtrVec<Electron>(lep1label_);
        std::vector<Muon *> & muons = event->GetPtrVec<Muon>(lep2label_);
        std::vector<TriggerObject *> const& mulow_objs = event->GetPtrVec<TriggerObject>(mulow_label_);
        std::vector<TriggerObject *> const& muhigh_objs = event->GetPtrVec<TriggerObject>(muhigh_label_);
        // Test Scenario 1: MuLow trigger fires
        std::vector<Electron *> elecs_copy = elecs;        
        std::vector<Muon *> muons_copy = muons;    
        erase_if(elecs_copy, !boost::bind(MinPtMaxEta, _1, 20.0, 999.));    
        erase_if(muons_copy, !boost::bind(MinPtMaxEta, _1, 10.0, 999.));    
        erase_if(elecs_copy, !boost::bind(IsFilterMatched, _1, mulow_objs, mulow_el_, 0.5));
        erase_if(muons_copy, !boost::bind(IsFilterMatched, _1, mulow_objs, mulow_mu_, 0.5));
        if (elecs_copy.size() > 0 && muons_copy.size() > 0) {
          elecs = elecs_copy;
          muons = muons_copy;
        } else { // Test Scenario 2: MuHigh trigger fires
          elecs_copy = elecs;        
          muons_copy = muons;    
          // EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
          // if (eventInfo->event() == 184633) {
          //   std::cout << muhigh_mu << "\t" << CityHash64(muhigh_mu) << std::endl;
          //   std::cout << muhigh_el << "\t" << CityHash64(muhigh_el) << std::endl;
          //   for (unsigned i = 0; i < muhigh_objs.size(); ++i) {
          //     std::cout << "[" << i << "]" << std::endl;
          //     muhigh_objs[i]->Print();
          //     std::vector<std::size_t> labels = muhigh_objs[i]->filters();
          //     for (unsigned j = 0; j < labels.size(); ++j) {
          //       std::cout << ":" << labels[j];
          //     }
          //     std::cout << std::endl;
          //   }
          //   std::cout << mulow_mu << "\t" << CityHash64(mulow_mu) << std::endl;
          //   std::cout << mulow_el << "\t" << CityHash64(mulow_el) << std::endl;
          //   for (unsigned i = 0; i < mulow_objs.size(); ++i) {
          //     std::cout << "[" << i << "]" << std::endl;
          //     mulow_objs[i]->Print();
          //     std::vector<std::size_t> labels = mulow_objs[i]->filters();
          //     for (unsigned j = 0; j < labels.size(); ++j) {
          //       std::cout << ":" << labels[j];
          //     }
          //     std::cout << std::endl;
          //   }
          // }
          erase_if(elecs_copy, !boost::bind(MinPtMaxEta, _1, 10.0, 999.));    
          erase_if(muons_copy, !boost::bind(MinPtMaxEta, _1, 20.0, 999.));
          erase_if(elecs_copy, !boost::bind(IsFilterMatched, _1, muhigh_objs, muhigh_el_, 0.5));
          erase_if(muons_copy, !boost::bind(IsFilterMatched, _1, muhigh_objs, muhigh_mu_, 0.5));

          elecs = elecs_copy;
          muons = muons_copy;
        }
      }
    }
      return 0;
  }
  int HttMCTriggerFilter::PostAnalysis() {
    return 0;
  }

  void HttMCTriggerFilter::PrintInfo() {
    ;
  }
}
