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
#include "HiggsTauTau/interface/HTTGenAnalysis.h"

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
 do_qcd_scale_wts_=false;
 do_qcd_scale_wts_ = js["do_qcd_scale_wts"].asBool();
 if (output_name.find("SUSYGluGluToBBHToTauTau_M") != output_name.npos && output_name.find("NLO") != output_name.npos) do_qcd_scale_wts_=true;
 
 
 is_data      = json["is_data"].asBool();
 is_embedded  = json["is_embedded"].asBool();
 real_tau_sample = false; //This gets set properly later
 jlepton_fake = false;
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
 if(channel_str=="zmm") muon_shift = json["baseline"]["muon_es_shift"].asDouble();
 else muon_shift = 1.0;
 if(channel_str!="em"){
 tau_shift = json["baseline"]["tau_es_shift"].asDouble();
 } else {
 tau_shift = 1.0;//Just to set it to something
 elec_shift_barrel = json["baseline"]["elec_es_shift_barrel"].asDouble();
 elec_shift_endcap = json["baseline"]["elec_es_shift_endcap"].asDouble();
 }
 tau_shift_1prong0pi0 = 1.0;
 tau_shift_1prong1pi0 = 1.0;
 tau_shift_3prong0pi0 = 1.0;
 fakeE_tau_shift_0pi = 1.0;
 fakeE_tau_shift_1pi = 1.0;
 if(strategy_type==strategy::mssmsummer16){
   fakeE_tau_shift_0pi = json["baseline"]["efaketau_0pi_es_shift"].asDouble();
   fakeE_tau_shift_1pi = json["baseline"]["efaketau_1pi_es_shift"].asDouble();
   tau_shift_1prong0pi0 = json["baseline"]["tau_1prong0pi0_es_shift"].asDouble();
   tau_shift_1prong1pi0 = json["baseline"]["tau_1prong1pi0_es_shift"].asDouble();
   tau_shift_3prong0pi0 = json["baseline"]["tau_3prong0pi0_es_shift"].asDouble();
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
  if (channel == channel::em && strategy_type ==strategy::mssmsummer16){
    //Apply jet->lepton fake rates?
    if( (output_name.find("W") != output_name.npos) && (output_name.find("JetsToLNu") != output_name.npos)) jlepton_fake = true; //Applied for W+Jets...
    if( (output_name.find("VV") != output_name.npos) || (output_name.find("ZZ") != output_name.npos) || (output_name.find("WZ") !=output_name.npos) || (output_name.find("WW") != output_name.npos)) jlepton_fake = true; //Applied for diboson
    if( (output_name.find("DY") != output_name.npos) && (output_name.find("JetsToLL") != output_name.npos)) jlepton_fake = true; //Applied for DY
  }

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
    .set_fs(fs.get())
    .set_do_qcd_scale_wts(do_qcd_scale_wts_).set_do_pdf_wts(js["do_pdf_wts"].asBool()));
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


 if(output_name.find("WZJetsTo3LNu") != output_name.npos && (mc_type == mc::fall15_76X)){ 
  BuildModule(GenericModule("lowGenMLLEventVeto")
   .set_function([](ic::TreeEvent *event){
     std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>("lheParticles");
     std::vector<GenParticle *> sel_particles;
     for (unsigned i=0; i < particles.size(); ++i){
       if(particles[i]->status() != 1) continue;
       unsigned id = abs(particles[i]->pdgid());
       if( id == 11 || id == 13 || id == 15){
        sel_particles.push_back(particles[i]);
      }
    }
    bool skip_evt=false;
    for(unsigned i = 0; i<sel_particles.size(); ++i){
      for(unsigned j = 0; j<i; ++j){
        if(skip_evt==false && sel_particles[i]->pdgid()==-sel_particles[j]->pdgid()){
          if((sel_particles[i]->vector()+sel_particles[j]->vector()).M() < 30)  skip_evt=true;
        }
      }
    }
    if(skip_evt) return 1;
    else return 0;
   }));
  }


if(js["test_nlo_reweight"].asBool()) {
  nloweights::ReadFile();
  BuildModule(NLOWeighting("NLOWeights")
    .set_fs(fs.get()));
}

if(!is_data && js["do_gen_analysis"].asBool()){
   BuildModule(CopyCollection<PFJet>("CopyFilteredJets",jets_label,jets_label+"UnFiltered"));

SimpleFilter<PFJet> jetIDFilter = SimpleFilter<PFJet>("JetIDFilter")
.set_input_label(jets_label);
jetIDFilter.set_predicate(bind(PFJetID2016, _1));

TH2F bbtag_eff;
TH2F cbtag_eff;
TH2F othbtag_eff;
bbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_Moriond2017.root","/","btag_eff_b");
cbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_Moriond2017.root","/","btag_eff_c");
othbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_Moriond2017.root","/","btag_eff_oth");

BuildModule(BTagWeightRun2("BTagWeightRun2")
 .set_channel(channel::mt)
 .set_era(era_type)
 .set_strategy(strategy_type)
 .set_jet_label(jets_label)
 .set_bbtag_eff(new TH2F(bbtag_eff))
 .set_cbtag_eff(new TH2F(cbtag_eff))
 .set_othbtag_eff(new TH2F(othbtag_eff))
 .set_do_reshape(do_reshape)
 .set_btag_mode(btag_mode)
 .set_bfake_mode(bfake_mode));

BuildModule(jetIDFilter);
  TH2F btag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_Moriond2017.root","/","btag_eff_b");
  BuildModule(HTTGenAnalysis("HTTGenAnalysis")
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
    .set_do_theory_uncert(true)
    .set_bbtag_eff(new TH2F(btag_eff))
  );
  return;  
}


/*  BuildModule(GenericModule("checkGoodVertices")
    .set_function([](ic::TreeEvent *event){
       std::vector<ic::Vertex*> vertices = event->GetPtrVec<ic::Vertex>("vertices");
       bool is_good_vertex = GoodVertex(vertices.at(0));
       event->Add("good_first_vertex",is_good_vertex);
       //if(is_good_vertex) return 1;
       //else return 0;
       return 0;
    }));*/
       
/*  BuildModule(GenericModule("checkGenVertices")
    .set_function([](ic::TreeEvent *event){
       std::vector<ic::Vertex*> vertices = event->GetPtrVec<ic::Vertex>("vertices");
       std::vector<ic::Vertex*> genvertices = event->GetPtrVec<ic::Vertex>("genVertices");
       bool pv_is_gen = false;
       bool is_good_vertex = GoodVertex(vertices.at(0));
       if (fabs(vertices.at(0)->vz()-genvertices.at(0)->vz())<0.01&&fabs(vertices.at(0)->vy()-genvertices.at(0)->vy())<0.01&&fabs(vertices.at(0)->vx()-genvertices.at(0)->vx())<0.01){
           pv_is_gen=true;
         } 
       if(!pv_is_gen) return 1;
       //if(is_good_vertex) return 1;
       else return 0;
       //return 0;
    }));

  BuildModule(GenericModule("checkGoodVerticesAfterGen")
    .set_function([](ic::TreeEvent *event){
       std::vector<ic::Vertex*> vertices = event->GetPtrVec<ic::Vertex>("vertices");
       bool is_good_vertex = GoodVertex(vertices.at(0));
  //     event->Add("good_first_vertex",is_good_vertex);
       if(!is_good_vertex) return 1;
       else return 0;
       //return 0;
    }));
*/



  // If desired, run the HTTGenEventModule which will add some handily-
  // formatted generator-level info into the Event
  if (js["run_gen_info"].asBool()) {
    BuildModule(HTTGenEvent("HTTGenEvent")
        .set_genparticle_label(js["genTaus"].asString())
        .set_genjet_label(js["genJets"].asString()));
  }


 //Build LumiMask module here - json file definition eralier in the code to be able to 
 //run *just* this module
 if(is_data && strategy_type!=strategy::phys14){
   LumiMask lumiMask = LumiMask("LumiMask")
     .set_produce_output_jsons("")
     .set_input_file(data_json);
 
 if(js["save_output_jsons"].asBool()){
  lumiMask.set_produce_output_jsons(lumimask_output_name.c_str());
   }
  BuildModule(lumiMask);
 }

if((strategy_type == strategy::fall15 || strategy_type ==strategy::mssmsummer16) && output_name.find("WGToLNuG")!=output_name.npos){
  SimpleCounter<GenParticle> wgammaStarFilter = SimpleCounter<GenParticle>("WgammaStarSelector")
    .set_input_label("genParticles")
    .set_predicate(
      (bind(&GenParticle::status,_1) == 44) &&
      (bind(::abs,(bind(&GenParticle::pdgid, _1))) == 22))
    .set_min(0).set_max(0);
 
  BuildModule(wgammaStarFilter);
}


if(ztautau_mode > 0 && strategy_type != strategy::spring15 && strategy_type != strategy::fall15 && strategy_type != strategy::mssmspring16 && strategy_type != strategy::smspring16 && strategy_type != strategy::mssmsummer16){
  SimpleCounter<GenParticle> zTauTauFilter = SimpleCounter<GenParticle>("ZToTauTauSelector")
    .set_input_label("genParticles")
    .set_predicate(
      (bind(&GenParticle::status,_1) == 3) &&
      (bind(::abs,(bind(&GenParticle::pdgid, _1))) == 15))
    .set_min(2);
  if (ztautau_mode == 2) zTauTauFilter.set_min(0).set_max(0);
 
  BuildModule(zTauTauFilter);
}

if(vh_filter_mode > 0 && strategy_type==strategy::paper2013){

  auto wh_selector = [](GenParticle const* p) -> bool {
    if (p->status() == 3 && std::abs(p->pdgid()) == 24) {
      return true;
    } else {
      return false;
    }
  };

  SimpleCounter<GenParticle> vhFilter = SimpleCounter<GenParticle>("VHFilter")
    .set_input_label("genParticles")
    .set_predicate(wh_selector)
    .set_min(0).set_max(0);
  if (vh_filter_mode == 1) vhFilter.set_predicate(wh_selector).set_min(1).set_max(1); // Exactly one W boson => WH
  if (vh_filter_mode == 2) vhFilter.set_predicate(wh_selector).set_min(2).set_max(999); // Two W bosons => ttH
  if (vh_filter_mode == 3) vhFilter.set_predicate(wh_selector).set_min(0).set_max(0); // At least one Z boson => ZH

 BuildModule(vhFilter);

}

  double mssm_mass = 0.0;
  bool do_mass_filter = false;
  if ((output_name.find("SUSYGluGluToHToTauTau") != output_name.npos) || 
      (output_name.find("SUSYBBHToTauTau") != output_name.npos)) {
    std::size_t pos = output_name.find("_M-");
    if (pos != output_name.npos) {
      std::string mass_string;
      for (unsigned i = (pos+3); i < output_name.size(); ++i) {
        if (output_name.at(i) != '_') mass_string += output_name.at(i);
        if (output_name.at(i) == '_') break;
      }
  //    std::cout<<mass_string<<std::endl;
//      mssm_mass = boost::lexical_cast<double>(mass_string);
      do_mass_filter = true;
      std::cout << "** MSSM Signal Sample **" << std::endl;
 //     std::cout << boost::format(param_fmt) % "mssm_mass" % mssm_mass;
    }
  }
 
 if(do_mass_filter==true && strategy_type==strategy::paper2013){
 BuildModule(SimpleCounter<GenParticle>("MSSMMassFilter")
    .set_input_label("genParticles")
    .set_predicate(bind(GenParticleInMassBand, _1, 36, 0.7*mssm_mass, 1.3*mssm_mass))
    .set_min(1));
  }



 if(is_embedded){
  BuildModule(SimpleCounter<GenParticle>("EmbeddedMassFilter")
    .set_input_label("genParticlesEmbedded")
    .set_predicate(bind(GenParticleInMassBand, _1, 23, 50., 9999999.))
    .set_min(1));
 }


  if (channel == channel::et) BuildETPairs();
  if (channel == channel::mt) BuildMTPairs();
  if (channel == channel::em) BuildEMPairs();
  if (channel == channel::tt) BuildTTPairs();
  if (channel == channel::zee) BuildZEEPairs();
  if (channel == channel::zmm) BuildZMMPairs();
  if (channel == channel::wmnu) BuildWMuNu();
  if (channel == channel::tpzee) BuildTPZEEPairs();
  if (channel == channel::tpzmm) BuildTPZMMPairs();

  // Pair DeltaR filtering
if( channel !=channel::wmnu) {
BuildModule(SimpleFilter<CompositeCandidate>("PairFilter")
      .set_input_label("ditau").set_min(1)
      .set_predicate([=](CompositeCandidate const* c) {
        return DeltaR(c->at(0)->vector(), c->at(1)->vector())
            > pair_dr;
      }));

//if(channel == channel::zmm){
//  // filters dimuon pairs if muons are in the same endcap with dPhi < 70 degrees
//  BuildModule(SimpleFilter<CompositeCandidate>("MuonPairDPhiFilter")
//    .set_input_label("ditau").set_min(1)
//    .set_predicate([=](CompositeCandidate const* c) {
//      Candidate *m1 = c->GetCandidate("lepton1");
//      Candidate *m2 = c->GetCandidate("lepton2");
//      return !(fabs(m1->eta()) > 0.9 && fabs(m2->eta()) > 0.9 && m1->eta()*m2->eta() > 0 && fabs(ROOT::Math::VectorUtil::DeltaPhi(m1->vector(),m2->vector())) < M_PI*7/18);
//    }));
//}

  // Trigger filtering
//    if (js["run_trg_filter"].asBool()) {
// if(is_data){

   if(channel != channel::wmnu) {

   if(channel != channel::tpzmm &&channel !=channel::tpzee && !js["qcd_study"].asBool()){  
     if((is_data || js["trg_in_mc"].asBool()) && (strategy_type!=strategy::mssmsummer16 || js["filter_trg"].asBool()) &&(channel==channel::em || channel==channel::tt || js["do_leptonplustau"].asBool()||js["do_singlelepton"].asBool())){
       if(!is_embedded || (is_embedded && strategy_type==strategy::paper2013 && era_type==era::data_2012_rereco)){
           BuildModule(HTTTriggerFilter("HTTTriggerFilter")
               .set_channel(channel)
               .set_mc(mc_type)
               .set_era(era_type)
               .set_strategy(strategy_type)
               .set_is_data(is_data)
               .set_is_embedded(is_embedded)
               .set_do_leptonplustau(js["do_leptonplustau"].asBool())
               .set_do_singlelepton(js["do_singlelepton"].asBool())
               .set_do_singletau(js["do_singletau"].asBool())
               .set_do_filter(true)
               .set_pair_label("ditau"));
       }
     }
   }
   
     HTTPairSelector httPairSelector = HTTPairSelector("HTTPairSelector")
       .set_channel(channel)
       .set_fs(fs.get())
       .set_pair_label("ditau")
       .set_met_label(met_label)
       .set_strategy(strategy_type)
       .set_mva_met_from_vector(mva_met_mode==1)
       .set_faked_tau_selector(faked_tau_selector)
       .set_hadronic_tau_selector(hadronic_tau_selector)
       .set_ztt_mode(ztautau_mode)
       .set_gen_taus_label(is_embedded ? "genParticlesEmbedded" : "genParticlesTaus")
       .set_scale_met_for_tau((tau_scale_mode > 0 || (moriond_tau_scale && (is_embedded || !is_data) )   ))
       .set_tau_scale(tau_shift)
       .set_use_most_isolated((strategy_type != strategy::paper2013) && (!(channel == channel::zee || channel == channel::zmm || channel == channel::tpzmm || channel == channel::tpzee)))
       .set_use_os_preference((strategy_type == strategy::paper2013) || (channel == channel::zee || channel == channel::zmm || channel == channel::tpzmm || channel == channel::tpzee))
       .set_allowed_tau_modes(allowed_tau_modes);
   
     if(strategy_type == strategy::spring15 || strategy_type == strategy::fall15 || strategy_type == strategy::mssmspring16 || strategy_type == strategy::smspring16 || strategy_type == strategy::mssmsummer16){
       httPairSelector.set_gen_taus_label("genParticles");
     }
     
     BuildModule(httPairSelector);
   }
   
   if(js["store_hltpaths"].asBool()){

     if(is_data || js["trg_in_mc"].asBool()){  
         
       BuildModule(HTTTriggerFilter2("HTTTriggerFilter2")
           .set_channel(channel)
           .set_mc(mc_type)
           .set_era(era_type)
           .set_is_data(is_data)
           .set_pair_label("ditau"));

     }
   } else {
   if(channel != channel::tpzmm &&channel !=channel::tpzee && !js["qcd_study"].asBool()){  
     if((is_data || js["trg_in_mc"].asBool()) && (strategy_type==strategy::mssmsummer16 && !js["filter_trg"].asBool())&& (channel==channel::em || channel==channel::tt || js["do_leptonplustau"].asBool()||js["do_singlelepton"].asBool())){
       if(!is_embedded || (is_embedded && strategy_type==strategy::paper2013 && era_type==era::data_2012_rereco)){
           BuildModule(HTTTriggerFilter("HTTTriggerFilter")
               .set_channel(channel)
               .set_mc(mc_type)
               .set_era(era_type)
               .set_strategy(strategy_type)
               .set_is_data(is_data)
               .set_is_embedded(is_embedded)
               .set_do_leptonplustau(js["do_leptonplustau"].asBool())
               .set_do_singlelepton(js["do_singlelepton"].asBool())
               .set_do_singletau(js["do_singletau"].asBool())
               .set_do_filter(false)
               .set_pair_label("ditau"));
       }
     }
   }
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
  if (js["do_pu_wt"].asBool()&&!is_data) {
    BuildModule( PileupWeight("PileupWeight")
        .set_data(new TH1D(d_pu)).set_mc(new TH1D(m_pu)));
  }
}

/*if(strategy_type == strategy::spring15 && js["do_pu_wt"].asBool() &&!is_data){
   TH1F vertex_wts = GetFromTFile<TH1F>(js["nvtx_weight_file"].asString(),"/","nvtx_weights");
   BuildModule(NvtxWeight("NvtxWeight")
       .set_vertex_dist(new TH1F(vertex_wts)));
 }*/

if(do_met_filters && is_data){
  BuildModule(GenericModule("MetFilters")
    .set_function([=](ic::TreeEvent *event){
       EventInfo *eventInfo = event->GetPtr<EventInfo>("eventInfo");
       std::vector<std::string> met_filters = {"Flag_eeBadScFilter"};
       bool pass_filters = true;
       for(unsigned i=0;i<met_filters.size();++i){
        pass_filters = pass_filters&& eventInfo->filter_result(met_filters.at(i));
       }
       return !pass_filters;
    }));
}
if(do_met_filters){
  BuildModule(GenericModule("MetFiltersRecoEffect")
    .set_function([=](ic::TreeEvent *event){
       EventInfo *eventInfo = event->GetPtr<EventInfo>("eventInfo");
       std::vector<std::string> met_filters = {"Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_goodVertices", "badChargedHadronFilter","badMuonFilter", "Flag_globalTightHalo2016Filter"};
       bool pass_filters = true;
       for(unsigned i=0;i<met_filters.size();++i){
        pass_filters = pass_filters&& eventInfo->filter_result(met_filters.at(i));
       }
       return !pass_filters;
    }));
}

BuildModule(GenericModule("BadMuonFilters")
  .set_function([=](ic::TreeEvent *event){
     EventInfo *eventInfo = event->GetPtr<EventInfo>("eventInfo");
     std::vector<std::string> bad_muon_filters = {"Flag_badMuons","Flag_duplicateMuons"};
     bool pass_filters = true;
     for(unsigned i=0;i<bad_muon_filters.size();++i){
      pass_filters = pass_filters&& eventInfo->filter_result(bad_muon_filters.at(i));
     }
     return !pass_filters;
  }));



if(channel == channel::tpzmm || channel == channel::tpzee){
  BuildModule(GenericModule("TPTriggerInformation")
    .set_function([=](ic::TreeEvent *event){
       std::string trig_obj_label_tag;
       std::string tp_filter_tag;
       std::string trig_obj_label_probe;
       std::string tp_filter_probe;
       if(channel_str == "tpzmm"){ 
         if(!is_data){
           trig_obj_label_tag = "triggerObjectsIsoMu17";
           tp_filter_tag = "hltL3crIsoL1sSingleMu16erL1f0L2f10QL3f17QL3trkIsoFiltered0p09";
           trig_obj_label_probe = "triggerObjectsIsoMu17";
           tp_filter_probe = "hltL3crIsoL1sSingleMu16erL1f0L2f10QL3f17QL3trkIsoFiltered0p09";
         } else {
           if(strategy_type != strategy::mssmspring16 && strategy_type != strategy::smspring16 && strategy_type != strategy::mssmsummer16){
             trig_obj_label_tag = "triggerObjectsIsoMu18";
             tp_filter_tag = "hltL3crIsoL1sMu16L1f0L2f10QL3f18QL3trkIsoFiltered0p09"; 
             trig_obj_label_probe = "triggerObjectsIsoMu18";
             tp_filter_probe = "hltL3crIsoL1sMu16L1f0L2f10QL3f18QL3trkIsoFiltered0p09"; 
           } else {
             trig_obj_label_tag = "triggerObjectsIsoMu22";
             tp_filter_tag = "hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09";
             trig_obj_label_probe = "triggerObjectsIsoMu19LooseTau20SingleL1";
             tp_filter_probe = "hltL3crIsoL1sSingleMu18erIorSingleMu20erL1f0L2f10QL3f19QL3trkIsoFiltered0p09";
           }
         }
       } else if (channel_str == "tpzee"){
         if(!is_data){
           trig_obj_label_tag = "triggerObjectsEle22Gsf";
           tp_filter_tag = "hltSingleEle22WP75GsfTrackIsoFilter";
           trig_obj_label_probe = "triggerObjectsEle22Gsf";
           tp_filter_probe = "hltSingleEle22WP75GsfTrackIsoFilter";
         } else {
           if(strategy_type != strategy::mssmspring16 && strategy_type != strategy::smspring16 && strategy_type != strategy::mssmsummer16){
             trig_obj_label_tag = "triggerObjectsEle23";
             tp_filter_tag = "hltEle23WPLooseGsfTrackIsoFilter";
             trig_obj_label_probe = "triggerObjectsEle23";
             tp_filter_probe = "hltEle23WPLooseGsfTrackIsoFilter";
            } else {
             trig_obj_label_tag = "triggerObjectsEle25GsfTightEta2p1";
             tp_filter_tag = "hltEle25erWPTightGsfTrackIsoFilter";
             trig_obj_label_probe= "triggerObjectsEle24LooseTau20SingleL1";
             tp_filter_probe = "hltEle24WPLooseL1SingleIsoEG22erGsfTrackIsoFilter";
           }
         }
       }
       std::vector<CompositeCandidate *> & dileptons = event->GetPtrVec<CompositeCandidate>("ditau");
       CompositeCandidate const* ditau  = dileptons.at(0);
       std::vector<TriggerObject *> const& objs_tag = event->GetPtrVec<TriggerObject>(trig_obj_label_tag);
       std::vector<TriggerObject *> const& objs_probe = event->GetPtrVec<TriggerObject>(trig_obj_label_probe);
       bool tp_tag_leg1_match = IsFilterMatched(ditau->At(0), objs_tag, tp_filter_tag, 0.5);
       bool tp_tag_leg2_match = IsFilterMatched(ditau->At(1), objs_tag, tp_filter_tag, 0.5);
       bool tp_probe_leg1_match = IsFilterMatched(ditau->At(0), objs_probe, tp_filter_probe, 0.5);
       bool tp_probe_leg2_match = IsFilterMatched(ditau->At(1), objs_probe, tp_filter_probe, 0.5);
       event->Add("tp_tag_leg1_match",tp_tag_leg1_match);
       event->Add("tp_tag_leg2_match",tp_tag_leg2_match);
       event->Add("tp_probe_leg1_match",tp_probe_leg1_match);
       event->Add("tp_probe_leg2_match",tp_probe_leg2_match);
       return 0;
    }));

 }


if((strategy_type==strategy::spring15||strategy_type==strategy::fall15||strategy_type==strategy::mssmspring16 || strategy_type==strategy::smspring16 || strategy_type == strategy::mssmsummer16)&&!is_data&&channel != channel::wmnu){
  BuildModule(HTTPairGenInfo("HTTPairGenInfo")
    .set_fs(fs.get())
    .set_write_plots(false)
    .set_ditau_label("ditau"));
}

if((strategy_type!=strategy::spring15&&strategy_type!=strategy::fall15&&strategy_type!=strategy::mssmspring16&&strategy_type!=strategy::smspring16 && strategy_type != strategy::mssmsummer16)&&!is_data&&js["do_btag_eff"].asBool()){
   BuildModule(BTagCheck("BTagCheck")
    .set_fs(fs.get())
    .set_channel(channel)
    .set_era(era_type)
    .set_strategy(strategy_type)
    .set_do_legacy(true)
    .set_jet_label(jets_label));
}




 if (jes_mode > 0 && !is_data ){
  std::string jes_input_file = "input/jec/JEC11_V12_AK5PF_UncertaintySources.txt";
  std::string jes_input_set  = "SubTotalDataMC";
  if (era_type == era::data_2012_rereco) {
    jes_input_file = "input/jec/Summer13_V1_DATA_UncertaintySources_AK5PF.txt";
    jes_input_set  = "SubTotalMC";
  }
  if (era_type == era::data_2015) {
    jes_input_file = "input/jec/Fall15_25nsV2_DATA_UncertaintySources_AK4PFchs.txt";
    jes_input_set  = "Total";
  }
  if (era_type == era::data_2016) {
    jes_input_file = "input/jec/Spring16_25nsV6_DATA_UncertaintySources_AK4PFchs.txt";
    if (strategy_type != strategy::mssmsummer16) jes_input_file = "input/jec/Summer16_23Sep2016HV4_DATA_UncertaintySources_AK4PFchs.txt"; 
    jes_input_set  = "Total";
  }
    
 BuildModule(JetEnergyUncertainty<PFJet>("JetEnergyUncertainty")
  .set_input_label(jets_label)
  .set_jes_shift_mode(jes_mode)
  .set_uncert_file(jes_input_file)
  .set_uncert_set(jes_input_set));

}
  
 BuildModule(CopyCollection<PFJet>("CopyFilteredJets",jets_label,jets_label+"UnFiltered"));

SimpleFilter<PFJet> jetIDFilter = SimpleFilter<PFJet>("JetIDFilter")
.set_input_label(jets_label);
if(strategy_type == strategy::paper2013) {
  jetIDFilter.set_predicate((bind(PFJetIDNoHFCut, _1)) && bind(PileupJetID, _1, pu_id_training, false));
} else if(strategy_type != strategy::mssmspring16 && strategy_type != strategy::smspring16 && strategy_type != strategy::mssmsummer16){
  jetIDFilter.set_predicate((bind(PFJetID2015, _1))); 
} else {
  jetIDFilter.set_predicate(bind(PFJetID2016, _1));
}
BuildModule(jetIDFilter);


if(channel != channel::wmnu) {
  BuildModule(OverlapFilter<PFJet, CompositeCandidate>("JetLeptonOverlapFilter")
    .set_input_label(jets_label)
    .set_reference_label("ditau")
    .set_min_dr(0.5));
} else if (channel == channel::wmnu){
  BuildModule(OverlapFilter<PFJet,Muon>("JetLeptonOverlapFilter")
    .set_input_label(jets_label)
    .set_reference_label("sel_muons")
    .set_min_dr(0.5));
}


if((strategy_type==strategy::fall15||strategy_type==strategy::mssmspring16||strategy_type==strategy::smspring16 || strategy_type == strategy::mssmsummer16)&&!is_data&&js["do_btag_eff"].asBool()){
   BuildModule(BTagCheck("BTagCheck")
    .set_fs(fs.get())
    .set_channel(channel)
    .set_era(era_type)
    .set_strategy(strategy_type)
    .set_do_legacy(false)
    .set_era(era_type)
    .set_jet_label(jets_label));
}


  if(strategy_type == strategy::paper2013){
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


 if((strategy_type == strategy::fall15|| strategy_type==strategy::mssmspring16 ||strategy_type == strategy::smspring16 || strategy_type == strategy::mssmsummer16) && channel!=channel::wmnu){
    BuildModule(HTTRun2RecoilCorrector("HTTRun2RecoilCorrector")
     .set_sample(output_name)
     .set_channel(channel)
     .set_mc(mc_type)
     .set_met_label(met_label)
     .set_jets_label(jets_label)
     .set_strategy(strategy_type)
     .set_use_quantile_map(false)
     .set_met_scale_mode(metscale_mode)
     .set_met_res_mode(metres_mode)
     .set_store_boson_pt(js["make_sync_ntuple"].asBool()));
  }

/*
  if(js["metscale_mode"].asUInt() > 0  && !is_data ){
   BuildModule(HhhMetScale("HhhMetScale")
     .set_met_scale_mode(js["metscale_mode"].asUInt())
     .set_met_label(met_label)
     .set_scale_shift(0.04));
   }*/

  SVFitTest svFitTest  = SVFitTest("SVFitTest")
    .set_channel(channel)
    .set_strategy(strategy_type)
    .set_outname(svfit_override == "" ? output_name : svfit_override)
    .set_run_mode(new_svfit_mode)
    .set_fail_mode(0)
    .set_require_inputs_match(true)
    .set_split(7000)
    .set_dilepton_label("ditau")
    .set_met_label(met_label)
    .set_fullpath(svfit_folder)
    .set_legacy_svfit(true)
    .set_do_preselection(false)
    .set_MC(true)
    .set_do_vloose_preselection(js["baseline"]["do_ff_weights"].asBool());
 if(era_type == era::data_2015 || era_type == era::data_2016){
   svFitTest.set_legacy_svfit(false);
   svFitTest.set_do_preselection(!js["make_sync_ntuple"].asBool());
   svFitTest.set_read_svfit_mt(true);
   svFitTest.set_tau_optimisation(js["tau_id_study"].asBool());
   svFitTest.set_read_all(js["read_all_svfit_files"].asBool());
   svFitTest.set_from_grid(js["svfit_from_grid"].asBool());
 }

BuildModule(svFitTest);

if(js["do_preselection"].asBool()){
  BuildModule(GenericModule("PreselectionFilter")
    .set_function([](ic::TreeEvent *event){
      //Pass preselection in case we're accidentally not running any preselection in SVFitTest but somehow have
      //requested preselection in the config anyway...
      bool pass_presel = event->Exists("pass_preselection") ? event->Get<bool>("pass_preselection") : 1;
      if (!pass_presel) return 1;
      else return 0;
     }));
 }


if((strategy_type == strategy::fall15 || strategy_type == strategy::mssmspring16 ||strategy_type == strategy::smspring16 || strategy_type == strategy::mssmsummer16) && !is_data){
 TH2F bbtag_eff;
 TH2F cbtag_eff;
 TH2F othbtag_eff;

  if(strategy_type == strategy::fall15){
    if(channel != channel::tt){
      bbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies.root","/","btag_eff_b");
      cbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies.root","/","btag_eff_c");
      othbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies.root","/","btag_eff_oth");
    } else {
      bbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_loosewp.root","/","btag_eff_b");
      cbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_loosewp.root","/","btag_eff_c");
      othbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_loosewp.root","/","btag_eff_oth");
    }
  }  else if (strategy_type != strategy::mssmsummer16){
    bbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_ichep2016.root","/","btag_eff_b");
    cbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_ichep2016.root","/","btag_eff_c");
    othbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_ichep2016.root","/","btag_eff_oth");
  } else {
    bbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_Moriond2017.root","/","btag_eff_b");
    cbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_Moriond2017.root","/","btag_eff_c");
    othbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_Moriond2017.root","/","btag_eff_oth");
  }

  BuildModule(BTagWeightRun2("BTagWeightRun2")
   .set_channel(channel)
   .set_era(era_type)
   .set_strategy(strategy_type)
   .set_jet_label(jets_label)
   .set_bbtag_eff(new TH2F(bbtag_eff))
   .set_cbtag_eff(new TH2F(cbtag_eff))
   .set_othbtag_eff(new TH2F(othbtag_eff))
   .set_do_reshape(do_reshape)
   .set_btag_mode(btag_mode)
   .set_bfake_mode(bfake_mode));
}

 if(strategy_type == strategy::paper2013){
  HTTWeights httWeights = HTTWeights("HTTWeights")
    .set_channel(channel)
    .set_era(era_type)
    .set_mc(mc_type);
    httWeights.set_trg_applied_in_mc(true)
    .set_do_trg_weights(false)
    .set_do_etau_fakerate(false)
    .set_do_mtau_fakerate(false);
    httWeights.set_do_idiso_weights(false)
    .set_do_id_weights(false) // This will override do_idiso_weights, applying only ID weights in embedded
    .set_do_emu_e_fakerates(false);
    httWeights.set_do_emu_m_fakerates(false)
    .set_do_top_factors(false)
    .set_do_tau_id_weights(real_tau_sample);
    httWeights.set_gen_tau_collection(is_embedded ? "genParticlesEmbedded" : "genParticlesTaus")
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
      //httWeights.set_ggh_mass(mass_string);
    }
  }

 BuildModule(httWeights);
  
    HTTStitching httStitching = HTTStitching("HTTStitching")  
    .set_era(era_type)
    .set_fs(fs.get());
  if (output_name.find("WJetsToLNuSoup") != output_name.npos) {
    httStitching.set_do_w_soup(true);
    if (mc_type == mc::fall11_42X) {
      httStitching.SetWTargetFractions(0.752332, 0.171539, 0.0538005, 0.0159036, 0.00642444);
      httStitching.SetWInputYields(81295381.0, 70712575.0, 25320546.0, 7541595.0, 12973738.0);
    }
    if (mc_type == mc::summer12_53X && strategy_type == strategy::paper2013) {
      httStitching.SetWTargetFractions(0.743925, 0.175999, 0.0562617, 0.0168926, 0.00692218);
      httStitching.SetWInputYields(76102995.0, 52926398.0, 64738774.0, 30780647.0, 13382803.0);
    }
  }
  if (output_name.find("DYJets") != output_name.npos && output_name.find("Soup") != output_name.npos) {
    if (mc_type == mc::summer12_53X) {
      httStitching.set_do_dy_soup(true);
      httStitching.SetDYTargetFractions(0.723342373, 0.190169492, 0.061355932, 0.017322034, 0.007810169);
      httStitching.SetDYInputYields(30459503.0, 24045248.0, 21852156.0, 11015445.0, 6402827.0);
    }
  }
 BuildModule(httStitching);

}


 if(strategy_type == strategy::spring15 &&channel!=channel::wmnu){
   TH2D et_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_2015.root","/","Electron_SingleEle_MC_eff");
   TH2D et_trig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_2015.root","/","Electron_SingleEle_Data_eff");
   TH2D mt_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_2015.root","/","Muon_SingleMu_MC_eff");
   TH2D mt_trig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_2015.root","/","Muon_SingleMu_Data_eff");
   TH2D et_idiso_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_2015.root","/","Electron_IdIso0p10_MC_eff");
   TH2D et_idiso_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_2015.root","/","Electron_IdIso0p10_Data_eff");
   TH2D em_e_idiso_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_2015.root","/","Electron_IdIso0p15_MC_eff");
   TH2D em_e_idiso_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_2015.root","/","Electron_IdIso0p15_Data_eff");
   TH2D mt_idiso_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_2015.root","/","Muon_IdIso0p10_MC_eff");
   TH2D mt_idiso_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_2015.root","/","Muon_IdIso0p10_Data_eff");
   TH2D em_m_idiso_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_2015.root","/","Muon_IdIso0p15_MC_eff");
   TH2D em_m_idiso_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_2015.root","/","Muon_IdIso0p15_Data_eff");
   TH2D em_m17_trig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_2015.root","/","Muon_Mu17_Data_eff");
   TH2D em_m17_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_2015.root","/","Muon_Mu17_MC_eff");
   TH2D em_m8_trig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_2015.root","/","Muon_Mu8_Data_eff");
   TH2D em_m8_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_2015.root","/","Muon_Mu8_MC_eff");
   TH2D em_e17_trig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_2015.root","/","Electron_Ele17_Data_eff");
   TH2D em_e17_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_2015.root","/","Electron_Ele17_MC_eff");
   TH2D em_e12_trig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_2015.root","/","Electron_Ele12_Data_eff");
   TH2D em_e12_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_2015.root","/","Electron_Ele12_MC_eff");

   HTTWeights httWeights = HTTWeights("HTTWeights")   
    .set_channel(channel)
    .set_era(era_type)
    .set_mc(mc_type);
    httWeights.set_do_tau_id_weights(false)
    .set_ditau_label("ditau")
    .set_jets_label("ak4PFJetsCHS");
    httWeights.set_et_trig_mc(new TH2D(et_trig_mc)).set_et_trig_data(new TH2D(et_trig_data))
    .set_mt_trig_mc(new TH2D(mt_trig_mc)).set_mt_trig_data(new TH2D(mt_trig_data));
    httWeights.set_et_idiso_mc(new TH2D(et_idiso_mc)).set_et_idiso_data(new TH2D(et_idiso_data))
    .set_mt_idiso_mc(new TH2D(mt_idiso_mc)).set_mt_idiso_data(new TH2D(mt_idiso_data));
    httWeights.set_em_m17_trig_mc(new TH2D(em_m17_trig_mc)).set_em_m17_trig_data(new TH2D(em_m17_trig_data))
    .set_em_m8_trig_mc(new TH2D(em_m8_trig_mc)).set_em_m8_trig_data(new TH2D(em_m8_trig_data));
    httWeights.set_em_e17_trig_mc(new TH2D(em_e17_trig_mc)).set_em_e17_trig_data(new TH2D(em_e17_trig_data))
    .set_em_e12_trig_mc(new TH2D(em_e12_trig_mc)).set_em_e12_trig_data(new TH2D(em_e12_trig_data));
    httWeights.set_em_e_idiso_mc(new TH2D(em_e_idiso_mc)).set_em_e_idiso_data(new TH2D(em_e_idiso_data))
    .set_em_m_idiso_mc(new TH2D(em_m_idiso_mc)).set_em_m_idiso_data(new TH2D(em_m_idiso_data));
  if (!is_data ) {
    httWeights.set_do_trg_weights(true).set_trg_applied_in_mc(true).set_do_idiso_weights(true);
    if(channel ==channel::zmm || channel==channel::zee) httWeights.set_do_trg_weights(false).set_trg_applied_in_mc(false);
  }
    BuildModule(httWeights);

    HTTStitching httStitching = HTTStitching("HTTStitching")  
    .set_era(era_type)
    .set_fs(fs.get());
  if (output_name.find("DYJetsToLL_M-50") != output_name.npos){
      httStitching.set_do_dy_soup_htbinned(true);
      httStitching.SetDYInputCrossSections(4895,139.4,42.75,5.497,2.21);
      httStitching.SetDYInputYields(9042031,2725655,973937,1067758,998912);
    }
  
  if (output_name.find("DYJetsToLL_M-5-") != output_name.npos || output_name.find("DYJetsToLL_M-5_") != output_name.npos){
      httStitching.set_do_dy_soup_htbinned(true);
      httStitching.SetDYInputCrossSections(71310,224.2,37.2,3.581,1.124);
      httStitching.SetDYInputYields(9404398,1013479,1011756,998751,1007309);
    }

    if (output_name.find("WJetsToLNu") != output_name.npos){
      httStitching.set_do_w_soup_htbinned(true);
      httStitching.SetWInputCrossSections(50690,1345,359.7,48.91,18.77);
      httStitching.SetWInputYields(72207128,10152718,5221599,1745914,1039152);
    }
    BuildModule(httStitching);
   

  }

 if(strategy_type ==strategy::fall15&&channel!=channel::wmnu){
   TH2D et_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_Fall15.root","/","Electron_SingleEle_MC_eff");
   TH2D et_trig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_Fall15.root","/","Electron_SingleEle_Data_eff");
   TH2D mt_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_Fall15.root","/","Muon_SingleMu_MC_eff");
   TH2D mt_trig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_Fall15.root","/","Muon_SingleMu_Data_eff");
   TH2D et_idiso_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_Fall15.root","/","Electron_IdIso0p10_MC_eff");
   TH2D et_idiso_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_Fall15.root","/","Electron_IdIso0p10_Data_eff");
   TH2D em_e_idiso_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_Fall15.root","/","Electron_IdIso0p15_MC_eff");
   TH2D em_e_idiso_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_Fall15.root","/","Electron_IdIso0p15_Data_eff");
   TH2D mt_idiso_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_Fall15.root","/","Muon_IdIso0p10_MC_eff");
   TH2D mt_idiso_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_Fall15.root","/","Muon_IdIso0p10_Data_eff");
   TH2D em_m_idiso_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_Fall15.root","/","Muon_IdIso0p15_MC_eff");
   TH2D em_m_idiso_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_Fall15.root","/","Muon_IdIso0p15_Data_eff");
   TH2D em_m17_trig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_Fall15.root","/","Muon_Mu17_Data_eff");
   TH2D em_m17_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_Fall15.root","/","Muon_Mu17_MC_eff");
   TH2D em_m8_trig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_Fall15.root","/","Muon_Mu8_Data_eff");
   TH2D em_m8_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_Fall15.root","/","Muon_Mu8_MC_eff");
   TH2D em_e17_trig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_Fall15.root","/","Electron_Ele17_Data_eff");
   TH2D em_e17_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_Fall15.root","/","Electron_Ele17_MC_eff");
   TH2D em_e12_trig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_Fall15.root","/","Electron_Ele12_Data_eff");
   TH2D em_e12_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_Fall15.root","/","Electron_Ele12_MC_eff");
   TH2D em_qcd_cr1_lt2 = GetFromTFile<TH2D>("input/emu_qcd_weights/QCD_weight_emu.root","/","QCDratio_CR1_dRLt2");
   TH2D em_qcd_cr2_lt2 = GetFromTFile<TH2D>("input/emu_qcd_weights/QCD_weight_emu.root","/","QCDratio_CR2_dRLt2");
   TH2D em_qcd_cr1_2to4 = GetFromTFile<TH2D>("input/emu_qcd_weights/QCD_weight_emu.root","/","QCDratio_CR1_dR2to4");
   TH2D em_qcd_cr2_2to4 = GetFromTFile<TH2D>("input/emu_qcd_weights/QCD_weight_emu.root","/","QCDratio_CR2_dR2to4");
   TH2D em_qcd_cr1_gt4 = GetFromTFile<TH2D>("input/emu_qcd_weights/QCD_weight_emu.root","/","QCDratio_CR1_dRGt4");
   TH2D em_qcd_cr2_gt4 = GetFromTFile<TH2D>("input/emu_qcd_weights/QCD_weight_emu.root","/","QCDratio_CR2_dRGt4");
   TH2D z_pt_weights = GetFromTFile<TH2D>("input/zpt_weights/zpt_weights.root","/","zptmass_histo");

   HTTWeights httWeights = HTTWeights("HTTWeights")   
    .set_channel(channel)
    .set_era(era_type)
    .set_mc(mc_type);
    httWeights.set_do_tau_id_weights(real_tau_sample)
    .set_do_em_qcd_weights(true)
    .set_ditau_label("ditau");
    httWeights.set_jets_label("ak4PFJetsCHS")
    .set_et_trig_mc(new TH2D(et_trig_mc)).set_et_trig_data(new TH2D(et_trig_data));
    httWeights.set_mt_trig_mc(new TH2D(mt_trig_mc)).set_mt_trig_data(new TH2D(mt_trig_data))
    .set_et_idiso_mc(new TH2D(et_idiso_mc)).set_et_idiso_data(new TH2D(et_idiso_data));
    httWeights.set_mt_idiso_mc(new TH2D(mt_idiso_mc)).set_mt_idiso_data(new TH2D(mt_idiso_data))
    .set_em_m17_trig_mc(new TH2D(em_m17_trig_mc)).set_em_m17_trig_data(new TH2D(em_m17_trig_data));
    httWeights.set_em_m8_trig_mc(new TH2D(em_m8_trig_mc)).set_em_m8_trig_data(new TH2D(em_m8_trig_data))
    .set_em_e17_trig_mc(new TH2D(em_e17_trig_mc)).set_em_e17_trig_data(new TH2D(em_e17_trig_data));
    httWeights.set_em_e12_trig_mc(new TH2D(em_e12_trig_mc)).set_em_e12_trig_data(new TH2D(em_e12_trig_data))
    .set_em_e_idiso_mc(new TH2D(em_e_idiso_mc)).set_em_e_idiso_data(new TH2D(em_e_idiso_data));
    httWeights.set_em_m_idiso_mc(new TH2D(em_m_idiso_mc)).set_em_m_idiso_data(new TH2D(em_m_idiso_data))
    .set_em_qcd_cr1_lt2(new TH2D(em_qcd_cr1_lt2)).set_em_qcd_cr2_lt2(new TH2D(em_qcd_cr2_lt2));
    httWeights.set_em_qcd_cr1_2to4(new TH2D(em_qcd_cr1_2to4)).set_em_qcd_cr2_2to4(new TH2D(em_qcd_cr2_2to4))
    .set_em_qcd_cr1_gt4(new TH2D(em_qcd_cr1_gt4)).set_em_qcd_cr2_gt4(new TH2D(em_qcd_cr2_gt4));
    httWeights.set_z_pt_mass_hist(new TH2D(z_pt_weights));
  if (!is_data ) {
    httWeights.set_do_trg_weights(!js["qcd_study"].asBool()).set_trg_applied_in_mc(js["trg_in_mc"].asBool()).set_do_idiso_weights(true);
    if(channel ==channel::zmm || channel==channel::zee) httWeights.set_do_trg_weights(false).set_trg_applied_in_mc(false);
    if(channel == channel::et) httWeights.set_do_etau_fakerate(true);
  }
  if ((output_name.find("DY") != output_name.npos && output_name.find("JetsToLL_M-50") != output_name.npos) || output_name.find("DYJetsToLL_M-150-LO")!=output_name.npos){
    httWeights.set_do_zpt_weight(true);
  }
  if (output_name.find("TT-ext") != output_name.npos) httWeights.set_do_topquark_weights(true);
  BuildModule(httWeights);

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
   TH2D et_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16temp.root","/","Ele25_Data_Eff");
   TH2D et_trig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16temp.root","/","Ele25_Data_Eff");
   TH2D et_antiiso1_trig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_antiiso1_spring16temp.root","/","Ele25_Data_Eff");
   TH2D et_antiiso2_trig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_antiiso2_spring16temp.root","/","Ele25_Data_Eff");
   TH2D et_xtrig_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16temp.root","/","Ele24_Data_Eff");
   TH2D et_xtrig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16temp.root","/","Ele24_Data_Eff");
   TH2D et_conditional_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_conditional.root","/","Ele25GivenEle24_Data_Eff");
   TH2D et_conditional_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_conditional.root","/","Ele25GivenEle24_Data_Eff");
   TH2D mt_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16temp.root","/","Mu22_Data_Eff");
   TH2D mt_trig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16temp.root","/","Mu22_Data_Eff");
   TH2D mt_antiiso1_trig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_antiiso1_spring16temp.root","/","Mu22_Data_Eff");
   TH2D mt_antiiso2_trig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_antiiso2_spring16temp.root","/","Mu22_Data_Eff");
   TH2D mt_xtrig_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16temp.root","/","Mu19_Data_Eff");
   TH2D mt_xtrig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16temp.root","/","Mu19_Data_Eff");
   TH2D mt_conditional_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_conditional.root","/","Mu22GivenMu19_Data_Eff");
   TH2D mt_conditional_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_conditional.root","/","Mu22GivenMu19_Data_Eff");
   TH2D et_idiso_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16.root","/","Electron_IdIso0p10_MC_eff");
   TH2D et_idiso_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16.root","/","Electron_IdIso0p10_Data_eff");
   TH2D em_e_idiso_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16.root","/","Electron_IdIso0p10_MC_eff");
   TH2D em_e_idiso_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16.root","/","Electron_IdIso0p10_Data_eff");
   TH2D mt_idiso_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16.root","/","Muon_IdIso0p10_MC_eff");
   TH2D mt_idiso_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16.root","/","Muon_IdIso0p10_Data_eff");
   TH2D em_m_idiso_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16.root","/","Muon_IdIso0p10_MC_eff");
   TH2D em_m_idiso_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16.root","/","Muon_IdIso0p10_Data_eff");
   TH2D em_m17_trig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16.root","/","Muon_Mu17_Data_eff");
   TH2D em_m17_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16.root","/","Muon_Mu17_MC_eff");
   TH2D em_m8_trig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16.root","/","Muon_Mu8_Data_eff");
   TH2D em_m8_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16.root","/","Muon_Mu8_MC_eff");
   TH2D em_e17_trig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16.root","/","Electron_Ele17_Data_eff");
   TH2D em_e17_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16.root","/","Electron_Ele17_MC_eff");
   TH2D em_e12_trig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16.root","/","Electron_Ele12_Data_eff");
   TH2D em_e12_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16.root","/","Electron_Ele12_MC_eff");
   TH2F ele_tracking_sf = GetFromTFile<TH2F>("input/scale_factors/EGamma_gsf_tracking.root","/","EGamma_SF2D");
   TH1D muon_tracking_sf = GetFromTFile<TH1D>("input/scale_factors/muon_trk_eff.root","/","muon_trk_eff");
   TH2D em_qcd_cr1_lt2 = GetFromTFile<TH2D>("input/emu_qcd_weights/QCD_weight_emu_2016BCD.root","/","QCDratio_CR1_dRLt2");
   TH2D em_qcd_cr2_lt2 = GetFromTFile<TH2D>("input/emu_qcd_weights/QCD_weight_emu_2016BCD.root","/","QCDratio_CR2_dRLt2");
   TH2D em_qcd_cr1_2to4 = GetFromTFile<TH2D>("input/emu_qcd_weights/QCD_weight_emu_2016BCD.root","/","QCDratio_CR1_dR2to4");
   TH2D em_qcd_cr2_2to4 = GetFromTFile<TH2D>("input/emu_qcd_weights/QCD_weight_emu_2016BCD.root","/","QCDratio_CR2_dR2to4");
   TH2D em_qcd_cr1_gt4 = GetFromTFile<TH2D>("input/emu_qcd_weights/QCD_weight_emu_2016BCD.root","/","QCDratio_CR1_dRGt4");
   TH2D em_qcd_cr2_gt4 = GetFromTFile<TH2D>("input/emu_qcd_weights/QCD_weight_emu_2016BCD.root","/","QCDratio_CR2_dRGt4");
   TH2D z_pt_weights = GetFromTFile<TH2D>("input/zpt_weights/zpt_weights_2016.root","/","zptmass_histo");

   HTTWeights httWeights = HTTWeights("HTTWeights")   
    .set_channel(channel)
    .set_era(era_type)
    .set_mc(mc_type);
    httWeights.set_do_tau_id_weights(real_tau_sample)
    .set_do_tau_id_sf(real_tau_sample)
    .set_do_em_qcd_weights(true);
    httWeights.set_ditau_label("ditau")
    .set_jets_label("ak4PFJetsCHS")
    .set_do_single_lepton_trg(js["do_singlelepton"].asBool());
    httWeights.set_do_cross_trg(js["do_leptonplustau"].asBool())
    .set_tt_trg_iso_mode(js["tt_trg_iso_mode"].asUInt())
    .set_em_m17_trig_mc(new TH2D(em_m17_trig_mc)).set_em_m17_trig_data(new TH2D(em_m17_trig_data));
    httWeights.set_em_m8_trig_mc(new TH2D(em_m8_trig_mc)).set_em_m8_trig_data(new TH2D(em_m8_trig_data))
    .set_em_e17_trig_mc(new TH2D(em_e17_trig_mc)).set_em_e17_trig_data(new TH2D(em_e17_trig_data));
    httWeights.set_em_e12_trig_mc(new TH2D(em_e12_trig_mc)).set_em_e12_trig_data(new TH2D(em_e12_trig_data))
    .set_em_qcd_cr1_lt2(new TH2D(em_qcd_cr1_lt2)).set_em_qcd_cr2_lt2(new TH2D(em_qcd_cr2_lt2));
    httWeights.set_em_qcd_cr1_2to4(new TH2D(em_qcd_cr1_2to4)).set_em_qcd_cr2_2to4(new TH2D(em_qcd_cr2_2to4))
    .set_em_qcd_cr1_gt4(new TH2D(em_qcd_cr1_gt4)).set_em_qcd_cr2_gt4(new TH2D(em_qcd_cr2_gt4));
    httWeights.set_z_pt_mass_hist(new TH2D(z_pt_weights));
    if(js["force_old_effs"].asBool()) {
        httWeights.set_et_trig_mc(new TH2D(et_trig_mc)).set_et_trig_data(new TH2D(et_trig_data))
        .set_muon_tracking_sf(new TH1D(muon_tracking_sf));
        httWeights.set_ele_tracking_sf(new TH2F(ele_tracking_sf))
        .set_em_e_idiso_mc(new TH2D(em_e_idiso_mc)).set_em_e_idiso_data(new TH2D(em_e_idiso_data));
        httWeights.set_em_m_idiso_mc(new TH2D(em_m_idiso_mc)).set_em_m_idiso_data(new TH2D(em_m_idiso_data))
        .set_et_antiiso1_trig_data(new TH2D(et_antiiso1_trig_data)).set_et_antiiso2_trig_data(new TH2D(et_antiiso2_trig_data));
        httWeights.set_et_xtrig_mc(new TH2D(et_xtrig_mc)).set_et_xtrig_data(new TH2D(et_xtrig_data))
        .set_et_conditional_mc(new TH2D(et_conditional_mc)).set_et_conditional_data(new TH2D(et_conditional_data));
        httWeights.set_mt_trig_mc(new TH2D(mt_trig_mc)).set_mt_trig_data(new TH2D(mt_trig_data))
        .set_mt_antiiso1_trig_data(new TH2D(mt_antiiso1_trig_data)).set_mt_antiiso2_trig_data(new TH2D(mt_antiiso2_trig_data));
        httWeights.set_mt_xtrig_mc(new TH2D(mt_xtrig_mc)).set_mt_xtrig_data(new TH2D(mt_xtrig_data))
        .set_mt_conditional_mc(new TH2D(mt_conditional_mc)).set_mt_conditional_data(new TH2D(mt_conditional_data));
    }else{
        httWeights.set_scalefactor_file("input/scale_factors/htt_scalefactors_v5.root");
    }
  if (!is_data ) {
    httWeights.set_do_trg_weights(!js["qcd_study"].asBool()).set_trg_applied_in_mc(js["trg_in_mc"].asBool()).set_do_idiso_weights(true);
    //if(channel ==channel::zmm || channel==channel::zee) httWeights.set_do_trg_weights(false).set_trg_applied_in_mc(false);
    if(channel == channel::et || channel == channel::mt || channel==channel::tt) httWeights.set_do_etau_fakerate(true);
    if(channel == channel::mt || channel == channel::et ||channel == channel::tt) httWeights.set_do_mtau_fakerate(true);
    if(channel == channel::et || channel==channel::em || channel==channel::mt) httWeights.set_do_tracking_eff(true);
  }

  if ((output_name.find("DY") != output_name.npos && output_name.find("JetsToLL-LO") != output_name.npos && !(output_name.find("JetsToLL-LO-10-50") != output_name.npos))){
   std::cout << "do zpt weight!" << std::endl;
    httWeights.set_do_zpt_weight(true);
  }

  if (output_name.find("TT") != output_name.npos) httWeights.set_do_topquark_weights(true);
  if (output_name.find("WJetsToLNu-LO") != output_name.npos || output_name.find("W1JetsToLNu-LO") != output_name.npos || output_name.find("W2JetsToLNu-LO") != output_name.npos ||
       output_name.find("W3JetsToLNu-LO") != output_name.npos || output_name.find("W4JetsToLNu-LO") != output_name.npos){
    httWeights.set_do_tau_fake_weights(true);
  }

  

    BuildModule(httWeights);
   
   
    HTTStitching httStitching = HTTStitching("HTTStitching")  
    .set_era(era_type)
    .set_fs(fs.get());
     if (output_name.find("WJetsToLNu-LO") != output_name.npos || output_name.find("W1JetsToLNu-LO") != output_name.npos || output_name.find("W2JetsToLNu-LO") != output_name.npos ||
       output_name.find("W3JetsToLNu-LO") != output_name.npos || output_name.find("W4JetsToLNu-LO") != output_name.npos){
    httStitching.set_do_w_soup(true);
    httStitching.SetWInputCrossSections(50380,9644.5,3144.5,954.8,485.6);
    httStitching.SetWInputYields(28210360,39855520,29984239,19869053,9174756);
   }
   if ((output_name.find("DY") != output_name.npos && output_name.find("JetsToLL-LO") != output_name.npos )){
     httStitching.set_do_dy_soup(true);
     httStitching.SetDYInputCrossSections(4954, 1012.5, 332.8, 101.8,54.8); //Target fractions are xs_n-jet/xs_inclusive
     httStitching.SetDYInputYields(90951295,65485168 , 19695514, 5753813, 4101383);
   }

    
   BuildModule(httStitching);
  
  
  }
  
if(strategy_type == strategy::mssmsummer16&&channel!=channel::wmnu){
   TH2D et_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16temp.root","/","Ele25_Data_Eff");
   TH2D et_trig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16temp.root","/","Ele25_Data_Eff");
   TH2D et_antiiso1_trig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_antiiso1_spring16temp.root","/","Ele25_Data_Eff");
   TH2D et_antiiso2_trig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_antiiso2_spring16temp.root","/","Ele25_Data_Eff");
   TH2D et_xtrig_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16temp.root","/","Ele24_Data_Eff");
   TH2D et_xtrig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16temp.root","/","Ele24_Data_Eff");
   TH2D et_conditional_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_conditional.root","/","Ele25GivenEle24_Data_Eff");
   TH2D et_conditional_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_conditional.root","/","Ele25GivenEle24_Data_Eff");
   TH2D mt_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16temp.root","/","Mu22_Data_Eff");
   TH2D mt_trig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16temp.root","/","Mu22_Data_Eff");
   TH2D mt_antiiso1_trig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_antiiso1_spring16temp.root","/","Mu22_Data_Eff");
   TH2D mt_antiiso2_trig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_antiiso2_spring16temp.root","/","Mu22_Data_Eff");
   TH2D mt_xtrig_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16temp.root","/","Mu19_Data_Eff");
   TH2D mt_xtrig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16temp.root","/","Mu19_Data_Eff");
   TH2D mt_conditional_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_conditional.root","/","Mu22GivenMu19_Data_Eff");
   TH2D mt_conditional_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_conditional.root","/","Mu22GivenMu19_Data_Eff");
   TH2D et_idiso_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16.root","/","Electron_IdIso0p10_MC_eff");
   TH2D et_idiso_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16.root","/","Electron_IdIso0p10_Data_eff");
   TH2D em_e_idiso_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16.root","/","Electron_IdIso0p10_MC_eff");
   TH2D em_e_idiso_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16.root","/","Electron_IdIso0p10_Data_eff");
   TH2D mt_idiso_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16.root","/","Muon_IdIso0p10_MC_eff");
   TH2D mt_idiso_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16.root","/","Muon_IdIso0p10_Data_eff");
   TH2D em_m_idiso_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16.root","/","Muon_IdIso0p10_MC_eff");
   TH2D em_m_idiso_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16.root","/","Muon_IdIso0p10_Data_eff");
   TH2D em_m17_trig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16.root","/","Muon_Mu17_Data_eff");
   TH2D em_m17_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16.root","/","Muon_Mu17_MC_eff");
   TH2D em_m8_trig_data = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16.root","/","Muon_Mu8_Data_eff");
   TH2D em_m8_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Muon_SF_spring16.root","/","Muon_Mu8_MC_eff");
   TH2D em_e17_trig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16.root","/","Electron_Ele17_Data_eff");
   TH2D em_e17_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16.root","/","Electron_Ele17_MC_eff");
   TH2D em_e12_trig_data = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16.root","/","Electron_Ele12_Data_eff");
   TH2D em_e12_trig_mc = GetFromTFile<TH2D>("input/scale_factors/Ele_SF_spring16.root","/","Electron_Ele12_MC_eff");
   TH2F ele_tracking_sf = GetFromTFile<TH2F>("input/scale_factors/EGamma_gsf_tracking.root","/","EGamma_SF2D");
   TH1D muon_tracking_sf = GetFromTFile<TH1D>("input/scale_factors/muon_trk_eff.root","/","muon_trk_eff");
   TH2D em_qcd_cr1_lt2 = GetFromTFile<TH2D>("input/emu_qcd_weights/QCD_weight_emu_2016BtoH.root","/","QCDratio_CR1_dRLt2");
   TH2D em_qcd_cr1_2to4 = GetFromTFile<TH2D>("input/emu_qcd_weights/QCD_weight_emu_2016BtoH.root","/","QCDratio_CR1_dR2to4");
   TH2D em_qcd_cr1_gt4 = GetFromTFile<TH2D>("input/emu_qcd_weights/QCD_weight_emu_2016BtoH.root","/","QCDratio_CR1_dRGt4");
   TH2D z_pt_weights = GetFromTFile<TH2D>("input/zpt_weights/zpt_weights_summer2016_v2.root","/","zptmass_histo");

   HTTWeights httWeights = HTTWeights("HTTWeights")   
    .set_channel(channel)
    .set_era(era_type)
    .set_mc(mc_type)
    .set_do_tau_id_weights(real_tau_sample)
    .set_do_tau_id_sf(real_tau_sample)
    .set_do_jlepton_fake(jlepton_fake)
    .set_do_em_qcd_weights(true)
    .set_ditau_label("ditau")
    .set_jets_label("ak4PFJetsCHS")
    .set_do_single_lepton_trg(js["do_singlelepton"].asBool())
    .set_do_cross_trg(js["do_leptonplustau"].asBool())
    .set_tt_trg_iso_mode(js["tt_trg_iso_mode"].asUInt())
    .set_em_m17_trig_mc(new TH2D(em_m17_trig_mc)).set_em_m17_trig_data(new TH2D(em_m17_trig_data))
    .set_em_m8_trig_mc(new TH2D(em_m8_trig_mc)).set_em_m8_trig_data(new TH2D(em_m8_trig_data))
    .set_em_e17_trig_mc(new TH2D(em_e17_trig_mc)).set_em_e17_trig_data(new TH2D(em_e17_trig_data))
    .set_em_e12_trig_mc(new TH2D(em_e12_trig_mc)).set_em_e12_trig_data(new TH2D(em_e12_trig_data))
    .set_em_qcd_cr1_lt2(new TH2D(em_qcd_cr1_lt2))
    .set_em_qcd_cr1_2to4(new TH2D(em_qcd_cr1_2to4))
    .set_em_qcd_cr1_gt4(new TH2D(em_qcd_cr1_gt4))
    .set_z_pt_mass_hist(new TH2D(z_pt_weights));
    if(js["force_old_effs"].asBool()) {
        httWeights.set_et_trig_mc(new TH2D(et_trig_mc)).set_et_trig_data(new TH2D(et_trig_data))
        .set_muon_tracking_sf(new TH1D(muon_tracking_sf))
        .set_ele_tracking_sf(new TH2F(ele_tracking_sf))
        .set_em_e_idiso_mc(new TH2D(em_e_idiso_mc)).set_em_e_idiso_data(new TH2D(em_e_idiso_data))
        .set_em_m_idiso_mc(new TH2D(em_m_idiso_mc)).set_em_m_idiso_data(new TH2D(em_m_idiso_data))
        .set_et_antiiso1_trig_data(new TH2D(et_antiiso1_trig_data)).set_et_antiiso2_trig_data(new TH2D(et_antiiso2_trig_data))
        .set_et_xtrig_mc(new TH2D(et_xtrig_mc)).set_et_xtrig_data(new TH2D(et_xtrig_data))
        .set_et_conditional_mc(new TH2D(et_conditional_mc)).set_et_conditional_data(new TH2D(et_conditional_data))
        .set_mt_trig_mc(new TH2D(mt_trig_mc)).set_mt_trig_data(new TH2D(mt_trig_data))
        .set_mt_antiiso1_trig_data(new TH2D(mt_antiiso1_trig_data)).set_mt_antiiso2_trig_data(new TH2D(mt_antiiso2_trig_data))
        .set_mt_xtrig_mc(new TH2D(mt_xtrig_mc)).set_mt_xtrig_data(new TH2D(mt_xtrig_data))
        .set_mt_conditional_mc(new TH2D(mt_conditional_mc)).set_mt_conditional_data(new TH2D(mt_conditional_data));
    }else{
        httWeights.set_scalefactor_file("input/scale_factors/htt_scalefactors_v16_5.root");
    }
  if (!is_data ) {
    httWeights.set_do_trg_weights(!js["qcd_study"].asBool()).set_trg_applied_in_mc(js["trg_in_mc"].asBool()).set_do_idiso_weights(true);
    //if(channel ==channel::zmm || channel==channel::zee) httWeights.set_do_trg_weights(false).set_trg_applied_in_mc(false);
    if(channel == channel::et || channel == channel::mt || channel==channel::tt) httWeights.set_do_etau_fakerate(true);
    if(channel == channel::mt || channel == channel::et ||channel == channel::tt) httWeights.set_do_mtau_fakerate(true);
    if(channel == channel::et || channel==channel::em || channel==channel::mt || channel==channel::zmm || channel==channel::zee) httWeights.set_do_tracking_eff(true);
  }

  if ((output_name.find("DY") != output_name.npos && output_name.find("JetsToLL-LO") != output_name.npos && !(output_name.find("JetsToLL-LO-10-50") != output_name.npos))){
    httWeights.set_do_zpt_weight(true);
  }

  if (output_name.find("TT") != output_name.npos) httWeights.set_do_topquark_weights(true);
  if (output_name.find("WJetsToLNu-LO") != output_name.npos || output_name.find("W1JetsToLNu-LO") != output_name.npos || output_name.find("W2JetsToLNu-LO") != output_name.npos ||
       output_name.find("W3JetsToLNu-LO") != output_name.npos || output_name.find("W4JetsToLNu-LO") != output_name.npos){
    httWeights.set_do_tau_fake_weights(true);
  }

  

    BuildModule(httWeights);
    
    HTTStitching httStitching = HTTStitching("HTTStitching")  
        .set_era(era_type)
        .set_fs(fs.get());
         if (output_name.find("WJetsToLNu-LO") != output_name.npos || output_name.find("W1JetsToLNu-LO") != output_name.npos || output_name.find("W2JetsToLNu-LO") != output_name.npos ||
           output_name.find("W3JetsToLNu-LO") != output_name.npos || output_name.find("W4JetsToLNu-LO") != output_name.npos){
        httStitching.set_do_w_soup(true);
        httStitching.SetWInputCrossSections(50380,9644.5,3144.5,954.8,485.6);
        httStitching.SetWInputYields(57026058 + 29705748,45367044,29878415 + 30319351,19798117 + 39269431,9170576 + 2073275 + 18751462);
       }
       if ((output_name.find("DY") != output_name.npos && output_name.find("JetsToLL-LO") != output_name.npos && !(output_name.find("JetsToLL-LO-10-50") != output_name.npos))){
         httStitching.set_do_dy_soup(true);
         httStitching.SetDYInputCrossSections(4954, 1012.5, 332.8, 101.8,54.8); //Target fractions are xs_n-jet/xs_inclusive
         httStitching.SetDYInputYields(96658943 + 49144274,62627174, 19970551, 5856110, 4197868);
       }
   
    BuildModule(httStitching);   
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
//  BuildModule(HhhEMuMVA("HhhEMuMVA")
 //   .set_ditau_label("ditau"));
// BuildModule(HhhEMuMVABoth("HhhEMuMVABoth")
 //   .set_ditau_label("ditau"));
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

// if(js["make_sync_ntuple"].asBool() && strategy_type!=strategy::phys14){
//    BuildModule(HTTSync("HTTSync","HTTSequenceSyncfiles/SYNCFILE_" + output_name, channel)
//     .set_is_embedded(is_embedded).set_met_label(met_label).set_ditau_label("ditau"));
// }

// if(js["make_sync_ntuple"].asBool() && strategy_type==strategy::phys14){
//    BuildModule(HTTSyncTemp("HTTSyncTemp","HTTSequenceSyncfiles/SYNCFILE_" + output_name, channel)
//      .set_is_embedded(is_embedded).set_met_label(met_label).set_ditau_label("ditau").set_jet_label(jets_label));
// }

if(js["baseline"]["do_ff_weights"].asBool()){
  BuildModule(HTTFakeFactorWeights("HTTFakeFactorWeights")
      .set_channel(channel)
      .set_ditau_label("ditau")
      .set_met_label(met_label)
      .set_jets_label(jets_label)
      .set_strategy(strategy_type)
      .set_categories(js["baseline"]["ff_categories"].asString())
      .set_do_systematics(js["baseline"]["do_ff_systematics"].asBool())
      .set_ff_file(js["baseline"]["ff_file"].asString()));
}
    
if(channel != channel::wmnu) {
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
    .set_make_sync_ntuple(js["make_sync_ntuple"].asBool())
    .set_sync_output_name(js["output_folder"].asString()+"/SYNCFILE_"+output_name)
    .set_iso_study(js["iso_study"].asBool())
    .set_tau_id_study(js["tau_id_study"].asBool())
    .set_qcd_study(js["qcd_study"].asBool())
    .set_optimisation_study(js["optimisation_study"].asBool())
    .set_mass_shift(mass_shift)
    .set_add_nlo_weights(js["test_nlo_reweight"].asBool())
    .set_is_embedded(is_embedded)
    .set_is_data(is_data)
    .set_systematic_shift(addit_output_folder!="")
    .set_add_Hhh_variables(js["add_Hhh_variables"].asBool())
    .set_do_HLT_Studies(js["store_hltpaths"].asBool() && (is_data || js["trg_in_mc"].asBool()))
    //Good to avoid accidentally overwriting existing output files when syncing
    .set_write_tree(!js["make_sync_ntuple"].asBool())
    .set_do_ff_weights(js["baseline"]["do_ff_weights"].asBool())
    .set_ff_categories(js["baseline"]["ff_categories"].asString())
    .set_do_ff_systematics(js["baseline"]["do_ff_systematics"].asBool())
    .set_do_qcd_scale_wts(do_qcd_scale_wts_)
    .set_do_pdf_wts(js["do_pdf_wts"].asBool()));

 } else {
BuildModule(WMuNuCategories("WMuNuCategories")
    .set_fs(fs.get())
    .set_channel(channel)
    .set_era(era_type)
    .set_strategy(strategy_type)
    .set_muon_label("sel_muons")
    .set_met_label(met_label)
    .set_jets_label(jets_label)
    .set_is_data(is_data)
    //Good to avoid accidentally overwriting existing output files when syncing
    .set_write_tree(true));

 }
}
}
//}
// --------------------------------------------------------------------------
// TT Pair Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildTTPairs(){
 
 BuildTauSelection();

 BuildModule(CompositeProducer<Tau, Tau>("TTPairProducer")
      .set_input_label_first(js["taus"].asString())
      .set_input_label_second(js["taus"].asString())
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));


 }

  

// --------------------------------------------------------------------------
// ET Pair Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildETPairs() {
  ic::strategy strategy_type  = String2Strategy(strategy_str);

  BuildModule(CopyCollection<Electron>("CopyToSelectedElectrons",
      js["electrons"].asString(), "sel_electrons"));

if(js["do_iso_eff"].asBool()&&!js["make_sync_ntuple"].asBool()){
BuildModule(HTTElectronEfficiency("ElectronEfficiencyForIDStudy")
    .set_dirname("ElectronEfficiencyForIDStudy")
    .set_fs(fs.get()));
}


  std::function<bool(Electron const*)> ElecID;
  if(strategy_type==strategy::paper2013){
    if(special_mode == 20 || special_mode == 22){
     ElecID = [](Electron const* e) { return HttEMuFakeElectron(e); };
    } else if (special_mode == 25){
     ElecID = [](Electron const* e) { return e->pt()>=0;}; //We want this to always return true for the purposes of the filter below
    } else {
      ElecID = [](Electron const* e) { return ElectronHTTId(e, false); };
    }
   } else if(strategy_type==strategy::phys14){
      ElecID = [](Electron const* e) { return ElectronHTTIdPhys14(e, false); };
    } else if(strategy_type==strategy::spring15 || strategy_type==strategy::fall15 || strategy_type==strategy::mssmspring16 || strategy_type==strategy::smspring16){
      ElecID = [](Electron const* e) { return ElectronHTTIdSpring15(e, false); };
    } else if(strategy_type == strategy::mssmsummer16){
      ElecID = [](Electron const* e) { return ElectronHTTIdSpring16(e, false); };
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

if(js["do_iso_eff"].asBool()&&!js["make_sync_ntuple"].asBool()){
BuildModule(HTTElectronEfficiency("ElectronEfficiencyForIsoStudy")
    .set_dirname("ElectronEfficiencyForIsoStudy")
    .set_fs(fs.get()));
}


 double elec_iso_max_val =0.;
 double elec_iso_min_val =0.; 
 elec_iso_min_val = -0.01;
 if(strategy_type == strategy::paper2013){
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
  
//Isolation applied at plotting time for run 2 analysis   
if( strategy_type == strategy::paper2013) {
 if (js["baseline"]["lep_iso"].asBool()&&special_mode !=25 &&special_mode != 23 ) {
    BuildModule(SimpleFilter<Electron>("ElectronIsoFilter")
        .set_input_label("sel_electrons").set_min(1)
        .set_predicate([=](Electron const* e) {
          if(special_mode == 24){
            return !PF04IsolationEBElec(e, 0.5, 0.15, 0.1);
          } else {
            return PF04IsolationVal(e, 0.5, 1) > elec_iso_min_val && PF04IsolationVal(e,0.5, 1) < elec_iso_max_val;
          } 
        }));
  }


/*  if (js["baseline"]["lep_iso"].asBool() &&strategy_type==strategy::phys14) {
    BuildModule(SimpleFilter<Electron>("ElectronIsoFilter")
        .set_input_label("sel_electrons").set_min(1)
        .set_predicate([=](Electron const* e) {
            return PF03IsolationVal(e, 0.5, 1)<0.15;
          //return PF04IsolationEBElec(e, 0.5, 0.15, 0.1);
        }));
  }*/
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
 ic::strategy strategy_type  = String2Strategy(strategy_str);

 BuildModule(CopyCollection<Muon>("CopyToSelectedMuons",
      js["muons"].asString(), "sel_muons"));


  std::function<bool(Muon const*)> MuonID;
  if(strategy_type==strategy::paper2013){
    if(special_mode == 21 || special_mode == 22){
     MuonID = [](Muon const* m) { return HttEMuFakeMuon(m); };
    } else if(special_mode ==25){
     MuonID = [](Muon const* m) {return m->pt()>= 0; } ;
    } else { 
      MuonID = [](Muon const* m) { return MuonTight(m); };
    }
   } else if (strategy_type!=strategy::mssmspring16&&strategy_type!=strategy::smspring16&&strategy_type!=strategy::mssmsummer16){
    MuonID = [](Muon const* m) { return MuonMedium(m); };
   } else if (strategy_type==strategy::mssmspring16||strategy_type==strategy::smspring16) {
    MuonID = [](Muon const* m) {return MuonMediumHIPsafe(m); };
   } else if (strategy_type==strategy::mssmsummer16) {
    if( !is_data || output_name.find("MuonEGG") != output_name.npos || output_name.find("MuonEGH") != output_name.npos || output_name.find("SingleElectronEGG") != output_name.npos || output_name.find("SingleElectronH") != output_name.npos || output_name.find("SingleMuonG") != output_name.npos || output_name.find("SingleMuonH") != output_name.npos || output_name.find("TauG") != output_name.npos || output_name.find("TauH") != output_name.npos) MuonID = [](Muon const* m) {return MuonMedium(m); };
    else MuonID = [](Muon const* m) {return MuonMediumHIPsafe(m); };
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

//Isolation applied at plotting time for run 2 analysis   
if(strategy_type == strategy::paper2013) {
  if (js["baseline"]["lep_iso"].asBool()&&special_mode !=25 &&special_mode != 22 &&special_mode != 21 ) {
    BuildModule(SimpleFilter<Muon>("MuonIsoFilter")
        .set_input_label("sel_muons").set_min(1)
        .set_predicate([=](Muon const* m) {
          return PF04IsolationVal(m, 0.5, 1)<muon_iso_max && PF04IsolationVal(m,0.5,1)>muon_iso_min;
        }));
  }



/*  if (js["baseline"]["lep_iso"].asBool()&&strategy_type==strategy::phys14) {
    BuildModule(SimpleFilter<Muon>("MuonIsoFilter")
        .set_input_label("sel_muons").set_min(1)
        .set_predicate([=](Muon const* m) {
          return PF03IsolationVal(m, 0.5,0)<0.1;
        }));
  }*/
}
 

if(js["do_iso_eff"].asBool()&&!js["make_sync_ntuple"].asBool()){
BuildModule(HTTMuonEfficiency("MuonEfficiency")
    .set_fs(fs.get()));
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

 ic::strategy strategy_type  = String2Strategy(strategy_str);

 if(tau_scale_mode > 0 && !is_data && strategy_type!=strategy::fall15 && strategy_type!=strategy::mssmspring16&&strategy_type!=strategy::smspring16 && strategy_type != strategy::mssmsummer16){
   BuildModule(EnergyShifter<Electron>("ElectronEnergyScaleCorrection")
        .set_input_label(js["electrons"].asString())
        .set_shift(tau_shift));
 }

 if(tau_scale_mode >0 && !is_data && (strategy_type==strategy::fall15 || strategy_type==strategy::mssmspring16 || strategy_type==strategy::smspring16 || strategy_type == strategy::mssmsummer16)){
    BuildModule(HTTEnergyScale("ElectronEnergyScaleCorrection")
        .set_input_label(js["electrons"].asString())
        .set_shift(elec_shift_barrel)
        .set_shift_endcap(elec_shift_endcap)
        .set_strategy(strategy_type)
        .set_channel(channel::em)
        .set_moriond_corrections(moriond_tau_scale));
 }

  if (js["baseline"]["do_em_extras"].asBool()&&strategy_type==strategy::paper2013) {
    BuildModule(HTTEMuExtras("EMExtras"));
  }

  BuildModule(CopyCollection<Electron>("CopyToSelectedElectrons",
      js["electrons"].asString(), "sel_electrons"));

  BuildModule(CopyCollection<Muon>("CopyToSelectedMuons",
      js["muons"].asString(), "sel_muons"));


  std::function<bool(Electron const*)> ElecID;
  if(strategy_type==strategy::paper2013){
    if(special_mode == 20 || special_mode == 22){
     ElecID = [](Electron const* e) { return HttEMuFakeElectron(e); };
    } else if(special_mode ==25){
     ElecID = [](Electron const* e) {return e->pt()>=0; } ;//Want this to always return true (and not to get compiler warnings)
    } else {
      ElecID = [](Electron const* e) { return ElectronHTTId(e, true); };
    }
   } else if(strategy_type==strategy::phys14){
      ElecID = [](Electron const* e) { return ElectronHTTIdPhys14(e, false); };
   } else if(strategy_type==strategy::spring15 || strategy_type==strategy::fall15 || strategy_type==strategy::mssmspring16 || strategy_type==strategy::smspring16){
      ElecID = [](Electron const* e) { return ElectronHTTIdSpring15(e, false); };
   } else if(strategy_type == strategy::mssmsummer16){
      ElecID = [](Electron const* e) { return ElectronHTTIdSpring16(e, false); };
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
 if(strategy_type == strategy::paper2013){
  if(special_mode == 2){
   elec_iso_max_val = 0.5;
  } else if (special_mode ==3 || special_mode ==4){
   elec_iso_max_val = 0.5;
   elec_iso_min_val = 0.2;
  } 
 }
   

//Isolation applied at plotting time for run 2 analysis   
if(strategy_type == strategy::paper2013) {
 if (js["baseline"]["lep_iso"].asBool()&&special_mode!=22&&special_mode!=20&&special_mode !=25 &&special_mode != 23 &&strategy_type==strategy::paper2013) {
    BuildModule(SimpleFilter<Electron>("ElectronIsoFilter")
        .set_input_label("sel_electrons").set_min(1)
        .set_predicate([=](Electron const* e) {
          if(special_mode == 24){
            return !PF04IsolationEBElec(e, 0.5, 0.15, 0.1);
          } else if (special_mode == 2 || special_mode ==3){
            return PF04IsolationVal(e, 0.5,1) > elec_iso_min_val && PF04IsolationVal(e,0.5,1) < elec_iso_max_val;
          } else {
            return PF04IsolationEBElec(e, 0.5, 0.15, 0.1);
          }
        }));
  }

/*  if (js["baseline"]["lep_iso"].asBool() &&strategy_type==strategy::phys14) {
    BuildModule(SimpleFilter<Electron>("ElectronIsoFilter")
        .set_input_label("sel_electrons").set_min(1)
        .set_predicate([=](Electron const* e) {
            return PF03IsolationVal(e, 0.5,1)<0.15;
        }));
  }*/
}

//  BuildModule(OverlapFilter<Electron, Muon>("ElecMuonOverlapFilter")
//      .set_input_label("sel_electrons")
//      .set_reference_label(js["muons"].asString())
//      .set_min_dr(0.3));



  std::function<bool(Muon const*)> MuonID;
  if(strategy_type==strategy::paper2013){
    if(special_mode == 21 || special_mode == 22){
     MuonID = [](Muon const* m) { return HttEMuFakeMuon(m); };
    } else if(special_mode ==25){
     MuonID = [](Muon const* m) {return m->pt()>=0; } ;
    } else { 
      MuonID = [](Muon const* m) { return MuonTight(m); };
    }
   } else if (strategy_type!=strategy::mssmspring16&&strategy_type!=strategy::smspring16&&strategy_type!=strategy::mssmsummer16){
    MuonID = [](Muon const* m) { return MuonMedium(m); };
   } else if (strategy_type==strategy::mssmspring16||strategy_type==strategy::smspring16) {
    MuonID = [](Muon const* m) {return MuonMediumHIPsafe(m); };
   } else if (strategy_type==strategy::mssmsummer16) {
    if( !is_data || output_name.find("MuonEGG") != output_name.npos || output_name.find("MuonEGH") != output_name.npos || output_name.find("SingleElectronEGG") != output_name.npos || output_name.find("SingleElectronH") != output_name.npos || output_name.find("SingleMuonG") != output_name.npos || output_name.find("SingleMuonH") != output_name.npos || output_name.find("TauG") != output_name.npos || output_name.find("TauH") != output_name.npos) MuonID = [](Muon const* m) {return MuonMedium(m); };
    else MuonID = [](Muon const* m) {return MuonMediumHIPsafe(m); };
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


//Isolation applied at plotting time for run 2 analysis   
if(strategy_type == strategy::paper2013) {
  if (js["baseline"]["lep_iso"].asBool()&&special_mode !=25 &&special_mode != 22 &&special_mode != 21) {
    BuildModule(SimpleFilter<Muon>("MuonIsoFilter")
        .set_input_label("sel_muons").set_min(1)
        .set_predicate([=](Muon const* m) {
         if(special_mode == 2|| special_mode ==3 ||special_mode ==4){
          return PF04IsolationVal(m, 0.5,1)<muon_iso_max && PF04IsolationVal(m,0.5,1)>muon_iso_min;
         } else{
          return PF04IsolationEB(m, 0.5, 0.15, 0.1);
        }
        }));
  }


  /*if (js["baseline"]["lep_iso"].asBool()&&strategy_type==strategy::phys14) {
    BuildModule(SimpleFilter<Muon>("MuonIsoFilter")
        .set_input_label("sel_muons").set_min(1)
        .set_predicate([=](Muon const* m) {
          return PF03IsolationVal(m, 0.5,0)<0.15;
        }));
  }*/
}
     if(js["do_iso_eff"].asBool()&&!js["make_sync_ntuple"].asBool()){
BuildModule(HTTElectronEfficiency("ElectronEfficiency")
    .set_dirname("ElectronEfficiencyForIDStudy")
    .set_fs(fs.get()));
BuildModule(HTTMuonEfficiency("MuonEfficiency")
    .set_fs(fs.get()));
}

  
  BuildModule(CompositeProducer<Electron, Muon>("EMPairProducer")
      .set_input_label_first("sel_electrons")
      .set_input_label_second("sel_muons")
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));

if(strategy_type == strategy::fall15){
  BuildModule(SimpleFilter<CompositeCandidate>("EMPairFilter")
      .set_input_label("ditau").set_min(1)
      .set_predicate([=](CompositeCandidate const* c) {
        return PairOneWithPt(c, 18.0);
      }));
  }
if(strategy_type == strategy::mssmspring16 || strategy_type == strategy::smspring16 || strategy_type == strategy::mssmsummer16){
  BuildModule(SimpleFilter<CompositeCandidate>("EMPairFilter")
      .set_input_label("ditau").set_min(1)
      .set_predicate([=](CompositeCandidate const* c) {
        return PairOneWithPt(c, 24.0);
      }));
  }



}

// --------------------------------------------------------------------------
// ZEE Pair Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildZEEPairs() {

 ic::strategy strategy_type  = String2Strategy(strategy_str);


  BuildModule(CopyCollection<Electron>("CopyToSelectedElectrons",
      js["electrons"].asString(), "sel_electrons"));

  std::function<bool(Electron const*)> ElecID;
   if(strategy_type==strategy::spring15 || strategy_type==strategy::fall15 ||strategy_type==strategy::mssmspring16||strategy_type==strategy::smspring16){
      ElecID = [](Electron const* e) { return ElectronHTTIdSpring15(e, false); };
   } else if(strategy_type == strategy::mssmsummer16){
      ElecID = [](Electron const* e) { return ElectronHTTIdSpring16(e, false); };
   }

  BuildModule(SimpleFilter<Electron>("ElectronFilter")
      .set_input_label("sel_electrons").set_min(2)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > 20.        &&
                fabs(e->eta())          < 2.4        &&
                fabs(e->dxy_vertex())   < elec_dxy   &&
                fabs(e->dz_vertex())    < elec_dz    &&
                ElecID(e) ;

      }));
  
  BuildModule(CopyCollection<Electron>("CopyToSelectedElectrons",
      "sel_electrons", "sel_lead_electrons"));
  
  BuildModule(SimpleFilter<Electron>("ElectronFilter")
      .set_input_label("sel_lead_electrons").set_min(1)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > elec_pt    &&
                fabs(e->eta())          < elec_eta   &&
                fabs(e->dxy_vertex())   < elec_dxy   &&
                fabs(e->dz_vertex())    < elec_dz    &&
                ElecID(e) ;

      }));

     
  BuildModule(CompositeProducer<Electron, Electron>("ZEEPairProducer")
      .set_input_label_first("sel_lead_electrons")
      .set_input_label_second("sel_electrons")
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));

}

// --------------------------------------------------------------------------
// Tag and probe ZEE Pair Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildTPZEEPairs() {

 ic::strategy strategy_type  = String2Strategy(strategy_str);


  BuildModule(CopyCollection<Electron>("CopyToSelectedElectrons",
      js["electrons"].asString(), "sel_electrons"));

  std::function<bool(Electron const*)> ElecID;
   if(strategy_type==strategy::spring15||strategy_type ==strategy::fall15 || strategy_type==strategy::mssmspring16 || strategy_type==strategy::smspring16){
      ElecID = [](Electron const* e) { return ElectronHTTIdSpring15(e, false); };
   } else if(strategy_type==strategy::mssmsummer16){
      ElecID = [](Electron const* e) { return ElectronHTTIdSpring16(e, false); };
   }

  BuildModule(SimpleFilter<Electron>("ProbeFilter")
      .set_input_label("sel_electrons").set_min(2)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > elec_pt    &&
                fabs(e->eta())          < elec_eta   &&
                fabs(e->dxy_vertex())   < elec_dxy   &&
                fabs(e->dz_vertex())    < elec_dz    ;

      }));

/*  BuildModule(SimpleFilter<Electron>("TagFilter")
      .set_input_label("sel_electrons").set_min(1)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > elec_pt    &&
                fabs(e->eta())          < elec_eta   &&
                fabs(e->dxy_vertex())   < elec_dxy   &&
                fabs(e->dz_vertex())    < elec_dz  ;//  &&
//                ElecID(e) ;

      }));*/

  BuildModule(CompositeProducer<Electron, Electron>("ZEEPairProducer")
      .set_input_label_first("sel_electrons")
      .set_input_label_second("sel_electrons")
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));

}

// --------------------------------------------------------------------------
// ZMM Pair Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildZMMPairs() {

 ic::strategy strategy_type  = String2Strategy(strategy_str);
 
 if (tau_scale_mode > 0 && strategy_type == strategy::mssmsummer16){
 BuildModule(EnergyShifter<Muon>("MuonEnergyScaleCorrection")
    .set_input_label("muons")
    .set_shift(muon_shift));
 }
 
 BuildModule(CopyCollection<Muon>("CopyToSelectedMuons",
      js["muons"].asString(), "sel_muons"));

  std::function<bool(Muon const*)> MuonID;
  if(strategy_type==strategy::spring15 || strategy_type==strategy::fall15){
    MuonID = [](Muon const* m) { return MuonMedium(m); };
  } else if (strategy_type==strategy::mssmspring16 ||strategy_type==strategy::smspring16){
    MuonID = [](Muon const* m) { return MuonMediumHIPsafe(m); };
  } else if (strategy_type==strategy::mssmsummer16) {
    if( !is_data || output_name.find("MuonEGG") != output_name.npos || output_name.find("MuonEGH") != output_name.npos || output_name.find("SingleElectronEGG") != output_name.npos || output_name.find("SingleElectronH") != output_name.npos || output_name.find("SingleMuonG") != output_name.npos || output_name.find("SingleMuonH") != output_name.npos || output_name.find("TauG") != output_name.npos || output_name.find("TauH") != output_name.npos) MuonID = [](Muon const* m) {return MuonMedium(m); };
    else MuonID = [](Muon const* m) {return MuonMediumHIPsafe(m); };
   }
   
  BuildModule(SimpleFilter<Muon>("MuonFilter")
      .set_input_label("sel_muons").set_min(2)
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > 10.        &&
                fabs(m->eta())          < muon_eta   &&
                fabs(m->dxy_vertex())   < muon_dxy   &&
                fabs(m->dz_vertex())    < muon_dz   &&
                MuonID(m);

      }));
  
  BuildModule(CopyCollection<Muon>("CopyToSelectedMuons",
      "sel_muons", "sel_lead_muons"));
  
  BuildModule(SimpleFilter<Muon>("LeadMuonFilter")
      .set_input_label("sel_lead_muons").set_min(1)
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > muon_pt    &&
                fabs(m->eta())          < muon_eta   &&
                fabs(m->dxy_vertex())   < muon_dxy   &&
                fabs(m->dz_vertex())    < muon_dz   &&
                MuonID(m);

      }));
  
  BuildModule(CompositeProducer<Muon, Muon>("ZMMPairProducer")
      .set_input_label_first("sel_lead_muons")
      .set_input_label_second("sel_muons")
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));

}

// --------------------------------------------------------------------------
// Tag and probe ZMM Pair Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildTPZMMPairs() {

 ic::strategy strategy_type  = String2Strategy(strategy_str);
 
 BuildModule(CopyCollection<Muon>("CopyToSelectedMuons",
      js["muons"].asString(), "sel_muons"));

  std::function<bool(Muon const*)> MuonID;
  if(strategy_type==strategy::spring15||strategy_type==strategy::fall15){
    MuonID = [](Muon const* m) { return MuonMedium(m); };
  } else if (strategy_type==strategy::mssmspring16 ||strategy_type==strategy::smspring16){
    MuonID = [](Muon const* m) { return MuonMediumHIPsafe(m); };
  } else if (strategy_type==strategy::mssmsummer16) {
    if( !is_data || output_name.find("MuonEGG") != output_name.npos || output_name.find("MuonEGH") != output_name.npos || output_name.find("SingleElectronEGG") != output_name.npos || output_name.find("SingleElectronH") != output_name.npos || output_name.find("SingleMuonG") != output_name.npos || output_name.find("SingleMuonH") != output_name.npos || output_name.find("TauG") != output_name.npos || output_name.find("TauH") != output_name.npos) MuonID = [](Muon const* m) {return MuonMedium(m); };
    else MuonID = [](Muon const* m) {return MuonMediumHIPsafe(m); };
   }

  BuildModule(SimpleFilter<Muon>("ProbeFilter")
      .set_input_label("sel_muons").set_min(2)
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > muon_pt    &&
                fabs(m->eta())          < muon_eta   &&
                fabs(m->dxy_vertex())   < muon_dxy   &&
                fabs(m->dz_vertex())    < muon_dz  ;

      }));
  
/*  BuildModule(SimpleFilter<Muon>("TagFilter")
      .set_input_label("sel_muons").set_min(1)
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > muon_pt    &&
                fabs(m->eta())          < muon_eta   &&
                fabs(m->dxy_vertex())   < muon_dxy   &&
                fabs(m->dz_vertex())    < muon_dz   &&
                MuonID(m);

      }));*/
  
  BuildModule(CompositeProducer<Muon, Muon>("ZMMPairProducer")
      .set_input_label_first("sel_muons")
      .set_input_label_second("sel_muons")
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));


}

// --------------------------------------------------------------------------
// W->MuNu Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildWMuNu() {

 ic::strategy strategy_type  = String2Strategy(strategy_str);
 
 BuildModule(CopyCollection<Muon>("CopyToSelectedMuons",
      js["muons"].asString(), "sel_muons"));

  std::function<bool(Muon const*)> MuonID;
  if(strategy_type==strategy::spring15||strategy_type==strategy::fall15){
    MuonID = [](Muon const* m) { return MuonMedium(m); };
  } else if(strategy_type == strategy::mssmspring16 || strategy_type == strategy::smspring16){
    MuonID = [](Muon const* m) { return MuonMediumHIPsafe(m); };
  } else if (strategy_type==strategy::mssmsummer16) {
    if( !is_data || output_name.find("MuonEGG") != output_name.npos || output_name.find("MuonEGH") != output_name.npos || output_name.find("SingleElectronEGG") != output_name.npos || output_name.find("SingleElectronH") != output_name.npos || output_name.find("SingleMuonG") != output_name.npos || output_name.find("SingleMuonH") != output_name.npos || output_name.find("TauG") != output_name.npos || output_name.find("TauH") != output_name.npos) MuonID = [](Muon const* m) {return MuonMedium(m); };
    else MuonID = [](Muon const* m) {return MuonMediumHIPsafe(m); };
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

}

// --------------------------------------------------------------------------
// Tau Selection Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildTauSelection(){
  Json::Value base = js["baseline"];
  ic::strategy strategy_type  = String2Strategy(strategy_str);
  ic::channel channel_type         = String2Channel(channel_str);

 bool moriond_tau_scale =false;
 if(real_tau_sample&&strategy_type==strategy::paper2013) moriond_tau_scale = true; 
 
 if (tau_scale_mode > 0 && !is_data&& (!moriond_tau_scale||strategy_type==strategy::spring15||strategy_type==strategy::fall15||strategy_type==strategy::mssmspring16||strategy_type==strategy::smspring16 || strategy_type == strategy::mssmsummer16)){
    // Tau energy scale is applied to genuine taus only - this works by selecting taus matched to generator level hadronic taus and saving these as a collection of pointers. The the shift is then applied to this collection which in turn shifts the tau energy for all corresponding taus in the origional tau collection (this will only work if both collections of taus are stored as pointers!)
    BuildModule(HTTGenMatchSelector<Tau>("HTTGenMatchSelector")
      .set_input_vec_label(js["taus"].asString())
      .set_output_vec_label("genmatched_taus")
      .set_gen_match(mcorigin::tauHad));

    BuildModule(CopyCollection<Tau>("CopyTauHadTo1Prong0Pi",
      "genmatched_taus", "genmatched_taus_1prong0pi0"));
    
    BuildModule(CopyCollection<Tau>("CopyTauHadTo1Prong1Pi",
      "genmatched_taus", "genmatched_taus_1prong1pi0"));

    BuildModule(CopyCollection<Tau>("CopyTauHadTo3Prong0Pi",
      "genmatched_taus", "genmatched_taus_3prong0pi0"));

    BuildModule(SimpleFilter<Tau>("1Prong0PiTauHadFilter")
      .set_input_label("genmatched_taus_1prong0pi0")
      .set_predicate([=](Tau const* t) {
        return  t->decay_mode() == 0;
      }));
    
    BuildModule(SimpleFilter<Tau>("1Prong1PiTauHadFilter")
      .set_input_label("genmatched_taus_1prong1pi0")
      .set_predicate([=](Tau const* t) {
        return  t->decay_mode() == 1;
      }));

    BuildModule(SimpleFilter<Tau>("3Prong0PiTauHadFilter")
      .set_input_label("genmatched_taus_3prong0pi0")
      .set_predicate([=](Tau const* t) {
        return  t->decay_mode() == 10;
      }));
     
    BuildModule(EnergyShifter<Tau>("TauEnergyShifter1prong0pi0")
    .set_input_label("genmatched_taus_1prong0pi0")
    .set_save_shifts(true) 
    .set_shift_label("scales_taues_1prong0pi0") 
    .set_shift(tau_shift_1prong0pi0));

    BuildModule(EnergyShifter<Tau>("TauEnergyShifter1prong1pi0")
    .set_input_label("genmatched_taus_1prong1pi0")
    .set_save_shifts(true) 
    .set_shift_label("scales_taues_1prong1pi0") 
    .set_shift(tau_shift_1prong1pi0));

    BuildModule(EnergyShifter<Tau>("TauEnergyShifter3prong0pi0")
    .set_input_label("genmatched_taus_3prong0pi0")
    .set_save_shifts(true) 
    .set_shift_label("scales_taues_3prong0pi0") 
    .set_shift(tau_shift_3prong0pi0));
 }
 
  if (!is_data &&strategy_type == strategy::mssmsummer16){
    BuildModule(HTTGenMatchSelector<Tau>("FakeEGenMatchSelector")
      .set_input_vec_label(js["taus"].asString())
      .set_output_vec_label("fakeE_genmatched_taus")
      .set_gen_match(mcorigin::promptE));
    
    BuildModule(CopyCollection<Tau>("CopyTo1Prong0Pi",
      "fakeE_genmatched_taus", "fakeE_genmatched_taus_0pi"));
    
    BuildModule(CopyCollection<Tau>("CopyTo1Prong1Pi",
      "fakeE_genmatched_taus", "fakeE_genmatched_taus_1pi"));
    
    BuildModule(SimpleFilter<Tau>("1Prong0PiTauFilter")
      .set_input_label("fakeE_genmatched_taus_0pi")
      .set_predicate([=](Tau const* t) {
        return  t->decay_mode() == 0;
      }));
    
    BuildModule(SimpleFilter<Tau>("1Prong1PiTauFilter")
      .set_input_label("fakeE_genmatched_taus_1pi")
      .set_predicate([=](Tau const* t) {
        return  t->decay_mode() == 1;
      }));
     
    BuildModule(EnergyShifter<Tau>("FakeE1Prong0PiEnergyShifter")
    .set_input_label("fakeE_genmatched_taus_0pi")
    .set_save_shifts(true)
    .set_shift_label("scales_efaketaues_1prong0pi0")
    .set_shift(fakeE_tau_shift_0pi));
    
    BuildModule(EnergyShifter<Tau>("FakeE1Prong1PiEnergyShifter")
    .set_input_label("fakeE_genmatched_taus_1pi")
    .set_save_shifts(true)
    .set_shift_label("scales_efaketaues_1prong1pi0")
    .set_shift(fakeE_tau_shift_1pi));
  }

 if(moriond_tau_scale&&(!is_data||is_embedded)){
  BuildModule(HTTEnergyScale("TauEnergyScaleCorrection")
      .set_input_label("taus")
      .set_shift(base["tau_es_shift"].asDouble())
      .set_strategy(strategy_type)
      .set_moriond_corrections(moriond_tau_scale));
 }

if(strategy_type == strategy::paper2013){
  BuildModule(SimpleFilter<Tau>("TauFilter")
      .set_input_label(js["taus"].asString()).set_min(min_taus)
      .set_predicate([=](Tau const* t) {
        return  t->pt()                     >  tau_pt     &&
                fabs(t->eta())              <  tau_eta    &&
                fabs(t->lead_dz_vertex())   <  tau_dz     &&
                t->GetTauID("decayModeFinding") > 0.5;

      }));
  } else if(strategy_type == strategy::phys14 || strategy_type == strategy::spring15){
  BuildModule(SimpleFilter<Tau>("TauFilter")
      .set_input_label(js["taus"].asString()).set_min(min_taus)
      .set_predicate([=](Tau const* t) {
        return  t->pt()                     >  tau_pt     &&
                fabs(t->eta())              <  tau_eta    &&
                fabs(t->lead_dz_vertex())   <  tau_dz     &&
                fabs(t->charge())           == 1          &&
                t->GetTauID("decayModeFindingNewDMs") > 0.5;

      }));
  } else if (strategy_type == strategy::fall15||strategy_type == strategy::mssmspring16 ||strategy_type==strategy::smspring16 || strategy_type == strategy::mssmsummer16){
  BuildModule(SimpleFilter<Tau>("TauFilter")
      .set_input_label(js["taus"].asString()).set_min(min_taus)
      .set_predicate([=](Tau const* t) {
        return  t->pt()                     >  tau_pt     &&
                fabs(t->eta())              <  tau_eta    &&
                fabs(t->lead_dz_vertex())   <  tau_dz     &&
                fabs(t->charge())           == 1          &&
                t->GetTauID("decayModeFinding") > 0.5;

      }));
   }

if(js["do_tau_eff"].asBool()&&!js["make_sync_ntuple"].asBool()){
BuildModule(HTTTauEfficiency("HTTTauEfficiency")
    .set_fs(fs.get()));
}

if(strategy_type == strategy::phys14){
  BuildModule(VertexFilter<Tau>("TauVertexFilter")
    .set_input_label(js["taus"].asString()).set_min(min_taus));
 }

//Isolation and anti-muon/anti-electron discriminators applied at plotting time for run 2 analysis   
if(strategy_type==strategy::paper2013) {
  if (base["lep_iso"].asBool()) {
  if(strategy_type!= strategy::phys14 && strategy_type!=strategy::paper2013 && strategy_type!=strategy::spring15){
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
        });
      if(channel_type == channel::mt && strategy_type==strategy::paper2013){
       tauAntiMuonFilter.set_predicate([=](Tau const* t){
         return t->GetTauID(tau_anti_muon_discr)> 0.5 &&
                TauEoverP(t, 0.2);
         });
       }
  BuildModule(tauAntiMuonFilter);
  }
 }
}


void HTTSequence::BuildDiElecVeto() {
  ic::strategy strategy_type  = String2Strategy(strategy_str);

//  if(strategy_type!=strategy::spring15){
  BuildModule(CopyCollection<Electron>("CopyToVetoElecs",
      js["electrons"].asString(), "veto_elecs"));
 /* } else {
    BuildModule(CopyCollection<Electron>("CopyToVetoElecs",
        js["electrons"].asString(),"pre_iso_veto_elecs"));
  }*/

   
/*  if(strategy_type==strategy::spring15){
    BuildModule(GenericModule("vetoElecIsolation")
      .set_function([](ic::TreeEvent *event){
        std::vector<ic::Electron*> elecs = event->GetPtrVec<Electron>("pre_iso_veto_elecs");
 //       ic::EventInfo* evtInfo = event->GetPtr<EventInfo>("eventInfo");
        ic::erase_if_not(elecs,[=](ic::Electron* e){return PF03IsolationVal(e,0.5,0)<0.3;});
        event->Add("veto_elecs",elecs);
        return 0;
       }));
   }*/

  SimpleFilter<Electron> vetoElecFilter = SimpleFilter<Electron>("VetoElecFilter")
      .set_input_label("veto_elecs");
   if(strategy_type==strategy::paper2013){
      vetoElecFilter.set_predicate([=](Electron const* e) {
        return  e->pt()                 > veto_dielec_pt    &&
                fabs(e->eta())          < veto_dielec_eta   &&
                fabs(e->dxy_vertex())   < veto_dielec_dxy   &&
                fabs(e->dz_vertex())    < veto_dielec_dz   &&
                Electron2011WP95ID(e)     &&                
                PF04IsolationVal(e, 0.5,1) < 0.3;
      });
    } else if(strategy_type==strategy::phys14){
       vetoElecFilter.set_predicate([=](Electron const* e) {
        return  e->pt()                 > veto_dielec_pt    &&
                fabs(e->eta())          < veto_dielec_eta   &&
                fabs(e->dxy_vertex())   < veto_dielec_dxy   &&
                fabs(e->dz_vertex())    < veto_dielec_dz   &&
                VetoElectronIDPhys14(e)           &&
                //PF04IsolationVal(e, 0.5,0) < 0.3;
                PF03IsolationVal(e, 0.5,0) < 0.3;
      });
   } else if(strategy_type==strategy::spring15||strategy_type==strategy::fall15||strategy_type==strategy::mssmspring16 ||strategy_type==strategy::smspring16){
       vetoElecFilter.set_predicate([=](Electron const* e) {
        return  e->pt()                 > veto_dielec_pt    &&
                fabs(e->eta())          < veto_dielec_eta   &&
                fabs(e->dxy_vertex())   < veto_dielec_dxy   &&
                fabs(e->dz_vertex())    < veto_dielec_dz    &&
                //ElectronHTTIdSpring15(e,true)                   &&
                VetoElectronIDSpring15(e)                   &&
                //PF04IsolationVal(e, 0.5,0) < 0.3;
                PF03IsolationVal(e, 0.5,0) < 0.3;
      });
   }
       else if(strategy_type == strategy::mssmsummer16){
       vetoElecFilter.set_predicate([=](Electron const* e) {
        return  e->pt()                 > veto_dielec_pt    &&
                fabs(e->eta())          < veto_dielec_eta   &&
                fabs(e->dxy_vertex())   < veto_dielec_dxy   &&
                fabs(e->dz_vertex())    < veto_dielec_dz    &&
                //ElectronHTTIdSpring15(e,true)                   &&
                VetoElectronIDSpring16(e)                   &&
                //PF04IsolationVal(e, 0.5,0) < 0.3;
                PF03IsolationVal(e, 0.5,0) < 0.3;
      });
  }

  BuildModule(vetoElecFilter);

  BuildModule(OneCollCompositeProducer<Electron>("VetoElecPairProducer")
      .set_input_label("veto_elecs").set_output_label("elec_veto_pairs")
      .set_candidate_name_first("elec1").set_candidate_name_second("elec2"));

  HTTFilter<CompositeCandidate> vetoElecPairFilter = HTTFilter<CompositeCandidate>("VetoElecPairFilter")
      .set_input_label("elec_veto_pairs").set_min(0).set_max(0)
      .set_predicate([=](CompositeCandidate const* c) {
        return  c->DeltaR("elec1", "elec2") > 0.15 &&
                c->charge() == 0;
      });
	
// Use special mode of veto module which stores the veto value but doesnt actually apply the filter for run 2 analysis	
    if(strategy_type!=strategy::paper2013 ){
  	  vetoElecPairFilter.set_veto_name("dielec_veto");
  		vetoElecPairFilter.set_no_filter(true);
  	}

	BuildModule(vetoElecPairFilter);
 }

 void HTTSequence::BuildDiMuonVeto() {
  ic::strategy strategy_type  = String2Strategy(strategy_str);

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
                PF04IsolationVal(m, 0.5,1) < 0.3;
      });
    } else if(strategy_type==strategy::phys14 || strategy_type==strategy::spring15 || strategy_type==strategy::fall15){
      vetoMuonFilter.set_predicate([=](Muon const* m) {
        return  m->pt()                 > veto_dimuon_pt    &&
                fabs(m->eta())          < veto_dimuon_eta   &&
                fabs(m->dxy_vertex())   < veto_dimuon_dxy   &&
                fabs(m->dz_vertex())    < veto_dimuon_dz    &&
                m->is_global()                    &&
                m->is_tracker()                   &&
                m->is_pf()                        &&
                //PF04IsolationVal(m, 0.5,0) < 0.3;
                PF03IsolationVal(m, 0.5,0) < 0.3;
      });
    } else if(strategy_type==strategy::mssmspring16 ||strategy_type==strategy::smspring16 || strategy_type == strategy::mssmsummer16){
      vetoMuonFilter.set_predicate([=](Muon const* m) {
        return  m->pt()                 > veto_dimuon_pt    &&
                fabs(m->eta())          < veto_dimuon_eta   &&
                fabs(m->dxy_vertex())   < veto_dimuon_dxy   &&
                fabs(m->dz_vertex())    < veto_dimuon_dz    &&
                m->is_global()                    &&
                m->is_tracker()                   &&
                m->is_pf()                        &&
                PF04IsolationVal(m, 0.5,0) < 0.3;
      });
   }

  BuildModule(vetoMuonFilter);

  BuildModule(OneCollCompositeProducer<Muon>("VetoMuonPairProducer")
      .set_input_label("veto_muons").set_output_label("muon_veto_pairs")
      .set_candidate_name_first("muon1").set_candidate_name_second("muon2"));

  HTTFilter<CompositeCandidate> vetoMuonPairFilter = HTTFilter<CompositeCandidate>("VetoMuonPairFilter")
	   .set_input_label("muon_veto_pairs").set_min(0).set_max(0)
		 .set_predicate([=](CompositeCandidate const* c){
		   return c->DeltaR("muon1", "muon2") > 0.15 &&
			        c->charge() == 0;
			});
	
// Use special mode of veto module which stores the veto value but doesnt actually apply the filter for run 2 analysis	
	if(strategy_type==strategy::phys14 || strategy_type==strategy::spring15 ||strategy_type==strategy::fall15||strategy_type==strategy::mssmspring16||strategy_type==strategy::smspring16 || strategy_type == strategy::mssmsummer16) {
	  vetoMuonPairFilter.set_veto_name("dimuon_veto");
		vetoMuonPairFilter.set_no_filter(true);
	}
	
	BuildModule(vetoMuonPairFilter);

}

void HTTSequence::BuildExtraElecVeto(){
  ic::strategy strategy_type  = String2Strategy(strategy_str);

  BuildModule(CopyCollection<Electron>("CopyToExtraElecs",
      js["electrons"].asString(), "extra_elecs"));
 
 HTTFilter<Electron> extraElecFilter = HTTFilter<Electron>("ExtraElecFilter")
      .set_input_label("extra_elecs")
			.set_veto_name("extra_elec_veto")
      .set_min(0).set_max(js["baseline"]["max_extra_elecs"].asUInt());
  if(strategy_type == strategy::paper2013){
      extraElecFilter.set_predicate([=](Electron const* e) {
        return  e->pt()                 > veto_elec_pt    &&
                fabs(e->eta())          < veto_elec_eta   &&
                fabs(e->dxy_vertex())   < veto_elec_dxy   &&
                fabs(e->dz_vertex())    < veto_elec_dz    &&
                ElectronHTTId(e, true)                    &&
                PF04IsolationVal(e, 0.5,1) <0.3;
      });
  }
// Use special mode of veto module which stores the veto value but doesnt actually apply the filter for run 2 analysis	
 if(strategy_type == strategy::phys14){
      extraElecFilter.set_no_filter(true);
      extraElecFilter.set_predicate([=](Electron const* e) {
        return  e->pt()                 > veto_elec_pt    &&
                fabs(e->eta())          < veto_elec_eta   &&
                fabs(e->dxy_vertex())   < veto_elec_dxy   &&
                fabs(e->dz_vertex())    < veto_elec_dz    &&
                ElectronHTTIdPhys14(e, true)             &&
                PF03IsolationVal(e, 0.5,0) < 0.3;
      });
  }

 if(strategy_type==strategy::spring15||strategy_type==strategy::fall15||strategy_type==strategy::mssmspring16||strategy_type==strategy::smspring16){
      extraElecFilter.set_no_filter(true);
      extraElecFilter.set_predicate([=](Electron const* e) {
        return  e->pt()                 > veto_elec_pt    &&
                fabs(e->eta())          < veto_elec_eta   &&
                fabs(e->dxy_vertex())   < veto_elec_dxy   &&
                fabs(e->dz_vertex())    < veto_elec_dz    &&
                ElectronHTTIdSpring15(e, true)           &&
                PF03IsolationVal(e, 0.5,0) < 0.3;
      });
  }
  
 if(strategy_type == strategy::mssmsummer16){
      extraElecFilter.set_no_filter(true);
      extraElecFilter.set_predicate([=](Electron const* e) {
        return  e->pt()                 > veto_elec_pt    &&
                fabs(e->eta())          < veto_elec_eta   &&
                fabs(e->dxy_vertex())   < veto_elec_dxy   &&
                fabs(e->dz_vertex())    < veto_elec_dz    &&
                ElectronHTTIdSpring16(e, true)           &&
                PF03IsolationVal(e, 0.5,0) < 0.3;
      });
  }


BuildModule(extraElecFilter);
 }


void HTTSequence::BuildExtraMuonVeto(){
  ic::strategy strategy_type  = String2Strategy(strategy_str);

  BuildModule(CopyCollection<Muon>("CopyToExtraMuons",
      js["muons"].asString(), "extra_muons"));

  HTTFilter<Muon> extraMuonFilter = HTTFilter<Muon>("ExtraMuonFilter")
      .set_input_label("extra_muons")
			.set_veto_name("extra_muon_veto")
      .set_min(0).set_max(js["baseline"]["max_extra_muons"].asUInt());
  if(strategy_type == strategy::paper2013){
      extraMuonFilter.set_predicate([=](Muon const* m) {
        return  m->pt()                 > veto_muon_pt    &&
                fabs(m->eta())          < veto_muon_eta   &&
                fabs(m->dxy_vertex())   < veto_muon_dxy   &&
                fabs(m->dz_vertex())    < veto_muon_dz    &&
                MuonTight(m)                              &&
                PF04IsolationVal(m, 0.5,1) < 0.3;
      });
   } else if (strategy_type == strategy::phys14 || strategy_type==strategy::spring15 || strategy_type==strategy::fall15){
// Use special mode of veto module which stores the veto value but doesnt actually apply the filter for run 2 analysis	
	    extraMuonFilter.set_no_filter(true);
      extraMuonFilter.set_predicate([=](Muon const* m) {
        return  m->pt()                 > veto_muon_pt    &&
                fabs(m->eta())          < veto_muon_eta   &&
                fabs(m->dxy_vertex())   < veto_muon_dxy   &&
                fabs(m->dz_vertex())    < veto_muon_dz    &&
                MuonMedium(m)                     &&
                //PF04IsolationVal(m, 0.5,0) < 0.3;
                PF03IsolationVal(m, 0.5,0) < 0.3;
      });
   } else if (strategy_type == strategy::mssmspring16||strategy_type==strategy::smspring16){
	    extraMuonFilter.set_no_filter(true);
      extraMuonFilter.set_predicate([=](Muon const* m) {
        return  m->pt()                 > veto_muon_pt    &&
                fabs(m->eta())          < veto_muon_eta   &&
                fabs(m->dxy_vertex())   < veto_muon_dxy   &&
                fabs(m->dz_vertex())    < veto_muon_dz    &&
                MuonMediumHIPsafe(m)                     &&
                PF04IsolationVal(m, 0.5,0) < 0.3;
      });
   } else if (strategy_type == strategy::mssmsummer16){
            std::function<bool(Muon const*)> ExtraMuonID;
            if( !is_data || output_name.find("MuonEGG") != output_name.npos || output_name.find("MuonEGH") != output_name.npos || output_name.find("SingleElectronEGG") != output_name.npos || output_name.find("SingleElectronH") != output_name.npos || output_name.find("SingleMuonG") != output_name.npos || output_name.find("SingleMuonH") != output_name.npos || output_name.find("TauG") != output_name.npos || output_name.find("TauH") != output_name.npos) ExtraMuonID = [](Muon const* m) {return MuonMedium(m); };
            else ExtraMuonID = [](Muon const* m) {return MuonMediumHIPsafe(m); };
	    extraMuonFilter.set_no_filter(true);
      extraMuonFilter.set_predicate([=](Muon const* m) {
        return  m->pt()                 > veto_muon_pt    &&
                fabs(m->eta())          < veto_muon_eta   &&
                fabs(m->dxy_vertex())   < veto_muon_dxy   &&
                fabs(m->dz_vertex())    < veto_muon_dz    &&
                ExtraMuonID(m)                     &&
                PF04IsolationVal(m, 0.5,0) < 0.3;
      });
   }

  BuildModule(extraMuonFilter);

 }

}
