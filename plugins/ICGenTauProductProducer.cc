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
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

#include "boost/format.hpp"


ICGenTauProductProducer::ICGenTauProductProducer(const edm::ParameterSet& iConfig) {
  produces<std::vector<unsigned> >("selectGenParticles");
  input_label_ = iConfig.getParameter<edm::InputTag>("inputLabel");
  consumes<reco::GenParticleCollection>(input_label_);

}


ICGenTauProductProducer::~ICGenTauProductProducer() {
 // do anything here that needs to be done at desctruction time
 // (e.g. close files, deallocate resources etc.)
}

void ICGenTauProductProducer::FindDecayProducts(const reco::GenParticle* theParticle, std::vector<const reco::GenParticle *> &container) 
{
  if(theParticle)
  {  
    container.push_back(theParticle);

    unsigned int nDaughters=theParticle->numberOfDaughters();
    for(unsigned j=0; j<nDaughters; j++)
    {
       const reco::Candidate * d=theParticle->daughter(j);
       // if((static_cast<const reco::GenParticle*>(d))->numberOfDaughters() > 0)
       // {
       //   container.push_back(static_cast<const reco::GenParticle*>(d));
       // }
       FindDecayProducts( static_cast<const reco::GenParticle*>(d) , container);
    }
  }
}


// ------------ method called to produce the data  ------------
void ICGenTauProductProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {


  edm::Handle<reco::GenParticleCollection> partCollection;
  iEvent.getByLabel(input_label_, partCollection); 
  if(partCollection->size()==0) return;
  std::vector<const reco::GenParticle *> daughters ; 

  std::unique_ptr<std::vector<unsigned> > daughter_index(new std::vector<unsigned>());      

  for(unsigned i = 0; i<partCollection->size(); i++)
  {
     const reco::GenParticle &p = (*partCollection)[i];
     if(p.pdgId()==15 || p.pdgId()==-15)
     {
        daughters.clear();
        FindDecayProducts(&p, daughters);
        for(unsigned j=0; j<daughters.size(); j++)
        {
          unsigned idx = unsigned(daughters[j] - &(partCollection->at(0)));
          daughter_index->push_back(idx);
        } 
     }
  }
  iEvent.put(std::move(daughter_index), "selectGenParticles");

}

// ------------ method called once each job just before starting event loop  ------------
void ICGenTauProductProducer::beginJob() {
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
