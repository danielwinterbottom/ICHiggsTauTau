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
#include "Modules/interface/GenericModule.h"
#include "HGCAL/interface/HGCALAnalysis.h"
#include "HGCAL/interface/TauReco.h"

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

  vector<string> files;
  for (auto const& filelist : js["filelists"]) {
    auto i_files = ic::ParseFileLines(filelist.asString());
    files.insert(files.end(), i_files.begin(), i_files.end());
  }
  vector<string> do_files;
  unsigned file_offset = js.get("file_offset", 0).asUInt();
  unsigned file_step = js.get("file_step", 1).asUInt();
  for (unsigned i = file_offset; i < files.size(); i += file_step) {
    do_files.push_back(js.get("file_prefix", "").asString() + files[i]);
  }

  std::string outputdir = "./output";
  std::map<std::string, std::shared_ptr<fwlite::TFileService>> fs;
  for (auto const& seq : {"Main"}) {
    fs[seq] = std::make_shared<fwlite::TFileService>(
        outputdir + "/" + seq + "/" + js["output_name"].asString());
    // fs[seq.asString()] = fwlite::TFileService(js["output_dir"].asString() +
    //                                           "/" + seq.asString() + "/" +
    //                                           js["output_name"].asString());
  }

  ic::AnalysisBase analysis("HGCAL", do_files, "ana/hgc", js["max_events"].asInt64());
  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);


  Sequence main_seq;

  main_seq.BuildModule(ic::HGCALObjectBuilder("HGCALObjectBuilder"));
  main_seq.BuildModule(ic::TauReco("TauReco")
    .set_fs(fs.at("Main").get())
    .set_s1_rechit_threshold(js["s1_rechit_threshold"].asDouble())
    .set_s1_pu_strategy(js["s1_pu_strategy"].asInt())
    .set_s2_jet_distance(js["s2_jet_distance"].asDouble())
    .set_s2_min_jet_pt(js["s2_min_jet_pt"].asDouble())
    .set_s3_min_surrounding_hits(js["s3_min_surrounding_hits"].asInt())
    .set_s3_min_lower_energy_hits(js["s3_min_lower_energy_hits"].asInt())
    .set_s3_use_hcal_dxy(js["s3_use_hcal_dxy"].asBool())
    .set_s3_hcal_dxy(js["s3_hcal_dxy"].asDouble())
    .set_s3_use_single_hit(js["s3_use_single_hit"].asBool())
    .set_s3_single_hit_min(js["s3_single_hit_min"].asDouble())
    .set_s4_hit_merge_dr(js["s4_hit_merge_dr"].asDouble())
    .set_s4_min_hits_for_prong(js["s4_min_hits_for_prong"].asUInt())
    .set_s5_merge_strategy(js["s5_merge_strategy"].asInt())
    .set_s5_exp_merge_scale(js["s5_exp_merge_scale"].asDouble())
    );

  /*
  main_seq.BuildModule(ic::TauReco("TauRecoR0p4")
    .set_fs(fs.at("Main").get())
    .set_s1_rechit_threshold(js["s1_rechit_threshold"].asDouble())
    .set_s1_pu_strategy(js["s1_pu_strategy"].asInt())
    .set_s2_jet_distance(0.4)
    .set_s2_min_jet_pt(js["s2_min_jet_pt"].asDouble())
    .set_s3_min_surrounding_hits(js["s3_min_surrounding_hits"].asInt())
    .set_s3_min_lower_energy_hits(js["s3_min_lower_energy_hits"].asInt())
    .set_s3_use_hcal_dxy(js["s3_use_hcal_dxy"].asBool())
    .set_s3_hcal_dxy(js["s3_hcal_dxy"].asDouble())
    .set_s3_use_single_hit(js["s3_use_single_hit"].asBool())
    .set_s3_single_hit_min(js["s3_single_hit_min"].asDouble())
    .set_s4_hit_merge_dr(js["s4_hit_merge_dr"].asDouble())
    .set_s4_min_hits_for_prong(js["s4_min_hits_for_prong"].asUInt())
    .set_s5_merge_strategy(js["s5_merge_strategy"].asInt())
    .set_s5_exp_merge_scale(js["s5_exp_merge_scale"].asDouble())
    .set_postfix("_r0p4")
    );
 */
  main_seq.BuildModule(ic::HGCALTest("HGCALTest")
    .set_fs(fs.at("Main").get())
    .set_do_fakes(js.get("do_fakes", false).asBool())
    );

  main_seq.InsertSequence("Main", analysis);

  analysis.RunAnalysis();
}
