#include <iostream>
#include <vector>
#include <string>
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/format.hpp"
#include "TSystem.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Utilities/interface/FnRootTools.h"
#include "Core/interface/AnalysisBase.h"
#include "Modules/interface/CopyCollection.h"
#include "Modules/interface/SimpleFilter.h"
#include "Modules/interface/OverlapFilter.h"
#include "Modules/interface/CheckEvents.h"
#include "Modules/interface/CompositeProducer.h"
#include "Modules/interface/OneCollCompositeProducer.h"
#include "Modules/interface/PileupWeight.h"
#include "HiggsTauTau/interface/HTTPairSelector.h"
#include "HiggsTauTau/interface/HTTWeights.h"
#include "Modules/interface/QuarkGluonDiscriminatorStudy.h"
#include "HiggsTauTau/interface/GenLevelStudy.h"
#include "HiggsTauTau/interface/HTTRecoilCorrector.h"
#include "HiggsTauTau/interface/HhhBJetRegression.h"
#include "HiggsTauTau/interface/HTTSync.h"
#include "HiggsTauTau/interface/HTTSyncTemp.h"
#include "HiggsTauTau/interface/HTTPrint.h"
#include "Modules/interface/MakeRunStats.h"
#include "Modules/interface/EnergyShifter.h"
#include "Modules/interface/SVFit.h"
#include "HiggsTauTau/interface/SVFitTest.h"
#include "Modules/interface/JetEnergyCorrections.h"
#include "HiggsTauTau/interface/JetEnergyUncertainty.h"
#include "Modules/interface/LumiMask.h"
#include "HiggsTauTau/interface/HTTConfig.h"
#include "HiggsTauTau/interface/HTTEnergyScale.h"
#include "HiggsTauTau/interface/HTTCategories.h"
#include "HiggsTauTau/interface/HTTTriggerFilter.h"
#include "HiggsTauTau/interface/TauDzFixer.h"
#include "HiggsTauTau/interface/HTTEMuExtras.h"
#include "HiggsTauTau/interface/HTTEMuMVA.h"
#include "HiggsTauTau/interface/HhhEMuMVA.h"
#include "HiggsTauTau/interface/HhhEMuMVABoth.h"
#include "HiggsTauTau/interface/HhhMTMVABoth.h"
#include "HiggsTauTau/interface/HhhMTMVACategory.h"
#include "HiggsTauTau/interface/HTTL1MetCorrector.h"
#include "HiggsTauTau/interface/HTTL1MetCut.h"
#include "HiggsTauTau/interface/TauEfficiency.h"
#include "HiggsTauTau/interface/EmbeddingKineReweightProducer.h"
#include "HiggsTauTau/interface/BTagCheck.h"
#include "HiggsTauTau/interface/HhhMetScale.h"

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
  unsigned btag_mode;             // 0 = no shift, 1 = shift down, 2 = shift up
  unsigned bfake_mode;            // 0 = no shift, 1 = shift down, 2 = shift up
  unsigned jes_mode;              // 0 = no shift, 1 = shift down, 2 = shift up
  unsigned l1met_mode;              // 0 = no shift, 1 = shift down, 2 = shift up
  unsigned metscale_mode;          // 0 = no shift, 1 = shift down, 2 = shift up
  unsigned mass_scale_mode;       // 0 = no shift, 1 = nominal, but in TSCALE_DOWN, 2 = shift up, 3 = shift up again, in TSCALE_UP
  unsigned svfit_mode;            // 0 = not run, 1 = generate jobs, 2 = read-in job output
  unsigned new_svfit_mode;        // 0 = not run, 1 = generate jobs, 2 = read-in job output
  string svfit_folder;            // Folder containing svfit jobs & output
  string svfit_override;          // Override the svfit results to use
  unsigned kinfit_mode;           // 0 = don't run, 1 = run simple 125,125 default fit, 2 = run extra masses default fit, 3 = run m_bb only fit
  unsigned ztautau_mode;          // 0 = not run, 1 = select Z->tautau, 2 = select Z->ee and Z->mumu
  unsigned faked_tau_selector;    // 0 = not run, 1 = tau matched to gen. lepton, 2 = tau not matched to lepton
  unsigned hadronic_tau_selector;    // 0 = not run, 1 = tau matched to gen. lepton, 2 = tau not matched to lepton
  unsigned mva_met_mode;          // 0 = standard mva met, 1 = mva met from vector (only when mva met is being used)
  bool make_sync_ntuple;          // Generate a sync ntuple
  bool quark_gluon_study;         // Run study on quark-gluon jet discriminators
  bool make_gen_plots;            // Make generator level plots at start of chain
  bool bjet_regr_correction;      // Apply b jet regression corrections
  string allowed_tau_modes;       // "" means all, otherwise "1,10"=allow 1prong1pizero,3prong
  bool moriond_tau_scale;         // Use new central tau scale shifts
  bool large_tscale_shift;        // Shift tau energy scale by +/- 6% instead of 3%
  bool do_tau_eff;                // Run the tau efficiency module
  unsigned pu_id_training;        // Pileup jet id training
  unsigned vh_filter_mode;        // 0 = no filter, 1 = WH/ttH, 2 = ZH
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
      ("btag_mode",           po::value<unsigned>(&btag_mode)->default_value(0))
      ("bfake_mode",          po::value<unsigned>(&bfake_mode)->default_value(0))
      ("jes_mode",            po::value<unsigned>(&jes_mode)->default_value(0))
      ("l1met_mode",          po::value<unsigned>(&l1met_mode)->default_value(0))
      ("metscale_mode",       po::value<unsigned>(&metscale_mode)->default_value(0))
      ("mass_scale_mode",     po::value<unsigned>(&mass_scale_mode)->default_value(0))
      ("svfit_mode",          po::value<unsigned>(&svfit_mode)->default_value(0))
      ("new_svfit_mode",      po::value<unsigned>(&new_svfit_mode)->default_value(0))
      ("svfit_folder",        po::value<string>(&svfit_folder)->default_value(""))
      ("svfit_override",      po::value<string>(&svfit_override)->default_value(""))
      ("kinfit_mode",         po::value<unsigned>(&kinfit_mode)->default_value(0))
      ("ztautau_mode",        po::value<unsigned>(&ztautau_mode)->default_value(0))
      ("faked_tau_selector",  po::value<unsigned>(&faked_tau_selector)->default_value(0))
      ("hadronic_tau_selector",  po::value<unsigned>(&hadronic_tau_selector)->default_value(0))
      ("mva_met_mode",        po::value<unsigned>(&mva_met_mode)->default_value(1))
      ("quark_gluon_study",   po::value<bool>(&quark_gluon_study)->default_value(false))
      ("bjet_regr_correction",po::value<bool>(&bjet_regr_correction)->default_value(false))
      ("make_gen_plots",      po::value<bool>(&make_gen_plots)->default_value(false))
      ("make_sync_ntuple",    po::value<bool>(&make_sync_ntuple)->default_value(false))
      ("moriond_tau_scale",   po::value<bool>(&moriond_tau_scale)->default_value(false))
      ("large_tscale_shift",  po::value<bool>(&large_tscale_shift)->default_value(false))
      ("do_tau_eff",          po::value<bool>(&do_tau_eff)->default_value(false))
      ("allowed_tau_modes",   po::value<string>(&allowed_tau_modes)->default_value(""))
      ("pu_id_training",      po::value<unsigned>(&pu_id_training)->default_value(1))
      ("vh_filter_mode",      po::value<unsigned>(&vh_filter_mode)->default_value(0));
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
  po::notify(vm);

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
  if (mass_scale_mode == 1) output_folder += "TSCALE_DOWN/";
  if (mass_scale_mode == 3) output_folder += "TSCALE_UP/";
  if (btag_mode == 1) output_folder += "BTAG_DOWN/";
  if (btag_mode == 2) output_folder += "BTAG_UP/";
  if (bfake_mode == 1) output_folder += "BFAKE_DOWN/";
  if (bfake_mode == 2) output_folder += "BFAKE_UP/";
  if (jes_mode == 1) output_folder += "JES_DOWN/";
  if (jes_mode == 2) output_folder += "JES_UP/";
  if (l1met_mode == 1) output_folder += "L1MET_DOWN/";
  if (l1met_mode == 2) output_folder += "L1MET_UP/";
  if (metscale_mode == 1) { 
    output_folder += "MET_DOWN/";
    svfit_folder += "MET_DOWN/";
  }
  if (metscale_mode == 2) {
    output_folder += "MET_UP/";
    svfit_folder += "MET_UP/";
  }
  
  



  std::cout << "-------------------------------------" << std::endl;
  std::cout << "HiggsToTauTau Analysis" << std::endl;
  std::cout << "-------------------------------------" << std::endl;      string param_fmt = "%-25s %-40s\n";
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
  std::cout << boost::format(param_fmt) % "mass_scale_mode" % mass_scale_mode;
  std::cout << boost::format(param_fmt) % "svfit_mode" % svfit_mode;
  std::cout << boost::format(param_fmt) % "new_svfit_mode" % new_svfit_mode;
  std::cout << boost::format(param_fmt) % "vh_filter_mode" % vh_filter_mode;
  if (svfit_mode > 0) {
    std::cout << boost::format(param_fmt) % "svfit_folder" % svfit_folder;
    std::cout << boost::format(param_fmt) % "svfit_override" % svfit_override;
  }
  std::cout << boost::format(param_fmt) % "kinfit_mode" % kinfit_mode;
  std::cout << boost::format(param_fmt) % "ztautau_mode" % ztautau_mode;
  std::cout << boost::format(param_fmt) % "faked_tau_selector" % faked_tau_selector;
  std::cout << boost::format(param_fmt) % "hadronic_tau_selector" % hadronic_tau_selector;
  std::cout << boost::format(param_fmt) % "mva_met_mode" % mva_met_mode;
  std::cout << boost::format(param_fmt) % "make_sync_ntuple" % make_sync_ntuple;
  std::cout << boost::format(param_fmt) % "allowed_tau_modes" % allowed_tau_modes;
  std::cout << boost::format(param_fmt) % "moriond_tau_scale" % moriond_tau_scale;
  std::cout << boost::format(param_fmt) % "large_tscale_shift" % large_tscale_shift;
  std::cout << boost::format(param_fmt) % "pu_id_training" % pu_id_training;
  std::cout << boost::format(param_fmt) % "make_gen_plots" % make_gen_plots;
  std::cout << boost::format(param_fmt) % "bjet_regr_correction" % bjet_regr_correction;

  // Load necessary libraries for ROOT I/O of custom classes
  // gSystem->Load("libFWCoreFWLite.dylib");
  // gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  // AutoLibraryLoader::enable();

  // Build a vector of input files
  vector<string> files = ParseFileLines(filelist);
  if (files.size() == 0) {
    std::cout << "Error, filelist is missing or empty" << std::endl;
    exit(1);
  }
  for (unsigned i = 0; i < files.size(); ++i) files[i] = input_prefix + files[i];
  
  // Create ROOT output fileservice
  fwlite::TFileService *fs = new fwlite::TFileService((output_folder+output_name).c_str());
  
  // Setup Lepton kinematic cuts
  std::string met_label;
  double elec_dz, muon_dz, elec_dxy, muon_dxy, tau_dz;
  double elec_pt, elec_eta, muon_pt, muon_eta, tau_pt, tau_eta;
  met_label = "pfMVAMet";

  if (channel == channel::et || channel == channel::etmet) {
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
    } else if (era == era::data_2012_rereco) {
      elec_pt = 24.0;
      elec_eta = 2.1;
      muon_pt = 20.0;
      muon_eta = 2.1;
    } else {
      elec_pt = 23.0;
      elec_eta = 2.5;
      muon_pt = 18.0;
      muon_eta = 2.1;
    }
    tau_pt = 20.0;
    tau_eta = 2.3;
    if (channel == channel::etmet) elec_pt = 13.0;
  }

  if (channel == channel::mt || channel == channel::mtmet) {
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
    } else if (era == era::data_2012_rereco) {
      elec_pt = 24.0;
      elec_eta = 2.1;
      muon_pt = 20.0;
      muon_eta = 2.1;
    } else {
      elec_pt = 23.0;
      elec_eta = 2.5;
      muon_pt = 18.0;
      muon_eta = 2.1;
    }
    tau_pt = 20.0;
    tau_eta = 2.3;
    if (channel == channel::mtmet) muon_pt = 9.0;
  }

  if (channel == channel::em) {
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
    if(era == era::data_2015) {
      //I think this is probably wrong and just duplicated from the other channels, but keep for now to be consistent with twiki
      elec_dz = 0.2;
      elec_dxy = 0.045;
      muon_dz = 0.2;
      muon_dxy = 0.045;
      elec_pt = 13.0;
      elec_eta = 2.5;
      muon_pt = 9.5; //THIS IS NOT WHAT'S ON THE TWIKI! Only have MVAMET pairs with pt>9.5 
      muon_eta = 2.4;
    }
    if (special_mode == 22 || special_mode == 25) {
      elec_dxy = 999.;  
      muon_dxy = 999.;  
      elec_dz = 999.;  
      muon_dz = 999.;  
    }  
    if (special_mode == 20) {
      elec_dxy = 999.;  
      elec_dz = 999.;  
    }   
    if (special_mode == 21) {
      muon_dxy = 999.;  
      muon_dz = 999.;  
    }   
  }

  // Lower pt thresholds on electrons and taus when skimming, 
  // to allow for energy scale shifts later
  if (do_skim) {
    tau_pt = 18.0;
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

  // Create analysis object
  ic::AnalysisBase analysis(
    "HiggsTauTau",        // Analysis name
    files,                // Input files
    "icEventProducer/EventTree", // TTree name
    max_events);          // Max. events to process (-1 = all)
  if (do_skim && skim_path != "") analysis.DoSkimming(skim_path);
  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);

  // ------------------------------------------------------------------------------------
  // Misc Modules
  // ------------------------------------------------------------------------------------

  string data_json = "";
  if (era == era::data_2011) {
    if (channel == channel::em) {
      data_json = "input/json/json_data_2011.txt";
    } else{
      data_json = "input/json/json_data_2011_et_mt.txt";
    }
  }             
  if (era == era::data_2012_rereco)       data_json = "input/json/data_2012_rereco.txt";
  //if (era == era::data_2015)       data_json = "input/json/data_2015.txt";

  LumiMask lumiMask = LumiMask("LumiMask")
    .set_produce_output_jsons("")
    .set_input_file(data_json);

  MakeRunStats runStats = MakeRunStats("RunStats")
    .set_output_name(output_folder+output_name+".runstats");

  HTTPrint httPrint("HTTPrint");
  if(era == era::data_2015){
   httPrint.set_muon_label("muons");
   httPrint.set_jet_label("ak4PFJetsCHS");
  }

  string mc_pu_file;
  if (mc == mc::fall11_42X) mc_pu_file    = "input/pileup/MC_Fall11_PU_S6-500bins.root";
  if (mc == mc::summer12_53X) mc_pu_file  = "input/pileup/MC_Summer12_PU_S10-600bins.root";
  //for now set phys14 to read the 2012 PU files, even though they are not used
  if (mc == mc::phys14_72X) mc_pu_file  = "input/pileup/MC_Summer12_PU_S10-600bins.root";
  //if (mc == mc::summer15_74X) mc_pu_file  = "input/pileup/MC_Summer12_PU_S10-600bins.root";
  string data_pu_file;
  if (era == era::data_2011) data_pu_file     =  "input/pileup/Data_Pileup_2011_HCP-500bins.root";
  if (era == era::data_2012_rereco) data_pu_file    =  "input/pileup/Data_Pileup_2012_ReRecoPixel-600bins.root";
  //for now set phys14 to read the 2012 PU files, even though they are not used
  if (era == era::data_2015) data_pu_file    =  "input/pileup/Data_Pileup_2012_ReRecoPixel-600bins.root";
  if (channel == channel::mtmet) data_pu_file       =  "input/pileup/Data_Pileup_2012_ReRecoD_All-600bins.root";

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
    .set_era(era)
    .set_is_data(is_data)
    .set_is_embedded(is_embedded)
    .set_pair_label("emtauCandidates");

  SimpleCounter<GenParticle> zTauTauFilter = SimpleCounter<GenParticle>("ZToTauTauSelector")
    .set_input_label("genParticles")
    .set_predicate(
      (bind(&GenParticle::status, _1) == 3) && 
      (bind(abs,(bind(&GenParticle::pdgid, _1))) == 15))
    .set_min(2);
  if (ztautau_mode == 2) zTauTauFilter.set_min(0).set_max(0);

  // auto zh_selector = [](GenParticle const* p) -> bool {
  //   if (p->status() == 3 && std::abs(p->pdgid()) == 23) {
  //     return true;
  //   } else {
  //     return false;
  //   }
  // };

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


  EmbeddingKineReweightProducer rechitWeights = EmbeddingKineReweightProducer("RecHitWeights")
    .set_genparticle_label("genParticlesEmbedded")
    .set_channel(channel);
  if (channel == channel::et) rechitWeights.set_file("input/rechit_weights/embeddingKineReweight_ePtGt20tauPtGt18_recEmbedded.root");
  if (channel == channel::mt) rechitWeights.set_file("input/rechit_weights/embeddingKineReweight_muPtGt16tauPtGt18_recEmbedded.root");
  if (channel == channel::em) rechitWeights.set_file("input/rechit_weights/embeddingKineReweight_recEmbedding_emu.root");
  if (channel == channel::mtmet) rechitWeights.set_file("input/rechit_weights/embeddingKineReweight_muPt7to25tauPtGt18_recEmbedded.root");

  std::string jets_label="pfJetsPFlow";
  if(era==era::data_2015) jets_label="ak4PFJetsCHS";

  string jec_payload = is_data ? "GR_P_V42_AN3" : "START53_V15";
  JetEnergyCorrections<PFJet> jetEnergyCorrections = JetEnergyCorrections<PFJet>
  ("JetEnergyCorrections")
  .set_input_label(jets_label)
  .set_is_data(is_data)
  .set_l1_file("input/jec/"+jec_payload+"_L1FastJet_AK5PF.txt")
  .set_l2_file("input/jec/"+jec_payload+"_L2Relative_AK5PF.txt")
  .set_l3_file("input/jec/"+jec_payload+"_L3Absolute_AK5PF.txt")
  .set_res_file("input/jec/"+jec_payload+"_L2L3Residual_AK5PF.txt");

  string jes_input_file = "input/jec/JEC11_V12_AK5PF_UncertaintySources.txt";
  string jes_input_set  = "SubTotalDataMC";
  if (era == era::data_2012_rereco) {
    jes_input_file = "input/jec/Summer13_V1_DATA_UncertaintySources_AK5PF.txt";
    jes_input_set  = "SubTotalMC";
  }
  //if (era == era::data_2015) {
  //  jes_input_file = "input/jec/Summer13_V1_DATA_UncertaintySources_AK5PF.txt";
  //  jes_input_set  = "SubTotalMC";
  //}
  auto jetEnergyUncertainty = JetEnergyUncertainty<PFJet>("JetEnergyUncertainty")
  .set_input_label(jets_label)
  .set_jes_shift_mode(jes_mode)
  .set_uncert_file(jes_input_file)
  .set_uncert_set(jes_input_set);

  HTTL1MetCorrector httL1MetCorrector("HTTL1MetCorrector");
  httL1MetCorrector.set_l1_met_mode(l1met_mode);
  HTTL1MetCut httL1MetCut("HTTL1MetCut");
  
  // ------------------------------------------------------------------------------------
  // Electron Modules
  // ------------------------------------------------------------------------------------
  double elec_shift = 1.0;
  if (tau_scale_mode == 1) elec_shift = 0.99;
  if (tau_scale_mode == 2) elec_shift = 1.01;
  EnergyShifter<Electron> electronEnergyShifter = EnergyShifter<Electron>
  ("ElectronEnergyShifter")
    .set_input_label("electrons")
    .set_shift(elec_shift);

  HTTEMuExtras emuExtras("EMuExtras");
  HTTEMuMVA emuMVA = HTTEMuMVA("EMuMVA");
  //Some attempts at MVA for the H->hh analysis, could possibly be used in the future
  HhhEMuMVA HhhemuMVA = HhhEMuMVA("HhhEMuMVA");
  HhhEMuMVABoth HhhemuMVABoth = HhhEMuMVABoth("HhhEMuMVABoth");

  HhhMTMVABoth HhhmtMVABoth = HhhMTMVABoth("HhhMTMVABoth"); 
  HhhMTMVACategory HhhmtMVACategory = HhhMTMVACategory("HhhMTMVACategory");
  
  CopyCollection<Electron>  
    selElectronCopyCollection("CopyToSelElectrons","electrons","selElectrons");

  boost::function<bool (Electron const*)> elec_idiso_func;
  if(strategy == strategy::paper2013) {
    if (special_mode == 20 || special_mode == 22) {
      elec_idiso_func = bind(HttEMuFakeElectron, _1);
    } else if (special_mode == 2) {
      elec_idiso_func = bind(ElectronHTTId, _1, (channel == channel::em)) && (bind(PF04IsolationVal<Electron>, _1, 0.5, 1) < 0.5);
    } else if (special_mode == 3 || special_mode == 4) {
      elec_idiso_func = bind(ElectronHTTId, _1, (channel == channel::em)) && (bind(PF04IsolationVal<Electron>, _1, 0.5, 1) > 0.2) && (bind(PF04IsolationVal<Electron>, _1, 0.5, 1) < 0.5);
    } else if (special_mode == 23) {
      elec_idiso_func = bind(ElectronHTTId, _1, (channel == channel::em));
    } else if (special_mode == 24) {
      elec_idiso_func = bind(ElectronHTTId, _1, (channel == channel::em)) && !bind(PF04IsolationEBElec, _1, 0.5, 0.15, 0.1);
    } else if (special_mode == 25) {
      elec_idiso_func = (bind(PF04IsolationVal<Electron>, _1, 0.5,1) >= 0.0); // Dummy function, will always pass
    } else {
      if (channel == channel::em) {
        elec_idiso_func = bind(ElectronHTTId, _1, true) && bind(PF04IsolationEBElec, _1, 0.5, 0.15, 0.1);
      } else {
        elec_idiso_func = bind(ElectronHTTId, _1, false) && (bind(PF04IsolationVal<Electron>, _1, 0.5, 1) < 0.1);
      }
    }
  } else if (strategy == strategy::phys14) {
    //Special modes and emu channel need more thought, just placeholders for now
    if (special_mode == 20 || special_mode == 22) {
      elec_idiso_func = bind(HttEMuFakeElectron, _1);
    } else if (special_mode == 2) {
      elec_idiso_func = bind(ElectronHTTId, _1, (channel == channel::em)) && (bind(PF04IsolationVal<Electron>, _1, 0.5, 1) < 0.5);
    } else if (special_mode == 3 || special_mode == 4) {
      elec_idiso_func = bind(ElectronHTTId, _1, (channel == channel::em)) && (bind(PF04IsolationVal<Electron>, _1, 0.5, 1) > 0.2) && (bind(PF04IsolationVal<Electron>, _1, 0.5, 1) < 0.5);
    } else if (special_mode == 23) {
      elec_idiso_func = bind(ElectronHTTId, _1, (channel == channel::em));
    } else if (special_mode == 24) {
      elec_idiso_func = bind(ElectronHTTId, _1, (channel == channel::em)) && !bind(PF04IsolationEBElec, _1, 0.5, 0.15, 0.1);
    } else if (special_mode == 25) {
      elec_idiso_func = (bind(PF04IsolationVal<Electron>, _1, 0.5, 1) >= 0.0); // Dummy function, will always pass
    } else {
      if (channel == channel::em) {
        elec_idiso_func = bind(ElectronHTTIdPhys14, _1, false) && (bind(PF04IsolationVal<Electron>, _1, 0.5, 0)<0.15);
      } else {
        elec_idiso_func = bind(ElectronHTTIdPhys14, _1, false) && (bind(PF04IsolationVal<Electron>, _1, 0.5, 0) < 0.1);
      }
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
      bind(PF04IsolationVal<Electron>, _1, 0.5, 1) < 0.3);
  if(strategy == strategy::phys14) {
      vetoElectronFilter.set_predicate(
      bind(MinPtMaxEta, _1, 10.0, 2.5) &&
      bind(fabs, bind(&Electron::dxy_vertex, _1)) < elec_dxy &&
      bind(fabs, bind(&Electron::dz_vertex, _1)) < elec_dz &&
      bind(VetoElectronIDPhys14, _1) &&
      bind(PF04IsolationVal<Electron>, _1, 0.5, 0) < 0.3);
  }

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
                && bind(PF04IsolationVal<Electron>, _1, 0.5, 1) < 0.3
                && bind(fabs, bind(&Electron::dxy_vertex, _1)) < 0.045
                && bind(fabs, bind(&Electron::dz_vertex, _1)) < 0.2)
    .set_min(0).set_max((channel == channel::et || channel == channel::etmet || channel == channel::em) ? 1 : 0);
    if(strategy == strategy::phys14) {
      extraElectronVeto.set_predicate(bind(MinPtMaxEta, _1, 10.0, 2.5)
                && bind(VetoElectronIDPhys14, _1)
                && bind(fabs, bind(&Electron::dxy_vertex, _1)) < 0.045
                && bind(fabs, bind(&Electron::dz_vertex, _1)) < 0.2
                && bind(PF04IsolationVal<Electron>, _1, 0.5, 0) < 0.3);
    }

  // ------------------------------------------------------------------------------------
  // Muon Modules
  // ------------------------------------------------------------------------------------
  std::string muon_label="muonsPFlow";
  if(era == era::data_2015) muon_label="muons";
  CopyCollection<Muon> selMuonCopyCollection("CopyToSelMuons",muon_label,"selMuons");

  boost::function<bool (Muon const*)> muon_idiso_func;
  if(strategy == strategy::paper2013) {
    if (special_mode == 21 || special_mode == 22) {
      muon_idiso_func = bind(HttEMuFakeMuon, _1);
    } else if (special_mode == 2) {
      muon_idiso_func = bind(MuonTight, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5, 1) < 0.5);
    } else if (special_mode == 3 || special_mode == 4) {
      muon_idiso_func = bind(MuonTight, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5, 1) > 0.2) && (bind(PF04IsolationVal<Muon>, _1, 0.5, 1) < 0.5);
    } else if (special_mode == 25) {
      muon_idiso_func = (bind(PF04IsolationVal<Muon>, _1, 0.5, 1) >= 0.0);
    } else {
      if (channel == channel::em) {
        muon_idiso_func = bind(MuonTight, _1) && bind(PF04IsolationEB<Muon>, _1, 0.5, 0.15, 0.1);
      } else {
        muon_idiso_func = bind(MuonTight, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5, 1) < 0.1);
      }
    }
    //Special modes and emu channel need more thought, just placeholders for now
  } else if (strategy == strategy::phys14) {
    if (special_mode == 21 || special_mode == 22) {
      muon_idiso_func = bind(HttEMuFakeMuon, _1);
    } else if (special_mode == 2) {
      muon_idiso_func = bind(MuonTight, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5, 1) < 0.5);
    } else if (special_mode == 3 || special_mode == 4) {
      muon_idiso_func = bind(MuonTight, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5, 1) > 0.2) && (bind(PF04IsolationVal<Muon>, _1, 0.5, 1) < 0.5);
    } else if (special_mode == 25) {
      muon_idiso_func = (bind(PF04IsolationVal<Muon>, _1, 0.5, 1) >= 0.0);
    } else {
      if (channel == channel::em) {
        muon_idiso_func = bind(MuonMedium, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5, 0) < 0.15);
      } else {
        muon_idiso_func = bind(MuonMedium, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5, 0) < 0.1);
      }
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
  CopyCollection<Muon> vetoMuonCopyCollection("CopyToVetoMuons",muon_label,"vetoMuons");

  SimpleFilter<Muon> vetoMuonFilter = SimpleFilter<Muon>("VetoMuonFilter")
    .set_input_label("vetoMuons")
    .set_predicate(
      bind(MinPtMaxEta, _1, 15.0, 2.4) &&
      bind(fabs, bind(&Muon::dxy_vertex, _1)) < muon_dxy && 
      bind(fabs, bind(&Muon::dz_vertex, _1)) < muon_dz &&
      bind(&Muon::is_global, _1) &&
      bind(PF04IsolationVal<Muon>, _1, 0.5, 1) < 0.3);
  if (strategy == strategy::paper2013){ 
    vetoMuonFilter.set_predicate(
      bind(MinPtMaxEta, _1, 15.0, 2.4) &&
      bind(fabs, bind(&Muon::dxy_vertex, _1)) < muon_dxy && 
      bind(fabs, bind(&Muon::dz_vertex, _1)) < muon_dz &&
      bind(&Muon::is_global, _1) && bind(&Muon::is_tracker, _1) &&
      bind(PF04IsolationVal<Muon>, _1, 0.5, 1) < 0.3);
  } else if(strategy == strategy::phys14){
    vetoMuonFilter.set_predicate(
      bind(MinPtMaxEta, _1, 15.0, 2.4) &&
      bind(fabs, bind(&Muon::dxy_vertex, _1)) < muon_dxy && 
      bind(fabs, bind(&Muon::dz_vertex, _1)) < muon_dz &&
      bind(&Muon::is_global, _1) && bind(&Muon::is_tracker, _1) &&
      bind(PF04IsolationVal<Muon>, _1, 0.5, 0) < 0.3);
 }


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
    .set_reference_label(muon_label)
    .set_min_dr(0.3); 

  SimpleCounter<Muon> extraMuonVeto = SimpleCounter<Muon>("ExtraMuonVeto")
  .set_input_label(muon_label)
  .set_predicate(bind(MinPtMaxEta, _1, 10.0, 2.4) 
              && bind(MuonTight, _1) 
              && bind(PF04Isolation<Muon>, _1, 0.5, 0.3)
              && (bind(fabs, bind(&Muon::dxy_vertex, _1)) < 0.045)
              && (bind(fabs, bind(&Muon::dz_vertex, _1)) < 0.2))
  .set_min(0).set_max((channel == channel::mt || channel == channel::mtmet || channel == channel::em) ? 1 : 0); 
  if(strategy == strategy::phys14) { 
    extraMuonVeto.set_predicate(bind(MinPtMaxEta, _1, 10.0, 2.4)
              && bind(MuonMedium, _1) 
              && bind(PF04IsolationVal<Muon>, _1, 0.5, 0)< 0.3
              && (bind(fabs, bind(&Muon::dxy_vertex, _1)) < 0.045)
              && (bind(fabs, bind(&Muon::dz_vertex, _1)) < 0.2));
  }

  // ------------------------------------------------------------------------------------
  // Tau Modules
  // ------------------------------------------------------------------------------------
  bool real_tau_sample = ( (output_name.find("HToTauTau")             != output_name.npos)
                        || (output_name.find("HTohh")                 != output_name.npos)
                        || (output_name.find("AToZh")                 != output_name.npos)
                        || (output_name.find("DYJetsToTauTau")        != output_name.npos)
                        || (output_name.find("Embedded")              != output_name.npos)
                        || (output_name.find("RecHit")                != output_name.npos) );
  if (output_name.find("DYJetsToTauTau-L") != output_name.npos) real_tau_sample = false;
  if (output_name.find("DYJetsToTauTau-JJ") != output_name.npos) real_tau_sample = false;

  bool correct_es_sample = real_tau_sample;

  TauDzFixer tauDzFixer("TauDzFixer");

  double tau_shift = 1.0;
  if (tau_scale_mode == 1) tau_shift = 0.97;
  if (tau_scale_mode == 2) tau_shift = 1.03;
  if (large_tscale_shift) {
    if (tau_scale_mode == 1) tau_shift = 0.94;
    if (tau_scale_mode == 2) tau_shift = 1.06;
  }
  EnergyShifter<Tau> tauEnergyShifter = EnergyShifter<Tau>
  ("TauEnergyShifter")
    .set_input_label("taus")
    .set_shift(tau_shift);

  HTTEnergyScale httEnergyScale = HTTEnergyScale("HTTEnergyScale")
    .set_input_label("taus")
    .set_shift(tau_shift)
    .set_strategy(strategy)
    .set_moriond_corrections(false);
    if (correct_es_sample) httEnergyScale.set_moriond_corrections(moriond_tau_scale);
  

  SimpleFilter<Tau> tauPtEtaFilter = SimpleFilter<Tau>("TauPtEtaFilter")
    .set_input_label("taus")
    .set_predicate(bind(MinPtMaxEta, _1, tau_pt, tau_eta))
    .set_min(1);

  SimpleFilter<Tau> tauDzFilter = SimpleFilter<Tau>("TauDzFilter")
    .set_input_label("taus")
    .set_predicate(bind(fabs, bind(&Tau::lead_dz_vertex, _1)) < tau_dz)
    .set_min(1);

  std::string tau_iso_discr, tau_anti_elec_discr_1, tau_anti_elec_discr_2, tau_anti_muon_discr;
  if (strategy == strategy::paper2013) {
    if (channel == channel::et || channel == channel::etmet) {
      tau_iso_discr         = "byCombinedIsolationDeltaBetaCorrRaw3Hits";
      //tau_iso_discr         = "byLooseCombinedIsolationDeltaBetaCorr3Hits";
      tau_anti_elec_discr_1 = "againstElectronTightMVA3";
      tau_anti_elec_discr_2 = "againstElectronTightMVA3";
      tau_anti_muon_discr   = "againstMuonLoose";
      // At the moment revert the relaxed Z->ee to the HCP/Moriond approach of
      // just using againstElectronMVA (but need to make a separate special 18 skim)
      if (special_mode == 18) tau_anti_elec_discr_1 = "againstElectronMVA"; 
    }
    if (channel == channel::mt || channel == channel::mtmet) {
      tau_iso_discr         = "byCombinedIsolationDeltaBetaCorrRaw3Hits";
      //tau_iso_discr         = "byLooseCombinedIsolationDeltaBetaCorr3Hits";
      tau_anti_elec_discr_1 = "againstElectronLoose";
      tau_anti_elec_discr_2 = "againstElectronLoose";
      tau_anti_muon_discr   = "againstMuonTight";
    }
  //  if (do_skim) { // For 3hits make wp a bit looser when skimming
  //    tau_iso_discr         = "byCombinedIsolationDeltaBetaCorrRaw3Hits";
  //  }
  } else if (strategy == strategy::phys14) {
    if (channel == channel::et || channel == channel::etmet) {
      tau_iso_discr         = "byCombinedIsolationDeltaBetaCorrRaw3Hits";
      //tau_iso_discr         = "byLooseCombinedIsolationDeltaBetaCorr3Hits";
      tau_anti_elec_discr_1 = "againstElectronTightMVA5";
      tau_anti_elec_discr_2 = "againstElectronTightMVA5";
      tau_anti_muon_discr   = "againstMuonLoose3";
      // At the moment revert the relaxed Z->ee to the HCP/Moriond approach of
      // just using againstElectronMVA (but need to make a separate special 18 skim)
      if (special_mode == 18) tau_anti_elec_discr_1 = "againstElectronMVA"; 
    }
    if (channel == channel::mt || channel == channel::mtmet) {
      tau_iso_discr         = "byCombinedIsolationDeltaBetaCorrRaw3Hits";
      //tau_iso_discr         = "byLooseCombinedIsolationDeltaBetaCorr3Hits";
      tau_anti_elec_discr_1 = "againstElectronVLooseMVA5";
      tau_anti_elec_discr_2 = "againstElectronVLooseMVA5";
      tau_anti_muon_discr   = "againstMuonTight3";
    }
  }

  std::cout << "** Tau Discriminators **" << std::endl;
  std::cout << boost::format(param_fmt) % "isolation" %  tau_iso_discr;
  if(strategy != strategy::paper2013 || !(channel == channel::et || channel == channel::etmet)) std::cout << boost::format(param_fmt) % "anti-electron1" % tau_anti_elec_discr_1;
  if(strategy != strategy::paper2013 || !(channel == channel::et || channel == channel::etmet)) std::cout << boost::format(param_fmt) % "anti-electron2" % tau_anti_elec_discr_2;
  if(strategy == strategy::paper2013 && (channel == channel::et || channel == channel::etmet)) std::cout << boost::format(param_fmt) % "anti-electron1" % "Using optimised WP";
  if(strategy == strategy::paper2013 && (channel == channel::et || channel == channel::etmet)) std::cout << boost::format(param_fmt) % "anti-electron2" % "Using optimised WP";
  std::cout << boost::format(param_fmt) % "anti-muon" % tau_anti_muon_discr;

  TauEfficiency tauEfficiency = TauEfficiency("TauEfficiency")
    .set_fs(fs)
    .set_channel(channel)
    .set_is_data(is_data)
    .set_is_embedded(is_embedded)
    .set_gen_match(true)
    .set_is_fake(false)
    .set_tau_label("taus")
    .set_gen_label(is_embedded ? "genParticlesEmbedded" : "genParticlesTaus");
  if (is_data && special_mode == 3) tauEfficiency.set_is_fake(true);


  SimpleFilter<Tau> tauIsoFilter = SimpleFilter<Tau>("TauIsoFilter")
    .set_input_label("taus")
    .set_predicate((bind(&Tau::GetTauID, _1, tau_iso_discr) > 0.5) && (bind(&Tau::GetTauID, _1, "decayModeFinding") > 0.5))
    .set_min(1);
  if (strategy == strategy::paper2013) {
    double tau_3hit_cut = 1.5;
    if (special_mode == 4 || special_mode == 5) tau_3hit_cut = 10.;
    tauIsoFilter.set_predicate(    (bind(&Tau::GetTauID, _1, tau_iso_discr)       < tau_3hit_cut)
                                && (bind(&Tau::GetTauID, _1, "decayModeFinding")  > 0.5));
  }
  if(strategy == strategy::phys14) {
    double tau_3hit_cut = 1.5;
    if (special_mode == 4 || special_mode == 5) tau_3hit_cut = 10.;
    tauIsoFilter.set_predicate(    (bind(&Tau::GetTauID, _1, tau_iso_discr) < tau_3hit_cut) 
                    && (  (bind(&Tau::GetTauID, _1, "decayModeFinding") > 0.5) || (bind(&Tau::GetTauID, _1, "decayModeFindingNewDMs") > 0.5)  )   );
  }

  SimpleFilter<Tau> tauElRejectFilter = SimpleFilter<Tau>("TauElRejectFilter")
    .set_predicate( (bind(&Tau::GetTauID, _1, tau_anti_elec_discr_1) > 0.5) && (bind(&Tau::GetTauID, _1, tau_anti_elec_discr_2) > 0.5) )                     
    .set_input_label("taus").set_min(1); 
  if ( (channel == channel::et || channel == channel::etmet) && special_mode == 18) tauElRejectFilter
    .set_predicate(bind(&Tau::GetTauID, _1, tau_anti_elec_discr_1) > 0.5);
  if ( (channel == channel::et || channel == channel::etmet) && strategy == strategy::paper2013) tauElRejectFilter
    .set_predicate(bind(passAntiEMVA, _1, 0)); 

  SimpleFilter<Tau> tauMuRejectFilter = SimpleFilter<Tau>("TauMuRejectFilter")
    .set_predicate(bind(&Tau::GetTauID, _1, tau_anti_muon_discr) > 0.5)
    .set_input_label("taus").set_min(1);
  if ( (channel == channel::mt || channel == channel::mtmet) && strategy == strategy::paper2013) tauMuRejectFilter
    .set_predicate((bind(&Tau::GetTauID, _1, tau_anti_muon_discr) > 0.5)
      && (bind(TauEoverP,_1, 0.2)));

                  
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
  if (channel == channel::etmet) pairFilter
    .set_predicate( (bind(&CompositeCandidate::DeltaR, _1, "lepton1","lepton2") > 0.5) && (bind(&CompositeCandidate::PtOf, _1, "lepton1") <= 24.0));

  // ------------------------------------------------------------------------------------
  // Jet Modules
  // ------------------------------------------------------------------------------------  
  OverlapFilter<PFJet, CompositeCandidate> jetLeptonOverlapFilter = OverlapFilter<PFJet, CompositeCandidate>
    ("JetLeptonOverlapFilter")
    .set_input_label(jets_label)
    .set_reference_label("emtauCandidates")
    .set_min_dr(0.5);


  SimpleFilter<PFJet> jetIDFilter = SimpleFilter<PFJet>
    ("JetIDFilter")
    .set_input_label(jets_label)
    .set_predicate((bind(PFJetIDNoHFCut, _1)) && bind(PileupJetID, _1, pu_id_training));

  CopyCollection<PFJet>
    filteredJetCopyCollection("CopyFilteredJets",jets_label,"pfJetsPFlowFiltered");
  
  HhhBJetRegression hhhBJetRegression = HhhBJetRegression
  ("hhhBJetRegression")
  .set_jets_label(jets_label);
   
  // ------------------------------------------------------------------------------------
  // Pair & Selection Modules
  // ------------------------------------------------------------------------------------ 
  HTTPairSelector httPairSelector = HTTPairSelector("HTTPairSelector")
    .set_channel(channel)
    .set_fs(fs)
    .set_met_label(met_label)
    .set_mva_met_from_vector(mva_met_mode == 1)
    .set_faked_tau_selector(faked_tau_selector)
    .set_hadronic_tau_selector(hadronic_tau_selector)
    .set_gen_taus_label(is_embedded ? "genParticlesEmbedded" : "genParticlesTaus")
    .set_scale_met_for_tau((tau_scale_mode > 0 || (moriond_tau_scale && (is_embedded || !is_data) )   ))
    .set_tau_scale(tau_shift)
    .set_allowed_tau_modes(allowed_tau_modes);
  if (channel == channel::em) httPairSelector.set_tau_scale(elec_shift);

  HTTRecoilCorrector httRecoilCorrector = HTTRecoilCorrector("HTTRecoilCorrector")
    .set_sample(output_name)
    .set_channel(channel)
    .set_mc(mc)
    .set_met_label(met_label)
    .set_jets_label(jets_label)
    .set_strategy(strategy)
    //option to take met scale uncertainty from recoil corrector files - off for now as files have over-inflated uncertainties
    //.set_met_scale_mode(metscale_mode)
    .set_w_hack(true);

  //Met scale by hand for now
  HhhMetScale hhhMetScale = HhhMetScale("HhhMetScale")
    .set_met_scale_mode(metscale_mode)
    .set_met_label(met_label)
    .set_scale_shift(0.04);
   
  HTTWeights httWeights = HTTWeights("HTTWeights")
    .set_channel(channel)
    .set_era(era)
    .set_mc(mc)
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
    if (mc == mc::fall11_42X) {
      httWeights.SetWTargetFractions(0.752332, 0.171539, 0.0538005, 0.0159036, 0.00642444);
      httWeights.SetWInputYields(81295381.0, 70712575.0, 25320546.0, 7541595.0, 12973738.0);
    }
    if (mc == mc::summer12_53X && strategy == strategy::paper2013) {
      httWeights.SetWTargetFractions(0.743925, 0.175999, 0.0562617, 0.0168926, 0.00692218);
      httWeights.SetWInputYields(76102995.0, 52926398.0, 64738774.0, 30780647.0, 13382803.0);
    }
  }
  if (output_name.find("DYJets") != output_name.npos && output_name.find("Soup") != output_name.npos) {
    if (mc == mc::summer12_53X) {
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


  QuarkGluonDiscriminatorStudy quarkGluonDiscriminatorStudy = QuarkGluonDiscriminatorStudy
    ("QuarkGluonDiscriminatorStudy")
  .set_fs(fs);  
  
  GenLevelStudy genLevelStudy = GenLevelStudy
    ("GenLevelStudy")
  .set_fs(fs) 
  .set_make_plots(true);
  
  GenLevelStudy genLevelStudyPostAna = GenLevelStudy
    ("GenLevelStudyPostAna")
  .set_fs(fs) 
  .set_check_match(true);
  
  // ------------------------------------------------------------------------------------
  // Category Modules
  // ------------------------------------------------------------------------------------  
  HTTCategories httCategories = HTTCategories("HTTCategories")
    .set_fs(fs)
    .set_channel(channel)
    .set_era(era)
    .set_strategy(strategy)
    .set_ditau_label("emtauCandidates")
    .set_met_label(met_label)
    .set_jets_label(jets_label)
    .set_kinfit_mode(kinfit_mode)
    .set_bjet_regression(bjet_regr_correction)
    .set_write_tree(true);
  if (mass_scale_mode == 1) httCategories.set_mass_shift(1.00);
  if (mass_scale_mode == 2) httCategories.set_mass_shift(1.01);
  if (mass_scale_mode == 3) httCategories.set_mass_shift(1.02);
  if (era == era::data_2012_rereco && strategy == strategy::paper2013) {
    if (mass_scale_mode == 1) httCategories.set_mass_shift(1.00);
    if (mass_scale_mode == 2) httCategories.set_mass_shift(1.01);
    if (mass_scale_mode == 3) httCategories.set_mass_shift(1.02);
  }


  HTTSync httSync("HTTSync","HiggsTauTauSyncfiles/SYNCFILE_" + output_name, channel);
  httSync.set_is_embedded(is_embedded).set_met_label(met_label);

  HTTSyncTemp httSyncTemp("HTTSyncTemp","HiggsTauTauSyncfiles/SYNCFILE_" + output_name, channel);
  httSyncTemp.set_is_embedded(is_embedded).set_met_label(met_label).set_jet_label(jets_label);


  SVFit svfit("SVFit");
  svfit
    .set_outname(output_name)
    .set_op(svfit_mode)
    .set_dilepton_label("emtauCandidates")
    .set_met_label(met_label)
    .set_channel(channel);
  svfit.set_fullpath(svfit_folder);
  svfit.set_split(4000);
  if (svfit_override != "") {
    svfit.set_outname(svfit_override);
  }

  SVFitTest svfitTest("SVFitTest");
  svfitTest
    .set_channel(channel)
    .set_outname(svfit_override == "" ? output_name : svfit_override)
    .set_run_mode(new_svfit_mode)
    .set_fail_mode(1)
    .set_require_inputs_match(false)
    .set_split(7000)
    .set_dilepton_label("emtauCandidates")
    .set_met_label(met_label)
    .set_fullpath(svfit_folder)
    .set_MC(true);

  BTagCheck btagCheck("BTagCheck");
  btagCheck.set_fs(fs);

  // ------------------------------------------------------------------------------------
  // Build Analysis Sequence
  // ------------------------------------------------------------------------------------ 
  auto eventChecker = CheckEvents("EventChecker").set_skip_events(true);
  std::vector<int> to_check =
  {
  };
  for (auto ch : to_check) {
   eventChecker.CheckEvent(ch);
   httPrint.PrintEvent(ch);
  }
  httPrint.set_skip_events(false);
  if (to_check.size() > 0)        analysis.AddModule(&eventChecker);

  if(make_gen_plots) analysis.AddModule(&genLevelStudy);
  if ( (channel == channel::etmet || 
        channel == channel::mtmet)
        && !is_data )             analysis.AddModule(&httL1MetCorrector); 
  if (is_data && !do_skim && strategy != strategy::phys14) analysis.AddModule(&lumiMask);
  if (!is_data && !do_skim && strategy != strategy::phys14) analysis.AddModule(&pileupWeight);
  if (vh_filter_mode > 0)         analysis.AddModule(&vhFilter);
  if (ztautau_mode > 0)           analysis.AddModule(&zTauTauFilter);
  if (!is_data && do_mass_filter) analysis.AddModule(&mssmMassFilter);
  if (tau_scale_mode > 0 && channel != channel::em && !moriond_tau_scale && !do_skim)
                                  analysis.AddModule(&tauEnergyShifter);
  if (tau_scale_mode > 0 && channel == channel::em)         
                                  analysis.AddModule(&electronEnergyShifter);
  if (moriond_tau_scale && channel != channel::em && (!is_data || is_embedded) && !do_skim)          
                                  analysis.AddModule(&httEnergyScale);
  if (to_check.size() > 0)        analysis.AddModule(&httPrint);
  if (is_embedded)                analysis.AddModule(&embeddedMassFilter);

  if (channel == channel::et || channel == channel::etmet) {
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
  if (do_tau_eff) {
                                  analysis.AddModule(&tauElRejectFilter);
                                  analysis.AddModule(&tauMuRejectFilter);
                                  analysis.AddModule(&tauEfficiency);
  }
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
  if (do_tau_eff) {
                                  analysis.AddModule(&tauElRejectFilter);
                                  analysis.AddModule(&tauMuRejectFilter);
                                  analysis.AddModule(&tauEfficiency);
  }
                                  analysis.AddModule(&tauIsoFilter);
                                  analysis.AddModule(&tauElRejectFilter);
                                  analysis.AddModule(&tauMuRejectFilter);
    
                                  analysis.AddModule(&tauMuPairProducer);
                                  analysis.AddModule(&pairFilter);
  }

  if (channel == channel::em) {
    if (strategy == strategy::paper2013) {
                                  analysis.AddModule(&emuExtras);
    }
                                  analysis.AddModule(&selElectronCopyCollection);
                                  analysis.AddModule(&selElectronFilter);
    if (special_mode != 25) {
                                  analysis.AddModule(&elecMuonOverlapFilter);
    }
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
    if (!is_embedded && strategy != strategy::phys14)  { // Don't usually want trigger for embedded
                                  analysis.AddModule(&httTriggerFilter);
    }
    if (is_embedded && strategy == strategy::paper2013 && era == era::data_2012_rereco) {
                                  analysis.AddModule(&httTriggerFilter);
    }
    //                            analysis.AddModule(&runStats);
                                  analysis.AddModule(&httPairSelector);
    if (jes_mode > 0 && !is_data && strategy != strategy::phys14) analysis.AddModule(&jetEnergyUncertainty);
    //                            analysis.AddModule(&jetEnergyCorrections);
                                  analysis.AddModule(&jetIDFilter);
                                  analysis.AddModule(&filteredJetCopyCollection);
                                  analysis.AddModule(&jetLeptonOverlapFilter);
    if(strategy != strategy::phys14) analysis.AddModule(&httRecoilCorrector);
    if(metscale_mode > 0 
      && !is_data )               analysis.AddModule(&hhhMetScale);  

    if (svfit_mode > 0 && !(svfit_override != "" && svfit_mode == 1)) {
                                  analysis.AddModule(&svfit);
    }
    if (!(svfit_override != "" && new_svfit_mode == 1)) {
                                  analysis.AddModule(&svfitTest);
    }
    if (channel == channel::mtmet   // Only apply the L1 MET cut on MC and
      && (!is_data || is_embedded)  // embedded, when not skimming or generating
      && !do_skim  && !make_sync_ntuple        // svfit jobs
      && new_svfit_mode != 1) {
                                  analysis.AddModule(&httL1MetCut);
    }  
   if(strategy != strategy::phys14) analysis.AddModule(&httWeights);
   if (is_embedded && era == era::data_2012_rereco) {
                                  analysis.AddModule(&rechitWeights);
   }
   if (strategy == strategy::paper2013 && channel == channel::em) {
                                  analysis.AddModule(&emuMVA);
//                                  analysis.AddModule(&HhhemuMVA);
//	          		  analysis.AddModule(&HhhemuMVABoth);
   }
   if (strategy == strategy::paper2013 && channel ==channel::mt){
                                  analysis.AddModule(&HhhmtMVABoth);
                                  analysis.AddModule(&HhhmtMVACategory);
   }
   if(bjet_regr_correction) {
                                  analysis.AddModule(&hhhBJetRegression);
   }
    if (quark_gluon_study)        analysis.AddModule(&quarkGluonDiscriminatorStudy);                                 
    if (make_sync_ntuple && strategy==strategy::phys14){
         analysis.AddModule(&httSyncTemp);
    }
    if(make_sync_ntuple && strategy==strategy::paper2013){
         analysis.AddModule(&httSync);
    }
   
    if (!quark_gluon_study)       analysis.AddModule(&httCategories);
                                  //analysis.AddModule(&btagCheck);

  }

  if (do_skim) {
    if (faked_tau_selector > 0)   analysis.AddModule(&httPairSelector);
  }
  if(make_gen_plots) analysis.AddModule(&genLevelStudyPostAna);


  analysis.RunAnalysis();
  delete fs;
  return 0;
}




