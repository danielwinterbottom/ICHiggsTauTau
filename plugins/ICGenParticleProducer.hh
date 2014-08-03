#ifndef UserCode_ICHiggsTauTau_ICGenParticleProducer_h
#define UserCode_ICHiggsTauTau_ICGenParticleProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"

/**
 * @brief Produces an ic::GenParticle collection from any input compatible with
 *an edm::View<reco::GenParticle>
 *
 * **Example usage**
 * @snippet python/default_producers_cfi.py GenParticle
 */
class ICGenParticleProducer : public edm::EDProducer {
 public:
  explicit ICGenParticleProducer(const edm::ParameterSet &);
  ~ICGenParticleProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  std::vector<ic::GenParticle> *particles_;
  edm::InputTag input_;
  std::string branch_;
  boost::hash<reco::GenParticle const *> particle_hasher_;

  bool store_mothers_;
  bool store_daughters_;
};

#endif
