#ifndef ICHiggsTauTau_GenJet_hh
#define ICHiggsTauTau_GenJet_hh
#include <vector>
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "Rtypes.h"

namespace ic {

/**
 * @brief Stores the basic properties of a generator-level jet, in particular
 * can save the IDs of the  GenParticle constituents
 */
class GenJet : public Candidate {
 public:
  GenJet();
  virtual ~GenJet();
  virtual void Print() const;

  /// @name Properties
  /**@{*/
  /// The true flavour of the jet
  inline int flavour() const { return flavour_; }

  /// The number of GenParticle constituents
  inline unsigned n_constituents() const { return n_constituents_; }

  /// A vector of ic::GenParticle::id() values corresponding to the constituent
  /// particles
  inline std::vector<std::size_t> const& constituents() const {
    return constituents_;
  }
  /**@}*/

  /// @name Setters
  /**@{*/
  /// @copybrief flavour()
  inline void set_flavour(int const& flavour) { flavour_ = flavour; }

  /// @copybrief n_constituents()
  inline void set_n_constituents(unsigned const& n_constituents) {
    n_constituents_ = n_constituents;
  }

  /// @copybrief constituents()
  inline void set_constituents(std::vector<std::size_t> const& constituents) {
    constituents_ = constituents;
  }
  /**@}*/

 private:
  int flavour_;
  unsigned n_constituents_;
  std::vector<std::size_t> constituents_;

 #ifndef SKIP_CINT_DICT
 public:
  ClassDef(GenJet, 2);
 #endif
};

typedef std::vector<ic::GenJet> GenJetCollection;
}
/** \example plugins/ICGenJetProducer.cc */
#endif
