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
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/JetEnergyCorrections.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/LumiMask.h"

#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/SingleMetMaker.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvDataTriggerFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvWeights.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvWDecay.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvZDecay.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/ModifyMet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/JetMETModifier.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/MetLaserFilters.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvPrint.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/CJVFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/LightTreeJetMETval.h"

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
  bool doincludehighptz;          // For including high pt z sample

  string era_str;                 // Analysis data-taking era
  string mc_str;                  // Analysis MC production
  string prod;                    // Our production string

  string wstream;                 // W stream: enu, munu or taunu, or nunu for everything

  bool is_data;                   // true = data, false = mc         
  bool dojessyst;                 // Do Jet Energy Scale Systematic Run
  bool dodatajessyst;             // Do Alternate Data Jet Energy Scale Method Systematic Run
  bool jesupordown;               // If doing Jet Energy Scale Systematic Run, run with up or down correction (true for up, false for down)
  bool dosmear;                   // Do Smearing
  bool doaltmatch;                // Do runmetuncertainties gen jet matching
  bool doetsmear;                 // Do runmetuncertainties smearing
  bool dogaus;                    // Do gaussian smearing for jets with no gen jet match
  bool dospring10gaus;            // Do gaussian smearing for jets with no gen jet match
  bool dojersyst;                 // Do Jet Energy Resolution Systematic Run
  bool jerbetterorworse;          // If doing Jet Energy Resolution Systematic Run, run with with better or worse (true for better, false for worse)
  bool douessyst;                 // Do Unclustered MET Systematic Run
  bool uesupordown;               // If doing Unclustered MET Systematic Run, run with up or down correction (true for up, false for down) 
  bool docrosschecktau;           // If doing cross check tau use alternate tau id discriminant
  bool taulepdiscrtight;          // Use tight electron and muon discriminants
  bool dojerdebug;                // Access runmetunc collections for debugging
  bool dotopreweighting;          // Do Top reweighting
  bool dopromptskim;              // Use prompt compatible light tree skimming
  bool donoskim;                  // Do no skimming

  string mettype;                 // MET input collection to be used
  string jettype;                 // JET input collection to be used
  string jesuncfile;              // File to get JES uncertainties from
  bool doMetFilters;              // apply cleaning MET filters.
  string filters;
  //unsigned signal_region;       // DeltaPhi cut > 2.7
  bool dotrgeff;                  // Do trigger efficiency corrections
  bool do3dtrgeff;                // Do 3d trigger efficiency corrections
  bool do1dparkedtrgeff;          // Do 1d parked trigger efficiency corrections
  bool dofitted1dparkedtrgeff;    // Do 1d fitted parked trigger efficiency corrections
  bool dobinnedin2d1dtrgeff;      // Do 2d binned fitted 1d parked trigger efficiency corrections
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
  double jet2ptcut;               //jet2ptcut
  double jetptprecut;             //jetptprecut, should be the CJV threshold

  int randomseed;

  bool printEventList;  //print run,lumi,evt of events selected
  bool printEventContent; //print event content of events selected

  bool turnoffpuid;

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
    ("doincludehighptz",    po::value<bool>(&doincludehighptz)->default_value(false))
    ("era",                 po::value<string>(&era_str)->required())
    ("mc",                  po::value<string>(&mc_str)->required())
    ("prod",                po::value<string>(&prod)->required())
    ("wstream",             po::value<string>(&wstream)->default_value("nunu"))
    ("is_data",             po::value<bool>(&is_data)->required())
    ("mettype",             po::value<string>(&mettype)->default_value("pfMetType1Collection"))
    ("jettype",             po::value<string>(&jettype)->default_value("pfJetsPFlow"))
    ("jet1ptcut",           po::value<double>(&jet1ptcut)->default_value(30.))
    ("jet2ptcut",           po::value<double>(&jet2ptcut)->default_value(30.))
    ("jetptprecut",         po::value<double>(&jetptprecut)->default_value(15.))
    ("doMetFilters",        po::value<bool>(&doMetFilters)->default_value(false))
    ("filters",             po::value<string> (&filters)->default_value("HBHENoiseFilter,EcalDeadCellTriggerPrimitiveFilter,eeBadScFilter,trackingFailureFilter,manystripclus53X,toomanystripclus53X,logErrorTooManyClusters,CSCTightHaloFilter"))
    ("dojessyst",           po::value<bool>(&dojessyst)->default_value(false))
    ("dodatajessyst",       po::value<bool>(&dodatajessyst)->default_value(false))
    ("jesupordown",         po::value<bool>(&jesupordown)->default_value(true))
    ("dojersyst",           po::value<bool>(&dojersyst)->default_value(false))
    ("jerbetterorworse",    po::value<bool>(&jerbetterorworse)->default_value(true))
    ("douessyst",           po::value<bool>(&douessyst)->default_value(false))
    ("uesupordown",         po::value<bool>(&uesupordown)->default_value(true))
    ("docrosschecktau",     po::value<bool>(&docrosschecktau)->default_value(false))
    ("taulepdiscrtight",    po::value<bool>(&taulepdiscrtight)->default_value(false))
    ("dojerdebug",          po::value<bool>(&dojerdebug)->default_value(false))
    ("dotrgeff",            po::value<bool>(&dotrgeff)->default_value(false))
    ("do3dtrgeff",          po::value<bool>(&do3dtrgeff)->default_value(false))
    ("do1dparkedtrgeff",    po::value<bool>(&do1dparkedtrgeff)->default_value(false))
    ("dofitted1dparkedtrgeff",po::value<bool>(&dofitted1dparkedtrgeff)->default_value(false))
    ("dobinnedin2d1dtrgeff",po::value<bool>(&dobinnedin2d1dtrgeff)->default_value(false))
    ("doidisoeff",          po::value<bool>(&doidisoeff)->default_value(false))
    ("dotopreweighting",    po::value<bool>(&dotopreweighting)->default_value(false))
    ("dopromptskim",        po::value<bool>(&dopromptskim)->default_value(false))
    ("donoskim",            po::value<bool>(&donoskim)->default_value(false))
    ("doidisoerr",          po::value<bool>(&doidisoerr)->default_value(false))
    ("doidisoerrupordown",  po::value<bool>(&doidisoerrupordown)->default_value(true))
    ("doidisoerrmuore",     po::value<bool>(&doidisoerrmuore)->default_value(true))
    ("dolumixsweight",      po::value<bool>(&dolumixsweight)->default_value(false))
    ("inputparams",         po::value<string>(&inputparams)->default_value("filelists/Dec18/ParamsDec18.dat"))
    ("trg_weight_file",     po::value<string>(&trg_weight_file)->default_value("input/scale_factors/DataMCWeight_53X_v1.root"))
    ("trg_to_use",          po::value<string>(&trg_to_use)->default_value("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v"))
    ("printEventList",      po::value<bool>(&printEventList)->default_value(false))
    ("printEventContent",   po::value<bool>(&printEventContent)->default_value(false))
    ("dosmear",             po::value<bool>(&dosmear)->default_value(false))
    ("doaltmatch",          po::value<bool>(&doaltmatch)->default_value(false))
    ("doetsmear",           po::value<bool>(&doetsmear)->default_value(false))
    ("dogaus",              po::value<bool>(&dogaus)->default_value(false))
    ("dospring10gaus",      po::value<bool>(&dospring10gaus)->default_value(false))
    ("jesuncfile",          po::value<string>(&jesuncfile)->default_value("input/jec/Fall12_V7_MC_Uncertainty_AK5PF.txt"))
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

  std::cout << "**** HiggsNuNu Analysis *****" << std::endl;
  string param_fmt = "%-25s %-40s\n";
  std::cout << boost::format(param_fmt) % "max_events" % max_events;
  std::cout << boost::format(param_fmt) % "output" % (output_folder+output_name);
  std::cout << boost::format(param_fmt) % "dojessyst" % dojessyst;
  if (dojessyst) std::cout << boost::format(param_fmt) % "jesupordown" % jesupordown;
  std::cout << boost::format(param_fmt) % "dojersyst" % dojersyst;
  if (dojessyst) std::cout << boost::format(param_fmt) % "jerbettorworse" % jerbetterorworse;
  std::cout << boost::format(param_fmt) % "era" % era_str;
  std::cout << boost::format(param_fmt) % "mc" % mc_str;
  std::cout << boost::format(param_fmt) % "prod" % prod;
  std::cout << boost::format(param_fmt) % "wstream" % wstream;
  std::cout << boost::format(param_fmt) % "is_data" % is_data;
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
  std::ofstream checkfile;
  checkfile.open((output_folder+"tmp.txt").c_str());
  if(!(checkfile.good())){
    std::cout<<"OUTPUT FOLDER DOESN'T EXIST: EXITING"<<std::endl;
    return 1;
  }
  checkfile.close();
  fwlite::TFileService *fs = new fwlite::TFileService((output_folder+output_name).c_str());
  
  bool ignoreLeptons=false;
  if (output_name.find("iglep") != output_name.npos) {
    ignoreLeptons = true;
  }
   
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

  // ------------------------------------------------------------------------------------
  // Misc Modules
  // ------------------------------------------------------------------------------------
  
  
  string mc_pu_file;
  if (mc == mc::fall11_42X) mc_pu_file    = "input/pileup/MC_Fall11_PU_S6-500bins.root";
  if (mc == mc::summer12_53X) mc_pu_file  = "input/pileup/MC_Summer12_PU_S10-600bins.root";
  if (mc == mc::summer12_52X) mc_pu_file  = "input/pileup/MC_Summer12_PU_S7-600bins.root";
  if (mc == mc::phys14_72X) mc_pu_file  = "input/pileup/MC_Summer12_PU_S10-600bins.root";//!!FIX WITH NEW PU
  if (mc == mc::spring15_74X) mc_pu_file  = "input/pileup/MC_Summer12_PU_S10-600bins.root";//!!FIX WITH NEW PU

  string data_pu_file;
  if (era == era::data_2012_rereco) data_pu_file   =  "input/pileup/Data_Pileup_2012_ReRecoPixel-600bins.root";
  if (era == era::data_2011) data_pu_file     =  "input/pileup/Data_Pileup_2011_HCP-500bins.root";
  if (era == era::data_2012_ichep) data_pu_file     =  "input/pileup/Data_Pileup_2012.root";
  if (era == era::data_2012_hcp) data_pu_file       =  "input/pileup/Data_Pileup_2012_HCP-600bins.root";
  if (era == era::data_2012_moriond) data_pu_file   =  "input/pileup/Data_Pileup_2012_Moriond-600bins.root";
  if (era == era::data_2012_donly) data_pu_file     =  "input/pileup/Data_Pileup_2012_DOnly-600bins.root";
  if (era == era::data_2015_50ns) data_pu_file   =  "input/pileup/Data_Pileup_2012_ReRecoPixel-600bins.root";//!!FIX WITH NEW PU

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

  MetLaserFilters metLaserFilters = MetLaserFilters("MetLaserFilters",
						    "input/met_laser_filters/AllBadHCALLaser.txt",
						    "input/met_laser_filters/ecalLaserFilter_MET_Run2012AandB.txt",
						    doMetFilters);


  SimpleFilter<Vertex> goodVertexFilter = SimpleFilter<Vertex>("goodVertexFilter")
    .set_input_label("vertices")
    .set_predicate(bind(DummyFunction<Vertex>, _1))
    .set_min(1)
    .set_max(999);

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

  // ------------------------------------------------------------------------------------
  // Muon Modules
  // ------------------------------------------------------------------------------------
  
  // Muon Veto
  CopyCollection<Muon> vetoMuonCopyCollection("CopyToVetoMuons","muons","vetoMuons");

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

  CopyCollection<Muon> vetoMuonNoIsoCopyCollection("CopyToVetoMuonsNoIso","muons","vetoMuonsNoIso");
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
  CopyCollection<Muon> selMuonCopyCollection("CopyToSelMuons","muons","selMuons");
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

  HinvZDecay ZlowmassFilter = HinvZDecay("ZlowmassFilter",13,0,100,true);

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
    tau_anti_muon_discr   = "againstMuonTight3";
    tau_anti_elec_discr_1 = "againstElectronTightMVA5";
    tau_anti_elec_discr_2 = "againstElectronTightMVA5";
  }
  else{
    tau_anti_muon_discr   = "againstMuonLoose3";
    tau_anti_elec_discr_1 = "againstElectronLooseMVA5";
    tau_anti_elec_discr_2 = "againstElectronLooseMVA5";
  }
  
  //tau_iso_discr         = "byLooseCombinedIsolationDeltaBetaCorr3Hits";
  
  std::cout << "** Tau Discriminators **" << std::endl;
  std::cout << boost::format(param_fmt) % "isolation" %  tau_iso_discr;
  std::cout << boost::format(param_fmt) % "anti-electron1" % tau_anti_elec_discr_1;
  std::cout << boost::format(param_fmt) % "anti-electron2" % tau_anti_elec_discr_2;
  std::cout << boost::format(param_fmt) % "anti-muon" % tau_anti_muon_discr;


  SimpleFilter<Tau> tauIsoFilter = SimpleFilter<Tau>("TauIsoFilter")
    .set_input_label("taus")
    .set_predicate((bind(&Tau::GetTauID, _1, tau_iso_discr) > 0.5) && (bind(&Tau::GetTauID, _1, "decayModeFindingNewDMs") > 0.5))
    .set_min(0);

  SimpleFilter<Tau> tauElRejectFilter = SimpleFilter<Tau>("TauElRejectFilter")
    .set_predicate( (bind(&Tau::GetTauID, _1, tau_anti_elec_discr_1) > 0.5) && (bind(&Tau::GetTauID, _1, tau_anti_elec_discr_2) > 0.5) )                     
    .set_input_label("taus")
    .set_min(0); 

  SimpleFilter<Tau> tauMuRejectFilter = SimpleFilter<Tau>("TauMuRejectFilter")
    .set_predicate(bind(&Tau::GetTauID, _1, tau_anti_muon_discr) > 0.5)
    .set_input_label("taus")
    .set_min(0);


  // ------------------------------------------------------------------------------------
  // Jet Modules
  // ------------------------------------------------------------------------------------  

  JetMETModifier ModifyJetMET = JetMETModifier
    ("ModifyJetMET")
    .set_input_label(jettype)
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
    .set_douessyst(douessyst)
    .set_uesupordown(uesupordown)
    .set_randomseed(randomseed)
    .set_fs(fs);
  

  CopyCollection<PFJet> alljetsCopyCollection("copytoalljets",jettype,"AllpfJetsPFlow");
  
  SimpleFilter<PFJet> jetIDFilter = SimpleFilter<PFJet>
    ("JetIDFilter")
    .set_input_label(jettype);
    if(!turnoffpuid){
      jetIDFilter.set_predicate((bind(PFJetID2015, _1)) && bind(PileupJetID, _1,2));
    }
    else{
      jetIDFilter.set_predicate(bind(PFJetID2015, _1));
    }
    
  // Jet pT eta filter
  SimpleFilter<PFJet> jetPtEtaFilter = SimpleFilter<PFJet>
    ("JetPtEtaFilter")
    .set_input_label(jettype).set_predicate(bind(MinPtMaxEta, _1, jetptprecut, 4.7));


  //in principle no need to remove overlap because events with leptons are rejected...
  //except for specific e/mu selection for W background estimation.
  // and to calculate efficiencies of cuts correctly
  //we want the leading jets to really be jets even before vetoing leptons...
  //so: removal of jets matched with veto electrons and muons
  OverlapFilter<PFJet, Electron> jetElecOverlapFilter = OverlapFilter<PFJet, Electron>("jetElecOverlapFilter")
    .set_input_label(jettype)
    .set_reference_label("vetoElectrons")
    .set_min_dr(0.5);

  OverlapFilter<PFJet, Muon> jetMuonOverlapFilter = OverlapFilter<PFJet, Muon>("jetMuonOverlapFilter")
    .set_input_label(jettype)
    .set_reference_label("vetoMuons") //NoIso")
    .set_min_dr(0.5);

  OverlapFilter<PFJet, Tau> jetTauOverlapFilter = OverlapFilter<PFJet, Tau>("jetTauOverlapFilter")
    .set_input_label(jettype)
    .set_reference_label("taus") //NoIso")
    .set_min_dr(0.5);

  OneCollCompositeProducer<PFJet> jjLeadingPairProducer = OneCollCompositeProducer<PFJet>
    ("JetJetLeadingPairProducer")
    .set_input_label(jettype)
    .set_candidate_name_first("jet1")
    .set_candidate_name_second("jet2")
    .set_select_leading_pair(true)
    .set_output_label("jjLeadingCandidates");                                                        

  int npairs=1;
  if(donoskim)npairs=0;
  bool cutaboveorbelow=true;
  SimpleFilter<CompositeCandidate> jetPairFilter = SimpleFilter<CompositeCandidate>("JetPairFilter")
    .set_input_label("jjLeadingCandidates")
    .set_predicate( bind(OrderedPairPtSelection, _1,jet1ptcut, jet2ptcut, cutaboveorbelow) )
    .set_min(npairs)
    .set_max(999);

  // ------------------------------------------------------------------------------------
  // Met Modules
  // ------------------------------------------------------------------------------------  
  //SingleMetMaker singleMet = SingleMetMaker(mettype,mettype);
  MetSelection metFilters = MetSelection("MetFilters",mettype,doMetFilters,filtersVec,0);
  

  unsigned nLepToAdd = 100;
  //no need to be explicit in the number of leptons: will take the number 
  // that is asked in the selection (i.e. exactly one for the W->e,mu selections...)
  
  ModifyMet metNoMuons = ModifyMet("metNoMuons",mettype,"selMuons",2,nLepToAdd);
  ModifyMet metNoElectrons = ModifyMet("metNoElectrons",mettype,"selElectrons",1,nLepToAdd);
  ModifyMet metNoENoMu = ModifyMet("metNoENoMu","metNoMuons","selElectrons",1,nLepToAdd);


  // ------------------------------------------------------------------------------------
  // Weight Modules
  // ------------------------------------------------------------------------------------  
  
  HinvWeights hinvWeights = HinvWeights("HinvWeights")
    .set_era(era)
    .set_mc(mc)
    .set_save_weights(true)
    .set_do_top_reweighting(dotopreweighting)
    .set_do_trg_weights(false)
    .set_trg_applied_in_mc(false)
    .set_do_idiso_tight_weights(false)
    .set_do_idiso_veto_weights(false)
    .set_do_idiso_err(doidisoerr)
    .set_do_idiso_errupordown(doidisoerrupordown)
    .set_do_idiso_errmuore(doidisoerrmuore)
    .set_fs(fs)
    .set_input_met("metNoMuons");
  if (!is_data) {
    hinvWeights.set_do_trg_weights(dotrgeff)
      .set_do_3dtrg_weights(do3dtrgeff)
      .set_do_1dparkedtrg_weights(do1dparkedtrgeff)
      .set_do_fitted1dparkedtrg_weights(dofitted1dparkedtrgeff)
      .set_do_binnedin2d1dfittedtrg_weights(dobinnedin2d1dtrgeff)
      .set_trg_weight_file(trg_weight_file)
      .set_trg_applied_in_mc(false);
    if(do3dtrgeff){
      hinvWeights.set_Alumi(0.889)
	.set_BClumi(11.023)
	.set_Dlumi(7.315);
    }
    hinvWeights.set_do_idiso_veto_weights(false);
    hinvWeights.set_do_idiso_tight_weights(false);
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

    if (mc == mc::summer12_53X) {
      xsWeights.set_do_w_soup(true);
      xsWeights.set_do_w_reweighting(false);
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
  if (wstream == "enu") lFlavour = 11;
  else if (wstream == "munu") lFlavour = 13;
  else if (wstream == "taunu") lFlavour = 15;



  HinvWDecay WtoLeptonFilter = HinvWDecay("WtoLeptonSelector",lFlavour);
  WtoLeptonFilter.set_do_newstatuscodes(true);
  WtoLeptonFilter.set_do_statusflags(true);

  // ------------------------------------------------------------------------------------
  // Plot Modules
  // ------------------------------------------------------------------------------------  
  
  LightTreeJetMETval lightTree = LightTreeJetMETval("LightTreeJetMETval")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_jet_label(jettype)
    .set_is_data(is_data)
    .set_dotrigskim(false)
    .set_do_noskim(donoskim)
    .set_ignoreLeptons(ignoreLeptons)
    .set_trigger_path("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v")
    .set_trig_obj_label("triggerObjectsDiPFJet40PFMETnoMu65MJJ800VBFAllJets");

  // ------------------------------------------------------------------------------------
  // Build Analysis Sequence
  // ------------------------------------------------------------------------------------  

  //analysis.AddModule(&singleMet);
  if (!is_data) {
    //do W streaming to e,mu,tau
    if (output_name.find("JetsToLNu") != output_name.npos ||
	output_name.find("EWK-W2j") != output_name.npos) {
      if (wstream != "nunu") analysis.AddModule(&WtoLeptonFilter);
    }
    //Do PU Weight
    analysis.AddModule(&pileupWeight);
    //just apply W and Z weights
    analysis.AddModule(&xsWeights);
    //Z pt <100 GeV cut for inclusive DY samples
    //if(doincludehighptz && output_name.find("JetsToLL") != output_name.npos && output_name.find("PtZ-100-madgraph") == output_name.npos && output_name.find("DYJJ01") == output_name.npos){
    // analysis.AddModule(&ZlowmassFilter);
    //}    
  }
   
  //if (printEventList) analysis.AddModule(&hinvPrintList);
  if (is_data) {
    //FIXME: do MetFilters also on MC, but not saved right now in MC...
    analysis.AddModule(&metFilters);
    analysis.AddModule(&metLaserFilters);
  }

  //if(!donoskim)
  analysis.AddModule(&goodVertexFilter);
  //jet modules
  //analysis.AddModule(&jetIDFilter);
  //don't want pile-up jets to calculate HT,MHT...
  //analysis.AddModule(&alljetsCopyCollection);
  
  //prepare collections of veto leptons
  analysis.AddModule(&vetoElectronCopyCollection);
  analysis.AddModule(&vetoElectronFilter);
  analysis.AddModule(&vetoElectronIso);
  analysis.AddModule(&vetoMuonCopyCollection);
  analysis.AddModule(&vetoMuonFilter);
  
  //filter leptons before making jet pairs and changing MET...
  analysis.AddModule(&selElectronCopyCollection);
  analysis.AddModule(&selElectronFilter);
  analysis.AddModule(&selElectronIso);
  analysis.AddModule(&selMuonCopyCollection);
  analysis.AddModule(&selMuonFilter);
  analysis.AddModule(&elecMuonOverlapFilter);
  
  //filter taus for plots
  analysis.AddModule(&tauPtEtaFilter);
  
  //if (printEventList) analysis.AddModule(&hinvPrintList);
  
  //deal with removing overlap with selected leptons
  //analysis.AddModule(&jetMuonOverlapFilter);
  //analysis.AddModule(&jetElecOverlapFilter);
  //no need to clean taus, we don't do it in the signal selection.
  
  //Module to do jet smearing and systematics
  analysis.AddModule(&ModifyJetMET);
  
  //add met without leptons for plots
  analysis.AddModule(&metNoMuons);
  analysis.AddModule(&metNoElectrons);
  analysis.AddModule(&metNoENoMu);
  
  //filter taus
  analysis.AddModule(&tauDzFilter);
  analysis.AddModule(&tauIsoFilter);
  analysis.AddModule(&tauElRejectFilter);
  analysis.AddModule(&tauMuRejectFilter);
  
  //filter jets
  analysis.AddModule(&jetPtEtaFilter);
  
  //Need two jets and metnomuons to apply trigger weights.
  //need sel leptons to apply idiso weights
  if (!is_data) analysis.AddModule(&hinvWeights);
  
  //write tree with TMVA input variables
  analysis.AddModule(&lightTree);  
  
  // Run analysis
  analysis.RetryFileAfterFailure(5,5);// int <pause between attempts in seconds>, int <number of retry attempts to make> );
  analysis.StopOnFileFailure(true);
  analysis.SetTTreeCaching(true); 
  
  analysis.RunAnalysis();
  delete fs;
  return 0;
}




