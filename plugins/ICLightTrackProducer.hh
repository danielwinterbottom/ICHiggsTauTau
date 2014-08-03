#ifndef UserCode_ICHiggsTauTau_ICLightTrackProducer_h
#define UserCode_ICHiggsTauTau_ICLightTrackProducer_h

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
#include "UserCode/ICHiggsTauTau/interface/LightTrack.hh"

/**
 * @brief Produces an ic::LightTrack collection from any input compatible with
 *an edm::View<reco::Track>
 *
 * **Example usage**
 * @snippet python/default_producers_cfi.py LightTrack
 */
class ICLightTrackProducer : public edm::EDProducer {
 public:
  explicit ICLightTrackProducer(const edm::ParameterSet &);
  ~ICLightTrackProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  std::vector<ic::LightTrack> *tracks_;
  edm::InputTag input_;
  std::string branch_;
  boost::hash<reco::Track const*> track_hasher_;
};

#endif
