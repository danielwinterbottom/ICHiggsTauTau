#include "UserCode/ICHiggsTauTau/plugins/ICSuperClusterProducer.hh"
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
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/SuperCluster.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

ICSuperClusterProducer::ICSuperClusterProducer(const edm::ParameterSet& config)
    : input_barrel_(config.getParameter<edm::InputTag>("inputBarrel")),
      input_endcap_(config.getParameter<edm::InputTag>("inputEndcap")),
      branch_(config.getParameter<std::string>("branch")) {
  consumes<edm::View<reco::SuperCluster>>(input_barrel_);
  consumes<edm::View<reco::SuperCluster>>(input_endcap_);
  scs_ = new std::vector<ic::SuperCluster>();
  PrintHeaderWithBranch(config, branch_);
}

ICSuperClusterProducer::~ICSuperClusterProducer() { delete scs_; }

void ICSuperClusterProducer::produce(edm::Event& event,
                                     const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::SuperCluster> > sc_b_handle;
  edm::Handle<edm::View<reco::SuperCluster> > sc_e_handle;

  event.getByLabel(input_barrel_, sc_b_handle);
  event.getByLabel(input_endcap_, sc_e_handle);

  scs_->clear();
  scs_->resize(sc_b_handle->size() + sc_e_handle->size());

  for (unsigned i = 0; i < sc_b_handle->size(); ++i) {
    reco::SuperCluster const& src = sc_b_handle->at(i);
    ic::SuperCluster& dest = scs_->at(i);
    dest.set_vx(src.x());
    dest.set_vy(src.y());
    dest.set_vz(src.z());
    dest.set_energy(src.energy());
    dest.set_raw_energy(src.rawEnergy());
    dest.set_is_barrel(true);
    dest.set_id(sc_hasher_(&src));
  }

  for (unsigned i = 0; i < sc_e_handle->size(); ++i) {
    reco::SuperCluster const& src = sc_e_handle->at(i);
    ic::SuperCluster& dest = scs_->at(i + sc_b_handle->size());
    dest.set_vx(src.x());
    dest.set_vy(src.y());
    dest.set_vz(src.z());
    dest.set_energy(src.energy());
    dest.set_raw_energy(src.rawEnergy());
    dest.set_is_barrel(false);
    dest.set_id(sc_hasher_(&src));
  }
}

void ICSuperClusterProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &scs_);
}

void ICSuperClusterProducer::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICSuperClusterProducer);
