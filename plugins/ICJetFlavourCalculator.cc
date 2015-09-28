#include "UserCode/ICHiggsTauTau/plugins/ICJetFlavourCalculator.h"
#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfoMatching.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"

ICJetFlavourCalculator::ICJetFlavourCalculator(
    const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      input_jet_flavour_(config.getParameter<edm::InputTag>("flavourMap")) {
  produces<edm::ValueMap<int> >();
}

ICJetFlavourCalculator::~ICJetFlavourCalculator() {}

void ICJetFlavourCalculator::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {
  std::auto_ptr<edm::ValueMap<int> > product(new edm::ValueMap<int>());
  edm::Handle <edm::View <reco::Jet> > jets_handle;
  event.getByLabel(input_, jets_handle);

#if CMSSW_MAJOR_VERSION>=7 && CMSSW_MINOR_VERSION >=4 
  edm::Handle<reco::JetFlavourInfoMatchingCollection> flavour_handle;
#else
  edm::Handle<reco::JetFlavourMatchingCollection> flavour_handle;
#endif
  event.getByLabel(input_jet_flavour_, flavour_handle);

  std::vector<int> values(jets_handle->size(), 0);
  for (unsigned i = 0; i < jets_handle->size(); ++i) {
#if CMSSW_MAJOR_VERSION>=7 && CMSSW_MINOR_VERSION>=4
    values[i] = (*flavour_handle)[jets_handle->refAt(i)].getPartonFlavour();
#else
    values[i] = (*flavour_handle)[jets_handle->refAt(i)].getFlavour();
#endif
  }

  edm::ValueMap<int>::Filler filler(*product);
  filler.insert(jets_handle, values.begin(), values.end());
  filler.fill();

  event.put(product);
}

void ICJetFlavourCalculator::beginJob() {}

void ICJetFlavourCalculator::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICJetFlavourCalculator);
