#ifndef ICHiggsTauTau_HiggsNuNu_JECStudy_h
#define ICHiggsTauTau_HiggsNuNu_JECStudy_h

#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include <cmath>
#include "TH2F.h"
#include <vector>
#include <string>
#include "boost/bind.hpp"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Math/GenVector/VectorUtil.h"
#include "Math/Vector4Dfwd.h"
#include "TGraphErrors.h"
#include "TF1.h"

class TRandom3;

namespace ic {
  
  class JECStudy : public ModuleBase {
  private:

    JetCorrectorParameters *resJetPar_old_;
    JetCorrectorParameters *l3JetPar_old_;
    JetCorrectorParameters *l2JetPar_old_;
    JetCorrectorParameters *l1JetPar_old_;
    FactorizedJetCorrector *jetCorrector_old_;

    JetCorrectorParameters *resJetPar_new_;
    JetCorrectorParameters *l3JetPar_new_;
    JetCorrectorParameters *l2JetPar_new_;
    JetCorrectorParameters *l1JetPar_new_;
    FactorizedJetCorrector *jetCorrector_new_;

    CLASS_MEMBER(JECStudy,fwlite::TFileService*, fs);
    CLASS_MEMBER(JECStudy, bool, is_data);
    CLASS_MEMBER(JECStudy, bool, reapplyJEC);
    CLASS_MEMBER(JECStudy, std::vector<std::string>, jec_old_files);
    CLASS_MEMBER(JECStudy, std::vector<std::string>, jec_new_files);
    CLASS_MEMBER(JECStudy, std::string, input_label);

    TH2F* dpTvspTeta;
	
  public:
    JECStudy(std::string const& name);
    virtual ~JECStudy();    
    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
  };
  
	  
  
}

#endif
