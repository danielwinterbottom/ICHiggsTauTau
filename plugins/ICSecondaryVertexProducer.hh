#ifndef UserCode_ICHiggsTauTau_ICSecondaryVertexProducer_h
#define UserCode_ICHiggsTauTau_ICSecondaryVertexProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "UserCode/ICHiggsTauTau/interface/SecondaryVertex.hh"

/**
 * @brief Produces an ic::SecondaryVertex collection from any input compatible
 *with an edm::View<reco::SecondaryVertexTagInfo>
 *
 * **Example usage**
 * @snippet python/default_producers_cfi.py SecondaryVertex
 */
class ICSecondaryVertexProducer : public edm::EDProducer {
 public:
  explicit ICSecondaryVertexProducer(const edm::ParameterSet &);
  ~ICSecondaryVertexProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  std::vector<ic::SecondaryVertex> *vertices_;
  edm::InputTag input_;
  std::string branch_;
  double track_pt_threshold_;
  bool request_trks_;
  boost::hash<reco::Vertex const*> vertex_hasher_;
  boost::hash<reco::Track const*> track_hasher_;
};

#endif
