#ifndef ICHiggsTauTau_Tau_hh
#define ICHiggsTauTau_Tau_hh
#include <map>
#include <vector>
#include <string>
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

namespace ic {

class Tau : public Candidate {
 private:
  typedef std::map<std::size_t, float> UFmap;
  typedef std::map<std::size_t, std::string> TSmap;
  typedef ROOT::Math::XYZPoint Point;

 public:
  Tau();
  virtual ~Tau();

  inline UFmap const& tau_ids() const { return tau_ids_; }
  inline void set_tau_ids(UFmap const& tau_ids) { tau_ids_ = tau_ids; }

  inline int const& decay_mode() const { return decay_mode_; }
  inline void set_decay_mode(int const& decay_mode) {
    decay_mode_ = decay_mode;
  }

  inline float const& lead_ecal_energy() const { return lead_ecal_energy_; }
  inline void set_lead_ecal_energy(float const& lead_ecal_energy) {
    lead_ecal_energy_ = lead_ecal_energy;
  }

  inline float const& lead_hcal_energy() const { return lead_hcal_energy_; }
  inline void set_lead_hcal_energy(float const& lead_hcal_energy) {
    lead_hcal_energy_ = lead_hcal_energy;
  }

  inline float const& lead_p() const { return lead_p_; }
  inline void set_lead_p(float const& lead_p) { lead_p_ = lead_p; }

  inline float const& lead_dxy_vertex() const { return lead_dxy_vertex_; }
  inline void set_lead_dxy_vertex(float const& lead_dxy_vertex) {
    lead_dxy_vertex_ = lead_dxy_vertex;
  }

  inline float const& lead_dz_vertex() const { return lead_dz_vertex_; }
  inline void set_lead_dz_vertex(float const& lead_dz_vertex) {
    lead_dz_vertex_ = lead_dz_vertex;
  }

  inline double vx() const { return ref_point_.x(); }
  inline void set_vx(double const& x) { ref_point_.SetX(x); }

  inline double vy() const { return ref_point_.y(); }
  inline void set_vy(double const& y) { ref_point_.SetY(y); }

  inline double vz() const { return ref_point_.z(); }
  inline void set_vz(double const& z) { ref_point_.SetZ(z); }

  inline std::vector<std::size_t> const& constituent_tracks() const {
    return constituent_tracks_;
  }
  inline void set_constituent_tracks(
      std::vector<std::size_t> const& constituent_tracks) {
    constituent_tracks_ = constituent_tracks;
  }

  virtual void Print() const;

  void SetTauID(std::string const& name, float const& value);
  float GetTauID(std::string const& name) const;
  bool HasTauID(std::string const& name) const;

 private:
  UFmap tau_ids_;

  int decay_mode_;

  float lead_ecal_energy_;
  float lead_hcal_energy_;
  float lead_p_;
  float lead_dxy_vertex_;
  float lead_dz_vertex_;

  Point ref_point_;

  std::vector<std::size_t> constituent_tracks_;
};

typedef std::vector<ic::Tau> TauCollection;
}
#endif
