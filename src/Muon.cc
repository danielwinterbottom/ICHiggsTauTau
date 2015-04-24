#include "../interface/Muon.hh"
#include "../interface/city.h"

namespace ic {

Muon::Muon()
    : Candidate::Candidate(),
      dr03_tk_sum_pt_(0.),
      dr03_ecal_rechit_sum_et_(0.),
      dr03_hcal_tower_sum_et_(0.),
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
      is_standalone_(false),
      is_global_(false),
      is_tracker_(false),
      is_calo_(false),
      is_pf_(false),
      gt_normalized_chi2_(0.),
      cq_chi2_localposition_(0.),
      cq_trk_kink_(0.),
      gt_valid_muon_hits_(0),
      matched_stations_(0),
      it_pixel_hits_(0),
      it_tracker_hits_(0),
      it_layers_with_measurement_(0),
      it_valid_fraction_(0),
      segment_compatibility_(0),
      dxy_vertex_(0.),
      dz_vertex_(0.),
      dxy_beamspot_(0.) {}

  Muon::~Muon() {}

  void Muon::SetIdIso(std::string const& name, float const& value) {
    muon_idiso_[CityHash64(name)] = value;
  }

  bool Muon::HasIdIso(std::string const& name) const {
    return muon_idiso_.count(CityHash64(name)) > 0;
  }

  float Muon::GetIdIso(std::string const& name) const {
    UFmap::const_iterator iter = muon_idiso_.find(CityHash64(name));
    if (iter != muon_idiso_.end()) {
      return iter->second;
    } else {
      std::cerr << "Warning in <Muon::GetIdIso>: Label \""
          << name << "\" not found" << std::endl;
      return 0.0;
    }
  }

  void Muon::Print() const {
    Candidate::Print();
  }
}
