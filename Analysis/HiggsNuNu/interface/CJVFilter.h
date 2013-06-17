#ifndef ICHiggsTauTau_HiggsNuNu_CJVFilter_h
#define ICHiggsTauTau_HiggsNuNu_CJVFilter_h

#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <cmath>
#include "TH2F.h"
#include <vector>
#include <string>
#include "boost/bind.hpp"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Math/GenVector/VectorUtil.h"
#include "Math/Vector4Dfwd.h"

namespace ic {
  
  class CJVFilter : public ModuleBase {
  private:
    CLASS_MEMBER(CJVFilter,fwlite::TFileService*, fs);
    CLASS_MEMBER(CJVFilter, bool, is_data)
    CLASS_MEMBER(CJVFilter, std::string, jetsinput_label)
    CLASS_MEMBER(CJVFilter, std::string, pairinput_label)
    CLASS_MEMBER(CJVFilter, double, ptcut)
    TH1F* genericplot;  
        
  public:
    CJVFilter(std::string const& name);
    virtual ~CJVFilter();    
    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
  };
  
	  
  
}

#endif
