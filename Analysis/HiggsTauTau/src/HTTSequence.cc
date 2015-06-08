#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include <memory>
// ROOT
#include "TH1.h"
// Objects
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
// Utilities
#include "Utilities/interface/FnRootTools.h"
// HTT-specific modules
#include "HiggsTauTau/interface/HTTSequence.h"
#include "HiggsTauTau/interface/HTTTriggerFilter.h"
#include "HiggsTauTau/interface/HTTEnergyScale.h"
#include "HiggsTauTau/interface/HTTEMuExtras.h"
#include "HiggsTauTau/interface/HTTGenEvent.h"
#include "HiggsTauTau/interface/HTTCategories.h"
#include "HiggsTauTau/interface/HTTPairSelector.h"
#include "HiggsTauTau/interface/SVFitTest.h"
#include "HiggsTauTau/interface/HTTRecoilCorrector.h"
#include "HiggsTauTau/interface/HhhBJetRegression.h"
#include "HiggsTauTau/interface/HTTSyncTemp.h"
#include "HiggsTauTau/interface/HTTSync.h"
#include "HiggsTauTau/interface/HTTEMuMVA.h"
#include "HiggsTauTau/interface/HhhEMuMVA.h"
#include "HiggsTauTau/interface/HhhEMuMVABoth.h"
#include "HiggsTauTau/interface/HhhMTMVABoth.h"
#include "HiggsTauTau/interface/HhhMTMVACategory.h"
#include "HiggsTauTau/interface/HTTWeights.h"
#include "HiggsTauTau/interface/HhhMetScale.h"
#include "HiggsTauTau/interface/EmbeddingKineReweightProducer.h"
#include "HiggsTauTau/interface/JetEnergyUncertainty.h"
#include "HiggsTauTau/interface/HTTPrint.h"
#include "Modules/interface/LumiMask.h"

// Generic modules
#include "Modules/interface/SimpleFilter.h"
#include "Modules/interface/CompositeProducer.h"
#include "Modules/interface/CopyCollection.h"
#include "Modules/interface/OneCollCompositeProducer.h"
#include "Modules/interface/OverlapFilter.h"
#include "Modules/interface/EnergyShifter.h"
#include "Modules/interface/PileupWeight.h"
#include "Modules/interface/CheckEvents.h"

namespace ic {

HTTSequence::HTTSequence(std::string& chan, std::string& var, std::string postf, Json::Value const& json) {
  //j_postfix = json["job"]["output_postfix"].asString();
  svfit_folder = json["svfit_folder"].asString();
  svfit_override = json["svfit_override"].asString();
  output_name=json["output_name"].asString();
  output_folder=json["output_folder"].asString();
  addit_output_folder=json["baseline"]["addit_output_folder"].asString();
  output_folder=output_folder+"/"+addit_output_folder;
  

  fs = std::make_shared<fwlite::TFileService>(
       (output_folder+"/"+chan + "_" + output_name + "_" + postf + ".root").c_str());
  js = json;
  channel_str = chan;
  jes_mode=json["baseline"]["jes_mode"].asUInt();
/*  elec_pt = json["baseline"]["elec_pt"].asDouble();
  elec_eta = json["baseline"]["elec_eta"].asDouble();
  elec_dxy = json["baseline"]["elec_dxy"].asDouble();
  elec_dz = json["baseline"]["elec_dz"].asDouble();
  muon_pt = json["baseline"]["muon_pt"].asDouble();
  muon_eta = json["baseline"]["muon_eta"].asDouble(); 
  muon_dxy = json["baseline"]["muon_dxy"].asDouble();
  muon_dz = json["baseline"]["muon_dz"].asDouble();
  tau_pt = json["baseline"]["tau_pt"].asDouble();
  tau_eta = json["baseline"]["tau_eta"].asDouble();
  tau_dz = json["baseline"]["tau_dz"].asDouble();
  tau_iso = json["baseline"]["tau_iso"].asDouble();
  veto_elec_pt = json["baseline"]["veto_elec_pt"].asDouble(); 
  veto_elec_eta = json["baseline"]["veto_elec_eta"].asDouble();
  veto_elec_dxy = json["baseline"]["veto_elec_dxy"].asDouble();
  veto_elec_dz = json["baseline"]["veto_elec_dz"].asDouble();
  veto_dielec_pt = json["baseline"]["veto_dielec_pt"].asDouble(); 
  veto_dielec_eta = json["baseline"]["veto_dielec_eta"].asDouble();
  veto_dielec_dxy = json["baseline"]["veto_dielec_dxy"].asDouble();
  veto_dielec_dz = json["baseline"]["veto_dielec_dz"].asDouble();
  veto_muon_pt = json["baseline"]["veto_muon_pt"].asDouble(); 
  veto_muon_eta = json["baseline"]["veto_muon_eta"].asDouble();
  veto_muon_dxy = json["baseline"]["veto_muon_dxy"].asDouble();
  veto_muon_dz = json["baseline"]["veto_muon_dz"].asDouble();
  veto_dimuon_pt = json["baseline"]["veto_dimuon_pt"].asDouble(); 
  veto_dimuon_eta = json["baseline"]["veto_dimuon_eta"].asDouble();
  veto_dimuon_dxy = json["baseline"]["veto_dimuon_dxy"].asDouble();
  veto_dimuon_dz = json["baseline"]["veto_dimuon_dz"].asDouble();
  tau_anti_elec = json["baseline"]["tau_anti_elec"].asString();
  tau_anti_muon = json["baseline"]["tau_anti_muon"].asString();
  min_taus     = json["baseline"]["min_taus"].asUInt();
  pair_dr      = json["baseline"]["pair_dr"].asDouble();
*/
  ic::strategy strategy_type  = String2Strategy(js["strategy"].asString());
  ic::era  era_type  = String2Era(js["era"].asString());
  //if(strategy_type == strategy::phys14){
   veto_elec_pt = 10;
   veto_elec_eta = 2.5;
   veto_elec_dxy = 0.045;
   veto_elec_dz = 0.2;
   veto_muon_pt = 10;
   veto_muon_eta = 2.4;
   veto_muon_dxy = 0.045;
   veto_muon_dz = 0.2;
   veto_dielec_pt = 15;
   veto_dielec_eta = 2.5;
   veto_dielec_dxy = 0.045;
   veto_dielec_dz = 0.2;
   veto_dimuon_pt = 15;
   veto_dimuon_eta = 2.4;
   veto_dimuon_dxy = 0.045;
   veto_dimuon_dz = 0.2;
   if(channel_str == "em"){
   elec_dxy = 0.045;
   elec_dz = 0.2;
   muon_dxy = 0.045;
   muon_dz = 0.2;
   pair_dr = 0.3;
  if(era_type == era::data_2015){
   elec_pt = 13.;
   elec_eta = 2.5;
   muon_pt = 9.5;
   muon_eta = 2.1;
  } else {
   elec_pt = 10.0;
   elec_eta = 2.3;
   muon_pt = 10.0;
   muon_eta = 2.1;
   tau_pt = 20.0;
   tau_eta = 2.3;
   tau_dz = 0.2;
 }
 if(special_mode ==22 || special_mode ==25){
  elec_dxy = 999.;
  muon_dxy = 999.;
  elec_dz = 999.;
  muon_dz = 999.;
 }
 if(special_mode == 20){
  elec_dxy = 999.;
  elec_dz = 999.;
 }
 if(special_mode == 21){
  muon_dxy = 999.;
  muon_dz = 999.;
  }
 }
 //  extra_elec_veto = true;
 //  extra_muon_veto = true;
 //  max_extra_elecs = 1;
 //  max_extra_muons = 1;
   pair_dr = 0.3;
 if (channel_str == "et"){
   elec_dz = 0.2;
   elec_dxy = 0.045;
   muon_dxy = 0.045;
   muon_dz = 0.2;
   tau_dz = 0.2;
  if(strategy_type == strategy::phys14){
  tau_anti_elec_discr = "againstElectronTightMVA5";
  tau_anti_muon_discr = "againstMuonLoose3";
  tau_iso_discr = "byCombinedIsolationDeltaBetaCorrRaw3Hits";
  tau_iso = 1.5;
  } else if (strategy_type == strategy::paper2013){
  tau_anti_elec_discr = "againstElectronTightMVA3";
  tau_anti_muon_discr = "againstMuonLoose";
  tau_iso_discr = "byCombinedIsolationDeltaBetaCorrRaw3Hits";
  tau_iso = 1.5;
  if(special_mode == 4 || special_mode ==5){
   tau_iso = 10;
  }
 }
 if(era_type == era::data_2015){
   
   min_taus = 1;
   pair_dr = 0.5;
   elec_pt = 23;
   elec_eta = 2.5;
   tau_pt  = 20;
   tau_eta = 2.3;
//   tau_anti_elec = "againstElectronTightMVA5";
//   tau_anti_muon = "againstMuonLoose3";
 } else if (era_type == era::data_2011){
   elec_pt = 20;
   elec_eta = 2.1;
   muon_pt = 17.0;
   muon_eta = 2.1;
   tau_pt = 20.0;
   tau_eta = 2.3;
   min_taus = 1;
  } else {
  elec_pt = 24.0;
  elec_eta = 2.1;
  muon_pt = 20.0;
  muon_eta = 2.1;
  tau_pt = 20.0;
  tau_eta = 2.3;
  min_taus = 1;
  }
  } 
  if (channel_str == "mt"){
  muon_dxy = 0.045;
  muon_dz = 0.2;
  elec_dxy = 0.045;
  elec_dz = 0.2;
  tau_dz = 0.2;
  if(strategy_type == strategy::phys14){
  tau_anti_elec_discr = "againstElectronVLooseMVA5";
  tau_anti_muon_discr = "againstMuonTight3";
  tau_iso_discr = "byCombinedIsolationDeltaBetaCorrRaw3Hits";
  tau_iso = 1.5;
  } else if (strategy_type == strategy::paper2013){
  tau_anti_elec_discr = "againstElectronLoose";
  tau_anti_muon_discr = "againstMuonTight";
  tau_iso_discr = "byCombinedIsolationDeltaBetaCorrRaw3Hits";
  tau_iso = 1.5;
  if(special_mode == 4 || special_mode == 5){
  tau_iso = 10.;
  }
   }
  if (era_type == era::data_2015){
  
  muon_pt = 18.0;
  muon_eta = 2.1;
  tau_pt = 20;
  tau_eta = 2.3;
  min_taus = 1;
  pair_dr = 0.5;
  } else if(era_type == era::data_2011){
  muon_pt = 17.0;
  muon_eta = 2.1;
  elec_pt = 20.0;
  elec_eta = 2.1;
  tau_pt = 20.0;
  tau_eta = 2.3;
  min_taus = 1;
  } else {
  muon_pt = 20.0;
  muon_eta = 2.1;
  elec_pt = 24.0;
  elec_eta = 2.1;
  tau_pt = 20.0;
  tau_eta = 2.3;
  min_taus = 1;
 }
 }
  if (channel_str == "tt"){
  tau_pt = 45;
  tau_eta = 2.1;
  tau_iso = 1.0;
  tau_dz = 0.2;
  min_taus = 2;
  tau_iso_discr = "byCombinedIsolationDeltaBetaCorrRaw3Hits";
  tau_anti_elec_discr = "againstElectronTightMVA5";
  tau_anti_muon_discr = "againstMuonLoose3";
  pair_dr = 0.5;
  }
  
  jets_label   = json["jets"].asString();
  met_label    = json["met"].asString();
  moriond_tau_scale = json["moriond_tau_scale"].asBool();
  pu_id_training = json["pu_id_training"].asUInt();
  bjet_regr_correction = json["bjet_regr_correction"].asBool();
  mass_shift = json["baseline"]["mass_shift"].asDouble();
  new_svfit_mode = json["new_svfit_mode"].asUInt();
  kinfit_mode = json["kinfit_mode"].asUInt(); 
  mva_met_mode = json["mva_met_mode"].asUInt();
  faked_tau_selector = json["faked_tau_selector"].asUInt();
  hadronic_tau_selector = json["hadronic_tau_selector"].asUInt(); 
  tau_scale_mode = json["tau_scale_mode"].asUInt();
  if(channel_str!="em"){
  tau_shift = json["baseline"]["tau_es_shift"].asDouble();
  } else tau_shift = json["baseline"]["elec_es_shift"].asDouble();



}

HTTSequence::~HTTSequence() {}


void HTTSequence::BuildSequence(){
  using ROOT::Math::VectorUtil::DeltaR;

  // Set global parameters that get used in multiple places
  ic::channel channel         = String2Channel(channel_str);
  ic::mc mc_type              = String2MC(js["mc"].asString());
  ic::era era_type            = String2Era(js["era"].asString());
  ic::strategy strategy_type  = String2Strategy(js["strategy"].asString());
  // Other flags
  bool is_data        = js["is_data"].asBool();
  bool is_embedded    = js["is_embedded"].asBool();

  bool real_tau_sample = ( (output_name.find("HToTauTau")             != output_name.npos)
                        || (output_name.find("HTohh")                 != output_name.npos)
                        || (output_name.find("AToZh")                 != output_name.npos)
                        || (output_name.find("DYJetsToTauTau")        != output_name.npos)
                        || (output_name.find("Embedded")              != output_name.npos)
                        || (output_name.find("RecHit")                != output_name.npos) );
  if (output_name.find("DYJetsToTauTau-L") != output_name.npos) real_tau_sample = false;
  if (output_name.find("DYJetsToTauTau-JJ") != output_name.npos) real_tau_sample = false;


  auto eventChecker = CheckEvents("EventChecker").set_skip_events(true);
  std::vector<int> to_check =
  {
  };

  HTTPrint httPrint("HTTPrint");
  for (auto ch : to_check) {
   eventChecker.CheckEvent(ch);
   httPrint.PrintEvent(ch);
  }
  httPrint.set_skip_events(false);
  if (to_check.size() > 0){
  BuildModule(eventChecker);
  BuildModule(httPrint);  
}


  // If desired, run the HTTGenEventModule which will add some handily-
  // formatted generator-level info into the Event
  if (js["run_gen_info"].asBool()) {
    BuildModule(HTTGenEvent("HTTGenEvent")
        .set_genparticle_label(js["genTaus"].asString())
        .set_genjet_label(js["genJets"].asString()));
  }


  if(is_data && strategy_type!=strategy::phys14){
  std::string data_json = "";
  if (era_type == era::data_2011) {
    if (channel == channel::em) {
      data_json = "input/json/json_data_2011.txt";
    } else{
      data_json = "input/json/json_data_2011_et_mt.txt";
    }
  }             
  if (era_type == era::data_2012_rereco)       data_json = "input/json/data_2012_rereco.txt";
  if (era_type == era::data_2015)  data_json= "input/json/data_2012_rereco.txt";

  BuildModule(LumiMask("LumiMask")
    .set_produce_output_jsons("")
    .set_input_file(data_json));
  }


  if (channel == channel::et) BuildETPairs();
  if (channel == channel::mt) BuildMTPairs();
  if (channel == channel::em) BuildEMPairs();
  if (channel == channel::tt) BuildTTPairs();

  // Pair DeltaR filtering
  BuildModule(SimpleFilter<CompositeCandidate>("PairFilter")
      .set_input_label("ditau").set_min(1)
//      .set_predicate( (bind(&CompositeCandidate::DeltaR, _1, "lepton1","lepton2") > 0.5)));
      .set_predicate([=](CompositeCandidate const* c) {
        return DeltaR(c->at(0)->vector(), c->at(1)->vector())
            > pair_dr;
      }));


  // Trigger filtering
if(strategy_type != strategy::phys14){
    if (js["run_trg_filter"].asBool()) {
        BuildModule(HTTTriggerFilter("HTTTriggerFilter")
            .set_channel(channel)
            .set_mc(mc_type)
            .set_is_data(is_data)
            .set_is_embedded(is_embedded)
            .set_pair_label("ditau"));
      }
  }

  // Lepton Vetoes
  if (js["baseline"]["di_elec_veto"].asBool()) BuildDiElecVeto();
  if (js["baseline"]["di_muon_veto"].asBool()) BuildDiMuonVeto();
  if (js["baseline"]["extra_elec_veto"].asBool()) BuildExtraElecVeto();
  if (js["baseline"]["extra_muon_veto"].asBool()) BuildExtraMuonVeto();


  // Pileup Weighting
if(strategy_type != strategy::phys14){
  TH1D d_pu = GetFromTFile<TH1D>(js["data_pu_file"].asString(), "/", "pileup");
  TH1D m_pu = GetFromTFile<TH1D>(js["mc_pu_file"].asString(), "/", "pileup");
  if (js["do_pu_wt"].asBool()) {
    BuildModule( PileupWeight("PileupWeight")
        .set_data(new TH1D(d_pu)).set_mc(new TH1D(m_pu)));
  }
}


 // bool bjet_regr_correction = false;
 // bool moriond_tau_scale = 0;
  


  BuildModule(HTTPairSelector("HTTPairSelector")
    .set_channel(channel)
    .set_fs(fs.get())
    .set_pair_label("ditau")
    .set_met_label(met_label)
    .set_mva_met_from_vector(mva_met_mode == 1)
    .set_use_most_isolated(channel==channel::tt ? true : false)
    .set_faked_tau_selector(faked_tau_selector)
    .set_hadronic_tau_selector(hadronic_tau_selector)
    .set_gen_taus_label(is_embedded ? "genParticlesEmbedded" : "genParticlesTaus")
    .set_scale_met_for_tau((tau_scale_mode > 0 || (moriond_tau_scale && (is_embedded || !is_data) )   ))
    .set_tau_scale(tau_shift)
    .set_allowed_tau_modes(allowed_tau_modes));


 if (jes_mode > 0 && !is_data ){
  std::string jes_input_file = "input/jec/JEC11_V12_AK5PF_UncertaintySources.txt";
  std::string jes_input_set  = "SubTotalDataMC";
  if (era_type == era::data_2012_rereco) {
    jes_input_file = "input/jec/Summer13_V1_DATA_UncertaintySources_AK5PF.txt";
    jes_input_set  = "SubTotalMC";
  }
  if (era_type == era::data_2015) {
    jes_input_file = "input/jec/Summer13_V1_DATA_UncertaintySources_AK5PF.txt";
    jes_input_set  = "SubTotalMC";
  }
 BuildModule(JetEnergyUncertainty<PFJet>("JetEnergyUncertainty")
  .set_input_label(jets_label)
  .set_jes_shift_mode(jes_mode)
  .set_uncert_file(jes_input_file)
  .set_uncert_set(jes_input_set));

}

  
 // unsigned pu_id_training = 1;
  
    
  //if (channel == channel::em) httPairSelector.set_tau_scale(elec_shift);
  BuildModule(SimpleFilter<PFJet>("JetIDFilter")
    .set_input_label(jets_label)
    .set_predicate((bind(PFJetIDNoHFCut, _1)) && bind(PileupJetID, _1, pu_id_training)));

  BuildModule(CopyCollection<PFJet>("CopyFilteredJets",jets_label,"pfJetsPFlowFiltered"));

  BuildModule(OverlapFilter<PFJet, CompositeCandidate>("JetLeptonOverlapFilter")
    .set_input_label(jets_label)
    .set_reference_label("ditau")
    .set_min_dr(0.5));

//std::string output_name="";
    if(strategy_type != strategy::phys14){
  BuildModule(HTTRecoilCorrector("HTTRecoilCorrector")
    .set_sample(output_name)
    .set_channel(channel)
    .set_mc(mc_type)
    .set_met_label(met_label)
    .set_jets_label(jets_label)
    .set_strategy(strategy_type)
    .set_dilepton_label("ditau")
    //option to take met scale uncertainty from recoil corrector files - off for now as files have over-inflated uncertainties
    //.set_met_scale_mode(metscale_mode)
    .set_w_hack(true));
}

    if(js["metscale_mode"].asUInt() > 0  && !is_data ){
 BuildModule(HhhMetScale("HhhMetScale")
    .set_met_scale_mode(js["metscale_mode"].asUInt())
    .set_met_label(met_label)
    .set_scale_shift(0.04));
   }



  BuildModule(SVFitTest("SVFitTest")
    .set_channel(channel)
    .set_outname(svfit_override == "" ? output_name : svfit_override)
    .set_run_mode(new_svfit_mode)
    .set_fail_mode(1)
    .set_require_inputs_match(false)
    .set_split(7000)
    .set_dilepton_label("ditau")
    .set_met_label(met_label)
    .set_fullpath(svfit_folder)
    .set_MC(true));



   if(strategy_type != strategy::phys14){

  HTTWeights httWeights = HTTWeights("HTTWeights")
    .set_channel(channel)
    .set_era(era_type)
    .set_mc(mc_type)
    .set_trg_applied_in_mc(true)
    .set_do_trg_weights(false)
    .set_do_etau_fakerate(false)
    .set_do_mtau_fakerate(false)
    .set_do_idiso_weights(false)
    .set_do_id_weights(false) // This will override do_idiso_weights, applying only ID weights in embedded
    .set_do_emu_e_fakerates(false)
    .set_do_emu_m_fakerates(false)
    .set_do_top_factors(false)
    .set_do_tau_id_weights(real_tau_sample)
    .set_gen_tau_collection(is_embedded ? "genParticlesEmbedded" : "genParticlesTaus")
    .set_jets_label(jets_label)
    .set_ditau_label("ditau")
    .set_do_btag_weight(false);
  if (!is_data) {
    httWeights.set_do_trg_weights(true).set_trg_applied_in_mc(true).set_do_idiso_weights(true);
    httWeights.set_do_btag_weight(true).set_btag_mode(btag_mode).set_bfake_mode(bfake_mode);
  }
  if (output_name.find("DYJetsToLL") != output_name.npos && (channel == channel::et || channel == channel::etmet) ) httWeights.set_do_etau_fakerate(true);
  if (output_name.find("DYJetsToLL") != output_name.npos && (channel == channel::mt || channel == channel::mtmet) ) httWeights.set_do_mtau_fakerate(true);
  if (real_tau_sample && channel != channel::em) httWeights.set_do_tau_mode_scale(true);
  if (is_embedded) httWeights.set_do_trg_weights(true).set_trg_applied_in_mc(false).set_do_idiso_weights(false).set_do_id_weights(true);
  if (special_mode == 20 || special_mode == 22) httWeights.set_do_emu_e_fakerates(true);
  if (special_mode == 21 || special_mode == 22) httWeights.set_do_emu_m_fakerates(true);
  if (channel == channel::etmet || channel == channel::mtmet) httWeights.set_trg_applied_in_mc(false);
  if (output_name.find("TTJets") != output_name.npos) httWeights.set_do_topquark_weights(true);
  if (special_mode != 5 && output_name.find("WJetsToLNu") != output_name.npos) httWeights.set_do_tau_fake_weights(true);

  if (output_name.find("WJetsToLNuSoup") != output_name.npos) {
    httWeights.set_do_w_soup(true);
    if (mc_type == mc::fall11_42X) {
      httWeights.SetWTargetFractions(0.752332, 0.171539, 0.0538005, 0.0159036, 0.00642444);
      httWeights.SetWInputYields(81295381.0, 70712575.0, 25320546.0, 7541595.0, 12973738.0);
    }
    if (mc_type == mc::summer12_53X && strategy_type == strategy::paper2013) {
      httWeights.SetWTargetFractions(0.743925, 0.175999, 0.0562617, 0.0168926, 0.00692218);
      httWeights.SetWInputYields(76102995.0, 52926398.0, 64738774.0, 30780647.0, 13382803.0);
    }
  }
  if (output_name.find("DYJets") != output_name.npos && output_name.find("Soup") != output_name.npos) {
    if (mc_type == mc::summer12_53X) {
      httWeights.set_do_dy_soup(true);
      httWeights.SetDYTargetFractions(0.723342373, 0.190169492, 0.061355932, 0.017322034, 0.007810169);
      httWeights.SetDYInputYields(30459503.0, 24045248.0, 21852156.0, 11015445.0, 6402827.0);
    }
  }
  if ( (output_name.find("GluGluToHToTauTau_M-")          != output_name.npos ||
        output_name.find("GluGluToHToWWTo2LAndTau2Nu_M-") != output_name.npos ||
        output_name.find("GluGluToHToWWTo2L2Nu_M-")       != output_name.npos ||
        output_name.find("GluGluToHToWWTo2Tau2Nu_M-")     != output_name.npos ||
        output_name.find("GluGluToHToWWToLNuTauNu_M-")    != output_name.npos) &&
      output_name.find("SUSYGluGluToHToTauTau_M-")  == output_name.npos
      && output_name.find("minloHJJ")               == output_name.npos) {
    std::size_t pos = output_name.find("_M-");
    if (pos != output_name.npos) {
      std::string mass_string;
      for (unsigned i = (pos+3); i < output_name.size(); ++i) {
        if (output_name.at(i) != '_') mass_string += output_name.at(i);
        if (output_name.at(i) == '_') break;
      }
      std::cout << "SM ggH Signal sample detected with mass: " << mass_string << std::endl;
      httWeights.set_ggh_mass(mass_string);
    }
  }

 BuildModule(httWeights);



}
   if (is_embedded && era_type == era::data_2012_rereco) {
 EmbeddingKineReweightProducer rechitWeights = EmbeddingKineReweightProducer("RecHitWeights")
    .set_genparticle_label("genParticlesEmbedded")
    .set_ditau_label("ditau")
    .set_channel(channel);
  if (channel == channel::et) rechitWeights.set_file("input/rechit_weights/embeddingKineReweight_ePtGt20tauPtGt18_recEmbedded.root");
  if (channel == channel::mt) rechitWeights.set_file("input/rechit_weights/embeddingKineReweight_muPtGt16tauPtGt18_recEmbedded.root");
  if (channel == channel::em) rechitWeights.set_file("input/rechit_weights/embeddingKineReweight_recEmbedding_emu.root");

 BuildModule(rechitWeights);

   }

   if (strategy_type == strategy::paper2013 && channel == channel::em) {
  BuildModule(HTTEMuMVA("EMuMVA")
    .set_ditau_label("ditau"));
  //Some attempts at MVA for the H->hh analysis, could possibly be used in the future
  BuildModule(HhhEMuMVA("HhhEMuMVA")
    .set_ditau_label("ditau"));
 BuildModule(HhhEMuMVABoth("HhhEMuMVABoth")
    .set_ditau_label("ditau"));
}
if (strategy_type == strategy::paper2013 && channel ==channel::mt){
  BuildModule(HhhMTMVABoth("HhhMTMVABoth")
    .set_ditau_label("ditau")); 
 BuildModule(HhhMTMVACategory("HhhMTMVACategory")
    .set_ditau_label("ditau"));
   }

if(bjet_regr_correction){
 BuildModule(HhhBJetRegression("hhhBJetRegression")
  .set_jets_label(jets_label));
}

if(js["make_sync_ntuple"].asBool() && strategy_type!=strategy::phys14){
 BuildModule(HTTSync("HTTSync","SYNCFILE_" + output_name, channel)
 .set_is_embedded(is_embedded).set_met_label(met_label).set_ditau_label("ditau"));
 //.set_is_embedded(is_embedded).set_met_label(met_label).set_jet_label(jets_label));
}


  BuildModule(HTTCategories("HTTCategories")
    .set_fs(fs.get())
    .set_channel(channel)
    .set_era(era_type)
    .set_strategy(strategy_type)
    .set_ditau_label("ditau")
    .set_met_label(met_label)
    .set_jets_label(jets_label)
    .set_kinfit_mode(kinfit_mode)
    .set_bjet_regression(bjet_regr_correction)
    .set_mass_shift(mass_shift)
    .set_write_tree(true));



}

// --------------------------------------------------------------------------
// TT Pair Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildTTPairs(){
 
 BuildTauSelection();

 BuildModule(OneCollCompositeProducer<Tau>("TTPairProducer")
      .set_input_label(js["taus"].asString()).set_output_label("ditau")
      .set_sort_by_iso(true)
      .set_candidate_name_first("lepton1").set_candidate_name_second("lepton2"));
 }

  



// --------------------------------------------------------------------------
// ET Pair Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildETPairs() {
  ic::strategy strategy_type  = String2Strategy(js["strategy"].asString());

  BuildModule(CopyCollection<Electron>("CopyToSelectedElectrons",
      js["electrons"].asString(), "sel_electrons"));

  std::function<bool(Electron const*)> ElecID;
  std::string id_fn = js["baseline"]["elec_id"].asString();
  unsigned special_mode = js["baseline"]["special_mode"].asUInt();
  if(strategy_type!=strategy::phys14){
    if(special_mode == 20 || special_mode == 22){
     ElecID = [](Electron const* e) { return HttEMuFakeElectron(e); };
    } else if (special_mode == 25){
     ElecID = [](Electron const* e) { return true;};
    } else if (id_fn == "MVA:Loose") {
      ElecID = [](Electron const* e) { return ElectronHTTId(e, true); };
    } else if (id_fn == "MVA:Tight") {
      ElecID = [](Electron const* e) { return ElectronHTTId(e, false); };
    } else if (id_fn == "CutBased") {
      ElecID = [](Electron const* e) { return ElectronZbbID(e); };
    }
  }  else {
      ElecID = [](Electron const* e) { return ElectronHTTIdPhys14(e, false); };
    }


  BuildModule(SimpleFilter<Electron>("ElectronFilter")
      .set_input_label("sel_electrons").set_min(1)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > elec_pt    &&
                fabs(e->eta())          < elec_eta   &&
                fabs(e->dxy_vertex())   < elec_dxy   &&
                fabs(e->dz_vertex())    < elec_dz    &&
                ElecID(e) ;

      }));


 double elec_iso_max_val =0.;
 double elec_iso_min_val =0.; 
 elec_iso_min_val = -0.01;
 if(strategy_type != strategy::phys14){
  if(special_mode == 2){
   elec_iso_max_val = 0.5;
  } else if (special_mode ==3 || special_mode ==4){
   elec_iso_max_val = 0.5;
   elec_iso_min_val = 0.2;
  } else {
   elec_iso_max_val = 0.1;
   elec_iso_min_val = -0.01;
  }
}
  
   

  if (js["baseline"]["lep_iso"].asBool()&&special_mode !=25 &&special_mode != 23 &&strategy_type!=strategy::phys14) {
    BuildModule(SimpleFilter<Electron>("ElectronIsoFilter")
        .set_input_label("sel_electrons").set_min(1)
        .set_predicate([=](Electron const* e) {
          if(special_mode == 24){
            return !PF04IsolationEBElec(e, 0.5, 0.15, 0.1);
          } else {
            return PF04IsolationVal(e, 0.5) > elec_iso_min_val && PF04IsolationVal(e,0.5) < elec_iso_max_val;
          } 
        }));
  }


  if (js["baseline"]["lep_iso"].asBool() &&strategy_type==strategy::phys14) {
    BuildModule(SimpleFilter<Electron>("ElectronIsoFilter")
        .set_input_label("sel_electrons").set_min(1)
        .set_predicate([=](Electron const* e) {
            return PF03IsolationVal(e, 0.5)<0.15;
          //return PF04IsolationEBElec(e, 0.5, 0.15, 0.1);
        }));
  }


  BuildTauSelection();

  BuildModule(CompositeProducer<Electron, Tau>("ETPairProducer")
      .set_input_label_first("sel_electrons")
      .set_input_label_second(js["taus"].asString())
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));
}

// --------------------------------------------------------------------------
// MT Pair Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildMTPairs() {

 ic::strategy strategy_type  = String2Strategy(js["strategy"].asString());

  BuildModule(CopyCollection<Muon>("CopyToSelectedMuons",
      js["muons"].asString(), "sel_muons"));


  std::function<bool(Muon const*)> MuonID;
  if(strategy_type!=strategy::phys14){
    if(special_mode == 21 || special_mode == 22){
     MuonID = [](Muon const* m) { return HttEMuFakeMuon(m); };
    } else if(special_mode ==25){
     MuonID = [](Muon const* m) {return true; } ;
    } else { 
      MuonID = [](Muon const* m) { return MuonTight(m); };
    }
   } else{
    MuonID = [](Muon const* m) { return MuonMedium(m); };
   }
    



  BuildModule(SimpleFilter<Muon>("MuonFilter")
      .set_input_label("sel_muons").set_min(1)
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > muon_pt    &&
                fabs(m->eta())          < muon_eta   &&
                fabs(m->dxy_vertex())   < muon_dxy   &&
                fabs(m->dz_vertex())    < muon_dz   &&
                MuonID(m);

      }));

 double muon_iso_min = 0.;
 double muon_iso_max = 0.;
 if(special_mode ==2 ){
   muon_iso_min=-0.01;
   muon_iso_max=0.5;
 } else if (special_mode ==3 || special_mode==4){
   muon_iso_min=0.2;
   muon_iso_max=0.5;
 } else {
   muon_iso_min =-0.01;
   muon_iso_max = 0.1;
 }


  if (js["baseline"]["lep_iso"].asBool()&&special_mode !=25 &&special_mode != 22 &&special_mode != 21 &&strategy_type!=strategy::phys14) {
    BuildModule(SimpleFilter<Muon>("MuonIsoFilter")
        .set_input_label("sel_muons").set_min(1)
        .set_predicate([=](Muon const* m) {
          return PF04IsolationVal(m, 0.5)<muon_iso_max && PF04IsolationVal(m,0.5)>muon_iso_min;
        }));
  }



  if (js["baseline"]["lep_iso"].asBool()&&strategy_type==strategy::phys14) {
    BuildModule(SimpleFilter<Muon>("MuonIsoFilter")
        .set_input_label("sel_muons").set_min(1)
        .set_predicate([=](Muon const* m) {
          return PF03IsolationVal(m, 0.5,0)<0.1;
        }));
  }

 


  BuildTauSelection();

  BuildModule(CompositeProducer<Muon, Tau>("MTPairProducer")
      .set_input_label_first("sel_muons")
      .set_input_label_second(js["taus"].asString())
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));
}

// --------------------------------------------------------------------------
// EM Pair Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildEMPairs() {

 ic::strategy strategy_type  = String2Strategy(js["strategy"].asString());

  BuildModule(EnergyShifter<Electron>("ElectronEnergyScaleCorrection")
      .set_input_label(js["electrons"].asString())
      .set_shift(js["baseline"]["elec_es_shift"].asDouble()));

  if (js["baseline"]["do_em_extras"].asBool()) {
    BuildModule(HTTEMuExtras("EMExtras"));
  }

  BuildModule(CopyCollection<Electron>("CopyToSelectedElectrons",
      js["electrons"].asString(), "sel_electrons"));

  BuildModule(CopyCollection<Muon>("CopyToSelectedMuons",
      js["muons"].asString(), "sel_muons"));


  std::function<bool(Electron const*)> ElecID;
  std::string id_fn = js["baseline"]["elec_id"].asString();
  unsigned special_mode = js["baseline"]["special_mode"].asUInt();
  if(strategy_type!=strategy::phys14){
    if(special_mode == 20 || special_mode == 22){
     ElecID = [](Electron const* e) { return HttEMuFakeElectron(e); };
    } else if(special_mode ==25){
     ElecID = [](Electron const* e) {return true; } ;
    } else if (id_fn == "MVA:Loose") {
      ElecID = [](Electron const* e) { return ElectronHTTId(e, true); };
    } else if (id_fn == "MVA:Tight") {
      ElecID = [](Electron const* e) { return ElectronHTTId(e, false); };
    } else if (id_fn == "CutBased") {
      ElecID = [](Electron const* e) { return ElectronZbbID(e); };
    }
  } else {
      ElecID = [](Electron const* e) { return ElectronHTTIdPhys14(e, false); };
    }



  BuildModule(SimpleFilter<Electron>("ElectronFilter")
      .set_input_label("sel_electrons").set_min(1)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > elec_pt    &&
                fabs(e->eta())          < elec_eta   &&
                fabs(e->dxy_vertex())   < elec_dxy   &&
                fabs(e->dz_vertex())    < elec_dz    &&
                ElecID(e) ;

      }));


 double elec_iso_max_val = 0.;
 double elec_iso_min_val = 0.; 
 elec_iso_min_val = -0.1;
 elec_iso_max_val = 0.5;
 if(strategy_type != strategy::phys14){
  if(special_mode == 2){
   elec_iso_max_val = 0.5;
  } else if (special_mode ==3 || special_mode ==4){
   elec_iso_max_val = 0.5;
   elec_iso_min_val = 0.2;
  } 
}
   

  if (js["baseline"]["lep_iso"].asBool()&&special_mode !=25 &&special_mode != 23 &&strategy_type!=strategy::phys14) {
    BuildModule(SimpleFilter<Electron>("ElectronIsoFilter")
        .set_input_label("sel_electrons").set_min(1)
        .set_predicate([=](Electron const* e) {
          if(special_mode == 24){
            return !PF04IsolationEBElec(e, 0.5, 0.15, 0.1);
          } else if (special_mode == 2 || special_mode ==3){
            return PF04IsolationVal(e, 0.5) > elec_iso_min_val && PF04IsolationVal(e,0.5) < elec_iso_max_val;
          } else {
            return PF04IsolationEBElec(e, 0.5, 0.15, 0.1);
          }
        }));
  }


  if (js["baseline"]["lep_iso"].asBool() &&strategy_type==strategy::phys14) {
    BuildModule(SimpleFilter<Electron>("ElectronIsoFilter")
        .set_input_label("sel_electrons").set_min(1)
        .set_predicate([=](Electron const* e) {
            return PF03IsolationVal(e, 0.5)<0.15;
          //return PF04IsolationEBElec(e, 0.5, 0.15, 0.1);
        }));
  }

  BuildModule(OverlapFilter<Electron, Muon>("ElecMuonOverlapFilter")
      .set_input_label("sel_electrons")
      .set_reference_label(js["muons"].asString())
      .set_min_dr(0.3));


  std::function<bool(Muon const*)> MuonID;
  if(strategy_type!=strategy::phys14){
    if(special_mode == 21 || special_mode == 22){
     MuonID = [](Muon const* m) { return HttEMuFakeMuon(m); };
    } else if(special_mode ==25){
     MuonID = [](Muon const* m) {return true; } ;
    } else { 
      MuonID = [](Muon const* m) { return MuonTight(m); };
    }
   } else{
    MuonID = [](Muon const* m) { return MuonMedium(m); };
   }
    



  BuildModule(SimpleFilter<Muon>("MuonFilter")
      .set_input_label("sel_muons").set_min(1)
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > muon_pt    &&
                fabs(m->eta())          < muon_eta   &&
                fabs(m->dxy_vertex())   < muon_dxy   &&
                fabs(m->dz_vertex())    < muon_dz   &&
                MuonID(m);

      }));

 double muon_iso_min = 0.;
 double muon_iso_max = 0.;;
 if(special_mode ==2 ){
   muon_iso_min=-0.01;
   muon_iso_max=0.5;
 } else if (special_mode ==3 || special_mode==4){
   muon_iso_min=0.2;
   muon_iso_max=0.5;
 }


  if (js["baseline"]["lep_iso"].asBool()&&special_mode !=25 &&special_mode != 22 &&special_mode != 21 &&strategy_type!=strategy::phys14) {
    BuildModule(SimpleFilter<Muon>("MuonIsoFilter")
        .set_input_label("sel_muons").set_min(1)
        .set_predicate([=](Muon const* m) {
         if(special_mode == 2|| special_mode ==3 ||special_mode ==4){
          return PF04IsolationVal(m, 0.5)<muon_iso_max && PF04IsolationVal(m,0.5)>muon_iso_min;
         } else{
          return PF04IsolationEB(m, 0.5, 0.15, 0.1);
        }
        }));
  }



  if (js["baseline"]["lep_iso"].asBool()&&strategy_type==strategy::phys14) {
    BuildModule(SimpleFilter<Muon>("MuonIsoFilter")
        .set_input_label("sel_muons").set_min(1)
        .set_predicate([=](Muon const* m) {
          return PF03IsolationVal(m, 0.5,0)<0.15;
        }));
  }

     
  
  BuildModule(CompositeProducer<Electron, Muon>("EMPairProducer")
      .set_input_label_first("sel_electrons")
      .set_input_label_second("sel_muons")
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));

  BuildModule(SimpleFilter<CompositeCandidate>("EMPairFilter")
      .set_input_label("ditau").set_min(1)
      .set_predicate([=](CompositeCandidate const* c) {
        return PairOneWithPt(c, 20.0);
      }));
}

// --------------------------------------------------------------------------
// Tau Selection Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildTauSelection(){
  Json::Value base = js["baseline"];
  ic::strategy strategy_type  = String2Strategy(js["strategy"].asString());
  ic::channel channel_type         = String2Channel(channel_str);

bool moriond_tau_scale =false;
  bool real_tau_sample = ( (output_name.find("HToTauTau")             != output_name.npos)
                        || (output_name.find("HTohh")                 != output_name.npos)
                        || (output_name.find("AToZh")                 != output_name.npos)
                        || (output_name.find("DYJetsToTauTau")        != output_name.npos)
                        || (output_name.find("Embedded")              != output_name.npos)
                        || (output_name.find("RecHit")                != output_name.npos) );
  if (output_name.find("DYJetsToTauTau-L") != output_name.npos) real_tau_sample = false;
  if (output_name.find("DYJetsToTauTau-JJ") != output_name.npos) real_tau_sample = false;

if(real_tau_sample) moriond_tau_scale = true; 

  BuildModule(HTTEnergyScale("TauEnergyScaleCorrection")
      .set_input_label("taus")
      .set_shift(base["tau_es_shift"].asDouble())
      .set_strategy(strategy_type)
      .set_moriond_corrections(moriond_tau_scale));
    //  .set_moriond_corrections(base["tau_es_corr"].asBool()));
//    if (correct_es_sample) httEnergyScale.set_moriond_corrections(moriond_tau_scale);


  BuildModule(SimpleFilter<Tau>("TauFilter")
      .set_input_label(js["taus"].asString()).set_min(min_taus)
      .set_predicate([=](Tau const* t) {
        return  t->pt()                     >  tau_pt     &&
                fabs(t->eta())              <  tau_eta    &&
                fabs(t->lead_dz_vertex())   <  tau_dz     &&
                t->GetTauID("decayModeFinding") > 0.5;

      }));

  if (base["lep_iso"].asBool()) {
  if(strategy_type!= strategy::phys14 && strategy_type!=strategy::paper2013){
    BuildModule(SimpleFilter<Tau>("TauIsoFilter")
        .set_input_label(js["taus"].asString()).set_min(min_taus)
        .set_predicate([=](Tau const* t) {
          return t->GetTauID(tau_iso_discr) > 0.5;
        }));
  } else {
    BuildModule(SimpleFilter<Tau>("TauIsoFilter")
        .set_input_label(js["taus"].asString()).set_min(min_taus)
        .set_predicate([=](Tau const* t) {
          return t->GetTauID(tau_iso_discr) < tau_iso;
        }));
  }

 }

  if (base["do_tau_anti_elec"].asBool()) {
   SimpleFilter<Tau> tauAntiElecFilter = SimpleFilter<Tau>("TauAntiElecFilter")
        .set_input_label(js["taus"].asString()).set_min(min_taus)
        .set_predicate([=](Tau const* t) {
          return t->GetTauID(tau_anti_elec_discr) > 0.5;
          //return t->GetTauID(base["tau_anti_elec"].asString()) > 0.5;
        });
    if(channel_type == channel::et && strategy_type == strategy::paper2013){
       tauAntiElecFilter.set_predicate([=](Tau const* t){
         return passAntiEMVA(t, 0);
       });
     }     


BuildModule(tauAntiElecFilter);
  }

  if (base["do_tau_anti_muon"].asBool()) {
    SimpleFilter<Tau> tauAntiMuonFilter = SimpleFilter<Tau>("TauAntiMuonFilter")
        .set_input_label(js["taus"].asString()).set_min(min_taus)
        .set_predicate([=](Tau const* t) {
          return t->GetTauID(tau_anti_muon_discr) > 0.5;
          //return t->GetTauID(base["tau_anti_muon"].asString()) > 0.5;
          // TauEoverP(t, 0.2) needed in legacy
        });
      if(channel_type == channel::mt && strategy::paper2013){
       tauAntiMuonFilter.set_predicate([=](Tau const* t){
         return t->GetTauID(tau_anti_muon_discr)> 0.5 &&
                TauEoverP(t, 0.2);
         });
       }
  BuildModule(tauAntiMuonFilter);
  }
}

void HTTSequence::BuildDiElecVeto() {
  ic::strategy strategy_type  = String2Strategy(js["strategy"].asString());

  BuildModule(CopyCollection<Electron>("CopyToVetoElecs",
      js["electrons"].asString(), "veto_elecs"));

SimpleFilter<Electron> vetoElecFilter = SimpleFilter<Electron>("VetoElecFilter")
      .set_input_label("veto_elecs");
    if(strategy_type==strategy::paper2013){
      vetoElecFilter.set_predicate([=](Electron const* e) {
        return  e->pt()                 > veto_dielec_pt    &&
                fabs(e->eta())          < veto_dielec_eta   &&
                fabs(e->dxy_vertex())   < veto_dielec_dxy   &&
                fabs(e->dz_vertex())    < veto_dielec_dz   &&
                Electron2011WP95ID(e)     &&                
                PF04IsolationVal(e, 0.5) < 0.3;
      });
    } else if(strategy_type==strategy::phys14){
       vetoElecFilter.set_predicate([=](Electron const* e) {
        return  e->pt()                 > veto_dielec_pt    &&
                fabs(e->eta())          < veto_dielec_eta   &&
                fabs(e->dxy_vertex())   < veto_dielec_dxy   &&
                fabs(e->dz_vertex())    < veto_dielec_dz   &&
    //Probably need to use different filter, but just for now until something is defined:
                VetoElectronIDPhys14(e)           &&
                PF03IsolationVal(e, 0.5) < 0.3;
      });
   } 


  BuildModule(vetoElecFilter);

  BuildModule(OneCollCompositeProducer<Electron>("VetoElecPairProducer")
      .set_input_label("veto_elecs").set_output_label("elec_veto_pairs")
      .set_candidate_name_first("elec1").set_candidate_name_second("elec2"));

  BuildModule(SimpleFilter<CompositeCandidate>("VetoElecPairFilter")
      .set_input_label("elec_veto_pairs").set_min(0).set_max(0)
      .set_predicate([=](CompositeCandidate const* c) {
        return  c->DeltaR("elec1", "elec2") > 0.15 &&
                c->charge() == 0;
      }));
}

void HTTSequence::BuildDiMuonVeto() {
  ic::strategy strategy_type  = String2Strategy(js["strategy"].asString());

  BuildModule(CopyCollection<Muon>("CopyToVetoMuons",
      js["muons"].asString(), "veto_muons"));

 SimpleFilter<Muon> vetoMuonFilter = SimpleFilter<Muon>("VetoMuonFilter")
      .set_input_label("veto_muons");
  if(strategy_type==strategy::paper2013){
      vetoMuonFilter.set_predicate([=](Muon const* m) {
        return  m->pt()                 > veto_dimuon_pt    &&
                fabs(m->eta())          < veto_dimuon_eta   &&
                fabs(m->dxy_vertex())   < veto_dimuon_dxy   &&
                fabs(m->dz_vertex())    < veto_dimuon_dz    &&
                m->is_global()                    &&
                m->is_tracker()                   &&
                PF04IsolationVal(m, 0.5) < 0.3;
      });
    } else if(strategy_type==strategy::phys14){
      vetoMuonFilter.set_predicate([=](Muon const* m) {
        return  m->pt()                 > veto_dimuon_pt    &&
                fabs(m->eta())          < veto_dimuon_eta   &&
                fabs(m->dxy_vertex())   < veto_dimuon_dxy   &&
                fabs(m->dz_vertex())    < veto_dimuon_dz    &&
                m->is_global()                    &&
                m->is_tracker()                   &&
                PF03IsolationVal(m, 0.5) < 0.3;
      });
    }

  BuildModule(vetoMuonFilter);

  BuildModule(OneCollCompositeProducer<Muon>("VetoMuonPairProducer")
      .set_input_label("veto_muons").set_output_label("muon_veto_pairs")
      .set_candidate_name_first("muon1").set_candidate_name_second("muon2"));

  BuildModule(SimpleFilter<CompositeCandidate>("VetoMuonPairFilter")
      .set_input_label("muon_veto_pairs").set_min(0).set_max(0)
      .set_predicate([=](CompositeCandidate const* c) {
        return  c->DeltaR("muon1", "muon2") > 0.15 &&
                c->charge() == 0;
      }));
}

void HTTSequence::BuildExtraElecVeto(){
  ic::strategy strategy_type  = String2Strategy(js["strategy"].asString());

  BuildModule(CopyCollection<Electron>("CopyToExtraElecs",
      js["electrons"].asString(), "extra_elecs"));
 
 SimpleFilter<Electron> extraElecFilter = SimpleFilter<Electron>("ExtraElecFilter")
      .set_input_label("extra_elecs")
      .set_min(0).set_max(js["baseline"]["max_extra_elecs"].asUInt());
  if(strategy_type == strategy::paper2013){
      extraElecFilter.set_predicate([=](Electron const* e) {
        return  e->pt()                 > veto_elec_pt    &&
                fabs(e->eta())          < veto_elec_eta   &&
                fabs(e->dxy_vertex())   < veto_elec_dxy   &&
                fabs(e->dz_vertex())    < veto_elec_dz    &&
                ElectronHTTId(e, true)                    &&
                PF04IsolationVal(e, 0.5) <0.3;
      });
  }
 if(strategy_type == strategy::phys14){
      extraElecFilter.set_predicate([=](Electron const* e) {
        return  e->pt()                 > veto_elec_pt    &&
                fabs(e->eta())          < veto_elec_eta   &&
                fabs(e->dxy_vertex())   < veto_elec_dxy   &&
                fabs(e->dz_vertex())    < veto_elec_dz    &&
                ElectronHTTIdPhys14(e,true)             &&
                PF03IsolationVal(e, 0.5) < 0.3;
      });
  }

BuildModule(extraElecFilter);
 }


void HTTSequence::BuildExtraMuonVeto(){
  ic::strategy strategy_type  = String2Strategy(js["strategy"].asString());

  BuildModule(CopyCollection<Muon>("CopyToExtraMuons",
      js["muons"].asString(), "extra_muons"));

  SimpleFilter<Muon> extraMuonFilter = SimpleFilter<Muon>("ExtraMuonFilter")
      .set_input_label("extra_muons")
      .set_min(0).set_max(js["baseline"]["max_extra_muons"].asUInt());
  if(strategy_type == strategy::paper2013){
      extraMuonFilter.set_predicate([=](Muon const* m) {
        return  m->pt()                 > veto_muon_pt    &&
                fabs(m->eta())          < veto_muon_eta   &&
                fabs(m->dxy_vertex())   < veto_muon_dxy   &&
                fabs(m->dz_vertex())    < veto_muon_dz    &&
                MuonTight(m)                              &&
                PF04IsolationVal(m, 0.5) < 0.3;
      });
   } else if (strategy_type == strategy::phys14){
      extraMuonFilter.set_predicate([=](Muon const* m) {
        return  m->pt()                 > veto_muon_pt    &&
                fabs(m->eta())          < veto_muon_eta   &&
                fabs(m->dxy_vertex())   < veto_muon_dxy   &&
                fabs(m->dz_vertex())    < veto_muon_dz    &&
                MuonMedium(m)                     &&
                PF03IsolationVal(m, 0.5) < 0.3;
      });
   }

  BuildModule(extraMuonFilter);

 }

}
