#ifndef UserCode_ICHiggsTauTau_ICSuperClusterProducer_h
#define UserCode_ICHiggsTauTau_ICSuperClusterProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "UserCode/ICHiggsTauTau/interface/SuperCluster.hh"

class ICSuperClusterProducer : public edm::EDProducer {
 public:
  explicit ICSuperClusterProducer(const edm::ParameterSet&);
  ~ICSuperClusterProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  std::vector<ic::SuperCluster>* scs_;
  edm::InputTag input_barrel_;
  edm::InputTag input_endcap_;
  std::string branch_;

  boost::hash<reco::SuperCluster const*> sc_hasher_;
};

#endif
