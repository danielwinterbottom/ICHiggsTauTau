#ifndef ICHiggsTauTau_SuperCluster_hh
#define ICHiggsTauTau_SuperCluster_hh
#include <map>
#include <string>
#include <vector>
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"

namespace ic {

class SuperCluster {
 private:
  typedef ROOT::Math::XYZPoint Point;
  typedef ROOT::Math::PtEtaPhiEVector Vector;

 public:
  SuperCluster();
  virtual ~SuperCluster();

  virtual void Print() const;

  inline Point const& point() const { return point_; }
  inline void set_point(Point const& point) { point_ = point; }

  inline double vx() const { return point_.x(); }
  inline void set_vx(double const& x) { point_.SetX(x); }

  inline double vy() const { return point_.y(); }
  inline void set_vy(double const& y) { point_.SetY(y); }

  inline double vz() const { return point_.z(); }
  inline void set_vz(double const& z) { point_.SetZ(z); }

  inline double pt() const { return (energy_ * sin(point_.theta())); }

  inline double eta() const { return (point_.eta()); }

  inline double phi() const { return (point_.phi()); }

  inline Vector vector() const { return Vector(pt(), eta(), phi(), energy_); }

  inline std::size_t id() const { return id_; }
  inline void set_id(std::size_t const& id) { id_ = id; }

  inline double const& energy() const { return energy_; }
  inline void set_energy(double const& energy) { energy_ = energy; }

  inline double const& raw_energy() const { return raw_energy_; }
  inline void set_raw_energy(double const& raw_energy) {
    raw_energy_ = raw_energy;
  }

  inline bool const& is_barrel() const { return is_barrel_; }
  inline void set_is_barrel(bool const& is_barrel) { is_barrel_ = is_barrel; }

 private:
  Point point_;
  std::size_t id_;
  double energy_;
  double raw_energy_;
  bool is_barrel_;
};

typedef std::vector<ic::SuperCluster> SuperClusterCollection;
}
#endif
