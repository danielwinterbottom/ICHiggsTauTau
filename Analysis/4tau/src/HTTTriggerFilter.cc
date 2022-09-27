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
    return 0;
  }

  int HTTTriggerFilter::Execute(TreeEvent *event) {

    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    unsigned run = eventInfo->run();


    bool passed_doubletau_12 = false;
    bool passed_doubletau_13 = false;
    bool passed_doubletau_14 = false;
    bool passed_doubletau_23 = false;
    bool passed_doubletau_24 = false;
    bool passed_doubletau_34 = false;
    bool passed_singlemuon_1 = false;
    bool passed_singlemuon_2 = false;
    bool passed_singleelectron_1 = false;
    bool passed_singleelectron_2 = false;
    bool passed_doublemuon_12 = false;
    bool passed_doubleelectron_12 = false;
    bool passed_mutaucross_12 = false;
    bool passed_mutaucross_13 = false;
    bool passed_mutaucross_14 = false;
    bool passed_mutaucross_23 = false;
    bool passed_mutaucross_24 = false;
    bool passed_etaucross_12 = false;
    bool passed_etaucross_13 = false;
    bool passed_etaucross_14 = false;
    bool passed_etaucross_23 = false;
    bool passed_etaucross_24 = false;
    bool passed_emucross_12 = false;

    std::vector<std::string> trg_obj_label_doubletau;
    std::vector<std::string> trg_obj_label_doubleelectron; 
    std::vector<std::string> trg_obj_label_doublemuon;
    std::vector<std::string> trg_obj_label_singleelectron;
    std::vector<std::string> trg_obj_label_singlemuon;
    std::vector<std::string> trg_obj_label_emucross;
    std::vector<std::string> trg_obj_label_etaucross;
    std::vector<std::string> trg_obj_label_mutaucross;

    std::vector<std::string> leg_doubletau_1;
    std::vector<std::string> leg_doubletau_2;
    std::vector<std::string> leg_doubleelectron_1;
    std::vector<std::string> leg_doubleelectron_2;
    std::vector<std::string> leg_doublemuon_1;
    std::vector<std::string> leg_doublemuon_2;
    std::vector<std::string> leg_singleelectron;
    std::vector<std::string> leg_singlemuon;
    std::vector<std::string> leg_emucross_1;
    std::vector<std::string> leg_emucross_2;
    std::vector<std::string> leg_emucross_dZ;
    std::vector<std::string> leg_etaucross_1;
    std::vector<std::string> leg_etaucross_2;
    std::vector<std::string> leg_etaucross_overlap;
    std::vector<std::string> leg_mutaucross_1;
    std::vector<std::string> leg_mutaucross_2;
    std::vector<std::string> leg_mutaucross_overlap;

    //doubletau
    if(era_ == era::data_2016 || era_ == era::data_2016UL_preVFP || era_ == era::data_2016UL_postVFP){
      trg_obj_label_doubletau = {"triggerObjectsDoubleMediumTau35","triggerObjectsDoubleMediumCombinedIsoTau35Reg"};
      leg_doubletau_1 = {"hltDoublePFTau35TrackPt1MediumIsolationDz02Reg","hltDoublePFTau35TrackPt1MediumCombinedIsolationDz02Reg"};
      leg_doubletau_2 = {"hltDoublePFTau35TrackPt1MediumIsolationDz02Reg","hltDoublePFTau35TrackPt1MediumCombinedIsolationDz02Reg"};
    } else if (era_ == era::data_2017 || era_ == era::data_2017UL) {
      trg_obj_label_doubletau = {"triggerObjectsDoubleTightIsoTau35","triggerObjectsDoubleMediumIsoTau40"};
      leg_doubletau_1 = {"hltDoublePFTau35TrackPt1TightChargedIsolationAndTightOOSCPhotonsDz02Reg","hltDoublePFTau40TrackPt1MediumChargedIsolationAndTightOOSCPhotonsDz02Reg"};
      leg_doubletau_2 = {"hltDoublePFTau35TrackPt1TightChargedIsolationAndTightOOSCPhotonsDz02Reg","hltDoublePFTau40TrackPt1MediumChargedIsolationAndTightOOSCPhotonsDz02Reg"};
    } else if (era_ == era::data_2018 || era_ == era::data_2018UL) {
      if(is_data_) {
        if(run >= 314472 && run < 317509){ // pre HPS
          trg_obj_label_doubletau = {"triggerObjectsDoubleTightIsoTau35","triggerObjectsDoubleMediumIsoTau40TightID"};
          leg_doubletau_1 = {"hltDoublePFTau35TrackPt1TightChargedIsolationAndTightOOSCPhotonsDz02Reg","hltDoublePFTau40TrackPt1MediumChargedIsolationAndTightOOSCPhotonsDz02Reg"};
          leg_doubletau_2 = {"hltDoublePFTau35TrackPt1TightChargedIsolationAndTightOOSCPhotonsDz02Reg","hltDoublePFTau40TrackPt1MediumChargedIsolationAndTightOOSCPhotonsDz02Reg"};
        } else if (run >= 317509) { // post HPS
          trg_obj_label_doubletau = {"triggerObjectsDoubleMediumIsoTauHPS35"};
          leg_doubletau_1 = {"hltHpsDoublePFTau35TrackPt1MediumChargedIsolationDz02Reg"};
          leg_doubletau_2 = {"hltHpsDoublePFTau35TrackPt1MediumChargedIsolationDz02Reg"};
        }
      } else {
          trg_obj_label_doubletau = {"triggerObjectsDoubleMediumIsoTauHPS35"};
          leg_doubletau_1 = {"hltHpsDoublePFTau35TrackPt1MediumChargedIsolationDz02Reg"};
          leg_doubletau_2 = {"hltHpsDoublePFTau35TrackPt1MediumChargedIsolationDz02Reg"};
      }
    }

    //doubleelectron
    if(era_ == era::data_2016 || era_ == era::data_2016UL_preVFP || era_ == era::data_2016UL_postVFP){
      trg_obj_label_doubleelectron = {"triggerObjectsEle23Ele12DZ"};
      leg_doubleelectron_1 = {""};
      leg_doubleelectron_2 = {""};
    } else if (era_ == era::data_2017 || era_ == era::data_2017UL) {
      trg_obj_label_doubleelectron = {"triggerObjectsEle24Ele12"};
      leg_doubleelectron_1 = {""};
      leg_doubleelectron_2 = {""};
    } else if (era_ == era::data_2018 || era_ == era::data_2018UL) {
      trg_obj_label_doubleelectron = {"triggerObjectsEle24Ele12"};
      leg_doubleelectron_1 = {""};
      leg_doubleelectron_2 = {""};
    }

    //doublemuon
    if(era_ == era::data_2016 || era_ == era::data_2016UL_preVFP || era_ == era::data_2016UL_postVFP){
      trg_obj_label_doublemuon = {"triggerObjectsMu17Mu8","triggerObjectsMu17Mu8DZ","triggerObjectsMu17TkMu8","triggerObjectsMu17TkMu8DZ","triggerObjectsTkMu17TkMu8","triggerObjectsTkMu17TkMu8DZ"};
      leg_doublemuon_1 = {""};
      leg_doublemuon_2 = {""};
    } else if (era_ == era::data_2017 || era_ == era::data_2017UL) {
      trg_obj_label_doublemuon = {"triggerObjectsMu17Mu8DZmass3p8","triggerObjectsMu17Mu8DZmass8"};
      leg_doublemuon_1 = {""};
      leg_doublemuon_2 = {""};
    } else if (era_ == era::data_2018 || era_ == era::data_2018UL) {
      trg_obj_label_doublemuon = {"triggerObjectsDoubleMu178"};
      leg_doublemuon_1 = {"hltDiMuon178RelTrkIsoFiltered0p4"};
      leg_doublemuon_2 = {"hltDiMuon178RelTrkIsoFiltered0p4"};
    }

    //singleelectron
    if(era_ == era::data_2016 || era_ == era::data_2016UL_preVFP || era_ == era::data_2016UL_postVFP){
      trg_obj_label_singleelectron = {"triggerObjectsEle25GsfTightEta2p1"};
      leg_singleelectron = {"hltEle25erWPTightGsfTrackIsoFilter"};
    } else if (era_ == era::data_2017 || era_ == era::data_2017UL) {
      trg_obj_label_singleelectron = {"triggerObjectsEle32L1DoubleEG","triggerObjectsEle35","triggerObjectsEle27"};
      leg_singleelectron = {"hltEle32L1DoubleEGWPTightGsfTrackIsoFilter","hltEle35noerWPTightGsfTrackIsoFilter","hltEle27WPTightGsfTrackIsoFilter"};
    } else if (era_ == era::data_2018 || era_ == era::data_2018UL) {
      trg_obj_label_singleelectron = {"triggerObjectsEle32","triggerObjectsEle35"};
      leg_singleelectron = {"hltEle32WPTightGsfTrackIsoFilter","hltEle35noerWPTightGsfTrackIsoFilter"};
    }

    //singlemuon
    if(era_ == era::data_2016 || era_ == era::data_2016UL_preVFP || era_ == era::data_2016UL_postVFP){
      trg_obj_label_singlemuon = {"triggerObjectsIsoMu22","triggerObjectsIsoTkMu22","triggerObjectsIsoMu22Eta2p1","triggerObjectsIsoTkMu22Eta2p1"};
      leg_singlemuon = {"hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09","hltL3fL1sMu20L1f0Tkf22QL3trkIsoFiltered0p09","hltL3crIsoL1sSingleMu20erL1f0L2f10QL3f22QL3trkIsoFiltered0p09","hltL3fL1sMu20erL1f0Tkf22QL3trkIsoFiltered0p09"};
    } else if (era_ == era::data_2017 || era_ == era::data_2017UL) {
      trg_obj_label_singlemuon = {"triggerObjectsIsoMu24","triggerObjectsIsoMu27"};
      leg_singlemuon = {"hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07","hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07"};
    } else if (era_ == era::data_2018 || era_ == era::data_2018UL) {
      trg_obj_label_singlemuon = {"triggerObjectsIsoMu24","triggerObjectsIsoMu27"};
      leg_singlemuon = {"hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07","hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07"};
    }

    //etaucross
    if(era_ == era::data_2016 || era_ == era::data_2016UL_preVFP || era_ == era::data_2016UL_postVFP){
      if(is_data_) {
        if(run < 276215) { 
          trg_obj_label_etaucross = {"triggerObjectsEle24LooseTau20SingleL1"};
          leg_etaucross_2 = {"hltEle24WPLooseL1SingleIsoEG22erGsfTrackIsoFilter"};
          leg_etaucross_1 = {"hltPFTau20TrackLooseIso"};
          leg_etaucross_overlap = {"hltOverlapFilterSingleIsoEle24WPLooseGsfLooseIsoPFTau20"};
        } else if (run >= 276215 && run < 278270) {
          trg_obj_label_etaucross = {"triggerObjectsEle24LooseTau20"};
          leg_etaucross_2 = {"hltEle24WPLooseL1IsoEG22erTau20erGsfTrackIsoFilter"};
          leg_etaucross_1 = {"hltPFTau20TrackLooseIso"};
          leg_etaucross_overlap = {"hltOverlapFilterIsoEle24WPLooseGsfLooseIsoPFTau20"};
        } else if (run >= 278270) {
          trg_obj_label_etaucross = {""}; // currently missing
          leg_etaucross_2 = {""};
          leg_etaucross_1 = {""};
          leg_etaucross_overlap = {""};
        }
      } else {
        trg_obj_label_etaucross = {"triggerObjectsEle24LooseTau20SingleL1"};
        leg_etaucross_2 = {"hltEle24WPLooseL1SingleIsoEG22erGsfTrackIsoFilter"};
        leg_etaucross_1 = {"hltPFTau20TrackLooseIso"};
        leg_etaucross_overlap = {"hltOverlapFilterSingleIsoEle24WPLooseGsfLooseIsoPFTau20"};
      }
    } else if (era_ == era::data_2017 || era_ == era::data_2017UL) {
      trg_obj_label_etaucross = {"triggerObjectsEle24Tau30"};
      leg_etaucross_2 = {"hltSelectedPFTau30LooseChargedIsolationL1HLTMatched"};
      leg_etaucross_1 = {"hltEle24erWPTightGsfTrackIsoFilterForTau"};
      leg_etaucross_overlap = {"hltOverlapFilterIsoEle24WPTightGsfLooseIsoPFTau30"};
    } else if (era_ == era::data_2018 || era_ == era::data_2018UL) {
      trg_obj_label_etaucross = {"triggerObjectsEle24TauHPS30"};
      leg_etaucross_2 = {"hltHpsSelectedPFTau30LooseChargedIsolationL1HLTMatched"};
      leg_etaucross_1 = {"hltEle24erWPTightGsfTrackIsoFilterForTau"};
      leg_etaucross_overlap = {"hltHpsOverlapFilterIsoEle24WPTightGsfLooseIsoPFTau30"};
    }

    //mutaucross
    if(era_ == era::data_2016 || era_ == era::data_2016UL_preVFP || era_ == era::data_2016UL_postVFP) {
      trg_obj_label_mutaucross = {"triggerObjectsIsoMu19LooseTau20SingleL1"};
      leg_mutaucross_2 = {"hltL3crIsoL1sSingleMu18erIorSingleMu20erL1f0L2f10QL3f19QL3trkIsoFiltered0p09"};
      leg_mutaucross_1 = {"hltPFTau20TrackLooseIsoAgainstMuon"};
      leg_mutaucross_overlap = {"hltOverlapFilterSingleIsoMu19LooseIsoPFTau20"};
    } else if (era_ == era::data_2017 || era_ == era::data_2017UL) {
      trg_obj_label_mutaucross = {"triggerObjectsIsoMu20Tau27"};
      leg_mutaucross_2 = {"hltSelectedPFTau27LooseChargedIsolationAgainstMuonL1HLTMatched"};
      leg_mutaucross_1 = {"hltL3crIsoL1sMu18erTau24erIorMu20erTau24erL1f0L2f10QL3f20QL3trkIsoFiltered0p07"};
      leg_mutaucross_overlap = {"hltOverlapFilterIsoMu20LooseChargedIsoPFTau27L1Seeded"};
    } else if (era_ == era::data_2018 || era_ == era::data_2018UL) {
      trg_obj_label_mutaucross = {"triggerObjectsIsoMu20TauHPS27"};
      leg_mutaucross_2 = {"hltHpsSelectedPFTau27LooseChargedIsolationAgainstMuonL1HLTMatched","hltHpsSelectedPFTau27LooseChargedIsolationAgainstMuonL1HLTMatched"};
      leg_mutaucross_1 = {"hltL3crIsoBigORMu18erTauXXer2p1L1f0L2f10QL3f20QL3trkIsoFiltered0p07","hltL3crIsoL1sMu18erTau24erIorMu20erTau24erL1f0L2f10QL3f20QL3trkIsoFiltered0p07"};
      leg_mutaucross_overlap = {"hltHpsOverlapFilterIsoMu20LooseChargedIsoPFTau27L1Seeded"};
    }

    //emucross
    if(era_ == era::data_2016 || era_ == era::data_2016UL_preVFP || era_ == era::data_2016UL_postVFP){
      trg_obj_label_emucross = {"triggerObjectsEle12Mu23","triggerObjectsEle23Mu8"};
      leg_emucross_1 = {"hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter","hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter"};
      leg_emucross_2 = {"hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23","hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8"};
      leg_emucross_dZ = {};
    } else if (era_ == era::data_2017 || era_ == era::data_2017UL) {
      trg_obj_label_emucross = {"triggerObjectsMu23Ele12DZ","triggerObjectsMu8Ele23DZ"};
      leg_emucross_1 = {"hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter","hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter"};
      leg_emucross_2 = {"hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23","hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8"};
      leg_emucross_dZ = {"hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLDZFilter","hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLDZFilter"};
    } else if (era_ == era::data_2018 || era_ == era::data_2018UL) {
      trg_obj_label_emucross = {"triggerObjectsMu23Ele12DZ","triggerObjectsMu8Ele23DZ"};
      leg_emucross_1 = {"hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter","hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter"};
      leg_emucross_2 = {"hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23","hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8"};
      leg_emucross_dZ = {"hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLDZFilter","hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLDZFilter"};
    }

    std::vector<CompositeCandidate *> & multileptons = event->GetPtrVec<CompositeCandidate>(pair_label_);
    std::vector<CompositeCandidate *> multileptons_pass;


    // doubletau trigger
    for (unsigned i = 0; i < trg_obj_label_doubletau.size(); ++i) {
      std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trg_obj_label_doubletau[i]);
      if (channel_ == channel::tttt) {
        passed_doubletau_12 =  passed_doubletau_12 || ((IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_doubletau_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_doubletau_2[i], 0.5).first));
        passed_doubletau_13 =  passed_doubletau_13 || ((IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_doubletau_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(2), objs, leg_doubletau_2[i], 0.5).first));
        passed_doubletau_14 =  passed_doubletau_14 || ((IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_doubletau_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(3), objs, leg_doubletau_2[i], 0.5).first));
      }
      if (channel_ == channel::tttt || channel_ == channel::ettt || channel_ == channel::mttt) {
        passed_doubletau_23 =  passed_doubletau_23 || ((IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_doubletau_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(2), objs, leg_doubletau_2[i], 0.5).first));
        passed_doubletau_24 =  passed_doubletau_24 || ((IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_doubletau_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(3), objs, leg_doubletau_2[i], 0.5).first));
      }
      if (channel_ == channel::tttt || channel_ == channel::ettt || channel_ == channel::mttt ||  channel_ == channel::eett || channel_ == channel::mmtt || channel_ == channel::emtt) {
        passed_doubletau_34 =  passed_doubletau_34 || ((IsFilterMatchedWithIndex(multileptons[0]->At(2), objs, leg_doubletau_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(3), objs, leg_doubletau_2[i], 0.5).first));
      }
    }

    ////doubleelectron
    //for (unsigned i = 0; i < trg_obj_label_doubleelectron.size(); ++i) {
    //  std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trg_obj_label_doubleelectron[i]);
    //  if (channel_ == channel::eett) {
    //    passed_doubleelectron_12 =  passed_doubleelectron_12 || ((IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_doubleelectron_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_doubleelectron_2[i], 0.5).first));
    //  }  
    //}

    ////doublemuon
    //for (unsigned i = 0; i < trg_obj_label_doublemuon.size(); ++i) { 
    //  std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trg_obj_label_doublemuon[i]);
    //  if (channel_ == channel::mmtt) {
    //    passed_doublemuon_12 =  passed_doublemuon_12 || ((IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_doublemuon_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_doublemuon_2[i], 0.5).first));
    //  }
    //}


    //singleelectron
    for (unsigned i = 0; i < trg_obj_label_singleelectron.size(); ++i) {
      std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trg_obj_label_singleelectron[i]);
      if (channel_ == channel::eett || channel_ == channel::ettt || channel_ == channel::emtt) {
        passed_singleelectron_1 =  passed_singleelectron_1 || (IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_singleelectron[i], 0.5).first);
      }
      if (channel_ == channel::eett) {
        passed_singleelectron_2 =  passed_singleelectron_2 || (IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_singleelectron[i], 0.5).first);
      }
    }


    //singlemuon
    for (unsigned i = 0; i < trg_obj_label_singlemuon.size(); ++i) {
      std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trg_obj_label_singlemuon[i]);
      if (channel_ == channel::mmtt || channel_ == channel::mttt) {
        passed_singlemuon_1 =  passed_singlemuon_1 || (IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_singlemuon[i], 0.5).first);
      }
      if (channel_ == channel::mmtt || channel_ == channel::emtt) {
        passed_singlemuon_2 =  passed_singlemuon_2 || (IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_singlemuon[i], 0.5).first);
      }
    }

    ////etaucross
    //for (unsigned i = 0; i < trg_obj_label_etaucross.size(); ++i) {
    //  std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trg_obj_label_etaucross[i]);
    //  if (channel_ == channel::ettt) {
    //    passed_etaucross_12 =  passed_etaucross_12 || ((IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_etaucross_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_etaucross_2[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_etaucross_overlap[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_etaucross_overlap[i], 0.5).first));
    //  }
    //  if (channel_ == channel::ettt || channel_ == channel::eett || channel_ == channel::emtt) {
    //    passed_etaucross_13 =  passed_etaucross_13 || ((IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_etaucross_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(2), objs, leg_etaucross_2[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_etaucross_overlap[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(2), objs, leg_etaucross_overlap[i], 0.5).first));
    //    passed_etaucross_14 =  passed_etaucross_14 || ((IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_etaucross_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(3), objs, leg_etaucross_2[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_etaucross_overlap[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(3), objs, leg_etaucross_overlap[i], 0.5).first));
    //  }
    //  if (channel_ == channel::eett) {
    //    passed_etaucross_23 =  passed_etaucross_23 || ((IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_etaucross_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(2), objs, leg_etaucross_2[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_etaucross_overlap[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(2), objs, leg_etaucross_overlap[i], 0.5).first));
    //    passed_etaucross_24 =  passed_etaucross_24 || ((IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_etaucross_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(3), objs, leg_etaucross_2[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_etaucross_overlap[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(3), objs, leg_etaucross_overlap[i], 0.5).first));
    //  }
    //}

    ////mutaucross
    //for (unsigned i = 0; i < trg_obj_label_mutaucross.size(); ++i) {
    //  std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trg_obj_label_mutaucross[i]);
    //  if (channel_ == channel::mttt) {
    //    passed_mutaucross_12 =  passed_mutaucross_12 || ((IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_mutaucross_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_mutaucross_2[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_mutaucross_overlap[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_mutaucross_overlap[i], 0.5).first));
    //  }
    //  if (channel_ == channel::mttt || channel_ == channel::mmtt) {
    //    passed_mutaucross_13 =  passed_mutaucross_13 || ((IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_mutaucross_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(2), objs, leg_mutaucross_2[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_mutaucross_overlap[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(2), objs, leg_mutaucross_overlap[i], 0.5).first));
    //    passed_mutaucross_14 =  passed_mutaucross_14 || ((IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_mutaucross_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(3), objs, leg_mutaucross_2[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_mutaucross_overlap[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(3), objs, leg_mutaucross_overlap[i], 0.5).first));
    //  }
    //  if (channel_ == channel::mmtt || channel_ == channel::emtt) {
    //    passed_mutaucross_23 =  passed_mutaucross_23 || ((IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_mutaucross_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(2), objs, leg_mutaucross_2[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_mutaucross_overlap[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(2), objs, leg_mutaucross_overlap[i], 0.5).first));
    //    passed_mutaucross_24 =  passed_mutaucross_24 || ((IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_mutaucross_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(3), objs, leg_mutaucross_2[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_mutaucross_overlap[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(3), objs, leg_mutaucross_overlap[i], 0.5).first));
    //  }
    //}

    ////emucross
    //for (unsigned i = 0; i < trg_obj_label_emucross.size(); ++i) {
    //  std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trg_obj_label_emucross[i]);
    //  if (channel_ == channel::emtt) {
    //    passed_emucross_12 =  passed_emucross_12 || ((IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_emucross_1[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_emucross_2[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(0), objs, leg_emucross_dZ[i], 0.5).first) && (IsFilterMatchedWithIndex(multileptons[0]->At(1), objs, leg_emucross_dZ[i], 0.5).first));
    //  }
    //}

    event->Add("trg_doubletau_12", passed_doubletau_12);
    event->Add("trg_doubletau_13", passed_doubletau_13);
    event->Add("trg_doubletau_14", passed_doubletau_14);
    event->Add("trg_doubletau_23", passed_doubletau_23);
    event->Add("trg_doubletau_24", passed_doubletau_24);
    event->Add("trg_doubletau_34", passed_doubletau_34);
    event->Add("trg_doubleelectron_12", passed_doubleelectron_12);
    event->Add("trg_doublemuon_12", passed_doublemuon_12);
    event->Add("trg_singlemuon_1", passed_singlemuon_1);
    event->Add("trg_singlemuon_2", passed_singlemuon_2);
    event->Add("trg_singleelectron_1", passed_singleelectron_1);
    event->Add("trg_singleelectron_2", passed_singleelectron_2);
    event->Add("trg_mutaucross_12", passed_mutaucross_12);
    event->Add("trg_mutaucross_13", passed_mutaucross_13);
    event->Add("trg_mutaucross_14", passed_mutaucross_14);
    event->Add("trg_mutaucross_23", passed_mutaucross_23);
    event->Add("trg_mutaucross_24", passed_mutaucross_24);
    event->Add("trg_etaucross_12", passed_etaucross_12);
    event->Add("trg_etaucross_13", passed_etaucross_13);
    event->Add("trg_etaucross_14", passed_etaucross_14);
    event->Add("trg_etaucross_23", passed_etaucross_23);
    event->Add("trg_etaucross_24", passed_etaucross_24);
    event->Add("trg_emucross_12", passed_emucross_12);


    return 0;
  }

  int HTTTriggerFilter::PostAnalysis() {
    return 0;
  }

  void HTTTriggerFilter::PrintInfo() {
    ;
  }
}
