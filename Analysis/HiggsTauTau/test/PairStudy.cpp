#include <iostream>
#include <vector>
#include <string>
#include "TSystem.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Utilities/interface/JsonTools.h"
#include "Utilities/interface/FnRootTools.h"
#include "Core/interface/AnalysisBase.h"
#include "HiggsTauTau/interface/HTTConfig.h"
#include "HiggsTauTau/interface/HTTGenEvent.h"
#include "HiggsTauTau/interface/HTTPairStudy.h"
#include "Modules/interface/JetEnergyCorrections.h"

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


  auto httGenEvent = ic::HTTGenEvent("HttGenEvent")
      .set_genparticle_label("genParticlesTaus");

  auto et_tight = ic::HTTPairStudy("ETTight")
                      .set_channel(ic::channel::et)
                      .set_do_loose(false);

  auto et_loose = ic::HTTPairStudy("ETLoose")
                      .set_channel(ic::channel::et)
                      .set_do_loose(true);

  auto mt_tight = ic::HTTPairStudy("MTTight")
                      .set_channel(ic::channel::mt)
                      .set_do_loose(false);

  auto mt_loose = ic::HTTPairStudy("MTLoose")
                      .set_channel(ic::channel::mt)
                      .set_do_loose(true);

  analysis.AddModule(&httGenEvent);
  analysis.AddModule(&et_tight);
  analysis.AddModule(&et_loose);
  analysis.AddModule(&mt_tight);
  analysis.AddModule(&mt_loose);
  analysis.RunAnalysis();

  delete fs;

  return 0;
}
