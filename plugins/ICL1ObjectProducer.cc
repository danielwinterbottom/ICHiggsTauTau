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
  L1Muons_ = new std::vector<ic::L1TMuon>();
  L1EGammas_ = new std::vector<ic::L1TEGamma>();
  L1Taus_ = new std::vector<ic::L1TTau>();
  L1Jets_ = new std::vector<ic::L1TJet>();
  L1Sums_ = new std::vector<ic::L1TSum>();
  

  l1UpgradeData = l1Upgrade->getData();
  
  // set up output
  //tree_=fs_->make<TTree>("L1Objects", "L1ObjectsTree");
  //tree_->Branch("L1Upgrade", "L1Analysis::L1AnalysisL1UpgradeDataFormat", &l1UpgradeData, 32000, 3);


}

ICL1ObjectProducer::~ICL1ObjectProducer() 
{   
    delete L1Muons_;
    delete L1EGammas_;
    delete L1Taus_;
    delete L1Jets_;
    delete L1Sums_;
}

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

  L1Muons_->clear();
  L1EGammas_->clear();
  L1Taus_->clear();
  L1Jets_->clear();
  L1Sums_->clear();


  if (eg.isValid()){ 
    //l1Upgrade->SetEm(eg, maxL1Upgrade_);
    
    for (int ibx = eg->getFirstBX(); ibx <= eg->getLastBX(); ++ibx) {
        for (l1t::EGammaBxCollection::const_iterator it=eg->begin(ibx); it!=eg->end(ibx) && l1UpgradeData->nEGs<maxL1Upgrade_; it++){
            ic::L1TEGamma thisEGamma;
            thisEGamma.isolation = it->hwIso();
            ROOT::Math::PtEtaPhiEVector tempVector(it->pt(), it->eta(),it->phi(), it->et());
            thisEGamma.set_vector(tempVector);
            L1EGammas_->push_back(thisEGamma);
        }
    }
    
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Em not found. Branch will not be filled" << std::endl;
  }
  if (jet.isValid()){ 
    //l1Upgrade->SetJet(jet, maxL1Upgrade_);
    
    for (int ibx = jet->getFirstBX(); ibx <= jet->getLastBX(); ++ibx) {
        for (l1t::JetBxCollection::const_iterator it=jet->begin(ibx); it!=jet->end(ibx) && l1UpgradeData->nJets<maxL1Upgrade_; it++){
            ic::L1TJet thisJet;
            ROOT::Math::PtEtaPhiEVector tempVector(it->pt(), it->eta(),it->phi(), it->et());
            thisJet.set_vector(tempVector);
            L1Jets_->push_back(thisJet);
        }
    }
    
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Jets not found. Branch will not be filled" << std::endl;
  }

  if (sums.isValid()){ 
    //l1Upgrade->SetSum(sums, maxL1Upgrade_);  
    
    for (int ibx = sums->getFirstBX(); ibx <= sums->getLastBX(); ++ibx) {
        for (l1t::EtSumBxCollection::const_iterator it=sums->begin(ibx); it!=sums->end(ibx) && l1UpgradeData->nSums<maxL1Upgrade_; it++){
            ic::L1TSum thisSum;
            int type = static_cast<int>( it->getType() );
            thisSum.sumType = type;
            thisSum.et = it->et();
            thisSum.phi = it->phi();
            L1Sums_->push_back(thisSum);
        }
    }
    
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade EtSums not found. Branch will not be filled" << std::endl;
  }

  if (muon.isValid()){ 
    //l1Upgrade->SetMuon(muon, maxL1Upgrade_);
    
    for (int ibx = muon->getFirstBX(); ibx <= muon->getLastBX(); ++ibx) {
        for (l1t::MuonBxCollection::const_iterator it=muon->begin(ibx); it!=muon->end(ibx) && l1UpgradeData->nMuons<maxL1Upgrade_; it++){
            ic::L1TMuon thisMuon;
            thisMuon.isolation = it->hwIso();
            thisMuon.charge = it->charge(); 
            thisMuon.quality = it->hwQual();
            ROOT::Math::PtEtaPhiEVector tempVector(it->pt(), it->eta(),it->phi(), it->et());
            thisMuon.set_vector(tempVector);
            L1Muons_->push_back(thisMuon);
        }
    }
    
    
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Muons not found. Branch will not be filled" << std::endl;
  }

  if (tau.isValid()){ //this was muon before not sure why???
    //l1Upgrade->SetTau(tau, maxL1Upgrade_);
    
    for (int ibx = tau->getFirstBX(); ibx <= tau->getLastBX(); ++ibx) {
        for (l1t::TauBxCollection::const_iterator it=tau->begin(ibx); it!=tau->end(ibx) && l1UpgradeData->nTaus<maxL1Upgrade_; it++){
            ic::L1TTau thisTau;
            thisTau.isolation = it->hwIso();
            ROOT::Math::PtEtaPhiEVector tempVector(it->pt(), it->eta(),it->phi(), it->et());
            thisTau.set_vector(tempVector);
            L1Taus_->push_back(thisTau);
        }
    }
    
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Tau not found. Branch will not be filled" << std::endl;
  }

  tree_->Fill();

}


void ICL1ObjectProducer::beginJob() {
    
  ic::StaticTree::tree_->Branch(branch_.c_str(), &L1Muons_);
  ic::StaticTree::tree_->Branch(branch_.c_str(), &L1Taus_);
  ic::StaticTree::tree_->Branch(branch_.c_str(), &L1EGammas_);
  ic::StaticTree::tree_->Branch(branch_.c_str(), &L1Jets_);
  ic::StaticTree::tree_->Branch(branch_.c_str(), &L1Sums_);
  
}

void ICL1ObjectProducer::endJob() {}

DEFINE_FWK_MODULE(ICL1ObjectProducer);
