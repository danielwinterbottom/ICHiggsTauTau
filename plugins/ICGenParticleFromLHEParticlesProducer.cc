#include "UserCode/ICHiggsTauTau/plugins/ICGenParticleFromLHEParticlesProducer.hh"
#include <string>
#include <vector>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include <bitset>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

ICGenParticleFromLHEParticlesProducer::ICGenParticleFromLHEParticlesProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")){
  consumes<LHEEventProduct>(input_);
  particles_ = new std::vector<ic::GenParticle>();

  PrintHeaderWithProduces(config, input_, branch_);
}

ICGenParticleFromLHEParticlesProducer::~ICGenParticleFromLHEParticlesProducer() { delete particles_; }

void ICGenParticleFromLHEParticlesProducer::produce(edm::Event& event,
                                    const edm::EventSetup& setup) {

  edm::Handle<LHEEventProduct> lhe_handle;
  event.getByLabel(input_, lhe_handle);
  std::vector<lhef::HEPEUP::FiveVector> lheParticles = lhe_handle->hepeup().PUP;

  particles_->clear();
  particles_->resize(lheParticles.size(), ic::GenParticle());

  ROOT::Math::PxPyPzEVector cand_;
  for (unsigned i = 0; i < lheParticles.size(); ++i) {
    ic::GenParticle& dest = particles_->at(i);
    cand_ = ROOT::Math::PxPyPzEVector(lheParticles[i][0],lheParticles[i][1],lheParticles[i][2],lheParticles[i][3]);
    dest.set_pt((cand_).Pt());
    dest.set_eta((cand_).Eta());
    dest.set_phi((cand_).Phi());
    dest.set_energy((cand_).E());
    dest.set_pdgid(lhe_handle->hepeup().IDUP[i]);
    dest.set_status(lhe_handle->hepeup().ISTUP[i]);
  }
}

void ICGenParticleFromLHEParticlesProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &particles_);

}

void ICGenParticleFromLHEParticlesProducer::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICGenParticleFromLHEParticlesProducer);
