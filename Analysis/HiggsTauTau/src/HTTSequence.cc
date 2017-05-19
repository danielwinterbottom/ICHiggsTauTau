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
//boost
#include <boost/format.hpp>
#include "boost/lexical_cast.hpp"
// Utilities
#include "Utilities/interface/FnRootTools.h"
// HTT-specific modules
#include "HiggsTauTau/interface/HTTSequence.h"
#include "HiggsTauTau/interface/HTTElectronEfficiency.h"
#include "HiggsTauTau/interface/HTTMuonEfficiency.h"
#include "HiggsTauTau/interface/HTTTauEfficiency.h"
#include "HiggsTauTau/interface/HTTTriggerFilter.h"
#include "HiggsTauTau/interface/HTTTriggerFilter2.h"
#include "HiggsTauTau/interface/HTTEnergyScale.h"
#include "HiggsTauTau/interface/HTTEMuExtras.h"
#include "HiggsTauTau/interface/HTTGenEvent.h"
#include "HiggsTauTau/interface/HTTCategories.h"
#include "HiggsTauTau/interface/WMuNuCategories.h"
#include "HiggsTauTau/interface/HTTPairSelector.h"
#include "HiggsTauTau/interface/HTTPairGenInfo.h"
#include "HiggsTauTau/interface/BTagCheck.h"
#include "HiggsTauTau/interface/SVFitTest.h"
#include "HiggsTauTau/interface/HTTRecoilCorrector.h"
#include "HiggsTauTau/interface/HTTRun2RecoilCorrector.h"
#include "HiggsTauTau/interface/HhhBJetRegression.h"
#include "HiggsTauTau/interface/HTTSync.h"
#include "HiggsTauTau/interface/HTTEMuMVA.h"
#include "HiggsTauTau/interface/HhhEMuMVA.h"
#include "HiggsTauTau/interface/HhhEMuMVABoth.h"
#include "HiggsTauTau/interface/HhhMTMVABoth.h"
#include "HiggsTauTau/interface/HhhMTMVACategory.h"
#include "HiggsTauTau/interface/HTTWeights.h"
#include "HiggsTauTau/interface/HTTStitching.h"
#include "HiggsTauTau/interface/HhhMetScale.h"
#include "HiggsTauTau/interface/EmbeddingKineReweightProducer.h"
#include "HiggsTauTau/interface/JetEnergyUncertainty.h"
#include "HiggsTauTau/interface/HTTPrint.h"
#include "HiggsTauTau/interface/HTTFilter.h"
#include "Modules/interface/LumiMask.h"
#include "HiggsTauTau/interface/VertexFilter.h"
#include "HiggsTauTau/interface/EffectiveEvents.h"
#include "HiggsTauTau/interface/NvtxWeight.h"
#include "HiggsTauTau/interface/BTagWeightRun2.h"
#include "HiggsTauTau/interface/HTTGenMatchSelector.h"
#include "HiggsTauTau/interface/HTTFakeFactorWeights.h"

// Generic modules
#include "Modules/interface/SimpleFilter.h"
#include "Modules/interface/CompositeProducer.h"
#include "Modules/interface/CopyCollection.h"
#include "Modules/interface/OneCollCompositeProducer.h"
#include "Modules/interface/OverlapFilter.h"
#include "Modules/interface/EnergyShifter.h"
#include "Modules/interface/PileupWeight.h"
#include "Modules/interface/CheckEvents.h"
#include "Modules/interface/GenericModule.h"
#include "HiggsTauTau/interface/NLOWeighting.h"

//Moj dodatak

#include "HiggsTauTau/interface/HTTVBFGenAnalysis.h"

namespace ic {

HTTSequence::HTTSequence(std::string& chan, std::string postf, Json::Value const& json) {
  addit_output_folder=json["baseline"]["addit_output_folder"].asString();
  new_svfit_mode = json["new_svfit_mode"].asUInt();
  if(new_svfit_mode > 0){
    if(json["svfit_folder"].asString()!="") {svfit_folder = json["svfit_folder"].asString();} else {std::cout<<"ERROR: svfit_folder not set"<<std::endl; exit(1);};
    svfit_folder=svfit_folder+"/"+addit_output_folder+"/";
  }
  svfit_override = json["svfit_override"].asString();
  if(json["output_name"].asString()!=""){output_name=json["output_name"].asString();} else{std::cout<<"ERROR: output_name not set"<<std::endl; exit(1);};
  if(json["output_folder"].asString()!=""){output_folder=json["output_folder"].asString();} else{std::cout<<"ERROR: output_folder not set"<<std::endl; exit(1);};
  addit_output_folder=json["baseline"]["addit_output_folder"].asString();
  output_folder=output_folder+"/"+addit_output_folder+"/";
  lumimask_output_name=output_name + "_" +chan + "_" + postf; 
  //output_name=chan + "_" +output_name +  "_" + var + "_" + postf + ".root";
  output_name=output_name + "_" + chan + "_" + postf + ".root";
  special_mode=json["special_mode"].asUInt();
  if (special_mode > 0) output_name = "Special_"+boost::lexical_cast<std::string>(special_mode)+"_" + output_name;
  //if(json["make_sync_ntuple"].asBool()) {
//	output_folder="HTTSequenceSyncfilesNEW/";
 //   output_name="SYNCFILE"+output_name;
 // }
  if(!json["make_sync_ntuple"].asBool()) {
      fs = std::make_shared<fwlite::TFileService>(
       (output_folder+output_name).c_str());
  } else {
      // do not create output file when making sync ntuples
      fs = NULL;
  }
  js = json;
  channel_str = chan;
  jes_mode=json["baseline"]["jes_mode"].asUInt();
  metscale_mode=json["baseline"]["metscale_mode"].asUInt();
  metres_mode=json["baseline"]["metres_mode"].asUInt();
  do_reshape=json["baseline"]["do_reshape"].asBool(); 
  btag_mode=json["baseline"]["btag_mode"].asUInt();
  bfake_mode=json["baseline"]["bfake_mode"].asUInt();
  if(js["mc"].asString()==""){std::cout<<"ERROR: MC type not set"<<std::endl; exit(1);}
  mc_str = js["mc"].asString();
  if(js["era"].asString()=="") {std::cout<<"ERROR: era not set"<<std::endl; exit(1);}
  era_str = js["era"].asString();
  if(js["strategy"].asString()==""){std::cout<<"ERROR: strategy not set"<<std::endl; exit(1);}
  strategy_str = js["strategy"].asString();
  ic::strategy strategy_type  = String2Strategy(strategy_str);
  ic::era  era_type  = String2Era(era_str);
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
     elec_dxy = 0.02;
     elec_dz = 0.1;
     muon_dxy = 0.02;
     muon_dz = 0.1;
     pair_dr = 0.3;
    if(era_type == era::data_2015 || era_type == era::data_2016){
       if(js["store_hltpaths"].asBool()){
         elec_dxy = 0.045;
         elec_dz = 0.2;
         muon_dxy = 0.045;
         muon_dz = 0.2;
         elec_pt = 6.;
         elec_eta = 2.5;
         muon_pt = 6.;
         muon_eta = 2.4;
       } else {
         elec_dxy = 0.045;
         elec_dz = 0.2;
         muon_dxy = 0.045;
         muon_dz = 0.2;
         elec_pt = 13.;
         elec_eta = 2.5;
         muon_pt = 10;
         muon_eta = 2.4;
       }
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
 if (channel_str == "et"){
   elec_dz = 0.2;
   elec_dxy = 0.045;
   muon_dxy = 0.045;
   muon_dz = 0.2;
   tau_dz = 0.2;
   pair_dr = 0.5;
  if(strategy_type == strategy::phys14){
    tau_anti_elec_discr = "againstElectronTightMVA5";
    tau_anti_muon_discr = "againstMuonLoose3";
    tau_iso_discr = "byCombinedIsolationDeltaBetaCorrRaw3Hits";
    tau_iso = 1.5;
   } else if(strategy_type == strategy::spring15){
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
   elec_pt = 24;//Up from 23 to avoid turn on in new data trigger for first set of runs from run D
   elec_eta = 2.1;
   tau_pt  = 20;
   tau_eta = 2.3;
   } else if (era_type == era::data_2016){
   if(js["store_hltpaths"].asBool()){
     min_taus = 1;
     pair_dr = 0.5;
     elec_pt = 20;
     elec_eta = 2.5;
     tau_pt  = 20;
     tau_eta = 2.3;
   } else{
     min_taus = 1;
     pair_dr = 0.5;
     elec_pt = 25;
     elec_eta = 2.1;
     tau_pt  = 20;
     tau_eta = 2.3;
   
     if (strategy_type == strategy::mssmspring16 || strategy_type == strategy::mssmsummer16){
       elec_pt = 26;
       tau_pt = 30;
      }
      if (strategy_type == strategy::smspring16){
       elec_pt = 26;
      }
   }

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
  pair_dr = 0.5;
  if(strategy_type == strategy::phys14){
    tau_anti_elec_discr = "againstElectronVLooseMVA5";
    tau_anti_muon_discr = "againstMuonTight3";
    tau_iso_discr = "byCombinedIsolationDeltaBetaCorrRaw3Hits";
    tau_iso = 1.5;
  } else if(strategy_type == strategy::spring15){
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
    muon_pt = 19.0;//Up from 18 to avoid trigger turn on in new data trigger
    muon_eta = 2.1;
    tau_pt = 20;
    tau_eta = 2.3;
    min_taus = 1;
    pair_dr = 0.5;
  } else if(era_type == era::data_2016){
    if(js["store_hltpaths"].asBool()){
      muon_pt = 16.0;
      muon_eta = 2.4;
      tau_pt = 20;
      tau_eta = 2.3;
      min_taus = 1;
      pair_dr = 0.5;    
    } else{
      muon_pt = 20.0; //cross trigger
      if (strategy_type == strategy::smspring16) muon_eta = 2.4;
      else muon_eta = 2.1;
      tau_pt = 20;
      tau_eta = 2.3;
      min_taus = 1;
      pair_dr = 0.5;
      if(strategy_type == strategy::mssmspring16 || strategy_type == strategy::mssmsummer16){
        tau_pt = 30;
        muon_pt = 23;
      }
      if (strategy_type == strategy::smspring16){
       muon_pt = 23;
      }
    }

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

  if (strategy_type == strategy::fall15 || strategy_type == strategy::mssmspring16 || strategy_type == strategy::smspring16 || strategy_type == strategy::mssmsummer16){
    tau_pt=40;
    if(js["store_hltpaths"].asBool()) tau_pt = 25;
  }

  tau_eta = 2.1;
  tau_iso = 1.0;
  tau_dz = 0.2;
  min_taus = 2;
  tau_iso_discr = "byCombinedIsolationDeltaBetaCorrRaw3Hits";
  tau_anti_elec_discr = "againstElectronVLooseMVA5";
  tau_anti_muon_discr = "againstMuonLoose3";
  pair_dr = 0.5;
  }
 if(channel_str == "zee"){
   elec_dz = 0.2;
   elec_dxy = 0.045;
   pair_dr = 0.5;
   elec_pt = 26;
   elec_eta = 2.1;
 }
 if(channel_str == "zmm"){
   muon_dz = 0.2;
   muon_dxy = 0.045;
   pair_dr = 0.3;
   muon_pt = 23;
   muon_eta = 2.1;
 }
 if(channel_str == "wmnu"){
   muon_dz = 0.2;
   muon_dxy = 0.045;
   pair_dr = 0.5;
   muon_pt = 20;
   muon_eta = 2.1;
 }
 if(channel_str == "tpzmm"){
   muon_dz = 0.5;
   muon_dxy = 0.2;
   pair_dr = 0.5;
   muon_pt = 10;
   muon_eta = 2.4;
 }
 if(channel_str == "tpzee"){
   elec_dz = 0.5;
   elec_dxy = 0.2;
   pair_dr = 0.5;
   elec_pt = 10;
   elec_eta = 2.5;
 }
 
 is_data      = json["is_data"].asBool();
 is_embedded  = json["is_embedded"].asBool();
 real_tau_sample = false; //This gets set properly later
 jets_label   = json["jets"].asString();
 met_label    = json["met"].asString();
 moriond_tau_scale = json["moriond_tau_scale"].asBool();
 pu_id_training = json["pu_id_training"].asUInt();
 bjet_regr_correction = json["bjet_regr_correction"].asBool();
 /*btag_mode    = json["btag_mode"].asUInt();
 bfake_mode   = json["bfake_mode"].asUInt();*/
 if(json["baseline"]["mass_scale_mode"].asBool()==true){
   mass_shift = json["baseline"]["mass_shift"].asDouble();
 } else mass_shift=1.00;
 kinfit_mode = json["kinfit_mode"].asUInt(); 
 mva_met_mode = json["mva_met_mode"].asUInt();
 do_met_filters = json["do_met_filters"].asBool();
 faked_tau_selector = json["faked_tau_selector"].asUInt();
 ztautau_mode = json["ztautau_mode"].asUInt();
 vh_filter_mode = json["vh_filter_mode"].asUInt();
 hadronic_tau_selector = json["hadronic_tau_selector"].asUInt(); 
 tau_scale_mode = json["baseline"]["tau_scale_mode"].asBool();
 //Need this to correctly set tau /elec ES
 if(channel_str!="em"){
 tau_shift = json["baseline"]["tau_es_shift"].asDouble();
 } else {
 tau_shift = 1.0;//Just to set it to something
 elec_shift_barrel = json["baseline"]["elec_es_shift_barrel"].asDouble();
 elec_shift_endcap = json["baseline"]["elec_es_shift_endcap"].asDouble();
 }



}

HTTSequence::~HTTSequence() {}


void HTTSequence::BuildSequence(){
  using ROOT::Math::VectorUtil::DeltaR;
  


  // Set global parameters that get used in multiple places
  ic::channel channel         = String2Channel(channel_str);
  ic::mc mc_type              = String2MC(mc_str);
  ic::era era_type            = String2Era(era_str);
  ic::strategy strategy_type  = String2Strategy(strategy_str);
  // Other flags

  real_tau_sample = ( (output_name.find("HToTauTau")             != output_name.npos)
                        || (output_name.find("HTohh")                 != output_name.npos)
                        || (output_name.find("AToZh")                 != output_name.npos)
                        || (output_name.find("DYJetsToTauTau")        != output_name.npos)
                        || (output_name.find("Embedded")              != output_name.npos)
                        || (output_name.find("RecHit")                != output_name.npos) 
                        || ((output_name.find("DY") != output_name.npos) && (output_name.find("JetsToLL") != output_name.npos)) );
  if (output_name.find("DYJetsToTauTau-L") != output_name.npos) real_tau_sample = false;
  if (output_name.find("DYJetsToTauTau-JJ") != output_name.npos) real_tau_sample = false;
  if (era_type == era::data_2016 && !is_data) real_tau_sample = ( (output_name.find("W") != output_name.npos) && (output_name.find("JetsToLNu") != output_name.npos)) ? false : true;
  if (channel == channel::zmm || channel == channel::zee) real_tau_sample = false;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "HiggsToTauTau Analysis" << std::endl;
  std::cout << "-------------------------------------" << std::endl;      std::string param_fmt = "%-25s %-40s\n";
//  std::cout << boost::format(param_fmt) % "max_events" % max_events;
  std::cout << boost::format(param_fmt) % "output" % (output_folder+output_name);
  std::cout << boost::format(param_fmt) % "strategy" % Strategy2String(strategy_type);
  std::cout << boost::format(param_fmt) % "era" % Era2String(era_type);
  std::cout << boost::format(param_fmt) % "mc" % MC2String(mc_type);
  std::cout << boost::format(param_fmt) % "channel" % channel_str;
  std::cout << boost::format(param_fmt) % "is_data" % is_data;
  std::cout << boost::format(param_fmt) % "is_embedded" % is_embedded;
  std::cout << boost::format(param_fmt) % "special_mode" % special_mode;
  std::cout << boost::format(param_fmt) % "tau_scale_mode" % tau_scale_mode;
//  std::cout << boost::format(param_fmt) % "mass_scale_mode" % mass_scale_mode;
  std::cout << boost::format(param_fmt) % "new_svfit_mode" % new_svfit_mode;
  std::cout << boost::format(param_fmt) % "vh_filter_mode" % vh_filter_mode;
  if (new_svfit_mode > 0) {
    std::cout << boost::format(param_fmt) % "svfit_folder" % svfit_folder;
    std::cout << boost::format(param_fmt) % "svfit_override" % svfit_override;
  }
  std::cout << boost::format(param_fmt) % "kinfit_mode" % kinfit_mode;
  std::cout << boost::format(param_fmt) % "ztautau_mode" % ztautau_mode;
  std::cout << boost::format(param_fmt) % "faked_tau_selector" % faked_tau_selector;
  std::cout << boost::format(param_fmt) % "hadronic_tau_selector" % hadronic_tau_selector;
  std::cout << boost::format(param_fmt) % "mva_met_mode" % mva_met_mode;
  std::cout << boost::format(param_fmt) % "make_sync_ntuple" % js["make_sync_ntuple"].asBool();
  std::cout << boost::format(param_fmt) % "save_output_jsons" % js["save_output_jsons"].asBool();
  std::cout << boost::format(param_fmt) % "allowed_tau_modes" % allowed_tau_modes;
  std::cout << boost::format(param_fmt) % "moriond_tau_scale" % moriond_tau_scale;
//  std::cout << boost::format(param_fmt) % "large_tscale_shift" % large_tscale_shift;
  std::cout << boost::format(param_fmt) % "pu_id_training" % pu_id_training;
//  std::cout << boost::format(param_fmt) % "make_gen_plots" % make_gen_plots;
  std::cout << boost::format(param_fmt) % "bjet_regr_correction" % bjet_regr_correction;
  std::cout << "** Kinematics **" << std::endl;
  std::cout << boost::format(param_fmt) % "elec_pt" % elec_pt;
  std::cout << boost::format(param_fmt) % "elec_eta" % elec_eta;
  std::cout << boost::format(param_fmt) % "elec_dxy" % elec_dxy;
  std::cout << boost::format(param_fmt) % "elec_dz" % elec_dz;
  std::cout << boost::format(param_fmt) % "muon_pt" % muon_pt;
  std::cout << boost::format(param_fmt) % "muon_eta" % muon_eta;
  std::cout << boost::format(param_fmt) % "muon_dxy" % muon_dxy;
  std::cout << boost::format(param_fmt) % "muon_dz" % muon_dz;
  std::cout << boost::format(param_fmt) % "tau_pt" % tau_pt;
  std::cout << boost::format(param_fmt) % "tau_eta" % tau_eta;
  std::cout << boost::format(param_fmt) % "tau_dz" % tau_dz;
  std::cout << boost::format(param_fmt) % "veto_elec_pt" % veto_elec_pt;
  std::cout << boost::format(param_fmt) % "veto_elec_eta" % veto_elec_eta;
  std::cout << boost::format(param_fmt) % "veto_elec_dxy" % veto_elec_dxy;
  std::cout << boost::format(param_fmt) % "veto_elec_dz" % veto_elec_dz;
  std::cout << boost::format(param_fmt) % "veto_dielec_pt" % veto_dielec_pt;
  std::cout << boost::format(param_fmt) % "veto_dielec_eta" % veto_dielec_eta;
  std::cout << boost::format(param_fmt) % "veto_dielec_dxy" % veto_dielec_dxy;
  std::cout << boost::format(param_fmt) % "veto_dielec_dz" % veto_dielec_dz;
  std::cout << boost::format(param_fmt) % "veto_muon_pt" % veto_muon_pt;
  std::cout << boost::format(param_fmt) % "veto_muon_eta" % veto_muon_eta;
  std::cout << boost::format(param_fmt) % "veto_muon_dxy" % veto_muon_dxy;
  std::cout << boost::format(param_fmt) % "veto_muon_dz" % veto_muon_dz;
  std::cout << boost::format(param_fmt) % "veto_dimuon_pt" % veto_dimuon_pt;
  std::cout << boost::format(param_fmt) % "veto_dimuon_eta" % veto_dimuon_eta;
  std::cout << boost::format(param_fmt) % "veto_dimuon_dxy" % veto_dimuon_dxy;
  std::cout << boost::format(param_fmt) % "veto_dimuon_dz" % veto_dimuon_dz;
  std::cout << "** Tau Discriminators **" << std::endl;
  std::cout << boost::format(param_fmt) % "isolation" %  tau_iso_discr;
  std::cout << boost::format(param_fmt) % "isolation cut" %  tau_iso;
  std::cout << boost::format(param_fmt) % "anti-electron" % tau_anti_elec_discr;
  std::cout << boost::format(param_fmt) % "anti-muon" % tau_anti_muon_discr;

  auto eventChecker = CheckEvents("EventChecker").set_skip_events(true);
  std::vector<int> to_check =
  {
  };   

	if(strcmp((js["event_check_file"].asString()).c_str(),"")!=0){
    std::ifstream file;
    file.open((js["event_check_file"].asString()).c_str());
    if (!file.is_open()) {
      std::cerr << "Warning: File " << js["event_check_file"].asString() << " cannot be opened." << std::endl;
    }
		int nums;
		while(file >> nums){
		  to_check.push_back(nums);
		}

  	//std::vector<int> addit_to_check(std::istream_iterator<int>(file), std::istream_iterator<int>());
    file.close();
		//for(unsigned j=0;j<addit_to_check.size();j++){
  //		to_check.push_back(addit_to_check.at(j));
	 // }
   } 


// Defining good-lumi jsons
  std::string data_json = "";
  if (era_type == era::data_2011) {
    if (channel == channel::em) {
      data_json = "input/json/json_data_2011.txt";
    } else{
      data_json = "input/json/json_data_2011_et_mt.txt";
    }
  }             
  if (era_type == era::data_2012_rereco)       data_json = "input/json/data_2012_rereco.txt";
  //if (era_type == era::data_2015)  data_json= "input/json/data_2015_prompt_1507151716.txt";
  if (era_type == era::data_2015&&output_name.find("2015C")!=output_name.npos)  data_json= "input/json/Cert_246908-255031_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt";
  if (era_type == era::data_2015&&output_name.find("2015B")!=output_name.npos)  data_json= "input/json/Cert_246908-255031_13TeV_PromptReco_Collisions15_50ns_JSON_v2.txt";
  //if (era_type == era::data_2015&&output_name.find("2015D")!=output_name.npos)  data_json= "input/json/Cert_246908-258750_13TeV_PromptReco_Collisions15_25ns_JSON.txt";  //json used for datacard sync v1
  if (era_type == era::data_2015&&output_name.find("2015D")!=output_name.npos)  data_json= "input/json/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt";
  if (era_type == era::data_2016){
    data_json= "input/json/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt";
    if(strategy_type == strategy::mssmsummer16) data_json= "input/json/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt";
  }


 if(js["get_effective"].asBool() && js["make_sync_ntuple"].asBool()){
   std::cerr<< "Error: cannot run effective number of event module in make_syncntuple mode"<<std::endl;
   throw;
 }
 if(js["get_effective"].asBool()){
  BuildModule(EffectiveEvents("EffectiveEvents")
    .set_fs(fs.get()));
/*  BuildModule(HTTElectronEfficiency("ElectronEfficiency")
    .set_fs(fs.get()));*/
  }else if(is_data && js["lumi_mask_only"].asBool()){
   //LumiMask module  just for the mode in which we only write out the 
   //output jsons
   LumiMask lumiMask = LumiMask("LumiMask")
     .set_produce_output_jsons(lumimask_output_name.c_str())
     .set_input_file(data_json);
 
    BuildModule(lumiMask);
  }else if(js["gen_stitching_study"].asBool()){
        
    if((strategy_type ==strategy::fall15)&&channel!=channel::wmnu){
        HTTStitching httStitching = HTTStitching("HTTStitching")  
        .set_era(era_type)
        .set_fs(fs.get());
      if ((output_name.find("DY") != output_name.npos && output_name.find("JetsToLL_M-50") != output_name.npos) || output_name.find("DYJetsToLL_M-150-LO")!=output_name.npos){
        /*httWeights.set_do_dy_soup_high_mass(true);
        httWeights.SetDYInputCrossSectionsHighMass(4954, 1012.5, 332.8, 101.8,54.8,6.7); //Target fractions are xs_n-jet/xs_inclusive
        httWeights.SetDYInputYieldsHighMass(239058696,65314144 , 20019059, 5701878, 4189017, 6079415);*/
        //Removing high mass sample:
        httStitching.set_do_dy_soup(true);
        httStitching.SetDYInputCrossSections(4954, 1012.5, 332.8, 101.8,54.8); //Target fractions are xs_n-jet/xs_inclusive
        httStitching.SetDYInputYields(238776168,65314144 , 20019059, 5701878, 4189017);
      }

      if (output_name.find("WJetsToLNu-LO") != output_name.npos || output_name.find("W1JetsToLNu-LO") != output_name.npos || output_name.find("W2JetsToLNu-LO") != output_name.npos ||
           output_name.find("W3JetsToLNu-LO") != output_name.npos || output_name.find("W4JetsToLNu-LO") != output_name.npos){
        httStitching.set_do_w_soup(true);
        httStitching.SetWInputCrossSections(50380,9644.5,3144.5,954.8,485.6);
        httStitching.SetWInputYields(47101324,45442170,30190119,18007936,8815779);
      }
       BuildModule(httStitching); 

    } 
    
    if((strategy_type ==strategy::mssmspring16||strategy_type == strategy::smspring16)&&channel!=channel::wmnu){
        HTTStitching httStitching = HTTStitching("HTTStitching")  
        .set_era(era_type)
        .set_fs(fs.get());
         if (output_name.find("WJetsToLNu-LO") != output_name.npos || output_name.find("W1JetsToLNu-LO") != output_name.npos || output_name.find("W2JetsToLNu-LO") != output_name.npos ||
           output_name.find("W3JetsToLNu-LO") != output_name.npos || output_name.find("W4JetsToLNu-LO") != output_name.npos){
        httStitching.set_do_w_soup(true);
        httStitching.SetWInputCrossSections(50380,9644.5,3144.5,954.8,485.6);
        httStitching.SetWInputYields(28210360,39855520,29884200,19869053,9174756);
       }
       if ((output_name.find("DY") != output_name.npos && output_name.find("JetsToLL-LO") != output_name.npos && !(output_name.find("JetsToLL-LO-10-50") != output_name.npos))){
         httStitching.set_do_dy_soup(true);
         httStitching.SetDYInputCrossSections(4954, 1012.5, 332.8, 101.8,54.8); //Target fractions are xs_n-jet/xs_inclusive
         httStitching.SetDYInputYields(49877138,65485168 , 19695514, 5753813, 4115140);
       }
   
       BuildModule(httStitching); 
    }
      
    if(strategy_type == strategy::mssmsummer16&&channel!=channel::wmnu){
        HTTStitching httStitching = HTTStitching("HTTStitching")  
        .set_era(era_type)
        .set_fs(fs.get());
         if (output_name.find("WJetsToLNu-LO") != output_name.npos || output_name.find("W1JetsToLNu-LO") != output_name.npos || output_name.find("W2JetsToLNu-LO") != output_name.npos ||
           output_name.find("W3JetsToLNu-LO") != output_name.npos || output_name.find("W4JetsToLNu-LO") != output_name.npos){
        httStitching.set_do_w_soup(true);
        httStitching.SetWInputCrossSections(50380,9644.5,3144.5,954.8,485.6);
        httStitching.SetWInputYields(57026058,45367044,29878415,19798117,18751462);
       }
       if ((output_name.find("DY") != output_name.npos && output_name.find("JetsToLL-LO") != output_name.npos && !(output_name.find("JetsToLL-LO-10-50") != output_name.npos))){
         httStitching.set_do_dy_soup(true);
         httStitching.SetDYInputCrossSections(4954, 1012.5, 332.8, 101.8,54.8); //Target fractions are xs_n-jet/xs_inclusive
         httStitching.SetDYInputYields(96658943,62627174, 19970551, 5856110, 4197868);
       }
   
       BuildModule(httStitching);

    }
    
  
  } else{

  HTTPrint httPrint("HTTPrint");
  if(era_type==era::data_2015 || era_type==era::data_2016){
    httPrint.set_muon_label(js["muons"].asString());
    httPrint.set_jet_label(jets_label);
  }
  for (auto ch : to_check) {
   eventChecker.CheckEvent(ch);
   httPrint.PrintEvent(ch);
  }
  httPrint.set_skip_events(false);
  if (to_check.size() > 0){
  BuildModule(eventChecker);
  BuildModule(httPrint);  
}

//Moj dodatak

BuildModule(HTTVBFGenAnalysis("HTTVBFGenAnalysis")
        .set_fs(fs.get())
        .set_channel_str(channel_str)
        .set_min_jet_pt(30.)
        .set_max_jet_eta(4.7)
        .set_min_e_pt(0)
        .set_min_mu_pt(0)
        .set_min_tau1_pt(0)
        .set_min_tau2_pt(0)
        .set_max_e_eta(1000)
        .set_max_mu_eta(1000)
        .set_max_tau_eta(1000)
        .set_do_theory_uncert(false)
        );

  }
 }
}
