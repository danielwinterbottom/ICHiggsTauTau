#include "../interface/Met.hh"
#include <map>
#include <string>
#include <vector>
#include "../interface/city.h"
#include "boost/format.hpp"

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
  
  void Met::Print() const { 
    Candidate::Print(); 
    
    std::cout << "--corrected METs--" << std::endl;
    UBMmap::const_iterator uf_it;
    for (uf_it = correctedmets_.begin(); uf_it != correctedmets_.end(); ++uf_it) {
      std::cout << boost::format("%-30s %-30s\n") %
	UnHashMetCor(uf_it->first) % uf_it->second;
    }
  }

  void Met::SetCorrectedMet(std::string const& name, ic::Met::BasicMet const& factor) {
    correctedmets_[CityHash64(name)] = factor;
  }

  void Met::SetShiftedMet(std::string const& name, ic::Met::BasicMet const& factor) {
    shiftedmets_[CityHash64(name)] = factor;
  }

  ic::Met::BasicMet Met::GetCorrectedMet(std::string const& name) const {
    UBMmap::const_iterator iter = correctedmets_.find(CityHash64(name));
    if (iter != correctedmets_.end()) {
      return iter->second;
    } else {
      std::cerr << "Warning in <Met::GetCorrectedMet>: MET \"" << name
                << "\" not found" << std::endl;
      ic::Met::BasicMet tmp;
      tmp.px=0;
      tmp.py=0;
      tmp.sumet=0;
      return tmp;
    }
  }

  ic::Met::BasicMet Met::GetShiftedMet(std::string const& name) const {
    UBMmap::const_iterator iter = shiftedmets_.find(CityHash64(name));
    if (iter != shiftedmets_.end()) {
      return iter->second;
    } else {
      std::cerr << "Warning in <Met::GetShiftedMet>: MET \"" << name
                << "\" not found" << std::endl;
      ic::Met::BasicMet tmp;
      tmp.px=0;
      tmp.py=0;
      tmp.sumet=0;
      return tmp;
    }
  }

  std::string Met::UnHashMetCor(std::size_t cor) const {
    static TSmap dict;
    static bool is_set = false;
    if (!is_set) {
      dict[CityHash64("Raw")] = "Raw";
      dict[CityHash64("Type1")] = "Type1";
      dict[CityHash64("Type01")] = "Type01";
      dict[CityHash64("TypeXY")] = "TypeXY";
      dict[CityHash64("Type1XY")] = "Type1XY";
      dict[CityHash64("Type01XY")] = "Type01XY";
      dict[CityHash64("Type1Smear")] = "Type1Smear";
      dict[CityHash64("Type01Smear")] = "Type01Smear";
      dict[CityHash64("Type1SmearXY")] = "Type1SmearXY";
      dict[CityHash64("Type01SmearXY")] = "Type01SmearXY";
      dict[CityHash64("RawCalo")] = "RawCalo";
    }
    is_set = true;
    TSmap::const_iterator it = dict.find(cor);
    if (it != dict.end()) {
      return it->second;
    } else {
      std::cerr << "Warning in <Met>: Unable to unhash \"" << cor
                << "\", returning empty string" << std::endl;
    }
    return "";
  }

  std::string Met::UnHashMetUnc(std::size_t unc) const {
    static TSmap dict;
    static bool is_set = false;
    if (!is_set) {
      dict[CityHash64("JetResUp")] = "JetResUp";
      dict[CityHash64("JetResDown")] = "JetResDown";
      dict[CityHash64("JetEnUp")] = "JetEnUp";
      dict[CityHash64("JetEnDown")] = "JetEnDown";
      dict[CityHash64("MuonEnUp")] = "MuonEnUp";
      dict[CityHash64("MuonEnDown")] = "MuonEnDown";
      dict[CityHash64("ElectronEnUp")] = "ElectronEnUp";
      dict[CityHash64("ElectronEnDown")] = "ElectronEnDown";
      dict[CityHash64("TauEnUp")] = "TauEnUp";
      dict[CityHash64("TauEnDown")] = "TauEnDown";
      dict[CityHash64("UnclusteredEnUp")] = "UnclusteredEnUp";
      dict[CityHash64("UnclusteredEnDown")] = "UnclusteredEnDown";
      dict[CityHash64("PhotonEnUp")] = "PhotonEnUp";
      dict[CityHash64("PhotonEnDown")] = "PhotonEnDown";
      dict[CityHash64("NoShift")] = "NoShift";
    }
    is_set = true;
    TSmap::const_iterator it = dict.find(unc);
    if (it != dict.end()) {
      return it->second;
    } else {
      std::cerr << "Warning in <Met>: Unable to unhash \"" << unc
                << "\", returning empty string" << std::endl;
    }
    return "";
  }

  std::ostream& operator<<(std::ostream& os,const ic::Met::BasicMet& met)
  {
    return os << " pt=" << met.pt() << " phi=" << met.phi() << " sumet=" << met.sumet << std::endl;
  }
  
}//namespace
