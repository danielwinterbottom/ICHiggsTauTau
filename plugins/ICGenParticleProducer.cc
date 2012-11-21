#include "UserCode/ICHiggsTauTau/plugins/ICGenParticleProducer.hh"
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
#include "boost/regex.hpp"
#include "boost/lexical_cast.hpp"

ICGenParticleProducer::ICGenParticleProducer(const edm::ParameterSet& iConfig) {
  cand_vec = new std::vector<ic::GenParticle>();
  merge_labels_ = iConfig.getUntrackedParameter<std::vector<std::string> >("mergeLabels");
  store_mothers_ = iConfig.getUntrackedParameter<bool>("storeMothers");
  store_daughters_ = iConfig.getUntrackedParameter<bool>("storeDaughters");
  add_status_1_ = iConfig.getUntrackedParameter<bool>("addAllStatus1");
  add_status_2_ = iConfig.getUntrackedParameter<bool>("addAllStatus2");
  add_status_3_ = iConfig.getUntrackedParameter<bool>("addAllStatus3");
  status_1_str_ = iConfig.getUntrackedParameter<std::vector<std::string> >("addAllStatus1Regex");
  status_2_str_ = iConfig.getUntrackedParameter<std::vector<std::string> >("addAllStatus2Regex");
  status_3_str_ = iConfig.getUntrackedParameter<std::vector<std::string> >("addAllStatus3Regex");
  status_1_pt_ = iConfig.getUntrackedParameter<double>("addAllStatus1PtThreshold");
  status_2_pt_ = iConfig.getUntrackedParameter<double>("addAllStatus2PtThreshold");
  status_3_pt_ = iConfig.getUntrackedParameter<double>("addAllStatus3PtThreshold");
  branch_name_ = iConfig.getUntrackedParameter<std::string>("branchName");
  override_collection_ = iConfig.getUntrackedParameter<std::string>("overrideCollection");

  for (unsigned i = 0; i < status_1_str_.size(); ++i){
    status_1_regex_.push_back(boost::regex(status_1_str_[i]));
  }
  for (unsigned i = 0; i < status_2_str_.size(); ++i){
    status_2_regex_.push_back(boost::regex(status_2_str_[i]));
  }
  for (unsigned i = 0; i < status_3_str_.size(); ++i){
    status_3_regex_.push_back(boost::regex(status_3_str_[i]));
  }

  std::cout << "Info in <ICGenParticleProducer>: Picking up GenParticle requests from the following modules:" << std::endl;
  for (unsigned i = 0; i < merge_labels_.size(); ++i) {
    std::cout << "-- " << merge_labels_[i] << std::endl;
  }
  if (add_status_1_) {
    std::cout << "-- Additionally adding all status 1 GenParticles with pT > " << status_1_pt_
    << " GeV and PDG ID matching one of: " << std::endl;
    for (unsigned i = 0; i < status_1_str_.size(); ++i) std::cout << "---- '" << status_1_str_[i] << "'" << std::endl;
  } else {
    std::cout << "-- Addition of other status 1 GenParticles is switched off" << std::endl;
  }
  if (add_status_2_) {
    std::cout << "-- Additionally adding all status 2 GenParticles with pT > " << status_2_pt_
    << " GeV and PDG ID matching one of: " << std::endl;
    for (unsigned i = 0; i < status_2_str_.size(); ++i) std::cout << "---- '" << status_2_str_[i] << "'" << std::endl;
  } else {
    std::cout << "-- Addition of other status 2 GenParticles is switched off" << std::endl;
  }
  if (add_status_3_) {
    std::cout << "-- Additionally adding all status 3 GenParticles with pT > " << status_3_pt_
    << " GeV and PDG ID matching one of: " << std::endl;
    for (unsigned i = 0; i < status_3_str_.size(); ++i) std::cout << "---- '" << status_3_str_[i] << "'" << std::endl;
  } else {
    std::cout << "-- Addition of other status 3 GenParticles is switched off" << std::endl;
  }

  if (override_collection_ != "") std::cout << "Override label is " << override_collection_ << std::endl;
}


ICGenParticleProducer::~ICGenParticleProducer() {
 // do anything here that needs to be done at desctruction time
 // (e.g. close files, deallocate resources etc.)
 delete cand_vec;
}



// ------------ method called to produce the data  ------------
void ICGenParticleProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  boost::hash<reco::GenParticle const*> hasher;

  edm::Handle<reco::GenParticleCollection> partCollection;
  std::string collection = "SIM";
  if (override_collection_ != "") collection = override_collection_;
  iEvent.getByLabel(edm::InputTag("genParticles","",collection),partCollection);
  if (partCollection->size() == 0) return;
  const reco::GenParticle *ptr_to_first  = &((partCollection->at(0)));
  
  std::set<reco::GenParticle const*> ptr_set;
  std::vector<edm::Handle<std::vector<unsigned> > > merge_handles;
  merge_handles.resize(merge_labels_.size());
  for (unsigned i = 0; i < merge_handles.size(); ++i) {
    iEvent.getByLabel(merge_labels_[i],"selectGenParticles", merge_handles[i]);
    for (unsigned j = 0; j < merge_handles[i]->size(); ++j) {
      const reco::GenParticle *ptr = &(partCollection->at(merge_handles[i]->at(j)));
      ptr_set.insert(ptr);
    }
  }

  //Pick up any other requested particles
  for (unsigned i = 0; i < partCollection->size(); ++i) {
    reco::GenParticle const& p = (*partCollection)[i];
    bool keep = false;
    std::string pdgid = boost::lexical_cast<std::string>(p.pdgId());
    if (p.status() == 1 && add_status_1_){
      for (unsigned j = 0; j < status_1_regex_.size(); ++j){
        if (boost::regex_match(pdgid,status_1_regex_[j]) && p.pt() > status_1_pt_) keep = true;
      }
    }
    if (p.status() == 2 && add_status_2_){
      for (unsigned j = 0; j < status_2_regex_.size(); ++j){
        if (boost::regex_match(pdgid,status_2_regex_[j]) && p.pt() > status_2_pt_) keep = true;
      }
    }
    if (p.status() == 3 && add_status_3_){
      for (unsigned j = 0; j < status_3_regex_.size(); ++j){
        if (boost::regex_match(pdgid,status_3_regex_[j]) && p.pt() > status_3_pt_) keep = true;
      }
    }
    if (keep) ptr_set.insert(&p);
  }


  //reco::GenParticleCollection::const_iterator iter;
  std::set<reco::GenParticle const*>::const_iterator iter;
  cand_vec->resize(0);
  cand_vec->reserve(partCollection->size());

  for (iter = ptr_set.begin(); iter != ptr_set.end(); ++iter) {
    cand_vec->push_back(ic::GenParticle());
    ic::GenParticle & cand = cand_vec->back();
    //Set candidate data
    cand.set_id(hasher(*iter));
    cand.set_pt((*iter)->pt());
    cand.set_eta((*iter)->eta());
    cand.set_phi((*iter)->phi());
    cand.set_energy((*iter)->energy());
    cand.set_charge((*iter)->charge());
    cand.set_index(int(*iter - ptr_to_first));
    cand.set_pdgid((*iter)->pdgId());
    cand.set_status((*iter)->status());
    if (store_mothers_) {
      std::vector<int> mothers((*iter)->numberOfMothers(), 0);
      for (unsigned i = 0; i < (*iter)->numberOfMothers(); ++i) {
        mothers[i] = ((reco::GenParticle*)(*iter)->mother(i) - ptr_to_first);
      }
      cand.set_mothers(mothers);
    }
    if (store_daughters_) {
      std::vector<int> daughters((*iter)->numberOfDaughters(), 0);
      for (unsigned i = 0; i < (*iter)->numberOfDaughters(); ++i) {
        daughters[i] = ((reco::GenParticle*)(*iter)->daughter(i) - ptr_to_first);
      }
      cand.set_daughters(daughters); 
    }
  }
}

// ------------ method called once each job just before starting event loop  ------------
void ICGenParticleProducer::beginJob() {
 ic::StaticTree::tree_->Branch(branch_name_.c_str() ,&cand_vec);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICGenParticleProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void ICGenParticleProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICGenParticleProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICGenParticleProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICGenParticleProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICGenParticleProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICGenParticleProducer);
