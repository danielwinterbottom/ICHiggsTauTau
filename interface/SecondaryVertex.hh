#ifndef ICHiggsTauTau_SecondaryVertex_hh
#define ICHiggsTauTau_SecondaryVertex_hh
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include <utility>
#include <vector>
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"

namespace ic {
  
  class SecondaryVertex : public Vertex {

    private:
      typedef ROOT::Math::XYZPoint Point;
      typedef std::pair<std::size_t, float> TrkPair;

    public:
      SecondaryVertex();
      virtual ~SecondaryVertex();
      
      inline double distance_3d() const { return distance_3d_; }
      inline void set_distance_3d(double  const& distance_3d) { distance_3d_ = distance_3d; }

      inline double distance_err_3d() const { return distance_err_3d_; }
      inline void set_distance_err_3d(double  const& distance_err_3d) { distance_err_3d_ = distance_err_3d; }
      
      virtual void Print() const;

    private:
      double distance_3d_;
      double distance_err_3d_;
  };

  typedef std::vector<ic::SecondaryVertex> SecondaryVertexCollection;
  typedef std::vector<ic::SecondaryVertex *> SecondaryVertexPtrVec;

}
#endif
