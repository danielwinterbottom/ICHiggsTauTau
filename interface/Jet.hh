#ifndef ICHiggsTauTau_Jet_hh
#define ICHiggsTauTau_Jet_hh
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include <map>
#include <string>
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"


namespace ic {


  class Jet : public Candidate {

    private:
      typedef std::map<std::size_t, float> UFmap;
      typedef std::map<std::size_t, std::string> TSmap;

    public:
      Jet();
      virtual ~Jet();

      inline UFmap const& jec_factors() const { return jec_factors_; }
      inline void set_jec_factors(UFmap const& jec_factors) { jec_factors_ = jec_factors; }

      inline UFmap const& b_discriminators() const { return b_discriminators_; }
      inline void set_b_discriminators(UFmap const& b_discriminators) { b_discriminators_ = b_discriminators; }

      inline std::vector<std::size_t> const& gen_particles() const { return gen_particles_; }
      inline void set_gen_particles(std::vector<std::size_t> const& gen_particles) { gen_particles_ = gen_particles; }
      
      inline double const& uncorrected_energy() const { return uncorrected_energy_; }
      inline void set_uncorrected_energy(double const& uncorrected_energy) { uncorrected_energy_ = uncorrected_energy; }

      inline float const& jet_area() const { return jet_area_; }
      inline void set_jet_area(float const& jet_area) { jet_area_ = jet_area; }

      inline int const& parton_flavour() const { return parton_flavour_; }
      inline void set_parton_flavour(int const& parton_flavour) { parton_flavour_ = parton_flavour; }

      virtual void Print() const;
      void SetJecFactor(std::string const& name, float const& value);
      float GetJecFactor(std::string const& name) const;
      
      void SetBDiscriminator(std::string const& name, float const& value);
      float GetBDiscriminator(std::string const& name) const;

    private:
      UFmap jec_factors_;
      UFmap b_discriminators_;
      std::vector<std::size_t> gen_particles_;
      double uncorrected_energy_;
      float jet_area_;

      std::string UnHashJecFactor(std::size_t jec) const;
      std::string UnHashDiscr(std::size_t dis) const;

      int parton_flavour_;

  };

  typedef std::vector<ic::Jet> JetCollection;

}
#endif
