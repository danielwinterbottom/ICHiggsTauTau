#include "UserCode/ICHiggsTauTau/plugins/ICElectronHcalDepthCalculator.h"
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
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDefs.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaTowerIsolation.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

ICElectronHcalDepthCalculator::ICElectronHcalDepthCalculator(
    const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")) {
  consumes<reco::GsfElectronCollection>(input_);
  consumes<CaloTowerCollection>({"towerMaker"}); 
  produces<edm::ValueMap<float> >();
}

ICElectronHcalDepthCalculator::~ICElectronHcalDepthCalculator() {}

void ICElectronHcalDepthCalculator::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {
  std::unique_ptr<edm::ValueMap<float> > product(new edm::ValueMap<float>());
  edm::Handle<reco::GsfElectronCollection> elecs_handle;
  event.getByLabel(input_, elecs_handle);

  edm::Handle<CaloTowerCollection> tower_handle;
  event.getByLabel("towerMaker", tower_handle);
  EgammaTowerIsolation had_depth1_iso03(0.3, 0.0, 0.0, 1,
                                        tower_handle.product());
  EgammaTowerIsolation had_depth2_iso03(0.3, 0.0, 0.0, 2,
                                        tower_handle.product());

  std::vector<float> values(elecs_handle->size(), 0.);
  for (unsigned i = 0; i < elecs_handle->size(); ++i) {
    reco::GsfElectron const& src = elecs_handle->at(i);
    float had_depth1_sum_et_03 = had_depth1_iso03.getTowerEtSum(&src);
    float had_depth2_sum_et_03 = had_depth2_iso03.getTowerEtSum(&src);
    values[i] = had_depth1_sum_et_03 + had_depth2_sum_et_03;
  }

  edm::ValueMap<float>::Filler filler(*product);
  filler.insert(elecs_handle, values.begin(), values.end());
  filler.fill();

  event.put(std::move(product));
}

void ICElectronHcalDepthCalculator::beginJob() {}

void ICElectronHcalDepthCalculator::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICElectronHcalDepthCalculator);
