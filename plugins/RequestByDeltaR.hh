#ifndef UserCode_ICHiggsTauTau_RequestByDeltaR_h
#define UserCode_ICHiggsTauTau_RequestByDeltaR_h

#include <memory>
#include <vector>
#include "boost/functional/hash.hpp"
#include "boost/format.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

template <class T>
class RequestByDeltaR : public edm::EDProducer {
 public:
  explicit RequestByDeltaR(const edm::ParameterSet &);
  ~RequestByDeltaR();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  edm::InputTag input_;
  edm::InputTag reference_;
  double dr_;

  typedef std::vector<T> Vec;
  typedef edm::RefVector<Vec> RefVectorVec;
  typedef edm::Ref<Vec> RefVec;
};

// =============================
// Template class implementation
// =============================
template <class T>
RequestByDeltaR<T>::RequestByDeltaR(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("src")),
      reference_(config.getParameter<edm::InputTag>("reference")),
      dr_(config.getParameter<double>("deltaR")) {
  consumes<edm::View<T> >(input_);
  consumes<edm::View<reco::Candidate> >(reference_);
  produces<RefVectorVec>();
  PrintHeaderWithProduces(config, input_, "");
  std::cout << boost::format("%-15s : %-60s\n") % "Reference" %
                   reference_.encode();
  std::cout << boost::format("%-15s : %-60s\n") % "DeltaR" % dr_;
}

template <class T>
RequestByDeltaR<T>::~RequestByDeltaR() { }

// =============
// Main producer
// =============
template <class T>
void RequestByDeltaR<T>::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {

  edm::Handle<edm::View<reco::Candidate> > ref_handle;
  event.getByLabel(reference_, ref_handle);
  edm::Handle<edm::View<T> > in_handle;
  event.getByLabel(input_, in_handle);

  std::unique_ptr<RefVectorVec> product(new RefVectorVec());

  for (unsigned i = 0; i < in_handle->size(); ++i) {
    for (unsigned j = 0; j < ref_handle->size(); ++j) {
      double dr = reco::deltaR(in_handle->at(i), ref_handle->at(j));
      if (dr <= dr_) {
        product->push_back(in_handle->refAt(i).template castTo<RefVec>());
        break;
      }
    }
  }
  event.put(std::move(product));
}

template <class T>
void RequestByDeltaR<T>::beginJob() {}

template <class T>
void RequestByDeltaR<T>::endJob() {}

#endif
