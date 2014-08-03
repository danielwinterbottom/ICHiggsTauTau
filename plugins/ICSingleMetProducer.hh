#ifndef UserCode_ICHiggsTauTau_ICSingleMetProducer_h
#define UserCode_ICHiggsTauTau_ICSingleMetProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/METReco/interface/MET.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"

/**
 * @brief Produces a single ic::Met object from the first entry in any input
 *collection compatible with an edm::View<reco::MET>
 *
 * **Example usage**
 * @snippet python/default_producers_cfi.py SingleMet
 */
class ICSingleMetProducer : public edm::EDProducer {
 public:
  explicit ICSingleMetProducer(const edm::ParameterSet&);
  ~ICSingleMetProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  ic::Met* met_;
  edm::InputTag input_;
  std::string branch_;
  boost::hash<reco::MET const*> met_hasher_;
};

#endif
