#ifndef ICHiggsTauTau_PFJet_hh
#define ICHiggsTauTau_PFJet_hh
#include <map>
#include <string>
#include "UserCode/ICHiggsTauTau/interface/Jet.hh"


namespace ic {


  class PFJet : public Jet {
    public:
      PFJet();
      virtual ~PFJet();

      virtual void Print() const;

      inline int const& muon_multiplicity() const { return muon_multiplicity_; }
      inline void set_muon_multiplicity(int const& muon_multiplicity) { muon_multiplicity_ = muon_multiplicity; }

      inline int const& charged_multiplicity() const { return charged_multiplicity_; }
      inline void set_charged_multiplicity(int const& charged_multiplicity) { charged_multiplicity_ = charged_multiplicity; }

      inline int const& charged_had_multiplicity() const { return charged_had_multiplicity_; }
      inline void set_charged_had_multiplicity(int const& charged_had_multiplicity) { charged_had_multiplicity_ = charged_had_multiplicity; }

      inline int const& neutral_had_multiplicity() const { return neutral_had_multiplicity_; }
      inline void set_neutral_had_multiplicity(int const& neutral_had_multiplicity) { neutral_had_multiplicity_ = neutral_had_multiplicity; }

      inline int const& photon_multiplicity() const { return photon_multiplicity_; }
      inline void set_photon_multiplicity(int const& photon_multiplicity) { photon_multiplicity_ = photon_multiplicity; }

      inline int const& electron_multiplicity() const { return electron_multiplicity_; }
      inline void set_electron_multiplicity(int const& electron_multiplicity) { electron_multiplicity_ = electron_multiplicity; }

      inline int const& HF_had_multiplicity() const { return HF_had_multiplicity_; }
      inline void set_HF_had_multiplicity(int const& HF_had_multiplicity) { HF_had_multiplicity_ = HF_had_multiplicity; }
      
      inline int const& HF_em_multiplicity() const { return HF_em_multiplicity_; }
      inline void set_HF_em_multiplicity(int const& HF_em_multiplicity) { HF_em_multiplicity_ = HF_em_multiplicity; }
      
      inline int const& neutral_multiplicity() const { return neutral_multiplicity_; }
      inline void set_neutral_multiplicity(int const& neutral_multiplicity) { neutral_multiplicity_ = neutral_multiplicity; }

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

      inline float const& photon_energy() const { return photon_energy_; }
      inline void set_photon_energy(float const& photon_energy) { photon_energy_ = photon_energy; }
      inline float  photon_energy_frac() const { 
            return (photon_energy_ / uncorrected_energy()); 
      }

      inline float const& electron_energy() const { return electron_energy_; }
      inline void set_electron_energy(float const& electron_energy) { electron_energy_ = electron_energy; }
      inline float  electron_energy_frac() const { 
            return (electron_energy_ / uncorrected_energy()); 
      }

      inline float const& muon_energy() const { return muon_energy_; }
      inline void set_muon_energy(float const& muon_energy) { muon_energy_ = muon_energy; }
      inline float  muon_energy_frac() const { 
            return (muon_energy_ / uncorrected_energy()); 
      }

      inline float const& HF_had_energy() const { return HF_had_energy_; }
      inline void set_HF_had_energy(float const& HF_had_energy) { HF_had_energy_ = HF_had_energy; }
      inline float  HF_had_energy_frac() const { 
            return (HF_had_energy_ / uncorrected_energy()); 
      }

      inline float const& HF_em_energy() const { return HF_em_energy_; }
      inline void set_HF_em_energy(float const& HF_em_energy) { HF_em_energy_ = HF_em_energy; }
      inline float  HF_em_energy_frac() const { 
            return (HF_em_energy_ / uncorrected_energy()); 
      }

      inline float const& charged_mu_energy() const { return charged_mu_energy_; }
      inline void set_charged_mu_energy(float const& charged_mu_energy) { charged_mu_energy_ = charged_mu_energy; }
      inline float  charged_mu_energy_frac() const { 
            return (charged_mu_energy_ / uncorrected_energy()); 
      }

      inline float const& beta() const { return beta_; }
      inline void set_beta(float const& beta) { beta_ = beta; }

      inline float const& beta_max() const { return beta_max_; }
      inline void set_beta_max(float const& beta_max) { beta_max_ = beta_max; }

      inline float const& pu_id_mva_value() const { return pu_id_mva_value_; }
      inline void set_pu_id_mva_value(float const& pu_id_mva_value) { pu_id_mva_value_ = pu_id_mva_value; }

      //inline bool const& pu_id_mva_loose() const { return pu_id_mva_loose_; }
      //inline void set_pu_id_mva_loose(bool const& pu_id_mva_loose) { pu_id_mva_loose_ = pu_id_mva_loose; }

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
                  } else return pu_id_mva_loose_;
            } else if (pt > 30.) {
                  if (abs_eta < 2.5) {
                        return (pu_id_mva_value_ > -0.80);
                  } else if (abs_eta < 2.75) {
                        return (pu_id_mva_value_ > -0.74);
                  } else if (abs_eta < 3.0) {
                        return (pu_id_mva_value_ > -0.68);
                  } else if (abs_eta < 5.0) {
                        return (pu_id_mva_value_ > -0.77);
                  } else return pu_id_mva_loose_;
            } else return pu_id_mva_loose_;
      }

      inline void set_pu_id_mva_loose(bool const& pu_id_mva_loose) { pu_id_mva_loose_ = pu_id_mva_loose; }

      inline bool const& pu_id_mva_medium() const { return pu_id_mva_medium_; }
      inline void set_pu_id_mva_medium(bool const& pu_id_mva_medium) { pu_id_mva_medium_ = pu_id_mva_medium; }

      inline bool const& pu_id_mva_tight() const { return pu_id_mva_tight_; }
      inline void set_pu_id_mva_tight(bool const& pu_id_mva_tight) { pu_id_mva_tight_ = pu_id_mva_tight; }

      inline std::vector<std::size_t> const& constituent_tracks() const { return constituent_tracks_; }
      inline void set_constituent_tracks(std::vector<std::size_t> const& constituent_tracks) { constituent_tracks_ = constituent_tracks; }



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
      bool pu_id_mva_loose_;
      bool pu_id_mva_medium_;
      bool pu_id_mva_tight_;

      std::vector<std::size_t> constituent_tracks_;


  };

  typedef std::vector<ic::PFJet> PFJetCollection;

}
#endif
