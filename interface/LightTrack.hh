#ifndef ICHiggsTauTau_LightTrack_hh
#define ICHiggsTauTau_LightTrack_hh
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include "Math/Vector3D.h"
#include "Math/Vector3Dfwd.h"
#include <vector>

namespace ic {


  class LightTrack {

    private:

    public:
      LightTrack();
      virtual ~LightTrack();

      inline std::size_t id() const { return id_; }
      inline void set_id(std::size_t const& id) { id_ = id; }

      inline double pt() const { return pt_; }
      inline void set_pt(double const& pt) { pt_ = pt; }

      inline double vz() const { return vz_; }
      inline void set_vz(double const& vz) { vz_ = vz; }

      virtual void Print() const;

    private:
      float pt_;
      float vz_;
      std::size_t id_;
  };

  typedef std::vector<ic::LightTrack> LightTrackCollection;

}
#endif
