#ifndef ICHiggsTauTau_HiggsNuNu_LightTreeAnalyser_h
#define ICHiggsTauTau_HiggsNuNu_LightTreeAnalyser_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include <string>

namespace ic{

  class LTAnalyser {
  CLASS_MEMBER(LTAnalyser,int,verbosity)
    CLASS_MEMBER(LTAnalyser,std::string,baseselection)
  protected:
    std::vector<ic::LTModule *> modulelist_;
    LTFiles filemanager_;    
    bool print_module_list_;
  public:
    LTAnalyser();
    LTAnalyser(int);

    LTAnalyser AddModule(ic::LTModule*);
    LTAnalyser AddFile(std::string,std::string,std::string);
    void SetInFolder(std::string);
    void SetInputParams(std::string);

    bool PostModule(int);
    int RunAnalysis();
  };

}

#endif
