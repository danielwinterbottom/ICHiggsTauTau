#include <iostream>
#include <vector>
#include <string>
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/format.hpp"
#include "TSystem.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/AnalysisBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/CopyCollection.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/SimpleFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/OverlapFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/IDOverlapFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/CompositeProducer.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/OneCollCompositeProducer.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/PileupWeight.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttPairSelector.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttWeights.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttSelection.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttMetStudy.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/QuarkGluonDiscriminatorStudy.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttRecoilCorrector.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttSync.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttPrint.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MakeRunStats.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/EnergyShifter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/SVFit.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/JetEnergyCorrections.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/LumiMask.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HTTCategories.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HTTTriggerFilter.h"

using boost::lexical_cast;
using boost::bind;
namespace po = boost::program_options;
using std::string;
using std::vector;
using namespace ic;

int main(int argc, char* argv[]){

  // Configurable parameters
  string cfg;                     // The configuration file
  int max_events;                 // Maximum number of events to process
  string filelist;                // The file containing a list of files to use as input
  string input_prefix;            // A prefix that will be added to the path of each input file
  string output_name;             // Name of the ouput ROOT File
  string output_folder;           // Folder to write the output in
  bool do_skim;                   // For making skimmed ntuples
  string skim_path = "";          // Local folder where skimmed ntuples should be written

  string strategy_str;            // Analysis strategy
  string era_str;                 // Analysis data-taking era
  string mc_str;                  // Analysis MC production
  string channel_str;             // Analysis channel

  bool is_data;                   // true = data, false = mc         
  bool is_embedded;               // true = embedded, false = not an embedded sample
  unsigned special_mode;          // 0 = normal processing, > 0 (see below)
  unsigned tau_scale_mode;        // 0 = no shift, 1 = shift down, 2 = shift up
  unsigned svfit_mode;            // 0 = not run, 1 = generate jobs, 2 = read-in job output
  string svfit_folder;            // Folder containing svfit jobs & output
  string svfit_override;          // Override the svfit results to use
  unsigned ztautau_mode;          // 0 = not run, 1 = select Z->tautau, 2 = select Z->ee and Z->mumu
  unsigned faked_tau_selector;    // 0 = not run, 1 = tau matched to gen. lepton, 2 = tau not matched to lepton
  unsigned mva_met_mode;          // 0 = standard mva met, 1 = mva met from vector (only when mva met is being used)
  bool make_sync_ntuple;          // Generate a sync ntuple
  bool quark_gluon_study;         // Run study on quark-gluon jet discriminators

  bool do_vbf_mva = true;
  // bool disable_mc_trigger = false;
  // bool disable_reweighting = false;
  // bool do_btag_weight = false;

  // Special Mode 2  Relaxed Selection Trigger   Weights 
  // Special Mode 3  QCD Selection Trigger Weights 
  // Special Mode 6  QCD Selection Trigger Weights Lower vbf pt to 20 GeV, lower vbf mva to 0.0
  // Special Mode 7  Full Selection  Trigger Weights mT sideband: 60-120 GeV
  // Special Mode 8  Full Selection  Trigger Weights twojet requires pt 20 GeV jets, eta < 4.7
  // Special Mode 9  Full Selection  Trigger Weights npt20jets >= 1, npt30jets <= 1
  // Speical Mode 10 Relaxed Selection Trigger Weights npt20jets >= 1, npt30jets <= 1
  // Special Mode 18 - relax tau e rejection for ZL shape

  /* Skims/notes needed for em channel
  // Speical Mode 20 Fake Electron for emu
  // Speical Mode 21 Fake Muon for emu 
  // Speical Mode 22 Fake Electron & Muon for emu
  // Special Mode 23 No isolation on the electron leg
  // Special Mode 24 Inverted isolation of the electron leg
  // Special Mode 25 No ID or Iso or muon or electron leg (for skimming only)
    - Need to be able to shift the mass (m_sv and m_vis) in the embedded samples up by
      1% - this could be done easily in HTTCategories
    - Need a category without central jet veto for diboson/single top/ttbar shapes
    - For the fake rate method: one skim with just electron fakes, one with just muon fakes, and one with both
    - Need a skim with full relaxed electron isolation for SS shape for 0jet low, 1jet low and 0jet high
  */

  // Load the config
  po::options_description preconfig("Pre-Configuration");
  preconfig.add_options()("cfg", po::value<std::string>(&cfg)->required());
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
      ("max_events",          po::value<int>(&max_events)-> default_value(-1))
      ("filelist",            po::value<string>(&filelist)->required())
      ("input_prefix",        po::value<string>(&input_prefix)->default_value(""))
      ("output_name",         po::value<string>(&output_name)->required())
      ("output_folder",       po::value<string>(&output_folder)->default_value(""))
      ("do_skim",             po::value<bool>(&do_skim)->default_value(false))
      ("skim_path",           po::value<string>(&skim_path)->default_value(""))
      ("strategy",            po::value<string>(&strategy_str)->required())
      ("era",                 po::value<string>(&era_str)->required())
      ("mc",                  po::value<string>(&mc_str)->required())
      ("channel",             po::value<string>(&channel_str)->required())
      ("is_data",             po::value<bool>(&is_data)->required())
      ("is_embedded",         po::value<bool>(&is_embedded)->default_value(false))
      ("special_mode",        po::value<unsigned>(&special_mode)->default_value(0))
      ("tau_scale_mode",      po::value<unsigned>(&tau_scale_mode)->default_value(0))
      ("svfit_mode",          po::value<unsigned>(&svfit_mode)->default_value(0))
      ("svfit_folder",        po::value<string>(&svfit_folder)->default_value(""))
      ("svfit_override",      po::value<string>(&svfit_override)->default_value(""))
      ("ztautau_mode",        po::value<unsigned>(&ztautau_mode)->default_value(0))
      ("faked_tau_selector",  po::value<unsigned>(&faked_tau_selector)->default_value(0))
      ("mva_met_mode",        po::value<unsigned>(&mva_met_mode)->default_value(1))
      ("quark_gluon_study",   po::value<bool>(&quark_gluon_study)->default_value(false))
      ("make_sync_ntuple",    po::value<bool>(&make_sync_ntuple)->default_value(false));
      // ("do_vbf_mva", po::value<bool>(&do_vbf_mva)->default_value(true), "0=disabled, 1 = enabled")
      // ("scan_trigger", po::value<bool>(&scan_trigger)->default_value(false), "true/false")
      // ("disable_mc_trigger", po::value<bool>(&disable_mc_trigger)->default_value(false), "true/false")
      // ("disable_reweighting", po::value<bool>(&disable_reweighting)->default_value(false), "true/false")
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
  po::notify(vm);

  // do_btag_weight = true;
  // if (do_skim) disable_reweighting = true;

  // Some options must now be re-configured based on other options
  ic::strategy strategy = String2Strategy(strategy_str);
  ic::era era           = String2Era(era_str);
  ic::mc mc             = String2MC(mc_str);
  ic::channel channel   = String2Channel(channel_str);
  if (special_mode > 0) output_name = "Special_"+lexical_cast<string>(special_mode)+"_" + output_name;
  if (tau_scale_mode == 1) {
    output_folder += "TSCALE_DOWN/";
    svfit_folder += "TSCALE_DOWN/";
  }
  if (tau_scale_mode == 2) {
    output_folder += "TSCALE_UP/";
    svfit_folder += "TSCALE_UP/";
  }

  std::cout << "**** HiggsTauTau Analysis *****" << std::endl;
  string param_fmt = "%-25s %-40s\n";
  std::cout << boost::format(param_fmt) % "max_events" % max_events;
  std::cout << boost::format(param_fmt) % "output" % (output_folder+output_name);
  std::cout << boost::format(param_fmt) % "do_skim" % do_skim;
  if (do_skim) std::cout << boost::format(param_fmt) % "skim_path" % skim_path;
  std::cout << boost::format(param_fmt) % "strategy" % strategy_str;
  std::cout << boost::format(param_fmt) % "era" % era_str;
  std::cout << boost::format(param_fmt) % "mc" % mc_str;
  std::cout << boost::format(param_fmt) % "channel" % channel_str;
  std::cout << boost::format(param_fmt) % "is_data" % is_data;
  std::cout << boost::format(param_fmt) % "is_embedded" % is_embedded;
  std::cout << boost::format(param_fmt) % "special_mode" % special_mode;
  std::cout << boost::format(param_fmt) % "tau_scale_mode" % tau_scale_mode;
  std::cout << boost::format(param_fmt) % "svfit_mode" % svfit_mode;
  if (svfit_mode > 0) {
    std::cout << boost::format(param_fmt) % "svfit_folder" % svfit_folder;
    std::cout << boost::format(param_fmt) % "svfit_override" % svfit_override;
  }
  std::cout << boost::format(param_fmt) % "ztautau_mode" % ztautau_mode;
  std::cout << boost::format(param_fmt) % "faked_tau_selector" % faked_tau_selector;
  std::cout << boost::format(param_fmt) % "mva_met_mode" % mva_met_mode;
  std::cout << boost::format(param_fmt) % "make_sync_ntuple" % make_sync_ntuple;

  // Load necessary libraries for ROOT I/O of custom classes
  gSystem->Load("libFWCoreFWLite.dylib");
  gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  AutoLibraryLoader::enable();

  // Build a vector of input files
  vector<string> files = ParseFileLines(filelist);
  for (unsigned i = 0; i < files.size(); ++i) files[i] = input_prefix + files[i];
  
  // Create ROOT output fileservice
  fwlite::TFileService *fs = new fwlite::TFileService((output_folder+output_name).c_str());
  
  // Setup Lepton kinematic cuts
  std::string lep1_label, lep2_label, lep1sel_label, lep2sel_label, met_label;
  double elec_dz, muon_dz, elec_dxy, muon_dxy, tau_dz, mt_max_selection;
  double elec_pt, elec_eta, muon_pt, muon_eta, tau_pt, tau_eta;

  if (strategy == strategy::ichep2012) {
    met_label = "pfMet";
    mt_max_selection = 40.0;
    do_vbf_mva = true;
  }
  if (strategy == strategy::hcp2012) {
    met_label = "pfMVAMet";
    mt_max_selection = 20.0;
    do_vbf_mva = false;

  }
  if (strategy == strategy::moriond2013) {
    met_label = "pfMVAMet";
    mt_max_selection = 20.0;
    do_vbf_mva = false;

  }

  if (channel == channel::et) {
    lep1_label = "electrons";
    lep1sel_label = "selElectrons";
    lep2_label = "taus";
    lep2sel_label = "taus";
    elec_dz = 0.2;
    elec_dxy = 0.045;
    muon_dz = 0.2;
    muon_dxy = 0.045;
    tau_dz = 0.2;
    if (era == era::data_2011) {
      elec_pt = 20.0;
      elec_eta = 2.1;
      muon_pt = 17.0;
      muon_eta = 2.1;
    } else {
      elec_pt = 24.0;
      elec_eta = 2.1;
      muon_pt = 20.0;
      muon_eta = 2.1;
    }
    tau_pt = 20.0;
    tau_eta = 2.3;
  }

  if (channel == channel::mt || channel == channel::mtmet) {
    lep1_label = "muonsPFlow";
    lep1sel_label = "selMuons";
    lep2_label = "taus";
    lep2sel_label = "taus";
    elec_dz = 0.2;
    elec_dxy = 0.045;
    muon_dz = 0.2;
    muon_dxy = 0.045;
    tau_dz = 0.2;
    if (era == era::data_2011) {
      elec_pt = 20.0;
      elec_eta = 2.1;
      muon_pt = 17.0;
      muon_eta = 2.1;
    } else {
      elec_pt = 24.0;
      elec_eta = 2.1;
      muon_pt = 20.0;
      muon_eta = 2.1;
    }
    tau_pt = 20.0;
    tau_eta = 2.3;
    if (channel == channel::mtmet) muon_pt = 9.0;
  }

  if (channel == channel::em) {
    lep1_label = "electrons";
    lep1sel_label = "selElectrons";
    lep2_label = "muonsPFlow";
    lep2sel_label = "selMuons";
    elec_dz = 0.1;
    elec_dxy = 0.02;
    muon_dz = 0.1;
    muon_dxy = 0.02;
    tau_dz = 0.2;
    elec_pt = 10.0;
    elec_eta = 2.3;
    muon_pt = 10.0;
    muon_eta = 2.1;
    tau_pt = 20.0;
    tau_eta = 2.3;
    if (special_mode == 20 || special_mode == 22) elec_dxy = 0.2;     
    if (special_mode == 21 || special_mode == 22) muon_dxy = 0.2;   
  }


  // Lower pt thresholds on electrons and taus when skimming, 
  // to allow for energy scale shifts later
  if (do_skim) {
    tau_pt = 19.0;
   if (channel == channel::em) elec_pt = 9.5;
  }

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
  std::cout << boost::format(param_fmt) % "tau_dz" % tau_dz;
  std::cout << boost::format(param_fmt) % "mt_max_selection" % mt_max_selection;

  // Create analysis object
  ic::AnalysisBase analysis(
    "HiggsTauTau",        // Analysis name
    files,                // Input files
    "icEventProducer",    // TTree path
    "EventTree",          // TTree name
    max_events);          // Max. events to process (-1 = all)
  if (do_skim && skim_path != "") analysis.DoSkimming(skim_path);

  // ------------------------------------------------------------------------------------
  // Misc Modules
  // ------------------------------------------------------------------------------------

  string data_json;
  if (era == era::data_2011) data_json           =  "data/json/json_data_2011_et_mt.txt";
  if (era == era::data_2012_ichep) data_json     =  "data/json/data_2012_ichep.txt";
  if (era == era::data_2012_hcp) data_json       =  "data/json/data_2012_hcp.txt";
  if (era == era::data_2012_moriond) data_json   =  "data/json/data_2012_moriond.txt";
  LumiMask lumiMask = LumiMask("LumiMask")
    .set_produce_output_jsons("")
    .set_input_file(data_json);

  MakeRunStats runStats = MakeRunStats("RunStats")
    .set_output_name(output_folder+output_name+".runstats");

  HttPrint httPrint("HttPrint");

  string mc_pu_file;
  if (mc == mc::fall11_42X) mc_pu_file    = "data/pileup/MC_Fall11_PU_S6-500bins.root";
  if (mc == mc::summer12_53X) mc_pu_file  = "data/pileup/MC_Summer12_PU_S10-600bins.root";
  string data_pu_file;
  if (era == era::data_2011) data_pu_file     =  "data/pileup/Data_Pileup_2011_HCP-500bins.root";
  if (era == era::data_2012_ichep) data_pu_file     =  "data/pileup/Data_Pileup_2012.root";
  if (era == era::data_2012_hcp) data_pu_file       =  "data/pileup/Data_Pileup_2012_HCP-600bins.root";
  if (era == era::data_2012_moriond) data_pu_file   =  "data/pileup/Data_Pileup_2012_Moriond-600bins.root";
  if (era == era::data_2012_moriond && channel == channel::mtmet) data_pu_file   =  "data/pileup/Data_Pileup_2012D-600bins.root";

  TH1D data_pu  = GetFromTFile<TH1D>(data_pu_file, "/", "pileup");
  TH1D mc_pu    = GetFromTFile<TH1D>(mc_pu_file, "/", "pileup");
  if (!is_data) {
    std::cout << "** Pileup Files **" << std::endl;
    std::cout << boost::format(param_fmt) % "mc_pu_file" % mc_pu_file;
    std::cout << boost::format(param_fmt) % "data_pu_file" % data_pu_file;
  }
  PileupWeight pileupWeight = PileupWeight("PileupWeight")
    .set_data(&data_pu)
    .set_mc(&mc_pu)
    .set_print_weights(false);

  HTTTriggerFilter httTriggerFilter = HTTTriggerFilter("HTTTriggerFilter")
    .set_channel(channel)
    .set_mc(mc)
    .set_is_data(is_data)
    .set_pair_label("emtauCandidates");

  SimpleCounter<GenParticle> zTauTauFilter = SimpleCounter<GenParticle>("ZToTauTauSelector")
    .set_input_label("genParticles")
    .set_predicate(
      (bind(&GenParticle::status, _1) == 3) && 
      (bind(abs,(bind(&GenParticle::pdgid, _1))) == 15))
    .set_min(2);
  if (ztautau_mode == 2) zTauTauFilter.set_min(0).set_max(0);

  double mssm_mass = 0.0;
  bool do_mass_filter = false;
  if ((output_name.find("SUSYGluGluToHToTauTau") != output_name.npos) || (output_name.find("SUSYBBHToTauTau") != output_name.npos)) {
    std::size_t pos = output_name.find("_M-");
    if (pos != output_name.npos) {
      std::string mass_string;
      for (unsigned i = (pos+3); i < output_name.size(); ++i) {
        if (output_name.at(i) != '_') mass_string += output_name.at(i);
        if (output_name.at(i) == '_') break;
      }
      mssm_mass = boost::lexical_cast<double>(mass_string);
      do_mass_filter = true;
      std::cout << "** MSSM Signal Sample **" << std::endl;
      std::cout << boost::format(param_fmt) % "mssm_mass" % mssm_mass;
    }
  }
  SimpleCounter<GenParticle> mssmMassFilter = SimpleCounter<GenParticle>("MSSMMassFilter")
    .set_input_label("genParticles")
    .set_predicate(bind(GenParticleInMassBand, _1, 36, 0.7*mssm_mass, 1.3*mssm_mass))
    .set_min(1);

  SimpleCounter<GenParticle> embeddedMassFilter = SimpleCounter<GenParticle>("EmbeddedMassFilter")
    .set_input_label("genParticlesEmbedded")
    .set_predicate(bind(GenParticleInMassBand, _1, 23, 50., 9999999.))
    .set_min(1);

  // JetEnergyCorrections<PFJet> jetEnergyCorrections = JetEnergyCorrections<PFJet>
  // ("JetEnergyCorrections")
  // .set_input_label("pfJetsPFlow")
  // .set_is_data(is_data)
  // .set_l1_file("data/jec/START53_V10_L1FastJet_AK5PF.txt")
  // .set_l2_file("data/jec/START53_V10_L2Relative_AK5PF.txt")
  // .set_l3_file("data/jec/START53_V10_L3Absolute_AK5PF.txt");
  
  // ------------------------------------------------------------------------------------
  // Electron Modules
  // ------------------------------------------------------------------------------------
  CopyCollection<Electron>  
    selElectronCopyCollection("CopyToSelElectrons","electrons","selElectrons");

  boost::function<bool (Electron const*)> elec_idiso_func;
  if (special_mode == 20 || special_mode == 22) {
    elec_idiso_func = bind(HttEMuFakeElectron, _1);
  } else if (special_mode == 2 || special_mode == 5 || special_mode == 12) {
    elec_idiso_func = bind(ElectronHTTId, _1, (channel == channel::em)) && (bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.5);
  } else if (special_mode == 3 || special_mode == 6 || special_mode == 10) {
    elec_idiso_func = bind(ElectronHTTId, _1, (channel == channel::em)) && (bind(PF04IsolationVal<Electron>, _1, 0.5) > 0.2) && (bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.5);
  } else if (special_mode == 23) {
    elec_idiso_func = bind(ElectronHTTId, _1, (channel == channel::em));
  } else if (special_mode == 24) {
    elec_idiso_func = bind(ElectronHTTId, _1, (channel == channel::em)) && !bind(PF04IsolationEBElec, _1, 0.5, 0.15, 0.1);
  } else if (special_mode == 25) {
    elec_idiso_func = (bind(PF04IsolationVal<Electron>, _1, 0.5) >= 0.0); // Dummy function, will always pass
  } else {
    if (channel == channel::em) {
      elec_idiso_func = bind(ElectronHTTId, _1, (channel == channel::em)) && bind(PF04IsolationEBElec, _1, 0.5, 0.15, 0.1);
    } else {
      elec_idiso_func = bind(ElectronHTTId, _1, (channel == channel::em)) && (bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1);
    }
  }
  SimpleFilter<Electron> selElectronFilter = SimpleFilter<Electron>("SelElectronFilter")
    .set_input_label("selElectrons")
    .set_predicate(
      bind(MinPtMaxEta, _1, elec_pt, elec_eta) &&
      bind(fabs, bind(&Electron::dxy_vertex, _1)) < elec_dxy &&
      bind(fabs, bind(&Electron::dz_vertex, _1)) < elec_dz &&
      bind(elec_idiso_func, _1))
    .set_min(1);
  
  // Electron Veto
  CopyCollection<Electron>  vetoElectronCopyCollection("CopyToVetoElectrons","electrons","vetoElectrons");

  SimpleFilter<Electron> vetoElectronFilter = SimpleFilter<Electron>("VetoElectronFilter")
    .set_input_label("vetoElectrons")
    .set_predicate(
      bind(MinPtMaxEta, _1, 15.0, 2.5) &&
      bind(fabs, bind(&Electron::dxy_vertex, _1)) < elec_dxy &&
      bind(fabs, bind(&Electron::dz_vertex, _1)) < elec_dz &&
      bind(Electron2011WP95ID, _1) &&
      bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.3);

  OneCollCompositeProducer<Electron> vetoElectronPairProducer = OneCollCompositeProducer<Electron>("VetoPairProducer")
    .set_input_label("vetoElectrons")
    .set_candidate_name_first("elec1")
    .set_candidate_name_second("elec2")
    .set_output_label("vetoPairs");

  SimpleFilter<CompositeCandidate> vetoElectronPairFilter = SimpleFilter<CompositeCandidate>("VetoPairFilter")
    .set_input_label("vetoPairs")
    .set_predicate( (bind(&CompositeCandidate::DeltaR, _1, "elec1","elec2") > 0.15) && (bind(&CompositeCandidate::charge, _1) == 0) )
    .set_min(0)
    .set_max(0);

    SimpleCounter<Electron> extraElectronVeto = SimpleCounter<Electron>("ExtraElectronVeto")
    .set_input_label("electrons")
    .set_predicate(bind(MinPtMaxEta, _1, 10.0, 2.5) 
                && bind(ElectronHTTId, _1, true)
                && bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.3
                && bind(fabs, bind(&Electron::dxy_vertex, _1)) < elec_dxy
                && bind(fabs, bind(&Electron::dz_vertex, _1)) < elec_dz)
    .set_min(0).set_max((channel == channel::et || channel == channel::em) ? 1 : 0);

  // ------------------------------------------------------------------------------------
  // Muon Modules
  // ------------------------------------------------------------------------------------
  CopyCollection<Muon> selMuonCopyCollection("CopyToSelMuons","muonsPFlow","selMuons");

  boost::function<bool (Muon const*)> muon_idiso_func;
  if (special_mode == 21 || special_mode == 22) {
    muon_idiso_func = bind(HttEMuFakeMuon, _1);
  } else if (special_mode == 2 || special_mode == 5 || special_mode == 12) {
    muon_idiso_func = bind(MuonTight, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.5);
  } else if (special_mode == 3 || special_mode == 6 || special_mode == 10) {
    muon_idiso_func = bind(MuonTight, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5) > 0.2) && (bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.5);
  } else if (special_mode == 25) {
    muon_idiso_func = (bind(PF04IsolationVal<Muon>, _1, 0.5) >= 0.0);
  } else {
    if (channel == channel::em) {
      muon_idiso_func = bind(MuonTight, _1) && bind(PF04IsolationEB<Muon>, _1, 0.5, 0.15, 0.1);
    } else {
      muon_idiso_func = bind(MuonTight, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1);
    }
  }
  SimpleFilter<Muon> selMuonFilter = SimpleFilter<Muon>("SelMuonFilter")
    .set_input_label("selMuons")
    .set_predicate(
      bind(MinPtMaxEta, _1, muon_pt, muon_eta) && 
      bind(fabs, bind(&Muon::dxy_vertex, _1)) < muon_dxy &&
      bind(fabs, bind(&Muon::dz_vertex, _1)) < muon_dz &&
      bind(muon_idiso_func, _1))
    .set_min(1);

   // Muon Veto
  CopyCollection<Muon> vetoMuonCopyCollection("CopyToVetoMuons","muonsPFlow","vetoMuons");

  SimpleFilter<Muon> vetoMuonFilter = SimpleFilter<Muon>("VetoMuonFilter")
    .set_input_label("vetoMuons")
    .set_predicate(
      bind(MinPtMaxEta, _1, 15.0, 2.4) &&
      bind(fabs, bind(&Muon::dxy_vertex, _1)) < muon_dxy && 
      bind(fabs, bind(&Muon::dz_vertex, _1)) < muon_dz &&
      bind(&Muon::is_global, _1) &&
      bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.3);

  OneCollCompositeProducer<Muon> vetoMuonPairProducer = OneCollCompositeProducer<Muon>("VetoPairProducer")
    .set_input_label("vetoMuons")
    .set_candidate_name_first("muon1")
    .set_candidate_name_second("muon2")
    .set_output_label("vetoPairs");

  SimpleFilter<CompositeCandidate> vetoMuonPairFilter = SimpleFilter<CompositeCandidate>("VetoPairFilter")
    .set_predicate( (bind(&CompositeCandidate::DeltaR, _1, "muon1","muon2") > 0.15) && (bind(&CompositeCandidate::charge, _1) == 0) )
    .set_input_label("vetoPairs")
    .set_min(0)
    .set_max(0);       

  OverlapFilter<Electron, Muon> elecMuonOverlapFilter = OverlapFilter<Electron, Muon>("ElecMuonOverlapFilter")
    .set_input_label("selElectrons")
    .set_reference_label("muonsPFlow")
    .set_min_dr(0.3); 

  SimpleCounter<Muon> extraMuonVeto = SimpleCounter<Muon>("ExtraMuonVeto")
  .set_input_label("muonsPFlow")
  .set_predicate(bind(MinPtMaxEta, _1, 10.0, 2.4) 
              && bind(MuonTight, _1) 
              && bind(PF04Isolation<Muon>, _1, 0.5, 0.3)
              && (bind(fabs, bind(&Muon::dxy_vertex, _1)) < muon_dxy)
              && (bind(fabs, bind(&Muon::dz_vertex, _1)) < muon_dz))
  .set_min(0).set_max((channel == channel::mt || channel == channel::mtmet || channel == channel::em) ? 1 : 0);                   

  // ------------------------------------------------------------------------------------
  // Tau Modules
  // ------------------------------------------------------------------------------------
  double tau_shift = 1.0;
  if (tau_scale_mode == 1) tau_shift = 0.97;
  if (tau_scale_mode == 2) tau_shift = 1.03;
  EnergyShifter<Tau> tauEnergyShifter = EnergyShifter<Tau>
  ("TauEnergyShifter")
    .set_input_label("taus")
    .set_shift(tau_shift);

  SimpleFilter<Tau> tauPtEtaFilter = SimpleFilter<Tau>("TauPtEtaFilter")
    .set_input_label("taus")
    .set_predicate(bind(MinPtMaxEta, _1, tau_pt, tau_eta))
    .set_min(1);

  SimpleFilter<Tau> tauDzFilter = SimpleFilter<Tau>("TauDzFilter")
    .set_input_label("taus")
    .set_predicate(bind(fabs, bind(&Tau::lead_dz_vertex, _1)) < tau_dz)
    .set_min(1);

  SimpleFilter<Tau> tauIsoFilter = SimpleFilter<Tau>("TauIsoFilter")
    .set_input_label("taus")
    .set_predicate((bind(&Tau::GetTauID, _1, "byLooseIsolationMVA") > 0.5) && (bind(&Tau::GetTauID, _1, "decayModeFinding") > 0.5))
    .set_min(1);
    if (special_mode == 1) {
      tauIsoFilter.set_predicate( (bind(&Tau::GetTauID, _1, "decayModeFinding") > 0.5));
    } else if (special_mode == 2 || special_mode == 3 || special_mode == 5 || special_mode == 6 || special_mode == 10 || special_mode == 12 || special_mode == 16) {
      tauIsoFilter.set_predicate( (bind(&Tau::GetTauID, _1, "byIsolationMVAraw") > 0.7) && (bind(&Tau::GetTauID, _1, "decayModeFinding") > 0.5));
    } else if (special_mode == 4) {
      tauIsoFilter.set_predicate( (bind(&Tau::GetTauID, _1, "decayModeFinding") > 0.5));
    }

  SimpleFilter<Tau> tauElRejectFilter = SimpleFilter<Tau>("TauElRejectFilter")
    .set_predicate(bind(&Tau::GetTauID, _1, "againstElectronLoose") > 0.5)
    .set_input_label("taus").set_min(1); 
  if (channel == channel::et) tauElRejectFilter
    .set_predicate( bind(&Tau::GetTauID, _1, "againstElectronMVA") > 0.5);                        
  if (channel == channel::et && !do_skim && special_mode != 18) tauElRejectFilter
    .set_predicate( (bind(&Tau::GetTauID, _1, "againstElectronTightMVA2") > 0.5) && (bind(&Tau::GetTauID, _1, "againstElectronMVA") > 0.5) );                        

  SimpleFilter<Tau> tauMuRejectFilter = SimpleFilter<Tau>("TauMuRejectFilter")
    .set_predicate(bind(&Tau::GetTauID, _1, "againstMuonTight") > 0.5)
    .set_input_label("taus").set_min(1);  
  if (channel == channel::et) tauMuRejectFilter
    .set_predicate( bind(&Tau::GetTauID, _1, "againstMuonLoose") > 0.5);                        

  // ------------------------------------------------------------------------------------
  // e/m + tau Pair Modules
  // ------------------------------------------------------------------------------------  
  CompositeProducer<Electron, Tau> tauElPairProducer = CompositeProducer<Electron, Tau>
    ("TauElPairProducer")
    .set_input_label_first("selElectrons")
    .set_input_label_second("taus")
    .set_candidate_name_first("lepton1")
    .set_candidate_name_second("lepton2")
    .set_output_label("emtauCandidates"); 

  CompositeProducer<Muon, Tau> tauMuPairProducer = CompositeProducer<Muon, Tau>
    ("TauMuPairProducer")
    .set_input_label_first("selMuons")
    .set_input_label_second("taus")
    .set_candidate_name_first("lepton1")
    .set_candidate_name_second("lepton2")
    .set_output_label("emtauCandidates");   

  CompositeProducer<Electron, Muon> elMuPairProducer = CompositeProducer<Electron, Muon>
    ("ElecMuPairProducer")
    .set_input_label_first("selElectrons")
    .set_input_label_second("selMuons")
    .set_candidate_name_first("lepton1")
    .set_candidate_name_second("lepton2")
    .set_output_label("emtauCandidates");                                                        

  SimpleFilter<CompositeCandidate> pairFilter = SimpleFilter<CompositeCandidate>("PairFilter")
    .set_input_label("emtauCandidates")
    .set_predicate( (bind(&CompositeCandidate::DeltaR, _1, "lepton1","lepton2") > 0.5))
    .set_min(1)
    .set_max(999);    
  if (channel == channel::em) pairFilter
    .set_predicate( (bind(PairOneWithPt, _1, 20.0)) && (bind(&CompositeCandidate::DeltaR, _1, "lepton1","lepton2") > 0.3));

  if (channel == channel::mtmet) pairFilter
    .set_predicate( (bind(&CompositeCandidate::DeltaR, _1, "lepton1","lepton2") > 0.5) && (bind(&CompositeCandidate::PtOf, _1, "lepton1") <= 20.0));

  // ------------------------------------------------------------------------------------
  // Jet Modules
  // ------------------------------------------------------------------------------------  
  OverlapFilter<PFJet, CompositeCandidate> jetLeptonOverlapFilter = OverlapFilter<PFJet, CompositeCandidate>
    ("JetLeptonOverlapFilter")
    .set_input_label("pfJetsPFlow")
    .set_reference_label("emtauCandidates")
    .set_min_dr(0.5);

  SimpleFilter<PFJet> jetIDFilter = SimpleFilter<PFJet>
    ("JetIDFilter")
    .set_input_label("pfJetsPFlow")
    .set_predicate((bind(PFJetID, _1)) && bind(&PFJet::pu_id_mva_loose, _1));

   

  // ------------------------------------------------------------------------------------
  // Pair & Selection Modules
  // ------------------------------------------------------------------------------------  
  HttPairSelector httPairSelector = HttPairSelector("HttPairSelector")
    .set_channel(channel)
    .set_fs(fs)
    .set_met_label(met_label)
    .set_mva_met_from_vector(mva_met_mode == 1)
    .set_faked_tau_selector(faked_tau_selector)
    .set_scale_met_for_tau(tau_scale_mode > 0)
    .set_tau_scale(tau_shift);

  HttRecoilCorrector httRecoilCorrector = HttRecoilCorrector("HttRecoilCorrector")
    .set_sample(output_name)
    .set_channel(channel)
    .set_mc(mc)
    .set_met_label(met_label)
    .set_strategy(strategy);

  HttWeights httWeights = HttWeights("HttWeights")
    .set_channel(channel)
    .set_era(era)
    .set_mc(mc)
    .set_trg_applied_in_mc(true)
    .set_do_trg_weights(false)
    .set_do_etau_fakerate(false)
    .set_do_idiso_weights(false)
    .set_do_emu_e_fakerates(false)
    .set_do_emu_m_fakerates(false)
    .set_do_top_factors(false)
    .set_do_btag_weight(false);
  if (!is_data) {
    httWeights.set_do_trg_weights(true).set_trg_applied_in_mc(true).set_do_idiso_weights(true);
    httWeights.set_do_btag_weight(true);
  }
  if (output_name.find("DYJetsToLL") != output_name.npos && channel == channel::et) httWeights.set_do_etau_fakerate(true);
  if (is_embedded) httWeights.set_do_trg_weights(true).set_trg_applied_in_mc(false).set_do_idiso_weights(false);
  if (special_mode == 20 || special_mode == 22) httWeights.set_do_emu_e_fakerates(true);
  if (special_mode == 21 || special_mode == 22) httWeights.set_do_emu_m_fakerates(true);
  //if (outname.find("TTJets") != outname.npos && mode == 2 && era == 0) httWeights.set_do_top_factors(true);
  if (output_name.find("WJetsToLNuSoup") != output_name.npos) {
    httWeights.set_do_w_soup(true);
    if (mc == mc::fall11_42X) {
      httWeights.SetWTargetFractions(0.752332, 0.171539, 0.0538005, 0.0159036, 0.00642444);
      httWeights.SetWInputYields(81295381.0, 70712575.0, 25320546.0, 7541595.0, 12973738.0);
    }
    if (mc == mc::summer12_53X) {
      httWeights.SetWTargetFractions(0.743925, 0.175999, 0.0562617, 0.0168926, 0.00692218);
      httWeights.SetWInputYields(76102995.0, 23101598.0, 33884921.0, 15539503.0, 13382803.0);
    }
  }
  if (mc == mc::fall11_42X && output_name.find("GluGluToHToTauTau_M-") != output_name.npos && output_name.find("SUSYGluGluToHToTauTau_M-") == output_name.npos) {
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


  HttSelection httSelection = HttSelection("HttSelection")
    .set_fs(fs)
    .set_channel(channel)
    .set_met_label(met_label);
  httSelection.set_mt_max_selection(mt_max_selection);
  if (special_mode == 5 || special_mode == 12) httSelection.set_distinguish_os(false);
  if (special_mode == 7) httSelection.set_mt_min_control(60.0).set_mt_max_control(120.0);

  QuarkGluonDiscriminatorStudy quarkGluonDiscriminatorStudy = QuarkGluonDiscriminatorStudy
    ("QuarkGluonDiscriminatorStudy")
  .set_fs(fs);  
  
  // HttMetStudy httMetStudy = HttMetStudy
  //   ("HttMetStudy")
  //   .set_fs(fs)
  //   .set_mode(mode)
  //   .set_met_label(met_label);

  // ------------------------------------------------------------------------------------
  // Category Modules
  // ------------------------------------------------------------------------------------  
  HTTCategories httCategories = HTTCategories("HTTCategories")
    .set_fs(fs)
    .set_channel(channel)
    .set_is_embedded(is_embedded)
    .set_ditau_label("emtauCandidates")
    .set_met_label(met_label);

  // HttVbfCategory httVbfCategory = HttVbfCategory
  //   ("HttVbfCategory","pfJetsPFlow")
  //   .set_fs(fs)
  //   .set_channel(channel)
  //   .set_met_label(met_label)
  //   .set_do_jetvtx_assoc(false)
  //   .set_do_mc_eff(false)
  //   .set_do_vbf_mva(do_vbf_mva)
  //   .set_do_mva_pu_id(true)
  //   .set_make_plots(false)
  //   .set_mva_cut(0.5)
  //   .set_make_mva_tree(false)
  //   .set_do_cjv(true)
  //   .set_vbf_cjv_pt(30.0)
  //   .set_mva_name("MVA_"+output_name);
  // if (special_mode == 6 || special_mode == 8) httVbfCategory.set_jet_pt(20.0);
  // if (special_mode == 6) httVbfCategory.set_mva_cut(0.0);
  // httVbfCategory.set_era(era).set_use_hcp_mva(true);
  // if (era == era::data_2011) {
  //   httVbfCategory.set_mva_cut(0.676);
  //   if (channel == channel::em) httVbfCategory.set_mva_cut(0.9);
  // } else {
  //   httVbfCategory.set_mva_cut(0.978);
  //   if (channel == channel::em) httVbfCategory.set_mva_cut(0.9);
  // }
  // if (strategy == strategy::ichep2012) {
  //   httVbfCategory.set_use_hcp_mva(false).set_mva_cut(0.5);
  //   httVbfCategory.set_jet_eta(5.0);
  // }

  // HttVHCategory httVHCategory = HttVHCategory
  //   ("HttVHCategory","pfJetsPFlow");

  // HttOneJetCategory httOneJetHighPtCategory = HttOneJetCategory
  //   ("HttOneJetHighPtCategory","pfJetsPFlow").set_fs(fs).set_high_pt(true).set_do_met_cut(true).set_channel(channel).set_met_label(met_label);
  // httOneJetHighPtCategory.set_met_cut(30.0);
  // if (strategy == strategy::ichep2012) httOneJetHighPtCategory.set_jet_eta(5.0);
  

  // HttOneJetCategory httOneJetLowPtCategory = HttOneJetCategory
  //   ("HttOneJetLowPtCategory","pfJetsPFlow").set_fs(fs).set_high_pt(false).set_do_met_cut(true).set_channel(channel).set_met_label(met_label);
  // httOneJetLowPtCategory.set_met_cut(30.0);
  // if (strategy == strategy::ichep2012) httOneJetLowPtCategory.set_jet_eta(5.0);


  // HttOneBJetCategory httOneBJetHighPtCategory = HttOneBJetCategory
  //   ("HttBOneJetHighPtCategory","pfJetsPFlow").set_fs(fs).set_high_pt(true).set_channel(channel).set_met_label(met_label);;
  // if (special_mode == 9 || special_mode == 10) httOneBJetHighPtCategory.set_apply_special_mode(true).set_special_min(1).set_special_max(99).set_special_pt(20.0).set_special_eta(2.4).set_met_label(met_label);
  // if (special_mode == 13) httOneBJetHighPtCategory.set_apply_special_mode(true).set_special_min(1).set_special_max(99).set_special_pt(20.0).set_special_eta(2.4).set_met_label(met_label);

  // HttOneBJetCategory httOneBJetLowPtCategory = HttOneBJetCategory
  //   ("HttBOneJetLowPtCategory","pfJetsPFlow").set_fs(fs).set_high_pt(false).set_channel(channel).set_met_label(met_label);;
  // if (special_mode == 9 || special_mode == 10) httOneBJetLowPtCategory.set_apply_special_mode(true).set_special_min(1).set_special_max(99).set_special_pt(20.0).set_special_eta(2.4).set_met_label(met_label);
  // if (special_mode == 13) httOneBJetLowPtCategory.set_apply_special_mode(true).set_special_min(1).set_special_max(99).set_special_pt(20.0).set_special_eta(2.4).set_met_label(met_label);

  // HttBTagCategory httBTagCategory = HttBTagCategory
  //   ("HttBTagCategory","pfJetsPFlow").set_fs(fs).set_channel(channel).set_met_label(met_label);
  // if (special_mode == 9 || special_mode == 10) httBTagCategory.set_apply_special_mode(true).set_special_min(1).set_special_max(99).set_special_pt(20.0).set_special_eta(2.4).set_met_label(met_label);
  // if (special_mode == 13) httBTagCategory.set_apply_special_mode(true).set_special_min(1).set_special_max(99).set_special_pt(20.0).set_special_eta(2.4).set_met_label(met_label);

  // HttNoBTagCategory httNoBTagCategory = HttNoBTagCategory
  //   ("HttNoBTagCategory","pfJetsPFlow").set_fs(fs).set_do_met_cut(false).set_channel(channel).set_met_label(met_label);

  // HttZeroJetCategory httZeroJetHighPtCategory = HttZeroJetCategory
  //   ("HttZeroJetHighPtCategory").set_fs(fs).set_high_pt(true).set_do_met_cut(true).set_channel(channel).set_met_label(met_label);
  // httZeroJetHighPtCategory.set_met_cut(0.0);
  // if (strategy == strategy::ichep2012) {
  //   httZeroJetHighPtCategory.set_met_cut(30.0);
  //   httZeroJetHighPtCategory.set_veto_jet_eta(5.0);
  // }

  // HttZeroJetCategory httZeroJetLowPtCategory = HttZeroJetCategory
  //   ("HttZeroJetLowPtCategory").set_fs(fs).set_high_pt(false).set_do_met_cut(true).set_channel(channel).set_met_label(met_label);
  // httZeroJetLowPtCategory.set_met_cut(0.0);
  // if (strategy == strategy::ichep2012) {
  //   httZeroJetLowPtCategory.set_met_cut(30.0);
  //   httZeroJetLowPtCategory.set_veto_jet_eta(5.0);
  // }

  // if (!is_data) {
  //   httVbfCategory.set_do_btag_weight(true);
  //   httOneJetHighPtCategory.set_do_btag_weight(true);
  //   httOneJetLowPtCategory.set_do_btag_weight(true);
  //   httOneBJetHighPtCategory.set_do_btag_weight(true);
  //   httOneBJetLowPtCategory.set_do_btag_weight(true);
  //   httZeroJetHighPtCategory.set_do_btag_weight(true);
  //   httZeroJetLowPtCategory.set_do_btag_weight(true);
  //   if (special_mode == 9 || special_mode == 10 || special_mode == 13) {
  //     httOneBJetHighPtCategory.set_do_btag_weight(false);
  //     httOneBJetLowPtCategory.set_do_btag_weight(false);
  //   }
  // }

  HttSync httSync("HttSync","SYNCFILE_" + output_name, channel);
  httSync.set_is_embedded(is_embedded).set_met_label(met_label);
  if (strategy == strategy::ichep2012) httSync.set_jet_eta(5.0);

  SVFit svfit("SVFit");
  svfit.set_outname(output_name).set_op(svfit_mode).set_dilepton_label("emtauCandidates").set_met_label(met_label).set_channel(channel);
  svfit.set_fullpath(svfit_folder);
  if (svfit_override != "") {
    svfit.set_outname(svfit_override);
  }

  // ------------------------------------------------------------------------------------
  // Build Analysis Sequence
  // ------------------------------------------------------------------------------------  
  //                              analysis.AddModule(&httPrint);
  if (is_data && !do_skim)        analysis.AddModule(&lumiMask);
  if (!is_data && !do_skim)       analysis.AddModule(&pileupWeight);
  if (ztautau_mode > 0)           analysis.AddModule(&zTauTauFilter);
  if (!is_data && do_mass_filter) analysis.AddModule(&mssmMassFilter);
  if (tau_scale_mode > 0)         analysis.AddModule(&tauEnergyShifter);
  if (is_embedded)                analysis.AddModule(&embeddedMassFilter);

  if (channel == channel::et) {
                                  analysis.AddModule(&selElectronCopyCollection);
                                  analysis.AddModule(&selElectronFilter);
    if (!do_skim) {                              
                                  analysis.AddModule(&vetoElectronCopyCollection);
                                  analysis.AddModule(&vetoElectronFilter);
                                  analysis.AddModule(&vetoElectronPairProducer);
      if (special_mode != 18)     analysis.AddModule(&vetoElectronPairFilter);
      if (special_mode != 18)     analysis.AddModule(&extraElectronVeto);
      if (special_mode != 18)     analysis.AddModule(&extraMuonVeto);
    }
                                  analysis.AddModule(&tauPtEtaFilter);
                                  analysis.AddModule(&tauDzFilter);
                                  analysis.AddModule(&tauIsoFilter);
                                  analysis.AddModule(&tauElRejectFilter);
                                  analysis.AddModule(&tauMuRejectFilter);

                                  analysis.AddModule(&tauElPairProducer);
                                  analysis.AddModule(&pairFilter);
  }

  if (channel == channel::mt || channel == channel::mtmet) {
                                  analysis.AddModule(&selMuonCopyCollection);
                                  analysis.AddModule(&selMuonFilter);
    if (!do_skim) {                              
                                  analysis.AddModule(&vetoMuonCopyCollection);
                                  analysis.AddModule(&vetoMuonFilter);
                                  analysis.AddModule(&vetoMuonPairProducer);
                                  analysis.AddModule(&vetoMuonPairFilter);
                                  analysis.AddModule(&extraElectronVeto);
                                  analysis.AddModule(&extraMuonVeto);
    }
                                  analysis.AddModule(&tauPtEtaFilter);
                                  analysis.AddModule(&tauDzFilter);
                                  analysis.AddModule(&tauIsoFilter);
                                  analysis.AddModule(&tauElRejectFilter);
                                  analysis.AddModule(&tauMuRejectFilter);
    
                                  analysis.AddModule(&tauMuPairProducer);
                                  analysis.AddModule(&pairFilter);
  }

  if (channel == channel::em) {
                                  analysis.AddModule(&selElectronCopyCollection);
                                  analysis.AddModule(&selElectronFilter);
                                  analysis.AddModule(&elecMuonOverlapFilter);
                                  analysis.AddModule(&selMuonCopyCollection);
                                  analysis.AddModule(&selMuonFilter);
  
                                  analysis.AddModule(&elMuPairProducer);
                                  analysis.AddModule(&pairFilter);
    if (!do_skim) {                              
                                  analysis.AddModule(&extraElectronVeto);
                                  analysis.AddModule(&extraMuonVeto);
    }
  }

  if (!do_skim) {
    if (!is_embedded)             analysis.AddModule(&httTriggerFilter);
                                  // analysis.AddModule(&runStats);
                                  analysis.AddModule(&httPairSelector);
    //                            analysis.AddModule(&jetEnergyCorrections);
                                  analysis.AddModule(&jetIDFilter);
                                  analysis.AddModule(&jetLeptonOverlapFilter);
                                  analysis.AddModule(&httRecoilCorrector);

    if (svfit_mode > 0 && !(svfit_override != "" && svfit_mode == 1)) 
                                  analysis.AddModule(&svfit);

                                  analysis.AddModule(&httWeights);
    if(quark_gluon_study)         analysis.AddModule(&quarkGluonDiscriminatorStudy);                                 
    if (make_sync_ntuple)         analysis.AddModule(&httSync);
    //                            analysis.AddModule(&httSelection);
    //                            analysis.AddModule(&httMetStudy);
    //                            analysis.AddModule(&jetVtxStudy);
    if(!quark_gluon_study)        analysis.AddModule(&httCategories);

    // if (special_mode == 9 || special_mode == 10) {
    //                            analysis.AddModule(&httOneBJetHighPtCategory);
    //                            analysis.AddModule(&httOneBJetLowPtCategory);
    // } else {
    //                            analysis.AddModule(&httVbfCategory);
    //                            analysis.AddModule(&httOneJetHighPtCategory);
    //                            analysis.AddModule(&httOneJetLowPtCategory);
    //                            analysis.AddModule(&httOneBJetHighPtCategory);
    //                            analysis.AddModule(&httOneBJetLowPtCategory);
    //                            analysis.AddModule(&httZeroJetHighPtCategory);
    //                            analysis.AddModule(&httZeroJetLowPtCategory);
    //                            analysis.AddModule(&httNoBTagCategory);
    //                            analysis.AddModule(&httBTagCategory);
    // }
  }

  if (do_skim) {
    if (faked_tau_selector > 0)   analysis.AddModule(&httPairSelector);
  }

  // Run analysis
  analysis.RunAnalysis();
  delete fs;
  return 0;
}




