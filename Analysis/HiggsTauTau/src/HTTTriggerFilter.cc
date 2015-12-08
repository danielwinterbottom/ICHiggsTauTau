#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTTriggerFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
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
#include "boost/format.hpp"

namespace ic {

  HTTTriggerFilter::HTTTriggerFilter(std::string const& name) : ModuleBase(name), channel_(channel::zee), mc_(mc::summer12_53X), era_(era::data_2015) {
  }

  HTTTriggerFilter::~HTTTriggerFilter() {
    ;
  }

  int HTTTriggerFilter::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTTriggerFilter" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
    std::cout << boost::format(param_fmt()) % "channel"         % Channel2String(channel_);
    std::cout << boost::format(param_fmt()) % "mc"              % MC2String(mc_);
    std::cout << boost::format(param_fmt()) % "dilepton_label"  % pair_label_;
    std::cout << boost::format(param_fmt()) % "is_data"         % is_data_;
    std::cout << boost::format(param_fmt()) % "is_embedded"     % is_embedded_;
    std::cout << boost::format(param_fmt()) % "do_leptonplustau" % do_leptonplustau_;
    std::cout << boost::format(param_fmt()) % "do_singlelepton" % do_singlelepton_;
    return 0;
  }

  int HTTTriggerFilter::Execute(TreeEvent *event) {

    std::string trig_obj_label;
		std::string alt_trig_obj_label;
    std::string leg1_filter;
    std::string leg2_filter;
		std::string extra_leg2_filter;
		std::string alt_leg1_filter;
/*    std::string alt_trig_obj_label_2;
    std::string alt_leg1_filter_2;*/
		std::string alt_leg2_filter;
    std::string em_alt_trig_obj_label;
    std::string em_alt_leg1_filter;
    std::string em_alt_leg2_filter;
    double alt_min_online_pt=0;
    double high_leg_pt = 0;

    if (is_data_) { //Switch this part off temporarily as we don't have this vector in first processed data
      EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");

      unsigned run = eventInfo->run();
      bool path_found     = false;
      bool fallback_found = false;
      auto const& triggerPathPtrVec = event->GetPtrVec<TriggerPath>("triggerPaths");

      for (unsigned i = 0; i < triggerPathPtrVec.size(); ++i) {
        std::string name = triggerPathPtrVec[i]->name();

        if (channel_ == channel::et||channel_ == channel::zee || channel_ == channel::tpzee) {
          // 2011 Triggers
          if (run >= 160404 && run <= 163869 && name.find("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v") != name.npos) path_found = true;
          if (run >= 165088 && run <= 167913 && name.find("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v") != name.npos) path_found = true;
          if (run >= 170249 && run <= 173198 && name.find("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v") != name.npos) path_found = true;
          if (run >= 173236 && run <= 178380 && name.find("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v") != name.npos) path_found = true;
          if (run >= 173236 && run <= 178380 && name.find("HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v") != name.npos) {
            path_found      = true;
            fallback_found  = true; 
          }
          if (run >= 178420 && run <= 180252 && name.find("HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v") != name.npos) path_found = true;
          // 2012 Triggers
          if (run >= 190456 && run <= 193751 && name.find("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v") != name.npos) path_found = true; 
          //if (run >= 193752/* //&& run <= xxxxx*/ //&& name.find("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v") != name.npos) path_found = true; 
       if (run >= 193752 && run <= 247600 && name.find("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v") != name.npos) path_found = true; 
          //2015 triggers
       if (run >= 250985 /*&& run <= 258654*/  && (name.find("HLT_Ele23_WPLoose_Gsf_v") != name.npos) ) path_found = true;
       //if (run >= 258655/* &&run <=xxxxx*/ && (name.find("HLT_Ele22_eta2p1_WPLoose_Gsf_v") != name.npos) ) path_found = true;
       //if (run >= 250985/* && run <= xxxxx*/ && (name.find("HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_v") != name.npos || name.find("HLT_Ele32_eta2p1_WPTight_Gsf_v") != name.npos) ) path_found = true;
        }
        if (channel_ == channel::mt || channel_ == channel::zmm || channel_ == channel::tpzmm) {
          if (run >= 160404 && run <= 163869 && name.find("HLT_IsoMu12_LooseIsoPFTau10_v") != name.npos) path_found = true;//215.634 pb
          if (run >= 165088 && run <= 173198 && name.find("HLT_IsoMu15_LooseIsoPFTau15_v") != name.npos) path_found = true; // 1787 pb
          if (run >= 165088 && run <= 180252 && name.find("HLT_IsoMu15_LooseIsoPFTau15_v") != name.npos) {
            path_found      = true; // 1787 pb
            fallback_found  = true; // 1787 pb
          }
          if (run >= 173236 && run <= 180252 && name.find("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v") != name.npos) path_found = true; //2979 pb
          //2012 Triggers
          if (run >= 190456 && run <= 193751 && name.find("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v") != name.npos) path_found = true;          
          if (run >= 193752 && run <= 247600 && name.find("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v") != name.npos) path_found = true;       
          //2015 Triggers
          if (run >= 250985 && run <= 256464 && (name.find("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v") != name.npos || name.find("HLT_IsoMu24_eta2p1_v") != name.npos)) path_found = true;
          if (run >= 256630 /* && run <= xxxxx*/ && (name.find("HLT_IsoMu18_v") != name.npos)) path_found = true;
          //if (run >= 256630 /* && run <= xxxxx*/ && (name.find("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v") != name.npos || name.find("HLT_IsoMu22_v") != name.npos)) path_found = true;
        }
        if (channel_ == channel::em) {
          // Look for Mu Low trigger first
          if (run >= 160404 && run <= 167913 && name.find("HLT_Mu8_Ele17_CaloIdL_v") != name.npos) path_found = true;
          if (run >= 170249 && run <= 180252 && name.find("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v") != name.npos) path_found = true;
          if (run >= 190456 && run <= 247600 && name.find("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v") != name.npos) path_found = true; 
          if (run >= 160404 && run <= 173198 && name.find("HLT_Mu17_Ele8_CaloIdL_v") != name.npos) path_found = true;
          if (run >= 173199 && run <= 180252 && name.find("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v") != name.npos) path_found = true;
          if (run >= 190456 && run <= 247600 && name.find("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v") != name.npos) path_found = true; 
          //if (run >= 250985 /*&& run <= xxxxx*/ && (name.find("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") != name.npos || name.find("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v") != name.npos)) path_found = true;
          if (run >= 250985 /*&& run <= xxxxx*/ && (name.find("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") != name.npos || name.find("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v") != name.npos)) path_found = true;
        }
        if (channel_ == channel::tt){
          if (run >= 250985 /*&& run <= xxxxx*/ && (name.find("HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg_v") != name.npos)) path_found=true;
          //if (run >= 250985 /*&& run <= xxxxx*/ && (name.find("HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v") != name.npos)) path_found=true;
        }
        if (channel_ == channel::mtmet) {
          //2012 Triggers
          if (run >= 203768 /*&& run <= ???*/ && name.find("HLT_IsoMu8_eta2p1_LooseIsoPFTau20_L1ETM26_v") != name.npos) path_found = true;
        }
        if (channel_ == channel::etmet) {
          //2012 Triggers
          if (run >= 203768 /*&& run <= ???*/ && name.find("HLT_Ele13_eta2p1_WP90Rho_LooseIsoPFTau20_L1ETM36_v") != name.npos) path_found = true;
        }
        if (is_embedded_) {
          path_found = false;
          if (run >= 190456 /*&& run <= ???*/ && name.find("HLT_Mu17_Mu8_v") != name.npos) {
            path_found = true;
            break;
          }
        }
         if (path_found) break;
      }
      if (!path_found) return 1;

      if (channel_ == channel::et || channel_ == channel::zee || channel_ == channel :: tpzee) {
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
        if (run >= 173236 && run <= 178380 && fallback_found == true) {
          trig_obj_label = "triggerObjectsEle20MediumTau20";
          leg1_filter = "hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20";
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
        if (run >= 193752 && run <= 247600) {
         trig_obj_label = "triggerObjectsEle22WP90RhoLooseTau20";
          leg1_filter = "hltEle22WP90RhoTrackIsoFilter";
          leg2_filter = "hltIsoElePFTau20TrackLooseIso";
        }
        if (run >= 250985 && run <= 253620){

          trig_obj_label = "triggerObjectsEle22LooseTau20";
          leg1_filter = "hltSingleEle22WPLooseGsfTrackIsoFilter";
          leg2_filter = "hltPFTau20TrackLooseIso";
          extra_leg2_filter = "hltOverlapFilterIsoEle22WPLooseGsfLooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsEle32Gsf";
          alt_leg1_filter = "hltEle32WPTightGsfTrackIsoFilter";
        }
//        if (run >= 253621 /*&& run <= xxxxxx*/){
 /*         trig_obj_label = "triggerObjectsEle22LooseTau20";
          leg1_filter = "hltEle22WPLooseL1IsoEG20erTau20erGsfTrackIsoFilter";
          leg2_filter = "hltPFTau20TrackLooseIso";
          extra_leg2_filter = "hltOverlapFilterIsoEle22WPLooseGsfLooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsEle32Gsf";
          alt_leg1_filter = "hltEle32WPTightGsfTrackIsoFilter";
          high_leg_pt = 33.;
        }*/
        if (run >= 253621 /*&& run <= xxxxxxxxx*/){
          trig_obj_label = "triggerObjectsEle22LooseTau20";
          leg1_filter = "hltEle22WPLooseL1IsoEG20erTau20erGsfTrackIsoFilter";
          leg2_filter = "hltPFTau20TrackLooseIso";
          extra_leg2_filter = "hltOverlapFilterIsoEle22WPLooseGsfLooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsEle23";
          alt_leg1_filter = "hltEle23WPLooseGsfTrackIsoFilter";
          high_leg_pt = 24.;
        }
       // if (run >= 258655 /*&& run <= xxxxxxxx*/){
        /*  trig_obj_label = "triggerObjectsEle22LooseTau20";
          leg1_filter = "hltEle22WPLooseL1IsoEG20erTau20erGsfTrackIsoFilter";
          leg2_filter = "hltPFTau20TrackLooseIso";
          extra_leg2_filter = "hltOverlapFilterIsoEle22WPLooseGsfLooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsEle22";
          alt_leg1_filter = "hltSingleEle22WPLooseGsfTrackIsoFilter";
          high_leg_pt = 23.;
        }*/


      }
      if (channel_ == channel::mt || channel_ == channel::zmm || channel_ == channel::tpzmm) {
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
        if (run >= 165088 && run <= 180252 && fallback_found == true) {
          trig_obj_label = "triggerObjectsIsoMu15LooseTau15";
          leg1_filter = "hltSingleMuIsoL3IsoFiltered15";
          leg2_filter = "hltPFTau15TrackLooseIso";  
        }
        // 2012 Triggers
        if (run >= 190456 && run <= 193751) {
          trig_obj_label = "triggerObjectsIsoMu18LooseTau20";
          leg1_filter = "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f18QL3crIsoFiltered10";
          leg2_filter = "hltPFTau20IsoMuVertex";
        }
        if (run >= 193752 && run <= 247606) {
          trig_obj_label = "triggerObjectsIsoMu17LooseTau20";
          leg1_filter = "hltL3crIsoL1sMu14erORMu16erL1f0L2f14QL3f17QL3crIsoRhoFiltered0p15";
          leg2_filter = "hltIsoMuPFTau20TrackLooseIso";
        }
        if (run >= 250985 && run <= 256465) {
          trig_obj_label = "triggerObjectsIsoMu17LooseTau20";
          leg1_filter = "hltL3crIsoL1sMu16erTauJet20erL1f0L2f10QL3f17QL3trkIsoFiltered0p09";
          leg2_filter = "hltPFTau20TrackLooseIsoAgainstMuon";
          extra_leg2_filter = "hltOverlapFilterIsoMu17LooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsIsoMu24";
          alt_leg1_filter = "hltL3crIsoL1sMu20Eta2p1L1f0L2f10QL3f24QL3trkIsoFiltered0p09";
        }
//        if (run >= 256466 /*&& run <= xxxxx*/) {
 /*         trig_obj_label = "triggerObjectsIsoMu17LooseTau20";
          leg1_filter = "hltL3crIsoL1sMu16erTauJet20erL1f0L2f10QL3f17QL3trkIsoFiltered0p09";
          leg2_filter = "hltPFTau20TrackLooseIsoAgainstMuon";
          extra_leg2_filter = "hltOverlapFilterIsoMu17LooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsIsoMu22";
          alt_leg1_filter =  "hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09"; 
          high_leg_pt = 25.;
        }*/
        if (run >= 256466 /*&& run <= xxxxx*/) {
          trig_obj_label = "triggerObjectsIsoMu17LooseTau20";
          leg1_filter = "hltL3crIsoL1sMu16erTauJet20erL1f0L2f10QL3f17QL3trkIsoFiltered0p09";
          leg2_filter = "hltPFTau20TrackLooseIsoAgainstMuon";
          extra_leg2_filter = "hltOverlapFilterIsoMu17LooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsIsoMu18";
          alt_leg1_filter =  "hltL3crIsoL1sMu16L1f0L2f10QL3f18QL3trkIsoFiltered0p09"; 
          high_leg_pt = 19.;
        }

      }
      if (channel_ == channel::em) {
        // 2011 Triggers
        if (run >= 160404 && run <= 163261) {
          trig_obj_label = "triggerObjectsMu8Ele17IdL";
          leg1_filter = "hltL1NonIsoHLTNonIsoMu8Ele17PixelMatchFilter";
          leg2_filter = "hltL1Mu3EG5L3Filtered8";
        }
        if (run >= 163262 && run <= 167913) {
          trig_obj_label = "triggerObjectsMu8Ele17IdL";
          leg1_filter = "hltL1NonIsoHLTNonIsoMu8Ele17PixelMatchFilter";
          leg2_filter = "hltL1MuOpenEG5L3Filtered8";
        }
        if (run >= 170249 && run <= 180252) {
          trig_obj_label = "triggerObjectsMu8Ele17";
          leg1_filter = "hltMu8Ele17CaloIdTCaloIsoVLPixelMatchFilter";
          leg2_filter = "hltL1MuOpenEG12L3Filtered8";  
        }
        // 2012 Triggers
        if (run >= 190456 && run <= 191690) {
          trig_obj_label = "triggerObjectsMu8Ele17";
          leg1_filter = "hltMu8Ele17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
          leg2_filter = "hltL1MuOpenEG12L3Filtered8";
        }
        if (run >= 191691 && run <= 247606) {
          trig_obj_label = "triggerObjectsMu8Ele17";
          leg1_filter = "hltMu8Ele17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
          leg2_filter = "hltL1sL1Mu3p5EG12ORL1MuOpenEG12L3Filtered8";
        }
        // 2011 Triggers
        if (run >= 160404 && run <= 163261) {
          em_alt_trig_obj_label = "triggerObjectsMu17Ele8IdL";
          em_alt_leg1_filter = "hltL1NonIsoHLTNonIsoMu17Ele8PixelMatchFilter";
          em_alt_leg2_filter = "hltL1Mu3EG5L3Filtered17";  // V1,V2
        }
        if (run >= 163262 && run <= 167913) {
          em_alt_trig_obj_label = "triggerObjectsMu17Ele8IdL";
          em_alt_leg1_filter = "hltL1NonIsoHLTNonIsoMu17Ele8PixelMatchFilter";
          em_alt_leg2_filter = "hltL1MuOpenEG5L3Filtered17";
        }
        if (run >= 170249 && run <= 173198) {
          em_alt_trig_obj_label = "triggerObjectsMu17Ele8IdL";
          em_alt_leg1_filter = "hltL1NonIsoHLTNonIsoMu17Ele8PixelMatchFilter";
          em_alt_leg2_filter = "hltL1Mu7EG5L3MuFiltered17";
        }
        if (run >= 173236 && run <= 180252) {
          em_alt_trig_obj_label = "triggerObjectsMu17Ele8";
          em_alt_leg1_filter = "hltMu17Ele8CaloIdTPixelMatchFilter";
          em_alt_leg2_filter = "hltL1Mu12EG5L3MuFiltered17";  
        }
        // 2012 Triggers
        if (run >= 190456 && run <= 193751) {
          em_alt_trig_obj_label = "triggerObjectsMu17Ele8";
          em_alt_leg1_filter = "hltMu17Ele8CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
          em_alt_leg2_filter = "hltL1Mu12EG7L3MuFiltered17";
        }
        if (run >= 193752 && run <= 247606) {
          em_alt_trig_obj_label = "triggerObjectsMu17Ele8";
          em_alt_leg1_filter = "hltMu17Ele8CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter";
          em_alt_leg2_filter = "hltL1Mu12EG7L3MuFiltered17";
        }
        // 2015 Triggers
        if (run >= 250985 /*&& run <= xxxxxx*/){
        /* trig_obj_label = "triggerObjectsEle12Mu23";
         leg1_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         leg2_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23";
         alt_trig_obj_label = "triggerObjectsEle23Mu8";
         alt_leg1_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         alt_leg2_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8";*/

//        if (run >= 250985 /*&& run <= xxxxxx*/){
         trig_obj_label = "triggerObjectsEle12Mu17";
         leg1_filter = "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         leg2_filter = "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered17";
         alt_trig_obj_label = "triggerObjectsEle17Mu8";
         alt_leg1_filter = "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         alt_leg2_filter = "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8";


        }

      }
      if(channel_ == channel::tt){
        if(run >= 250985 /*&& run <= xxxxxx*/){
          trig_obj_label = "triggerObjectsDiTau35";
          leg1_filter = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
          leg2_filter = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
        }
//        if(run >= 250985 /*&& run <= xxxxxx*/){
 /*         trig_obj_label = "triggerObjectsDoubleMediumTau40";
          leg1_filter = "hltDoublePFTau40TrackPt1MediumIsolationDz02Reg";
          leg2_filter = "hltDoublePFTau40TrackPt1MediumIsolationDz02Reg";
        }*/

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
    /*} else if(is_data_ && era_==era::data_2015){
     if(channel_ == channel::tt){
       trig_obj_label = "triggerObjectsDoubleMediumTau40";
       leg1_filter = "hltDoublePFTau40TrackPt1MediumIsolationDz02Reg";
       leg2_filter = "hltDoublePFTau40TrackPt1MediumIsolationDz02Reg";
     } 
     if(channel_ == channel::em){
         trig_obj_label = "triggerObjectsEle12Mu23";
         leg1_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         leg2_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23";
         alt_trig_obj_label = "triggerObjectsEle23Mu8";
         alt_leg1_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         alt_leg2_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8";
      }
     if(channel_ == channel::mt || channel_ == channel::zmm){
        trig_obj_label = "triggerObjectsIsoMu17LooseTau20";
        leg1_filter = "hltL3crIsoL1sMu16erTauJet20erL1f0L2f10QL3f17QL3trkIsoFiltered0p09";
        leg2_filter = "hltPFTau20TrackLooseIsoAgainstMuon";
        extra_leg2_filter = "hltOverlapFilterIsoMu17LooseIsoPFTau20";
        alt_trig_obj_label = "triggerObjectsIsoMu24";
        alt_leg1_filter = "hltL3crIsoL1sMu20Eta2p1L1f0L2f10QL3f24QL3trkIsoFiltered0p09";
        high_leg_pt = 25.;
      }
    if(channel_ == channel::et || channel_ == channel::zee){
        trig_obj_label = "triggerObjectsEle22LooseTau20";
        leg1_filter = "hltSingleEle22WPLooseGsfTrackIsoFilter";
        //leg1_filter = "hltSingleEle22WPLooseGsfTrackIsoFilter";
        leg2_filter = "hltPFTau20TrackLooseIso";
        extra_leg2_filter = "hltOverlapFilterIsoEle22WPLooseGsfLooseIsoPFTau20";
        alt_trig_obj_label = "triggerObjectsEle32Gsf";
        alt_leg1_filter = "hltEle32WPTightGsfTrackIsoFilter";
        high_leg_pt = 33.;
     }*/
    } else {
      if (channel_ == channel::et || channel_ == channel::zee || channel_ ==  channel::tpzee) {
        if (mc_ == mc::fall11_42X) {
          trig_obj_label   = "triggerObjectsEle18MediumTau20";
          leg1_filter      = "hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter";
          leg2_filter       = "hltPFTauMediumIso20TrackMediumIso";
        } else if (mc_ == mc::summer12_53X) {
          trig_obj_label   = "triggerObjectsEle22WP90RhoLooseTau20";
          leg1_filter      = "hltEle22WP90RhoTrackIsoFilter";
          leg2_filter       = "hltIsoElePFTau20TrackLooseIso";
        }  else if (mc_ == mc::phys14_72X){
          trig_obj_label   = "triggerObjectsEle22LooseTau20";
          leg1_filter      = "hltOverlapFilterIsoEle22WP85GsfLooseIsoPFTau20";
          leg2_filter      = "hltL1sL1IsoEG20erTauJet20er";
          extra_leg2_filter = "hltOverlapFilterIsoEle22WP85GsfLooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsEle27Gsf";
          alt_leg1_filter  = "hltEle27WP85GsfTrackIsoFilter";
          alt_leg2_filter  = "";
        }  else if (mc_ == mc::spring15_74X){
          trig_obj_label   = "triggerObjectsEle22LooseTau20";
          leg1_filter      = "hltEle22WP75L1IsoEG20erTau20erGsfTrackIsoFilter";
          leg2_filter      = "hltPFTau20TrackLooseIso";
          extra_leg2_filter = "hltOverlapFilterIsoEle22WP75GsfLooseIsoPFTau20";
/*          alt_trig_obj_label = "triggerObjectsEle32Gsf";
          alt_leg1_filter  = "hltEle32WP75GsfTrackIsoFilter";
          alt_leg2_filter  = "";
          high_leg_pt = 33.;*/
          alt_trig_obj_label = "triggerObjectsEle22Gsf";
          alt_leg1_filter  = "hltSingleEle22WP75GsfTrackIsoFilter";
          alt_leg2_filter  = "";
          alt_min_online_pt = 23.;
          high_leg_pt = 23.;

					}

      } else if (channel_ == channel::mt || channel_ == channel::zmm || channel_ == channel::tpzmm) {
        if (mc_ == mc::fall11_42X) {
          trig_obj_label   = "triggerObjectsIsoMu15LooseTau15";
          leg1_filter      = "hltSingleMuIsoL3IsoFiltered15";
          leg2_filter       = "hltPFTau15TrackLooseIso";
        } else if (mc_ == mc::summer12_53X) {
          trig_obj_label   = "triggerObjectsIsoMu17LooseTau20";
          leg1_filter      = "hltL3crIsoL1sMu14erORMu16erL1f0L2f14QL3f17QL3crIsoRhoFiltered0p15";
          leg2_filter       = "hltIsoMuPFTau20TrackLooseIso";
        }  else if (mc_ == mc::phys14_72X) {
          trig_obj_label  = "triggerObjectsIsoMu17LooseTau20";
          leg1_filter     = "hltOverlapFilterIsoMu17LooseIsoPFTau20";
          leg2_filter     = "hltL1sMu16erTauJet20er";
          extra_leg2_filter = "hltOverlapFilterIsoMu17LooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsIsoMu24IterTrk";
          alt_leg1_filter = "hltL3crIsoL1sMu20Eta2p1L1f0L2f20QL3f24QL3crIsoRhoFiltered0p15IterTrk02";
          alt_leg2_filter = "";
        } else if (mc_ == mc::spring15_74X) {
           trig_obj_label  = "triggerObjectsIsoMu17LooseTau20";
          leg1_filter     = "hltL3crIsoL1sMu16erTauJet20erL1f0L2f10QL3f17QL3trkIsoFiltered0p09";
          leg2_filter     = "hltPFTau20TrackLooseIsoAgainstMuon";
          extra_leg2_filter = "hltOverlapFilterIsoMu17LooseIsoPFTau20";
         // alt_trig_obj_label = "triggerObjectsIsoMu24";
         // alt_leg1_filter = "hltL3crIsoL1sMu20Eta2p1L1f0L2f10QL3f24QL3trkIsoFiltered0p09";
          alt_trig_obj_label = "triggerObjectsIsoMu17";
          alt_leg1_filter = "hltL3crIsoL1sSingleMu16erL1f0L2f10QL3f17QL3trkIsoFiltered0p09";
          alt_leg2_filter = "";
          alt_min_online_pt = 18.;
          high_leg_pt = 18.;
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
        } else if (mc_ == mc::phys14_72X){
          trig_obj_label            = "triggerObjectsEle12Mu23";
          leg1_filter               = "hltMu23Ele12GsfTrackIsoLegEle12GsfCaloIdTrackIdIsoMediumWPFilter";
          leg2_filter               = "hltL1Mu12EG7L3IsoMuFiltered23";
          alt_trig_obj_label        = "triggerObjectsEle23Mu8";
          alt_leg1_filter           = "hltMu8Ele23GsfTrackIsoLegEle23GsfCaloIdTrackIdIsoMediumWPFilter";
          alt_leg2_filter           = "hltL1sL1Mu5EG20ORL1Mu5IsoEG18L3IsoFiltered8";
        } else if (mc_ == mc::spring15_74X){
/*          trig_obj_label            = "triggerObjectsEle12Mu23";
          leg1_filter               = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
          leg2_filter               = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23";
          alt_trig_obj_label        = "triggerObjectsEle23Mu8";
          alt_leg1_filter           = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
          alt_leg2_filter           = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8";
*/

       trig_obj_label = "triggerObjectsEle12Mu17";
         leg1_filter = "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         leg2_filter = "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered17";
         alt_trig_obj_label = "triggerObjectsEle17Mu8";
         alt_leg1_filter = "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         alt_leg2_filter = "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8";

				} 
      } else if (channel_ == channel::tt){
         if (mc_ == mc::phys14_72X){
         trig_obj_label              = "triggerObjectsDoubleMediumTau40";
         alt_trig_obj_label          = "triggerObjectsDoubleMediumTau40";
         leg1_filter                 = "hltL1sDoubleTauJet36erORDoubleTauJet68er";
         extra_leg2_filter           = "hltDoubleL2IsoTau35eta2p1";
         leg2_filter                 = "hltDoublePFTau40TrackPt1MediumIsolationDz02Reg";
       } else if (mc_ == mc::spring15_74X){
         trig_obj_label              = "triggerObjectsDoubleMediumTau40";
         alt_trig_obj_label          = "triggerObjectsDoubleMediumTau40";
         leg1_filter                 = "hltDoublePFTau40TrackPt1MediumIsolationDz02Reg";
         extra_leg2_filter           = "hltDoublePFTau40TrackPt1MediumIsolationDz02Reg";
         leg2_filter                 = "hltDoublePFTau40TrackPt1MediumIsolationDz02Reg";
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
    if (!is_data_ && is_embedded_) {
      std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>("triggerObjectsMu17Mu8");
      if (objs.size() > 0) {
        return 0;
      } else {
        return 1;
      }
    }

    if (is_embedded_) return 0; // Don't do object matching for embedded events

    std::vector<CompositeCandidate *> & dileptons = event->GetPtrVec<CompositeCandidate>(pair_label_);
    std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trig_obj_label);

    std::vector<CompositeCandidate *> dileptons_pass;

    if ((channel_ == channel::et || channel_ == channel::mt)&&mc_ != mc::phys14_72X&&mc_ != mc::spring15_74X) {
      for (unsigned i = 0; i < dileptons.size(); ++i) {
        bool leg1_match = IsFilterMatched(dileptons[i]->At(0), objs, leg1_filter, 0.5);
        bool leg2_match = IsFilterMatched(dileptons[i]->At(1), objs, leg2_filter, 0.5);
        if (leg1_match && leg2_match) dileptons_pass.push_back(dileptons[i]);
      }
    }

    if ((channel_ == channel::et || channel_ == channel::mt || channel_ == channel::zmm || channel_ == channel::zee || channel_ == channel::tpzee || channel_ == channel::tpzmm) 
        &&(mc_ == mc::phys14_72X || mc_ == mc::spring15_74X)) {
      std::vector<TriggerObject *> alt_objs = event->GetPtrVec<TriggerObject>(alt_trig_obj_label);
      ic::erase_if_not(alt_objs,boost::bind(&TriggerObject::pt,_1)>alt_min_online_pt);
      for (unsigned i = 0; i < dileptons.size(); ++i) {
        bool leg1_match = false;
        bool leg2_match = false;
        //unsigned leg1_match_index = 0;
        //unsigned leg2_match_index = 0;
       if(do_leptonplustau_){
         leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, leg1_filter, 0.5).first&&IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, extra_leg2_filter,0.5).first;
         leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, leg2_filter, 0.5).first&&IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, extra_leg2_filter,0.5).first;
         //leg1_match_index = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, leg1_filter, 0.5).second;
         //leg2_match_index = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, leg2_filter, 0.5).second;
         }
       if (leg1_match && leg2_match){
         dileptons_pass.push_back(dileptons[i]);
         /*double leg1_trigger_object_pt = objs.at(leg1_match_index)->pt();
         double leg1_trigger_object_eta = objs.at(leg1_match_index)->eta();
         double leg2_trigger_object_pt = objs.at(leg2_match_index)->pt();
         double leg2_trigger_object_eta = objs.at(leg2_match_index)->eta();
         if(store_trigobjpt_){
           event->Add("leg1_trigger_obj_pt",leg1_trigger_object_pt);
           event->Add("leg1_trigger_obj_eta",leg1_trigger_object_eta);
           event->Add("leg2_trigger_obj_pt",leg2_trigger_object_pt);
           event->Add("leg2_trigger_obj_eta",leg2_trigger_object_eta);
         }*/
        } else if(do_singlelepton_) {
         bool highpt_leg = dileptons[i]->At(0)->pt()>high_leg_pt;
         leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_objs, alt_leg1_filter, 0.5).first;
         //leg1_match_index = IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_objs, alt_leg1_filter, 0.5).second;
//					leg2_match = IsFilterMatched(dileptons[i]->At(1),alt_objs, alt_leg2_filter, 0.5);
         if (leg1_match&&highpt_leg){
           dileptons_pass.push_back(dileptons[i]);	
          /* double trigger_object_pt = alt_objs.at(leg1_match_index)->pt();
           double trigger_object_eta = alt_objs.at(leg1_match_index)->eta();
           if(store_trigobjpt_){
             event->Add("leg1_trig_obj_pt",trigger_object_pt);
             event->Add("leg1_trig_obj_eta",trigger_object_eta);
           }*/
         }
        }
      }
    }


    if (channel_ == channel::em && mc_ != mc::phys14_72X && mc_ != mc::spring15_74X) {
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

   if (channel_ == channel::em && (mc_ == mc::phys14_72X||mc_ == mc::spring15_74X)){
      std::vector<TriggerObject *> const& alt_objs = event->GetPtrVec<TriggerObject>(alt_trig_obj_label);
      for(unsigned i = 0; i < dileptons.size(); ++i){
        bool leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, leg1_filter, 0.5).first;
        bool leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, leg2_filter, 0.5).first;
        //unsigned leg1_match_index = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, leg1_filter, 0.5).second;
        //unsigned leg2_match_index = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, leg2_filter, 0.5).second;
        bool highpt_leg = dileptons[i]->At(1)->pt() >18.0;
        if (leg1_match && leg2_match && highpt_leg) {
          dileptons_pass.push_back(dileptons[i]);
       /*  double leg1_trigger_object_pt = objs.at(leg1_match_index)->pt();
         double leg1_trigger_object_eta = objs.at(leg1_match_index)->eta();
         double leg2_trigger_object_pt = objs.at(leg2_match_index)->pt();
         double leg2_trigger_object_eta = objs.at(leg2_match_index)->eta();
         if(store_trigobjpt_){
           event->Add("leg1_trigger_obj_pt",leg1_trigger_object_pt);
           event->Add("leg1_trigger_obj_eta",leg1_trigger_object_eta);
           event->Add("leg2_trigger_obj_pt",leg2_trigger_object_pt);
           event->Add("leg2_trigger_obj_eta",leg2_trigger_object_eta);
         }*/
        } else {
          leg1_match = IsFilterMatchedWithIndex(dileptons[i]->at(0), alt_objs, alt_leg1_filter, 0.5).first;
          leg2_match = IsFilterMatchedWithIndex(dileptons[i]->at(1), alt_objs, alt_leg2_filter, 0.5).first;
          //leg1_match_index = IsFilterMatchedWithIndex(dileptons[i]->at(0), alt_objs, alt_leg1_filter, 0.5).second;
          //leg2_match_index = IsFilterMatchedWithIndex(dileptons[i]->at(1), alt_objs, alt_leg2_filter, 0.5).second;
          highpt_leg = dileptons[i]->At(0)->pt() > 18.0;
           if (leg1_match && leg2_match && highpt_leg){
              dileptons_pass.push_back(dileptons[i]);
           /*   double leg1_trigger_object_pt = alt_objs.at(leg1_match_index)->pt();
              double leg1_trigger_object_eta = alt_objs.at(leg1_match_index)->eta();
              double leg2_trigger_object_pt = alt_objs.at(leg2_match_index)->pt();
              double leg2_trigger_object_eta = alt_objs.at(leg2_match_index)->eta();
              if(store_trigobjpt_){
                event->Add("leg1_trigger_obj_pt",leg1_trigger_object_pt);
                event->Add("leg1_trigger_obj_eta",leg1_trigger_object_eta);
                event->Add("leg2_trigger_obj_pt",leg2_trigger_object_pt);
                event->Add("leg2_trigger_obj_eta",leg2_trigger_object_eta);
             }*/
           }
        }
      }
    }

   if (channel_ == channel::tt && mc_ == mc::phys14_72X){
     for(unsigned i = 0; i < dileptons.size(); ++i){
       bool leg1_match = IsFilterMatched(dileptons[i]->At(0), objs, leg1_filter, 0.5) && IsFilterMatched(dileptons[i]->At(0), objs, leg2_filter, 0.5) && IsFilterMatched(dileptons[i]->At(0), objs, extra_leg2_filter, 0.5);
       bool leg2_match = IsFilterMatched(dileptons[i]->At(1), objs, leg1_filter, 0.5) && IsFilterMatched(dileptons[i]->At(1), objs, leg2_filter, 0.5) && IsFilterMatched(dileptons[i]->At(1), objs, extra_leg2_filter, 0.5);
       if (leg1_match && leg2_match) dileptons_pass.push_back(dileptons[i]);
      }
    }

   if (channel_ == channel::tt && mc_ == mc::spring15_74X){
     for(unsigned i = 0; i < dileptons.size(); ++i){
       bool leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, leg1_filter, 0.5).first;
       bool leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, leg2_filter, 0.5).first;
       //unsigned leg1_match_index = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, leg1_filter, 0.5).second;
       //unsigned leg2_match_index = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, leg2_filter, 0.5).second;
       if (leg1_match && leg2_match){
         dileptons_pass.push_back(dileptons[i]);
         /*double leg1_trigger_object_pt = objs.at(leg1_match_index)->pt();
         double leg1_trigger_object_eta = objs.at(leg1_match_index)->eta();
         double leg2_trigger_object_pt = objs.at(leg2_match_index)->pt();
         double leg2_trigger_object_eta = objs.at(leg2_match_index)->eta();
         if(store_trigobjpt_){
           event->Add("leg1_trigger_obj_pt",leg1_trigger_object_pt);
           event->Add("leg1_trigger_obj_eta",leg1_trigger_object_eta);
           event->Add("leg2_trigger_obj_pt",leg2_trigger_object_pt);
           event->Add("leg2_trigger_obj_eta",leg2_trigger_object_eta);
         }*/
        }
      }
    }




    if ( channel_ == channel::mtmet && is_data_) {
      for (unsigned i = 0; i < dileptons.size(); ++i) {
        bool leg1_match = IsFilterMatched(dileptons[i]->At(0), objs, leg1_filter, 0.5);
        bool leg2_match = IsFilterMatched(dileptons[i]->At(1), objs, leg2_filter, 0.5);
        if (leg1_match && leg2_match) dileptons_pass.push_back(dileptons[i]);
      }
    }

    if (channel_ == channel::mtmet) {
      /*
      std::vector<TriggerObject *> mu8_obj = objs; // Make a copy of the Mu8 Trigger objects so we can filter
      ic::erase_if(mu8_obj, !boost::bind(MinPtMaxEta, _1, 8.0, 2.1));
      std::vector<Candidate *> l1muon = event->GetPtrVec<Candidate>("l1extraMuons");
      ic::erase_if(l1muon, !boost::bind(MinPtMaxEta, _1, 7.0, 2.1));
      std::vector<Tau *> hlt_taus = event->GetPtrVec<Tau>("hltTaus");
      ic::erase_if(hlt_taus, !boost::bind(MinPtMaxEta, _1, 20.0, 999.0));
      ic::erase_if(hlt_taus, !(boost::bind(&Tau::GetTauID, _1, "decayModeFinding") > 0.5) );
      ic::erase_if(hlt_taus, !(boost::bind(&Tau::GetTauID, _1, "byIsolation") > 0.5) );
      */
      // std::vector<Candidate *> const& l1met = event->GetPtrVec<Candidate>("l1extraMET");
      for (unsigned i = 0; i < dileptons.size(); ++i) {
        // bool leg1_hlt_match = IsFilterMatched(dileptons[i]->At(0), mu8_obj, leg1_filter, 0.5);
        // std::vector<Candidate *> mu_as_vector;
        // mu_as_vector.push_back(dileptons[i]->At(0));
        // bool leg1_l1_match = MatchByDR(mu_as_vector, l1muon, 0.5, false, false).size() > 0;
        // bool leg1_match = leg1_hlt_match && leg1_l1_match;
        // std::vector<Candidate *> tau_as_vector;
        // tau_as_vector.push_back(dileptons[i]->At(1));
        // bool leg2_match = MatchByDR(tau_as_vector, hlt_taus, 0.5, false, false).size() > 0;
        // bool l1_met = l1met.at(0)->pt() > 26.;
        // if (leg1_match && leg2_match && l1_met) dileptons_pass.push_back(dileptons[i]);
        if (true) dileptons_pass.push_back(dileptons[i]); // we'll apply this in HTTPairSelector instead
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
