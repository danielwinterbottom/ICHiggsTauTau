#ifndef ICHiggsTauTau_JPTJet_hh
#define ICHiggsTauTau_JPTJet_hh
#include <map>
#include <string>
#include <vector>
#include "UserCode/ICHiggsTauTau/interface/CaloJet.hh"
#include "Rtypes.h"

namespace ic {

/**
 * @brief A class derived from ic::Jet which adds Jet-plus-track specific
 * information
 */
class JPTJet : public CaloJet {
 public:
  JPTJet();
  virtual ~JPTJet();
  virtual void Print() const;

  /// @name Properties
  /**@{*/
  /// Number of muon constituents
  inline int const& muon_multiplicity() const { return muon_multiplicity_; }

  /// Total number of charged constituents
  inline int const& charged_multiplicity() const {
    return charged_multiplicity_;
  }

  /// Charged EM energy
  inline float const& charged_em_energy() const { return charged_em_energy_; }

  /// Charged EM fraction of the total energy
  inline float charged_em_energy_frac() const {
    return (charged_em_energy_ / uncorrected_energy());
  }

  /// Neutral EM energy
  inline float const& neutral_em_energy() const { return neutral_em_energy_; }

  /// Neutral EM fraction of the total energy
  inline float neutral_em_energy_frac() const {
    return (neutral_em_energy_ / uncorrected_energy());
  }

  /// Charged hadron energy
  inline float const& charged_had_energy() const { return charged_had_energy_; }

  /// Charged hadron fraction of the total energy
  inline float charged_had_energy_frac() const {
    return (charged_had_energy_ / uncorrected_energy());
  }

  /// Neutral hadron energy
  inline float const& neutral_had_energy() const { return neutral_had_energy_; }

  /// Neutral hadron fraction of the total energy
  inline float neutral_had_energy_frac() const {
    return (neutral_had_energy_ / uncorrected_energy());
  }

  /// A vector of ic::Track::id() values for pions compatible with the jet both
  /// at the vertex and the calorimeter
  inline std::vector<std::size_t> const& pions_in_vtx_in_calo() const {
    return pions_in_vtx_in_calo_;
  }

  /// A vector of ic::Track::id() values for pions compatible with the jet
  /// at the vertex but not at the calorimeter
  inline std::vector<std::size_t> const& pions_in_vtx_out_calo() const {
    return pions_in_vtx_out_calo_;
  }

  /// A vector of ic::Track::id() values for pions compatible with the jet
  /// at the calorimeter but not at the vertex
  inline std::vector<std::size_t> const& pions_out_vtx_in_calo() const {
    return pions_out_vtx_in_calo_;
  }

  /// A vector of ic::Track::id() values for muons compatible with the jet both
  /// at the vertex and the calorimeter
  inline std::vector<std::size_t> const& muons_in_vtx_in_calo() const {
    return muons_in_vtx_in_calo_;
  }

  /// A vector of ic::Track::id() values for muons compatible with the jet
  /// at the vertex but not at the calorimeter
  inline std::vector<std::size_t> const& muons_in_vtx_out_calo() const {
    return muons_in_vtx_out_calo_;
  }

  /// A vector of ic::Track::id() values for muons compatible with the jet
  /// at the calorimeter but not at the vertex
  inline std::vector<std::size_t> const& muons_out_vtx_in_calo() const {
    return muons_out_vtx_in_calo_;
  }

  /// A vector of ic::Track::id() values for electrons compatible with the jet
  /// both at the vertex and the calorimeter
  inline std::vector<std::size_t> const& elecs_in_vtx_in_calo() const {
    return elecs_in_vtx_in_calo_;
  }

  /// A vector of ic::Track::id() values for electrons compatible with the jet
  /// at the vertex but not at the calorimeter
  inline std::vector<std::size_t> const& elecs_in_vtx_out_calo() const {
    return elecs_in_vtx_out_calo_;
  }

  /// A vector of ic::Track::id() values for electrons compatible with the jet
  /// at the calorimeter but not at the vertex
  inline std::vector<std::size_t> const& elecs_out_vtx_in_calo() const {
    return elecs_out_vtx_in_calo_;
  }

  /// A vector of ic::Track::id() that refers to the complete set of associated
  /// tracks
  std::vector<std::size_t> constituent_tracks() const;

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

  /// The scalar sum of track \f$p_{\mathrm{T}}\f$
  inline float const& track_pt_total() const { return track_pt_total_; }
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

  /// @copybrief pions_in_vtx_in_calo()
  inline void set_pions_in_vtx_in_calo(
      std::vector<std::size_t> const& pions_in_vtx_in_calo) {
    pions_in_vtx_in_calo_ = pions_in_vtx_in_calo;
  }

  /// @copybrief pions_in_vtx_out_calo()
  inline void set_pions_in_vtx_out_calo(
      std::vector<std::size_t> const& pions_in_vtx_out_calo) {
    pions_in_vtx_out_calo_ = pions_in_vtx_out_calo;
  }

  /// @copybrief pions_out_vtx_in_calo()
  inline void set_pions_out_vtx_in_calo(
      std::vector<std::size_t> const& pions_out_vtx_in_calo) {
    pions_out_vtx_in_calo_ = pions_out_vtx_in_calo;
  }

  /// @copybrief muons_in_vtx_in_calo()
  inline void set_muons_in_vtx_in_calo(
      std::vector<std::size_t> const& muons_in_vtx_in_calo) {
    muons_in_vtx_in_calo_ = muons_in_vtx_in_calo;
  }

  /// @copybrief muons_in_vtx_out_calo()
  inline void set_muons_in_vtx_out_calo(
      std::vector<std::size_t> const& muons_in_vtx_out_calo) {
    muons_in_vtx_out_calo_ = muons_in_vtx_out_calo;
  }

  /// @copybrief muons_out_vtx_in_calo()
  inline void set_muons_out_vtx_in_calo(
      std::vector<std::size_t> const& muons_out_vtx_in_calo) {
    muons_out_vtx_in_calo_ = muons_out_vtx_in_calo;
  }

  /// @copybrief elecs_in_vtx_in_calo()
  inline void set_elecs_in_vtx_in_calo(
      std::vector<std::size_t> const& elecs_in_vtx_in_calo) {
    elecs_in_vtx_in_calo_ = elecs_in_vtx_in_calo;
  }

  /// @copybrief elecs_in_vtx_out_calo()
  inline void set_elecs_in_vtx_out_calo(
      std::vector<std::size_t> const& elecs_in_vtx_out_calo) {
    elecs_in_vtx_out_calo_ = elecs_in_vtx_out_calo;
  }

  /// @copybrief elecs_out_vtx_in_calo()
  inline void set_elecs_out_vtx_in_calo(
      std::vector<std::size_t> const& elecs_out_vtx_in_calo) {
    elecs_out_vtx_in_calo_ = elecs_out_vtx_in_calo;
  }

  /// @copybrief beta()
  inline void set_beta(float const& beta) { beta_ = beta; }

  /// @copybrief beta_max()
  inline void set_beta_max(float const& beta_max) { beta_max_ = beta_max; }

  /// @copybrief track_pt_total()
  inline void set_track_pt_total(float const& track_pt_total) {
    track_pt_total_ = track_pt_total;
  }
  /**@}*/

 private:
  int muon_multiplicity_;
  int charged_multiplicity_;
  float charged_em_energy_;
  float neutral_em_energy_;
  float charged_had_energy_;
  float neutral_had_energy_;

  float beta_;
  float beta_max_;
  float track_pt_total_;

  std::vector<std::size_t> pions_in_vtx_in_calo_;
  std::vector<std::size_t> pions_in_vtx_out_calo_;
  std::vector<std::size_t> pions_out_vtx_in_calo_;
  std::vector<std::size_t> muons_in_vtx_in_calo_;
  std::vector<std::size_t> muons_in_vtx_out_calo_;
  std::vector<std::size_t> muons_out_vtx_in_calo_;
  std::vector<std::size_t> elecs_in_vtx_in_calo_;
  std::vector<std::size_t> elecs_in_vtx_out_calo_;
  std::vector<std::size_t> elecs_out_vtx_in_calo_;

 public:
  ClassDef(JPTJet, 2);
};

typedef std::vector<ic::JPTJet> JPTJetCollection;
}
#endif
