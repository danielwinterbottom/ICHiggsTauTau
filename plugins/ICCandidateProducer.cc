#include "UserCode/ICHiggsTauTau/plugins/ICCandidateProducer.hh"
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
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"

ICCandidateProducer::ICCandidateProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")) {
  consumes<edm::View<reco::Candidate>>(input_);
  candidates_ = new std::vector<ic::Candidate>();
  PrintHeaderWithProduces(config, input_, branch_);
}

ICCandidateProducer::~ICCandidateProducer() { delete candidates_; }

void ICCandidateProducer::produce(edm::Event& event,
                                  const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::Candidate> > cand_handle;
  event.getByLabel(input_, cand_handle);

  candidates_->clear();
  candidates_->resize(cand_handle->size(), ic::Candidate());

  for (unsigned i = 0; i < cand_handle->size(); ++i) {
    reco::Candidate const& src = cand_handle->at(i);
    ic::Candidate & dest = candidates_->at(i);
    dest.set_id(cand_hasher_(&src));
    dest.set_pt(src.pt());
    dest.set_eta(src.eta());
    dest.set_phi(src.phi());
    dest.set_energy(src.energy());
    dest.set_charge(src.charge());
  }
}

void ICCandidateProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &candidates_);
}

void ICCandidateProducer::endJob() {}

DEFINE_FWK_MODULE(ICCandidateProducer);
