#ifndef CMSSW_VBFHiggsTauTau_GenParticleAnalysis_GenAnalysisVBFHiggsDataFormat
#define CMSSW_VBFHiggsTauTau_GenParticleAnalysis_GenAnalysisVBFHiggsDataFormat

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include <vector>

namespace VBFHiggs {

  enum HiggsDecay {
    EleEle=1,
    EleMuo=2,
    EleHad=3,
    MuoMuo=4,
    MuoHad=5,
    HadHad=6,
    Invisible=7,
  };
  
  enum TauDecay {
    Ele=1,
    Muo=2,
    Had=3,
  };
  
  class GenAnalysisDataFormat {
  public: // Enumerator
    
    GenAnalysisDataFormat(){ reset();};
    ~GenAnalysisDataFormat(){};
    
    void reset(){
      higgs_decayType=0;
      tau1_decayType =0;
      tau2_decayType =0;
      genMet         =0;
      
      tau1Visproducts_       .clear();
      tau2Visproducts_       .clear();
      tau1Invisproducts_     .clear();
      tau2Invisproducts_     .clear();
      VBFParton1_ = reco::GenParticle();         
      VBFParton2_ = reco::GenParticle();         
      VBFParton1Visproducts_ .clear();
      VBFParton2Visproducts_ .clear();
      VBFgenjet1_ = reco::GenJet();
      VBFgenjet2_ = reco::GenJet();
      
      VBFParton1Matched = false;
      VBFParton2Matched = false;
    }
    
  public: // Members
    unsigned char higgs_decayType;
    unsigned char tau1_decayType;
    unsigned char tau2_decayType;
    
    double genMet;
    
    std::vector<reco::GenParticle> tau1Visproducts_;
    std::vector<reco::GenParticle> tau2Visproducts_;
    std::vector<reco::GenParticle> tau1Invisproducts_;
    std::vector<reco::GenParticle> tau2Invisproducts_;
    
    reco::GenParticle VBFParton1_;
    reco::GenParticle VBFParton2_;
    std::vector<reco::GenParticle> VBFParton1Visproducts_;
    std::vector<reco::GenParticle> VBFParton2Visproducts_;
    
    reco::GenJet VBFgenjet1_;
    reco::GenJet VBFgenjet2_;
    
    bool VBFParton1Matched;
    bool VBFParton2Matched;
    
  }; 
}
#endif