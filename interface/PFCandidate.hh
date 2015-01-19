#ifndef ICHiggsTauTau_PFCandidate_hh
#define ICHiggsTauTau_PFCandidate_hh
#include <vector>
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include "../interface/Candidate.hh"
#include "Rtypes.h"

namespace ic {

/**
 * @brief Stores the basic properties of PFCandidates (and PackedCandidates for
 * CMSSW 7_X_Y)
 */
class PFCandidate : public Candidate {
 private:
  // typedef ROOT::Math::XYZPoint Point;

 public:
  PFCandidate();
  virtual ~PFCandidate();
  virtual void Print() const;

  /// @name Properties
  /**@{*/

  /// PDG number to identify the PF candidate type
  inline int pdgid() const { return pdgid_; }

  /// A vector to refer to the constituent track ic::Track::id() values
  inline std::vector<std::size_t> const& constituent_tracks() const {
    return constituent_tracks_;
  }

  /// A vector to refer to the constituent gsf track ic::Track::id() values
  inline std::vector<std::size_t> const& constituent_gsf_tracks() const {
    return constituent_gsf_tracks_;
  }
  /**@}*/

  /// @name Setters
  /**@{*/
  /// @copybrief pdgid()
  inline void set_pdgid(int const& pdgid) { pdgid_ = pdgid; }

  /// @copybrief constituent_tracks()
  inline void set_constituent_tracks(
      std::vector<std::size_t> const& constituent_tracks) {
    constituent_tracks_ = constituent_tracks;
  }

  inline void set_constituent_gsf_tracks(
      std::vector<std::size_t> const& constituent_gsf_tracks) {
    constituent_gsf_tracks_ = constituent_gsf_tracks;
  }
  /**@}*/

 private:
  int pdgid_;
  std::vector<std::size_t> constituent_tracks_;
  std::vector<std::size_t> constituent_gsf_tracks_;


 #ifndef SKIP_CINT_DICT
 public:
  ClassDef(PFCandidate, 2);
 #endif
};

typedef std::vector<ic::PFCandidate> PFCandidateCollection;
}
/** \example plugins/ICPFCandidateProducer.cc */
#endif
