#ifndef ICHiggsTauTau_Vertex_hh
#define ICHiggsTauTau_Vertex_hh
#include <utility>
#include <vector>
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"

namespace ic {

/**
 * @brief Stores information about the position of an event vertex and the
 * quality of the track fit
 */
class Vertex {
 private:
  typedef ROOT::Math::XYZPoint Point;
  typedef std::pair<std::size_t, float> TrkPair;

 public:
  Vertex();
  virtual ~Vertex();
  virtual void Print() const;

  /// @name Properties
  /**@{*/
  /// The vertex position co-ordinates
  inline Point const& point() const { return point_; }

  /// Unique identifier
  inline std::size_t id() const { return id_; }

  /// Vertex x-coordinate
  inline float vx() const { return point_.x(); }

  /// Vertex y-coordinate
  inline float vy() const { return point_.y(); }

  /// Vertex z-coordinate
  inline float vz() const { return point_.z(); }

  /// The \f$\chi^{2}\f$ of the vertex fit
  inline float chi2() const { return chi2_; }

  /// The number-of-degrees-of-freedom in the vertex fit
  inline float ndof() const { return ndof_; }

  /// A vector of track information pairs, where each contains an
  /// ic::Track::id() and the weight of that track in the vertex fit
  inline std::vector<std::pair<std::size_t, float> > const& tracks() const {
    return tracks_;
  }
  /**@}*/

  /// @name Setters
  /**@{*/

  /// @copybrief point()
  inline void set_point(Point const& point) { point_ = point; }

  /// @copybrief id()
  inline void set_id(std::size_t const& id) { id_ = id; }

  /// @copybrief vx()
  inline void set_vx(float const& x) { point_.SetX(x); }

  /// @copybrief vy()
  inline void set_vy(float const& y) { point_.SetY(y); }

  /// @copybrief vz()
  inline void set_vz(float const& z) { point_.SetZ(z); }

  /// @copybrief chi2()
  inline void set_chi2(float const& chi2) { chi2_ = chi2; }

  /// @copybrief ndof()
  inline void set_ndof(float const& ndof) { ndof_ = ndof; }

  /// @copybrief tracks()
  inline void set_tracks(
      std::vector<std::pair<std::size_t, float> > const& tracks) {
    tracks_ = tracks;
  }

  /// Add an ic::Track::id with a vertex fit weight
  inline void AddTrack(std::size_t id, float weight) {
    tracks_.push_back(std::make_pair(id, weight));
  }
  /**@}*/

 private:
  Point point_;
  float chi2_;
  float ndof_;
  std::vector<TrkPair> tracks_;
  std::size_t id_;
};

typedef std::vector<ic::Vertex> VertexCollection;
}
/** \example plugins/ICVertexProducer.cc */
#endif
