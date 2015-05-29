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
#include "HiggsTauTau/interface/HTTSequenceCrash.h"
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

HTTSequenceCrash::HTTSequenceCrash(std::string& chan, std::string& var,
                                   Json::Value const& json) {
  fs = std::make_shared<fwlite::TFileService>(
      (chan + "_" + var + ".root").c_str());
  js = json;
  channel_str = chan;
}

 HTTSequenceCrash::~HTTSequenceCrash() {}

void HTTSequenceCrash::BuildSequence(){
  using ROOT::Math::VectorUtil::DeltaR;


  // Set global parameters that get used in multiple places
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

  // Pair DeltaR filtering
  BuildModule(SimpleFilter<CompositeCandidate>("PairFilter")
      .set_input_label("ditau").set_min(1)
      .set_predicate( (bind(&CompositeCandidate::DeltaR, _1, "lepton1","lepton2") > 0.5))
      );



  std::string met_label = "pfMVAMet";

  std::string jets_label="ak4PFJetsCHS";
  unsigned mva_met_mode = 1;
  unsigned faked_tau_selector = 0;
  unsigned hadronic_tau_selector = 0; 
  unsigned tau_scale_mode = 0;
  bool moriond_tau_scale = 0;
  double tau_shift = 1.0;
  std::string allowed_tau_modes = "";
  
BuildModule(HTTPairSelector("HTTPairSelector")
    .set_channel(channel)
    .set_fs(fs.get())
    .set_pair_label("ditau")
    .set_met_label(met_label)
    .set_mva_met_from_vector(mva_met_mode == 1)
    .set_faked_tau_selector(faked_tau_selector)
    .set_hadronic_tau_selector(hadronic_tau_selector)
    .set_gen_taus_label(is_embedded ? "genParticlesEmbedded" : "genParticlesTaus")
    .set_scale_met_for_tau((tau_scale_mode > 0 || (moriond_tau_scale && (is_embedded || !is_data) )   ))
    .set_tau_scale(tau_shift)
    .set_allowed_tau_modes(allowed_tau_modes));


}

// --------------------------------------------------------------------------
// ET Pair Sequence
// --------------------------------------------------------------------------
void HTTSequenceCrash::BuildETPairs() {
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
  std::cout<<"adfasfA"<<std::endl;

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
void HTTSequenceCrash::BuildMTPairs() {
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
void HTTSequenceCrash::BuildEMPairs() {
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
void HTTSequenceCrash::BuildTauSelection(){
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

void HTTSequenceCrash::BuildDiElecVeto() {
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

void HTTSequenceCrash::BuildDiMuonVeto() {
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

void HTTSequenceCrash::BuildExtraElecVeto(){
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

void HTTSequenceCrash::BuildExtraMuonVeto(){
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
