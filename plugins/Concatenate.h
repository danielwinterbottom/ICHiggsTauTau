#ifndef UserCode_ICHiggsTauTau_Concatenate_h
#define UserCode_ICHiggsTauTau_Concatenate_h

#include <vector>
#include <set>
#include <memory>
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

namespace ic {

template <class T>
class Concatenate : public edm::EDProducer {
 public:
  explicit Concatenate(const edm::ParameterSet &);
  ~Concatenate();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  typedef std::vector<T> Vec;
  typedef std::vector<edm::InputTag> VInputTag;

  std::vector<edm::InputTag> inputs_;
};

// =============================
// Template class implementation
// =============================
template <class T>
Concatenate<T>::Concatenate(const edm::ParameterSet &config)
    : inputs_(config.getParameter<VInputTag>("concatenate")) {
  for(unsigned i = 0; i < inputs_.size(); ++i) {
    consumes<Vec>(inputs_[i]);
  }
  produces<Vec>();
}

template <class T>
Concatenate<T>::~Concatenate() {}

template <class T>
void Concatenate<T>::beginJob() {}

template <class T>
void Concatenate<T>::endJob() {}

template <class T>
void Concatenate<T>::produce(edm::Event &event,
                                    const edm::EventSetup &setup) {
  std::auto_ptr<Vec> product(new Vec());
  for (unsigned i = 0; i < inputs_.size(); ++i) {
    edm::Handle<Vec> vec;
    event.getByLabel(inputs_[i], vec);
    for (unsigned j = 0; j < vec->size(); ++j) {
      product->push_back(vec->at(j));
    }
  }
  event.put(product);
}
}

#endif
