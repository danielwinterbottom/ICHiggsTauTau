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

#include "DataFormats/Common/interface/ValueMap.h"                  
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



template <class ICL1TObj_template>
class ICL1TObjectProducer : public edm::EDProducer {
 
 public:
   explicit ICL1TObjectProducer(const edm::ParameterSet &);
   ~ICL1TObjectProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();
  void constructSpecific(edm::Handle<ICL1TObj_template> const& candidate_collection,
                         edm::Event& event, const edm::EventSetup& setup);
  
  edm::InputTag input_;
  std::string branch_;
  std::string input_TObject_type_;

  std::vector<ic::ICL1TObject> *ICL1TObj_;
  edm::EDGetTokenT<ICL1TObj_template> m_EDToken_L1TObj;
  
  // output file
  edm::Service<TFileService> fs_;
  
  // tree
  TTree * tree_;
};


// =============================
// Template class implementation
// =============================


template <class ICL1TObj_template>
ICL1TObjectProducer<ICL1TObj_template>::ICL1TObjectProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      input_TObject_type_(config.getParameter<std::string>("TObject_type")) {
  
  consumes<edm::View<ICL1TObj_template>>(input_);
  produces<edm::ValueMap<double> >();
  
  ICL1TObj_ = new std::vector<ic::ICL1TObject>();
  
  PrintHeaderWithProduces(config, input_, branch_);
  
//  PrintOptional(1, input_TObject_type_, "TObject_type");       // ???
  
  edm::InputTag inputTag_TObject = config.getUntrackedParameter<edm::InputTag>( ("inputTag_"+branch_).c_str(), edm::InputTag("simCaloStage2Digis") );
  m_EDToken_L1TObj = consumes<ICL1TObj_template>(inputTag_TObject);

}

template <class ICL1TObj_template>
ICL1TObjectProducer<ICL1TObj_template>::~ICL1TObjectProducer() {   
  delete ICL1TObj_;
}



// =============
// Main producer
// =============


template <class ICL1TObj_template>
void ICL1TObjectProducer<ICL1TObj_template>::produce(edm::Event& event, const edm::EventSetup& setup) {
  
  edm::Handle<ICL1TObj_template> candidate_collection;
  event.getByLabel(input_, candidate_collection);

  constructSpecific(candidate_collection, event, setup);

  event.getByToken(m_EDToken_L1TObj, candidate_collection); //????
  ICL1TObj_->clear();
}



// ==================
// Specific producers
// ==================
template <class ICL1TObj_template>
void ICL1TObjectProducer<ICL1TObj_template>::constructSpecific(edm::Handle<ICL1TObj_template> const& candidate_collection,
                                                               edm::Event& event, 
                                                               const edm::EventSetup& setup) {

// common 

}

// ======
// EGamma
// ======                                                               
template <>
void ICL1TObjectProducer<l1t::EGammaBxCollection>::constructSpecific( edm::Handle<l1t::EGammaBxCollection> const& EGammaBxCollection_,
                                                                      edm::Event& event, 
                                                                      const edm::EventSetup& setup) {
    
    if (EGammaBxCollection_.isValid()){ 
        for (int ibx = EGammaBxCollection_->getFirstBX(); ibx <= EGammaBxCollection_->getLastBX(); ++ibx) {
            int i = 0;
            for (typename l1t::EGammaBxCollection::const_iterator it=EGammaBxCollection_->begin(ibx); it!=EGammaBxCollection_->end(ibx); it++){
                ic::ICL1TObject EGamma_;
                
                EGamma_.isolation = it->hwIso();  
                
                ROOT::Math::PtEtaPhiEVector tempVector(it->pt(), it->eta(),it->phi(), it->energy());
                EGamma_.set_vector(tempVector);
                ICL1TObj_->push_back(EGamma_);
                //std::cout << i <<" "<< it.bx() << std::endl;
                //std::cout << i <<" "<< it.pt() << std::endl;
                ++i;
            }
        }
    } 
    else {
        edm::LogWarning("MissingProduct ") << "L1Upgrade EGamma not found. Branch will not be filled" << std::endl;
    }
    
//  edm::ValueMap<double>::Filler filler(*product);
//  filler.insert(candidate_collection, values.begin(), values.end());
//  filler.fill();
    
//  event.put(product);
}                                                               


// ===
// Jet
// ===
template <>
void ICL1TObjectProducer<l1t::JetBxCollection>::constructSpecific( edm::Handle<l1t::JetBxCollection> const& JetBxCollection_,
                                                                   edm::Event& event,
                                                                   const edm::EventSetup& setup) {

  if (JetBxCollection_.isValid()){ 
      for (int ibx = JetBxCollection_->getFirstBX(); ibx <= JetBxCollection_->getLastBX(); ++ibx) {
          int i = 0;
          for (typename l1t::JetBxCollection::const_iterator it=JetBxCollection_->begin(ibx); it!=JetBxCollection_->end(ibx); it++){
               ic::ICL1TObject Jet_;
          
               
         
               ROOT::Math::PtEtaPhiEVector tempVector(it->pt(), it->eta(),it->phi(), it->energy());
               Jet_.set_vector(tempVector);
               ICL1TObj_->push_back(Jet_);
               ++i;
          }
      }
  } 
  else {
      edm::LogWarning("MissingProduct") << "L1Upgrade Jet not found. Branch will not be filled" << std::endl;
  }
 
//   edm::ValueMap<double>::Filler filler(*product);
//   filler.insert(candidate_collection, values.begin(), values.end());
//   filler.fill();
 
 //  event.put(product);
}                                                                                    


// ===
// Tau
// ===
template <>
void ICL1TObjectProducer<l1t::TauBxCollection>::constructSpecific( edm::Handle<l1t::TauBxCollection> const& TauBxCollection_,
                                                                   edm::Event& event,
                                                                   const edm::EventSetup& setup) {

    if (TauBxCollection_.isValid()){ 
        for (int ibx = TauBxCollection_->getFirstBX(); ibx <= TauBxCollection_->getLastBX(); ++ibx) {
            int i = 0;
            for (typename l1t::TauBxCollection::const_iterator it=TauBxCollection_->begin(ibx); it!=TauBxCollection_->end(ibx); it++){
                ic::ICL1TObject Tau_;
                
                Tau_.isolation = it->hwIso();  
        
                ROOT::Math::PtEtaPhiEVector tempVector(it->pt(), it->eta(),it->phi(), it->energy());
                Tau_.set_vector(tempVector);
                ICL1TObj_->push_back(Tau_);
                ++i;
            }
        }
    } 
    else {
        edm::LogWarning("MissingProduct") << "L1Upgrade Tau not found. Branch will not be filled" << std::endl;
    }

//  edm::ValueMap<double>::Filler filler(*product);
//  filler.insert(candidate_collection, values.begin(), values.end());
//  filler.fill();
//
//  event.put(product);
}                                                               


// ====
// Muon                                                                
// ====                                                                
template <>
void ICL1TObjectProducer<l1t::MuonBxCollection>::constructSpecific( edm::Handle<l1t::MuonBxCollection> const& MuonBxCollection_,
                                                                    edm::Event& event, 
                                                                    const edm::EventSetup& setup) {

  if (MuonBxCollection_.isValid()){ 
      for (int ibx = MuonBxCollection_->getFirstBX(); ibx <= MuonBxCollection_->getLastBX(); ++ibx) {
          int i = 0;
          for (typename l1t::MuonBxCollection::const_iterator it=MuonBxCollection_->begin(ibx); it!=MuonBxCollection_->end(ibx); it++){
              ic::ICL1TObject Muon_;
              
              Muon_.isolation = it->hwIso();
              Muon_.charge    = it->charge();
              Muon_.quality   = it->hwQual();  
              
              ROOT::Math::PtEtaPhiEVector tempVector(it->pt(), it->eta(),it->phi(), it->energy());
              Muon_.set_vector(tempVector);
              ICL1TObj_->push_back(Muon_);
              ++i;
          }
      }
  } 
  else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Muon not found. Branch will not be filled" << std::endl;
  }

//  edm::ValueMap<double>::Filler filler(*product);
//  filler.insert(candidate_collection, values.begin(), values.end());
//  filler.fill();
//
//  event.put(product);
}                                                               


// =====
// EtSum
// =====
template <>
void ICL1TObjectProducer<l1t::EtSumBxCollection>::constructSpecific( edm::Handle<l1t::EtSumBxCollection> const& EtSumBxCollection_,
                                                                     edm::Event& event, 
                                                                     const edm::EventSetup& setup) {

  if (EtSumBxCollection_.isValid()){ 
      for (int ibx = EtSumBxCollection_->getFirstBX(); ibx <= EtSumBxCollection_->getLastBX(); ++ibx) {
          int i = 0;
          for (typename l1t::EtSumBxCollection::const_iterator it=EtSumBxCollection_->begin(ibx); it!=EtSumBxCollection_->end(ibx); it++){
              ic::ICL1TObject EtSum_;
              
              int type = static_cast<int>( it->getType() );
              EtSum_.sumType = type;
              //std::cout << type <<"    "<< std::endl;
              
              ROOT::Math::PtEtaPhiEVector tempVector(it->pt(), it->eta(),it->phi(), it->energy());
              EtSum_.set_vector(tempVector);
              ICL1TObj_->push_back(EtSum_);
              ++i;
          }
      }
  } 
  else {
    edm::LogWarning("MissingProduct") << "L1Upgrade EtSum not found. Branch will not be filled" << std::endl;
  }

//  edm::ValueMap<double>::Filler filler(*product);
//  filler.insert(candidate_collection, values.begin(), values.end());
//  filler.fill();
//
//  event.put(product);
}                                                               





template <class ICL1TObj_template>
void ICL1TObjectProducer<ICL1TObj_template>::beginJob() {
ic::StaticTree::tree_->Branch(branch_.c_str(),   &ICL1TObj_);  
}

template <class ICL1TObj_template>
void ICL1TObjectProducer<ICL1TObj_template>::endJob() {}                                                               


#endif
