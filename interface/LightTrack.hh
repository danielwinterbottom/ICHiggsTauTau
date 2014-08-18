#ifndef ICHiggsTauTau_LightTrack_hh
#define ICHiggsTauTau_LightTrack_hh
#include <vector>
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include "Math/Vector3D.h"
#include "Math/Vector3Dfwd.h"
#include "Rtypes.h"

namespace ic {

/**
 * @brief Stores a minimal amount of track information. May be used instead of
 * the ic::Track class to save file space.
 */
class LightTrack {
 public:
  LightTrack();
  virtual ~LightTrack();
  virtual void Print() const;

  /// @name Properties
  /**@{*/
  /// The unique identifier
  inline std::size_t id() const { return id_; }

  /// The track transverse momentum
  inline double pt() const { return pt_; }

  /// The z-coordinate of the point-of-closest-approach to the beamspot
  inline double vz() const { return vz_; }
  /**@}*/

  /// @name Setters
  /**@{*/
  /// @copybrief id()
  inline void set_id(std::size_t const& id) { id_ = id; }

  /// @copybrief pt()
  inline void set_pt(double const& pt) { pt_ = pt; }

  /// @copybrief vz()
  inline void set_vz(double const& vz) { vz_ = vz; }
  /**@}*/

 private:
  float pt_;
  float vz_;
  std::size_t id_;

 #ifndef SKIP_CINT_DICT
 public:
  ClassDef(LightTrack, 2);
 #endif
};

typedef std::vector<ic::LightTrack> LightTrackCollection;
}
/** \example plugins/ICLightTrackProducer.cc */
#endif
