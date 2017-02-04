#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
// #include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
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
#include "HiggsTauTau/interface/HTTFutureSequence.h"
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
  vector<string> flatjsons;
  unsigned offset;
  unsigned nlines;

  po::options_description config("config");
  config.add_options()
      ("offset", po::value<unsigned>(&offset)->default_value(0))
      ("nlines", po::value<unsigned>(&nlines)->default_value(0))(
      "cfg", po::value<vector<string>>(&cfgs)->multitoken()->required(),
      "json config files")(
      "json", po::value<vector<string>>(&jsons)->multitoken(),
      "json fragments")(
      "flatjson", po::value<vector<string>>(&flatjsons)->multitoken(),
      "json flat fragments");
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
    std::vector<std::string> json_strs;
    Json::Value extra ;
    Json::Reader reader(Json::Features::all());
    reader.parse(jsons[i], extra);
    std::cout << ">> Updating config with fragment:\n";
    std::cout << extra;
    ic::UpdateJson(js_init, extra);
  }
  for (unsigned i = 0; i < flatjsons.size(); ++i) {
    Json::Value extra = ic::ExtractJsonFromFlatString(flatjsons[i]);
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
  vector<string> files;
  if(nlines != 0){
    vector<string> files_all = ic::ParseFileLines(js["job"]["filelist"].asString());
    for(unsigned k=0; k<nlines; k++){
      if((offset*nlines)+k < files_all.size()){
        files.push_back(files_all.at((offset*nlines)+k));
      }
    }
  } else files = ic::ParseFileLines(js["job"]["filelist"].asString());
      
  for (auto & f : files) f = js["job"]["file_prefix"].asString() + f;

  AnalysisBase analysis("HiggsTauTau", files, "icEventProducer/EventTree",
                        js["job"]["max_events"].asInt64());
  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);
//  analysis.DoSkimming("./skim/");
  analysis.CalculateTimings(js["job"]["timings"].asBool());
  
  std::map<std::string, ic::HTTFutureSequence> seqs;
  std::vector<std::string> ignore_chans;

  for(unsigned i = 0; i<js["job"]["ignore_channels"].size();++i){
   ignore_chans.push_back(js["job"]["ignore_channels"][i].asString());
  }

  std::string output_name = js["sequence"]["output_name"].asString();
  bool is_data = js["sequence"]["is_data"].asBool();
  for (unsigned i = 0; i < js["job"]["channels"].size(); ++i) {
    std::string channel_str = js["job"]["channels"][i].asString();
    if(is_data &&  ( (channel_str.find("em") != channel_str.npos && output_name.find("MuonEG")==output_name.npos )|| (channel_str.find("mt") != channel_str.npos && output_name.find("SingleMuon") == output_name.npos ) || (channel_str.find("et") != channel_str.npos && output_name.find("SingleEle") == output_name.npos ) || (channel_str.find("tt") != channel_str.npos && output_name.find("Tau") == output_name.npos))) continue; 

    bool ignore_channel =false;
    bool duplicate_channel = false;
    for(unsigned k = 0; k<ignore_chans.size();k++){
      if(ignore_chans.at(k)==channel_str){ignore_channel=true;}
    }
    if(ignore_channel){
      std::cout<<"SKIPPING CHANNEL "<<channel_str<<std::endl;
      continue;
    } 
    for(unsigned k=0;k<js["job"]["channels"].size();++k){
      if(k!=i&&js["job"]["channels"][k].asString()==js["job"]["channels"][i].asString()){duplicate_channel=true;}
    }
    if(duplicate_channel){
      std::cout<<"Channel "<<js["job"]["channels"][i].asString()<<" sequences already created, skipping"<<std::endl; 
      continue;
    }
    std::vector<std::string> vars;
    for (unsigned j = 0; j < js["job"]["sequences"]["all"].size(); ++j) {
      vars.push_back(js["job"]["sequences"]["all"][j].asString());
    }
    for (unsigned j = 0; j < js["job"]["sequences"][channel_str].size(); ++j) {
      vars.push_back(js["job"]["sequences"][channel_str][j].asString());
    }


    for (unsigned j = 0; j < vars.size(); ++j) {
      std::string seq_str = channel_str+"_"+vars[j];
      Json::Value js_merged = js["sequence"];
      ic::UpdateJson(js_merged, js["channels"][channel_str]);
      ic::UpdateJson(js_merged, js["sequences"][vars[j]]);
      // std::cout << js_merged;
      seqs[seq_str] = ic::HTTFutureSequence(channel_str,std::to_string(offset),js_merged);
      seqs[seq_str].BuildSequence();
      ic::HTTFutureSequence::ModuleSequence seq_run = *(seqs[seq_str].getSequence());
      for (auto m : seq_run) analysis.AddModule(seq_str, m.get());
    }
  }

  analysis.RunAnalysis();

  return 0;
}
