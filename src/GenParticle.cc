#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"

namespace ic {
  //Constructors/Destructors
  GenParticle::GenParticle() {
  }

  GenParticle::~GenParticle() {
  }

  void GenParticle::Print() const {
    Candidate::Print();
  }
}
