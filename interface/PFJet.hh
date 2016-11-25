#ifndef ICHiggsTauTau_PFJet_hh
#define ICHiggsTauTau_PFJet_hh
#include <map>
#include <string>
#include "UserCode/ICHiggsTauTau/interface/Jet.hh"
#include "Rtypes.h"

namespace ic {

/**
 * @brief A class deriving from ic::Jet that provides specific information for
 * particle-flow jets
 */
class PFJet : public Jet {
 public:
  PFJet();
  virtual ~PFJet();
  virtual void Print() const;

  /// @name Properties
  /**@{*/
  /// Number of PF muon constituents
  inline int const& muon_multiplicity() const { return muon_multiplicity_; }

  /// Number of charged constituents
  inline int const& charged_multiplicity() const {
    return charged_multiplicity_;
  }

  /// Number of charged hadron constituents
  inline int const& charged_had_multiplicity() const {
    return charged_had_multiplicity_;
  }

  /// Number of neutral hadron constituents
  inline int const& neutral_had_multiplicity() const {
    return neutral_had_multiplicity_;
  }

  /// Number of PF photon constituents
  inline int const& photon_multiplicity() const { return photon_multiplicity_; }

  /// Number of PF electron constituents
  inline int const& electron_multiplicity() const {
    return electron_multiplicity_;
  }

  /// Number PF HF hadronic constituents
  inline int const& HF_had_multiplicity() const { return HF_had_multiplicity_; }

  /// Number of PF HF electromagnetic constituents
  inline int const& HF_em_multiplicity() const { return HF_em_multiplicity_; }

  /// Number of neutral constituents
  inline int const& neutral_multiplicity() const {
    return neutral_multiplicity_;
  }

  /// Charged electromagnetic energy
  inline float const& charged_em_energy() const { return charged_em_energy_; }

  /// Calculates the charged electromagnetic fraction of the raw jet energy
  inline float charged_em_energy_frac() const {
    return (charged_em_energy_ / uncorrected_energy());
  }

  /// Neutral electromagnetic energy
  inline float const& neutral_em_energy() const { return neutral_em_energy_; }

  /// Calculates the neutral electromagnetic fraction of the raw jet energy
  inline float neutral_em_energy_frac() const {
    return (neutral_em_energy_ / uncorrected_energy());
  }

  /// Charged hadron energy
  inline float const& charged_had_energy() const { return charged_had_energy_; }

  /// Calculates the charged hadron fraction of the raw jet energy
  inline float charged_had_energy_frac() const {
    return (charged_had_energy_ / uncorrected_energy());
  }

  /// Neutral hadron energy
  inline float const& neutral_had_energy() const { return neutral_had_energy_; }

  /// Calculates the neutral hadron fraction of the raw jet energy
  inline float neutral_had_energy_frac() const {
    return (neutral_had_energy_ / uncorrected_energy());
  }

  /// Photon energy
  inline float const& photon_energy() const { return photon_energy_; }

  /// Calculates the photon fraction of the raw jet energy
  inline float photon_energy_frac() const {
    return (photon_energy_ / uncorrected_energy());
  }

  /// Electron energy
  inline float const& electron_energy() const { return electron_energy_; }

  /// Calculates the electron fraction of the raw jet energy
  inline float electron_energy_frac() const {
    return (electron_energy_ / uncorrected_energy());
  }

  /// Muon energy
  inline float const& muon_energy() const { return muon_energy_; }

  /// Calculates the muon fraction of the raw jet energy
  inline float muon_energy_frac() const {
    return (muon_energy_ / uncorrected_energy());
  }

  /// HF hadronic energy
  inline float const& HF_had_energy() const { return HF_had_energy_; }

  /// Calculates the HF hadronic fraction of the raw jet energy
  inline float HF_had_energy_frac() const {
    return (HF_had_energy_ / uncorrected_energy());
  }

  /// HF electromagnetic energy
  inline float const& HF_em_energy() const { return HF_em_energy_; }

  /// Calculates the HF electromagnetic fraction of the raw jet energy
  inline float HF_em_energy_frac() const {
    return (HF_em_energy_ / uncorrected_energy());
  }

  /// Muon energy
  inline float const& charged_mu_energy() const { return charged_mu_energy_; }

  /// Calculates the muon fraction of the raw jet energy
  inline float charged_mu_energy_frac() const {
    return (charged_mu_energy_ / uncorrected_energy());
  }

  /**
   * @brief The fraction of the constituent track momentum associated to the
   * primary vertex
   * @details Defined as:
   * \f[
   * \beta =
   * \frac{\sum^{\mathrm{tracks}}_{\mathrm{PV}}p_{\mathrm{T}}^{\mathrm{trk}}}
   * {\sum^{\mathrm{tracks}}p_{\mathrm{T}}^{\mathrm{trk}}},
   * \f]
   * where the sums are over the tracks associated to the jet, and PV
   * implies the subset of these tracks that were used in the leading primary
   * vertex fit. In the case that a track was not used in any vertex fit in the
   * event, it is considered associated to the primary vertex if the
   * longitudinal distance between the track and vertex \f$\Delta Z <
   * 0.2\,\mathrm{cm}\f$.
   */
  inline float const& beta() const { return beta_; }

  /// The largest value of beta() obtained when it is evaluated for every vertex
  /// in the event
  inline float const& beta_max() const { return beta_max_; }

  /// The output of the pileup ID MVA
  inline float const& pu_id_mva_value() const { return pu_id_mva_value_; }


  /// Whether the loose working point of the MVA ID is passed
  /**
   * @deprecated The working point has changed with each new training of the
   * MVA, and should instead be implemented in a free function
   */
  inline bool pu_id_mva_loose() const {
    // Pt2030_Loose   = cms.vdouble(-0.80,-0.85,-0.84,-0.85),
    // Pt3050_Loose   = cms.vdouble(-0.80,-0.74,-0.68,-0.77)
    // #4 Eta Categories  0-2.5 2.5-2.75 2.75-3.0 3.0-5.0
    double abs_eta = fabs(this->eta());
    double pt = this->pt();
    if (pt > 20. && pt <= 30) {
      if (abs_eta < 2.5) {
        return (pu_id_mva_value_ > -0.80);
      } else if (abs_eta < 2.75) {
        return (pu_id_mva_value_ > -0.85);
      } else if (abs_eta < 3.0) {
        return (pu_id_mva_value_ > -0.84);
      } else if (abs_eta < 5.0) {
        return (pu_id_mva_value_ > -0.85);
      } else {
        return true;
      }
    } else if (pt > 30.) {
      if (abs_eta < 2.5) {
        return (pu_id_mva_value_ > -0.80);
      } else if (abs_eta < 2.75) {
        return (pu_id_mva_value_ > -0.74);
      } else if (abs_eta < 3.0) {
        return (pu_id_mva_value_ > -0.68);
      } else if (abs_eta < 5.0) {
        return (pu_id_mva_value_ > -0.77);
      } else {
        return true;
      }
    } else {
      return true;
    }
  }

  /// A vector to refer to the constituent track ic::Track::id() values
  inline std::vector<std::size_t> const& constituent_tracks() const {
    return constituent_tracks_;
  }

  /// The number of charged constituents associated to the PV
  inline int const& charged_multiplicity_nopu() const {
    return charged_multiplicity_nopu_;
  }

  /// Used for quark-gluon discrimination
  /**
   * @details Defined as:
   * \f[
   * \mathrm{LRM} =
   * \frac{\sum^{\mathrm{tracks}}_{\mathrm{PV}}\left[p_{\mathrm{T}}^{\mathrm{trk}}\times\Delta
   * R(\mathrm{jet},\mathrm{track})\right]}{p_{\mathrm{T}}^{\mathrm{jet}}}.
   * \f]
   */
  inline float const& linear_radial_moment() const {
    return linear_radial_moment_;
  }
  /**@}*/

  /// @name Setters
  /**@{*/


  /// @copybrief muon_multiplicity()
  inline void set_muon_multiplicity(int const& muon_multiplicity) {
    muon_multiplicity_ = muon_multiplicity;
  }

  /// @copybrief charged_multiplicity()
  inline void set_charged_multiplicity(int const& charged_multiplicity) {
    charged_multiplicity_ = charged_multiplicity;
  }

  /// @copybrief charged_had_multiplicity()
  inline void set_charged_had_multiplicity(
      int const& charged_had_multiplicity) {
    charged_had_multiplicity_ = charged_had_multiplicity;
  }

  /// @copybrief neutral_had_multiplicity()
  inline void set_neutral_had_multiplicity(
      int const& neutral_had_multiplicity) {
    neutral_had_multiplicity_ = neutral_had_multiplicity;
  }

  /// @copybrief photon_multiplicity()
  inline void set_photon_multiplicity(int const& photon_multiplicity) {
    photon_multiplicity_ = photon_multiplicity;
  }

  /// @copybrief electron_multiplicity()
  inline void set_electron_multiplicity(int const& electron_multiplicity) {
    electron_multiplicity_ = electron_multiplicity;
  }

  /// @copybrief HF_had_multiplicity()
  inline void set_HF_had_multiplicity(int const& HF_had_multiplicity) {
    HF_had_multiplicity_ = HF_had_multiplicity;
  }

  /// @copybrief HF_em_multiplicity()
  inline void set_HF_em_multiplicity(int const& HF_em_multiplicity) {
    HF_em_multiplicity_ = HF_em_multiplicity;
  }

  /// @copybrief neutral_multiplicity()
  inline void set_neutral_multiplicity(int const& neutral_multiplicity) {
    neutral_multiplicity_ = neutral_multiplicity;
  }

  /// @copybrief charged_em_energy()
  inline void set_charged_em_energy(float const& charged_em_energy) {
    charged_em_energy_ = charged_em_energy;
  }

  /// @copybrief neutral_em_energy()
  inline void set_neutral_em_energy(float const& neutral_em_energy) {
    neutral_em_energy_ = neutral_em_energy;
  }

  /// @copybrief charged_had_energy()
  inline void set_charged_had_energy(float const& charged_had_energy) {
    charged_had_energy_ = charged_had_energy;
  }

  /// @copybrief neutral_had_energy()
  inline void set_neutral_had_energy(float const& neutral_had_energy) {
    neutral_had_energy_ = neutral_had_energy;
  }

  /// @copybrief photon_energy()
  inline void set_photon_energy(float const& photon_energy) {
    photon_energy_ = photon_energy;
  }

  /// @copybrief electron_energy()
  inline void set_electron_energy(float const& electron_energy) {
    electron_energy_ = electron_energy;
  }

  /// @copybrief muon_energy()
  inline void set_muon_energy(float const& muon_energy) {
    muon_energy_ = muon_energy;
  }

  /// @copybrief HF_had_energy()
  inline void set_HF_had_energy(float const& HF_had_energy) {
    HF_had_energy_ = HF_had_energy;
  }


  /// @copybrief HF_em_energy()
  inline void set_HF_em_energy(float const& HF_em_energy) {
    HF_em_energy_ = HF_em_energy;
  }

  /// @copybrief charged_mu_energy()
  inline void set_charged_mu_energy(float const& charged_mu_energy) {
    charged_mu_energy_ = charged_mu_energy;
  }

  /// @copybrief beta()
  inline void set_beta(float const& beta) { beta_ = beta; }

  /// @copybrief beta_max()
  inline void set_beta_max(float const& beta_max) { beta_max_ = beta_max; }

  /// @copybrief pu_id_mva_value()
  inline void set_pu_id_mva_value(float const& pu_id_mva_value) {
    pu_id_mva_value_ = pu_id_mva_value;
  }

  /// @copybrief constituent_tracks()
  inline void set_constituent_tracks(
      std::vector<std::size_t> const& constituent_tracks) {
    constituent_tracks_ = constituent_tracks;
  }

  /// @copybrief charged_multiplicity_nopu()
  inline void set_charged_multiplicity_nopu(
      int const& charged_multiplicity_nopu) {
    charged_multiplicity_nopu_ = charged_multiplicity_nopu;
  }

  /// @copybrief linear_radial_moment()
  inline void set_linear_radial_moment(float const& linear_radial_moment) {
    linear_radial_moment_ = linear_radial_moment;
  }
  /**@}*/

 private:
  int muon_multiplicity_;
  int charged_multiplicity_;
  int charged_had_multiplicity_;
  int neutral_had_multiplicity_;
  int photon_multiplicity_;
  int electron_multiplicity_;
  int HF_had_multiplicity_;
  int HF_em_multiplicity_;
  int neutral_multiplicity_;

  float charged_em_energy_;
  float neutral_em_energy_;
  float charged_had_energy_;
  float neutral_had_energy_;
  float photon_energy_;
  float electron_energy_;
  float muon_energy_;
  float HF_had_energy_;
  float HF_em_energy_;
  float charged_mu_energy_;

  float beta_;
  float beta_max_;

  float pu_id_mva_value_;

  std::vector<std::size_t> constituent_tracks_;

  float linear_radial_moment_;
  int charged_multiplicity_nopu_;

 #ifndef SKIP_CINT_DICT
 public:
  ClassDef(PFJet, 2);
 #endif
};

typedef std::vector<ic::PFJet> PFJetCollection;
}
#endif
