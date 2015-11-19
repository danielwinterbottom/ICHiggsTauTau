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
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/TmvaInputs.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/TrigeffInputs.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/LightTree.h"

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
  bool do_trigeff;                // Apply trigger at end of selection to work out trigger efficiency and bin variables
  bool do_trigeff_tree;           // Generate tree of variables used for trig study
  bool do_light_tree;             // Generate tree for light tree steps
  bool do_parked_trigs;           // Option to do parked triggers
  bool do_gensteps;               // For getting numbers of events at gen level
  bool doincludehighptz;          // For including high pt z sample
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
  bool dospring10gaus;                    // Do gaussian smearing for jets with no gen jet match
  bool dojersyst;                 // Do Jet Energy Resolution Systematic Run
  bool jerbetterorworse;          // If doing Jet Energy Resolution Systematic Run, run with with better or worse (true for better, false for worse)
  bool docrosschecktau;           // If doing cross check tau use alternate tau id discriminant
  bool taulepdiscrtight;          // Use tight electron and muon discriminants
  bool dojerdebug;                // Access runmetunc collections for debugging

  string mettype;                 // MET input collection to be used
  string jesuncfile;              // File to get JES uncertainties from
  bool doMetFilters;              // apply cleaning MET filters.
  string filters;
  //unsigned signal_region;       // DeltaPhi cut > 2.7
  bool dotrgeff;                  // Do trigger efficiency corrections
  bool doidisoeff;                // Do lepton ID-iso efficiency corrections
  bool doidisoerr;                // Do lepton ID-iso efficiency correction error
  bool doidisoerrupordown;        // Do lepton ID-iso efficiency correction error up or down
  bool doidisoerrmuore;           // Do lepton ID-iso efficiency correction error for muons or electrons
  bool dolumixsweight;            // Do lumi*xs/evt weight online
  string inputparams;             // Params file to use for info on lumi xs and evt
  string trg_weight_file;         // Trigger weights to use
  string trg_to_use;              // Which trigger to use for data

  //CUTS
  double jet1ptcut;               //jet1ptcut
  double jet1ptcutmax;            //jet1ptcutmax
  double jet2ptcut;               //jet2ptcut
  double jet2ptcutmax;            //jet2ptcutmax
  double jetptprecut;             //jetptprecut, should be the CJV threshold
  double detajjcut;               //delta eta between jets cut
  double detajjcut_max;           //delta eta between jets cut
  double dphi_cut;                //delta phi cut FOR TRIGGER STUDY ONLY
  double dphi_cut_max;            //delta phi cut max FOR TRIGGER STUDY ONLY
  double met_cut;                 // MET cut min to apply for signal, QCD or skim
  double met_cut_max;             // MET cut max to apply for signal, QCD or skim
  double mjj_cut;                 // mjjcut
  double mjj_cut_max;             // maxmjjcut


  int randomseed;



  bool printEventList;  //print run,lumi,evt of events selected
  bool printEventContent; //print event content of events selected

  bool doMCFMstudy;
  bool turnoffpuid;

  bool doTopCR;

  bool doTmvaTree; //write out a filewith a tree with TMVA variables

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
    ("do_trigeff",          po::value<bool>(&do_trigeff)->default_value(false))
    ("do_trigeff_tree",     po::value<bool>(&do_trigeff_tree)->default_value(false))
    ("do_light_tree",       po::value<bool>(&do_light_tree)->default_value(false))
    ("do_parked_trigs",     po::value<bool>(&do_parked_trigs)->default_value(false))
    ("do_gensteps",         po::value<bool>(&do_gensteps)->default_value(false))
    ("doincludehighptz",    po::value<bool>(&doincludehighptz)->default_value(false))
    ("skim_path",           po::value<string>(&skim_path)->default_value(""))
    ("era",                 po::value<string>(&era_str)->required())
    ("mc",                  po::value<string>(&mc_str)->required())
    ("prod",                po::value<string>(&prod)->required())
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
    ("jet1ptcut",           po::value<double>(&jet1ptcut)->default_value(50.))
    ("jet1ptcutmax",        po::value<double>(&jet1ptcutmax)->default_value(999999999.))
    ("jet2ptcut",           po::value<double>(&jet2ptcut)->default_value(50.))
    ("jet2ptcutmax",        po::value<double>(&jet2ptcutmax)->default_value(999999999.))
    ("jetptprecut",         po::value<double>(&jetptprecut)->default_value(30.))
    ("detajjcut",           po::value<double>(&detajjcut)->default_value(4.2))
    ("detajjcut_max",       po::value<double>(&detajjcut_max)->default_value(20.))
    ("dphi_cut",            po::value<double>(&dphi_cut)->default_value(0.))
    ("dphi_cut_max",        po::value<double>(&dphi_cut_max)->default_value(6.3))
    ("mjj_cut",             po::value<double>(&mjj_cut)->default_value(1200.))
    ("mjj_cut_max",         po::value<double>(&mjj_cut_max)->default_value(8000.))
    ("doMetFilters",        po::value<bool>(&doMetFilters)->default_value(false))
    ("filters",             po::value<string> (&filters)->default_value("HBHENoiseFilter,EcalDeadCellTriggerPrimitiveFilter,eeBadScFilter,trackingFailureFilter,manystripclus53X,toomanystripclus53X,logErrorTooManyClusters,CSCTightHaloFilter"))
    ("dojessyst",           po::value<bool>(&dojessyst)->default_value(false))
    ("dodatajessyst",       po::value<bool>(&dodatajessyst)->default_value(false))
    ("jesupordown",         po::value<bool>(&jesupordown)->default_value(true))
    ("dojersyst",           po::value<bool>(&dojersyst)->default_value(false))
    ("jerbetterorworse",    po::value<bool>(&jerbetterorworse)->default_value(true))
    ("docrosschecktau",     po::value<bool>(&docrosschecktau)->default_value(false))
    ("taulepdiscrtight",    po::value<bool>(&taulepdiscrtight)->default_value(false))
    ("dojerdebug",          po::value<bool>(&dojerdebug)->default_value(false))
    ("dotrgeff",            po::value<bool>(&dotrgeff)->default_value(false))
    ("doidisoeff",          po::value<bool>(&doidisoeff)->default_value(false))
    ("doidisoerr",          po::value<bool>(&doidisoerr)->default_value(false))
    ("doidisoerrupordown",  po::value<bool>(&doidisoerrupordown)->default_value(true))
    ("doidisoerrmuore",     po::value<bool>(&doidisoerrmuore)->default_value(true))
    ("dolumixsweight",      po::value<bool>(&dolumixsweight)->default_value(false))
    ("inputparams",         po::value<string>(&inputparams)->default_value("filelists/Apr04/ParamsApr04.dat"))
    ("trg_weight_fle",      po::value<string>(&trg_weight_file)->default_value("input/scale_factors/DataMCWeight_53X_v1.root"))
    ("trg_to_use",          po::value<string>(&trg_to_use)->default_value("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v"))
    ("printEventList",      po::value<bool>(&printEventList)->default_value(false))
    ("printEventContent",   po::value<bool>(&printEventContent)->default_value(false))
    ("eventsToSkim",        po::value<string>(&eventsToSkim)->default_value("input/runDChayanitUniq.dat"))
    ("dosmear",             po::value<bool>(&dosmear)->default_value(false))
    ("doaltmatch",          po::value<bool>(&doaltmatch)->default_value(false))
    ("doetsmear",           po::value<bool>(&doetsmear)->default_value(false))
    ("dogaus",              po::value<bool>(&dogaus)->default_value(false))
    ("dospring10gaus",      po::value<bool>(&dospring10gaus)->default_value(false))
    ("jesuncfile",          po::value<string>(&jesuncfile)->default_value("input/jec/Fall12_V7_MC_Uncertainty_AK5PF.txt"))
    ("doMCFMstudy",         po::value<bool>(&doMCFMstudy)->default_value(false))
    ("doTopCR",             po::value<bool>(&doTopCR)->default_value(false))
    ("doTmvaTree",          po::value<bool>(&doTmvaTree)->default_value(false))
    ("turnoffpuid",         po::value<bool>(&turnoffpuid)->default_value(false))
    ("randomseed",          po::value<int>(&randomseed)->default_value(4357));

  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
  po::notify(vm);

  vector<string> filtersVec;
  boost::split(filtersVec, filters, boost::is_any_of(","));

  // Some options must now be re-configured based on other options
  ic::era era           = String2Era(era_str);
  ic::mc mc             = String2MC(mc_str);
  ic::channel channel   = String2Channel(channel_str);

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
  std::cout << boost::format(param_fmt) % "doTopCR" % doTopCR;
  std::cout << boost::format(param_fmt) % "doTmvaTree" % doTmvaTree;

  if (channel==channel::nunulowmet || channel==channel::nunulowmetiglep){
    met_cut_max=met_cut;
    met_cut=0.;
  }

  // Load necessary libraries for ROOT I/O of custom classes
  // gSystem->Load("libFWCoreFWLite.dylib");
  // gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  // AutoLibraryLoader::enable();


  // Build a vector of input files
  vector<string> files = ParseFileLines(filelist);
  for (unsigned i = 0; i < files.size(); ++i) files[i] = input_prefix + files[i];
  
  // Create ROOT output fileservice
  std::ofstream checkfile;
  checkfile.open((output_folder+"tmp.txt").c_str());
  if(!(checkfile.good())){
    std::cout<<"OUTPUT FOLDER DOESN'T EXIST: EXITING"<<std::endl;
    return 1;
  }
  checkfile.close();
  fwlite::TFileService *fs = new fwlite::TFileService((output_folder+output_name).c_str());
   
  double elec_dz, elec_dxy;
  double muon_dz, muon_dxy;
  double veto_elec_dz, veto_elec_dxy;
  //double veto_muon_dz, veto_muon_dxy;
  double elec_pt, elec_eta, muon_pt, muon_eta;
  double veto_elec_pt, veto_elec_eta, veto_muon_pt, veto_muon_eta;
  
  elec_dz = 0.1;
  elec_dxy = 0.02;
  veto_elec_dz = 0.2;
  veto_elec_dxy = 0.04;
  muon_dz = 0.2;
  muon_dxy = 0.045;
  //veto_muon_dz = 0.2;
  //veto_muon_dxy = 0.045;
  
  elec_pt = 20.0;
  elec_eta = 2.4;
  muon_pt = 20.0;
  muon_eta = 2.1;
  
  veto_elec_pt = 10.0;
  veto_elec_eta = 2.4;
  veto_muon_pt = 10.0;
  veto_muon_eta = 2.1;

  if(do_skim) jetptprecut = 30.0;
   
  std::cout << "----------PARAMETERS----------" << std::endl;
  std::cout << boost::format("%-15s %-10s\n") % "elec_pt:" % elec_pt;
  std::cout << boost::format("%-15s %-10s\n") % "elec_eta:" % elec_eta;
  std::cout << boost::format("%-15s %-10s\n") % "veto_elec_pt:" % veto_elec_pt;
  std::cout << boost::format("%-15s %-10s\n") % "veto_elec_eta:" % veto_elec_eta;
  std::cout << boost::format("%-15s %-10s\n") % "elec_dxy:" % elec_dxy;
  std::cout << boost::format("%-15s %-10s\n") % "elec_dz:" % elec_dz;
  std::cout << boost::format("%-15s %-10s\n") % "veto_elec_dxy:" % veto_elec_dxy;
  std::cout << boost::format("%-15s %-10s\n") % "veto_elec_dz:" % veto_elec_dz;
  std::cout << boost::format("%-15s %-10s\n") % "muon_pt:" % muon_pt;
  std::cout << boost::format("%-15s %-10s\n") % "muon_eta:" % muon_eta;
  std::cout << boost::format("%-15s %-10s\n") % "veto_muon_pt:" % veto_muon_pt;
  std::cout << boost::format("%-15s %-10s\n") % "veto muon_eta:" % veto_muon_eta;
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
  
  MCFMStudy mcfmStudy = MCFMStudy("MCFMStudy",13,75,105);


  HinvPrint hinvFilter("HinvFilter",is_data,true,false);

  //COMMENTED OUT FOR OTHER SKIM
//   if (do_skim && channel==channel::nunu){
//     //fill hinvFilter with events to be skimmed from inputfile
//      std::ifstream levtlist;
//      levtlist.open(eventsToSkim.c_str());
//      if(!levtlist.is_open()){
//        std::cerr<<"Unable to open file " << eventsToSkim << " for filtering events. "<<std::endl;
//        return 1; 
//      }
//      unsigned counter = 0;
//      while(1){
//        unsigned lEvt=0;
//        unsigned lRun=0;
//        unsigned lLumi=0;
//        if (is_data) {
//  	levtlist>>lRun;
//  	levtlist>>lLumi;
//        }
//        levtlist>>lEvt;
//        if (lEvt!=0 || lRun!=0 || lLumi!=0){
//  	std::cout << " -- Adding event: " << lRun << ":" << lLumi << ":" << lEvt << std::endl;
//  	hinvFilter.PrintEvent(lRun,lLumi,lEvt);
//  	counter++;
//        }
//        if(levtlist.eof()){
//  	break; 
//        }
//      }

//      std::cout << " - Total of " << counter << " events added to skim." << std::endl;
//      levtlist.close();
//    }

  //print the event content
  HinvPrint hinvPrint("HinvPrint",is_data);

  //print run,lumi,evt of events selected
  HinvPrint hinvPrintList("HinvPrintList",is_data,false,true);

  //bool is_ewkZ = false;
  //if (output_name.find("DYJJ01") != output_name.npos) {
  if (output_name.find("EWK-Z2jiglep") != output_name.npos) {
    ignoreLeptons = true;
    //is_ewkZ = true;
  }
  if (channel==channel::nunuiglep||channel==channel::nunulowmetiglep){
    ignoreLeptons = true;
  }

  string data_json;
  if (era == era::data_2011) data_json           =  "input/json/json_data_2011_et_mt.txt";
  if (era == era::data_2012_ichep) data_json     =  "input/json/data_2012_ichep.txt";
  if (era == era::data_2012_hcp) data_json       =  "input/json/data_2012_hcp.txt";
  if (era == era::data_2012_moriond) data_json   =  "input/json/data_2012_moriond.txt";
  if (era == era::data_2012_donly) data_json     =  "input/json/data_2012_donly.txt";
  LumiMask lumiMask = LumiMask("LumiMask")
    .set_produce_output_jsons("")
    .set_input_file(data_json);

  MakeRunStats runStats = MakeRunStats("RunStats")
    .set_output_name(output_folder+output_name+".runstats");
  
  
  string mc_pu_file;
  if (mc == mc::fall11_42X) mc_pu_file    = "input/pileup/MC_Fall11_PU_S6-500bins.root";
  if (mc == mc::summer12_53X) mc_pu_file  = "input/pileup/MC_Summer12_PU_S10-600bins.root";
  if (mc == mc::summer12_52X) mc_pu_file  = "input/pileup/MC_Summer12_PU_S7-600bins.root";

  string data_pu_file;
  if (era == era::data_2011) data_pu_file     =  "input/pileup/Data_Pileup_2011_HCP-500bins.root";
  if (era == era::data_2012_ichep) data_pu_file     =  "input/pileup/Data_Pileup_2012.root";
  if (era == era::data_2012_hcp) data_pu_file       =  "input/pileup/Data_Pileup_2012_HCP-600bins.root";
  if (era == era::data_2012_moriond) data_pu_file   =  "input/pileup/Data_Pileup_2012_Moriond-600bins.root";
  if (era == era::data_2012_donly) data_pu_file     =  "input/pileup/Data_Pileup_2012_DOnly-600bins.root";

  TH1D data_pu  = GetFromTFile<TH1D>(data_pu_file, "/", "pileup");
  TH1D mc_pu    = GetFromTFile<TH1D>(mc_pu_file, "/", "pileup");

  TH1D data_pu_up  = GetFromTFile<TH1D>("input/pileup/Data_Pileup_2012_Moriond-600bins-Up.root", "/", "pileup");
  TH1D data_pu_down  = GetFromTFile<TH1D>("input/pileup/Data_Pileup_2012_Moriond-600bins-Down.root", "/", "pileup");

  if (!is_data) {
    std::cout << "** Pileup Files **" << std::endl;
    std::cout << boost::format(param_fmt) % "mc_pu_file" % mc_pu_file;
    std::cout << boost::format(param_fmt) % "data_pu_file" % data_pu_file;
  }
  PileupWeight pileupWeight = PileupWeight("PileupWeight")
    .set_data(&data_pu)
    .set_mc(&mc_pu)
    .set_print_weights(false);
  PileupWeight pileupWeight_up = PileupWeight("PileupWeight_up","!pileup_up")
    .set_data(&data_pu_up)
    .set_mc(&mc_pu)
    .set_print_weights(false);
  PileupWeight pileupWeight_down = PileupWeight("PileupWeight_down","!pileup_down")
    .set_data(&data_pu_down)
    .set_mc(&mc_pu)
    .set_print_weights(false);

  HinvDataTriggerFilter dataMCTriggerPathFilter("TriggerPathFilter");
  dataMCTriggerPathFilter.set_is_data(is_data);
  dataMCTriggerPathFilter.set_trigger_path("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v");
  dataMCTriggerPathFilter.set_trig_obj_label("triggerObjectsDiPFJet40PFMETnoMu65MJJ800VBFAllJets");

  JetEnergyCorrections<PFJet> jetEnergyCorrections = JetEnergyCorrections<PFJet>
  ("JetEnergyCorrections")
  .set_input_label("pfJetsPFlow")
  .set_is_data(is_data)
  .set_l1_file("input/jec/START53_V10_L1FastJet_AK5PF.txt")
  .set_l2_file("input/jec/START53_V10_L2Relative_AK5PF.txt")
  .set_l3_file("input/jec/START53_V10_L3Absolute_AK5PF.txt");
  
  std::vector<string> inputVec;
  inputVec.push_back("input/met_laser_filters/AllBadHCALLaser.txt");
  inputVec.push_back("input/met_laser_filters/ecalLaserFilter_MET_Run2012AandB.txt");
  MetEventFilters metEventFilters = MetEventFilters("MetEventFilters",
						    inputVec,
						    doMetFilters);



  // ------------------------------------------------------------------------------------
  // Electron Modules
  // ------------------------------------------------------------------------------------

  // Electron Veto
  CopyCollection<Electron>  vetoElectronCopyCollection("CopyToVetoElectrons","electrons","vetoElectrons");

  SimpleFilter<Electron> vetoElectronFilter = SimpleFilter<Electron>
    ("VetoElectronPtEtaFilter")
    .set_input_label("vetoElectrons").set_predicate(bind(MinPtMaxEta, _1, veto_elec_pt, veto_elec_eta) &&
						    bind(VetoElectronID, _1) && 
						    bind(fabs, bind(&Electron::dxy_vertex, _1)) < veto_elec_dxy && 
						    bind(fabs, bind(&Electron::dz_vertex, _1)) < veto_elec_dz
						    )
    .set_min(0)
    .set_max(999);

  EffectiveAreaIsolationFilter vetoElectronIso = EffectiveAreaIsolationFilter("VetoElectronIso","vetoElectrons",0.15);
 
  //electron selection 
  CopyCollection<Electron>  selElectronCopyCollection("CopyToSelElectrons","electrons","selElectrons");
  SimpleFilter<Electron> selElectronFilter = SimpleFilter<Electron>
    ("SelElectronPtEtaFilter")
    .set_input_label("selElectrons").set_predicate(bind(MinPtMaxEta, _1, elec_pt, elec_eta) &&
						   bind(Electron2011WP70ID, _1) &&
						   bind(fabs, bind(&Electron::dxy_vertex, _1)) < elec_dxy && 
						   bind(fabs, bind(&Electron::dz_vertex, _1)) < elec_dz
						   )
    .set_min(0)
    .set_max(999);

  EffectiveAreaIsolationFilter selElectronIso = EffectiveAreaIsolationFilter("SelElectronIso","selElectrons",0.10);

  OverlapFilter<Electron, Muon> elecMuonOverlapFilter = OverlapFilter<Electron, Muon>("ElecMuonOverlapFilter")
    .set_input_label("selElectrons")
    .set_reference_label("vetoMuons")
    .set_min_dr(0.3);

  SimpleFilter<Electron> oneElectronFilter = SimpleFilter<Electron>
    ("OneElectronFilter")
    .set_input_label("selElectrons")
    .set_predicate( bind(DummyFunction<Electron>, _1) )
    .set_min(1)
    .set_max(1);


  SimpleFilter<Electron> oneVetoElectronFilter = SimpleFilter<Electron>
    ("OneVetoElectronFilter")
    .set_input_label("vetoElectrons")
    .set_predicate( bind(DummyFunction<Electron>, _1) )
    .set_min(1)
    .set_max(1);

  SimpleFilter<Electron> zeroVetoElectronFilter = SimpleFilter<Electron>
    ("ZeroVetoElectronFilter")
    .set_input_label("vetoElectrons")
    .set_predicate( bind(DummyFunction<Electron>, _1) )
    .set_min(0)
    .set_max(0);



  // ------------------------------------------------------------------------------------
  // Muon Modules
  // ------------------------------------------------------------------------------------
  
  // Muon Veto
  CopyCollection<Muon> vetoMuonCopyCollection("CopyToVetoMuons","muonsPFlow","vetoMuons");

  SimpleFilter<Muon> vetoMuonFilter = SimpleFilter<Muon>
    ("VetoMuonPtEtaFilter")
    .set_input_label("vetoMuons").set_predicate(bind(MinPtMaxEta, _1, veto_muon_pt, veto_muon_eta) &&
						(bind(&Muon::is_global, _1) || bind(&Muon::is_tracker, _1))
						&& bind(PF04Isolation<Muon>, _1, 0.5, 0.2)
						//&& bind(fabs, bind(&Muon::dxy_vertex, _1)) < veto_muon_dxy 
						//&& bind(fabs, bind(&Muon::dz_vertex, _1)) < veto_muon_dz
						)
    .set_min(0)
    .set_max(999);

  CopyCollection<Muon> vetoMuonNoIsoCopyCollection("CopyToVetoMuonsNoIso","muonsPFlow","vetoMuonsNoIso");
  SimpleFilter<Muon> vetoMuonNoIsoFilter = SimpleFilter<Muon>
    ("VetoMuonNoIsoPtEtaFilter")
    .set_input_label("vetoMuonsNoIso").set_predicate(bind(MinPtMaxEta, _1, veto_muon_pt, veto_muon_eta) &&
						(bind(&Muon::is_global, _1) || bind(&Muon::is_tracker, _1))
						//&& bind(PF04Isolation<Muon>, _1, 0.5, 0.2)
						//&& bind(fabs, bind(&Muon::dxy_vertex, _1)) < veto_muon_dxy 
						//&& bind(fabs, bind(&Muon::dz_vertex, _1)) < veto_muon_dz
						)
    .set_min(0)
    .set_max(999);

  //sel muons
  CopyCollection<Muon> selMuonCopyCollection("CopyToSelMuons","muonsPFlow","selMuons");
  SimpleFilter<Muon> selMuonFilter = SimpleFilter<Muon>
    ("SelMuonPtEtaFilter")
    .set_input_label("selMuons").set_predicate(bind(MinPtMaxEta, _1, muon_pt, muon_eta) &&
					       bind(&Muon::is_global, _1) &&
					       bind(MuonTight, _1) && 
					       bind(PF04Isolation<Muon>, _1, 0.5, 0.12) &&
					       bind(fabs, bind(&Muon::dxy_vertex, _1)) < muon_dxy && 
					       bind(fabs, bind(&Muon::dz_vertex, _1)) < muon_dz
					       )
    .set_min(0)
    .set_max(999);


  SimpleFilter<Muon> oneMuonFilter = SimpleFilter<Muon>
    ("OneMuonFilter")
    .set_input_label("selMuons")
    .set_predicate( bind(DummyFunction<Muon>, _1) )
    .set_min(1)
    .set_max(1);
  
  SimpleFilter<Muon> twoMuonFilter = SimpleFilter<Muon>
    ("TwoMuonFilter")
    .set_input_label("selMuons")
    .set_predicate( bind(DummyFunction<Muon>, _1) )
    .set_min(2)
    .set_max(2);

  SimpleFilter<Muon> oneVetoMuonFilter = SimpleFilter<Muon>
    ("OneVetoMuonFilter")
    .set_input_label("vetoMuons")
    .set_predicate( bind(DummyFunction<Muon>, _1) )
    .set_min(1)
    .set_max(1);

  SimpleFilter<Muon> twoVetoMuonFilter = SimpleFilter<Muon>
    ("TwoVetoMuonFilter")
    .set_input_label("vetoMuons")
    .set_predicate( bind(DummyFunction<Muon>, _1) )
    .set_min(2)
    .set_max(2);

  SimpleFilter<Muon> zeroVetoMuonFilter = SimpleFilter<Muon>
    ("ZeroVetoMuonFilter")
    .set_input_label("vetoMuons")
    .set_predicate( bind(DummyFunction<Muon>, _1) )
    .set_min(0)
    .set_max(0);

  OneCollCompositeProducer<Muon> mumuLeadingPairProducer = OneCollCompositeProducer<Muon>
    ("MuMuLeadingPairProducer")
    .set_input_label("selMuons")
    .set_candidate_name_first("muon1")
    .set_candidate_name_second("muon2")
    .set_select_leading_pair(true)
    .set_output_label("mumuLeadingCandidates");                                                        

  SimpleFilter<CompositeCandidate> MassMuonPairFilter = SimpleFilter<CompositeCandidate>("MassMuonPairFilter")
    .set_input_label("mumuLeadingCandidates")
    .set_predicate( bind(PairMassInRange, _1,60,120) )
    .set_min(1)
    .set_max(999);    

  // ------------------------------------------------------------------------------------
  // Tau modules
  // ------------------------------------------------------------------------------------

  SimpleFilter<Tau> tauPtEtaFilter = SimpleFilter<Tau>("TauPtEtaFilter")
    .set_input_label("taus")
    .set_predicate(bind(MinPtMaxEta, _1, 20, 2.3))
    .set_min(0);

  SimpleFilter<Tau> tauDzFilter = SimpleFilter<Tau>("TauDzFilter")
    .set_input_label("taus")
    .set_predicate(bind(fabs, bind(&Tau::lead_dz_vertex, _1)) < 0.2)
    .set_min(0);

  std::string tau_iso_discr, tau_anti_elec_discr_1, tau_anti_elec_discr_2, tau_anti_muon_discr;
  if(!docrosschecktau){
    tau_iso_discr       = "byTightCombinedIsolationDeltaBetaCorr3Hits";
  }
  else{
    tau_iso_discr       = "byMediumIsolationMVA2";
  }
  if(taulepdiscrtight){
    tau_anti_muon_discr   = "againstMuonTight";
    tau_anti_elec_discr_1 = "againstElectronTight";
    tau_anti_elec_discr_2 = "againstElectronTight";
  }
  else{
    tau_anti_muon_discr   = "againstMuonLoose";
    tau_anti_elec_discr_1 = "againstElectronLoose";
    tau_anti_elec_discr_2 = "againstElectronLoose";
  }
  
  //tau_iso_discr         = "byLooseCombinedIsolationDeltaBetaCorr3Hits";
  
  std::cout << "** Tau Discriminators **" << std::endl;
  std::cout << boost::format(param_fmt) % "isolation" %  tau_iso_discr;
  std::cout << boost::format(param_fmt) % "anti-electron1" % tau_anti_elec_discr_1;
  std::cout << boost::format(param_fmt) % "anti-electron2" % tau_anti_elec_discr_2;
  std::cout << boost::format(param_fmt) % "anti-muon" % tau_anti_muon_discr;


  SimpleFilter<Tau> tauIsoFilter = SimpleFilter<Tau>("TauIsoFilter")
    .set_input_label("taus")
    .set_predicate((bind(&Tau::GetTauID, _1, tau_iso_discr) > 0.5) && (bind(&Tau::GetTauID, _1, "decayModeFinding") > 0.5))
    .set_min(0);

  SimpleFilter<Tau> tauElRejectFilter = SimpleFilter<Tau>("TauElRejectFilter")
    .set_predicate( (bind(&Tau::GetTauID, _1, tau_anti_elec_discr_1) > 0.5) && (bind(&Tau::GetTauID, _1, tau_anti_elec_discr_2) > 0.5) )                     
    .set_input_label("taus")
    .set_min(0); 

  SimpleFilter<Tau> tauMuRejectFilter = SimpleFilter<Tau>("TauMuRejectFilter")
    .set_predicate(bind(&Tau::GetTauID, _1, tau_anti_muon_discr) > 0.5)
    .set_input_label("taus")
    .set_min(0);


  SimpleFilter<Tau> oneTauFilter = SimpleFilter<Tau>
    ("OneTauFilter")
    .set_input_label("taus")
    .set_predicate( bind(DummyFunction<Tau>, _1) )
    .set_min(1)
    .set_max(1000);
  // ------------------------------------------------------------------------------------
  // Jet Modules
  // ------------------------------------------------------------------------------------  

  JetMETModifier ModifyJetMET = JetMETModifier
    ("ModifyJetMET")
    .set_input_label("pfJetsPFlow")
    .set_met_label(mettype)
    .set_dosmear(dosmear)
    .set_doaltmatch(doaltmatch)
    .set_doetsmear(doetsmear)
    .set_dogaus(dogaus)
    .set_dospring10gaus(dospring10gaus)
    .set_is_data(is_data)
    .set_dojessyst(dojessyst)
    .set_dodatajessyst(dodatajessyst)
    .set_jesupordown(jesupordown)
    .set_dojersyst(dojersyst)
    .set_jerbetterorworse(jerbetterorworse)
    .set_jesuncfile(jesuncfile)
    .set_dojerdebug(dojerdebug)
    .set_randomseed(randomseed)
    .set_fs(fs);
  

  
  
  
  CopyCollection<PFJet> alljetsCopyCollection("copytoalljets","pfJetsPFlow","AllpfJetsPFlow");
  CopyCollection<PFJet> bjetsCopyCollection("copytobjets","pfJetsPFlow","bTaggedpfJetsPFlow");

  SimpleFilter<PFJet> jetIDFilter = SimpleFilter<PFJet>
    ("JetIDFilter")
    .set_input_label("pfJetsPFlow");
    if(!turnoffpuid){
      jetIDFilter.set_predicate((bind(PFJetID, _1)) && bind(&PFJet::pu_id_mva_loose, _1));
    }
    else{
      jetIDFilter.set_predicate(bind(PFJetID, _1));
    }
    
  // Jet pT eta filter
  //CopyCollection<PFJet> jetCopyCollection("CopyToJet","pfJetsPFlow","selJets");



  SimpleFilter<PFJet> jetPtEtaFilter = SimpleFilter<PFJet>
    ("JetPtEtaFilter")
    .set_input_label("pfJetsPFlow").set_predicate(bind(MinPtMaxEta, _1, jetptprecut, 4.7));


  CJVFilter FilterCJV = CJVFilter("FilterCJV")
    .set_jetsinput_label("pfJetsPFlow")
    .set_pairinput_label("jjLeadingCandidates")
    .set_ptcut(30);
  
  SimpleFilter<PFJet> fourJetsFilter = SimpleFilter<PFJet>
    ("FourJetsFilter")
    .set_input_label("pfJetsPFlow")
    .set_predicate( bind(DummyFunction<PFJet>, _1) )
    .set_min(4)
    .set_max(1000);

  SimpleFilter<PFJet> oneBtaggedJetFilter = SimpleFilter<PFJet>
    ("oneBtaggedJetFilter")
    .set_input_label("bTaggedpfJetsPFlow")
    .set_predicate( bind(CSVMediumWP, _1, 0.679) )
    .set_min(1)
    .set_max(1000);

  //in principle no need to remove overlap because events with leptons are rejected...
  //except for specific e/mu selection for W background estimation.
  // and to calculate efficiencies of cuts correctly
  //we want the leading jets to really be jets even before vetoing leptons...
  //so: removal of jets matched with veto electrons and muons
  OverlapFilter<PFJet, Electron> jetElecOverlapFilter = OverlapFilter<PFJet, Electron>("jetElecOverlapFilter")
    .set_input_label("pfJetsPFlow")
    .set_reference_label("vetoElectrons")
    .set_min_dr(0.5);

  OverlapFilter<PFJet, Muon> jetMuonOverlapFilter = OverlapFilter<PFJet, Muon>("jetMuonOverlapFilter")
    .set_input_label("pfJetsPFlow")
    .set_reference_label("vetoMuons") //NoIso")
    .set_min_dr(0.5);

  OverlapFilter<PFJet, Tau> jetTauOverlapFilter = OverlapFilter<PFJet, Tau>("jetTauOverlapFilter")
    .set_input_label("pfJetsPFlow")
    .set_reference_label("taus") //NoIso")
    .set_min_dr(0.5);

  OneCollCompositeProducer<PFJet> jjPairProducer = OneCollCompositeProducer<PFJet>
    ("JetJetPairProducer")
    .set_input_label("pfJetsPFlow")
    .set_candidate_name_first("jet1")
    .set_candidate_name_second("jet2")
    .set_select_leading_pair(false)
    .set_output_label("jjCandidates");                                                        

  OneCollCompositeProducer<PFJet> jjLeadingPairProducer = OneCollCompositeProducer<PFJet>
    ("JetJetLeadingPairProducer")
    .set_input_label("pfJetsPFlow")
    .set_candidate_name_first("jet1")
    .set_candidate_name_second("jet2")
    .set_select_leading_pair(true)
    .set_output_label("jjLeadingCandidates");                                                        

  bool cutaboveorbelow=true;
  SimpleFilter<CompositeCandidate> jetPairFilter = SimpleFilter<CompositeCandidate>("JetPairFilter")
    .set_input_label("jjLeadingCandidates")
    .set_predicate( bind(OrderedPairPtSelection, _1,jet1ptcut, jet2ptcut, cutaboveorbelow) )
    .set_min(1)
    .set_max(999);

  cutaboveorbelow=false;
  SimpleFilter<CompositeCandidate> jetPairMaxPtFilter = SimpleFilter<CompositeCandidate>("JetPairMaxPtFilter")
    .set_input_label("jjLeadingCandidates")
    .set_predicate( bind(OrderedPairPtSelection, _1, jet1ptcutmax, jet2ptcutmax, cutaboveorbelow) )
    .set_min(1)
    .set_max(999);
  
  SimpleFilter<CompositeCandidate> etaProdJetPairFilter = SimpleFilter<CompositeCandidate>("EtaProdJetPairFilter")
    .set_input_label("jjLeadingCandidates")
    .set_predicate( bind(PairEtaProdLessThan, _1, 0))
    .set_min(1)
    .set_max(999);


  SimpleFilter<CompositeCandidate> detaJetPairFilter = SimpleFilter<CompositeCandidate>("DetaJetPairFilter")
    .set_input_label("jjLeadingCandidates")
    .set_predicate( !bind(PairDEtaLessThan, _1, detajjcut) )
    .set_min(1)
    .set_max(999);    

  SimpleFilter<CompositeCandidate> detaJetPairMaxFilter = SimpleFilter<CompositeCandidate>("DetaJetPairMaxFilter")
    .set_input_label("jjLeadingCandidates")
    .set_predicate( bind(PairDEtaLessThan, _1, detajjcut_max) )
    .set_min(1)
    .set_max(999);    


  double minmjj=900;
  if(do_skim)minmjj=600;

  SimpleFilter<CompositeCandidate> looseMassJetPairFilter = SimpleFilter<CompositeCandidate>("LooseMassJetPairFilter")
    .set_input_label("jjLeadingCandidates")
    .set_predicate( bind(PairMassInRange, _1,minmjj,8000) )
    .set_min(1)
    .set_max(999);
    
  SimpleFilter<CompositeCandidate> tightMassJetPairFilter = SimpleFilter<CompositeCandidate>("TightMassJetPairFilter")
    .set_input_label("jjLeadingCandidates")
    .set_predicate( bind(PairMassInRange, _1,mjj_cut,mjj_cut_max) )
    .set_min(1)
    .set_max(999);    

 
  SimpleFilter<CompositeCandidate> dphiJetPairFilter = SimpleFilter<CompositeCandidate>("DphiJetPairFilter")
    .set_input_label("jjLeadingCandidates")
    .set_predicate( bind(PairAbsDPhiLessThan, _1,1.0) )
    .set_min(1)
    .set_max(999);    

  SimpleFilter<CompositeCandidate> dphiQCDJetPairFilter = SimpleFilter<CompositeCandidate>("DphiQCDJetPairFilter")
    .set_input_label("jjLeadingCandidates")
    .set_predicate( !bind(PairAbsDPhiLessThan, _1,2.6) )
    .set_min(1)
    .set_max(999);    

  SimpleFilter<CompositeCandidate> dphilowFilter = SimpleFilter<CompositeCandidate>("DphiLowFilter")
    .set_input_label("jjLeadingCandidates")
    .set_predicate( !bind(PairAbsDPhiLessThan, _1,dphi_cut) )
    .set_min(1)
    .set_max(999);    

  SimpleFilter<CompositeCandidate> dphimaxFilter = SimpleFilter<CompositeCandidate>("DphiMaxFilter")
    .set_input_label("jjLeadingCandidates")
    .set_predicate( bind(PairAbsDPhiLessThan, _1,dphi_cut_max) )
    .set_min(1)
    .set_max(999);    

  OneCollCompositeProducer<PFJet> dummyjjPairProducer = OneCollCompositeProducer<PFJet>
    ("DummyJetJetPairProducer")
    .set_input_label("pfJetsPFlow")
    .set_candidate_name_first("jet1")
    .set_candidate_name_second("jet2")
    .set_select_leading_pair(false)
    .set_output_label("dummyjjCandidates");                                                        
 
  // ------------------------------------------------------------------------------------
  // Met Modules
  // ------------------------------------------------------------------------------------  
  MetSelection metFilters = MetSelection("MetFilters",mettype,doMetFilters,filtersVec,0);
  

  unsigned nLepToAdd = 100;
  //no need to be explicit in the number of leptons: will take the number 
  // that is asked in the selection (i.e. exactly one for the W->e,mu selections...)
  //   if (channel == channel::munu || 
  //       channel == channel::enu || 
  //       channel == channel::emu
  //       ) nLepToAdd = 1;
  
  ModifyMet metNoMuons = ModifyMet("metNoMuons",mettype,"selMuons",2,nLepToAdd);
  ModifyMet metNoElectrons = ModifyMet("metNoElectrons",mettype,"selElectrons",1,nLepToAdd);
  ModifyMet metNoENoMu = ModifyMet("metNoENoMu","metNoMuons","selElectrons",1,nLepToAdd);

  MetSelection metNoMuonFilter = MetSelection("MetNoMuonFilter","metNoMuons",false,filtersVec,met_cut,met_cut_max);
  MetSelection metNoElectronFilter = MetSelection("MetNoElectronFilter","metNoElectrons",false,filtersVec,met_cut,met_cut_max);
  MetSelection metNoENoMuFilter = MetSelection("MetNoENoMuFilter","metNoENoMu",false,filtersVec,met_cut,met_cut_max);

  //if (is_ewkZ)  mettype="metNoENoMu";
  if (ignoreLeptons) mettype="metNoMuons";
  if (channel==channel::mumu) mettype="metNoMuons"; 
  MetSelection metCut = MetSelection("MetCutFilter",mettype,false,filtersVec,met_cut,met_cut_max);

  //------------------------------------------------------------------------------------
  // W selection Modules
  // ------------------------------------------------------------------------------------
  double mtcut_min = 0;
  double mtcut_max = 8000;
  MTSelection muonMTFilter = MTSelection("MuonMTFilter",mettype,"selMuons",2,mtcut_min,mtcut_max);
  MTSelection electronMTFilter = MTSelection("ElectronMTFilter",mettype,"selElectrons",1,mtcut_min,mtcut_max);

  // ------------------------------------------------------------------------------------
  // Selection Modules
  // ------------------------------------------------------------------------------------  

  
  HinvWeights hinvWeights = HinvWeights("HinvWeights")
    .set_era(era)
    .set_mc(mc)
    .set_save_weights(true)
    .set_do_trg_weights(false)
    .set_trg_applied_in_mc(true)
    .set_do_idiso_tight_weights(false)
    .set_do_idiso_veto_weights(false)
    .set_do_idiso_err(doidisoerr)
    .set_do_idiso_errupordown(doidisoerrupordown)
    .set_do_idiso_errmuore(doidisoerrmuore)
    .set_fs(fs)
    .set_input_met("metNoMuons");
  //  if (channel == channel::enu || channel == channel::emu) hinvWeights.set_input_met("metNoENoMu");
  if (!is_data) {
    hinvWeights.set_do_trg_weights(dotrgeff)
      .set_trg_weight_file(trg_weight_file)
      .set_trg_applied_in_mc(true);
    if(!do_light_tree){
      if (channel==channel::nunu ||channel==channel::nunulowmet|| channel == channel::taunu){
	hinvWeights.set_do_idiso_veto_weights(doidisoeff);
      }
      else hinvWeights.set_do_idiso_tight_weights(doidisoeff);
    }
    else{
      hinvWeights.set_do_idiso_veto_weights(false);
      hinvWeights.set_do_idiso_tight_weights(false);
    }
    if (ignoreLeptons){ 
      hinvWeights.set_do_idiso_veto_weights(false);
      hinvWeights.set_do_idiso_tight_weights(false);
    }
  }

  HinvWeights xsWeights = HinvWeights("XSWeights")
    .set_era(era)
    .set_mc(mc)
    .set_save_weights(false)
    .set_do_trg_weights(false)
    .set_trg_applied_in_mc(false)
    .set_do_idiso_tight_weights(false)
    .set_do_idiso_veto_weights(false)
    .set_do_lumixs_weights(dolumixsweight)
    .set_input_params(inputparams)
    .set_sample_name(output_name)
    .set_fs(fs);
  

  if (output_name.find("JetsToLNu") != output_name.npos) {
    xsWeights.set_do_w_soup(true);
    xsWeights.set_do_w_reweighting(false);

    if (mc == mc::summer12_53X) {
      xsWeights.SetWTargetFractions(0.74069073, 0.1776316, 0.0575658, 0.0170724, 0.00703947);
      xsWeights.SetWInputYields(76102995.0, 23141598.0, 34044921.0, 15539503.0, 13382803.0);
      //xsWeights.SetWInputYields(76102995.0, 23141598.0, 33901569.0, 15539503.0, 13382803.0);
    }
  }
  if (output_name.find("JetsToLL") != output_name.npos && 
      output_name.find("PtZ-100-madgraph") == output_name.npos && 
      output_name.find("DYJJ01") == output_name.npos) {
    if (mc == mc::summer12_53X) {
      xsWeights.set_do_dy_soup(true);
      xsWeights.set_do_dy_reweighting(false);
      xsWeights.SetDYTargetFractions(0.723342373, 0.190169492, 0.061355932, 0.017322034, 0.007810169);
      if(prod=="Apr04"){
	xsWeights.SetDYInputYields(30459503.0, 23970248.0, 21852156.0, 11015445.0, 6402827.0);
      }
      else{
	xsWeights.SetDYInputYields(30459503.0, 24045248.0, 21852156.0, 11015445.0, 6402827.0);
      }
    }
  }

  // ------------------------------------------------------------------------------------
  // Gen particle selection modules
  // ------------------------------------------------------------------------------------

  int lFlavour = 1;
  //if (channel == channel::enu) lFlavour = 11;
  //else if (channel == channel::munu) lFlavour = 13;
  //else if (channel == channel::taunu) lFlavour = 15;
  if (wstream == "enu") lFlavour = 11;
  else if (wstream == "munu") lFlavour = 13;
  else if (wstream == "taunu") lFlavour = 15;



  HinvWDecay WtoLeptonFilter = HinvWDecay("WtoLeptonSelector",lFlavour);

  //if (wstream == "ee") lFlavour = 11;
  //else if (wstream == "mumu") lFlavour = 13;
  //else if (wstream == "tautau") lFlavour = 15;
  //do only muons for Znunu estimate...
  HinvZDecay ZmassFilter = HinvZDecay("ZmassFilter",13,80,100);
  HinvZDecay ZmassPreFilter = HinvZDecay("ZmasspreFilter",13,0,9999999);
  HinvZDecay ZlowmassFilter = HinvZDecay("ZlowmassFilter",13,0,100,true);

  // ------------------------------------------------------------------------------------
  // Plot Modules
  // ------------------------------------------------------------------------------------  
  
  TmvaInputs tmvaInputs = TmvaInputs("TmvaInputs")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("JetPair")
    .set_is_data(is_data)
    .set_channel(channel_str);

  TrigeffInputs trigeffInputs = TrigeffInputs("TrigeffInputs")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("JetPair")
    .set_is_data(is_data)
    .set_channel(channel_str)
    .set_trigger_path("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v")
    .set_trig_obj_label("triggerObjectsDiPFJet40PFMETnoMu65MJJ800VBFAllJets");

//   LightTree lightTree = LightTree("LightTree")
//     .set_fs(fs)
//     .set_met_label(mettype)
//     .set_dijet_label("jjLeadingCandidates")
//     .set_sel_label("JetPair")
//     .set_is_data(is_data)
//     .set_trigger_path("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v")
//     .set_trig_obj_label("triggerObjectsDiPFJet40PFMETnoMu65MJJ800VBFAllJets");

  HinvControlPlots controlPlots_gen = HinvControlPlots("GenControlPlots")
    .set_fs(fs)
    .set_met_label("pfMetType1")
    .set_dijet_label("dummyjjCandidates")
    .set_sel_label("GenNoFilters")
    .set_is_data(is_data)
    .set_genlevelskip(true)
    .set_channel(channel_str);

  HinvControlPlots controlPlots_genzmassfiltered = HinvControlPlots("GenZMassFilteredControlPlots")
    .set_fs(fs)
    .set_met_label("pfMetType1")
    .set_dijet_label("dummyjjCandidates")
    .set_sel_label("GenZMassFiltered")
    .set_is_data(is_data)
    .set_genlevelskip(true)
    .set_channel(channel_str);

 
  HinvControlPlots controlPlots_hlt = HinvControlPlots("HLTControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjCandidates")
    .set_sel_label("HLTMetClean")
    .set_is_data(is_data)
    .set_channel(channel_str);

  if (is_embedded) controlPlots_hlt.set_genparticles_label("genParticlesEmbedded").set_is_embedded(true);

  //set collections to all leptons for the first set of plots before selecting/vetoing them
  HinvWJetsPlots wjetsPlots_hlt = HinvWJetsPlots("HLTWJetsPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("electrons")
    .set_muons_label("muonsPFlow")
    .set_dijet_label("jjCandidates")
    .set_sel_label("HLTMetClean");

  //  if (channel==channel::enu)
  //wjetsPlots_hlt.set_met_nolep_label("metNoElectrons");
  //else if (channel==channel::emu)
  //wjetsPlots_hlt.set_met_nolep_label("metNoENoMu");


  HinvControlPlots controlPlots_dijet = HinvControlPlots("DijetControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("JetPair")
    .set_is_data(is_data)
    .set_channel(channel_str);

  if (is_embedded) controlPlots_dijet.set_genparticles_label("genParticlesEmbedded").set_is_embedded(true);

  //set collections to all leptons for the first set of plots before selecting/vetoing them
  HinvWJetsPlots wjetsPlots_dijet = HinvWJetsPlots("DijetWJetsPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("electrons")
    .set_muons_label("muonsPFlow")
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("JetPair");

  //if (channel==channel::enu)
  //wjetsPlots_dijet.set_met_nolep_label("metNoElectrons");
  //else if (channel==channel::emu)
  //wjetsPlots_dijet.set_met_nolep_label("metNoENoMu");

  HinvControlPlots controlPlots_AN = HinvControlPlots("ANControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_is_data(is_data)
    .set_sel_label("AN")
    .set_channel(channel_str);

  if (is_embedded) controlPlots_AN.set_genparticles_label("genParticlesEmbedded").set_is_embedded(true);

  
  HinvWJetsPlots wjetsPlots_AN = HinvWJetsPlots("ANWJetsPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("selElectrons")
    .set_muons_label("selMuons")
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("AN");

  //if (channel==channel::enu)
  //wjetsPlots_AN.set_met_nolep_label("metNoElectrons");
  //else if (channel==channel::emu)
  //wjetsPlots_AN.set_met_nolep_label("metNoENoMu");

  HinvControlPlots controlPlots_met = HinvControlPlots("METControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("MET")
    .set_is_data(is_data)
    .set_channel(channel_str);

  if (is_embedded) controlPlots_met.set_genparticles_label("genParticlesEmbedded").set_is_embedded(true);

    HinvWJetsPlots wjetsPlots_met = HinvWJetsPlots("METWJetsPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("selElectrons")
    .set_muons_label("selMuons")
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("MET");

  //if (channel==channel::enu)
  //wjetsPlots_met.set_met_nolep_label("metNoElectrons");
  //else if (channel==channel::emu)
  //wjetsPlots_met.set_met_nolep_label("metNoENoMu");

  HinvControlPlots controlPlots_looseMjj = HinvControlPlots("LooseMjjControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("LooseMjj")
    .set_is_data(is_data)
    .set_channel(channel_str);

  if (is_embedded) controlPlots_looseMjj.set_genparticles_label("genParticlesEmbedded").set_is_embedded(true);

  HinvControlPlots controlPlots_deta = HinvControlPlots("DEtaControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("DEta")
    .set_is_data(is_data)
    .set_channel(channel_str);

  if (is_embedded) controlPlots_deta.set_genparticles_label("genParticlesEmbedded").set_is_embedded(true);

  HinvWJetsPlots wjetsPlots_deta = HinvWJetsPlots("DEtaWJetsPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("selElectrons")
    .set_muons_label("selMuons")
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("DEta");

  //if (channel==channel::enu)
  //wjetsPlots_deta.set_met_nolep_label("metNoElectrons");
  //else if (channel==channel::emu)
  //wjetsPlots_deta.set_met_nolep_label("metNoENoMu");

  HinvControlPlots controlPlots_lepveto = HinvControlPlots("LeptonVetoControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("LeptonVeto")
    .set_is_data(is_data)
    .set_channel(channel_str);

  if (is_embedded) controlPlots_lepveto.set_genparticles_label("genParticlesEmbedded").set_is_embedded(true);

  HinvControlPlots controlPlots_wsel = HinvControlPlots("WSelectionControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("WSelection")
    .set_is_data(is_data)
    .set_channel(channel_str);

  HinvControlPlots controlPlots_zsel = HinvControlPlots("ZSelectionControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("ZSelection")
    .set_is_data(is_data)
    .set_channel(channel_str);

  HinvControlPlots controlPlots_iglep = HinvControlPlots("IgnoreLeptonsControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("IgnoreLeptons")
    .set_is_data(is_data)
    .set_channel(channel_str);

  if (is_embedded) controlPlots_wsel.set_genparticles_label("genParticlesEmbedded").set_is_embedded(true);

  HinvWJetsPlots wjetsPlots_wsel = HinvWJetsPlots("WSelectionWJetsPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("selElectrons")
    .set_muons_label("selMuons")
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("WSelection");

  //if (channel==channel::enu)
  //wjetsPlots_wsel.set_met_nolep_label("metNoElectrons");
  //else if (channel==channel::emu)
  //wjetsPlots_wsel.set_met_nolep_label("metNoENoMu");

  HinvControlPlots controlPlots_dphi_qcd_cjvpass = HinvControlPlots("DPhiControlPlotsQCD_cjvpass")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("DPhiQCD_CJVpass")
    .set_is_data(is_data)
    .set_channel(channel_str);

  if (is_embedded) controlPlots_dphi_qcd_cjvpass.set_genparticles_label("genParticlesEmbedded").set_is_embedded(true);

  HinvWJetsPlots wjetsPlots_dphi_qcd_cjvpass = HinvWJetsPlots("DPhiWJetsPlotsQCD_cjvpass")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("selElectrons")
    .set_muons_label("selMuons")
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("DPhiQCD_CJVpass");

  HinvControlPlots controlPlots_dphi_qcd_nocjv = HinvControlPlots("DPhiControlPlotsQCD_noCJV")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("DPhiQCD_noCJV")
    .set_is_data(is_data)
    .set_channel(channel_str);

  if (is_embedded) controlPlots_dphi_qcd_nocjv.set_genparticles_label("genParticlesEmbedded").set_is_embedded(true);

  HinvWJetsPlots wjetsPlots_dphi_qcd_nocjv = HinvWJetsPlots("DPhiWJetsPlotsQCD_noCJV")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("selElectrons")
    .set_muons_label("selMuons")
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("DPhiQCD_noCJV");

  HinvControlPlots controlPlots_dphi_qcd_cjvfail = HinvControlPlots("DPhiControlPlotsQCD_cjvfail")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("DPhiQCD_CJVfail")
    .set_is_data(is_data)
    .set_channel(channel_str);

  if (is_embedded) controlPlots_dphi_qcd_cjvfail.set_genparticles_label("genParticlesEmbedded").set_is_embedded(true);

  HinvWJetsPlots wjetsPlots_dphi_qcd_cjvfail = HinvWJetsPlots("DPhiWJetsPlotsQCD_cjvfail")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("selElectrons")
    .set_muons_label("selMuons")
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("DPhiQCD_CJVfail");

  //if (channel==channel::enu)
  //wjetsPlots_dphi_qcd.set_met_nolep_label("metNoElectrons");
  //else if (channel==channel::emu)
  //wjetsPlots_dphi_qcd.set_met_nolep_label("metNoENoMu");

  HinvControlPlots controlPlots_dphi_signal_cjvpass = HinvControlPlots("DPhiControlPlotsSIGNAL_cjvpass")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("DPhiSIGNAL_CJVpass")
    .set_is_data(is_data)
    .set_channel(channel_str);

  if (is_embedded) controlPlots_dphi_signal_cjvpass.set_genparticles_label("genParticlesEmbedded").set_is_embedded(true);

  HinvWJetsPlots wjetsPlots_dphi_signal_cjvpass = HinvWJetsPlots("DPhiWJetsPlotsSIGNAL_cjvpass")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("selElectrons")
    .set_muons_label("selMuons")
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("DPhiSIGNAL_CJVpass");

  HinvControlPlots controlPlots_dphi_signal_nocjv = HinvControlPlots("DPhiControlPlotsSIGNAL_noCJV")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("DPhiSIGNAL_noCJV") 
    .set_is_data(is_data)
    .set_channel(channel_str);

  if (is_embedded) controlPlots_dphi_signal_nocjv.set_genparticles_label("genParticlesEmbedded").set_is_embedded(true);

  HinvWJetsPlots wjetsPlots_dphi_signal_nocjv = HinvWJetsPlots("DPhiWJetsPlotsSIGNAL_noCJV")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("selElectrons")
    .set_muons_label("selMuons")
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("DPhiSIGNAL_noCJV");

  HinvControlPlots controlPlots_dphi_signal_cjvfail = HinvControlPlots("DPhiControlPlotsSIGNAL_cjvfail")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("DPhiSIGNAL_CJVfail")
    .set_is_data(is_data)
    .set_channel(channel_str);

  if (is_embedded) controlPlots_dphi_signal_cjvfail.set_genparticles_label("genParticlesEmbedded").set_is_embedded(true);

  HinvWJetsPlots wjetsPlots_dphi_signal_cjvfail = HinvWJetsPlots("DPhiWJetsPlotsSIGNAL_cjvfail")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("selElectrons")
    .set_muons_label("selMuons")
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("DPhiSIGNAL_CJVfail");

  //if (channel==channel::enu)
  //wjetsPlots_dphi_signal.set_met_nolep_label("metNoElectrons");
  //else if (channel==channel::emu)
  //wjetsPlots_dphi_signal.set_met_nolep_label("metNoENoMu");

  HinvControlPlots controlPlots_tightMjj = HinvControlPlots("TightMjjControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("TightMjj")
    .set_is_data(is_data)
    .set_channel(channel_str);

  if (is_embedded) controlPlots_tightMjj.set_genparticles_label("genParticlesEmbedded").set_is_embedded(true);

  HinvWJetsPlots wjetsPlots_tightMjj = HinvWJetsPlots("TightMjjWJetsPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("selElectrons")
    .set_muons_label("selMuons")
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("TightMjj");

  HinvControlPlots controlPlots_cjvpass = HinvControlPlots("CJVControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("CJVpass")
    .set_is_data(is_data)
    .set_channel(channel_str);

  if (is_embedded) controlPlots_cjvpass.set_genparticles_label("genParticlesEmbedded").set_is_embedded(true);

  HinvWJetsPlots wjetsPlots_cjvpass = HinvWJetsPlots("CJVWJetsPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("selElectrons")
    .set_muons_label("selMuons")
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("CJVpass");

  HinvControlPlots controlPlots_cjvfail = HinvControlPlots("CJVFailControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("CJVfail")
    .set_is_data(is_data)
    .set_channel(channel_str);

  if (is_embedded) controlPlots_cjvfail.set_genparticles_label("genParticlesEmbedded").set_is_embedded(true);

  HinvWJetsPlots wjetsPlots_cjvfail = HinvWJetsPlots("CJVFailWJetsPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("selElectrons")
    .set_muons_label("selMuons")
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("CJVfail");

  HinvControlPlots controlPlots_pretrig_cjvfail = HinvControlPlots("pretrigCJVFailControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("pretrig_CJVfail")
    .set_is_data(is_data)
    .set_channel(channel_str);

  HinvControlPlots controlPlots_pretrig_cjvpass = HinvControlPlots("pretrigCJVPassControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("pretrig_CJVpass")
    .set_is_data(is_data)
    .set_channel(channel_str);

  HinvControlPlots controlPlots_pretrig_nocjv = HinvControlPlots("pretrigControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("pretrig_nocjv")
    .set_is_data(is_data)
    .set_channel(channel_str);

  HinvControlPlots controlPlots_posttrig_cjvfail = HinvControlPlots("posttrigCJVFailControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("posttrig_CJVfail")
    .set_is_data(is_data)
    .set_channel(channel_str);

  HinvControlPlots controlPlots_posttrig_cjvpass = HinvControlPlots("posttrigCJVPassControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("posttrig_CJVpass")
    .set_is_data(is_data)
    .set_channel(channel_str);

  HinvControlPlots controlPlots_posttrig_nocjv = HinvControlPlots("posttrigControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("posttrig_nocjv")
    .set_is_data(is_data)
    .set_channel(channel_str);


  //if (channel==channel::enu)
  //wjetsPlots_tightMjj.set_met_nolep_label("metNoElectrons");
  //else if (channel==channel::emu)
  //wjetsPlots_tightMjj.set_met_nolep_label("metNoENoMu");

  // ------------------------------------------------------------------------------------
  // Build Analysis Sequence
  // ------------------------------------------------------------------------------------  

  if (doMCFMstudy) {
    analysis.AddModule(&mcfmStudy);
  }
  else {


  //if (is_data && !do_skim)        analysis.AddModule(&lumiMask);
    if (!is_data) {
      //do W streaming to e,mu,tau
      if (output_name.find("JetsToLNu") != output_name.npos ||
	  output_name.find("EWK-W2j") != output_name.npos) {
	if (wstream != "nunu") analysis.AddModule(&WtoLeptonFilter);
      }
      if (!do_skim)       {
	//Put 100 GeV gen level cut on inclusive DY samples
	analysis.AddModule(&pileupWeight);
	analysis.AddModule(&pileupWeight_up);
	analysis.AddModule(&pileupWeight_down);
	//just apply W and Z weights
	analysis.AddModule(&xsWeights);
	//Z pt <100 GeV cut for inclusive DY samples
	if(doincludehighptz && output_name.find("JetsToLL") != output_name.npos && output_name.find("PtZ-100-madgraph") == output_name.npos && output_name.find("DYJJ01") == output_name.npos){
	  analysis.AddModule(&ZlowmassFilter);
	}


      }
      if(!do_skim){
	if(do_gensteps){
	  if (ignoreLeptons){
	    analysis.AddModule(&dummyjjPairProducer);
	    analysis.AddModule(&ZmassPreFilter);
	    analysis.AddModule(&controlPlots_gen);
	    analysis.AddModule(&ZmassFilter);
	    analysis.AddModule(&controlPlots_genzmassfiltered);
	  }
	  else{
	    analysis.AddModule(&dummyjjPairProducer);
	    analysis.AddModule(&controlPlots_gen); //Add dummy steps so that controlplots.cpp finds these directories
	    analysis.AddModule(&controlPlots_genzmassfiltered);
	  }
	}
	else if(channel == channel::nunuiglep||channel == channel::nunulowmetiglep){
	  analysis.AddModule(&ZmassFilter);
	}
      }
    }
    else if(is_data && !do_skim  && do_gensteps){
      analysis.AddModule(&dummyjjPairProducer);
      analysis.AddModule(&controlPlots_gen);//Add dummy step so that ControlPlots.cpp finds these directories
      analysis.AddModule(&controlPlots_genzmassfiltered);
    }

   
   //if (!do_skim) {

     //if (printEventList) analysis.AddModule(&hinvPrintList);

    //Require trigger to fire unless doing trig eff studies, also do not require trigger for MC in parked analysis
    if(!do_trigeff_tree&&!do_trigeff&&!do_light_tree){
      if(is_data||(!is_data&&!do_parked_trigs)) analysis.AddModule(&dataMCTriggerPathFilter);
    }
    
    //if (printEventList) analysis.AddModule(&hinvPrintList);
  
    //NEW: change skimming to write event at a specific moment in the chain of modules.
    if (do_skim) analysis.WriteSkimHere();
    
    ////analysis.AddModule(&runStats);
    
    if (is_data && !is_embedded ) {
      //FIXME: do MetFilters also on MC, but not saved right now in MC...
      analysis.AddModule(&metFilters);
      analysis.AddModule(&metEventFilters);
      //if (printEventList) analysis.AddModule(&hinvPrintList);
    }
    
    //jet modules
    analysis.AddModule(&jetIDFilter);
    //don't want pile-up jets to calculate HT,MHT...
    analysis.AddModule(&alljetsCopyCollection);
    
    
    //prepare collections of veto leptons
    analysis.AddModule(&vetoElectronCopyCollection);
    analysis.AddModule(&vetoElectronFilter);
    analysis.AddModule(&vetoElectronIso);
    analysis.AddModule(&vetoMuonCopyCollection);
    analysis.AddModule(&vetoMuonFilter);
    // analysis.AddModule(&vetoMuonNoIsoCopyCollection);
    //analysis.AddModule(&vetoMuonNoIsoFilter);
    
    //filter leptons before making jet pairs and changing MET...
    analysis.AddModule(&selElectronCopyCollection);
    analysis.AddModule(&selElectronFilter);
    analysis.AddModule(&selElectronIso);
    analysis.AddModule(&selMuonCopyCollection);
    analysis.AddModule(&selMuonFilter);
    analysis.AddModule(&elecMuonOverlapFilter);
    
    //filter taus for plots
    analysis.AddModule(&tauPtEtaFilter);
   
    //add met without leptons for plots
    analysis.AddModule(&metNoMuons);
    analysis.AddModule(&metNoElectrons);
    analysis.AddModule(&metNoENoMu);
    
    //if (printEventList) analysis.AddModule(&hinvPrintList);
    
    //deal with removing overlap with selected leptons
    analysis.AddModule(&jetMuonOverlapFilter);
    analysis.AddModule(&jetElecOverlapFilter);
     //no need to clean taus, we don't do it in the signal selection.
     //if (channel == channel::taunu) analysis.AddModule(&jetTauOverlapFilter);

     //Module to do jet smearing and systematics
     if (!do_skim) analysis.AddModule(&ModifyJetMET);

    //jet pair production before plotting
     analysis.AddModule(&jjPairProducer);



     //plot before cutting
     analysis.AddModule(&controlPlots_hlt);
     analysis.AddModule(&wjetsPlots_hlt);

     //filter taus
     analysis.AddModule(&tauDzFilter);
     analysis.AddModule(&tauIsoFilter);
     analysis.AddModule(&tauElRejectFilter);
     analysis.AddModule(&tauMuRejectFilter);

     //filter jets
     analysis.AddModule(&jetPtEtaFilter);
     //if (printEventContent) analysis.AddModule(&hinvPrint);
    
     //two-leading jet pair production before plotting
     analysis.AddModule(&jjLeadingPairProducer);
     //if (printEventContent) analysis.AddModule(&hinvPrint);

     //analysis.AddModule(&hinvPrint);
     
     //lepton selections or veto
     if (channel == channel::munu){
       analysis.AddModule(&oneMuonFilter);
       analysis.AddModule(&oneVetoMuonFilter);
       analysis.AddModule(&zeroVetoElectronFilter);
       //analysis.AddModule(&muonMTFilter);
     }
     else if (channel == channel::mumu){//Require exactly 2 reco muons in the detector acceptance in the Z mass window
       analysis.AddModule(&twoMuonFilter);
       analysis.AddModule(&twoVetoMuonFilter);
       analysis.AddModule(&zeroVetoElectronFilter);
       analysis.AddModule(&mumuLeadingPairProducer);
       analysis.AddModule(&MassMuonPairFilter);
       //analysis.AddModule(&muonMTFilter); //Leave this commented out for now
     }
     else if (channel == channel::enu){
       analysis.AddModule(&oneElectronFilter);
       analysis.AddModule(&oneVetoElectronFilter);
       analysis.AddModule(&zeroVetoMuonFilter);
       //analysis.AddModule(&electronMTFilter);
     }
     else if (channel == channel::emu){
       analysis.AddModule(&oneElectronFilter);
       //analysis.AddModule(&oneVetoElectronFilter);
       analysis.AddModule(&oneMuonFilter);
       //analysis.AddModule(&oneVetoMuonFilter);
     }
     else if (channel == channel::taunu){
       analysis.AddModule(&oneTauFilter);
       if (!ignoreLeptons && !doTopCR){
	 analysis.AddModule(&zeroVetoElectronFilter);
	 analysis.AddModule(&zeroVetoMuonFilter);
       }
     }
     else {
       //lepton veto modules
       if (!ignoreLeptons){
	 if(!do_trigeff_tree&&!do_trigeff&&!do_light_tree){
	   analysis.AddModule(&zeroVetoMuonFilter);
	   //if (printEventList) analysis.AddModule(&hinvPrintList);
	   analysis.AddModule(&zeroVetoElectronFilter);
	 }
	 //if (printEventList) analysis.AddModule(&hinvPrintList);
      }
     }

     //Need two jets and metnomuons to apply trigger weights.
     //need sel leptons to apply idiso weights
     if (!is_data && !do_skim) analysis.AddModule(&hinvWeights);

     if (channel == channel::nunu||channel==channel::nunulowmet){
       analysis.AddModule(&controlPlots_lepveto);
     }
     else if(channel == channel::nunuiglep||channel==channel::nunulowmetiglep){
       analysis.AddModule(&controlPlots_iglep);
     }
     else if(channel == channel::mumu){
       analysis.AddModule(&controlPlots_zsel);
     }
     else {
       analysis.AddModule(&controlPlots_wsel);
       analysis.AddModule(&wjetsPlots_wsel);
     }

     if(!do_light_tree)analysis.AddModule(&jetPairFilter);
     if(do_trigeff) analysis.AddModule(&jetPairMaxPtFilter);//Allow max pt cut to enable binning in jet pt for trig studies
       
  
     //if (printEventList) analysis.AddModule(&hinvPrintList);

     if (doTopCR) {
       analysis.AddModule(&fourJetsFilter);
       analysis.AddModule(&bjetsCopyCollection);
       analysis.AddModule(&oneBtaggedJetFilter);
     }

     //record the number of jets in the gap
     analysis.AddModule(&FilterCJV);

     //jet pair selection
     if(!do_light_tree)analysis.AddModule(&etaProdJetPairFilter);
     //if (printEventList) analysis.AddModule(&hinvPrintList);

     analysis.AddModule(&controlPlots_dijet);
     analysis.AddModule(&wjetsPlots_dijet);

     //loose Mjj WP + met cut for control plots only 
     //analysis.AddModule(&looseMassJetPairFilter);
     analysis.AddModule(&controlPlots_AN);
     analysis.AddModule(&wjetsPlots_AN);

     //write tree with TMVA input variables
     if (doTmvaTree) analysis.AddModule(&tmvaInputs);
     else if(do_trigeff_tree) analysis.AddModule(&trigeffInputs);
     //else if(do_light_tree) analysis.AddModule(&lightTree);
     else {

       analysis.AddModule(&detaJetPairFilter);
       if(do_trigeff) analysis.AddModule(&detaJetPairMaxFilter);//Put max cut in to allow binning for trigger study
       analysis.AddModule(&controlPlots_deta);
       analysis.AddModule(&wjetsPlots_deta);
       //if (printEventList) analysis.AddModule(&hinvPrintList);
       
       //met modules
       
       if (channel == channel::munu || channel == channel::mumu || channel ==channel::nunuiglep||channel==channel::nunulowmetiglep){
	 analysis.AddModule(&metNoMuonFilter);
       }
       else {
	 analysis.AddModule(&metCut);
       }
       
       analysis.AddModule(&controlPlots_met);
       analysis.AddModule(&wjetsPlots_met);
       
       //if (printEventList) analysis.AddModule(&hinvPrintList);
       
       //tight Mjj cut
       analysis.AddModule(&tightMassJetPairFilter);
       analysis.AddModule(&controlPlots_tightMjj);
       analysis.AddModule(&wjetsPlots_tightMjj);
       if(!do_trigeff&&!do_trigeff_tree&&!do_light_tree){
	 //save signal and QCD regions without CJV
	 analysis.AddModule(&controlPlots_dphi_qcd_nocjv);
	 analysis.AddModule(&wjetsPlots_dphi_qcd_nocjv);
	 analysis.AddModule(&controlPlots_dphi_signal_nocjv);
	 analysis.AddModule(&wjetsPlots_dphi_signal_nocjv);
	 
	 if(printEventList) analysis.AddModule(&hinvPrintList);
	 //if(printEventContent) analysis.AddModule(&hinvPrint);
	 
	 //save all, signal and QCD regions with failed CJV
	 analysis.AddModule(&controlPlots_cjvfail);
	 analysis.AddModule(&wjetsPlots_cjvfail);
	 analysis.AddModule(&controlPlots_dphi_qcd_cjvfail);
	 analysis.AddModule(&wjetsPlots_dphi_qcd_cjvfail);
	 analysis.AddModule(&controlPlots_dphi_signal_cjvfail);
	 analysis.AddModule(&wjetsPlots_dphi_signal_cjvfail);
	 
	 analysis.AddModule(&controlPlots_cjvpass);
	 analysis.AddModule(&wjetsPlots_cjvpass);
	 
	 //if (printEventList) analysis.AddModule(&hinvPrintList);
	 
	 //dphi cut: don't filter events anymore !
	 //Just plot histograms for different regions
	 ////if (signal_region==0) analysis.AddModule(&dphiJetPairFilter);
	 ////else if (signal_region==1) analysis.AddModule(&dphiQCDJetPairFilter);
	 
	 analysis.AddModule(&controlPlots_dphi_qcd_cjvpass);
	 analysis.AddModule(&wjetsPlots_dphi_qcd_cjvpass);
	 analysis.AddModule(&controlPlots_dphi_signal_cjvpass);
	 analysis.AddModule(&wjetsPlots_dphi_signal_cjvpass);
       }
     
       if(do_trigeff){//Do trigger efficiency studies
	 analysis.AddModule(&dphilowFilter);
	 analysis.AddModule(&dphimaxFilter);
	 analysis.AddModule(&controlPlots_pretrig_cjvpass);
	 analysis.AddModule(&controlPlots_pretrig_cjvfail);
	 analysis.AddModule(&controlPlots_pretrig_nocjv);
	 analysis.AddModule(&dataMCTriggerPathFilter);
	 analysis.AddModule(&controlPlots_posttrig_cjvpass);
	 analysis.AddModule(&controlPlots_posttrig_cjvfail);
	 analysis.AddModule(&controlPlots_posttrig_nocjv);
       }
     
       //debug
       //analysis.AddModule(&dphiJetPairFilter);
       //if (!is_data) analysis.AddModule(&hinvWeights);
       //}
       //else {
       //Build Skimming Analysis
       //analysis.AddModule(pointer to module defined above)
       //analysis.AddModule(&dataMCTriggerPathFilter);
       
       //SINGLE EVENT SKIMMING
       // if (channel == channel::nunu){
       //        analysis.AddModule(&hinvFilter);
       //        analysis.AddModule(&jetIDFilter);
       
       //        //prepare collections of veto leptons
       //        analysis.AddModule(&vetoElectronCopyCollection);
       //        analysis.AddModule(&vetoElectronFilter);
       //        analysis.AddModule(&vetoElectronIso);
       //        analysis.AddModule(&vetoMuonCopyCollection);
       //        analysis.AddModule(&vetoMuonFilter);
       //        analysis.AddModule(&vetoMuonNoIsoCopyCollection);
       //        analysis.AddModule(&vetoMuonNoIsoFilter);
       
       //        //filter leptons before making jet pairs and changing MET...
       //        analysis.AddModule(&selElectronCopyCollection);
       //        analysis.AddModule(&selElectronFilter);
       //        analysis.AddModule(&selElectronIso);
       //        analysis.AddModule(&selMuonCopyCollection);
       //        analysis.AddModule(&selMuonFilter);
       //        analysis.AddModule(&elecMuonOverlapFilter);
       
       //        //filter jets
       //        analysis.AddModule(&jetPtEtaFilter);
       
       //        //deal with removing overlap with selected leptons
       //        analysis.AddModule(&jetMuonOverlapFilter);
       //        analysis.AddModule(&jetElecOverlapFilter);
       
       //        //two-leading jet pair production before plotting
       //        analysis.AddModule(&jjLeadingPairProducer);
       //        analysis.AddModule(&hinvPrint);
       //        //analysis.AddModule(&jetPtEtaFilter);
       //        //analysis.AddModule(&metCut);
       //        //analysis.AddModule(&jjPairProducer);
       //        //analysis.AddModule(&looseMassJetPairFilter);
       //      }
       //      else if (!is_data){
       //        analysis.AddModule(&WtoLeptonFilter);
       //      }
       //}
       
     }//if not doTmvaTree
  }//if not do MCFM
  // Run analysis
   
  analysis.RetryFileAfterFailure(5,5);// int <pause between attempts in seconds>, int <number of retry attempts to make> );
  analysis.StopOnFileFailure(true);
  analysis.SetTTreeCaching(true); 
  
  analysis.RunAnalysis();
  delete fs;
  return 0;
}




