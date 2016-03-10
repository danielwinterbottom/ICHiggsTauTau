#include "UserCode/ICHiggsTauTau/plugins/ICL1EventInfoProducer.hh"
#include <memory>
#include <string>
#include <vector>
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

ICL1EventInfoProducer::ICL1EventInfoProducer(const edm::ParameterSet& config)
    : branch_(config.getParameter<std::string>("branch"))
{
    info_ = new ic::EventInfo();
}

ICL1EventInfoProducer::~ICL1EventInfoProducer() {
  delete info_;
}

void ICL1EventInfoProducer::endRun(edm::Run const& run, edm::EventSetup const& es) {}

void ICL1EventInfoProducer::produce(edm::Event& event,
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

 
}

void ICL1EventInfoProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &info_);
}

void ICL1EventInfoProducer::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICL1EventInfoProducer);
