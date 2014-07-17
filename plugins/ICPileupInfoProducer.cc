#include "UserCode/ICHiggsTauTau/plugins/ICPileupInfoProducer.hh"
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
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/PileupInfo.hh"
// #include "boost/format.hpp"

ICPileupInfoProducer::ICPileupInfoProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")) {
  info_ = new std::vector<ic::PileupInfo>();
}

ICPileupInfoProducer::~ICPileupInfoProducer() { delete info_; }

void ICPileupInfoProducer::produce(edm::Event& event,
                                   const edm::EventSetup& setup) {
  edm::Handle<edm::View<PileupSummaryInfo> > info_handle;
  event.getByLabel(input_, info_handle);
  info_->clear();
  info_->resize(info_handle->size());
  for (unsigned i = 0; i < info_handle->size(); ++i) {
    PileupSummaryInfo const& src = info_handle->at(i);
    ic::PileupInfo& dest = info_->at(i);
    dest.set_num_interactions(src.getPU_NumInteractions());
    dest.set_bunch_crossing(src.getBunchCrossing());
    dest.set_true_num_interactions(src.getTrueNumInteractions());
  }
}

void ICPileupInfoProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &info_);
}

void ICPileupInfoProducer::endJob() {
}

// define this as a plug-in
DEFINE_FWK_MODULE(ICPileupInfoProducer);
