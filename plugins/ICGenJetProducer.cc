#include "UserCode/ICHiggsTauTau/plugins/ICGenJetProducer.hh"
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
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

ICGenJetProducer::ICGenJetProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      input_particles_(config.getParameter<edm::InputTag>("inputGenParticles")),
      request_gen_particles_(config.getParameter<bool>("requestGenParticles")),
      is_slimmed_(config.getParameter<bool>("isSlimmed")) {
  consumes<edm::View<reco::GenJet>>(input_);
  consumes<reco::GenParticleCollection>(input_particles_);
  gen_jets_ = new std::vector<ic::GenJet>();
  if (request_gen_particles_) {
    produces<reco::GenParticleRefVector>("requestedGenParticles");
  }

  PrintHeaderWithProduces(config, input_, branch_);
  PrintOptional(1, request_gen_particles_, "requestGenParticles");
}

ICGenJetProducer::~ICGenJetProducer() { delete gen_jets_; }

void ICGenJetProducer::produce(edm::Event& event,
                               const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::GenJet> > jets_handle;
  event.getByLabel(input_, jets_handle);

  edm::Handle<reco::GenParticleCollection> parts_handle;
  std::map<reco::GenParticle const*, std::size_t> part_map;
  if (request_gen_particles_) {
    event.getByLabel(input_particles_, parts_handle);
    for (unsigned i = 0; i < parts_handle->size(); ++i) {
      part_map[&parts_handle->at(i)] = i;
    }
  }

  std::unique_ptr<reco::GenParticleRefVector> part_requests(
      new reco::GenParticleRefVector());

  gen_jets_->clear();
  gen_jets_->resize(jets_handle->size());

  for (unsigned i = 0; i < jets_handle->size(); ++i) {
    reco::GenJet const& src = jets_handle->at(i);
    ic::GenJet& dest = gen_jets_->at(i);
    dest.set_id(gen_jet_hasher_(&src));
    dest.set_pt(src.pt());
    dest.set_eta(src.eta());
    dest.set_phi(src.phi());
    dest.set_energy(src.energy());
    dest.set_charge(src.charge());
    dest.set_flavour(0);
    if(is_slimmed_) dest.set_n_constituents(src.numberOfDaughters());
    else dest.set_n_constituents(src.getGenConstituents().size());
    if (request_gen_particles_) {
      std::vector<std::size_t> constituents;
      for (unsigned i = 0; i < src.getGenConstituents().size(); ++i) {
        reco::GenParticle const* ptr = src.getGenConstituent(i);
        std::map<reco::GenParticle const*, std::size_t>::const_iterator it =
            part_map.find(ptr);
        if (it != part_map.end()) {
          constituents.push_back(particle_hasher_(ptr));
          part_requests->push_back(
              reco::GenParticleRef(parts_handle, it->second));
        }
      }
      dest.set_constituents(constituents);
    }
  }
  if (request_gen_particles_) event.put(std::move(part_requests), "requestedGenParticles");
}

void ICGenJetProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &gen_jets_);
}

void ICGenJetProducer::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICGenJetProducer);
