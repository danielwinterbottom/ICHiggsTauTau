#ifndef UserCode_ICHiggsTauTau_ICTrackProducer_h
#define UserCode_ICHiggsTauTau_ICTrackProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "UserCode/ICHiggsTauTau/interface/Track.hh"

/**
 * @brief Produces an ic::Track collection from any input compatible with
 *an edm::View<reco::Track>
 *
 * **Example usage**
 * @snippet python/default_producers_cfi.py Track
 */
class ICTrackProducer : public edm::EDProducer {
 public:
  explicit ICTrackProducer(const edm::ParameterSet &);
  ~ICTrackProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  std::vector<ic::Track> *tracks_;
  edm::InputTag input_;
  std::string branch_;
  boost::hash<reco::Track const*> track_hasher_;
};

#endif
