#include <iostream>
#include <vector>
#include <string>
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "boost/bind.hpp"
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
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttVbfCategory.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttVHCategory.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttOneJetCategory.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttOneBJetCategory.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttBTagCategory.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttNoBTagCategory.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttZeroJetCategory.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttRecoilCorrector.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttSync.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttPrint.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/WJetsWeights.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/FSRStudy.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/RunFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/EnergyShifter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/SVFit.h"

using boost::lexical_cast;
using boost::bind;
namespace po = boost::program_options;

int main(int argc, char* argv[]){
  using namespace ic;
  
  // Configurable parameters
  int max_events;
  unsigned mode, mssm_mode, tau_scale_mode;
  unsigned is_data, era;
  // Era 0: 2012 52X ICHEP
  // Era 1: 2012 53X ICHEP
  // Era 2: 2012 53X HCP
  bool is_2012;
  bool scan_trigger;
  std::string outname, filelist, configfile, svfit_override, outfolder, svfit_folder;
  bool do_svfit = false;
  int svfit_op;
  bool do_skim;
  std::string skim_path = "";
  int ztatau_mode;
  bool do_vbf_mva = true;
  unsigned special_mode = 0; // No special mode
  bool disable_mc_trigger = false;
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

  bool is_embedded = false;
  bool disable_reweighting = false;
  bool mva_met_from_vector = true;
  bool do_btag_weight = false;
  bool do_sync = false;
  unsigned faked_tau_selector = 0;


  // Load the config
  po::options_description preconfig("Pre-Configuration");
  preconfig.add_options()
      ("cfg", po::value<std::string>(&configfile)->required(), 
            "configuration file");
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
      ("max_events", po::value<int>(&max_events)->default_value(-1), "maximum number of events to process")
      ("mode", po::value<unsigned>(&mode)->required(), "0=elec-tau, 1=mu-tau, 2=elec-mu")
      ("mssm_mode", po::value<unsigned>(&mssm_mode)->default_value(0), "0=off, 1=on")
      ("is_2012", po::value<bool>(&is_2012)->required(), "0=2011, 1=2012")
      ("era", po::value<unsigned>(&era)->required(), "era")
      ("is_data", po::value<unsigned>(&is_data)->required(), "0=mc, 1=data")
      ("output", po::value<std::string>(&outname)->required(), "ROOT file output name")
      ("filelist", po::value<std::string>(&filelist)->required(), "input filelist")
      ("skim", po::value<bool>(&do_skim)->default_value(false), "run in skimming mode")
      ("do_sync", po::value<bool>(&do_sync)->default_value(false), "make sync ntuple")
      ("skim_path", po::value<std::string>(&skim_path), "output folder for skims")
      ("ztautau_mode", po::value<int>(&ztatau_mode)->default_value(0), "0=disabled, 1 = require 2 taus, 2 = veto taus")
      ("do_svfit", po::value<bool>(&do_svfit)->default_value(false), "0=disabled, 1 = enabled")
      ("do_vbf_mva", po::value<bool>(&do_vbf_mva)->default_value(true), "0=disabled, 1 = enabled")
      ("svfit_op", po::value<int>(&svfit_op)->default_value(2), "svfit_op")
      ("special_mode", po::value<unsigned>(&special_mode)->default_value(0), "special_mode")
      ("is_embedded", po::value<bool>(&is_embedded)->default_value(false), "true/false")
      ("scan_trigger", po::value<bool>(&scan_trigger)->default_value(false), "true/false")
      ("mva_met_from_vector", po::value<bool>(&mva_met_from_vector)->default_value(true), "true/false")
      ("disable_mc_trigger", po::value<bool>(&disable_mc_trigger)->default_value(false), "true/false")
      ("disable_reweighting", po::value<bool>(&disable_reweighting)->default_value(false), "true/false")
      ("faked_tau_selector", po::value<unsigned>(&faked_tau_selector)->default_value(0), "faked_tau_selector")
      ("tau_scale_mode", po::value<unsigned>(&tau_scale_mode)->default_value(0), "tau_scale_mode")
      ("svfit_override", po::value<std::string>(&svfit_override)->default_value(""), "override the svfit folder")
      ("outfolder", po::value<std::string>(&outfolder)->default_value(""), "output folder")
      ("svfit_folder", po::value<std::string>(&svfit_folder)->default_value("SVFIT_2012/"), "svfit folder");
  po::store(po::command_line_parser(argc, argv).
            options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(configfile.c_str(), config), vm);
  po::notify(vm);

  do_btag_weight = true;


  if (do_skim) disable_reweighting = true;
  if (special_mode > 0) {
    outname = "Special_"+lexical_cast<std::string>(special_mode)+"_" + outname;
  }

  std::cout << "-------------------------------------------------------------------" << std::endl;
  std::cout << "HiggsTauTau Analysis" << std::endl;
  std::cout << "-------------------------------------------------------------------" << std::endl;
  std::cout << "----------CORE----------" << std::endl;
  std::cout << "Mode: " << mode << std::endl;
  std::cout << "MSSM Mode: " << mssm_mode << std::endl;
  std::cout << "Is Data?: " << is_data << std::endl;
  std::cout << "Is 2012?: " << is_2012 << std::endl;
  std::cout << "Era: " << era << std::endl;
  std::cout << "Using VBF MVA: " << do_vbf_mva << std::endl;
  std::cout << "Tau Scale Mode:" << tau_scale_mode << std::endl;
  if (outfolder == "") outfolder = ".";
  if (tau_scale_mode == 1) outfolder += "/TSCALE_DOWN";
  if (tau_scale_mode == 2) outfolder += "/TSCALE_UP";
  if (tau_scale_mode == 1) svfit_folder += "/TSCALE_DOWN";
  if (tau_scale_mode == 2) svfit_folder += "/TSCALE_UP";
  if (outfolder == "") std::cout << "Output file: " << outname << std::endl;
  if (outfolder != "") std::cout << "Output file: " << (outfolder+"/"+outname) << std::endl;
  std::cout << "----------EXTRA----------" << std::endl;
  if (special_mode > 0) {
    std::cout << "Applying Special Mode: " << special_mode << std::endl;
  }
  if (do_skim) std::cout << "Running in skim mode!" << std::endl;
  if (ztatau_mode > 0) {
    if (ztatau_mode == 1) std::cout << "ztautau_mode: " << ztatau_mode << " (Selecting Z->tautau events)" << std::endl;
    if (ztatau_mode == 2) std::cout << "ztautau_mode: " << ztatau_mode << " (Selecting Z->ee/mumu events)" << std::endl;
  } 
  std::cout << "Taking MVA MET from vector?: " << mva_met_from_vector << std::endl;


  // Load necessary libraries for ROOT I/O of custom classes
  gSystem->Load("libFWCoreFWLite.dylib");
  gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  AutoLibraryLoader::enable();
  // Build a vector of input files
  std::vector<std::string> files = ParseFileLines(filelist);
  // Create ROOT output fileservice
  fwlite::TFileService *fs;
  if (outfolder == "") {
    fs = new fwlite::TFileService(outname.c_str());
  } else {
    fs = new fwlite::TFileService((outfolder+"/"+outname).c_str());
  }
  // Set-up pileup reweighting files 
  // 2011
  std::string data_pu_path = "data/pileup/Data_Pileup_2011_HCP-500bins.root";
  std::string mc_pu_path = "data/pileup/MC_Fall11_PU_S6-500bins.root";
  if (is_2012) {
    if (era == 0) {
      data_pu_path = "data/pileup/Data_Pileup_2012.root";
      mc_pu_path = "data/pileup/MC_Summer12_PU_S7.root";
    } else if (era == 1) {
      data_pu_path = "/data/pileup/Data_Pileup_2012.root";
      mc_pu_path = "data/pileup/MC_Summer12_PU_S7.root";
    } else if (era == 2) {
      data_pu_path = "data/pileup/Data_Pileup_2012_HCP-600bins.root";
      mc_pu_path = "data/pileup/MC_Summer12_PU_S10-600bins.root";
    }
  }
  TH1D data_pu  = GetFromTFile<TH1D>(data_pu_path, "/", "pileup");
  TH1D mc_pu    = GetFromTFile<TH1D>(mc_pu_path, "/", "pileup");
  

  std::string lep1_label, lep2_label, lep1sel_label, lep2sel_label, met_label;
  double elec_dz, muon_dz, elec_dxy, muon_dxy, tau_dz, mt_max_selection;
  double elec_pt, elec_eta, muon_pt, muon_eta, tau_pt, tau_eta;
  met_label = "pfMVAMet";
  if (era == 0 || era == 1) met_label = "pfMet";
  mt_max_selection = 40.0;
  if (mode == 0) {
    lep1_label = "electrons";
    lep1sel_label = "selElectrons";
    lep2_label = "taus";
    lep2sel_label = "taus";
    elec_dz = 0.2;
    elec_dxy = 0.045;
    muon_dz = 0.2;
    muon_dxy = 0.045;
    tau_dz = 0.2;
    if (!is_2012) {
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
  } else if (mode == 1) {
    lep1_label = "muonsPFlow";
    lep1sel_label = "selMuons";
    lep2_label = "taus";
    lep2sel_label = "taus";
    elec_dz = 0.2;
    elec_dxy = 0.045;
    muon_dz = 0.2;
    muon_dxy = 0.045;
    tau_dz = 0.2;
    if (!is_2012) {
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
  }else if (mode == 2) {
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
  }

  if (special_mode == 14 || special_mode == 17) elec_dxy = 0.02;     
  if (special_mode == 15 || special_mode == 17) muon_dxy = 0.2;                          
  // Lower pt thresholds on e and taus when skimming - to do energy scale datacards later
  if (do_skim) {
    tau_pt = 19.0;
    if (mode == 0) {
      if (!is_2012) {
        elec_pt = 19.0;
      } else {
        elec_pt = 22.0;
      }
    } else if (mode == 2) {
      elec_pt = 9.5;
    }
  }                     


  std::cout << "----------PARAMETERS----------" << std::endl;
  std::cout << boost::format("%-15s %-10s\n") % "elec_pt:" % elec_pt;
  std::cout << boost::format("%-15s %-10s\n") % "elec_eta:" % elec_eta;
  std::cout << boost::format("%-15s %-10s\n") % "elec_dxy:" % elec_dxy;
  std::cout << boost::format("%-15s %-10s\n") % "elec_dz:" % elec_dz;
  std::cout << boost::format("%-15s %-10s\n") % "muon_pt:" % muon_pt;
  std::cout << boost::format("%-15s %-10s\n") % "muon_eta:" % muon_eta;
  std::cout << boost::format("%-15s %-10s\n") % "muon_dxy:" % muon_dxy;
  std::cout << boost::format("%-15s %-10s\n") % "muon_dz:" % muon_dz;
  std::cout << boost::format("%-15s %-10s\n") % "tau_pt:" % tau_pt;
  std::cout << boost::format("%-15s %-10s\n") % "tau_eta:" % tau_eta;
  std::cout << boost::format("%-15s %-10s\n") % "tau_dz:" % tau_dz;
  // Create analysis object
  ic::AnalysisBase analysis(
    "HiggsTauTau",        // Analysis name
    files,                // Input files
    "agEventProducer",    // TTree path
    "EventTree",          // TTree name
    max_events);          // Max. events to process (-1 = all)
  if (do_skim && skim_path != "") analysis.DoSkimming(skim_path);

  // ------------------------------------------------------------------------------------
  // Misc Modules
  // ------------------------------------------------------------------------------------
  FSRStudy fsrStudy("FSRStudy");
  fsrStudy.set_fs(fs);

  RunFilter runFilter2011("RunFilter2011");
  runFilter2011.FilterRun(160431); runFilter2011.FilterRun(160577); runFilter2011.FilterRun(160578);
  runFilter2011.FilterRun(160871); runFilter2011.FilterRun(160872); runFilter2011.FilterRun(160873);
  runFilter2011.FilterRun(160874); runFilter2011.FilterRun(160939); runFilter2011.FilterRun(160940);
  runFilter2011.FilterRun(160942); runFilter2011.FilterRun(160943); runFilter2011.FilterRun(160955);
  runFilter2011.FilterRun(160956); runFilter2011.FilterRun(160957); runFilter2011.FilterRun(160998);
  runFilter2011.FilterRun(161008); runFilter2011.FilterRun(161016); runFilter2011.FilterRun(161103);
  runFilter2011.FilterRun(161106); runFilter2011.FilterRun(161107); runFilter2011.FilterRun(161113);
  runFilter2011.FilterRun(161116); runFilter2011.FilterRun(161117); runFilter2011.FilterRun(161119);
  runFilter2011.FilterRun(161156); runFilter2011.FilterRun(161176); runFilter2011.FilterRun(161217); 
  runFilter2011.FilterRun(161222); runFilter2011.FilterRun(161223); runFilter2011.FilterRun(161233);
  runFilter2011.FilterRun(161310); runFilter2011.FilterRun(161311); runFilter2011.FilterRun(161312);
  runFilter2011.FilterRun(162762); runFilter2011.FilterRun(162765); runFilter2011.FilterRun(162803);
  runFilter2011.FilterRun(162808); runFilter2011.FilterRun(162811); runFilter2011.FilterRun(162822);
  runFilter2011.FilterRun(162825); runFilter2011.FilterRun(162826); runFilter2011.FilterRun(162828);
  runFilter2011.FilterRun(162909); runFilter2011.FilterRun(163046); runFilter2011.FilterRun(163069);
  runFilter2011.FilterRun(163071); runFilter2011.FilterRun(163078); runFilter2011.FilterRun(163232);
  runFilter2011.FilterRun(163233); runFilter2011.FilterRun(163234); runFilter2011.FilterRun(163235);
  runFilter2011.FilterRun(163237); runFilter2011.FilterRun(163238); runFilter2011.FilterRun(163252);
  runFilter2011.FilterRun(163255); runFilter2011.FilterRun(163261);

  RunFilter runFilterICHEP("RunFilterICHEP");
  runFilter2011.FilterRun(160431); runFilter2011.FilterRun(160577); runFilter2011.FilterRun(160578);
  runFilter2011.FilterRun(160871); runFilter2011.FilterRun(160872); runFilter2011.FilterRun(160873);
  runFilter2011.FilterRun(160874); runFilter2011.FilterRun(160939); runFilter2011.FilterRun(160940);
  runFilter2011.FilterRun(160942); runFilter2011.FilterRun(160943); runFilter2011.FilterRun(160955);


  HttPrint httPrint("HttPrint", mode);

  PileupWeight pileupWeight = PileupWeight
    ("PileupWeight").set_data(&data_pu).set_mc(&mc_pu).set_print_weights(false);

  HttDataTriggerFilter dataTriggerPathFilter("TriggerPathFilter");
  dataTriggerPathFilter.set_mode(mode).set_do_obj_match(true).set_lep1label(lep1sel_label).set_lep2label(lep2sel_label);
  if (special_mode == 14 || special_mode == 15 || special_mode == 17) dataTriggerPathFilter.set_do_obj_match(true);
  if (scan_trigger) dataTriggerPathFilter.set_dump_run_yield(true);

  HttMCTriggerFilter mcTriggerPathFilter = HttMCTriggerFilter
    ("TriggerPathMCFilter").set_is_2012(is_2012).set_mode(mode).set_era(era)
    .set_do_obj_match(true).set_lep1label(lep1sel_label).set_lep2label(lep2sel_label);

  SimpleCounter<GenParticle> zTauTauFilter = SimpleCounter<GenParticle>
    ("ZToTauTauSelector")
    .set_input_label("genParticles")
    .set_predicate((bind(&GenParticle::status, _1) == 3) && ((bind(&GenParticle::pdgid, _1) == 15)||(bind(&GenParticle::pdgid, _1) == -15)))
    .set_min(2);
  if (ztatau_mode == 2) zTauTauFilter.set_min(0).set_max(0);

  double mssm_mass = 10.;
  bool do_mass_filter = false;
  if ((outname.find("SUSYGluGluToHToTauTau") != outname.npos) || (outname.find("SUSYBBHToTauTau") != outname.npos)) {
    std::size_t pos = outname.find("_M-");
    if (pos != outname.npos) {
      std::string mass_string;
      for (unsigned i = (pos+3); i < outname.size(); ++i) {
        if (outname.at(i) != '_') mass_string += outname.at(i);
        if (outname.at(i) == '_') break;
      }
      std::cout << "MSSM Signal sample detected with mass: " << mass_string << std::endl;
      mssm_mass = boost::lexical_cast<double>(mass_string);
      do_mass_filter = true;
    }
  }
  
  SimpleCounter<GenParticle> mssmMassFilter = SimpleCounter<GenParticle>
    ("MSSMMassFilter")
    .set_input_label("genParticles")
    .set_predicate(bind(GenParticleInMassBand, _1, 36, 0.7*mssm_mass, 1.3*mssm_mass))
    .set_min(1);

  SimpleCounter<GenParticle> embeddedMassFilter = SimpleCounter<GenParticle>
    ("EmbeddedMassFilter")
    .set_input_label("genParticles")
    .set_predicate(bind(GenParticleInMassBand, _1, 23, 50., 9999999.))
    .set_min(1);

  WJetsWeights wjetsWeights("WJetsWeights");
  wjetsWeights.set_fs(fs);
  
  // ------------------------------------------------------------------------------------
  // Electron Modules
  // ------------------------------------------------------------------------------------
  CopyCollection<Electron>  
    selElectronCopyCollection("CopyToSelElectrons","electrons","selElectrons");

  EnergyShifter<Electron> electronEnergyShifter = EnergyShifter<Electron>
    ("ElectronEnergyShifter")
    .set_input_label("electrons")
    .set_shift(1.03);



  SimpleFilter<Electron> selElectronPtEtaFilter = SimpleFilter<Electron>
    ("SelElectronPtEtaFilter")
    .set_input_label("selElectrons")
    .set_predicate(bind(MinPtMaxEta, _1, elec_pt, elec_eta))
    .set_min(1);

  SimpleFilter<Electron> selElectronDxyFilter = SimpleFilter<Electron>
    ("SelElectronDxyFilter")
    .set_input_label("selElectrons")
    .set_predicate(bind(fabs, bind(&Electron::dxy_vertex, _1)) < elec_dxy)
    // .set_predicate([&](Electron const* elec) { return true; })
    .set_min(1);

  SimpleFilter<Electron> selElectronDzFilter = SimpleFilter<Electron>
    ("SelElectronDzFilter")
    .set_input_label("selElectrons")
    .set_predicate(bind(fabs, bind(&Electron::dz_vertex, _1)) < elec_dz)
    .set_min(1);

  SimpleFilter<Electron> selElectronIdIsoFilter = SimpleFilter<Electron>
    ("SelElectronIdIsoFilter")
    .set_input_label("selElectrons")
    .set_predicate(bind(ElectronHTTIdIso, _1, mode) && bind(PF04Isolation<Electron>, _1, 0.5, 0.1))
    .set_min(1);
    if (mode == 2) selElectronIdIsoFilter.set_predicate(bind(ElectronHTTIdIso, _1, mode) && bind(PF04IsolationEBElec, _1, 0.5, 0.15, 0.1));
    if (special_mode == 1) {
      selElectronIdIsoFilter.set_predicate(bind(ElectronHTTIdIso, _1, mode));
    } else if (special_mode == 3 || special_mode == 6 || special_mode == 10) {
      selElectronIdIsoFilter.set_predicate(bind(ElectronHTTIdIso, _1, mode) && (bind(PF04IsolationVal<Electron>, _1, 0.5) > 0.2) && (bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.5));
    } else if (special_mode == 16) {
      selElectronIdIsoFilter.set_predicate(bind(ElectronHTTIdIso, _1, mode) && (bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.3));
    } else if (special_mode == 2 || special_mode == 5 || /*special_mode == 10 ||*/ special_mode == 12) {
      selElectronIdIsoFilter.set_predicate(bind(ElectronHTTIdIso, _1, mode) && (bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.5));
    } else if (special_mode == 14 || special_mode == 17) {
      selElectronIdIsoFilter.set_predicate(bind(HttEMuFakeElectron, _1));
    }
  

  // Electron Veto
  CopyCollection<Electron>  vetoElectronCopyCollection("CopyToVetoElectrons","electrons","vetoElectrons");

  SimpleFilter<Electron> vetoElectronPtEtaFilter = SimpleFilter<Electron>
    ("VetoElectronPtEtaFilter")
    .set_input_label("vetoElectrons").set_predicate(bind(MinPtMaxEta, _1, 15.0, 2.5));

  SimpleFilter<Electron> vetoElectronIdIsoFilter = SimpleFilter<Electron>
    ("VetoElectronIdIsoFilter")
    .set_input_label("vetoElectrons").set_predicate(bind(Electron2011WP95ID, _1) && bind(PF04Isolation<Electron>, _1, 0.5, 0.3));

  SimpleFilter<Electron> vetoElectronDxyFilter = SimpleFilter<Electron>
    ("VetoElectronDxyFilter")
    .set_input_label("vetoElectrons").set_predicate(bind(fabs, bind(&Electron::dxy_vertex, _1)) < elec_dxy);

  SimpleFilter<Electron> vetoElectronDzFilter = SimpleFilter<Electron>
    ("VetoElectronDzFilter")
    .set_input_label("vetoElectrons").set_predicate(bind(fabs, bind(&Electron::dz_vertex, _1)) < elec_dz);

  OneCollCompositeProducer<Electron> vetoElectronPairProducer = OneCollCompositeProducer<Electron>
    ("VetoPairProducer")
    .set_input_label("vetoElectrons").set_candidate_name_first("elec1").set_candidate_name_second("elec2").set_output_label("vetoPairs");

  SimpleFilter<CompositeCandidate> vetoElectronPairFilter = SimpleFilter<CompositeCandidate>
    ("VetoPairFilter")
    .set_input_label("vetoPairs")
    .set_predicate( (bind(&CompositeCandidate::DeltaR, _1, "elec1","elec2") > 0.15) && (bind(&CompositeCandidate::charge, _1) == 0) )
    .set_min(0)
    .set_max(0);

    SimpleCounter<Electron> extraElectronVeto = SimpleCounter<Electron>
    ("ExtraElectronVeto")
    .set_input_label("electrons")
    .set_predicate(bind(MinPtMaxEta, _1, 10.0, 2.5) 
                && bind(ElectronHTTIdIso, _1, 2)
                && bind(PF04Isolation<Electron>, _1, 0.5, 0.3)
                && (bind(fabs, bind(&Electron::dxy_vertex, _1)) < elec_dxy)
                && (bind(fabs, bind(&Electron::dz_vertex, _1)) < elec_dz)
                )
      // .set_predicate([&](Electron const* elec) { return
      //   elec->pt() > 10 
      //   && fabs(elec->eta()) < 2.5
      //   && ElectronHTTIdIso(elec, 2)
      //   && PF04Isolation(elec, 0.5, 0.3)
      //   && fabs(elec->dxy_vertex()) < elec_dxy
      //   && fabs(elec->dz_vertex() < elec_dz);
      // })
    .set_min(0).set_max(mode == 0 ? 1 : 0);
                

  // ------------------------------------------------------------------------------------
  // Muon Modules
  // ------------------------------------------------------------------------------------
  CopyCollection<Muon> selMuonCopyCollection("CopyToSelMuons","muonsPFlow","selMuons");

  SimpleFilter<Muon> selMuonPtEtaFilter = SimpleFilter<Muon>
    ("SelMuonPtEtaFilter")
    .set_input_label("selMuons")
    .set_predicate(bind(MinPtMaxEta, _1, muon_pt, muon_eta))
    .set_min(1);
  
  SimpleFilter<Muon> selMuonDxyFilter = SimpleFilter<Muon>
    ("SelMuonDxyFilter")
    .set_input_label("selMuons")  
    .set_predicate(bind(fabs, bind(&Muon::dxy_vertex, _1)) < muon_dxy)
    .set_min(1);

  SimpleFilter<Muon> selMuonDzFilter = SimpleFilter<Muon>
    ("MuonDzFilter")
    .set_input_label("selMuons")
    .set_predicate(bind(fabs, bind(&Muon::dz_vertex, _1)) < muon_dz)
    .set_min(1);

  SimpleFilter<Muon> selMuonIdIsoFilter = SimpleFilter<Muon>
    ("SelMuonIdIsoFilter")
    .set_input_label("selMuons")
    .set_predicate(bind(MuonTight, _1) && bind(PF04Isolation<Muon>, _1, 0.5, 0.1) )
    .set_min(1);
    if (mode == 2) selMuonIdIsoFilter.set_predicate(bind(MuonTight, _1) && bind(PF04IsolationEB<Muon>, _1, 0.5, 0.15, 0.1));
    if (special_mode == 1) 
      selMuonIdIsoFilter.set_predicate(bind(MuonTight, _1));
    if (special_mode == 3 || special_mode == 6 || special_mode == 10) 
      selMuonIdIsoFilter.set_predicate(bind(MuonTight, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5) > 0.2) && (bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.5));
    if (special_mode == 16) 
     selMuonIdIsoFilter.set_predicate(bind(MuonTight, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.3));
    if (special_mode == 2 || special_mode == 5 || /*special_mode == 10 ||*/ special_mode == 12) 
      selMuonIdIsoFilter.set_predicate(bind(MuonTight, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.5));
    if (special_mode == 15 || special_mode == 17) 
      selMuonIdIsoFilter.set_predicate(bind(HttEMuFakeMuon, _1));
  
   // Muon Veto
  CopyCollection<Muon> vetoMuonCopyCollection("CopyToVetoMuons","muonsPFlow","vetoMuons");

  SimpleFilter<Muon> vetoMuonPtEtaFilter = SimpleFilter<Muon>
    ("VetoMuonPtEtaFilter")
    .set_input_label("vetoMuons").set_predicate(bind(MinPtMaxEta, _1, 15.0, 2.4));

  SimpleFilter<Muon> vetoMuonIdIsoFilter = SimpleFilter<Muon>
    ("VetoMuonIdIsoFilter")
    .set_input_label("vetoMuons").set_predicate(bind(&Muon::is_global, _1) && bind(PF04Isolation<Muon>, _1, 0.5, 0.3));

  SimpleFilter<Muon> vetoMuonDxyFilter = SimpleFilter<Muon>
    ("VetoMuonDxyFilter")
    .set_input_label("vetoMuons").set_predicate(bind(fabs, bind(&Muon::dxy_vertex, _1)) < muon_dxy);

  SimpleFilter<Muon> vetoMuonDzFilter = SimpleFilter<Muon>
    ("VetoMuonDzFilter")
    .set_input_label("vetoMuons").set_predicate(bind(fabs, bind(&Muon::dz_vertex, _1)) < muon_dz);

  OneCollCompositeProducer<Muon> vetoMuonPairProducer = OneCollCompositeProducer<Muon>
    ("VetoPairProducer")
    .set_input_label("vetoMuons").set_candidate_name_first("muon1").set_candidate_name_second("muon2").set_output_label("vetoPairs");

  SimpleFilter<CompositeCandidate> vetoMuonPairFilter = SimpleFilter<CompositeCandidate>
    ("VetoPairFilter")
    .set_predicate( (bind(&CompositeCandidate::DeltaR, _1, "muon1","muon2") > 0.15) && (bind(&CompositeCandidate::charge, _1) == 0) )
    .set_input_label("vetoPairs")
    .set_min(0)
    .set_max(0);       

  OverlapFilter<Electron, Muon> elecMuonOverlapFilter = OverlapFilter<Electron, Muon>
    ("ElecMuonOverlapFilter")
    .set_input_label("selElectrons")
    .set_reference_label("muonsPFlow")
    .set_min_dr(0.3); 

  SimpleCounter<Muon> extraMuonVeto = SimpleCounter<Muon>
  ("ExtraMuonVeto")
  .set_input_label("muonsPFlow")
  .set_predicate(bind(MinPtMaxEta, _1, 10.0, 2.4) 
              && bind(MuonTight, _1) 
              && bind(PF04Isolation<Muon>, _1, 0.5, 0.3)
              && (bind(fabs, bind(&Muon::dxy_vertex, _1)) < muon_dxy)
              && (bind(fabs, bind(&Muon::dz_vertex, _1)) < muon_dz)
              )
  .set_min(0).set_max(mode == 1 ? 1 : 0);                   

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

  SimpleFilter<Tau> tauPtEtaFilter = SimpleFilter<Tau>
    ("TauPtEtaFilter")
    .set_input_label("taus")
    .set_predicate(bind(MinPtMaxEta, _1, tau_pt, tau_eta))
    .set_min(1); 

  SimpleFilter<Tau> tauDzFilter = SimpleFilter<Tau>
    ("TauDzFilter")
    .set_input_label("taus")
    .set_predicate(bind(fabs, bind(&Tau::lead_dz_vertex, _1)) < tau_dz)
    .set_min(1);

  SimpleFilter<Tau> tauIsoFilter = SimpleFilter<Tau>
    ("TauIsoFilter")
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

  SimpleFilter<Tau> tauElRejectFilter = SimpleFilter<Tau>
    ("TauElRejectFilter")
    .set_predicate(bind(&Tau::GetTauID, _1, "againstElectronLoose") > 0.5)
    .set_input_label("taus").set_min(1); 
  if (mode == 0) tauElRejectFilter
    .set_predicate( bind(&Tau::GetTauID, _1, "againstElectronMVA") > 0.5);                        
  if (mode == 0 && !do_skim && special_mode != 18) tauElRejectFilter
    .set_predicate( (bind(&Tau::GetTauID, _1, "againstElectronTightMVA2") > 0.5) && (bind(&Tau::GetTauID, _1, "againstElectronMVA") > 0.5) );                        

  SimpleFilter<Tau> tauMuRejectFilter = SimpleFilter<Tau>
    ("TauMuRejectFilter")
    .set_predicate(bind(&Tau::GetTauID, _1, "againstMuonTight") > 0.5)
    .set_input_label("taus").set_min(1);  
  if (mode == 0) tauMuRejectFilter
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

  if (mode == 2) pairFilter
    .set_predicate( (bind(PairOneWithPt, _1, 20.0)) && (bind(&CompositeCandidate::DeltaR, _1, "lepton1","lepton2") > 0.3));

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
  HttPairSelector httPairSelector = HttPairSelector
    ("HttPairSelector")
    .set_mode(mode)
    .set_fs(fs)
    .set_met_label(met_label)
    .set_mva_met_from_vector(mva_met_from_vector)
    .set_faked_tau_selector(faked_tau_selector)
    .set_scale_met_for_tau(tau_scale_mode > 0)
    .set_tau_scale(tau_shift);

  HttRecoilCorrector httRecoilCorrector = HttRecoilCorrector
    ("HttRecoilCorrector");
    httRecoilCorrector.set_sample(outname).set_mode(mode).set_is_2012(is_2012).set_met_label(met_label).set_era(era);

  HttWeights httWeights = HttWeights
    ("HttWeights")
    .set_mode(mode)
    .set_era(era)
    .set_is_2012(is_2012)
    .set_trg_applied_in_mc(true)
    .set_do_trg_weights(false)
    .set_do_etau_fakerate(false)
    .set_do_idiso_weights(false)
    .set_do_emu_e_fakerates(false)
    .set_do_emu_m_fakerates(false)
    .set_do_top_factors(false)
    .set_do_btag_weight(false);
  if (!is_data && !disable_reweighting) {
    httWeights.set_do_trg_weights(true).set_trg_applied_in_mc(true).set_do_idiso_weights(true);
  }
  if (!is_data && do_btag_weight) httWeights.set_do_btag_weight(true);
  if (outname.find("DYJetsToLL") != outname.npos && mode == 0 && !is_2012) httWeights.set_do_etau_fakerate(true);
  if (is_embedded) httWeights.set_do_trg_weights(true).set_trg_applied_in_mc(false).set_do_idiso_weights(false);
  if (special_mode == 14 || special_mode == 17) httWeights.set_do_emu_e_fakerates(true);
  if (special_mode == 15 || special_mode == 17) httWeights.set_do_emu_m_fakerates(true);
  if (outname.find("TTJets") != outname.npos && mode == 2 && era == 0) httWeights.set_do_top_factors(true);
  if (outname.find("JetsToLNu") != outname.npos) {
    httWeights.set_do_w_soup(true);
    // 2011
    httWeights.SetWTargetFractions(0.752332, 0.171539, 0.0538005, 0.0159036, 0.00642444);
    httWeights.SetWInputYields(81295381.0, 70712575.0, 25320546.0, 7541595.0, 12973738.0);
    if (is_2012) {
      // 52X
      if (era == 0) {
        httWeights.SetWTargetFractions(0.743925, 0.175999, 0.0562617, 0.0168926, 0.00692218);
        httWeights.SetWInputYields(76510124.0, 0.0, 0.0, 15539503.0, 0.0);
      // 53X
      } else if (era == 1 || era == 2) {
        httWeights.SetWTargetFractions(0.743925, 0.175999, 0.0562617, 0.0168926, 0.00692218);
        httWeights.SetWInputYields(76102995.0, 23101598.0, 33884921.0, 15539503.0, 13382803.0);
      }
    }
  }
  if (!is_2012 && outname.find("GluGluToHToTauTau_M-") != outname.npos && outname.find("SUSYGluGluToHToTauTau_M-") == outname.npos) {
    std::size_t pos = outname.find("_M-");
    if (pos != outname.npos) {
      std::string mass_string;
      for (unsigned i = (pos+3); i < outname.size(); ++i) {
        if (outname.at(i) != '_') mass_string += outname.at(i);
        if (outname.at(i) == '_') break;
      }
      std::cout << "SM ggH Signal sample detected with mass: " << mass_string << std::endl;
      httWeights.set_ggh_mass(mass_string);
    }
  }


  HttSelection httSelection = HttSelection
    ("HttSelection")
    .set_fs(fs)
    .set_mode(mode)
    .set_met_label(met_label);
  httSelection.set_mssm_mode(mssm_mode);
  httSelection.set_mt_max_selection(mt_max_selection);
  if (special_mode == 5 || special_mode == 12) httSelection.set_distinguish_os(false);
  if (special_mode == 7) httSelection.set_mt_min_control(60.0).set_mt_max_control(120.0);
  if (era == 0 || era == 1) httSelection.set_mt_max_selection(40.0);

  HttMetStudy httMetStudy = HttMetStudy
    ("HttMetStudy")
    .set_fs(fs)
    .set_mode(mode)
    .set_met_label(met_label);

  // ------------------------------------------------------------------------------------
  // Category Modules
  // ------------------------------------------------------------------------------------  
  HttVbfCategory httVbfCategory = HttVbfCategory
    ("HttVbfCategory","pfJetsPFlow")
    .set_fs(fs)
    .set_mode(mode)
    .set_met_label(met_label)
    .set_do_jetvtx_assoc(false)
    .set_do_mc_eff(false)
    .set_do_vbf_mva(do_vbf_mva)
    .set_do_mva_pu_id(true)
    .set_make_plots(false)
    .set_mva_cut(0.5)
    .set_make_mva_tree(false)
    .set_do_cjv(true)
    .set_vbf_cjv_pt(30.0)
    .set_mva_name("MVA_"+outname);
  if (special_mode == 6 || special_mode == 8) httVbfCategory.set_jet_pt(20.0);
  if (special_mode == 6) httVbfCategory.set_mva_cut(0.0);
  httVbfCategory.set_is_2012(is_2012).set_use_hcp_mva(true);
  if (!is_2012) {
    httVbfCategory.set_mva_cut(0.676);
    if (mode == 2) httVbfCategory.set_mva_cut(0.9);
  } else {
    httVbfCategory.set_mva_cut(0.978);
    if (mode == 2) httVbfCategory.set_mva_cut(0.9);
  }
  if (era == 0 || era == 1) {
    httVbfCategory.set_use_hcp_mva(false).set_mva_cut(0.5);
    httVbfCategory.set_jet_eta(5.0);
  }

  HttVHCategory httVHCategory = HttVHCategory
    ("HttVHCategory","pfJetsPFlow");

  HttOneJetCategory httOneJetHighPtCategory = HttOneJetCategory
    ("HttOneJetHighPtCategory","pfJetsPFlow").set_fs(fs).set_high_pt(true).set_do_met_cut(true).set_mode(mode).set_met_label(met_label);
  httOneJetHighPtCategory.set_met_cut(30.0);
  if (era == 0 || era == 1) httOneJetHighPtCategory.set_jet_eta(5.0);
  

  HttOneJetCategory httOneJetLowPtCategory = HttOneJetCategory
    ("HttOneJetLowPtCategory","pfJetsPFlow").set_fs(fs).set_high_pt(false).set_do_met_cut(true).set_mode(mode).set_met_label(met_label);
  httOneJetLowPtCategory.set_met_cut(30.0);
  if (era == 0 || era == 1) httOneJetLowPtCategory.set_jet_eta(5.0);


  HttOneBJetCategory httOneBJetHighPtCategory = HttOneBJetCategory
    ("HttBOneJetHighPtCategory","pfJetsPFlow").set_fs(fs).set_high_pt(true).set_mode(mode).set_met_label(met_label);;
  if (special_mode == 9 || special_mode == 10) httOneBJetHighPtCategory.set_apply_special_mode(true).set_special_min(1).set_special_max(99).set_special_pt(20.0).set_special_eta(2.4).set_met_label(met_label);
  if (special_mode == 13) httOneBJetHighPtCategory.set_apply_special_mode(true).set_special_min(1).set_special_max(99).set_special_pt(20.0).set_special_eta(2.4).set_met_label(met_label);

  HttOneBJetCategory httOneBJetLowPtCategory = HttOneBJetCategory
    ("HttBOneJetLowPtCategory","pfJetsPFlow").set_fs(fs).set_high_pt(false).set_mode(mode).set_met_label(met_label);;
  if (special_mode == 9 || special_mode == 10) httOneBJetLowPtCategory.set_apply_special_mode(true).set_special_min(1).set_special_max(99).set_special_pt(20.0).set_special_eta(2.4).set_met_label(met_label);
  if (special_mode == 13) httOneBJetLowPtCategory.set_apply_special_mode(true).set_special_min(1).set_special_max(99).set_special_pt(20.0).set_special_eta(2.4).set_met_label(met_label);

  HttBTagCategory httBTagCategory = HttBTagCategory
    ("HttBTagCategory","pfJetsPFlow").set_fs(fs).set_mode(mode).set_met_label(met_label);
  if (special_mode == 9 || special_mode == 10) httBTagCategory.set_apply_special_mode(true).set_special_min(1).set_special_max(99).set_special_pt(20.0).set_special_eta(2.4).set_met_label(met_label);
  if (special_mode == 13) httBTagCategory.set_apply_special_mode(true).set_special_min(1).set_special_max(99).set_special_pt(20.0).set_special_eta(2.4).set_met_label(met_label);

  HttNoBTagCategory httNoBTagCategory = HttNoBTagCategory
    ("HttNoBTagCategory","pfJetsPFlow").set_fs(fs).set_do_met_cut(false).set_mode(mode).set_met_label(met_label);

  HttZeroJetCategory httZeroJetHighPtCategory = HttZeroJetCategory
    ("HttZeroJetHighPtCategory").set_fs(fs).set_high_pt(true).set_do_met_cut(true).set_mode(mode).set_met_label(met_label);
  httZeroJetHighPtCategory.set_met_cut(0.0);
  if (era == 0 || era == 1) {
    httZeroJetHighPtCategory.set_met_cut(30.0);
    httZeroJetHighPtCategory.set_veto_jet_eta(5.0);
  }

  HttZeroJetCategory httZeroJetLowPtCategory = HttZeroJetCategory
    ("HttZeroJetLowPtCategory").set_fs(fs).set_high_pt(false).set_do_met_cut(true).set_mode(mode).set_met_label(met_label);
  httZeroJetLowPtCategory.set_met_cut(0.0);
  if (era == 0 || era == 1) {
    httZeroJetLowPtCategory.set_met_cut(30.0);
    httZeroJetLowPtCategory.set_veto_jet_eta(5.0);
  }


  if (!is_data && do_btag_weight) {
    httVbfCategory.set_do_btag_weight(true);
    httOneJetHighPtCategory.set_do_btag_weight(true);
    httOneJetLowPtCategory.set_do_btag_weight(true);
    httOneBJetHighPtCategory.set_do_btag_weight(true);
    httOneBJetLowPtCategory.set_do_btag_weight(true);
    httZeroJetHighPtCategory.set_do_btag_weight(true);
    httZeroJetLowPtCategory.set_do_btag_weight(true);
    if (special_mode == 9 || special_mode == 10 || special_mode == 13) {
      httOneBJetHighPtCategory.set_do_btag_weight(false);
      httOneBJetLowPtCategory.set_do_btag_weight(false);
    }
  }

  HttSync httSync("HttSync","SYNCFILE_" + outname, mode);
  httSync.set_is_embedded(is_embedded).set_met_label(met_label);
  if (era == 0 || era == 1) httSync.set_jet_eta(5.0);

  SVFit svfit("SVFit");
  svfit.set_outname(outname).set_op(svfit_op).set_dilepton_label("emtauCandidates").set_met_label(met_label).set_mode(mode);
  svfit.set_fullpath(svfit_folder);
  if (svfit_override != "") {
    svfit.set_outname(svfit_override);
  }

  // ------------------------------------------------------------------------------------
  // Build Analysis Sequence
  // ------------------------------------------------------------------------------------  
  //analysis.AddModule(&httPrint);
  //analysis.AddModule(&wjetsWeights);
  if (is_data && !is_2012 && (mode == 0 || mode == 1)) analysis.AddModule(&runFilter2011);
  if (!is_data && !disable_reweighting) analysis.AddModule(&pileupWeight);
  if (ztatau_mode > 0) analysis.AddModule(&zTauTauFilter);
  if (!is_data && do_mass_filter) analysis.AddModule(&mssmMassFilter);
  if (tau_scale_mode > 0) analysis.AddModule(&tauEnergyShifter);
  if (is_embedded) analysis.AddModule(&embeddedMassFilter);

  if (mode == 0) {
    analysis.AddModule(&selElectronCopyCollection);
    if (is_data  && !do_skim && !is_embedded) analysis.AddModule(&dataTriggerPathFilter);
    if (!is_data  && !do_skim && !disable_mc_trigger) analysis.AddModule(&mcTriggerPathFilter);
    analysis.AddModule(&selElectronPtEtaFilter);
    analysis.AddModule(&selElectronDxyFilter);
    analysis.AddModule(&selElectronDzFilter);
    analysis.AddModule(&selElectronIdIsoFilter);

    analysis.AddModule(&vetoElectronCopyCollection);
    analysis.AddModule(&vetoElectronPtEtaFilter);
    analysis.AddModule(&vetoElectronIdIsoFilter);
    analysis.AddModule(&vetoElectronDzFilter);
    analysis.AddModule(&vetoElectronPairProducer);
    if (special_mode != 18) analysis.AddModule(&vetoElectronPairFilter);


    analysis.AddModule(&tauPtEtaFilter);
    analysis.AddModule(&tauDzFilter);
    analysis.AddModule(&tauIsoFilter);
    analysis.AddModule(&tauElRejectFilter);
    analysis.AddModule(&tauMuRejectFilter);
    analysis.AddModule(&tauElPairProducer);
    analysis.AddModule(&pairFilter);
    if (svfit_op != 1 && special_mode != 18 && era > 1) {
      analysis.AddModule(&extraElectronVeto);
      analysis.AddModule(&extraMuonVeto);
    }
  }

  if (mode == 1) {
    analysis.AddModule(&selMuonCopyCollection);
    if (is_data && !do_skim && !is_embedded) analysis.AddModule(&dataTriggerPathFilter);
    if (!is_data  && !do_skim && !disable_mc_trigger) analysis.AddModule(&mcTriggerPathFilter);
    analysis.AddModule(&selMuonPtEtaFilter);
    analysis.AddModule(&selMuonDxyFilter);
    analysis.AddModule(&selMuonDzFilter);
    analysis.AddModule(&selMuonIdIsoFilter);
    
    analysis.AddModule(&vetoMuonCopyCollection);
    analysis.AddModule(&vetoMuonPtEtaFilter);
    analysis.AddModule(&vetoMuonIdIsoFilter);
    analysis.AddModule(&vetoMuonDzFilter);
    analysis.AddModule(&vetoMuonPairProducer);
    analysis.AddModule(&vetoMuonPairFilter);


    analysis.AddModule(&tauPtEtaFilter);
    analysis.AddModule(&tauDzFilter);
    analysis.AddModule(&tauIsoFilter);
    analysis.AddModule(&tauElRejectFilter);
    analysis.AddModule(&tauMuRejectFilter);
    analysis.AddModule(&tauMuPairProducer);
    analysis.AddModule(&pairFilter);  
    if (svfit_op != 1 && era > 1) {
      analysis.AddModule(&extraElectronVeto);
      analysis.AddModule(&extraMuonVeto);
    }
  }

  if (mode == 2) {
    if (is_embedded && !do_skim && era == 0) analysis.AddModule(&electronEnergyShifter);
    analysis.AddModule(&selElectronCopyCollection);
    analysis.AddModule(&selElectronPtEtaFilter);
    analysis.AddModule(&selElectronDxyFilter);
    analysis.AddModule(&selElectronIdIsoFilter);
    analysis.AddModule(&selElectronDzFilter);
    analysis.AddModule(&elecMuonOverlapFilter);
    analysis.AddModule(&selMuonCopyCollection);
    analysis.AddModule(&selMuonPtEtaFilter);
    analysis.AddModule(&selMuonDxyFilter);
    analysis.AddModule(&selMuonIdIsoFilter);
    analysis.AddModule(&selMuonDzFilter);
    if (is_data && !do_skim && !is_embedded) analysis.AddModule(&dataTriggerPathFilter);
    if (!is_data && !do_skim) analysis.AddModule(&mcTriggerPathFilter);
    analysis.AddModule(&elMuPairProducer);
    analysis.AddModule(&pairFilter);
  }

  if (!do_skim && !scan_trigger) {
    analysis.AddModule(&httPairSelector);
    analysis.AddModule(&jetIDFilter);
    analysis.AddModule(&jetLeptonOverlapFilter);
    if (era > 0) analysis.AddModule(&httRecoilCorrector);
    if (do_svfit && !(svfit_override != "" && svfit_op == 1)) analysis.AddModule(&svfit);
    analysis.AddModule(&httWeights);
    if (do_sync) analysis.AddModule(&httSync);
    analysis.AddModule(&httSelection);
    //analysis.AddModule(&httMetStudy);
    // analysis.AddModule(&jetVtxStudy);
    if (special_mode == 9 || special_mode == 10) {
      analysis.AddModule(&httOneBJetHighPtCategory);
      analysis.AddModule(&httOneBJetLowPtCategory);
    } else {
      analysis.AddModule(&httVbfCategory);
      analysis.AddModule(&httOneJetHighPtCategory);
      analysis.AddModule(&httOneJetLowPtCategory);
      analysis.AddModule(&httOneBJetHighPtCategory);
      analysis.AddModule(&httOneBJetLowPtCategory);
      analysis.AddModule(&httZeroJetHighPtCategory);
      analysis.AddModule(&httZeroJetLowPtCategory);

      analysis.AddModule(&httNoBTagCategory);
      analysis.AddModule(&httBTagCategory);
    }
  }

  if (do_skim && faked_tau_selector > 0) analysis.AddModule(&httPairSelector);

  // Run analysis

  //analysis.AddModule(&fsrStudy);
  analysis.RunAnalysis();
  delete fs;
  return 0;
}




