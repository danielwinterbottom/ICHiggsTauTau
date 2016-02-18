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
    : input_(pset.getParameter<edm::InputTag>("input")) {
  consumes<edm::View<reco::Candidate>>(input_);
  candidates_ = new std::vector<ic::Candidate>();
  PrintHeaderWithProduces(pset, input_, branch_);

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
 
  //maxL1Upgrade_ = pset.getParameter<unsigned int>("maxL1Upgrade");
  maxL1Upgrade_ = 1000;
  l1Upgrade = new L1Analysis::L1AnalysisL1Upgrade();
  l1UpgradeData = l1Upgrade->getData();
  
  // set up output
  tree_=fs_->make<TTree>("L1Objects", "L1ObjectsTree");
  tree_->Branch("L1Upgrade", "L1Analysis::L1AnalysisL1UpgradeDataFormat", &l1UpgradeData, 32000, 3);

}

ICL1ObjectProducer::~ICL1ObjectProducer() { delete candidates_; }

void ICL1ObjectProducer::produce(edm::Event& event,
                                  const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::Candidate> > cand_handle;
  event.getByLabel(input_, cand_handle);

  candidates_->clear();
  candidates_->resize(cand_handle->size(), ic::Candidate());

  for (unsigned i = 0; i < cand_handle->size(); ++i) {
    reco::Candidate const& src = cand_handle->at(i);
    ic::Candidate & dest = candidates_->at(i);
    dest.set_id(cand_hasher_(&src));
    dest.set_pt(src.pt());
    dest.set_eta(src.eta());
    dest.set_phi(src.phi());
    dest.set_energy(src.energy());
    dest.set_charge(src.charge());
  }

}

//
// member functions
//

// ------------ method called to for each event  ------------

void ICL1ObjectProducer::analyze(const edm::Event& iEvent,
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

void ICL1ObjectProducer::beginJob() {
  //ic::StaticTree::tree_->Branch(branch_.c_str(), &candidates_);
}

void ICL1ObjectProducer::endJob() {}

DEFINE_FWK_MODULE(ICL1ObjectProducer);
