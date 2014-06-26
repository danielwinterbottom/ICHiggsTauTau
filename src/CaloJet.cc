#include "UserCode/ICHiggsTauTau/interface/CaloJet.hh"

namespace ic {
CaloJet::CaloJet()
    : Jet::Jet(),
      max_em_tower_energy_(0.),
      max_had_tower_energy_(0.),
      energy_frac_had_(0.),
      energy_frac_em_(0.),
      had_energy_HB_(0.),
      had_energy_HO_(0.),
      had_energy_HE_(0.),
      had_energy_HF_(0.),
      em_energy_EB_(0.),
      em_energy_EE_(0.),
      em_energy_HF_(0.),
      towers_area_(0.),
      n90_(0),
      n60_(0),
      fHPD_(0.),
      fRBX_(0.),
      id_loose_(false),
      id_tight_(false) {}

CaloJet::~CaloJet() {}

void CaloJet::Print() const { Candidate::Print(); }
}
