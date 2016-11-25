#include "Modules/interface/LumiMask.h"
#include "Utilities/interface/JsonTools.h"
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "boost/lexical_cast.hpp"
#include <fstream>

namespace ic {

LumiMask::LumiMask(std::string const& name) : ModuleBase(name) {
  produce_output_jsons_ = "";
}

LumiMask::~LumiMask() { ; }

int LumiMask::PreAnalysis() {
  PrintHeader("LumiMask");
  if (input_file_ != "") {
    PrintArg("input_file", input_file_);
    Json::Value js = ExtractJsonFromFile(input_file_);
    FillJsonMapFromJson(input_json_, js);
  } else {
    PrintArg("input_file", "-");
  }
  if (produce_output_jsons_ != "") {
    PrintArg("produce_output",
             produce_output_jsons_ + "{_all,_accept_,reject}.json");
  } else {
    PrintArg("produce_output", false);
  }
  return 0;
}

int LumiMask::Execute(TreeEvent* event) {
  EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
  unsigned run = eventInfo->run();
  unsigned ls = eventInfo->lumi_block();
  all_json_[run].insert(ls);
  bool accept = false;
  if (input_json_.find(run) != input_json_.end()) {
    if (input_json_[run].find(ls) != input_json_[run].end()) accept = true;
  }
  // If no input json file was loaded we will also accept this event
  if (input_file_ == "") {
    accept = true;
  }
  if (accept) {
    accept_json_[run].insert(ls);
    return 0;
  } else {
    reject_json_[run].insert(ls);
    return 1;
  }

  return 0;
}
int LumiMask::PostAnalysis() {
  if (produce_output_jsons_ != "") {
    std::ofstream output;
    output.open((produce_output_jsons_ + "_all.json").c_str());
    WriteJson(all_json_, output);
    output.close();
    output.open((produce_output_jsons_ + "_accept.json").c_str());
    WriteJson(accept_json_, output);
    output.close();
    output.open((produce_output_jsons_ + "_reject.json").c_str());
    WriteJson(reject_json_, output);
    output.close();
  }
  return 0;
}

void LumiMask::FillJsonMapFromJson(JsonMap & jsmap, Json::Value const& js) {
  for (auto const& key : js.getMemberNames()) {
    Json::Value const& run_js = js[key];
    unsigned run = boost::lexical_cast<unsigned>(key);
    for (unsigned i = 0; i < run_js.size(); ++i) {
      if (run_js[i].size() != 2) {
        throw std::runtime_error(
            "[LumiMask] Lumi range not in the form [X,Y]");
      }
      unsigned range_min = run_js[i][0].asUInt();
      unsigned range_max = run_js[i][1].asUInt();
      if (range_max < range_min) {
        throw std::runtime_error(
            "[LumiMask] Have lumi range [X,Y] where Y < X");
      }
      for (unsigned i = range_min; i <= range_max; ++i) {
          jsmap[run].insert(i);
      }
    }
  }
}

Json::Value LumiMask::JsonFromJsonMap(JsonMap const& jsmap) {
  Json::Value js;
  for (auto const& info : jsmap) {
    auto const& run = boost::lexical_cast<std::string>(info.first);
    auto const& lumis = info.second;
    if (lumis.size() == 0) continue;
    auto it = lumis.begin();
    unsigned first_val = *it;
    unsigned last_val = *it;
    while (it != lumis.end()) {
      ++it;
      if (it != lumis.end() && *it == last_val + 1) {
        last_val = *it;
      } else {
        Json::Value lumi_pair;
        lumi_pair.append(first_val);
        lumi_pair.append(last_val);
        js[run].append(lumi_pair);
        if (it != lumis.end()) {
          first_val = *it;
          last_val = *it;
        }
      }
    }
  }
  return js;
}

void LumiMask::WriteJson(JsonMap const& json, std::ofstream& output) {
  Json::StyledWriter writer;
  output << writer.write(JsonFromJsonMap(json));
}

void LumiMask::PrintInfo() { ; }
}
