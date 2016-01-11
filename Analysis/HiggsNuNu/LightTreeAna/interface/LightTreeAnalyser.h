#ifndef ICHiggsTauTau_HiggsNuNu_LightTreeAnalyser_h
#define ICHiggsTauTau_HiggsNuNu_LightTreeAnalyser_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include <string>
#include "TFile.h"

namespace ic{

  class LTAnalyser {
  CLASS_MEMBER(LTAnalyser,int,verbosity)
    CLASS_MEMBER(LTAnalyser,std::string,baseselection)
    CLASS_MEMBER(LTAnalyser,std::string,outputname)
  protected:
    std::vector<ic::LTModule *> modulelist_;
    LTFiles filemanager_;    
    bool print_module_list_;
  public:
    TFile* fs;
    LTAnalyser(std::string);
    LTAnalyser(std::string,int);

    LTAnalyser AddModule(ic::LTModule*);
    LTAnalyser AddFile(std::string,std::string,std::string);
    LTAnalyser AddFiles(std::string);
    void SetInFolder(std::string);
    void SetEosFolders(std::string,std::string);
    void SetInputParams(std::string);

    bool PostModule(int);
    int RunAnalysis();
  };

}

#endif
