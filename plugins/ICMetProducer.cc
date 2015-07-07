#include "UserCode/ICHiggsTauTau/plugins/ICMetProducer.hh"
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
#include "DataFormats/METReco/interface/MET.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"

ICMetProducer::SigTags::SigTags(edm::ParameterSet const& pset)
  : metsig(pset.getParameter<edm::InputTag>("metsig")),
    metsigcov00(pset.getParameter<edm::InputTag>("metsigcov00")),
    metsigcov01(pset.getParameter<edm::InputTag>("metsigcov01")),
    metsigcov10(pset.getParameter<edm::InputTag>("metsigcov10")),
    metsigcov11(pset.getParameter<edm::InputTag>("metsigcov11")) {}

ICMetProducer::SigTagsMethod2::SigTagsMethod2(edm::ParameterSet const& pset)
  : metsig(pset.getParameter<edm::InputTag>("metsig")),
    metsigcov(pset.getParameter<edm::InputTag>("metsigcov")) {}


ICMetProducer::ICMetProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      do_custom_id_(config.getParameter<bool>("includeCustomID")),
      inputID_(config.getParameter<edm::InputTag>("inputCustomID")),
      do_external_metsig_(config.getParameter<bool>("includeExternalMetsig")),
      do_external_metsig_method2_(config.getParameter<bool>("includeExternalMetsigMethod2")),
      metsig_(config.getParameterSet("metsig")),
      metsig_method2_(config.getParameterSet("metsig_method2")) {
  met_ = new std::vector<ic::Met>();
  PrintHeaderWithProduces(config, input_, branch_);
  PrintOptional(1, do_custom_id_, "includeCustomID");
}

ICMetProducer::~ICMetProducer() {
  delete met_;
}

void ICMetProducer::produce(edm::Event& event, const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::MET> > mets_handle;
  event.getByLabel(input_, mets_handle);
  edm::Handle<std::vector<std::size_t> > id_handle;
  if (do_custom_id_) {
    event.getByLabel(inputID_, id_handle);
  }
  // event.getByLabel(merge_labels_[i], "MVAMetId", id_handle);

  met_->clear();
  met_->resize(mets_handle->size(), ic::Met());

  for (unsigned i = 0; i < mets_handle->size(); ++i) {
    reco::MET const& src = mets_handle->at(i);
    ic::Met& dest = met_->at(i);
    if (do_custom_id_) {
      dest.set_id(id_handle->at(i));
    } else {
      dest.set_id(met_hasher_(&src));
    }
    dest.set_pt(src.pt());
    dest.set_eta(src.eta());
    dest.set_phi(src.phi());
    dest.set_energy(src.energy());
    dest.set_sum_et(src.sumEt());
    if(!do_external_metsig_&&!do_external_metsig_method2_){
      dest.set_et_sig(src.mEtSig());
      dest.set_xx_sig(src.getSignificanceMatrix()(0, 0));
      dest.set_xy_sig(src.getSignificanceMatrix()(0, 1));
      dest.set_yx_sig(src.getSignificanceMatrix()(1, 0));
      dest.set_yy_sig(src.getSignificanceMatrix()(1, 1));
    }
    else{
      if(!do_external_metsig_method2_){
	edm::Handle<double> metsig;
	edm::Handle<double> metsigcov00;
	edm::Handle<double> metsigcov01;
	edm::Handle<double> metsigcov10;
	edm::Handle<double> metsigcov11;
	event.getByLabel(metsig_.metsig, metsig);
	event.getByLabel(metsig_.metsigcov00, metsigcov00);
	event.getByLabel(metsig_.metsigcov01, metsigcov01);
	event.getByLabel(metsig_.metsigcov10, metsigcov10);
	event.getByLabel(metsig_.metsigcov11, metsigcov11);
	dest.set_et_sig(*metsig);
	dest.set_xx_sig(*metsigcov00);
	dest.set_xy_sig(*metsigcov01);
	dest.set_yx_sig(*metsigcov10);
	dest.set_yy_sig(*metsigcov11);
      }
      else{
	edm::Handle<double> metsig;
	edm::Handle<math::Error<2>::type> metsigcov;
	event.getByLabel(metsig_method2_.metsig, metsig);
	event.getByLabel(metsig_method2_.metsigcov, metsigcov);
	dest.set_et_sig(*metsig);
	dest.set_xx_sig((*metsigcov)(0,0));
	dest.set_xy_sig((*metsigcov)(0,1));
	dest.set_yx_sig((*metsigcov)(1,0));
	dest.set_yy_sig((*metsigcov)(1,1));
      }
    }
  }
}

void ICMetProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &met_);
}

void ICMetProducer::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICMetProducer);
