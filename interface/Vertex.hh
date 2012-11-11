#ifndef ICHiggsTauTau_Vertex_hh
#define ICHiggsTauTau_Vertex_hh
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include <utility>
#include <vector>

namespace ic {


  class Vertex {

    private:
      typedef ROOT::Math::XYZPoint Point;
      typedef std::pair<std::size_t, float> TrkPair;

    public:
      Vertex();
      virtual ~Vertex();

      inline Point const& point() const { return point_; }
      inline void set_point(Point const& point) { point_ = point; }
            
      inline std::size_t id() const { return id_; }
      inline void set_id(std::size_t const& id) { id_ = id; }

      inline float vx() const { return point_.x(); }
      inline void set_vx(float const& x) { point_.SetX(x); }

      // inline float vx_error() const { return vx_error_; }
      // inline void set_vx_error(float const& vx_error) { vx_error_ = vx_error; }

      inline float vy() const { return point_.y(); }
      inline void set_vy(float const& y) { point_.SetY(y); }

      // inline float vy_error() const { return vy_error_; }
      // inline void set_vy_error(float const& vy_error) { vy_error_ = vy_error; }

      inline float vz() const { return point_.z(); }
      inline void set_vz(float const& z) { point_.SetZ(z); }

      // inline float vz_error() const { return vz_error_; }
      // inline void set_vz_error(float const& vz_error) { vz_error_ = vz_error; }

      inline float chi2() const { return chi2_; }
      inline void set_chi2(float const& chi2) { chi2_ = chi2; }

      inline float ndof() const { return ndof_; }
      inline void set_ndof(float const& ndof) { ndof_ = ndof; }

      inline std::vector<TrkPair> const& tracks() const { return tracks_; }
      inline void set_tracks(std::vector<TrkPair> const& tracks) { tracks_ = tracks; }

      inline void AddTrack(std::size_t id, float weight) {
            tracks_.push_back(std::make_pair(id,weight));
      }

      virtual void Print() const;


    private:
      Point point_;
      // float vx_error_;
      // float vy_error_;
      // float vz_error_;
      float chi2_;
      float ndof_;
      std::vector<TrkPair> tracks_;


      std::size_t id_;
  };

  typedef std::vector<ic::Vertex> VertexCollection;
  typedef std::vector<ic::Vertex *> VertexPtrVec;

}
#endif
