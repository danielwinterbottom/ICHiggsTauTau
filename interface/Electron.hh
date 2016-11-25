#ifndef ICHiggsTauTau_Electron_hh
#define ICHiggsTauTau_Electron_hh
#include <map>
#include <string>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "Rtypes.h"

namespace ic {

/**
 * @brief This class stores a subset of the reco::GsfElectron
 * properties which are most commonly used in analysis.
 *
 * Useful links:
 *  - https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideGsfElectronObject
 *  - https://twiki.cern.ch/twiki/bin/view/CMSbackupJan10/HEEPElectronID
 *  - https://twiki.cern.ch/twiki/bin/viewauth/CMS/ConversionBackgroundRejection
 *  - https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideEgammaShowerShape
 *  - https://twiki.cern.ch/twiki/bin/view/CMS/VbtfEleID2011
 *  - https://twiki.cern.ch/twiki/bin/viewauth/CMS/SimpleCutBasedEleID2011
 */
class Electron : public Candidate {
 private:
  typedef ROOT::Math::XYZPoint Point;
  typedef std::map<std::size_t, float> UFmap;

 public:
  Electron();
  virtual ~Electron();
  virtual void Print() const;

  /// @name Properties
  /**@{*/
  /// A vector to refer to ic::GenParticle::id() values
  /**
   * @deprecated This property is no longer filled in the ICElectronProducer. It
   * is advised to save generator-level electrons that may be needed for
   * matching using the ICGenParticleProducer and to identify matches in the
   * analysis code.
   */
  inline std::vector<std::size_t> const& gen_particles() const {
    return gen_particles_;
  }

  /// Point-of-closest-approach (PCA) of the electron track to the beamspot
  inline Point const& ref_point() const { return ref_point_; }

  /// Tracker isolation in a cone with \f$ \Delta R = 0.3 \f$
  inline float dr03_tk_sum_pt() const { return dr03_tk_sum_pt_; }

  /// ECAL isolation in a cone with \f$ \Delta R = 0.3 \f$
  inline float dr03_ecal_rechit_sum_et() const {
    return dr03_ecal_rechit_sum_et_;
  }

  /// HCAL isolation in a cone with \f$ \Delta R = 0.3 \f$
  inline float dr03_hcal_tower_sum_et() const {
    return dr03_hcal_tower_sum_et_;
  }

  ///ECAL PF cluster isolation
  inline float ecal_pf_cluster_iso() const {
    return ecal_pf_cluster_iso_;
  }

  ///HCAL PF cluster isolation
  inline float hcal_pf_cluster_iso() const {
    return hcal_pf_cluster_iso_;
  }

  /// PF isolation, using all charged particles in a cone with
  /// \f$ \Delta R = 0.3 \f$
  inline float dr03_pfiso_charged_all() const {
    return dr03_pfiso_charged_all_;
  }

  /// PF isolation, using charged hadrons in a cone with \f$ \Delta R = 0.3 \f$
  inline float dr03_pfiso_charged() const { return dr03_pfiso_charged_; }

  /// PF isolation, using neutral hadrons in a cone with \f$ \Delta R = 0.3 \f$
  inline float dr03_pfiso_neutral() const { return dr03_pfiso_neutral_; }

  /// PF isolation, using photons in a cone with \f$ \Delta R = 0.3 \f$
  inline float dr03_pfiso_gamma() const { return dr03_pfiso_gamma_; }

  /// PF isolation, using charged pileup in a cone with \f$ \Delta R = 0.3 \f$
  inline float dr03_pfiso_pu() const { return dr03_pfiso_pu_; }

  /// PF isolation, using all charged particles in a cone with
  /// \f$ \Delta R = 0.4 \f$
  inline float dr04_pfiso_charged_all() const {
    return dr04_pfiso_charged_all_;
  }

  /// PF isolation, using charged hadrons in a cone with \f$ \Delta R = 0.4 \f$
  inline float dr04_pfiso_charged() const { return dr04_pfiso_charged_; }

  /// PF isolation, using neutral hadrons in a cone with \f$ \Delta R = 0.4 \f$
  inline float dr04_pfiso_neutral() const { return dr04_pfiso_neutral_; }

  /// PF isolation, using photons in a cone with \f$ \Delta R = 0.4 \f$
  inline float dr04_pfiso_gamma() const { return dr04_pfiso_gamma_; }

  /// PF isolation, using charged pileup in a cone with \f$ \Delta R = 0.4 \f$
  inline float dr04_pfiso_pu() const { return dr04_pfiso_pu_; }

  /// Ratio of HCAL to supercluster energy in the region of the electron hit
  inline float hadronic_over_em() const { return hadronic_over_em_; }

  /// Weighted cluster RMS in the \f$\eta\f$-direction, measured in units
  /// of crystals in a 5x5 block centred on the seed crystal
  inline float full5x5_sigma_IetaIeta() const { return full5x5_sigma_IetaIeta_; }

  /// Weighted cluster RMS in the \f$\eta\f$-direction, measured in units
  /// of crystals in a 5x5 block centred on the seed crystal [this method 
  /// needs updated description as it does differ from full5x5_sigma_IetaIeta] 
  inline float sigma_IetaIeta() const { return sigma_IetaIeta_; }

  /// \f$\Delta\phi\f$ between the supercluster and track, where the latter
  /// is evaluated from the vertex position
  inline float dphi_sc_tk_at_vtx() const { return dphi_sc_tk_at_vtx_; }

  /// \f$\Delta\eta\f$ between the supercluster and track, where the latter
  /// is evaluated from the vertex position
  inline float deta_sc_tk_at_vtx() const { return deta_sc_tk_at_vtx_; }

  /// Number of tracker layers between the vertex and the first detected
  /// hit the electron should have traversed
  inline int gsf_tk_nhits() const { return gsf_tk_nhits_; }

  /// Minimum distance between conversion tracks (if found)
  inline float conv_dist() const { return conv_dist_; }

  /// \f$\Delta\cot\theta\f$ between conversion tracks at the conversion
  /// vertex
  inline float conv_dcot() const { return conv_dcot_; }

  /// Fraction of momentum lost as bremsstrahlung, as measured in the GSF fit
  inline float f_brem() const { return f_brem_; }

  /// Supercluster \f$\eta\f$
  inline float sc_eta() const { return sc_eta_; }

  /// Supercluster \f$\theta\f$
  inline float sc_theta() const { return sc_theta_; }

  /// Supercluster energy
  inline float sc_energy() const { return sc_energy_; }

  /// Ratio of supercluster energy to track momentum, where the latter is
  /// evaluated at the PCA to the beamspot
  inline float sc_e_over_p() const { return sc_e_over_p_; }

  /// The ratio of the energy in the 3x3 crystal array centred on the seed
  /// over the energy of the complete supercluster
  inline float r9() const { return r9_; }

  /// Calorimeter tower isolation sum
  inline float hcal_sum() const { return hcal_sum_; }

  /// The corrected supercluster energy
  inline float ecal_energy() const { return ecal_energy_; }

  /// True if electron is matched to a conversion vertex
  inline bool has_matched_conversion() const { return has_matched_conversion_; }

  /// The x-coordinate of the ref_point()
  inline double vx() const { return ref_point_.x(); }

  /// The y-coordinate of the ref_point()
  inline double vy() const { return ref_point_.y(); }

  /// The z-coordinate of the ref_point()
  inline double vz() const { return ref_point_.z(); }

  /// Transverse impact parameter of the GSF track with the primary vertex
  inline double dxy_vertex() const { return dxy_vertex_; }

  /// Longitudinal impact parameter of the GSF track with the primary vertex
  inline double dz_vertex() const { return dz_vertex_; }

  /// Transverse impact parameter of the GSF track with the beamspot
  inline double dxy_beamspot() const { return dxy_beamspot_; }
  /**@}*/

  /// @name Setters
  /**@{*/
  /// @copybrief gen_particles()
  inline void set_gen_particles(std::vector<std::size_t> const& gen_particles) {
    gen_particles_ = gen_particles;
  }

  /// @copybrief ref_point()
  inline void set_ref_point(Point const& ref_point) { ref_point_ = ref_point; }

  /// @copybrief dr03_tk_sum_pt()
  inline void set_dr03_tk_sum_pt(float const& dr03_tk_sum_pt) {
    dr03_tk_sum_pt_ = dr03_tk_sum_pt;
  }

  /// @copybrief dr03_ecal_rechit_sum_et()
  inline void set_dr03_ecal_rechit_sum_et(
      float const& dr03_ecal_rechit_sum_et) {
    dr03_ecal_rechit_sum_et_ = dr03_ecal_rechit_sum_et;
  }

  /// @copybrief dr03_hcal_tower_sum_et()
  inline void set_dr03_hcal_tower_sum_et(float const& dr03_hcal_tower_sum_et) {
    dr03_hcal_tower_sum_et_ = dr03_hcal_tower_sum_et;
  }

  /// @copybrief ecal_pf_cluster_iso()
  inline void set_ecal_pf_cluster_iso(float const& ecal_pf_cluster_iso) {
    ecal_pf_cluster_iso_ = ecal_pf_cluster_iso;
  }

  /// @copybrief hcal_pf_cluster_iso()
  inline void set_hcal_pf_cluster_iso(float const& hcal_pf_cluster_iso){
    hcal_pf_cluster_iso_ = hcal_pf_cluster_iso;
  }

  /// @copybrief dr03_pfiso_charged_all()
  inline void set_dr03_pfiso_charged_all(float const& dr03_pfiso_charged_all) {
    dr03_pfiso_charged_all_ = dr03_pfiso_charged_all;
  }

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

  /// @copybrief dr03_pfiso_pu()
  inline void set_dr03_pfiso_pu(float const& dr03_pfiso_pu) {
    dr03_pfiso_pu_ = dr03_pfiso_pu;
  }

  /// @copybrief dr04_pfiso_charged_all()
  inline void set_dr04_pfiso_charged_all(float const& dr04_pfiso_charged_all) {
    dr04_pfiso_charged_all_ = dr04_pfiso_charged_all;
  }

  /// @copybrief dr04_pfiso_charged()
  inline void set_dr04_pfiso_charged(float const& dr04_pfiso_charged) {
    dr04_pfiso_charged_ = dr04_pfiso_charged;
  }

  /// @copybrief dr04_pfiso_neutral()
  inline void set_dr04_pfiso_neutral(float const& dr04_pfiso_neutral) {
    dr04_pfiso_neutral_ = dr04_pfiso_neutral;
  }

  /// @copybrief dr04_pfiso_gamma()
  inline void set_dr04_pfiso_gamma(float const& dr04_pfiso_gamma) {
    dr04_pfiso_gamma_ = dr04_pfiso_gamma;
  }

  /// @copybrief dr04_pfiso_pu()
  inline void set_dr04_pfiso_pu(float const& dr04_pfiso_pu) {
    dr04_pfiso_pu_ = dr04_pfiso_pu;
  }

  /// @copybrief hadronic_over_em()
  inline void set_hadronic_over_em(float const& hadronic_over_em) {
    hadronic_over_em_ = hadronic_over_em;
  }

  /// @copybrief full5x5_sigma_IetaIeta()
  inline void set_full5x5_sigma_IetaIeta(float const& full5x5_sigma_IetaIeta) {
    full5x5_sigma_IetaIeta_ = full5x5_sigma_IetaIeta;
  }


  /// @copybrief sigma_IetaIeta()
  inline void set_sigma_IetaIeta(float const& sigma_IetaIeta) {
    sigma_IetaIeta_ = sigma_IetaIeta;
  }

  /// @copybrief dphi_sc_tk_at_vtx()
  inline void set_dphi_sc_tk_at_vtx(float const& dphi_sc_tk_at_vtx) {
    dphi_sc_tk_at_vtx_ = dphi_sc_tk_at_vtx;
  }

  /// @copybrief deta_sc_tk_at_vtx()
  inline void set_deta_sc_tk_at_vtx(float const& deta_sc_tk_at_vtx) {
    deta_sc_tk_at_vtx_ = deta_sc_tk_at_vtx;
  }

  /// @copybrief gsf_tk_nhits()
  inline void set_gsf_tk_nhits(int const& gsf_tk_nhits) {
    gsf_tk_nhits_ = gsf_tk_nhits;
  }


  /// @copybrief conv_dist()
  inline void set_conv_dist(float const& conv_dist) { conv_dist_ = conv_dist; }

  /// @copybrief conv_dcot()
  inline void set_conv_dcot(float const& conv_dcot) { conv_dcot_ = conv_dcot; }

  /// @copybrief f_brem()
  inline void set_f_brem(float const& f_brem) { f_brem_ = f_brem; }

  /// @copybrief sc_eta()
  inline void set_sc_eta(float const& sc_eta) { sc_eta_ = sc_eta; }

  /// @copybrief sc_theta()
  inline void set_sc_theta(float const& sc_theta) { sc_theta_ = sc_theta; }

  /// @copybrief sc_energy()
  inline void set_sc_energy(float const& sc_energy) { sc_energy_ = sc_energy; }

  /// @copybrief sc_e_over_p()
  inline void set_sc_e_over_p(float const& sc_e_over_p) {
    sc_e_over_p_ = sc_e_over_p;
  }

  /// @copybrief r9()
  inline void set_r9(float const& r9) { r9_ = r9; }

  /// @copybrief hcal_sum()
  inline void set_hcal_sum(float const& hcal_sum) { hcal_sum_ = hcal_sum; }

  /// @copybrief ecal_energy()
  inline void set_ecal_energy(float const& ecal_energy) {
    ecal_energy_ = ecal_energy;
  }

  /// @copybrief has_matched_conversion()
  inline void set_has_matched_conversion(bool const& has_matched_conversion) {
    has_matched_conversion_ = has_matched_conversion;
  }

  /// @copybrief vx()
  inline void set_vx(double const& x) { ref_point_.SetX(x); }

  /// @copybrief vy()
  inline void set_vy(double const& y) { ref_point_.SetY(y); }

  /// @copybrief vz()
  inline void set_vz(double const& z) { ref_point_.SetZ(z); }

  /// @copybrief dxy_vertex()
  inline void set_dxy_vertex(double const& dxy_vertex) {
    dxy_vertex_ = dxy_vertex;
  }

  /// @copybrief dz_vertex()
  inline void set_dz_vertex(double const& dz_vertex) { dz_vertex_ = dz_vertex; }

  /// @copybrief dxy_beamspot()
  inline void set_dxy_beamspot(double const& dxy_beamspot) {
    dxy_beamspot_ = dxy_beamspot;
  }
  /**@}*/

  /**
   * @name Extra ID and isolation properties
   * @details The Electron class contains a map for storing arbitrary pairs
   * of hashed strings and floats. The most common usage for this
   * is to add additional ID and isolation values, for example the
   * output of an MVA selection. */
  /**@{*/
  /**
   * @brief Add a new entry, overwriting any existing one with the same \a name
   * @param name A label to identify the value, will be stored as a hash
   * @param value The value to associate to the label \a name
   */
  void SetIdIso(std::string const& name, float const& value);
  /**
   * @brief Check if a value with label \a name has already been defined
   * @param name The label to check
   * @return True if the label exists in the map, false otherwise
   */
  bool HasIdIso(std::string const& name) const;
  /**
   * @brief Get the value associated to a label
   * @param name The label to retrieve
   * @return The value associated to the label if found, otherwise zero.
   */
  float GetIdIso(std::string const& name) const;
  /**@}*/

 private:
  // SimpleCutBasedEleID Variable:
  // https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID
  float dr03_tk_sum_pt_;
  float dr03_ecal_rechit_sum_et_;
  float dr03_hcal_tower_sum_et_;
 
  //ecal/hcal pf cluster isolation 
  float ecal_pf_cluster_iso_;
  float hcal_pf_cluster_iso_;

  // PF-based isolation variables
  float dr03_pfiso_charged_all_;
  float dr03_pfiso_charged_;
  float dr03_pfiso_neutral_;
  float dr03_pfiso_gamma_;
  float dr03_pfiso_pu_;
  float dr04_pfiso_charged_all_;
  float dr04_pfiso_charged_;
  float dr04_pfiso_neutral_;
  float dr04_pfiso_gamma_;
  float dr04_pfiso_pu_;

  float hadronic_over_em_;
  float full5x5_sigma_IetaIeta_;
  float sigma_IetaIeta_;
  float dphi_sc_tk_at_vtx_;
  float deta_sc_tk_at_vtx_;
  int gsf_tk_nhits_;
  float conv_dist_;
  float conv_dcot_;

  float f_brem_;
  float sc_eta_;
  float sc_theta_;
  float sc_e_over_p_;
  float sc_energy_;
  float r9_;
  float hcal_sum_;

  float ecal_energy_;
  bool has_matched_conversion_;

  Point ref_point_;

  double dxy_vertex_;
  double dz_vertex_;
  double dxy_beamspot_;

  std::vector<std::size_t> gen_particles_;

  UFmap elec_idiso_;

 #ifndef SKIP_CINT_DICT
 public:
  ClassDef(Electron, 4);
 #endif
};

typedef std::vector<ic::Electron> ElectronCollection;
}

/** \example plugins/ICElectronProducer.cc */
#endif
