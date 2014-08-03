#ifndef UserCode_ICHiggsTauTau_ICCandidateProducer_h
#define UserCode_ICHiggsTauTau_ICCandidateProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

/**
 * @brief Produces an ic::Candidate collection from any input compatible
 *with an edm::View<reco::Candidate>
 *
 * **Example usage**
 * @snippet python/default_producers_cfi.py Candidate
 */
class ICCandidateProducer : public edm::EDProducer {
 public:
  explicit ICCandidateProducer(const edm::ParameterSet &);
  ~ICCandidateProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  std::vector<ic::Candidate> *candidates_;
  edm::InputTag input_;
  std::string branch_;
  boost::hash<reco::Candidate const*> cand_hasher_;
};

#endif
