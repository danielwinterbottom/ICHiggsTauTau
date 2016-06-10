#include "UserCode/ICHiggsTauTau/plugins/ICL1TObjectProducer.hh"
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
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// ROOT output stuff
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

// CMSSW includes
#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1Upgrade.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisEventDataFormat.h"

// ICHiggsTauTau Objects
#include "UserCode/ICHiggsTauTau/interface/ICL1TObject.hh"


ICL1TObjectProducer::ICL1TObjectProducer(const edm::ParameterSet& pset) 
    : input_(pset.getParameter<edm::InputTag>("input")),
      branch_(pset.getParameter<std::string>("branch"))
  {
  
    consumes<edm::View<ICL1TObj_template>>(input_);
    ICL1TObj_ = new std::vector<ic::ICL1TObject>();
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

  ICL1TObj_   = new std::vector<ic::ICL1TObject>();
 // L1Muons_   = new std::vector<ic::L1TMuon>();
 // L1EGammas_ = new std::vector<ic::L1TEGamma>();
 // L1Taus_    = new std::vector<ic::L1TTau>();
 // L1Jets_    = new std::vector<ic::L1TJet>();
 // L1Sums_    = new std::vector<ic::L1TSum>();

}

ICL1TObjectProducer::~ICL1TObjectProducer() 
{   
    delete ICL1TObj_;
    //delete L1Muons_;
    //delete L1EGammas_;
    //delete L1Taus_;
    //delete L1Jets_;
    //delete L1Sums_;
}

void ICL1TObjectProducer::produce(edm::Event& iEvent,
                                   const edm::EventSetup& iSetup) {

  edm::Handle<ICL1TObj_template> generic_method;
  //edm::Handle<l1t::EGammaBxCollection> eg;
  //edm::Handle<l1t::JetBxCollection>    jet;
  //edm::Handle<l1t::EtSumBxCollection>  sums;
  //edm::Handle<l1t::MuonBxCollection>   muon; 
  //edm::Handle<l1t::TauBxCollection>    tau; 

  iEvent.getByToken(m_EDToken_L1TObj,generic_method);
  //iEvent.getByToken(m_EDToken_L1TEGamma,eg);
  //iEvent.getByToken(m_EDToken_L1TJet,   jet);
  //iEvent.getByToken(m_EDToken_L1TSum,   sums);
  //iEvent.getByToken(m_EDToken_L1TMuon,  muon);
  //iEvent.getByToken(m_EDToken_L1TTau,   tau);

  ICL1TObj_   ->clear();
  //L1Muons_  ->clear();
  //L1EGammas_->clear();
  //L1Taus_   ->clear();
  //L1Jets_   ->clear();
  //L1Sums_   ->clear();

  

  if (generic_method.isValid()){ 
   for (int ibx = generic_method->getFirstBX(); ibx <= generic_method->getLastBX(); ++ibx) {
    for (ICL1TObj_template::const_iterator it=generic_method->begin(ibx); it!=generic_method->end(ibx); it++){
      ic::ICL1TObject thisTObject;
      
      int type = static_cast<int>( it->getType() );
      thisTObject.sumType = type;
      
      thisTObject.isolation = it->hwIso(); //valid for egamma, muon, tau   not for jet, sum
      thisTObject.charge    = it->charge();//valid for muon                not for egamma, jet, sum
      thisTObject.quality   = it->hwQual();//valid for muon                not for egamma, jet, sum
      thisTObject.bx        = it->bx();

      ROOT::Math::PtEtaPhiEVector tempVector(it->pt(), it->eta(),it->phi(), it->energy());
      thisTObject.set_vector(tempVector);
      ICL1TObj_->push_back(thisTObject);
      std::cout << i <<" "<< it.bx() << std::endl;
      std::cout << i <<" "<< it.pt() << std::endl;
      std::cout << type <<"    "<< std::endl;
    }
   }
  } 
  else {
    edm::LogWarning("MissingProduct") << "L1Upgrade Em not found. Branch will not be filled" << std::endl;
  }
}


void ICL1TObjectProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.str(),   &ICL1TObj_);  
  //ic::StaticTree::tree_->Branch("L1Muons",   &L1Muons_);
  //ic::StaticTree::tree_->Branch("L1Taus",    &L1Taus_);
  //ic::StaticTree::tree_->Branch("L1EGammas", &L1EGammas_);
  //ic::StaticTree::tree_->Branch("L1Jets",    &L1Jets_);
  //ic::StaticTree::tree_->Branch("L1Sums",    &L1Sums_);
  
}

void ICL1TObjectProducer::endJob() {}

DEFINE_FWK_MODULE(ICL1TObjectProducer);
