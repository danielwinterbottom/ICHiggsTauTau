#ifndef ICHiggsTauTau_Tau_hh
#define ICHiggsTauTau_Tau_hh
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include <map>
#include <string>
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"


namespace ic {


  class Tau : public Candidate {

    private:
      typedef std::map<std::size_t, float> UFmap;
      typedef std::map<std::size_t, std::string> TSmap;
      typedef ROOT::Math::XYZPoint Point;


    public:
      Tau();
      virtual ~Tau();

      inline UFmap const& tau_ids() const { return tau_ids_; }
      inline void set_tau_ids(UFmap const& tau_ids) { tau_ids_ = tau_ids; }
      
      inline int const& decay_mode() const { return decay_mode_; }
      inline void set_decay_mode(int const& decay_mode) { decay_mode_ = decay_mode; }

      inline float const& elec_preid_output() const { return elec_preid_output_; }
      inline void set_elec_preid_output(float const& elec_preid_output) { elec_preid_output_ = elec_preid_output; }

      inline bool const& elec_preid_decision() const { return elec_preid_decision_; }
      inline void set_elec_preid_decision(bool const& elec_preid_decision) { elec_preid_decision_ = elec_preid_decision; }
 
      inline float const& lead_ecal_energy() const { return lead_ecal_energy_; }
      inline void set_lead_ecal_energy(float const& lead_ecal_energy) { lead_ecal_energy_ = lead_ecal_energy; }

      inline float const& lead_hcal_energy() const { return lead_hcal_energy_; }
      inline void set_lead_hcal_energy(float const& lead_hcal_energy) { lead_hcal_energy_ = lead_hcal_energy; }

      inline float const& lead_p() const { return lead_p_; }
      inline void set_lead_p(float const& lead_p) { lead_p_ = lead_p; }

      inline float const& lead_dxy_vertex() const { return lead_dxy_vertex_; }
      inline void set_lead_dxy_vertex(float const& lead_dxy_vertex) { lead_dxy_vertex_ = lead_dxy_vertex; }

      inline float const& lead_dz_vertex() const { return lead_dz_vertex_; }
      inline void set_lead_dz_vertex(float const& lead_dz_vertex) { lead_dz_vertex_ = lead_dz_vertex; }

      inline double vx() const { return ref_point_.x(); }
      inline void set_vx(double const& x) { ref_point_.SetX(x); }

      inline double vy() const { return ref_point_.y(); }
      inline void set_vy(double const& y) { ref_point_.SetY(y); }

      inline double vz() const { return ref_point_.z(); }
      inline void set_vz(double const& z) { ref_point_.SetZ(z); }

      inline std::vector<std::size_t> const& constituent_tracks() const { return constituent_tracks_; }
      inline void set_constituent_tracks(std::vector<std::size_t> const& constituent_tracks) { constituent_tracks_ = constituent_tracks; }

      inline std::vector<std::size_t>  const& hlt_match_paths() const { return hlt_match_paths_; }
      inline void set_hlt_match_paths(std::vector<std::size_t>  const& hlt_match_paths) { hlt_match_paths_ = hlt_match_paths; }

      inline std::vector<std::size_t>  const& hlt_match_filters() const { return hlt_match_filters_; }
      inline void set_hlt_match_filters(std::vector<std::size_t>  const& hlt_match_filters) { hlt_match_filters_ = hlt_match_filters; }

      virtual void Print() const;

      
      void SetTauID(std::string const& name, float const& value);
      float GetTauID(std::string const& name) const;

    private:
      UFmap tau_ids_;

      int decay_mode_;

      float elec_preid_output_;
      bool elec_preid_decision_;

      float lead_ecal_energy_;
      float lead_hcal_energy_;
      float lead_p_;
      float lead_dxy_vertex_;
      float lead_dz_vertex_;


      //std::string UnHashDiscr(std::size_t dis) const;
      Point ref_point_;



      std::vector<std::size_t> constituent_tracks_;

      std::vector<std::size_t> hlt_match_paths_;
      std::vector<std::size_t> hlt_match_filters_;


  };

  typedef std::vector<ic::Tau> TauCollection;

}
#endif
