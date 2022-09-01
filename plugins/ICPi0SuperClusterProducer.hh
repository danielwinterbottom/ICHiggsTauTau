#ifndef UserCode_ICHiggsTauTau_ICPi0SuperClusterProducer_h
#define UserCode_ICHiggsTauTau_ICPi0SuperClusterProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "UserCode/ICHiggsTauTau/interface/SuperCluster.hh"
#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/Records/interface/CaloTopologyRecord.h"

/**
 * @brief Produces an ic::SuperCluster collection from the separate barrel and endcap collections, which must be compatible with
 *an edm::View<reco::SuperCluster>
 *
 * **Example usage**
 * @snippet python/default_producers_cfi.py SuperCluster
 */
class ICPi0SuperClusterProducer : public edm::stream::EDProducer<> {
 public:
  explicit ICPi0SuperClusterProducer(const edm::ParameterSet&);
  ~ICPi0SuperClusterProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  std::vector<ic::SuperCluster>* scs_;
  edm::InputTag input_;
  edm::InputTag input_EERecHits_;
  edm::InputTag input_EBRecHits_;
  edm::InputTag input_taus_;
  std::string branch_;

  #if (CMSSW_MAJOR_VERSION >= 9 && CMSSW_MINOR_VERSION >= 4 && CMSSW_REVISION > 4) || CMSSW_MAJOR_VERSION >= 10
  edm::ESHandle<CaloTopology> theCaloTopo_;
  #endif

  boost::hash<reco::SuperCluster const*> sc_hasher_;
};

#endif
