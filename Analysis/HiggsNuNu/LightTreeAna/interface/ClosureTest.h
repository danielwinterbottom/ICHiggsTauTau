#ifndef ICHiggsTauTau_HiggsNuNu_ClosureTest_h
#define ICHiggsTauTau_HiggsNuNu_ClosureTest_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class ClosureTest : public LTModule{
    CLASS_MEMBER(ClosureTest,std::string,shape)
      CLASS_MEMBER(ClosureTest,std::string,shapename)
      CLASS_MEMBER(ClosureTest,std::string,sigmcset)
      CLASS_MEMBER(ClosureTest,std::string,contmcset)
      CLASS_MEMBER(ClosureTest,std::vector<std::string>,contbkgset)
      CLASS_MEMBER(ClosureTest,std::vector<std::string>,contbkgextrafactordir)
      CLASS_MEMBER(ClosureTest,std::vector<int>,contbkgisz)
      CLASS_MEMBER(ClosureTest,std::string,dataset)
      CLASS_MEMBER(ClosureTest,std::string,sigcat)
      CLASS_MEMBER(ClosureTest,std::string,zcontcat)
      CLASS_MEMBER(ClosureTest,std::string,contcat)
      CLASS_MEMBER(ClosureTest,std::string,dataextrasel)
      CLASS_MEMBER(ClosureTest,std::string,sigmcextrasel)
      CLASS_MEMBER(ClosureTest,std::string,contmcextrasel)
      CLASS_MEMBER(ClosureTest,std::string,contbkgextrasel)
      CLASS_MEMBER(ClosureTest,std::string,basesel)
      CLASS_MEMBER(ClosureTest,std::string,sigmcweight)
      CLASS_MEMBER(ClosureTest,std::string,contmcweight)
      CLASS_MEMBER(ClosureTest,std::string,contmczweight)
      CLASS_MEMBER(ClosureTest,std::string,dataweight)
      CLASS_MEMBER(ClosureTest,double,additionalsyst)
      CLASS_MEMBER(ClosureTest,std::vector<double>,binning)
      CLASS_MEMBER(ClosureTest,double,sigcontextrafactor)
      CLASS_MEMBER(ClosureTest,bool,do_latex)
      CLASS_MEMBER(ClosureTest,std::string,dirname)
      public:
    ClosureTest(std::string);
    virtual ~ClosureTest();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
