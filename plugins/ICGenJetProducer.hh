#ifndef UserCode_ICHiggsTauTau_ICGenJetProducer_h
#define UserCode_ICHiggsTauTau_ICGenJetProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"

/**
 * @brief See documentation [here](\ref objs-genjet)
 */
class ICGenJetProducer : public edm::EDProducer {
 public:
  explicit ICGenJetProducer(const edm::ParameterSet&);
  ~ICGenJetProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  std::vector<ic::GenJet>* gen_jets_;
  edm::InputTag input_;
  std::string branch_;
  edm::InputTag input_particles_;
  boost::hash<reco::GenJet const*> gen_jet_hasher_;
  boost::hash<reco::GenParticle const *> particle_hasher_;


  bool request_gen_particles_;
};

#endif
