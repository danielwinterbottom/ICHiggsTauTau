#include "../interface/Met.hh"

namespace ic {
Met::Met()
    : ic::Candidate(),
      sum_et_(0.),
      et_sig_(0.),
      xx_sig_(0.),
      xy_sig_(0.),
      yx_sig_(0.),
      yy_sig_(0.) {}

Met::~Met() {}

void Met::Print() const { Candidate::Print(); }
}
