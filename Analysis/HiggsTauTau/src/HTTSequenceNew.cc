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
#include "HiggsTauTau/interface/HTTSequenceNew.h"
#include "HiggsTauTau/interface/HTTTriggerFilter.h"
#include "HiggsTauTau/interface/HTTEnergyScale.h"
#include "HiggsTauTau/interface/HTTEMuExtras.h"
#include "HiggsTauTau/interface/HTTGenEvent.h"
#include "HiggsTauTau/interface/HTTCategories.h"
#include "HiggsTauTau/interface/HTTPairSelector.h"
#include "HiggsTauTau/interface/SVFitTest.h"
// Generic modules
#include "Modules/interface/SimpleFilter.h"
#include "Modules/interface/CompositeProducer.h"
#include "Modules/interface/CopyCollection.h"
#include "Modules/interface/OneCollCompositeProducer.h"
#include "Modules/interface/OverlapFilter.h"
#include "Modules/interface/EnergyShifter.h"
#include "Modules/interface/PileupWeight.h"

namespace ic {

HTTSequenceNew::HTTSequenceNew(std::string& chan, std::string& var, Json::Value const& json){
 fs = new fwlite::TFileService((chan+"_"+var+".root").c_str()); 
 js = json;
 channel_str = chan;
 }

HTTSequenceNew::~HTTSequenceNew(){
 delete fs;
 }
 

void HTTSequenceNew::BuildSequence(){
  using ROOT::Math::VectorUtil::DeltaR;


  // Set global parameters that get used in multiple places
  ic::mc mc_type              = String2MC(js["mc"].asString());
  ic::era era_type            = String2Era(js["era"].asString());
  ic::strategy strategy_type  = String2Strategy(js["strategy"].asString());
  ic::channel channel         = String2Channel(channel_str);
  // Other flags
  bool is_data        = js["is_data"].asBool();
  bool is_embedded    = js["is_embedded"].asBool();

  // If desired, run the HTTGenEventModule which will add some handily-
  // formatted generator-level info into the Event
  if (js["run_gen_info"].asBool()) {
    BuildModule(HTTGenEvent("HTTGenEvent")
        .set_genparticle_label(js["genTaus"].asString())
        .set_genjet_label(js["genJets"].asString()));
  }

  if (channel == channel::et) BuildETPairs();
  if (channel == channel::mt) BuildMTPairs();
  if (channel == channel::em) BuildEMPairs();

  // Pair DeltaR filtering
  BuildModule(SimpleFilter<CompositeCandidate>("PairFilter")
      .set_input_label("ditau").set_min(1)
      .set_predicate([=](CompositeCandidate const* c) {
        return DeltaR(c->at(0)->vector(), c->at(1)->vector())
            > js["baseline"]["pair_dr"].asDouble();
      }));

  // Trigger filtering
  if (js["run_trg_filter"].asBool()) {
    BuildModule(HTTTriggerFilter("HTTTriggerFilter")
        .set_channel(channel)
        .set_mc(mc_type)
        .set_is_data(is_data)
        .set_is_embedded(is_embedded)
        .set_pair_label("ditau"));
  }

  // Lepton Vetoes
  if (js["baseline"]["di_elec_veto"].asBool()) BuildDiElecVeto();
  if (js["baseline"]["di_muon_veto"].asBool()) BuildDiMuonVeto();
  if (js["baseline"]["extra_elec_veto"].asBool()) BuildExtraElecVeto();
  if (js["baseline"]["extra_muon_veto"].asBool()) BuildExtraMuonVeto();

  // Pileup Weighting
  TH1D d_pu = GetFromTFile<TH1D>(js["data_pu_file"].asString(), "/", "pileup");
  TH1D m_pu = GetFromTFile<TH1D>(js["mc_pu_file"].asString(), "/", "pileup");
  if (js["do_pu_wt"].asBool()) {
    BuildModule(PileupWeight("PileupWeight")
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
  
  
  
 


  BuildModule(HTTPairSelector("HTTPairSelector")
    .set_channel(channel)
    .set_fs(fs)
    .set_pair_label("ditau")
    .set_met_label(met_label)
    .set_mva_met_from_vector(mva_met_mode == 1)
    .set_faked_tau_selector(faked_tau_selector)
    .set_hadronic_tau_selector(hadronic_tau_selector)
    .set_gen_taus_label(is_embedded ? "genParticlesEmbedded" : "genParticlesTaus")
    .set_scale_met_for_tau((tau_scale_mode > 0 || (moriond_tau_scale && (is_embedded || !is_data) )   ))
    .set_tau_scale(tau_shift)
    .set_allowed_tau_modes(allowed_tau_modes));


  
  unsigned pu_id_training = 1;
  
    
  //if (channel == channel::em) httPairSelector.set_tau_scale(elec_shift);
  BuildModule(SimpleFilter<PFJet>("JetIDFilter")
    .set_input_label(jets_label)
    .set_predicate((bind(PFJetIDNoHFCut, _1)) && bind(PileupJetID, _1, pu_id_training)));

  BuildModule(CopyCollection<PFJet>("CopyFilteredJets",jets_label,"pfJetsPFlowFiltered"));

  BuildModule(OverlapFilter<PFJet, CompositeCandidate>("JetLeptonOverlapFilter")
    .set_input_label(jets_label)
    .set_reference_label("ditau")
    .set_min_dr(0.5));

std::string output_name="";


  BuildModule(SVFitTest("SVFitTest")
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


  BuildModule(HTTCategories("HTTCategories")
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
// ET Pair Sequence
// --------------------------------------------------------------------------
void HTTSequenceNew::BuildETPairs() {
  BuildModule(CopyCollection<Electron>("CopyToSelectedElectrons",
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

  BuildModule(SimpleFilter<Electron>("ElectronFilter")
      .set_input_label("sel_electrons").set_min(1)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > 24.0    &&
                fabs(e->eta())          < 2.1     &&
                fabs(e->dxy_vertex())   < 0.045   &&
                fabs(e->dz_vertex())    < 0.2     &&
                ElecID(e);
      }));

  if (js["baseline"]["lep_iso"].asBool()) {
    BuildModule(SimpleFilter<Electron>("ElectronIsoFilter")
        .set_input_label("sel_electrons").set_min(1)
        .set_predicate([=](Electron const* e) {
          return PF04IsolationVal(e, 0.5) < 0.1;
        }));
  }

  BuildTauSelection();

  BuildModule(CompositeProducer<Electron, Tau>("ETPairProducer")
      .set_input_label_first("sel_electrons")
      .set_input_label_second(js["taus"].asString())
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));
}

// --------------------------------------------------------------------------
// MT Pair Sequence
// --------------------------------------------------------------------------
void HTTSequenceNew::BuildMTPairs() {
  BuildModule(CopyCollection<Muon>("CopyToSelectedMuons",
      js["muons"].asString(), "sel_muons"));

  BuildModule(SimpleFilter<Muon>("MuonFilter")
      .set_input_label("sel_muons").set_min(1)
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > 20.0    &&
                fabs(m->eta())          < 2.1     &&
                fabs(m->dxy_vertex())   < 0.045   &&
                fabs(m->dz_vertex())    < 0.2     &&
                MuonTight(m);
      }));

  if (js["baseline"]["lep_iso"].asBool()) {
    BuildModule(SimpleFilter<Muon>("MuonIsoFilter")
        .set_input_label("sel_muons").set_min(1)
        .set_predicate([=](Muon const* m) {
          return PF04IsolationVal(m, 0.5) < 0.1;
        }));
  }

  BuildTauSelection();

  BuildModule(CompositeProducer<Muon, Tau>("MTPairProducer")
      .set_input_label_first("sel_muons")
      .set_input_label_second(js["taus"].asString())
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));
}

// --------------------------------------------------------------------------
// EM Pair Sequence
// --------------------------------------------------------------------------
void HTTSequenceNew::BuildEMPairs() {
  BuildModule(EnergyShifter<Electron>("ElectronEnergyScaleCorrection")
      .set_input_label(js["electrons"].asString())
      .set_shift(js["baseline"]["elec_es_shift"].asDouble()));

  if (js["baseline"]["do_em_extras"].asBool()) {
    BuildModule(HTTEMuExtras("EMExtras"));
  }

  BuildModule(CopyCollection<Electron>("CopyToSelectedElectrons",
      js["electrons"].asString(), "sel_electrons"));

  BuildModule(CopyCollection<Muon>("CopyToSelectedMuons",
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


  BuildModule(SimpleFilter<Electron>("ElectronFilter")
      .set_input_label("sel_electrons").set_min(1)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > 10.0    &&
                fabs(e->eta())          < 2.3     &&
                fabs(e->dxy_vertex())   < 0.02    &&
                fabs(e->dz_vertex())    < 0.1     &&
                ElecID(e);
      }));

  if (js["baseline"]["lep_iso"].asBool()) {
    BuildModule(SimpleFilter<Electron>("ElectronIsoFilter")
        .set_input_label("sel_electrons").set_min(1)
        .set_predicate([=](Electron const* e) {
          return PF04IsolationEBElec(e, 0.5, 0.15, 0.1);
        }));
  }

  BuildModule(OverlapFilter<Electron, Muon>("ElecMuonOverlapFilter")
      .set_input_label("sel_electrons")
      .set_reference_label(js["muons"].asString())
      .set_min_dr(0.3));

  BuildModule(SimpleFilter<Muon>("MuonFilter")
      .set_input_label("sel_muons").set_min(1)
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > 10.0    &&
                fabs(m->eta())          < 2.1     &&
                fabs(m->dxy_vertex())   < 0.02    &&
                fabs(m->dz_vertex())    < 0.1     &&
                MuonTight(m);
      }));

  if (js["baseline"]["lep_iso"].asBool()) {
    BuildModule(SimpleFilter<Muon>("MuonIsoFilter")
        .set_input_label("sel_muons").set_min(1)
        .set_predicate([=](Muon const* m) {
          return PF04IsolationEB(m, 0.5, 0.15, 0.1);
        }));
  }

  BuildModule(CompositeProducer<Electron, Muon>("EMPairProducer")
      .set_input_label_first("sel_electrons")
      .set_input_label_second("sel_muons")
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));

  BuildModule(SimpleFilter<CompositeCandidate>("EMPairFilter")
      .set_input_label("ditau").set_min(1)
      .set_predicate([=](CompositeCandidate const* c) {
        return PairOneWithPt(c, 20.0);
      }));
}

// --------------------------------------------------------------------------
// Tau Selection Sequence
// --------------------------------------------------------------------------
void HTTSequenceNew::BuildTauSelection(){
  Json::Value base = js["baseline"];

  BuildModule(HTTEnergyScale("TauEnergyScaleCorrection")
      .set_input_label("taus")
      .set_shift(base["tau_es_shift"].asDouble())
      .set_strategy(strategy::paper2013)
      .set_moriond_corrections(base["tau_es_corr"].asBool()));

  BuildModule(SimpleFilter<Tau>("TauFilter")
      .set_input_label(js["taus"].asString()).set_min(1)
      .set_predicate([=](Tau const* t) {
        return  t->pt()                     > 20.0      &&
                fabs(t->eta())              <  2.3      &&
                fabs(t->lead_dz_vertex())   <  0.2      &&
                t->GetTauID("decayModeFinding") > 0.5;
      }));

  if (base["lep_iso"].asBool()) {
    BuildModule(SimpleFilter<Tau>("TauIsoFilter")
        .set_input_label(js["taus"].asString()).set_min(1)
        .set_predicate([=](Tau const* t) {
          return t->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 1.5;
        }));
  }

  if (base["do_tau_anti_elec"].asBool()) {
    BuildModule(SimpleFilter<Tau>("TauAntiElecFilter")
        .set_input_label(js["taus"].asString()).set_min(1)
        .set_predicate([=](Tau const* t) {
          return t->GetTauID(base["tau_anti_elec"].asString()) > 0.5;
        }));
  }

  if (base["do_tau_anti_muon"].asBool()) {
    BuildModule(SimpleFilter<Tau>("TauAntiMuonFilter")
        .set_input_label(js["taus"].asString()).set_min(1)
        .set_predicate([=](Tau const* t) {
          return t->GetTauID(base["tau_anti_muon"].asString()) > 0.5;
          // TauEoverP(t, 0.2) needed in legacy
        }));
  }
}

void HTTSequenceNew::BuildDiElecVeto() {
  BuildModule(CopyCollection<Electron>("CopyToVetoElecs",
      js["electrons"].asString(), "veto_elecs"));

  BuildModule(SimpleFilter<Electron>("VetoElecFilter")
      .set_input_label("veto_elecs")
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > 15.0    &&
                fabs(e->eta())          < 2.5     &&
                fabs(e->dxy_vertex())   < 0.045   &&
                fabs(e->dz_vertex())    < 0.2     &&
                Electron2011WP95ID(e)             &&
                PF04IsolationVal(e, 0.5) < 0.3;
      }));

  BuildModule(OneCollCompositeProducer<Electron>("VetoElecPairProducer")
      .set_input_label("veto_elecs").set_output_label("elec_veto_pairs")
      .set_candidate_name_first("elec1").set_candidate_name_second("elec2"));

  BuildModule(SimpleFilter<CompositeCandidate>("VetoElecPairFilter")
      .set_input_label("elec_veto_pairs").set_min(0).set_max(0)
      .set_predicate([=](CompositeCandidate const* c) {
        return  c->DeltaR("elec1", "elec2") > 0.15 &&
                c->charge() == 0;
      }));
}

void HTTSequenceNew::BuildDiMuonVeto() {
  BuildModule(CopyCollection<Muon>("CopyToVetoMuons",
      js["muons"].asString(), "veto_muons"));

  BuildModule(SimpleFilter<Muon>("VetoMuonFilter")
      .set_input_label("veto_muons")
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > 15.0    &&
                fabs(m->eta())          < 2.4     &&
                fabs(m->dxy_vertex())   < 0.045   &&
                fabs(m->dz_vertex())    < 0.2     &&
                m->is_global()                    &&
                m->is_tracker()                   &&
                PF04IsolationVal(m, 0.5) < 0.3;
      }));

  BuildModule(OneCollCompositeProducer<Muon>("VetoMuonPairProducer")
      .set_input_label("veto_muons").set_output_label("muon_veto_pairs")
      .set_candidate_name_first("muon1").set_candidate_name_second("muon2"));

  BuildModule(SimpleFilter<CompositeCandidate>("VetoMuonPairFilter")
      .set_input_label("muon_veto_pairs").set_min(0).set_max(0)
      .set_predicate([=](CompositeCandidate const* c) {
        return  c->DeltaR("muon1", "muon2") > 0.15 &&
                c->charge() == 0;
      }));
}

void HTTSequenceNew::BuildExtraElecVeto(){
  BuildModule(CopyCollection<Electron>("CopyToExtraElecs",
      js["electrons"].asString(), "extra_elecs"));

  BuildModule(SimpleFilter<Electron>("ExtraElecFilter")
      .set_input_label("extra_elecs")
      .set_min(0).set_max(js["max_extra_elecs"].asUInt())
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > 10.0    &&
                fabs(e->eta())          < 2.5     &&
                fabs(e->dxy_vertex())   < 0.045   &&
                fabs(e->dz_vertex())    < 0.2     &&
                /*ElectronHTTId(e, true)            &&*/
                PF04IsolationVal(e, 0.5) < 0.3;
      }));
}

void HTTSequenceNew::BuildExtraMuonVeto(){
  BuildModule(CopyCollection<Muon>("CopyToExtraMuons",
      js["muons"].asString(), "extra_muons"));

  BuildModule(SimpleFilter<Muon>("ExtraMuonFilter")
      .set_input_label("extra_muons")
      .set_min(0).set_max(js["max_extra_muons"].asUInt())
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > 10.0    &&
                fabs(m->eta())          < 2.4     &&
                fabs(m->dxy_vertex())   < 0.045   &&
                fabs(m->dz_vertex())    < 0.2     &&
                MuonTight(m)                      &&
                PF04IsolationVal(m, 0.5) < 0.3;
      }));
}



}
