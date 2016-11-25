#ifndef ICHiggsTauTau_HiggsNuNu_SimplePlots_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_SimplePlots_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class SimplePlots : public LTModule{
    CLASS_MEMBER(SimplePlots,std::string,qcdset)
    CLASS_MEMBER(SimplePlots,std::string,sigset)
    CLASS_MEMBER(SimplePlots,std::string,dataset)
    CLASS_MEMBER(SimplePlots,std::vector<std::string>,ewkset)
    CLASS_MEMBER(SimplePlots,std::string,cat)
    CLASS_MEMBER(SimplePlots,std::string,basesel)
    CLASS_MEMBER(SimplePlots,std::vector<std::string>,shapes)
  public:
    SimplePlots(std::string);
    virtual ~SimplePlots();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);

    TH1F * make_histogram(TH1F aHist,
			  const std::vector<std::string> & strs);

  };

}
#endif
