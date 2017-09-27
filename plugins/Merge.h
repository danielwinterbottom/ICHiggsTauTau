#ifndef UserCode_ICHiggsTauTau_Merge_h
#define UserCode_ICHiggsTauTau_Merge_h

#include <vector>
#include <set>
#include <memory>
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefVector.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

namespace ic {

/**
 * @brief %Merge several collections into one, removing any duplicates in
 *the process.
 * @details The input collections (containing or referencing objects of type `T`)
 *are specified by the *merge* parameter, which takes a vector of `edm::InputTag`
 *objects. The objects in each collection must all originate from the same
 *parent collection. Different input collection types may freely be combined,
 *provided each can be accessed as an `edm::View` into the type `T`. The merged
 *collection produced by this module is of type `edm::RefVector<T>`. This template
 *class must be explicitly defined as a CMSSW module for each desired input type
 *`T`. Check the implementation file Merge.cc to see if such a module has already
 *been defined for the type you need. If not, the appropriate typedef and module
 *definition should be added, following the naming convention IC<object>Merger.
 *
 * @tparam T The type of object contained or referenced in each input
 *collection.
 *
 * **Example usage** (where `T = reco::Track`)
 * ~~~~~~~~~{.py}
 * process.mergedTracks = cms.EDProducer("ICTrackMerger",
 *   merge = cms.VInputTag("electronTrks", "muonTrks", "jetTrks")
 * )
 * ~~~~~~~~~
 */
template <class T>
class Merge : public edm::EDProducer {
 public:
  explicit Merge(const edm::ParameterSet &);
  ~Merge();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  typedef std::vector<T> Vec;
  typedef edm::RefVector<Vec> RefVectorVec;
  typedef edm::Ref<Vec> RefVec;
  typedef std::vector<edm::InputTag> VInputTag;

  std::vector<edm::InputTag> merge_inputs_;
};

// =============================
// Template class implementation
// =============================
template <class T>
Merge<T>::Merge(const edm::ParameterSet &config)
    : merge_inputs_(config.getParameter<VInputTag>("merge")) {
  for( unsigned i = 0; i < merge_inputs_.size(); ++i){
    consumes<edm::View<T>>(merge_inputs_[i]);
  }
  produces<RefVectorVec>();
}

template <class T>
Merge<T>::~Merge() {}

template <class T>
void Merge<T>::beginJob() {}

template <class T>
void Merge<T>::endJob() {}

template <class T>
void Merge<T>::produce(edm::Event &event,
                                    const edm::EventSetup &setup) {
  std::unique_ptr<RefVectorVec> product(new RefVectorVec());
  std::set<T const *> ptr_set;
  for (unsigned i = 0; i < merge_inputs_.size(); ++i) {
    edm::Handle<edm::View<T> > view;
    event.getByLabel(merge_inputs_[i], view);
    for (unsigned j = 0; j < view->size(); ++j) {
      if (!ptr_set.count(&(view->at(j)))) {
        product->push_back(view->refAt(j).template castTo<RefVec>());
        ptr_set.insert(&(view->at(j)));
      }
    }
  }
  event.put(std::move(product));
}
}

#endif
