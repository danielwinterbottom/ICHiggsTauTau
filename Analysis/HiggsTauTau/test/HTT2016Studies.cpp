#include <iostream>
#include <vector>
#include <string>
#include <sys/stat.h>
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
#include "HiggsTauTau/interface/WJetsStudy.h"
#include "HiggsTauTau/interface/HTT2016Studies.h"
#include "HiggsTauTau/interface/EffectiveEvents.h"


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

int main(int argc, char* argv[]) {
  Json::Value js = ic::MergedJson(argc, argv);


  bool is_data = js.get("is_data", false).asBool();

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

  fwlite::TFileService fs((js["output"].asString()));

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
  TH1D m_pu = ic::OpenFromTFile<TH1D>(js["mc_pu"].asString());

  auto puweight_module = ic::PileupWeight("PileupWeight")
    .set_data(&d_pu)
    .set_mc(&m_pu)
    .set_print_weights(false);

  auto trigger_module = ic::TriggerInfo("TriggerInfo")
    .set_output_file(js.get("trigger_info_output", "trigger_info.json").asString());

  if (seqs.count("TriggerInfo") && is_data) {
    auto & seq = seqs["TriggerInfo"];

    seq.BuildModule(ic::LumiMask(lumimask_module)
      .set_produce_output_jsons(js.get("lumi_out", "lumi_out").asString()));
    seq.BuildModule(ic::TriggerInfo("TriggerInfo")
    .set_output_file(js.get("trigger_info_output", "trigger_info.json").asString()));
    seq.InsertSequence("TriggerInfo", analysis);
  }

  Json::Value js_hash_map;
  if (seqs.count("HashMap")) {
    seqs["HashMap"].BuildModule(ic::GenericModule("HashMapWriter")
      .set_function([&](ic::TreeEvent * event) {
        std::string id = boost::lexical_cast<std::string>(*event->GetPtr<ULong64_t>("id"));
        std::string str = *(event->GetPtr<std::string>("string"));
        js_hash_map[id] = str;
        return 0;
      }));
    seqs["HashMap"].InsertSequence("HashMap", analysis);
  }

  if (seqs.count("EffectiveEvents")) {
    auto & seq = seqs["EffectiveEvents"];
    if (!is_data) {
      seq.BuildModule(ic::EffectiveEvents("EffectiveEvents")
        .set_fs(&fs));
    }
    seq.InsertSequence("EffectiveEvents", analysis);
  }

  if (seqs.count("Zmm")) {
    auto & seq = seqs["Zmm"];

    seq.BuildModule(ic::CopyCollection<ic::Muon>("CopyToSelectedMuons",
        "muons", "sel_muons"));

    seq.BuildModule(ic::SimpleFilter<ic::Muon>("MuonFilter")
    .set_input_label("sel_muons").set_min(2)
    .set_predicate([=](ic::Muon const* m) {
      return  m->pt()                 > 10.    &&
              fabs(m->eta())          < 2.1    &&
              fabs(m->dxy_vertex())   < 0.045  &&
              fabs(m->dz_vertex())    < 0.2    &&
              MuonMediumHIPsafe(m);
    }));

    seq.BuildModule(ic::OneCollCompositeProducer<ic::Muon>("ZMMPairProducer")
        .set_input_label("sel_muons")
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
              > 0.5;
        })
    );

    // At this point we're done filtering, can calculate other things we nedd
    if (!is_data) {
      seq.BuildModule(puweight_module);
    } else {
      seq.BuildModule(lumimask_module);
    }

    seq.BuildModule(ic::ZmmTreeProducer("ZmmTreeProducer")
      .set_fs(&fs)
      .set_sf_workspace("input/scale_factors/scalefactors_2016_v1.root")
    );
    seq.InsertSequence("Zmm", analysis);
  }

  if (seqs.count("Zee")) {
    auto & seq = seqs["Zee"];

    seq.BuildModule(ic::CopyCollection<ic::Electron>("CopyToSelectedElecs",
        "electrons", "sel_elecs"));

    seq.BuildModule(ic::SimpleFilter<ic::Electron>("ElectronFilter")
    .set_input_label("sel_elecs").set_min(2)
    .set_predicate([=](ic::Electron const* e) {
      return  e->pt()                 > 26.    &&
              fabs(e->eta())          < 2.1    &&
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
      .set_fs(&fs)
      .set_sf_workspace("input/scale_factors/scalefactors_2016_v1.root")
    );
    seq.InsertSequence("Zee", analysis);
  }

  if (seqs.count("ZmmTP")) {
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

    seq.BuildModule(ic::SimpleFilter<ic::Muon>("ProbeMuonFilter")
    .set_input_label("probe_muons").set_min(1)
    .set_predicate([=](ic::Muon const* m) {
      return  m->pt()                 > 10.    &&
              fabs(m->eta())          < 2.4    &&
              m->is_tracker();
    }));

    seq.BuildModule(ic::CompositeProducer<ic::Muon, ic::Muon>("ZMMPairProducer")
        .set_input_label_first("tag_muons")
        .set_input_label_second("probe_muons")
        .set_candidate_name_first("tag")
        .set_candidate_name_second("probe")
        .set_output_label("dimuon")
    );

    using ROOT::Math::VectorUtil::DeltaR;
    seq.BuildModule(ic::SimpleFilter<ic::CompositeCandidate>("PairFilter")
        .set_input_label("dimuon")
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


    seq.BuildModule(ic::ZmmTPTreeProducer("ZmmTPTreeProducer")
      .set_fs(&fs)
      .set_sf_workspace("input/scale_factors/scalefactors_2016_v1.root")
    );
    seq.InsertSequence("ZmmTP", analysis);
  }

  if (seqs.count("ZeeTP")) {
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
      .set_fs(&fs)
      .set_sf_workspace("input/scale_factors/scalefactors_2016_v1.root")
    );
    seq.InsertSequence("ZeeTP", analysis);
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
