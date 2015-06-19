#include "UserCode/ICHiggsTauTau/plugins/ICElectronIsolation.h"
#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

ICElectronIsolation::ICElectronIsolation(
    const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      input_isolation_type_(config.getParameter<std::string>("iso_type")) {
  produces<edm::ValueMap<double> >();
}

ICElectronIsolation::~ICElectronIsolation() {}

void ICElectronIsolation::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {
  std::auto_ptr<edm::ValueMap<double> > product(new edm::ValueMap<double>());
  edm::Handle <pat::ElectronCollection> elecs_handle;
  event.getByLabel(input_, elecs_handle);

  std::vector<double> values(elecs_handle->size(), 0);
  for (unsigned i = 0; i < elecs_handle->size(); ++i) {
    pat::Electron const& src = elecs_handle->at(i);
    if(input_isolation_type_=="charged_iso"){
      values[i] = src.chargedHadronIso();
    } else if(input_isolation_type_ == "photon_iso"){
      values[i] = src.photonIso();
    } else if(input_isolation_type_ == "neutral_iso"){
      values[i] = src.neutralHadronIso();
    } else if(input_isolation_type_ == "pu_iso"){
      values[i] = src.puChargedHadronIso();
//    } else if(input_isolation_type_ == "allcharged_iso"){
 //     values[i] = src.chargedIso();
    } else values[i] = 0;

  }

  edm::ValueMap<double>::Filler filler(*product);
  filler.insert(elecs_handle, values.begin(), values.end());
  filler.fill();

  event.put(product);
}

void ICElectronIsolation::beginJob() {}

void ICElectronIsolation::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICElectronIsolation);
