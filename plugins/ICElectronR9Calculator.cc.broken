#include "UserCode/ICHiggsTauTau/plugins/ICElectronR9Calculator.h"
#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"


ICElectronR9Calculator::ICElectronR9Calculator(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")) {
  produces<edm::ValueMap<float> >();
}

ICElectronR9Calculator::~ICElectronR9Calculator() {}

void ICElectronR9Calculator::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {
  std::auto_ptr<edm::ValueMap<float> > product(new edm::ValueMap<float>());
  edm::Handle<reco::GsfElectronCollection> elecs_handle;
  event.getByLabel(input_, elecs_handle);

  EcalClusterLazyTools lazy_tool(event, setup,
                                edm::InputTag("reducedEcalRecHitsEB"),
                                edm::InputTag("reducedEcalRecHitsEE"));

  std::vector<float> values(elecs_handle->size(), 0.);
  for (unsigned i = 0; i < elecs_handle->size(); ++i) {
    reco::GsfElectron const& src = elecs_handle->at(i);
    float e3x3 = lazy_tool.e3x3(*(src.superCluster()));
    float sc_raw_energy = src.superCluster()->rawEnergy();
    values[i] = e3x3 / sc_raw_energy;
  }

  edm::ValueMap<float>::Filler filler(*product);
  filler.insert(elecs_handle, values.begin(), values.end());
  filler.fill();

  event.put(product);
}

void ICElectronR9Calculator::beginJob() {}

void ICElectronR9Calculator::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICElectronR9Calculator);
