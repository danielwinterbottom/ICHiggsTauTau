#ifndef HTFromLHEParticles_h
#define HTFromLHEParticles_h

#include <iostream>
#include <vector>
#include "TMath.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"

using namespace ic;

double HTFromLHEParticles(std::vector<GenParticle *> lheParticles);

#endif