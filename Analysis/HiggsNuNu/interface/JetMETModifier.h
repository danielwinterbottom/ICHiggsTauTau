#ifndef ICHiggsTauTau_HiggsNuNu_JetMETModifier_h
#define ICHiggsTauTau_HiggsNuNu_JetMETModifier_h

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
  
  class JetMETModifier : public ModuleBase {
  private:

    enum jetmetCor {
      jecData=0,
      jecMC=1,
      smearMC=2
    };
    enum jetmetSyst {
      jesUp=0,
      jesDown=1,
      jerBetter=2,
      jerWorse=3,
      uesUp=4,
      uesDown=5
    };

    JetCorrectionUncertainty *jetCorUnc_;
    JetCorrectorParameters *resJetPar_data_;
    JetCorrectorParameters *l3JetPar_data_;
    JetCorrectorParameters *l2JetPar_data_;
    JetCorrectorParameters *l1JetPar_data_;
    FactorizedJetCorrector *jetCorrector_data_;

    CLASS_MEMBER(JetMETModifier,fwlite::TFileService*, fs);
    CLASS_MEMBER(JetMETModifier, bool, is_data)
    CLASS_MEMBER(JetMETModifier, bool, reapplyJEC)
    CLASS_MEMBER(JetMETModifier, std::vector<std::string>, jec_data_files)

    CLASS_MEMBER(JetMETModifier, bool, dojetresmeasurement)
    CLASS_MEMBER(JetMETModifier, bool, dojessyst)
    CLASS_MEMBER(JetMETModifier, bool, dodatajessyst)
    CLASS_MEMBER(JetMETModifier, bool, jesupordown)
    CLASS_MEMBER(JetMETModifier, bool, dojersyst)
    CLASS_MEMBER(JetMETModifier, bool, dojerworsegauss)
    CLASS_MEMBER(JetMETModifier, bool, jerbetterorworse)
    CLASS_MEMBER(JetMETModifier, bool, douessyst)
    CLASS_MEMBER(JetMETModifier, bool, uesupordown)
    CLASS_MEMBER(JetMETModifier, std::string, input_label)
    CLASS_MEMBER(JetMETModifier, std::string, met_label)
    CLASS_MEMBER(JetMETModifier, bool, dosmear)
    CLASS_MEMBER(JetMETModifier, bool, dogaus)
    CLASS_MEMBER(JetMETModifier, bool, dospring10gaus)
    CLASS_MEMBER(JetMETModifier, bool, doetsmear)
    CLASS_MEMBER(JetMETModifier, bool, doaltmatch)
    CLASS_MEMBER(JetMETModifier, bool, dojerdebug)
    CLASS_MEMBER(JetMETModifier, std::string, jesuncfile)
    CLASS_MEMBER(JetMETModifier, int, randomseed)
    TH2F* JEScorrfac;
    TH1F* JESmetdiff;
    TH1F* JESjetphidiff;
    TH1F* JESjetetadiff;
    TH1F* JESisordersame;
    TH1F* Smearptdiff;
    TH1F* Smear50miss;
    TH1F* Smearjetgenjetptdiff;
    TH1F* Smeargenmindr;
    TH1F* Smearjetgenjetptratio;
    TH2F* Smearjetgenjetptratioetabin;
    TH1F* icjetrunmetjetptdiff;
    TH1F* icjetrunmetjetptratio;
    TH1F* icjetpt;
    TH1F* runmetjetpt;
    TH1F* nojerjetpt;
    TH1F* matchedicjetpt;
    TH1F* matchedrunmetjetpt;
    TH1F* matchednojerjetpt;
    TH1F* icjetnosmearptratio;
    TH1F* runmetjetnosmearptratio;
    TH1F* runmetjetgenjetptratio;
    TH1F* icjetgenjetptratio;
    TRandom3* randomno;

    std::string pts[70];
    TH1F* recogenjetptratio[5][70]; //BINNED JET 
    TGraphErrors* res[5]; 
    TF1* resfunc[5];
    TF1* spring10resfunc[5]; 
	
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
