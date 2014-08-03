#ifndef UserCode_ICHiggsTauTau_ICMetProducer_h
#define UserCode_ICHiggsTauTau_ICMetProducer_h

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
 * @brief Produces an ic::Met collection from any input compatible with
 *an edm::View<reco::MET>
 *
 * **Example usage**
 * @snippet python/default_producers_cfi.py Met
 */
class ICMetProducer : public edm::EDProducer {
 public:
  explicit ICMetProducer(const edm::ParameterSet&);
  ~ICMetProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  std::vector<ic::Met>* met_;
  edm::InputTag input_;
  std::string branch_;
  boost::hash<reco::MET const*> met_hasher_;
  bool do_custom_id_;
  edm::InputTag inputID_;
};

#endif
