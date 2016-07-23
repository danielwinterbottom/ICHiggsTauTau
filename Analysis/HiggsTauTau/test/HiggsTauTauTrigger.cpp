#include <iostream>
#include <fstream>
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
#include "HiggsTauTau/interface/HTTWeights.h"
#include "HiggsTauTau/interface/HTTPrint.h"
#include "Modules/interface/MakeRunStats.h"
#include "Modules/interface/EnergyShifter.h"
#include "Modules/interface/JetEnergyCorrections.h"
#include "HiggsTauTau/interface/JetEnergyUncertainty.h"
#include "HiggsTauTau/interface/HTTConfig.h"
#include "HiggsTauTau/interface/HTTEnergyScale.h"
#include "HiggsTauTau/interface/TauDzFixer.h"
#include "HiggsTauTau/interface/HTTL1MetCorrector.h"
#include "HiggsTauTau/interface/HTTL1MetCut.h"
#include "HiggsTauTau/interface/TauEfficiency.h"
#include "HiggsTauTau/interface/EmbeddingKineReweightProducer.h"
#include "HiggsTauTau/interface/HhhMetScale.h"
#include "Modules/interface/L1VariableHistograms.h"
#include "Modules/interface/VariableHistograms.h"
#include "Modules/interface/Efficiency.h"
#include "Modules/interface/EfficiencyGenMatch.h"
#include "Modules/interface/L1TFilterPlots.h"
#include "Modules/interface/L1TFilter.h"
#include "Modules/interface/TT_L1TFilter.h"
#include "Modules/interface/VBFFilter.h"
#include "Modules/interface/VBFPlots.h"
#include "Modules/interface/GenChannelFilter.h"
#include "Modules/interface/FilterGoodEvents.h"

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
  string skim_path = "";          // Local folder where skimmed ntuples should be written

  string strategy_str;            // Analysis strategy
  string era_str;                 // Analysis data-taking era
  string mc_str;                  // Analysis MC production
  string channel_str;             // Analysis channel

  bool is_data;                   // true = data, false = mc         
  unsigned tau_scale_mode;        // 0 = no shift, 1 = shift down, 2 = shift up
  unsigned jes_mode;              // 0 = no shift, 1 = shift down, 2 = shift up
  unsigned l1met_mode;              // 0 = no shift, 1 = shift down, 2 = shift up
  unsigned metscale_mode;          // 0 = no shift, 1 = shift down, 2 = shift up
  unsigned mass_scale_mode;       // 0 = no shift, 1 = nominal, but in TSCALE_DOWN, 2 = shift up, 3 = shift up again, in TSCALE_UP
  unsigned kinfit_mode;           // 0 = don't run, 1 = run simple 125,125 default fit, 2 = run extra masses default fit, 3 = run m_bb only fit
  unsigned faked_tau_selector;    // 0 = not run, 1 = tau matched to gen. lepton, 2 = tau not matched to lepton
  unsigned hadronic_tau_selector;    // 0 = not run, 1 = tau matched to gen. lepton, 2 = tau not matched to lepton
  unsigned mva_met_mode;          // 0 = standard mva met, 1 = mva met from vector (only when mva met is being used)
  bool bjet_regr_correction;      // Apply b jet regression corrections
  string allowed_tau_modes;       // "" means all, otherwise "1,10"=allow 1prong1pizero,3prong
  bool moriond_tau_scale;         // Use new central tau scale shifts
  bool large_tscale_shift;        // Shift tau energy scale by +/- 6% instead of 3%
  unsigned pu_id_training;        // Pileup jet id training
  unsigned inputnum;
  unsigned isZeroBias;
  string L1_infile_name;
  unsigned makeEffPlots;
  unsigned doAdditionalVBF;
  unsigned doL1;
  unsigned version_number;
  unsigned isDY;
  unsigned doTTJets;
  string L1_Muon_branch = "L1Muon";
  string tree_name;

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
      ("strategy",            po::value<string>(&strategy_str)->required())
      ("era",                 po::value<string>(&era_str)->required())
      ("mc",                  po::value<string>(&mc_str)->required())
      ("channel",             po::value<string>(&channel_str)->required())
      ("is_data",             po::value<bool>(&is_data)->required())
      ("tau_scale_mode",      po::value<unsigned>(&tau_scale_mode)->default_value(0))
      ("jes_mode",            po::value<unsigned>(&jes_mode)->default_value(0))
      ("l1met_mode",          po::value<unsigned>(&l1met_mode)->default_value(0))
      ("metscale_mode",       po::value<unsigned>(&metscale_mode)->default_value(0))
      ("mass_scale_mode",     po::value<unsigned>(&mass_scale_mode)->default_value(0))
      ("kinfit_mode",         po::value<unsigned>(&kinfit_mode)->default_value(0))
      ("faked_tau_selector",  po::value<unsigned>(&faked_tau_selector)->default_value(0))
      ("hadronic_tau_selector",  po::value<unsigned>(&hadronic_tau_selector)->default_value(0))
      ("mva_met_mode",        po::value<unsigned>(&mva_met_mode)->default_value(1))
      ("bjet_regr_correction",po::value<bool>(&bjet_regr_correction)->default_value(false))
      ("moriond_tau_scale",   po::value<bool>(&moriond_tau_scale)->default_value(false))
      ("large_tscale_shift",  po::value<bool>(&large_tscale_shift)->default_value(false))
      ("allowed_tau_modes",   po::value<string>(&allowed_tau_modes)->default_value(""))
      ("pu_id_training",      po::value<unsigned>(&pu_id_training)->default_value(1))
      ("line",                po::value<unsigned>(&inputnum)->default_value(1))
      ("zb",                  po::value<unsigned>(&isZeroBias)->default_value(0))
      ("MakeEffPlots",        po::value<unsigned>(&makeEffPlots)->default_value(0))
      ("DY",                  po::value<unsigned>(&isDY)->default_value(0))
      ("offlineVBFCuts",      po::value<unsigned>(&doAdditionalVBF)->default_value(0))
      ("doL1Selection",       po::value<unsigned>(&doL1)->default_value(1))
      ("doTTJets",            po::value<unsigned>(&doTTJets)->default_value(0))
      ("l1input",             po::value<string>(&L1_infile_name)->default_value("InputTriggers.txt"))
      ("version",             po::value<unsigned>(&version_number)->default_value(5))
      ("tree",                po::value<string>(&tree_name)->default_value("EventTree"));
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
  po::notify(vm);

  // Some options must now be re-configured based on other options
  ic::strategy strategy = String2Strategy(strategy_str);
  ic::era era           = String2Era(era_str);
  ic::mc mc             = String2MC(mc_str);
  ic::channel channel   = String2Channel(channel_str);

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "HiggsToTauTau Analysis" << std::endl;
  std::cout << "-------------------------------------" << std::endl;      string param_fmt = "%-25s %-40s\n";
  std::cout << boost::format(param_fmt) % "max_events" % max_events;
  std::cout << boost::format(param_fmt) % "output" % (output_folder+output_name);
  std::cout << boost::format(param_fmt) % "strategy" % strategy_str;
  std::cout << boost::format(param_fmt) % "era" % era_str;
  std::cout << boost::format(param_fmt) % "mc" % mc_str;
  std::cout << boost::format(param_fmt) % "channel" % channel_str;
  std::cout << boost::format(param_fmt) % "is_data" % is_data;


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
  double lead_tau_pt = 20;
  met_label = "pfMVAMet";
  
  L1Cuts l1Cuts;
  
  if(era == era::trigger_2016){
      
    std::ifstream L1_infile;
    L1_infile.open (L1_infile_name);
    unsigned i = 1;
    double in[16];
    while(L1_infile >> in[0] >> in[1] >> in[2] >> in[3] >> in[4] >> in[5] >> in[6] >> in[7] >> in[8] >> in[9] >> in[10] >> in[11] >> in[12] >> in[13] >> in[14] >> in[15]){
      if(i == inputnum){
        l1Cuts.EGPt = in[0];
        l1Cuts.MuPt = in[1];
        l1Cuts.Tau1Pt = in[2];
        l1Cuts.Tau2Pt = in[3];
        if(in[4] == 1) l1Cuts.IsoEG = true;
        else l1Cuts.IsoEG = false;
        if(in[5] == 1) l1Cuts.IsoMu = true;
        else l1Cuts.IsoMu = false;
        if(in[6] == 1) l1Cuts.IsoTau = true;
        else l1Cuts.IsoTau = false;
        l1Cuts.Jet1Pt = in[7];
        l1Cuts.Jet2Pt = in[8];
        l1Cuts.Mjj = in[9];
        l1Cuts.DeltaEta = in[10];
        if(in[11] == 1) l1Cuts.JetFilter = true;
        else l1Cuts.JetFilter = false;
        l1Cuts.AvePt = in[12];
        l1Cuts.VecPt = in[13];
        if(in[14] == 1) l1Cuts.DeltaRFilter = true;
        else l1Cuts.DeltaRFilter = false;
        l1Cuts.Ht = in[15];
        break;
      }
      i++;
    }
      
  }
  
  OfflineCuts offlineCuts;
  
  offlineCuts.ElPt = std::max(l1Cuts.EGPt + 3., 5.);
  offlineCuts.MuPt = std::max(l1Cuts.MuPt + 2., 5.);
  offlineCuts.Tau1Pt = std::max(l1Cuts.Tau1Pt + 10., 20.);
  offlineCuts.Tau2Pt = std::max(l1Cuts.Tau2Pt + 10., 20.);

  offlineCuts.Mjj = 500.;
  offlineCuts.Jet1Pt = 30.;
  offlineCuts.Jet2Pt = 30.;
  offlineCuts.DeltaEta = 3.5;
  if(doAdditionalVBF == 1){
      offlineCuts.Jet1Pt = std::max(l1Cuts.Jet1Pt + 10., 30.);
      offlineCuts.Jet2Pt = std::max(l1Cuts.Jet2Pt + 10., 30.);
      offlineCuts.DeltaEta = std::max(l1Cuts.DeltaEta + 0.25, 3.5);
      offlineCuts.Mjj = std::max(l1Cuts.Mjj + 50., 500.);
      if(l1Cuts.AvePt > 0) offlineCuts.AvePt = l1Cuts.AvePt + 10.;
      if(l1Cuts.VecPt > 0) offlineCuts.VecPt = l1Cuts.VecPt + 10.;
  }

  if (channel == channel::et) {
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
      elec_pt = 24.0;
      elec_eta = 2.1;
      muon_pt = 18.0;
      muon_eta = 2.1;
    }
    tau_pt = 20.0;

    tau_eta = 2.3;
    if (channel == channel::etmet) elec_pt = 13.0;
    
    if (era == era::trigger_2016) {
      
      elec_pt = offlineCuts.ElPt;
      muon_pt = 18;
      muon_eta = 2.4;
      elec_eta = 2.4;
      tau_pt = offlineCuts.Tau1Pt;
      tau_eta = 2.3;
  }
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
      muon_pt = 19.0;
      muon_eta = 2.1;
    }
    tau_pt = 20.0;
    tau_eta = 2.3;
    
    if (channel == channel::mtmet) muon_pt = 9.0;
    
    if (era == era::trigger_2016) {
      elec_pt = 23.0;
      muon_pt = offlineCuts.MuPt;
      muon_eta = 2.4;
      elec_eta = 2.4; 
      tau_pt = offlineCuts.Tau1Pt;
      tau_eta = 2.3;
  }
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
      muon_pt = 10;
      muon_eta = 2.4;
    }
    if (era == era::trigger_2016) {
      elec_pt = offlineCuts.ElPt;
      muon_pt = offlineCuts.MuPt;
      muon_eta = 2.4;
      elec_eta = 2.4; 
      tau_pt = 20;
      tau_eta = 2.3;
    }
  }
  

  if (channel == channel::tt){
    //tau_pt = 45;
    tau_pt = 40;
    lead_tau_pt =40;
    tau_eta = 2.3;
    tau_dz = 0.2;
    elec_pt = 23;
    elec_eta = 2.5;
    muon_pt = 18.0;
    muon_eta = 2.1;
    elec_dz = 0.2;
    elec_dxy = 0.045;
    muon_dz = 0.2;
    muon_dxy = 0.045;
    if (era == era::trigger_2016) {
      elec_pt = 23;
      muon_eta = 2.4;
      elec_eta = 2.4;
      muon_pt = 18;
      tau_pt = offlineCuts.Tau2Pt;
      lead_tau_pt = offlineCuts.Tau1Pt;
      tau_eta = 2.3;
  }
    }  
    
  //elec_pt = 0;
  //muon_pt = 0;
  //tau_pt  = 0;
  //lead_tau_pt = 0;
  if(l1Cuts.JetFilter){
      if(channel == channel::tt) tau_eta = 2.1;
      if(channel == channel::mt) muon_eta = 2.1;
      if(channel == channel::et) elec_eta = 2.1;
      if(channel == channel::em) muon_eta = 2.1;
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
  std::cout << boost::format(param_fmt) % "lead_tau_pt" % lead_tau_pt;
  std::cout << boost::format(param_fmt) % "tau_eta" % tau_eta;
  std::cout << boost::format(param_fmt) % "tau_dz" % tau_dz;
  std::cout << boost::format(param_fmt) % "Jet1Pt" % offlineCuts.Jet1Pt;
  std::cout << boost::format(param_fmt) % "Jet2Pt" % offlineCuts.Jet2Pt;
  std::cout << boost::format(param_fmt) % "JetsAvgPt" % offlineCuts.AvePt;
  std::cout << boost::format(param_fmt) % "JetsVecPt" % offlineCuts.VecPt;
  std::cout << boost::format(param_fmt) % "Mjj" % offlineCuts.Mjj;
  std::cout << boost::format(param_fmt) % "DeltaEta" % offlineCuts.DeltaEta;

  // Create analysis object
  std::string EventTreeName = tree_name;
  if(doL1==0) EventTreeName = "icEventProducer/EventTree";
  if(version_number == 5) L1_Muon_branch = "L1Muons";
  ic::AnalysisBase analysis(
    "HiggsTauTau",        // Analysis name
    files,                // Input files
    EventTreeName, // TTree name
    //"icEventProducer/EventTree", // TTree name
    max_events);          // Max. events to process (-1 = all)

  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);

  // ------------------------------------------------------------------------------------
  // Misc Modules
  // ------------------------------------------------------------------------------------       

  MakeRunStats runStats = MakeRunStats("RunStats")
    .set_output_name(output_folder+output_name+".runstats");

  HTTPrint httPrint("HTTPrint");
  if(era == era::trigger_2016){
   httPrint.set_muon_label("muons");
   httPrint.set_jet_label("ak4PFJetsCHS");
  }

  string mc_pu_file;
  if (mc == mc::phys14_72X) mc_pu_file  = "input/pileup/MC_Summer12_PU_S10-600bins.root";
  //if (mc == mc::summer15_74X) mc_pu_file  = "input/pileup/MC_Summer12_PU_S10-600bins.root";
  string data_pu_file;
  if (era == era::trigger_2016) data_pu_file    =  "input/pileup/Data_Pileup_2012_ReRecoPixel-600bins.root";

  TH1D data_pu  = GetFromTFile<TH1D>(data_pu_file, "/", "pileup");
  TH1D mc_pu    = GetFromTFile<TH1D>(mc_pu_file, "/", "pileup");
  if (!is_data) {
    std::cout << "** Pileup Files **" << std::endl;
    std::cout << boost::format(param_fmt) % "mc_pu_file" % mc_pu_file;
    std::cout << boost::format(param_fmt) % "data_pu_file" % data_pu_file;
  }
  
  std::string jets_label="pfJetsPFlow";
  if(era==era::trigger_2016) jets_label="ak4PFJetsCHS";

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
    
  CopyCollection<Electron>  
    selElectronCopyCollection("CopyToSelElectrons","electrons","selElectrons");

  boost::function<bool (Electron const*)> elec_idiso_func;

  if (strategy == strategy::phys14) {
    elec_idiso_func = bind(ElectronHTTIdSpring15, _1, false) && (bind(PF04IsolationVal<Electron>, _1, 0.5, 0) < 0.1);
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
  std::string muon_label="muons";
  CopyCollection<Muon> selMuonCopyCollection("CopyToSelMuons",muon_label,"selMuons");

  boost::function<bool (Muon const*)> muon_idiso_func;
  if (channel == channel::em) {
    muon_idiso_func = bind(MuonMedium, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5, 0) < 0.15);
  } else {
    muon_idiso_func = bind(MuonMedium, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5, 0) < 0.1);
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
      bind(&Muon::is_global, _1) && bind(&Muon::is_tracker, _1) &&
      bind(PF04IsolationVal<Muon>, _1, 0.5, 0) < 0.3);


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
    extraMuonVeto.set_predicate(bind(MinPtMaxEta, _1, 5.0, 2.4)
              && bind(MuonMedium, _1) 
              && bind(PF04IsolationVal<Muon>, _1, 0.5, 0)< 0.3
              && (bind(fabs, bind(&Muon::dxy_vertex, _1)) < 0.045)
              && (bind(fabs, bind(&Muon::dz_vertex, _1)) < 0.2));
  }

  // ------------------------------------------------------------------------------------
  // Tau Modules
  // ------------------------------------------------------------------------------------

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

  SimpleFilter<Tau> tauPtEtaFilter = SimpleFilter<Tau>("TauPtEtaFilter")
    .set_input_label("taus")
    .set_predicate(bind(MinPtMaxEta, _1, tau_pt, tau_eta))
    .set_min(1);
  if (channel == channel::tt) tauPtEtaFilter.set_min(2);
  
  CopyCollection<Tau> copyToSelectLeadTau("CopyToSelectLeadTaus","taus","leadingtaus");
  
  SimpleFilter<Tau> leadtauPtEtaFilter = SimpleFilter<Tau>("LeadTauPtEtaFilter")
    .set_input_label("leadingtaus")
    .set_predicate(bind(MinPtMaxEta, _1, lead_tau_pt, tau_eta))
    .set_min(1);
      
  SimpleFilter<Tau> tauDzFilter = SimpleFilter<Tau>("TauDzFilter")
    .set_input_label("taus")
    .set_predicate(bind(fabs, bind(&Tau::lead_dz_vertex, _1)) < tau_dz)
    .set_min(1);
  if (channel == channel::tt) tauDzFilter.set_min(2); 

  std::string tau_iso_discr, tau_anti_elec_discr_1, tau_anti_elec_discr_2, tau_anti_muon_discr;

  if (strategy == strategy::phys14) {
    if (channel == channel::et || channel == channel::etmet) {
      tau_iso_discr         = "byCombinedIsolationDeltaBetaCorrRaw3Hits";
      tau_anti_elec_discr_1 = "againstElectronTightMVA5";
      tau_anti_elec_discr_2 = "againstElectronTightMVA5";
      tau_anti_muon_discr   = "againstMuonLoose3";
    }
    if (channel == channel::mt) {
      tau_iso_discr         = "byCombinedIsolationDeltaBetaCorrRaw3Hits";
      tau_anti_elec_discr_1 = "againstElectronVLooseMVA5";
      tau_anti_elec_discr_2 = "againstElectronVLooseMVA5";
      tau_anti_muon_discr   = "againstMuonTight3";
    }
    if (channel == channel::tt ) {
     tau_iso_discr = "byCombinedIsolationDeltaBetaCorrRaw3Hits";
     tau_anti_elec_discr_1 = "againstElectronVLooseMVA5";
     tau_anti_elec_discr_2 = "againstElectronVLooseMVA5";
     tau_anti_muon_discr = "againstMuonLoose3";
    }
  }

  SimpleFilter<Tau> tauIsoFilter = SimpleFilter<Tau>("TauIsoFilter")
    .set_input_label("taus")
    .set_predicate((bind(&Tau::GetTauID, _1, tau_iso_discr) > 0.5) && (bind(&Tau::GetTauID, _1, "decayModeFinding") > 0.5))
    .set_min(1);
    if (channel== channel::tt){
      tauIsoFilter.set_min(2);
      tauIsoFilter.set_predicate(    (bind(&Tau::GetTauID, _1, tau_iso_discr) < 1.0)&& (bind(&Tau::GetTauID, _1, "decayModeFinding") > 0.5)); 
    }

  if(strategy == strategy::phys14) {
    double tau_3hit_cut = 1.5;
    tauIsoFilter.set_predicate(    (bind(&Tau::GetTauID, _1, tau_iso_discr) < tau_3hit_cut) 
                    && (  (bind(&Tau::GetTauID, _1, "decayModeFinding") > 0.5) || (bind(&Tau::GetTauID, _1, "decayModeFindingNewDMs") > 0.5)  )   );
  }

  SimpleFilter<Tau> tauElRejectFilter = SimpleFilter<Tau>("TauElRejectFilter")
    .set_predicate( (bind(&Tau::GetTauID, _1, tau_anti_elec_discr_1) > 0.5) && (bind(&Tau::GetTauID, _1, tau_anti_elec_discr_2) > 0.5) )                     
    .set_input_label("taus").set_min(1);
  if (channel == channel::tt) tauElRejectFilter.set_min(2); 

  SimpleFilter<Tau> tauMuRejectFilter = SimpleFilter<Tau>("TauMuRejectFilter")
    .set_predicate(bind(&Tau::GetTauID, _1, tau_anti_muon_discr) > 0.5)
    .set_input_label("taus").set_min(1);
  if (channel == channel::tt) tauMuRejectFilter.set_min(2);
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

  CompositeProducer<Tau, Tau> tauTauPairProducer = CompositeProducer<Tau, Tau>
    ("TauTauPairProducer")
    .set_input_label_first("taus")
    .set_input_label_second("taus")
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
    
  // ------------------------------------------------------------------------------------
  // Generator Channel Filter
  // ------------------------------------------------------------------------------------ 

  GenChannelFilter genChannelFilter = GenChannelFilter("GenChannelFilter", fs, channel_str, isDY);
  FilterGoodEvents goodEventsFilter = FilterGoodEvents("GoodEventsFilter", channel_str);
  
  // ------------------------------------------------------------------------------------
  // VBF Filter + plots
  // ------------------------------------------------------------------------------------ 
    
  VBFFilter vBFFilter = VBFFilter("VBFFilter", offlineCuts);
  
  VBFPlots vBFPlots = VBFPlots("VBFPlots", fs, "VBFPlots",0,channel_str);
  VBFPlots vBFPlotsGenFiltered = VBFPlots("VBFPlots", fs, "VBFPlotsGenFiltered",0,channel_str);
  VBFPlots vBFPlotsSR = VBFPlots("VBFPlots", fs, "VBFPlotsJetsPtSignalRegion",0,channel_str);
    
  // ------------------------------------------------------------------------------------
  // L1 Trigger Filter
  // ------------------------------------------------------------------------------------ 

  L1TFilterPlots l1TFilterPlots1 = L1TFilterPlots("L1TFilterPlots1", channel_str, fs, l1Cuts, "L1TFilterPlotsPreFiltering", 1);
  L1TFilterPlots l1TFilterPlots2 = L1TFilterPlots("L1TFilterPlots2", channel_str, fs, l1Cuts, "L1TFilterPlotsPostVBFFilter", 1);
  L1TFilter zeroBiasL1TFilter = L1TFilter("L1TFilter", channel_str, fs, l1Cuts, L1_Muon_branch, "ZeroBiasL1TFilter", isZeroBias);//L1Muons
  L1TFilter l1TFilter = L1TFilter("L1TFilter2", channel_str, fs, l1Cuts, "L1Muons", "L1TFilter", isZeroBias);
  L1TFilter l1TFilterTotal = L1TFilter("L1TFilterTotal", channel_str, fs, l1Cuts, "L1Muons", "L1TFilterTotal", isZeroBias);
                                     
  TT_L1TFilter TTzeroBiasL1TFilter = TT_L1TFilter("TTL1TFilter", channel_str, fs, l1Cuts, L1_Muon_branch, "TTZeroBiasL1TFilter", isZeroBias);//L1Muons
  TT_L1TFilter TTl1TFilter       = TT_L1TFilter("TTL1TFilter2", channel_str, fs, l1Cuts, "L1Muons", "TTL1TFilter", isZeroBias);
  TT_L1TFilter TTl1TFilterTotal    = TT_L1TFilter("TTL1TFilterTotal", channel_str, fs, l1Cuts, "L1Muons", "TTL1TFilterTotal", isZeroBias);
  
  // ------------------------------------------------------------------------------------
  // Histogram Plotting
  // ------------------------------------------------------------------------------------ 

  L1VariableHistograms l1VariableHistograms = L1VariableHistograms("L1VariableHistograms", fs, "L1THistogramsPreFiltering");
  VariableHistograms variableHistograms = VariableHistograms("VariableHistograms", fs, "OfflineHistogramsPreFiltering",channel_str);
  
  L1VariableHistograms l1VariableHistograms2 = L1VariableHistograms("L1VariableHistograms", fs, "L1THistogramsPostFiltering");
  VariableHistograms variableHistograms2 = VariableHistograms("VariableHistograms", fs, "OfflineHistogramsPostFiltering", channel_str);
  
  // ------------------------------------------------------------------------------------
  // Trigger Efficiencies
  // ------------------------------------------------------------------------------------ 
  
  EfficiencyGenMatch efficiencyGenMatch1 = EfficiencyGenMatch("EfficiencyGenMatch", fs, "EfficienciesGenMatch/TriggerEfficiencies1", 1, channel_str, l1Cuts);
  EfficiencyGenMatch efficiencyGenMatch2 = EfficiencyGenMatch("EfficiencyGenMatch", fs, "EfficienciesGenMatch/TriggerEfficiencies2", 2, channel_str, l1Cuts);
  EfficiencyGenMatch efficiencyGenMatch3 = EfficiencyGenMatch("EfficiencyGenMatch", fs, "EfficienciesGenMatch/TriggerEfficiencies3", 3, channel_str, l1Cuts);
  EfficiencyGenMatch efficiencyGenMatch4 = EfficiencyGenMatch("EfficiencyGenMatch", fs, "EfficienciesGenMatch/TriggerEfficiencies4", 4, channel_str, l1Cuts);
  EfficiencyGenMatch efficiencyGenMatch5 = EfficiencyGenMatch("EfficiencyGenMatch", fs, "EfficienciesGenMatch/TriggerEfficiencies5", 2, channel_str, l1Cuts);
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
  
  if(isZeroBias == 1 && doL1==1){
      if(doTTJets == 0) analysis.AddModule(&zeroBiasL1TFilter);
      if(doTTJets == 1) analysis.AddModule(&TTzeroBiasL1TFilter);
  }
  else if (isZeroBias ==0) {
    
      if (tau_scale_mode > 0 && channel != channel::em && !moriond_tau_scale)
                                    analysis.AddModule(&tauEnergyShifter);
      if (tau_scale_mode > 0 && channel == channel::em)         
                                    analysis.AddModule(&electronEnergyShifter);         
                                      analysis.AddModule(&httEnergyScale);
      if (to_check.size() > 0)        analysis.AddModule(&httPrint);
      
                                      if(doL1 == 1) analysis.AddModule(&vBFPlots);
                                      if(makeEffPlots == 1 && doL1 == 1){
                                          analysis.AddModule(&efficiencyGenMatch1);
                                          analysis.AddModule(&efficiencyGenMatch2);
                                          analysis.AddModule(&efficiencyGenMatch3);
                                          analysis.AddModule(&efficiencyGenMatch4);
                                      }
                                      analysis.AddModule(&genChannelFilter);
                                      if(era == era::trigger_2016 && doL1==1){
                                          if(doTTJets == 0) analysis.AddModule(&l1TFilterTotal);
                                          if(doTTJets == 1) analysis.AddModule(&TTl1TFilterTotal);
                                      }
                                      if(doL1 == 1) analysis.AddModule(&vBFPlotsGenFiltered);
      
                                      if(doL1 == 1) analysis.AddModule(&l1VariableHistograms);
                                      if(era == era::trigger_2016 && doL1==1) analysis.AddModule(&l1TFilterPlots1);
      
      
      if (channel == channel::et) {
                                      analysis.AddModule(&selElectronCopyCollection);
                                      analysis.AddModule(&selElectronFilter);                 
                                      analysis.AddModule(&vetoElectronCopyCollection);
                                      analysis.AddModule(&vetoElectronFilter);
                                      analysis.AddModule(&vetoElectronPairProducer);
                                      analysis.AddModule(&vetoElectronPairFilter);
                                      analysis.AddModule(&extraElectronVeto);
                                      analysis.AddModule(&extraMuonVeto);
                                      analysis.AddModule(&tauPtEtaFilter);
                                      analysis.AddModule(&tauDzFilter);                            
                                      analysis.AddModule(&tauIsoFilter);
                                      analysis.AddModule(&tauElRejectFilter);
                                      analysis.AddModule(&tauMuRejectFilter);
                                      
                                      analysis.AddModule(&tauElPairProducer);
                                      analysis.AddModule(&pairFilter);
      } 
    
      if (channel == channel::tt) {

                                      analysis.AddModule(&extraElectronVeto);
                                      analysis.AddModule(&extraMuonVeto);
                                      analysis.AddModule(&tauPtEtaFilter);
                                      analysis.AddModule(&copyToSelectLeadTau);
                                      analysis.AddModule(&leadtauPtEtaFilter);
                                      analysis.AddModule(&tauDzFilter);
     
                                      analysis.AddModule(&tauIsoFilter);
                                      analysis.AddModule(&tauElRejectFilter);
                                      analysis.AddModule(&tauMuRejectFilter);
    
                                      analysis.AddModule(&tauTauPairProducer);
                                      analysis.AddModule(&pairFilter);
                                      
      } 
    
      if (channel == channel::mt) {
                                      analysis.AddModule(&selMuonCopyCollection);
                                      analysis.AddModule(&selMuonFilter);                 
                                      analysis.AddModule(&vetoMuonCopyCollection);
                                      analysis.AddModule(&vetoMuonFilter);
                                      analysis.AddModule(&vetoMuonPairProducer);
                                      analysis.AddModule(&vetoMuonPairFilter);
                                      analysis.AddModule(&extraElectronVeto);
                                      analysis.AddModule(&extraMuonVeto);
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
                                      analysis.AddModule(&extraElectronVeto);
                                      analysis.AddModule(&extraMuonVeto);
      }
                                      analysis.AddModule(&jetIDFilter);
                                      analysis.AddModule(&filteredJetCopyCollection);
                                      analysis.AddModule(&jetLeptonOverlapFilter);

                                      analysis.AddModule(&variableHistograms);

                                      analysis.AddModule(&vBFFilter);
                                      if(era == era::trigger_2016 && doL1==1) analysis.AddModule(&l1TFilterPlots2);
                                      if(era == era::trigger_2016 && doL1==1){
                                          if(doTTJets == 0) analysis.AddModule(&l1TFilter);
                                          if(doTTJets == 1) analysis.AddModule(&TTl1TFilter);
                                      }

                                      if(doL1 == 1) analysis.AddModule(&l1VariableHistograms2);
                                      analysis.AddModule(&variableHistograms2);
                                      if(doL1 == 1){ 
                                        analysis.AddModule(&vBFPlotsSR);
                                        analysis.AddModule(&efficiencyGenMatch5);
                                      }

  }
  analysis.RunAnalysis();
  delete fs;
  return 0;
}




