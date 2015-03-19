#include <iostream>
#include <vector>
#include <string>
#include "TSystem.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Core/interface/AnalysisBase.h"
#include "Utilities/interface/JsonTools.h"
#include "Utilities/interface/FnRootTools.h"
#include "Modules/interface/CopyCollection.h"
#include "Modules/interface/SimpleFilter.h"
#include "Modules/interface/OneCollCompositeProducer.h"
#include "HiggsTauTau/interface/HTTConfig.h"
#include "HiggsTauTau/interface/HTTGenEvent.h"
#include "HiggsTauTau/interface/HTTPairStudy.h"
#include "HiggsTauTau/interface/HTTEnergyScale.h"

using std::string;
using std::vector;

using ic::AnalysisBase;
using std::vector;
using std::string;

int main(int argc, char* argv[]) {
  // Shorten: write a function that does this, or move the classes into Analysis
  gSystem->Load("libFWCoreFWLite.dylib");
  gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  AutoLibraryLoader::enable();

  Json::Value const js = ic::MergedJson(argc, argv);

  // Create ROOT output fileservice
  fwlite::TFileService* fs =
      new fwlite::TFileService((js["job"]["output_folder"].asString() +
                                js["job"]["output_name"].asString()).c_str());

  vector<string> files = ic::ParseFileLines(js["job"]["filelist"].asString());
  for (auto & f : files) f = js["job"]["file_prefix"].asString() + f;

  AnalysisBase analysis("HiggsTauTau", files, "icEventProducer/EventTree",
                        js["job"]["max_events"].asInt64());
  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);
  analysis.CalculateTimings(js["job"]["timings"].asBool());


  auto htt_energy_scale = ic::HTTEnergyScale("HTTEnergyScale")
    .set_input_label("taus")
    .set_shift(1.0)
    .set_strategy(ic::strategy::paper2013)
    .set_moriond_corrections(true);

  // Di-Electron Veto
  // --------------------------------------------------------------------------
  auto ele_veto_copy = ic::CopyCollection<ic::Electron>(
      "CopyToVetoElectrons", "electrons", "vetoElectrons");

  auto ele_veto_filter = ic::SimpleFilter<ic::Electron>("EleVetoFilter")
    .set_input_label("vetoElectrons")
    .set_predicate([](ic::Electron const* e) {
      return
        MinPtMaxEta(e, 15., 2.5)              &&
        Electron2011WP95ID(e)                 &&
        std::fabs(e->dxy_vertex())  < 0.045   &&
        std::fabs(e->dz_vertex())   < 0.2     &&
        PF04IsolationVal(e, 0.5)    < 0.3;
    });

  auto ele_veto_pairs =
    ic::OneCollCompositeProducer<ic::Electron>("EleVetoPairProducer")
      .set_input_label("vetoElectrons")
      .set_candidate_name_first("elec1")
      .set_candidate_name_second("elec2")
      .set_output_label("eleVetoPairs");

  auto ele_veto_pair_filter =
    ic::SimpleFilter<ic::CompositeCandidate>("EleVetoPairFilter")
      .set_input_label("eleVetoPairs")
      .set_predicate([](ic::CompositeCandidate const* c) {
        return
          c->DeltaR("elec1", "elec2") > 0.15 &&
          c->charge() == 0;
      });

  // Extra-Electron Veto
  // --------------------------------------------------------------------------
  auto ele_extra_copy = ic::CopyCollection<ic::Electron>(
      "CopyToExtraElectrons", "electrons", "extraElectrons");

  auto ele_extra_filter = ic::SimpleFilter<ic::Electron>("EleExtraVeto")
    .set_input_label("extraElectrons")
    .set_predicate([](ic::Electron const* e) {
      return
        MinPtMaxEta(e, 10., 2.5)              &&
        ElectronHTTId(e, true)                &&
        std::fabs(e->dxy_vertex())  < 0.045   &&
        std::fabs(e->dz_vertex())   < 0.2     &&
        PF04IsolationVal(e, 0.5)    < 0.3;
    });

  // Di-Muon Veto
  // --------------------------------------------------------------------------
  auto muo_veto_copy = ic::CopyCollection<ic::Muon>(
      "CopyToVetoMuons", "muonsPFlow", "vetoMuons");

  auto muo_veto_filter = ic::SimpleFilter<ic::Muon>("MuoVetoFilter")
    .set_input_label("vetoMuons")
    .set_predicate([](ic::Muon const* m) {
      return
        MinPtMaxEta(m, 15., 2.4)              &&
        m->is_global()                        &&
        m->is_tracker()                       &&
        std::fabs(m->dxy_vertex())  < 0.045   &&
        std::fabs(m->dz_vertex())   < 0.2     &&
        PF04IsolationVal(m, 0.5)    < 0.3;
    });

  auto muo_veto_pairs =
    ic::OneCollCompositeProducer<ic::Muon>("MuoVetoPairProducer")
      .set_input_label("vetoMuons")
      .set_candidate_name_first("muon1")
      .set_candidate_name_second("muon2")
      .set_output_label("muoVetoPairs");

  auto muo_veto_pair_filter =
    ic::SimpleFilter<ic::CompositeCandidate>("MuoVetoPairFilter")
      .set_input_label("muoVetoPairs")
      .set_predicate([](ic::CompositeCandidate const* c) {
        return
          c->DeltaR("muon1", "muon2") > 0.15 &&
          c->charge() == 0;
      });

  // Extra-Muon Veto
  // --------------------------------------------------------------------------
  auto muo_extra_copy = ic::CopyCollection<ic::Muon>(
      "CopyToExtraMuons", "muonsPFlow", "extraMuons");

  auto muo_extra_filter = ic::SimpleFilter<ic::Muon>("MuoExtraVeto")
    .set_input_label("extraMuons")
    .set_predicate([](ic::Muon const* m) {
      return
        MinPtMaxEta(m, 10., 2.4)              &&
        MuonTight(m)                          &&
        std::fabs(m->dxy_vertex())  < 0.045   &&
        std::fabs(m->dz_vertex())   < 0.2     &&
        PF04IsolationVal(m, 0.5)    < 0.3;
    });

 bool do_gen_evt = js["do_gen_evt"].asBool();

  auto httGenEvent = ic::HTTGenEvent("HttGenEvent")
      .set_genparticle_label("genParticlesTaus");

  auto et_tight = ic::HTTPairStudy("ETTight").set_fs(fs)
                      .set_channel(ic::channel::et)
                      .set_do_loose(false)
                      .set_do_gen_evt(do_gen_evt);

  auto et_loose = ic::HTTPairStudy("ETLoose").set_fs(fs)
                      .set_channel(ic::channel::et)
                      .set_do_loose(true)
                      .set_do_gen_evt(do_gen_evt);

  auto mt_tight = ic::HTTPairStudy("MTTight").set_fs(fs)
                      .set_channel(ic::channel::mt)
                      .set_do_loose(false)
                      .set_do_gen_evt(do_gen_evt);

  auto mt_loose = ic::HTTPairStudy("MTLoose").set_fs(fs)
                      .set_channel(ic::channel::mt)
                      .set_do_loose(true)
                      .set_do_gen_evt(do_gen_evt);

  auto em_tight = ic::HTTPairStudy("EMTight").set_fs(fs)
                      .set_channel(ic::channel::em)
                      .set_do_loose(false)
                      .set_do_gen_evt(do_gen_evt);

  auto em_loose = ic::HTTPairStudy("EMLoose").set_fs(fs)
                      .set_channel(ic::channel::em)
                      .set_do_loose(true)
                      .set_do_gen_evt(do_gen_evt);

  auto tt_tight = ic::HTTPairStudy("TTTight").set_fs(fs)
                      .set_channel(ic::channel::tt)
                      .set_do_loose(false)
                      .set_do_gen_evt(do_gen_evt);

  auto tt_loose = ic::HTTPairStudy("TTLoose").set_fs(fs)
                      .set_channel(ic::channel::tt)
                      .set_do_loose(true)
                      .set_do_gen_evt(do_gen_evt);

  analysis.AddModule(&htt_energy_scale);
  analysis.AddModule(&ele_veto_copy);
  analysis.AddModule(&ele_veto_filter);
  analysis.AddModule(&ele_veto_pairs);
  analysis.AddModule(&ele_veto_pair_filter);
  analysis.AddModule(&ele_extra_copy);
  analysis.AddModule(&ele_extra_filter);
  analysis.AddModule(&muo_veto_copy);
  analysis.AddModule(&muo_veto_filter);
  analysis.AddModule(&muo_veto_pairs);
  analysis.AddModule(&muo_veto_pair_filter);
  analysis.AddModule(&muo_extra_copy);
  analysis.AddModule(&muo_extra_filter);

  if (do_gen_evt) analysis.AddModule(&httGenEvent);

  analysis.AddModule(&et_tight);
  analysis.AddModule(&et_loose);
  analysis.AddModule(&mt_tight);
  analysis.AddModule(&mt_loose);
  analysis.AddModule(&em_tight);
  analysis.AddModule(&em_loose);
  analysis.AddModule(&tt_tight);
  analysis.AddModule(&tt_loose);
  analysis.RunAnalysis();

  delete fs;

  return 0;
}
