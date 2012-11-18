#ifndef ICHiggsTauTau_Electron_hh
#define ICHiggsTauTau_Electron_hh
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include <map>
#include <string>
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"


namespace ic {


  class Electron : public Candidate {

    private:
      typedef ROOT::Math::XYZPoint Point;
      typedef std::map<std::size_t, float> UFmap;


    public:
      Electron();
      virtual ~Electron();

      inline std::vector<std::size_t> const& gen_particles() const { return gen_particles_; }
      inline void set_gen_particles(std::vector<std::size_t> const& gen_particles) { gen_particles_ = gen_particles; }
      
      inline Point const& ref_point() const { return ref_point_; }
      inline void set_ref_point(Point const& ref_point) { ref_point_ = ref_point; }
      
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

      inline float hadronic_over_em() const { return hadronic_over_em_; }
      inline void set_hadronic_over_em(float const& hadronic_over_em) { hadronic_over_em_ = hadronic_over_em; }

      inline float sigma_IetaIeta() const { return sigma_IetaIeta_; }
      inline void set_sigma_IetaIeta(float const& sigma_IetaIeta) { sigma_IetaIeta_ = sigma_IetaIeta; }

      inline float dphi_sc_tk_at_vtx() const { return dphi_sc_tk_at_vtx_; }
      inline void set_dphi_sc_tk_at_vtx(float const& dphi_sc_tk_at_vtx) { dphi_sc_tk_at_vtx_ = dphi_sc_tk_at_vtx; }

      inline float deta_sc_tk_at_vtx() const { return deta_sc_tk_at_vtx_; }
      inline void set_deta_sc_tk_at_vtx(float const& deta_sc_tk_at_vtx) { deta_sc_tk_at_vtx_ = deta_sc_tk_at_vtx; }

      inline int gsf_tk_nhits() const { return gsf_tk_nhits_; }
      inline void set_gsf_tk_nhits(int const& gsf_tk_nhits) { gsf_tk_nhits_ = gsf_tk_nhits; }

      inline float conv_dist() const { return conv_dist_; }
      inline void set_conv_dist(float const& conv_dist) { conv_dist_ = conv_dist; }

      inline float conv_dcot() const { return conv_dcot_; }
      inline void set_conv_dcot(float const& conv_dcot) { conv_dcot_ = conv_dcot; }

      // inline float mva() const { return mva_; }
      // inline void set_mva(float const& mva) { mva_ = mva; }
 
      inline float f_brem() const { return f_brem_; }
      inline void set_f_brem(float const& f_brem) { f_brem_ = f_brem; }

      inline float sc_eta() const { return sc_eta_; }
      inline void set_sc_eta(float const& sc_eta) { sc_eta_ = sc_eta; }

      inline float sc_theta() const { return sc_theta_; }
      inline void set_sc_theta(float const& sc_theta) { sc_theta_ = sc_theta; }

      inline float sc_energy() const { return sc_energy_; }
      inline void set_sc_energy(float const& sc_energy) { sc_energy_ = sc_energy; }

      inline float sc_e_over_p() const { return sc_e_over_p_; }
      inline void set_sc_e_over_p(float const& sc_e_over_p) { sc_e_over_p_ = sc_e_over_p; }

      inline float r9() const { return r9_; }
      inline void set_r9(float const& r9) { r9_ = r9; }

      inline float hcal_sum() const { return hcal_sum_; }
      inline void set_hcal_sum(float const& hcal_sum) { hcal_sum_ = hcal_sum; }

      inline float ecal_energy() const { return ecal_energy_; }
      inline void set_ecal_energy(float const& ecal_energy) { ecal_energy_ = ecal_energy; }

      inline bool has_matched_conversion() const { return has_matched_conversion_; }
      inline void set_has_matched_conversion(bool const& has_matched_conversion) { has_matched_conversion_ = has_matched_conversion; }

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
      // SimpleCutBasedEleID Variable:
      // https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID
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

      float hadronic_over_em_;
      float sigma_IetaIeta_;
      float dphi_sc_tk_at_vtx_;
      float deta_sc_tk_at_vtx_;
      int gsf_tk_nhits_;
      float conv_dist_;
      float conv_dcot_;

      //float mva_;
      
      // The following added in:
      // https://twiki.cern.ch/twiki/bin/view/CMS/VbtfEleID2011
      // https://twiki.cern.ch/twiki/bin/viewauth/CMS/SimpleCutBasedEleID2011
      float f_brem_;
      float sc_eta_;
      float sc_theta_;
      float sc_e_over_p_;
      float sc_energy_;
      float r9_;
      float hcal_sum_;

      float ecal_energy_;
      bool  has_matched_conversion_;

      // CANDIDATE FOR REMOVAL - store in eventInfo
      // double rho_;
      // CANDIDATE FOR REMOVAL - store in eventInfo
      // double pfnopu_rho_;


      Point ref_point_;

      double dxy_vertex_;
      double dz_vertex_;
      double dxy_beamspot_;

      // std::vector<std::size_t> hlt_match_paths_;
      // std::vector<std::size_t> hlt_match_filters_;
      std::vector<std::size_t> gen_particles_;

      UFmap elec_idiso_;

  };

  typedef std::vector<ic::Electron> ElectronCollection;

}
#endif
