#include "UserCode/ICHiggsTauTau/plugins/ICEventInfoProducer.hh"
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
#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"

#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"

#include "UserCode/ICHiggsTauTau/interface/city.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "boost/format.hpp"


ICEventInfoProducer::ICEventInfoProducer(const edm::ParameterSet& iConfig) {
  rho_jets_name_ = iConfig.getParameter<std::string>("RhoJetsName");
  pfnopu_rho_jets_name_ = iConfig.getParameter<std::string>("PFNoPURhoJetsName");

  vertex_name_ = iConfig.getParameter<std::string>("vertexLabel");

  //embed_weight_ = iConfig.getParameter<std::string>("EmbedWeight");

  info_ = new ic::EventInfo();
}


ICEventInfoProducer::~ICEventInfoProducer() {
 // do anything here that needs to be done at desctruction time
 // (e.g. close files, deallocate resources etc.)
 delete info_;
}



// ------------ method called to produce the data  ------------
void ICEventInfoProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  *info_ = ic::EventInfo();
  info_->set_is_data(iEvent.isRealData());
  info_->set_run(iEvent.run());
  info_->set_event(iEvent.id().event());
  info_->set_lumi_block(iEvent.luminosityBlock());
  info_->set_bunch_crossing(iEvent.bunchCrossing());

  edm::Handle<double> rho_handle;
  iEvent.getByLabel(edm::InputTag(rho_jets_name_,"rho"),rho_handle);
  edm::Handle<double> pfnopu_rho_handle;
  iEvent.getByLabel(edm::InputTag(pfnopu_rho_jets_name_,"rho"),pfnopu_rho_handle);

  edm::Handle<double> basic_weight_handle;
  edm::Handle<GenFilterInfo> info_weight_handle;
  // For 42X samples
  if (iEvent.getByLabel(edm::InputTag("generator","weight","EmbeddedRECO"),basic_weight_handle))
    info_->set_weight("embed_weight", *basic_weight_handle);
  // For 52X samples
  if (iEvent.getByLabel(edm::InputTag("generator","minVisPtFilter","EmbeddedRECO"),info_weight_handle))
    info_->set_weight("embed_weight", info_weight_handle->filterEfficiency());

  edm::Handle<reco::VertexCollection> vertexCollection;
  iEvent.getByLabel(vertex_name_,vertexCollection);

  info_->set_good_vertices(vertexCollection->size());

  info_->set_rho(*rho_handle);
  info_->set_pfnopu_rho(*pfnopu_rho_handle);
}

// ------------ method called once each job just before starting event loop  ------------
void ICEventInfoProducer::beginJob() {
 ic::StaticTree::tree_->Branch("eventInfo",&info_);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICEventInfoProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void ICEventInfoProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICEventInfoProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICEventInfoProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICEventInfoProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICEventInfoProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICEventInfoProducer);
