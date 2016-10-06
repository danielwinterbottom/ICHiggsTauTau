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
  
  //if(argc < 2){
  //  std::cout << "Wrong number of arguments, expected minimum 2 arguments." << std::endl;
  //  return 0;
  //}
  
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

//void GetHiggsDecay(ic::) {
    
//}

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

  for(unsigned event=0; event<nentries1; event++){
    totaleventcount++;
    if(event % n_report == 0) std::cout << "Processing " << event << "th event." << std::endl;
    chIn1->GetEntry(event);
    
    //std::vector<ic::GenParticle*> higgs_products;
    
    for(unsigned i=0; i<gen_particles->size(); ++i){
        
      ic::GenParticle part = (*gen_particles)[i];
      ic::GenParticle higgs_product;
      
      unsigned genID = std::fabs(part.pdgid());
      if(genID != 15) continue;
      //std::cout << "Found a Tau" << std::endl;
      unsigned motherID = std::fabs((*gen_particles)[part.mothers().at(0)].pdgid());
      if(motherID != 25) continue;
      //std::cout << "And it comes from a Higgs!" << std::endl;
      ic::Candidate tauNu;
      for(unsigned j=0; j<part.daughters().size(); ++j){
        unsigned daughterID = std::fabs((*gen_particles)[part.daughters().at(j)].pdgid());
        if(daughterID == 16){
          tauNu.set_vector((*gen_particles)[part.daughters().at(j)].vector());
          //std::cout << "found tau neutrino!" << std::endl;
        }
        if(daughterID == 11 || daughterID == 13){
          higgs_product = (*gen_particles)[part.daughters().at(j)];
          std::cout << "found electron or muon!" << std::endl;
          break;    
        }
        //if no electron or muon is found that tau must be hadronic tau. Subtract neutrino 4 vector from tau to get visible hadronic tau.
        std::cout << "no electron or muon found!" << std::endl;
        std::cout << "tau Pt = " << part.vector().Pt() << std::endl;
        std::cout << "tau neutrino Pt = " << tauNu.vector().Pt() << std::endl;
        higgs_product = part;
        higgs_product.set_vector(higgs_product.vector() - tauNu.vector());
        std::cout << "visible Pt = " << higgs_product.vector().Pt() << std::endl;
      }

      //higgs_products.push_back(higgs_product);
    }    
  }
  
  std::cout << "Total events processed: " << totaleventcount << std::endl;
  return 0;
}
