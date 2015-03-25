#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include "boost/program_options.hpp"
#include "TSystem.h"
#include "Utilities/interface/json.h"
// #include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Utilities/interface/JsonTools.h"
#include "Utilities/interface/FnRootTools.h"
#include "Utilities/interface/FnPredicates.h"
#include "Core/interface/AnalysisBase.h"
#include "Modules/interface/SimpleFilter.h"
#include "Modules/interface/CompositeProducer.h"
#include "HiggsTauTau/interface/HTTConfig.h"
#include "HiggsTauTau/interface/HTTGenEvent.h"
#include "HiggsTauTau/interface/HTTGenEventPlots.h"
#include "Modules/interface/JetEnergyCorrections.h"

namespace po = boost::program_options;
using std::string;
using std::vector;

using ic::AnalysisBase;
using std::vector;
using std::string;
using ic::Electron;

int main(int argc, char* argv[]) {
  // Shorten: write a function that does this, or move the classes into Analysis
  // gSystem->Load("libFWCoreFWLite.dylib");
  // gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  // AutoLibraryLoader::enable();

  vector<string> cfgs;
  vector<string> jsons;

  po::options_description config("config");
  config.add_options()(
      "cfg", po::value<vector<string>>(&cfgs)->multitoken()->required(),
      "json config files")(
      "json", po::value<vector<string>>(&jsons)->multitoken(),
      "json fragments");
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(config).run(), vm);
  po::notify(vm);

  Json::Value js_init = ic::ExtractJsonFromFile(cfgs[0]);
  for (unsigned i = 1; i < cfgs.size(); ++i) {
    std::cout << ">> Updating config with file " << cfgs[i] << ":\n";
    Json::Value extra = ic::ExtractJsonFromFile(cfgs[i]);
    std::cout << extra;
    ic::UpdateJson(js_init, extra);
  }
  for (unsigned i = 0; i < jsons.size(); ++i) {
    Json::Value extra;
    Json::Reader reader(Json::Features::all());
    reader.parse(jsons[i], extra);
    std::cout << ">> Updating config with fragment:\n";
    std::cout << extra;
    ic::UpdateJson(js_init, extra);
}

  Json::Value const js = js_init;

  // Create ROOT output fileservice
  fwlite::TFileService* fs =
      new fwlite::TFileService((js["job"]["output_folder"].asString() +
                                js["job"]["output_name"].asString()).c_str());

  /*
    This should really be shortened into one inline function in the AnalysisBase
    declaration. GetPrefixedFilelist(prefix, filelist)
  */
  vector<string> files = {js["job"]["file"].asString()};

  AnalysisBase analysis("HiggsTauTau", files, "icEventProducer/EventTree", -1);
  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);

  auto httGenEvent = ic::HTTGenEvent("HttGenEvent")
      .set_genparticle_label("genParticles")
      .set_is_pythia8(true);

  auto httGenEventPlots = ic::HTTGenEventPlots("HttGenEventPlots").set_fs(fs);
  analysis.AddModule(&httGenEvent);
  analysis.AddModule(&httGenEventPlots);

  analysis.RunAnalysis();

  delete fs;

  return 0;
}
