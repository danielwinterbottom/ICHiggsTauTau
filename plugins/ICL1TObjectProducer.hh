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
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"



template <class T>
class ICL1TObjectProducer : public edm::EDProducer {
 
 public:
   explicit ICL1TObjectProducer(const edm::ParameterSet &);
   ~ICL1TObjectProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();
  
  void constructSpecific(T const &src, ic::L1TObject &dest);

  edm::InputTag input_;
  std::string branch_;
  bool doBXloop_;
  
  std::vector<ic::L1TObject> *ic_l1t_object_;
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
      branch_(config.getParameter<std::string>("branch")),
      doBXloop_(config.getParameter<bool>("doBXloop")){

  ic_l1t_object_ = new std::vector<ic::L1TObject>();
  PrintHeaderWithProduces(config, input_, branch_);
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
  ic_l1t_object_->clear();
  //std::cout << " -- The candidate_collection size is  " << candidate_collection->size() << std::endl;
  //size is nbx*nelements: total size....
  unsigned NObjectsAllBXs = 0;
  for(int ibx = candidate_collection->getFirstBX(); ibx <= candidate_collection->getLastBX(); ++ibx) NObjectsAllBXs += candidate_collection->size(ibx); 
  ic_l1t_object_->resize(doBXloop_? NObjectsAllBXs : candidate_collection->size(0), ic::L1TObject());

  unsigned counter_objects = 0;
  
  if (candidate_collection.isValid()){ 
    for (int ibx = ( doBXloop_ ? candidate_collection->getFirstBX() : 0 ); ibx <= ( doBXloop_ ? candidate_collection->getLastBX() : 0); ++ibx) {
        //if (doBXloop_) {
	//	std::cout << " -- Processing ibx = " << ibx << std::endl;
	//}
        for (unsigned i = 0; i < candidate_collection->size(ibx); ++i){
          T const& src = candidate_collection->at(ibx,i);
          ic::L1TObject& dest = ic_l1t_object_->at(counter_objects);
          
	  dest.set_bx(ibx);
          
	  constructSpecific(src,dest);
           
          ROOT::Math::PtEtaPhiEVector tempVector( src.pt(), src.eta(), src.phi(), src.energy() );
          dest.set_vector(tempVector);
          //ic_l1t_object_->push_back(dest);
	  //std::cout << " type " << dest.sumType();
	  //dest.Print();
          counter_objects++;
        }
      }
  } 
  else {
    edm::LogWarning("MissingProduct ") << " L1Upgrade " << candidate_collection << " not found. Branch will not be filled." << std::endl;
  }
}



// ==================
// Specific producers
// ==================
template <class T>
void ICL1TObjectProducer<T>::constructSpecific(T const &src, ic::L1TObject &dest) {

  // common 
}


// ======
// EGamma
// ======                                                               
template <>
void ICL1TObjectProducer<l1t::EGamma>::constructSpecific( l1t::EGamma const &src, ic::L1TObject &dest ) {
  
  dest.set_isolation(src.hwIso());
}                                                               


// ===
// Jet
// ===
template <>
void ICL1TObjectProducer<l1t::Jet>::constructSpecific( l1t::Jet const &src, ic::L1TObject &dest ) {}                                                                                    


// ===
// Tau
// ===
template <>
void ICL1TObjectProducer<l1t::Tau>::constructSpecific( l1t::Tau const &src, ic::L1TObject &dest ) {

  dest.set_isolation(src.hwIso());  
}                                                               


// ====
// Muon                                                                
// ====                                                                
template <>
void ICL1TObjectProducer<l1t::Muon>::constructSpecific( l1t::Muon const &src, ic::L1TObject &dest ) {
                
  dest.set_isolation(src.hwIso());
  dest.set_charge(src.charge());
  dest.set_quality(src.hwQual());  
}                                                               


// =====
// EtSum
// =====
template <>
void ICL1TObjectProducer<l1t::EtSum>::constructSpecific( l1t::EtSum const &src, ic::L1TObject &dest ) {
                
                int type = static_cast<int>( src.getType() );
                dest.set_sumType(type);
                //if (doBXloop_) {
		//	std::cout << "The sumType is: " << dest.sumType() <<"    ";
                //	std::cout << "The pt is:      " << src.pt()       <<"    "<< std::endl;
                //}
}


template <class T>
void ICL1TObjectProducer<T>::beginJob() {
  ic::StaticTree::tree_->Branch( branch_.c_str(), &ic_l1t_object_ );
}

template <class T>
void ICL1TObjectProducer<T>::endJob() {}
#endif
