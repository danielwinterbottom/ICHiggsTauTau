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
  std::cout << "** PreAnalysis Info for LumiMask **" << std::endl;
  if (input_file_ != "") {
    std::cout << "Parsing json file: " << input_file_ << std::endl;
    Json::Value js = ExtractJsonFromFile(input_file_);
    for (auto const& key : js.getMemberNames()) {
      Json::Value const& run_js = js[key];
      unsigned run = boost::lexical_cast<unsigned>(key);
      for (unsigned i = 0; i < run_js.size(); ++i) {
        unsigned range_min = 0;
        unsigned range_max = 0;
        unsigned counted = 0;
        for (unsigned j = 0; j < run_js[i].size(); ++j) {
          ++counted;
          if (counted == 1) range_min = run_js[i][j].asUInt();
          if (counted == 2) range_max = run_js[i][j].asUInt();
        }
        if (counted != 2 || range_max < range_min) {
          std::cout << "Something has gone wrong parsing the json file!\n";
          throw;
        } else {
          for (unsigned i = range_min; i <= range_max; ++i)
            input_json[run].insert(i);
        }
      }
    }
  } else {
    std::cout << "No json file specified!" << std::endl;
  }
  if (produce_output_jsons_ != "") {
    std::cout << "Writing output jsons with name: " << produce_output_jsons_
              << std::endl;
  }
  return 0;
}

int LumiMask::Execute(TreeEvent* event) {
  EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
  unsigned run = eventInfo->run();
  unsigned ls = eventInfo->lumi_block();
  all_json[run].insert(ls);
  bool accept = false;
  if (input_json.find(run) != input_json.end()) {
    if (input_json[run].find(ls) != input_json[run].end()) accept = true;
  }
  if (accept) {
    accept_json[run].insert(ls);
    return 0;
  } else {
    reject_json[run].insert(ls);
    return 1;
  }

  return 0;
}
int LumiMask::PostAnalysis() {
  if (produce_output_jsons_ != "") {
    std::ofstream output;
    output.open((produce_output_jsons_ + "_all.json").c_str());
    WriteJson(all_json, output);
    output.close();
    output.open((produce_output_jsons_ + "_accept.json").c_str());
    WriteJson(accept_json, output);
    output.close();
    output.open((produce_output_jsons_ + "_reject.json").c_str());
    WriteJson(reject_json, output);
    output.close();
  }
  return 0;
}

void LumiMask::WriteJson(JsonMap const& json, std::ofstream& output) {
  for (JsonIt r_it = json.begin(); r_it != json.end(); ++r_it) {
    if (r_it == json.begin()) {
      output << "{\"";
    } else {
      output << " \"";
    }
    output << r_it->first << "\": [";
    std::set<unsigned>::const_iterator it = r_it->second.begin();
    while (it != r_it->second.end()) {
      unsigned first_val = *it;
      unsigned last_val = *it;
      while (true) {
        ++it;
        if (it == r_it->second.end()) break;
        if (*it == (last_val + 1)) {
          last_val = *it;
        } else {
          break;
        }
      }
      output << "[" << first_val << ", " << last_val << "]";
      if (it != r_it->second.end()) {
        output << ", ";
      } else {
        output << "]";
      }
    }
    JsonIt r_test = r_it;
    ++r_test;
    if (r_test == json.end()) {
      output << "}" << std::endl;
    } else {
      output << "," << std::endl;
    }
  }
}

void LumiMask::PrintInfo() { ; }
}
