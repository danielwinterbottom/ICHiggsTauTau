#include "UserCode/ICHiggsTauTau/plugins/ICHashTreeProducer.hh"
#include <memory>
#include "TTree.h"
#include "Compression.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"

std::map<std::size_t, std::string> ICHashTreeProducer::unhash_map_;

ICHashTreeProducer::ICHashTreeProducer(const edm::ParameterSet& config) {}

ICHashTreeProducer::~ICHashTreeProducer() {}

void ICHashTreeProducer::Add(std::size_t const& id, std::string const& str) {
  unhash_map_[id] = str;
}


void ICHashTreeProducer::produce(edm::Event& /*event*/,
                              const edm::EventSetup& /*setup*/) {}

void ICHashTreeProducer::beginJob() {}

void ICHashTreeProducer::endJob() {
  edm::Service<TFileService> fs;
  TTree *tree = fs->make<TTree>("HashTree", "HashTree");
  fs->file().SetCompressionSettings(ROOT::CompressionSettings(ROOT::kLZMA, 5));
  ULong64_t id;
  std::string str;
  tree->Branch("id", &id);
  tree->Branch("string", &str);
  for (auto const& vals : unhash_map_) {
    id = vals.first;
    str = vals.second;
    tree->Fill();
  }
}

// define this as a plug-in
DEFINE_FWK_MODULE(ICHashTreeProducer);
