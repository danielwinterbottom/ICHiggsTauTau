// C++ Standard Library
#include <iostream>
#include <vector>
#include <string>
// Boost
#include "boost/program_options.hpp"
#include "boost/bind.hpp"
#include "boost/format.hpp"
// ROOT
#include "TSystem.h"
#include "TH1.h"
// CMSSW
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
// CMSSW ICHiggsTauTau
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
// Analysis Framework
#include "Core/interface/AnalysisBase.h"
#include "Utilities/interface/FnRootTools.h"
#include "Utilities/interface/FnPredicates.h"
#include "Modules/interface/CopyCollection.h"
#include "Modules/interface/SimpleFilter.h"
#include "Modules/interface/OverlapFilter.h"
#include "Modules/interface/OneCollCompositeProducer.h"
#include "Modules/interface/PileupWeight.h"
#include "Zbb/interface/ZbbPairSelector.h"
#include "Zbb/interface/ZbbTriggerFilter.h"

// using-directives & namespaces
namespace po = boost::program_options;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using boost::bind;
using boost::format;

int main(int argc, char* argv[]) {
  // Configurable parameters
  string cfg;              // The configuration file
  int max_events;          // Maximum number of events to process
  string filelist;         // The list of input files
  string input_prefix;     // A prefix for each file path
  string output_name;      // Name of the ouput ROOT File
  string output_folder;    // Folder to write the output in

  bool is_data;            // true = data, false = mc
  unsigned btag_mode;      // 0 = no shift, 1 = shift down, 2 = shift up
  unsigned bfake_mode;     // 0 = no shift, 1 = shift down, 2 = shift up
  unsigned jes_mode;       // 0 = no shift, 1 = shift down, 2 = shift up

  // Load the config
  po::options_description preconfig("pre-config");
  preconfig.add_options()("cfg", po::value<string>(&cfg)->required());
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv)
    .options(preconfig)
    .allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("config");
  config.add_options()
      ("max_events",      po::value<int>(&max_events)->default_value(-1))
      ("filelist",        po::value<string>(&filelist)->required())
      ("input_prefix",    po::value<string>(&input_prefix)->default_value(""))
      ("output_name",     po::value<string>(&output_name)->required())
      ("output_folder",   po::value<string>(&output_folder)->default_value(""))
      ("is_data",         po::value<bool>(&is_data)->required())
      ("btag_mode",       po::value<unsigned>(&btag_mode)->default_value(0))
      ("bfake_mode",      po::value<unsigned>(&bfake_mode)->default_value(0))
      ("jes_mode",        po::value<unsigned>(&jes_mode)->default_value(0));
  po::store(po::command_line_parser(argc, argv)
    .options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
  po::notify(vm);

  // Some options must now be re-configured based on other options
  if (btag_mode == 1) output_folder += "BTAG_DOWN/";
  if (btag_mode == 2) output_folder += "BTAG_UP/";
  if (bfake_mode == 1) output_folder += "BFAKE_DOWN/";
  if (bfake_mode == 2) output_folder += "BFAKE_UP/";
  if (jes_mode == 1) output_folder += "JES_DOWN/";
  if (jes_mode == 2) output_folder += "JES_UP/";

  string param_fmt = "%-25s %-40s\n";
  cout << "-------------------------------------" << std::endl;
  cout << "Zbb Analysis" << std::endl;
  cout << "-------------------------------------" << std::endl;
  cout << format(param_fmt) % "max_events" % max_events;
  cout << format(param_fmt) % "output" % (output_folder+output_name);
  cout << format(param_fmt) % "is_data" % is_data;

  // Load necessary libraries for ROOT I/O of custom classes
  gSystem->Load("libFWCoreFWLite.dylib");
  gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  AutoLibraryLoader::enable();

  // Build a vector of input files
  vector<string> files = ic::ParseFileLines(filelist);
  for (auto & f : files) f = input_prefix + f;

  // Create ROOT output fileservice
  // fwlite::TFileService *fs =
  //  new fwlite::TFileService((output_folder+output_name).c_str());

  // Create analysis object
  ic::AnalysisBase analysis(
    "Zbb",        // Analysis name
    files,                // Input files
    "icEventProducer",    // TTree path
    "EventTree",          // TTree name
    max_events);          // Max. events to process (-1 = all)
  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);

  // Define modules
  TH1D data_pu  = ic::GetFromTFile<TH1D>
    ("data/Pileup_2011_to_173692_CD111018.root", "/", "pileup");
  TH1D mc_pu    = ic::GetFromTFile<TH1D>
    ("data/Summer11_PU_S4_spikesmear.root", "/", "pileup");
  auto pileup_weight = ic::PileupWeight("PileupWeight")
    .set_data(&data_pu)
    .set_mc(&mc_pu)
    .set_print_weights(false);

  // Trigger filter

  // Jet energy scale uncertainty

  auto copy_elecs = ic::CopyCollection<ic::Electron>("CopyElectrons",
    "electrons", "selected_electrons");

  auto copy_muons = ic::CopyCollection<ic::Muon>("CopyMuons",
    "muonsPFlow", "selected_muons");

  auto select_elecs = ic::SimpleFilter<ic::Electron>("SelectElectrons")
    .set_input_label("selected_electrons")
    .set_predicate([](ic::Electron const* ele) {
      return ic::MinPtMaxEta(ele, 25.0, 2.5) &&
        !ic::InEcalGap(ele) &&
        std::fabs(ele->dxy_beamspot()) < 0.02 &&
        ic::ElectronSimpleWP85ID(ele) &&
        ic::ElectronSimpleWP85Iso(ele);
    });

  auto select_muons = ic::SimpleFilter<ic::Muon>("SelectMuons")
    .set_input_label("selected_muons")
    .set_predicate([](ic::Muon const* muo) {
      return ic::MinPtMaxEta(muo, 20.0, 2.1) &&
        std::fabs(muo->dxy_beamspot()) < 0.02 &&
        ic::MuonTight(muo) &&
        ic::MuonIso(muo);
    });

  auto elec_pairs =
    ic::OneCollCompositeProducer<ic::Electron>("ProduceElectronPairs")
    .set_input_label("selected_electrons")
    .set_candidate_name_first("elec1")
    .set_candidate_name_second("elec2")
    .set_output_label("electron_pairs");

  auto muon_pairs =
    ic::OneCollCompositeProducer<ic::Muon>("ProduceMuonPairs")
    .set_input_label("selected_muons")
    .set_candidate_name_first("muon1")
    .set_candidate_name_second("muon2")
    .set_output_label("muon_pairs");

  auto elec_pair_filter =
    ic::SimpleFilter<ic::CompositeCandidate>("FilterElectronPairs")
    .set_input_label("electron_pairs")
    .set_predicate((bind(&ic::CompositeCandidate::charge, _1) == 0));

  auto muon_pair_filter =
    ic::SimpleFilter<ic::CompositeCandidate>("FilterMuonPairs")
    .set_input_label("muon_pairs")
    .set_predicate((bind(&ic::CompositeCandidate::charge, _1) == 0));

  auto trigger_filter =
    ic::ZbbTriggerFilter("TriggerFilter")
    .set_elec_pairs("electron_pairs")
    .set_muon_pairs("muon_pairs");

  auto pair_selector = ic::ZbbPairSelector("PairSelector")
    .set_elec_pairs("electron_pairs")
    .set_muon_pairs("muon_pairs")
    .set_output_label("lepton_pair");

  auto copy_jets = ic::CopyCollection<ic::PFJet>("CopyJets",
    "pfJetsPFlow", "selected_jets");

  auto select_jets = ic::SimpleFilter<ic::PFJet>("SelectJets")
    .set_input_label("selected_jets")
    .set_predicate([](ic::PFJet const* jet) {
      return ic::MinPtMaxEta(jet, 25.0, 2.1) &&
        ic::PFJetIDNoHFCut(jet);
    });

  auto jet_overlap = ic::OverlapFilter<ic::PFJet, ic::CompositeCandidate>
    ("JetLeptonOverlapFilter")
    .set_input_label("selected_jets")
    .set_reference_label("lepton_pair")
    .set_min_dr(0.5);

  if (!is_data) analysis.AddModule(&pileup_weight);
  analysis.AddModule(&copy_elecs);
  analysis.AddModule(&copy_muons);
  analysis.AddModule(&select_elecs);
  analysis.AddModule(&select_muons);
  analysis.AddModule(&elec_pairs);
  analysis.AddModule(&muon_pairs);
  analysis.AddModule(&elec_pair_filter);
  analysis.AddModule(&muon_pair_filter);
  if (is_data) analysis.AddModule(&trigger_filter);
  analysis.AddModule(&pair_selector);
  analysis.AddModule(&copy_jets);
  analysis.AddModule(&select_jets);
  analysis.AddModule(&jet_overlap);

  analysis.RunAnalysis();
//   delete fs;
  return 0;
}
