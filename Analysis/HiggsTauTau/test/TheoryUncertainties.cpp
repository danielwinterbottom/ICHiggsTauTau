// ROOT includes
#include "TChain.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TBranch.h"

// C++ includes
#include <iostream>
#include <fstream>
#include <memory>
#include <map>
#include <string>
#include <cstdlib>
#include <math.h> 

// ICHiggsTauTau includes
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

struct swap_labels{
  bool operator() (std::string a,std::string b) {
    if(a=="t" && b!="t") return false;
    else if(a=="m" &&  (b!="m" && b!="t")) return false;
    else return true;
  }
};

double delta_R(ic::Candidate a, ic::Candidate b){
  double eta_1 = a.vector().Rapidity();
  double eta_2 = b.vector().Rapidity();
  double phi_1 = a.vector().Phi();
  double phi_2 = b.vector().Phi();
  double deltaR = sqrt(pow(eta_1-eta_2,2) + pow(phi_1-phi_2,2));
  return deltaR;
}

struct ProgramOptions{
  ProgramOptions(){
    input          = "input.dat";
    minFile        = 1;
    maxFile        = -1;
    maxEvents      = -1;
    outputFilename = "output.root";
    makePlots = false;
    
  }

  std::string           input;
  int                   minFile;
  int                   maxFile;
  int                   maxEvents;
  std::string           outputFilename;
  bool makePlots;
  
};

bool processArgs(int argc, char* argv[], ProgramOptions &opt){

  for (int i=1; i < argc; ++i) {
    std::string arg = argv[i];
    if(arg == "--makePlots") {opt.makePlots  = true;}
    else if(arg == "--input"){
      if(i+1<argc){i++; opt.input = argv[i];}
    } else if(arg == "--minFile"){
      if(i+1<argc){i++; opt.minFile = std::atoi(argv[i]);}
    }else if(arg == "--maxFile"){
      if(i+1<argc){i++; opt.maxFile = std::atoi(argv[i]);}
    }else if(arg == "--maxEvents"){
      if(i+1<argc){i++; opt.maxEvents = atoi(argv[i]);}
    }else if(arg == "--outputFilename"){
      if(i+1<argc){i++; opt.outputFilename = argv[i];}
    }
  }
  
  return 1;
}

std::vector<ic::GenParticle> FamilyTree (std::vector<ic::GenParticle> &v, ic::GenParticle p, std::vector<ic::GenParticle> *gen_particles){
  if(p.status() == 1){
    unsigned ID = std::fabs(p.pdgid());
    if(!(ID==12 || ID==14 || ID==16)){
      v.push_back(p);
    }
  }
  else{
    for(size_t i = 0; i < p.daughters().size(); ++i ){
      ic::GenParticle d = (*gen_particles)[p.daughters().at(i)];
      FamilyTree(v,d, gen_particles);
    }
  }
  return v;
}

int main(int argc, char* argv[]){
    
  ProgramOptions options;
  if(!processArgs(argc, argv, options)){return -1;}
  
  std::cout << "===== program options ====="                 << std::endl;
  std::cout << "input          = " << options.input          << std::endl;
  std::cout << "minFile        = " << options.minFile        << std::endl;
  std::cout << "maxFile        = " << options.maxFile        << std::endl;
  std::cout << "maxEvents      = " << options.maxEvents      << std::endl;
  std::cout << "outputFilename = " << options.outputFilename << std::endl;
  std::cout << "makePlots      = " << options.makePlots      << std::endl;
  
  //All cuts are set here
  double prompt_e_minpt  = 10.;
  double prompt_mu_minpt = 10.;
  double min_jet_pt      = 30.;
  double max_jet_eta     = 4.7;
  
  TChain *chIn1 = new TChain("icEventProducer/EventTree");
  TString filename1; 
  std::ifstream infile1;
  infile1.open(options.input);
  if(!infile1){
    std::cout << "Input filelist does not exist!" << std::endl;
    return 0;    
  }
  int fileCount=1;
  while (infile1 >> filename1){
      std::cout << filename1 << std::endl;
    if(fileCount >= options.minFile && (fileCount < options.maxFile || options.maxFile == -1 )){
      std::cout << "Adding file to TChain: " << filename1 << std::endl;  
      chIn1->Add(filename1);
    }
    fileCount++;
  }
  infile1.close();
  
  unsigned nentries1;
  if(options.maxEvents == -1){
    nentries1 = chIn1->GetEntries();
  }else{
    nentries1 = (unsigned)options.maxEvents;
  }
  if(nentries1 > chIn1->GetEntries()) nentries1 = chIn1->GetEntries();
  
  std::cout << "Running over " << nentries1 << " events" << std::endl;
  
  unsigned n_report = 10000;
  unsigned totaleventcount = 0;
  
  std::vector<ic::GenParticle> *gen_particles = new std::vector<ic::GenParticle>();
  chIn1->SetBranchAddress("genParticles",&gen_particles);
  std::vector<ic::GenJet> *gen_jets = new std::vector<ic::GenJet>();
  chIn1->SetBranchAddress("genJets",&gen_jets);

  for(unsigned event=0; event<nentries1; event++){
    totaleventcount++;
    if(event % n_report == 0) std::cout << "Processing " << event << "th event." << std::endl;
    chIn1->GetEntry(event);
    
    std::vector<ic::GenParticle> higgs_products;
    ic::Candidate met; 
    std::vector<ic::GenParticle> prompt_leptons;
    std::vector<std::string> decay_types;
    std::vector<ic::GenJet> filtered_jets;
    
    for(unsigned i=0; i<gen_particles->size(); ++i){
      
      ic::GenParticle part = (*gen_particles)[i];
      ic::GenParticle higgs_product;
      
      unsigned genID = std::fabs(part.pdgid());
      
      // add neutrinos 4-vector to get gen met
      if(genID == 12 || genID == 14 || genID == 16) met.set_vector(met.vector() + part.vector());
      
      //find all prompt electrons and muons - for extra lepton veto and filtering jet collection
      bool isPrompt = part.statusFlags().at(0) || part.statusFlags().at(5);
      if(isPrompt){
        double prompt_pt = part.vector().Pt();    
        if     (genID == 11 && prompt_pt > prompt_e_minpt)  prompt_leptons.push_back(part);
        else if(genID == 13 && prompt_pt > prompt_mu_minpt) prompt_leptons.push_back(part);
      }
      
      // check if particle is a tau, if it is and mother is a Higgs find stable visible daughters
      if(genID != 15) continue;
      unsigned motherID = std::fabs((*gen_particles)[part.mothers().at(0)].pdgid());
      if(motherID != 25) continue;

      std::vector<ic::GenParticle> family;
      FamilyTree(family, part, gen_particles);
      
      if(family.size() == 1){
        higgs_products.push_back(family[0]);
        unsigned familyID = std::fabs(family[0].pdgid());
        if     (familyID == 11) decay_types.push_back("e");
        else if(familyID == 13) decay_types.push_back("m");
      } else {
        decay_types.push_back("t");  
        ic::GenParticle had_tau;
        int charge = 0;
        int pdgid = part.pdgid();
        for(unsigned j=0; j<family.size(); ++j){
          had_tau.set_vector(had_tau.vector() + family[j].vector());
          charge += family[j].charge();
        }
          had_tau.set_charge(charge);
          had_tau.set_pdgid(pdgid);
          higgs_products.push_back(had_tau);
      }
    }
    
    //size of decay_types vector should always be 2 but added this line just to be sure
    if(decay_types.size()!=2) continue;
    std::sort(decay_types.begin(),decay_types.end(),swap_labels());
    std::string decayType = decay_types[0]+decay_types[1];
    
    //filter lepton collections by pt and eta
    //filter prompt_leptons by eta (and pt but get rid of other cut!)
    //apply extra lepton veto
    
    for(unsigned i=0; i<gen_jets->size(); ++i){
      ic::GenJet jet = (*gen_jets)[i];
      double jetPt = jet.vector().Pt();
      double jetEta = std::fabs(jet.vector().Rapidity());
      bool MatchedToPrompt = false;
      for(unsigned j=0; j<higgs_products.size(); ++j){
        double deltaR = delta_R(jet, higgs_products[j]);
        MatchedToPrompt = deltaR < 0.5;
      }
      //select jets above minimum pt threshold that aren't matched to Higgs decay products
      if(jetPt > min_jet_pt && jetEta < max_jet_eta && !MatchedToPrompt) filtered_jets.push_back(jet); 
    }
    
    // produce plots making sure things make sense, pt, eta of objects (seperate by channel), met, mjj, dEta, ect
    std::cout << "Next Event!" << std::endl;
  }
  
  
  std::cout << "Total events processed: " << totaleventcount << std::endl;
  return 0;
}
