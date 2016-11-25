#ifndef ICHiggsTauTau_Tau_hh
#define ICHiggsTauTau_Tau_hh
#include <map>
#include <vector>
#include <string>
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "Rtypes.h"

namespace ic {

/**
 * @brief This class stores a subset of the reco::PFTau
 * properties which are most commonly used in analysis.
 */
class Tau : public Candidate {
 private:
  typedef std::map<std::size_t, float> UFmap;
  typedef ROOT::Math::XYZPoint Point;

 public:
  Tau();
  virtual ~Tau();
  virtual void Print() const;


  /// @name Properties
  /**@{*/
  /// Get the map containing the hashed discriminator labels and corresponding
  /// values
  inline std::map<std::size_t, float> const& tau_ids() const {
    return tau_ids_;
  }

  /// The tau decay mode identifier
  inline int const& decay_mode() const { return decay_mode_; }

  /// ECAL energy of the leading charged PF constituent
  inline float const& lead_ecal_energy() const { return lead_ecal_energy_; }

  /// HCAL energy of the leading charged PF constituent
  inline float const& lead_hcal_energy() const { return lead_hcal_energy_; }

  /// Momentum of the leading charged PF constituent
  inline float const& lead_p() const { return lead_p_; }

  /// Transverse impact parameter of the leading charged constituent track with
  /// the primary vertex
  inline float const& lead_dxy_vertex() const { return lead_dxy_vertex_; }

  /// Longitudinal impact parameter of the leading charged constituent track
  /// with the primary vertex
  inline float const& lead_dz_vertex() const { return lead_dz_vertex_; }

  /// The x-coordinate of the leading track PCA
  inline double vx() const { return ref_point_.x(); }

  /// The y-coordinate of the leading track PCA
  inline double vy() const { return ref_point_.y(); }

  /// The z-coordinate of the leading track PCA
  inline double vz() const { return ref_point_.z(); }

  /// A vector referring to the constituent track ic::Track::id()
  inline std::vector<std::size_t> const& constituent_tracks() const {
    return constituent_tracks_;
  }

  /// A vector referring to the signal PFCandidates
  inline std::vector<std::size_t> const& sig_charged_cands() const {
    return sig_charged_cands_;
  }

  /// A vector referring to the isolation PFCandidates
  inline std::vector<std::size_t> const& iso_charged_cands() const {
    return iso_charged_cands_;
  }

  /// A vector referring to the signal PFCandidates
  inline std::vector<std::size_t> const& sig_gamma_cands() const {
    return sig_gamma_cands_;
  }

  /// A vector referring to the isolation PFCandidates
  inline std::vector<std::size_t> const& iso_gamma_cands() const {
    return iso_gamma_cands_;
  }
  /**@}*/

  /// @name Setters
  /**@{*/
  /// @copybrief tau_ids()
  inline void set_tau_ids(std::map<std::size_t, float> const& tau_ids) {
    tau_ids_ = tau_ids;
  }

  /// @copybrief decay_mode()
  inline void set_decay_mode(int const& decay_mode) {
    decay_mode_ = decay_mode;
  }

  /// @copybrief lead_ecal_energy()
  inline void set_lead_ecal_energy(float const& lead_ecal_energy) {
    lead_ecal_energy_ = lead_ecal_energy;
  }

  /// @copybrief lead_hcal_energy()
  inline void set_lead_hcal_energy(float const& lead_hcal_energy) {
    lead_hcal_energy_ = lead_hcal_energy;
  }

  /// @copybrief lead_p()
  inline void set_lead_p(float const& lead_p) { lead_p_ = lead_p; }

  /// @copybrief lead_dxy_vertex()
  inline void set_lead_dxy_vertex(float const& lead_dxy_vertex) {
    lead_dxy_vertex_ = lead_dxy_vertex;
  }

  /// @copybrief lead_dz_vertex()
  inline void set_lead_dz_vertex(float const& lead_dz_vertex) {
    lead_dz_vertex_ = lead_dz_vertex;
  }

  /// @copybrief vx()
  inline void set_vx(double const& x) { ref_point_.SetX(x); }

  /// @copybrief vy()
  inline void set_vy(double const& y) { ref_point_.SetY(y); }

  /// @copybrief vz()
  inline void set_vz(double const& z) { ref_point_.SetZ(z); }

  /// @copybrief constituent_tracks()
  inline void set_constituent_tracks(
      std::vector<std::size_t> const& constituent_tracks) {
    constituent_tracks_ = constituent_tracks;
  }

  /// @copybrief iso_charged_cands()
  inline void set_sig_charged_cands(
      std::vector<std::size_t> const& sig_charged_cands) {
    sig_charged_cands_ = sig_charged_cands;
  }

  /// @copybrief iso_charged_cands()
  inline void set_iso_charged_cands(
      std::vector<std::size_t> const& iso_charged_cands) {
    iso_charged_cands_ = iso_charged_cands;
  }

  /// @copybrief iso_gamma_cands()
  inline void set_sig_gamma_cands(
      std::vector<std::size_t> const& sig_gamma_cands) {
    sig_gamma_cands_ = sig_gamma_cands;
  }

  /// @copybrief iso_gamma_cands()
  inline void set_iso_gamma_cands(
      std::vector<std::size_t> const& iso_gamma_cands) {
    iso_gamma_cands_ = iso_gamma_cands;
  }
  /**@}*/

  /**
   * @name Tau discriminators
   * @details The Tau class contains a map for storing arbitrary pairs
   * of hashed strings and floats, most commonly used for storing the output of
   * the main tau discriminators. */
  /**@{*/
  /**
   * @brief Add a new entry, overwriting any existing one with the same \a name
   * @param name A label to identify the value, will be stored as a hash
   * @param value The value to associate to the label \a name
   */
  void SetTauID(std::string const& name, float const& value);
  /**
   * @brief Check if a value with label \a name has already been defined
   * @param name The label to check
   * @return True if the label exists in the map, false otherwise
   */
  bool HasTauID(std::string const& name) const;
  /**
   * @brief Get the value associated to a label
   * @param name The label to retrieve
   * @return The value associated to the label if found, otherwise zero.
   */
  float GetTauID(std::string const& name) const;
  /**@}*/

 private:
  UFmap tau_ids_;

  int decay_mode_;

  float lead_ecal_energy_;
  float lead_hcal_energy_;
  float lead_p_;
  float lead_dxy_vertex_;
  float lead_dz_vertex_;

  Point ref_point_;

  std::vector<std::size_t> constituent_tracks_;
  std::vector<std::size_t> sig_charged_cands_;
  std::vector<std::size_t> iso_charged_cands_;
  std::vector<std::size_t> sig_gamma_cands_;
  std::vector<std::size_t> iso_gamma_cands_;

 #ifndef SKIP_CINT_DICT
 public:
  ClassDef(Tau, 3);
 #endif
};

typedef std::vector<ic::Tau> TauCollection;
}
/** \example plugins/ICTauProducer.hh */
#endif
