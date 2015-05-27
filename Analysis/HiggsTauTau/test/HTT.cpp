#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
// #include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
// #include "boost/bind.hpp"
// #include "boost/function.hpp"
// #include "boost/format.hpp"
#include "TSystem.h"
#include "Utilities/interface/json.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Utilities/interface/JsonTools.h"
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
  std::string output_folder = "";
  

  /*
    This should really be shortened into one inline function in the AnalysisBase
    declaration. GetPrefixedFilelist(prefix, filelist)
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

  ic::HTTSequence sequence_builder;
  std::map<std::string, ic::HTTSequence::ModuleSequence> seqs;
  std::vector<fwlite::TFileService *> tfileservices;
  fwlite::TFileService *fs1;
  fwlite::TFileService *fs2;
  fwlite::TFileService *fs3;
  fwlite::TFileService *fs4;
  fwlite::TFileService *fs5;
  fwlite::TFileService *fs6;
  fwlite::TFileService *fs7;
  fwlite::TFileService *fs8;
  fwlite::TFileService *fs9;
  fwlite::TFileService *fs10;
  fwlite::TFileService *fs11;
  fwlite::TFileService *fs12;
  fwlite::TFileService *fs13;
  fwlite::TFileService *fs14;
  fwlite::TFileService *fs15;
  fwlite::TFileService *fs16;
  fwlite::TFileService *fs17;
  fwlite::TFileService *fs18;
  fwlite::TFileService *fs19;
  fwlite::TFileService *fs20;
  fwlite::TFileService *fs21;
  fwlite::TFileService *fs22;
  fwlite::TFileService *fs23;
  tfileservices.push_back(fs1);
  tfileservices.push_back(fs2);
  tfileservices.push_back(fs3);
  tfileservices.push_back(fs4);
  tfileservices.push_back(fs5);
  tfileservices.push_back(fs6);
  tfileservices.push_back(fs7);
  tfileservices.push_back(fs8);
  tfileservices.push_back(fs9);
  tfileservices.push_back(fs10);
  tfileservices.push_back(fs11);
  tfileservices.push_back(fs12);
  tfileservices.push_back(fs13);
  tfileservices.push_back(fs14);
  tfileservices.push_back(fs15);
  tfileservices.push_back(fs16);
  tfileservices.push_back(fs17);
  tfileservices.push_back(fs18);
  tfileservices.push_back(fs19);
  tfileservices.push_back(fs20);
  tfileservices.push_back(fs21);
  tfileservices.push_back(fs22);
  tfileservices.push_back(fs23);
  unsigned fileservicesize=0;

 for (unsigned i = 0; i < js["job"]["channels"].size(); ++i) {
    std::string channel_str = js["job"]["channels"][i].asString();
    std::vector<std::string> fsvars;
    for (unsigned j = 0; j < js["job"]["sequences"]["all"].size(); ++j) {
      fsvars.push_back(js["job"]["sequences"]["all"][j].asString());
    }
    for (unsigned j = 0; j < js["job"]["sequences"][channel_str].size(); ++j) {
      fsvars.push_back(js["job"]["sequences"][channel_str][j].asString());
    }
  
    for(unsigned j=0; j < fsvars.size(); ++j){
     tfileservices.at(fileservicesize) = new fwlite::TFileService((output_folder+channel_str+"_"+fsvars[j]+".root").c_str());
     fileservicesize++;
    }
 }
    

  unsigned  fsit=0;
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
      ic::HTTSequence::ModuleSequence& seq = seqs[seq_str];
      ic::channel channel = ic::String2Channel(channel_str);
      Json::Value js_merged = js["sequence"];
      ic::UpdateJson(js_merged, js["channels"][channel_str]);
      ic::UpdateJson(js_merged, js["sequences"][vars[j]]);
      // std::cout << js_merged;
      sequence_builder.BuildSequence(&seq, channel, tfileservices.at(fsit) ,js_merged);
      fsit++;
      for (auto m : seq) analysis.AddModule(seq_str, m.get());
    }
  }

  analysis.RunAnalysis();
  for(unsigned j = 0; j<fileservicesize; ++j){
    delete tfileservices.at(j);
  }

  return 0;
}
