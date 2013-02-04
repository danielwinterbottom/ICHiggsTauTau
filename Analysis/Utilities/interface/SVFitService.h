#ifndef ICHiggsTauTau_Analysis_SVFitService_h
#define ICHiggsTauTau_Analysis_SVFitService_h

#include <string>
#include <iostream>
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"

#include "TTree.h"

namespace ic {

  class SVFitService {
  private:

  public:
    SVFitService();
    ~SVFitService();

    static double SVFitMassLepHad(Candidate const* lep, Candidate const* had, Met const* met);
    static double SVFitMassLepLep(Candidate const* lep1, Candidate const* lep2, Met const* met);

  };
}


#endif
