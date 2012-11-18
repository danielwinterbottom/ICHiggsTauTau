#include "UserCode/ICHiggsTauTau/plugins/ICGenJetProducer.hh"
#include <boost/functional/hash.hpp>
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

#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/Jet.hh"


#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"

#include "UserCode/ICHiggsTauTau/interface/city.h"

#include "boost/format.hpp"


ICGenJetProducer::ICGenJetProducer(const edm::ParameterSet& iConfig) {
  produces<std::vector<unsigned> >("selectGenParticles");
  input_label_ = iConfig.getParameter<edm::InputTag>("inputLabel");
  branch_name_ = iConfig.getUntrackedParameter<std::string>("branchName");
  add_all_ = iConfig.getUntrackedParameter<bool>("addAll");
  store_gen_particles_ = iConfig.getUntrackedParameter<bool>("storeGenParticles");
  add_all_pt_cut_ = iConfig.getUntrackedParameter<double>("addAllPtCut");
  add_all_eta_cut_ = iConfig.getUntrackedParameter<double>("addAllEtaCut");
  cand_vec = new std::vector<ic::GenJet>();
  merge_labels_ = iConfig.getUntrackedParameter<std::vector<std::string> >("mergeLabels");
  std::cout << "Info in <ICGenJetProducer>: Picking up GenJet requests from the following modules:" << std::endl;
  for (unsigned i = 0; i < merge_labels_.size(); ++i) {
    std::cout << "-- " << merge_labels_[i] << std::endl;
  }
  if (add_all_) {
    std::cout << "-- Additionally adding all GenJets with pT > " << add_all_pt_cut_
        << " GeV and |eta| < " << add_all_eta_cut_ << std::endl;
  } else {
    std::cout << "-- Addition of other GenJets is switched off" << std::endl;
  }
}


ICGenJetProducer::~ICGenJetProducer() {
 // do anything here that needs to be done at desctruction time
 // (e.g. close files, deallocate resources etc.)
 delete cand_vec;
}



// ------------ method called to produce the data  ------------
void ICGenJetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  boost::hash<reco::GenJet const*> hasher;
  boost::hash<reco::GenParticle const*> part_hasher;

  edm::Handle<reco::GenJetCollection> genjetCollection;
  iEvent.getByLabel(input_label_,genjetCollection);
  //const reco::GenJet *ptr_to_first  = &((genjetCollection->at(0)));

  edm::Handle<reco::GenParticleCollection> partCollection;
  iEvent.getByLabel("genParticles",partCollection);
  std::auto_ptr<std::vector<unsigned> > jet_particles(new std::vector<unsigned>());
  
  std::set<reco::GenJet const*> ptr_set;
  std::vector<edm::Handle<std::vector<unsigned> > > merge_handles;
  merge_handles.resize(merge_labels_.size());

  if (!add_all_) {
    for (unsigned i = 0; i < merge_handles.size(); ++i) {
      iEvent.getByLabel(merge_labels_[i],"selectGenJets", merge_handles[i]);
      for (unsigned j = 0; j < merge_handles[i]->size(); ++j) {
        const reco::GenJet *ptr = &(genjetCollection->at(merge_handles[i]->at(j)));
        ptr_set.insert(ptr);
      }
    }
  } else {
    for (unsigned i = 0; i < genjetCollection->size(); ++i)
      if (genjetCollection->at(i).pt() >= add_all_pt_cut_ && 
        fabs(genjetCollection->at(i).eta()) <= fabs(add_all_eta_cut_)) {
        ptr_set.insert(&(genjetCollection->at(i)));
    }
  }


  //reco::GenJetCollection::const_iterator iter;
  std::set<reco::GenJet const*>::const_iterator iter;
  cand_vec->resize(0);
  cand_vec->reserve(genjetCollection->size());

  for (iter = ptr_set.begin(); iter != ptr_set.end(); ++iter) {
    cand_vec->push_back(ic::GenJet());
    ic::GenJet & cand = cand_vec->back();
    //Set candidate data
    cand.set_id(hasher(*iter));
    cand.set_pt((*iter)->pt());
    cand.set_eta((*iter)->eta());
    cand.set_phi((*iter)->phi());
    cand.set_energy((*iter)->energy());
    cand.set_charge((*iter)->charge());
    cand.set_flavour(0);
    cand.set_n_constituents((*iter)->getGenConstituents().size());
    
    std::vector<std::size_t> constituents;
    for (unsigned i = 0; i < (*iter)->getGenConstituents().size(); ++i) {
      reco::GenParticle const *part = (*iter)->getGenConstituent(i);
      unsigned idx = unsigned(part - &(partCollection->at(0)));
      jet_particles->push_back(idx);
      if (store_gen_particles_) constituents.push_back(part_hasher(part));
    }
    cand.set_constituents(constituents); 

  }
  iEvent.put(jet_particles, "selectGenParticles");
}

// ------------ method called once each job just before starting event loop  ------------
void ICGenJetProducer::beginJob() {
 ic::StaticTree::tree_->Branch(branch_name_.c_str(),&cand_vec);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICGenJetProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void ICGenJetProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICGenJetProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICGenJetProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICGenJetProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICGenJetProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICGenJetProducer);
