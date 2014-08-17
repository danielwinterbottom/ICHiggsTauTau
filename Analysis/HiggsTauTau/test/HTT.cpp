#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
// #include "boost/lexical_cast.hpp"
// #include "boost/program_options.hpp"
// #include "boost/bind.hpp"
// #include "boost/function.hpp"
// #include "boost/format.hpp"
#include "TSystem.h"
#include "Utilities/interface/json.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
// #include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Utilities/interface/FnRootTools.h"
#include "Utilities/interface/FnPredicates.h"
#include "Core/interface/AnalysisBase.h"
// #include "Modules/interface/CopyCollection.h"
#include "Modules/interface/SimpleFilter.h"
// #include "Modules/interface/OverlapFilter.h"
// #include "Modules/interface/CheckEvents.h"
#include "Modules/interface/CompositeProducer.h"
#include "HiggsTauTau/interface/HTTSequence.h"
#include "HiggsTauTau/interface/HTTConfig.h"

// using boost::lexical_cast;
// using boost::bind;
// namespace po = boost::program_options;
// using std::string;
// using std::vector;
using namespace ic;

using ic::AnalysisBase;
using std::vector;
using std::string;
using ic::Electron;

Json::Value ExtractJson(std::string const& file) {
  Json::Value js;
  Json::Reader json_reader;
  std::fstream input;
  input.open(file);
  json_reader.parse(input, js);
  return js;
}

void UpdateJson(Json::Value& a, Json::Value const& b) {
  if (!a.isObject() || !b.isObject()) return;

  for (const auto& key : b.getMemberNames()) {
    if (a[key].isObject()) {
      UpdateJson(a[key], b[key]);
    } else {
      a[key] = b[key];
    }
  }
}

int main(int argc, char* argv[]) {
  // Shorten: write a function that does this, or move the classes into Analysis
  gSystem->Load("libFWCoreFWLite.dylib");
  gSystem->Load("libUserCodeICHiggsTauTau.dylib");
  AutoLibraryLoader::enable();

  // In the end will recursively merge json here
  if (argc < 2) return 1;
  Json::Value js_init = ExtractJson(argv[1]);
  if (argc > 2) {
    for (int i = 2; i < argc; ++i) {
      Json::Value extra;
      Json::Reader reader(Json::Features::all());
      reader.parse(argv[i], extra);
      std::cout << extra;
      UpdateJson(js_init, extra);
    }
  }

  Json::Value const js = js_init;

  /*
    This should really be shortened into one inline function in the AnalysisBase declaration. GetPrefixedFilelist(prefix, filelist)
  */
  vector<string> files = ic::ParseFileLines(js["job"]["filelist"].asString());
  for (auto & f : files) f = js["job"]["file_prefix"].asString() + f;

  AnalysisBase analysis("HiggsTauTau", files, "icEventProducer/EventTree",
                        js["job"]["max_events"].asInt64());
  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);
  // analysis.DoSkimming("./skim/");
  analysis.CalculateTimings(js["job"]["timings"].asBool());

  HTTSequence sequence_builder;
  std::map<std::string, HTTSequence::ModuleSequence> seqs;

  for (unsigned i = 0; i < js["job"]["channels"].size(); ++i) {
    std::string channel_str = js["job"]["channels"][i].asString();
    std::vector<std::string> vars;
    for (unsigned j = 0; j < js["job"]["sequences"]["all"].size(); ++j) {
      vars.push_back(js["job"]["sequences"]["all"][j].asString());
    }
    for (unsigned j = 0; j < js["job"]["sequences"][channel_str].size(); ++j) {
      vars.push_back(js["job"]["sequences"][channel_str][j].asString());
    }

    for (unsigned j = 0; j < vars.size(); ++j) {
      std::string seq_str = channel_str+"_"+vars[j];
      HTTSequence::ModuleSequence& seq = seqs[seq_str];
      ic::channel channel = String2Channel(channel_str);
      Json::Value js_merged = js["sequence"];
      UpdateJson(js_merged, js["channels"][channel_str]);
      UpdateJson(js_merged, js["channels"][vars[j]]);
      std::cout << js_merged;
      sequence_builder.BuildSequence(&seq, channel, js_merged);
      for (auto m : seq) analysis.AddModule(seq_str, m.get());
    }

  }


  analysis.RunAnalysis();

  return 0;
}
