#ifndef ICHiggsTauTau_Utilities_BTagWeight_h
#define ICHiggsTauTau_Utilities_BTagWeight_h

#include <vector>
#include <string>
#include <math.h>
#include <iostream>
#include "TF1.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "TRandom3.h"

namespace ic {


class BTagWeight {

public:
  struct JetInfo {
    JetInfo(float mceff,float datasf) : eff(mceff), sf(datasf) {}
    float eff;
    float sf;
  };

  BTagWeight();
  ~BTagWeight();

  double LouvainBEff(int flavor, std::string const& algo, double pt, double eta) const;
  double BEff(int flavor, std::string const& algo_label, double pt, double eta) const;
  double SF(int flavor, std::string const& algo_label, double pt, double eta) const;
  std::pair<float,float> weight(std::vector<JetInfo> jets, int minTags, int maxTags) const;
  double GetWeight(std::vector<PFJet *> const& jets, std::string const& tagger, unsigned min, unsigned max, bool is_2012 = false) const;
  void ReTag(std::vector<PFJet *> & jets, bool is_2012) const;
  double GetLouvainWeight(std::vector<PFJet *> const& jets, std::string const& tagger, unsigned min, unsigned max) const;

private:

  TF1 *louvain_eff_;
  TRandom3  *rand;
  
};


}

#endif