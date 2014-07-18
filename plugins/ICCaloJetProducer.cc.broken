#include "UserCode/ICHiggsTauTau/plugins/ICCaloJetProducer.hh"
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
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"

#include "UserCode/ICHiggsTauTau/interface/PATConstruction.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"

#include "boost/format.hpp"


ICCaloJetProducer::ICCaloJetProducer(const edm::ParameterSet& iConfig) {
  produces<std::vector<unsigned> >("selectGenParticles");
  input_label_ = iConfig.getParameter<edm::InputTag>("inputLabel");
  branch_name_ = iConfig.getUntrackedParameter<std::string>("branchName");
  calojets_ = new std::vector<ic::CaloJet>();
  loose_id_ = new JetIDSelectionFunctor( JetIDSelectionFunctor::PURE09, JetIDSelectionFunctor::LOOSE);
  tight_id_ = new JetIDSelectionFunctor( JetIDSelectionFunctor::PURE09, JetIDSelectionFunctor::TIGHT);
}


ICCaloJetProducer::~ICCaloJetProducer() {
 delete calojets_;
 delete loose_id_;
 delete tight_id_;
}

// ------------ method called to produce the data  ------------
void ICCaloJetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  boost::hash<reco::GenParticle const*> hasher;


  edm::Handle<std::vector<pat::Jet> > jetCollection;
  iEvent.getByLabel(input_label_,jetCollection);
  std::vector<pat::Jet>::const_iterator iter;
  //Set the Jet vector size to zero, then allocate enough space
  //for at least as many elements in the PAT::Jet vector
  calojets_->resize(0);
  calojets_->reserve(jetCollection->size());

  edm::Handle<reco::GenParticleCollection> partCollection;
  iEvent.getByLabel("genParticles",partCollection);
  std::auto_ptr<std::vector<unsigned> > jet_particles(new std::vector<unsigned>());

  for (iter = jetCollection->begin(); iter != jetCollection->end(); ++iter) {
    calojets_->push_back(ic::CaloJet());
    ic::CaloJet & jet = calojets_->back();
    ic::ConstructCaloJet(&jet, &(*iter));
    //Set candidate data


    if (iter->genParton()) {
      unsigned idx = unsigned((reco::GenParticle*)iter->genParton() - &(partCollection->at(0)));
      jet_particles->push_back(idx);
      std::vector<std::size_t> gen_index;
      gen_index.push_back(hasher(iter->genParton()));
      jet.set_gen_particles(gen_index); 
    } 

    //Set observed jet energy correction factors
    std::vector<std::string> jec_levels = iter->availableJECLevels();
    for (unsigned i = 0; i < jec_levels.size(); ++i) {
      observed_jec_[jec_levels[i]] = CityHash64(jec_levels[i]);
    }

    //Set observed b-tagging discriminators
    std::vector<std::pair<std::string, float> > const& b_discr = iter->getPairDiscri();
    for (unsigned i = 0; i < b_discr.size(); ++i) {
      observed_btag_[b_discr.at(i).first] = CityHash64(b_discr.at(i).first);
    }

    jet.set_id_loose((*loose_id_)(*iter));
    jet.set_id_tight((*tight_id_)(*iter));
  }
  iEvent.put(jet_particles, "selectGenParticles");
}

// ------------ method called once each job just before starting event loop  ------------
void ICCaloJetProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_name_.c_str(), &calojets_);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICCaloJetProducer::endJob() {
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Info in <ICCaloJetProducer>: EndJob Summary" << std::endl;
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Jet Energy Correction Hash Summary:" << std::endl;
  std::map<std::string, std::size_t>::const_iterator iter;
  for (iter = observed_jec_.begin(); iter != observed_jec_.end(); ++iter) {
    std::cout << boost::format("%-30s %-30s\n") % iter->first % iter->second;
  }
  std::cout << "b-Tag Discriminator Hash Summary:" << std::endl;
  for (iter = observed_btag_.begin(); iter != observed_btag_.end(); ++iter) {
    std::cout << boost::format("%-45s %-20s\n") % iter->first % iter->second;
  }
}

// ------------ method called when starting to processes a run  ------------
void ICCaloJetProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICCaloJetProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICCaloJetProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICCaloJetProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICCaloJetProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICCaloJetProducer);
