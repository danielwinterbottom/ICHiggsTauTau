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
  typedef double Matrix[3][3];
  typedef double Vec[3];

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

  inline float const& lead_dxy_vertex_error() const { return lead_dxy_vertex_error_; }

  inline float const& lead_dz_vertex_error() const { return lead_dz_vertex_error_; }

  /// Transverse momentum of the leading charged PF constituent
  inline float const& lead_pt() const { return lead_pt_; }

  /// Eta of the leading charged PF constituent
  inline float const& lead_eta() const { return lead_eta_; }

  /// Phi of the leading charged PF constituent
  inline float const& lead_phi() const { return lead_phi_; }

  /// Energy of the leading charged PF constituent
  inline float const& lead_energy() const { return lead_energy_; }

  /// The x-coordinate of the leading track PCA
  inline double vx() const { return ref_point_.x(); }

  /// The y-coordinate of the leading track PCA
  inline double vy() const { return ref_point_.y(); }

  /// The z-coordinate of the leading track PCA
  inline double vz() const { return ref_point_.z(); }

  /// The x-coordinate of the leading track PCA
  inline double svx() const { return s_ref_point_.x(); }
  
  /// The y-coordinate of the leading track PCA
  inline double svy() const { return s_ref_point_.y(); }
  
  /// The z-coordinate of the leading track PCA
  inline double svz() const { return s_ref_point_.z(); }

  inline Point secondary_vertex() const { return secondary_vertex_; }

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

  /// A vector referring to the signal neutral PFCandidates
  inline std::vector<std::size_t> const& sig_neutral_cands() const {
    return sig_neutral_cands_;
  }

  /// The vtx covariance matrix
  inline Matrix* vtx_covariance() { return &(vtx_covariance_); }

  inline bool hasSV() const { return hasSV_; }

  inline Matrix* s_vtx_covariance() { return &(s_vtx_covariance_); }

  inline Vec* track_params() { return &(track_params_); }

  inline Matrix* track_params_covariance() { return &(track_params_covariance_); }

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

  /// @copybrief lead_dxy_vertex()
  inline void set_lead_dxy_vertex_error(float const& lead_dxy_vertex_error) {
    lead_dxy_vertex_error_ = lead_dxy_vertex_error;
  }

  /// @copybrief lead_dz_vertex()
  inline void set_lead_dz_vertex_error(float const& lead_dz_vertex_error) {
    lead_dz_vertex_error_ = lead_dz_vertex_error;
  }
  /// @copybrief lead_pt()
  inline void set_lead_pt(float const& lead_pt) { lead_pt_ = lead_pt; }

  /// @copybrief lead_energy()
  inline void set_lead_energy(float const& lead_energy) { lead_energy_ = lead_energy; }

  /// @copybrief lead_eta()
  inline void set_lead_eta(float const& lead_eta) { lead_eta_ = lead_eta; }

  /// @copybrief lead_phi()
  inline void set_lead_phi(float const& lead_phi) { lead_phi_ = lead_phi; }

  /// @copybrief vx()
  inline void set_vx(double const& x) { ref_point_.SetX(x); }

  /// @copybrief vy()
  inline void set_vy(double const& y) { ref_point_.SetY(y); }

  /// @copybrief vz()
  inline void set_vz(double const& z) { ref_point_.SetZ(z); }

  /// @copybrief svx()
  inline void set_svx(double const& x) { s_ref_point_.SetX(x); }

   /// @copybrief svy()
  inline void set_svy(double const& y) { s_ref_point_.SetY(y); }
  
  /// @copybrief svz()
  inline void set_svz(double const& z) { s_ref_point_.SetZ(z); }

  inline void set_secondary_vertex(double const& x, double const& y, double const& z) { 
    secondary_vertex_.SetX(x);
    secondary_vertex_.SetY(y);
    secondary_vertex_.SetZ(z); 
  }

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

  inline void set_sig_neutral_cands(
      std::vector<std::size_t> const& sig_neutral_cands) {
    sig_neutral_cands_ = sig_neutral_cands;
  }

  inline void set_vtx_covariance(double cov00, double cov01, double cov02, double cov10, double cov11, double cov12, double cov20, double cov21, double cov22) {
    //covij where i=row and j=column
    vtx_covariance_[0][0] = cov00;
    vtx_covariance_[0][1] = cov01;
    vtx_covariance_[0][2] = cov02;
    vtx_covariance_[1][0] = cov10;
    vtx_covariance_[1][1] = cov11;
    vtx_covariance_[1][2] = cov12;
    vtx_covariance_[2][0] = cov20;
    vtx_covariance_[2][1] = cov21;
    vtx_covariance_[2][2] = cov22;
  }

  inline void set_hasSV(bool const&  hasSV){
    hasSV_ = hasSV;
  }


  inline void set_s_vtx_covariance(double cov00, double cov01, double cov02, double cov10, double cov11, double cov12, double cov20, double cov21, double cov22) {
    //covij where i=row and j=column
    s_vtx_covariance_[0][0] = cov00;
    s_vtx_covariance_[0][1] = cov01;
    s_vtx_covariance_[0][2] = cov02;
    s_vtx_covariance_[1][0] = cov10;
    s_vtx_covariance_[1][1] = cov11;
    s_vtx_covariance_[1][2] = cov12;
    s_vtx_covariance_[2][0] = cov20;
    s_vtx_covariance_[2][1] = cov21;
    s_vtx_covariance_[2][2] = cov22;
  }

  inline void set_track_params(double p1, double p2, double p3) {
    track_params_[0] = p1;
    track_params_[1] = p2;
    track_params_[2] = p3;  
  }

  inline void set_track_params_covariance(double cov00, double cov01, double cov02, double cov10, double cov11, double cov12, double cov20, double cov21, double cov22) {
    //covij where i=row and j=column
    track_params_covariance_[0][0] = cov00;
    track_params_covariance_[0][1] = cov01;
    track_params_covariance_[0][2] = cov02;
    track_params_covariance_[1][0] = cov10;
    track_params_covariance_[1][1] = cov11;
    track_params_covariance_[1][2] = cov12;
    track_params_covariance_[2][0] = cov20;
    track_params_covariance_[2][1] = cov21;
    track_params_covariance_[2][2] = cov22;
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
  float lead_dxy_vertex_error_;
  float lead_dz_vertex_error_;

  float lead_pt_;
  float lead_eta_;
  float lead_phi_;
  float lead_energy_;

  Point ref_point_;
  Point s_ref_point_;
  Point secondary_vertex_;

  Matrix vtx_covariance_;

  bool hasSV_;

  Matrix s_vtx_covariance_;

  Vec track_params_;
  Matrix track_params_covariance_;

  std::vector<std::size_t> constituent_tracks_;
  std::vector<std::size_t> sig_charged_cands_;
  std::vector<std::size_t> iso_charged_cands_;
  std::vector<std::size_t> sig_gamma_cands_;
  std::vector<std::size_t> iso_gamma_cands_;
  std::vector<std::size_t> sig_neutral_cands_;

 #ifndef SKIP_CINT_DICT
 public:
  ClassDef(Tau, 5);
 #endif
};

typedef std::vector<ic::Tau> TauCollection;
}
/** \example plugins/ICTauProducer.hh */
#endif
