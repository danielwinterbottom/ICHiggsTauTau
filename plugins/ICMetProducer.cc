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

ICMetProducer::ICMetProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      do_custom_id_(config.getParameter<bool>("includeCustomID")),
      inputID_(config.getParameter<edm::InputTag>("inputCustomID")) {
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
    dest.set_et_sig(src.mEtSig());
    TMatrixD const& sig_matrix = src.getSignificanceMatrix();
    dest.set_xx_sig(sig_matrix(0, 0));
    dest.set_xy_sig(sig_matrix(0, 1));
    dest.set_yx_sig(sig_matrix(1, 0));
    dest.set_yy_sig(sig_matrix(1, 1));
  }
}

void ICMetProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &met_);
}

void ICMetProducer::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICMetProducer);
