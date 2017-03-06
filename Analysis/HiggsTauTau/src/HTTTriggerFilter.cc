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

  HTTTriggerFilter::HTTTriggerFilter(std::string const& name) : ModuleBase(name), channel_(channel::zee), mc_(mc::fall15_76X), era_(era::data_2015), strategy_(strategy::fall15) {
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
    std::cout << boost::format(param_fmt()) % "do_singletau"    % do_singletau_;
    std::cout << boost::format(param_fmt()) % "do_filter"       % do_filter_;
    return 0;
  }

  int HTTTriggerFilter::Execute(TreeEvent *event) {

    std::string trig_obj_label;
		std::string alt_trig_obj_label="";
		std::string alt_trk_trig_obj_label;
                std::string alt_er_trig_obj_label;
                std::string alt_er_trk_trig_obj_label;
    std::string leg1_filter;
    std::string leg2_filter;
		std::string extra_leg2_filter;
		std::string alt_leg1_filter;
		std::string alt_trk_leg1_filter;
                std::string alt_er_leg1_filter;
                std::string alt_er_trk_leg1_filter;
/*    std::string alt_trig_obj_label_2;
    std::string alt_leg1_filter_2;*/
		std::string alt_leg2_filter;
    std::string em_alt_trig_obj_label;
    std::string em_alt_leg1_filter;
    std::string em_alt_leg2_filter;
    double alt_min_online_pt=0;
    double alt_trk_min_online_pt=0;
    double alt_er_min_online_pt=0;
    double alt_er_trk_min_online_pt=0;
    double high_leg_pt = 0;
    
    std::string singletau_trg_obj_label;
    double min_online_singletau_pt=0;
    std::string singletau_leg1_filter;
    
    if (is_data_) { //Switch this part off temporarily as we don't have this vector in first processed data
      EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");

      unsigned run = eventInfo->run();
      bool path_found     = false;
      auto const& triggerPathPtrVec = event->GetPtrVec<TriggerPath>("triggerPaths");

      for (unsigned i = 0; i < triggerPathPtrVec.size(); ++i) {
        std::string name = triggerPathPtrVec[i]->name();

        if (channel_ == channel::et||channel_ == channel::zee || channel_ == channel::tpzee) {
       //2015 triggers
       if (run >= 250985 && run <= 271035  && (name.find("HLT_Ele23_WPLoose_Gsf_v") != name.npos)) path_found = true;
       //2016
       if (run >= 271036 /*&& run <= 258654*/  && (name.find("HLT_Ele25_eta2p1_WPTight_Gsf_v") != name.npos) && do_singlelepton_) path_found = true;
       if (run >= 250985 /*&& run <= 258654*/  && (name.find("HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v") != name.npos) && do_leptonplustau_) path_found = true;
        }
        if (channel_ == channel::mt || channel_ == channel::zmm || channel_ == channel::tpzmm) {
           //2015 Triggers
          if (run >= 250985 && run <= 256464 && (name.find("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v") != name.npos || name.find("HLT_IsoMu24_eta2p1_v") != name.npos)) path_found = true;
          if (run >= 256630  && run <= 271035 && (name.find("HLT_IsoMu18_v") != name.npos)) path_found = true;
          if (run >= 271036  /*&& run <= xxxxx*/ && (name.find("HLT_IsoMu22_v") != name.npos || name.find("HLT_IsoTkMu22_v")!=name.npos || name.find("HLT_IsoMu22_eta2p1_v") || name.find("HLT_IsoTkMu22_eta2p1_v")!=name.npos) && do_singlelepton_) path_found = true;
          if (run >= 271036  /*&& run <= xxxxx*/ && (name.find("HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v") != name.npos) && do_leptonplustau_) path_found = true;
          //if (run >= 256630 /* && run <= xxxxx*/ && (name.find("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v") != name.npos || name.find("HLT_IsoMu22_v") != name.npos)) path_found = true;
        }
        if (channel_ == channel::em) {
          if (run >= 250985 && run <= 271035 && (name.find("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") != name.npos || name.find("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v") != name.npos)) path_found = true;
          if (run >= 271036 /*&& run <= xxxx*/ && (name.find("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") != name.npos || name.find("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v") != name.npos)) path_found = true;
        }
        if (channel_ == channel::tt){
          if (run >= 250985 /*&& run <= xxxxx*/ && (name.find("HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg_v") != name.npos || name.find("HLT_DoubleMediumCombinedIsoPFTau35_Trk1_eta2p1_Reg_v") != name.npos)) path_found=true;
          //if (run >= 250985 /*&& run <= xxxxx*/ && (name.find("HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v") != name.npos)) path_found=true;
        }
         if (path_found) break;
      }
      if (!path_found) return 1;

      if (channel_ == channel::et || channel_ == channel::zee || channel_ == channel :: tpzee) {
        if (run >= 250985 && run <= 253620){
          trig_obj_label = "triggerObjectsEle22LooseTau20";
          leg1_filter = "hltSingleEle22WPLooseGsfTrackIsoFilter";
          leg2_filter = "hltPFTau20TrackLooseIso";
          extra_leg2_filter = "hltOverlapFilterIsoEle22WPLooseGsfLooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsEle32Gsf";
          alt_leg1_filter = "hltEle32WPTightGsfTrackIsoFilter";
        }
        if (run >= 253621 && run <= 271035){
          trig_obj_label = "triggerObjectsEle22LooseTau20";
          leg1_filter = "hltEle22WPLooseL1IsoEG20erTau20erGsfTrackIsoFilter";
          leg2_filter = "hltPFTau20TrackLooseIso";
          extra_leg2_filter = "hltOverlapFilterIsoEle22WPLooseGsfLooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsEle23";
          alt_leg1_filter = "hltEle23WPLooseGsfTrackIsoFilter";
          high_leg_pt = 24.;
        }
        if (run >= 271036 /*&& run <= xxxxx*/){
          trig_obj_label = "triggerObjectsEle24LooseTau20SingleL1";
          leg1_filter = "hltEle24WPLooseL1SingleIsoEG22erGsfTrackIsoFilter";
          leg2_filter = "hltPFTau20TrackLooseIso";
          extra_leg2_filter = "hltOverlapFilterSingleIsoEle24WPLooseGsfLooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsEle25GsfTightEta2p1";
          alt_leg1_filter = "hltEle25erWPTightGsfTrackIsoFilter";
          high_leg_pt = 26.;
        }


      }
      if (channel_ == channel::mt || channel_ == channel::zmm || channel_ == channel::tpzmm) {
        if (run >= 250985 && run <= 256465) {
          trig_obj_label = "triggerObjectsIsoMu17LooseTau20";
          leg1_filter = "hltL3crIsoL1sMu16erTauJet20erL1f0L2f10QL3f17QL3trkIsoFiltered0p09";
          leg2_filter = "hltPFTau20TrackLooseIsoAgainstMuon";
          extra_leg2_filter = "hltOverlapFilterIsoMu17LooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsIsoMu24";
          alt_leg1_filter = "hltL3crIsoL1sMu20Eta2p1L1f0L2f10QL3f24QL3trkIsoFiltered0p09";
        }
        if (run >= 256466 && run <= 271035) {
          trig_obj_label = "triggerObjectsIsoMu17LooseTau20";
          leg1_filter = "hltL3crIsoL1sMu16erTauJet20erL1f0L2f10QL3f17QL3trkIsoFiltered0p09";
          leg2_filter = "hltPFTau20TrackLooseIsoAgainstMuon";
          extra_leg2_filter = "hltOverlapFilterIsoMu17LooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsIsoMu18";
          alt_leg1_filter =  "hltL3crIsoL1sMu16L1f0L2f10QL3f18QL3trkIsoFiltered0p09"; 
          high_leg_pt = 19.;
        }
        if (run >= 271036 /*&& run <= xxxxxx*/) {
          trig_obj_label = "triggerObjectsIsoMu19LooseTau20SingleL1";
          leg1_filter = "hltL3crIsoL1sSingleMu18erIorSingleMu20erL1f0L2f10QL3f19QL3trkIsoFiltered0p09";
          leg2_filter = "hltPFTau20TrackLooseIsoAgainstMuon";
          extra_leg2_filter = "hltOverlapFilterSingleIsoMu19LooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsIsoMu22";
          alt_leg1_filter =  "hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09"; 
          alt_trk_trig_obj_label = "triggerObjectsIsoTkMu22";
          alt_trk_leg1_filter =  "hltL3fL1sMu20L1f0Tkf22QL3trkIsoFiltered0p09";
          if (strategy_ == strategy::mssmsummer16){
            alt_er_trig_obj_label = "triggerObjectsIsoMu22Eta2p1";
            alt_er_leg1_filter =  "hltL3crIsoL1sSingleMu20erL1f0L2f10QL3f22QL3trkIsoFiltered0p09"; 
            alt_er_trk_trig_obj_label = "triggerObjectsIsoTkMu22Eta2p1";
            alt_er_trk_leg1_filter = "hltL3fL1sMu20erL1f0Tkf22QL3trkIsoFiltered0p09";
          }
          high_leg_pt = 23.;
          
        }

      }
      if (channel_ == channel::em) {
        // 2015 Triggers
        if (run >= 250985 && run <= 271035){
          trig_obj_label = "triggerObjectsEle12Mu17";
          leg1_filter = "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
          leg2_filter = "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered17";
          alt_trig_obj_label = "triggerObjectsEle17Mu8";
          alt_leg1_filter = "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
          alt_leg2_filter = "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8";
        }
        if (run >= 271036 /*&& run <= XXXXX*/){
         trig_obj_label = "triggerObjectsEle12Mu23";
         leg1_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         leg2_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23";
         alt_trig_obj_label = "triggerObjectsEle23Mu8";
         alt_leg1_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         alt_leg2_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8";

        }

      }
      if(channel_ == channel::tt){
        if(run >= 250985 && run <= 271035){
          trig_obj_label = "triggerObjectsDiTau35";
          leg1_filter = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
          leg2_filter = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
        }
        if(run >= 271036 /*&& run <= xxxxx*/){
          trig_obj_label = "triggerObjectsDoubleMediumTau35";
          leg1_filter = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
          leg2_filter = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
          if(strategy_ == strategy::mssmsummer16){
            alt_trig_obj_label = "triggerObjectsDoubleMediumCombinedIsoTau35Reg";
            alt_leg1_filter = "hltDoublePFTau35TrackPt1MediumCombinedIsolationDz02Reg";
            alt_leg2_filter = "hltDoublePFTau35TrackPt1MediumCombinedIsolationDz02Reg";
          }
        }
      }
    } else {
      //single tau trigger  
      if (channel_ == channel::mt || channel_ == channel::et || channel_ == channel::tt || channel_ == channel::zmm || channel_ == channel::zee){
        singletau_trg_obj_label = "triggerObjectsSingleTau140";
        min_online_singletau_pt=150; // don't know what this would be at the moment so just keep as 0 for now
        singletau_leg1_filter = "hltPFTau140TrackPt50LooseAbsOrRelVLooseIso";
      }  
        
      if (channel_ == channel::et || channel_ == channel::zee || channel_ ==  channel::tpzee) {
				if (mc_ ==mc::fall15_76X){
           trig_obj_label = "triggerObjectsEle22LooseTau20";
           leg1_filter    = "hltEle22WPLooseL1IsoEG20erTau20erGsfTrackIsoFilter";
           leg2_filter    = "hltPFTau20TrackLooseIso";
           extra_leg2_filter  = "hltOverlapFilterIsoEle22WPLooseGsfLooseIsoPFTau20";
           alt_trig_obj_label = "triggerObjectsEle23";
           alt_leg1_filter    = "hltEle23WPLooseGsfTrackIsoFilter";
           alt_min_online_pt = 0.;
           high_leg_pt = 24.;
          } else if(mc_ == mc::spring16_80X){
          trig_obj_label = "triggerObjectsEle24LooseTau20SingleL1";
          leg1_filter = "hltEle24WPLooseL1SingleIsoEG22erGsfTrackIsoFilter";
          leg2_filter = "hltPFTau20TrackLooseIso";
          extra_leg2_filter = "hltOverlapFilterSingleIsoEle24WPLooseGsfLooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsEle25GsfTightEta2p1";
          alt_leg1_filter = "hltEle25erWPTightGsfTrackIsoFilter";
          high_leg_pt = 26.;
          } else if(mc_ == mc::summer16_80X){
          trig_obj_label = "triggerObjectsEle24LooseTau20SingleL1";
          leg1_filter = "hltEle24WPLooseL1SingleIsoEG22erGsfTrackIsoFilter";
          leg2_filter = "hltPFTau20TrackLooseIso";
          extra_leg2_filter = "hltOverlapFilterSingleIsoEle24WPLooseGsfLooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsEle25GsfTightEta2p1";
          alt_leg1_filter = "hltEle25erWPTightGsfTrackIsoFilter";
          high_leg_pt = 26.;
          }
      } else if (channel_ == channel::mt || channel_ == channel::zmm || channel_ == channel::tpzmm) {
        if (mc_ == mc::fall15_76X){
          trig_obj_label  = "triggerObjectsIsoMu17LooseTau20";
          leg1_filter     = "hltL3crIsoL1sMu16erTauJet20erL1f0L2f10QL3f17QL3trkIsoFiltered0p09";
          leg2_filter     = "hltPFTau20TrackLooseIsoAgainstMuon";
          extra_leg2_filter = "hltOverlapFilterIsoMu17LooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsIsoMu18";
          alt_leg1_filter = "hltL3crIsoL1sMu16L1f0L2f10QL3f18QL3trkIsoFiltered0p09";
          alt_leg2_filter = "";
          alt_min_online_pt = 0.;
          high_leg_pt = 19.;
        } else if (mc_ ==mc::spring16_80X){
          trig_obj_label = "triggerObjectsIsoMu19LooseTau20SingleL1";
          leg1_filter = "hltL3crIsoL1sSingleMu18erIorSingleMu20erL1f0L2f10QL3f19QL3trkIsoFiltered0p09";
          leg2_filter = "hltPFTau20TrackLooseIsoAgainstMuon";
          extra_leg2_filter = "hltOverlapFilterSingleIsoMu19LooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsIsoMu22";
          alt_leg1_filter =  "hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09";
          high_leg_pt = 23.;
        } else if (mc_ ==mc::summer16_80X){
          trig_obj_label = "triggerObjectsIsoMu19LooseTau20SingleL1";
          leg1_filter = "hltL3crIsoL1sSingleMu18erIorSingleMu20erL1f0L2f10QL3f19QL3trkIsoFiltered0p09";
          leg2_filter = "hltPFTau20TrackLooseIsoAgainstMuon";
          extra_leg2_filter = "hltOverlapFilterSingleIsoMu19LooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsIsoMu22";
          alt_leg1_filter =  "hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09"; 
          alt_trk_trig_obj_label = "triggerObjectsIsoTkMu22";
          alt_trk_leg1_filter =  "hltL3fL1sMu20L1f0Tkf22QL3trkIsoFiltered0p09";
          alt_er_trig_obj_label = "triggerObjectsIsoMu22Eta2p1";
          alt_er_leg1_filter =  "hltL3crIsoL1sSingleMu20erL1f0L2f10QL3f22QL3trkIsoFiltered0p09"; 
          alt_er_trk_trig_obj_label = "triggerObjectsIsoTkMu22Eta2p1";
          alt_er_trk_leg1_filter = "hltL3fL1sMu20erL1f0Tkf22QL3trkIsoFiltered0p09";
          high_leg_pt = 23.;
        }
      } else if (channel_ == channel::em) {
				if (mc_ ==mc::fall15_76X){
         trig_obj_label = "triggerObjectsEle12Mu17";
         leg1_filter = "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         leg2_filter = "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered17";
         alt_trig_obj_label = "triggerObjectsEle17Mu8";
         alt_leg1_filter = "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         alt_leg2_filter = "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8";
	    } else if (mc_ ==mc::spring16_80X){
         trig_obj_label = "triggerObjectsEle12Mu23";
         leg1_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         leg2_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23";
         alt_trig_obj_label = "triggerObjectsEle23Mu8";
         alt_leg1_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         alt_leg2_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8";
        } else if (mc_ ==mc::summer16_80X){
         trig_obj_label = "triggerObjectsEle12Mu23";
         leg1_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         leg2_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23";
         alt_trig_obj_label = "triggerObjectsEle23Mu8";
         alt_leg1_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         alt_leg2_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8";
        }
      } else if (channel_ == channel::tt){
       if (mc_ == mc::fall15_76X){
         trig_obj_label              = "triggerObjectsDoubleMediumTau35";
         alt_trig_obj_label          = "triggerObjectsDoubleMediumTau35";
         leg1_filter                 = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
         extra_leg2_filter           = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
         leg2_filter                 = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
       } else if (mc_ == mc::summer16_80X){
         trig_obj_label = "triggerObjectsDoubleMediumTau35";
         leg1_filter = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
         leg2_filter = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
         alt_trig_obj_label = "triggerObjectsDoubleMediumCombinedIsoTau35Reg";
         alt_leg1_filter = "hltDoublePFTau35TrackPt1MediumCombinedIsolationDz02Reg";
         alt_leg1_filter = "hltDoublePFTau35TrackPt1MediumCombinedIsolationDz02Reg";
      }
    }
    }

    if (is_embedded_) return 0; // Don't do object matching for embedded events

    std::vector<CompositeCandidate *> & dileptons = event->GetPtrVec<CompositeCandidate>(pair_label_);
    std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trig_obj_label);

    std::vector<CompositeCandidate *> dileptons_pass;

    bool passed_singlemuon = false;
    bool passed_singleelectron = false;
    if ((channel_ == channel::et || channel_ == channel::mt || channel_ == channel::zmm || channel_ == channel::zee || channel_ == channel::tpzee || channel_ == channel::tpzmm) 
        &&(mc_ == mc::fall15_76X || mc_ == mc::spring16_80X || mc_ == mc::summer16_80X)) {
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
         
         if(channel_==channel::mt && mc_==mc::spring16_80X) {
            std::vector<TriggerObject *> alt_trk_objs = event->GetPtrVec<TriggerObject>(alt_trk_trig_obj_label);
            ic::erase_if_not(alt_trk_objs,boost::bind(&TriggerObject::pt,_1)>alt_trk_min_online_pt);
            leg1_match = (IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_objs, alt_leg1_filter, 0.5).first || IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_trk_objs, alt_trk_leg1_filter, 0.5).first);
         }
         
         if((channel_==channel::mt || channel_==channel::zmm || channel_ == channel::tpzmm)&& (mc_ == mc::summer16_80X) ) {
            std::vector<TriggerObject *> alt_trk_objs = event->GetPtrVec<TriggerObject>(alt_trk_trig_obj_label);
            ic::erase_if_not(alt_trk_objs,boost::bind(&TriggerObject::pt,_1)>alt_trk_min_online_pt);
            std::vector<TriggerObject *> alt_er_objs = event->GetPtrVec<TriggerObject>(alt_er_trig_obj_label);
            ic::erase_if_not(alt_er_objs,boost::bind(&TriggerObject::pt,_1)>alt_er_min_online_pt);             
            std::vector<TriggerObject *> alt_er_trk_objs = event->GetPtrVec<TriggerObject>(alt_er_trk_trig_obj_label);
            ic::erase_if_not(alt_er_trk_objs,boost::bind(&TriggerObject::pt,_1)>alt_er_trk_min_online_pt);

            leg1_match = (IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_objs, alt_leg1_filter, 0.5).first || IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_trk_objs, alt_trk_leg1_filter, 0.5).first) || (IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_er_objs, alt_er_leg1_filter, 0.5).first) || (IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_er_trk_objs, alt_er_trk_leg1_filter, 0.5).first); 
            
            //if (channel_==channel::zmm || channel_ == channel::tpzmm){
            //    leg2_match = (IsFilterMatchedWithIndex(dileptons[i]->At(1),alt_objs, alt_leg1_filter, 0.5).first || IsFilterMatchedWithIndex(dileptons[i]->At(1),alt_trk_objs, alt_trk_leg1_filter, 0.5).first) || (IsFilterMatchedWithIndex(dileptons[i]->At(1),alt_er_objs, alt_er_leg1_filter, 0.5).first) || (IsFilterMatchedWithIndex(dileptons[i]->At(1),alt_er_trk_objs, alt_er_trk_leg1_filter, 0.5).first);
            //}   //commented out to only allow lead muon to fire trigger
         }
         
         bool highpt_leg2 = false;
         if (channel_==channel::zmm || channel_ == channel::tpzmm || channel_ == channel::zee || channel_ == channel::tpzee){
           if(mc_ != mc::summer16_80X) leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1),alt_objs, alt_leg1_filter, 0.5).first; 
           highpt_leg2 = dileptons[i]->At(1)->pt()>high_leg_pt;
         }
         //leg1_match_index = IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_objs, alt_leg1_filter, 0.5).second;
//					leg2_match = IsFilterMatched(dileptons[i]->At(1),alt_objs, alt_leg2_filter, 0.5);
         if ((leg1_match&&highpt_leg) || (leg2_match&&highpt_leg2)){
           dileptons_pass.push_back(dileptons[i]);
           if (channel_ == channel::et || channel_ == channel::zee) passed_singleelectron = true;
           if (channel_ == channel::mt || channel_ == channel::zmm) passed_singlemuon = true;
           
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
    event->Add("trg_singlemuon", passed_singlemuon);
    event->Add("trg_singleelectron", passed_singleelectron);

   bool passed_muonelectron = false;
   if (channel_ == channel::em && (mc_ == mc::fall15_76X ||mc_ == mc::spring16_80X || mc_ == mc::summer16_80X)){
      std::vector<TriggerObject *> const& alt_objs = event->GetPtrVec<TriggerObject>(alt_trig_obj_label);
      for(unsigned i = 0; i < dileptons.size(); ++i){
        bool leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, leg1_filter, 0.5).first;
        bool leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, leg2_filter, 0.5).first;
        //unsigned leg1_match_index = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, leg1_filter, 0.5).second;
        //unsigned leg2_match_index = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, leg2_filter, 0.5).second;
        bool highpt_leg = dileptons[i]->At(1)->pt() >18.0;
        if(mc_ == mc::spring16_80X || mc_ == mc::summer16_80X) highpt_leg = dileptons[i]->At(1)->pt() >24.0;
        if (leg1_match && leg2_match && highpt_leg) {
          passed_muonelectron = true;  
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
          if(mc_ == mc::spring16_80X || mc_ == mc::summer16_80X) highpt_leg = dileptons[i]->At(0)->pt() >24.0;
           if (leg1_match && leg2_match && highpt_leg){
              passed_muonelectron = true; 
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
    event->Add("trg_muonelectron", passed_muonelectron);


   if (channel_ == channel::tt && (mc_ == mc::fall15_76X || mc_ == mc::spring16_80X)){
     std::vector<Candidate *> l1taus;
     if(!is_data_){
       l1taus = event->GetPtrVec<Candidate>("l1isoTaus");
       ic::erase_if(l1taus, !boost::bind(MinPtMaxEta, _1, 28.0, 999.0));
     }
     for(unsigned i = 0; i < dileptons.size(); ++i){
       bool leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, leg1_filter, 0.5).first;
       bool leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, leg2_filter, 0.5).first;
       std::vector<Candidate *> match_taus;
       match_taus.push_back(dileptons[i]->At(0));
       match_taus.push_back(dileptons[i]->At(1));
       bool match_l1_parts = true;
       if(!is_data_){
         match_l1_parts = (MatchByDR(match_taus,l1taus,0.5,true,true)).size() == 2;
       }
       //std::cout<<match_l1_parts<<std::endl;
       //unsigned leg1_match_index = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, leg1_filter, 0.5).second;
       //unsigned leg2_match_index = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, leg2_filter, 0.5).second;
       if (leg1_match && leg2_match && match_l1_parts){  
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
    
   bool passed_doubletau = false;
   if (channel_ == channel::tt && mc_ == mc::summer16_80X){
     for(unsigned i = 0; i < dileptons.size(); ++i){
       bool leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, leg1_filter, 0.5).first;
       bool leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, leg2_filter, 0.5).first;
       
       if(alt_trig_obj_label!=""){
         std::vector<TriggerObject *> alt_objs = event->GetPtrVec<TriggerObject>(alt_trig_obj_label);  
         leg1_match = leg1_match || IsFilterMatchedWithIndex(dileptons[i]->At(0), alt_objs, alt_leg1_filter, 0.5).first;
         leg2_match = leg2_match || IsFilterMatchedWithIndex(dileptons[i]->At(1), alt_objs, alt_leg2_filter, 0.5).first;
       }

       if (leg1_match && leg2_match){
         passed_doubletau = true;  
         dileptons_pass.push_back(dileptons[i]);
        }
      }
    }
    event->Add("trg_doubletau", passed_doubletau);
    
    bool passed_singletau_1 = false;
    bool passed_singletau_2 = false;
    if (do_singletau_ && (channel_ == channel::tt || channel_ == channel::mt || channel_ == channel::et || channel_ == channel::zmm || channel_ == channel::zee)){
      for(unsigned i = 0; i < dileptons.size(); ++i){  
        std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(singletau_trg_obj_label);
        bool leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, singletau_leg1_filter, 0.5).first;
        bool taupt_leg1 = dileptons[i]->At(0)->pt() > min_online_singletau_pt;
        bool leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, singletau_leg1_filter, 0.5).first;
        bool taupt_leg2 = dileptons[i]->At(1)->pt() > min_online_singletau_pt;
        
        if (leg1_match && taupt_leg1) passed_singletau_1 = true;
        if (leg2_match && taupt_leg2) passed_singletau_2 = true;
        if(passed_singletau_1 || passed_singletau_2) dileptons_pass.push_back(dileptons[i]);
      }
    }
    event->Add("trg_singletau_1", passed_singletau_1);
    event->Add("trg_singletau_2", passed_singletau_2);


    
    if(!do_filter_){
      return 0;    
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
