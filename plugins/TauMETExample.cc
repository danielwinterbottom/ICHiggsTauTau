#include "UserCode/ICHiggsTauTau/plugins/TauMETExample.hh"
#include <boost/functional/hash.hpp>
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"

TauMETExample::TauMETExample(const edm::ParameterSet& iConfig) {

}


TauMETExample::~TauMETExample() {

}



// ------------ method called to produce the data  ------------
void TauMETExample::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
 
  // Load the tau collection
  edm::Handle<std::vector<reco::PFTau> > tauCollection;
  iEvent.getByLabel("hpsPFTauProducer",tauCollection);

  // How to load different discriminators
  edm::Handle<reco::PFTauDiscriminator> ByDecayModeFinding;
  iEvent.getByLabel("hpsPFTauDiscriminationByDecayModeFinding", ByDecayModeFinding);

  edm::Handle<reco::PFTauDiscriminator> ByLooseIsolationMVA;
  iEvent.getByLabel("hpsPFTauDiscriminationByLooseIsolationMVA", ByLooseIsolationMVA);

  edm::Handle<reco::PFTauDiscriminator> ByMVAElectronRejection;
  iEvent.getByLabel("hpsPFTauDiscriminationByMVAElectronRejection", ByMVAElectronRejection);

  // Example, loop through taus and check decayModeFinding
  for ( unsigned i = 0; i < tauCollection->size(); ++i ) {
      reco::PFTauRef tau(tauCollection, i);
      std::cout << "Tau with pT " << tau->pt() << " has decayModeFinding: " << (*ByDecayModeFinding)[tau] << std::endl;
  }


  // Load the type-1 corrected MET
  edm::Handle<std::vector<reco::PFMET> > metCollection;
  iEvent.getByLabel("pfType1CorrectedMet",metCollection);
  std::cout << "MET has Et: " << metCollection->at(0).pt() << std::endl;


}

// ------------ method called once each job just before starting event loop  ------------
void TauMETExample::beginJob() {

}

// ------------ method called once each job just after ending the event loop  ------------
void TauMETExample::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void TauMETExample::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void TauMETExample::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void TauMETExample::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void TauMETExample::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TauMETExample::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TauMETExample);
