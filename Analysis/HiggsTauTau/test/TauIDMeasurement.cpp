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
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/CompositeProducer.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/OneCollCompositeProducer.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/PileupWeight.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTPairSelector.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTWeights.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTRecoilCorrector.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/LumiMask.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTEnergyScale.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/TauIDCategories.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/TauIDTriggerFilter.h"

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

  string strategy_str;            // Analysis strategy
  string era_str;                 // Analysis data-taking era
  string mc_str;                  // Analysis MC production
  string channel_str;             // Analysis channel

  bool is_data;                   // true = data, false = mc         
  bool is_embedded;               // true = embedded, false = not an embedded sample
  unsigned special_mode;          // 0 = normal processing, > 0 (see below)
  unsigned tau_scale_mode;        // 0 = no shift, 1 = shift down, 2 = shift up
  unsigned mass_scale_mode;       // 0 = no shift, 1 = nominal, but in TSCALE_DOWN, 2 = shift up, 3 = shift up again, in TSCALE_UP
  unsigned ztautau_mode;          // 0 = not run, 1 = select Z->tautau, 2 = select Z->ee and Z->mumu
  unsigned faked_tau_selector;    // 0 = not run, 1 = tau matched to gen. lepton, 2 = tau not matched to lepton
  unsigned mva_met_mode;          // 0 = standard mva met, 1 = mva met from vector (only when mva met is being used)
  string allowed_tau_modes;       // "" means all, otherwise "1,10"=allow 1prong1pizero,3prong


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
      ("is_embedded",         po::value<bool>(&is_embedded)->default_value(false))
      ("special_mode",        po::value<unsigned>(&special_mode)->default_value(0))
      ("tau_scale_mode",      po::value<unsigned>(&tau_scale_mode)->default_value(0))
      ("mass_scale_mode",     po::value<unsigned>(&mass_scale_mode)->default_value(0))
      ("ztautau_mode",        po::value<unsigned>(&ztautau_mode)->default_value(0))
      ("faked_tau_selector",  po::value<unsigned>(&faked_tau_selector)->default_value(0))
      ("mva_met_mode",        po::value<unsigned>(&mva_met_mode)->default_value(1))
      ("allowed_tau_modes",   po::value<string>(&allowed_tau_modes)->default_value(""));
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
  }
  if (tau_scale_mode == 2) {
    output_folder += "TSCALE_UP/";
  }
  if (mass_scale_mode == 1) {
    output_folder += "TSCALE_DOWN/";
  }
  if (mass_scale_mode == 3) {
    output_folder += "TSCALE_UP/";
  }

  std::cout << "**** Tau ID Measurements *****" << std::endl;
  string param_fmt = "%-25s %-40s\n";
  std::cout << boost::format(param_fmt) % "max_events" % max_events;
  std::cout << boost::format(param_fmt) % "output" % (output_folder+output_name);
  std::cout << boost::format(param_fmt) % "strategy" % strategy_str;
  std::cout << boost::format(param_fmt) % "era" % era_str;
  std::cout << boost::format(param_fmt) % "mc" % mc_str;
  std::cout << boost::format(param_fmt) % "channel" % channel_str;
  std::cout << boost::format(param_fmt) % "is_data" % is_data;
  std::cout << boost::format(param_fmt) % "is_embedded" % is_embedded;
  std::cout << boost::format(param_fmt) % "special_mode" % special_mode;
  std::cout << boost::format(param_fmt) % "tau_scale_mode" % tau_scale_mode;
  std::cout << boost::format(param_fmt) % "mass_scale_mode" % mass_scale_mode;
  std::cout << boost::format(param_fmt) % "ztautau_mode" % ztautau_mode;
  std::cout << boost::format(param_fmt) % "faked_tau_selector" % faked_tau_selector;
  std::cout << boost::format(param_fmt) % "mva_met_mode" % mva_met_mode;
  std::cout << boost::format(param_fmt) % "allowed_tau_modes" % allowed_tau_modes;

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
  std::string met_label;
  double elec_dz, muon_dz, elec_dxy, muon_dxy, tau_dz;
  double elec_pt, elec_eta, muon_pt, muon_eta, tau_pt, tau_eta;
  met_label = "pfMVAMet";

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
    muon_pt = 26.0;
    muon_eta = 2.1;
  }
  tau_pt = 20.0;
  tau_eta = 2.3;

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
    "TauID",              // Analysis name
    files,                // Input files
    "icEventProducer",    // TTree path
    "EventTree",          // TTree name
    max_events);          // Max. events to process (-1 = all)
  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);

  // ------------------------------------------------------------------------------------
  // Misc Modules
  // ------------------------------------------------------------------------------------

  string data_json = "";
  if (era == era::data_2011)              data_json = "data/json/json_data_2011_et_mt.txt";
  if (era == era::data_2012_hcp)          data_json = "data/json/data_2012_hcp.txt";
  if (era == era::data_2012_moriond) {
    if (strategy == strategy::hcp2012 || strategy == strategy::moriond2013) {
                                          data_json = "data/json/data_2012_moriond_no_recovery.txt";
      if (channel == channel::em)         data_json = "data/json/data_2012_moriond_valentina.txt";
    }
    if (strategy == strategy::paper2013)  data_json = "data/json/data_2012_moriond_final.txt";
  }
  if (era == era::data_2012_donly)        data_json = "data/json/data_2012_donly.txt";


  LumiMask lumiMask = LumiMask("LumiMask")
    .set_produce_output_jsons("")
    .set_input_file(data_json);

  string mc_pu_file;
  if (mc == mc::fall11_42X) mc_pu_file    = "data/pileup/MC_Fall11_PU_S6-500bins.root";
  if (mc == mc::summer12_53X) mc_pu_file  = "data/pileup/MC_Summer12_PU_S10-600bins.root";
  string data_pu_file;
  if (era == era::data_2011) data_pu_file     =  "data/pileup/Data_Pileup_2011_HCP-500bins.root";
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

  TauIDTriggerFilter tauIDTriggerFilter = TauIDTriggerFilter("TauIDTriggerFilter")
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


  SimpleCounter<GenParticle> embeddedMassFilter = SimpleCounter<GenParticle>("EmbeddedMassFilter")
    .set_input_label("genParticlesEmbedded")
    .set_predicate(bind(GenParticleInMassBand, _1, 23, 50., 9999999.))
    .set_min(1);
  if (era == era::data_2011) embeddedMassFilter.set_input_label("genParticles");

  // ------------------------------------------------------------------------------------
  // Muon Modules
  // ------------------------------------------------------------------------------------
  CopyCollection<Muon> selMuonCopyCollection("CopyToSelMuons","muonsPFlow","selMuons");

  boost::function<bool (Muon const*)> muon_idiso_func;
  if (special_mode == 2 || special_mode == 5 || special_mode == 12) {
    muon_idiso_func = bind(MuonTight, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.5);
  } else if (special_mode == 3 || special_mode == 6 || special_mode == 10) {
    muon_idiso_func = bind(MuonTight, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5) > 0.2) && (bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.5);
  } else {
    muon_idiso_func = bind(MuonTight, _1) && (bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1);
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
              && (bind(fabs, bind(&Muon::dxy_vertex, _1)) < 0.045)
              && (bind(fabs, bind(&Muon::dz_vertex, _1)) < 0.2))
  .set_min(0).set_max((channel == channel::mt || channel == channel::mtmet || channel == channel::em) ? 1 : 0);                   

  // ------------------------------------------------------------------------------------
  // Tau Modules
  // ------------------------------------------------------------------------------------
  double tau_shift = 1.0;
  if (tau_scale_mode == 1) tau_shift = 0.97;
  if (tau_scale_mode == 2) tau_shift = 1.03;

  HTTEnergyScale httEnergyScale = HTTEnergyScale("HTTEnergyScale")
    .set_input_label("taus")
    .set_shift(tau_shift)
    .set_moriond_corrections(false);
    if ( (output_name.find("DYJetsToTauTau")        != output_name.npos) 
      || (output_name.find("Embedded")              != output_name.npos) ) httEnergyScale.set_moriond_corrections(true);
  

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

  SimpleFilter<Tau> tauElRejectFilter = SimpleFilter<Tau>("TauElRejectFilter")
    .set_predicate(bind(&Tau::GetTauID, _1, "againstElectronLoose") > 0.5)
    .set_input_label("taus").set_min(1); 
                       

  SimpleFilter<Tau> tauMuRejectFilter = SimpleFilter<Tau>("TauMuRejectFilter")
    .set_predicate(bind(&Tau::GetTauID, _1, "againstMuonTight") > 0.5)
    .set_input_label("taus").set_min(1);  
                  
  // ------------------------------------------------------------------------------------
  // e/m + tau Pair Modules
  // ------------------------------------------------------------------------------------  
  CompositeProducer<Muon, Tau> tauMuPairProducer = CompositeProducer<Muon, Tau>
    ("TauMuPairProducer")
    .set_input_label_first("selMuons")
    .set_input_label_second("taus")
    .set_candidate_name_first("lepton1")
    .set_candidate_name_second("lepton2")
    .set_output_label("emtauCandidates");   
                                                   
  SimpleFilter<CompositeCandidate> pairFilter = SimpleFilter<CompositeCandidate>("PairFilter")
    .set_input_label("emtauCandidates")
    .set_predicate( (bind(&CompositeCandidate::DeltaR, _1, "lepton1","lepton2") > 0.5))
    .set_min(1)
    .set_max(999);    
  
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
  HTTPairSelector httPairSelector = HTTPairSelector("HTTPairSelector")
    .set_channel(channel)
    .set_fs(fs)
    .set_met_label(met_label)
    .set_mva_met_from_vector(mva_met_mode == 1)
    .set_faked_tau_selector(faked_tau_selector)
    .set_scale_met_for_tau((tau_scale_mode > 0 || ((is_embedded || !is_data) )   ))
    .set_tau_scale(tau_shift)
    .set_allowed_tau_modes(allowed_tau_modes);

  HTTRecoilCorrector httRecoilCorrector = HTTRecoilCorrector("HTTRecoilCorrector")
    .set_sample(output_name)
    .set_channel(channel)
    .set_mc(mc)
    .set_met_label(met_label)
    .set_strategy(strategy::moriond2013)
    .set_w_hack(false);

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
    .set_do_btag_weight(false);
  if (!is_data) {
    httWeights.set_do_trg_weights(false).set_trg_applied_in_mc(true).set_do_idiso_weights(false);
    httWeights.set_do_btag_weight(true);
  }
  if (output_name.find("DYJetsToLL") != output_name.npos) httWeights.set_do_mtau_fakerate(true);
  if (is_embedded) httWeights.set_do_trg_weights(false).set_trg_applied_in_mc(false).set_do_idiso_weights(false).set_do_id_weights(false);
  if (output_name.find("WJetsToLNuSoup") != output_name.npos) {
    httWeights.set_do_w_soup(true);
    httWeights.SetWTargetFractions(0.743925, 0.175999, 0.0562617, 0.0168926, 0.00692218);
    httWeights.SetWInputYields(76102995.0, 23141598.0, 34044921.0, 15539503.0, 13382803.0);
  }
  if (output_name.find("DYJets") != output_name.npos && output_name.find("Soup") != output_name.npos) {
    httWeights.set_do_dy_soup(true);
    httWeights.SetDYTargetFractions(0.723342373, 0.190169492, 0.061355932, 0.017322034, 0.007810169);
    httWeights.SetDYInputYields(30459503.0, 24045248.0, 21852156.0, 11015445.0, 6402827.0);
  }

  // ------------------------------------------------------------------------------------
  // Category Modules
  // ------------------------------------------------------------------------------------  
  TauIDCategories tauIDCategories = TauIDCategories("TauIDCategories")
    .set_fs(fs)
    .set_channel(channel)
    .set_era(era)
    .set_ditau_label("emtauCandidates")
    .set_met_label(met_label);
  if (mass_scale_mode == 1) tauIDCategories.set_mass_shift(1.00);
  if (mass_scale_mode == 2) tauIDCategories.set_mass_shift(1.01);
  if (mass_scale_mode == 3) tauIDCategories.set_mass_shift(1.02);

  // ------------------------------------------------------------------------------------
  // Build Analysis Sequence
  // ------------------------------------------------------------------------------------  
  if (is_data)                    analysis.AddModule(&lumiMask);
  if (!is_data)                   analysis.AddModule(&pileupWeight);
  if (ztautau_mode > 0)           analysis.AddModule(&zTauTauFilter);
  if (!is_data || is_embedded)    analysis.AddModule(&httEnergyScale);
  if (is_embedded)                analysis.AddModule(&embeddedMassFilter);
                                  analysis.AddModule(&selMuonCopyCollection);
                                  analysis.AddModule(&selMuonFilter);
                                  analysis.AddModule(&vetoMuonCopyCollection);
                                  analysis.AddModule(&vetoMuonFilter);
                                  analysis.AddModule(&vetoMuonPairProducer);
                                  analysis.AddModule(&vetoMuonPairFilter);
                                  analysis.AddModule(&extraMuonVeto);
                                  analysis.AddModule(&tauPtEtaFilter);
                                  analysis.AddModule(&tauDzFilter);
                                  analysis.AddModule(&tauIsoFilter);
    //                             analysis.AddModule(&tauElRejectFilter);
    //                             analysis.AddModule(&tauMuRejectFilter);
                                  analysis.AddModule(&tauMuPairProducer);
                                  analysis.AddModule(&pairFilter);
  if (!is_embedded)               analysis.AddModule(&tauIDTriggerFilter);
                                  analysis.AddModule(&httPairSelector);
                                  analysis.AddModule(&jetIDFilter);
                                  analysis.AddModule(&jetLeptonOverlapFilter);
                                  analysis.AddModule(&httRecoilCorrector);
                                  analysis.AddModule(&httWeights);
                                  analysis.AddModule(&tauIDCategories);
  

  analysis.RunAnalysis();
  delete fs;
  return 0;
}