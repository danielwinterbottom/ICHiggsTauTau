#ifndef ICHiggsTauTau_Utilities_SimpleParamParser_h
#define ICHiggsTauTau_Utilities_SimpleParamParser_h

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "boost/lexical_cast.hpp"

class SimpleParamParser {
   private:
   std::map<std::string, std::string> param_map_;
  
  public:
    SimpleParamParser() {
      ;
    }

    ~SimpleParamParser() {
      ;
    }

    bool ParseFile(std::string file_name) {
      std::ifstream file;
      file.open(file_name.c_str());
      // Check if file is open
      if (!file.is_open()) {
        std::cerr << "Warning: File " << file_name << " cannot be opened." << std::endl;
        return false;
      }
      std::string line = "";
      while(std::getline(file, line)){//while loop through lines
        // If there is at least one character in the line, see if it is a comment
        std::vector<std::string> vec_format;
        if (line.size() > 1) {
          if (line[0] == '#') {
            continue;
          } else {
            boost::split(vec_format, line, boost::is_any_of(":"));
          }
        }
        // There must be exactly two fields
        if (vec_format.size() != 2){
          std::cerr << "Warning: the following line is not in the correct format:" << std::endl;
          std::cerr << line << std::endl;
          continue;
        }
        param_map_[vec_format[0]] = vec_format[1];
      }//while loop through lines
      return true;
    }

    template <class T>
      T GetParam(std::string name) {
        if (param_map_.count(name) > 0) {
          return boost::lexical_cast<T>(param_map_[name]);
        } else {
          std::cerr << "Warning: Parameter " << name << " not found." << std::endl;
          throw;
        }
      }

    inline bool ParamExists(std::string name) {
      return (param_map_.count(name));
    }
};

#endif