#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Core/interface/AnalysisBase.h"
#include "Objects/interface/Muon.hh"
#include "Objects/interface/Unhash.h"
#include "Utilities/interface/JsonTools.h"
#include "Utilities/interface/FnRootTools.h"
#include "Modules/interface/LumiMask.h"
#include "Modules/interface/GenericModule.h"
#include "Modules/interface/TriggerInfo.h"
#include "Modules/interface/SimpleFilter.h"
#include "Modules/interface/CopyCollection.h"
#include "Modules/interface/OneCollCompositeProducer.h"
#include "Modules/interface/CompositeProducer.h"
#include "Modules/interface/OverlapFilter.h"
#include "Modules/interface/PileupWeight.h"
#include "Modules/interface/EnergyShifter.h"
#include "HiggsTauTau/interface/HTT2016Studies.h"
#include "HiggsTauTau/interface/EffectiveEvents.h"
#include "HiggsTauTau/interface/SampleStitching.h"
#include "HiggsTauTau/interface/HTTPairGenInfo.h"
#include "HiggsTauTau/interface/HTTFilter.h"
#include "HiggsTauTau/interface/BTagWeightRun2.h"
#include "HiggsTauTau/interface/HTTRun2RecoilCorrector.h"

using std::string;
using std::vector;
using std::set;

// Stolen from:
// http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
bool exists(const std::string& name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

class Sequence {
 private:
  std::vector<std::shared_ptr<ic::ModuleBase>> seq;

 public:
  typedef std::vector<std::shared_ptr<ic::ModuleBase>> ModuleSequence;
  Sequence() = default;
  ~Sequence() = default;

  template<class T>
  void BuildModule(T const& mod) {
     seq.push_back(std::shared_ptr<ic::ModuleBase>(new T(mod)));
  }

  void InsertSequence(std::string name, ic::AnalysisBase & ana) {
    for (auto m : seq) {
      ana.AddModule(name, m.get());
    }
  }
};

void BuildLeptonVetoes(Sequence & seq, unsigned max_muons, unsigned max_elecs);
void BuildDYStitching(Sequence & seq, Json::Value & js);
void BuildWStitching(Sequence & seq, Json::Value & js);

int main(int argc, char* argv[]) {
  Json::Value js = ic::MergedJson(argc, argv);


  bool is_data = js.get("is_data", false).asBool();
  bool do_zpt_reweighting = js.get("do_zpt_reweighting", false).asBool();
  bool do_top_reweighting = js.get("do_top_reweighting", false).asBool();
  std::string sf_wsp = js["sf_wsp"].asString();

  // Set the Unhash map if it exists
  Json::Value js_unhash;
  if (exists(js["hash_map_input"].asString())) {
    js_unhash = ic::ExtractJsonFromFile(js["hash_map_input"].asString());
  }
  std::map<std::size_t, std::string> unhash_map;
  for (auto val : js_unhash.getMemberNames()) {
    unhash_map[boost::lexical_cast<std::size_t>(val)] = js_unhash[val].asString();
  }
  ic::Unhash::SetMap(unhash_map);

  std::map<std::string, std::shared_ptr<fwlite::TFileService>> fs;

  // fwlite::TFileService fs((js["output"].asString()));

  // Create the input filelist
  vector<string> files;
  for (auto const& filelist : js["filelists"]) {
    auto i_files = ic::ParseFileLines(filelist.asString());
    files.insert(files.end(), i_files.begin(), i_files.end());
  }

  vector<string> do_files;
  unsigned file_offset = js.get("file_offset", 0).asUInt();
  unsigned file_step = js.get("file_step", 1).asUInt();
  for (auto & f : files) f = js.get("file_prefix", std::string("")).asString() + f;

  for (unsigned i = file_offset; i < files.size(); i += file_step) {
    do_files.push_back(files[i]);
  }

  // Get the sequences we've been asked to
  std::map<std::string, Sequence> seqs;
  for (auto const& seq : js["sequences"]) {
    seqs[seq.asString()] = Sequence();
    fs[seq.asString()] = std::make_shared<fwlite::TFileService>(
        js["output_dir"].asString() + "/" + seq.asString() + "/" +
        js["output_name"].asString());
    // fs[seq.asString()] = fwlite::TFileService(js["output_dir"].asString() +
    //                                           "/" + seq.asString() + "/" +
    //                                           js["output_name"].asString());
  }

  // If we're going to make the hash map redirect the input tree
  std::string tree_name = "icEventProducer/EventTree";
  if (seqs.count("HashMap")) {
    tree_name = "icHashTreeProducer/HashTree";
  }

  ic::AnalysisBase analysis("HTT2016Studies", do_files, tree_name,
                            js["max_events"].asInt64());
  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);
  analysis.CalculateTimings(js.get("timings", false).asBool());


  // Define some common modules that will be used by multiple sequences
  auto lumimask_module = ic::LumiMask("LumiMask")
    .set_input_file(js.get("lumi_mask", "").asString());

  TH1D d_pu = ic::OpenFromTFile<TH1D>(js["data_pu"].asString());
  TH1D d_pu_hi = ic::OpenFromTFile<TH1D>(js["data_pu_hi"].asString());
  TH1D m_pu = ic::OpenFromTFile<TH1D>(js["mc_pu"].asString());

  auto puweight_module = ic::PileupWeight("PileupWeight")
    .set_data(&d_pu)
    .set_mc(&m_pu)
    .set_print_weights(false);

  auto puweight_module_hi = ic::PileupWeight("PileupWeight", "pileup_hi")
    .set_data(&d_pu_hi)
    .set_mc(&m_pu)
    .set_print_weights(false)
    .set_weight_is_active(false);

  auto trigger_module = ic::TriggerInfo("TriggerInfo")
    .set_output_file(js.get("trigger_info_output", "trigger_info.json").asString());

  // B-tagging efficiencies for reweighting
  TH2F bbtag_eff = ic::OpenFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_ichep2016.root:btag_eff_b");
  TH2F cbtag_eff = ic::OpenFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_ichep2016.root:btag_eff_c");
  TH2F othbtag_eff = ic::OpenFromTFile<TH2F>("input/btag_sf/tagging_efficiencies_ichep2016.root:btag_eff_oth");

  Json::Value js_hash_map;

  for (auto const& seqnjs : js["sequences"]) {
    std::string fullseqn = seqnjs.asString();
    std::vector<std::string> split_seqn;
    boost::split(split_seqn, fullseqn, boost::is_any_of("/"));
    std::string seqn = split_seqn[0];
    std::string subseqn = "";
    if (split_seqn.size() >= 2) {
      subseqn = split_seqn[1];
    }

    if (fullseqn == "TriggerInfo" && is_data) {
      auto & seq = seqs["TriggerInfo"];

      seq.BuildModule(ic::LumiMask(lumimask_module)
        .set_produce_output_jsons(js.get("lumi_out", "lumi_out").asString()));
      seq.BuildModule(ic::TriggerInfo("TriggerInfo")
      .set_output_file(js.get("trigger_info_output", "trigger_info.json").asString()));
      seq.InsertSequence("TriggerInfo", analysis);
    }

    if (fullseqn == "HashMap") {
      seqs["HashMap"].BuildModule(ic::GenericModule("HashMapWriter")
        .set_function([&](ic::TreeEvent * event) {
          std::string id = boost::lexical_cast<std::string>(*event->GetPtr<ULong64_t>("id"));
          std::string str = *(event->GetPtr<std::string>("string"));
          js_hash_map[id] = str;
          return 0;
        }));
      seqs["HashMap"].InsertSequence("HashMap", analysis);
    }

    if (fullseqn == "EffectiveEvents") {
      auto & seq = seqs["EffectiveEvents"];
      if (!is_data) {
        seq.BuildModule(ic::EffectiveEvents("EffectiveEvents")
          .set_fs(fs.at("EffectiveEvents").get()));
      }
      seq.InsertSequence("EffectiveEvents", analysis);
    }

    if (fullseqn == "WJetsStudy") {
      auto & seq = seqs["WJetsStudy"];
      seq.BuildModule(ic::WJetsStudy("WJetsStudy")
        .set_fs(fs.at("WJetsStudy").get())
        .set_genparticle_label("genParticles")
        .set_sample_name(js.get("wjets_sample_name", std::string("")).asString()));
      seq.InsertSequence("WJetsStudy", analysis);
    }

    if (fullseqn == "DYDebug") {
      auto & seq = seqs["DYDebug"];
      if (js.get("do_dyjets_stitching", false).asBool()) {
        auto jss = js["dyjets_stitching"];
        seq.BuildModule(ic::DYJetsStitching("DYJetsStitching")
          .Set_DYJetsToLL(jss["evt_DYJetsToLL"].asUInt(), jss["xs_DYJetsToLL"].asDouble())
          .Set_DY1JetsToLL(jss["evt_DY1JetsToLL"].asUInt(), jss["xs_DY1JetsToLL"].asDouble())
          .Set_DY2JetsToLL(jss["evt_DY2JetsToLL"].asUInt(), jss["xs_DY2JetsToLL"].asDouble())
          .Set_DY3JetsToLL(jss["evt_DY3JetsToLL"].asUInt(), jss["xs_DY3JetsToLL"].asDouble())
          .Set_DY4JetsToLL(jss["evt_DY4JetsToLL"].asUInt(), jss["xs_DY4JetsToLL"].asDouble())
          .set_fs(fs.at("DYDebug").get()));
      }
      seq.InsertSequence("DYDebug", analysis);
    }

    if (seqn == "Zmm") {
      auto & seq = seqs[fullseqn];

      double mu_es_shift = 1.00;
      if (subseqn == "scale_m_lo") mu_es_shift = 0.99;
      if (subseqn == "scale_m_hi") mu_es_shift = 1.01;
      seq.BuildModule(ic::EnergyShifter<ic::Muon>("MuonEnergyScale")
        .set_input_label("muons")
        .set_shift(mu_es_shift)
        .set_save_shifts(true));

      seq.BuildModule(ic::CopyCollection<ic::Muon>("CopyToSelectedMuons",
          "muons", "sel_muons"));

      seq.BuildModule(ic::SimpleFilter<ic::Muon>("MuonFilter")
      .set_input_label("sel_muons").set_min(2)
      .set_predicate([=](ic::Muon const* m) {
        return  m->pt()                 > 10.    &&
                fabs(m->eta())          < 2.4    &&
                fabs(m->dxy_vertex())   < 0.045  &&
                fabs(m->dz_vertex())    < 0.2    &&
                MuonMediumHIPsafe(m);
      }));

      seq.BuildModule(ic::CompositeProducer<ic::Muon, ic::Muon>("ZMMPairProducer")
          .set_input_label_first("sel_muons")
          .set_input_label_second("sel_muons")
          .set_candidate_name_first("lepton1")
          .set_candidate_name_second("lepton2")
          .set_output_label("dimuon")
      );

      using ROOT::Math::VectorUtil::DeltaR;
      seq.BuildModule(ic::SimpleFilter<ic::CompositeCandidate>("PairFilter")
          .set_input_label("dimuon")
          .set_min(1)
          .set_predicate([=](ic::CompositeCandidate const* c) {
            return DeltaR(c->at(0)->vector(), c->at(1)->vector())
                > 0.3;
          })
      );

      seq.BuildModule(ic::GenericModule("PairSorter")
          .set_function(
            [](ic::TreeEvent * evt) {
              auto & pairs = evt->GetPtrVec<ic::CompositeCandidate>("dimuon");
              std::sort(pairs.begin(), pairs.end(), ic::SortMM);
              return 0;
            }
        ));


      // At this point we're done filtering, can calculate other things we nedd
      if (!is_data) {
        seq.BuildModule(ic::HTTPairGenInfo("PairGenInfo")
            .set_ditau_label("dimuon")
        );
        seq.BuildModule(puweight_module);
      } else {
        seq.BuildModule(lumimask_module);
      }

      BuildDYStitching(seq, js);
      BuildWStitching(seq, js);

      BuildLeptonVetoes(seq, /*max_muons=*/2, /*max_elecs=*/0);

      seq.BuildModule(ic::SimpleFilter<ic::PFJet>("JetIDFilter")
        .set_input_label("ak4PFJetsCHS")
        .set_predicate([](ic::PFJet const* j) {
          return PFJetID2016(j);
        })
      );

      seq.BuildModule(ic::ZmmTreeProducer("ZmmTreeProducer")
        .set_fs(fs.at(fullseqn).get())
        .set_sf_workspace(sf_wsp)
        .set_do_zpt_reweighting(do_zpt_reweighting)
        .set_do_top_reweighting(do_top_reweighting)
      );
      seq.InsertSequence(fullseqn, analysis);
    }

    if (seqn == "Theory") {
      auto & seq = seqs[fullseqn];

      seq.BuildModule(ic::TheoryTreeProducer("TheoryTreeProducer")
        .set_fs(fs.at(fullseqn).get())
        .set_sample_mass(js["sample_mass"].asString())
      );
      seq.InsertSequence(fullseqn, analysis);
    }

    if (fullseqn == "Zee") {
      auto & seq = seqs["Zee"];

      seq.BuildModule(ic::CopyCollection<ic::Electron>("CopyToSelectedElecs",
          "electrons", "sel_elecs"));

      seq.BuildModule(ic::SimpleFilter<ic::Electron>("ElectronFilter")
      .set_input_label("sel_elecs").set_min(2)
      .set_predicate([=](ic::Electron const* e) {
        return  e->pt()                 > 10.    &&
                fabs(e->eta())          < 2.5    &&
                fabs(e->dxy_vertex())   < 0.045  &&
                fabs(e->dz_vertex())    < 0.2    &&
                ElectronHTTIdSpring15(e, false);
      }));

      seq.BuildModule(ic::OneCollCompositeProducer<ic::Electron>("ZEEPairProducer")
          .set_input_label("sel_elecs")
          .set_candidate_name_first("lepton1")
          .set_candidate_name_second("lepton2")
          .set_output_label("dielec")
      );

      using ROOT::Math::VectorUtil::DeltaR;
      seq.BuildModule(ic::SimpleFilter<ic::CompositeCandidate>("PairFilter")
          .set_input_label("dielec")
          .set_min(1)
          .set_predicate([=](ic::CompositeCandidate const* c) {
            return DeltaR(c->at(0)->vector(), c->at(1)->vector())
                > 0.5;
          })
      );

      // At this point we're done filtering, can calculate other things we nedd
      if (!is_data) {
        seq.BuildModule(puweight_module);
      } else {
        seq.BuildModule(lumimask_module);
      }

      seq.BuildModule(ic::ZeeTreeProducer("ZeeTreeProducer")
        .set_fs(fs.at("Zee").get())
        .set_sf_workspace(sf_wsp)
      );
      seq.InsertSequence("Zee", analysis);
    }

    if (fullseqn == "ZmmTP") {
      auto & seq = seqs["ZmmTP"];


      seq.BuildModule(ic::CopyCollection<ic::Muon>("CopyToTagMuons",
          "muons", "tag_muons"));

      seq.BuildModule(ic::SimpleFilter<ic::Muon>("TagMuonFilter")
      .set_input_label("tag_muons").set_min(1)
      .set_predicate([=](ic::Muon const* m) {
        return  m->pt()                 > 23.    &&
                fabs(m->eta())          < 2.4    &&
                fabs(m->dxy_vertex())   < 0.045  &&
                fabs(m->dz_vertex())    < 0.2    &&
                MuonMediumHIPsafe(m)             &&
                PF04IsolationVal(m, 0.5, false) < 0.2;
      }));

      seq.BuildModule(ic::CopyCollection<ic::Muon>("CopyToProbeMuons",
          "muons", "probe_muons"));

      seq.BuildModule(ic::CopyCollection<ic::Track>("CopyToProbeTracks",
          "tracks", "probe_tracks"));

      seq.BuildModule(ic::SimpleFilter<ic::Muon>("ProbeMuonFilter")
      .set_input_label("probe_muons")
      .set_predicate([=](ic::Muon const* m) {
        return  m->pt()                 > 10.    &&
                fabs(m->eta())          < 2.4    &&
                m->is_tracker();
      }));

      seq.BuildModule(ic::SimpleFilter<ic::Track>("ProbeTrackFilter")
      .set_input_label("probe_tracks")
      .set_predicate([=](ic::Track const* t) {
        return  t->pt()                 > 10.    &&
                fabs(t->eta())          < 2.4;
      }));

      seq.BuildModule(ic::GenericModule("TrackCandConverter").set_function([](ic::TreeEvent *evt) {
        auto const& trks = evt->GetPtrVec<ic::Track>("probe_tracks");
        std::vector<ic::Candidate> trk_cands;
        for (auto trk : trks) {
          ic::Candidate cand;
          cand.set_vector(ROOT::Math::PtEtaPhiEVector(trk->vector()));
          cand.set_charge(trk->charge());
          trk_cands.push_back(cand);
        }
        evt->Add("probe_cands_prod", trk_cands);
        auto & trk_prods = evt->Get<std::vector<ic::Candidate>>("probe_cands_prod");
        std::vector<ic::Candidate *> cand_ptrs;
        for (auto & cand : trk_prods) {
          cand_ptrs.push_back(&cand);
        }
        evt->Add("probe_cands", cand_ptrs);
        return 0;
      })
      );

      seq.BuildModule(ic::CompositeProducer<ic::Muon, ic::Muon>("ZMMPairProducer")
          .set_input_label_first("tag_muons")
          .set_input_label_second("probe_muons")
          .set_candidate_name_first("tag")
          .set_candidate_name_second("probe")
          .set_output_label("dimuon")
      );

      seq.BuildModule(ic::CompositeProducer<ic::Muon, ic::Candidate>("ZMTrkPairProducer")
          .set_input_label_first("tag_muons")
          .set_input_label_second("probe_cands")
          .set_candidate_name_first("tag")
          .set_candidate_name_second("probe")
          .set_output_label("mutrk")
      );

      using ROOT::Math::VectorUtil::DeltaR;
      seq.BuildModule(ic::SimpleFilter<ic::CompositeCandidate>("ZMMPairFilter")
          .set_input_label("dimuon")
          .set_predicate([=](ic::CompositeCandidate const* c) {
            return c->charge() == 0 &&
              DeltaR(c->at(0)->vector(), c->at(1)->vector()) > 0.5;
          })
      );

      seq.BuildModule(ic::SimpleFilter<ic::CompositeCandidate>("MuTrkPairFilter")
          .set_input_label("mutrk")
          .set_predicate([=](ic::CompositeCandidate const* c) {
            return c->charge() == 0 &&
              DeltaR(c->at(0)->vector(), c->at(1)->vector()) > 0.5;
          })
      );

      // At this point we're done filtering, can calculate other things we nedd
      if (!is_data) {
        seq.BuildModule(puweight_module);
      } else {
        seq.BuildModule(lumimask_module);
      }


      seq.BuildModule(ic::ZmmTPTreeProducer("ZmmTPTreeProducer")
        .set_fs(fs.at("ZmmTP").get())
        .set_sf_workspace(sf_wsp)
      );
      seq.InsertSequence("ZmmTP", analysis);
    }

    if (fullseqn == "ZeeTP") {
      auto & seq = seqs["ZeeTP"];


      seq.BuildModule(ic::CopyCollection<ic::Electron>("CopyToTagElecs",
          "electrons", "tag_elecs"));

      seq.BuildModule(ic::SimpleFilter<ic::Electron>("TagElectronFilter")
      .set_input_label("tag_elecs").set_min(1)
      .set_predicate([=](ic::Electron const* e) {
        return  e->pt()                 > 26.    &&
                fabs(e->eta())          < 2.1    &&
                fabs(e->dxy_vertex())   < 0.045  &&
                fabs(e->dz_vertex())    < 0.2    &&
                ElectronHTTIdSpring15(e, false)  &&
                PF03IsolationVal(e, 0.5, false) < 0.1;
      }));

      seq.BuildModule(ic::CopyCollection<ic::Electron>("CopyToProbeElecs",
          "electrons", "probe_elecs"));

      seq.BuildModule(ic::SimpleFilter<ic::Electron>("ProbeElectronFilter")
      .set_input_label("probe_elecs").set_min(1)
      .set_predicate([=](ic::Electron const* e) {
        return  e->pt()                 > 10.    &&
                fabs(e->eta())          < 2.5;
      }));

      seq.BuildModule(ic::CompositeProducer<ic::Electron, ic::Electron>("ZEEPairProducer")
          .set_input_label_first("tag_elecs")
          .set_input_label_second("probe_elecs")
          .set_candidate_name_first("tag")
          .set_candidate_name_second("probe")
          .set_output_label("dielec")
      );

      using ROOT::Math::VectorUtil::DeltaR;
      seq.BuildModule(ic::SimpleFilter<ic::CompositeCandidate>("PairFilter")
          .set_input_label("dielec")
          .set_min(1)
          .set_predicate([=](ic::CompositeCandidate const* c) {
            return c->charge() == 0 &&
              DeltaR(c->at(0)->vector(), c->at(1)->vector()) > 0.5;
          })
      );

      // At this point we're done filtering, can calculate other things we nedd
      if (!is_data) {
        seq.BuildModule(puweight_module);
      } else {
        seq.BuildModule(lumimask_module);
      }


      seq.BuildModule(ic::ZeeTPTreeProducer("ZeeTPTreeProducer")
        .set_fs(fs.at("ZeeTP").get())
        .set_sf_workspace("input/scale_factors/scalefactors_2016_v1.root")
      );
      seq.InsertSequence("ZeeTP", analysis);
    }

    if (seqn == "ZmtTP") {
      auto & seq = seqs[fullseqn];

      seq.BuildModule(ic::CopyCollection<ic::Muon>("CopyToSelectedMuons",
          "muons", "sel_muons"));

      seq.BuildModule(ic::SimpleFilter<ic::Muon>("MuonFilter")
        .set_input_label("sel_muons").set_min(1)
        .set_predicate([=](ic::Muon const* m) {
          return  m->pt()                 > 23.    &&
                  fabs(m->eta())          < 2.1    &&
                  fabs(m->dxy_vertex())   < 0.045  &&
                  fabs(m->dz_vertex())    < 0.2    &&
                  MuonMediumHIPsafe(m)             &&
                  PF04IsolationVal(m, 0.5, 0) < 0.15;
        }));

      double tau_es_shift = 1.00;
      if (subseqn == "scale_t_lo") tau_es_shift = 0.97;
      if (subseqn == "scale_t_hi") tau_es_shift = 1.03;
      seq.BuildModule(ic::EnergyShifter<ic::Tau>("TauEnergyScale")
        .set_input_label("taus")
        .set_shift(tau_es_shift)
        .set_save_shifts(true));

      seq.BuildModule(ic::CopyCollection<ic::Tau>("CopyToSelectedTaus",
          "taus", "sel_taus"));

      seq.BuildModule(ic::SimpleFilter<ic::Tau>("TauFilter")
        .set_input_label("sel_taus").set_min(1)
        .set_predicate([=](ic::Tau const* t) {
          return  t->pt()                     > 20.    &&
                  fabs(t->eta())              < 2.3    &&
                  fabs(t->lead_dz_vertex())   < 0.2    &&
                  abs(t->charge())            == 1     &&
                  t->GetTauID(js["ZmtTP_tauDM"].asString()) > 0.5;
        }));

      seq.BuildModule(ic::CompositeProducer<ic::Muon, ic::Tau>("MTPairProducer")
        .set_input_label_first("sel_muons")
        .set_input_label_second("sel_taus")
        .set_candidate_name_first("lepton1")
        .set_candidate_name_second("lepton2")
        .set_output_label("ditau")
      );

      using ROOT::Math::VectorUtil::DeltaR;
      seq.BuildModule(ic::SimpleFilter<ic::CompositeCandidate>("PairFilter")
          .set_input_label("ditau")
          .set_min(1)
          .set_predicate([=](ic::CompositeCandidate const* c) {
            return DeltaR(c->at(0)->vector(), c->at(1)->vector())
                > 0.5;
          })
      );

      seq.BuildModule(ic::GenericModule("PairSorter")
          .set_function(
            [](ic::TreeEvent * evt) {
              auto & pairs = evt->GetPtrVec<ic::CompositeCandidate>("ditau");
              std::sort(pairs.begin(), pairs.end(), ic::SortByIsoMT);
              return 0;
            })
      );

      seq.BuildModule(ic::SelectMVAMET("SelectMVAMET")
          .set_pairs_label("ditau")
          .set_met_label("pfMVAMetVector")
          .set_met_target("mvamet")
          .set_correct_for_lepton2("shifts_TauEnergyScale")
      );
      // At this point we're done filtering, can calculate other things we need
      if (!is_data) {
        seq.BuildModule(ic::HTTPairGenInfo("PairGenInfo")
            .set_ditau_label("ditau")
        );
        seq.BuildModule(puweight_module);
        seq.BuildModule(puweight_module_hi);

        seq.BuildModule(ic::HTTRun2RecoilCorrector("HTTRun2RecoilCorrector")
            .set_sample(js.get("recoil_sample", "NoCorrection").asString())
            .set_channel(ic::channel::mt)
            .set_mc(ic::mc::spring16_80X)
            .set_met_label("mvamet")
            .set_jets_label("ak4PFJetsCHS")
            .set_strategy(ic::strategy::mssmspring16)
            .set_use_quantile_map(false)
        );

        seq.BuildModule(ic::BTagWeightRun2("BTagWeightRun2")
            .set_channel(ic::channel::mt)
            .set_era(ic::era::data_2016)
            .set_jet_label("ak4PFJetsCHS")
            .set_bbtag_eff(&bbtag_eff)
            .set_cbtag_eff(&cbtag_eff)
            .set_othbtag_eff(&othbtag_eff)
            .set_do_reshape(false)
            .set_btag_mode(0)
            .set_bfake_mode(0)
        );
      } else {
        seq.BuildModule(lumimask_module);
      }

      BuildDYStitching(seq, js);
      BuildWStitching(seq, js);

      BuildLeptonVetoes(seq, /*max_muons=*/1, /*max_elecs=*/0);


      seq.BuildModule(ic::ZmtTPTreeProducer("ZmtTPTreeProducer")
        .set_fs(fs.at(fullseqn).get())
        .set_sf_workspace(sf_wsp)
        .set_do_zpt_reweighting(do_zpt_reweighting)
        .set_do_top_reweighting(do_top_reweighting)
      );
      seq.InsertSequence(fullseqn, analysis);
    }

    if (seqn == "SM_mt") {
      auto & seq = seqs[fullseqn];

      seq.BuildModule(ic::CopyCollection<ic::Muon>("CopyToSelectedMuons",
          "muons", "sel_muons"));

      seq.BuildModule(ic::SimpleFilter<ic::Muon>("MuonFilter")
        .set_input_label("sel_muons").set_min(1)
        .set_predicate([=](ic::Muon const* m) {
          return  m->pt()                 > 20.    &&
                  fabs(m->eta())          < 2.1    &&
                  fabs(m->dxy_vertex())   < 0.045  &&
                  fabs(m->dz_vertex())    < 0.2    &&
                  MuonMediumHIPsafe(m);
        }));

      double tau_es_shift = 1.00;
      if (subseqn == "scale_t_lo") tau_es_shift = 0.97;
      if (subseqn == "scale_t_hi") tau_es_shift = 1.03;
      seq.BuildModule(ic::EnergyShifter<ic::Tau>("TauEnergyScale")
        .set_input_label("taus")
        .set_shift(tau_es_shift)
        .set_save_shifts(true));

      seq.BuildModule(ic::CopyCollection<ic::Tau>("CopyToSelectedTaus",
          "taus", "sel_taus"));

      seq.BuildModule(ic::SimpleFilter<ic::Tau>("TauFilter")
        .set_input_label("sel_taus").set_min(1)
        .set_predicate([=](ic::Tau const* t) {
          return  t->pt()                     > 20.    &&
                  fabs(t->eta())              < 2.3    &&
                  fabs(t->lead_dz_vertex())   < 0.2    &&
                  abs(t->charge())            == 1     &&
                  t->GetTauID("decayModeFinding") > 0.5;
        }));

      seq.BuildModule(ic::CompositeProducer<ic::Muon, ic::Tau>("MTPairProducer")
        .set_input_label_first("sel_muons")
        .set_input_label_second("sel_taus")
        .set_candidate_name_first("lepton1")
        .set_candidate_name_second("lepton2")
        .set_output_label("ditau")
      );

      using ROOT::Math::VectorUtil::DeltaR;
      seq.BuildModule(ic::SimpleFilter<ic::CompositeCandidate>("PairFilter")
          .set_input_label("ditau")
          .set_min(1)
          .set_predicate([=](ic::CompositeCandidate const* c) {
            return DeltaR(c->at(0)->vector(), c->at(1)->vector())
                > 0.5;
          })
      );

      // Extra lepton veto flags

      // Trigger flags

      // At this point we're done filtering, can calculate other things we nedd
      if (!is_data) {
        seq.BuildModule(puweight_module);
      } else {
        seq.BuildModule(lumimask_module);
      }

      // seq.BuildModule(ic::SMTreeProducer("SMTreeProducer")
      //   .set_channel(...)
      //   .set_fs(fs.at(fullseqn).get())
      //   .set_sf_workspace(sf_wsp)
      // );
      seq.InsertSequence(fullseqn, analysis);
    }

    if (seqn == "SM_et") {
      auto & seq = seqs[fullseqn];

      seq.BuildModule(ic::CopyCollection<ic::Electron>("CopyToSelectedElecs",
          "electrons", "sel_elecs"));

      seq.BuildModule(ic::SimpleFilter<ic::Electron>("ElectronFilter")
      .set_input_label("sel_elecs").set_min(1)
      .set_predicate([=](ic::Electron const* e) {
        return  e->pt()                 > 25.    &&
                fabs(e->eta())          < 2.1    &&
                fabs(e->dxy_vertex())   < 0.045  &&
                fabs(e->dz_vertex())    < 0.2    &&
                ElectronHTTIdSpring15(e, false);
      }));

      double tau_es_shift = 1.00;
      if (subseqn == "scale_t_lo") tau_es_shift = 0.97;
      if (subseqn == "scale_t_hi") tau_es_shift = 1.03;
      seq.BuildModule(ic::EnergyShifter<ic::Tau>("TauEnergyScale")
        .set_input_label("taus")
        .set_shift(tau_es_shift)
        .set_save_shifts(true));

      seq.BuildModule(ic::CopyCollection<ic::Tau>("CopyToSelectedTaus",
          "taus", "sel_taus"));

      seq.BuildModule(ic::SimpleFilter<ic::Tau>("TauFilter")
        .set_input_label("sel_taus").set_min(1)
        .set_predicate([=](ic::Tau const* t) {
          return  t->pt()                     > 20.    &&
                  fabs(t->eta())              < 2.3    &&
                  fabs(t->lead_dz_vertex())   < 0.2    &&
                  abs(t->charge())            == 1     &&
                  t->GetTauID("decayModeFinding") > 0.5;
        }));

      seq.BuildModule(ic::CompositeProducer<ic::Electron, ic::Tau>("ETPairProducer")
        .set_input_label_first("sel_elecs")
        .set_input_label_second("sel_taus")
        .set_candidate_name_first("lepton1")
        .set_candidate_name_second("lepton2")
        .set_output_label("ditau")
      );

      using ROOT::Math::VectorUtil::DeltaR;
      seq.BuildModule(ic::SimpleFilter<ic::CompositeCandidate>("PairFilter")
          .set_input_label("ditau")
          .set_min(1)
          .set_predicate([=](ic::CompositeCandidate const* c) {
            return DeltaR(c->at(0)->vector(), c->at(1)->vector())
                > 0.5;
          })
      );

      // At this point we're done filtering, can calculate other things we nedd
      if (!is_data) {
        seq.BuildModule(puweight_module);
      } else {
        seq.BuildModule(lumimask_module);
      }

      // seq.BuildModule(ic::SMTreeProducer("SMTreeProducer")
      //   .set_channel(...)
      //   .set_fs(fs.at(fullseqn).get())
      //   .set_sf_workspace(sf_wsp)
      // );
      seq.InsertSequence(fullseqn, analysis);
    }

    if (seqn == "SM_em") {
      auto & seq = seqs[fullseqn];

      seq.BuildModule(ic::CopyCollection<ic::Muon>("CopyToSelectedMuons",
          "muons", "sel_muons"));

      seq.BuildModule(ic::SimpleFilter<ic::Muon>("MuonFilter")
        .set_input_label("sel_muons").set_min(1)
        .set_predicate([=](ic::Muon const* m) {
          return  m->pt()                 > 10.    &&
                  fabs(m->eta())          < 2.4    &&
                  fabs(m->dxy_vertex())   < 0.045  &&
                  fabs(m->dz_vertex())    < 0.2    &&
                  MuonMediumHIPsafe(m);
        }));

      seq.BuildModule(ic::CopyCollection<ic::Electron>("CopyToSelectedElecs",
          "electrons", "sel_elecs"));

      seq.BuildModule(ic::SimpleFilter<ic::Electron>("ElectronFilter")
      .set_input_label("sel_elecs").set_min(1)
      .set_predicate([=](ic::Electron const* e) {
        return  e->pt()                 > 13.    &&
                fabs(e->eta())          < 2.5    &&
                fabs(e->dxy_vertex())   < 0.045  &&
                fabs(e->dz_vertex())    < 0.2    &&
                ElectronHTTIdSpring15(e, false);
      }));

      seq.BuildModule(ic::CompositeProducer<ic::Electron, ic::Muon>("EMPairProducer")
        .set_input_label_first("sel_elecs")
        .set_input_label_second("sel_muons")
        .set_candidate_name_first("lepton1")
        .set_candidate_name_second("lepton2")
        .set_output_label("ditau")
      );

      using ROOT::Math::VectorUtil::DeltaR;
      seq.BuildModule(ic::SimpleFilter<ic::CompositeCandidate>("PairFilter")
          .set_input_label("ditau")
          .set_min(1)
          .set_predicate([=](ic::CompositeCandidate const* c) {
            return DeltaR(c->at(0)->vector(), c->at(1)->vector())
                > 0.3;
          })
      );

      // Extra lepton veto flags

      // Trigger flags

      // At this point we're done filtering, can calculate other things we nedd
      if (!is_data) {
        seq.BuildModule(puweight_module);
      } else {
        seq.BuildModule(lumimask_module);
      }

      // seq.BuildModule(ic::SMTreeProducer("SMTreeProducer")
      //   .set_channel(...)
      //   .set_fs(fs.at(fullseqn).get())
      //   .set_sf_workspace(sf_wsp)
      // );
      seq.InsertSequence(fullseqn, analysis);
    }

    if (seqn == "SM_tt") {
      auto & seq = seqs[fullseqn];

      double tau_es_shift = 1.00;
      if (subseqn == "scale_t_lo") tau_es_shift = 0.97;
      if (subseqn == "scale_t_hi") tau_es_shift = 1.03;
      seq.BuildModule(ic::EnergyShifter<ic::Tau>("TauEnergyScale")
        .set_input_label("taus")
        .set_shift(tau_es_shift)
        .set_save_shifts(true));

      seq.BuildModule(ic::CopyCollection<ic::Tau>("CopyToSelectedTaus",
          "taus", "sel_taus"));

      seq.BuildModule(ic::SimpleFilter<ic::Tau>("TauFilter")
        .set_input_label("sel_taus").set_min(2)
        .set_predicate([=](ic::Tau const* t) {
          return  t->pt()                     > 40.    &&
                  fabs(t->eta())              < 2.1    &&
                  fabs(t->lead_dz_vertex())   < 0.2    &&
                  abs(t->charge())            == 1     &&
                  t->GetTauID("decayModeFinding") > 0.5;
        }));

      seq.BuildModule(ic::CompositeProducer<ic::Tau, ic::Tau>("TTPairProducer")
        .set_input_label_first("sel_taus")
        .set_input_label_second("sel_taus")
        .set_candidate_name_first("lepton1")
        .set_candidate_name_second("lepton2")
        .set_output_label("ditau")
      );

      using ROOT::Math::VectorUtil::DeltaR;
      seq.BuildModule(ic::SimpleFilter<ic::CompositeCandidate>("PairFilter")
          .set_input_label("ditau")
          .set_min(1)
          .set_predicate([=](ic::CompositeCandidate const* c) {
            return DeltaR(c->at(0)->vector(), c->at(1)->vector())
                > 0.5;
          })
      );

      // At this point we're done filtering, can calculate other things we nedd
      if (!is_data) {
        seq.BuildModule(puweight_module);
      } else {
        seq.BuildModule(lumimask_module);
      }

      // seq.BuildModule(ic::SMTreeProducer("SMTreeProducer")
      //   .set_channel(...)
      //   .set_fs(fs.at(fullseqn).get())
      //   .set_sf_workspace(sf_wsp)
      // );
      seq.InsertSequence(fullseqn, analysis);
    }

}

  analysis.RunAnalysis();

  if (seqs.count("HashMap")) {
    Json::StyledWriter writer;
    std::ofstream output;
    output.open(js.get("hash_map_output", "hash_map_out.json").asString());
    output << writer.write(js_hash_map);
    output.close();
  }

  return 0;
}

void BuildLeptonVetoes(Sequence & seq, unsigned max_muons, unsigned max_elecs) {
  seq.BuildModule(ic::CopyCollection<ic::Muon>("CopyToExtraMuons",
      "muons", "extra_muons"));

  seq.BuildModule(ic::HTTFilter<ic::Muon>("ExtraMuonFilter")
    .set_input_label("extra_muons")
    .set_veto_name("extra_muon_veto")
    .set_min(0)
    .set_max(max_muons)
    .set_no_filter(true)
    .set_predicate([](ic::Muon const* m) {
      return  m->pt()                 > 10.    &&
              fabs(m->eta())          < 2.4    &&
              fabs(m->dxy_vertex())   < 0.045  &&
              fabs(m->dz_vertex())    < 0.2    &&
              MuonMediumHIPsafe(m)             &&
              PF04IsolationVal(m, 0.5, 0) < 0.3;
    })
  );

  seq.BuildModule(ic::CopyCollection<ic::Electron>("CopyToExtraElecs",
      "electrons", "extra_elecs"));

  seq.BuildModule(ic::HTTFilter<ic::Electron>("ExtraElecFilter")
    .set_input_label("extra_elecs")
    .set_veto_name("extra_elec_veto")
    .set_min(0)
    .set_max(max_elecs)
    .set_no_filter(true)
    .set_predicate([](ic::Electron const* e) {
      return  e->pt()                 > 10.     &&
              fabs(e->eta())          < 2.5     &&
              fabs(e->dxy_vertex())   < 0.045   &&
              fabs(e->dz_vertex())    < 0.2     &&
              ElectronHTTIdSpring15(e, true)    &&
              PF03IsolationVal(e, 0.5,0) < 0.3;
    })
  );
}
void BuildDYStitching(Sequence & seq, Json::Value & js) {
  if (js.get("do_dyjets_stitching", false).asBool()) {
    auto jss = js["dyjets_stitching"];
    seq.BuildModule(ic::DYJetsStitching("DYJetsStitching")
      .Set_DYJetsToLL(jss["evt_DYJetsToLL"].asUInt(), jss["xs_DYJetsToLL"].asDouble())
      .Set_DY1JetsToLL(jss["evt_DY1JetsToLL"].asUInt(), jss["xs_DY1JetsToLL"].asDouble())
      .Set_DY2JetsToLL(jss["evt_DY2JetsToLL"].asUInt(), jss["xs_DY2JetsToLL"].asDouble())
      .Set_DY3JetsToLL(jss["evt_DY3JetsToLL"].asUInt(), jss["xs_DY3JetsToLL"].asDouble())
      .Set_DY4JetsToLL(jss["evt_DY4JetsToLL"].asUInt(), jss["xs_DY4JetsToLL"].asDouble()));
  }
}

void BuildWStitching(Sequence & seq, Json::Value & js) {
  if (js.get("do_wjets_stitching", false).asBool()) {
    auto jss = js["wjets_stitching"];
    seq.BuildModule(ic::WJetsStitching("WJetsStitching")
      .Set_WJetsToLNu(jss["evt_WJetsToLNu"].asUInt(), jss["xs_WJetsToLNu"].asDouble())
      .Set_W1JetsToLNu(jss["evt_W1JetsToLNu"].asUInt(), jss["xs_W1JetsToLNu"].asDouble())
      .Set_W2JetsToLNu(jss["evt_W2JetsToLNu"].asUInt(), jss["xs_W2JetsToLNu"].asDouble())
      .Set_W3JetsToLNu(jss["evt_W3JetsToLNu"].asUInt(), jss["xs_W3JetsToLNu"].asDouble())
      .Set_W4JetsToLNu(jss["evt_W4JetsToLNu"].asUInt(), jss["xs_W4JetsToLNu"].asDouble()));
  }
}
