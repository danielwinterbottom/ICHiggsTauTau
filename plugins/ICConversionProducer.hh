#ifndef UserCode_ICHiggsTauTau_ICConversionProducer_h
#define UserCode_ICHiggsTauTau_ICConversionProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"
#include "UserCode/ICHiggsTauTau/interface/Track.hh"

/**
 * @brief See documentation [here](\ref objs-vertex)
 */
class ICConversionProducer : public edm::EDProducer {
 public:
  explicit ICConversionProducer(const edm::ParameterSet&);
  ~ICConversionProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  std::vector<ic::Vertex> *vertices_;
  std::vector<ic::Track> *tracks_;
  edm::InputTag input_;
  std::string branch_;
  std::string tracks_branch_;
  bool request_trks_;
  boost::hash<reco::Conversion const*> conversion_hasher_;
  boost::hash<reco::TrackBase const*> track_hasher_;
};

#endif
