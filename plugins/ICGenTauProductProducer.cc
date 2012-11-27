#include "UserCode/ICHiggsTauTau/plugins/ICGenTauProductProducer.hh"
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


#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "UserCode/ICHiggsTauTau/interface/city.h"

#include "boost/format.hpp"


ICGenTauProductProducer::ICGenTauProductProducer(const edm::ParameterSet& iConfig) {
  //produces<std::vector<unsigned> >("selectGenParticles");
  merge_labels_ = iConfig.getUntrackedParameter<std::vector<std::string> >("mergeLabels");
  std::cout << "Info in <ICGenTauProductProducer>: Picking up GenTau requests from the following modules:" << std::endl;
  for (unsigned i = 0; i < merge_labels_.size(); ++i) {
    std::cout << "-- " << merge_labels_[i] << std::endl;
  }
}


ICGenTauProductProducer::~ICGenTauProductProducer() {
 // do anything here that needs to be done at desctruction time
 // (e.g. close files, deallocate resources etc.)
}

void ICGenTauProductProducer::FindDecayProducts(const reco::GenParticle* theParticle, std::vector<const reco::GenParticle *> &container) 
{
  if(theParticle)
  {  
    if(theParticle->status()==1)
    { 
      container.push_back(theParticle);
      return;
    }
    else
    { 
      unsigned int nDaughters=theParticle->numberOfDaughters();
      for(unsigned j=0; j<nDaughters; j++)
      {
         const reco::Candidate * d=theParticle->daughter(j);
         if((static_cast<const reco::GenParticle*>(d))->numberOfDaughters() > 0)
         {
           container.push_back(static_cast<const reco::GenParticle*>(d));
         }
         FindDecayProducts( static_cast<const reco::GenParticle*>(d) , container);
      }
    }
  }
}


// ------------ method called to produce the data  ------------
void ICGenTauProductProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::cout << "===================================" << std::endl;  
  //boost::hash<reco::GenParticle const*> hasher;

  edm::Handle<reco::GenParticleCollection> partCollection;
  iEvent.getByLabel(edm::InputTag("genParticles", "", "SIM"), partCollection); 
  if(partCollection->size()==0) return;
  std::vector<const reco::GenParticle *> daughters ; 

  std::auto_ptr<std::vector<unsigned> > daughter_index(new std::vector<unsigned>());      

  for(unsigned i = 0; i<partCollection->size(); i++)
  {
     const reco::GenParticle &p = (*partCollection)[i];
     if(p.pdgId()==15 || p.pdgId()==-15)
     {
        daughters.clear();
        std::cout << "tau with status: " << p.status()<< std::endl;
        FindDecayProducts(&p, daughters);
        std::cout << "direct daughters of tau" << std::endl;
        for(unsigned j=0; j<p.numberOfDaughters(); j++)
        {
            std::cout << "pdgId for daughter " << j+1 << ": "<< (p.daughter(j))->pdgId() << std::endl;
        }
        std::cout << "daughter indices from function" << std::endl;
        std::cout << "number of daughters found by function: " <<daughters.size() << std::endl;
        for(unsigned j=0; j<daughters.size(); j++)
        {
          unsigned idx = unsigned(daughters[j] - &(partCollection->at(0)));
          std::cout << "index for daughter " << j+1<<": " <<idx << std::endl;
          daughter_index->push_back(idx);

        } 
     }
  }
  //iEvent.put(daughter_index, "selectGenTauParticles");

}

// ------------ method called once each job just before starting event loop  ------------
void ICGenTauProductProducer::beginJob() {
 //ic::StaticTree::tree_->Branch(branch_name_.c_str(),&cand_vec);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICGenTauProductProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void ICGenTauProductProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICGenTauProductProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICGenTauProductProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICGenTauProductProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICGenTauProductProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICGenTauProductProducer);
