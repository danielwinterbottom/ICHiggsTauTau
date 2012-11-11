#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include <boost/lexical_cast.hpp>
#include <fstream>


namespace ic {

  std::vector<std::string> ParseFileLines(std::string const& file_name) {
    // Build a vector of input files
    std::vector<std::string> files;
    std::ifstream file;
    file.open(file_name.c_str());
    if (!file.is_open()) {
      std::cerr << "Warning: File " << file_name << " cannot be opened." << std::endl;
      return files;
    }
    std::string line = "";
    while(std::getline(file, line)) {//while loop through lines
      files.push_back(line);
    }
    return files;
  }



} //namespace
