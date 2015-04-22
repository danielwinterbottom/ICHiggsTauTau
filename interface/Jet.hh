#ifndef ICHiggsTauTau_Jet_hh
#define ICHiggsTauTau_Jet_hh
#include <map>
#include <string>
#include <vector>
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "Rtypes.h"

namespace ic {

/**
 * @brief A basic jet class to store properties common to all jet types
 *
 * Useful links:
 *  - https://twiki.cern.ch/twiki/bin/viewauth/CMS/IntroToJEC
 */
class Jet : public Candidate {
 private:
  typedef std::map<std::size_t, float> UFmap;
  typedef std::map<std::size_t, std::string> TSmap;

 public:
  Jet();
  virtual ~Jet();
  virtual void Print() const;

  /// @name Properties
  /**@{*/
  /**
   * @brief A map containing the jet energy corrections, where the key is stored
   * as a hash of the identifying string. Important: see the details below for
   * the definition of the correction factors.
   * @details In CMS jet energy corrections are applied sequentially, and scale
   * the entire four-momentum. See [this
   * link](https://twiki.cern.ch/twiki/bin/viewauth/CMS/IntroToJEC) for further
   * information. This implies that the correction factor at a given level
   * depends on the jet four-momentum already being corrected to the previous
   * level.
   */
  inline std::map<std::size_t, float> const& jec_factors() const {
    return jec_factors_;
  }

  /// A map containing the b-tagging discriminators, where the key is stored as
  /// a hash of the identifying string
  inline std::map<std::size_t, float> const& b_discriminators() const {
    return b_discriminators_;
  }

  /// A vector to refer to ic::GenParticle::id() values
  /**
   * @deprecated This property is no longer filled in the ICJetProducer. It
   * is advised to save generator-level particles or jets that may be needed for
   * matching.
   */
  inline std::vector<std::size_t> const& gen_particles() const {
    return gen_particles_;
  }

  /// A map containing the b-tagging discriminators, where the key is stored as
  /// a hash of the identifying string
  inline std::vector<std::size_t> const& secondary_vertices() const {
    return secondary_vertices_;
  }

  /// The raw value of the jet energy
  inline double const& uncorrected_energy() const {
    return uncorrected_energy_;
  }

  /// The jet area as used in the L1FastJet correction
  inline float const& jet_area() const { return jet_area_; }

  /// An algorithmic determination of the true flavour of the jet, e.g. gluon,
  /// light, c or b quark. This is defined only in simulation and returns a PDG
  /// code.
  inline int const& parton_flavour() const { return parton_flavour_; }

  /// Returns a specific correction factor if `name` is defined, otherwise
  /// returns zero
  float GetJecFactor(std::string const& name) const;

  /// Returns a specific discriminator value if `name` is defined, otherwise
  /// returns zero
  float GetBDiscriminator(std::string const& name) const;
  /**@}*/

  /// @name Setters
  /**@{*/
  /// @copybrief jec_factors()
  inline void set_jec_factors(std::map<std::size_t, float> const& jec_factors) {
    jec_factors_ = jec_factors;
  }

  /// @copybrief b_discriminators()
  inline void set_b_discriminators(
      std::map<std::size_t, float> const& b_discriminators) {
    b_discriminators_ = b_discriminators;
  }

  /// @copybrief gen_particles()
  inline void set_gen_particles(std::vector<std::size_t> const& gen_particles) {
    gen_particles_ = gen_particles;
  }

  /// @copybrief secondary_vertices()
  inline void set_secondary_vertices(
      std::vector<std::size_t> const& secondary_vertices) {
    secondary_vertices_ = secondary_vertices;
  }

  /// @copybrief uncorrected_energy()
  inline void set_uncorrected_energy(double const& uncorrected_energy) {
    uncorrected_energy_ = uncorrected_energy;
  }

  /// @copybrief jet_area()
  inline void set_jet_area(float const& jet_area) { jet_area_ = jet_area; }

  /// @copybrief parton_flavour()
  inline void set_parton_flavour(int const& parton_flavour) {
    parton_flavour_ = parton_flavour;
  }

  inline void set_pu_jet_id(double const& pu_jet_id){
    pu_jet_id_ = pu_jet_id;
  }


  /// Store a jet energy correction factor, overwriting any existing value with
  /// label `name`
  void SetJecFactor(std::string const& name, float const& value);

  /// Store a b-tagging discriminator, overwriting any existing value with
  /// label `name`
  void SetBDiscriminator(std::string const& name, float const& value);
  /**@}*/

 private:
  UFmap jec_factors_;
  UFmap b_discriminators_;
  std::vector<std::size_t> gen_particles_;
  std::vector<std::size_t> secondary_vertices_;
  double uncorrected_energy_;
  float jet_area_;
  double pu_jet_id_;
  int parton_flavour_;
  

  std::string UnHashJecFactor(std::size_t jec) const;
  std::string UnHashDiscr(std::size_t dis) const;

 #ifndef SKIP_CINT_DICT
 public:
  ClassDef(Jet, 3);
 #endif
};

typedef std::vector<ic::Jet> JetCollection;
}

/** \example plugins/ICJetProducer.hh */
/** \example plugins/ICJetDestHelper.hh */
#endif
