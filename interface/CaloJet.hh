#ifndef ICHiggsTauTau_CaloJet_hh
#define ICHiggsTauTau_CaloJet_hh
#include <map>
#include <string>
#include <vector>
#include "UserCode/ICHiggsTauTau/interface/Jet.hh"

namespace ic {

class CaloJet : public Jet {
 public:
  CaloJet();
  virtual ~CaloJet();

  virtual void Print() const;

  inline float const& max_em_tower_energy() const {
    return max_em_tower_energy_;
  }
  inline void set_max_em_tower_energy(float const& max_em_tower_energy) {
    max_em_tower_energy_ = max_em_tower_energy;
  }

  inline float const& max_had_tower_energy() const {
    return max_had_tower_energy_;
  }
  inline void set_max_had_tower_energy(float const& max_had_tower_energy) {
    max_had_tower_energy_ = max_had_tower_energy;
  }

  inline float const& energy_frac_had() const { return energy_frac_had_; }
  inline void set_energy_frac_had(float const& energy_frac_had) {
    energy_frac_had_ = energy_frac_had;
  }

  inline float const& energy_frac_em() const { return energy_frac_em_; }
  inline void set_energy_frac_em(float const& energy_frac_em) {
    energy_frac_em_ = energy_frac_em;
  }

  inline float const& had_energy_HB() const { return had_energy_HB_; }
  inline void set_had_energy_HB(float const& had_energy_HB) {
    had_energy_HB_ = had_energy_HB;
  }

  inline float const& had_energy_HO() const { return had_energy_HO_; }
  inline void set_had_energy_HO(float const& had_energy_HO) {
    had_energy_HO_ = had_energy_HO;
  }

  inline float const& had_energy_HE() const { return had_energy_HE_; }
  inline void set_had_energy_HE(float const& had_energy_HE) {
    had_energy_HE_ = had_energy_HE;
  }

  inline float const& had_energy_HF() const { return had_energy_HF_; }
  inline void set_had_energy_HF(float const& had_energy_HF) {
    had_energy_HF_ = had_energy_HF;
  }

  inline float const& em_energy_EB() const { return em_energy_EB_; }
  inline void set_em_energy_EB(float const& em_energy_EB) {
    em_energy_EB_ = em_energy_EB;
  }

  inline float const& em_energy_EE() const { return em_energy_EE_; }
  inline void set_em_energy_EE(float const& em_energy_EE) {
    em_energy_EE_ = em_energy_EE;
  }

  inline float const& em_energy_HF() const { return em_energy_HF_; }
  inline void set_em_energy_HF(float const& em_energy_HF) {
    em_energy_HF_ = em_energy_HF;
  }

  inline float const& towers_area() const { return towers_area_; }
  inline void set_towers_area(float const& towers_area) {
    towers_area_ = towers_area;
  }

  inline int const& n90() const { return n90_; }
  inline void set_n90(int const& n90) { n90_ = n90; }

  inline int const& n60() const { return n60_; }
  inline void set_n60(int const& n60) { n60_ = n60; }

  inline float const& fHPD() const { return fHPD_; }
  inline void set_fHPD(float const& fHPD) { fHPD_ = fHPD; }

  inline float const& fRBX() const { return fRBX_; }
  inline void set_fRBX(float const& fRBX) { fRBX_ = fRBX; }

  inline bool const& id_loose() const { return id_loose_; }
  inline void set_id_loose(bool const& id_loose) { id_loose_ = id_loose; }

  inline bool const& id_tight() const { return id_tight_; }
  inline void set_id_tight(bool const& id_tight) { id_tight_ = id_tight; }

 private:
  float max_em_tower_energy_;
  float max_had_tower_energy_;
  float energy_frac_had_;
  float energy_frac_em_;
  float had_energy_HB_;
  float had_energy_HO_;
  float had_energy_HE_;
  float had_energy_HF_;
  float em_energy_EB_;
  float em_energy_EE_;
  float em_energy_HF_;
  float towers_area_;
  int n90_;
  int n60_;
  float fHPD_;
  float fRBX_;
  bool id_loose_;
  bool id_tight_;
};

typedef std::vector<ic::CaloJet> CaloJetCollection;
}
#endif
