 #include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTTriggerFilter.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "boost/bind.hpp"

namespace ic {

  HTTTriggerFilter::HTTTriggerFilter(std::string const& name) : ModuleBase(name), channel_(channel::zee), mc_(mc::summer12_53X) {
  }

  HTTTriggerFilter::~HTTTriggerFilter() {
    ;
  }

  int HTTTriggerFilter::PreAnalysis() {
    std::cout << "** PreAnalysis Info for HTTTriggerFilter **" << std::endl;
    std::cout << "Channel: " << Channel2String(channel_) << std::endl;
    std::cout << "MC: " << MC2String(mc_) << std::endl;
    std::cout << "Pair Collection: " << pair_label_ << std::endl;
    std::cout << "Is Data?: " << is_data_ << std::endl;
    return 0;
  }

  int HTTTriggerFilter::Execute(TreeEvent *event) {

    std::string trig_obj_label;
    std::string leg1_filter;
    std::string leg2_filter;
    std::string em_alt_trig_obj_label;
    std::string em_alt_leg1_filter;
    std::string em_alt_leg2_filter;

    if (is_data_) {
      EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
      unsigned run = eventInfo->run();
      bool path_found = false;
      TriggerPathPtrVec const& triggerPathPtrVec = event->GetPtrVec<TriggerPath>("triggerPaths");

      for (unsigned i = 0; i < triggerPathPtrVec.size(); ++i) {
        std::string name = triggerPathPtrVec[i]->name();

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
        }
        if (channel_ == channel::mt) {
          if (run >= 160404 && run <= 163869 && name.find("HLT_IsoMu12_LooseIsoPFTau10_v") != name.npos) path_found = true;//215.634 pb
          if (run >= 165088 && run <= 173198 && name.find("HLT_IsoMu15_LooseIsoPFTau15_v") != name.npos) path_found = true; // 1787 pb
          if (run >= 173236 && run <= 180252 && name.find("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v") != name.npos) path_found = true; //2979 pb
          //2012 Triggers
          if (run >= 190456 && run <= 193751 && name.find("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v") != name.npos) path_found = true;          
          if (run >= 193752/* && run <= xxxxx*/ && name.find("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v") != name.npos) path_found = true;       
        }
        if (channel_ == channel::em) {
          // Look for Mu Low trigger first
          if (run >= 160404 && run <= 167913 && name.find("HLT_Mu8_Ele17_CaloIdL_v") != name.npos) path_found = true;
          if (run >= 170249 && run <= 180252 && name.find("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v") != name.npos) path_found = true;
          if (run >= 190456/* && run <= xxxxx*/ && name.find("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v") != name.npos) path_found = true; 
          if (run >= 160404 && run <= 167913 && name.find("HLT_Mu17_Ele8_CaloIdL_v") != name.npos) path_found = true;
          if (run >= 170249 && run <= 180252 && name.find("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v") != name.npos) path_found = true;
          if (run >= 190456/* && run <= xxxxx*/ && name.find("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v") != name.npos) path_found = true; 
        }
        if (channel_ == channel::mtmet) {
          //2012 Triggers
          if (run >= 203768 /*&& run <= ???*/ && name.find("HLT_IsoMu8_eta2p1_LooseIsoPFTau20_L1ETM26_v") != name.npos) path_found = true;
        }
        if (channel_ == channel::etmet) {
          //2012 Triggers
          if (run >= 203768 /*&& run <= ???*/ && name.find("HLT_Ele13_eta2p1_WP90Rho_LooseIsoPFTau20_L1ETM36_v") != name.npos) path_found = true;
        }
        if (path_found) break;
      }
      if (!path_found) return 1;

      if (channel_ == channel::et) {
        // 2011 Triggers
        if (run >= 160404 && run <= 163869) {
          trig_obj_label = "triggerObjectsEle15LooseTau15";
          leg1_filter = "hltEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter";
          leg2_filter = "hltPFTau15TrackLooseIso";  
        }
        if (run >= 165088 && run <= 167913) {
          trig_obj_label = "triggerObjectsEle15LooseTau20";
          leg1_filter = "hltEle15CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter";
          leg2_filter = "hltPFTau20TrackLooseIso";  
        }
        if (run >= 170249 && run <= 173198) {
          trig_obj_label = "triggerObjectsEle15TightTau20";
          leg1_filter = "hltEle15CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter";
          leg2_filter = "hltPFTauTightIso20TrackTightIso";  
        }
        if (run >= 173236 && run <= 178380) {
          trig_obj_label = "triggerObjectsEle18MediumTau20";
          leg1_filter = "hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter";
          leg2_filter = "hltPFTauMediumIso20TrackMediumIso";  
        }
        if (run >= 178420 && run <= 180252) {
          trig_obj_label = "triggerObjectsEle20MediumTau20";
          leg1_filter = "hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20";
          leg2_filter = "hltPFTauMediumIso20TrackMediumIso";  
        }
        // 2012 Triggers
        if (run >= 190456 && run <= 193751) {
          trig_obj_label = "triggerObjectsEle20RhoLooseTau20";
          leg1_filter = "hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1IsoEG18OrEG20";
          leg2_filter = "hltPFTauIsoEleVertex20";
        }
        if (run >= 193752/* && run <= xxxxx*/) {
          trig_obj_label = "triggerObjectsEle22WP90RhoLooseTau20";
          leg1_filter = "hltEle22WP90RhoTrackIsoFilter";
          leg2_filter = "hltIsoElePFTau20TrackLooseIso";
        }
      }
      if (channel_ == channel::mt) {
        // 2011 Triggers
        if (run >= 160404 && run <= 163869) {
          trig_obj_label = "triggerObjectsIsoMu12LooseTau10";
          leg1_filter = "hltSingleMuIsoL3IsoFiltered12";
          leg2_filter = "hltFilterIsoMu12IsoPFTau10LooseIsolation";  
        }
        if (run >= 165088 && run <= 173198) {
          trig_obj_label = "triggerObjectsIsoMu15LooseTau15";
          leg1_filter = "hltSingleMuIsoL3IsoFiltered15";
          leg2_filter = "hltPFTau15TrackLooseIso";  
        }
        if (run >= 173236 && run <= 180252) {
          trig_obj_label = "triggerObjectsIsoMu15LooseTau20";
          leg1_filter = "hltSingleMuIsoL1s14L3IsoFiltered15eta2p1";
          leg2_filter = "hltPFTau20TrackLooseIso";  
        }
        // 2012 Triggers
        if (run >= 190456 && run <= 193751) {
          trig_obj_label = "triggerObjectsIsoMu18LooseTau20";
          leg1_filter = "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f18QL3crIsoFiltered10";
          leg2_filter = "hltPFTau20IsoMuVertex";
        }
        if (run >= 193752/* && run <= xxxxx*/) {
          trig_obj_label = "triggerObjectsIsoMu17LooseTau20";
          leg1_filter = "hltL3crIsoL1sMu14erORMu16erL1f0L2f14QL3f17QL3crIsoRhoFiltered0p15";
          leg2_filter = "hltIsoMuPFTau20TrackLooseIso";
        }
      }
      if (channel_ == channel::em) {
        // 2011 Triggers
        if (run >= 160404 && run <= 167913) {
          trig_obj_label = "triggerObjectsMu8Ele17IdL";
          leg1_filter = "hltL1NonIsoHLTNonIsoMu8Ele17PixelMatchFilter";
          leg2_filter = "hltL1MuOpenEG5L3Filtered8";  
        }
        if (run >= 170249 && run <= 180252) {
          trig_obj_label = "triggerObjectsMu8Ele17";
          leg1_filter = "hltMu8Ele17CaloIdTCaloIsoVLPixelMatchFilter";
          leg2_filter = "hltL1MuOpenEG5L3Filtered8";  
        }
        // 2012 Triggers
        if (run >= 190456 && run <= 191690) {
          trig_obj_label = "triggerObjectsMu8Ele17";
          leg1_filter = "hltMu8Ele17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
          leg2_filter = "hltL1MuOpenEG12L3Filtered8";
        }
        if (run >= 191691/* && run <= xxxxx*/) {
          trig_obj_label = "triggerObjectsMu8Ele17";
          leg1_filter = "hltMu8Ele17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
          leg2_filter = "hltL1sL1Mu3p5EG12ORL1MuOpenEG12L3Filtered8";
        }
        // 2011 Triggers
        if (run >= 160404 && run <= 167913) {
          em_alt_trig_obj_label = "triggerObjectsMu17Ele8IdL";
          em_alt_leg1_filter = "hltL1NonIsoHLTNonIsoMu17Ele8PixelMatchFilter";
          em_alt_leg2_filter = "hltL1MuOpenEG5L3Filtered17";  
        }
        if (run >= 170249 && run <= 180252) {
          em_alt_trig_obj_label = "triggerObjectsMu17Ele8";
          em_alt_leg1_filter = "hltMu17Ele8CaloIdTPixelMatchFilter";
          em_alt_leg2_filter = "hltL1Mu7EG5L3MuFiltered17";  
        }
        // 2012 Triggers
        if (run >= 190456 && run <= 193751) {
          em_alt_trig_obj_label = "triggerObjectsMu17Ele8";
          em_alt_leg1_filter = "hltMu17Ele8CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
          em_alt_leg2_filter = "hltL1Mu12EG7L3MuFiltered17";
        }
        if (run >= 193752/* && run <= xxxxx*/) {
          em_alt_trig_obj_label = "triggerObjectsMu17Ele8";
          em_alt_leg1_filter = "hltMu17Ele8CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
          em_alt_leg2_filter = "hltL1Mu12EG7L3MuFiltered17";
        }
      }
      if (channel_ == channel::mtmet) {
        if (run >= 203777/* && run <= xxxxx*/) {
          trig_obj_label = "triggerObjectsIsoMu8LooseTau20L1ETM26";
          leg1_filter = "hltL3crIsoL1sMu7Eta2p1L1f0L2f7QL3f8QL3crIsoRhoFiltered0p15";
          leg2_filter = "hltIsoMu8PFTau20TrackLooseIso";
        }
      }
      if (channel_ == channel::etmet) {
        if (run >= 203777/* && run <= xxxxx*/) {
          trig_obj_label = "triggerObjectsEle13LooseTau20L1ETM36";
          leg1_filter = "hltEle13WP90RhoTrackIsoFilter";
          leg2_filter = "hltIsoEle13PFTau20TrackLooseIso";
        }
      }
    } else {
      if (channel_ == channel::et) {
        if (mc_ == mc::fall11_42X) {
          trig_obj_label   = "triggerObjectsEle18MediumTau20";
          leg1_filter      = "hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter";
          leg2_filter       = "hltPFTauMediumIso20TrackMediumIso";
        } else if (mc_ == mc::summer12_53X) {
          trig_obj_label   = "triggerObjectsEle22WP90RhoLooseTau20";
          leg1_filter      = "hltEle22WP90RhoTrackIsoFilter";
          leg2_filter       = "hltIsoElePFTau20TrackLooseIso";
        }
      } else if (channel_ == channel::mt) {
        if (mc_ == mc::fall11_42X) {
          trig_obj_label   = "triggerObjectsIsoMu15LooseTau15";
          leg1_filter      = "hltSingleMuIsoL3IsoFiltered15";
          leg2_filter       = "hltPFTau15TrackLooseIso";
        } else if (mc_ == mc::summer12_53X) {
          trig_obj_label   = "triggerObjectsIsoMu17LooseTau20";
          leg1_filter      = "hltL3crIsoL1sMu14erORMu16erL1f0L2f14QL3f17QL3crIsoRhoFiltered0p15";
          leg2_filter       = "hltIsoMuPFTau20TrackLooseIso";
        }
      } else if (channel_ == channel::em) {
        if (mc_ == mc::fall11_42X) {
          trig_obj_label            = "triggerObjectsMu8Ele17";
          leg1_filter               = "hltMu8Ele17CaloIdTCaloIsoVLPixelMatchFilter";
          leg2_filter               = "hltL1MuOpenEG12L3Filtered8";
          em_alt_trig_obj_label     = "triggerObjectsMu17Ele8";
          em_alt_leg1_filter        = "hltMu17Ele8CaloIdTPixelMatchFilter";
          em_alt_leg2_filter        = "hltL1Mu12EG5L3MuFiltered17";
        } else if (mc_ == mc::summer12_53X) {
          trig_obj_label            = "triggerObjectsMu8Ele17";
          leg1_filter               = "hltMu8Ele17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
          leg2_filter               = "hltL1sL1Mu3p5EG12ORL1MuOpenEG12L3Filtered8";
          em_alt_trig_obj_label     = "triggerObjectsMu17Ele8";
          em_alt_leg1_filter        = "hltMu17Ele8CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
          em_alt_leg2_filter        = "hltL1Mu12EG7L3MuFiltered17";
        }
      }
      /*
      [2] HLT triggers in MC to "emulate" lepton legs of new triggers in data:
      - HLT_Mu8_v16, mu filter to match: hltL3fL1sMu3L3Filtered8 apply cut
      |eta|<2.1, in addition match to L1Extra Mu, pT>7, |eta|<2.1 to have
      correct L1Mu seed
      - HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v14, ele filter to
      match: hltEle8TightIdLooseIsoTrackIsoFilter, apply cut pt>13 and
      |eta|<2.1, in addition match to L1Extra IsolatedEG, pT>12, |eta|<2.17
      to have correct L1EG seed

      [3]. Reconstruct fixed cone PFTau with settings used at HLT with
      offline PFlow and vertices. I can provide you python configuration. It
      can be reconstructed on any stage of analysis where PFlow (and PFjets)
      and vertices are accessible.
      */
      if (channel_ == channel::mtmet) {
        if (mc_ == mc::summer12_53X) {
          trig_obj_label   = "triggerObjectsMu8"; // triggerObjectsMu8, hltTaus
          leg1_filter      = "hltL3fL1sMu3L3Filtered8";
          leg2_filter       = "";
        }
      }
      if (channel_ == channel::etmet) {
        if (mc_ == mc::summer12_53X) {
          trig_obj_label   = "triggerObjectsEle8"; // triggerObjectsEle8, hltTaus
          leg1_filter      = "hltEle8TightIdLooseIsoTrackIsoFilter";
          leg2_filter       = "";
        }
      }
    }


    std::vector<CompositeCandidate *> & dileptons = event->GetPtrVec<CompositeCandidate>(pair_label_);
    std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trig_obj_label);

    std::vector<CompositeCandidate *> dileptons_pass;

    if (channel_ == channel::et || channel_ == channel::mt) {
      for (unsigned i = 0; i < dileptons.size(); ++i) {
        bool leg1_match = IsFilterMatched(dileptons[i]->At(0), objs, leg1_filter, 0.5);
        bool leg2_match = IsFilterMatched(dileptons[i]->At(1), objs, leg2_filter, 0.5);
        if (leg1_match && leg2_match) dileptons_pass.push_back(dileptons[i]);
      }
    }

    if (channel_ == channel::em) {
      std::vector<TriggerObject *> const& em_alt_objs = event->GetPtrVec<TriggerObject>(em_alt_trig_obj_label);
      for (unsigned i = 0; i < dileptons.size(); ++i) {
        bool leg1_match = IsFilterMatched(dileptons[i]->At(0), objs, leg1_filter, 0.5);
        bool leg2_match = IsFilterMatched(dileptons[i]->At(1), objs, leg2_filter, 0.5);
        bool highpt_leg = dileptons[i]->At(0)->pt() > 20.0; // electron leg pT > 20 GeV
        if (leg1_match && leg2_match && highpt_leg) {
          dileptons_pass.push_back(dileptons[i]);
        } else {
          leg1_match = IsFilterMatched(dileptons[i]->At(0), em_alt_objs, em_alt_leg1_filter, 0.5);
          leg2_match = IsFilterMatched(dileptons[i]->At(1), em_alt_objs, em_alt_leg2_filter, 0.5);
          highpt_leg = dileptons[i]->At(1)->pt() > 20.0; // muon leg pT > 20 GeV
          if (leg1_match && leg2_match && highpt_leg) dileptons_pass.push_back(dileptons[i]);
        }
      }
    }

    if ( (channel_ == channel::mtmet || channel_ == channel::etmet) && is_data_) {
      for (unsigned i = 0; i < dileptons.size(); ++i) {
        bool leg1_match = IsFilterMatched(dileptons[i]->At(0), objs, leg1_filter, 0.5);
        bool leg2_match = IsFilterMatched(dileptons[i]->At(1), objs, leg2_filter, 0.5);
        if (leg1_match && leg2_match) dileptons_pass.push_back(dileptons[i]);
      }
    }

    if (channel_ == channel::mtmet && !is_data_) {
      std::vector<TriggerObject *> mu8_obj = objs; // Make a copy of the Mu8 Trigger objects so we can filter
      ic::erase_if(mu8_obj, !boost::bind(MinPtMaxEta, _1, 8.0, 2.1));
      std::vector<Candidate *> l1muon = event->GetPtrVec<Candidate>("l1extraMuons");
      ic::erase_if(l1muon, !boost::bind(MinPtMaxEta, _1, 7.0, 2.1));
      std::vector<Tau *> hlt_taus = event->GetPtrVec<Tau>("hltTaus");
      ic::erase_if(hlt_taus, !boost::bind(MinPtMaxEta, _1, 20.0, 999.0));
      ic::erase_if(hlt_taus, !(boost::bind(&Tau::GetTauID, _1, "decayModeFinding") > 0.5) );
      ic::erase_if(hlt_taus, !(boost::bind(&Tau::GetTauID, _1, "byIsolation") > 0.5) );
      std::vector<Candidate *> const& l1met = event->GetPtrVec<Candidate>("l1extraMET");
      for (unsigned i = 0; i < dileptons.size(); ++i) {
        bool leg1_hlt_match = IsFilterMatched(dileptons[i]->At(0), mu8_obj, leg1_filter, 0.5);
        std::vector<Candidate *> mu_as_vector;
        mu_as_vector.push_back(dileptons[i]->At(0));
        bool leg1_l1_match = MatchByDR(mu_as_vector, l1muon, 0.5, false, false).size() > 0;
        bool leg1_match = leg1_hlt_match && leg1_l1_match;
        std::vector<Candidate *> tau_as_vector;
        tau_as_vector.push_back(dileptons[i]->At(1));
        bool leg2_match = MatchByDR(tau_as_vector, hlt_taus, 0.5, false, false).size() > 0;
        bool l1_met = l1met.at(0)->pt() > 26.;
        if (leg1_match && leg2_match && l1_met) dileptons_pass.push_back(dileptons[i]);
      }
    }

    if (channel_ == channel::etmet && !is_data_) {
      std::vector<TriggerObject *> ele8_obj = objs; // Make a copy of the Mu8 Trigger objects so we can filter
      ic::erase_if(ele8_obj, !boost::bind(MinPtMaxEta, _1, 13.0, 2.1));
      std::vector<Candidate *> l1emiso = event->GetPtrVec<Candidate>("l1extraEmIsolated");
      ic::erase_if(l1emiso, !boost::bind(MinPtMaxEta, _1, 12.0, 2.17));
      std::vector<Tau *> hlt_taus = event->GetPtrVec<Tau>("hltTaus");
      ic::erase_if(hlt_taus, !boost::bind(MinPtMaxEta, _1, 20.0, 999.0));
      ic::erase_if(hlt_taus, !(boost::bind(&Tau::GetTauID, _1, "decayModeFinding") > 0.5) );
      ic::erase_if(hlt_taus, !(boost::bind(&Tau::GetTauID, _1, "byIsolation") > 0.5) );
      std::vector<Candidate *> const& l1met = event->GetPtrVec<Candidate>("l1extraMET");
      for (unsigned i = 0; i < dileptons.size(); ++i) {
        bool leg1_hlt_match = IsFilterMatched(dileptons[i]->At(0), ele8_obj, leg1_filter, 0.5);
        std::vector<Candidate *> ele_as_vector;
        ele_as_vector.push_back(dileptons[i]->At(0));
        bool leg1_l1_match = MatchByDR(ele_as_vector, l1emiso, 0.5, false, false).size() > 0;
        bool leg1_match = leg1_hlt_match && leg1_l1_match;
        std::vector<Candidate *> tau_as_vector;
        tau_as_vector.push_back(dileptons[i]->At(1));
        bool leg2_match = MatchByDR(tau_as_vector, hlt_taus, 0.5, false, false).size() > 0;
        bool l1_met = l1met.at(0)->pt() > 36.;
        if (leg1_match && leg2_match && l1_met) dileptons_pass.push_back(dileptons[i]);
      }
    }

    dileptons = dileptons_pass;
    if (dileptons.size() >= 1) {
      return 0;
    } else {
      return 1;
    }
}

  int HTTTriggerFilter::PostAnalysis() {
    return 0;
  }

  void HTTTriggerFilter::PrintInfo() {
    ;
  }
}
