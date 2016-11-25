#ifndef ICHiggsTauTau_Module_LightTreeMCGen_h
#define ICHiggsTauTau_Module_LightTreeMCGen_h

#include <string>

#include "PhysicsTools/FWLite/interface/TFileService.h"

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"

#include "TTree.h"
#include "TVector3.h"
#include "Math/VectorUtil.h"

namespace ic {
  class LightTreeMCGen : public ModuleBase {

  private:

    CLASS_MEMBER(LightTreeMCGen,fwlite::TFileService*, fs);

    TTree *outputTree_;
    
    unsigned run_;
    unsigned lumi_;
    unsigned event_;

    unsigned nSave_;
    unsigned nJetsSave_;

    unsigned nParticles_;
    int* pdgid_;
    unsigned* status_;
    double* pt_;
    double* eta_;
    double* phi_;
    double* E_;
    double* m_;

    unsigned nGenJets_;
    double* genjet_pt_;
    double* genjet_eta_;
    double* genjet_phi_;
    double* genjet_E_;

    unsigned countHt0_100_;
    unsigned countHt100_200_;
    unsigned countHt200_400_;
    unsigned countHt400_600_;
    unsigned countHt600_inf_;

    unsigned count0Parton_;
    unsigned count1Parton_;
    unsigned count2Parton_;
    unsigned count3Parton_;
    unsigned count4Parton_;

  public:
    LightTreeMCGen(std::string const& name);
    virtual ~LightTreeMCGen();

    void resetAllTreeVariables();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();

   };


}


#endif
