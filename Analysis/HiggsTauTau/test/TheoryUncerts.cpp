#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include "Utilities/interface/json.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Utilities/interface/JsonTools.h"
#include "Utilities/interface/FnRootTools.h"
#include "Core/interface/AnalysisBase.h"
#include "HiggsTauTau/interface/TheoryWeights.h"

using namespace std;

int main(int argc, char* argv[]) {
  auto js = ic::MergedJson(argc, argv);

  // Create ROOT output fileservice
  fwlite::TFileService* fs =
      new fwlite::TFileService(js["output"].asString());

  vector<string> files;
  if (js.isMember("filelist")) {
    files = ic::ParseFileLines(js["filelist"].asString());
  }
  if (js.isMember("files")) {
    for (auto const& f : js["files"]) {
      files.push_back(f.asString());
    }
  }

  ic::AnalysisBase analysis("TheoryUncerts", files, "icEventProducer/EventTree",
                            js.get("max_events", -1).asInt());
  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);

  auto theoryWeights = ic::TheoryWeights("TheoryWeights").set_fs(fs);
  analysis.AddModule("theory", &theoryWeights);

  analysis.RunAnalysis();

  delete fs;

  return 0;
}
