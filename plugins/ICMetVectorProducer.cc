#include "UserCode/ICHiggsTauTau/plugins/ICMetVectorProducer.hh"
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
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"

#include "UserCode/ICHiggsTauTau/interface/city.h"

#include "TVector.h"

#include "boost/format.hpp"


ICMetVectorProducer::ICMetVectorProducer(const edm::ParameterSet& iConfig) {
  merge_labels_ = iConfig.getUntrackedParameter<std::vector<std::string> >("mergeLabels");
  branch_name_ = iConfig.getUntrackedParameter<std::string>("branchName");
  met_ = new std::vector<ic::Met>();
}


ICMetVectorProducer::~ICMetVectorProducer() {
 // do anything here that needs to be done at desctruction time
 // (e.g. close files, deallocate resources etc.)
  delete met_;
}



// ------------ method called to produce the data  ------------
void ICMetVectorProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  met_->resize(0);
  for (unsigned i = 0; i < merge_labels_.size(); ++i) {
    edm::Handle<reco::PFMETCollection> met_handle;
    edm::Handle<std::vector<std::size_t> > id_handle; 
    iEvent.getByLabel(merge_labels_[i], "MVAMetId", id_handle);
    iEvent.getByLabel(merge_labels_[i],met_handle);
    std::vector<reco::PFMET>::const_iterator iter;

    for (iter = met_handle->begin(); iter != met_handle->end(); ++iter) {
      met_->push_back(ic::Met());
      ic::Met & met = met_->back();
      met.set_pt(iter->pt());
      met.set_eta(iter->eta());
      met.set_phi(iter->phi());
      met.set_energy(iter->energy());
      met.set_sum_et(iter->sumEt());
      met.set_et_sig(iter->mEtSig());
      if (id_handle->size() == met_handle->size()) {
        met.set_id(id_handle->at(unsigned(iter - met_handle->begin())));
      } else {
        met.set_id(0);      
      }
      TMatrixD sig_matrix = iter->getSignificanceMatrix();
      met.set_xx_sig(sig_matrix(0,0));
      met.set_xy_sig(sig_matrix(0,1));
      met.set_yx_sig(sig_matrix(1,0));
      met.set_yy_sig(sig_matrix(1,1));
    }
  }
}

// ------------ method called once each job just before starting event loop  ------------
void ICMetVectorProducer::beginJob() {
 ic::StaticTree::tree_->Branch(branch_name_.c_str(),&met_);

}

// ------------ method called once each job just after ending the event loop  ------------
void ICMetVectorProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void ICMetVectorProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICMetVectorProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICMetVectorProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICMetVectorProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICMetVectorProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICMetVectorProducer);
