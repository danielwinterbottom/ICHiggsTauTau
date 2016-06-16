#ifndef UserCode_ICHiggsTauTau_ICL1TObjectProducer_h
#define UserCode_ICHiggsTauTau_ICL1TObjectProducer_h

#include <memory>
#include <vector>
#include <string>

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"             
#include "FWCore/Framework/interface/Event.h"                       
#include "FWCore/Framework/interface/EventSetup.h"                  
#include "FWCore/Utilities/interface/InputTag.h"                    

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"           
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"            
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"        
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

// data formats
#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "DataFormats/L1Trigger/interface/Tau.h"
#include "DataFormats/L1Trigger/interface/Jet.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"
#include "DataFormats/L1Trigger/interface/BXVector.h"

// system include files
#include <memory>                                                   

// framework
#include "FWCore/Framework/interface/Frameworkfwd.h"                
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"                 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"           

// ROOT output stuff
#include "FWCore/ServiceRegistry/interface/Service.h"               
#include "CommonTools/UtilAlgos/interface/TFileService.h"           
#include "TTree.h"                                                  

// ICHiggsTauTau Objects
#include "UserCode/ICHiggsTauTau/interface/ICL1TObject.hh"



template <class T>
class ICL1TObjectProducer : public edm::EDProducer {
 
 public:
   explicit ICL1TObjectProducer(const edm::ParameterSet &);
   ~ICL1TObjectProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();
  
  void constructSpecific(T const &src, ic::ICL1TObject &dest);

  edm::InputTag input_;
  std::string branch_;

  std::vector<ic::ICL1TObject> *ic_l1t_object_;
  edm::EDGetTokenT< BXVector<T> > m_EDToken_l1t_object;

  // output file
  edm::Service<TFileService> fs_;
  
  // tree
  TTree * tree_;
};


// =============================
// Template class implementation
// =============================
template <class T>
ICL1TObjectProducer<T>::ICL1TObjectProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")) {
  
  //consumes< BXVector<T> >(input_);

  ic_l1t_object_ = new std::vector<ic::ICL1TObject>();

  PrintHeaderWithProduces(config, input_, branch_);
  
  //edm::InputTag inputTag_TObject = config.getUntrackedParameter<edm::InputTag>( ("inputTag_"+branch_).c_str(), edm::InputTag("simCaloStage2Digis") );
  m_EDToken_l1t_object = consumes< BXVector<T> >(input_);

}

template <class T>
ICL1TObjectProducer<T>::~ICL1TObjectProducer() {   
  delete ic_l1t_object_;
}



// =============
// Main producer
// =============
template <class T>
void ICL1TObjectProducer<T>::produce(edm::Event& event, const edm::EventSetup& setup) {
  
  edm::Handle< BXVector<T> > candidate_collection;

  event.getByToken(m_EDToken_l1t_object, candidate_collection);
  //event.getByLabel(input_, candidate_collection);  //adding something which should work with older CMSSW versions you need to use getByLabel
  ic_l1t_object_->clear();
  ic_l1t_object_->resize(candidate_collection->size(), ic::ICL1TObject());
  
  unsigned counter_objects = 0;
  
  if (candidate_collection.isValid()){ 
    //for (int ibx = candidate_collection->getFirstBX(); ibx <= candidate_collection->getLastBX(); ++ibx) {
      //cout << "Processing ibx=" << ibx << endl;
      for (unsigned i = 0; i < candidate_collection->size(0); ++i){
        T const& src = candidate_collection->at(0,i);
        ic::ICL1TObject& dest = ic_l1t_object_->at(counter_objects);
        
        constructSpecific(src,dest);
         
        ROOT::Math::PtEtaPhiEVector tempVector( src.pt(), src.eta(), src.phi(), src.energy() );
        dest.set_vector(tempVector);
        ic_l1t_object_->push_back(dest);
        
        counter_objects++;
      }
    //}
  } 
  else {
    edm::LogWarning("MissingProduct ") << " L1Upgrade " << candidate_collection << " not found. Branch will not be filled." << std::endl;
  }
}



// ==================
// Specific producers
// ==================
template <class T>
void ICL1TObjectProducer<T>::constructSpecific(T const &src, ic::ICL1TObject &dest) {

  // common 
}


// ======
// EGamma
// ======                                                               
template <>
void ICL1TObjectProducer<l1t::EGamma>::constructSpecific( l1t::EGamma const &src, ic::ICL1TObject &dest ) {
  
  dest.setIsolation(src.hwIso());
}                                                               


// ===
// Jet
// ===
template <>
void ICL1TObjectProducer<l1t::Jet>::constructSpecific( l1t::Jet const &src, ic::ICL1TObject &dest ) {}                                                                                    


// ===
// Tau
// ===
template <>
void ICL1TObjectProducer<l1t::Tau>::constructSpecific( l1t::Tau const &src, ic::ICL1TObject &dest ) {

  dest.setIsolation(src.hwIso());  
}                                                               


// ====
// Muon                                                                
// ====                                                                
template <>
void ICL1TObjectProducer<l1t::Muon>::constructSpecific( l1t::Muon const &src, ic::ICL1TObject &dest ) {
                
  dest.setIsolation(src.hwIso());
  dest.setCharge(src.charge());
  dest.setQuality(src.hwQual());  
}                                                               


// =====
// EtSum
// =====
template <>
void ICL1TObjectProducer<l1t::EtSum>::constructSpecific( l1t::EtSum const &src, ic::ICL1TObject &dest ) {
                
                int type = static_cast<int>( src.getType() );
                dest.setSumType(type);
                std::cout << "The getSumType is: " << dest.getSumType() <<"    "<< std::endl;
                std::cout << "The pt is:         " << src.pt()          <<"    "<< std::endl;
}


template <class T>
void ICL1TObjectProducer<T>::beginJob() {
  ic::StaticTree::tree_->Branch( branch_.c_str(), &ic_l1t_object_ );
}

template <class T>
void ICL1TObjectProducer<T>::endJob() {}


#endif
