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

// CMSSW includes
#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1Upgrade.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisEventDataFormat.h"

// ICHiggsTauTau Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"

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
//declare lt1muon ect..
//delete on destructor, clear in producer
//add it to a vector


  l1UpgradeData = l1Upgrade->getData();
  
  // set up output
  tree_=fs_->make<TTree>("L1Objects", "L1ObjectsTree");
  tree_->Branch("L1Upgrade", "L1Analysis::L1AnalysisL1UpgradeDataFormat", &l1UpgradeData, 32000, 3);
//instead of this branch 1 branch per object L1TMuon ect. l1UpgradeData is actual object to be saved wont need 2 lines above
//need something like 
/*void ICElectronProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &electrons_);
}*/ //for all lt1muon ect.. 



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

  /*L1Analysis::L1AnalysisEventDataFormat *productL1Event = new L1Analysis::L1AnalysisEventDataFormat();
  vector<ic::L1TEMuon> L1Muon;

  unsigned short int nL1TMuon = productL1Upgrade->nMuons;
  for(unsigned i=0; i<nL1TMuon; i++){
      
    if(productL1Upgrade->egBx[i] != 0){continue;}
      
    ic::L1TMuon thisEG;
    thisEG.set_pt (productL1Upgrade->egEt [i]);
    thisEG.set_eta(productL1Upgrade->egEta[i]);
    thisEG.set_phi(productL1Upgrade->egPhi[i]);
    L1Muon.push_back(thisEG);
  }*/


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


void ICL1ObjectProducer::beginJob() {

}

void ICL1ObjectProducer::endJob() {}

DEFINE_FWK_MODULE(ICL1ObjectProducer);
