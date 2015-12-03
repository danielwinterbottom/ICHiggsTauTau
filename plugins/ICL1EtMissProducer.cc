#include "UserCode/ICHiggsTauTau/plugins/ICL1EtMissProducer.hh"
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
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

ICL1EtMissProducer::ICL1EtMissProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")) {
  consumes<edm::View<l1extra::L1EtMissParticle>>(input_);
  candidates_ = new std::vector<ic::Candidate>();
  PrintHeaderWithProduces(config, input_, branch_);
}

ICL1EtMissProducer::~ICL1EtMissProducer() { delete candidates_; }

void ICL1EtMissProducer::produce(edm::Event& event,
                                  const edm::EventSetup& setup) {
  edm::Handle<edm::View<l1extra::L1EtMissParticle> > l1_handle;
  event.getByLabel(input_, l1_handle);

  candidates_->clear();
  candidates_->resize(l1_handle->size(), ic::Candidate());

  for (unsigned i = 0; i < l1_handle->size(); ++i) {
    l1extra::L1EtMissParticle const& src = l1_handle->at(i);
    ic::Candidate & dest = candidates_->at(i);
    dest.set_id(l1_hasher_(&src));
    dest.set_pt(src.pt());
    dest.set_eta(src.eta());
    dest.set_phi(src.phi());
    dest.set_energy(src.etTotal());
    dest.set_charge(src.charge());
    //std::cout<<i<<" "<<src.bx()<<std::endl;
  }
}

void ICL1EtMissProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &candidates_);
}

void ICL1EtMissProducer::endJob() {}

DEFINE_FWK_MODULE(ICL1EtMissProducer);
