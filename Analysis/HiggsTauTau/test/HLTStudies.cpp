// ROOT includes
#include "TChain.h"
#include "TCanvas.h"
#include "TMath.h"
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

int main(int argc, char* argv[]){

  if(argc < 4){
    std::cout << "Wrong number of arguments, expected minimum 3 arguments." << std::endl;
    return 0;
  }
  std::string channel      = argv[1];
  std::string outputDir    = argv[2];
  std::string signalType   = argv[3];
  std::string TriggerName[2];
  
  if(argc < 5){
    if(channel == "mt") {
      TriggerName[0] = "HLT_IsoMu22_v"; 
      TriggerName[1] = "HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v"; 
    } else if(channel == "et") {
      TriggerName[0] = "HLT_Ele25_eta2p1_WPTight_Gsf_v"; 
      TriggerName[1] = "HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v"; 
    } else if(channel == "em") {
      TriggerName[0] = "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v"; 
      TriggerName[1] = "HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v"; 
    } else if(channel == "tt") {
      TriggerName[0] = "HLT_DoubleMediumIsoPFTau32_Trk1_eta2p1_Reg_v"; 
      TriggerName[1] = "HLT_DoubleMediumIsoPFTau32_Trk1_eta2p1_Reg_v"; 
    }
  }
  else if(argc < 6){
    TriggerName[0] = argv[4];
    if(channel == "mt")      TriggerName[1] = "HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v";
    else if(channel == "et") TriggerName[1] = "HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v" ;
    else if(channel == "em") TriggerName[1] = "HLT_Mu23_TrkIsoVVL_Ele8 CaloIdL_TrackIdL_IsoVL_v";
    else if(channel == "tt") TriggerName[1] = "HLT_DoubleMediumIsoPFTau32_Trk1_eta2p1_Reg_v";
  }
  else {
   TriggerName[0] = argv[4];
   TriggerName[1] = argv[5];   
  }
  
  double ExtraPtCut1[2] = {};
  double ExtraPtCut2[2] = {};
  for(unsigned i = 0; i < 2 ; ++i){
    if     (TriggerName[i] == "HLT_IsoMu22_v"){
      ExtraPtCut1[i] = 23;
      ExtraPtCut2[i] = 0;
    } else if(TriggerName[i] == "HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v"){
      ExtraPtCut1[i] = 20;
      ExtraPtCut2[i] = 20;
    } else if(TriggerName[i] == "HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v"){
      ExtraPtCut1[i] = 20;
      ExtraPtCut2[i] = 20;
    } else if(TriggerName[i] == "HLT_Ele25_eta2p1_WPTight_Gsf_v"){
      ExtraPtCut1[i] = 26;
      ExtraPtCut2[i] = 0;
    } else if(TriggerName[i] == "HLT_Ele27_eta2p1_WPLoose_Gsf_v"){
      ExtraPtCut1[i] = 28;
      ExtraPtCut2[i] = 0;
    } else if(TriggerName[i] == "HLT_Ele27_eta2p1_WPTight_Gsf_v"){
      ExtraPtCut1[i] = 28;
      ExtraPtCut2[i] = 0;
    } else if(TriggerName[i] == "HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v"){
      ExtraPtCut1[i] = 25;
      ExtraPtCut2[i] = 20;
    } else if(TriggerName[i] == "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v"){
      ExtraPtCut1[i] = 24;
      ExtraPtCut2[i] = 9;
    } else if(TriggerName[i] == "HLT_Mu23_TrkIsoVVL_Ele8 CaloIdL_TrackIdL_IsoVL_v"){
      ExtraPtCut1[i] = 9;
      ExtraPtCut2[i] = 24;
    } else if(TriggerName[i] == "HLT_DoubleMediumIsoPFTau32_Trk1_eta2p1_Reg_v"){
      ExtraPtCut1[i] = 40;
      ExtraPtCut2[i] = 40;
    }
  }
  
  std::cout << "Channel: "            << channel        << std::endl;
  std::cout << "Output Directory: = " << outputDir      << std::endl;
  std::cout << "Signal Sample: "      << signalType     << std::endl;
  std::cout << "Trigger 1: "          << TriggerName[0] << std::endl;
  std::cout << "Trigger 2: "          << TriggerName[1] << std::endl;
  
  std::string filename1;
  if(signalType == "GluGlu"){
    filename1 = "output/Jul03/GluGluHToTauTau_M-125_"+channel+"_2015.root";
  } else if (signalType == "VBF") {
    filename1 = "output/Jul03/VBFHToTauTau_M-125_"+channel+"_2015.root";
  }
  else {
    std::cout << "Incorrect signal input" << std::endl;
    return 0;
  }
  std::cout << "Getting tree from file: " << filename1 << std::endl;
  
  TChain *tIn = new TChain("ntuple");
  tIn->Add(filename1.c_str());
  unsigned nentries1 = tIn->GetEntries();
  
  std::cout << "Input tree contains " << nentries1 << " entries" << std::endl;

  double iso_1;
  tIn->SetBranchAddress("iso_1",&iso_1);
  double iso_2;
  tIn->SetBranchAddress("iso_2",&iso_2);
  bool mva_olddm_vtight_1;
  tIn->SetBranchAddress("mva_olddm_vtight_1",&mva_olddm_vtight_1);
  bool mva_olddm_vtight_2;
  tIn->SetBranchAddress("mva_olddm_vtight_2",&mva_olddm_vtight_2);
  bool mva_olddm_tight_2;
  tIn->SetBranchAddress("mva_olddm_tight_2",&mva_olddm_tight_2);
  bool antiele_1;
  tIn->SetBranchAddress("antiele_1",&antiele_1);
  bool antiele_2;
  tIn->SetBranchAddress("antiele_2",&antiele_2);
  bool antimu_1;
  tIn->SetBranchAddress("antimu_1",&antimu_1);
  bool antimu_2;
  tIn->SetBranchAddress("antimu_2",&antimu_2);
  bool leptonveto;
  tIn->SetBranchAddress("leptonveto",&leptonveto);
  double pt_1;
  tIn->SetBranchAddress("pt_1",&pt_1);
  double pt_2;
  tIn->SetBranchAddress("pt_2",&pt_2);
  
  bool PassedTrigger1;
  bool PassedTrigger2;
  
  tIn->SetBranchAddress(TriggerName[0].c_str(),&PassedTrigger1);
  tIn->SetBranchAddress(TriggerName[1].c_str(),&PassedTrigger2);
  
  unsigned countTotal = 0;
  unsigned countTrigger1 = 0;
  unsigned countTrigger2 = 0;
  unsigned countCross = 0;
  
  TCanvas *c1 = new TCanvas("c1");
  c1->SetTickx(1);
  c1->SetTicky(1);
  
  TH1D *h_lep1pt                = new TH1D("h_lep1pt", "h_lep1pt", 50, 0, 100);
  TH1D *h_lep2pt                = new TH1D("h_lep2pt", "h_lep2pt", 50, 0, 100);
  TH1D *h_lep1pt_denum_trigger1 = new TH1D("h_lep1pt_denum_trigger1", "h_lep1pt_denum_trigger1", 50, 0, 100);
  TH1D *h_lep2pt_denum_trigger1 = new TH1D("h_lep2pt_denum_trigger1", "h_lep2pt_denum_trigger1", 50, 0, 100);
  TH1D *h_lep1pt_denum_trigger2 = new TH1D("h_lep1pt_denum_trigger2", "h_lep1pt_denum_trigger2", 50, 0, 100);
  TH1D *h_lep2pt_denum_trigger2 = new TH1D("h_lep2pt_denum_trigger2", "h_lep2pt_denum_trigger2", 50, 0, 100);
  TH1D *h_lep1_eff_trigger1     = new TH1D("h_lep1_eff_trigger1", TriggerName[0].c_str(), 50, 0, 100);
  TH1D *h_lep2_eff_trigger1     = new TH1D("h_lep2_eff_trigger1", TriggerName[0].c_str(), 50, 0, 100);
  TH1D *h_lep1_eff_trigger2     = new TH1D("h_lep1_eff_trigger2", TriggerName[1].c_str(), 50, 0, 100);
  TH1D *h_lep2_eff_trigger2     = new TH1D("h_lep2_eff_trigger2", TriggerName[1].c_str(), 50, 0, 100);
  
  h_lep1pt_denum_trigger1->Sumw2();
  h_lep2pt_denum_trigger1->Sumw2();
  h_lep1pt_denum_trigger2->Sumw2();
  h_lep2pt_denum_trigger2->Sumw2();
  h_lep1_eff_trigger1->Sumw2();
  h_lep2_eff_trigger1->Sumw2();
  h_lep1_eff_trigger2->Sumw2();
  h_lep2_eff_trigger2->Sumw2();
  h_lep1_eff_trigger1->SetStats(0);
  h_lep2_eff_trigger1->SetStats(0);
  h_lep1_eff_trigger2->SetStats(0);
  h_lep2_eff_trigger2->SetStats(0);
  h_lep1_eff_trigger1->GetYaxis()->SetTitle("Efficiency");
  h_lep2_eff_trigger1->GetYaxis()->SetTitle("Efficiency");
  h_lep1_eff_trigger2->GetYaxis()->SetTitle("Efficiency");
  h_lep2_eff_trigger2->GetYaxis()->SetTitle("Efficiency");
  h_lep1_eff_trigger1->GetXaxis()->SetTitle("lep1_pt [GeV]");
  h_lep2_eff_trigger1->GetXaxis()->SetTitle("lep2_pt [GeV]");
  h_lep1_eff_trigger2->GetXaxis()->SetTitle("lep1_pt [GeV]");
  h_lep2_eff_trigger2->GetXaxis()->SetTitle("lep2_pt [GeV]");
    
  for(unsigned i=0; i<nentries1; ++i){
    tIn->GetEntry(i);
    bool PassedBaseLine = false;
    if      ((channel == "et" || channel == "mt") && iso_1<0.1 && mva_olddm_tight_2 && antiele_2 && antimu_2 && !leptonveto) PassedBaseLine = true;
    else if (channel == "tt" && mva_olddm_vtight_1>0.5 && mva_olddm_vtight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto) PassedBaseLine = true;
    else if (channel == "em" && iso_1<0.15 && iso_2<0.15 && !leptonveto) PassedBaseLine = true;
    if(PassedBaseLine){
      h_lep1pt->Fill(pt_1);
      h_lep2pt->Fill(pt_2);
      countTotal++;
      if(pt_1 > ExtraPtCut1[0] && pt_2 > ExtraPtCut2[0]){
        h_lep1pt_denum_trigger1->Fill(pt_1);
        h_lep2pt_denum_trigger1->Fill(pt_2);
      }
      if(pt_1 > ExtraPtCut1[1] && pt_2 > ExtraPtCut2[1]){
        h_lep1pt_denum_trigger2->Fill(pt_1);
        h_lep2pt_denum_trigger2->Fill(pt_2);
      }
      if(PassedTrigger1){
        countTrigger1++;    
        h_lep1_eff_trigger1->Fill(pt_1);
        h_lep2_eff_trigger1->Fill(pt_2);
      }
      if(PassedTrigger2){
        countTrigger2++;    
        h_lep1_eff_trigger2->Fill(pt_1);
        h_lep2_eff_trigger2->Fill(pt_2);
      }
      if(PassedTrigger1 || PassedTrigger2){
        countCross++;    
      }
    }
  }
  
  std::cout << "Total events passing offline selection: " << countTotal << std::endl;
  std::cout << "Events passing " << TriggerName[0] << ": " << countTrigger1 << std::endl;
  std::cout << "Events passing " << TriggerName[1] << ": " << countTrigger2 << std::endl;
  std::cout << "Events passing " << TriggerName[0] << " OR " << TriggerName[1] << ": " << countCross << std::endl;
  double gains = 100*((double)countCross/std::max(countTrigger1,countTrigger2)-1.);
  std::cout << "Gain from using OR trigger = " << gains << "%" << std::endl;
  
  std::string outName1 = outputDir+"/"+signalType+"_"+channel+"_"+TriggerName[0];
  std::string outName2 = outputDir+"/"+signalType+"_"+channel+"_"+TriggerName[1];
  std::string outName3 = outputDir+"/"+signalType+"_"+channel;
  
  //h_lep1_eff_trigger1->Draw();
  //c1->Print((outName1+"_lep1pt_passed.pdf").c_str());
  //h_lep2_eff_trigger1->Draw();
  //c1->Print((outName1+"_lep2pt_passed.pdf").c_str());
  //h_lep1_eff_trigger2->Draw();
  //c1->Print((outName2+"_lep1pt_passed.pdf").c_str());
  //h_lep2_eff_trigger2->Draw();
  //c1->Print((outName2+"_lep2pt_passed.pdf").c_str());
  
  //h_lep1_eff_trigger1->GetYaxis()->SetRangeUser(0,1.1);
  //h_lep2_eff_trigger1->GetYaxis()->SetRangeUser(0,1.1);
  //h_lep1_eff_trigger2->GetYaxis()->SetRangeUser(0,1.1);
  //h_lep2_eff_trigger2->GetYaxis()->SetRangeUser(0,1.1);
  
  h_lep1_eff_trigger1->Divide(h_lep1pt_denum_trigger1);
  h_lep1_eff_trigger1->Draw();
  c1->Print((outName1+"_lep1.pdf").c_str());
  h_lep2_eff_trigger1->Divide(h_lep2pt_denum_trigger1);
  h_lep2_eff_trigger1->Draw();
  c1->Print((outName1+"_lep2.pdf").c_str());
  h_lep1_eff_trigger2->Divide(h_lep1pt_denum_trigger2);
  h_lep1_eff_trigger2->Draw();
  c1->Print((outName2+"_lep1.pdf").c_str());
  h_lep2_eff_trigger2->Divide(h_lep2pt_denum_trigger2);
  h_lep2_eff_trigger2->Draw();
  c1->Print((outName2+"_lep2.pdf").c_str());
  h_lep1pt->Draw();
  c1->Print((outName3+"_lep1_pt.pdf").c_str());
  h_lep2pt->Draw();
  c1->Print((outName3+"_lep2_pt.pdf").c_str());
  
  return 0;  

}


