#include "../interface/Track.hh"

namespace ic {
// Constructors/Destructors
Track::Track()
    : id_(0),
      charge_(0),
      normalized_chi2_(0.),
      hits_(0),
      pixel_hits_(0),
      algorithm_(0),
      pt_err_(0.),
      quality_(0),
      hits_miss_inner_(0) {}

Track::~Track() {}

void Track::Print() const {
  std::cout << momentum_ << std::endl;
  std::cout << ref_point_ << std::endl;
}
}
