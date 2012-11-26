#ifndef ICHiggsTauTau_Candidate_hh
#define ICHiggsTauTau_Candidate_hh
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include <vector>

namespace ic {


  //! A Candidate class
  /*!
    A basic class containing a four-momentum vector, a charge and an ID. 
  */


  class Candidate {

    private:
      typedef ROOT::Math::PtEtaPhiEVector Vector;

    public:
      Candidate();
      virtual ~Candidate();

      //! The Candidate four-momentum
      /*! Returns a ROOT PtEtaPhiEVector, from which a large number of kinematic
      variables can be calculated. Please note, specific getter functions are provided 
      by the Candidate class for the most commonly used variables(pt, eta, phi, energy and mass).
      */
      inline Vector const& vector() const { return vector_; }
      inline void set_vector(Vector const& vector) { vector_ = vector; }
      
      //! The object ID
      /*! Each Candidate object (and any object deriving from it) can have a unique
          ID assigned.  This makes it possible for one object to refer to any number
          of other objects in a given event.  For example, A GenJet may contain the 
          IDs of its GenParticle constituents. In another case, a PFJet contain the
          IDs of its constituent tracks.  
      */
      inline std::size_t id() const { return id_; }
      inline void set_id(std::size_t const& id) { id_ = id; }

      inline double pt() const { return vector_.Pt(); }
      inline void set_pt(double const& pt) { vector_.SetPt(pt); }

      inline double eta() const { return vector_.Eta(); }
      inline void set_eta(double const& eta) { vector_.SetEta(eta); }

      inline double phi() const { return vector_.Phi(); }
      inline void set_phi(double const& phi) { vector_.SetPhi(phi); }

      inline double energy() const { return vector_.E(); }
      inline void set_energy(double const& energy) { vector_.SetE(energy); }

      inline int charge() const { return charge_; }
      inline void set_charge(int const& charge) { charge_ = charge; }

      inline double M() const { return vector_.M(); }

      virtual void Print() const;


    private:


      Vector vector_;   
      std::size_t id_;
      int charge_;
  };

  typedef std::vector<ic::Candidate> CandidateCollection;

}
#endif
