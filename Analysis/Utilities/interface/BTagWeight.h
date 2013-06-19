#ifndef ICHiggsTauTau_Utilities_BTagWeight_h
#define ICHiggsTauTau_Utilities_BTagWeight_h

#include <vector>
#include <string>
#include <math.h>
#include <iostream>
#include "TF1.h"
#include "TH1.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "TRandom3.h"

namespace ic {


class BTagWeight {


public:
enum class payload { ALL2011, MORIOND2013, EPS13 };
enum class tagger { SSVHEM, SSVHPT, CSVM };
  struct JetInfo {
    JetInfo(float mceff,float datasf) : eff(mceff), sf(datasf) {}
    float eff;
    float sf;
  };

  BTagWeight();
  ~BTagWeight();

  double LouvainBEff(int flavor, BTagWeight::tagger const& tag, double pt, double eta) const;
  double GetLouvainWeight(std::vector<PFJet *> const& jets, BTagWeight::tagger const& algo, unsigned min, unsigned max) const;

  double BEff(BTagWeight::payload const& set, 
              unsigned flavour, 
              BTagWeight::tagger const& algo, 
              double pt, 
              double eta) const;

  double SF(BTagWeight::payload const& set, 
            unsigned flavour, 
            BTagWeight::tagger const& algo, 
            double pt, 
            double eta,
            int Btag_mode,
            int Bfake_mode) const;

  std::pair<float,float> weight(std::vector<JetInfo> jets, int minTags, int maxTags) const;

  double GetWeight( std::vector<PFJet *> const& jets, 
                    BTagWeight::payload const& set, 
                    BTagWeight::tagger const& algo, 
                    unsigned min, 
                    unsigned max) const;
  
  std::map<std::size_t, bool> ReTag(std::vector<PFJet *> const& jets, 
                                    BTagWeight::payload const& set, 
                                    BTagWeight::tagger const& algo,
                                    int Btag_mode,
                                    int Bfake_mode) const;

private:

  TF1 *louvain_eff_;
  TRandom3  *rand;
  TH1F *SFb_error_;
  
};


}

#endif
