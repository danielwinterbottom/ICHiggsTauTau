#include "UserCode/ICHiggsTauTau/interface/PATConstruction.hh"
#include <boost/functional/hash.hpp>
#include "UserCode/ICHiggsTauTau/interface/city.h"

namespace ic {

  void ConstructJet(ic::Jet * jet, pat::Jet const* pat_jet) {
    static boost::hash<const pat::Jet*> hasher;

    jet->set_id(hasher(pat_jet));
    jet->set_pt(pat_jet->pt());
    jet->set_eta(pat_jet->eta());
    jet->set_phi(pat_jet->phi());
    jet->set_energy(pat_jet->energy());
    jet->set_charge(pat_jet->jetCharge());

    jet->set_uncorrected_energy((pat_jet->jecSetsAvailable() 
      ? pat_jet->jecFactor(0) : 1.)*pat_jet->energy());

    std::vector<std::string> jec_levels = pat_jet->availableJECLevels();
    for (unsigned i = 0; i < jec_levels.size(); ++i) {
      jet->SetJecFactor(jec_levels[i], pat_jet->jecFactor(jec_levels[i]));
    }

    typedef std::vector<std::pair<std::string, float> > SFPairVec;
    SFPairVec const& b_discr = pat_jet->getPairDiscri();
    for (unsigned i = 0; i < b_discr.size(); ++i) {
      jet->SetBDiscriminator(b_discr.at(i).first, b_discr.at(i).second);
    }

    jet->set_parton_flavour(pat_jet->partonFlavour());
    jet->set_jet_area(pat_jet->jetArea());
  }

  void ConstructCaloJet(ic::CaloJet * jet, pat::Jet const* pat_jet) {
    ConstructJet(jet, pat_jet);
    jet->set_max_em_tower_energy(pat_jet->maxEInEmTowers());
    jet->set_max_had_tower_energy(pat_jet->maxEInHadTowers());
    jet->set_energy_frac_had(pat_jet->energyFractionHadronic());
    jet->set_energy_frac_em(pat_jet->emEnergyFraction());
    jet->set_had_energy_HB(pat_jet->hadEnergyInHB());
    jet->set_had_energy_HO(pat_jet->hadEnergyInHO());
    jet->set_had_energy_HE(pat_jet->hadEnergyInHE());
    jet->set_had_energy_HF(pat_jet->hadEnergyInHF());
    jet->set_em_energy_EB(pat_jet->emEnergyInEB());
    jet->set_em_energy_EE(pat_jet->emEnergyInEE());
    jet->set_em_energy_HF(pat_jet->emEnergyInHF());
    jet->set_towers_area(pat_jet->towersArea());
    jet->set_n90(pat_jet->n90());
    jet->set_n60(pat_jet->n60());
    jet->set_fHPD(pat_jet->jetID().fHPD);
    jet->set_fRBX(pat_jet->jetID().fRBX);
  }

  void ConstructPFJet(ic::PFJet * jet, pat::Jet const* pat_jet) {
    ConstructJet(jet, pat_jet);
    jet->set_muon_multiplicity(pat_jet->muonMultiplicity());
    jet->set_charged_multiplicity(pat_jet->chargedMultiplicity());
    jet->set_charged_had_multiplicity(pat_jet->chargedHadronMultiplicity());
    jet->set_neutral_had_multiplicity(pat_jet->neutralHadronMultiplicity());
    jet->set_photon_multiplicity(pat_jet->photonMultiplicity());
    jet->set_electron_multiplicity(pat_jet->electronMultiplicity());
    jet->set_HF_had_multiplicity(pat_jet->HFHadronMultiplicity());
    jet->set_HF_em_multiplicity(pat_jet->HFEMMultiplicity());
    jet->set_neutral_multiplicity(pat_jet->neutralMultiplicity());
    jet->set_charged_em_energy(pat_jet->chargedEmEnergy());
    jet->set_neutral_em_energy(pat_jet->neutralEmEnergy());
    jet->set_charged_had_energy(pat_jet->chargedHadronEnergy());
    jet->set_neutral_had_energy(pat_jet->neutralHadronEnergy());
    jet->set_photon_energy(pat_jet->photonEnergy());
    jet->set_electron_energy(pat_jet->electronEnergy());
    jet->set_muon_energy(pat_jet->muonEnergy());
    jet->set_HF_had_energy(pat_jet->HFHadronEnergy());
    jet->set_HF_em_energy(pat_jet->HFEMEnergy());
    jet->set_charged_mu_energy(pat_jet->chargedMuEnergy());
  }

  void ConstructJPTJet(ic::JPTJet * jet, pat::Jet const* pat_jet) {
    ConstructJet(jet, pat_jet);
    ConstructCaloJet(jet, pat_jet);
    jet->set_muon_multiplicity(pat_jet->muonMultiplicity());
    jet->set_charged_multiplicity(pat_jet->chargedMultiplicity());
    jet->set_charged_em_energy(pat_jet->chargedEmEnergy());
    jet->set_neutral_em_energy(pat_jet->neutralEmEnergy());
    jet->set_charged_had_energy(pat_jet->chargedHadronEnergy());
    jet->set_neutral_had_energy(pat_jet->neutralHadronEnergy());
  }


  void ConstructGenParticle(ic::GenParticle * particle, 
                            reco::GenParticle const* reco_particle) {

  }

}
