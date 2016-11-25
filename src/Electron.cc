#include "../interface/Electron.hh"
#include "../interface/city.h"
#include "boost/format.hpp"

namespace ic {

Electron::Electron()
    : Candidate::Candidate(),
      dr03_tk_sum_pt_(0.),
      dr03_ecal_rechit_sum_et_(0.),
      dr03_hcal_tower_sum_et_(0.),
      ecal_pf_cluster_iso_(0.),
      hcal_pf_cluster_iso_(0.),
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
      hadronic_over_em_(0.),
      full5x5_sigma_IetaIeta_(0.),
      sigma_IetaIeta_(0.),
      dphi_sc_tk_at_vtx_(0.),
      deta_sc_tk_at_vtx_(0.),
      gsf_tk_nhits_(0),
      conv_dist_(0.),
      conv_dcot_(0.),
      f_brem_(0.),
      sc_eta_(0.),
      sc_theta_(0.),
      sc_e_over_p_(0.),
      sc_energy_(0.),
      r9_(0.),
      hcal_sum_(0.),
      ecal_energy_(0.),
      has_matched_conversion_(false),
      dxy_vertex_(0.),
      dz_vertex_(0.),
      dxy_beamspot_(0.) {}

Electron::~Electron() {}

void Electron::SetIdIso(std::string const& name, float const& value) {
  elec_idiso_[CityHash64(name)] = value;
}

bool Electron::HasIdIso(std::string const& name) const {
  return elec_idiso_.count(CityHash64(name)) > 0;
}

float Electron::GetIdIso(std::string const& name) const {
  UFmap::const_iterator iter = elec_idiso_.find(CityHash64(name));
  if (iter != elec_idiso_.end()) {
    return iter->second;
  } else {
    std::cerr << "Warning in <Electron::GetIdIso>: Label \""
        << name << "\" not found" << std::endl;
    return 0.0;
  }
}

void Electron::Print() const {
  std::cout << "-sc_eta: " << this->sc_eta() << std::endl;
  std::cout << "-sigma_IetaIeta: " << this->sigma_IetaIeta() << std::endl;
  std::cout << "-hadronic_over_em: " << this->hadronic_over_em() << std::endl;
  std::cout << "-dphi_sc_tk_at_vtx: " << this->dphi_sc_tk_at_vtx() << std::endl;
  std::cout << "-deta_sc_tk_at_vtx: " << this->deta_sc_tk_at_vtx() << std::endl;
  std::cout << "-gsf_tk_nhits: " << this->gsf_tk_nhits() << std::endl;
  std::cout << "-has_matched_conversion: " << this->has_matched_conversion()
            << std::endl;
  std::cout << "-dxy_vertex: " << this->dxy_vertex() << std::endl;
  std::cout << "-dz_vertex:  " << this->dz_vertex() << std::endl;
  std::cout << "-hadronic_over_em: " << this->hadronic_over_em() << std::endl;
  std::cout << "-dr04_pfiso_charged_all " << this->dr04_pfiso_charged_all()
            << std::endl;
  std::cout << "-dr04_pfiso_charged " << this->dr04_pfiso_charged()
            << std::endl;
  std::cout << "-dr04_pfiso_neutral " << this->dr04_pfiso_neutral()
            << std::endl;
  std::cout << "-dr04_pfiso_gamma " << this->dr04_pfiso_gamma() << std::endl;
  std::cout << "-dr04_pfiso_pu " << this->dr04_pfiso_pu() << std::endl;
}
}
