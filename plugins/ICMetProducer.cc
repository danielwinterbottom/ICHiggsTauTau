#include "UserCode/ICHiggsTauTau/plugins/ICMetProducer.hh"
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
#include "DataFormats/PatCandidates/interface/MET.h"
#include "AnalysisDataFormats/TauAnalysis/interface/PFMEtSignCovMatrix.h"

#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"

#include "UserCode/ICHiggsTauTau/interface/city.h"

#include "TVector.h"

#include "boost/format.hpp"


ICMetProducer::ICMetProducer(const edm::ParameterSet& iConfig) {
  add_gen_ = iConfig.getUntrackedParameter<bool>("addGen");
  input_label_ = iConfig.getParameter<edm::InputTag>("inputLabel");
  branch_name_ = iConfig.getUntrackedParameter<std::string>("branchName");
  input_sig_ = iConfig.getUntrackedParameter<std::string>("InputSig");

  met_ = new ic::Met();
  if (add_gen_) {
    met_gen_ = new ic::Met();  
  } else {
    met_gen_ = NULL;
  }
}


ICMetProducer::~ICMetProducer() {
 // do anything here that needs to be done at desctruction time
 // (e.g. close files, deallocate resources etc.)
  delete met_;
  if (met_gen_) delete met_gen_;
}



// ------------ method called to produce the data  ------------
void ICMetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  *met_ = ic::Met();
  edm::Handle<std::vector<pat::MET> > met_handle; 
  iEvent.getByLabel(input_label_,met_handle);
  pat::MET const& ref = met_handle->at(0);
  met_->set_pt(ref.pt());
  met_->set_eta(ref.eta());
  met_->set_phi(ref.phi());
  met_->set_energy(ref.energy());
  met_->set_sum_et(ref.sumEt());
  met_->set_et_sig(ref.mEtSig());
  met_->set_id(0);
  TMatrixD sig_matrix = ref.getSignificanceMatrix();
  if (input_sig_ != "") {
    edm::Handle<PFMEtSignCovMatrix> sig_handle;
    iEvent.getByLabel(input_sig_,sig_handle);
    met_->set_xx_sig((*sig_handle)(0,0));
    //std::cout << (*sig_handle)(0,0) << std::endl;
    met_->set_xy_sig((*sig_handle)(0,1));
    met_->set_yx_sig((*sig_handle)(1,0));
    met_->set_yy_sig((*sig_handle)(1,1));
  } else {
    met_->set_xx_sig(sig_matrix(0,0));
    met_->set_xy_sig(sig_matrix(0,1));
    met_->set_yx_sig(sig_matrix(1,0));
    met_->set_yy_sig(sig_matrix(1,1));
  }

  if (add_gen_) {
    reco::GenMET const *genptr = ref.genMET();
    *met_gen_ = ic::Met();
    if (genptr) {
      met_gen_->set_pt(genptr->pt());
      met_gen_->set_eta(genptr->eta());
      met_gen_->set_phi(genptr->phi());
      met_gen_->set_energy(genptr->energy());
      met_gen_->set_sum_et(genptr->sumEt());
      met_gen_->set_et_sig(genptr->mEtSig());
      TMatrixD sig_matrix_gen = genptr->getSignificanceMatrix();
      met_gen_->set_xx_sig(sig_matrix_gen(0,0));
      met_gen_->set_xy_sig(sig_matrix_gen(0,1));
      met_gen_->set_yx_sig(sig_matrix_gen(1,0));
      met_gen_->set_yy_sig(sig_matrix_gen(1,1));
      met_gen_->set_id(0);
    }
  }
}

// ------------ method called once each job just before starting event loop  ------------
void ICMetProducer::beginJob() {
 ic::StaticTree::tree_->Branch(branch_name_.c_str(),&met_);
 if (add_gen_) {
    ic::StaticTree::tree_->Branch((branch_name_+"Gen").c_str(),&met_gen_);
 }
}

// ------------ method called once each job just after ending the event loop  ------------
void ICMetProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void ICMetProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICMetProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICMetProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICMetProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICMetProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICMetProducer);
