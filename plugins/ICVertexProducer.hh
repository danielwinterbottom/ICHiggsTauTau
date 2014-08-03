#ifndef UserCode_ICHiggsTauTau_ICVertexProducer_h
#define UserCode_ICHiggsTauTau_ICVertexProducer_h

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
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"

/**
 * @brief Produces an ic::Vertex collection from any input compatible with
 *an edm::View<reco::Vertex>
 *
 * **Example usage**
 * @snippet python/default_producers_cfi.py Vertex
 */
class ICVertexProducer : public edm::EDProducer {
 public:
  explicit ICVertexProducer(const edm::ParameterSet&);
  ~ICVertexProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  std::vector<ic::Vertex> *vertices_;
  edm::InputTag input_;
  std::string branch_;
  bool first_only_;
  double track_pt_threshold_;
  bool request_trks_;
  boost::hash<reco::Vertex const*> vertex_hasher_;
  boost::hash<reco::Track const*> track_hasher_;
};

#endif
