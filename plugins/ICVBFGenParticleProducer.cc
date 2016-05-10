#include "UserCode/ICHiggsTauTau/plugins/ICVBFGenParticleProducer.hh"
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
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

using namespace edm;
using namespace std;

ICVBFGenParticleProducer::ICVBFGenParticleProducer(const edm::ParameterSet& config)
 {
  
  ps = config;
  
  edm::InputTag inputTag_HepMCProduct          = ps.getUntrackedParameter<edm::InputTag>("inputTag_HepMCProduct",         edm::InputTag("generator"));
  edm::InputTag inputTag_GenJetCollection      = ps.getUntrackedParameter<edm::InputTag>("inputTag_GenJetCollection",     edm::InputTag("ak4GenJetsNoNu"));  
  edm::InputTag inputTag_GenParticleCollection = ps.getUntrackedParameter<edm::InputTag>("inputTag_GenParticleCollection",edm::InputTag("genParticles")); 
  
  m_genAnalysisData = new VBFHiggs::GenAnalysisDataFormat();
  
}

ICVBFGenParticleProducer::~ICVBFGenParticleProducer() {}

void ICVBFGenParticleProducer::produce(edm::Event& event,
                                    const edm::EventSetup& setup) {
    
  m_genAnalysisData->reset();
  
  Handle<reco::GenParticleCollection> genParticles;
  event.getByToken(m_inputTag_GenParticleCollection, genParticles);
  
  Handle<edm::View<reco::GenJet> > jets_handle;
  event.getByToken(m_inputTag_GenJetCollection, jets_handle);
  
  ROOT::Math::PtEtaPhiEVector vecNeutrino(0,0,0,0);
  
  unsigned tauCount  = 0;
  unsigned elecCount = 0;
  unsigned muCount   = 0;
  
  for(size_t i = 0; i < genParticles->size(); ++ i) {
    const reco::GenParticle & p = (*genParticles)[i];
    
    if(p.status()==1 && (fabs(p.pdgId())==12 || fabs(p.pdgId())==14 || fabs(p.pdgId())==16)) vecNeutrino += p.p4();
    
    if(fabs(p.pdgId())==15){
    
        
      if(p.mother()->pdgId() == 25){
          
        unsigned n = p.numberOfDaughters();
        for(size_t j = 0; j < n; ++ j) {
          const reco::GenParticle *d = (reco::GenParticle*) p.daughter( j );
          
          if(tauCount == 0){
              if(fabs(p.pdgId())==12 || fabs(p.pdgId())==14 || fabs(p.pdgId())==16) m_genAnalysisData->tau1Invisproducts_   .push_back(*d);
              else                                                                  m_genAnalysisData->tau1Visproducts_     .push_back(*d);
          }                                                                                                                 
          if(tauCount == 1){                                                                                                
              if(fabs(p.pdgId())==12 || fabs(p.pdgId())==14 || fabs(p.pdgId())==16) m_genAnalysisData->tau2Invisproducts_   .push_back(*d);
              else                                                                  m_genAnalysisData->tau2Visproducts_     .push_back(*d);
          }
          
          if(fabs(p.pdgId())==11){
              elecCount++;
              if(tauCount == 0) m_genAnalysisData->tau1_decayType = VBFHiggs::TauDecay::Ele;
              if(tauCount == 1) m_genAnalysisData->tau2_decayType = VBFHiggs::TauDecay::Ele;
          }
          else if(fabs(p.pdgId())==13){
              muCount++;
              if(tauCount == 0) m_genAnalysisData->tau1_decayType = VBFHiggs::TauDecay::Muo;
              if(tauCount == 1) m_genAnalysisData->tau2_decayType = VBFHiggs::TauDecay::Muo;
          }
          else{
              if(tauCount == 0) m_genAnalysisData->tau1_decayType = VBFHiggs::TauDecay::Had;
              if(tauCount == 1) m_genAnalysisData->tau2_decayType = VBFHiggs::TauDecay::Had;
          }
          
          tauCount++;
        }
      }
    }
    
    if(i == 5){
      m_genAnalysisData->VBFParton1_ = p;
      unsigned n = p.numberOfDaughters();
      for(size_t j = 0; j < n; ++ j) {
        const reco::GenParticle *d = (reco::GenParticle*) p.daughter( j );
        if(d->status()==1 && !(fabs(d->pdgId())==12 || fabs(d->pdgId())==14 || fabs(d->pdgId())==16)) m_genAnalysisData->VBFParton1Visproducts_   .push_back(*d);
      }
      
      int genjetIndex = -1;
      double DeltaRMin = 1000000;
      
      for(size_t j = 0; i< jets_handle->size(); ++j){
      
        const reco::GenJet & genjet = (*jets_handle)[j];
        
        double DeltaR = sqrt(pow(genjet.phi()-p.phi(),2) + pow(genjet.eta()-p.eta(),2));
        if(DeltaR < DeltaRMin && DeltaR < 0.5){
            DeltaRMin = DeltaR;
            genjetIndex = j;
        }
      }
      if(genjetIndex != -1){
        m_genAnalysisData->VBFParton1Matched = true;
        m_genAnalysisData->VBFgenjet1_ = (*jets_handle)[genjetIndex];
      }
      else m_genAnalysisData->VBFParton1Matched = false;
    }
    if(i == 6){
      m_genAnalysisData->VBFParton2_ = p;
      unsigned n = p.numberOfDaughters();
      for(size_t j = 0; j < n; ++ j) {
        const reco::GenParticle *d = (reco::GenParticle*) p.daughter( j );
        if(d->status()==1 && !(fabs(d->pdgId())==12 || fabs(d->pdgId())==14 || fabs(d->pdgId())==16)) m_genAnalysisData->VBFParton2Visproducts_   .push_back(*d);
      }
      
      int genjetIndex = -1;
      double DeltaRMin = 1000000;
      
      for(size_t j = 0; i< jets_handle->size(); ++j){
      
        const reco::GenJet & genjet = (*jets_handle)[j];
        
        double DeltaR = sqrt(pow(genjet.phi()-p.phi(),2) + pow(genjet.eta()-p.eta(),2));
        if(DeltaR < DeltaRMin && DeltaR < 0.5){
            DeltaRMin = DeltaR;
            genjetIndex = j;
        }
      }
      if(genjetIndex != -1){
        m_genAnalysisData->VBFParton2Matched = true;
        m_genAnalysisData->VBFgenjet2_ = (*jets_handle)[genjetIndex];
      }
      else m_genAnalysisData->VBFParton2Matched = false;
    }
    
  }
  
  if     (elecCount == 2 && muCount == 0 && tauCount == 2) m_genAnalysisData->higgs_decayType=VBFHiggs::HiggsDecay::EleEle;
  else if(elecCount == 1 && muCount == 1 && tauCount == 2) m_genAnalysisData->higgs_decayType=VBFHiggs::HiggsDecay::EleMuo;
  else if(elecCount == 1 && muCount == 0 && tauCount == 2) m_genAnalysisData->higgs_decayType=VBFHiggs::HiggsDecay::EleHad;
  else if(elecCount == 0 && muCount == 2 && tauCount == 2) m_genAnalysisData->higgs_decayType=VBFHiggs::HiggsDecay::MuoMuo;
  else if(elecCount == 0 && muCount == 1 && tauCount == 2) m_genAnalysisData->higgs_decayType=VBFHiggs::HiggsDecay::MuoHad;
  else if(elecCount == 0 && muCount == 0 && tauCount == 2) m_genAnalysisData->higgs_decayType=VBFHiggs::HiggsDecay::HadHad;
  
  m_genAnalysisData->genMet = vecNeutrino.pt();
  
}

void ICVBFGenParticleProducer::beginJob() {
  ic::StaticTree::tree_->Branch("VBFgenParticles", &m_genAnalysisData);

}

void ICVBFGenParticleProducer::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICVBFGenParticleProducer);
