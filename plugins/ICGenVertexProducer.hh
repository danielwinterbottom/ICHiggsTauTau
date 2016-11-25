#ifndef UserCode_ICHiggsTauTau_ICGenVertexProducer_h
#define UserCode_ICHiggsTauTau_ICGenVertexProducer_h

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
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"

class ICGenVertexProducer : public edm::EDProducer {
 public:
  explicit ICGenVertexProducer(const edm::ParameterSet &);
  ~ICGenVertexProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  std::vector<ic::Vertex> *vertices_;
  edm::InputTag input_;
  std::string branch_;

  std::string type_;
  std::set<std::tuple<double, double, double>> done_vtxs_;

};

#endif
