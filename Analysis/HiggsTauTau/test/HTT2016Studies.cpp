#include <iostream>
#include <vector>
#include <string>
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Core/interface/AnalysisBase.h"
#include "Utilities/interface/JsonTools.h"
#include "Utilities/interface/FnRootTools.h"
#include "HiggsTauTau/interface/WJetsStudy.h"
#include "Modules/interface/LumiMask.h"
#include "Modules/interface/GenericModule.h"
#include "Modules/interface/TriggerInfo.h"
#include "Objects/interface/Unhash.h"

using std::string;
using std::vector;

int main(int argc, char* argv[]) {


  Json::Value js = ic::MergedJson(argc, argv);

  Json::Value js_unhash = ic::ExtractJsonFromFile("hash_map.json");
  std::map<std::size_t, std::string> unhash_map;
  for (auto val : js_unhash.getMemberNames()) {
    unhash_map[boost::lexical_cast<std::size_t>(val)] = js_unhash[val].asString();
  }
  ic::Unhash::SetMap(unhash_map);

  fwlite::TFileService fs((js["output"].asString()));

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

  std::string tree_name = "icEventProducer/EventTree";
  if (js["hash_map_mode"].asBool()) {
    tree_name = "icHashTreeProducer/HashTree";
  }

  ic::AnalysisBase analysis("HTT2016Studies", do_files, tree_name,
                            js["max_events"].asInt64());
  analysis.SetTTreeCaching(true);
  analysis.StopOnFileFailure(true);
  analysis.RetryFileAfterFailure(7, 3);
  analysis.CalculateTimings(js.get("timings", false).asBool());

  auto lumimask_module = ic::LumiMask("LumiMask")
    .set_input_file(js.get("lumi_mask", "").asString())
    .set_produce_output_jsons(js.get("lumi_out", "lumi_out").asString());

  auto trigger_module = ic::TriggerInfo("TriggerInfo")
    .set_output_file(js.get("trigger_info_output", "trigger_info.json").asString());


  if (js["hash_map_mode"].asBool()) {
    Json::Value js_hash_map;
    auto hash_map_writer = ic::GenericModule("HashMapWriter")
      .set_function([&](ic::TreeEvent * event) {
        std::string id = boost::lexical_cast<std::string>(*event->GetPtr<ULong64_t>("id"));
        std::string str = *(event->GetPtr<std::string>("string"));
        js_hash_map[id] = str;
        return 0;
      });
    analysis.AddModule(&hash_map_writer);
    analysis.RunAnalysis();
    Json::StyledWriter writer;
    std::ofstream output;
    output.open("hash_map.json");
    output << writer.write(js_hash_map);
    output.close();
  } else {
    analysis.AddModule(&lumimask_module);
    analysis.AddModule(&trigger_module);
    analysis.RunAnalysis();
  }

  return 0;
}
