#include "UserCode/ICHiggsTauTau/plugins/ICPileupInfoProducer.hh"
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
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 

#include "UserCode/ICHiggsTauTau/interface/PileupInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"

#include "UserCode/ICHiggsTauTau/interface/city.h"

#include "boost/format.hpp"


ICPileupInfoProducer::ICPileupInfoProducer(const edm::ParameterSet& iConfig) {
  info_ = new std::vector<ic::PileupInfo>();
}


ICPileupInfoProducer::~ICPileupInfoProducer() {
 // do anything here that needs to be done at desctruction time
 // (e.g. close files, deallocate resources etc.)
 delete info_;
}



// ------------ method called to produce the data  ------------
void ICPileupInfoProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<std::vector<PileupSummaryInfo> > pu_info;
  iEvent.getByLabel("addPileupInfo",pu_info);
  info_->resize(0);
  info_->reserve(pu_info->size());
  for (unsigned i = 0; i < pu_info->size(); ++i) {
    info_->push_back(ic::PileupInfo());
    ic::PileupInfo & info = info_->back();
    info.set_num_interactions(pu_info->at(i).getPU_NumInteractions());
    info.set_bunch_crossing(pu_info->at(i).getBunchCrossing());
    // info.set_z_positions(pu_info->at(i).getPU_zpositions());
    info.set_true_num_interactions(pu_info->at(i).getTrueNumInteractions());
  }
}

// ------------ method called once each job just before starting event loop  ------------
void ICPileupInfoProducer::beginJob() {
 ic::StaticTree::tree_->Branch("pileupInfo",&info_);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICPileupInfoProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void ICPileupInfoProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICPileupInfoProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICPileupInfoProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICPileupInfoProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICPileupInfoProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICPileupInfoProducer);
