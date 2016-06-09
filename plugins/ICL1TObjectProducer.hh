#ifndef UserCode_ICHiggsTauTau_ICL1TObjectProducer_h
#define UserCode_ICHiggsTauTau_ICL1TObjectProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

// data formats
#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "DataFormats/L1Trigger/interface/Tau.h"
#include "DataFormats/L1Trigger/interface/Jet.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"

// ROOT output stuff
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

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

#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1Upgrade.h"

// ICHiggsTauTau Objects
#include "UserCode/ICHiggsTauTau/interface/ICL1TObject.hh"

/**
 * @brief See documentation [here](\ref objs-candidate)
 */


 

//ICL1TObjectProducer<lt1::EGammaBxCollection> obj;

template <class ICL1TObj_template>
class ICL1TObjectProducer : public edm::EDProducer {
 public:
   explicit ICL1TObjectProducer(const edm::ParameterSet &);
   ~ICL1TObjectProducer();

 private:
  virtual void beginJob();
    virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

 public:
  L1Analysis::L1AnalysisL1Upgrade* l1Upgrade;
  L1Analysis::L1AnalysisL1UpgradeDataFormat* l1UpgradeData;

 private:
     
    std::vector<ic::ICL1TObject> *ICL1TObj_;
  
  std::string branch_;
  boost::hash<reco::Candidate const*> cand_hasher_;

  unsigned maxL1Upgrade_;
  std::string m_L1TEra;

  // output file
  edm::Service<TFileService> fs_;
  
  // tree
  TTree * tree_;
  
  edm::EDGetTokenT<ICL1TObj_template> m_EDToken_L1TObj;
//   edm::EDGetTokenT<l1t::EGammaBxCollection> m_EDToken_L1TEGamma;
//   edm::EDGetTokenT<l1t::JetBxCollection>    m_EDToken_L1TJet;
//   edm::EDGetTokenT<l1t::EtSumBxCollection>  m_EDToken_L1TSum;
//   edm::EDGetTokenT<l1t::MuonBxCollection>   m_EDToken_L1TMuon;
//   edm::EDGetTokenT<l1t::TauBxCollection>    m_EDToken_L1TTau;
//   edm::EDGetTokenT<l1t::TauBxCollection>    m_EDToken_L1TTauIso;

};

#endif
