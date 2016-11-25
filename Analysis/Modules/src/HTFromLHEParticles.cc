#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HTFromLHEParticles.h"
#include "TMath.h"
#include <iostream>
#include <vector>
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"

using namespace ic;

double HTFromLHEParticles(std::vector<GenParticle *> lheParticles){
  unsigned nOutgoingPartons = 0;
  double lheHT = 0.;

  for(size_t idxPart = 0; idxPart < lheParticles.size();++idxPart){
    unsigned absPdgId = TMath::Abs(lheParticles[idxPart]->pdgid());

    if(lheParticles[idxPart]->status()==1 &&((absPdgId >=1 &&absPdgId<=6) || absPdgId == 21)){
      //lheHT += TMath::Sqrt(TMath::Power(lheParticles[idxPart][0],2) + TMath::Power(lheParticles[idxPart][1],2));
      lheHT += lheParticles[idxPart]->pt();
      nOutgoingPartons++;
  }
 }
 return lheHT;
}
