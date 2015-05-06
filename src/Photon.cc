#include "../interface/Photon.hh"

namespace ic {
Photon::Photon()
    : ic::Candidate(),
      dr03_pfiso_charged_all_(0.),
      dr03_pfiso_charged_(0.),
      dr03_pfiso_neutral_(0.),
      dr03_pfiso_gamma_(0.),
      dr03_pfiso_pu_(0.),
      dr04_pfiso_charged_all_(0.),
      dr04_pfiso_charged_(0.),
      dr04_pfiso_neutral_(0.),
      dr04_pfiso_gamma_(0.),
      dr04_pfiso_pu_(0.),
      had_tower_over_em_(0.),
      sigma_IetaIeta_(0.),
      pass_electron_veto_(false) {}

Photon::~Photon() {}

void Photon::Print() const { Candidate::Print(); }
}
