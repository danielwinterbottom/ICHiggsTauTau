#ifndef ICHiggsTauTau_HiggsNuNu_HistPlotter_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_HistPlotter_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LTPlotElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LTShapeElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>
#include <vector>
#include <utility>
#include "TH1F.h"
#include "TPad.h"
#include "TLatex.h"
#include "TLine.h"
#include "TBox.h"
#include "TASImage.h"


namespace ic {

  class HistPlotter : public LTModule{ 
    CLASS_MEMBER(HistPlotter,std::string,dirname)
    CLASS_MEMBER(HistPlotter,std::vector<LTPlotElement>,elements)   
    CLASS_MEMBER(HistPlotter,std::vector<LTShapeElement>,shapes)
      //CLASS_MEMBER(HistPlotter,std::vector<std::string>,shapes)   
      //CLASS_MEMBER(HistPlotter,std::vector<std::string>,histTitles)   
    CLASS_MEMBER(HistPlotter,bool,do_debug)
    CLASS_MEMBER(HistPlotter,bool,do_norm)
    CLASS_MEMBER(HistPlotter,bool,do_ratio)
    CLASS_MEMBER(HistPlotter,bool,do_ratio_line)
    CLASS_MEMBER(HistPlotter,bool,do_ratio_fitline)
    CLASS_MEMBER(HistPlotter,bool,add_underflows)
    CLASS_MEMBER(HistPlotter,bool,add_overflows)
    CLASS_MEMBER(HistPlotter,std::string,outsuffix)

  public:	
    HistPlotter(std::string);
    virtual ~HistPlotter();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
    static void SetMCStackStyle(LTPlotElement*);
    static void SetSignalStyle(LTPlotElement*);
    static void SetDataStyle(LTPlotElement*);
  };

}
#endif
