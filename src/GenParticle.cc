#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "boost/format.hpp"
namespace ic {
// Constructors/Destructors
GenParticle::GenParticle() : index_(0), pdgid_(0), status_(0) {}

GenParticle::~GenParticle() {}

void GenParticle::Print() const {
  std::cout << (boost::format("idx: %-4i  st: %-3i  id: %4i  %-40s  M: %f\n") %
                this->index() % this->status() % this->pdgid()) %
                   this->vector() % this->M();
  if (this->mothers().size()) {
    std::cout << "  mothers:  ";
    for (unsigned i = 0; i < this->mothers().size(); ++i) {
      std::cout << " " << this->mothers().at(i);
    }
    std::cout << "\n";
  }
  if (this->daughters().size()) {
    std::cout << "  daughters:";
    for (unsigned i = 0; i < this->daughters().size(); ++i) {
      std::cout << " " << this->daughters().at(i);
    }
    std::cout << "\n";
  }
}
}
