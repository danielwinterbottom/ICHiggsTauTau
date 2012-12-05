 #include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttDataTriggerFilter.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/city.h"


namespace ic {

  HttDataTriggerFilter::HttDataTriggerFilter(std::string const& name) : ModuleBase(name), channel_(channel::et) {
    do_obj_match_ = false;
  }

  HttDataTriggerFilter::~HttDataTriggerFilter() {
    ;
  }

  int HttDataTriggerFilter::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HttDataTriggerFilter" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Channel: " << Channel2String(channel_) << std::endl;
    std::cout << "Require match to HLT object: " << do_obj_match_ << std::endl;
    std::cout << "1st Lepton Collection: " << lep1label_ << std::endl;
    std::cout << "2nd Lepton Collection: " << lep2label_ << std::endl;
    return 0;
  }

  int HttDataTriggerFilter::Execute(TreeEvent *event) {
    TriggerPathPtrVec const& triggerPathPtrVec = 
    event->GetPtrVec<TriggerPath>("triggerPathPtrVec","triggerPaths");
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    unsigned run = eventInfo->run();
    bool path_found = false;
    bool mu_low_found = false;

    for (unsigned i = 0; i < triggerPathPtrVec.size(); ++i) {
      std::string name = triggerPathPtrVec[i]->name();
      triggerPathPtrVec[i]->prescale();
      if (channel_ == channel::et) {
        // 2011 Triggers
        if (run >= 160404 && run <= 163869 && name.find("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v") != name.npos) path_found = true;
        if (run >= 165088 && run <= 167913 && name.find("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v") != name.npos) path_found = true;
        if (run >= 170249 && run <= 173198 && name.find("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v") != name.npos) path_found = true;
        if (run >= 173236 && run <= 178380 && name.find("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v") != name.npos) path_found = true;
        if (run >= 178420 && run <= 180252 && name.find("HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v") != name.npos) path_found = true;
        // 2012 Triggers
        if (run >= 190456 && run <= 193751 && name.find("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v") != name.npos) path_found = true; 
        if (run >= 193752/* && run <= xxxxx*/ && name.find("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v") != name.npos) path_found = true;          
      } else if (channel_ == channel::mt) {
        //2011 Triggers
        if (run >= 160404 && run <= 163869 && name.find("HLT_IsoMu12_LooseIsoPFTau10_v") != name.npos) path_found = true;//215.634 pb
        if (run >= 165088 && run <= 173198 && name.find("HLT_IsoMu15_LooseIsoPFTau15_v") != name.npos) path_found = true; // 1787 pb
        if (run >= 173236 && run <= 180252 && name.find("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v") != name.npos) path_found = true; //2979 pb
        //2012 Triggers
        if (run >= 190456 && run <= 193751 && name.find("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v") != name.npos) path_found = true;          
        if (run >= 193752/* && run <= xxxxx*/ && name.find("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v") != name.npos) path_found = true;          
      } else if (channel_ == channel::em) {
        // Look for Mu Low trigger first
        if (run >= 160404 && run <= 167913 && name.find("HLT_Mu8_Ele17_CaloIdL_v") != name.npos) path_found = true;
        if (run >= 170249 && run <= 180252 && name.find("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v") != name.npos) path_found = true;
        if (run >= 190456/* && run <= xxxxx*/ && name.find("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v") != name.npos) path_found = true; 
        if (path_found) {
          mu_low_found = true;        
        } else {
          if (run >= 160404 && run <= 167913 && name.find("HLT_Mu17_Ele8_CaloIdL_v") != name.npos) path_found = true;
          if (run >= 170249 && run <= 180252 && name.find("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v") != name.npos) path_found = true;
          if (run >= 190456/* && run <= xxxxx*/ && name.find("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v") != name.npos) path_found = true; 
        }
      }
      /*
      In preparation for Z->ee, Z->mm:
      if (channel == EE) {
        //2011 Triggers
        if (run >= 160404 && run <= 167913 && name.find("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v") != name.npos) path_found = true;
        if (run >= 170249 && run <= 180252 && name.find("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v") != name.npos) path_found = true;
        //2012 Triggers
        if (run >= 190456 && run <= ??? && name.find("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v") != name.npos) path_found = true;
        
      }
      if (channel == MM) {
        //2011 Triggers
        if (run >= 160404 && run <= 163869 && name.find("HLT_DoubleMu7_v") != name.npos) path_found = true;
        if (run >= 165088 && run <= 178380 && name.find("HLT_Mu13_Mu8_v") != name.npos) path_found = true;
        if (run >= 178420 && run <= 180252 && name.find("HLT_Mu17_Mu8_v") != name.npos) path_found = true;
        //2012 Triggers
        if (run >= 190456 && run <= ??? && name.find("HLT_Mu17_Mu8_v") != name.npos) path_found = true;
      }
      */
    }

    if (do_obj_match_ && path_found) {
      std::string trig_obj_label;
      std::string elmu_filter;
      std::string tau_filter;

      if (channel_ == channel::et) {
        // 2011 Triggers
        if (run >= 160404 && run <= 163869) {
          trig_obj_label = "triggerObjectsEle15LooseTau15";
          elmu_filter = "hltEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter";
          tau_filter = "hltPFTau15TrackLooseIso";  
        }
        if (run >= 165088 && run <= 167913) {
          trig_obj_label = "triggerObjectsEle15LooseTau20";
          elmu_filter = "hltEle15CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter";
          tau_filter = "hltPFTau20TrackLooseIso";  
        }
        if (run >= 170249 && run <= 173198) {
          trig_obj_label = "triggerObjectsEle15TightTau20";
          elmu_filter = "hltEle15CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter";
          tau_filter = "hltPFTauTightIso20TrackTightIso";  
        }
        if (run >= 173236 && run <= 178380) {
          trig_obj_label = "triggerObjectsEle18MediumTau20";
          elmu_filter = "hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter";
          tau_filter = "hltPFTauMediumIso20TrackMediumIso";  
        }
        if (run >= 178420 && run <= 180252) {
          trig_obj_label = "triggerObjectsEle20MediumTau20";
          elmu_filter = "hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20";
          tau_filter = "hltPFTauMediumIso20TrackMediumIso";  
        }
        // 2012 Triggers
        if (run >= 190456 && run <= 193751) {
          trig_obj_label = "triggerObjectsEle20RhoLooseTau20";
          elmu_filter = "hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1IsoEG18OrEG20";
          tau_filter = "hltPFTauIsoEleVertex20";
        }
        if (run >= 193752/* && run <= xxxxx*/) {
          trig_obj_label = "triggerObjectsEle22WP90RhoLooseTau20";
          elmu_filter = "hltEle22WP90RhoTrackIsoFilter";
          tau_filter = "hltIsoElePFTau20TrackLooseIso";
        }
        std::vector<Electron *> & elmus = event->GetPtrVec<Electron>(lep1label_);
        std::vector<Tau *> & taus = event->GetPtrVec<Tau>(lep2label_);
        std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trig_obj_label);
        ic::erase_if(elmus, !boost::bind(IsFilterMatched, _1, objs, elmu_filter, 0.5));
        ic::erase_if(taus, !boost::bind(IsFilterMatched, _1, objs, tau_filter, 0.5));
      } else if (channel_ == channel::mt) {
        // 2011 Triggers
        if (run >= 160404 && run <= 163869) {
          trig_obj_label = "triggerObjectsIsoMu12LooseTau10";
          elmu_filter = "hltSingleMuIsoL3IsoFiltered12";
          tau_filter = "hltFilterIsoMu12IsoPFTau10LooseIsolation";  
        }
        if (run >= 165088 && run <= 173198) {
          trig_obj_label = "triggerObjectsIsoMu15LooseTau15";
          elmu_filter = "hltSingleMuIsoL3IsoFiltered15";
          tau_filter = "hltPFTau15TrackLooseIso";  
        }
        if (run >= 173236 && run <= 180252) {
          trig_obj_label = "triggerObjectsIsoMu15LooseTau20";
          elmu_filter = "hltSingleMuIsoL1s14L3IsoFiltered15eta2p1";
          tau_filter = "hltPFTau20TrackLooseIso";  
        }
        // 2012 Triggers
        if (run >= 190456 && run <= 193751) {
          trig_obj_label = "triggerObjectsIsoMu18LooseTau20";
          elmu_filter = "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f18QL3crIsoFiltered10";
          tau_filter = "hltPFTau20IsoMuVertex";
        }
        if (run >= 193752/* && run <= xxxxx*/) {
          trig_obj_label = "triggerObjectsIsoMu17LooseTau20";
          elmu_filter = "hltL3crIsoL1sMu14erORMu16erL1f0L2f14QL3f17QL3crIsoRhoFiltered0p15";
          tau_filter = "hltIsoMuPFTau20TrackLooseIso";
        }
        std::vector<Muon *> & elmus = event->GetPtrVec<Muon>(lep1label_);
        std::vector<Tau *> & taus = event->GetPtrVec<Tau>(lep2label_);
        std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trig_obj_label);
        ic::erase_if(elmus, !boost::bind(IsFilterMatched, _1, objs, elmu_filter, 0.5));
        ic::erase_if(taus, !boost::bind(IsFilterMatched, _1, objs, tau_filter, 0.5));
      } else if (channel_ == channel::em) {
        std::vector<Electron *> & elecs = event->GetPtrVec<Electron>(lep1label_);
        std::vector<Muon *> & muons = event->GetPtrVec<Muon>(lep2label_);

        //If mu_low_found, filter on mu_low labels
        if (mu_low_found) {
          // 2011 Triggers
          if (run >= 160404 && run <= 167913) {
            trig_obj_label = "triggerObjectsMu8Ele17IdL";
            elmu_filter = "hltL1NonIsoHLTNonIsoMu8Ele17PixelMatchFilter";
            tau_filter = "hltL1MuOpenEG5L3Filtered8";  
          }
          if (run >= 170249 && run <= 180252) {
            trig_obj_label = "triggerObjectsMu8Ele17";
            elmu_filter = "hltMu8Ele17CaloIdTCaloIsoVLPixelMatchFilter";
            tau_filter = "hltL1MuOpenEG5L3Filtered8";  
          }
          // 2012 Triggers
          if (run >= 190456 && run <= 191718) {
            trig_obj_label = "triggerObjectsMu8Ele17";
            elmu_filter = "hltMu8Ele17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
            tau_filter = "hltL1MuOpenEG12L3Filtered8";
          }
          if (run >= 191718/* && run <= xxxxx*/) {
            trig_obj_label = "triggerObjectsMu8Ele17";
            elmu_filter = "hltMu8Ele17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
            tau_filter = "hltL1sL1Mu3p5EG12ORL1MuOpenEG12L3Filtered8";
          }
        } else {
          // 2011 Triggers
          if (run >= 160404 && run <= 167913) {
            trig_obj_label = "triggerObjectsMu17Ele8IdL";
            elmu_filter = "hltL1NonIsoHLTNonIsoMu17Ele8PixelMatchFilter";
            tau_filter = "hltL1MuOpenEG5L3Filtered17";  
          }
          if (run >= 170249 && run <= 180252) {
            trig_obj_label = "triggerObjectsMu17Ele8";
            elmu_filter = "hltMu17Ele8CaloIdTPixelMatchFilter";
            tau_filter = "hltL1Mu7EG5L3MuFiltered17";  
          }
          // 2012 Triggers
          if (run >= 190456 && run <= 193751) {
            trig_obj_label = "triggerObjectsMu17Ele8";
            elmu_filter = "hltMu17Ele8CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
            tau_filter = "hltL1Mu12EG7L3MuFiltered17";
          }
          if (run >= 193752/* && run <= xxxxx*/) {
            trig_obj_label = "triggerObjectsMu17Ele8";
            elmu_filter = "hltMu17Ele8CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
            tau_filter = "hltL1Mu12EG7L3MuFiltered17";
          }


        }
        std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trig_obj_label);
        std::vector<Electron *> temp_elecs = elecs;
        std::vector<Muon *> temp_muons = muons;
        ic::erase_if(temp_elecs, !boost::bind(IsFilterMatched, _1, objs, elmu_filter, 0.5));
        ic::erase_if(temp_muons, !boost::bind(IsFilterMatched, _1, objs, tau_filter, 0.5));
        if (temp_elecs.size() == 0 && temp_muons.size() == 0) {
          if (run >= 160404 && run <= 167913) {
            trig_obj_label = "triggerObjectsMu17Ele8IdL";
            elmu_filter = "hltL1NonIsoHLTNonIsoMu17Ele8PixelMatchFilter";
            tau_filter = "hltL1MuOpenEG5L3Filtered17";  
          }
          if (run >= 170249 && run <= 180252) {
            trig_obj_label = "triggerObjectsMu17Ele8";
            elmu_filter = "hltMu17Ele8CaloIdTPixelMatchFilter";
            tau_filter = "hltL1Mu7EG5L3MuFiltered17";  
          }
          if (run >= 190456 && run <= 193751) {
            trig_obj_label = "triggerObjectsMu17Ele8";
            elmu_filter = "hltMu17Ele8CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
            tau_filter = "hltL1Mu12EG7L3MuFiltered17";
          }
          if (run >= 193752/* && run <= xxxxx*/) {
            trig_obj_label = "triggerObjectsMu17Ele8";
            elmu_filter = "hltMu17Ele8CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
            tau_filter = "hltL1Mu12EG7L3MuFiltered17";
          }
          std::vector<TriggerObject *> const& objs2 = event->GetPtrVec<TriggerObject>(trig_obj_label);
          ic::erase_if(elecs, !boost::bind(IsFilterMatched, _1, objs2, elmu_filter, 0.5));
          ic::erase_if(muons, !boost::bind(IsFilterMatched, _1, objs2, tau_filter, 0.5));
        } else {
          elecs = temp_elecs;
          muons = temp_muons;
        }

        // if (run >= 190456 && run <= 193751) {
        //   trig_obj_label = "triggerObjectsMu17Ele8";
        //   elmu_filter = "hltMu17Ele8CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
        //   tau_filter = "hltL1Mu12EG7L3MuFiltered17";
        // }
        // if (run >= 193752/* && run <= xxxxx*/) {
        //   trig_obj_label = "triggerObjectsMu17Ele8";
        //   elmu_filter = "hltMu17Ele8CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
        //   tau_filter = "hltL1Mu12EG7L3MuFiltered17";
        // }
        // objs = event->GetPtrVec<TriggerObject>(trig_obj_label);

        // if (elecs.size() == 0 && muons.size() == 0) {
        //     std::cout << elmu_filter << "\t" << CityHash64(elmu_filter) << std::endl;
        //     std::cout << tau_filter << "\t" << CityHash64(tau_filter) << std::endl;
        //     for (unsigned i = 0; i < objs.size(); ++i) {
        //       std::cout << "[" << i << "]" << std::endl;
        //       objs[i]->Print();
        //       std::vector<std::size_t> labels = objs[i]->filters();
        //       for (unsigned j = 0; j < labels.size(); ++j) {
        //         std::cout << ":" << labels[j];
        //       }
        //   }
        // }
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
      } //el-mu
    } // do obj match

    if (path_found) {
      return 0;
    } else {
      return 1;
    }
  }

  int HttDataTriggerFilter::PostAnalysis() {
    return 0;
  }

  void HttDataTriggerFilter::PrintInfo() {
    ;
  }
}
