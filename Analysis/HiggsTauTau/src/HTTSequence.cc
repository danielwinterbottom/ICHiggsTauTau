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
// Generic modules
#include "Modules/interface/SimpleFilter.h"
#include "Modules/interface/CompositeProducer.h"
#include "Modules/interface/CopyCollection.h"
#include "Modules/interface/OneCollCompositeProducer.h"
#include "Modules/interface/OverlapFilter.h"
#include "Modules/interface/EnergyShifter.h"
#include "Modules/interface/PileupWeight.h"

namespace ic {

void HTTSequence::BuildSequence(ModuleSequence* seq, ic::channel channel,
                                Json::Value const& js) {
  BuildModule(seq, HTTGenEvent("HTTGenEvent")
                       .set_genparticle_label(js["genTaus"].asString())
                       .set_genjet_label("genJets"));

  if (channel == channel::et) BuildETPairs(seq, js);
  if (channel == channel::mt) BuildMTPairs(seq, js);
  if (channel == channel::em) BuildEMPairs(seq, js);


  BuildModule(seq, SimpleFilter<CompositeCandidate>("PairFilter")
      .set_input_label("ditau").set_min(1)
      .set_predicate([=](CompositeCandidate const* c) {
        return ROOT::Math::VectorUtil::DeltaR(c->At(0)->vector(), c->At(1)->vector()) > js["pair_dr"].asDouble();
      }));

  if (js["do_trg_filter"].asBool()) {
    BuildModule(seq, HTTTriggerFilter("HTTTriggerFilter")
        .set_channel(channel)
        .set_mc(mc::summer12_53X)
        .set_is_data(false)
        .set_is_embedded(false)
        .set_pair_label("ditau"));
  }

  if (channel == channel::et) BuildDiElecVeto(seq, js);
  if (channel == channel::mt) BuildDiMuonVeto(seq, js);
  BuildExtraElecVeto(seq, js);
  BuildExtraMuonVeto(seq, js);

  TH1D d_pu = GetFromTFile<TH1D>(js["data_pu_file"].asString(), "/", "pileup");
  TH1D m_pu = GetFromTFile<TH1D>(js["mc_pu_file"].asString(), "/", "pileup");
  if (js["do_pu_wt"].asBool()) {
    BuildModule(seq, PileupWeight("PileupWeight")
        .set_data(new TH1D(d_pu)).set_mc(new TH1D(m_pu)));
  }
}

void HTTSequence::BuildETPairs(ModuleSequence* seq, Json::Value const& js) {
  BuildModule(seq, CopyCollection<Electron>("CopyToSelectedElectrons",
      js["electrons"].asString(), "sel_electrons"));

  BuildModule(seq, SimpleFilter<Electron>("ElectronFilter")
      .set_input_label("sel_electrons").set_min(1)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > 24.0    &&
                fabs(e->eta())          < 2.1     &&
                fabs(e->dxy_vertex())   < 0.045   &&
                fabs(e->dz_vertex())    < 0.2     &&
                /*ElectronHTTId(e, false)           &&*/
                ElectronZbbID(e)                  &&
                PF04IsolationVal(e, 0.5) < 0.1;
      }));

  BuildModule(seq, HTTEnergyScale("TauEnergyScaleCorrection")
      .set_input_label("taus")
      .set_shift(js["tau_es_shift"].asDouble())
      .set_strategy(strategy::paper2013)
      .set_moriond_corrections(true));

  BuildModule(seq, SimpleFilter<Tau>("TauFilter")
      .set_input_label(js["taus"].asString()).set_min(1)
      .set_predicate([=](Tau const* t) {
        return  t->pt()                     > 20.0      &&
                fabs(t->eta())              <  2.3      &&
                fabs(t->lead_dz_vertex())   <  0.2      &&
                t->GetTauID("decayModeFinding") > 0.5/*   &&
                t->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 1.5 &&
                passAntiEMVA(t, 0) &&
                t->GetTauID("againstMuonLoose") > 0.5*/;
      }));

  BuildModule(seq, CompositeProducer<Electron, Tau>("ETPairProducer")
      .set_input_label_first("sel_electrons")
      .set_input_label_second(js["taus"].asString())
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));
}

void HTTSequence::BuildMTPairs(ModuleSequence* seq, Json::Value const& js) {
  BuildModule(seq, CopyCollection<Muon>("CopyToSelectedMuons",
      js["muons"].asString(), "sel_muons"));

  BuildModule(seq, SimpleFilter<Muon>("MuonFilter")
      .set_input_label("sel_muons").set_min(1)
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > 20.0    &&
                fabs(m->eta())          < 2.1     &&
                fabs(m->dxy_vertex())   < 0.045   &&
                fabs(m->dz_vertex())    < 0.2     &&
                MuonTight(m)                      &&
                PF04IsolationVal(m, 0.5) < 0.1;
      }));

  BuildModule(seq, HTTEnergyScale("TauEnergyScaleCorrection")
      .set_input_label("taus")
      .set_shift(js["tau_es_shift"].asDouble())
      .set_strategy(strategy::paper2013)
      .set_moriond_corrections(true));

  BuildModule(seq, SimpleFilter<Tau>("TauFilter")
      .set_input_label(js["taus"].asString()).set_min(1)
      .set_predicate([=](Tau const* t) {
        return  t->pt()                     > 20.0      &&
                fabs(t->eta())              <  2.3      &&
                fabs(t->lead_dz_vertex())   <  0.2      &&
                t->GetTauID("decayModeFinding") > 0.5/*   &&
                t->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 1.5 &&
                t->GetTauID("againstElectronLoose") > 0.5 &&
                t->GetTauID("againstMuonTight")     > 0.5 &&
                TauEoverP(t, 0.2)*/;
      }));

  BuildModule(seq, CompositeProducer<Muon, Tau>("MTPairProducer")
      .set_input_label_first("sel_muons")
      .set_input_label_second(js["taus"].asString())
      .set_candidate_name_first("lepton1")
      .set_candidate_name_second("lepton2")
      .set_output_label("ditau"));
}

void HTTSequence::BuildEMPairs(ModuleSequence* seq, Json::Value const& js) {
  BuildModule(seq, EnergyShifter<Electron>("ElectronEnergyScaleCorrection")
      .set_input_label(js["electrons"].asString())
      .set_shift(js["elec_es_shift"].asDouble()));

  if (js["do_em_extras"].asBool()) {
    BuildModule(seq, HTTEMuExtras("EMExtras"));
  }

  BuildModule(seq, CopyCollection<Electron>("CopyToSelectedElectrons",
      js["electrons"].asString(), "sel_electrons"));

  BuildModule(seq, CopyCollection<Muon>("CopyToSelectedMuons",
      js["muons"].asString(), "sel_muons"));

  BuildModule(seq, SimpleFilter<Electron>("ElectronFilter")
      .set_input_label("sel_electrons").set_min(1)
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > 10.0    &&
                fabs(e->eta())          < 2.3     &&
                fabs(e->dxy_vertex())   < 0.02    &&
                fabs(e->dz_vertex())    < 0.1     &&
                /*ElectronHTTId(e, true)            &&*/
                PF04IsolationEBElec(e, 0.5, 0.15, 0.1);
      }));

  BuildModule(seq, OverlapFilter<Electron, Muon>("ElecMuonOverlapFilter")
      .set_input_label("sel_electrons")
      .set_reference_label(js["muons"].asString())
      .set_min_dr(0.3));

  BuildModule(seq, SimpleFilter<Muon>("MuonFilter")
      .set_input_label("sel_muons").set_min(1)
      .set_predicate([=](Muon const* m) {
        return  m->pt()                 > 10.0    &&
                fabs(m->eta())          < 2.1     &&
                fabs(m->dxy_vertex())   < 0.02    &&
                fabs(m->dz_vertex())    < 0.1     &&
                MuonTight(m)                      &&
                PF04IsolationEB(m, 0.5, 0.15, 0.1);
      }));

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

void HTTSequence::BuildDiElecVeto(ModuleSequence* seq, Json::Value const& js) {
  BuildModule(seq, CopyCollection<Electron>("CopyToVetoElecs",
      js["electrons"].asString(), "veto_elecs"));

  BuildModule(seq, SimpleFilter<Electron>("VetoElecFilter")
      .set_input_label("veto_elecs")
      .set_predicate([=](Electron const* e) {
        return  e->pt()                 > 15.0    &&
                fabs(e->eta())          < 2.5     &&
                fabs(e->dxy_vertex())   < 0.045   &&
                fabs(e->dz_vertex())    < 0.2     &&
                Electron2011WP95ID(e)             &&
                PF04IsolationVal(e, 0.5) < 0.3;
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
        return  m->pt()                 > 15.0    &&
                fabs(m->eta())          < 2.4     &&
                fabs(m->dxy_vertex())   < 0.045   &&
                fabs(m->dz_vertex())    < 0.2     &&
                m->is_global()                    &&
                m->is_tracker()                   &&
                PF04IsolationVal(m, 0.5) < 0.3;
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

void HTTSequence::BuildExtraMuonVeto(ModuleSequence* seq,
                                     Json::Value const& js) {
  BuildModule(seq, CopyCollection<Muon>("CopyToExtraMuons",
      js["muons"].asString(), "extra_muons"));

  BuildModule(seq, SimpleFilter<Muon>("ExtraMuonFilter")
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
