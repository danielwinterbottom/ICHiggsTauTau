#include "UserCode/ICHiggsTauTau/plugins/ICTauSpinnerProducer.hh"
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <TFile.h>
#include <TTree.h>
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
#include "Tauola/Tauola.h"
#include <TRandom3.h>

ICTauSpinnerProducer::ICTauSpinnerProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      theta_(config.getParameter<std::string>("theta"))
{
  consumes<edm::View<reco::GenParticle>>(input_);

  PrintHeaderWithProduces(config, input_, branch_);
  TauSpinnerSettingsPDF="NNPDF30_nlo_as_0118";
  Ipp=true;
  Ipol=0;
  nonSM2=0;
  nonSMN=0;
  CMSENE=13000.0;
  bosonPdgId_=25;
  info_ = new ic::EventInfo();
  outFile.open("weights.csv");
  rootFile = new TFile("smearingEffects_smearing1.root", "RECREATE");
  outTree = new TTree("outTree", "outTree");
}

ICTauSpinnerProducer::~ICTauSpinnerProducer()
{
	outFile.close();
}

std::vector<std::pair<std::string,double>> ICTauSpinnerProducer::SplitString(std::string instring){
  std::vector<std::pair<std::string,double>> out;
  std::stringstream ss(instring);   
  std::string splitstring;  
  while(std::getline(ss, splitstring, ',')){
    double val = std::stod(splitstring);
    if(splitstring.find(".") != std::string::npos) splitstring.replace(splitstring.find("."),1,"p");
    if(splitstring.find("-") != std::string::npos) splitstring.replace(splitstring.find("-"),1,"minus");
    std::string weight_name = "wt_cp_"+splitstring;    
    out.push_back(std::make_pair(weight_name,val)); 
  }
  return out;
}

reco::GenParticle ICTauSpinnerProducer::getBoson(edm::Handle<edm::View<reco::GenParticle> > parts_handle){
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

reco::GenParticle ICTauSpinnerProducer::getLastCopy(reco::GenParticle part, edm::Handle<edm::View<reco::GenParticle> > parts_handle) {
  reco::GenParticle last_part;
  reco::GenStatusFlags  flags = part.statusFlags();
  bool isLastCopy = flags.isLastCopy();

  if (!isLastCopy) {
    int pdgid = part.pdgId();
    for (unsigned i = 0; i < part.daughterRefVector().size(); ++i) {
      int daughter_index = static_cast<int>(part.daughterRefVector().at(i).key());
      reco::GenParticle daughter = parts_handle->at(daughter_index);
      int daughter_pdgid = daughter.pdgId();
      if (daughter_pdgid==pdgid) last_part = getLastCopy(daughter, parts_handle);
    }
  }
  else last_part =  part;
  return last_part;
}

std::vector<reco::GenParticle> ICTauSpinnerProducer::getTaus(reco::GenParticle boson, edm::Handle<edm::View<reco::GenParticle> > parts_handle){
  std::vector<reco::GenParticle> taus;
  unsigned nTaus=0;
  for (unsigned i = 0; i < boson.daughterRefVector().size(); ++i) {
    int daughter_index = static_cast<int>(boson.daughterRefVector().at(i).key());
    reco::GenParticle daughter = parts_handle->at(daughter_index);
    int daughter_pdgid = fabs(daughter.pdgId());
    if (daughter_pdgid != 15) continue;
    reco::GenParticle tau = getLastCopy(daughter, parts_handle);
    taus.push_back(tau);
    nTaus++;
  }
  if(nTaus!=2) {std::cout << "ICTauSpinnerProducer: Found " << nTaus << " taus, expected 2. Throwing exception." << std::endl; throw;}
  return taus;
}

void ICTauSpinnerProducer::getTauDaughters(std::vector<reco::GenParticle> &tau_daughters, unsigned &type, reco::GenParticle tau, edm::Handle<edm::View<reco::GenParticle> > parts_handle){
  for (unsigned i = 0; i < tau.daughterRefVector().size(); ++i) {
    int daughter_index = static_cast<int>(tau.daughterRefVector().at(i).key());
    reco::GenParticle daughter = parts_handle->at(daughter_index);
    int daughter_pdgid = fabs(daughter.pdgId());
    if(daughter_pdgid == 22 || daughter_pdgid == 111 || daughter_pdgid == 211 || daughter_pdgid == 321 || daughter_pdgid == 130 || daughter_pdgid == 310 || daughter_pdgid == 11 || daughter_pdgid == 12 || daughter_pdgid == 13 || daughter_pdgid == 14 || daughter_pdgid == 16){
      if(daughter_pdgid == 11) type = 1;
      if(daughter_pdgid == 13) type = 2;
      //neutrino smearing
      //if (daughter_pdgid == 12 || daughter_pdgid == 14 || daughter_pdgid == 16){
      
      //reco::Particle::PolarLorentzVector pppp(100.0, 200.0, 0.5, 400.0);
      //std::cout << "Old pt:" << daughter.pt() << '\n';
      //daughter.setP4(pppp);
      //std::cout << "New mass:" << daughter.mass() << '\n';
      //std::cout << "New phi:" << daughter.phi() << '\n';
      tau_daughters.push_back(daughter);
    }
    else getTauDaughters(tau_daughters, type, daughter, parts_handle);
  }
}

void ICTauSpinnerProducer::removeGammas(std::vector<TauSpinner::SimpleParticle> &tau_daughters){
    for(int i=tau_daughters.size()-1; i>=0; --i){
    int pdgid = fabs(tau_daughters[i].pdgid());
    if(pdgid == 22) {
      tau_daughters.erase(tau_daughters.begin()+i);
    }
  }
}

void ICTauSpinnerProducer::removeSecondGamma(std::vector<TauSpinner::SimpleParticle> &tau_daughters){
  int gamma_count=0;
  int gamma_index=0;
  double min_gamma_pt=0;
  for(unsigned i=0; i<tau_daughters.size(); ++i){
    int pdgid = fabs(tau_daughters[i].pdgid());
    if(pdgid == 22){
      double gamma_pt=sqrt(pow(tau_daughters[i].px(),2) + pow(tau_daughters[i].py(),2));
      if(gamma_count==0){ min_gamma_pt = gamma_pt; gamma_index=i;}
      else if (gamma_pt<min_gamma_pt){ min_gamma_pt = gamma_pt; gamma_index=i;}
      gamma_count++;
    }
  }
  if(gamma_count==2) tau_daughters.erase(tau_daughters.begin()+gamma_index);
}

bool ICTauSpinnerProducer::channelMatch(std::vector<reco::GenParticle> parts, std::vector<int> matches, bool signed_){
  std::vector<int> list;
  for(unsigned i=0; i<parts.size();++i) list.push_back(parts[i].pdgId());
                
  for(unsigned i=0;i<matches.size();i++) {
    bool found = false;  
    int match = matches[i];
    if(!signed_) match = fabs(match);
    
    for(unsigned j=0;j<list.size(); j++){
      int pdgid = list[j];
      if(!signed_) pdgid = fabs(pdgid);
      // if pdgid is found - erese it from the list and search for the next one
      if(pdgid==match) {
        found = true;
        list.erase(list.begin()+j);
        break;
      }
    }
    
    if(!found) return false;
  }
  if(list.size()!=0) return false;
  return true;
    
}

TauSpinner::SimpleParticle ICTauSpinnerProducer::ConvertToSimplePart(reco::GenParticle input_part){
  return TauSpinner::SimpleParticle(input_part.px(), input_part.py(), input_part.pz(), input_part.energy(), input_part.pdgId());
}

void ICTauSpinnerProducer::initialize(){
  Tauolapp::Tauola::setNewCurrents(1);
  Tauolapp::Tauola::initialize();
  LHAPDF::initPDFSetByName(TauSpinnerSettingsPDF);
  TauSpinner::initialize_spinner(Ipp, Ipol, nonSM2, nonSMN,  CMSENE);
}

void ICTauSpinnerProducer::produce(edm::Event& event,
                                  const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::GenParticle> > parts_handle;
  event.getByLabel(input_, parts_handle);
  
  reco::GenParticle boson = getBoson(parts_handle);
  std::vector<reco::GenParticle> taus = getTaus(boson,parts_handle);
  
  std::vector<reco::GenParticle> tau1_daughters;
  std::vector<reco::GenParticle> tau2_daughters;
  unsigned type1 = 0; 
  unsigned type2 = 0;
  getTauDaughters(tau1_daughters,type1,taus[0],parts_handle);  
  getTauDaughters(tau2_daughters,type2,taus[1],parts_handle); 
  
  //Check: can we get the parents tau eta and phi components ? That would help to give the 'default neutrino values'
  //std::cout << "\nThe pt of the mother of the first tau decay product: " << tau1_daughters[0].mother()->pt() << '\n';  
 
  TauSpinner::SimpleParticle simple_boson = ConvertToSimplePart(boson);
  TauSpinner::SimpleParticle simple_tau1 = ConvertToSimplePart(taus[0]);
  TauSpinner::SimpleParticle simple_tau2 = ConvertToSimplePart(taus[1]);
  std::vector<TauSpinner::SimpleParticle> simple_tau1_daughters;
  std::vector<TauSpinner::SimpleParticle> simple_tau2_daughters;
  
  // Set pdgIDs to 0 before adding existing particles
  for(unsigned int i=0; i<10; i++)
  {
  	tau1DaughtersArray[i][4] = 0;
  	tau2DaughtersArray[i][4] = 0;
  }
  
  for(unsigned i=0; i<tau1_daughters.size(); ++i){
		// Smearing neutrinos:
    int daughter1_pdgid = fabs(tau1_daughters[i].pdgId());
    // we should only do this for tau neutrinos !
    if (daughter1_pdgid == 16)
    {
      //smearing
      double smearing_nu1_pt = rndm.Gaus(1.65,32.56);
      double smearing_nu1_eta = rndm.Gaus(-0.01, 0.09);
      double smearing_nu1_phi = rndm.Gaus(0.09, 0.96);
      smearingArray[0] = smearing_nu1_pt;
      smearingArray[1] = smearing_nu1_eta;
      smearingArray[2] = smearing_nu1_phi;
      std::cout << "Random value:" << smearing_nu1_pt << " pt1 " << smearing_nu1_eta<< " eta1 " << smearing_nu1_phi << " phi1 " << '\n';
      reco::Particle::PolarLorentzVector smeared_nu1_4vector(tau1_daughters[i].pt()+smearing_nu1_pt, tau1_daughters[i].eta()+smearing_nu1_eta, tau1_daughters[i].phi()+smearing_nu1_phi, tau1_daughters[i].mass());
      tau1_daughters[i].setP4(smeared_nu1_4vector);
 
     
/* // Generic/best guess value
      double visible1_energy = tau1_daughters[i].mother()->energy()-tau1_daughters[i].energy();
      double visible1_px = tau1_daughters[i].mother()->px()-tau1_daughters[i].px();
      double visible1_py = tau1_daughters[i].mother()->py()-tau1_daughters[i].py();
      double visible1_pz = tau1_daughters[i].mother()->pz()-tau1_daughters[i].pz();

      reco::Particle::LorentzVector visible1_4vector(visible1_energy, visible1_px, visible1_py, visible1_pz);
      reco::Particle::PolarLorentzVector generic_nu1_4vector(13.6, visible1_4vector.eta(), visible1_4vector.phi(), 0);
      tau1_daughters[i].setP4(generic_nu1_4vector); 
*/
    }
    simple_tau1_daughters.push_back(ConvertToSimplePart(tau1_daughters[i])); 
    // Save tau1daughters to .root:
    tau1DaughtersArray[i][0] = tau1_daughters[i].energy();
    tau1DaughtersArray[i][1] = tau1_daughters[i].px();
    tau1DaughtersArray[i][2] = tau1_daughters[i].py();
    tau1DaughtersArray[i][3] = tau1_daughters[i].pz();
    tau1DaughtersArray[i][4] = tau1_daughters[i].pdgId();
  }
  
  for(unsigned i=0; i<tau2_daughters.size(); ++i) {
    int daughter2_pdgid = fabs(tau2_daughters[i].pdgId());
    if (daughter2_pdgid == 16)
    {
      //smearing
      double smearing_nu2_pt = rndm.Gaus(1.20,29.41);
      double smearing_nu2_eta = rndm.Gaus(0.12, 5.01);
      double smearing_nu2_phi = rndm.Gaus(0.03, 0.74);
      smearingArray[3] = smearing_nu2_pt;
      smearingArray[4] = smearing_nu2_eta;
      smearingArray[5] = smearing_nu2_phi;
      std::cout << "Random value:" << smearing_nu2_pt << " pt2 " << smearing_nu2_eta<< " eta2 " << smearing_nu2_phi << " phi2 " << '\n';
      reco::Particle::PolarLorentzVector smeared_nu2_4vector(tau2_daughters[i].pt()+smearing_nu2_pt, tau2_daughters[i].eta()+smearing_nu2_eta, tau2_daughters[i].phi()+smearing_nu2_phi, tau2_daughters[i].mass());
      tau2_daughters[i].setP4(smeared_nu2_4vector);
     
      
/*    	// If we want the 'generic'/best guess neutrinos
      double visible2_energy = tau2_daughters[i].mother()->energy()-tau2_daughters[i].energy();
      double visible2_px = tau2_daughters[i].mother()->px()-tau2_daughters[i].px();
      double visible2_py = tau2_daughters[i].mother()->py()-tau2_daughters[i].py();
      double visible2_pz = tau2_daughters[i].mother()->pz()-tau2_daughters[i].pz();

      reco::Particle::LorentzVector visible2_4vector(visible2_energy, visible2_px, visible2_py, visible2_pz);
      reco::Particle::PolarLorentzVector generic_nu2_4vector(10.6, visible2_4vector.eta(), visible2_4vector.phi(), 0);
      tau2_daughters[i].setP4(generic_nu2_4vector);
*/
    }
    simple_tau2_daughters.push_back(ConvertToSimplePart(tau2_daughters[i]));
    // Save tau2daughters to .root:
    tau2DaughtersArray[i][0] = tau2_daughters[i].energy();
    tau2DaughtersArray[i][1] = tau2_daughters[i].px();
    tau2DaughtersArray[i][2] = tau2_daughters[i].py();
    tau2DaughtersArray[i][3] = tau2_daughters[i].pz();
    tau2DaughtersArray[i][4] = tau2_daughters[i].pdgId();
  }

	TauSpinner::setHiggsParametersTR(-cos(2*M_PI*0),cos(2*M_PI*0),-sin(2*M_PI*0),-sin(2*M_PI*0));

	Tauolapp::Tauola::setNewCurrents(0);

	TauSpinner::calculateWeightFromParticlesH(simple_boson,simple_tau1,simple_tau2,simple_tau1_daughters,simple_tau2_daughters);

	double WTp = TauSpinner::getWtamplitP();
	double WTm = TauSpinner::getWtamplitM();

	Tauolapp::Tauola::setNewCurrents(1);

	TauSpinner::calculateWeightFromParticlesH(simple_boson,simple_tau1,simple_tau2,simple_tau1_daughters,simple_tau2_daughters);

	//std::cout << "--------" << std::endl;
	//std::cout << simple_tau1_daughters.size() << "    " << simple_tau2_daughters.size() << std::endl;
	//std::cout << TauSpinner::getWtamplitP() << "    " << WTp << "    " << TauSpinner::getWtamplitP()/WTp <<  std::endl;
	//std::cout << TauSpinner::getWtamplitM() << "    " << WTm << "    " << TauSpinner::getWtamplitM()/WTm <<std::endl;

	WTp = TauSpinner::getWtamplitP()/WTp;
	WTm = TauSpinner::getWtamplitM()/WTm;

	info_->set_weight("WTp",WTp,false);
	info_->set_weight("WTm",WTm,false);

  for(unsigned i=0; i<theta_vec_.size(); ++i){
    double theta_val_ = theta_vec_[i].second;
    std::string weight_name_ = theta_vec_[i].first;
    // Can make this more general by having boson pdgid as input or have option for set boson type
    TauSpinner::setHiggsParametersTR(-cos(2*M_PI*theta_val_),cos(2*M_PI*theta_val_),-sin(2*M_PI*theta_val_),-sin(2*M_PI*theta_val_));
    Tauolapp::Tauola::setNewCurrents(0);
    double weight_ = TauSpinner::calculateWeightFromParticlesH(simple_boson,simple_tau1,simple_tau2,simple_tau1_daughters,simple_tau2_daughters); 
    info_->set_weight(weight_name_,weight_,false);
    Tauolapp::Tauola::setNewCurrents(1);
    double weight_2_ = TauSpinner::calculateWeightFromParticlesH(simple_boson,simple_tau1,simple_tau2,simple_tau1_daughters,simple_tau2_daughters); 
    info_->set_weight(weight_name_+"_alt",weight_2_,false);
    
    outFile << weight_ << "," << weight_2_ << ",";
    weight1Array[i] = weight_;
    weight2Array[i] = weight_2_;
  }
  outFile << std::endl;
	outTree->Fill();
}

void ICTauSpinnerProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &info_);
  theta_vec_ = SplitString(theta_);  
  initialize();
  std::cout << "Hello World" << '\n';  
  theta_vec_ = SplitString(theta_);
  // Create arrays to store values for each event
  weight1Array = new double[theta_vec_.size()];
  weight2Array = new double[theta_vec_.size()];
  
  // Register branches
  for(unsigned int i=0; i<theta_vec_.size(); i++)
  {
  	outTree->Branch((theta_vec_[i].first+"_1").c_str(), &weight1Array[i], (theta_vec_[i].first+"_1"+"/D").c_str());
  	outTree->Branch((theta_vec_[i].first+"_2").c_str(), &weight2Array[i], (theta_vec_[i].first+"_2"+"/D").c_str());
  }
  
  for(unsigned int i=0; i<2; i++){
		outTree->Branch(("smearingPt_"+std::to_string(i)).c_str(), &smearingArray[3*i+0], ("smearingPt_"+std::to_string(i)+"/D").c_str());
		outTree->Branch(("smearingEta_"+std::to_string(i)).c_str(), &smearingArray[3*i+1], ("smearingEta_"+std::to_string(i)+"/D").c_str());
		outTree->Branch(("smearingPhi_"+std::to_string(i)).c_str(), &smearingArray[3*i+2], ("smearingPhi_"+std::to_string(i)+"/D").c_str());
  }
  
  for(unsigned int i=0; i<10; i++){
  	outTree->Branch(("tau1Daughter_E_"+std::to_string(i)).c_str(), &tau1DaughtersArray[i][0], ("tau1Daughter_E_"+std::to_string(i)+"/D").c_str());
  	outTree->Branch(("tau1Daughter_px_"+std::to_string(i)).c_str(), &tau1DaughtersArray[i][1], ("tau1Daughter_px_"+std::to_string(i)+"/D").c_str());
  	outTree->Branch(("tau1Daughter_py_"+std::to_string(i)).c_str(), &tau1DaughtersArray[i][2], ("tau1Daughter_py_"+std::to_string(i)+"/D").c_str());
  	outTree->Branch(("tau1Daughter_pz_"+std::to_string(i)).c_str(), &tau1DaughtersArray[i][3], ("tau1Daughter_pz_"+std::to_string(i)+"/D").c_str());
  	outTree->Branch(("tau1Daughter_pdgID_"+std::to_string(i)).c_str(), &tau1DaughtersArray[i][4], ("tau1Daughter_pdgID_"+std::to_string(i)+"/D").c_str());
  	
  	outTree->Branch(("tau2Daughter_E_"+std::to_string(i)).c_str(), &tau2DaughtersArray[i][0], ("tau2Daughter_E_"+std::to_string(i)+"/D").c_str());
  	outTree->Branch(("tau2Daughter_px_"+std::to_string(i)).c_str(), &tau2DaughtersArray[i][1], ("tau2Daughter_px_"+std::to_string(i)+"/D").c_str());
  	outTree->Branch(("tau2Daughter_py_"+std::to_string(i)).c_str(), &tau2DaughtersArray[i][2], ("tau2Daughter_py_"+std::to_string(i)+"/D").c_str());
  	outTree->Branch(("tau2Daughter_pz_"+std::to_string(i)).c_str(), &tau2DaughtersArray[i][3], ("tau2Daughter_pz_"+std::to_string(i)+"/D").c_str());
  	outTree->Branch(("tau2Daughter_pdgID_"+std::to_string(i)).c_str(), &tau2DaughtersArray[i][4], ("tau2Daughter_pdgID_"+std::to_string(i)+"/D").c_str());
  }
  std::cout << "Branches registered." << std::endl;
  
  initialize();
}

void ICTauSpinnerProducer::endJob()
{
	rootFile->Write();
	rootFile->Close();
}

DEFINE_FWK_MODULE(ICTauSpinnerProducer);

