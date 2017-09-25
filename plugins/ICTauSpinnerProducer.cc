#include "UserCode/ICHiggsTauTau/plugins/ICTauSpinnerProducer.hh"
#include <memory>
#include <string>
#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"


ICTauSpinnerProducer::ICTauSpinnerProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")) {
  consumes<edm::View<reco::Candidate>>(input_);

  PrintHeaderWithProduces(config, input_, branch_);
  TauSpinnerSettingsPDF="";
  TauSpinnerSettingsIpp=true;
  TauSpinnerSettingsIpol=0;
  TauSpinnerSettingsNonSM2=0;
  TauSpinnerSettingsNonSMN=0;
  TauSpinnerSettingsCmsEnergy=13.;
}

ICTauSpinnerProducer::~ICTauSpinnerProducer() {}



void ICTauSpinnerProducer::Init(){
  Tauolapp::Tauola::initialize();
  LHAPDF::initPDFSetByName(TauSpinnerSettingsPDF);
  
  TauSpinner::initialize_spinner(TauSpinnerSettingsIpp,
                                 TauSpinnerSettingsIpol,
                                 TauSpinnerSettingsNonSM2,
                                 TauSpinnerSettingsNonSMN,
                                 TauSpinnerSettingsCmsEnergy);
}

void ICTauSpinnerProducer::produce(edm::Event& event,
                                  const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::Candidate> > cand_handle;
  event.getByLabel(input_, cand_handle);

}

void ICTauSpinnerProducer::beginJob() {
  //ic::StaticTree::tree_->Branch(branch_.c_str(), &candidates_);
}

void ICTauSpinnerProducer::endJob() {}

DEFINE_FWK_MODULE(ICTauSpinnerProducer);
