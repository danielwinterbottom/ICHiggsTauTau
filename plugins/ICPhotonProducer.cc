#include "UserCode/ICHiggsTauTau/plugins/ICPhotonProducer.hh"
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
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"

#include "boost/format.hpp"


ICPhotonProducer::ICPhotonProducer(const edm::ParameterSet& iConfig) {
  input_ = iConfig.getParameter<edm::InputTag>("input");
  branch_name_ = iConfig.getParameter<std::string>("branchName");
  min_pt_ = iConfig.getParameter<double>("minPt");
  max_eta_ = iConfig.getParameter<double>("maxEta");
  isolator_ = new PFIsolationEstimator();
  isolator_->initializePhotonIsolation(true);
  isolator_->setConeSize(0.3);
  photons_ = new std::vector<ic::Photon>();
}

    
ICPhotonProducer::~ICPhotonProducer() {
  delete photons_;
  delete isolator_;
}

// ------------ method called to produce the data  ------------
void ICPhotonProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  boost::hash<reco::Photon const*> photon_hasher;

  // Get input collection
  edm::Handle<std::vector<reco::Photon> > photons_input;

  iEvent.getByLabel(input_, photons_input);
  std::vector<reco::Photon>::const_iterator iter;

  // Get other inputs
  edm::Handle<reco::BeamSpot> beamspot;
  iEvent.getByLabel(edm::InputTag("offlineBeamSpot"), beamspot);
  edm::Handle<reco::ConversionCollection> hConversions;
  iEvent.getByLabel("allConversions", hConversions);
  edm::Handle<reco::GsfElectronCollection> hElectrons;
  iEvent.getByLabel("gsfElectrons", hElectrons);
  Handle<reco::PFCandidateCollection> pfCandidatesH;
  iEvent.getByLabel("particleFlow", pfCandidatesH);
  edm::Handle<reco::VertexCollection> vertexCollection;
  iEvent.getByLabel("offlinePrimaryVertices", vertexCollection);


  // Prepare output collection
  photons_->resize(0);
  photons_->reserve(photons_input->size());

  
  unsigned idx = 0;
  for (iter = photons_input->begin(); iter != photons_input->end(); ++iter, ++idx) {

    if (! (iter->pt() >= min_pt_ && fabs(iter->eta()) <= max_eta_) ) continue;

    photons_->push_back(ic::Photon());
    ic::Photon & pho = photons_->back();

    pho.set_id(photon_hasher(&(*iter)));
    pho.set_pt(iter->pt());
    pho.set_eta(iter->eta());
    pho.set_phi(iter->phi());
    pho.set_energy(iter->energy());
    pho.set_charge(iter->charge());

    isolator_->fGetIsolation(&(*iter), &(*pfCandidatesH), reco::VertexRef(vertexCollection,0), vertexCollection);

    pho.set_dr03_pfiso_charged(isolator_->getIsolationCharged());
    pho.set_dr03_pfiso_gamma(isolator_->getIsolationPhoton());
    pho.set_dr03_pfiso_neutral(isolator_->getIsolationNeutral());

    pho.set_pass_electron_veto(!ConversionTools::hasMatchedPromptElectron(iter->superCluster(), hElectrons, hConversions, beamspot->position()));
    pho.set_sigma_IetaIeta(iter->sigmaIetaIeta());
#ifndef CMSSW_4_2_8_patch7
    pho.set_had_tower_over_em(iter->hadTowOverEm());
#else
    pho.set_had_tower_over_em(0.0);
#endif
  }  
}

// ------------ method called once each job just before starting event loop  ------------
void ICPhotonProducer::beginJob() {
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Info in <ICPhotonProducer>: BeginJob Summary for input: " << input_.label() << std::endl;
  std::cout << "-----------------------------------------------------------------" << std::endl;
  ic::StaticTree::tree_->Branch(branch_name_.c_str(), &photons_);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICPhotonProducer::endJob() {
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Info in <ICPhotonProducer>: EndJob Summary for input: " << input_.label() << std::endl;
  std::cout << "-----------------------------------------------------------------" << std::endl;
}

// ------------ method called when starting to processes a run  ------------
void ICPhotonProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICPhotonProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICPhotonProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICPhotonProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICPhotonProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


//define this as a plug-in
DEFINE_FWK_MODULE(ICPhotonProducer);
