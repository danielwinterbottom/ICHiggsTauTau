#include "UserCode/ICHiggsTauTau/plugins/ICGenParticleProducer.hh"
#include <string>
#include <vector>
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
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

#if CMSSW_MAJOR_VERSION > 7 || (CMSSW_MAJOR_VERSION == 7 && CMSSW_MINOR_VERSION >= 4)
#include "DataFormats/HepMCCandidate/interface/GenStatusFlags.h"
#endif

ICGenParticleProducer::ICGenParticleProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      store_mothers_(config.getParameter<bool>("includeMothers")),
      store_daughters_(config.getParameter<bool>("includeDaughters")),
      store_statusFlags_(config.getParameter<bool>("includeStatusFlags")){
  consumes<edm::View<reco::GenParticle>>(input_);
  particles_ = new std::vector<ic::GenParticle>();

  PrintHeaderWithProduces(config, input_, branch_);
  PrintOptional(1, store_mothers_, "includeMothers");
  PrintOptional(1, store_daughters_, "includeDaughters");
  PrintOptional(1, store_statusFlags_, "includeStatusFlags");
}

ICGenParticleProducer::~ICGenParticleProducer() { delete particles_; }

void ICGenParticleProducer::produce(edm::Event& event,
                                    const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::GenParticle> > parts_handle;
  event.getByLabel(input_, parts_handle);

  particles_->clear();
  particles_->resize(parts_handle->size(), ic::GenParticle());

  for (unsigned i = 0; i < parts_handle->size(); ++i) {
    reco::GenParticle const& src = parts_handle->at(i);
    // edm::RefToBase<reco::GenParticle> ref = parts_handle->refAt(i);
    ic::GenParticle& dest = particles_->at(i);
    dest.set_id(particle_hasher_(&src));
    dest.set_pt(src.pt());
    dest.set_eta(src.eta());
    dest.set_phi(src.phi());
    dest.set_energy(src.energy());
    dest.set_charge(src.charge());
    dest.set_index(static_cast<int>((parts_handle->refAt(i).key())));
    dest.set_pdgid(src.pdgId());
    dest.set_status(src.status());
#if CMSSW_MAJOR_VERSION > 7 || (CMSSW_MAJOR_VERSION == 7 && CMSSW_MINOR_VERSION >= 4 )
    if (store_statusFlags_){
      reco::GenStatusFlags statusflags = src.statusFlags();
      if(statusflags.flags_.size()!=15){
	throw cms::Exception("OptionNotSupported")<<"size of status flags bitset is "<<statusflags.flags_.size()<<" which is different to what was expected\n";
      }
      else{
	std::vector<bool> statusflags_;
	for(unsigned iFlag=0;iFlag<statusflags.flags_.size();iFlag++){
	  statusflags_.push_back(statusflags.flags_[iFlag]);
	}
	dest.set_statusFlags(statusflags_);
      }
    }
#endif

    if (store_mothers_) {
      std::vector<int> mothers(src.motherRefVector().size(), 0);
      for (unsigned j = 0; j < src.motherRefVector().size(); ++j) {
        mothers[j] = static_cast<int>(src.motherRefVector().at(j).key());
      }
      dest.set_mothers(mothers);
    }
    if (store_daughters_) {
      std::vector<int> daughters(src.daughterRefVector().size(), 0);
      for (unsigned j = 0; j < src.daughterRefVector().size(); ++j) {
        daughters[j] = static_cast<int>(src.daughterRefVector().at(j).key());
      }
      dest.set_daughters(daughters);
    }
  }
}

void ICGenParticleProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &particles_);
#if !(CMSSW_MAJOR_VERSION > 7 || (CMSSW_MAJOR_VERSION == 7 && CMSSW_MINOR_VERSION >= 4))
  if(store_statusFlags_){
    throw cms::Exception("OptionNotSupported")<<"status flags not supported for CMSSW versions before 7_4_X\n";
  }
#endif

}

void ICGenParticleProducer::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICGenParticleProducer);
