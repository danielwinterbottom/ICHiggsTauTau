#ifndef ICHiggsTauTau_HiggsNuNu_JetMETModifier_h
#define ICHiggsTauTau_HiggsNuNu_JetMETModifier_h

#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
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

  public:

    enum jetmetCor {
      jecData=1,
      jecMC=2,
      smearMC=3
    };
    enum jetmetSyst {
      none=0,
      jesUp=1,
      jesDown=2,
      jerBetter=3,
      jerWorse=4,
      uesUp=5,
      uesDown=6
    };
  
  private:

    JetCorrectionUncertainty *jetCorUnc_;
    JetCorrectorParameters *resJetPar_;
    JetCorrectorParameters *l3JetPar_;
    JetCorrectorParameters *l2JetPar_;
    JetCorrectorParameters *l1JetPar_;
    FactorizedJetCorrector *jetCorrector_;

    CLASS_MEMBER(JetMETModifier,fwlite::TFileService*, fs);
    CLASS_MEMBER(JetMETModifier, bool, is_data)
    CLASS_MEMBER(JetMETModifier, std::vector<std::string>, jec_data_files)
    CLASS_MEMBER(JetMETModifier, std::vector<std::string>, jec_mc_files)

    CLASS_MEMBER(JetMETModifier, bool, dojetresmeasurement)

    CLASS_MEMBER(JetMETModifier, std::string, input_label)
    CLASS_MEMBER(JetMETModifier, std::string, met_label)

    CLASS_MEMBER(JetMETModifier, bool, dogaus)
    CLASS_MEMBER(JetMETModifier, bool, dospring10gaus)
    CLASS_MEMBER(JetMETModifier, bool, doetsmear)
    CLASS_MEMBER(JetMETModifier, bool, dojerdebug)

    CLASS_MEMBER(JetMETModifier, std::string, jesuncfile)
    CLASS_MEMBER(JetMETModifier, int, randomseed)
    CLASS_MEMBER(JetMETModifier, bool, run2)

    std::vector<JetMETModifier::jetmetCor> corVec_;
    JetMETModifier::jetmetSyst syst_;

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

    bool reapplyJecData_;
    bool reapplyJecMC_;
    bool smear_;

    double applyCorrection(const ROOT::Math::PxPyPzEVector & rawjet,
			   const double & jetarea,
			   const double & rho);

    double applySmearing(const int error, 
			 const GenJet* match,
			 const ROOT::Math::PxPyPzEVector & oldjet);
    
    double getJERcorrfac(const double & abseta,
                         const int error,
                         const bool run2);


    double applyJESuncertainty(const bool doUp,
			       const ROOT::Math::PxPyPzEVector & newjet);

    void applyUESuncertainty(ic::Candidate* uesCorrected,
			     ROOT::Math::PxPyPzEVector & newmet,
			     double & sumetdiff);
    
    void applyUESuncertainty(const Met::BasicMet & shiftedMet,
			     const Met::BasicMet & centralMet,
			     ROOT::Math::PxPyPzEVector & newmet,
			     double & sumetdiff);
  public: 

    JetMETModifier(std::string const& name);
    virtual ~JetMETModifier();    
    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();

    inline void set_corVec(const std::vector<JetMETModifier::jetmetCor> & corVec){
      corVec_ = corVec;
    };

    inline void set_syst(const JetMETModifier::jetmetSyst syst){
      syst_ = syst;
    };

  };
  	  
  
}

#endif
