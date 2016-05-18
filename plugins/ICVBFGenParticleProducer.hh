#ifndef UserCode_ICHiggsTauTau_ICVBFGenParticleProducer_h
#define UserCode_ICHiggsTauTau_ICVBFGenParticleProducer_h

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
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "UserCode/ICHiggsTauTau/interface/VBFHiggsGenAnalysisDataFormat.h"
#include "TTree.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

/**
 * @brief See documentation [here](\ref objs-genparticle)
 */
class ICVBFGenParticleProducer : public edm::EDProducer {
 public:
  explicit ICVBFGenParticleProducer(const edm::ParameterSet &);
  ~ICVBFGenParticleProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  std::string branch_;
  
  edm::ParameterSet ps;
  
  TTree * tree_;
  
  // Input tags
  edm::EDGetTokenT< edm::HepMCProduct >           m_InputTag_HepMCProduct;
  edm::EDGetTokenT< reco::GenJetCollection > m_inputTag_GenJetCollection;
  edm::EDGetTokenT< reco::GenParticleCollection > m_inputTag_GenParticleCollection;
  
  VBFHiggs::GenAnalysisDataFormat *m_genAnalysisData; // Data for tree
  
  
};

#endif
