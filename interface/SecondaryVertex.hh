#ifndef ICHiggsTauTau_SecondaryVertex_hh
#define ICHiggsTauTau_SecondaryVertex_hh
#include <vector>
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"

namespace ic {

/**
 * @brief A class deriving from ic::Vertex and adding information on the 3D
 * flight distance
 */
class SecondaryVertex : public Vertex {
 public:
  SecondaryVertex();
  virtual ~SecondaryVertex();
  virtual void Print() const;

  /// @name Properties
  /**@{*/
  /// 3D flight-distance, in centimetres, from the primary to the secondary
  /// vertex
  inline double distance_3d() const { return distance_3d_; }

  /// 3D flight-distance uncertainty, in centimetres
  inline double distance_err_3d() const { return distance_err_3d_; }
  /**@}*/

  /// @name Setters
  /**@{*/
  /// @copybrief distance_3d()
  inline void set_distance_3d(double const& distance_3d) {
    distance_3d_ = distance_3d;
  }
  /// @copybrief distance_err_3d()
  inline void set_distance_err_3d(double const& distance_err_3d) {
    distance_err_3d_ = distance_err_3d;
  }
  /**@}*/

 private:
  double distance_3d_;
  double distance_err_3d_;
};

typedef std::vector<ic::SecondaryVertex> SecondaryVertexCollection;
}
/** \example plugins/ICSecondaryVertexProducer.cc */
#endif
