#include "UserCode/ICHiggsTauTau/plugins/ICSingleMetProducer.hh"
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
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

ICSingleMetProducer::ICSingleMetProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")) {
  consumes<edm::View<reco::MET>>(input_);
  met_ = new ic::Met();
  PrintHeaderWithProduces(config, input_, branch_);
}

ICSingleMetProducer::~ICSingleMetProducer() {
  delete met_;
}

void ICSingleMetProducer::produce(edm::Event& event,
                                  const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::MET> > mets_handle;
  event.getByLabel(input_, mets_handle);

  *met_ = ic::Met();

  if (mets_handle->size() >= 1) {
    reco::MET const& src = mets_handle->at(0);
    ic::Met& dest = *met_;
    dest.set_id(met_hasher_(&src));
    dest.set_pt(src.pt());
    dest.set_eta(src.eta());
    dest.set_phi(src.phi());
    dest.set_energy(src.energy());
    dest.set_sum_et(src.sumEt());
    dest.set_et_sig(src.mEtSig());
    dest.set_xx_sig(src.getSignificanceMatrix()(0, 0));
    dest.set_xy_sig(src.getSignificanceMatrix()(0, 1));
    dest.set_yx_sig(src.getSignificanceMatrix()(1, 0));
    dest.set_yy_sig(src.getSignificanceMatrix()(1, 1));
  }
}

void ICSingleMetProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &met_);
}

void ICSingleMetProducer::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICSingleMetProducer);
