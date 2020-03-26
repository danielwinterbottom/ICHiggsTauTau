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
#include "HiggsTauTauRun2/interface/HTTSequence.h"
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
#include "HiggsTauTau/interface/HTTLegacyRun2RecoilCorrector.h"
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
#include "HiggsTauTau/interface/JetEnergyResolution.h"
#include "HiggsTauTau/interface/HTTPrint.h"
#include "HiggsTauTau/interface/HTTFilter.h"
#include "Modules/interface/LumiMask.h"
#include "HiggsTauTau/interface/VertexFilter.h"
#include "HiggsTauTau/interface/EffectiveEvents.h"
#include "HiggsTauTau/interface/NvtxWeight.h"
#include "HiggsTauTau/interface/BTagWeightRun2.h"
#include "HiggsTauTau/interface/BTagWeightLegacyRun2.h"
#include "HiggsTauTau/interface/HTTGenMatchSelector.h"
#include "HiggsTauTau/interface/HTTFakeFactorWeights.h"
#include "HiggsTauTau/interface/HTTGenAnalysis.h"
#include "HiggsTauTau/interface/TagAndProbe.h"
#include "HiggsTauTau/interface/HTTShiftedJetVariables.h"
#include "HiggsTauTau/interface/HTTSmearScale.h"
#include "HiggsTauTau/interface/HTTPreFireWeight.h"
#include "HiggsTauTau/interface/RhoIDEmbedder.h"
#include "HiggsTauTau/interface/MVADMEmbedder.h"
#include "HiggsTauTau/interface/HTTMuonEnergyScale.h"
#include "HiggsTauTau/interface/Pi0MVA.h"
#include "HiggsTauTau/interface/HTTEventClassifier.h"

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
#include "HiggsTauTau/interface/MELATest.h"
#include "HiggsTauTau/interface/MELATestGen.h"


namespace ic {

HTTSequence::HTTSequence(std::string& chan, std::string postf, Json::Value const& json) {
  if(json["output_name"].asString()!=""){output_name=json["output_name"].asString();} else{std::cout<<"ERROR: output_name not set"<<std::endl; exit(1);};
  do_recoil = json["do_recoil"].asBool() && ((output_name.find("DY")!=output_name.npos && output_name.find("JetsToLL")!=output_name.npos) || output_name.find("HToTauTau")!=output_name.npos || output_name.find("WJetsToLNu") != output_name.npos || output_name.find("W1JetsToLNu") != output_name.npos || output_name.find("W2JetsToLNu")!=output_name.npos || output_name.find("W3JetsToLNu")!=output_name.npos || output_name.find("W4JetsToLNu")!=output_name.npos || output_name.find("WG")!=output_name.npos || output_name.find("EWKW")!=output_name.npos || output_name.find("EWKZ")!=output_name.npos || output_name.find("VBFH")!=output_name.npos || output_name.find("GluGluH")!=output_name.npos || output_name.find("ZHiggs")!=output_name.npos || output_name.find("WHiggs")!=output_name.npos || output_name.find("JJH")!=output_name.npos);
  addit_output_folder=json["baseline"]["addit_output_folder"].asString();
  new_svfit_mode = json["new_svfit_mode"].asUInt();
  if(new_svfit_mode > 0){
    if(json["svfit_folder"].asString()!="") {svfit_folder = json["svfit_folder"].asString();} else {std::cout<<"ERROR: svfit_folder not set"<<std::endl; exit(1);};
    if(addit_output_folder.find("BFAKE_") != output_name.npos || addit_output_folder.find("BTAG_") != output_name.npos) svfit_folder=svfit_folder+"/";
    else if(json["baseline"]["jer_mode"].asUInt() > 0 && do_recoil) svfit_folder=svfit_folder+"/";
    else if(json["baseline"]["jes_mode"].asUInt() > 0 && do_recoil) svfit_folder=svfit_folder+"/";
    else svfit_folder=svfit_folder+"/"+addit_output_folder+"/";
  }
  svfit_override = json["svfit_override"].asString();
  if(json["output_folder"].asString()!=""){output_folder=json["output_folder"].asString();} else{std::cout<<"ERROR: output_folder not set"<<std::endl; exit(1);};
  addit_output_folder=json["baseline"]["addit_output_folder"].asString();
  output_folder=output_folder+"/"+addit_output_folder+"/";
  lumimask_output_name=output_name + "_" +chan + "_" + postf; 
  output_name=output_name + "_" + chan + "_" + postf + ".root";
  special_mode=json["special_mode"].asUInt();
  if (special_mode > 0) output_name = "Special_"+boost::lexical_cast<std::string>(special_mode)+"_" + output_name;
  if(!json["make_sync_ntuple"].asBool()) {
      fs = std::make_shared<fwlite::TFileService>(
       (output_folder+output_name).c_str());
  } else {
      fs = NULL;
  }


  js = json;
  channel_str = chan;
  jes_mode=json["baseline"]["jes_mode"].asUInt();
  jer_mode=json["baseline"]["jer_mode"].asUInt();
  jes_corr_mode=json["baseline"]["jes_corr_mode"].asUInt();
  metscale_mode=json["baseline"]["metscale_mode"].asUInt();
  metres_mode=json["baseline"]["metres_mode"].asUInt();
  metcl_mode=json["baseline"]["metcl_mode"].asUInt();
  metuncl_mode=json["baseline"]["metuncl_mode"].asUInt();
  do_reshape=json["baseline"]["do_reshape"].asBool(); 
  use_deep_csv=json["baseline"]["use_deep_csv"].asBool(); 
  wp_to_check=json["baseline"]["wp_to_check"].asString(); 
  btag_mode=json["baseline"]["btag_mode"].asUInt();
  bfake_mode=json["baseline"]["bfake_mode"].asUInt();
  if(js["mc"].asString()=="" && era_str != "data_2017"){std::cout<<"ERROR: MC type not set"<<std::endl; exit(1);}
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
  lead_tau_pt=0;
  lead_min_taus=0;

  elec_dz = 0.2;
  elec_dxy = 0.045;
  muon_dxy = 0.045;
  muon_dz = 0.2;
  tau_dz = 0.2;
  pair_dr = 0.5;

  if(channel_str == "em"){
    pair_dr = 0.3;
    elec_pt = 13.;
    elec_eta = 2.5;
    muon_pt = 10;
    muon_eta = 2.4;
  }
  if (channel_str == "et"){
    min_taus = 1;
    elec_pt = 25;
    elec_eta = 2.1;
    tau_pt  = 20;
    tau_eta = 2.3;
  } 
  if (channel_str == "mt"){
   muon_eta = 2.1;
   tau_pt = 20;
   tau_eta = 2.3;
   min_taus = 1;
   if(era_type == era::data_2016) muon_pt = 20.0; 
   else  muon_pt = 21.0;
  }
  if (channel_str == "tt"){
   tau_pt=40;
   lead_tau_pt=40;
   if(js["do_vbftrg"].asBool()) {
     tau_pt = 20.;
     lead_tau_pt = 20;
   }
   tau_eta = 2.1;
   min_taus = 2;
   lead_min_taus = 1;
  }
  if(channel_str == "zee"){
    if(era_type == era::data_2017) elec_pt = 28.;
    if(era_type == era::data_2018) elec_pt = 33.;
    else elec_pt = 25.;
    elec_eta = 2.1;
  }
  if(channel_str == "zmm"){
    if(era_type == era::data_2018 || era_type == era::data_2017) muon_pt = 25.;
    else muon_pt = 23.;
    muon_eta = 2.1;
  }
  if(channel_str == "tpzmm"){
    muon_pt = 10;
    muon_eta = 2.4;
  }
  if(channel_str == "tpzee"){
    elec_pt = 10;
    elec_eta = 2.5;
  }
  if(channel_str == "tpmt"){
    muon_pt = 23;
    muon_eta = 2.1;
    min_taus = 1;
    tau_pt = 10; 
    tau_eta = 2.3;
  }
 do_qcd_scale_wts_=false;
 do_qcd_scale_wts_ = js["do_qcd_scale_wts"].asBool();
 if (output_name.find("SUSYGluGluToBBHToTauTau_M") != output_name.npos && output_name.find("NLO") != output_name.npos) do_qcd_scale_wts_=true;
  
 is_data      = json["is_data"].asBool();
 is_embedded  = json["is_embedded"].asBool();
 real_tau_sample = false; //This gets set properly later
 jets_label   = json["jets"].asString();
 met_label    = json["met"].asString();
 if(json["baseline"]["mass_scale_mode"].asBool()==true){
   mass_shift = json["baseline"]["mass_shift"].asDouble();
 } else mass_shift=1.00;
 do_met_filters = json["do_met_filters"].asBool();
 tau_scale_mode = json["baseline"]["tau_scale_mode"].asBool();
 e_scale_mode = json["baseline"]["e_scale_mode"].asBool();
 e_unc_mode = js["baseline"]["e_unc_mode"].asUInt();
 mu_scale_mode = json["baseline"]["mu_scale_mode"].asBool();
 //Need this to correctly set tau /elec ES
 muon_shift = json["baseline"]["muon_es_shift"].asDouble();
 muon_shift_barrel = json["baseline"]["muon_es_shift_barrel"].asDouble();
 muon_shift_nearendcap = json["baseline"]["muon_es_shift_nearendcap"].asDouble();
 muon_shift_farendcap = json["baseline"]["muon_es_shift_farendcap"].asDouble();
 if(channel_str!="em"){
 tau_shift = json["baseline"]["tau_es_shift"].asDouble();
 } else {
 tau_shift = 1.0;//Just to set it to something
 }
 elec_shift_barrel = json["baseline"]["elec_es_shift_barrel"].asDouble();
 elec_shift_endcap = json["baseline"]["elec_es_shift_endcap"].asDouble();
 std::cout << "---\n" << elec_shift_barrel << std::endl;
 tau_shift_1prong0pi0 = 1.0;
 tau_shift_1prong1pi0 = 1.0;
 tau_shift_3prong0pi0 = 1.0;
 tau_shift_3prong1pi0 = 1.0;
 fakeE_tau_shift_0pi = 1.0;
 fakeE_tau_shift_1pi = 1.0;
 fakeE_tau_shift_0pi_endcap = 1.0;
 fakeE_tau_shift_1pi_endcap = 1.0;
 fakeMu_tau_shift_0pi = 1.0;
 fakeMu_tau_shift_1pi = 1.0;
 if(strategy_type==strategy::mssmsummer16 || strategy_type == strategy::smsummer16 || strategy_type == strategy::cpsummer16 ||strategy_type == strategy::legacy16 || strategy_type == strategy::cpdecays16 || strategy_type == strategy::cpsummer17 || strategy_type == strategy::cpdecays17 || strategy_type == strategy::cpdecays18){
   fakeE_tau_shift_0pi = json["baseline"]["efaketau_0pi_es_shift"].asDouble();
   fakeE_tau_shift_1pi = json["baseline"]["efaketau_1pi_es_shift"].asDouble();
   fakeE_tau_shift_0pi_endcap = json["baseline"]["efaketau_0pi_es_shift_endcap"].asDouble();
   fakeE_tau_shift_1pi_endcap = json["baseline"]["efaketau_1pi_es_shift_endcap"].asDouble();
   if(!is_embedded){
     tau_shift_1prong0pi0 = json["baseline"]["tau_1prong0pi0_es_shift"].asDouble();
     tau_shift_1prong1pi0 = json["baseline"]["tau_1prong1pi0_es_shift"].asDouble();
     tau_shift_3prong0pi0 = json["baseline"]["tau_3prong0pi0_es_shift"].asDouble();
     tau_shift_3prong1pi0 = json["baseline"]["tau_3prong1pi0_es_shift"].asDouble();
   } else {
     tau_shift_1prong0pi0 = json["baseline"]["embedtau_1prong0pi0_es_shift"].asDouble();
     tau_shift_1prong1pi0 = json["baseline"]["embedtau_1prong1pi0_es_shift"].asDouble();
     tau_shift_3prong0pi0 = json["baseline"]["embedtau_3prong0pi0_es_shift"].asDouble();
     tau_shift_3prong1pi0 = json["baseline"]["embedtau_3prong1pi0_es_shift"].asDouble();
   }
 }
 if(strategy_type == strategy::smsummer16 || strategy_type == strategy::cpsummer16 || strategy_type == strategy::legacy16 || strategy_type == strategy::cpdecays16 || strategy_type == strategy::cpsummer17 || strategy_type == strategy::cpdecays17 || strategy_type == strategy::cpdecays18){
   fakeMu_tau_shift_0pi = json["baseline"]["mufaketau_0pi_es_shift"].asDouble();
   fakeMu_tau_shift_1pi = json["baseline"]["mufaketau_1pi_es_shift"].asDouble();
 }

tau_shift_func_1prong0pi0 = "";
tau_shift_func_1prong1pi0 = "";
tau_shift_func_3prong0pi0 = "";
tau_shift_func_3prong1pi0 = "";
if(json["baseline"]["tau_shift_func_1prong0pi0"].asString()!="") tau_shift_func_1prong0pi0=json["baseline"]["tau_shift_func_1prong0pi0"].asString();
if(json["baseline"]["tau_shift_func_1prong1pi0"].asString()!="") tau_shift_func_1prong1pi0=json["baseline"]["tau_shift_func_1prong1pi0"].asString();
if(json["baseline"]["tau_shift_func_3prong0pi0"].asString()!="") tau_shift_func_3prong0pi0=json["baseline"]["tau_shift_func_3prong0pi0"].asString();
if(json["baseline"]["tau_shift_func_3prong1pi0"].asString()!="") tau_shift_func_3prong1pi0=json["baseline"]["tau_shift_func_3prong1pi0"].asString();

alt_jes_input_set = json["baseline"]["jes_input_set"].asString();

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
  /
  real_tau_sample = ( (output_name.find("HToTauTau")             != output_name.npos)
                        || (output_name.find("HTohh")                 != output_name.npos)
                        || (output_name.find("AToZh")                 != output_name.npos)
                        || (output_name.find("DYJetsToTauTau")        != output_name.npos)
                        || (output_name.find("Embedded")              != output_name.npos)
                        || (output_name.find("RecHit")                != output_name.npos) 
                        || ((output_name.find("DY") != output_name.npos) && (output_name.find("JetsToLL") != output_name.npos)) );
  if (output_name.find("DYJetsToTauTau-L") != output_name.npos) real_tau_sample = false;
  if (output_name.find("DYJetsToTauTau-JJ") != output_name.npos) real_tau_sample = false;
  if ((era_type == era::data_2016 || era_type == era::data_2017 
              || era_type == era::data_2018) && !is_data) real_tau_sample = true;
  if (channel == channel::zmm || channel == channel::zee) real_tau_sample = false;
  if (channel == channel::em && strategy_type ==strategy::mssmsummer16){
    //Apply jet->lepton fake rates?
    if( (output_name.find("W") != output_name.npos) && (output_name.find("JetsToLNu") != output_name.npos)) jlepton_fake = true; //Applied for W+Jets...
    if( (output_name.find("VV") != output_name.npos) || (output_name.find("ZZ") != output_name.npos) || (output_name.find("WZ") !=output_name.npos) || (output_name.find("WW") != output_name.npos)) jlepton_fake = true; //Applied for diboson
    if( (output_name.find("DY") != output_name.npos) && (output_name.find("JetsToLL") != output_name.npos)) jlepton_fake = true; //Applied for DY
  }
  if (channel == channel::em && (strategy_type == strategy::smsummer16 || strategy_type == strategy::cpsummer16 || strategy_type == strategy::legacy16 ||  strategy_type == strategy::cpdecays16 || strategy_type == strategy::cpsummer17 || strategy_type == strategy::cpdecays17 || strategy_type == strategy::cpdecays18)) jlepton_fake = true;


  std::cout << "-------------------------------------" << std::endl;
  std::cout << "HiggsToTauTau Analysis" << std::endl;
  std::cout << "-------------------------------------" << std::endl;      std::string param_fmt = "%-25s %-40s\n";
  std::cout << boost::format(param_fmt) % "output" % (output_folder+output_name);
  std::cout << boost::format(param_fmt) % "strategy" % Strategy2String(strategy_type);
  std::cout << boost::format(param_fmt) % "era" % Era2String(era_type);
  std::cout << boost::format(param_fmt) % "mc" % MC2String(mc_type);
  std::cout << boost::format(param_fmt) % "channel" % channel_str;
  std::cout << boost::format(param_fmt) % "is_data" % is_data;
  std::cout << boost::format(param_fmt) % "is_embedded" % is_embedded;
  std::cout << boost::format(param_fmt) % "tau_scale_mode" % tau_scale_mode;
  std::cout << boost::format(param_fmt) % "new_svfit_mode" % new_svfit_mode;
  if (new_svfit_mode > 0) {
    std::cout << boost::format(param_fmt) % "svfit_folder" % svfit_folder;
    std::cout << boost::format(param_fmt) % "svfit_override" % svfit_override;
  }
  std::cout << boost::format(param_fmt) % "make_sync_ntuple" % js["make_sync_ntuple"].asBool();
  std::cout << boost::format(param_fmt) % "save_output_jsons" % js["save_output_jsons"].asBool();
  std::cout << boost::format(param_fmt) % "use_deep_csv" % use_deep_csv;
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
  std::cout << boost::format(param_fmt) % "lead_tau_pt" % lead_tau_pt;
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

  file.close();

  // Defining good-lumi jsons
  std::string data_json = "";
  if (era_type == era::data_2016)
      data_json= "input/json/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt";
  if (era_type == era::data_2017) 
      data_json= "input/json/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt";
  if (era_type == era::data_2018)
      data_json= "input/json/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt";

 if(js["get_effective"].asBool() && js["make_sync_ntuple"].asBool()){
   std::cerr<< "Error: cannot run effective number of event module in make_syncntuple mode"<<std::endl;
   throw;
 }
 if(js["get_effective"].asBool()){
  BuildModule(EffectiveEvents("EffectiveEvents")
    .set_fs(fs.get())
    .set_do_qcd_scale_wts(do_qcd_scale_wts_));
  }else if(is_data && js["lumi_mask_only"].asBool()){
   //LumiMask module  just for the mode in which we only write out the 
   //output jsons
   LumiMask lumiMask = LumiMask("LumiMask")
     .set_produce_output_jsons(lumimask_output_name.c_str())
     .set_input_file(data_json);
 
    BuildModule(lumiMask);
  
  } else{

  HTTPrint httPrint("HTTPrint");
  if(era_type==era::data_2015 || era_type==era::data_2016  
          || era_type == era::data_2017 || era_type == era::data_2018){
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

if(!is_data && js["do_gen_analysis"].asBool()){

  std::string mass_str = output_name;
  mass_str.erase(0, mass_str.find("_M-")+3);
  mass_str.erase(mass_str.find("_"),mass_str.length()-mass_str.find("_"));
  
  unsigned mela_mode = js["mela_mode"].asUInt();
  if(mela_mode!=0){
    std::string mela_folder;
    if(js["mela_folder"].asString()!="") {
      mela_folder = js["mela_folder"].asString();
    } else {
      std::cout<<"ERROR: mela_folder not set"<<std::endl; exit(1);
    }
    
    if(js["baseline"]["jes_mode"].asUInt() > 0 && js["baseline"]["split_by_source"].asBool()) mela_folder=mela_folder+"/";
    else mela_folder=mela_folder+"/"+addit_output_folder+"/";
  
    MELATestGen melaTestGen = MELATestGen("MELATestGen") 
      .set_channel(channel)
      .set_run_mode(mela_mode)
      .set_outname(output_name)
      .set_fullpath(mela_folder);
    BuildModule(melaTestGen);
  }

  HTTStitching httStitching = HTTStitching("HTTStitching")
      .set_era(era_type)
      .set_fs(fs.get())
      .set_do_ggH_soup(do_ggH_stitch);
      httStitching.SetggHInputYieldsAndFrac(n_inc, n_2, frac);

  BuildModule(httStitching);
  
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
    .set_mssm_mass(mass_str)
  );
  return;  
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

if((strategy_type == strategy::fall15 || strategy_type ==strategy::mssmsummer16 || strategy_type == strategy::smsummer16 || strategy_type == strategy::cpsummer16 || strategy_type == strategy::legacy16 || strategy_type == strategy::cpdecays16 || strategy_type == strategy::cpsummer17 || strategy_type == strategy::cpdecays17 || strategy_type == strategy::cpdecays18) && output_name.find("WGToLNuG")!=output_name.npos){
  SimpleCounter<GenParticle> wgammaStarFilter = SimpleCounter<GenParticle>("WgammaStarSelector")
    .set_input_label("genParticles")
    .set_predicate(
      (bind(&GenParticle::status,_1) == 44) &&
      (bind(::abs,(bind(&GenParticle::pdgid, _1))) == 22))
    .set_min(0).set_max(0);
 
  BuildModule(wgammaStarFilter);
}

  if (channel == channel::et) BuildETPairs();
  if (channel == channel::mt) BuildMTPairs();
  if (channel == channel::em) BuildEMPairs();
  if (channel == channel::tt) BuildTTPairs();
  if (channel == channel::zee) BuildZEEPairs();
  if (channel == channel::zmm) BuildZMMPairs();
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

if((era_type == era::data_2016 || era_type == era::data_2017) &a !is_embedded && !is_data) {
  TH2F prefire_hist; 
  if(era_type == era::data_2016) prefire_hisa = GetFromTFile<TH2F>("input/prefire/L1prefiring_jetpt_2016BtoH.root","/","L1prefiring_jetpt_2016BtoH");
  else prefire_hist = GetFromTFile<TH2F>("input/prefire/L1prefiring_jetpt_2017BtoF.root","/","L1prefiring_jetpt_2017BtoF");
  BuildModule(HTTPreFireWeight<PFJet>("HTTPreFireWeight")
    .set_prefire_hist(new TH2F(prefire_hist)));
}

// JER
if (!is_data && !is_embedded) {
   BuildModule(JetEnergyResolution<PFJet>("JetEnergyResolution")
     .set_input_label(jets_label)
     .set_jer_shift_mode(jer_mode)
     .set_EENoiseFix(era_type == era::data_2017)
   );
}

BuildModule(CopyCollection<PFJet>("CopyFilteredJets",jets_label,jets_label+"UnFiltered"));

SimpleFilter<PFJet> jetIDFilter = SimpleFilter<PFJet>("JetIDFilter")
.set_input_label(jets_label);
if (era_type == era::data_2016) {
  jetIDFilter.set_predicate(bind(PFJetID2016, _1));
} else if (era_type == era::data_2017) {
  jetIDFilter.set_predicate(bind(PFJetID2017, _1));
} else if (era_type == era::data_2018) {
  jetIDFilter.set_predicate(bind(PFJetID2018, _1));
}
BuildModule(jetIDFilter);


if (era_type == era::data_2017) {
  BuildModule(SimpleFilter<PFJet>("JetEENoiseVetoFilter")
    .set_input_label(jets_label)
    .set_predicate([=](PFJet const* jet) {
      return  (jet->pt() * (jet->uncorrected_energy()/jet->energy())) > 50 ||
        fabs(jet->eta()) > 3.139 ||
        fabs(jet->eta()) < 2.65 ;
    })
  );
}

if (jes_mode > 0 && !is_data ){
  if (era_type == era::data_2016) {
    jes_input_file = "input/jec/Regrouped_Summer16_07Aug2017_V11_MC_UncertaintySources_AK4PFchs.txt"; 
    jes_input_set  = "Total";
  }
  if (era_type == era::data_2017) {
    jes_input_file = "input/jec/Regrouped_Fall17_17Nov2017_V32_MC_UncertaintySources_AK4PFchs.txt";
    jes_input_set  = "Total";
  }
  if (era_type == era::data_2018) {
    jes_input_file = "input/jec/Regrouped_Autumn18_V19_MC_UncertaintySources_AK4PFchs.txt";
    jes_input_set  = "Total";
  }
  
  if(alt_jes_input_set!="") jes_input_set = alt_jes_input_set;
 
  BuildModule(JetEnergyUncertainty<PFJet>("JetEnergyUncertainty")
    .set_input_label(jets_label)
    .set_jes_shift_mode(jes_mode)
    .set_uncert_file(jes_input_file)
    .set_uncert_set(jes_input_set)
    .set_EENoiseFix(era_type == era::data_2017)
   );
 
}

// Apply loose PUJID universally
BuildModule(SimpleFilter<PFJet>("JetPUIDFilter")
  .set_input_label(jets_label)
  .set_predicate([=](PFJet const* jet) {
    return  PileupJetID(jet, pu_id_training, false, true);
  })
);

if(js["do_btag_eff"].asBool()){
   BuildModule(BTagCheck("BTagCheck")
    .set_fs(fs.get())
    .set_channel(channel)
    .set_era(era_type)
    .set_strategy(strategy_type)
    .set_do_legacy(false)
    .set_use_deep_csv(use_deep_csv)
    .set_wp_to_check(wp_to_check)
    .set_jet_label(jets_label));
}

  bool usePFMET = false;
  usePFMET = js["usePFMET"].asBool();
 
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
    .set_mc(mc_type)
    .set_gen_taus_label(is_embedded ? "genParticlesEmbedded" : "genParticlesTaus")
    .set_scale_met_for_tau((tau_scale_mode > 0 || (moriond_tau_scale && (is_embedded || !is_data) )   ))
    .set_tau_scale(tau_shift)
    .set_use_most_isolated((strategy_type != strategy::paper2013) && (!(channel == channel::zee || channel == channel::zmm || channel == channel::tpzmm || channel == channel::tpzee)))
    .set_use_os_preference((strategy_type == strategy::paper2013) || (channel == channel::zee || channel == channel::zmm || channel == channel::tpzmm || channel == channel::tpzee))
    .set_allowed_tau_modes(allowed_tau_modes)
    .set_metuncl_mode(metuncl_mode)
    .set_metcl_mode(metcl_mode)
    .set_shift_jes(!do_recoil)
    .set_usePFMET(usePFMET);
      
  httPairSelector.set_gen_taus_label("genParticles");
  
  BuildModule(httPairSelector);
   
  if(channel != channel::tpzmm &&channel !=channel::tpzee && (is_data || js["trg_in_mc"].asBool())){
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

// Lepton Vetoes
if (js["baseline"]["di_elec_veto"].asBool()) BuildDiElecVeto();
if (js["baseline"]["di_muon_veto"].asBool()) BuildDiMuonVeto();
if (js["baseline"]["extra_elec_veto"].asBool()) BuildExtraElecVeto();
if (js["baseline"]["extra_muon_veto"].asBool()) BuildExtraMuonVeto();


// Pileup Weighting
TH1D d_pu = GetFromTFile<TH1D>(js["data_pu_file"].asString(), "/", "pileup");
TH1D m_pu = GetFromTFile<TH1D>(js["mc_pu_file"].asString(), "/", "pileup");
if (js["do_pu_wt"].asBool()&&!is_data&&!is_embedded) {
  BuildModule( PileupWeight("PileupWeight")
      .set_data(new TH1D(d_pu)).set_mc(new TH1D(m_pu)));
}

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
      std::vector<std::string> met_filters = {
        "Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter",
        "Flag_goodVertices","badMuonFilter", "Flag_globalSuperTightHalo2016Filter"
      };
      if (era_type == era::data_2016 || era_type == era::data_2017) {
        met_filters.pop_back();
        met_filters.push_back("Flag_globalTightHalo2016Filter");
      }
      if (era_type == era::data_2017 || era_type == era::data_2018) 
        met_filters.push_back("ecalBadCalibReducedMINIAODFilter");
      bool pass_filters = true;
      for(unsigned i=0;i<met_filters.size();++i){
       pass_filters = pass_filters&& eventInfo->filter_result(met_filters.at(i));
      }
      if(js["make_sync_ntuple"].asBool() and false) {
        event->Add("flagMETFilter", pass_filters);  
        return false;    
      }
      return !pass_filters;
    }));
}

bool do_ngenjets = (output_name.find("GluGluHToTauTauUncorrelatedDecay") != output_name.npos || output_name.find("JJH")!=output_name.npos);
BuildModule(HTTPairGenInfo("HTTPairGenInfo")
  .set_fs(fs.get())
  .set_write_plots(false)
  .set_ditau_label("ditau")
  .set_channel(channel)
  .set_ngenjets(do_ngenjets));

BuildModule(OverlapFilter<PFJet, CompositeCandidate>("JetLeptonOverlapFilter")
  .set_input_label(jets_label)
  .set_reference_label("ditau")
  .set_min_dr(0.5));


 bool usePFMET = false;

 // in this case the older recoil correction tool already uses the proper PF MET corrections so use this if option is specified


 if(((strategy_type == strategy::fall15|| strategy_type==strategy::mssmspring16 ||strategy_type == strategy::smspring16 || strategy_type == strategy::mssmsummer16) || usePFMET) && channel!=channel::wmnu && do_recoil){ 
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

 if((strategy_type == strategy::smsummer16 || strategy_type == strategy::cpsummer16 || strategy_type == strategy::legacy16 || strategy_type == strategy::cpdecays16 || strategy_type == strategy::cpsummer17 || strategy_type == strategy::cpdecays17 || strategy_type == strategy::cpdecays18) && channel!=channel::wmnu && !usePFMET){
    unsigned njets_mode = js["njets_mode"].asUInt();

    BuildModule(HTTLegacyRun2RecoilCorrector("HTTLegacyRun2RecoilCorrector")
     .set_sample(output_name)
     .set_channel(channel)
     .set_mc(mc_type)
     .set_met_label(met_label)
     .set_jets_label(jets_label)
     .set_strategy(strategy_type)
     .set_use_quantile_map(true) // use quantile mapping now
     .set_use_puppimet(true) 
     .set_met_scale_mode(metscale_mode)
     .set_met_res_mode(metres_mode)
     .set_store_boson_pt(js["make_sync_ntuple"].asBool())
     .set_njets_mode(njets_mode)
     .set_do_recoil(do_recoil)
     );
  }

  int mode = new_svfit_mode==1 && (js["baseline"]["jes_mode"].asUInt() > 0 || js["baseline"]["jer_mode"].asUInt() > 0) && do_recoil ? 0 : new_svfit_mode;

  SVFitTest svFitTest  = SVFitTest("SVFitTest")
    .set_channel(channel)
    .set_strategy(strategy_type)
    .set_mc(mc_type)
    .set_outname(svfit_override == "" ? output_name : svfit_override)
    .set_run_mode(mode)
    .set_fail_mode(0)
    .set_require_inputs_match(false)
    .set_split(40000)
    .set_dilepton_label("ditau")
    .set_met_label(met_label)
    .set_fullpath(svfit_folder)
    .set_legacy_svfit(true)
    .set_do_preselection(false)
    .set_MC(true)
    .set_do_light(true)
    .set_do_vloose_preselection(js["baseline"]["do_ff_weights"].asBool());
 if(era_type == era::data_2015 || era_type == era::data_2016 
         || era_type == era::data_2017 || era_type == era::data_2018){
   svFitTest.set_legacy_svfit(false);
   svFitTest.set_do_preselection(!js["make_sync_ntuple"].asBool() && !js["baseline"]["do_faketaus"].asBool());
   svFitTest.set_read_svfit_mt(true);
   svFitTest.set_tau_optimisation(js["tau_id_study"].asBool());
   svFitTest.set_read_all(js["read_all_svfit_files"].asBool());
   svFitTest.set_from_grid(js["svfit_from_grid"].asBool());
 }

BuildModule(svFitTest);

if(js["do_preselection"].asBool() && !js["baseline"]["do_faketaus"].asBool() && channel != channel::tpzee && channel != channel::tpzmm){
  BuildModule(GenericModule("PreselectionFilter")
    .set_function([](ic::TreeEvent *event){
      //Pass preselection in case we're accidentally not running any preselection in SVFitTest but somehow have
      //requested preselection in the config anyway...
      bool pass_presel = event->Exists("pass_preselection") ? event->Get<bool>("pass_preselection") : 1;
      if (!pass_presel) return 1;
      else return 0;
     }));
 }


if(!is_data){
 TH2F bbtag_eff;
 TH2F cbtag_eff;
 TH2F othbtag_eff;

  if ((strategy_type == strategy::mssmsummer16 || strategy_type == strategy::smsummer16 || strategy_type == strategy::cpsummer16 || strategy_type == strategy::legacy16 || strategy_type == strategy::cpdecays16) && use_deep_csv){
    bbtag_eff = GetFromTFile<TH2F>("input/btag_sf/deepCSV_efficiencies_leg2016.root","/","btag_eff_b");
    cbtag_eff = GetFromTFile<TH2F>("input/btag_sf/deepCSV_efficiencies_leg2016.root","/","btag_eff_c");
    othbtag_eff = GetFromTFile<TH2F>("input/btag_sf/deepCSV_efficiencies_leg2016.root","/","btag_eff_oth");
  } else if ((strategy_type == strategy::cpsummer17 || strategy_type == strategy::cpdecays17) && use_deep_csv) {
    bbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_Winter2017_v2.root","/","btag_eff_b");
    cbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_Winter2017_v2.root","/","btag_eff_c");
    othbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_Winter2017_v2.root","/","btag_eff_oth");
  } else if (strategy_type == strategy::cpdecays18 && use_deep_csv) {
    bbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_2018_v1.root","/","btag_eff_b");
    cbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_2018_v1.root","/","btag_eff_c");
    othbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_2018_v1.root","/","btag_eff_oth");
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
 .set_use_deep_csv(use_deep_csv)
 .set_btag_mode(btag_mode)
 .set_bfake_mode(bfake_mode));
}
// NOT READY YET
if((strategy_type == strategy::legacy16 || strategy_type == strategy::cpdecays16 || strategy_type == strategy::cpdecays17 || strategy_type == strategy::cpdecays18) && !is_data){
  TH2F bbtag_eff;
  TH2F cbtag_eff;
  TH2F othbtag_eff;
  TH2F bbtag_eff_alt;
  TH2F cbtag_eff_alt;
  TH2F othbtag_eff_alt;

  if (strategy_type == strategy::legacy16){
    bbtag_eff = GetFromTFile<TH2F>("input/btag_sf/deepCSV_efficiencies_leg2016.root","/","btag_eff_b");
    cbtag_eff = GetFromTFile<TH2F>("input/btag_sf/deepCSV_efficiencies_leg2016.root","/","btag_eff_c");
    othbtag_eff = GetFromTFile<TH2F>("input/btag_sf/deepCSV_efficiencies_leg2016.root","/","btag_eff_oth");

    bbtag_eff_alt = GetFromTFile<TH2F>("input/btag_sf/deepCSV_efficiencies_leg2016_loose.root","/","btag_eff_b");
    cbtag_eff_alt = GetFromTFile<TH2F>("input/btag_sf/deepCSV_efficiencies_leg2016_loose.root","/","btag_eff_c");
    othbtag_eff_alt = GetFromTFile<TH2F>("input/btag_sf/deepCSV_efficiencies_leg2016_loose.root","/","btag_eff_oth");
  } else if (strategy_type == strategy::cpdecays17) {
    bbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_Winter2017_v2.root","/","btag_eff_b");
    cbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_Winter2017_v2.root","/","btag_eff_c");
    othbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_Winter2017_v2.root","/","btag_eff_oth");

    bbtag_eff_alt = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_2017_loose.root","/","btag_eff_b");
    cbtag_eff_alt = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_2017_loose.root","/","btag_eff_c");
    othbtag_eff_alt = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_2017_loose.root","/","btag_eff_oth");
  } else if (strategy_type == strategy::cpdecays18) {
    bbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_2018_v1.root","/","btag_eff_b");
    cbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_2018_v1.root","/","btag_eff_c");
    othbtag_eff = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_2018_v1.root","/","btag_eff_oth");

    bbtag_eff_alt = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_2018_loose.root","/","btag_eff_b");
    cbtag_eff_alt = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_2018_loose.root","/","btag_eff_c");
    othbtag_eff_alt = GetFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_deepCSV_2018_loose.root","/","btag_eff_oth");
  }

BuildModule(BTagWeightLegacyRun2("BTagWeightLegacyRun2")
 .set_channel(channel)
 .set_era(era_type)
 .set_strategy(strategy_type)
 .set_jet_label(jets_label)
 .set_bbtag_eff(new TH2F(bbtag_eff))
 .set_cbtag_eff(new TH2F(cbtag_eff))
 .set_othbtag_eff(new TH2F(othbtag_eff))
 .set_bbtag_eff_alt(new TH2F(bbtag_eff_alt))
 .set_cbtag_eff_alt(new TH2F(cbtag_eff_alt))
 .set_othbtag_eff_alt(new TH2F(othbtag_eff_alt)));
}


 if(strategy_type == strategy::paper2013){
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
    .set_mc(mc_type)
    .set_do_tau_id_weights(false)
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
   TH2D z_pt_weights_sm; GetFromTFile<TH2F>("input/zpt_weights/zpt_weights_2016_BtoH.root","/","zptmass_histo").Copy(z_pt_weights_sm);

   HTTWeights httWeights = HTTWeights("HTTWeights")   
    .set_channel(channel)
    .set_era(era_type)
    .set_mc(mc_type)
    .set_do_tau_id_weights(real_tau_sample)
    .set_do_tau_id_sf(real_tau_sample)
    .set_do_jlepton_fake(jlepton_fake)
    .set_do_em_qcd_weights(true)
    .set_ditau_label("ditau")
    .set_jets_label(jets_label)
    .set_do_single_lepton_trg(js["do_singlelepton"].asBool())
    .set_do_cross_trg(js["do_leptonplustau"].asBool())
    .set_tt_trg_iso_mode(js["tt_trg_iso_mode"].asUInt())
    .set_em_m17_trig_mc(new TH2D(em_m17_trig_mc)).set_em_m17_trig_data(new TH2D(em_m17_trig_data))
    .set_em_m8_trig_mc(new TH2D(em_m8_trig_mc)).set_em_m8_trig_data(new TH2D(em_m8_trig_data))
    .set_em_e17_trig_mc(new TH2D(em_e17_trig_mc)).set_em_e17_trig_data(new TH2D(em_e17_trig_data))
    .set_em_e12_trig_mc(new TH2D(em_e12_trig_mc)).set_em_e12_trig_data(new TH2D(em_e12_trig_data))
    .set_em_qcd_cr1_lt2(new TH2D(em_qcd_cr1_lt2))
    .set_em_qcd_cr1_2to4(new TH2D(em_qcd_cr1_2to4))
    .set_em_qcd_cr1_gt4(new TH2D(em_qcd_cr1_gt4));
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
      httWeights.set_z_pt_mass_hist(new TH2D(z_pt_weights));
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
  if(output_name.find("SUSYGluGluToHToTauTau_M") != output_name.npos){
    httWeights.set_do_mssm_higgspt(true); 
    httWeights.set_mssm_higgspt_file("input/mssm_higgspt/higgs_pt_v2_mssm_mode.root");
    std::string mass_str = output_name;
    mass_str.erase(0, mass_str.find("_M-")+3);
    mass_str.erase(mass_str.find("_"),mass_str.length()-output_name.find("_"));
    httWeights.set_mssm_mass(mass_str);
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
  
if((strategy_type == strategy::smsummer16 || strategy_type == strategy::cpsummer16 || strategy_type == strategy::legacy16 || strategy_type == strategy::cpdecays16) &&channel!=channel::wmnu){
   TH2D z_pt_weights = GetFromTFile<TH2D>("input/zpt_weights/zpt_weights_summer2016_v2.root","/","zptmass_histo");
   TH2D z_pt_weights_sm; GetFromTFile<TH2F>("input/zpt_weights/zpt_weights_2016_BtoH.root","/","zptmass_histo").Copy(z_pt_weights_sm);

   HTTWeights httWeights = HTTWeights("HTTWeights")   
    .set_channel(channel)
    .set_era(era_type)
    .set_mc(mc_type)
    .set_do_tau_id_weights(real_tau_sample)
    .set_do_tau_id_sf(real_tau_sample && !js["do_mt_tagandprobe"].asBool())
    .set_do_jlepton_fake(jlepton_fake)
    .set_do_em_qcd_weights(true)
    .set_ditau_label("ditau")
    .set_jets_label(jets_label)
    .set_do_single_lepton_trg(js["do_singlelepton"].asBool())
    .set_do_cross_trg(js["do_leptonplustau"].asBool())
    .set_tt_trg_iso_mode(js["tt_trg_iso_mode"].asUInt())
    .set_do_quarkmass_higgspt(do_ggH_stitch||output_name.find("JJH")!=output_name.npos)
    .set_do_ps_weights(do_ggH_stitch||output_name.find("JJH")!=output_name.npos)
    .set_do_nnlops_weights(do_ggH_stitch||output_name.find("JJH")!=output_name.npos||output_name.find("GluGluHToTauTauUncorrelatedDecay") != output_name.npos);
    //.set_do_nnlops_weights(false);
    httWeights.set_strategy(strategy_type);
    httWeights.set_scalefactor_file("input/scale_factors/htt_scalefactors_v16_5_1.root");
    httWeights.set_scalefactor_file_ggh("input/ggh_weights/htt_scalefactors_2016_MGggh.root");
    if (strategy_type == strategy::legacy16) httWeights.set_scalefactor_file("input/scale_factors/htt_scalefactors_legacy_2016.root");
    if(is_embedded) httWeights.set_embedding_scalefactor_file("input/scale_factors/htt_scalefactors_v16_9_embedded.root");
    httWeights.set_is_embedded(is_embedded);
    httWeights.set_z_pt_mass_hist(new TH2D(z_pt_weights_sm));
    bool z_sample = (output_name.find("DY") != output_name.npos && (output_name.find("JetsToLL-LO") != output_name.npos || output_name.find("JetsToLL_M-10-50-LO") != output_name.npos)) || output_name.find("EWKZ2Jets") != output_name.npos;
    httWeights.set_do_z_weights(strategy_type == strategy::smsummer16 && z_sample && channel !=channel::zmm &&channel!=channel::tpzee&&channel!=channel::tpzmm&&channel!=channel::tpmt&&channel!= channel::tpem);
  if (!is_data ) {
    httWeights.set_do_trg_weights(true).set_trg_applied_in_mc(js["trg_in_mc"].asBool()).set_do_idiso_weights(true);
    if(channel == channel::et || channel == channel::mt || channel==channel::tt) httWeights.set_do_etau_fakerate(true);
    if(channel == channel::mt || channel == channel::et ||channel == channel::tt) httWeights.set_do_mtau_fakerate(true);
    if((channel == channel::et || channel==channel::em || channel==channel::mt || channel==channel::zmm || channel==channel::zee)) httWeights.set_do_tracking_eff(true);
  }

  if ((output_name.find("DY") != output_name.npos && output_name.find("JetsToLL-LO") != output_name.npos && !(output_name.find("JetsToLL-LO-10-50") != output_name.npos))){
    httWeights.set_do_zpt_weight(true&&channel!=channel::tpzee&&channel!=channel::tpzmm&&channel!=channel::tpmt&&channel != channel::tpem);
  }

  if (output_name.find("TT") != output_name.npos) httWeights.set_do_topquark_weights(true);
  if (output_name.find("WJetsToLNu-LO") != output_name.npos || output_name.find("W1JetsToLNu-LO") != output_name.npos || output_name.find("W2JetsToLNu-LO") != output_name.npos ||
       output_name.find("W3JetsToLNu-LO") != output_name.npos || output_name.find("W4JetsToLNu-LO") != output_name.npos){
    httWeights.set_do_tau_fake_weights(true);
  }
  if(output_name.find("SUSYGluGluToHToTauTau_M") != output_name.npos){
    httWeights.set_do_mssm_higgspt(true); 
    httWeights.set_mssm_higgspt_file("input/mssm_higgspt/higgs_pt_v2_mssm_mode.root");
    std::string mass_str = output_name;
    mass_str.erase(0, mass_str.find("_M-")+3);
    mass_str.erase(mass_str.find("_"),mass_str.length()-output_name.find("_"));
    httWeights.set_mssm_mass(mass_str);
  }

  

    if(channel!=channel::tpzee&&channel!=channel::tpzmm) BuildModule(httWeights);
    if(  (strategy_type == strategy::cpsummer16 || strategy_type == strategy::legacy16) && channel!=channel::tpzee&&channel!=channel::tpzmm&&channel!=channel::tpmt&&channel != channel::tpem){

      HTTStitching httStitching = HTTStitching("HTTStitching")  
          .set_era(era_type)
          .set_fs(fs.get())
          .set_do_ggH_soup(do_ggH_stitch);
          httStitching.SetggHInputYieldsAndFrac(n_inc, n_2, frac);
           if (output_name.find("WJetsToLNu-LO") != output_name.npos || output_name.find("W1JetsToLNu-LO") != output_name.npos || output_name.find("W2JetsToLNu-LO") != output_name.npos ||
             output_name.find("W3JetsToLNu-LO") != output_name.npos || output_name.find("W4JetsToLNu-LO") != output_name.npos){
          httStitching.set_do_w_soup(true);
          httStitching.SetWInputCrossSections(50380,9644.5,3144.5,954.8,485.6); 
          httStitching.SetWInputYields(58592840+114659635, 45283121, 30064264+30374504, 39501912+19798117, 18751462+2073275+9116657);
         }
         if ((output_name.find("DY") != output_name.npos && output_name.find("JetsToLL-LO") != output_name.npos && !(output_name.find("JetsToLL-LO-10-50") != output_name.npos))){
           httStitching.set_do_dy_soup(true);
           httStitching.SetDYInputCrossSections(4954, 1012.5, 332.8, 101.8,54.8); //Target fractions are xs_n-jet/xs_inclusive
           httStitching.SetDYInputYields(49748967+96531428, 63730337, 19879279, 5857441, 4197868); 
         }
      BuildModule(httStitching);   
    }
  }

  if((strategy_type == strategy::cpsummer17 || strategy_type == strategy::cpdecays17) && channel!=channel::wmnu){
    TH2D z_pt_weights = GetFromTFile<TH2D>("input/zpt_weights/Zpt2017new.root","/","zptmass_histo");
  
    HTTWeights httWeights = HTTWeights("HTTWeights")   
     .set_channel(channel)
     .set_era(era_type)
     .set_mc(mc_type)
     .set_do_tau_id_weights(real_tau_sample)
     .set_do_tau_id_sf(real_tau_sample && !js["do_mt_tagandprobe"].asBool())
     .set_do_jlepton_fake(jlepton_fake)
     .set_do_em_qcd_weights(true)
     .set_ditau_label("ditau")
     .set_jets_label(jets_label)
     .set_do_single_lepton_trg(js["do_singlelepton"].asBool())
     .set_do_cross_trg(js["do_leptonplustau"].asBool())
     .set_tt_trg_iso_mode(js["tt_trg_iso_mode"].asUInt())
     .set_do_quarkmass_higgspt(do_ggH_stitch||output_name.find("JJH")!=output_name.npos)
     .set_do_ps_weights(do_ggH_stitch||output_name.find("JJH")!=output_name.npos)
     .set_do_nnlops_weights(do_ggH_stitch||output_name.find("JJH")!=output_name.npos || output_name.find("GluGluHToTauTauUncorrelatedDecay") != output_name.npos);
     //.set_do_nnlops_weights(false);
     httWeights.set_strategy(strategy_type);
     httWeights.set_scalefactor_file("input/scale_factors/htt_scalefactors_legacy_2017.root");
     httWeights.set_scalefactor_file_ggh("input/ggh_weights/htt_scalefactors_2017_MGggh.root");
     httWeights.set_is_embedded(is_embedded);
     httWeights.set_z_pt_mass_hist(new TH2D(z_pt_weights));
   if (!is_data ) {
     httWeights.set_do_trg_weights(true).set_trg_applied_in_mc(js["trg_in_mc"].asBool()).set_do_idiso_weights(true);
     if(channel == channel::et || channel == channel::mt || channel==channel::tt) httWeights.set_do_etau_fakerate(true);
     if(channel == channel::mt || channel == channel::et ||channel == channel::tt) httWeights.set_do_mtau_fakerate(true);
     if((channel == channel::et || channel==channel::em || channel==channel::mt || channel==channel::zmm || channel==channel::zee)) httWeights.set_do_tracking_eff(true);
   }
  
   if ((output_name.find("DY") != output_name.npos && output_name.find("JetsToLL-LO") != output_name.npos && !(output_name.find("JetsToLL-LO-10-50") != output_name.npos))){
     httWeights.set_do_zpt_weight(channel!=channel::tpzee&&channel!=channel::tpzmm&&channel!=channel::tpmt&&channel != channel::tpem);
   }
  
   if (output_name.find("TT") != output_name.npos) httWeights.set_do_topquark_weights(true);
   if (output_name.find("WJetsToLNu-LO") != output_name.npos || output_name.find("W1JetsToLNu-LO") != output_name.npos || output_name.find("W2JetsToLNu-LO") != output_name.npos ||
        output_name.find("W3JetsToLNu-LO") != output_name.npos || output_name.find("W4JetsToLNu-LO") != output_name.npos){
     httWeights.set_do_tau_fake_weights(true);
   }
   if(output_name.find("SUSYGluGluToHToTauTau_M") != output_name.npos){
     httWeights.set_do_mssm_higgspt(true); 
     httWeights.set_mssm_higgspt_file("input/mssm_higgspt/higgs_pt_v2_mssm_mode.root");
     std::string mass_str = output_name;
     mass_str.erase(0, mass_str.find("_M-")+3);
     mass_str.erase(mass_str.find("_"),mass_str.length()-output_name.find("_"));
     httWeights.set_mssm_mass(mass_str);
   }
  
   
  
     BuildModule(httWeights);
     if(channel!=channel::tpzee&&channel!=channel::tpzmm&&channel!=channel::tpmt&&channel != channel::tpem){
       HTTStitching httStitching = HTTStitching("HTTStitching")  
           .set_era(era_type)
           .set_fs(fs.get())
           .set_do_ggH_soup(do_ggH_stitch);
            httStitching.SetggHInputYieldsAndFrac(n_inc, n_2, frac);
            if (output_name.find("WJetsToLNu-LO") != output_name.npos || output_name.find("W1JetsToLNu-LO") != output_name.npos || output_name.find("W2JetsToLNu-LO") != output_name.npos ||
              output_name.find("W3JetsToLNu-LO") != output_name.npos || output_name.find("W4JetsToLNu-LO") != output_name.npos){
           httStitching.set_do_w_soup(true);
           // W numbers need updating
           httStitching.SetWInputCrossSections(1.0,0.1522,0.0515,0.0184,0.0103);
           httStitching.SetWInputYields(77558539,54013116,6558503,19669693,10325586); 
          }
          if ((output_name.find("DY") != output_name.npos && output_name.find("JetsToLL-LO") != output_name.npos 
                      && !(output_name.find("JetsToLL-LO-5-50") != output_name.npos) && !(output_name.find("JetsToLL-LO-10-50") != output_name.npos))){
            httStitching.set_do_dy_soup(true);
            // DY XS's are relative to the inclusive XS
            httStitching.SetDYInputCrossSections(1.0, 0.1641, 0.0571, 0.0208, 0.0118); //Target fractions are xs_n-jet/xs_inclusive
            httStitching.SetDYInputYields(97502567,75523396,10016760,6887893,4317756);
          }
       
       BuildModule(httStitching);   
     }
   }
// adding 2018 stitching
//
  if((strategy_type == strategy::cpdecays18) && channel!=channel::wmnu){
    TH2D z_pt_weights = GetFromTFile<TH2D>("input/zpt_weights/Zpt2018new.root","/","zptmass_histo");
  
    HTTWeights httWeights = HTTWeights("HTTWeights")   
     .set_channel(channel)
     .set_era(era_type)
     .set_mc(mc_type)
     .set_do_tau_id_weights(real_tau_sample)
     .set_do_tau_id_sf(real_tau_sample && !js["do_mt_tagandprobe"].asBool())
     .set_do_jlepton_fake(jlepton_fake)
     .set_do_em_qcd_weights(true)
     .set_ditau_label("ditau")
     .set_jets_label(jets_label)
     .set_do_single_lepton_trg(js["do_singlelepton"].asBool())
     .set_do_cross_trg(js["do_leptonplustau"].asBool())
     .set_tt_trg_iso_mode(js["tt_trg_iso_mode"].asUInt())
     .set_do_quarkmass_higgspt(do_ggH_stitch||output_name.find("JJH")!=output_name.npos)
     .set_do_ps_weights(do_ggH_stitch || output_name.find("JJH")!=output_name.npos)
     .set_do_nnlops_weights(do_ggH_stitch||output_name.find("JJH")!=output_name.npos || output_name.find("GluGluHToTauTauUncorrelatedDecay") != output_name.npos);
     //.set_do_nnlops_weights(false);
     httWeights.set_strategy(strategy_type);
     httWeights.set_scalefactor_file("input/scale_factors/htt_scalefactors_legacy_2018.root");
     httWeights.set_scalefactor_file_ggh("input/ggh_weights/htt_scalefactors_2017_MGggh.root");
     httWeights.set_is_embedded(is_embedded);
     httWeights.set_z_pt_mass_hist(new TH2D(z_pt_weights));
   if (!is_data ) {
     httWeights.set_do_trg_weights(true).set_trg_applied_in_mc(js["trg_in_mc"].asBool()).set_do_idiso_weights(true);
     if(channel == channel::et || channel == channel::mt || channel==channel::tt) httWeights.set_do_etau_fakerate(true);
     if(channel == channel::mt || channel == channel::et ||channel == channel::tt) httWeights.set_do_mtau_fakerate(true);
     if((channel == channel::et || channel==channel::em || channel==channel::mt || channel==channel::zmm || channel==channel::zee)) httWeights.set_do_tracking_eff(true);
   }
  
   if ((output_name.find("DY") != output_name.npos && output_name.find("JetsToLL-LO") != output_name.npos && !(output_name.find("JetsToLL-LO-10-50") != output_name.npos))){
     httWeights.set_do_zpt_weight(channel!=channel::tpzee&&channel!=channel::tpzmm&&channel!=channel::tpmt&&channel != channel::tpem);
   }
  
   if (output_name.find("TT") != output_name.npos) httWeights.set_do_topquark_weights(true);
   if (output_name.find("WJetsToLNu-LO") != output_name.npos || output_name.find("W1JetsToLNu-LO") != output_name.npos || output_name.find("W2JetsToLNu-LO") != output_name.npos ||
        output_name.find("W3JetsToLNu-LO") != output_name.npos || output_name.find("W4JetsToLNu-LO") != output_name.npos){
     httWeights.set_do_tau_fake_weights(true);
   }
   
  
     BuildModule(httWeights);
     if(channel!=channel::tpzee&&channel!=channel::tpzmm&&channel!=channel::tpmt&&channel != channel::tpem){
       HTTStitching httStitching = HTTStitching("HTTStitching")  
         .set_era(era_type)
         .set_fs(fs.get())
         .set_do_ggH_soup(do_ggH_stitch);
          httStitching.SetggHInputYieldsAndFrac(n_inc, n_2, frac);
          if (output_name.find("WJetsToLNu-LO") != output_name.npos  || 
              output_name.find("W1JetsToLNu-LO") != output_name.npos || 
              output_name.find("W2JetsToLNu-LO") != output_name.npos ||
              output_name.find("W3JetsToLNu-LO") != output_name.npos || 
              output_name.find("W4JetsToLNu-LO") != output_name.npos){
            httStitching.set_do_w_soup(true);
            // W numbers need updating
            httStitching.SetWInputCrossSections(1.0,0.1522,0.0515,0.0184,0.0103);
            httStitching.SetWInputYields(70548058, 51017448, 23174717, 14351015, 10062198); // correspond to params Jan24
          }
          if ((output_name.find("DY") != output_name.npos && output_name.find("JetsToLL-LO") != output_name.npos 
              && !(output_name.find("JetsToLL-LO-5-50") != output_name.npos) && !(output_name.find("JetsToLL-LO-10-50") != output_name.npos))){
            httStitching.set_do_dy_soup(true);
            // DY XS's are relative to the inclusive XS
            httStitching.SetDYInputCrossSections(1.0, 0.1641, 0.0571, 0.0208, 0.0118); //Target fractions are xs_n-jet/xs_inclusive
            httStitching.SetDYInputYields(100035605, 68536356, 20259039, 5626027, 2812482); // correspond to params Jan24
          }
       
       BuildModule(httStitching);   
     }
   }
///

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

if(js["baseline"]["do_ff_weights"].asBool() && (addit_output_folder=="" || addit_output_folder.find("TSCALE")!=std::string::npos || addit_output_folder.find("ESCALE")!=std::string::npos)){
  BuildModule(HTTFakeFactorWeights("HTTFakeFactorWeights")
      .set_channel(channel)
      .set_ditau_label("ditau")
      .set_met_label(met_label)
      .set_jets_label(jets_label)
      .set_strategy(strategy_type)
      .set_categories(js["baseline"]["ff_categories"].asString())
      .set_do_systematics(js["baseline"]["do_ff_systematics"].asBool())
      .set_ff_file(js["baseline"]["ff_file"].asString())
      .set_fracs_file(js["baseline"]["ff_fracs_file"].asString())
      .set_is_embedded(is_embedded)
      );
}
    
if(channel != channel::wmnu) {
bool do_mssm_higgspt = output_name.find("SUSYGluGluToHToTauTau_M") != output_name.npos && strategy_type == strategy::mssmsummer16;
bool do_sm_scale_wts = (output_name.find("GluGluH2JetsToTauTau_M") != output_name.npos || output_name.find("GluGluToHToTauTau_amcNLO_M-") != output_name.npos || output_name.find("GluGluToHToTauTau_M") != output_name.npos || output_name.find("GluGluToHToTauTau_M125_amcatnloFXFX") != output_name.npos || output_name.find("GluGluToHToTauTauPlusTwoJets_M125_amcatnloFXFX") != output_name.npos || output_name.find("GluGluToPseudoscalarHToTauTau_M125_amcatnloFXFX") != output_name.npos || output_name.find("GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_amcatnloFXFX") != output_name.npos || output_name.find("GluGluToMaxmixHToTauTau_M125_amcatnloFXFX") != output_name.npos || output_name.find("GluGluToMaxmixHToTauTauPlusTwoJets_M125_amcatnloFXFX") != output_name.npos) && output_name.find("SUSY") == output_name.npos && (strategy_type == strategy::smsummer16 || strategy_type == strategy::cpsummer16 || strategy_type == strategy::legacy16 || strategy_type == strategy::cpdecays16 || strategy_type == strategy::cpsummer17 || strategy_type == strategy::cpdecays17 || strategy_type == strategy::cpdecays18);
bool z_sample = (output_name.find("DY") != output_name.npos && (output_name.find("JetsToLL-LO") != output_name.npos || output_name.find("JetsToLL_M-10-50-LO") != output_name.npos)) || output_name.find("EWKZ2Jets") != output_name.npos;

/*if (era_type == era::data_2017 || era_type == era::data_2018) {
  BuildModule(HTTEventClassifier("HTTEventClassifier")
      .set_fs(fs.get())
      .set_channel(channel)
      .set_ditau_label("ditau")
      .set_met_label(met_label)
      .set_jets_label(jets_label)
      .set_era(era_type));
}*/
do_sm_scale_wts = true; // set this to false after!
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
    .set_do_ff_systematics(js["baseline"]["do_ff_systematics"].asBool()&& (addit_output_folder=="" || addit_output_folder.find("TSCALE")!=std::string::npos || addit_output_folder.find("ESCALE")!=std::string::npos || addit_output_folder.find("MUSCALE")!=std::string::npos))
    .set_do_qcd_scale_wts(do_qcd_scale_wts_)
    .set_do_mssm_higgspt(do_mssm_higgspt)
    .set_do_sm_scale_wts(do_sm_scale_wts||output_name.find("JJH")!=output_name.npos) 
    .set_do_sm_ps_wts(do_sm_scale_wts||output_name.find("JJH")!=output_name.npos)
    .set_do_faketaus(js["baseline"]["do_faketaus"].asBool())
    .set_do_z_weights(strategy_type == strategy::smsummer16 && z_sample)
    .set_trg_applied_in_mc(js["trg_in_mc"].asBool())
    .set_official_ggH(official_ggH||output_name.find("JJH")!=output_name.npos));
 } 


  if(channel == channel::tpzmm){  
    std::function<bool(Muon const*)> muon_probe_id;
    
    if(strategy_type==strategy::cpsummer17 || strategy_type == strategy::cpdecays17 || strategy_type == strategy::cpdecays18){

        muon_probe_id = [](Muon const* m) {return MuonMedium(m); };
        std::function<bool(Muon const*)> MuonLooseID = [](Muon const* m) { return MuonLoose(m) && m->is_global(); }; 
        // low pT mu leg of e+mu trigger
        BuildModule(TagAndProbe<Muon const*>("TagAndProbe_EMLow")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsIsoMu27")
          .set_tag_trg_filters("hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07")
          .set_probe_id(muon_probe_id)
          .set_tag_id(muon_probe_id) 
          .set_probe_trg_objects("triggerObjectsMu17Mu8")
          .set_probe_trg_filters("hltDiMuon178RelTrkIsoFiltered0p4")
          .set_extra_hlt_probe_pt(8.)
          .set_extra_l1_probe_pt(5.)
          .set_add_name("_em_low")
        );
        // high pT mu leg of e+mu trigger
        BuildModule(TagAndProbe<Muon const*>("TagAndProbe_EMHigh")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsIsoMu27")
          .set_tag_trg_filters("hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07")
          .set_probe_id(muon_probe_id)
          .set_tag_id(muon_probe_id)
          .set_probe_trg_objects("triggerObjectsMu17Mu8")
          .set_probe_trg_filters("hltDiMuon178RelTrkIsoFiltered0p4")
          .set_extra_hlt_probe_pt(23.)
          .set_extra_l1_probe_pt(20.)
          .set_add_name("_em_high")
        );
        // mu leg of mu+tau cross trigger for runs C-F
        BuildModule(TagAndProbe<Muon const*>("TagAndProbe_MT_CtoF")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsIsoMu27")
          .set_tag_trg_filters("hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07")
          .set_probe_id(muon_probe_id)
          .set_tag_id(muon_probe_id)
          .set_probe_trg_objects("triggerObjectsDoubleMu20")
          .set_probe_trg_filters("hltL3crIsoL1sDoubleMu18erL1f0L2f10QL3f20QL3trkIsoFiltered0p07")
          .set_add_name("_mt_c_to_f")
        );
        // mu leg of mu+tau cross trigger for run B
        BuildModule(TagAndProbe<Muon const*>("TagAndProbe_MT_B")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsIsoMu27")
          .set_tag_trg_filters("hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07")
          .set_probe_id(muon_probe_id)
          .set_tag_id(muon_probe_id)
          .set_probe_trg_objects("triggerObjectsIsoMu20Tau27")
          .set_probe_trg_filters("hltL3crIsoL1sMu18erTau24erIorMu20erTau24erL1f0L2f10QL3f20QL3trkIsoFiltered0p07")
          .set_do_extra(true)
          .set_add_name("_mt_b")
        );
        // single muons trigger measurment
        BuildModule(TagAndProbe<Muon const*>("TagAndProbe_single")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsIsoMu27")
          .set_tag_trg_filters("hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07")
          .set_probe_id(muon_probe_id)
          .set_tag_id(muon_probe_id)
          .set_probe_trg_objects("triggerObjectsIsoMu27,triggerObjectsIsoMu24")
          .set_probe_trg_filters("hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07,hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07")  
        );
        // low pT leg of dimuon trigger used for embedded selection
        BuildModule(TagAndProbe<Muon const*>("TagAndProbe_DiMuLow")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsIsoMu27")
          .set_tag_trg_filters("hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07")
          .set_probe_id(MuonLooseID)
          .set_tag_id(muon_probe_id)
          .set_probe_trg_objects("triggerObjectsIsoMu27,triggerObjectsIsoMu24")
          .set_probe_trg_filters("hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07,hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07")
          .set_add_name("_dimu_low")
          .set_probe_trg_objects("triggerObjectsMu17Mu8DZ,triggerObjectsMu17Mu8DZmass8,triggerObjectsMu17Mu8")
          .set_probe_trg_filters("hltDiMuon178RelTrkIsoFiltered0p4,hltDiMuon178RelTrkIsoFiltered0p4,hltDiMuon178RelTrkIsoFiltered0p4")
          .set_extra_hlt_probe_pt(8.)

        );
        // high pT leg of dimuon trigger used for embedded selection
        BuildModule(TagAndProbe<Muon const*>("TagAndProbe_DiMuhigh")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsIsoMu27")
          .set_tag_trg_filters("hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07")
          .set_probe_id(MuonLooseID)
          .set_tag_id(muon_probe_id)
          .set_probe_trg_objects("triggerObjectsIsoMu27,triggerObjectsIsoMu24")
          .set_probe_trg_filters("hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07,hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07")
          .set_add_name("_dimu_high")
          .set_probe_trg_objects("triggerObjectsMu17Mu8DZ,triggerObjectsMu17Mu8DZmass8")
          .set_probe_trg_filters("hltDiMuon178RelTrkIsoFiltered0p4,hltDiMuon178RelTrkIsoFiltered0p4")
          .set_extra_hlt_probe_pt(17.)
        );
        // use to measure DZ filter efficiency for embedded selection selection
        BuildModule(TagAndProbe<Muon const*>("TagAndProbe_DiMuDZ")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsIsoMu27")
          .set_tag_trg_filters("hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07")
          .set_probe_id(MuonLooseID)
          .set_tag_id(MuonLooseID)
          .set_probe_trg_objects("triggerObjectsIsoMu27,triggerObjectsIsoMu24")
          .set_probe_trg_filters("hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07,hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07")
          .set_add_name("_dimu_dz")
          .set_probe_trg_objects("triggerObjectsMu17Mu8DZ,triggerObjectsMu17Mu8DZmass8,triggerObjectsMu17Mu8")
          .set_probe_trg_filters("hltDiMuon178RelTrkIsoFiltered0p4,hltDiMuon178RelTrkIsoFiltered0p4,hltDiMuon178RelTrkIsoFiltered0p4")
          .set_extra_hlt_probe_pt(8.)
          .set_do_dzmass(true)
        );
         
    } else {
      muon_probe_id = [](Muon const* m) {return MuonMedium(m); };
      std::function<bool(Muon const*)> MuonLooseID = [](Muon const* m) { return MuonLoose(m) && m->is_global(); };
      // for mu8 leg of EMu cross-trigger
      BuildModule(TagAndProbe<Muon const*>("TagAndProbe_emLow")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsIsoMu24")
          .set_tag_trg_filters("hltL3crIsoL1sMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p09")
          .set_probe_id(muon_probe_id)
          //.set_probe_id(MuonLooseID)
          .set_tag_id(muon_probe_id)
          .set_add_name("_emLow")          
          .set_probe_trg_objects("triggerObjectsMu17Mu8")
          .set_probe_trg_filters("hltDiMuonGlb17Glb8RelTrkIsoFiltered0p4")
          .set_extra_hlt_probe_pt(8.)
          .set_extra_l1_probe_pt(5.)
      );          
          
          // for mu23 leg of EMu cross-trigger
      BuildModule(TagAndProbe<Muon const*>("TagAndProbe_emHigh")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsIsoMu24")
          .set_tag_trg_filters("hltL3crIsoL1sMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p09")
          .set_probe_id(muon_probe_id)
          //.set_probe_id(MuonLooseID)
          .set_tag_id(muon_probe_id)
          .set_add_name("_emHigh")          
          .set_probe_trg_objects("triggerObjectsMu17Mu8")
          .set_probe_trg_filters("hltDiMuonGlb17Glb8RelTrkIsoFiltered0p4")
          .set_extra_hlt_probe_pt(23.)
          .set_extra_l1_probe_pt(20.)
      );
          
      // for single muon trigger:
      BuildModule(TagAndProbe<Muon const*>("TagAndProbe_Single")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsIsoMu24")
          .set_tag_trg_filters("hltL3crIsoL1sMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p09")
          .set_probe_id(muon_probe_id)
          //.set_probe_id(MuonLooseID)
          .set_tag_id(muon_probe_id)
          .set_add_name("_Single")                    
          .set_probe_trg_objects("triggerObjectsIsoMu22,triggerObjectsIsoTkMu22,triggerObjectsIsoMu22Eta2p1,triggerObjectsIsoTkMu22Eta2p1")
          .set_probe_trg_filters("hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09,hltL3fL1sMu20L1f0Tkf22QL3trkIsoFiltered0p09,hltL3crIsoL1sSingleMu20erL1f0L2f10QL3f22QL3trkIsoFiltered0p09,hltL3fL1sMu20erL1f0Tkf22QL3trkIsoFiltered0p09")
      );

          // for muon leg of mu+tau cross trigger:
      BuildModule(TagAndProbe<Muon const*>("TagAndProbe_mt")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsIsoMu24")
          .set_tag_trg_filters("hltL3crIsoL1sMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p09")
          .set_probe_id(muon_probe_id)
          .set_tag_id(muon_probe_id)
          .set_add_name("_mt")          
          .set_probe_trg_objects("triggerObjectsIsoMu19LooseTau20SingleL1")
          .set_probe_trg_filters("hltL3crIsoL1sSingleMu18erIorSingleMu20erL1f0L2f10QL3f19QL3trkIsoFiltered0p09")
      
      );

        // low pT leg of dimuon trigger used for embedded selection
        BuildModule(TagAndProbe<Muon const*>("TagAndProbe_DiMuLow")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsIsoMu24")
          .set_tag_trg_filters("hltL3crIsoL1sMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p09")
          .set_probe_id(MuonLooseID)
          .set_tag_id(muon_probe_id)
          .set_add_name("_dimu_low")
          .set_probe_trg_objects("triggerObjectsMu17TkMu8,triggerObjectsMu17Mu8")
          .set_probe_trg_filters("hltDiMuonGlb17Trk8RelTrkIsoFiltered0p4,hltDiMuonGlb17Glb8RelTrkIsoFiltered0p4")
          .set_extra_hlt_probe_pt(8.)
        );

        // high pT leg of dimuon trigger used for embedded selection
        BuildModule(TagAndProbe<Muon const*>("TagAndProbe_DiMuhigh")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsIsoMu24")
          .set_tag_trg_filters("hltL3crIsoL1sMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p09")
          .set_probe_id(MuonLooseID)
          .set_tag_id(muon_probe_id)
          .set_add_name("_dimu_high")
          .set_probe_trg_objects("triggerObjectsMu17TkMu8,triggerObjectsMu17Mu8")
          .set_probe_trg_filters("hltDiMuonGlb17Trk8RelTrkIsoFiltered0p4,hltDiMuonGlb17Glb8RelTrkIsoFiltered0p4")
          .set_extra_hlt_probe_pt(17.)
        );

        // use to measure DZ filter efficiency for embedded selection selection
        BuildModule(TagAndProbe<Muon const*>("TagAndProbe_DiMuDZ")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsIsoMu24")
          .set_tag_trg_filters("hltL3crIsoL1sMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p09")
          .set_probe_id(MuonLooseID)
          .set_tag_id(MuonLooseID)
          .set_add_name("_dimu_dz")
          .set_probe_trg_objects("triggerObjectsMu17TkMu8,triggerObjectsMu17Mu8")
          .set_probe_trg_filters("hltDiMuonGlb17Trk8RelTrkIsoFiltered0p4,hltDiMuonGlb17Glb8RelTrkIsoFiltered0p4")
          .set_extra_hlt_probe_pt(8.)
          .set_do_dzmass(true)
        );

    }
  } 

  if(channel == channel::tpzee){
    if(strategy_type == strategy::cpsummer17 || strategy_type == strategy::cpdecays17 || strategy_type == strategy::cpdecays18){
      std::function<bool(Electron const*)> elec_probe_id = [](Electron const* e) { return ElectronHTTIdFall17V2(e, true); };
      std::function<bool(Electron const*)> elec_tag_id = [](Electron const* e) { return ElectronHTTIdFall17V2(e, false); };

      // for el12 leg of EMu cross-trigger
      BuildModule(TagAndProbe<Electron const*>("TagAndProbe_emLow")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsEle35")
          .set_tag_trg_filters("hltEle35noerWPTightGsfTrackIsoFilter")
          .set_extra_l1_tag_pt(32.) // ensure L1 was not prescaled during data-taking
          .set_probe_id(elec_probe_id)
          .set_tag_id(elec_tag_id)
          .set_probe_trg_objects("triggerObjectsEle24Ele12") 
          .set_probe_trg_filters("hltEle23Ele12CaloIdLTrackIdLIsoVLTrackIsoLeg2Filter")
          .set_extra_l1_probe_pt(10.)
          .set_add_name("_emLow")
      );

      // for el23 leg of EMu cross-trigger
      BuildModule(TagAndProbe<Electron const*>("TagAndProbe_emHigh")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsEle35")
          .set_tag_trg_filters("hltEle35noerWPTightGsfTrackIsoFilter")
          .set_extra_l1_tag_pt(32.) // ensure L1 was not prescaled during data-taking
          .set_probe_id(elec_probe_id)
          .set_tag_id(elec_tag_id)
          .set_probe_trg_objects("triggerObjectsEle24Ele12") //Ele23 actually-> 
          .set_probe_trg_filters("hltEle23Ele12CaloIdLTrackIdLIsoVLTrackIsoLeg1Filter")
          .set_extra_l1_probe_pt(23.)
          .set_extra_l1_iso_probe_pt(20.)
          .set_add_name("_emHigh")
      );

      if(strategy_type == strategy::cpdecays18) {
        BuildModule(TagAndProbe<Electron const*>("TagAndProbe_ET")
            .set_fs(fs.get())
            .set_channel(channel)
            .set_strategy(strategy_type)
            .set_ditau_label("ditau")
            .set_tag_trg_objects("triggerObjectsEle35")
            .set_tag_trg_filters("hltEle35noerWPTightGsfTrackIsoFilter")
            .set_extra_l1_tag_pt(32.) // ensure L1 was not prescaled during data-taking
            .set_probe_id(elec_probe_id)
            .set_tag_id(elec_tag_id)
            .set_probe_trg_objects("triggerObjectsEle24Tau30,triggerObjectsEle24TauHPS30") // for 2018
            .set_probe_trg_filters("hltEle24erWPTightClusterShapeFilterForTau,hltEle24erWPTightClusterShapeFilterForTau") // for 2018
            .set_do_extra(true)
            .set_add_name("_ET")
        );
      } else {

        // for el23 leg of EMu cross-trigger
        BuildModule(TagAndProbe<Electron const*>("TagAndProbe_ET")
            .set_fs(fs.get())
            .set_channel(channel)
            .set_strategy(strategy_type)
            .set_ditau_label("ditau")
            .set_tag_trg_objects("triggerObjectsEle35")
            .set_tag_trg_filters("hltEle35noerWPTightGsfTrackIsoFilter")
            .set_extra_l1_tag_pt(32.) // ensure L1 was not prescaled during data-taking
            .set_probe_id(elec_probe_id)
            .set_tag_id(elec_tag_id)
            .set_probe_trg_objects("triggerObjectsEle24Tau30") // for 2017
            .set_probe_trg_filters("hltEle24erWPTightGsfTrackIsoFilterForTau") // for 2017
            .set_do_extra(true)
            .set_add_name("_ET")
        );
      }

      BuildModule(TagAndProbe<Electron const*>("TagAndProbe_single")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsEle35")
          .set_tag_trg_filters("hltEle35noerWPTightGsfTrackIsoFilter")
          .set_extra_l1_tag_pt(32.) // ensure L1 was not prescaled during data-taking
          .set_probe_id(elec_probe_id)
          .set_tag_id(elec_tag_id)
          .set_probe_trg_objects("triggerObjectsEle27,triggerObjectsEle32L1DoubleEG") 
          .set_probe_trg_filters("hltEle27WPTightGsfTrackIsoFilter,hltEle32L1DoubleEGWPTightGsfTrackIsoFilter")
      );

    } else {
        
      std::function<bool(Electron const*)> elec_probe_id;  
      std::function<bool(Electron const*)> elec_tag_id;  
      elec_probe_id = [](Electron const* e) { return ElectronHTTIdFall17V2(e, true); };
      elec_tag_id = [](Electron const* e) { return ElectronHTTIdFall17V2(e, false); };
      
          // for Ele12 leg of EMu cross-trigger
      BuildModule(TagAndProbe<Electron const*>("TagAndProbe_emLow")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsEle25GsfTightEta2p1")
          .set_tag_trg_filters("hltEle25erWPTightGsfTrackIsoFilter")
          .set_add_name("_emLow")          
          .set_probe_trg_objects("triggerObjectsEle23Ele12")
          .set_probe_trg_filters("hltEle23Ele12CaloIdLTrackIdLIsoVLTrackIsoLeg2Filter")
          .set_extra_l1_probe_pt(10.)
          .set_probe_id(elec_probe_id)
          .set_tag_id(elec_tag_id)
          // em filters hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter -> electron 
      );   
          
          //// for Ele23 leg of EMu cross-trigger
      BuildModule(TagAndProbe<Electron const*>("TagAndProbe_emHigh")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_tag_trg_objects("triggerObjectsEle25GsfTightEta2p1")
          .set_tag_trg_filters("hltEle25erWPTightGsfTrackIsoFilter")
          .set_add_name("_emHigh")          
          .set_probe_trg_objects("triggerObjectsEle23Ele12")
          .set_probe_trg_filters("hltEle23Ele12CaloIdLTrackIdLIsoVLTrackIsoLeg1Filter")
          .set_extra_l1_probe_pt(20.)
          .set_extra_l1_iso_probe_pt(18.)
          .set_probe_id(elec_probe_id)
          .set_tag_id(elec_tag_id)
      );
          // for single electron trigger
      BuildModule(TagAndProbe<Electron const*>("TagAndProbe_Single")
          .set_fs(fs.get())
          .set_channel(channel)
          .set_strategy(strategy_type)
          .set_ditau_label("ditau")
          .set_probe_id(elec_probe_id)
          .set_tag_id(elec_tag_id)
          .set_tag_trg_objects("triggerObjectsEle25GsfTightEta2p1")
          .set_tag_trg_filters("hltEle25erWPTightGsfTrackIsoFilter")          
          .set_add_name("_Single")          
          .set_probe_trg_objects("triggerObjectsEle25GsfTightEta2p1")
          .set_probe_trg_filters("hltEle25erWPTightGsfTrackIsoFilter")
          
      );  
    }
  } 

}
}

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

  std::function<bool(Electron const*)> ElecID = [](Electron const* e) { return ElectronHTTIdFall17V2(e, true); };

  BuildModule(SimpleFilter<Electron>("ElectronFilter")
      .set_input_label("sel_electrons").set_min(1)
      .set_predicate([=](Electron const* e) {
        return ElecID(e);
      }));

  if (!is_embedded) {
    BuildModule(HTTSmearScale("ElectronSmearScaleCorrection")
        .set_input_label(js["electrons"].asString())
        .set_e_unc_mode(e_unc_mode)
    );
  }
  if(!is_data && is_embedded){
    BuildModule(HTTEnergyScale("ElectronEnergyScaleCorrection")
        .set_input_label(js["electrons"].asString())
        .set_shift(elec_shift_barrel)
        .set_shift_endcap(elec_shift_endcap)
        .set_strategy(strategy_type)
        .set_channel(channel::em)
        .set_moriond_corrections(moriond_tau_scale));
  }

  BuildModule(SimpleFilter<Electron>("ElectronFilter")
      .set_input_label("sel_electrons").set_min(1)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > elec_pt    &&
                fabs(e->eta())          < elec_eta   &&
                fabs(e->dxy_vertex())   < elec_dxy   &&
                fabs(e->dz_vertex())    < elec_dz ;

      }));

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

  if (mu_scale_mode > 0){
    BuildModule(HTTMuonEnergyScale("MuonEnergyScaleCorrection")
       .set_input_label("muons")
       .set_far_endcap(muon_shift_farendcap)
       .set_near_endcap(muon_shift_nearendcap)
       .set_barrel(muon_shift_barrel)
       );
  }
   
  BuildModule(CopyCollection<Muon>("CopyToSelectedMuons",
       js["muons"].asString(), "sel_muons"));

  std::function<bool(Muon const*)> MuonID = [](Muon const* m) { return MuonMedium(m); };

  BuildModule(SimpleFilter<Muon>("MuonFilter")
    .set_input_label("sel_muons").set_min(1)
    .set_predicate([=](Muon const* m) {
      return  m->pt()                 > muon_pt    &&
              fabs(m->eta())          < muon_eta   &&
              fabs(m->dxy_vertex())   < muon_dxy   &&
              fabs(m->dz_vertex())    < muon_dz   &&
              MuonID(m);
    }));

  BuildTauSelection();

  if(js["do_mt_tagandprobe"].asBool()){
    BuildModule(OverlapFilter<Tau,Muon>("TauMuonOverlapFilter")
      .set_input_label(js["taus"].asString())
      .set_reference_label("loose_muons")
      .set_min_dr(0.5));
  }

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

  if (mu_scale_mode > 0){
    BuildModule(HTTMuonEnergyScale("MuonEnergyScaleCorrection")
       .set_input_label("muons")
       .set_far_endcap(muon_shift_farendcap)
       .set_near_endcap(muon_shift_nearendcap)
       .set_barrel(muon_shift_barrel)
       );
  }

  BuildModule(CopyCollection<Muon>("CopyToSelectedMuons",
      js["muons"].asString(), "sel_muons"));

  std::function<bool(Muon const*)> MuonID = [](Muon const* m) { return MuonMedium(m); };

  BuildModule(SimpleFilter<Muon>("MuonFilter")
      .set_input_label("sel_muons").set_min(1)
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > muon_pt    &&
                fabs(m->eta())          < muon_eta   &&
                fabs(m->dxy_vertex())   < muon_dxy   &&
                fabs(m->dz_vertex())    < muon_dz   &&
                PF04IsolationVal(m, 0.5,0) < 0.5  &&
                MuonID(m);

      }));

  BuildModule(CopyCollection<Electron>("CopyToSelectedElectrons",
      js["electrons"].asString(), "sel_electrons"));


  std::function<bool(Electron const*)> ElecID = [](Electron const* e) { return ElectronHTTIdFall17V2(e, true); }; 

  BuildModule(SimpleFilter<Electron>("ElectronFilter")
      .set_input_label("sel_electrons").set_min(1)
      .set_predicate([=](Electron const* e) {
        return ElecID(e);
      }));

  if (!is_embedded) {
    BuildModule(HTTSmearScale("ElectronSmearScaleCorrection")
        .set_input_label(js["electrons"].asString())
        .set_e_unc_mode(e_unc_mode)
    );
  }
  if(!is_data && is_embedded){
    BuildModule(HTTEnergyScale("ElectronEnergyScaleCorrection")
        .set_input_label(js["electrons"].asString())
        .set_shift(elec_shift_barrel)
        .set_shift_endcap(elec_shift_endcap)
        .set_strategy(strategy_type)
        .set_channel(channel::em)
        .set_moriond_corrections(moriond_tau_scale));
  }

  BuildModule(SimpleFilter<Electron>("ElectronFilter")
      .set_input_label("sel_electrons").set_min(1)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > elec_pt    &&
                fabs(e->eta())          < elec_eta   &&
                fabs(e->dxy_vertex())   < elec_dxy   &&
                fabs(e->dz_vertex())    < elec_dz ;
      }));
  
  BuildModule(CompositeProducer<Electron, Muon>("EMPairProducer")
      .set_input_label_first("sel_electrons")
      .set_input_label_second("sel_muons")
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));

  BuildModule(SimpleFilter<CompositeCandidate>("EMPairFilter")
      .set_input_label("ditau").set_min(1)
      .set_predicate([=](CompositeCandidate const* c) {
        return PairOneWithPt(c, 24.0);
      }));
}

// --------------------------------------------------------------------------
// ZEE Pair Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildZEEPairs() {
  ic::strategy strategy_type  = String2Strategy(strategy_str);

  BuildModule(CopyCollection<Electron>("CopyToSelectedElectrons",
      js["electrons"].asString(), "sel_electrons"));
  BuildModule(CopyCollection<Electron>("CopyToSelectedElectrons",
      js["electrons"].asString(), "sel_lead_electrons"));

  std::function<bool(Electron const*)> ElecID = [](Electron const* e) { return ElectronHTTIdFall17V2(e, true); }; 

  BuildModule(SimpleFilter<Electron>("ElectronIDFilter")
      .set_input_label("sel_electrons").set_min(2)
      .set_predicate([=](Electron const* e) {
        return ElecID(e);
      }));
  BuildModule(SimpleFilter<Electron>("LeadElectronIDFilter")
      .set_input_label("sel_lead_electrons").set_min(1)
      .set_predicate([=](Electron const* e) {
        return ElecID(e) ;
      }));

  if (!is_embedded) {
    BuildModule(HTTSmearScale("ElectronSmearScaleCorrection")
        .set_input_label(js["electrons"].asString())
        .set_e_unc_mode(e_unc_mode)
    );
  }
  if(!is_data && is_embedded){
    BuildModule(HTTEnergyScale("ElectronEnergyScaleCorrection")
        .set_input_label(js["electrons"].asString())
        .set_shift(elec_shift_barrel)
        .set_shift_endcap(elec_shift_endcap)
        .set_strategy(strategy_type)
        .set_channel(channel::em)
        .set_moriond_corrections(moriond_tau_scale));
  }

  BuildModule(SimpleFilter<Electron>("ElectronFilter")
      .set_input_label("sel_electrons").set_min(2)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > 13.        &&
                fabs(e->eta())          < 2.4        &&
                fabs(e->dxy_vertex())   < elec_dxy   &&
                fabs(e->dz_vertex())    < elec_dz ;

      }));
  
  BuildModule(SimpleFilter<Electron>("LeadElectronFilter")
      .set_input_label("sel_lead_electrons").set_min(1)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > elec_pt    &&
                fabs(e->eta())          < elec_eta   &&
                fabs(e->dxy_vertex())   < elec_dxy   &&
                fabs(e->dz_vertex())    < elec_dz ;

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

  BuildModule(CopyCollection<Electron>("CopyToSelectedElectrons",
      js["electrons"].asString(), "sel_electrons"));

  std::function<bool(Electron const*)> ElecID = [](Electron const* e) { return ElectronHTTIdFall17V2(e, true); }; //set to false for 80wp //ElectronHTTIdFall17V2 //ElectronHTTIsoIdFall17V2

  BuildModule(SimpleFilter<Electron>("ProbeFilter")
      .set_input_label("sel_electrons").set_min(2)
      .set_max(2)
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
 
 if (mu_scale_mode > 0){
   BuildModule(HTTMuonEnergyScale("MuonEnergyScaleCorrection")
      .set_input_label("muons")
      .set_far_endcap(muon_shift_farendcap)
      .set_near_endcap(muon_shift_nearendcap)
      .set_barrel(muon_shift_barrel)
      );
 }
 
 BuildModule(CopyCollection<Muon>("CopyToSelectedMuons",
      js["muons"].asString(), "sel_muons"));

  std::function<bool(Muon const*)> MuonID = [](Muon const* m) { return MuonMedium(m); };
   
  BuildModule(SimpleFilter<Muon>("MuonFilter")
      .set_input_label("sel_muons").set_min(2)
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > 10.        &&
                fabs(m->eta())          < 2.4   &&
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
  
  
  if(js["baseline"]["do_faketaus"].asBool()){
    BuildModule(SimpleFilter<Tau>("TauFilter")
        .set_input_label(js["taus"].asString()).set_min(0)
        .set_predicate([=](Tau const* t) {
          return  t->pt()                     > 15          &&
                  fabs(t->eta())              <  2.3        &&
                  fabs(t->lead_dz_vertex())   <  0.2        &&
                  fabs(t->charge())           == 1          &&
                  t->GetTauID("decayModeFindingNewDMs") > 0.5 && (t->decay_mode()<2 || t->decay_mode()>9);
    
        }));  
    BuildModule(OverlapFilter<Tau,CompositeCandidate>("TauMuonOverlapFilter")
      .set_input_label(js["taus"].asString())
      .set_reference_label("ditau")
      .set_min_dr(0.5));
  }
  
}

// --------------------------------------------------------------------------
// Tag and probe ZMM Pair Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildTPZMMPairs() {
 
 BuildModule(CopyCollection<Muon>("CopyToSelectedMuons",
      js["muons"].asString(), "sel_muons"));

  std::function<bool(Muon const*)> MuonID = [](Muon const* m) { return MuonMedium(m); };

  BuildModule(SimpleFilter<Muon>("ProbeFilter")
      .set_input_label("sel_muons").set_min(2).set_max(2)
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
// Tau Selection Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildTauSelection(){
  
  // filter taus first with loose pT cut - this avoids running more time consuming parts of the code for events with taus that just wont pass the offline cuts anyway 
  double loose_tau_pt = tau_pt*0.8;
  BuildModule(SimpleFilter<Tau>("TauFilterNewDMLoosePT")
     .set_input_label(js["taus"].asString()).set_min(min_taus)
     .set_predicate([=](Tau const* t) {
       return  t->pt()                     >  loose_tau_pt &&
               fabs(t->eta())              <  tau_eta    &&
               fabs(t->lead_dz_vertex())   <  tau_dz     &&
               fabs(t->charge())           == 1          &&
               t->GetTauID("decayModeFindingNewDMs") > 0.5 && (t->decay_mode()<2 || t->decay_mode()>9) &&
               t->GetTauID("byVVVLooseDeepTau2017v2p1VSjet") > 0.5 && t->GetTauID("byVVVLooseDeepTau2017v2p1VSe") > 0.5 && t->GetTauID("byVLooseDeepTau2017v2p1VSmu") > 0.5;

     }));
 
 if (tau_scale_mode > 0 && !is_data){
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

    BuildModule(CopyCollection<Tau>("CopyTauHadTo3Prong1Pi",
      "genmatched_taus", "genmatched_taus_3prong1pi0"));

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
    
    BuildModule(SimpleFilter<Tau>("3Prong1PiTauHadFilter")
      .set_input_label("genmatched_taus_3prong1pi0")
      .set_predicate([=](Tau const* t) {
        return  t->decay_mode() == 11;
      }));
 
    BuildModule(EnergyShifter<Tau>("TauEnergyShifter1prong0pi0")
    .set_input_label("genmatched_taus_1prong0pi0")
    .set_save_shifts(true) 
    .set_shift_label("scales_taues_1prong0pi0") 
    .set_shift(tau_shift_1prong0pi0)
    .set_shift_func(tau_shift_func_1prong0pi0));

    BuildModule(EnergyShifter<Tau>("TauEnergyShifter1prong1pi0")
    .set_input_label("genmatched_taus_1prong1pi0")
    .set_save_shifts(true) 
    .set_shift_label("scales_taues_1prong1pi0") 
    .set_shift(tau_shift_1prong1pi0)
    .set_shift_func(tau_shift_func_1prong1pi0));

    BuildModule(EnergyShifter<Tau>("TauEnergyShifter3prong0pi0")
    .set_input_label("genmatched_taus_3prong0pi0")
    .set_save_shifts(true) 
    .set_shift_label("scales_taues_3prong0pi0") 
    .set_shift(tau_shift_3prong0pi0)
    .set_shift_func(tau_shift_func_3prong0pi0));

    BuildModule(EnergyShifter<Tau>("TauEnergyShifter3prong1pi0")
    .set_input_label("genmatched_taus_3prong1pi0")
    .set_save_shifts(true)
    .set_shift_label("scales_taues_3prong1pi0")
    .set_shift(tau_shift_3prong1pi0)
    .set_shift_func(tau_shift_func_3prong1pi0));
 }
 if (!is_data){
   BuildModule(HTTGenMatchSelector<Tau>("FakeEGenMatchSelector")
     .set_input_vec_label(js["taus"].asString())
     .set_output_vec_label("fakeE_genmatched_taus")
     .set_gen_match(mcorigin::promptE));
   
   BuildModule(CopyCollection<Tau>("CopyTo1Prong0Pi",
     "fakeE_genmatched_taus", "fakeE_genmatched_taus_0pi"));
   
   BuildModule(CopyCollection<Tau>("CopyTo1Prong1Pi",
     "fakeE_genmatched_taus", "fakeE_genmatched_taus_1pi"));

   BuildModule(CopyCollection<Tau>("CopyTo1Prong0Pi",
     "fakeE_genmatched_taus", "fakeE_genmatched_taus_0pi_endcap"));

   BuildModule(CopyCollection<Tau>("CopyTo1Prong1Pi",
     "fakeE_genmatched_taus", "fakeE_genmatched_taus_1pi_endcap"));
   
   BuildModule(SimpleFilter<Tau>("1Prong0PiTauFilter")
     .set_input_label("fakeE_genmatched_taus_0pi")
     .set_predicate([=](Tau const* t) {
       return  t->decay_mode() == 0 && fabs(t->eta()) < 1.5;
     }));
   
   BuildModule(SimpleFilter<Tau>("1Prong1PiTauFilter")
     .set_input_label("fakeE_genmatched_taus_1pi")
     .set_predicate([=](Tau const* t) {
       return  t->decay_mode() == 1 && fabs(t->eta()) < 1.5;
     }));
    
   BuildModule(SimpleFilter<Tau>("1Prong0PiEndCapTauFilter")
     .set_input_label("fakeE_genmatched_taus_0pi_endcap")
     .set_predicate([=](Tau const* t) {
       return  t->decay_mode() == 0 && fabs(t->eta()) >= 1.5;
     }));

   BuildModule(SimpleFilter<Tau>("1Prong1PiEndCapTauFilter")
     .set_input_label("fakeE_genmatched_taus_1pi_endcap")
     .set_predicate([=](Tau const* t) {
       return  t->decay_mode() == 1 && fabs(t->eta()) >= 1.5;
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

   BuildModule(EnergyShifter<Tau>("FakeE1Prong0PiEndCapEnergyShifter")
   .set_input_label("fakeE_genmatched_taus_0pi_endcap")
   .set_save_shifts(true)
   .set_shift_label("scales_efaketaues_1prong0pi0_endcap")
   .set_shift(fakeE_tau_shift_0pi_endcap));

   BuildModule(EnergyShifter<Tau>("FakeE1Prong1PiEndCapEnergyShifter")
   .set_input_label("fakeE_genmatched_taus_1pi_endcap")
   .set_save_shifts(true)
   .set_shift_label("scales_efaketaues_1prong1pi0_endcap")
   .set_shift(fakeE_tau_shift_1pi_endcap));

   BuildModule(HTTGenMatchSelector<Tau>("FakeMuGenMatchSelector")
     .set_input_vec_label(js["taus"].asString())
     .set_output_vec_label("fakeMu_genmatched_taus")
     .set_gen_match(mcorigin::promptMu));
   
   BuildModule(CopyCollection<Tau>("CopyTo1Prong0Pi",
     "fakeMu_genmatched_taus", "fakeMu_genmatched_taus_0pi"));
   
   BuildModule(CopyCollection<Tau>("CopyTo1Prong1Pi",
     "fakeMu_genmatched_taus", "fakeMu_genmatched_taus_1pi"));
   
   BuildModule(SimpleFilter<Tau>("1Prong0PiTauFilter")
     .set_input_label("fakeMu_genmatched_taus_0pi")
     .set_predicate([=](Tau const* t) {
       return  t->decay_mode() == 0;
     }));
   
   BuildModule(SimpleFilter<Tau>("1Prong1PiTauFilter")
     .set_input_label("fakeMu_genmatched_taus_1pi")
     .set_predicate([=](Tau const* t) {
       return  t->decay_mode() == 1;
     }));
    
   BuildModule(EnergyShifter<Tau>("FakeMu1Prong0PiEnergyShifter")
   .set_input_label("fakeMu_genmatched_taus_0pi")
   .set_save_shifts(true)
   .set_shift_label("scales_mufaketaues_1prong0pi0")
   .set_shift(fakeMu_tau_shift_0pi));
   
   BuildModule(EnergyShifter<Tau>("FakeMu1Prong1PiEnergyShifter")
   .set_input_label("fakeMu_genmatched_taus_1pi")
   .set_save_shifts(true)
   .set_shift_label("scales_mufaketaues_1prong1pi0")
   .set_shift(fakeMu_tau_shift_1pi));

 }

 BuildModule(SimpleFilter<Tau>("TauFilterNewDM")
    .set_input_label(js["taus"].asString()).set_min(min_taus)
    .set_predicate([=](Tau const* t) {
      return  t->pt()                     >  tau_pt     &&
              fabs(t->eta())              <  tau_eta    &&
              fabs(t->lead_dz_vertex())   <  tau_dz     &&
              fabs(t->charge())           == 1          &&
              t->GetTauID("decayModeFindingNewDMs") > 0.5 && (t->decay_mode()<2 || t->decay_mode()>9) &&
              t->GetTauID("byVVVLooseDeepTau2017v2p1VSjet") > 0.5 && t->GetTauID("byVVVLooseDeepTau2017v2p1VSe") > 0.5 && t->GetTauID("byVLooseDeepTau2017v2p1VSmu") > 0.5; 

    }));

 }

 void HTTSequence::BuildDiElecVeto() {

   BuildModule(CopyCollection<Electron>("CopyToVetoElecs",
       js["electrons"].asString(), "veto_elecs"));

   BuildModule(GenericModule("VetoElecIDIsoFilter")
      .set_function([=](ic::TreeEvent *event){
      EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
      std::vector<Electron*> & vec = event->GetPtrVec<Electron>("veto_elecs");
      ic::erase_if(vec,!boost::bind(VetoElectronIDFall17,_1, eventInfo->jet_rho())); //lepton_rho
      ic::erase_if(vec,!boost::bind(PF03EAElecIsolation, _1, eventInfo->jet_rho(), 0.3));
      return 0;
   }));    

   SimpleFilter<Electron> vetoElecFilter = SimpleFilter<Electron>("VetoElecFilter")
      .set_input_label("veto_elecs");
   vetoElecFilter.set_predicate([=](Electron const* e) {
     return  e->pt()                 > veto_dielec_pt    &&
            fabs(e->eta())          < veto_dielec_eta   &&
            fabs(e->dxy_vertex())   < veto_dielec_dxy   &&
            fabs(e->dz_vertex())    < veto_dielec_dz;
   });

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
   BuildModule(vetoElecPairFilter);
 }

 void HTTSequence::BuildDiMuonVeto() {

  BuildModule(CopyCollection<Muon>("CopyToVetoMuons",
      js["muons"].asString(), "veto_muons"));

  SimpleFilter<Muon> vetoMuonFilter = SimpleFilter<Muon>("VetoMuonFilter")
      .set_input_label("veto_muons");
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
	
  BuildModule(vetoMuonPairFilter);

 }

 void HTTSequence::BuildExtraElecVeto(){
 
   BuildModule(CopyCollection<Electron>("CopyToExtraElecs",
       js["electrons"].asString(), "extra_elecs"));
 
   BuildModule(GenericModule("ExtraElecIsoFilter")
     .set_function([=](ic::TreeEvent *event){
        EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
        std::vector<Electron*> & vec = event->GetPtrVec<Electron>("extra_elecs");
        ic::erase_if(vec,!boost::bind(PF03EAElecIsolation, _1, eventInfo->jet_rho(), 0.3)); //lepton_rho
        return 0;
   }));
  
   HTTFilter<Electron> extraElecFilter = HTTFilter<Electron>("ExtraElecFilter")
       .set_input_label("extra_elecs")
       .set_veto_name("extra_elec_veto")
       .set_min(0).set_max(js["baseline"]["max_extra_elecs"].asUInt());
   extraElecFilter.set_no_filter(true);
   extraElecFilter.set_predicate([=](Electron const* e) {
      return  e->pt()                 > veto_elec_pt    &&
              fabs(e->eta())          < veto_elec_eta   &&
              fabs(e->dxy_vertex())   < veto_elec_dxy   &&
              fabs(e->dz_vertex())    < veto_elec_dz    &&
              ElectronHTTIdFall17V2(e, true);
    });
   BuildModule(extraElecFilter);
 }
 
 
 void HTTSequence::BuildExtraMuonVeto(){
 
   BuildModule(CopyCollection<Muon>("CopyToExtraMuons",
       js["muons"].asString(), "extra_muons"));
 
   HTTFilter<Muon> extraMuonFilter = HTTFilter<Muon>("ExtraMuonFilter")
       .set_input_label("extra_muons")
       .set_veto_name("extra_muon_veto")
       .set_min(0).set_max(js["baseline"]["max_extra_muons"].asUInt());
   std::function<bool(Muon const*)> ExtraMuonID = [](Muon const* m) {return MuonMedium(m); };
   extraMuonFilter.set_no_filter(true);
   extraMuonFilter.set_predicate([=](Muon const* m) {
     return  m->pt()                 > veto_muon_pt    &&
             fabs(m->eta())          < veto_muon_eta   &&
             fabs(m->dxy_vertex())   < veto_muon_dxy   &&
             fabs(m->dz_vertex())    < veto_muon_dz    &&
             ExtraMuonID(m)                     &&
             PF04IsolationVal(m, 0.5,0) < 0.3;
   });
   BuildModule(extraMuonFilter);
 
 
 }
}
