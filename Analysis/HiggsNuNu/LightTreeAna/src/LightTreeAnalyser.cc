#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeAnalyser.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <algorithm>
#include "boost/bind.hpp"

namespace ic{

  LTAnalyser::LTAnalyser(std::string outputname){
    verbosity_=1;
    outputname_=outputname;
    fs = new fwlite::TFileService((outputname_).c_str());
  };
  
  LTAnalyser::LTAnalyser(std::string outputname, int verbosity){
    verbosity_=verbosity;
    outputname_=outputname;
    fs = new fwlite::TFileService((outputname_).c_str());
  };
  
  LTAnalyser LTAnalyser::AddModule(LTModule *module){
    modulelist_.push_back(module);
    return *this;
  };

  LTAnalyser LTAnalyser::AddFile(std::string name, std::string set, std::string path){
    filemanager_.AddFile(name,set,path);
    return *this;
  };

  void LTAnalyser::SetInFolder(std::string infolder){
    filemanager_.set_infolder(infolder);
  };

  void LTAnalyser::SetInputParams(std::string inputparams){
    filemanager_.set_input_params(inputparams);
  };

  bool LTAnalyser::PostModule(int status) {
    if (status > 0) {

      return false;
    } else {
      return true;
    }
  }

  int LTAnalyser::RunAnalysis(){

    if (print_module_list_) {
      std::cout << "-------------------------------------" << std::endl;
      std::cout << "Module List" << std::endl;
      std::cout << "-------------------------------------" << std::endl;
      for (unsigned i = 0; i < modulelist_.size(); ++i) {
	std::cout << modulelist_[i]->module_name() << std::endl;
      }
    }

    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Module Initialisation Output" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    for(unsigned module = 0; module<modulelist_.size();++module) {
      modulelist_[module]->Init(fs);
    }

    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Beginning Main Analysis Sequence" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    for (unsigned module = 0; module < modulelist_.size(); ++module) {
      int status = modulelist_[module]->Run(&filemanager_);
      if (!PostModule(status)) {
	if (status == 1) break;
      }
    }
    return 0;
  };

}
