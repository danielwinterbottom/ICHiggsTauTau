#ifndef ICHiggsTauTau_Analysis_SVFitServiceRun2_h
#define ICHiggsTauTau_Analysis_SVFitServiceRun2_h

#include <string>
#include <iostream>
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "HiggsTauTau/SVFitRun2/interface/SVfitStandaloneAlgorithm.h"

#include "TTree.h"

namespace ic {

  class SVFitServiceRun2 {
  private:

  public:
    SVFitServiceRun2();
    ~SVFitServiceRun2();

    static double SVFitMassEleHad(Candidate const* lep, Candidate const* had, int decm2, Met const* met, bool MC=false);
    static double SVFitMassMuHad(Candidate const* lep, Candidate const* had, int decm2, Met const* met, bool MC=false);
    static double SVFitMassEleMu(Candidate const* lep1, Candidate const* lep2, Met const* met, bool MC=false);
    static double SVFitMassHadHad(Candidate const* had1, int decm1, Candidate const* had2, int decm2, Met const* met, bool MC=false);
    static std::pair<Candidate, double> SVFitCandidateEleHad(Candidate const* lep, Candidate const* had, int decm2,  Met const* met, bool MC=false);
    static std::pair<Candidate, double> SVFitCandidateMuHad(Candidate const* lep, Candidate const* had, int decm2, Met const* met, bool MC=false);
    static std::pair<Candidate, double> SVFitCandidateEleMu(Candidate const* lep1, Candidate const* lep2, Met const* met, bool MC=false);
    static std::pair<Candidate, double> SVFitCandidateHadHad(Candidate const* had1, int decm1, Candidate const* had2,int decm2,  Met const* met, bool MC=false);

  };
}


#endif
