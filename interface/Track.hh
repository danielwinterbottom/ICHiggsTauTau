#ifndef ICHiggsTauTau_Track_hh
#define ICHiggsTauTau_Track_hh
#include <vector>
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include "Math/Vector3D.h"
#include "Math/Vector3Dfwd.h"
#include "Rtypes.h"

namespace ic {

/**
 * @brief Stores limited information about the track kinematics and trajectory
 */
class Track {
 private:
  typedef ROOT::Math::RhoEtaPhiVector ThreeVector;
  typedef ROOT::Math::XYZPoint Point;

 public:
  Track();
  virtual ~Track();
  virtual void Print() const;

  /// @name Properties
  /**@{*/
  /// The track momentum
  inline ThreeVector const& momentum() const { return momentum_; }

  /// The point-of-closest-approach (PCA) of the track to the beamspot
  inline Point const& ref_point() const { return ref_point_; }

  /// The unique identifier
  inline std::size_t id() const { return id_; }

  /// The track transverse momentum
  inline double pt() const { return momentum_.Rho(); }

  /// Energy under the assumption of a massless particle, e.g.
  /// \f$E =|\vec{p}|\f$
  inline double energy() const { return momentum_.r(); }

  /// Track pseudorapidity
  inline double eta() const { return momentum_.Eta(); }

  /// Track azimuthal angle
  inline double phi() const { return momentum_.Phi(); }

  /// The z-coordinate of the PCA
  inline double vx() const { return ref_point_.x(); }

  /// The y-coordinate of the PCA
  inline double vy() const { return ref_point_.y(); }

  /// The z-coordinate of the PCA
  inline double vz() const { return ref_point_.z(); }

  /// The normalised chi2 of the track fit
  inline double normalized_chi2() const { return normalized_chi2_; }

  /// Number of tracker hits
  inline int hits() const { return hits_; }

  /// Number of pixel hits
  inline int pixel_hits() const { return pixel_hits_; }

  /// Approximate dxy
  /// Copied from DataFormats/TrackReco/interface/TrackBase.h
  inline double dxy(Point const& point) {
    return (-(vx() - point.x()) * momentum().y() +
            (vy() - point.y()) * momentum().z()) /
           pt();
  }

  /// Approximate dz
  /// Copied from DataFormats/TrackReco/interface/TrackBase.h
  inline double dz(Point const& point) {
    return (vz() - point.z()) -
           ((vx() - point.x()) * momentum().x() +
            (vy() - point.y()) * momentum().y()) /
               pt() * momentum().z() / pt();
  }

  /// The track charge
  inline int charge() const { return charge_; }
  /**@}*/

  /// @name Setters
  /**@{*/
  /// @copybrief momentum()
  inline void set_momentum(ThreeVector const& momentum) {
    momentum_ = momentum;
  }

  /// @copybrief ref_point()
  inline void set_ref_point(Point const& ref_point) { ref_point_ = ref_point; }

  /// @copybrief id()
  inline void set_id(std::size_t const& id) { id_ = id; }

  /// @copybrief pt()
  inline void set_pt(double const& pt) { momentum_.SetRho(pt); }

  /// @copybrief eta()
  inline void set_eta(double const& eta) { momentum_.SetEta(eta); }

  /// @copybrief phi()
  inline void set_phi(double const& phi) { momentum_.SetPhi(phi); }

  /// @copybrief vx()
  inline void set_vx(double const& x) { ref_point_.SetX(x); }

  /// @copybrief vy()
  inline void set_vy(double const& y) { ref_point_.SetY(y); }

  /// @copybrief vz()
  inline void set_vz(double const& z) { ref_point_.SetZ(z); }

  /// @copybrief normalized_chi2()
  inline void set_normalized_chi2(double const& normalized_chi2) {
    normalized_chi2_ = normalized_chi2;
  }

  /// @copybrief hits()
  inline void set_hits(int const& hits) {
    hits_ = hits;
  }

  /// @copybrief pixel_hits()
  inline void set_pixel_hits(int const& pixel_hits) {
    pixel_hits_ = pixel_hits;
  }

  /// @copybrief charge()
  inline void set_charge(int const& charge) { charge_ = charge; }
  /**@}*/

 private:
  ThreeVector momentum_;
  Point ref_point_;
  std::size_t id_;
  int charge_;

  double normalized_chi2_;
  int hits_;
  int pixel_hits_;


 #ifndef SKIP_CINT_DICT
 public:
  ClassDef(Track, 3);
 #endif
};

typedef std::vector<ic::Track> TrackCollection;
}
/** \example plugins/ICTrackProducer.cc */
#endif
