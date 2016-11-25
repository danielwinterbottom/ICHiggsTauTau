#include "UserCode/ICHiggsTauTau/plugins/ICEventProducer.hh"
#include <memory>
#include "TTree.h"
#ifndef CMSSW_4_2_8_patch7
 #include "Compression.h"
#endif
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"

ICEventProducer::ICEventProducer(const edm::ParameterSet& config)
    : processed_(0) {
  edm::Service<TFileService> fs;
  ic::StaticTree::tree_ = fs->make<TTree>("EventTree", "EventTree");
  #ifndef CMSSW_4_2_8_patch7
   fs->file().SetCompressionSettings(ROOT::CompressionSettings(ROOT::kLZMA, 5));
  #endif
}

ICEventProducer::~ICEventProducer() {}

void ICEventProducer::produce(edm::Event& /*event*/,
                              const edm::EventSetup& /*setup*/) {
  ic::StaticTree::tree_->Fill();
  ++processed_;
  if (processed_ == 500) ic::StaticTree::tree_->OptimizeBaskets();
}

void ICEventProducer::beginJob() {}

void ICEventProducer::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICEventProducer);
