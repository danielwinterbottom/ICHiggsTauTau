#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include <memory>
// ROOT
#include "TH1.h"
// Objects
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
// Utilities
#include "Utilities/interface/FnRootTools.h"
// HTT-specific modules
#include "HiggsTauTau/interface/HTTSequence.h"
#include "HiggsTauTau/interface/HTTTriggerFilter.h"
#include "HiggsTauTau/interface/HTTEnergyScale.h"
#include "HiggsTauTau/interface/HTTEMuExtras.h"
#include "HiggsTauTau/interface/HTTGenEvent.h"
#include "HiggsTauTau/interface/HTTCategories.h"
#include "HiggsTauTau/interface/HTTPairSelector.h"
#include "HiggsTauTau/interface/SVFitTest.h"
#include "HiggsTauTau/interface/TauCompositeProducer.h"
// Generic modules
#include "Modules/interface/SimpleFilter.h"
#include "Modules/interface/CompositeProducer.h"
#include "Modules/interface/CopyCollection.h"
#include "Modules/interface/OneCollCompositeProducer.h"
#include "Modules/interface/OverlapFilter.h"
#include "Modules/interface/EnergyShifter.h"
#include "Modules/interface/PileupWeight.h"

namespace ic {

double elec_pt,elec_eta,elec_dz,elec_dxy;
double muon_pt,muon_eta,muon_dz,muon_dxy;
double emu_elec_pt,emu_elec_eta,emu_elec_dz,emu_elec_dxy;
double emu_muon_pt,emu_muon_eta,emu_muon_dz,emu_muon_dxy;
double tau_pt, tau_eta, tau_dz, tau_iso;
double ditau_tau_pt, ditau_tau_eta, ditau_tau_dz, ditau_tau_iso;
double veto_elec_pt, veto_elec_eta, veto_elec_dxy, veto_elec_dz;
double veto_muon_pt, veto_muon_eta, veto_muon_dxy, veto_muon_dz;
double veto_dielec_pt, veto_dielec_eta, veto_dielec_dxy, veto_dielec_dz;
double veto_dimuon_pt, veto_dimuon_eta, veto_dimuon_dxy, veto_dimuon_dz;
std::string tau_anti_elec, tau_anti_muon;


void HTTSequence::BuildSequence(ModuleSequence* seq, ic::channel channel,fwlite::TFileService *fs,
                                Json::Value const& js) {
  using ROOT::Math::VectorUtil::DeltaR;

  // Set global parameters that get used in multiple places
  ic::mc mc_type              = String2MC(js["mc"].asString());
  ic::era era_type            = String2Era(js["era"].asString());
   ic::strategy strategy_type  = String2Strategy(js["strategy"].asString());
  // Other flags
  bool is_data        = js["is_data"].asBool();
  bool is_embedded    = js["is_embedded"].asBool();

  elec_pt=23.0;
  elec_eta=2.5;
  elec_dz=0.2;
  elec_dxy=0.045;
  tau_pt=20;
  tau_eta=2.3;
  tau_dz=0.2;
  tau_iso=1.5;
  ditau_tau_pt=45;
  ditau_tau_eta=2.1;
  ditau_tau_dz=0.2;
  ditau_tau_iso=1.0;
  tau_anti_elec="againstElectronTightMVA5";
  tau_anti_muon="againstMuonLoose3";
  muon_pt=18.0;
  muon_eta=2.1;
  muon_dxy=0.045;
  muon_dz=0.02;
  emu_elec_pt=13;
  emu_elec_eta=2.5;
  emu_elec_dxy=0.045;
  emu_elec_dz=0.2;
  emu_muon_pt=9.5;//Otherwise MVAMet won't work because I have a cut at 9.5 there in the ntuples->need to fix for next iteration
  emu_muon_eta=2.4;
  emu_muon_dxy=0.045;
  emu_muon_dz=0.2;
  veto_elec_pt=10;
  veto_elec_eta=2.5;
  veto_elec_dxy=0.045;
  veto_elec_dz=0.2;
  veto_muon_pt=10;
  veto_muon_eta=2.4;
  veto_muon_dxy=0.045;
  veto_muon_dz=0.2;
  veto_dielec_pt=15;
  veto_dielec_eta=2.5;
  veto_dielec_dxy=0.045;
  veto_dielec_dz=0.02;
  veto_dimuon_pt=15;
  veto_dimuon_eta=2.4;
  veto_dimuon_dxy=0.045;
  veto_dimuon_dz=0.2;
  
  


  // If desired, run the HTTGenEventModule which will add some handily-
  // formatted generator-level info into the Event
  if (js["run_gen_info"].asBool()) {
    BuildModule(seq, HTTGenEvent("HTTGenEvent")
        .set_genparticle_label(js["genTaus"].asString())
        .set_genjet_label(js["genJets"].asString()));
  }

  if (channel == channel::et) BuildETPairs(seq, js);
  if (channel == channel::mt) BuildMTPairs(seq, js);
  if (channel == channel::em) BuildEMPairs(seq, js);
  if (channel == channel::tt) BuildTTPairs(seq, js);

  // Pair DeltaR filtering
  BuildModule(seq, SimpleFilter<CompositeCandidate>("PairFilter")
      .set_input_label("ditau").set_min(1)
//      .set_predicate( (bind(&CompositeCandidate::DeltaR, _1, "lepton1","lepton2") > 0.5)));
      .set_predicate([=](CompositeCandidate const* c) {
        return DeltaR(c->at(0)->vector(), c->at(1)->vector())
            > js["baseline"]["pair_dr"].asDouble();
      }));


  // Trigger filtering
  if (js["run_trg_filter"].asBool()) {
    BuildModule(seq, HTTTriggerFilter("HTTTriggerFilter")
        .set_channel(channel)
        .set_mc(mc_type)
        .set_is_data(is_data)
        .set_is_embedded(is_embedded)
        .set_pair_label("ditau"));
  }

  // Lepton Vetoes
  if (js["baseline"]["di_elec_veto"].asBool()) BuildDiElecVeto(seq, js);
  if (js["baseline"]["di_muon_veto"].asBool()) BuildDiMuonVeto(seq, js);
  if (js["baseline"]["extra_elec_veto"].asBool()) BuildExtraElecVeto(seq, js);
  if (js["baseline"]["extra_muon_veto"].asBool()) BuildExtraMuonVeto(seq, js);


  // Pileup Weighting
  TH1D d_pu = GetFromTFile<TH1D>(js["data_pu_file"].asString(), "/", "pileup");
  TH1D m_pu = GetFromTFile<TH1D>(js["mc_pu_file"].asString(), "/", "pileup");
  if (js["do_pu_wt"].asBool()) {
    BuildModule(seq, PileupWeight("PileupWeight")
        .set_data(new TH1D(d_pu)).set_mc(new TH1D(m_pu)));
  }


  std::string met_label;
  met_label = "pfMVAMet";

  std::string jets_label="ak4PFJetsCHS";
//  if(era==era::data_2015) jets_label="ak4PFJetsCHS";
  unsigned new_svfit_mode = 0;
  std::string svfit_folder = "";
  std::string svfit_override = ""; 
  unsigned kinfit_mode = 0;
  bool bjet_regr_correction = false;
  unsigned mva_met_mode = 1;
  unsigned faked_tau_selector = 0;
  unsigned hadronic_tau_selector = 0; 
  unsigned tau_scale_mode = 0;
  bool moriond_tau_scale = 0;
  double tau_shift = 1.0;
  std::string allowed_tau_modes = "";
  
  
  
 


  BuildModule(seq, HTTPairSelector("HTTPairSelector")
    .set_channel(channel)
    .set_fs(fs)
    .set_pair_label("ditau")
    .set_met_label(met_label)
    .set_mva_met_from_vector(mva_met_mode == 1)
    .set_use_most_isolated(channel==channel::tt ? true : false)
    .set_faked_tau_selector(faked_tau_selector)
    .set_hadronic_tau_selector(hadronic_tau_selector)
    .set_gen_taus_label(is_embedded ? "genParticlesEmbedded" : "genParticlesTaus")
    .set_scale_met_for_tau((tau_scale_mode > 0 || (moriond_tau_scale && (is_embedded || !is_data) )   ))
    .set_tau_scale(tau_shift)
    .set_allowed_tau_modes(allowed_tau_modes));


  
  unsigned pu_id_training = 1;
  
    
  //if (channel == channel::em) httPairSelector.set_tau_scale(elec_shift);
  BuildModule(seq, SimpleFilter<PFJet>("JetIDFilter")
    .set_input_label(jets_label)
    .set_predicate((bind(PFJetIDNoHFCut, _1)) && bind(PileupJetID, _1, pu_id_training)));

  BuildModule(seq, CopyCollection<PFJet>("CopyFilteredJets",jets_label,"pfJetsPFlowFiltered"));

  BuildModule(seq, OverlapFilter<PFJet, CompositeCandidate>("JetLeptonOverlapFilter")
    .set_input_label(jets_label)
    .set_reference_label("ditau")
    .set_min_dr(0.5));

std::string output_name="";


  BuildModule(seq, SVFitTest("SVFitTest")
    .set_channel(channel)
    .set_outname(svfit_override == "" ? output_name : svfit_override)
    .set_run_mode(new_svfit_mode)
    .set_fail_mode(1)
    .set_require_inputs_match(false)
    .set_split(7000)
    .set_dilepton_label("ditau")
    .set_met_label(met_label)
    .set_fullpath(svfit_folder)
    .set_MC(true));

 double massshf=1.00;


  BuildModule(seq, HTTCategories("HTTCategories")
    .set_fs(fs)
    .set_channel(channel)
    .set_era(era_type)
    .set_strategy(strategy_type)
    .set_ditau_label("ditau")
    .set_met_label(met_label)
    .set_jets_label(jets_label)
    .set_kinfit_mode(kinfit_mode)
    .set_bjet_regression(bjet_regr_correction)
    .set_mass_shift(massshf)
    .set_write_tree(true));

/*  if (mass_scale_mode == 1) HTTCategories.set_mass_shift(1.00);
  if (mass_scale_mode == 2) HTTCategories.set_mass_shift(1.01);
  if (mass_scale_mode == 3) HTTCategories.set_mass_shift(1.02);
  if (era == era::data_2012_rereco && strategy == strategy::paper2013) {
    if (mass_scale_mode == 1) HTTCategories.set_mass_shift(1.00);
    if (mass_scale_mode == 2) HTTCategories.set_mass_shift(1.01);
    if (mass_scale_mode == 3) HTTCategories.set_mass_shift(1.02);
  }
*/


}

// --------------------------------------------------------------------------
// TT Pair Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildTTPairs(ModuleSequence* seq, Json::Value const& js){


  Json::Value base = js["baseline"];

//BuildModule(seq, CopyCollection<Tau>("CopyToSelectedTaus",
 //     js["taus"].asString(), "taus"));


BuildModule(seq, HTTEnergyScale("TauEnergyScaleCorrection")
    .set_input_label(js["taus"].asString())
    .set_shift(base["tau_es_shift"].asDouble())
    .set_strategy(strategy::paper2013)
    .set_moriond_corrections(base["tau_es_corr"].asBool()));

BuildModule(seq, SimpleFilter<Tau>("TauFilter")
    .set_input_label(js["taus"].asString()).set_min(2)
    .set_predicate([=](Tau const* t) {
      return  t->pt()                     >  ditau_tau_pt         &&
	      fabs(t->eta())              <  ditau_tau_eta        &&
	      fabs(t->lead_dz_vertex())   <  ditau_tau_dz        &&
	      t->GetTauID("decayModeFinding") > 0.5;
    }));

if (base["lep_iso"].asBool()) {
  BuildModule(seq, SimpleFilter<Tau>("TauIsoFilter")
      .set_input_label(js["taus"].asString()).set_min(2)
      .set_predicate([=](Tau const* t) {
	return t->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < ditau_tau_iso;
      }));
}

if (base["do_tau_anti_elec"].asBool()) {
  BuildModule(seq, SimpleFilter<Tau>("TauAntiElecFilter")
      .set_input_label(js["taus"].asString()).set_min(2)
      .set_predicate([=](Tau const* t) {
	return t->GetTauID(tau_anti_elec) > 0.5;
	//return t->GetTauID(base["tau_anti_elec"].asString()) > 0.5;
      }));
}

if (base["do_tau_anti_muon"].asBool()) {
  BuildModule(seq, SimpleFilter<Tau>("TauAntiMuonFilter")
      .set_input_label(js["taus"].asString()).set_min(2)
      .set_predicate([=](Tau const* t) {
	return t->GetTauID(tau_anti_muon) > 0.5;
	//return t->GetTauID(base["tau_anti_muon"].asString()) > 0.5;
	// TauEoverP(t, 0.2) needed in legacy
      }));
}



  BuildModule(seq, OneCollCompositeProducer<Tau>("TTPairProducer")
      .set_input_label(js["taus"].asString()).set_output_label("ditau")
      .set_candidate_name_first("lepton1").set_candidate_name_second("lepton2"));
 }

  



// --------------------------------------------------------------------------
// ET Pair Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildETPairs(ModuleSequence* seq, Json::Value const& js) {
  BuildModule(seq, CopyCollection<Electron>("CopyToSelectedElectrons",
      js["electrons"].asString(), "sel_electrons"));

  std::function<bool(Electron const*)> ElecID;
  std::string id_fn = js["baseline"]["elec_id"].asString();
  if (id_fn == "MVA:Loose") {
    ElecID = [](Electron const* e) { return ElectronHTTId(e, true); };
  } else if (id_fn == "MVA:Tight") {
    ElecID = [](Electron const* e) { return ElectronHTTId(e, false); };
  } else if (id_fn == "CutBased") {
    ElecID = [](Electron const* e) { return ElectronZbbID(e); };
  }

  BuildModule(seq, SimpleFilter<Electron>("ElectronFilter")
      .set_input_label("sel_electrons").set_min(1)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > elec_pt    &&
                fabs(e->eta())          < elec_eta   &&
                fabs(e->dxy_vertex())   < elec_dxy   &&
                fabs(e->dz_vertex())    < elec_dz    &&
                ElecID(e);
      }));

  if (js["baseline"]["lep_iso"].asBool()) {
    BuildModule(seq, SimpleFilter<Electron>("ElectronIsoFilter")
        .set_input_label("sel_electrons").set_min(1)
        .set_predicate([=](Electron const* e) {
          return PF03IsolationVal(e, 0.5) < 0.1;
        }));
  }


  BuildTauSelection(seq, js);

  BuildModule(seq, CompositeProducer<Electron, Tau>("ETPairProducer")
      .set_input_label_first("sel_electrons")
      .set_input_label_second(js["taus"].asString())
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));
}

// --------------------------------------------------------------------------
// MT Pair Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildMTPairs(ModuleSequence* seq, Json::Value const& js) {
  BuildModule(seq, CopyCollection<Muon>("CopyToSelectedMuons",
      js["muons"].asString(), "sel_muons"));

  BuildModule(seq, SimpleFilter<Muon>("MuonFilter")
      .set_input_label("sel_muons").set_min(1)
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > muon_pt    &&
                fabs(m->eta())          < muon_eta   &&
                fabs(m->dxy_vertex())   < muon_dxy   &&
                fabs(m->dz_vertex())    < muon_dz    &&
                MuonMedium(m);
      }));

  if (js["baseline"]["lep_iso"].asBool()) {
    BuildModule(seq, SimpleFilter<Muon>("MuonIsoFilter")
        .set_input_label("sel_muons").set_min(1)
        .set_predicate([=](Muon const* m) {
          return PF03IsolationVal(m, 0.5) < 0.1;
        }));
  }

  BuildTauSelection(seq, js);

  BuildModule(seq, CompositeProducer<Muon, Tau>("MTPairProducer")
      .set_input_label_first("sel_muons")
      .set_input_label_second(js["taus"].asString())
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));
}

// --------------------------------------------------------------------------
// EM Pair Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildEMPairs(ModuleSequence* seq, Json::Value const& js) {
  BuildModule(seq, EnergyShifter<Electron>("ElectronEnergyScaleCorrection")
      .set_input_label(js["electrons"].asString())
      .set_shift(js["baseline"]["elec_es_shift"].asDouble()));

  if (js["baseline"]["do_em_extras"].asBool()) {
    BuildModule(seq, HTTEMuExtras("EMExtras"));
  }

  BuildModule(seq, CopyCollection<Electron>("CopyToSelectedElectrons",
      js["electrons"].asString(), "sel_electrons"));

  BuildModule(seq, CopyCollection<Muon>("CopyToSelectedMuons",
      js["muons"].asString(), "sel_muons"));


  std::function<bool(Electron const*)> ElecID;
  std::string id_fn = js["baseline"]["elec_id"].asString();
  if (id_fn == "MVA:Loose") {
    ElecID = [](Electron const* e) { return ElectronHTTId(e, true); };
  } else if (id_fn == "MVA:Tight") {
    ElecID = [](Electron const* e) { return ElectronHTTId(e, false); };
  } else if (id_fn == "CutBased") {
    ElecID = [](Electron const* e) { return ElectronZbbID(e); };
  }


  BuildModule(seq, SimpleFilter<Electron>("ElectronFilter")
      .set_input_label("sel_electrons").set_min(1)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > emu_elec_pt    &&
                fabs(e->eta())          < emu_elec_eta   &&
                fabs(e->dxy_vertex())   < emu_elec_dxy   &&
                fabs(e->dz_vertex())    < emu_elec_dz    &&
                ElecID(e);
      }));

  if (js["baseline"]["lep_iso"].asBool()) {
    BuildModule(seq, SimpleFilter<Electron>("ElectronIsoFilter")
        .set_input_label("sel_electrons").set_min(1)
        .set_predicate([=](Electron const* e) {
          return PF03IsolationVal(e, 0.5)<0.15;
          //return PF04IsolationEBElec(e, 0.5, 0.15, 0.1);
        }));
  }

  BuildModule(seq, OverlapFilter<Electron, Muon>("ElecMuonOverlapFilter")
      .set_input_label("sel_electrons")
      .set_reference_label(js["muons"].asString())
      .set_min_dr(0.3));

  BuildModule(seq, SimpleFilter<Muon>("MuonFilter")
      .set_input_label("sel_muons").set_min(1)
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > emu_muon_pt    &&
                fabs(m->eta())          < emu_muon_eta   &&
                fabs(m->dxy_vertex())   < emu_muon_dxy   &&
                fabs(m->dz_vertex())    < emu_muon_dz    &&
                MuonMedium(m);
      }));

  if (js["baseline"]["lep_iso"].asBool()) {
    BuildModule(seq, SimpleFilter<Muon>("MuonIsoFilter")
        .set_input_label("sel_muons").set_min(1)
        .set_predicate([=](Muon const* m) {
          return PF03IsolationVal(m, 0.5)<0.15;
          //return PF04IsolationEB(m, 0.5, 0.15, 0.1);
        }));
  }

  BuildModule(seq, CompositeProducer<Electron, Muon>("EMPairProducer")
      .set_input_label_first("sel_electrons")
      .set_input_label_second("sel_muons")
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));

  BuildModule(seq, SimpleFilter<CompositeCandidate>("EMPairFilter")
      .set_input_label("ditau").set_min(1)
      .set_predicate([=](CompositeCandidate const* c) {
        return PairOneWithPt(c, 20.0);
      }));
}

// --------------------------------------------------------------------------
// Tau Selection Sequence
// --------------------------------------------------------------------------
void HTTSequence::BuildTauSelection(ModuleSequence* seq,
                                    Json::Value const& js) {
  Json::Value base = js["baseline"];

  BuildModule(seq, HTTEnergyScale("TauEnergyScaleCorrection")
      .set_input_label("taus")
      .set_shift(base["tau_es_shift"].asDouble())
      .set_strategy(strategy::paper2013)
      .set_moriond_corrections(base["tau_es_corr"].asBool()));

  BuildModule(seq, SimpleFilter<Tau>("TauFilter")
      .set_input_label(js["taus"].asString()).set_min(1)
      .set_predicate([=](Tau const* t) {
        return  t->pt()                     >  tau_pt     &&
                fabs(t->eta())              <  tau_eta    &&
                fabs(t->lead_dz_vertex())   <  tau_dz     &&
                t->GetTauID("decayModeFinding") > 0.5;
      }));

  if (base["lep_iso"].asBool()) {
    BuildModule(seq, SimpleFilter<Tau>("TauIsoFilter")
        .set_input_label(js["taus"].asString()).set_min(1)
        .set_predicate([=](Tau const* t) {
          return t->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < tau_iso;
        }));
  }

  if (base["do_tau_anti_elec"].asBool()) {
    BuildModule(seq, SimpleFilter<Tau>("TauAntiElecFilter")
        .set_input_label(js["taus"].asString()).set_min(1)
        .set_predicate([=](Tau const* t) {
          return t->GetTauID(tau_anti_elec) > 0.5;
          //return t->GetTauID(base["tau_anti_elec"].asString()) > 0.5;
        }));
  }

  if (base["do_tau_anti_muon"].asBool()) {
    BuildModule(seq, SimpleFilter<Tau>("TauAntiMuonFilter")
        .set_input_label(js["taus"].asString()).set_min(1)
        .set_predicate([=](Tau const* t) {
          return t->GetTauID(tau_anti_muon) > 0.5;
          //return t->GetTauID(base["tau_anti_muon"].asString()) > 0.5;
          // TauEoverP(t, 0.2) needed in legacy
        }));
  }
}

void HTTSequence::BuildDiElecVeto(ModuleSequence* seq, Json::Value const& js) {
  BuildModule(seq, CopyCollection<Electron>("CopyToVetoElecs",
      js["electrons"].asString(), "veto_elecs"));

  BuildModule(seq, SimpleFilter<Electron>("VetoElecFilter")
      .set_input_label("veto_elecs")
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > veto_dielec_pt    &&
                fabs(e->eta())          < veto_dielec_eta   &&
                fabs(e->dxy_vertex())   < veto_dielec_dxy   &&
                fabs(e->dz_vertex())    < veto_dielec_dz    &&
    //Probably need to use different filter, but just for now until something is defined:
                VetoElectronIDPhys14(e)           &&
                PF03IsolationVal(e, 0.5) < 0.3;
      }));

  BuildModule(seq, OneCollCompositeProducer<Electron>("VetoElecPairProducer")
      .set_input_label("veto_elecs").set_output_label("elec_veto_pairs")
      .set_candidate_name_first("elec1").set_candidate_name_second("elec2"));

  BuildModule(seq, SimpleFilter<CompositeCandidate>("VetoElecPairFilter")
      .set_input_label("elec_veto_pairs").set_min(0).set_max(0)
      .set_predicate([=](CompositeCandidate const* c) {
        return  c->DeltaR("elec1", "elec2") > 0.15 &&
                c->charge() == 0;
      }));
}

void HTTSequence::BuildDiMuonVeto(ModuleSequence* seq, Json::Value const& js) {
  BuildModule(seq, CopyCollection<Muon>("CopyToVetoMuons",
      js["muons"].asString(), "veto_muons"));

  BuildModule(seq, SimpleFilter<Muon>("VetoMuonFilter")
      .set_input_label("veto_muons")
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > veto_dimuon_pt    &&
                fabs(m->eta())          < veto_dimuon_eta   &&
                fabs(m->dxy_vertex())   < veto_dimuon_dxy   &&
                fabs(m->dz_vertex())    < veto_dimuon_dz    &&
                m->is_global()                    &&
                m->is_tracker()                   &&
                PF03IsolationVal(m, 0.5) < 0.3;
      }));

  BuildModule(seq, OneCollCompositeProducer<Muon>("VetoMuonPairProducer")
      .set_input_label("veto_muons").set_output_label("muon_veto_pairs")
      .set_candidate_name_first("muon1").set_candidate_name_second("muon2"));

  BuildModule(seq, SimpleFilter<CompositeCandidate>("VetoMuonPairFilter")
      .set_input_label("muon_veto_pairs").set_min(0).set_max(0)
      .set_predicate([=](CompositeCandidate const* c) {
        return  c->DeltaR("muon1", "muon2") > 0.15 &&
                c->charge() == 0;
      }));
}

void HTTSequence::BuildExtraElecVeto(ModuleSequence* seq,
                                     Json::Value const& js) {
  BuildModule(seq, CopyCollection<Electron>("CopyToExtraElecs",
      js["electrons"].asString(), "extra_elecs"));

  BuildModule(seq, SimpleFilter<Electron>("ExtraElecFilter")
      .set_input_label("extra_elecs")
      .set_min(0).set_max(js["baseline"]["max_extra_elecs"].asUInt())
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > veto_elec_pt    &&
                fabs(e->eta())          < veto_elec_eta   &&
                fabs(e->dxy_vertex())   < veto_elec_dxy   &&
                fabs(e->dz_vertex())    < veto_elec_dz    &&
                ElectronHTTIdPhys14(e,true)             &&
                /*ElectronHTTId(e, true)            &&*/
                PF03IsolationVal(e, 0.5) < 0.3;
      }));
}

void HTTSequence::BuildExtraMuonVeto(ModuleSequence* seq,
                                     Json::Value const& js) {
  BuildModule(seq, CopyCollection<Muon>("CopyToExtraMuons",
      js["muons"].asString(), "extra_muons"));

  BuildModule(seq, SimpleFilter<Muon>("ExtraMuonFilter")
      .set_input_label("extra_muons")
      .set_min(0).set_max(js["baseline"]["max_extra_muons"].asUInt())
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > veto_muon_pt    &&
                fabs(m->eta())          < veto_muon_eta   &&
                fabs(m->dxy_vertex())   < veto_muon_dxy   &&
                fabs(m->dz_vertex())    < veto_muon_dz    &&
                MuonMedium(m)                     &&
                PF03IsolationVal(m, 0.5) < 0.3;
      }));
}



}
