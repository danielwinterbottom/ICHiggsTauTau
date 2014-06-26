#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"

namespace ic {

PFJet::PFJet()
    : Jet::Jet(),
      muon_multiplicity_(0),
      charged_multiplicity_(0),
      charged_had_multiplicity_(0),
      neutral_had_multiplicity_(0),
      photon_multiplicity_(0),
      electron_multiplicity_(0),
      HF_had_multiplicity_(0),
      HF_em_multiplicity_(0),
      neutral_multiplicity_(0),
      charged_em_energy_(0.),
      neutral_em_energy_(0.),
      charged_had_energy_(0.),
      neutral_had_energy_(0.),
      photon_energy_(0.),
      electron_energy_(0.),
      muon_energy_(0.),
      HF_had_energy_(0.),
      HF_em_energy_(0.),
      charged_mu_energy_(0.),
      beta_(0.),
      beta_max_(0.),
      pu_id_mva_value_(0.),
      linear_radial_moment_(0.),
      charged_multiplicity_nopu_(0) {}

PFJet::~PFJet() {}

void PFJet::Print() const { Candidate::Print(); }
}
