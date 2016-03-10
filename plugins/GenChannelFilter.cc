#include "UserCode/ICHiggsTauTau/plugins/GenChannelFilter.hh"
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

GenChannelFilter::GenChannelFilter(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")), //"prunedGenParticles"
    channel_(config.getParameter<int>("channel")) 
{
  consumes<reco::GenParticleCollection>(input_); 
}

GenChannelFilter::~GenChannelFilter() {}

bool GenChannelFilter::filter(edm::Event& event,
                                  const edm::EventSetup& setup) {
 
  edm::Handle<reco::GenParticleCollection> genParticles;
  event.getByLabel(input_, genParticles); 
  
  int DecayChannelID = 0;
  unsigned n_GenTaus = 0;
  
  bool e_found[2] =   {false};
  bool mu_found[2] =  {false};
  bool tau_found[2] = {false};
  
  for(unsigned i = 0; i < genParticles->size(); ++ i) {
      
      const reco::GenParticle & p = (*genParticles)[i];
      
      if(fabs(p.pdgId())==15 && p.mother()->pdgId() == 25){
          // Particle is a tau from a Higgs decay.
          
          for(unsigned j=0; j < p.numberOfDaughters(); j++){
              if(fabs(p.daughter(j)->pdgId()) == 11)  e_found[n_GenTaus] = true; 
              if(fabs(p.daughter(j)->pdgId()) == 13) mu_found[n_GenTaus] = true;
          }
          
          if(e_found[n_GenTaus] != true && mu_found[n_GenTaus] != true) tau_found[n_GenTaus] = true;
          n_GenTaus++;
      }
      
  }
  
  if(e_found[0] == true && e_found[1] == true)                                                         DecayChannelID = 1; // ee
  if((e_found[0] == true && mu_found[1] == true) || (e_found[1] == true && mu_found[0] == true))       DecayChannelID = 2; // emu
  if((e_found[0] == true && tau_found[1] == true) || (e_found[1] == true && tau_found[0] == true))     DecayChannelID = 3; // etau
  if(mu_found[0] == true && mu_found[1] == true)                                                       DecayChannelID = 4; // mumu
  if((tau_found[0] == true && mu_found[1] == true) || (tau_found[1] == true && mu_found[0] == true))   DecayChannelID = 5; // mutau
  if((tau_found[0] == true && tau_found[1] == true) || (tau_found[1] == true && tau_found[0] == true)) DecayChannelID = 6; // tautau
  
  if (DecayChannelID == channel_) return true;
  else if (channel_ == 2 ) return true; // Use channel_ = 0 option to not filter by decay channels.  
  else return false;
    
}


void GenChannelFilter::beginJob() {}

void GenChannelFilter::endJob() {}

DEFINE_FWK_MODULE(GenChannelFilter);
