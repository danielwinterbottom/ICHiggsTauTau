#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/format.hpp"
#include "boost/algorithm/string.hpp"
#include "TSystem.h"
// #include "FWCore/FWLite/interface/AutoLibraryLoader.h"
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
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/EffectiveAreaIsolationFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MetSelection.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MTSelection.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MakeRunStats.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/JetEnergyCorrections.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/LumiMask.h"

#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvDataTriggerFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvWeights.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvWDecay.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvZDecay.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/MCFMStudy.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvControlPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvWJetsPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/ModifyMet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/JetMETModifier.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/MetEventFilters.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvPrint.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/CJVFilter.h"

#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvConfig.h"


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

  string era_str;                 // Analysis data-taking era
  string mc_str;                  // Analysis MC production
  string prod;                    // Our prdocution string

  string channel_str;             // Analysis channel
  string wstream;                 // W stream: enu, munu or taunu, or nunu for everything

  bool ignoreLeptons;             // for Znunu estimates using DYJets samples

  bool is_data;                   // true = data, false = mc         
  bool is_embedded;               // true = embedded, false = not an embedded sample
  unsigned mva_met_mode;          // 0 = standard mva met, 1 = mva met from vector (only when mva met is being used)
  bool make_sync_ntuple;          // Generate a sync ntuple
  bool dojessyst;                 // Do Jet Energy Scale Systematic Run
  bool dodatajessyst;             // Do Alternate Data Jet Energy Scale Method Systematic Run
  bool jesupordown;               // If doing Jet Energy Scale Systematic Run, run with up or down correction (true for up, false for down)
  bool dosmear;                   // Do Smearing
  bool doaltmatch;                // Do runmetuncertainties gen jet matching
  bool doetsmear;                 // Do runmetuncertainties smearing
  bool dogaus;                    // Do gaussian smearing for jets with no gen jet match
  bool dojersyst;                 // Do Jet Energy Resolution Systematic Run
  bool jerbetterorworse;          // If doing Jet Energy Resolution Systematic Run, run with with better or worse (true for better, false for worse)
  bool docrosschecktau;           // If doing cross check tau use alternate tau id discriminant
  bool taulepdiscrtight;          // Use tight electron and muon discriminants

  string mettype;                 // MET input collection to be used
  string jesuncfile;              // File to get JES uncertainties from
  bool doMetFilters;              // apply cleaning MET filters.
  string filters;
  //unsigned signal_region;       // DeltaPhi cut > 2.7
  double met_cut;                 // MET cut min to apply for signal, QCD or skim
  double met_cut_max;                 // MET cut max to apply for signal, QCD or skim
  bool dotrgeff;                  // Do trigger efficiency corrections
  bool doidisoeff;                // Do lepton ID-iso efficiency corrections

  double mjj_cut;                 // mjjcut

  bool printEventList;  //print run,lumi,evt of events selected
  bool printEventContent; //print event content of events selected

  bool doMCFMstudy;
  bool turnoffpuid;

  std::string eventsToSkim; //name of input file containing run,lumi,evt of events to be skimmed

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
    ("era",                 po::value<string>(&era_str)->required())
    ("mc",                  po::value<string>(&mc_str)->required())
    ("prod",                 po::value<string>(&prod)->required())
    ("channel",             po::value<string>(&channel_str)->default_value("nunu"))
    ("wstream",             po::value<string>(&wstream)->default_value("nunu"))
    ("ignoreLeptons",       po::value<bool>(&ignoreLeptons)->default_value(false))
    ("is_data",             po::value<bool>(&is_data)->required())
    ("is_embedded",         po::value<bool>(&is_embedded)->default_value(false))
    ("mva_met_mode",        po::value<unsigned>(&mva_met_mode)->default_value(1))
    ("make_sync_ntuple",    po::value<bool>(&make_sync_ntuple)->default_value(false))
    ("mettype",             po::value<string>(&mettype)->default_value("pfMetType1"))
    ("met_cut",             po::value<double>(&met_cut)->default_value(130.))
    ("met_cut_max",         po::value<double>(&met_cut_max)->default_value(14000.))
    ("mjj_cut",             po::value<double>(&mjj_cut)->default_value(1200.))
    ("doMetFilters",        po::value<bool>(&doMetFilters)->default_value(false))
    ("filters",             po::value<string> (&filters)->default_value("HBHENoiseFilter,EcalDeadCellTriggerPrimitiveFilter,eeBadScFilter,trackingFailureFilter,manystripclus53X,toomanystripclus53X,logErrorTooManyClusters,CSCTightHaloFilter"))
    ("dojessyst",           po::value<bool>(&dojessyst)->default_value(false))
    ("dodatajessyst",       po::value<bool>(&dodatajessyst)->default_value(false))
    ("jesupordown",         po::value<bool>(&jesupordown)->default_value(true))
    ("dojersyst",           po::value<bool>(&dojersyst)->default_value(false))
    ("jerbetterorworse",    po::value<bool>(&jerbetterorworse)->default_value(true))
    ("docrosschecktau",    po::value<bool>(&docrosschecktau)->default_value(false))
    ("taulepdiscrtight",    po::value<bool>(&taulepdiscrtight)->default_value(false))
    ("dotrgeff",            po::value<bool>(&dotrgeff)->default_value(false))
    ("doidisoeff",          po::value<bool>(&doidisoeff)->default_value(false))
    ("printEventList",      po::value<bool>(&printEventList)->default_value(false))
    ("printEventContent",   po::value<bool>(&printEventContent)->default_value(false))
    ("eventsToSkim",        po::value<string>(&eventsToSkim)->default_value("input/runDChayanitUniq.dat"))
    ("dosmear",             po::value<bool>(&dosmear)->default_value(false))
    ("doaltmatch",          po::value<bool>(&doaltmatch)->default_value(false))
    ("doetsmear",           po::value<bool>(&doetsmear)->default_value(false))
    ("dogaus",              po::value<bool>(&dogaus)->default_value(false))
    ("jesuncfile",          po::value<string>(&jesuncfile)->default_value("input/jec/Fall12_V7_MC_Uncertainty_AK5PF.txt"))
    ("doMCFMstudy",         po::value<bool>(&doMCFMstudy)->default_value(false))
    ("turnoffpuid",         po::value<bool>(&turnoffpuid)->default_value(false));
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
  po::notify(vm);

  vector<string> filtersVec;
  boost::split(filtersVec, filters, boost::is_any_of(","));

  // Some options must now be re-configured based on other options
  //ic::era era           = String2Era(era_str); //could be used in future but commented out to remove compiler warnings
  //ic::mc mc             = String2MC(mc_str); //could be used in future but commented out to remove compiler warnings
  //ic::channel channel   = String2Channel(channel_str); //could be used in future but commented out to remove compiler warnings

  std::cout << "**** HiggsNuNu Analysis *****" << std::endl;
  string param_fmt = "%-25s %-40s\n";
  std::cout << boost::format(param_fmt) % "max_events" % max_events;
  std::cout << boost::format(param_fmt) % "output" % (output_folder+output_name);
  std::cout << boost::format(param_fmt) % "do_skim" % do_skim;
  if (do_skim) std::cout << boost::format(param_fmt) % "skim_path" % skim_path;
  std::cout << boost::format(param_fmt) % "dojessyst" % dojessyst;
  if (dojessyst) std::cout << boost::format(param_fmt) % "jesupordown" % jesupordown;
  std::cout << boost::format(param_fmt) % "dojersyst" % dojersyst;
  if (dojessyst) std::cout << boost::format(param_fmt) % "jerbettorworse" % jerbetterorworse;
  std::cout << boost::format(param_fmt) % "era" % era_str;
  std::cout << boost::format(param_fmt) % "mc" % mc_str;
  std::cout << boost::format(param_fmt) % "prod" % prod;
  std::cout << boost::format(param_fmt) % "channel" % channel_str;
  std::cout << boost::format(param_fmt) % "wstream" % wstream;
  std::cout << boost::format(param_fmt) % "is_data" % is_data;
  std::cout << boost::format(param_fmt) % "is_embedded" % is_embedded;
  std::cout << boost::format(param_fmt) % "mva_met_mode" % mva_met_mode;
  std::cout << boost::format(param_fmt) % "make_sync_ntuple" % make_sync_ntuple;
  std::cout << boost::format(param_fmt) % "met_cut" % met_cut ;
  std::cout << boost::format(param_fmt) % "met_cut_max" % met_cut_max ;
  std::cout << boost::format(param_fmt) % "mjj_cut" % mjj_cut ;
  std::cout << boost::format(param_fmt) % "doMetFilters" % doMetFilters;
  std::cout << boost::format(param_fmt) % "filters" % filters;
  std::cout << boost::format(param_fmt) % "dotrgeff" % dotrgeff;
  std::cout << boost::format(param_fmt) % "doidisoeff" % doidisoeff;

  // Load necessary libraries for ROOT I/O of custom classes
  // gSystem->Load("libFWCoreFWLite.dylib");
  // gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  // AutoLibraryLoader::enable();


  // Build a vector of input files
  vector<string> files = ParseFileLines(filelist);
  for (unsigned i = 0; i < files.size(); ++i) files[i] = input_prefix + files[i];
  
  // Create ROOT output fileservice
  fwlite::TFileService *fs = new fwlite::TFileService((output_folder+output_name).c_str());
  
  double elec_dz, elec_dxy;
  double muon_dz, muon_dxy;
  //double veto_elec_dz, veto_elec_dxy; //could be used in future but commented out to remove compiler warnings
  //double veto_muon_dz, veto_muon_dxy;
  double elec_pt, elec_eta, muon_pt, muon_eta;
  //double veto_elec_pt, veto_elec_eta, veto_muon_pt, veto_muon_eta; //could be used in future but commented out to remove compiler warnings
  
  elec_dz = 0.1;
  elec_dxy = 0.02;
  //veto_elec_dz = 0.2; //could be used in future but commented out to remove compiler warnings
  //veto_elec_dxy = 0.04; //could be used in future but commented out to remove compiler warnings
  muon_dz = 0.2;
  muon_dxy = 0.045;
  //veto_muon_dz = 0.2;
  //veto_muon_dxy = 0.045;
  
  elec_pt = 20.0;
  elec_eta = 2.4;
  muon_pt = 20.0;
  muon_eta = 2.4;

  std::cout << "----------PARAMETERS----------" << std::endl;
  std::cout << boost::format("%-15s %-10s\n") % "elec_pt:" % elec_pt;
  std::cout << boost::format("%-15s %-10s\n") % "elec_eta:" % elec_eta;
  std::cout << boost::format("%-15s %-10s\n") % "elec_dxy:" % elec_dxy;
  std::cout << boost::format("%-15s %-10s\n") % "elec_dz:" % elec_dz;
  std::cout << boost::format("%-15s %-10s\n") % "muon_pt:" % muon_pt;
  std::cout << boost::format("%-15s %-10s\n") % "muon_eta:" % muon_eta;
  std::cout << boost::format("%-15s %-10s\n") % "muon_dxy:" % muon_dxy;
  std::cout << boost::format("%-15s %-10s\n") % "muon_dz:" % muon_dz;
  
  
  
  // Create analysis object
  ic::AnalysisBase analysis(
    "HiggsNuNu",        // Analysis name
    files,                // Input files
    "icEventProducer/EventTree", // TTree name
    max_events);          // Max. events to process (-1 = all)
  if (do_skim && skim_path == "") {
    std::cout << "-- Skimming mode: requires skim_path parameter to be set !! Quitting." << std::endl;
    return 1;
  }
  if (do_skim && skim_path != "") analysis.DoSkimming(skim_path);

  // ------------------------------------------------------------------------------------
  // Misc Modules
  // ------------------------------------------------------------------------------------
  
  MakeRunStats runStats = MakeRunStats("RunStats")
    .set_output_name(output_folder+output_name+".runstats");
  
  HinvDataTriggerFilter dataMCTriggerPathFilter("TriggerPathFilter");
  dataMCTriggerPathFilter.set_is_data(is_data);
  dataMCTriggerPathFilter.set_trigger_path("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v");
  dataMCTriggerPathFilter.set_trig_obj_label("triggerObjectsDiPFJet40PFMETnoMu65MJJ800VBFAllJets");

  // ------------------------------------------------------------------------------------
  // Muon Modules
  // ------------------------------------------------------------------------------------
  
  //sel muons
  CopyCollection<Muon> selMuonCopyCollection("CopyToSelMuons","muonsPFlow","selMuons");   
  SimpleFilter<Muon> selMuonFilter = SimpleFilter<Muon>
    ("SelMuonPtEtaFilter")
    .set_input_label("selMuons").set_predicate(bind(MinPtMaxEta, _1, muon_pt, muon_eta) &&
					       bind(&Muon::is_global, _1) &&
					       bind(MuonTight, _1) && 
					       bind(PF04Isolation<Muon>, _1, 0.5, 0.12)
					       )
    .set_min(1)
    .set_max(1);

  // ------------------------------------------------------------------------------------
  // Build Analysis Sequence
  // ------------------------------------------------------------------------------------  
  
  //     analysis.AddModule(&dataMCTriggerPathFilter);
  
  analysis.AddModule(&selMuonCopyCollection);
  analysis.AddModule(&selMuonFilter);
  
  // Run analysis

  analysis.RetryFileAfterFailure(5,5);// int <pause between attempts in seconds>, int <number of retry attempts to make> );
  analysis.StopOnFileFailure(true);
  analysis.SetTTreeCaching(true);

  analysis.RunAnalysis();
  delete fs;
  return 0;
}




