#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"

namespace ic {
GenJet::GenJet() : flavour_(0), n_constituents_(0) {}

GenJet::~GenJet() {}

void GenJet::Print() const { Candidate::Print(); }
}
