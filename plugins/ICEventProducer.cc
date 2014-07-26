#include "UserCode/ICHiggsTauTau/plugins/ICEventProducer.hh"
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"

#include "TTree.h"
#include "Compression.h"

ICEventProducer::ICEventProducer(const edm::ParameterSet& iConfig)
    : processed_(0) {
  file_ = new TFile("EventTree.root", "RECREATE");
  file_->SetCompressionSettings(ROOT::CompressionSettings(ROOT::kLZMA, 5));
  gDirectory->mkdir("icEventProducer");
  gDirectory->cd("icEventProducer");
  ic::StaticTree::tree_ = new TTree("EventTree", "EventTree");
}

ICEventProducer::~ICEventProducer() {
  // delete ic::StaticTree::tree_;
  delete file_;
}

void ICEventProducer::produce(edm::Event& /*event*/,
                              const edm::EventSetup& /*setup*/) {
  ic::StaticTree::tree_->Fill();
  ++processed_;
  if (processed_ == 500) ic::StaticTree::tree_->OptimizeBaskets();
}

void ICEventProducer::beginJob() {
}

void ICEventProducer::endJob() {
  ic::StaticTree::tree_->Write();
  file_->Close();
}

// define this as a plug-in
DEFINE_FWK_MODULE(ICEventProducer);
