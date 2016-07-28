#include <iostream>
#include <vector>
#include <string>
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Core/interface/AnalysisBase.h"
#include "Utilities/interface/JsonTools.h"
#include "Utilities/interface/FnRootTools.h"
#include "HiggsTauTau/interface/WJetsStudy.h"
#include "HiggsTauTau/interface/HTTWeights.h"


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

  ic::AnalysisBase analysis("GenWeightStudy", do_files, "icEventProducer/EventTree",
                            js["max_events"].asInt64());
  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);
  analysis.CalculateTimings(js.get("timings", false).asBool());

  ic::HTTWeights httWeights = ic::HTTWeights("HTTWeights")
    .set_jets_label("ak4PFJetsCHS")
    .set_ditau_label("ditau")
    .set_fs(fs);

  httWeights.set_do_dy_soup_high_mass(true);
  httWeights.SetDYInputCrossSectionsHighMass(4954, 1012.5, 332.8, 101.8,54.8,6.7); //Target fractions are xs_n-jet/xs_inclusive
  httWeights.SetDYInputYieldsHighMass(239058696,65314144 , 20019059, 5701878, 4189017, 6079415);

  analysis.AddModule(&httWeights);

  analysis.RunAnalysis();

  delete fs;

  return 0;
}
