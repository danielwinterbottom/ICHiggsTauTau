#ifndef ICHiggsTauTau_JPTJet_hh
#define ICHiggsTauTau_JPTJet_hh
#include <map>
#include <string>
#include "UserCode/ICHiggsTauTau/interface/CaloJet.hh"


namespace ic {


  class JPTJet : public CaloJet {
    public:
      JPTJet();
      virtual ~JPTJet();

      inline int const& muon_multiplicity() const { return muon_multiplicity_; }
      inline void set_muon_multiplicity(int const& muon_multiplicity) { muon_multiplicity_ = muon_multiplicity; }

      inline int const& charged_multiplicity() const { return charged_multiplicity_; }
      inline void set_charged_multiplicity(int const& charged_multiplicity) { charged_multiplicity_ = charged_multiplicity; }

      inline float const& charged_em_energy() const { return charged_em_energy_; }
      inline void set_charged_em_energy(float const& charged_em_energy) { charged_em_energy_ = charged_em_energy; }
      inline float  charged_em_energy_frac() const { 
            return (charged_em_energy_ / uncorrected_energy()); 
      }

      inline float const& neutral_em_energy() const { return neutral_em_energy_; }
      inline void set_neutral_em_energy(float const& neutral_em_energy) { neutral_em_energy_ = neutral_em_energy; }
      inline float  neutral_em_energy_frac() const { 
            return (neutral_em_energy_ / uncorrected_energy()); 
      }

      inline float const& charged_had_energy() const { return charged_had_energy_; }
      inline void set_charged_had_energy(float const& charged_had_energy) { charged_had_energy_ = charged_had_energy; }
      inline float  charged_had_energy_frac() const { 
            return (charged_had_energy_ / uncorrected_energy()); 
      }

      inline float const& neutral_had_energy() const { return neutral_had_energy_; }
      inline void set_neutral_had_energy(float const& neutral_had_energy) { neutral_had_energy_ = neutral_had_energy; }
      inline float  neutral_had_energy_frac() const { 
            return (neutral_had_energy_ / uncorrected_energy()); 
      }

      inline std::vector<std::size_t> const& pions_in_vtx_in_calo() const { return pions_in_vtx_in_calo_; }
      inline void set_pions_in_vtx_in_calo(std::vector<std::size_t> const& pions_in_vtx_in_calo) { pions_in_vtx_in_calo_ = pions_in_vtx_in_calo; }

      inline std::vector<std::size_t> const& pions_in_vtx_out_calo() const { return pions_in_vtx_out_calo_; }
      inline void set_pions_in_vtx_out_calo(std::vector<std::size_t> const& pions_in_vtx_out_calo) { pions_in_vtx_out_calo_ = pions_in_vtx_out_calo; }

      inline std::vector<std::size_t> const& pions_out_vtx_in_calo() const { return pions_out_vtx_in_calo_; }
      inline void set_pions_out_vtx_in_calo(std::vector<std::size_t> const& pions_out_vtx_in_calo) { pions_out_vtx_in_calo_ = pions_out_vtx_in_calo; }

      inline std::vector<std::size_t> const& muons_in_vtx_in_calo() const { return muons_in_vtx_in_calo_; }
      inline void set_muons_in_vtx_in_calo(std::vector<std::size_t> const& muons_in_vtx_in_calo) { muons_in_vtx_in_calo_ = muons_in_vtx_in_calo; }

      inline std::vector<std::size_t> const& muons_in_vtx_out_calo() const { return muons_in_vtx_out_calo_; }
      inline void set_muons_in_vtx_out_calo(std::vector<std::size_t> const& muons_in_vtx_out_calo) { muons_in_vtx_out_calo_ = muons_in_vtx_out_calo; }

      inline std::vector<std::size_t> const& muons_out_vtx_in_calo() const { return muons_out_vtx_in_calo_; }
      inline void set_muons_out_vtx_in_calo(std::vector<std::size_t> const& muons_out_vtx_in_calo) { muons_out_vtx_in_calo_ = muons_out_vtx_in_calo; }

      inline std::vector<std::size_t> const& elecs_in_vtx_in_calo() const { return elecs_in_vtx_in_calo_; }
      inline void set_elecs_in_vtx_in_calo(std::vector<std::size_t> const& elecs_in_vtx_in_calo) { elecs_in_vtx_in_calo_ = elecs_in_vtx_in_calo; }

      inline std::vector<std::size_t> const& elecs_in_vtx_out_calo() const { return elecs_in_vtx_out_calo_; }
      inline void set_elecs_in_vtx_out_calo(std::vector<std::size_t> const& elecs_in_vtx_out_calo) { elecs_in_vtx_out_calo_ = elecs_in_vtx_out_calo; }

      inline std::vector<std::size_t> const& elecs_out_vtx_in_calo() const { return elecs_out_vtx_in_calo_; }
      inline void set_elecs_out_vtx_in_calo(std::vector<std::size_t> const& elecs_out_vtx_in_calo) { elecs_out_vtx_in_calo_ = elecs_out_vtx_in_calo; }

      inline float const& beta() const { return beta_; }
      inline void set_beta(float const& beta) { beta_ = beta; }

      inline float const& beta_max() const { return beta_max_; }
      inline void set_beta_max(float const& beta_max) { beta_max_ = beta_max; }
    
      inline float const& track_pt_total() const { return track_pt_total_; }
      inline void set_track_pt_total(float const& track_pt_total) { track_pt_total_ = track_pt_total; }


      std::vector<std::size_t> constituent_tracks() const;

      virtual void Print() const;

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
     
  };

  typedef std::vector<ic::JPTJet> JPTJetCollection;

}
#endif
