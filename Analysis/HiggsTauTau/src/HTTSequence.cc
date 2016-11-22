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
#include "HiggsTauTau/interface/HTTRun2RecoilCorrector.h"
#include "HiggsTauTau/interface/HhhBJetRegression.h"
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
    } 
 }
 if (channel_str == "et"){
   elec_dz = 0.2;
   elec_dxy = 0.045;
   muon_dxy = 0.045;
   muon_dz = 0.2;
   tau_dz = 0.2;
   pair_dr = 0.5;

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
   
     if (strategy_type == strategy::mssmspring16){
       elec_pt = 26;
       tau_pt = 30;
      }
      if (strategy_type == strategy::smspring16){
       elec_pt = 26;
      }
    }
  }
 } 
 if (channel_str == "mt"){
  muon_dxy = 0.045;
  muon_dz = 0.2;
  elec_dxy = 0.045;
  elec_dz = 0.2;
  tau_dz = 0.2;
  pair_dr = 0.5;
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
      if(strategy_type == strategy::mssmspring16){
        tau_pt = 30;
        muon_pt = 23;
      }
      if (strategy_type == strategy::smspring16){
       muon_pt = 23;
      }
    }

   }
 }
 if (channel_str == "tt"){
  tau_pt = 45;

  if (strategy_type == strategy::fall15 || strategy_type == strategy::mssmspring16 || strategy_type == strategy::smspring16){
    tau_pt=40;
    if(js["store_hltpaths"].asBool()) tau_pt = 25;
  }

  tau_eta = 2.1;
  tau_iso = 1.0;
  tau_dz = 0.2;
  min_taus = 2;
  pair_dr = 0.5;
  }
 if(channel_str == "zee"){
   elec_dz = 0.2;
   elec_dxy = 0.045;
   pair_dr = 0.5;
   elec_pt = 20;
   elec_eta = 2.1;
 }
 if(channel_str == "zmm"){
   muon_dz = 0.2;
   muon_dxy = 0.045;
   pair_dr = 0.5;
   muon_pt = 20;
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
  if (era_type == era::data_2015&&output_name.find("2015D")!=output_name.npos)  data_json= "input/json/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt";
  if (era_type == era::data_2016)  data_json= "input/json/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt";


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
 if(is_data){
   LumiMask lumiMask = LumiMask("LumiMask")
     .set_produce_output_jsons("")
     .set_input_file(data_json);
 
 if(js["save_output_jsons"].asBool()){
  lumiMask.set_produce_output_jsons(lumimask_output_name.c_str());
   }
  BuildModule(lumiMask);
 }

if(strategy_type == strategy::fall15 && output_name.find("WGToLNuG")!=output_name.npos){
  SimpleCounter<GenParticle> wgammaStarFilter = SimpleCounter<GenParticle>("WgammaStarSelector")
    .set_input_label("genParticles")
    .set_predicate(
      (bind(&GenParticle::status,_1) == 44) &&
      (bind(::abs,(bind(&GenParticle::pdgid, _1))) == 22))
    .set_min(0).set_max(0);
 
  BuildModule(wgammaStarFilter);
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


  // Trigger filtering
//    if (js["run_trg_filter"].asBool()) {
// if(is_data){

   if(js["store_hltpaths"].asBool()){
       
     if(channel != channel::wmnu) {
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
          .set_gen_taus_label("genParticles")
          .set_scale_met_for_tau((tau_scale_mode > 0 || (moriond_tau_scale && (is_embedded || !is_data) )   ))
          .set_tau_scale(tau_shift)
          .set_use_most_isolated(!(channel == channel::zee || channel == channel::zmm || channel == channel::tpzmm || channel == channel::tpzee))
          .set_use_os_preference((channel == channel::zee || channel == channel::zmm || channel == channel::tpzmm || channel == channel::tpzee))
          .set_allowed_tau_modes(allowed_tau_modes);
      
        BuildModule(httPairSelector);

      }

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
     if((is_data || js["trg_in_mc"].asBool()) && (channel==channel::em || channel==channel::tt || js["do_leptonplustau"].asBool()||js["do_singlelepton"].asBool())){
       if(!is_embedded ){
           BuildModule(HTTTriggerFilter("HTTTriggerFilter")
               .set_channel(channel)
               .set_mc(mc_type)
               .set_era(era_type)
               .set_is_data(is_data)
               .set_is_embedded(is_embedded)
               .set_do_leptonplustau(js["do_leptonplustau"].asBool())
               .set_do_singlelepton(js["do_singlelepton"].asBool())
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
TH1D d_pu = GetFromTFile<TH1D>(js["data_pu_file"].asString(), "/", "pileup");
TH1D m_pu = GetFromTFile<TH1D>(js["mc_pu_file"].asString(), "/", "pileup");
if (js["do_pu_wt"].asBool()&&!is_data) {
  BuildModule( PileupWeight("PileupWeight")
      .set_data(new TH1D(d_pu)).set_mc(new TH1D(m_pu)));
}



if(do_met_filters && is_data){
  BuildModule(GenericModule("MetFilters")
    .set_function([=](ic::TreeEvent *event){
       EventInfo *eventInfo = event->GetPtr<EventInfo>("eventInfo");
       std::vector<std::string> met_filters = {"Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter","Flag_goodVertices","Flag_eeBadScFilter","Flag_globalTightHalo2016Filter"};
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
       std::vector<std::string> met_filters = {"badChargedHadronFilter","badMuonFilter"};
       bool pass_filters = true;
       for(unsigned i=0;i<met_filters.size();++i){
        pass_filters = pass_filters&& eventInfo->filter_result(met_filters.at(i));
       }
       return !pass_filters;
    }));
}


if(channel != channel::wmnu && !js["store_hltpaths"].asBool()) {

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
    .set_gen_taus_label("genParticles")
    .set_scale_met_for_tau((tau_scale_mode > 0 || (moriond_tau_scale && (is_embedded || !is_data) )   ))
    .set_tau_scale(tau_shift)
    .set_use_most_isolated(!(channel == channel::zee || channel == channel::zmm || channel == channel::tpzmm || channel == channel::tpzee))
    .set_use_os_preference(channel == channel::zee || channel == channel::zmm || channel == channel::tpzmm || channel == channel::tpzee)
    .set_allowed_tau_modes(allowed_tau_modes);

  BuildModule(httPairSelector);
}

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
           if(strategy_type != strategy::mssmspring16 && strategy_type != strategy::smspring16){
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
           if(strategy_type != strategy::mssmspring16 && strategy_type != strategy::smspring16){
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


  BuildModule(HTTPairGenInfo("HTTPairGenInfo")
    .set_fs(fs.get())
    .set_write_plots(false)
    .set_ditau_label("ditau"));

if(!is_data&&js["do_btag_eff"].asBool()){
   BuildModule(BTagCheck("BTagCheck")
    .set_fs(fs.get())
    .set_channel(channel)
    .set_do_legacy(true)
    .set_jet_label(jets_label));
}




 if (jes_mode > 0 && !is_data ){
  std::string jes_input_file = "input/jec/Fall15_25nsV6_DATA_UncertaintySources_AK4PFchs.txt";
  std::string jes_input_set  = "Total";
  if (era_type == era::data_2016) {
    jes_input_file = "input/jec/Spring16_25nsV6_DATA_UncertaintySources_AK4PFchs.txt";
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
if(strategy_type != strategy::mssmspring16 && strategy_type != strategy::smspring16){
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


if((strategy_type==strategy::fall15||strategy_type==strategy::mssmspring16||strategy_type==strategy::smspring16)&&!is_data&&js["do_btag_eff"].asBool()){
   BuildModule(BTagCheck("BTagCheck")
    .set_fs(fs.get())
    .set_channel(channel)
    .set_do_legacy(false)
    .set_era(era_type)
    .set_jet_label(jets_label));
}



 if((strategy_type == strategy::fall15|| strategy_type==strategy::mssmspring16) && channel!=channel::wmnu){
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
    .set_do_preselection(false)
    .set_MC(true);

   svFitTest.set_legacy_svfit(false);
   svFitTest.set_do_preselection(!js["make_sync_ntuple"].asBool());
   svFitTest.set_read_svfit_mt(true);
   svFitTest.set_tau_optimisation(js["tau_id_study"].asBool());
   svFitTest.set_read_all(js["read_all_svfit_files"].asBool());
   svFitTest.set_from_grid(js["svfit_from_grid"].asBool());

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


if((strategy_type == strategy::fall15 || strategy_type == strategy::mssmspring16) && !is_data){
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
  }  else {
    bbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_ichep2016.root","/","btag_eff_b");
    cbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_ichep2016.root","/","btag_eff_c");
    othbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_ichep2016.root","/","btag_eff_oth");
  }

  BuildModule(BTagWeightRun2("BTagWeightRun2")
   .set_channel(channel)
   .set_era(era_type)
   .set_jet_label(jets_label)
   .set_bbtag_eff(new TH2F(bbtag_eff))
   .set_cbtag_eff(new TH2F(cbtag_eff))
   .set_othbtag_eff(new TH2F(othbtag_eff))
   .set_do_reshape(do_reshape)
   .set_btag_mode(btag_mode)
   .set_bfake_mode(bfake_mode));
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
    .set_mc(mc_type)
    .set_do_tau_id_weights(real_tau_sample)
    .set_do_em_qcd_weights(true)
    .set_ditau_label("ditau")
    .set_jets_label("ak4PFJetsCHS")
    .set_et_trig_mc(new TH2D(et_trig_mc)).set_et_trig_data(new TH2D(et_trig_data))
    .set_mt_trig_mc(new TH2D(mt_trig_mc)).set_mt_trig_data(new TH2D(mt_trig_data))
    .set_et_idiso_mc(new TH2D(et_idiso_mc)).set_et_idiso_data(new TH2D(et_idiso_data))
    .set_mt_idiso_mc(new TH2D(mt_idiso_mc)).set_mt_idiso_data(new TH2D(mt_idiso_data))
    .set_em_m17_trig_mc(new TH2D(em_m17_trig_mc)).set_em_m17_trig_data(new TH2D(em_m17_trig_data))
    .set_em_m8_trig_mc(new TH2D(em_m8_trig_mc)).set_em_m8_trig_data(new TH2D(em_m8_trig_data))
    .set_em_e17_trig_mc(new TH2D(em_e17_trig_mc)).set_em_e17_trig_data(new TH2D(em_e17_trig_data))
    .set_em_e12_trig_mc(new TH2D(em_e12_trig_mc)).set_em_e12_trig_data(new TH2D(em_e12_trig_data))
    .set_em_e_idiso_mc(new TH2D(em_e_idiso_mc)).set_em_e_idiso_data(new TH2D(em_e_idiso_data))
    .set_em_m_idiso_mc(new TH2D(em_m_idiso_mc)).set_em_m_idiso_data(new TH2D(em_m_idiso_data))
    .set_em_qcd_cr1_lt2(new TH2D(em_qcd_cr1_lt2)).set_em_qcd_cr2_lt2(new TH2D(em_qcd_cr2_lt2))
    .set_em_qcd_cr1_2to4(new TH2D(em_qcd_cr1_2to4)).set_em_qcd_cr2_2to4(new TH2D(em_qcd_cr2_2to4))
    .set_em_qcd_cr1_gt4(new TH2D(em_qcd_cr1_gt4)).set_em_qcd_cr2_gt4(new TH2D(em_qcd_cr2_gt4))
    .set_z_pt_mass_hist(new TH2D(z_pt_weights));
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
    .set_mc(mc_type)
    .set_do_tau_id_weights(real_tau_sample)
    .set_do_tau_id_sf(real_tau_sample)
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
    .set_em_qcd_cr1_lt2(new TH2D(em_qcd_cr1_lt2)).set_em_qcd_cr2_lt2(new TH2D(em_qcd_cr2_lt2))
    .set_em_qcd_cr1_2to4(new TH2D(em_qcd_cr1_2to4)).set_em_qcd_cr2_2to4(new TH2D(em_qcd_cr2_2to4))
    .set_em_qcd_cr1_gt4(new TH2D(em_qcd_cr1_gt4)).set_em_qcd_cr2_gt4(new TH2D(em_qcd_cr2_gt4))
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
    .set_write_tree(!js["make_sync_ntuple"].asBool()));

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
    if(strategy_type==strategy::fall15 || strategy_type==strategy::mssmspring16 || strategy_type==strategy::smspring16){
      ElecID = [](Electron const* e) { return ElectronHTTIdSpring15(e, false); };
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
  if (strategy_type!=strategy::mssmspring16&&strategy_type!=strategy::smspring16){
    MuonID = [](Muon const* m) { return MuonMedium(m); };
  } else {
   MuonID = [](Muon const* m) {return MuonMediumHIPsafe(m); };
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

 if(tau_scale_mode >0 && !is_data && (strategy_type==strategy::fall15 || strategy_type==strategy::mssmspring16 || strategy_type==strategy::smspring16)){
    BuildModule(HTTEnergyScale("ElectronEnergyScaleCorrection")
        .set_input_label(js["electrons"].asString())
        .set_shift(elec_shift_barrel)
        .set_shift_endcap(elec_shift_endcap)
        .set_strategy(strategy_type)
        .set_channel(channel::em)
        .set_moriond_corrections(moriond_tau_scale));
 }

  BuildModule(CopyCollection<Electron>("CopyToSelectedElectrons",
      js["electrons"].asString(), "sel_electrons"));

  BuildModule(CopyCollection<Muon>("CopyToSelectedMuons",
      js["muons"].asString(), "sel_muons"));


  std::function<bool(Electron const*)> ElecID;
  if(strategy_type==strategy::fall15 || strategy_type==strategy::mssmspring16 || strategy_type==strategy::smspring16){
      ElecID = [](Electron const* e) { return ElectronHTTIdSpring15(e, false); };
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



  std::function<bool(Muon const*)> MuonID;
  if (strategy_type!=strategy::mssmspring16 && strategy_type!=strategy::smspring16){
    MuonID = [](Muon const* m) { return MuonMedium(m); };
   } else {
    MuonID = [](Muon const* m) { return MuonMediumHIPsafe(m); };
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
if(strategy_type == strategy::mssmspring16 || strategy_type == strategy::smspring16){
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
   if(strategy_type==strategy::fall15 ||strategy_type==strategy::mssmspring16||strategy_type==strategy::smspring16){
      ElecID = [](Electron const* e) { return ElectronHTTIdSpring15(e, false); };
   }

  BuildModule(SimpleFilter<Electron>("ElectronFilter")
      .set_input_label("sel_electrons").set_min(2)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > elec_pt    &&
                fabs(e->eta())          < elec_eta   &&
                fabs(e->dxy_vertex())   < elec_dxy   &&
                fabs(e->dz_vertex())    < elec_dz    &&
                ElecID(e) ;

      }));

     
  BuildModule(CompositeProducer<Electron, Electron>("ZEEPairProducer")
      .set_input_label_first("sel_electrons")
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
   if(strategy_type ==strategy::fall15 || strategy_type==strategy::mssmspring16 || strategy_type==strategy::smspring16){
      ElecID = [](Electron const* e) { return ElectronHTTIdSpring15(e, false); };
   }

  BuildModule(SimpleFilter<Electron>("ProbeFilter")
      .set_input_label("sel_electrons").set_min(2)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > elec_pt    &&
                fabs(e->eta())          < elec_eta   &&
                fabs(e->dxy_vertex())   < elec_dxy   &&
                fabs(e->dz_vertex())    < elec_dz    ;

      }));


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
 
 BuildModule(CopyCollection<Muon>("CopyToSelectedMuons",
      js["muons"].asString(), "sel_muons"));

  std::function<bool(Muon const*)> MuonID;
  if(strategy_type==strategy::fall15){
    MuonID = [](Muon const* m) { return MuonMedium(m); };
  } else if (strategy_type==strategy::mssmspring16 ||strategy_type==strategy::smspring16){
    MuonID = [](Muon const* m) { return MuonMediumHIPsafe(m); };
  }
  BuildModule(SimpleFilter<Muon>("MuonFilter")
      .set_input_label("sel_muons").set_min(2)
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > muon_pt    &&
                fabs(m->eta())          < muon_eta   &&
                fabs(m->dxy_vertex())   < muon_dxy   &&
                fabs(m->dz_vertex())    < muon_dz   &&
                MuonID(m);

      }));
  
  BuildModule(CompositeProducer<Muon, Muon>("ZMMPairProducer")
      .set_input_label_first("sel_muons")
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
  if(strategy_type==strategy::fall15){
    MuonID = [](Muon const* m) { return MuonMedium(m); };
  } else if (strategy_type==strategy::mssmspring16 ||strategy_type==strategy::smspring16){
    MuonID = [](Muon const* m) { return MuonMediumHIPsafe(m); };
  }

  BuildModule(SimpleFilter<Muon>("ProbeFilter")
      .set_input_label("sel_muons").set_min(2)
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > muon_pt    &&
                fabs(m->eta())          < muon_eta   &&
                fabs(m->dxy_vertex())   < muon_dxy   &&
                fabs(m->dz_vertex())    < muon_dz  ;

      }));
  
  
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
  if(strategy_type==strategy::fall15){
    MuonID = [](Muon const* m) { return MuonMedium(m); };
  } else if(strategy_type == strategy::mssmspring16 || strategy_type == strategy::smspring16){
    MuonID = [](Muon const* m) { return MuonMediumHIPsafe(m); };
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

 if (tau_scale_mode > 0 && (strategy_type==strategy::fall15||strategy_type==strategy::mssmspring16||strategy_type==strategy::smspring16)){
    BuildModule(EnergyShifter<Tau>("TauEnergyShifter")
    .set_input_label(js["taus"].asString())
    .set_shift(tau_shift));
 }


if (strategy_type == strategy::fall15||strategy_type == strategy::mssmspring16 ||strategy_type==strategy::smspring16){
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


}


void HTTSequence::BuildDiElecVeto() {
  ic::strategy strategy_type  = String2Strategy(strategy_str);

  BuildModule(CopyCollection<Electron>("CopyToVetoElecs",
      js["electrons"].asString(), "veto_elecs"));

  SimpleFilter<Electron> vetoElecFilter = SimpleFilter<Electron>("VetoElecFilter")
      .set_input_label("veto_elecs");
   if(strategy_type==strategy::fall15||strategy_type==strategy::mssmspring16 ||strategy_type==strategy::smspring16){
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
  vetoElecPairFilter.set_veto_name("dielec_veto");
	vetoElecPairFilter.set_no_filter(true);

	BuildModule(vetoElecPairFilter);
 }

 void HTTSequence::BuildDiMuonVeto() {
  ic::strategy strategy_type  = String2Strategy(strategy_str);

  BuildModule(CopyCollection<Muon>("CopyToVetoMuons",
      js["muons"].asString(), "veto_muons"));

  SimpleFilter<Muon> vetoMuonFilter = SimpleFilter<Muon>("VetoMuonFilter")
      .set_input_label("veto_muons");
     if(strategy_type==strategy::fall15){
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
    } else if(strategy_type==strategy::mssmspring16 ||strategy_type==strategy::smspring16){
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
	if(strategy_type==strategy::fall15||strategy_type==strategy::mssmspring16||strategy_type==strategy::smspring16) {
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
// Use special mode of veto module which stores the veto value but doesnt actually apply the filter for run 2 analysis	
 if(strategy_type==strategy::fall15||strategy_type==strategy::mssmspring16||strategy_type==strategy::smspring16){
      extraElecFilter.set_no_filter(true);
      extraElecFilter.set_predicate([=](Electron const* e) {
        return  e->pt()                 > veto_elec_pt    &&
                fabs(e->eta())          < veto_elec_eta   &&
                fabs(e->dxy_vertex())   < veto_elec_dxy   &&
                fabs(e->dz_vertex())    < veto_elec_dz    &&
                ElectronHTTIdSpring15(e, true)             &&
                //PF04IsolationVal(e, 0.5,0) < 0.3;
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
 if (strategy_type==strategy::fall15){
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
   }

  BuildModule(extraMuonFilter);

 }

}
