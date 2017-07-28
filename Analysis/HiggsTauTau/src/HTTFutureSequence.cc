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
#include "HiggsTauTau/interface/HTTFutureSequence.h"
#include "HiggsTauTau/interface/HTTElectronEfficiency.h"
#include "HiggsTauTau/interface/HTTMuonEfficiency.h"
#include "HiggsTauTau/interface/HTTTauEfficiency.h"
#include "HiggsTauTau/interface/HTTTriggerFilter.h"
#include "HiggsTauTau/interface/HTTTriggerFilter2.h"
#include "HiggsTauTau/interface/HTTEnergyScale.h"
#include "HiggsTauTau/interface/HTTEMuExtras.h"
#include "HiggsTauTau/interface/HTTGenEvent.h"
#include "HiggsTauTau/interface/HTTFutureGenEvent.h"
#include "HiggsTauTau/interface/HTTFutureTauEff.h"
#include "HiggsTauTau/interface/HTTFutureTauResolution.h"
#include "HiggsTauTau/interface/HTTFutureJetPV.h"
#include "HiggsTauTau/interface/HTTFutureTrees.h"
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

HTTFutureSequence::HTTFutureSequence(std::string& chan, std::string postf, Json::Value const& json) {
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
//  jes_mode=json["baseline"]["jes_mode"].asUInt();
//  metscale_mode=json["baseline"]["metscale_mode"].asUInt();
//  metres_mode=json["baseline"]["metres_mode"].asUInt();
//  do_reshape=json["baseline"]["do_reshape"].asBool(); 
//  btag_mode=json["baseline"]["btag_mode"].asUInt();
//  bfake_mode=json["baseline"]["bfake_mode"].asUInt();
/*  if(js["mc"].asString()==""){std::cout<<"ERROR: MC type not set"<<std::endl; exit(1);}
  mc_str = js["mc"].asString();
  if(js["era"].asString()=="") {std::cout<<"ERROR: era not set"<<std::endl; exit(1);}
  era_str = js["era"].asString();*/
  if(js["strategy"].asString()==""){std::cout<<"ERROR: strategy not set"<<std::endl; exit(1);}
  strategy_str = js["strategy"].asString();
  ic::strategy strategy_type  = String2Strategy(strategy_str);
//  ic::era  era_type  = String2Era(era_str);
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
     elec_dxy = 0.045;
     elec_dz = 0.2;
     muon_dxy = 0.045;
     muon_dz = 0.2;
     elec_pt = 13.;
     elec_eta = 2.5;
     muon_pt = 10;
     muon_eta = 2.4;
 }
 if (channel_str == "et"){
   elec_dz = 0.2;
   elec_dxy = 0.045;
   muon_dxy = 0.045;
   muon_dz = 0.2;
   tau_dz = 0.2;
   pair_dr = 0.5;
   min_taus = 1;
   pair_dr = 0.5;
   elec_pt = 25;
   elec_eta = 2.1;
   tau_pt  = 20;
   tau_eta = 2.3;
  } 
 if (channel_str == "mt"){
  muon_dxy = 0.045;
  muon_dz = 0.2;
  elec_dxy = 0.045;
  elec_dz = 0.2;
  tau_dz = 0.2;
  pair_dr = 0.5;
  muon_pt = 20.0; //cross trigger
  if (strategy_type == strategy::smspring16) muon_eta = 2.4;
  else muon_eta = 2.4;
  tau_pt = 20;
  tau_eta = 3;
  min_taus = 1;
  pair_dr = 0.5;
   }
 if (channel_str == "tt"){
  tau_pt=20;
  tau_eta = 3;
  tau_iso = 1.0;
  tau_dz = 0.2;
  min_taus = 2;
  pair_dr = 0.5;
  }
 jets_label   = json["jets"].asString();
 met_label    = json["met"].asString();
 pu_id_training = json["pu_id_training"].asUInt();

}

HTTFutureSequence::~HTTFutureSequence() {}


void HTTFutureSequence::BuildSequence(){
  using ROOT::Math::VectorUtil::DeltaR;
  
  // Set global parameters that get used in multiple places
  ic::channel channel         = String2Channel(channel_str);
  ic::strategy strategy_type  = String2Strategy(strategy_str);
  // Other flags

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "HiggsToTauTau Analysis" << std::endl;
  std::cout << "-------------------------------------" << std::endl;      std::string param_fmt = "%-25s %-40s\n";
  std::cout << boost::format(param_fmt) % "output" % (output_folder+output_name);
  std::cout << boost::format(param_fmt) % "strategy" % Strategy2String(strategy_type);
  std::cout << boost::format(param_fmt) % "channel" % channel_str;
  std::cout << boost::format(param_fmt) % "new_svfit_mode" % new_svfit_mode;
  if (new_svfit_mode > 0) {
    std::cout << boost::format(param_fmt) % "svfit_folder" % svfit_folder;
    std::cout << boost::format(param_fmt) % "svfit_override" % svfit_override;
  }
  std::cout << boost::format(param_fmt) % "make_sync_ntuple" % js["make_sync_ntuple"].asBool();
  std::cout << boost::format(param_fmt) % "save_output_jsons" % js["save_output_jsons"].asBool();
  std::cout << boost::format(param_fmt) % "allowed_tau_modes" % allowed_tau_modes;
//  std::cout << boost::format(param_fmt) % "large_tscale_shift" % large_tscale_shift;
  std::cout << boost::format(param_fmt) % "pu_id_training" % pu_id_training;
//  std::cout << boost::format(param_fmt) % "make_gen_plots" % make_gen_plots;
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

    file.close();
   } 


 if(js["get_effective"].asBool() && js["make_sync_ntuple"].asBool()){
   std::cerr<< "Error: cannot run effective number of event module in make_syncntuple mode"<<std::endl;
   throw;
 }
 if(js["get_effective"].asBool()){
  BuildModule(EffectiveEvents("EffectiveEvents")
    .set_fs(fs.get()));
  }else{ 
  HTTPrint httPrint("HTTPrint");
  httPrint.set_muon_label(js["muons"].asString());
  httPrint.set_jet_label(jets_label);
  for (auto ch : to_check) {
   eventChecker.CheckEvent(ch);
   httPrint.PrintEvent(ch);
  }
  httPrint.set_skip_events(false);
  if (to_check.size() > 0){
  BuildModule(eventChecker);
  BuildModule(httPrint);  
}

/*  BuildModule(HTTFutureGenEvent("HTTFutureGenEvent")
    .set_genparticle_label("genParticles")
    .set_genjet_label("genJets")
    .set_jets_label(jets_label)
    .set_fs(fs.get()));*/

/*  BuildModule(HTTFutureJetPV("HTTFutureJetPV")
    .set_genjet_label("genJets")
    .set_jets_label(jets_label)
    .set_genparticle_label("genParticles")
    .set_fs(fs.get()));
 
  BuildModule(HTTFutureTauEff("HTTFutureTauEff")
    .set_genparticle_label("genParticles")
    .set_taus_label(js["taus"].asString())
    .set_fs(fs.get()));*/

  // If desired, run the HTTGenEventModule which will add some handily-
  // formatted generator-level info into the Event
  if (js["run_gen_info"].asBool()) {
    BuildModule(HTTGenEvent("HTTGenEvent")
        .set_genparticle_label(js["genTaus"].asString())
        .set_genjet_label(js["genJets"].asString()));
  }


  BuildModule(HTTFutureTauResolution("HTTFutureTauResolution")
    .set_genparticle_label("genParticles")
    .set_taus_label(js["taus"].asString())
    .set_fs(fs.get()));


  if (channel == channel::et) BuildETPairs();
  if (channel == channel::mt) BuildMTPairs();
  if (channel == channel::em) BuildEMPairs();
  if (channel == channel::tt) BuildTTPairs();


  // Pair DeltaR filtering
BuildModule(SimpleFilter<CompositeCandidate>("PairFilter")
      .set_input_label("ditau").set_min(1)
      .set_predicate([=](CompositeCandidate const* c) {
        return DeltaR(c->at(0)->vector(), c->at(1)->vector())
            > pair_dr;
      }));

  // Lepton Vetoes
  if (js["baseline"]["di_elec_veto"].asBool()) BuildDiElecVeto();
  if (js["baseline"]["di_muon_veto"].asBool()) BuildDiMuonVeto();
//  if (js["baseline"]["extra_elec_veto"].asBool()) BuildExtraElecVeto();
 // if (js["baseline"]["extra_muon_veto"].asBool()) BuildExtraMuonVeto();




  HTTPairSelector httPairSelector = HTTPairSelector("HTTPairSelector")
    .set_channel(channel)
    .set_fs(fs.get())
    .set_pair_label("ditau")
    .set_met_label("pfMET")
    .set_strategy(strategy_type)
    .set_mva_met_from_vector(mva_met_mode==1)
    .set_faked_tau_selector(faked_tau_selector)
    .set_hadronic_tau_selector(hadronic_tau_selector)
    .set_ztt_mode(ztautau_mode)
    .set_gen_taus_label("genParticles")
    .set_scale_met_for_tau(false)
    .set_tau_scale(tau_shift)
    .set_use_most_isolated((strategy_type != strategy::paper2013) && (!(channel == channel::zee || channel == channel::zmm || channel == channel::tpzmm || channel == channel::tpzee)))
    .set_use_os_preference((strategy_type == strategy::paper2013) || (channel == channel::zee || channel == channel::zmm || channel == channel::tpzmm || channel == channel::tpzee))
    .set_allowed_tau_modes(allowed_tau_modes);

  BuildModule(httPairSelector);




  BuildModule(HTTPairGenInfo("HTTPairGenInfo")
    .set_fs(fs.get())
    .set_write_plots(false)
    .set_ditau_label("ditau"));

  
 //BuildModule(CopyCollection<PFJet>("CopyFilteredJets",jets_label,jets_label+"UnFiltered"));

SimpleFilter<PFJet> jetIDFilter = SimpleFilter<PFJet>("JetIDFilter")
.set_input_label(jets_label);
if(strategy_type == strategy::paper2013) {
  jetIDFilter.set_predicate((bind(PFJetIDNoHFCut, _1)) && bind(PileupJetID, _1, pu_id_training, false));
} else if(strategy_type != strategy::mssmspring16 && strategy_type != strategy::smspring16){
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




  SVFitTest svFitTest  = SVFitTest("SVFitTest")
    .set_channel(channel)
    .set_strategy(strategy_type)
    .set_outname(svfit_override == "" ? output_name : svfit_override)
    .set_run_mode(new_svfit_mode)
    .set_fail_mode(0)
    .set_require_inputs_match(true)
    .set_split(700)
    .set_dilepton_label("ditau")
    .set_met_label(met_label)
    .set_fullpath(svfit_folder)
    .set_legacy_svfit(true)
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



if(channel != channel::wmnu) {
BuildModule(HTTFutureTrees("HTTFutureTrees")
    .set_fs(fs.get())
    .set_channel(channel)
    .set_strategy(strategy_type)
    .set_ditau_label("ditau")
    .set_met_label(met_label)
    .set_jets_label(jets_label));
 } 
}
}
// --------------------------------------------------------------------------
// TT Pair Sequence
// --------------------------------------------------------------------------
void HTTFutureSequence::BuildTTPairs(){
 
 BuildTauSelection();

 BuildModule(CompositeProducer<Tau, Tau>("TTPairProducer")
      .set_input_label_first("genmatched_taus")
      .set_input_label_second("genmatched_taus")
      /*.set_input_label_first(js["taus"].asString())
      .set_input_label_second(js["taus"].asString())*/
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));


 }

  

// --------------------------------------------------------------------------
// ET Pair Sequence
// --------------------------------------------------------------------------
void HTTFutureSequence::BuildETPairs() {
  ic::strategy strategy_type  = String2Strategy(strategy_str);

  BuildModule(CopyCollection<Electron>("CopyToSelectedElectrons",
      js["electrons"].asString(), "sel_electrons"));


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

  BuildTauSelection();

  BuildModule(CompositeProducer<Electron, Tau>("ETPairProducer")
      .set_input_label_first("sel_electrons")
      .set_input_label_second("genmatched_taus")
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));
}

// --------------------------------------------------------------------------
// MT Pair Sequence
// --------------------------------------------------------------------------
void HTTFutureSequence::BuildMTPairs() {
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
   } else if (strategy_type!=strategy::mssmspring16&&strategy_type!=strategy::smspring16){
    MuonID = [](Muon const* m) { return MuonMedium(m); };
   } else {
    MuonID = [](Muon const* m) {return MuonMediumHIPsafe(m); };
   }

  BuildModule(HTTGenMatchSelector<Muon>("HTTGenMatchSelectorMuon")
    .set_input_vec_label("sel_muons")
    .set_output_vec_label("genmatched_muons")
    .set_gen_match(mcorigin::tauMu));

  BuildModule(SimpleFilter<Muon>("MuonFilter")
      .set_input_label("genmatched_muons").set_min(1)
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > muon_pt    &&
                fabs(m->eta())          < muon_eta;//   &&
  //              fabs(m->dxy_vertex())   < muon_dxy   &&
 //               fabs(m->dz_vertex())    < muon_dz   &&
//                MuonPuppiIsoVal(m)      < 0.372 &&
//                MuonID(m);

      }));


  BuildTauSelection();

  BuildModule(CompositeProducer<Muon, Tau>("MTPairProducer")
      .set_input_label_first("genmatched_muons")
      .set_input_label_second("genmatched_taus")
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));
}

// --------------------------------------------------------------------------
// EM Pair Sequence
// --------------------------------------------------------------------------
void HTTFutureSequence::BuildEMPairs() {

 ic::strategy strategy_type  = String2Strategy(strategy_str);


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
  if(strategy_type==strategy::paper2013){
    if(special_mode == 21 || special_mode == 22){
     MuonID = [](Muon const* m) { return HttEMuFakeMuon(m); };
    } else if(special_mode ==25){
     MuonID = [](Muon const* m) {return m->pt()>=0; } ;
    } else { 
      MuonID = [](Muon const* m) { return MuonTight(m); };
    }
   } else if (strategy_type!=strategy::mssmspring16 && strategy_type!=strategy::smspring16){
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


  
  BuildModule(CompositeProducer<Electron, Muon>("EMPairProducer")
      .set_input_label_first("sel_electrons")
      .set_input_label_second("sel_muons")
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));


if(strategy_type == strategy::mssmspring16 || strategy_type == strategy::smspring16){
  BuildModule(SimpleFilter<CompositeCandidate>("EMPairFilter")
      .set_input_label("ditau").set_min(1)
      .set_predicate([=](CompositeCandidate const* c) {
        return PairOneWithPt(c, 24.0);
      }));
  }


}


// --------------------------------------------------------------------------
// Tau Selection Sequence
// --------------------------------------------------------------------------
void HTTFutureSequence::BuildTauSelection(){
  Json::Value base = js["baseline"];

  BuildModule(HTTGenMatchSelector<Tau>("HTTGenMatchSelector")
      .set_input_vec_label(js["taus"].asString())
      .set_output_vec_label("genmatched_taus")
      .set_gen_match(mcorigin::tauHad));

  BuildModule(SimpleFilter<Tau>("TauFilter")
      .set_input_label("genmatched_taus").set_min(min_taus)
//      .set_input_label(js["taus"].asString()).set_min(min_taus)
      .set_predicate([=](Tau const* t) {
        return  t->pt()                     >  tau_pt     &&
                fabs(t->eta())              <  tau_eta    &&
                fabs(t->lead_dz_vertex())   <  tau_dz     &&
                fabs(t->charge())           == 1          &&
                //t->GetTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") >0.5 &&
                //t->GetTauID("chargedIsoPtSum") < 2 &&
                t->GetTauID("decayModeFinding") > 0.5;

      }));

}


void HTTFutureSequence::BuildDiElecVeto() {
  ic::strategy strategy_type  = String2Strategy(strategy_str);

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

 void HTTFutureSequence::BuildDiMuonVeto() {
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
	if(strategy_type==strategy::phys14 || strategy_type==strategy::spring15 ||strategy_type==strategy::fall15||strategy_type==strategy::mssmspring16||strategy_type==strategy::smspring16) {
	  vetoMuonPairFilter.set_veto_name("dimuon_veto");
		vetoMuonPairFilter.set_no_filter(true);
	}
	
	BuildModule(vetoMuonPairFilter);

}

void HTTFutureSequence::BuildExtraElecVeto(){
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
                ElectronHTTIdSpring15(e, true)             &&
                //PF04IsolationVal(e, 0.5,0) < 0.3;
                PF03IsolationVal(e, 0.5,0) < 0.3;
      });
  }


BuildModule(extraElecFilter);
 }


void HTTFutureSequence::BuildExtraMuonVeto(){
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
   }

  BuildModule(extraMuonFilter);

 }

}
