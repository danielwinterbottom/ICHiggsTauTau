#ifndef ICHiggsTauTau_Muon_hh
#define ICHiggsTauTau_Muon_hh
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
 * @brief This class stores a subset of the reco::Muon
 * properties which are most commonly used in analysis.
 *
 * Useful links:
 *  - https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMuonAnalysis
 *  - https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId
 */
class Muon : public Candidate {
 private:
  typedef ROOT::Math::XYZPoint Point;
  typedef std::map<std::size_t, float> UFmap;

 public:
  Muon();
  virtual ~Muon();
  virtual void Print() const;


  /// @name Properties
  /**@{*/
  /// A vector to refer to ic::GenParticle::id() values
  /**
   * @deprecated This property is no longer filled in the ICMuonProducer. It
   * is advised to save generator-level electrons that may be needed for
   * matching using the ICGenParticleProducer and to identify matches in the
   * analysis code.
   */
  inline std::vector<std::size_t> const& gen_particles() const {
    return gen_particles_;
  }

  /// Point-of-closest-approach (PCA) of the muon track to the beamspot
  inline Point const& ref_point() const { return ref_point_; }

  /// Muon was reconstructed by the stand-alone algorithm
  inline bool is_standalone() const { return is_standalone_; }

  /// Muon was reconstructed by the global algorithm
  inline bool is_global() const { return is_global_; }

  /// Muon was reconstructed by the tracker-only algorithm
  inline bool is_tracker() const { return is_tracker_; }

  /// Muon was reconstructed by the calo-muon algorithm
  inline bool is_calo() const { return is_calo_; }

  /// Muon was reconstructed by the PF algorithm
  inline bool is_pf() const { return is_pf_; }

  /// Normalised \f$\chi^2\f$ of the global track fit
  inline double gt_normalized_chi2() const { return gt_normalized_chi2_; }

  /// \f$\chi^2\f$ for the sta-tk matching of local position
  inline float cq_chi2_localposition() const { return cq_chi2_localposition_; }

  /// Kink position for the tracker stub and global track
  inline float cq_trk_kink() const { return cq_trk_kink_; }

  /// Number of muon station hits used in the global track fit
  inline int gt_valid_muon_hits() const { return gt_valid_muon_hits_; }

  /// Number of matched muon station track segments
  inline int matched_stations() const { return matched_stations_; }

  /// Number of pixel layer hits in the tracker
  inline int it_pixel_hits() const { return it_pixel_hits_; }

  /// Number of hits in the tracker
  inline int it_tracker_hits() const { return it_tracker_hits_; }

  /// Number of tracker layers with measurement
  inline int it_layers_with_measurement() const {
    return it_layers_with_measurement_;
  }

  /// Fraction of valid inner tracker hits
  inline double it_valid_fraction() const { return it_valid_fraction_; }

  /// Compatibility of segment with global fit
  inline float segment_compatibility() const { return segment_compatibility_; }

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

  /// The x-coordinate of the ref_point()
  inline double vx() const { return ref_point_.x(); }

  /// The y-coordinate of the ref_point()
  inline double vy() const { return ref_point_.y(); }

  /// The z-coordinate of the ref_point()
  inline double vz() const { return ref_point_.z(); }

  /// Transverse impact parameter of the tracker track with the primary vertex
  inline double dxy_vertex() const { return dxy_vertex_; }

  /// Longitudinal impact parameter of the tracker track with primary vertex
  inline double dz_vertex() const { return dz_vertex_; }

  /// Transverse impact parameter of the tracker track with the beamspot
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

  /// @copybrief is_standalone()
  inline void set_is_standalone(bool const& is_standalone) {
    is_standalone_ = is_standalone;
  }

  /// @copybrief is_global()
  inline void set_is_global(bool const& is_global) { is_global_ = is_global; }

  /// @copybrief is_tracker()
  inline void set_is_tracker(bool const& is_tracker) {
    is_tracker_ = is_tracker;
  }

  /// @copybrief is_calo()
  inline void set_is_calo(bool const& is_calo) { is_calo_ = is_calo; }

  /// @copybrief is_pf()
  inline void set_is_pf(bool const& is_pf) { is_pf_ = is_pf; }

  /// @copybrief gt_normalized_chi2()
  inline void set_gt_normalized_chi2(double const& gt_normalized_chi2) {
    gt_normalized_chi2_ = gt_normalized_chi2;
  }

  /// @copybrief cq_chi2_localposition()
  inline void set_cq_chi2_localposition(float const& cq_chi2_localposition) {
    cq_chi2_localposition_ = cq_chi2_localposition;
  }

  /// @copybrief cq_trk_kink()
  inline void set_cq_trk_kink(float const& cq_trk_kink) {
    cq_trk_kink_ = cq_trk_kink;
  }

  /// @copybrief gt_valid_muon_hits()
  inline void set_gt_valid_muon_hits(int const& gt_valid_muon_hits) {
    gt_valid_muon_hits_ = gt_valid_muon_hits;
  }

  /// @copybrief matched_stations()
  inline void set_matched_stations(int const& matched_stations) {
    matched_stations_ = matched_stations;
  }

  /// @copybrief it_pixel_hits()
  inline void set_it_pixel_hits(int const& it_pixel_hits) {
    it_pixel_hits_ = it_pixel_hits;
  }

  /// @copybrief it_tracker_hits()
  inline void set_it_tracker_hits(int const& it_tracker_hits) {
    it_tracker_hits_ = it_tracker_hits;
  }

  /// @copybrief it_layers_with_measurement()
  inline void set_it_layers_with_measurement(
      int const& it_layers_with_measurement) {
    it_layers_with_measurement_ = it_layers_with_measurement;
  }

  /// @copybrief it_valid_fraction()
  inline void set_it_valid_fraction(double const& it_valid_fraction) {
    it_valid_fraction_ = it_valid_fraction;
  }

  /// @copybrief segment_compatibility()
  inline void set_segment_compatibility(float const& segment_compatibility) {
    segment_compatibility_ = segment_compatibility;
  }

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
   * @details The Muon class contains a map for storing arbitrary pairs
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
  float dr03_tk_sum_pt_;
  float dr03_ecal_rechit_sum_et_;
  float dr03_hcal_tower_sum_et_;

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

  bool is_standalone_;
  bool is_global_;
  bool is_tracker_;
  bool is_calo_;
  bool is_pf_;
  double gt_normalized_chi2_;
  float cq_chi2_localposition_;
  float cq_trk_kink_;
  int gt_valid_muon_hits_;
  int matched_stations_;
  int it_pixel_hits_;
  int it_tracker_hits_;
  int it_layers_with_measurement_;
  double it_valid_fraction_;
  float segment_compatibility_;

  Point ref_point_;

  double dxy_vertex_;
  double dz_vertex_;
  double dxy_beamspot_;

  std::vector<std::size_t> gen_particles_;

  UFmap muon_idiso_;

 #ifndef SKIP_CINT_DICT
 public:
  ClassDef(Muon, 5);
 #endif
};

typedef std::vector<ic::Muon> MuonCollection;
}
/** \example plugins/ICMuonProducer.cc */
#endif
