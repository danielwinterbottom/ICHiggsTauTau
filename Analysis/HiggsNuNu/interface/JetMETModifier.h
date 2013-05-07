#ifndef ICHiggsTauTau_HiggsNuNu_JetMETModifier_h
#define ICHiggsTauTau_HiggsNuNu_JetMETModifier_h

#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include <cmath>
#include "TH2F.h"
#include <vector>
#include <string>
#include "boost/bind.hpp"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Math/GenVector/VectorUtil.h"
#include "Math/Vector4Dfwd.h"

namespace ic {
  
  class JetMETModifier : public ModuleBase {
  private:
    JetCorrectionUncertainty *total;
    JetCorrectionUncertainty *total2;
    CLASS_MEMBER(JetMETModifier,fwlite::TFileService*, fs);
    CLASS_MEMBER(JetMETModifier, bool, is_data)
    CLASS_MEMBER(JetMETModifier, bool, dojessyst)
    CLASS_MEMBER(JetMETModifier, bool, upordown)
    CLASS_MEMBER(JetMETModifier, std::string, input_label)
    CLASS_MEMBER(JetMETModifier, std::string, met_label)
    TH2F* JEScorrfac;
    TH1F* JESmetdiff;
    TH1F* JESjetphidiff;
    TH1F* JESjetetadiff;
    TH1F* JESisordersame;
    
  public:
    JetMETModifier(std::string const& name);
    virtual ~JetMETModifier();    
    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
  };
  
	  
  
}

#endif
