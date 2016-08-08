#ifndef UserCode_ICHiggsTauTau_ICL1ObjectProducer_h
#define UserCode_ICHiggsTauTau_ICL1ObjectProducer_h

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

// data formats
#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "DataFormats/L1Trigger/interface/Tau.h"
#include "DataFormats/L1Trigger/interface/Jet.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"

// ROOT output stuff
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1Upgrade.h"

// ICHiggsTauTau Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TSum.hh"

/**
 * @brief See documentation [here](\ref objs-candidate)
 */
class ICL1ObjectProducer : public edm::EDProducer {//edm::Analyzer {
 public:
  explicit ICL1ObjectProducer(const edm::ParameterSet &);
  ~ICL1ObjectProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

 public:
  L1Analysis::L1AnalysisL1Upgrade* l1Upgrade;
  L1Analysis::L1AnalysisL1UpgradeDataFormat * l1UpgradeData;

 private:
     
  std::vector<ic::L1TMuon> *L1Muons_;
  std::vector<ic::L1TEGamma> *L1EGammas_;
  std::vector<ic::L1TTau> *L1Taus_;
  std::vector<ic::L1TJet> *L1Jets_;
  std::vector<ic::L1TSum> *L1Sums_;
  
  std::string branch_;
  boost::hash<reco::Candidate const*> cand_hasher_;

  // output file
  edm::Service<TFileService> fs_;
  
  // tree
  TTree * tree_;
  
  edm::EDGetTokenT<l1t::EGammaBxCollection> m_EDToken_L1TEGamma;
  edm::EDGetTokenT<l1t::JetBxCollection> m_EDToken_L1TJet;
  edm::EDGetTokenT<l1t::EtSumBxCollection> m_EDToken_L1TSum;
  edm::EDGetTokenT<l1t::MuonBxCollection> m_EDToken_L1TMuon;
  edm::EDGetTokenT<l1t::TauBxCollection> m_EDToken_L1TTau;
  edm::EDGetTokenT<l1t::TauBxCollection> m_EDToken_L1TTauIso;

};

#endif
