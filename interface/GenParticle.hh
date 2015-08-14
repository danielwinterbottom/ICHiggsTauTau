#ifndef ICHiggsTauTau_GenParticle_hh
#define ICHiggsTauTau_GenParticle_hh
#include <vector>
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "Rtypes.h"


namespace ic {

/**
 * @brief Stores the order of the gen status flags stored inside the ic::GenParticle
 */
enum GenStatusBits {
  IsPrompt = 0,
  IsDecayedLeptonHadron,
  IsTauDecayProduct,
  IsPromptTauDecayProduct,
  IsDirectTauDecayProduct,
  IsDirectPromptTauDecayProduct,
  IsDirectHadronDecayProduct,
  IsHardProcess,
  FromHardProcess,
  IsHardProcessTauDecayProduct,
  IsDirectHardProcessTauDecayProduct,
  FromHardProcessBeforeFSR,
  IsFirstCopy,
  IsLastCopy,
  IsLastCopyBeforeFSR
};


/**
 * @brief Stores the basic properties of generator-level particles as well as
 * mother-daughter relations with other particles
 */
class GenParticle : public Candidate {

 private:
  typedef ROOT::Math::XYZPoint Point;

 public:
  GenParticle();
  virtual ~GenParticle();
  virtual void Print() const;

  /// @name Properties
  /**@{*/
  /// The index position of the particle in the original list
  inline int index() const { return index_; }

  /// PDG number to identify the particle type, see
  /// [this link] (http://pdg.lbl.gov/2002/montecarlorpp.pdf)
  inline int pdgid() const { return pdgid_; }

  /// The generator-dependent particle status
  inline int status() const { return status_; }

  /// A vector of ic::GenParticle::index() values that identify the mother
  /// particles
  inline std::vector<int> const& mothers() const { return mothers_; }

  /// A vector of ic::GenParticle::index() values that identify the daughter
  /// particles
  inline std::vector<int> const& daughters() const { return daughters_; }

  /// A genstatusflags object to give information about the production of the particle
  inline std::vector<bool> const& statusFlags() const { return statusFlags_; }
  /**@}*/

  /// @name Setters
  /**@{*/
  /// @copybrief index()
  inline void set_index(int const& index) { index_ = index; }

  /// @copybrief pdgid()
  inline void set_pdgid(int const& pdgid) { pdgid_ = pdgid; }

  /// @copybrief status()
  inline void set_status(int const& status) { status_ = status; }

  /// @copybrief mothers()
  inline void set_mothers(std::vector<int> const& mothers) {
    mothers_ = mothers;
  }

  /// @copybrief daughters()
  inline void set_daughters(std::vector<int> const& daughters) {
    daughters_ = daughters;
  }

  /// @copybrief statusFlags()
  inline void set_statusFlags(std::vector<bool> const& statusFlags) {
    statusFlags_ = statusFlags;
  }
  /**@}*/

 private:
  int index_;
  int pdgid_;
  int status_;
  std::vector<int> mothers_;
  std::vector<int> daughters_;
  std::vector<bool> statusFlags_;

 #ifndef SKIP_CINT_DICT
 public:
  ClassDef(GenParticle, 4);
 #endif
};

typedef std::vector<ic::GenParticle> GenParticleCollection;
}
/** \example plugins/ICGenParticleProducer.cc */
#endif
