#include "Utilities/interface/JsonTools.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"

namespace ic {
Json::Value ExtractJsonFromFile(std::string const& file) {
  Json::Value js;
  Json::Reader json_reader;
  std::fstream input;
  input.open(file);
  if (!input.is_open()) {
    throw std::runtime_error("[ExtractJsonFromFile] Unable to open file " + file);
  }
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
  std::vector<std::string> split_str;
  std::vector<std::string> split_lastarg;
  boost::split(split_str,str,boost::is_any_of(":"));
  boost::split(split_lastarg,split_str.at(split_str.size()-1),boost::is_any_of("^"));
  std::string jsonstr = "{\"";
  if(split_lastarg.size()==1){
    for(unsigned i = 0; i<split_str.size()-1;i++){
      if(i==split_str.size()-2){
        jsonstr = jsonstr+split_str.at(i)+"\":";
      } else jsonstr = jsonstr+split_str.at(i)+"\":{\"";
    }
    bool is_num = true;
    try{
      boost::lexical_cast<double>(split_str.at(split_str.size()-1));
    }
    catch(boost::bad_lexical_cast& e){
      is_num = false;
    }
    bool is_bool=false;
    if(strcmp((split_str.at(split_str.size()-1)).c_str(),"false")==0||strcmp((split_str.at(split_str.size()-1)).c_str(),"true")==0){
     is_bool = true;
    }
    if(is_bool || is_num){
      jsonstr = jsonstr+split_str.at(split_str.size()-1)+std::string(split_str.size()-1,'}');
    } else {
      if(strcmp(split_str.at(split_str.size()-1).c_str(),"")==0){
        jsonstr = jsonstr+"[]"+std::string(split_str.size()-1,'}');
      } else {
       jsonstr = jsonstr+"\""+split_str.at(split_str.size()-1)+"\""+std::string(split_str.size()-1,'}');
     }
   }

		
	} else {
   for(unsigned i = 0; i<split_str.size()-1;i++){
    if(i==split_str.size()-2){
      jsonstr = jsonstr+split_str.at(i)+"\":[";
     } else jsonstr = jsonstr+split_str.at(i)+"\":{\"";
   }
    bool is_num = true;
    bool are_all_num = true;
    for(unsigned k = 0; k<split_lastarg.size(); k++){
      if(strcmp((split_lastarg.at(k)).c_str(),"")!=0){
        try{
          boost::lexical_cast<double>(split_lastarg.at(k));
        }
        catch(boost::bad_lexical_cast& e){
          is_num = false;
        }
       are_all_num = are_all_num&&is_num;
     }
    }
    bool is_bool=false;
    bool are_all_bool = true;
    for( unsigned k = 0; k<split_lastarg.size();k++){
      if(strcmp((split_lastarg.at(k)).c_str(),"")!=0){
        if(strcmp((split_lastarg.at(k)).c_str(),"false")==0||strcmp((split_lastarg.at(k)).c_str(),"true")==0){
          is_bool = true;
        }
        are_all_bool = are_all_bool&&is_bool;
      }
	  }
    if(are_all_bool || are_all_num){
      for(unsigned k = 0; k<split_lastarg.size()-1;k++){ 
        if(strcmp("",split_lastarg.at(k).c_str())!=0){
          jsonstr = jsonstr+split_lastarg.at(k)+",";
        }
      } 
      jsonstr=jsonstr+split_lastarg.at(split_lastarg.size()-1)+"]"+std::string(split_str.size()-1,'}');
     } else {
     for(unsigned k = 0; k<split_lastarg.size()-1;k++){ 
       if(strcmp("",split_lastarg.at(k).c_str())!=0){
         jsonstr = jsonstr+"\""+split_lastarg.at(k)+"\",";
       }
      } 
      jsonstr=jsonstr+"\""+split_lastarg.at(split_lastarg.size()-1)+"\"]"+std::string(split_str.size()-1,'}');
    }
  }

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
