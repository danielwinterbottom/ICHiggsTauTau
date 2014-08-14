#ifndef ICHiggsTauTau_Photon_hh
#define ICHiggsTauTau_Photon_hh
#include <vector>
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "Rtypes.h"

namespace ic {

/**
 * @brief This class stores a subset of the reco::Photon
 * properties which are most commonly used in analysis.
 */
class Photon : public Candidate {
 public:
  Photon();
  virtual ~Photon();
  virtual void Print() const;

  /// @name Properties
  /**@{*/
  /// PF isolation, using charged hadrons in a cone with \f$ \Delta R = 0.3 \f$
  inline float dr03_pfiso_charged() const { return dr03_pfiso_charged_; }

  /// PF isolation, using neutral hadrons in a cone with \f$ \Delta R = 0.3 \f$
  inline float dr03_pfiso_neutral() const { return dr03_pfiso_neutral_; }

  /// PF isolation, using photons in a cone with \f$ \Delta R = 0.3 \f$
  inline float dr03_pfiso_gamma() const { return dr03_pfiso_gamma_; }

  /// Hadronic over electromagnetic energy fraction
  inline float had_tower_over_em() const { return had_tower_over_em_; }

  /// Weighted cluster RMS in the \f$\eta\f$-direction, measured in units
  /// of crystals in a 5x5 block centred on the seed crystal
  inline float sigma_IetaIeta() const { return sigma_IetaIeta_; }

  /// True if photon passes the electron veto
  inline bool pass_electron_veto() const { return pass_electron_veto_; }
  /**@}*/

  /// @name Setters
  /**@{*/
  /// @copybrief dr03_pfiso_charged()
  inline void set_dr03_pfiso_charged(float const& dr03_pfiso_charged) {
    dr03_pfiso_charged_ = dr03_pfiso_charged;
  }

  /// @copybrief dr03_pfiso_neutral()
  inline void set_dr03_pfiso_neutral(float const& dr03_pfiso_neutral) {
    dr03_pfiso_neutral_ = dr03_pfiso_neutral;
  }

  /// @copybrief dr03_pfiso_gamma()
  inline void set_dr03_pfiso_gamma(float const& dr03_pfiso_gamma) {
    dr03_pfiso_gamma_ = dr03_pfiso_gamma;
  }

  /// @copybrief had_tower_over_em()
  inline void set_had_tower_over_em(float const& had_tower_over_em) {
    had_tower_over_em_ = had_tower_over_em;
  }

  /// @copybrief sigma_IetaIeta()
  inline void set_sigma_IetaIeta(float const& sigma_IetaIeta) {
    sigma_IetaIeta_ = sigma_IetaIeta;
  }

  /// @copybrief pass_electron_veto()
  inline void set_pass_electron_veto(bool const& pass_electron_veto) {
    pass_electron_veto_ = pass_electron_veto;
  }
  /**@}*/

 private:
  float dr03_pfiso_charged_;
  float dr03_pfiso_neutral_;
  float dr03_pfiso_gamma_;

  float had_tower_over_em_;
  float sigma_IetaIeta_;

  bool pass_electron_veto_;

 public:
  ClassDef(Photon, 2);
};

typedef std::vector<ic::Photon> PhotonCollection;
}
/** \example plugins/ICPhotonProducer.cc */
#endif
