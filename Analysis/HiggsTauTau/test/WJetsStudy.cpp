#include <iostream>
#include <vector>
#include <string>
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Core/interface/AnalysisBase.h"
#include "Utilities/interface/JsonTools.h"
#include "Utilities/interface/FnRootTools.h"
#include "HiggsTauTau/interface/WJetsStudy.h"

using std::string;
using std::vector;

int main(int argc, char* argv[]) {

  Json::Value js = ic::MergedJson(argc, argv);

  fwlite::TFileService* fs =
      new fwlite::TFileService((js["output"].asString()));

  vector<string> files = ic::ParseFileLines(js["filelist"].asString());
  vector<string> do_files;
  unsigned file_offset = js.get("file_offset", 0).asUInt();
  unsigned file_step = js.get("file_step", 1).asUInt();
  for (auto & f : files) f = js.get("file_prefix", std::string("")).asString() + f;

  for (unsigned i = file_offset; i < files.size(); i += file_step) {
    do_files.push_back(files[i]);
  }

  ic::AnalysisBase analysis("QCDStudy", do_files, "icEventProducer/EventTree",
                            js["max_events"].asInt64());
  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);
  analysis.CalculateTimings(js.get("timings", false).asBool());


  auto wjets_module = ic::WJetsStudy("WJetsStudy")
    .set_fs(fs)
    .set_genparticle_label("genParticles")
    .set_sample_name(js.get("sample_name", std::string("")).asString());


  analysis.AddModule(&wjets_module);

  analysis.RunAnalysis();

  delete fs;

  return 0;
}
