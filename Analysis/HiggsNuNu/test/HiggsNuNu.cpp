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
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MetSelection.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MTSelection.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MakeRunStats.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/JetEnergyCorrections.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/LumiMask.h"

#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvDataTriggerFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvWeights.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvControlPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvWJetsPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/ModifyMet.h"

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
  string channel_str;             // Analysis channel

  bool is_data;                   // true = data, false = mc         
  bool is_embedded;               // true = embedded, false = not an embedded sample
  unsigned mva_met_mode;          // 0 = standard mva met, 1 = mva met from vector (only when mva met is being used)
  bool make_sync_ntuple;          // Generate a sync ntuple
  
  string mettype;                 // MET input collection to be used
  unsigned signal_region;             // DeltaPhi cut > 2.7
  double met_cut;                 // MET cut to apply for signal, QCD or skim

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
    ("channel",             po::value<string>(&channel_str)->default_value("nunu"))
    ("is_data",             po::value<bool>(&is_data)->required())
    ("is_embedded",         po::value<bool>(&is_embedded)->default_value(false))
    ("mva_met_mode",        po::value<unsigned>(&mva_met_mode)->default_value(1))
    ("make_sync_ntuple",    po::value<bool>(&make_sync_ntuple)->default_value(false))
    ("mettype",             po::value<string>(&mettype)->default_value("pfMetType1"))
    ("signal_region",       po::value<unsigned>(&signal_region)->default_value(1))
    ("met_cut",             po::value<double>(&met_cut)->default_value(130.));
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
  po::notify(vm);


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
  std::cout << boost::format(param_fmt) % "era" % era_str;
  std::cout << boost::format(param_fmt) % "mc" % mc_str;
  std::cout << boost::format(param_fmt) % "channel" % channel_str;
  std::cout << boost::format(param_fmt) % "is_data" % is_data;
  std::cout << boost::format(param_fmt) % "is_embedded" % is_embedded;
  std::cout << boost::format(param_fmt) % "mva_met_mode" % mva_met_mode;
  std::cout << boost::format(param_fmt) % "make_sync_ntuple" % make_sync_ntuple;
  std::cout << boost::format(param_fmt) % "signal_region" % signal_region;
  std::cout << boost::format(param_fmt) % "met_cut" % met_cut;


  // Load necessary libraries for ROOT I/O of custom classes
  gSystem->Load("libFWCoreFWLite.dylib");
  gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  AutoLibraryLoader::enable();


  // Build a vector of input files
  vector<string> files = ParseFileLines(filelist);
  for (unsigned i = 0; i < files.size(); ++i) files[i] = input_prefix + files[i];
  
  // Create ROOT output fileservice
  fwlite::TFileService *fs = new fwlite::TFileService((output_folder+output_name).c_str());
  
  double elec_dz, muon_dz, elec_dxy, muon_dxy;
  double elec_pt, elec_eta, muon_pt, muon_eta;
  double veto_elec_pt, veto_elec_eta, veto_muon_pt, veto_muon_eta;
  
  elec_dz = 0.2;
  elec_dxy = 0.045;
  muon_dz = 0.2;
  muon_dxy = 0.045;
  
  elec_pt = 20.0;
  elec_eta = 2.4;
  muon_pt = 20.0;
  muon_eta = 2.4;
  
  veto_elec_pt = 10.0;
  veto_elec_eta = 2.4;
  veto_muon_pt = 10.0;
  veto_muon_eta = 2.4;
   
  std::cout << "----------PARAMETERS----------" << std::endl;
  std::cout << boost::format("%-15s %-10s\n") % "elec_pt:" % elec_pt;
  std::cout << boost::format("%-15s %-10s\n") % "elec_eta:" % elec_eta;
  std::cout << boost::format("%-15s %-10s\n") % "veto_elec_pt:" % veto_elec_pt;
  std::cout << boost::format("%-15s %-10s\n") % "veto_elec_eta:" % veto_elec_eta;
  std::cout << boost::format("%-15s %-10s\n") % "elec_dxy:" % elec_dxy;
  std::cout << boost::format("%-15s %-10s\n") % "elec_dz:" % elec_dz;
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
    "icEventProducer",    // TTree path
    "EventTree",          // TTree name
    max_events);          // Max. events to process (-1 = all)
  if (do_skim && skim_path == "") {
    std::cout << "-- Skimming mode: requires skim_path parameter to be set !! Quitting." << std::endl;
    return 1;
  }
  if (do_skim && skim_path != "") analysis.DoSkimming(skim_path);

  // ------------------------------------------------------------------------------------
  // Misc Modules
  // ------------------------------------------------------------------------------------
  
  string data_json;
  if (era == era::data_2011) data_json           =  "data/json/json_data_2011_et_mt.txt";
  if (era == era::data_2012_ichep) data_json     =  "data/json/data_2012_ichep.txt";
  if (era == era::data_2012_hcp) data_json       =  "data/json/data_2012_hcp.txt";
  if (era == era::data_2012_moriond) data_json   =  "data/json/data_2012_moriond.txt";
  if (era == era::data_2012_donly) data_json     =  "data/json/data_2012_donly.txt";
  LumiMask lumiMask = LumiMask("LumiMask")
    .set_produce_output_jsons("")
    .set_input_file(data_json);

  MakeRunStats runStats = MakeRunStats("RunStats")
    .set_output_name(output_folder+output_name+".runstats");
  
  
  string mc_pu_file;
  if (mc == mc::fall11_42X) mc_pu_file    = "data/pileup/MC_Fall11_PU_S6-500bins.root";
  if (mc == mc::summer12_53X) mc_pu_file  = "data/pileup/MC_Summer12_PU_S10-600bins.root";
  if (mc == mc::summer12_52X) mc_pu_file  = "data/pileup/MC_Summer12_PU_S7-600bins.root";
  string data_pu_file;
  if (era == era::data_2011) data_pu_file     =  "data/pileup/Data_Pileup_2011_HCP-500bins.root";
  if (era == era::data_2012_ichep) data_pu_file     =  "data/pileup/Data_Pileup_2012.root";
  if (era == era::data_2012_hcp) data_pu_file       =  "data/pileup/Data_Pileup_2012_HCP-600bins.root";
  if (era == era::data_2012_moriond) data_pu_file   =  "data/pileup/Data_Pileup_2012_Moriond-600bins.root";
  if (era == era::data_2012_donly) data_pu_file     =  "data/pileup/Data_Pileup_2012_DOnly-600bins.root";

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

  HinvDataTriggerFilter dataMCTriggerPathFilter("TriggerPathFilter");
  dataMCTriggerPathFilter.set_is_data(is_data);
  dataMCTriggerPathFilter.set_trigger_path("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v");
  dataMCTriggerPathFilter.set_trig_obj_label("triggerObjectsDiPFJet40PFMETnoMu65MJJ800VBFAllJets");

  JetEnergyCorrections<PFJet> jetEnergyCorrections = JetEnergyCorrections<PFJet>
  ("JetEnergyCorrections")
  .set_input_label("pfJetsPFlow")
  .set_is_data(is_data)
  .set_l1_file("data/jec/START53_V10_L1FastJet_AK5PF.txt")
  .set_l2_file("data/jec/START53_V10_L2Relative_AK5PF.txt")
  .set_l3_file("data/jec/START53_V10_L3Absolute_AK5PF.txt");
  
  // ------------------------------------------------------------------------------------
  // Electron Modules
  // ------------------------------------------------------------------------------------

  // Electron Veto
  CopyCollection<Electron>  vetoElectronCopyCollection("CopyToVetoElectrons","electrons","vetoElectrons");

  SimpleFilter<Electron> vetoElectronFilter = SimpleFilter<Electron>
    ("VetoElectronPtEtaFilter")
    .set_input_label("vetoElectrons").set_predicate(bind(MinPtMaxEta, _1, veto_elec_pt, veto_elec_eta) &&
						    bind(Electron2011WP95ID, _1) && 
						    bind(PF04Isolation<Electron>, _1, 0.5, 0.3) //&&
						    //bind(fabs, bind(&Electron::dxy_vertex, _1)) < elec_dxy && 
						    //bind(fabs, bind(&Electron::dz_vertex, _1)) < elec_dz
						    )
    .set_min(0)
    .set_max(999);
  
  SimpleFilter<Electron> selElectronFilter = SimpleFilter<Electron>
    ("SelElectronPtEtaFilter")
    .set_input_label("electrons").set_predicate(bind(MinPtMaxEta, _1, elec_pt, elec_eta) &&
						bind(ElectronHTTId, _1,true) &&
						bind(PF04Isolation<Electron>, _1, 0.5, 0.1) &&
						bind(fabs, bind(&Electron::dxy_vertex, _1)) < elec_dxy && 
						bind(fabs, bind(&Electron::dz_vertex, _1)) < elec_dz
						)
    .set_min(0)
    .set_max(999);


  SimpleFilter<Electron> oneElectronFilter = SimpleFilter<Electron>
    ("OneElectronFilter")
    .set_input_label("electrons")
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
						bind(&Muon::is_global, _1) && 
						bind(PF04Isolation<Muon>, _1, 0.5, 0.3) //&&
						//bind(fabs, bind(&Muon::dxy_vertex, _1)) < muon_dxy && 
						//bind(fabs, bind(&Muon::dz_vertex, _1)) < muon_dz
						)
    .set_min(0)
    .set_max(999);

   
  SimpleFilter<Muon> selMuonFilter = SimpleFilter<Muon>
    ("SelMuonPtEtaFilter")
    .set_input_label("muonsPFlow").set_predicate(bind(MinPtMaxEta, _1, muon_pt, muon_eta) &&
						 bind(&Muon::is_global, _1) && 
						 bind(MuonTight, _1) && 
						 bind(PF04Isolation<Muon>, _1, 0.5, 0.1) &&
						 bind(fabs, bind(&Muon::dxy_vertex, _1)) < muon_dxy && 
						 bind(fabs, bind(&Muon::dz_vertex, _1)) < muon_dz
						 )
    .set_min(0)
    .set_max(999);


  SimpleFilter<Muon> oneMuonFilter = SimpleFilter<Muon>
    ("OneMuonFilter")
    .set_input_label("muonsPFlow")
    .set_predicate( bind(DummyFunction<Muon>, _1) )
    .set_min(1)
    .set_max(1);

  SimpleFilter<Muon> oneVetoMuonFilter = SimpleFilter<Muon>
    ("OneVetoMuonFilter")
    .set_input_label("vetoMuons")
    .set_predicate( bind(DummyFunction<Muon>, _1) )
    .set_min(1)
    .set_max(1);

  SimpleFilter<Muon> zeroVetoMuonFilter = SimpleFilter<Muon>
    ("ZeroVetoMuonFilter")
    .set_input_label("vetoMuons")
    .set_predicate( bind(DummyFunction<Muon>, _1) )
    .set_min(0)
    .set_max(0);

  // ------------------------------------------------------------------------------------
  // Jet Modules
  // ------------------------------------------------------------------------------------  
  //no need to remove overlap because events with leptons are rejected...
  //or maybe remove overlap with tau candidates ??
  //  OverlapFilter<PFJet, CompositeCandidate> jetLeptonOverlapFilter = OverlapFilter<PFJet, CompositeCandidate>
  //  ("JetLeptonOverlapFilter")
  //  .set_input_label("pfJetsPFlow")
  //  .set_reference_label("emtauCandidates")
  //  .set_min_dr(0.5);

  SimpleFilter<PFJet> jetIDFilter = SimpleFilter<PFJet>
    ("JetIDFilter")
    .set_input_label("pfJetsPFlow")
    .set_predicate((bind(PFJetID, _1)) && bind(&PFJet::pu_id_mva_loose, _1));

  // Jet pT eta filter
  //CopyCollection<PFJet> jetCopyCollection("CopyToJet","pfJetsPFlow","selJets");

  double jetptcut=50.0;
  if(do_skim) jetptcut = 40.0;

  SimpleFilter<PFJet> jetPtEtaFilter = SimpleFilter<PFJet>
    ("JetPtEtaFilter")
    .set_input_label("pfJetsPFlow").set_predicate(bind(MinPtMaxEta, _1, jetptcut, 4.7));

  OneCollCompositeProducer<PFJet> jjPairProducer = OneCollCompositeProducer<PFJet>
    ("JetJetPairProducer")
    .set_input_label("pfJetsPFlow")
    .set_candidate_name_first("jet1")
    .set_candidate_name_second("jet2")
    .set_select_leading_pair(true)
    .set_output_label("jjLeadingCandidates");                                                        

  SimpleFilter<CompositeCandidate> jetPairFilter = SimpleFilter<CompositeCandidate>("JetPairFilter")
    .set_input_label("jjLeadingCandidates")
    .set_predicate( bind(DummyFunction<CompositeCandidate>, _1) )
    .set_min(1)
    .set_max(999);

  SimpleFilter<CompositeCandidate> etaProdJetPairFilter = SimpleFilter<CompositeCandidate>("EtaProdJetPairFilter")
    .set_input_label("jjLeadingCandidates")
    .set_predicate( bind(PairEtaProdLessThan, _1, 0))
    .set_min(1)
    .set_max(999);


  SimpleFilter<CompositeCandidate> detaJetPairFilter = SimpleFilter<CompositeCandidate>("DetaJetPairFilter")
    .set_input_label("jjLeadingCandidates")
    .set_predicate( !bind(PairDEtaLessThan, _1, 4.2) )
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
    .set_predicate( bind(PairMassInRange, _1,1200,8000) )
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

 
  // ------------------------------------------------------------------------------------
  // Met Modules
  // ------------------------------------------------------------------------------------  
  MetSelection metFilter = MetSelection("MetFilter",mettype,met_cut);

  unsigned nLepToAdd = 0;
  if (channel == channel::munu || channel == channel::enu) nLepToAdd = 1; 

  ModifyMet metNoMuons = ModifyMet("metNoMuons",mettype,"muonsPFlow",2,nLepToAdd);
  ModifyMet metNoElectrons = ModifyMet("metNoElectrons",mettype,"electrons",1,nLepToAdd);

  MetSelection metNoMuonFilter = MetSelection("MetNoMuonFilter","metNoMuons",met_cut);
  MetSelection metNoElectronFilter = MetSelection("MetNoElectronFilter","metNoElectrons",met_cut);

  //------------------------------------------------------------------------------------
  // W selection Modules
  // ------------------------------------------------------------------------------------
  double mtcut = 40;
  MTSelection muonMTFilter = MTSelection("MuonMTFilter",mettype,"muonsPFlow",mtcut);
  MTSelection electronMTFilter = MTSelection("ElectronMTFilter",mettype,"electrons",mtcut);


  // ------------------------------------------------------------------------------------
  // Selection Modules
  // ------------------------------------------------------------------------------------  
  
  HinvWeights hinvWeights = HinvWeights("HinvWeights")
    .set_era(era)
    .set_mc(mc)
    .set_do_trg_weights(false)
    .set_trg_applied_in_mc(true);

  if (!is_data) {
    hinvWeights.set_do_trg_weights(true).set_trg_applied_in_mc(true);
  }

  if (output_name.find("JetsToLNu") != output_name.npos) {
    hinvWeights.set_do_w_soup(true);
    if (mc == mc::summer12_53X) {
      hinvWeights.SetWTargetFractions(0.743925, 0.175999, 0.0562617, 0.0168926, 0.00692218);
      //hinvWeights.SetWInputYields(76102995.0, 23141598.0, 34044921.0, 15539503.0, 13382803.0);
      hinvWeights.SetWInputYields(76102995.0, 23141598.0, 33901569.0, 15539503.0, 13382803.0);
    }
  }
  if (output_name.find("DYJets") != output_name.npos && output_name.find("Soup") != output_name.npos) {
    if (mc == mc::summer12_53X) {
      hinvWeights.set_do_dy_soup(true);
      hinvWeights.SetDYTargetFractions(0.723342373, 0.190169492, 0.061355932, 0.017322034, 0.007810169);
      hinvWeights.SetDYInputYields(30459503.0, 24045248.0, 21852156.0, 11015445.0, 6402827.0);
    }
  }

  // ------------------------------------------------------------------------------------
  // Gen particle selection modules
  // ------------------------------------------------------------------------------------

  int lFlavour = 1;
  if (channel == channel::enu) lFlavour = 11;
  else if (channel == channel::munu) lFlavour = 13;
  else if (channel == channel::taunu) lFlavour = 15;
	
  SimpleCounter<GenParticle> WtoLeptonFilter = SimpleCounter<GenParticle>("WtoLeptonSelector")
    .set_input_label("genParticles")
    .set_predicate(
		   (bind(&GenParticle::status, _1) == 3) && 
		   (bind(abs,(bind(&GenParticle::pdgid, _1))) == lFlavour))
    .set_min(1);

  
  // ------------------------------------------------------------------------------------
  // Plot Modules
  // ------------------------------------------------------------------------------------  
  

  HinvControlPlots controlPlots_dijet = HinvControlPlots("DijetControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("JetPair");

  HinvWJetsPlots wjetsPlots_dijet = HinvWJetsPlots("DijetWJetsPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("electrons")
    .set_muons_label("muonsPFlow")
    .set_sel_label("JetPair");

  if (channel==channel::enu)
    wjetsPlots_dijet.set_met_nolep_label("metNoElectrons");

  HinvControlPlots controlPlots_met = HinvControlPlots("METControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("MET");

  HinvControlPlots controlPlots_looseMjj = HinvControlPlots("LooseMjjControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("LooseMjj");

  HinvControlPlots controlPlots_deta = HinvControlPlots("DEtaControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("DEta");

  HinvWJetsPlots wjetsPlots_deta = HinvWJetsPlots("DEtaWJetsPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("electrons")
    .set_muons_label("muonsPFlow")
    .set_sel_label("DEta");

  if (channel==channel::enu)
    wjetsPlots_deta.set_met_nolep_label("metNoElectrons");

  HinvControlPlots controlPlots_lepveto = HinvControlPlots("LeptonVetoControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("LeptonVeto");

  HinvControlPlots controlPlots_wsel = HinvControlPlots("WSelectionControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("WSelection");

  HinvWJetsPlots wjetsPlots_wsel = HinvWJetsPlots("WSelectionWJetsPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("electrons")
    .set_muons_label("muonsPFlow")
    .set_sel_label("WSelection");

  if (channel==channel::enu)
    wjetsPlots_wsel.set_met_nolep_label("metNoElectrons");

  HinvControlPlots controlPlots_dphi = HinvControlPlots("DPhiControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("DPhi");

  HinvWJetsPlots wjetsPlots_dphi = HinvWJetsPlots("DPhiWJetsPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("electrons")
    .set_muons_label("muonsPFlow")
    .set_sel_label("DPhi");

  if (channel==channel::enu)
    wjetsPlots_dphi.set_met_nolep_label("metNoElectrons");

  HinvControlPlots controlPlots_tightMjj = HinvControlPlots("TightMjjControlPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_dijet_label("jjLeadingCandidates")
    .set_sel_label("TightMjj");

  HinvWJetsPlots wjetsPlots_tightMjj = HinvWJetsPlots("TightMjjWJetsPlots")
    .set_fs(fs)
    .set_met_label(mettype)
    .set_met_nolep_label("metNoMuons")
    .set_electrons_label("electrons")
    .set_muons_label("muonsPFlow")
    .set_sel_label("TightMjj");

  if (channel==channel::enu)
    wjetsPlots_tightMjj.set_met_nolep_label("metNoElectrons");

  // ------------------------------------------------------------------------------------
  // Build Analysis Sequence
  // ------------------------------------------------------------------------------------  

   if (is_data && !do_skim)        analysis.AddModule(&lumiMask);
   if (!is_data && !do_skim)       analysis.AddModule(&pileupWeight);
   
   if (!do_skim) {

     analysis.AddModule(&dataMCTriggerPathFilter);

     ////analysis.AddModule(&runStats);
     analysis.AddModule(&hinvWeights);

     //jet modules
     analysis.AddModule(&jetIDFilter);
     analysis.AddModule(&jetPtEtaFilter);
     analysis.AddModule(&jjPairProducer);
     analysis.AddModule(&etaProdJetPairFilter);
     //analysis.AddModule(&jetPairFilter);
     analysis.AddModule(&controlPlots_dijet);

     //filter leptons before changing MET...
     analysis.AddModule(&selMuonFilter);
     analysis.AddModule(&selElectronFilter);
     //add met without leptons for plots
     analysis.AddModule(&metNoMuons);
     analysis.AddModule(&metNoElectrons);

     analysis.AddModule(&wjetsPlots_dijet);

     //met modules
     if (channel == channel::munu){
       analysis.AddModule(&metNoMuonFilter);
     }
     else if (channel == channel::enu){
       analysis.AddModule(&metNoElectronFilter);
     }
     else {
       analysis.AddModule(&metFilter);
     }
     analysis.AddModule(&controlPlots_met);
     
     //dijet modules
     analysis.AddModule(&looseMassJetPairFilter);
     analysis.AddModule(&controlPlots_looseMjj);
     analysis.AddModule(&detaJetPairFilter);
     analysis.AddModule(&controlPlots_deta);
     
     //add leptons just to plot them before specific W or Hinv selection
     analysis.AddModule(&wjetsPlots_deta);

     //prepare collections of veto leptons
     analysis.AddModule(&vetoElectronCopyCollection);
     analysis.AddModule(&vetoElectronFilter);
     analysis.AddModule(&vetoMuonCopyCollection);
     analysis.AddModule(&vetoMuonFilter);

     if (channel == channel::munu){
       analysis.AddModule(&oneMuonFilter);
       analysis.AddModule(&oneVetoMuonFilter);
       analysis.AddModule(&zeroVetoElectronFilter);
       analysis.AddModule(&muonMTFilter);
       analysis.AddModule(&controlPlots_wsel);
       analysis.AddModule(&wjetsPlots_wsel);
     }
     else if (channel == channel::enu){
       analysis.AddModule(&oneElectronFilter);
       analysis.AddModule(&oneVetoElectronFilter);
       analysis.AddModule(&zeroVetoMuonFilter);
       analysis.AddModule(&electronMTFilter);
       analysis.AddModule(&controlPlots_wsel);
       analysis.AddModule(&wjetsPlots_wsel);
     }
     else {
       //lepton veto modules
       analysis.AddModule(&zeroVetoMuonFilter);
       analysis.AddModule(&zeroVetoElectronFilter);
       analysis.AddModule(&controlPlots_lepveto);
     }

     //dphi cut
     if (signal_region==0) analysis.AddModule(&dphiJetPairFilter);
     else if (signal_region==1) analysis.AddModule(&dphiQCDJetPairFilter);
     if (signal_region!=2) {
       analysis.AddModule(&controlPlots_dphi);
       analysis.AddModule(&wjetsPlots_dphi);
     }
     //tight Mjj cut
     analysis.AddModule(&tightMassJetPairFilter);
     analysis.AddModule(&controlPlots_tightMjj);
     analysis.AddModule(&wjetsPlots_tightMjj);


     if (signal_region==2) {
       analysis.AddModule(&dphiJetPairFilter);
       analysis.AddModule(&controlPlots_dphi);
       analysis.AddModule(&wjetsPlots_dphi);
     }
     
   }
   else {
     //Build Skimming Analysis
     //analysis.AddModule(pointer to module defined above)
     if (channel == channel::nunu){
       analysis.AddModule(&jetPtEtaFilter);
       analysis.AddModule(&metFilter);
       analysis.AddModule(&jjPairProducer);
       analysis.AddModule(&looseMassJetPairFilter);
     }
     else {
       analysis.AddModule(&WtoLeptonFilter);
     }
   }
   // Run analysis
   
   analysis.RetryFileAfterFailure(5,5);// int <pause between attempts in seconds>, int <number of retry attempts to make> );
   analysis.StopOnFileFailure(true);
   analysis.SetTTreeCaching(true); 

   analysis.RunAnalysis();
   delete fs;
   return 0;
}




