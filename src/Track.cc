#include "UserCode/ICHiggsTauTau/interface/Track.hh"

namespace ic {
// Constructors/Destructors
Track::Track() : id_(0), charge_(0) {}

Track::~Track() {}

void Track::Print() const {
  std::cout << momentum_ << std::endl;
  std::cout << ref_point_ << std::endl;
}
}
