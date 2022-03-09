#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTTriggerFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
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

  HTTTriggerFilter::HTTTriggerFilter(std::string const& name) : ModuleBase(name), channel_(channel::zee), mc_(mc::summer12_53X), era_(era::data_2015), strategy_(strategy::mssmsummer16){
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
    std::string alt_trig_obj_label_2;
    std::string alt_trig_obj_label_3;
    std::string leg1_filter;
    std::string leg2_filter;
    std::string extra_leg2_filter;
    std::string extra_filter ="";
    std::string alt_extra_filter ="";
    std::string alt_extra_filter_2 ="";
    std::string alt_leg1_filter;
    std::string alt_jetleg1_filter;
    std::string alt_jetleg2_filter;
    std::string alt_trk_leg1_filter;
    std::string alt_er_leg1_filter;
    std::string alt_er_trk_leg1_filter;
    std::string alt_leg1_filter_2;
    std::string alt_leg2_filter_2;
    std::string alt_leg1_filter_3;
    std::string alt_leg2_filter_3;
    std::string extra_filter_2;
    std::string alt_leg2_filter;
    std::string em_alt_trig_obj_label;
    std::string em_alt_leg1_filter;
    std::string em_alt_leg2_filter;
    std::string alt_cross_trig_obj_label;
    std::string alt_cross_leg1_filter;
    std::string alt_cross_leg2_filter;
    std::string alt_cross_extra_leg2_filter;
    
    double alt_min_online_pt=0;
    double alt_trk_min_online_pt=0;
    double alt_er_min_online_pt=0;
    double alt_er_trk_min_online_pt=0;
    double high_leg_pt = 0;
    
    std::string singletau_trg_obj_label;
    double min_online_singletau_pt=0;
    std::string singletau_leg1_filter;
    
    if (is_data_) { 
      EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");

      unsigned run = eventInfo->run();
      bool path_found     = false;
      auto const& triggerPathPtrVec = event->GetPtrVec<TriggerPath>("triggerPaths");

      for (unsigned i = 0; i < triggerPathPtrVec.size(); ++i) {
        std::string name = triggerPathPtrVec[i]->name();

        if (channel_ == channel::et||channel_ == channel::zee || channel_ == channel::tpzee) {
       if (run >= 271036 /*&& run <= 258654*/  && (name.find("HLT_Ele25_eta2p1_WPTight_Gsf_v") != name.npos)) path_found = true;
       if (run >= 294927 && run < 314472  && (name.find("HLT_Ele32_WPTight_Gsf_L1DoubleEG_v") != name.npos || name.find("HLT_Ele27_WPTight_Gsf_v") != name.npos || name.find("HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTau30_eta2p1_CrossL1_v") != name.npos || name.find("HLT_Ele35_WPTight_Gsf_v") != name.npos)) path_found = true;
       if (run >= 314472 && run < 317509  && (name.find("HLT_Ele32_WPTight_Gsf_v") != name.npos || name.find("HLT_Ele35_WPTight_Gsf_v") != name.npos || //consider adding Ele28 when available in ic miniaod
                   name.find("HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTau30_eta2p1_CrossL1_v") != name.npos)) path_found = true;
       if (run >= 317509 /*&& run < xxx*/  && (name.find("HLT_Ele32_WPTight_Gsf_v") != name.npos || name.find("HLT_Ele35_WPTight_Gsf_v") != name.npos 
                   || name.find("HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTauHPS30_eta2p1_CrossL1_v") != name.npos)) path_found = true;
        }
        if (channel_ == channel::mt || channel_ == channel::zmm || channel_ == channel::tpzmm) {
          if (run >= 271036  /*&& run <= xxxxx*/ && (name.find("HLT_IsoMu22_v") != name.npos || name.find("HLT_IsoTkMu22_v")!=name.npos || name.find("HLT_IsoMu22_eta2p1_v") || name.find("HLT_IsoTkMu22_eta2p1_v")!=name.npos) ) path_found = true;
          if (run >= 294927 && run < 317509 && (name.find("HLT_IsoMu24_v") != name.npos || name.find("HLT_IsoMu24_eta2p1_v") != name.npos || name.find("HLT_IsoMu27_v") != name.npos || name.find("HLT_IsoMu20_eta2p1_LooseChargedIsoPFTau27_eta2p1_CrossL1_v") != name.npos)) path_found = true;
          if (run >= 317509 /*&& run <= 258654*/  && (name.find("HLT_IsoMu24_v") != name.npos || name.find("HLT_IsoMu24_eta2p1_v") != name.npos || name.find("HLT_IsoMu27_v") != name.npos || name.find("HLT_IsoMu20_eta2p1_LooseChargedIsoPFTauHPS27_eta2p1_CrossL1_v") != name.npos)) path_found = true;
        }
        if (channel_ == channel::em) {
          if (run >= 271036 && run <= 278819 && (name.find("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") != name.npos || name.find("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v") != name.npos)) path_found = true;
          if (run >= 278820 && run <= 284044 && (name.find("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") != name.npos || name.find("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v") != name.npos)) path_found = true; 
          if (run >= 294927 /*&& run <= xxxx*/ && (name.find("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") != name.npos || name.find("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v") != name.npos)) path_found = true; 
        }
        if (channel_ == channel::tt){
          if (run >= 250985 && run <= 284044 && (name.find("HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg_v") != name.npos || name.find("HLT_DoubleMediumCombinedIsoPFTau35_Trk1_eta2p1_Reg_v") != name.npos)) path_found=true;
          if (run >= 294927 && run < 314472 && (name.find("HLT_DoubleTightChargedIsoPFTau35_Trk1_TightID_eta2p1_Reg_v") != name.npos || name.find("HLT_DoubleMediumChargedIsoPFTau40_Trk1_TightID_eta2p1_Reg_v") != name.npos || name.find("HLT_DoubleTightChargedIsoPFTau40_Trk1_eta2p1_Reg_v") != name.npos)) path_found=true;
          // only added vbf tau trg for 2018 here
          if (run >= 314472 && run < 317509 && (name.find("HLT_DoubleTightChargedIsoPFTau35_Trk1_TightID_eta2p1_Reg_v") != name.npos || name.find("HLT_DoubleMediumChargedIsoPFTau40_Trk1_TightID_eta2p1_Reg_v") != name.npos || name.find("HLT_DoubleTightChargedIsoPFTau40_Trk1_eta2p1_Reg_v") != name.npos || name.find("HLT_VBF_DoubleLooseChargedIsoPFTau20_Trk1_eta2p1_v") != name.npos)) path_found=true;
          if (run >= 317509 /*&& run < xxxxx*/ && (name.find("HLT_DoubleMediumChargedIsoPFTauHPS35_Trk1_eta2p1_Reg_v") != name.npos || name.find("HLT_VBF_DoubleLooseChargedIsoPFTauHPS20_Trk1_eta2p1_v") != name.npos )) path_found=true;
        }

         if (path_found) break;
      }
      if (!path_found) return 1;

      if (channel_ == channel::et || channel_ == channel::zee || channel_ == channel :: tpzee) {
        if (run >= 271036 && run <= 284044){
          trig_obj_label = "triggerObjectsEle24LooseTau20SingleL1";
          leg1_filter = "hltEle24WPLooseL1SingleIsoEG22erGsfTrackIsoFilter";
          leg2_filter = "hltPFTau20TrackLooseIso";
          extra_leg2_filter = "hltOverlapFilterSingleIsoEle24WPLooseGsfLooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsEle25GsfTightEta2p1";
          alt_leg1_filter = "hltEle25erWPTightGsfTrackIsoFilter";
          high_leg_pt = 26.;
        }
        if(run >= 294927 && run < 314472){ //2017
          trig_obj_label = "triggerObjectsEle24Tau30";
          leg1_filter = "hltEle24erWPTightGsfTrackIsoFilterForTau";
          leg2_filter = "hltSelectedPFTau30LooseChargedIsolationL1HLTMatched";
          extra_leg2_filter = "hltOverlapFilterIsoEle24WPTightGsfLooseIsoPFTau30";
          alt_trig_obj_label = "triggerObjectsEle27";
          alt_leg1_filter = "hltEle27WPTightGsfTrackIsoFilter";
          alt_trig_obj_label_2 = "triggerObjectsEle32L1DoubleEG";
          alt_leg1_filter_2 = "hltEle32L1DoubleEGWPTightGsfTrackIsoFilter";
          extra_filter_2 = "hltEGL1SingleEGOrFilter";
          high_leg_pt = 25.;
        }
        if(run >= 314472 && run < 317509){ //2018 pre hps
          trig_obj_label = "triggerObjectsEle24Tau30";
          leg1_filter = "hltEle24erWPTightGsfTrackIsoFilterForTau";
          leg2_filter = "hltSelectedPFTau30LooseChargedIsolationL1HLTMatched";
          extra_leg2_filter = "hltOverlapFilterIsoEle24WPTightGsfLooseIsoPFTau30";
          alt_trig_obj_label = "triggerObjectsEle32";
          alt_leg1_filter = "hltEle32WPTightGsfTrackIsoFilter";
          alt_trig_obj_label_2 = "triggerObjectsEle35";
          alt_leg1_filter_2 = "hltEle35noerWPTightGsfTrackIsoFilter";
          high_leg_pt = 25.;
        }
        if(run >= 317509){ //2018 post hps
          trig_obj_label = "triggerObjectsEle24TauHPS30";
          leg1_filter = "hltEle24erWPTightGsfTrackIsoFilterForTau";
          leg2_filter = "hltHpsSelectedPFTau30LooseChargedIsolationL1HLTMatched";
          extra_leg2_filter = "hltHpsOverlapFilterIsoEle24WPTightGsfLooseIsoPFTau30";
          alt_trig_obj_label = "triggerObjectsEle32";
          alt_leg1_filter = "hltEle32WPTightGsfTrackIsoFilter";
          alt_trig_obj_label_2 = "triggerObjectsEle35";
          alt_leg1_filter_2 = "hltEle35noerWPTightGsfTrackIsoFilter";
          high_leg_pt = 25.;
        }


      }
      if (channel_ == channel::mt || channel_ == channel::zmm || channel_ == channel::tpzmm) {
        if (run >= 271036 && run <= 284044 ) {
          alt_trig_obj_label = "triggerObjectsIsoMu22";
          alt_leg1_filter =  "hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09"; 
          alt_trk_trig_obj_label = "triggerObjectsIsoTkMu22";
          alt_trk_leg1_filter =  "hltL3fL1sMu20L1f0Tkf22QL3trkIsoFiltered0p09";
          alt_er_trig_obj_label = "triggerObjectsIsoMu22Eta2p1";
          alt_er_leg1_filter =  "hltL3crIsoL1sSingleMu20erL1f0L2f10QL3f22QL3trkIsoFiltered0p09"; 
          alt_er_trk_trig_obj_label = "triggerObjectsIsoTkMu22Eta2p1";
          alt_er_trk_leg1_filter = "hltL3fL1sMu20erL1f0Tkf22QL3trkIsoFiltered0p09";
          high_leg_pt = 23.;
          trig_obj_label = "triggerObjectsIsoMu19LooseTau20SingleL1";
          leg1_filter = "hltL3crIsoL1sSingleMu18erIorSingleMu20erL1f0L2f10QL3f19QL3trkIsoFiltered0p09";
          leg2_filter = "hltPFTau20TrackLooseIsoAgainstMuon";
          extra_leg2_filter = "hltOverlapFilterSingleIsoMu19LooseIsoPFTau20";
          alt_cross_trig_obj_label = "triggerObjectsIsoMu19LooseTau20";
          alt_cross_leg1_filter = "hltL3crIsoL1sMu18erTauJet20erL1f0L2f10QL3f19QL3trkIsoFiltered0p09";
          alt_cross_leg2_filter = "hltPFTau20TrackLooseIsoAgainstMuon";
          alt_cross_extra_leg2_filter = "hltOverlapFilterIsoMu19LooseIsoPFTau20";
        }
        if (run >= 294927 && run < 315974) { // 2017 + 2018 pre hps
          alt_trig_obj_label = "triggerObjectsIsoMu24";
          alt_leg1_filter =  "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07";
          alt_trk_trig_obj_label = "triggerObjectsIsoMu27";
          alt_trk_leg1_filter =  "hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07";
          high_leg_pt = 21.;
          trig_obj_label = "triggerObjectsIsoMu20Tau27";
          leg1_filter = "hltL3crIsoL1sMu18erTau24erIorMu20erTau24erL1f0L2f10QL3f20QL3trkIsoFiltered0p07";
          leg2_filter = "hltSelectedPFTau27LooseChargedIsolationAgainstMuonL1HLTMatched";
          extra_leg2_filter = "hltOverlapFilterIsoMu20LooseChargedIsoPFTau27L1Seeded";
        }
        if (run >= 315974 && run < 317509) { // 2018 pre hps with new filter
          alt_trig_obj_label = "triggerObjectsIsoMu24";
          alt_leg1_filter =  "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07";
          alt_trk_trig_obj_label = "triggerObjectsIsoMu27";
          alt_trk_leg1_filter =  "hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07";
          high_leg_pt = 21.;
          trig_obj_label = "triggerObjectsIsoMu20Tau27";
          leg1_filter = "hltL3crIsoBigORMu18erTauXXer2p1L1f0L2f10QL3f20QL3trkIsoFiltered0p07";
          leg2_filter = "hltSelectedPFTau27LooseChargedIsolationAgainstMuonL1HLTMatched";
          extra_leg2_filter = "hltOverlapFilterIsoMu20LooseChargedIsoPFTau27L1Seeded";
        }
        if (run >= 317509) { //2018 post hps
          alt_trig_obj_label = "triggerObjectsIsoMu24";
          alt_leg1_filter =  "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07";
          alt_trk_trig_obj_label = "triggerObjectsIsoMu27";
          alt_trk_leg1_filter =  "hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07";
          high_leg_pt = 21.;
          trig_obj_label = "triggerObjectsIsoMu20TauHPS27"; //introduce HPS X-trigger
          leg1_filter = "hltL3crIsoBigORMu18erTauXXer2p1L1f0L2f10QL3f20QL3trkIsoFiltered0p07";
          leg2_filter = "hltHpsSelectedPFTau27LooseChargedIsolationAgainstMuonL1HLTMatched";
          extra_leg2_filter = "hltHpsOverlapFilterIsoMu20LooseChargedIsoPFTau27L1Seeded";
        }

      }
      if (channel_ == channel::em) {
        if (run >= 271036 && run <= 278819){
         trig_obj_label = "triggerObjectsEle12Mu23";
         leg1_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         leg2_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23";
         alt_trig_obj_label = "triggerObjectsEle23Mu8";
         alt_leg1_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         alt_leg2_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8";
        }
        if (run >= 278820 && run <= 284044){
         trig_obj_label = "triggerObjectsEle12Mu23DZ";
         leg1_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         leg2_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23";
         extra_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLDZFilter";
         alt_trig_obj_label = "triggerObjectsEle23Mu8DZ";
         alt_leg1_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         alt_leg2_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8";
         alt_extra_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLDZFilter";
        }
        if (run >= 294927 /* && run <= xxx*/) {
          trig_obj_label = "triggerObjectsMu23Ele12DZ";
          leg1_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
          leg2_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23";
          extra_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLDZFilter";
          alt_trig_obj_label = "triggerObjectsMu8Ele23DZ";
          alt_leg1_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
          alt_leg2_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8";
          alt_leg2_filter_2 = "hltMuon8Ele23RelTrkIsoFiltered0p4MuonLeg";
          alt_extra_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLDZFilter";
        }

      }
      if(channel_ == channel::tt){
        if(run >= 271036 && run <= 284044){
          trig_obj_label = "triggerObjectsDoubleMediumTau35";
          leg1_filter = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
          leg2_filter = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
          if(strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16){
            alt_trig_obj_label = "triggerObjectsDoubleMediumCombinedIsoTau35Reg";
            alt_leg1_filter = "hltDoublePFTau35TrackPt1MediumCombinedIsolationDz02Reg";
            alt_leg2_filter = "hltDoublePFTau35TrackPt1MediumCombinedIsolationDz02Reg";
          }
        }
        if(run >= 294927 && run < 314472){
          trig_obj_label = "triggerObjectsDoubleTightIsoTau35";
          leg1_filter = "hltDoublePFTau35TrackPt1TightChargedIsolationAndTightOOSCPhotonsDz02Reg";
          leg2_filter = "hltDoublePFTau35TrackPt1TightChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_trig_obj_label = "triggerObjectsDoubleMediumIsoTau40";
          alt_leg1_filter = "hltDoublePFTau40TrackPt1MediumChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_leg2_filter = "hltDoublePFTau40TrackPt1MediumChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_trig_obj_label_2 = "triggerObjectsDoubleTightIsoTau40";
          alt_leg1_filter_2 = "hltDoublePFTau40TrackPt1TightChargedIsolationDz02Reg";
          alt_leg2_filter_2 = "hltDoublePFTau40TrackPt1TightChargedIsolationDz02Reg";
        }
        if(run >= 314472 && run < 317509){
          trig_obj_label = "triggerObjectsDoubleTightIsoTau35";
          leg1_filter = "hltDoublePFTau35TrackPt1TightChargedIsolationAndTightOOSCPhotonsDz02Reg";
          leg2_filter = "hltDoublePFTau35TrackPt1TightChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_trig_obj_label = "triggerObjectsDoubleMediumIsoTau40TightID";
          alt_leg1_filter = "hltDoublePFTau40TrackPt1MediumChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_leg2_filter = "hltDoublePFTau40TrackPt1MediumChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_trig_obj_label_2 = "triggerObjectsDoubleTightIsoTau40";
          alt_leg1_filter_2 = "hltDoublePFTau40TrackPt1TightChargedIsolationDz02Reg";
          alt_leg2_filter_2 = "hltDoublePFTau40TrackPt1TightChargedIsolationDz02Reg";
          alt_trig_obj_label_3 = "triggerObjectsVBFDoubleLooseChargedIsoPFTau20";
          alt_leg1_filter_3 = "hltDoublePFTau20TrackLooseChargedIsoAgainstMuon";
          alt_leg2_filter_3 = "hltDoublePFTau20TrackLooseChargedIsoAgainstMuon";
          alt_jetleg1_filter = "hltMatchedVBFOnePFJet2CrossCleanedFromDoubleLooseChargedIsoPFTau20"; // leading 
          //subleading from following (but contains leading as well)
          alt_jetleg2_filter = "hltMatchedVBFTwoPFJets2CrossCleanedFromDoubleLooseChargedIsoPFTau20"; 
        }
        if (run >= 317509) {
          trig_obj_label = "triggerObjectsDoubleMediumIsoTauHPS35";
          leg1_filter = "hltHpsDoublePFTau35TrackPt1MediumChargedIsolationDz02Reg";
          leg2_filter = "hltHpsDoublePFTau35TrackPt1MediumChargedIsolationDz02Reg";
          alt_trig_obj_label_3 = "triggerObjectsVBFDoubleLooseChargedIsoPFTauHPS20";
          alt_leg1_filter_3 = "hltHpsDoublePFTau20TrackLooseChargedIsoAgainstMuon";
          alt_leg2_filter_3 = "hltHpsDoublePFTau20TrackLooseChargedIsoAgainstMuon";
          alt_jetleg1_filter = "hltMatchedVBFOnePFJet2CrossCleanedFromDoubleLooseChargedIsoPFTauHPS20"; // leading 
          //subleading from following (but contains leading as well)
          alt_jetleg2_filter = "hltMatchedVBFTwoPFJets2CrossCleanedFromDoubleLooseChargedIsoPFTauHPS20"; 
        }
      }
      
      //single tau trigger  
      if (channel_ == channel::mt || channel_ == channel::et || channel_ == channel::tt || channel_ == channel::zmm || channel_ == channel::zee) {
        if(run >= 271036 /*&& run <= xxxxxx*/){
          singletau_trg_obj_label = "triggerObjectsSingleTau140";
          min_online_singletau_pt=150; // don't know what this would be at the moment so just keep as 0 for now
          singletau_leg1_filter = "hltPFTau140TrackPt50LooseAbsOrRelVLooseIso";
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
          if(era_ == era::data_2016){
          trig_obj_label = "triggerObjectsEle24LooseTau20SingleL1";
          leg1_filter = "hltEle24WPLooseL1SingleIsoEG22erGsfTrackIsoFilter";
          leg2_filter = "hltPFTau20TrackLooseIso";
          extra_leg2_filter = "hltOverlapFilterSingleIsoEle24WPLooseGsfLooseIsoPFTau20";
          alt_trig_obj_label = "triggerObjectsEle25GsfTightEta2p1";
          alt_leg1_filter = "hltEle25erWPTightGsfTrackIsoFilter";
          high_leg_pt = 26.;
          } else if (era_ == era::data_2017){
          trig_obj_label = "triggerObjectsEle24Tau30";
          leg1_filter = "hltEle24erWPTightGsfTrackIsoFilterForTau";
          leg2_filter = "hltSelectedPFTau30LooseChargedIsolationL1HLTMatched";
          extra_leg2_filter = "hltOverlapFilterIsoEle24WPTightGsfLooseIsoPFTau30";
          alt_trig_obj_label = "triggerObjectsEle27";
          alt_leg1_filter = "hltEle27WPTightGsfTrackIsoFilter";
          alt_trig_obj_label_2 = "triggerObjectsEle32L1DoubleEG";
          alt_leg1_filter_2 = "hltEle32L1DoubleEGWPTightGsfTrackIsoFilter";
          extra_filter_2 = "hltEGL1SingleEGOrFilter";
          high_leg_pt = 25.;
          if(is_embedded_){
            leg2_filter="hltL1sBigORLooseIsoEGXXerIsoTauYYerdRMin0p3";
            extra_leg2_filter="";
          }
          } else if (era_ == era::data_2018){
          trig_obj_label = "triggerObjectsEle24TauHPS30";
          leg1_filter = "hltEle24erWPTightGsfTrackIsoFilterForTau";
          leg2_filter = "hltHpsSelectedPFTau30LooseChargedIsolationL1HLTMatched";
          extra_leg2_filter = "hltHpsOverlapFilterIsoEle24WPTightGsfLooseIsoPFTau30";
          alt_trig_obj_label = "triggerObjectsEle32";
          alt_leg1_filter = "hltEle32WPTightGsfTrackIsoFilter";
          alt_trig_obj_label_2 = "triggerObjectsEle35";
          alt_leg1_filter_2 = "hltEle35noerWPTightGsfTrackIsoFilter";
          high_leg_pt = 25.;
          if(is_embedded_){
            // add non HPS version also for emebdded and data ??
            leg2_filter="hltL1sBigORLooseIsoEGXXerIsoTauYYerdRMin0p3";
            extra_leg2_filter="";
          }

          }
      } else if (channel_ == channel::mt || channel_ == channel::zmm || channel_ == channel::tpzmm) {
        if (era_ == era::data_2016){
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
          // cross triggers
          trig_obj_label = "triggerObjectsIsoMu19LooseTau20SingleL1";
          leg1_filter = "hltL3crIsoL1sSingleMu18erIorSingleMu20erL1f0L2f10QL3f19QL3trkIsoFiltered0p09";
          leg2_filter = "hltPFTau20TrackLooseIsoAgainstMuon";
          extra_leg2_filter = "hltOverlapFilterSingleIsoMu19LooseIsoPFTau20";
          alt_cross_trig_obj_label = "triggerObjectsIsoMu19LooseTau20";
          alt_cross_leg1_filter = "hltL3crIsoL1sMu18erTauJet20erL1f0L2f10QL3f19QL3trkIsoFiltered0p09";
          alt_cross_leg2_filter = "hltPFTau20TrackLooseIsoAgainstMuon";
          alt_cross_extra_leg2_filter = "hltOverlapFilterIsoMu19LooseIsoPFTau20";  
          if(is_embedded_){
            leg2_filter = "hltPFTau20";
            alt_cross_leg2_filter = "hltPFTau20";
            alt_cross_extra_leg2_filter = "";
            extra_leg2_filter = "";
          }
        } else if (era_ == era::data_2017) {
          alt_trig_obj_label = "triggerObjectsIsoMu24";
          alt_leg1_filter =  "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07";
          alt_trk_trig_obj_label = "triggerObjectsIsoMu27";
          alt_trk_leg1_filter =  "hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07";
          high_leg_pt = 21.;
          trig_obj_label = "triggerObjectsIsoMu20Tau27";
          leg1_filter = "hltL3crIsoL1sMu18erTau24erIorMu20erTau24erL1f0L2f10QL3f20QL3trkIsoFiltered0p07";
          leg2_filter = "hltSelectedPFTau27LooseChargedIsolationAgainstMuonL1HLTMatched";
          extra_leg2_filter= "hltOverlapFilterIsoMu20LooseChargedIsoPFTau27L1Seeded";
          if(is_embedded_){
            leg2_filter="hltL1sMu18erTau24erIorMu20erTau24er";
            extra_leg2_filter="";
          }
        } else if (era_ == era::data_2018) {
          alt_trig_obj_label = "triggerObjectsIsoMu24";
          alt_leg1_filter =  "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07";
          alt_trk_trig_obj_label = "triggerObjectsIsoMu27";
          alt_trk_leg1_filter =  "hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07";
          high_leg_pt = 21.;
          trig_obj_label = "triggerObjectsIsoMu20TauHPS27"; 
          leg1_filter = "hltL3crIsoBigORMu18erTauXXer2p1L1f0L2f10QL3f20QL3trkIsoFiltered0p07";
          leg2_filter = "hltHpsSelectedPFTau27LooseChargedIsolationAgainstMuonL1HLTMatched";
          extra_leg2_filter = "hltHpsOverlapFilterIsoMu20LooseChargedIsoPFTau27L1Seeded";

          if(is_embedded_) {
            trig_obj_label = "triggerObjectsIsoMu20TauHPS27"; // MC uses HPS only
            leg1_filter = "hltL3crIsoBigORMu18erTauXXer2p1L1f0L2f10QL3f20QL3trkIsoFiltered0p07";
            leg2_filter = "hltL1sBigORMu18erTauXXer2p1";
            extra_leg2_filter = "";
          }
        }

      } else if (channel_ == channel::em) {
        if (era_ == era::data_2016){
         trig_obj_label = "triggerObjectsEle12Mu23";
         leg1_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         leg2_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23";
         alt_trig_obj_label = "triggerObjectsEle23Mu8";
         alt_leg1_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
         alt_leg2_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8";
        } else if (era_ == era::data_2017 || era_ == era::data_2018){ 
            trig_obj_label = "triggerObjectsMu23Ele12DZ";
            leg1_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
            leg2_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23";
            extra_filter = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLDZFilter";
            alt_trig_obj_label = "triggerObjectsMu8Ele23DZ";
            alt_leg1_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
            alt_leg2_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8";
            alt_leg2_filter_2 = "hltMuon8Ele23RelTrkIsoFiltered0p4MuonLeg";
            alt_extra_filter = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLDZFilter";
        }
      } else if (channel_ == channel::tt){
       if (era_ == era::data_2016){
         trig_obj_label = "triggerObjectsDoubleMediumTau35";
         alt_trig_obj_label = "triggerObjectsDoubleMediumCombinedIsoTau35Reg";
         if(is_embedded_){ 
           alt_leg1_filter = "hltDoublePFTau35Reg";
           alt_leg2_filter = "hltDoublePFTau35Reg";
           leg1_filter = "hltDoublePFTau35Reg";
           leg2_filter = "hltDoublePFTau35Reg";
         } else {
           leg1_filter = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
           leg2_filter = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
           alt_leg1_filter = "hltDoublePFTau35TrackPt1MediumCombinedIsolationDz02Reg";
           alt_leg2_filter = "hltDoublePFTau35TrackPt1MediumCombinedIsolationDz02Reg";
         }
      } else if(era_ == era::data_2017){
          trig_obj_label = "triggerObjectsDoubleTightIsoTau35";
          leg1_filter = "hltDoublePFTau35TrackPt1TightChargedIsolationAndTightOOSCPhotonsDz02Reg";
          leg2_filter = "hltDoublePFTau35TrackPt1TightChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_trig_obj_label = "triggerObjectsDoubleMediumIsoTau40";
          alt_leg1_filter = "hltDoublePFTau40TrackPt1MediumChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_leg2_filter = "hltDoublePFTau40TrackPt1MediumChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_trig_obj_label_2 = "triggerObjectsDoubleTightIsoTau40";
          alt_leg1_filter_2 = "hltDoublePFTau40TrackPt1TightChargedIsolationDz02Reg";
          alt_leg2_filter_2 = "hltDoublePFTau40TrackPt1TightChargedIsolationDz02Reg";
          if(is_embedded_) {
             leg1_filter="hltDoubleL2IsoTau26eta2p2";
             leg2_filter=leg1_filter;
             alt_leg1_filter=leg1_filter;
             alt_leg2_filter=leg1_filter;
             alt_leg1_filter_2=leg1_filter;
             alt_leg2_filter_2=leg1_filter;
          }
        } else if (era_ == era::data_2018) {
            trig_obj_label = "triggerObjectsDoubleMediumIsoTauHPS35"; //HPS only
            leg1_filter = "hltHpsDoublePFTau35TrackPt1MediumChargedIsolationDz02Reg";
            leg2_filter = "hltHpsDoublePFTau35TrackPt1MediumChargedIsolationDz02Reg";
            alt_trig_obj_label_3 = "triggerObjectsVBFDoubleMediumChargedIsoPFTauHPS20";
            alt_leg1_filter_3 = "hltHpsDoublePFTau20TrackMediumChargedIsoAgainstMuon";
            alt_leg2_filter_3 = "hltHpsDoublePFTau20TrackMediumChargedIsoAgainstMuon";
            alt_jetleg1_filter = "hltMatchedVBFOnePFJet2CrossCleanedFromDoubleMediumChargedIsoPFTauHPS20"; // leading 
            //subleading from following (but contains leading as well)
            alt_jetleg2_filter = "hltMatchedVBFTwoPFJets2CrossCleanedFromDoubleMediumChargedIsoPFTauHPS20"; 
            if(is_embedded_) {
               leg1_filter="hltDoubleL2IsoTau26eta2p2";
               leg2_filter=leg1_filter;
               alt_leg1_filter=leg1_filter;
               alt_leg2_filter=leg1_filter;
               alt_leg1_filter_2=leg1_filter;
               alt_leg2_filter_2=leg1_filter;
            }
        }
      }
    }

    std::vector<CompositeCandidate *> & dileptons = event->GetPtrVec<CompositeCandidate>(pair_label_);
    std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trig_obj_label);

    std::vector<CompositeCandidate *> dileptons_pass;
    
    bool passed_singlemuon = false;
    bool passed_singleelectron = false;
    if ((channel_ == channel::et || channel_ == channel::mt || channel_ == channel::zmm || 
        channel_ == channel::zee || channel_ == channel::tpzee || channel_ == channel::tpzmm) ) {
      std::vector<TriggerObject *> alt_objs = event->GetPtrVec<TriggerObject>(alt_trig_obj_label);
      ic::erase_if_not(alt_objs,boost::bind(&TriggerObject::pt,_1)>alt_min_online_pt);
      for (unsigned i = 0; i < dileptons.size(); ++i) {
        bool leg1_match = false;
        bool highpt_leg = dileptons[i]->At(0)->pt()>high_leg_pt;
        leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_objs, alt_leg1_filter, 0.5).first;

        if((channel_==channel::et || channel_==channel::zee || channel_==channel::tpzee)&& era_ == era::data_2017) {
           std::vector<TriggerObject *> alt_objs_2 = event->GetPtrVec<TriggerObject>(alt_trig_obj_label_2);
           std::vector<TriggerObject *> l1_objs = event->GetPtrVec<TriggerObject>("triggerObjectsEle35"); // For the Ele32 trigger we need to make sure the electron is matched to the SingleElectron L1 filter since this path is also seeded by the L1 double electron triggers but this filter does not exist for the previous trigger objects so we much take it from an un-prescaled trigger
           leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_objs, alt_leg1_filter, 0.5).first || (IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_objs_2, alt_leg1_filter_2, 0.5).first&&IsFilterMatchedWithIndex(dileptons[i]->At(0), l1_objs, extra_filter_2,0.5).first);
          
        }
        if((channel_==channel::et || channel_==channel::zee || channel_==channel::tpzee)&& era_ == era::data_2018) {
           std::vector<TriggerObject *> alt_objs_2 = event->GetPtrVec<TriggerObject>(alt_trig_obj_label_2);
           leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_objs, alt_leg1_filter, 0.5).first || IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_objs_2, alt_leg1_filter_2, 0.5).first;

        }
        
        if((channel_==channel::mt || channel_==channel::zmm || channel_ == channel::tpzmm)&& era_ == era::data_2016 ) {
           std::vector<TriggerObject *> alt_trk_objs = event->GetPtrVec<TriggerObject>(alt_trk_trig_obj_label);
           ic::erase_if_not(alt_trk_objs,boost::bind(&TriggerObject::pt,_1)>alt_trk_min_online_pt);
           std::vector<TriggerObject *> alt_er_objs = event->GetPtrVec<TriggerObject>(alt_er_trig_obj_label);
           ic::erase_if_not(alt_er_objs,boost::bind(&TriggerObject::pt,_1)>alt_er_min_online_pt);             
           std::vector<TriggerObject *> alt_er_trk_objs = event->GetPtrVec<TriggerObject>(alt_er_trk_trig_obj_label);
           ic::erase_if_not(alt_er_trk_objs,boost::bind(&TriggerObject::pt,_1)>alt_er_trk_min_online_pt);

           leg1_match = (IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_objs, alt_leg1_filter, 0.5).first || IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_trk_objs, alt_trk_leg1_filter, 0.5).first) || (IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_er_objs, alt_er_leg1_filter, 0.5).first) || (IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_er_trk_objs, alt_er_trk_leg1_filter, 0.5).first); 
        } else if((channel_==channel::mt || channel_==channel::zmm || channel_ == channel::tpzmm) 
                && (era_ == era::data_2017 || era_ == era::data_2018)) {
           std::vector<TriggerObject *> alt_trk_objs = event->GetPtrVec<TriggerObject>(alt_trk_trig_obj_label);
           leg1_match = (IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_objs, alt_leg1_filter, 0.5).first || IsFilterMatchedWithIndex(dileptons[i]->At(0),alt_trk_objs, alt_trk_leg1_filter, 0.5).first);
        }
        
        if (leg1_match&&highpt_leg){
          dileptons_pass.push_back(dileptons[i]);
          if (channel_ == channel::et || channel_ == channel::zee) passed_singleelectron = true;
          if (channel_ == channel::mt || channel_ == channel::zmm) passed_singlemuon = true;
        }
      }
    }
    if(is_embedded_&& era_ == era::data_2017 && (channel_==channel::et || channel_==channel::zee)){
      // Electron triggers don't work properly for the embedded samples with e_eta>1.5 and pt<40 GeV so we allow all embedded events to pass these triggers and apply the efficiency measured for data as the SF in HTTWeights
      Electron const* elec = dynamic_cast<Electron const*>(dileptons[0]->At(0));
      double eta = fabs(elec->eta());
      double pt = elec->pt();
      if(pt<40 && eta>1.479) passed_singleelectron = true;
    }
    event->Add("trg_singlemuon", passed_singlemuon);
    event->Add("trg_singleelectron", passed_singleelectron);

    // mutau cross triggers
    bool passed_mutaucross = false;
    bool passed_mutaucross_alt = false;
    if (channel_ == channel::mt){
      std::vector<TriggerObject *> const& cross_objs_singlel1 = event->GetPtrVec<TriggerObject>(trig_obj_label);  
      for(unsigned i = 0; i < dileptons.size(); ++i){  
        bool leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), cross_objs_singlel1, leg1_filter, 0.5).first;
        if(extra_leg2_filter!="") leg1_match = leg1_match&&IsFilterMatchedWithIndex(dileptons[i]->At(0), cross_objs_singlel1, extra_leg2_filter,0.5).first;
        bool leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), cross_objs_singlel1, leg2_filter, 0.5).first;
        if(extra_leg2_filter!="") leg2_match = leg2_match&&IsFilterMatchedWithIndex(dileptons[i]->At(1), cross_objs_singlel1, extra_leg2_filter,0.5).first; 


        passed_mutaucross = leg1_match && leg2_match;
        if(alt_cross_trig_obj_label != ""){ 
          std::vector<TriggerObject *> const& cross_objs = event->GetPtrVec<TriggerObject>(alt_cross_trig_obj_label);
          bool alt_leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), cross_objs, alt_cross_leg1_filter, 0.5).first;
          if(alt_cross_extra_leg2_filter!="") alt_leg1_match = alt_leg1_match&&IsFilterMatchedWithIndex(dileptons[i]->At(0), cross_objs, alt_cross_extra_leg2_filter,0.5).first;
          bool alt_leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), cross_objs, alt_cross_leg2_filter, 0.5).first;
          if(alt_cross_extra_leg2_filter!="") alt_leg2_match = alt_leg2_match&&IsFilterMatchedWithIndex(dileptons[i]->At(1), cross_objs, alt_cross_extra_leg2_filter,0.5).first;  

          passed_mutaucross_alt = alt_leg1_match && alt_leg2_match; 
        }
        if(passed_mutaucross || passed_mutaucross_alt) dileptons_pass.push_back(dileptons[i]);
      }
    }
    event->Add("trg_mutaucross", passed_mutaucross || passed_mutaucross_alt);
    
    bool passed_etaucross = false;
    if (channel_ == channel::et){
      std::vector<TriggerObject *> const& cross_objs = event->GetPtrVec<TriggerObject>(trig_obj_label);  
      for(unsigned i = 0; i < dileptons.size(); ++i){  
        bool leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), cross_objs, leg1_filter, 0.5).first;
        if(extra_leg2_filter!="") leg1_match = leg1_match&&IsFilterMatchedWithIndex(dileptons[i]->At(0), cross_objs, extra_leg2_filter,0.5).first; 
        bool leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), cross_objs, leg2_filter, 0.5).first;
        if(extra_leg2_filter!="") leg2_match = leg2_match&&IsFilterMatchedWithIndex(dileptons[i]->At(1), cross_objs, extra_leg2_filter,0.5).first;

        passed_etaucross = leg1_match && leg2_match;
        if(is_embedded_&&(era_ == era::data_2017||era_ == era::data_2018)) {
          //L1 cut
          std::vector<Candidate const *> match_tau;
          match_tau.push_back(dileptons[0]->At(1));
          std::vector<ic::L1TObject*> l1taus = event->GetPtrVec<ic::L1TObject>("L1Taus");
          std::vector<ic::L1TObject*> passed_l1_taus_etau;
          for(unsigned ta=0; ta<l1taus.size(); ++ta){
              if(l1taus[ta]->isolation()!=0 && l1taus[ta]->vector().Pt() >= 26.) passed_l1_taus_etau.push_back(l1taus[ta]);
          }
          bool match_l1_part_etau = (MatchByDR(match_tau,passed_l1_taus_etau,0.5,true,true)).size() == 1;
          //if(passed_etaucross && !match_l1_part_etau) std::cout << "L1 not passed!" << std::endl;
          //else if(passed_etaucross) std::cout << "L1 passed!" << std::endl;
          passed_etaucross = passed_etaucross && match_l1_part_etau;
        }
        if(is_embedded_&& era_ == era::data_2017){
          // Electron triggers don't work properly for the embedded samples with e_eta>1.5 and pt<40 GeV so we allow all embedded events to pass these triggers and apply the efficiency measured for data as the SF in HTTWeights
          Electron const* elec = dynamic_cast<Electron const*>(dileptons[i]->At(0)); 
          double eta = fabs(elec->eta());
          double pt = elec->pt();
          if(pt<40 && eta>1.479) { passed_etaucross = true; }
        }
      if(passed_etaucross) dileptons_pass.push_back(dileptons[i]);
      }
    }
    event->Add("trg_etaucross", passed_etaucross);

   
   bool passed_muonelectron = false;
   if (channel_ == channel::em && era_ == era::data_2016){
      std::vector<TriggerObject *> const& alt_objs = event->GetPtrVec<TriggerObject>(alt_trig_obj_label);
      for(unsigned i = 0; i < dileptons.size(); ++i){
        bool leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, leg1_filter, 0.5).first;
        bool leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, leg2_filter, 0.5).first;
        bool extra_leg1_match=true;
        bool extra_leg2_match=true;
        if(extra_filter.find("hlt")!=extra_filter.npos){
          extra_leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, extra_filter, 0.5).first;
          extra_leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, extra_filter, 0.5).first;
        }
        bool highpt_leg = dileptons[i]->At(1)->pt() >24.0;
        if (leg1_match && leg2_match &&extra_leg1_match && extra_leg2_match && highpt_leg) {
          passed_muonelectron = true;  
          dileptons_pass.push_back(dileptons[i]);
        } 
        leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), alt_objs, alt_leg1_filter, 0.5).first;
        leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), alt_objs, alt_leg2_filter, 0.5).first;
        extra_leg1_match = true;
        extra_leg2_match = true;
        if(alt_extra_filter.find("hlt")!=extra_filter.npos){
          extra_leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), alt_objs, alt_extra_filter, 0.5).first;
          extra_leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), alt_objs, alt_extra_filter, 0.5).first;
        }
        highpt_leg = dileptons[i]->At(0)->pt() >24.0;
        if (leg1_match && leg2_match &&extra_leg1_match && extra_leg2_match && highpt_leg){
           passed_muonelectron = true; 
           dileptons_pass.push_back(dileptons[i]);
        }
      }
    }
   bool passed_dz = true;
   if (channel_ == channel::em && (era_ == era::data_2017 || era_ == era::data_2018)){
      std::vector<TriggerObject *> const& alt_objs = event->GetPtrVec<TriggerObject>(alt_trig_obj_label);
      // Mu23_Ele12_DZ matching
      for(unsigned i = 0; i < dileptons.size(); ++i){
        bool leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, leg1_filter, 0.5).first;
        bool leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, leg2_filter, 0.5).first;
        bool extra_leg1_match=true;
        bool extra_leg2_match=true;
        if(extra_filter!=""){
          extra_leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, extra_filter, 0.5).first;
          extra_leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, extra_filter, 0.5).first;
        }
        passed_dz = (extra_leg1_match && extra_leg2_match);
        bool highpt_leg = dileptons[i]->At(1)->pt() > 24.0;
        if (leg1_match && leg2_match && extra_leg1_match && extra_leg2_match && highpt_leg) {
          dileptons_pass.push_back(dileptons[i]);
          passed_muonelectron = true;
        } 
        // Mu8_Ele23_DZ matching
        leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), alt_objs, alt_leg1_filter, 0.5).first;
        leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), alt_objs, alt_leg2_filter, 0.5).first;
        bool leg2_match_2 = IsFilterMatchedWithIndex(dileptons[i]->At(1), alt_objs, alt_leg2_filter_2, 0.5).first; 
        extra_leg1_match = true;
        extra_leg2_match = true;
        highpt_leg = dileptons[i]->At(0)->pt() > 24.0;
        if(alt_extra_filter!=""){
          extra_leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), alt_objs, alt_extra_filter, 0.5).first;
          extra_leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), alt_objs, alt_extra_filter, 0.5).first;
        }
        passed_dz = passed_dz || (extra_leg1_match && extra_leg2_match);
        if (leg1_match && (leg2_match || leg2_match_2) && extra_leg1_match && extra_leg2_match && highpt_leg){
          if(!passed_muonelectron) dileptons_pass.push_back(dileptons[i]);
          passed_muonelectron = true;
        }
      }
    }
    event->Add("trg_muonelectron",   passed_muonelectron);
   
   bool passed_doubletau = false;
   bool passed_vbfdoubletau = false;
   bool match_l1_parts = false;
   bool jetleg1_match = false;
   bool jetleg2_match = false;
   if (channel_ == channel::tt){
     for(unsigned i = 0; i < dileptons.size(); ++i){
       bool leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, leg1_filter, 0.5).first;
       bool leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, leg2_filter, 0.5).first;
       
       if (leg1_match && leg2_match){
         passed_doubletau = true;  
         dileptons_pass.push_back(dileptons[i]);
        } else if(alt_trig_obj_label!=""){
           std::vector<TriggerObject *> alt_objs = event->GetPtrVec<TriggerObject>(alt_trig_obj_label);  
           leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), alt_objs, alt_leg1_filter, 0.5).first;
           leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), alt_objs, alt_leg2_filter, 0.5).first;
           if (leg1_match && leg2_match){
             passed_doubletau = true;  
             dileptons_pass.push_back(dileptons[i]);
          }
        }
        if(!passed_doubletau && alt_trig_obj_label_2!=""){
            std::vector<TriggerObject *> alt_objs_2 = event->GetPtrVec<TriggerObject>(alt_trig_obj_label_2);
            leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), alt_objs_2, alt_leg1_filter_2, 0.5).first;
            leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), alt_objs_2, alt_leg2_filter_2, 0.5).first;
            if (leg1_match && leg2_match){
              passed_doubletau = true;  
              dileptons_pass.push_back(dileptons[i]);
            }
        }
        if(!passed_doubletau && alt_trig_obj_label_3!=""){
            std::vector<TriggerObject *> alt_objs_3 = event->GetPtrVec<TriggerObject>(alt_trig_obj_label_3);
            leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), alt_objs_3, alt_leg1_filter_3, 0.5).first;
            leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), alt_objs_3, alt_leg2_filter_3, 0.5).first;
            if (leg1_match && leg2_match){
              passed_vbfdoubletau = true;  
              dileptons_pass.push_back(dileptons[i]);
            }
        }
        
        if(era_ == era::data_2017){
          // For 2017 MC we have to match the taus to L1 iso taus with pT>32 GeV to fit with how the SFs were measured
              
          std::vector<ic::L1TObject*> l1taus = event->GetPtrVec<ic::L1TObject>("L1Taus");
          std::vector<ic::L1TObject*> passed_l1_taus;
          for(unsigned ta=0; ta<l1taus.size(); ++ta){
            if(l1taus[ta]->isolation()!=0 && l1taus[ta]->vector().Pt() >= 32.) passed_l1_taus.push_back(l1taus[ta]);  
          }
          std::vector<Candidate *> match_taus;
          match_taus.push_back(dileptons[i]->At(0));
          match_taus.push_back(dileptons[i]->At(1));
          match_l1_parts = (MatchByDR(match_taus,passed_l1_taus,0.5,true,true)).size() == 2;
          passed_doubletau = passed_doubletau && match_l1_parts;
        }
      }

      // 2018 vbf trg jet matching after double tau trg part
      if (era_ == era::data_2018 && passed_vbfdoubletau && alt_trig_obj_label_3 != "") {
        std::vector<TriggerObject *> alt_objs_3 = event->GetPtrVec<TriggerObject>(alt_trig_obj_label_3);  
        std::vector<PFJet *> jets = event->GetPtrVec<PFJet>("ak4PFJetsCHS");
        // first need to find jets with maximum mjj and cross clean from taus?
        if (jets.size() >= 2) {
          unsigned int i1 = 0;
          unsigned int j1 = 0;
          double mjj_max = 0;
          
          for (unsigned int ijet = 0; ijet < jets.size()-1; ++ijet){
            for (unsigned int jjet = ijet+1; jjet < jets.size(); ++jjet){
              double mjj_test = (jets[ijet]->vector()+jets[jjet]->vector()).M();

              if (mjj_test > mjj_max){
                mjj_max = mjj_test;
                i1 = ijet;
                j1 = jjet;
              }
            } 
          }

          // add VBF trig here, need to do jet matching
          jetleg1_match = IsFilterMatchedWithIndex(jets[i1], alt_objs_3, alt_jetleg1_filter, 0.5).first;
          jetleg2_match = IsFilterMatchedWithIndex(jets[j1], alt_objs_3, alt_jetleg2_filter, 0.5).first;
          if (jetleg1_match && jetleg2_match) passed_vbfdoubletau = true;
          else passed_vbfdoubletau = false;
        }
      }
    }
    event->Add("trg_doubletau", passed_doubletau);
    event->Add("trg_vbfdoubletau", passed_vbfdoubletau);
    
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
