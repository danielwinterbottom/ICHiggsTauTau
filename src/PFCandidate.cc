#include "../interface/PFCandidate.hh"
#include "boost/format.hpp"
namespace ic {
// Constructors/Destructors
PFCandidate::PFCandidate() : pdgid_(0) {}

PFCandidate::~PFCandidate() {}

void PFCandidate::Print() const {
  Candidate::Print();
  std::cout << "[pdgid] = " << this->pdgid() << "\n";
}
}
