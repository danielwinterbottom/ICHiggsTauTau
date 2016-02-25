#include "UserCode/ICHiggsTauTau/plugins/ICL1ObjectProducer.hh"
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
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"

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

ICL1ObjectProducer::ICL1ObjectProducer(const edm::ParameterSet& pset)
  :    branch_(pset.getParameter<std::string>("branch")) {

  m_L1TEra = pset.getUntrackedParameter<std::string>("input_L1TEra",string("stage2"));
  if(m_L1TEra == "stage1"){
    
    edm::InputTag inputTag_EGamma = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TEGamma",edm::InputTag("simCaloStage1FinalDigis"));
    edm::InputTag inputTag_Muon   = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TMuon",  edm::InputTag("None"));
    edm::InputTag inputTag_Tau    = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TTau",   edm::InputTag("simCaloStage1FinalDigis:rlxTaus"));
    edm::InputTag inputTag_TauIso = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TTauIso",edm::InputTag("simCaloStage1FinalDigis:isoTaus"));
    edm::InputTag inputTag_Jet    = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TJet",   edm::InputTag("simCaloStage1FinalDigis"));
    edm::InputTag inputTag_Sum    = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TSum",   edm::InputTag("simCaloStage1FinalDigis"));
    
    m_EDToken_L1TEGamma = consumes<l1t::EGammaBxCollection>(inputTag_EGamma);
    m_EDToken_L1TMuon   = consumes<l1t::MuonBxCollection>  (inputTag_Muon);
    m_EDToken_L1TTau    = consumes<l1t::TauBxCollection>   (inputTag_Tau);
    m_EDToken_L1TTauIso = consumes<l1t::TauBxCollection>   (inputTag_TauIso);
    m_EDToken_L1TJet    = consumes<l1t::JetBxCollection>   (inputTag_Jet);
    m_EDToken_L1TSum    = consumes<l1t::EtSumBxCollection> (inputTag_Sum);
  }
  else if(m_L1TEra == "stage2"){
    
    edm::InputTag inputTag_EG   = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TEGamma",edm::InputTag("simCaloStage2Digis"));
    edm::InputTag inputTag_Muon = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TMuon",  edm::InputTag("simGmtStage2Digis"));
    edm::InputTag inputTag_Tau  = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TTau",   edm::InputTag("simCaloStage2Digis"));
    edm::InputTag inputTag_Jet  = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TJet",   edm::InputTag("simCaloStage2Digis"));
    edm::InputTag inputTag_Sum  = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TSum",   edm::InputTag("simCaloStage2Digis"));
    
    m_EDToken_L1TEGamma = consumes<l1t::EGammaBxCollection>(inputTag_EG);
    m_EDToken_L1TMuon   = consumes<l1t::MuonBxCollection>  (inputTag_Muon);
    m_EDToken_L1TTau    = consumes<l1t::TauBxCollection>   (inputTag_Tau);
    m_EDToken_L1TJet    = consumes<l1t::JetBxCollection>   (inputTag_Jet);
    m_EDToken_L1TSum    = consumes<l1t::EtSumBxCollection> (inputTag_Sum);
  }
 
  maxL1Upgrade_ = pset.getParameter<unsigned int>("maxL1Upgrade");
  l1Upgrade = new L1Analysis::L1AnalysisL1Upgrade();
  l1UpgradeData = l1Upgrade->getData();
  
  // set up output
  tree_=fs_->make<TTree>("L1Objects", "L1ObjectsTree");
  tree_->Branch("L1Upgrade", "L1Analysis::L1AnalysisL1UpgradeDataFormat", &l1UpgradeData, 32000, 3);

}

ICL1ObjectProducer::~ICL1ObjectProducer() {  }

void ICL1ObjectProducer::produce(edm::Event& iEvent,
                                   const edm::EventSetup& iSetup) {
  l1Upgrade->Reset();

  edm::Handle<l1t::EGammaBxCollection> eg;
  edm::Handle<l1t::JetBxCollection> jet;
  edm::Handle<l1t::EtSumBxCollection> sums;
  edm::Handle<l1t::MuonBxCollection> muon; 
  edm::Handle<l1t::TauBxCollection> tau; 

  iEvent.getByToken(m_EDToken_L1TEGamma,   eg);
  iEvent.getByToken(m_EDToken_L1TJet,  jet);
  iEvent.getByToken(m_EDToken_L1TSum, sums);
  iEvent.getByToken(m_EDToken_L1TMuon, muon);
  iEvent.getByToken(m_EDToken_L1TTau, tau);


  if (eg.isValid()){ 
    l1Upgrade->SetEm(eg, maxL1Upgrade_);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Em not found. Branch will not be filled" << std::endl;
  }
  if (jet.isValid()){ 
    l1Upgrade->SetJet(jet, maxL1Upgrade_);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Jets not found. Branch will not be filled" << std::endl;
  }

  if (sums.isValid()){ 
    l1Upgrade->SetSum(sums, maxL1Upgrade_);  
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade EtSums not found. Branch will not be filled" << std::endl;
  }

  if (muon.isValid()){ 
    l1Upgrade->SetMuon(muon, maxL1Upgrade_);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Muons not found. Branch will not be filled" << std::endl;
  }

  if (muon.isValid()){ 
    l1Upgrade->SetTau(tau, maxL1Upgrade_);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Tau not found. Branch will not be filled" << std::endl;
  }

  tree_->Fill();

}


void ICL1ObjectProducer::analyze(const edm::Event& iEvent,
                                   const edm::EventSetup& iSetup) {
  
  /*l1Upgrade->Reset();

  edm::Handle<l1t::EGammaBxCollection> eg;
  edm::Handle<l1t::JetBxCollection> jet;
  edm::Handle<l1t::EtSumBxCollection> sums;
  edm::Handle<l1t::MuonBxCollection> muon; 
  edm::Handle<l1t::TauBxCollection> tau; 

  iEvent.getByToken(m_EDToken_L1TEGamma,   eg);
  iEvent.getByToken(m_EDToken_L1TJet,  jet);
  iEvent.getByToken(m_EDToken_L1TSum, sums);
  iEvent.getByToken(m_EDToken_L1TMuon, muon);
  iEvent.getByToken(m_EDToken_L1TTau, tau);


  if (eg.isValid()){ 
    l1Upgrade->SetEm(eg, maxL1Upgrade_);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Em not found. Branch will not be filled" << std::endl;
  }
  if (jet.isValid()){ 
    l1Upgrade->SetJet(jet, maxL1Upgrade_);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Jets not found. Branch will not be filled" << std::endl;
  }

  if (sums.isValid()){ 
    l1Upgrade->SetSum(sums, maxL1Upgrade_);  
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade EtSums not found. Branch will not be filled" << std::endl;
  }

  if (muon.isValid()){ 
    l1Upgrade->SetMuon(muon, maxL1Upgrade_);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Muons not found. Branch will not be filled" << std::endl;
  }

  if (muon.isValid()){ 
    l1Upgrade->SetTau(tau, maxL1Upgrade_);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Tau not found. Branch will not be filled" << std::endl;
  }

  tree_->Fill();*/

}
/*
void L1Analysis::L1AnalysisL1Upgrade::SetEm(const edm::Handle<l1t::EGammaBxCollection> em, unsigned maxL1Upgrade)
{
  for (int ibx = em->getFirstBX(); ibx <= em->getLastBX(); ++ibx) {
    for (l1t::EGammaBxCollection::const_iterator it=em->begin(ibx); it!=em->end(ibx) && l1upgrade_.nEGs<maxL1Upgrade; it++){
      if (it->pt() > 0){
	l1upgrade_.egEt .push_back(it->pt());
	l1upgrade_.egEta.push_back(it->eta());
	l1upgrade_.egPhi.push_back(it->phi());
	l1upgrade_.egIEt .push_back(it->hwPt());
	l1upgrade_.egIEta.push_back(it->hwEta());
	l1upgrade_.egIPhi.push_back(it->hwPhi());
	l1upgrade_.egIso.push_back(it->hwIso());
	l1upgrade_.egBx .push_back(ibx);
	l1upgrade_.nEGs++;
      }
    }
  }
}


void L1Analysis::L1AnalysisL1Upgrade::SetTau(const edm::Handle<l1t::TauBxCollection> tau, unsigned maxL1Upgrade)
{
  for (int ibx = tau->getFirstBX(); ibx <= tau->getLastBX(); ++ibx) {
    for (l1t::TauBxCollection::const_iterator it=tau->begin(ibx); it!=tau->end(ibx) && l1upgrade_.nTaus<maxL1Upgrade; it++){
      if (it->pt() > 0){
	l1upgrade_.tauEt .push_back(it->et());
	l1upgrade_.tauEta.push_back(it->eta());
	l1upgrade_.tauPhi.push_back(it->phi());
	l1upgrade_.tauIEt .push_back(it->hwPt());
	l1upgrade_.tauIEta.push_back(it->hwEta());
	l1upgrade_.tauIPhi.push_back(it->hwPhi());
	l1upgrade_.tauIso.push_back(it->hwIso());
	l1upgrade_.tauBx .push_back(ibx);
	l1upgrade_.nTaus++;
      }
    }
  }
}


void L1Analysis::L1AnalysisL1Upgrade::SetJet(const edm::Handle<l1t::JetBxCollection> jet, unsigned maxL1Upgrade)
{
  for (int ibx = jet->getFirstBX(); ibx <= jet->getLastBX(); ++ibx) {
    for (l1t::JetBxCollection::const_iterator it=jet->begin(ibx); it!=jet->end(ibx) && l1upgrade_.nJets<maxL1Upgrade; it++){
      if (it->pt() > 0){
	l1upgrade_.jetEt .push_back(it->et());
	l1upgrade_.jetEta.push_back(it->eta());
	l1upgrade_.jetPhi.push_back(it->phi());
	l1upgrade_.jetIEt .push_back(it->hwPt());
	l1upgrade_.jetIEta.push_back(it->hwEta());
	l1upgrade_.jetIPhi.push_back(it->hwPhi());
	l1upgrade_.jetBx .push_back(ibx);
	l1upgrade_.nJets++;
      }
    }
  }
}


void L1Analysis::L1AnalysisL1Upgrade::SetMuon(const edm::Handle<l1t::MuonBxCollection> muon, unsigned maxL1Upgrade)
{
  for (int ibx = muon->getFirstBX(); ibx <= muon->getLastBX(); ++ibx) {
    for (l1t::MuonBxCollection::const_iterator it=muon->begin(ibx); it!=muon->end(ibx) && l1upgrade_.nMuons<maxL1Upgrade; it++){
      if (it->pt() > 0){
	l1upgrade_.muonEt .push_back(it->et());
	l1upgrade_.muonEta.push_back(it->eta());
	l1upgrade_.muonPhi.push_back(it->phi());
	l1upgrade_.muonIEt .push_back(it->hwPt());
	l1upgrade_.muonIEta.push_back(it->hwEta());
	l1upgrade_.muonIPhi.push_back(it->hwPhi());
	l1upgrade_.muonChg.push_back(0); //it->charge());
	l1upgrade_.muonIso.push_back(it->hwIso());
	l1upgrade_.muonQual.push_back(it->hwQual());
	l1upgrade_.muonBx .push_back(ibx);
	l1upgrade_.nMuons++;
      }
    }
  }
}

void L1Analysis::L1AnalysisL1Upgrade::SetSum(const edm::Handle<l1t::EtSumBxCollection> sums, unsigned maxL1Upgrade)
{
  for (int ibx = sums->getFirstBX(); ibx <= sums->getLastBX(); ++ibx) {
    for (l1t::EtSumBxCollection::const_iterator it=sums->begin(ibx); it!=sums->end(ibx) && l1upgrade_.nSums<maxL1Upgrade; it++) {
      int type = static_cast<int>( it->getType() ); 
      l1upgrade_.sumType. push_back( type ); 
      l1upgrade_.sumEt. push_back( it->et() ); 
      l1upgrade_.sumPhi.push_back( it->phi() );
      l1upgrade_.sumIEt. push_back( it->hwPt() ); 
      l1upgrade_.sumIPhi.push_back( it->hwPhi() );
      l1upgrade_.sumBx. push_back( ibx );
      l1upgrade_.nSums++;
    }
  }
}*/

void ICL1ObjectProducer::beginJob() {

}

void ICL1ObjectProducer::endJob() {}

DEFINE_FWK_MODULE(ICL1ObjectProducer);
