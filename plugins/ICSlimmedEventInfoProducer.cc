#include "UserCode/ICHiggsTauTau/plugins/ICSlimmedEventInfoProducer.hh"
#include <memory>
#include <string>
#include <vector>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "boost/format.hpp"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/METReco/interface/BeamHaloSummary.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

ICSlimmedEventInfoProducer::ICSlimmedEventInfoProducer(const edm::ParameterSet& config)
    : branch_(config.getParameter<std::string>("branch")),
      lhe_collection_(config.getParameter<edm::InputTag>("lheProducer"))
{
#if CMSSW_MAJOR_VERSION >= 7
      consumes<LHERunInfoProduct, edm::InRun>({lhe_collection_});
#endif
      consumes<LHEEventProduct>(lhe_collection_);

  info_ = new ic::EventInfo();

  PrintHeaderWithBranch(config, branch_);
}

ICSlimmedEventInfoProducer::~ICSlimmedEventInfoProducer() {
  delete info_;
}

void ICSlimmedEventInfoProducer::endRun(edm::Run const& run, edm::EventSetup const& es) {
  if (lhe_weight_labels_.size()) return;
  edm::Handle<LHERunInfoProduct> lhe_info;
  run.getByLabel(lhe_collection_, lhe_info);
  bool record = false;
  for (auto it = lhe_info->headers_begin(); it != lhe_info->headers_end();
       ++it) {
    std::vector<std::string>::const_iterator iLt = it->begin();
    for (; iLt != it->end(); ++iLt) {
      std::string const& line = *iLt;
      if (line.find("<weightgroup")  != std::string::npos) record = true;
      if (line.find("</weightgroup") != std::string::npos) record = false;
      if (record) lhe_weight_labels_.push_back(line);
    }
  }
}

void ICSlimmedEventInfoProducer::produce(edm::Event& event,
                                  const edm::EventSetup& setup) {
  *info_ = ic::EventInfo();
  info_->set_is_data(event.isRealData());
  info_->set_run(event.run());
#if CMSSW_MAJOR_VERSION > 7 || (CMSSW_MAJOR_VERSION ==7 && CMSSW_MINOR_VERSION >= 3)
  info_->set_event(event.id().event());
#else
  info_->set_event((unsigned long long)event.id().event());
#endif
  info_->set_lumi_block(event.luminosityBlock());
  info_->set_bunch_crossing(event.bunchCrossing());

  edm::Handle<LHEEventProduct> lhe_handle;
  event.getByLabel(lhe_collection_, lhe_handle);
  double nominal_wt = lhe_handle->hepeup().XWGTUP;
  info_->set_weight("wt_mc",nominal_wt);
  for (unsigned i = 0; i < lhe_handle->weights().size(); ++i) {
    info_->set_weight(lhe_handle->weights()[i].id,
                          lhe_handle->weights()[i].wgt / nominal_wt, false);
    }
  info_->set_npnlo(lhe_handle->npNLO());
  }


void ICSlimmedEventInfoProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &info_);
}

void ICSlimmedEventInfoProducer::endJob() {
  if (lhe_weight_labels_.size()) {
    std::cout << std::string(78, '-') << "\n";
    std::cout << "LHE event weights\n";
    for (unsigned l = 0; l < lhe_weight_labels_.size(); ++l) {
      std::cout << lhe_weight_labels_[l];
    }
  }
}

// define this as a plug-in
DEFINE_FWK_MODULE(ICSlimmedEventInfoProducer);
