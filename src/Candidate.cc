#include "../interface/Candidate.hh"

namespace ic {
// Constructors/Destructors
Candidate::Candidate() : vector_(Vector()), id_(0), charge_(0) {}

Candidate::~Candidate() {}

void Candidate::Print() const {
  std::cout << "[pt,eta,phi,e] = " << vector_ << " charge = " << charge_
            << std::endl;
}
}
