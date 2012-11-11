#ifndef ICHiggsTauTau_GenParticle_hh
#define ICHiggsTauTau_GenParticle_hh
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include <vector>
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"


namespace ic {


  class GenParticle : public Candidate {

    private:
      typedef ROOT::Math::XYZPoint Point;

    public:
      GenParticle();
      virtual ~GenParticle();

      inline Point const& vertex() const { return vertex_; }
      inline void set_vertex(Point const& vertex) { vertex_ = vertex; }
            
      inline int index() const { return index_; }
      inline void set_index(int const& index) { index_ = index; }

      inline int pdgid() const { return pdgid_; }
      inline void set_pdgid(int const& pdgid) { pdgid_ = pdgid; }
      
      inline int status() const { return status_; }
      inline void set_status(int const& status) { status_ = status; }
      
      inline std::vector<int> const& mothers() const { return mothers_; }
      inline void set_mothers(std::vector<int> const& mothers) { mothers_ = mothers; }

      inline std::vector<int> const& daughters() const { return daughters_; }
      inline void set_daughters(std::vector<int> const& daughters) { daughters_ = daughters; }
 
      
      
      virtual void Print() const;


    private:
      int index_;
      int pdgid_;
      int status_;
      Point vertex_;
      std::vector<int> mothers_;
      std::vector<int> daughters_;

  };

  typedef std::vector<ic::GenParticle> GenParticleCollection;

}
#endif
