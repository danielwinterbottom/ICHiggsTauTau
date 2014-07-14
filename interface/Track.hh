#ifndef ICHiggsTauTau_Track_hh
#define ICHiggsTauTau_Track_hh
#include <vector>
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include "Math/Vector3D.h"
#include "Math/Vector3Dfwd.h"

namespace ic {

class Track {
 private:
  typedef ROOT::Math::RhoEtaPhiVector ThreeVector;
  typedef ROOT::Math::XYZPoint Point;

 public:
  Track();
  virtual ~Track();

  inline ThreeVector const& momentum() const { return momentum_; }
  inline void set_momentum(ThreeVector const& momentum) {
    momentum_ = momentum;
  }

  inline Point const& ref_point() const { return ref_point_; }
  inline void set_ref_point(Point const& ref_point) { ref_point_ = ref_point; }

  inline std::size_t id() const { return id_; }
  inline void set_id(std::size_t const& id) { id_ = id; }

  inline double pt() const { return momentum_.Rho(); }
  inline void set_pt(double const& pt) { momentum_.SetRho(pt); }

  inline double energy() const { return momentum_.r(); }

  inline double eta() const { return momentum_.Eta(); }
  inline void set_eta(double const& eta) { momentum_.SetEta(eta); }

  inline double phi() const { return momentum_.Phi(); }
  inline void set_phi(double const& phi) { momentum_.SetPhi(phi); }

  inline double vx() const { return ref_point_.x(); }
  inline void set_vx(double const& x) { ref_point_.SetX(x); }

  inline double vy() const { return ref_point_.y(); }
  inline void set_vy(double const& y) { ref_point_.SetY(y); }

  inline double vz() const { return ref_point_.z(); }
  inline void set_vz(double const& z) { ref_point_.SetZ(z); }

  inline int charge() const { return charge_; }
  inline void set_charge(int const& charge) { charge_ = charge; }

  virtual void Print() const;

 private:
  ThreeVector momentum_;
  Point ref_point_;
  std::size_t id_;
  int charge_;
};

typedef std::vector<ic::Track> TrackCollection;
}
#endif
