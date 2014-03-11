#include "Zbb/interface/ZbbTriggerFilter.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "boost/bind.hpp"
#include "boost/format.hpp"

namespace ic {

ZbbTriggerFilter::ZbbTriggerFilter(std::string const& name)
    : ModuleBase(name) {
  elec_pairs_ = "electron_pairs";
  muon_pairs_ = "muon_pairs";
}

ZbbTriggerFilter::~ZbbTriggerFilter() { }

int ZbbTriggerFilter::PreAnalysis() {
  return 0;
}

int ZbbTriggerFilter::Execute(TreeEvent *event) {
  bool el_fired = false;
  bool mu_fired = false;
  std::string el_trig_obj_label;
  std::string el_leg1_filter;
  std::string el_leg2_filter;
  std::string mu_trig_obj_label;
  std::string mu_leg1_filter;
  std::string mu_leg2_filter;

  EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
  unsigned run = eventInfo->run();

  auto const& trigger_paths =
      event->GetPtrVec<TriggerPath>("triggerPaths");
  for (unsigned i = 0; i < trigger_paths.size(); ++i) {
    std::string name = trigger_paths[i]->name();
    if (run >= 160410 && run <= 170248 &&
      name.find("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v")
      != name.npos) el_fired = true;
    if (run >= 170249 /* && run <= x */ &&
      name.find("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v")
      != name.npos) el_fired = true;

    if (run >= 160410 && run <= 163268 &&
      name.find("HLT_DoubleMu6_v")
      != name.npos) mu_fired = true;
    if (run >= 163269 && run <= 165120 &&
      name.find("HLT_DoubleMu7_v")
      != name.npos) mu_fired = true;
    if (run >= 165121 /* && run <= x */ &&
      name.find("HLT_Mu13_Mu8_v")
      != name.npos) mu_fired = true;
  }
  if (!el_fired && !mu_fired) return 1;

  if (run >= 160410 && run <= 170248) {
    el_trig_obj_label = "triggerObjectsEle17Ele8L";
    el_leg1_filter = "hltEle17CaloIdIsoEle8CaloIdIsoPixelMatchDoubleFilter";
    el_leg2_filter = "hltEle17CaloIdLCaloIsoVLPixelMatchFilter";
  }
  if (run >= 170249 /* && run <= x */) {
    el_trig_obj_label = "triggerObjectsEle17Ele8";
    el_leg1_filter = "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolDoubleFilter";
    el_leg2_filter = "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolFilter";
  }

  if (run >= 160410 && run <= 163268) {
    mu_trig_obj_label = "triggerObjectsDoubleMu6";
    mu_leg1_filter = "hltDiMuonL3PreFiltered6";
    mu_leg2_filter = "hltDiMuonL3PreFiltered6";
  }
  if (run >= 163269 && run <= 165120) {
    mu_trig_obj_label = "triggerObjectsDoubleMu7";
    mu_leg1_filter = "hltDiMuonL3PreFiltered7";
    mu_leg2_filter = "hltDiMuonL3PreFiltered7";
  }
  if (run >= 165121 && run <= 173235) {
    mu_trig_obj_label = "triggerObjectsMu13Mu8";
    mu_leg1_filter = "hltDiMuonL3PreFiltered8";
    mu_leg2_filter = "hltSingleMu13L3Filtered13";
  }
  if (run >= 173236 /* && run <= x */) {
    mu_trig_obj_label = "triggerObjectsMu13Mu8";
    mu_leg1_filter = "hltDiMuonL3p5PreFiltered8";
    mu_leg2_filter = "hltSingleMu13L3Filtered13";
  }

  std::vector<CompositeCandidate *> & elec_pairs =
      event->GetPtrVec<CompositeCandidate>(elec_pairs_);

  std::vector<TriggerObject *> const& ele_objs =
      event->GetPtrVec<TriggerObject>(el_trig_obj_label);

  std::vector<CompositeCandidate *> elec_pass;

  for (unsigned i = 0; i < elec_pairs.size(); ++i) {
    unsigned n_low  = 0;
    unsigned n_high = 0;
    if (IsFilterMatched(elec_pairs[i]->At(0), ele_objs, el_leg1_filter, 0.5)) ++n_low;
    if (IsFilterMatched(elec_pairs[i]->At(1), ele_objs, el_leg1_filter, 0.5)) ++n_low;
    if (IsFilterMatched(elec_pairs[i]->At(0), ele_objs, el_leg2_filter, 0.5)) ++n_high;
    if (IsFilterMatched(elec_pairs[i]->At(1), ele_objs, el_leg2_filter, 0.5)) ++n_high;
    if (n_low == 2 && n_high > 0) elec_pass.push_back(elec_pairs[i]);
  }
  elec_pairs = elec_pass;

  std::vector<CompositeCandidate *> & muon_pairs =
      event->GetPtrVec<CompositeCandidate>(muon_pairs_);

  std::vector<TriggerObject *> const& muo_objs =
      event->GetPtrVec<TriggerObject>(mu_trig_obj_label);

  std::vector<CompositeCandidate *> muon_pass;

  for (unsigned i = 0; i < muon_pairs.size(); ++i) {
    unsigned n_low  = 0;
    unsigned n_high = 0;
    if (IsFilterMatched(muon_pairs[i]->At(0), muo_objs, mu_leg1_filter, 0.5)) ++n_low;
    if (IsFilterMatched(muon_pairs[i]->At(1), muo_objs, mu_leg1_filter, 0.5)) ++n_low;
    if (IsFilterMatched(muon_pairs[i]->At(0), muo_objs, mu_leg2_filter, 0.5)) ++n_high;
    if (IsFilterMatched(muon_pairs[i]->At(1), muo_objs, mu_leg2_filter, 0.5)) ++n_high;
    if (n_low == 2 && n_high > 0) muon_pass.push_back(muon_pairs[i]);
  }
  muon_pairs = muon_pass;

  if (elec_pairs.size() == 0 && muon_pairs.size() == 0) return 1;
  return 0;
}

int ZbbTriggerFilter::PostAnalysis() {
  return 0;
}

void ZbbTriggerFilter::PrintInfo() { }
}
