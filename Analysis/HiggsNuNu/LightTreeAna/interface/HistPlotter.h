#ifndef ICHiggsTauTau_HiggsNuNu_HistPlotterTest_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_HistPlotterTest_h
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

  class HistPlotterTest : public LTModule{ 
    CLASS_MEMBER(HistPlotterTest,std::string,dirname)
    CLASS_MEMBER(HistPlotterTest,std::vector<LTPlotElement>,elements)   
    CLASS_MEMBER(HistPlotterTest,std::vector<LTShapeElement>,shapes)
      //CLASS_MEMBER(HistPlotterTest,std::vector<std::string>,shapes)   
      //CLASS_MEMBER(HistPlotterTest,std::vector<std::string>,histTitles)   
    CLASS_MEMBER(HistPlotterTest,bool,do_debug)
    CLASS_MEMBER(HistPlotterTest,bool,do_norm)
    CLASS_MEMBER(HistPlotterTest,bool,do_ratio)
    CLASS_MEMBER(HistPlotterTest,bool,do_ratio_line)
    CLASS_MEMBER(HistPlotterTest,bool,do_ratio_fitline)
    CLASS_MEMBER(HistPlotterTest,bool,add_underflows)
    CLASS_MEMBER(HistPlotterTest,bool,add_overflows)
    CLASS_MEMBER(HistPlotterTest,std::string,outsuffix)

  public:	
    HistPlotterTest(std::string);
    virtual ~HistPlotterTest();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
    static void SetMCStackStyle(LTPlotElement*);
    static void SetSignalStyle(LTPlotElement*);
    static void SetDataStyle(LTPlotElement*);
  };

}
#endif
