#ifndef ICHiggsTauTau_Muon_hh
#define ICHiggsTauTau_Muon_hh
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include <map>
#include <string>
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"


namespace ic {


  class Muon : public Candidate {

    private:
      typedef ROOT::Math::XYZPoint Point;
      typedef std::map<std::size_t, float> UFmap;


    public:
      Muon();
      virtual ~Muon();

      inline std::vector<std::size_t> const& gen_particles() const { return gen_particles_; }
      inline void set_gen_particles(std::vector<std::size_t> const& gen_particles) { gen_particles_ = gen_particles; }
      
      inline Point const& ref_point() const { return ref_point_; }
      inline void set_ref_point(Point const& ref_point) { ref_point_ = ref_point; }
      
      inline bool is_standalone() const { return is_standalone_; }
      inline void set_is_standalone(bool const& is_standalone) { is_standalone_ = is_standalone; }

      inline bool is_global() const { return is_global_; }
      inline void set_is_global(bool const& is_global) { is_global_ = is_global; }

      inline bool is_tracker() const { return is_tracker_; }
      inline void set_is_tracker(bool const& is_tracker) { is_tracker_ = is_tracker; }

      inline bool is_calo() const { return is_calo_; }
      inline void set_is_calo(bool const& is_calo) { is_calo_ = is_calo; }

      inline double gt_normalized_chi2() const { return gt_normalized_chi2_; }
      inline void set_gt_normalized_chi2(double const& gt_normalized_chi2) { gt_normalized_chi2_ = gt_normalized_chi2; }
      
      inline int gt_valid_muon_hits() const { return gt_valid_muon_hits_; }
      inline void set_gt_valid_muon_hits(int const& gt_valid_muon_hits) { gt_valid_muon_hits_ = gt_valid_muon_hits; }
      
      inline int matched_stations() const { return matched_stations_; }
      inline void set_matched_stations(int const& matched_stations) { matched_stations_ = matched_stations; }
      
      inline int it_pixel_hits() const { return it_pixel_hits_; }
      inline void set_it_pixel_hits(int const& it_pixel_hits) { it_pixel_hits_ = it_pixel_hits; }
      
      inline int it_tracker_hits() const { return it_tracker_hits_; }
      inline void set_it_tracker_hits(int const& it_tracker_hits) { it_tracker_hits_ = it_tracker_hits; }
      
      inline int it_layers_with_measurement() const { return it_layers_with_measurement_; }
      inline void set_it_layers_with_measurement(int const& it_layers_with_measurement) { it_layers_with_measurement_ = it_layers_with_measurement; }
      
      inline float dr03_tk_sum_pt() const { return dr03_tk_sum_pt_; }
      inline void set_dr03_tk_sum_pt(float const& dr03_tk_sum_pt) { dr03_tk_sum_pt_ = dr03_tk_sum_pt; }

      inline float dr03_ecal_rechit_sum_et() const { return dr03_ecal_rechit_sum_et_; }
      inline void set_dr03_ecal_rechit_sum_et(float const& dr03_ecal_rechit_sum_et) { dr03_ecal_rechit_sum_et_ = dr03_ecal_rechit_sum_et; }

      inline float dr03_hcal_tower_sum_et() const { return dr03_hcal_tower_sum_et_; }
      inline void set_dr03_hcal_tower_sum_et(float const& dr03_hcal_tower_sum_et) { dr03_hcal_tower_sum_et_ = dr03_hcal_tower_sum_et; }
     
      inline float dr03_pfiso_charged_all() const { return dr03_pfiso_charged_all_; }
      inline void set_dr03_pfiso_charged_all(float const& dr03_pfiso_charged_all) { dr03_pfiso_charged_all_ = dr03_pfiso_charged_all; }

      inline float dr03_pfiso_charged() const { return dr03_pfiso_charged_; }
      inline void set_dr03_pfiso_charged(float const& dr03_pfiso_charged) { dr03_pfiso_charged_ = dr03_pfiso_charged; }

      inline float dr03_pfiso_neutral() const { return dr03_pfiso_neutral_; }
      inline void set_dr03_pfiso_neutral(float const& dr03_pfiso_neutral) { dr03_pfiso_neutral_ = dr03_pfiso_neutral; }

      inline float dr03_pfiso_gamma() const { return dr03_pfiso_gamma_; }
      inline void set_dr03_pfiso_gamma(float const& dr03_pfiso_gamma) { dr03_pfiso_gamma_ = dr03_pfiso_gamma; }

      inline float dr03_pfiso_pu() const { return dr03_pfiso_pu_; }
      inline void set_dr03_pfiso_pu(float const& dr03_pfiso_pu) { dr03_pfiso_pu_ = dr03_pfiso_pu; }

      inline float dr04_pfiso_charged_all() const { return dr04_pfiso_charged_all_; }
      inline void set_dr04_pfiso_charged_all(float const& dr04_pfiso_charged_all) { dr04_pfiso_charged_all_ = dr04_pfiso_charged_all; }

      inline float dr04_pfiso_charged() const { return dr04_pfiso_charged_; }
      inline void set_dr04_pfiso_charged(float const& dr04_pfiso_charged) { dr04_pfiso_charged_ = dr04_pfiso_charged; }

      inline float dr04_pfiso_neutral() const { return dr04_pfiso_neutral_; }
      inline void set_dr04_pfiso_neutral(float const& dr04_pfiso_neutral) { dr04_pfiso_neutral_ = dr04_pfiso_neutral; }

      inline float dr04_pfiso_gamma() const { return dr04_pfiso_gamma_; }
      inline void set_dr04_pfiso_gamma(float const& dr04_pfiso_gamma) { dr04_pfiso_gamma_ = dr04_pfiso_gamma; }

      inline float dr04_pfiso_pu() const { return dr04_pfiso_pu_; }
      inline void set_dr04_pfiso_pu(float const& dr04_pfiso_pu) { dr04_pfiso_pu_ = dr04_pfiso_pu; }

      // inline double rho() const { return rho_; }
      // inline void set_rho(double const& rho) { rho_ = rho; }

      // inline double pfnopu_rho() const { return pfnopu_rho_; }
      // inline void set_pfnopu_rho(double const& pfnopu_rho) { pfnopu_rho_ = pfnopu_rho; }

      inline double vx() const { return ref_point_.x(); }
      inline void set_vx(double const& x) { ref_point_.SetX(x); }

      inline double vy() const { return ref_point_.y(); }
      inline void set_vy(double const& y) { ref_point_.SetY(y); }

      inline double vz() const { return ref_point_.z(); }
      inline void set_vz(double const& z) { ref_point_.SetZ(z); }

      inline double dxy_vertex() const { return dxy_vertex_; }
      inline void set_dxy_vertex(double const& dxy_vertex) { dxy_vertex_ = dxy_vertex; }
  
      inline double dz_vertex() const { return dz_vertex_; }
      inline void set_dz_vertex(double const& dz_vertex) { dz_vertex_ = dz_vertex; }

      inline double dxy_beamspot() const { return dxy_beamspot_; }
      inline void set_dxy_beamspot(double const& dxy_beamspot) { dxy_beamspot_ = dxy_beamspot; }
      
      // inline std::vector<std::size_t>  const& hlt_match_paths() const { return hlt_match_paths_; }
      // inline void set_hlt_match_paths(std::vector<std::size_t>  const& hlt_match_paths) { hlt_match_paths_ = hlt_match_paths; }

      // inline std::vector<std::size_t>  const& hlt_match_filters() const { return hlt_match_filters_; }
      // inline void set_hlt_match_filters(std::vector<std::size_t>  const& hlt_match_filters) { hlt_match_filters_ = hlt_match_filters; }

      void SetIdIso(std::string const& name, float const& value);
      float GetIdIso(std::string const& name) const;

      virtual void Print() const;

    private:
      float dr03_tk_sum_pt_;
      float dr03_ecal_rechit_sum_et_;
      float dr03_hcal_tower_sum_et_;

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

      // Tight muon selection from:
      // https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId
      bool is_standalone_;
      bool is_global_;
      bool is_tracker_;
      bool is_calo_;
      double gt_normalized_chi2_;
      int gt_valid_muon_hits_;
      int matched_stations_;
      int it_pixel_hits_;
      int it_tracker_hits_;
      int it_layers_with_measurement_;
      // double rho_;
      // double pfnopu_rho_;


      Point ref_point_;

      double dxy_vertex_;
      double dz_vertex_;
      double dxy_beamspot_;

      // std::vector<std::size_t> hlt_match_paths_;
      // std::vector<std::size_t> hlt_match_filters_;

      std::vector<std::size_t> gen_particles_;

      UFmap muon_idiso_;

  };

  typedef std::vector<ic::Muon> MuonCollection;

}
#endif
