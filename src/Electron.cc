#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "boost/format.hpp"

namespace ic {
  //Constructors/Destructors
  Electron::Electron() {
  }

  Electron::~Electron() {
  }

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
    Candidate::Print();
    std::cout << "-sc_eta: " << this->sc_eta() << std::endl;
    std::cout << "-sigma_IetaIeta: " << this->sigma_IetaIeta() << std::endl;
    std::cout << "-hadronic_over_em: " << this->hadronic_over_em() << std::endl;
    std::cout << "-dphi_sc_tk_at_vtx: " << this->dphi_sc_tk_at_vtx() << std::endl;
    std::cout << "-deta_sc_tk_at_vtx: " << this->deta_sc_tk_at_vtx() << std::endl;
    std::cout << "-gsf_tk_nhits: " << this->gsf_tk_nhits() << std::endl;
    std::cout << "-has_matched_conversion: " << this->has_matched_conversion() << std::endl;
    std::cout << "-dxy_vertex: " << this->dxy_vertex() << std::endl;
    std::cout << "-dz_vertex:  " << this->dz_vertex() << std::endl;
    std::cout << "-hadronic_over_em: " << this->hadronic_over_em() << std::endl;
    std::cout << "-dr04_pfiso_charged_all " << this->dr04_pfiso_charged_all() << std::endl;
    std::cout << "-dr04_pfiso_charged " << this->dr04_pfiso_charged() << std::endl;
    std::cout << "-dr04_pfiso_neutral " << this->dr04_pfiso_neutral() << std::endl;
    std::cout << "-dr04_pfiso_gamma " << this->dr04_pfiso_gamma() << std::endl;
    std::cout << "-dr04_pfiso_pu " << this->dr04_pfiso_pu() << std::endl;

  }
}
