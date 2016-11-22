#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTTriggerFilter2.h"
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
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"

struct filters {
  std::string label;
  std::string path;  
  std::string leg1_filter;
  std::string leg2_filter;
  std::string leg2_extra;
  bool singleLepton_;
  bool pass;
  double lep1_pt;
  double lep2_pt;
  double leg1_extraHLTPt;
  double leg2_extraHLTPt;
  double leg1_extraL1Pt;
  double leg2_extraL1Pt;
  std::string L1filtername;
}; 


namespace ic {

  HTTTriggerFilter2::HTTTriggerFilter2(std::string const& name) : ModuleBase(name), channel_(channel::zee), mc_(mc::fall15_76X), era_(era::data_2015) {
  }

  HTTTriggerFilter2::~HTTTriggerFilter2() {
    ;
  }

  int HTTTriggerFilter2::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTTriggerFilter2" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
    std::cout << boost::format(param_fmt()) % "channel"         % Channel2String(channel_);
    std::cout << boost::format(param_fmt()) % "mc"              % MC2String(mc_);
    std::cout << boost::format(param_fmt()) % "dilepton_label"  % pair_label_;
    std::cout << boost::format(param_fmt()) % "is_data"         % is_data_;

    
    totalEventsPassed = 0;
    notMatched = 0;
    return 0;
  }

  int HTTTriggerFilter2::Execute(TreeEvent *event) {
    std::vector<filters> leg_filters;
    
    if (channel_ == channel::et || channel_ == channel::zee || channel_ ==  channel::tpzee) {

        filters fil;
        
        fil.label         = "triggerObjectsEle23";
        fil.path          = "HLT_Ele23_WPLoose_Gsf_v";
        fil.leg1_filter   = "hltEle23WPLooseGsfTrackIsoFilter";
        fil.leg2_filter   = "";
        fil.leg2_extra    = "";
        fil.singleLepton_ = true;
        fil.lep1_pt = 24.;
        fil.lep2_pt = 0.;
        leg_filters.push_back(fil);
        //hltEGL1SingleEG40ORSingleIsoEG22erOrSingleIsoEG24erORSingleIsoEG24OrSingleIsoEG26Filter
        
        fil.label         = "triggerObjectsEle24er";
        fil.path          = "HLT_Ele24_eta2p1_WPLoose_Gsf_v";
        fil.leg1_filter   = "hltSingleEle24WPLooseGsfTrackIsoFilter";
        fil.leg2_filter   = "";
        fil.leg2_extra    = "";
        fil.singleLepton_ = true;
        fil.lep1_pt = 25.;
        fil.lep2_pt = 0.;
        leg_filters.push_back(fil);
        
        fil.label         = "triggerObjectsEle25WPTight";
        fil.path          = "HLT_Ele25_WPTight_Gsf_v";
        fil.leg1_filter   = "hltEle25WPTightGsfTrackIsoFilter";
        fil.leg2_filter   = "";
        fil.leg2_extra    = "";
        fil.singleLepton_ = true;
        fil.lep1_pt = 26.;
        fil.lep2_pt = 0.;
        leg_filters.push_back(fil);
        
        fil.label         = "triggerObjectsEle25er";
        fil.path          = "HLT_Ele25_eta2p1_WPLoose_Gsf_v";
        fil.leg1_filter   = "hltEle25erWPLooseGsfTrackIsoFilter";
        fil.leg2_filter   = "";
        fil.leg2_extra    = "";
        fil.singleLepton_ = true;
        fil.lep1_pt = 26.;
        fil.lep2_pt = 0.;
        leg_filters.push_back(fil);
        
        fil.label         = "triggerObjectsEle25erWPTight";
        fil.path          = "HLT_Ele25_eta2p1_WPTight_Gsf_v";
        fil.leg1_filter   = "hltEle25erWPTightGsfTrackIsoFilter";
        fil.leg2_filter   = "";
        fil.leg2_extra    = "";
        fil.singleLepton_ = true;
        fil.lep1_pt = 26.;
        fil.lep2_pt = 0.;
        leg_filters.push_back(fil);
        
        fil.label         = "triggerObjectsEle27";
        fil.path          = "HLT_Ele27_WPLoose_Gsf_v";
        fil.leg1_filter   = "hltEle27noerWPLooseGsfTrackIsoFilter";
        fil.leg2_filter   = "";
        fil.leg2_extra    = "";
        fil.singleLepton_ = true;
        fil.lep1_pt = 28.;
        fil.lep2_pt = 0.;
        leg_filters.push_back(fil);
        
        fil.label         = "triggerObjectsEle27WPTight";
        fil.path          = "HLT_Ele27_WPTight_Gsf_v";
        fil.leg1_filter   = "hltEle27WPTightGsfTrackIsoFilter";
        fil.leg2_filter   = "";
        fil.leg2_extra    = "";
        fil.singleLepton_ = true;
        fil.lep1_pt = 28.;
        fil.lep2_pt = 0.;
        leg_filters.push_back(fil);
        
        fil.label         = "triggerObjectsEle27er";
        fil.path          = "HLT_Ele27_eta2p1_WPLoose_Gsf_v";
        fil.leg1_filter   = "hltEle27erWPLooseGsfTrackIsoFilter";
        fil.leg2_filter   = "";
        fil.leg2_extra    = "";
        fil.singleLepton_ = true;
        fil.lep1_pt = 28.;
        fil.lep2_pt = 0.;
        leg_filters.push_back(fil);
        
        fil.label         = "triggerObjectsEle27erWPTight";
        fil.path          = "HLT_Ele27_eta2p1_WPTight_Gsf_v";
        fil.leg1_filter   = "hltEle27erWPTightGsfTrackIsoFilter";
        fil.leg2_filter   = "";
        fil.leg2_extra    = "";
        fil.singleLepton_ = true;
        fil.lep1_pt = 28.;
        fil.lep2_pt = 0.;
        leg_filters.push_back(fil);
        
        fil.label         = "triggerObjectsEle32WPTight";
        fil.path          = "HLT_Ele32_eta2p1_WPTight_Gsf_v";
        fil.leg1_filter   = "hltEle32WPTightGsfTrackIsoFilter";
        fil.leg2_filter   = "";
        fil.leg2_extra    = "";
        fil.singleLepton_ = true;
        fil.lep1_pt = 33.;
        fil.lep2_pt = 0.;
        leg_filters.push_back(fil);
        
        fil.label         = "triggerObjectsEle22LooseTau20SingleL1";
        fil.path          = "HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v";
        fil.leg1_filter   = "hltEle22WPLooseL1SingleIsoEG20erGsfTrackIsoFilter";
        fil.leg2_filter   = "hltPFTau20TrackLooseIso";
        fil.leg2_extra    = "hltOverlapFilterSingleIsoEle22WPLooseGsfLooseIsoPFTau20";
        fil.singleLepton_ = false;
        fil.lep1_pt = 23.;
        fil.lep2_pt = 20.;
        leg_filters.push_back(fil);
        
        fil.label         = "triggerObjectsEle24LooseTau20SingleL1";
        fil.path          = "HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v";
        fil.leg1_filter   = "hltEle24WPLooseL1SingleIsoEG22erGsfTrackIsoFilter";
        fil.leg2_filter   = "hltPFTau20TrackLooseIso";
        fil.leg2_extra    = "hltOverlapFilterSingleIsoEle24WPLooseGsfLooseIsoPFTau20";
        fil.singleLepton_ = false;
        fil.lep1_pt = 25.;
        fil.lep2_pt = 20.;
        leg_filters.push_back(fil);
        
        fil.label         = "triggerObjectsEle24LooseTau20";
        fil.path          = "HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_v";
        fil.leg1_filter   = "hltEle24WPLooseL1IsoEG22erTau20erGsfTrackIsoFilter";
        fil.leg2_filter   = "hltPFTau20TrackLooseIso";
        fil.leg2_extra    = "hltOverlapFilterIsoEle24WPLooseGsfLooseIsoPFTau20";
        fil.singleLepton_ = false;
        fil.lep1_pt = 25.;
        fil.lep2_pt = 20.;
        leg_filters.push_back(fil);
        
        fil.label         = "triggerObjectsEle27LooseTau20SingleL1";
        fil.path          = "HLT_Ele27_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v";
        fil.leg1_filter   = "hltEle27erWPLooseGsfTrackIsoFilter";
        fil.leg2_filter   = "hltPFTau20TrackLooseIso";
        fil.leg2_extra    = "hltOverlapFilterIsoEle27WPLooseGsfLooseIsoPFTau20";
        fil.singleLepton_ = false;
        fil.lep1_pt = 28.;
        fil.lep2_pt = 20.;
        leg_filters.push_back(fil);
        
        fil.label         = "triggerObjectsEle32LooseTau20SingleL1";
        fil.path          = "HLT_Ele32_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v";
        fil.leg1_filter   = "hltEle32WPLooseGsfTrackIsoFilter";
        fil.leg2_filter   = "hltPFTau20TrackLooseIso";
        fil.leg2_extra    = "hltOverlapFilterIsoEle32WPLooseGsfLooseIsoPFTau20";
        fil.singleLepton_ = false;
        fil.lep1_pt = 33.;
        fil.lep2_pt = 20.;
        leg_filters.push_back(fil);
        
        fil.label         = "triggerObjectsEle24LooseTau20";
        fil.path          = "HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau30_v";
        fil.leg1_filter   = "hltEle24WPLooseL1IsoEG22erTau20erGsfTrackIsoFilter";
        fil.leg2_filter   = "hltPFTau20TrackLooseIso";
        fil.leg2_extra    = "hltOverlapFilterIsoEle24WPLooseGsfLooseIsoPFTau20";
        fil.singleLepton_ = false;
        fil.lep1_pt = 25.;
        fil.lep2_pt = 20.;
        fil.leg1_extraHLTPt = 0;
        fil.leg2_extraHLTPt = 30;
        fil.leg1_extraL1Pt = 22.;
        fil.leg2_extraL1Pt = 26.;
        fil.L1filtername = "hltL1sIsoEG22erTau20erdEtaMin0p2";
        leg_filters.push_back(fil);
      
        
        fil.label         = "triggerObjectsEle32WPTight";
        fil.path          = "HLT_Ele32_eta2p1_WPTight_Gsf_v_1pt45e34";
        fil.leg1_filter   = "hltEle32WPTightGsfTrackIsoFilter";
        fil.leg2_filter   = "";
        fil.leg2_extra    = "";
        fil.singleLepton_ = true;
        fil.lep1_pt = 33.;
        fil.lep2_pt = 0.;
        fil.leg1_extraHLTPt = 0.;
        fil.leg2_extraHLTPt = 0.;
        fil.leg1_extraL1Pt = 30.;
        fil.leg2_extraL1Pt = 0.;
        fil.L1filtername = "hltL1sSingleEG40IorSingleIsoEG22erIorSingleIsoEG24er";
        leg_filters.push_back(fil);

    } else if (channel_ == channel::mt || channel_ == channel::zmm || channel_ == channel::tpzmm) {
    
      filters fil;
      
      fil.label         = "triggerObjectsIsoMu18";
      fil.path          = "HLT_IsoMu18_v";
      fil.leg1_filter   = "hltL3crIsoL1sMu16L1f0L2f10QL3f18QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 19.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoMu20";
      fil.path          = "HLT_IsoMu20_v";
      fil.leg1_filter   = "hltL3crIsoL1sMu18L1f0L2f10QL3f20QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 21.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoMu22";
      fil.path          = "HLT_IsoMu22_v";
      fil.leg1_filter   = "hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 23.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoMu22er";
      fil.path          = "HLT_IsoMu22_eta2p1_v";
      fil.leg1_filter   = "hltL3crIsoL1sSingleMu20erL1f0L2f10QL3f22QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 23.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoMu27";
      fil.path          = "HLT_IsoMu27_v";
      fil.leg1_filter   = "hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 28.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoTkMu18";
      fil.path          = "HLT_IsoTkMu18_v";
      fil.leg1_filter   = "hltL3fL1sMu16L1f0Tkf18QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 19.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoTkMu20";
      fil.path          = "HLT_IsoTkMu20_v";
      fil.leg1_filter   = "hltL3fL1sMu18L1f0Tkf20QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 21.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoTkMu22er";
      fil.path          = "HLT_IsoTkMu22_eta2p1_v";
      fil.leg1_filter   = "hltL3fL1sMu20erL1f0Tkf22QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 21.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoTkMu22";
      fil.path          = "HLT_IsoTkMu22_v";
      fil.leg1_filter   = "hltL3fL1sMu20L1f0Tkf22QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 23.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoTkMu24";
      fil.path          = "HLT_IsoTkMu24_v";
      fil.leg1_filter   = "hltL3fL1sMu22L1f0Tkf24QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 25.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoTkMu27";
      fil.path          = "HLT_IsoTkMu27_v";
      fil.leg1_filter   = "hltL3fL1sMu22Or25L1f0Tkf27QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 28.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoMu17erLooseIsoTau20_SingleL1";
      fil.path          = "HLT_IsoMu17_eta2p1_LooseIsoPFTau20_SingleL1_v";
      fil.leg1_filter   = "hltL3crIsoL1sSingleMu16erL1f0L2f10QL3f17QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "hltPFTau20TrackLooseIsoAgainstMuon";
      fil.leg2_extra    = "hltOverlapFilterSingleIsoMu17LooseIsoPFTau20";
      fil.singleLepton_ = false;
      fil.lep1_pt = 18.;
      fil.lep2_pt = 20.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoMu17erLooseIsoTau20";
      fil.path          = "HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v";
      fil.leg1_filter   = "hltL3crIsoL1sMu16erTauJet20erL1f0L2f10QL3f17QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "hltPFTau20TrackLooseIsoAgainstMuon";
      fil.leg2_extra    = "hltOverlapFilterIsoMu17LooseIsoPFTau20";
      fil.singleLepton_ = false;
      fil.lep1_pt = 18.;
      fil.lep2_pt = 20.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoMu19erLooseIsoTau20_SingleL1";
      fil.path          = "HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v";
      fil.leg1_filter   = "hltL3crIsoL1sSingleMu18erIorSingleMu20erL1f0L2f10QL3f19QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "hltPFTau20TrackLooseIsoAgainstMuon";
      fil.leg2_extra    = "hltOverlapFilterSingleIsoMu19LooseIsoPFTau20";
      fil.singleLepton_ = false;
      fil.lep1_pt = 20.;
      fil.lep2_pt = 20.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoMu19erLooseIsoTau20";
      fil.path          = "HLT_IsoMu19_eta2p1_LooseIsoPFTau20_v";
      fil.leg1_filter   = "hltL3crIsoL1sMu18erTauJet20erL1f0L2f10QL3f19QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "hltPFTau20TrackLooseIsoAgainstMuon";
      fil.leg2_extra    = "hltOverlapFilterIsoMu19LooseIsoPFTau20";
      fil.singleLepton_ = false;
      fil.lep1_pt = 20.;
      fil.lep2_pt = 20.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoMu21erLooseIsoTau20_SingleL1";
      fil.path          = "HLT_IsoMu21_eta2p1_LooseIsoPFTau20_SingleL1_v";
      fil.leg1_filter   = "hltL3crIsoL1sSingleMu20erIorSingleMu22erL1f0L2f10QL3f21QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "hltPFTau20TrackLooseIsoAgainstMuon";
      fil.leg2_extra    = "hltOverlapFilterSingleIsoMu21LooseIsoPFTau20";
      fil.singleLepton_ = false;
      fil.lep1_pt = 22.;
      fil.lep2_pt = 20.;
      leg_filters.push_back(fil);

    } else if (channel_ == channel::em) {
    
      filters fil;
      
      fil.label         = "triggerObjectsEle23";
      fil.path          = "HLT_Ele23_WPLoose_Gsf_v";
      fil.leg1_filter   = "hltEle23WPLooseGsfTrackIsoFilter";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 24.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsEle24er";
      fil.path          = "HLT_Ele24_eta2p1_WPLoose_Gsf_v";
      fil.leg1_filter   = "hltSingleEle24WPLooseGsfTrackIsoFilter";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 25.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsEle25WPTight";
      fil.path          = "HLT_Ele25_WPTight_Gsf_v";
      fil.leg1_filter   = "hltEle25WPTightGsfTrackIsoFilter";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 26.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsEle25er";
      fil.path          = "HLT_Ele25_eta2p1_WPLoose_Gsf_v";
      fil.leg1_filter   = "hltEle25erWPLooseGsfTrackIsoFilter";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 26.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsEle25erWPTight";
      fil.path          = "HLT_Ele25_eta2p1_WPTight_Gsf_v";
      fil.leg1_filter   = "hltEle25erWPTightGsfTrackIsoFilter";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 26.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsEle27";
      fil.path          = "HLT_Ele27_WPLoose_Gsf_v";
      fil.leg1_filter   = "hltEle27noerWPLooseGsfTrackIsoFilter";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 28.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsEle27WPTight";
      fil.path          = "HLT_Ele27_WPTight_Gsf_v";
      fil.leg1_filter   = "hltEle27WPTightGsfTrackIsoFilter";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 28.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsEle27er";
      fil.path          = "HLT_Ele27_eta2p1_WPLoose_Gsf_v";
      fil.leg1_filter   = "hltEle27erWPLooseGsfTrackIsoFilter";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 28.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsEle27erWPTight";
      fil.path          = "HLT_Ele27_eta2p1_WPTight_Gsf_v";
      fil.leg1_filter   = "hltEle27erWPTightGsfTrackIsoFilter";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 28.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsEle32WPTight";
      fil.path          = "HLT_Ele32_eta2p1_WPTight_Gsf_v";
      fil.leg1_filter   = "hltEle32WPTightGsfTrackIsoFilter";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 33.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoMu18";
      fil.path          = "HLT_IsoMu18_v";
      fil.leg1_filter   = "hltL3crIsoL1sMu16L1f0L2f10QL3f18QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 19.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoMu20";
      fil.path          = "HLT_IsoMu20_v";
      fil.leg1_filter   = "hltL3crIsoL1sMu18L1f0L2f10QL3f20QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 21.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoMu22";
      fil.path          = "HLT_IsoMu22_v";
      fil.leg1_filter   = "hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 23.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoMu22er";
      fil.path          = "HLT_IsoMu22_eta2p1_v";
      fil.leg1_filter   = "hltL3crIsoL1sSingleMu20erL1f0L2f10QL3f22QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 23.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoMu27";
      fil.path          = "HLT_IsoMu27_v";
      fil.leg1_filter   = "hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 28.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoTkMu18";
      fil.path          = "HLT_IsoTkMu18_v";
      fil.leg1_filter   = "hltL3fL1sMu16L1f0Tkf18QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 19.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoTkMu20";
      fil.path          = "HLT_IsoTkMu20_v";
      fil.leg1_filter   = "hltL3fL1sMu18L1f0Tkf20QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 21.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoTkMu22er";
      fil.path          = "HLT_IsoTkMu22_eta2p1_v";
      fil.leg1_filter   = "hltL3fL1sMu20erL1f0Tkf22QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 21.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoTkMu22";
      fil.path          = "HLT_IsoTkMu22_v";
      fil.leg1_filter   = "hltL3fL1sMu20L1f0Tkf22QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 23.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoTkMu24";
      fil.path          = "HLT_IsoTkMu24_v";
      fil.leg1_filter   = "hltL3fL1sMu22L1f0Tkf24QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 25.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsIsoTkMu27";
      fil.path          = "HLT_IsoTkMu27_v";
      fil.leg1_filter   = "hltL3fL1sMu22Or25L1f0Tkf27QL3trkIsoFiltered0p09";
      fil.leg2_filter   = "";
      fil.leg2_extra    = "";
      fil.singleLepton_ = true;
      fil.lep1_pt = 28.;
      fil.lep2_pt = 0.;
      leg_filters.push_back(fil);
      
      
      fil.label         = "triggerObjectsMu8Ele17";
      fil.path          = "HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v";
      fil.leg1_filter   = "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8";
      fil.leg2_filter   = "hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
      fil.leg2_extra    = "";
      fil.singleLepton_ = false;
      fil.lep1_pt = 18.;
      fil.lep2_pt = 9.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsMu8Ele23";
      fil.path          = "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v";
      fil.leg1_filter   = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8";
      fil.leg2_filter   = "hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
      fil.leg2_extra    = "";
      fil.singleLepton_ = false;
      fil.lep1_pt = 24.;
      fil.lep2_pt = 9.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsMu17Ele12";
      fil.path          = "HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v";
      fil.leg1_filter   = "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered17";
      fil.leg2_filter   = "hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
      fil.leg2_extra    = "";
      fil.singleLepton_ = false;
      fil.lep1_pt = 13.;
      fil.lep2_pt = 18.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsMu23Ele12";
      fil.path          = "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v";
      fil.leg1_filter   = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23";
      fil.leg2_filter   = "hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
      fil.leg2_extra    = "";
      fil.singleLepton_ = false;
      fil.lep1_pt = 13.;
      fil.lep2_pt = 24.;
      leg_filters.push_back(fil);
      
      fil.label         = "triggerObjectsMu23Ele8";
      fil.path          = "HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v";
      fil.leg1_filter   = "hltMu23TrkIsoVVLEle8CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23";
      fil.leg2_filter   = "hltMu23TrkIsoVVLEle8CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter";
      fil.leg2_extra    = "";
      fil.singleLepton_ = false;
      fil.lep1_pt = 9.;
      fil.lep2_pt = 24.;
      leg_filters.push_back(fil);
    } else if (channel_ == channel::tt){
    
       filters fil;
       
       fil.label         = "triggerObjectsDoubleMediumTau32";
       fil.path          = "HLT_DoubleMediumIsoPFTau32_Trk1_eta2p1_Reg_v";
       fil.leg1_filter   = "hltDoublePFTau32TrackPt1MediumIsolationDz02Reg";
       fil.leg2_filter   = "hltDoublePFTau32TrackPt1MediumIsolationDz02Reg";
       fil.leg2_extra    = "hltDoublePFTau32TrackPt1MediumIsolationDz02Reg";
       fil.singleLepton_ = false;
       fil.lep1_pt = 0.;
       fil.lep2_pt = 0.;
       leg_filters.push_back(fil);
       
       fil.label         = "triggerObjectsDoubleMediumTau35";
       fil.path          = "HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg_v";
       fil.leg1_filter   = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
       fil.leg2_filter   = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
       fil.leg2_extra    = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
       fil.singleLepton_ = false;
       fil.lep1_pt = 0.;
       fil.lep2_pt = 0.;
       fil.L1filtername  = "hltL1sDoubleIsoTau26erIorDoubleIsoTau27erIorDoubleIsoTau28erIorDoubleIsoTau29erIorDoubleIsoTau30erIorDoubleIsoTau32er";
       leg_filters.push_back(fil);
       
       fil.label         = "triggerObjectsDoubleMediumTau40";
       fil.path          = "HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v";
       fil.leg1_filter   = "hltDoublePFTau40TrackPt1MediumIsolationDz02Reg";
       fil.leg2_filter   = "hltDoublePFTau40TrackPt1MediumIsolationDz02Reg";
       fil.leg2_extra    = "hltDoublePFTau40TrackPt1MediumIsolationDz02Reg";
       fil.singleLepton_ = false;
       fil.lep1_pt = 0.;
       fil.lep2_pt = 0.;
       leg_filters.push_back(fil);
    }
    
    std::vector<CompositeCandidate *> & dileptons = event->GetPtrVec<CompositeCandidate>(pair_label_);
    std::vector<CompositeCandidate *> dileptons_pass;
    
    if ((channel_ == channel::et || channel_ == channel::mt || channel_ == channel::em)) {
      for(unsigned i=0; i<leg_filters.size(); ++i){
        std::vector<CompositeCandidate *> dileptons_pass_reHLT;
        CompositeCandidate dilepton;
        std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(leg_filters[i].label);
        //bool PassTrigger = false;
        for (unsigned j=0; j < dileptons.size(); ++j) {
          bool leg1_match = false;
          bool leg2_match = false;

          if(leg_filters[i].singleLepton_){
            bool highpt_leg = false;
            if(channel_ == channel::em && leg_filters[i].leg1_filter == "") {
              highpt_leg = dileptons[j]->At(1)->pt()>leg_filters[i].lep1_pt;
              leg1_match = IsFilterMatchedWithIndex(dileptons[j]->At(1),objs, leg_filters[i].leg2_filter, 0.5).first;
            } else {
              highpt_leg = dileptons[j]->At(0)->pt()>leg_filters[i].lep1_pt;
              leg1_match = IsFilterMatchedWithIndex(dileptons[j]->At(0),objs, leg_filters[i].leg1_filter, 0.5).first;
            }

            
            unsigned leg1_match_index = IsFilterMatchedWithIndex(dileptons[j]->At(0),objs, leg_filters[i].leg1_filter, 0.5).second;
            bool applyAdditionalTriggerCuts_ = false;
            if(leg_filters[i].path == "HLT_Ele32_eta2p1_WPTight_Gsf_v_1pt45e34") applyAdditionalTriggerCuts_ = true; 
            if(applyAdditionalTriggerCuts_){
              
              if(leg_filters[i].leg1_extraHLTPt > 0 && leg1_match){
                if(objs[leg1_match_index]->pt() < leg_filters[i].leg1_extraHLTPt) leg1_match = false;
              }
              
              bool noEG = true;
              std::vector<unsigned> leg1_L1match_index = IsFilterMatchedWithMultipleIndexs(dileptons[j]->At(0), objs, leg_filters[i].L1filtername , 0.5).second; 
              for(unsigned y=0; y<leg1_L1match_index.size(); ++y){
                std::set<int16_t> lep1triggerTypes = GetTriggerTypes(objs[leg1_L1match_index[y]]);
                if(lep1triggerTypes.find(-98) != lep1triggerTypes.end()){
                  if(objs[leg1_L1match_index[y]]->pt() >= leg_filters[i].leg1_extraL1Pt ){
                      noEG = false;
                  }
                }
              }
              if(noEG) leg1_match = false;
            }
            
            std::string leg1_match_name = leg_filters[i].path+"_leg1_match";
            std::string leg2_match_name = leg_filters[i].path+"_leg2_match";
            event->Add(leg1_match_name, leg1_match);
            event->Add(leg2_match_name, true);
            //delete these lines below if you want to apply offline cut at trigger level
            highpt_leg = true;
            
            if (leg1_match && highpt_leg){
              dileptons_pass_reHLT.push_back(dileptons[j]);
              break;
              //dileptons_pass.push_back(dileptons[j]);
            }
              
          } else if(!leg_filters[i].singleLepton_){
            bool highpt1_leg = dileptons[j]->At(0)->pt()>leg_filters[i].lep1_pt;
            bool highpt2_leg = dileptons[j]->At(1)->pt()>leg_filters[i].lep2_pt;
            //delete these lines below if you want to apply offline cut at trigger level
            highpt1_leg = true;
            highpt2_leg = true;
            if (channel_ == channel::em){
              leg1_match = IsFilterMatchedWithIndex(dileptons[j]->At(1), objs, leg_filters[i].leg1_filter, 0.5).first;
              leg2_match = IsFilterMatchedWithIndex(dileptons[j]->At(0), objs, leg_filters[i].leg2_filter, 0.5).first;
          
            } else if (channel_ == channel::et){
              leg1_match = IsFilterMatchedWithIndex(dileptons[j]->At(0), objs, leg_filters[i].leg1_filter, 0.5).first&&IsFilterMatchedWithIndex(dileptons[j]->At(0), objs, leg_filters[i].leg2_extra,0.5).first;
              leg2_match = IsFilterMatchedWithIndex(dileptons[j]->At(1), objs, leg_filters[i].leg2_filter, 0.5).first&&IsFilterMatchedWithIndex(dileptons[j]->At(1), objs, leg_filters[i].leg2_extra,0.5).first;
              
              unsigned leg1_match_index = IsFilterMatchedWithIndex(dileptons[j]->At(0), objs, leg_filters[i].leg1_filter, 0.5).second;
              unsigned leg2_match_index = IsFilterMatchedWithIndex(dileptons[j]->At(1), objs, leg_filters[i].leg2_filter, 0.5).second;
              bool applyAdditionalTriggerCuts_ = false;
              if(leg_filters[i].path == "HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau30_v") applyAdditionalTriggerCuts_ = true; 
              if(applyAdditionalTriggerCuts_){
                if(leg_filters[i].leg1_extraHLTPt > 0 && leg1_match){
                  if(objs[leg1_match_index]->pt() < leg_filters[i].leg1_extraHLTPt) leg1_match = false;
                }
                if(leg_filters[i].leg2_extraHLTPt > 0 && leg2_match){
                  if(objs[leg2_match_index]->pt() < leg_filters[i].leg2_extraHLTPt) leg2_match = false;
                }
                
                int L1EGIndex = -1;
                
                bool noEG = true;
                std::vector<unsigned> leg1_L1match_index = IsFilterMatchedWithMultipleIndexs(dileptons[j]->At(0), objs, leg_filters[i].L1filtername , 0.5).second; 
                for(unsigned y=0; y<leg1_L1match_index.size(); ++y){
                  std::set<int16_t> lep1triggerTypes = GetTriggerTypes(objs[leg1_L1match_index[y]]);
                  if(lep1triggerTypes.find(-98) != lep1triggerTypes.end()){
                    if(objs[leg1_L1match_index[y]]->pt() >= leg_filters[i].leg1_extraL1Pt ){
                      noEG = false;
                      L1EGIndex = y;
                    }
                  }
                }
                if(noEG) leg1_match = false;
                
                std::vector<ic::L1TObject*> l1taus = event->GetPtrVec<ic::L1TObject>("L1Taus");
                std::vector<ic::L1TObject*> l1electrons = event->GetPtrVec<ic::L1TObject>("L1EGammas");
                
                bool PassedL1 = false;
                
                std::vector<ic::L1TObject*> passed_l1_taus;
                
                for(unsigned ta=0; ta<l1taus.size(); ++ta){
                  if(std::fabs(l1taus[ta]->vector().Rapidity())<=2.1 && l1taus[ta]->isolation() !=0 && l1taus[ta]->vector().Pt() >= leg_filters[i].leg1_extraL1Pt) passed_l1_taus.push_back(l1taus[ta]);  
                }
                for(unsigned ta=0; ta<passed_l1_taus.size(); ++ta){
                  //if(IsFilterMatchedWithIndex(passed_l1_taus[ta], objs, leg_filters[i].leg2_filter, 0.5).first) PassedL1 = true;
                    std::vector<unsigned> indexes = IsFilterMatchedWithMultipleIndexs(passed_l1_taus[ta], objs, leg_filters[i].L1filtername, 0.5).second;
                    for(unsigned b=0; b< indexes.size(); ++b){
                      std::set<int16_t> lep1triggerTypes = GetTriggerTypes(objs[indexes[b]]);
                      if(lep1triggerTypes.find(-100) != lep1triggerTypes.end()){
                        if(L1EGIndex!=-1){
                        double dEta = std::fabs(objs[L1EGIndex]->eta() - passed_l1_taus[ta]->vector().Rapidity());
                        if(dEta>=0.2) PassedL1 = true;
                        }
                      }
                    }
                }
                
                if(!PassedL1){
                  leg2_match = false;
                  leg1_match = false;
                }
                
                double IsoEGPt = -1;
                double EGPt = -1;
                for(unsigned eg=0; eg<l1electrons.size(); ++eg){
                  if(std::fabs(l1electrons[eg]->vector().Rapidity()) < 2.1){
                     if(l1electrons[eg]->vector().Pt()>EGPt) EGPt = l1electrons[eg]->vector().Pt(); 
                     if(l1electrons[eg]->isolation() !=0 && l1electrons[eg]->vector().Pt()>IsoEGPt) IsoEGPt = l1electrons[eg]->vector().Pt();  
                  }
                }
                bool PassedIso30NonIso34 = false;
                if(IsoEGPt >= 30. || EGPt >=34) PassedIso30NonIso34 = true;
                bool PassedIso30NonIso40 = false;
                if(IsoEGPt >= 30. || EGPt >=40) PassedIso30NonIso40 = true;
                
                if(PassedIso30NonIso34) totalEventsPassed++;
                if(PassedIso30NonIso34 && !PassedIso30NonIso40) notMatched++;
              }

            }
            else if (channel_ == channel::mt) {
              leg1_match = IsFilterMatchedWithIndex(dileptons[j]->At(0), objs, leg_filters[i].leg1_filter, 0.5).first&&IsFilterMatchedWithIndex(dileptons[j]->At(0), objs, leg_filters[i].leg2_extra,0.5).first;
              leg2_match = IsFilterMatchedWithIndex(dileptons[j]->At(1), objs, leg_filters[i].leg2_filter, 0.5).first&&IsFilterMatchedWithIndex(dileptons[j]->At(1), objs, leg_filters[i].leg2_extra,0.5).first;
            }
            std::string leg1_match_name = leg_filters[i].path+"_leg1_match";
            std::string leg2_match_name = leg_filters[i].path+"_leg2_match";
            event->Add(leg1_match_name, IsFilterMatchedWithIndex(dileptons[j]->At(0), objs, leg_filters[i].leg1_filter, 0.5).first);
            event->Add(leg2_match_name, leg2_match);
            if (leg1_match && leg2_match && highpt1_leg && highpt2_leg){
              dileptons_pass_reHLT.push_back(dileptons[j]);
              break;
              //dileptons_pass.push_back(dileptons[j]);
            }
          }
          
        }
        
        if (dileptons_pass_reHLT.size() >= 1){
          leg_filters[i].pass = true;
        }
        else                                  leg_filters[i].pass = false;
        
        
      }
        
    }
    
    if (channel_ == channel::tt) {
      
      for(unsigned i=0; i<leg_filters.size(); ++i){
          
        std::vector<CompositeCandidate *> dileptons_pass_reHLT;
        std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(leg_filters[i].label);
        for (unsigned j=0; j < dileptons.size(); ++j) {
          bool leg1_match = false;
          bool leg2_match = false;
          leg1_match = IsFilterMatchedWithIndex(dileptons[j]->At(0), objs, leg_filters[i].leg1_filter, 0.5).first;
          leg2_match = IsFilterMatchedWithIndex(dileptons[j]->At(1), objs, leg_filters[i].leg2_filter, 0.5).first;
          std::string leg1_match_name = leg_filters[i].path+"_leg1_match";
          std::string leg2_match_name = leg_filters[i].path+"_leg2_match";
          event->Add(leg1_match_name, leg1_match);
          event->Add(leg2_match_name, leg2_match);
          if (leg1_match && leg2_match){
            dileptons_pass_reHLT.push_back(dileptons[j]);
          }
        }
          
        
        if (dileptons_pass_reHLT.size() >= 1) leg_filters[i].pass = true;
        else                                  leg_filters[i].pass = false;
        
      }
        
    }

    bool passedHLT = false;
    for(unsigned i=0; i<leg_filters.size(); ++i){
      event->Add(leg_filters[i].path, leg_filters[i].pass);
      if(leg_filters[i].pass){
          passedHLT = true;
      }
    }
    if (passedHLT) return 0;
    else           return 0;
}

  int HTTTriggerFilter2::PostAnalysis() {
    std::cout << notMatched << " / " << totalEventsPassed << std::endl;  
    return 0;
  }

  void HTTTriggerFilter2::PrintInfo() {
    ;
  }
}
