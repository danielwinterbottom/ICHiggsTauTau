#include "UserCode/ICHiggsTauTau/plugins/ICMuonIsolation.h"
#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

ICMuonIsolation::ICMuonIsolation(
    const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      input_isolation_type_(config.getParameter<std::string>("iso_type")) {
  produces<edm::ValueMap<double> >();
}

ICMuonIsolation::~ICMuonIsolation() {}

void ICMuonIsolation::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {
  std::auto_ptr<edm::ValueMap<double> > product(new edm::ValueMap<double>());
  edm::Handle <pat::MuonCollection> muons_handle;
  event.getByLabel(input_, muons_handle);

  std::vector<double> values(muons_handle->size(), 0);
  for (unsigned i = 0; i < muons_handle->size(); ++i) {
    pat::Muon const& src = muons_handle->at(i);
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
  filler.insert(muons_handle, values.begin(), values.end());
  filler.fill();

  event.put(product);
}

void ICMuonIsolation::beginJob() {}

void ICMuonIsolation::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICMuonIsolation);
