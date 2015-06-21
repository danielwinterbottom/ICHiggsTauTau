#include "Utilities/interface/JsonTools.h"
#include <iostream>
#include <fstream>
#include <string>
#include "boost/algorithm/string.hpp"

namespace ic {
Json::Value ExtractJsonFromFile(std::string const& file) {
  Json::Value js;
  Json::Reader json_reader;
  std::fstream input;
  input.open(file);
  json_reader.parse(input, js);
  return js;
}

Json::Value ExtractJsonFromString(std::string const& str) {
  Json::Value js;
  Json::Reader reader(Json::Features::all());
  reader.parse(str, js);
  return js;
}

Json::Value ExtractJsonFromFlatString(std::string const&str){
 //THIS NEEDS TO BE TURNED INTO SOMETHING THAT WORKS FOR ANY DEPTH OF KEYS/VALUES
 //AND FOR ANY VALUE TYPE (this only works for strings)
  std::vector<std::string> split_str;
  std::vector<std::string> split_lastarg;
  boost::split(split_str,str,boost::is_any_of(":"));
  boost::split(split_lastarg,split_str.at(split_str.size()-1),boost::is_any_of("."));
  std::string jsonstr;
  if(split_str.size()==3){
  if(split_lastarg.size()>1){
  jsonstr ="{\""+split_str.at(0)+"\":{\""+split_str.at(1)+"\":[\"";//+split_str.at(2)+"\"]}}";
    for(unsigned k=0;k<split_lastarg.size()-1;k++){
      jsonstr=jsonstr+split_lastarg.at(k)+"\",\"";
    }
  jsonstr=jsonstr+split_lastarg.at(split_lastarg.size()-1)+"\"]}}";
  }
  else{
   jsonstr ="{\""+split_str.at(0)+"\":{\""+split_str.at(1)+"\":\""+split_str.at(2)+"\"}}";//+split_str.at(2)+"\"]}}";
  }
  } else if(split_str.size()==4){
  if(split_lastarg.size()>1){
   jsonstr ="{\""+split_str.at(0)+"\":{\""+split_str.at(1)+"\":{\""+split_str.at(2)+"\"[\"";//+split_str.at(2)+"\"]}}";
    for(unsigned k=0;k<split_lastarg.size()-1;k++){
      jsonstr=jsonstr+split_lastarg.at(k)+"\",\"";
    }
  jsonstr=jsonstr+split_lastarg.at(split_lastarg.size()-1)+"\"]}}}";
  }
  else{
   jsonstr ="{\""+split_str.at(0)+"\":{\""+split_str.at(1)+"\":{\""+split_str.at(2)+"\":\""+split_str.at(3)+"\"}}}";//+split_str.at(2)+"\"]}}";
  }
 } else{ std::cout<<"This method doesn't work for this number of arguments! use ExtractJsonFromString"<<std::endl; exit(1);}


  Json::Value js;
  Json::Reader reader(Json::Features::all());
  reader.parse(jsonstr, js);
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

Json::Value MergedJson(int argc, char* argv[]) {
  Json::Value res(Json::objectValue);
  if (argc >= 1) {
    for (int i = 1; i < argc; ++i) {
      if (boost::algorithm::iends_with(argv[i], ".json")) {
        Json::Value val = ExtractJsonFromFile(argv[i]);
        UpdateJson(res, val);
      } else {
        Json::Value val = ExtractJsonFromString(argv[i]);
        UpdateJson(res, val);
      }
    }
  }
  return res;
}

Json::Value MergedJson(std::vector<std::string> const& vec) {
  Json::Value res(Json::objectValue);
  for (auto const& str : vec) {
    if (boost::algorithm::iends_with(str, ".json")) {
      Json::Value val = ExtractJsonFromFile(str);
      UpdateJson(res, val);
    } else {
      Json::Value val = ExtractJsonFromString(str);
      UpdateJson(res, val);
    }
  }
  return res;
}
}
