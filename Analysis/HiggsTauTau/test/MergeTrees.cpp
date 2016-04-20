// ROOT includes
#include "TChain.h"
//#include "TMath.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TBranch.h"

// C++ includes
#include <iostream>
#include <fstream>
#include <memory>
#include <map>
#include <string>
#include <cstdlib>

// ICHiggsTauTau Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TSum.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"

  
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/PileupInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"



int main(int argc, char* argv[]){
  if(argc != 2){
      std::cout << "Wrong number of arguments, expected 1 argument." << std::endl;
      return 0;
  }
  int num = std::atoi(argv[1]);
  
  // Create the first tree. 
  std::string filename1;
  std::string filenametemp;
  TChain *chIn1 = new TChain("icEventProducer/EventTree");
  std::ifstream infile1;
  infile1.open("EventTreeAll.dat");
 
  int linenumber = 1;
  while(infile1 >> filenametemp){
      if(linenumber == num){
          filename1 = filenametemp;
          break;
      }
      linenumber++;
  }

  chIn1->Add(filename1.c_str());
  infile1.close();
  
  // Create the second tree. 
  TString filename2;
  TChain *chIn2 = new TChain("icEventProducer/EventTree");
  std::ifstream infile2;
  infile2.open("L1NTuplesV2_l1t-integration-v34_TauTau_Eta3.dat");
  while (infile2 >> filename2) chIn2->Add(filename2);
          
  infile2.close();
  
  unsigned nentries2 = chIn2->GetEntries();

  std::cout << "Cloning offline tree..." << std::endl;

  // Create output file and clone the first tree.
  std::string outputfilename = Form("/vols/cms02/dw515/l1t-integration-v34/HiggsTaTauEta3/EventTreeMerged_%d.root", num);
  TFile *fOut = new TFile(outputfilename.c_str(),"RECREATE");
  unsigned nentries1 = chIn1->GetEntries();
  //unsigned nentries1 = 100;
  //TTree *tOut = new TTree("EventTree", "EventTree");
  TTree *tOut = chIn1->CloneTree();
  
  // Get the eventInfo for both trees.

  ic::EventInfo *eventInfo2 = new ic::EventInfo();
  chIn2->SetBranchAddress("eventInfo",&eventInfo2);
  ic::EventInfo  *eventInfo1 = new ic::EventInfo ();
  tOut->SetBranchAddress("eventInfo",&eventInfo1);
  
  //get branches from tree 1
  
/*  std::vector<ic::Vertex> *vertices = new std::vector<ic::Vertex>();
  chIn1->SetBranchAddress("vertices",&vertices);
  std::vector<ic::Vertex> *genVertices = new std::vector<ic::Vertex>(); 
  chIn1->SetBranchAddress("genVertices",&genVertices);
  std::vector<ic::Electron> *electrons = new std::vector<ic::Electron>();
  chIn1->SetBranchAddress("electrons",&electrons);
  std::vector<ic::Muon> *muons = new std::vector<ic::Muon>();
  chIn1->SetBranchAddress("muons",&muons);
  std::vector<ic::Tau> *taus = new std::vector<ic::Tau>();
  chIn1->SetBranchAddress("taus",&taus);
  std::vector<ic::Met> *pfmet = new std::vector<ic::Met>();
  chIn1->SetBranchAddress("pfMet",&pfmet);
  std::vector<ic::Met> *puppimet = new std::vector<ic::Met>();
  chIn1->SetBranchAddress("puppiMet",&puppimet);
  std::vector<ic::Met> *pfMVAMetVector = new std::vector<ic::Met>();
  chIn1->SetBranchAddress("pfMVAMetVector",&pfMVAMetVector);
  std::vector<ic::Met> *pfMVAMet = new std::vector<ic::Met>();
  chIn1->SetBranchAddress("pfMVAMet",&pfMVAMet);
  std::vector<ic::GenParticle> *genParticles = new std::vector<ic::GenParticle>();
  chIn1->SetBranchAddress("genParticles",&genParticles);
  std::vector<ic::GenJet> *genJetsReclustered = new std::vector<ic::GenJet>();
  chIn1->SetBranchAddress("genJetsReclustered",&genJetsReclustered);
  std::vector<ic::GenJet> *genJets = new std::vector<ic::GenJet>();
  chIn1->SetBranchAddress("genJets",&genJets);
  std::vector<ic::PileupInfo> *pileupInfo = new std::vector<ic::PileupInfo>();
  chIn1->SetBranchAddress("pileupInfo",&pileupInfo);
  std::vector<ic::PFJet> *ak4PFJetsCHS = new std::vector<ic::PFJet>();
  chIn1->SetBranchAddress("ak4PFJetsCHS",&ak4PFJetsCHS);
  std::vector<ic::PFJet> *ak4PFJetsCHSReclustered = new std::vector<ic::PFJet>();
  chIn1->SetBranchAddress("ak4PFJetsCHSReclustered",&ak4PFJetsCHSReclustered);
  ic::EventInfo  *eventInfo1 = new ic::EventInfo ();
  chIn1->SetBranchAddress("eventInfo",&eventInfo1);*/
  
  
  // Get new branches from tree 2. 
  std::vector<ic::L1TTau> *l1taus = new std::vector<ic::L1TTau>();
  chIn2->SetBranchAddress("L1Taus",&l1taus);
  std::vector<ic::L1TMuon> *l1muons = new std::vector<ic::L1TMuon>();
  chIn2->SetBranchAddress("L1Muon",&l1muons);
  std::vector<ic::L1TEGamma> *l1egammas = new std::vector<ic::L1TEGamma>();
  chIn2->SetBranchAddress("L1EGammas",&l1egammas);
  std::vector<ic::L1TJet> *l1jets = new std::vector<ic::L1TJet>();
  chIn2->SetBranchAddress("L1Jets",&l1jets);
  std::vector<ic::L1TSum> *l1sums = new std::vector<ic::L1TSum>();
  chIn2->SetBranchAddress("L1Sums",&l1sums);
  
  /*std::vector<ic::L1TTau> *l1tausnew = new std::vector<ic::L1TTau>();
  tOut->Branch("L1Taus", &l1tausnew);
  std::vector<ic::L1TMuon> *l1muonsnew = new std::vector<ic::L1TMuon>();
  tOut->Branch("L1Muons", &l1muonsnew);
  std::vector<ic::L1TEGamma> *l1egammasnew = new std::vector<ic::L1TEGamma>();
  tOut->Branch("L1EGammas", &l1egammasnew);
  std::vector<ic::L1TJet> *l1jetsnew = new std::vector<ic::L1TJet>();
  tOut->Branch("L1Jets", &l1jetsnew);
  std::vector<ic::L1TSum> *l1sumsnew = new std::vector<ic::L1TSum>();
  tOut->Branch("L1Sums", &l1sumsnew);
  */
  // Create new braches on new tree.
  std::vector<ic::L1TTau> *l1tausnew = new std::vector<ic::L1TTau>();
  TBranch *newBranch1 = tOut->Branch("L1Taus", &l1tausnew);
  std::vector<ic::L1TMuon> *l1muonsnew = new std::vector<ic::L1TMuon>();
  TBranch *newBranch2 = tOut->Branch("L1Muons", &l1muonsnew);
  std::vector<ic::L1TEGamma> *l1egammasnew = new std::vector<ic::L1TEGamma>();
  TBranch *newBranch3 = tOut->Branch("L1EGammas", &l1egammasnew);
  std::vector<ic::L1TJet> *l1jetsnew = new std::vector<ic::L1TJet>();
  TBranch *newBranch4 = tOut->Branch("L1Jets", &l1jetsnew);
  std::vector<ic::L1TSum> *l1sumsnew = new std::vector<ic::L1TSum>();
  TBranch *newBranch5 = tOut->Branch("L1Sums", &l1sumsnew);
  /*std::vector<ic::Vertex> *verticesnew = new std::vector<ic::Vertex>();
  TBranch *newBranch6 = tOut->Branch("vertices",&verticesnew);
  std::vector<ic::Vertex> *genVerticesnew = new std::vector<ic::Vertex>(); 
  TBranch *newBranch7 = tOut->Branch("genVertices",&genVerticesnew);
  std::vector<ic::Electron> *electronsnew = new std::vector<ic::Electron>();
  TBranch *newBranch8 = tOut->Branch("electrons",&electronsnew);
  std::vector<ic::Muon> *muonsnew = new std::vector<ic::Muon>();
  TBranch *newBranch9 = tOut->Branch("muons",&muonsnew);
  std::vector<ic::Tau> *tausnew = new std::vector<ic::Tau>();
  TBranch *newBranch10 = tOut->Branch("taus",&tausnew);
  std::vector<ic::Met> *pfmetnew = new std::vector<ic::Met>();
  TBranch *newBranch11 = tOut->Branch("pfMet",&pfmetnew);
  std::vector<ic::Met> *puppimetnew = new std::vector<ic::Met>();
  TBranch *newBranch12 = tOut->Branch("puppiMet",&puppimetnew);
  std::vector<ic::Met> *pfMVAMetVectornew = new std::vector<ic::Met>();
  TBranch *newBranch13 = tOut->Branch("pfMVAMetVector",&pfMVAMetVectornew);
  std::vector<ic::Met> *pfMVAMetnew = new std::vector<ic::Met>();
  TBranch *newBranch14 = tOut->Branch("pfMVAMet",&pfMVAMetnew);
  std::vector<ic::GenParticle> *genParticlesnew = new std::vector<ic::GenParticle>();
  TBranch *newBranch15 = tOut->Branch("genParticles",&genParticlesnew);
  std::vector<ic::GenJet> *genJetsReclusterednew = new std::vector<ic::GenJet>();
  TBranch *newBranch16 = tOut->Branch("genJetsReclustered",&genJetsReclusterednew);
  std::vector<ic::GenJet> *genJetsnew = new std::vector<ic::GenJet>();
  TBranch *newBranch17 = tOut->Branch("genJets",&genJetsnew);
  std::vector<ic::PileupInfo> *pileupInfonew = new std::vector<ic::PileupInfo>();
  TBranch *newBranch18 = tOut->Branch("pileupInfo",&pileupInfonew);
  std::vector<ic::PFJet> *ak4PFJetsCHSnew = new std::vector<ic::PFJet>();
  TBranch *newBranch19 = tOut->Branch("ak4PFJetsCHS",&ak4PFJetsCHSnew);
  std::vector<ic::PFJet> *ak4PFJetsCHSReclusterednew = new std::vector<ic::PFJet>();
  TBranch *newBranch20 = tOut->Branch("ak4PFJetsCHSReclustered",&ak4PFJetsCHSReclusterednew);
  ic::EventInfo  *eventInfonew = new ic::EventInfo ();
  TBranch *newBranch21 = tOut->Branch("eventInfo",&eventInfonew);*/
  
  unsigned n_report = 1000;
  unsigned NEventsMerged=0;

  for(unsigned i=0; i<nentries1; i++){

      //chIn1->GetEntry(i);
      //std::cout << "i " << i << std::endl;

      for(unsigned j=0; j<nentries2; j++){
          
          tOut->GetEntry(i);
          chIn2->GetEntry(j);  
          
          // If eventIDs match then fill new branch with copied branches.
          if(eventInfo1->event() == eventInfo2->event()){
              //if(i % n_report == 0) std::cout << "Finished merging " << i << "th event." << std::endl;
              //tOut->GetEntry(NEventsMerged);
              //tOut->SetBranchAddress("L1Taus", &l1tausnew);
              //tOut->SetBranchAddress("L1EGammas", &l1egammasnew);
              //tOut->SetBranchAddress("L1Muons", &l1muonsnew);
              //tOut->SetBranchAddress("L1Jets", &l1jetsnew);
              //tOut->SetBranchAddress("L1Sums", &l1sumsnew);
              l1tausnew = l1taus;
              l1muonsnew = l1muons;
              l1egammasnew = l1egammas;
              l1jetsnew = l1jets;
              l1sumsnew = l1sums;
              /*verticesnew = vertices;
              genVerticesnew = genVertices;
              electronsnew = electrons;
              muonsnew = muons;
              tausnew = taus;
              pfmetnew = pfmet;
              puppimetnew = puppimet;
              pfMVAMetVectornew = pfMVAMetVector;
              pfMVAMetnew = pfMVAMet;
              genParticlesnew = genParticles;
              genJetsReclusterednew = genJetsReclustered;
              genJetsnew = genJets;
              pileupInfonew = pileupInfo;
              ak4PFJetsCHSnew = ak4PFJetsCHS;
              ak4PFJetsCHSReclusterednew = ak4PFJetsCHSReclustered;
              eventInfonew = eventInfo1;*/

              newBranch1->Fill();
              newBranch2->Fill();
              newBranch3->Fill();
              newBranch4->Fill();
              newBranch5->Fill();
              /*newBranch6->Fill();
              newBranch7->Fill();
              newBranch8->Fill();
              newBranch9->Fill();
              newBranch10->Fill();
              newBranch11->Fill();
              newBranch12->Fill();
              newBranch13->Fill();
              newBranch14->Fill();
              newBranch15->Fill();
              newBranch16->Fill();
              newBranch17->Fill();
              newBranch18->Fill();
              newBranch19->Fill();
              newBranch20->Fill();
              newBranch21->Fill();
              */
              //tOut->Fill();
              NEventsMerged++;
              

              
              // If a match is found it is likey that the next few preceeding events are also matched so this next part looks to match those events without doing the full loop.
              bool proceed = true;
              unsigned jtemp = j+1;
              unsigned itemp = i+1;
              
              while(proceed){
                  
                  tOut->GetEntry(itemp);
                  chIn2->GetEntry(jtemp);
                  
                  if(eventInfo1->event() == eventInfo2->event()){
                      if(itemp % n_report == 0) std::cout << "Finished merging " << itemp << "th event." << std::endl;
                      //continue filling  
                      //std::cout << "itemp " << itemp << std::endl;
                      //tOut->GetEntry(NEventsMerged);
                     // tOut->SetBranchAddress("L1Taus", &l1tausnew);
                     // tOut->SetBranchAddress("L1EGammas", &l1egammasnew);
                     // tOut->SetBranchAddress("L1Muons", &l1muonsnew);
                     // tOut->SetBranchAddress("L1Jets", &l1jetsnew);
                     // tOut->SetBranchAddress("L1Sums", &l1sumsnew);
                      l1tausnew = l1taus;
                      l1muonsnew = l1muons;
                      l1egammasnew = l1egammas;
                      l1jetsnew = l1jets;
                      l1sumsnew = l1sums;
                      /*verticesnew = vertices;
                      genVerticesnew = genVertices;
                      electronsnew = electrons;
                      muonsnew = muons;
                      tausnew = taus;
                      pfmetnew = pfmet;
                      puppimetnew = puppimet;
                      pfMVAMetVectornew = pfMVAMetVector;
                      pfMVAMetnew = pfMVAMet;
                      genParticlesnew = genParticles;
                      genJetsReclusterednew = genJetsReclustered;
                      genJetsnew = genJets;
                      pileupInfonew = pileupInfo;
                      ak4PFJetsCHSnew = ak4PFJetsCHS;
                      ak4PFJetsCHSReclusterednew = ak4PFJetsCHSReclustered;
                      eventInfonew = eventInfo1;*/

                      newBranch1->Fill();
                      newBranch2->Fill();
                      newBranch3->Fill();
                      newBranch4->Fill();
                      newBranch5->Fill();
                      /*newBranch6->Fill();
                      newBranch7->Fill();
                      newBranch8->Fill();
                      newBranch9->Fill();
                      newBranch10->Fill();
                      newBranch11->Fill();
                      newBranch12->Fill();
                      newBranch13->Fill();
                      newBranch14->Fill();
                      newBranch15->Fill();
                      newBranch16->Fill();
                      newBranch17->Fill();
                      newBranch18->Fill();
                      newBranch19->Fill();
                      newBranch20->Fill();
                      newBranch21->Fill();*/
                      
                      //tOut->Fill();
                      NEventsMerged++;
                      itemp++;
                      jtemp++;
                      
                      if(jtemp == nentries2){
                          i = itemp -1;
                          proceed = false;
                      }
                      if(itemp == nentries1){
                          std::cout << "Merged " << NEventsMerged << " out of " <<nentries1 << std::endl;
                          return 0;
                      }
                      
                  }
                  else{
                      i = itemp-1;
                      j = jtemp-1;
                      proceed = false;
                  }
              }
              
              break;
          }
          
      }
  }
  
  
  std::cout << "Merged " << NEventsMerged << " out of " <<nentries1 << std::endl;
  fOut->Write();  
  fOut->Close();
  
  return 0;  

}
