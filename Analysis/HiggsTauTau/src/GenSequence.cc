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
#include "HiggsTauTau/interface/GenSequence.h"
#include "HiggsTauTau/interface/HTTElectronEfficiency.h"
#include "HiggsTauTau/interface/HTTMuonEfficiency.h"
#include "HiggsTauTau/interface/HTTTauEfficiency.h"
#include "HiggsTauTau/interface/HTTTriggerFilter.h"
#include "HiggsTauTau/interface/HTTTriggerFilter2.h"
#include "HiggsTauTau/interface/HTTEnergyScale.h"
#include "HiggsTauTau/interface/HTTEMuExtras.h"
#include "HiggsTauTau/interface/HTTGenEvent.h"
#include "HiggsTauTau/interface/GenTrees.h"
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
#include "HiggsTauTau/interface/NLOWeighting.h"


namespace ic {

GenSequence::GenSequence(std::string& chan, std::string postf, Json::Value const& json) {
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
//	output_folder="GenSequenceSyncfilesNEW/";
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

GenSequence::~GenSequence() {}


void GenSequence::BuildSequence(){
  using ROOT::Math::VectorUtil::DeltaR;
  


  // Set global parameters that get used in multiple places
  ic::channel channel         = String2Channel(channel_str);
  // Other flags

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "HiggsToTauTau Analysis" << std::endl;
  std::cout << "-------------------------------------" << std::endl;      std::string param_fmt = "%-25s %-40s\n";
//  std::cout << boost::format(param_fmt) % "max_events" % max_events;
  		//for(unsigned j=0;j<addit_to_check.size();j++){
  //		to_check.push_back(addit_to_check.at(j));
	 // }


// Defining good-lumi jsons

 if(js["get_effective"].asBool() && js["make_sync_ntuple"].asBool()){
   std::cerr<< "Error: cannot run effective number of event module in make_syncntuple mode"<<std::endl;
   throw;
 }
  BuildModule(GenTrees("GenTrees")
    .set_fs(fs.get())
    .set_channel(channel));


}
}
