#include "UserCode/ICHiggsTauTau/plugins/ICTauSpinnerProducer.hh"
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
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"


ICTauSpinnerProducer::ICTauSpinnerProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      theta_(config.getParameter<double>("theta")){
  consumes<edm::View<reco::GenParticle>>(input_);

  PrintHeaderWithProduces(config, input_, branch_);
  TauSpinnerSettingsPDF="NNPDF30_nlo_as_0118";
  TauSpinnerSettingsIpp=true;
  TauSpinnerSettingsIpol=1;
  TauSpinnerSettingsNonSM2=0;
  TauSpinnerSettingsNonSMN=0;
  TauSpinnerSettingsCmsEnergy=13.;
  bosonPdgId_=25;
}

ICTauSpinnerProducer::~ICTauSpinnerProducer() {}

reco::GenParticle ICTauSpinnerProducer::getBoson(edm::Handle<edm::View<reco::GenParticle> > parts_handle){
  // Function return last copy of generator boson
  reco::GenParticle boson;
  bool foundBoson = false;
  for (unsigned i = 0; i < parts_handle->size(); ++i) {
    reco::GenParticle const& part = parts_handle->at(i);
    bool isLastCopy = part.isLastCopy();
    int part_pdgid = fabs(part.pdgId());
    if (part_pdgid==bosonPdgId_&&isLastCopy){
        boson = part;
        foundBoson = true;
        break;
    }
  }
  if(!foundBoson) {std::cout << "ICTauSpinnerProducer: Gen boson not found. Throwing exception." << std::endl; throw;}
  return boson;
}

std::vector<reco::GenParticle> ICTauSpinnerProducer::getTaus(reco::GenParticle boson, edm::Handle<edm::View<reco::GenParticle> > parts_handle){
  std::vector<reco::GenParticle> taus;
  unsigned nTaus=0;
  for (unsigned i = 0; i < boson.daughterRefVector().size(); ++i) {
    int daughter_index = static_cast<int>(boson.daughterRefVector().at(i).key());
    reco::GenParticle daughter = parts_handle->at(daughter_index);
    int daughter_pdgid = fabs(daughter.pdgId());
    if (daughter_pdgid != 15) continue;
    taus.push_back(daughter); 
    nTaus++;
  }
  if(nTaus!=2) {std::cout << "ICTauSpinnerProducer: Found " << nTaus << " taus, expected 2. Throwing exception." << std::endl; throw;}
  return taus;
}

void ICTauSpinnerProducer::getTauDaughters(std::vector<reco::GenParticle> &tau_daughters, reco::GenParticle tau, edm::Handle<edm::View<reco::GenParticle> > parts_handle){
  for (unsigned i = 0; i < tau.daughterRefVector().size(); ++i) {
    int daughter_index = static_cast<int>(tau.daughterRefVector().at(i).key());
    reco::GenParticle daughter = parts_handle->at(daughter_index);
    int daughter_pdgid = fabs(daughter.pdgId());
    if(daughter_pdgid == 22 || daughter_pdgid == 111 || daughter_pdgid == 211 || daughter_pdgid == 321 || daughter_pdgid == 130 || daughter_pdgid == 310 || daughter_pdgid == 11 || daughter_pdgid == 12 || daughter_pdgid == 13 || daughter_pdgid == 14 || daughter_pdgid == 16) tau_daughters.push_back(daughter);
    else getTauDaughters(tau_daughters, daughter, parts_handle);
  }

}

TauSpinner::SimpleParticle ICTauSpinnerProducer::ConvertToSimplePart(reco::GenParticle input_part){
  TauSpinner::SimpleParticle simple_part;
  simple_part.setPx(input_part.px());
  simple_part.setPy(input_part.py());
  simple_part.setPz(input_part.pz());
  simple_part.setE(input_part.energy());
  simple_part.setPdgid(input_part.pdgId());
  return simple_part;  
}

void ICTauSpinnerProducer::initialize(){
  Tauolapp::Tauola::initialize();
  LHAPDF::initPDFSetByName(TauSpinnerSettingsPDF);
  
  TauSpinner::initialize_spinner(TauSpinnerSettingsIpp,
                                 TauSpinnerSettingsIpol,
                                 TauSpinnerSettingsNonSM2,
                                 TauSpinnerSettingsNonSMN,
                                 TauSpinnerSettingsCmsEnergy);
}

void ICTauSpinnerProducer::produce(edm::Event& event,
                                  const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::GenParticle> > parts_handle;
  event.getByLabel(input_, parts_handle);
  
  reco::GenParticle boson = getBoson(parts_handle);
  std::vector<reco::GenParticle> taus = getTaus(boson,parts_handle);
  
  std::vector<reco::GenParticle> tau1_daughters;
  std::vector<reco::GenParticle> tau2_daughters;
  getTauDaughters(tau1_daughters,taus[0],parts_handle);  
  getTauDaughters(tau2_daughters,taus[0],parts_handle); 
  
  TauSpinner::SimpleParticle simple_boson = ConvertToSimplePart(boson);
  TauSpinner::SimpleParticle simple_tau1 = ConvertToSimplePart(taus[0]);
  TauSpinner::SimpleParticle simple_tau2 = ConvertToSimplePart(taus[1]);
  std::vector<TauSpinner::SimpleParticle> simple_tau1_daughters;
  std::vector<TauSpinner::SimpleParticle> simple_tau2_daughters;
  
  for(unsigned i=0; i<tau1_daughters.size(); ++i) simple_tau1_daughters.push_back(ConvertToSimplePart(tau1_daughters[i]));
  for(unsigned i=0; i<tau2_daughters.size(); ++i) simple_tau2_daughters.push_back(ConvertToSimplePart(tau2_daughters[i]));
  
  // Can make this more general by having boson pdgid as input or have option for set boson type
  TauSpinner::setHiggsParametersTR(-cos(2*M_PI*theta_),cos(2*M_PI*theta_),-sin(2*M_PI*theta_),-sin(2*M_PI*theta_));
  weight_ = TauSpinner::calculateWeightFromParticlesH(simple_boson,simple_tau1,simple_tau2,simple_tau1_daughters,simple_tau2_daughters);
  //std::cout << weight_ << std::endl;
  std::cout << TauSpinner::getWtNonSM() << std::endl;

}

void ICTauSpinnerProducer::beginJob() {
  initialize();  
  //ic::StaticTree::tree_->Branch(branch_.c_str(), &candidates_);
}

void ICTauSpinnerProducer::endJob() {}

DEFINE_FWK_MODULE(ICTauSpinnerProducer);
