#ifndef ICHiggsTauTau_Photon_hh
#define ICHiggsTauTau_Photon_hh
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"


namespace ic {


  class Photon : public Candidate {

    public:
      Photon();
      virtual ~Photon();

      //! PF isolation, using charged hadrons in a cone with \f$ \Delta R = 0.3 \f$
      inline float dr03_pfiso_charged() const { return dr03_pfiso_charged_; }
      inline void set_dr03_pfiso_charged(float const& dr03_pfiso_charged) { dr03_pfiso_charged_ = dr03_pfiso_charged; }

      //! PF isolation, using neutral hadrons in a cone with \f$ \Delta R = 0.3 \f$
      inline float dr03_pfiso_neutral() const { return dr03_pfiso_neutral_; }
      inline void set_dr03_pfiso_neutral(float const& dr03_pfiso_neutral) { dr03_pfiso_neutral_ = dr03_pfiso_neutral; }

      //! PF isolation, using photons in a cone with \f$ \Delta R = 0.3 \f$
      inline float dr03_pfiso_gamma() const { return dr03_pfiso_gamma_; }
      inline void set_dr03_pfiso_gamma(float const& dr03_pfiso_gamma) { dr03_pfiso_gamma_ = dr03_pfiso_gamma; }

      inline float had_tower_over_em() const { return had_tower_over_em_; }
      inline void set_had_tower_over_em(float const& had_tower_over_em) { had_tower_over_em_ = had_tower_over_em; }

      inline float sigma_IetaIeta() const { return sigma_IetaIeta_; }
      inline void set_sigma_IetaIeta(float const& sigma_IetaIeta) { sigma_IetaIeta_ = sigma_IetaIeta; }

      inline bool pass_electron_veto() const { return pass_electron_veto_; }
      inline void set_pass_electron_veto(bool const& pass_electron_veto) { pass_electron_veto_ = pass_electron_veto; }

      virtual void Print() const;

    private:
      // PF-based isolation variables
      float dr03_pfiso_charged_;
      float dr03_pfiso_neutral_;
      float dr03_pfiso_gamma_;

      float had_tower_over_em_;
      float sigma_IetaIeta_;

      bool  pass_electron_veto_;

  };

  typedef std::vector<ic::Photon> PhotonCollection;

}
#endif
