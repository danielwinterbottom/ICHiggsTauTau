#include "Utilities/interface/JsonTools.h"

#include <fstream>
#include <string>

namespace ic {
Json::Value ExtractJsonFromFile(std::string const& file) {
  Json::Value js;
  Json::Reader json_reader;
  std::fstream input;
  input.open(file);
  json_reader.parse(input, js);
  return js;
}

void UpdateJson(Json::Value& a, Json::Value const& b) {
  if (!a.isObject() || !b.isObject()) return;
  for (auto const& key : b.getMemberNames()) {
    if (a[key].isObject()) {
      UpdateJson(a[key], b[key]);
    } else {
      a[key] = b[key];
    }
  }
}
}
