#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"


namespace ic {
  //Constructors/Destructors
  Muon::Muon() {
  }

  Muon::~Muon() {
  }

  void Muon::SetIdIso(std::string const& name, float const& value) {
    muon_idiso_[CityHash64(name)] = value;
  }
  
  float Muon::GetIdIso(std::string const& name) const {
    UFmap::const_iterator iter = muon_idiso_.find(CityHash64(name));
    if (iter != muon_idiso_.end()) {
      return iter->second;
    } else {
      std::cerr << "Warning in <Electron::GetIdIso>: Label \"" 
          << name << "\" not found" << std::endl;
      return 0.0;
    }
  }

  void Muon::Print() const {
    Candidate::Print();
  }
}
