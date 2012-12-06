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
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttDataTriggerFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttPairSelector.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttWeights.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttSelection.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttMetStudy.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttMCTriggerFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttSync.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttPrint.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MakeRunStats.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/EnergyShifter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/SVFit.h"

#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/ZJetsControlPlots.h"

#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/JetEnergyCorrections.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/LumiMask.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

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
  string filelist;                // The list of files to use as input
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
  // bool is_embedded;               // true = embedded, false = not an embedded sample
  // unsigned special_mode;          // 0 = normal processing, > 0 (see below)
  // unsigned tau_scale_mode;        // 0 = no shift, 1 = shift down, 2 = shift up
  // unsigned svfit_mode;            // 0 = not run, 1 = generate jobs, 2 = read-in job output
  // string svfit_folder;            // Folder containing svfit jobs & output
  // string svfit_override;          // Override the svfit results to use
  // unsigned ztatau_mode;           // 0 = not run, 1 = select Z->tautau, 2 = select Z->ee and Z->mumu
  // unsigned faked_tau_selector;    // 0 = not run, 1 = tau matched to gen. lepton, 2 = tau not matched to lepton
  unsigned mva_met_mode;          // 0 = standard mva met, 1 = mva met from vector (only when mva met is being used)
  // bool make_sync_ntuple;          // Generate a sync ntuple

  // bool do_vbf_mva = true;
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
  // Speical Mode 14 Fake Electron Trigger Weights 
  // Speical Mode 15 Fake Muon Trigger Weights 
  // Speical Mode 17 Fake Muon Trigger Weights 
  // Special Mode 18 - relax tau e rejection for ZL shape

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
      // ("is_embedded",         po::value<bool>(&is_embedded)->default_value(false))
      // ("special_mode",        po::value<unsigned>(&special_mode)->default_value(0))
      // ("tau_scale_mode",      po::value<unsigned>(&tau_scale_mode)->default_value(0))
      // ("svfit_mode",          po::value<unsigned>(&svfit_mode)->default_value(0))
      // ("svfit_folder",        po::value<string>(&svfit_folder)->default_value(""))
      // ("svfit_override",      po::value<string>(&svfit_override)->default_value(""))
      // ("ztautau_mode",        po::value<unsigned>(&ztatau_mode)->default_value(0))
      // ("faked_tau_selector",  po::value<unsigned>(&faked_tau_selector)->default_value(0))
      ("mva_met_mode",        po::value<unsigned>(&mva_met_mode)->default_value(1));
      // ("make_sync_ntuple",    po::value<bool>(&make_sync_ntuple)->default_value(false));
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

  std::cout << "**** Z+Jets Analysis *****" << std::endl;
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
  // std::cout << boost::format(param_fmt) % "is_embedded" % is_embedded;
  // std::cout << boost::format(param_fmt) % "special_mode" % special_mode;
  // std::cout << boost::format(param_fmt) % "tau_scale_mode" % tau_scale_mode;
  // std::cout << boost::format(param_fmt) % "svfit_mode" % svfit_mode;
  // if (svfit_mode > 0) {
  //   std::cout << boost::format(param_fmt) % "svfit_folder" % svfit_folder;
  //   std::cout << boost::format(param_fmt) % "svfit_override" % svfit_override;
  // }
  // std::cout << boost::format(param_fmt) % "ztatau_mode" % ztatau_mode;
  // std::cout << boost::format(param_fmt) % "faked_tau_selector" % faked_tau_selector;
   std::cout << boost::format(param_fmt) % "mva_met_mode" % mva_met_mode;
  // std::cout << boost::format(param_fmt) % "make_sync_ntuple" % make_sync_ntuple;

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
  double elec_dz, muon_dz, elec_dxy, muon_dxy;
  double elec_pt, elec_eta, muon_pt, muon_eta;
  string elec_label, muon_label, jet_label;

  elec_label = "electrons";
  muon_label = "muonsPFlow";
  jet_label = "pfJetsPFlow";
  elec_dz = 0.2;
  elec_dxy = 0.045;
  muon_dz = 0.2;
  muon_dxy = 0.045;
  elec_pt = 20.0;
  elec_eta = 2.1;
  muon_pt = 20.0;
  muon_eta = 2.1;

  std::cout << "** Kinematics **" << std::endl;
  std::cout << boost::format(param_fmt) % "elec_pt" % elec_pt;
  std::cout << boost::format(param_fmt) % "elec_eta" % elec_eta;
  std::cout << boost::format(param_fmt) % "elec_dxy" % elec_dxy;
  std::cout << boost::format(param_fmt) % "elec_dz" % elec_dz;
  std::cout << boost::format(param_fmt) % "muon_pt" % muon_pt;
  std::cout << boost::format(param_fmt) % "muon_eta" % muon_eta;
  std::cout << boost::format(param_fmt) % "muon_dxy" % muon_dxy;
  std::cout << boost::format(param_fmt) % "muon_dz" % muon_dz;


  // Create analysis object
  ic::AnalysisBase analysis(
    "ZJets",              // Analysis name
    files,                // Input files
    "icEventProducer",    // TTree path
    "EventTree",          // TTree name
    max_events);          // Max. events to process (-1 = all)
  if (do_skim && skim_path != "") analysis.DoSkimming(skim_path);

  // ------------------------------------------------------------------------------------
  // Misc Modules
  // ------------------------------------------------------------------------------------

  string data_json;
  if (era == era::data_2011) data_json           =  "data/json/data_2011.txt";
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
  if (era == era::data_2012_ichep) data_pu_file     =  "data/pileup/Data_Pileup_2012.root";
  if (era == era::data_2012_hcp) data_pu_file       =  "data/pileup/Data_Pileup_2012_HCP-600bins.root";
  if (era == era::data_2012_moriond) data_pu_file   =  "data/pileup/Data_Pileup_2012_HCP-600bins.root";
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

  // HttDataTriggerFilter dataTriggerPathFilter = HttDataTriggerFilter("TriggerPathFilter")
  //   .set_channel(channel)
  //   .set_do_obj_match(true)
  //   .set_lep1label(lep1sel_label)
  //   .set_lep2label(lep2sel_label);

  // HttMCTriggerFilter mcTriggerPathFilter = HttMCTriggerFilter("TriggerPathMCFilter")
  //   .set_channel(channel)   
  //   .set_mc(mc)
  //   .set_do_obj_match(true)
  //   .set_lep1label(lep1sel_label)
  //   .set_lep2label(lep2sel_label);

  // SimpleCounter<GenParticle> zTauTauFilter = SimpleCounter<GenParticle>("ZToTauTauSelector")
  //   .set_input_label("genParticles")
  //   .set_predicate(
  //     (bind(&GenParticle::status, _1) == 3) && 
  //     (bind(abs,(bind(&GenParticle::pdgid, _1))) == 15))
  //   .set_min(2);
  // if (ztatau_mode == 2) zTauTauFilter.set_min(0).set_max(0);

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

  boost::function<bool (Electron const*)> elec_idiso_func;
  elec_idiso_func = bind(ElectronHTTId, _1, false) && (bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1);
  
  SimpleFilter<Electron> selElectronFilter = SimpleFilter<Electron>("SelElectronFilter")
    .set_input_label(elec_label)
    .set_predicate(
      bind(MinPtMaxEta, _1, elec_pt, elec_eta) &&
      bind(fabs, bind(&Electron::dxy_vertex, _1)) < elec_dxy &&
      bind(fabs, bind(&Electron::dz_vertex, _1)) < elec_dz &&
      bind(elec_idiso_func, _1))
    .set_min(2);
  
  // ------------------------------------------------------------------------------------
  // Muon Modules
  // ------------------------------------------------------------------------------------
  boost::function<bool (Muon const*)> muon_idiso_func;
  muon_idiso_func = bind(MuonTight, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1);
 
  SimpleFilter<Muon> selMuonFilter = SimpleFilter<Muon>("SelMuonFilter")
    .set_input_label(muon_label)
    .set_predicate(
      bind(MinPtMaxEta, _1, muon_pt, muon_eta) && 
      bind(fabs, bind(&Muon::dxy_vertex, _1)) < muon_dxy &&
      bind(fabs, bind(&Muon::dz_vertex, _1)) < muon_dz &&
      bind(muon_idiso_func, _1))
    .set_min(2);


  // ------------------------------------------------------------------------------------
  // e/m + tau Pair Modules
  // ------------------------------------------------------------------------------------  
  OneCollCompositeProducer<Electron> electronPairProducer = OneCollCompositeProducer<Electron>("PairProducer")
    .set_input_label(elec_label)
    .set_candidate_name_first("lep1")
    .set_candidate_name_second("lep2")
    .set_output_label("dileptons");

  OneCollCompositeProducer<Muon> muonPairProducer = OneCollCompositeProducer<Muon>("PairProducer")
    .set_input_label(muon_label)
    .set_candidate_name_first("lep1")
    .set_candidate_name_second("lep2")
    .set_output_label("dileptons");

  // SimpleFilter<CompositeCandidate> pairFilter = SimpleFilter<CompositeCandidate>("PairFilter")
  //   .set_input_label("emtauCandidates")
  //   .set_predicate( (bind(&CompositeCandidate::DeltaR, _1, "lepton1","lepton2") > 0.5))
  //   .set_min(1)
  //   .set_max(999);    
  // if (channel == channel::em) pairFilter
  //   .set_predicate( (bind(PairOneWithPt, _1, 20.0)) && (bind(&CompositeCandidate::DeltaR, _1, "lepton1","lepton2") > 0.3));

  SimpleFilter<CompositeCandidate> pairFilter = SimpleFilter<CompositeCandidate>("PairFilter")
    .set_predicate( bind(PairMassInRange, _1, 60.0, 120.0) && (bind(&CompositeCandidate::charge, _1) == 0) )
    .set_input_label("dileptons")
    .set_min(1)
    .set_max(1);       
  // ------------------------------------------------------------------------------------
  // Jet Modules
  // ------------------------------------------------------------------------------------  
  OverlapFilter<PFJet, CompositeCandidate> jetLeptonOverlapFilter = OverlapFilter<PFJet, CompositeCandidate>
    ("JetLeptonOverlapFilter")
    .set_input_label(jet_label)
    .set_reference_label("dileptons")
    .set_min_dr(0.5);

  SimpleFilter<PFJet> jetIDFilter = SimpleFilter<PFJet>
    ("JetIDFilter")
    .set_input_label(jet_label)
    .set_predicate((bind(PFJetID, _1)) && bind(&PFJet::pu_id_mva_loose, _1));

  // ------------------------------------------------------------------------------------
  // Pair & Selection Modules
  // ------------------------------------------------------------------------------------  
 ZJetsControlPlots zjetsControlPlots = ZJetsControlPlots("ZJetsControlPlots")
  .set_fs(fs)
  .set_channel(channel);

  // HttPairSelector httPairSelector = HttPairSelector("HttPairSelector")
  //   .set_channel(channel)
  //   .set_fs(fs)
  //   .set_met_label(met_label)
  //   .set_mva_met_from_vector(mva_met_mode == 1)
  //   .set_faked_tau_selector(faked_tau_selector)
  //   .set_scale_met_for_tau(tau_scale_mode > 0)
  //   .set_tau_scale(tau_shift);

  // HttWeights httWeights = HttWeights("HttWeights")
  //   .set_channel(channel)
  //   .set_era(era)
  //   .set_mc(mc)
  //   .set_trg_applied_in_mc(true)
  //   .set_do_trg_weights(false)
  //   .set_do_etau_fakerate(false)
  //   .set_do_idiso_weights(false)
  //   .set_do_emu_e_fakerates(false)
  //   .set_do_emu_m_fakerates(false)
  //   .set_do_top_factors(false)
  //   .set_do_btag_weight(false);
  // if (!is_data) {
  //   httWeights.set_do_trg_weights(true).set_trg_applied_in_mc(true).set_do_idiso_weights(true);
  //   httWeights.set_do_btag_weight(true);
  // }
  // if (output_name.find("DYJetsToLL") != output_name.npos && channel == channel::et) httWeights.set_do_etau_fakerate(true);
  // if (is_embedded) httWeights.set_do_trg_weights(true).set_trg_applied_in_mc(false).set_do_idiso_weights(false);
  // if (special_mode == 14 || special_mode == 17) httWeights.set_do_emu_e_fakerates(true);
  // if (special_mode == 15 || special_mode == 17) httWeights.set_do_emu_m_fakerates(true);
  // //if (outname.find("TTJets") != outname.npos && mode == 2 && era == 0) httWeights.set_do_top_factors(true);
  // if (output_name.find("WJetsToLNuSoup") != output_name.npos) {
  //   httWeights.set_do_w_soup(true);
  //   if (mc == mc::fall11_42X) {
  //     httWeights.SetWTargetFractions(0.752332, 0.171539, 0.0538005, 0.0159036, 0.00642444);
  //     httWeights.SetWInputYields(81295381.0, 70712575.0, 25320546.0, 7541595.0, 12973738.0);
  //   }
  //   if (mc == mc::summer12_53X) {
  //     httWeights.SetWTargetFractions(0.743925, 0.175999, 0.0562617, 0.0168926, 0.00692218);
  //     httWeights.SetWInputYields(76102995.0, 23101598.0, 33884921.0, 15539503.0, 13382803.0);
  //   }
  // }
  // if (mc == mc::fall11_42X && output_name.find("GluGluToHToTauTau_M-") != output_name.npos && output_name.find("SUSYGluGluToHToTauTau_M-") == output_name.npos) {
  //   std::size_t pos = output_name.find("_M-");
  //   if (pos != output_name.npos) {
  //     std::string mass_string;
  //     for (unsigned i = (pos+3); i < output_name.size(); ++i) {
  //       if (output_name.at(i) != '_') mass_string += output_name.at(i);
  //       if (output_name.at(i) == '_') break;
  //     }
  //     std::cout << "SM ggH Signal sample detected with mass: " << mass_string << std::endl;
  //     httWeights.set_ggh_mass(mass_string);
  //   }
  // }


  // HttSelection httSelection = HttSelection("HttSelection")
  //   .set_fs(fs)
  //   .set_channel(channel)
  //   .set_met_label(met_label);
  // httSelection.set_mt_max_selection(mt_max_selection);
  // if (special_mode == 5 || special_mode == 12) httSelection.set_distinguish_os(false);
  // if (special_mode == 7) httSelection.set_mt_min_control(60.0).set_mt_max_control(120.0);

  // HttMetStudy httMetStudy = HttMetStudy
  //   ("HttMetStudy")
  //   .set_fs(fs)
  //   .set_mode(mode)
  //   .set_met_label(met_label);

  
  // ------------------------------------------------------------------------------------
  // Build Analysis Sequence
  // ------------------------------------------------------------------------------------  
  //analysis.AddModule(&httPrint);
  //analysis.AddModule(&wjetsWeights);
  if (is_data) analysis.AddModule(&lumiMask);
  //if (is_data && !is_2012 && (mode == 0 || mode == 1)) analysis.AddModule(&runFilter2011);
  if (!is_data) analysis.AddModule(&pileupWeight);
  // if (ztatau_mode > 0) analysis.AddModule(&zTauTauFilter);

  if (channel == channel::zee) {
    analysis.AddModule(&selElectronFilter);
    // if (is_data  && !do_skim && !is_embedded) {
    //   analysis.AddModule(&dataTriggerPathFilter);
    //   //analysis.AddModule(&runStats);
    // }
    // if (!is_data  && !do_skim) analysis.AddModule(&mcTriggerPathFilter);
    analysis.AddModule(&electronPairProducer);
  }


  if (channel == channel::zmm) {
    analysis.AddModule(&selMuonFilter);
    // if (is_data && !do_skim && !is_embedded) {
    //   analysis.AddModule(&dataTriggerPathFilter);
    //   //analysis.AddModule(&runStats);
    // }
    // if (!is_data  && !do_skim) analysis.AddModule(&mcTriggerPathFilter);
    analysis.AddModule(&muonPairProducer);
  }

  if (!do_skim) {
    // analysis.AddModule(&jetEnergyCorrections);
    analysis.AddModule(&pairFilter);
    analysis.AddModule(&jetIDFilter);
    analysis.AddModule(&jetLeptonOverlapFilter);
    analysis.AddModule(&zjetsControlPlots);
  }

  // Run analysis
  analysis.RunAnalysis();
  delete fs;
  return 0;
}




