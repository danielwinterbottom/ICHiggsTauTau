// ROOT includes
#include "TChain.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraphAsymmErrors.h"
#include "TH1D.h"
#include "TBranch.h"
#include "TStyle.h"

// C++ includes
#include <iostream>
#include <fstream>
#include <memory>
#include <map>
#include <string>
#include <cstdlib>

struct greater_Candidate{
  bool operator() (unsigned a,unsigned b) {
    return (a > b);
  }
};

int main(int argc, char* argv[]){

  if(argc < 4){
    std::cout << "Wrong number of arguments, expected minimum 3 arguments." << std::endl;
    return 0;
  }
  std::string channel      = argv[1];
  std::string signalType   = argv[2];
  std::string TriggerName[10];
  
  unsigned numberofTriggers = argc - 3;
  std::cout << "Numbers of triggers to process = " << numberofTriggers << std::endl;
  for(unsigned i=0; i<numberofTriggers; ++i){
    TriggerName[i] = argv[3 + i];
  }
  
  std::cout << "Channel: "            << channel        << std::endl;
  std::cout << "Signal Sample: "      << signalType     << std::endl;
  for(unsigned i=0; i<numberofTriggers; ++i){
    std::cout << "Trigger " << i+1 << ": " << TriggerName[i] << std::endl;
  }
  
  std::string filename1;
  if(signalType == "GluGlu"){
    filename1 = "output/HighLumi/reHLT_GluGluHToTauTau_M-125_"+channel+"_2015.root";

  } else if (signalType == "VBF") {
    filename1 = "output/HighLumi/reHLT_VBFHToTauTau_M-125_"+channel+"_2015.root";
  } else if (signalType == "QCD"){
    filename1 = "output/Jul03/LegsSeperate4/QCD_Pt-15to80_MixedSamples_MuEnrichedPt5_"+channel+"_2015.root";  
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
  double eta_1;
  tIn->SetBranchAddress("eta_1",&eta_1);  
  double eta_2;
  tIn->SetBranchAddress("eta_2",&eta_2); 
  
  bool PassedTrigger[10];
  
  for(unsigned i=0; i<numberofTriggers; ++i){
    tIn->SetBranchAddress(TriggerName[i].c_str(),&PassedTrigger[i]);
  }
  
  double ExtraEtaCut1[10] = { 5 };
  double ExtraEtaCut2[10] = { 5 };
  double ExtraPtCut1[10] = { 0 };
  double ExtraPtCut2[10] = { 0 };
  
  for(unsigned i=0; i <numberofTriggers; i++){
    if (TriggerName[i] == "HLT_IsoMu22_v"){
      if(channel == "mt"){
        ExtraEtaCut2[i] = 2.3;
        ExtraPtCut2[i] = 20;
        ExtraPtCut1[i] = 23;
        ExtraEtaCut1[i] = 2.4;
      } else if (channel == "em"){
        ExtraEtaCut2[i] = 23;
        ExtraEtaCut2[i] = 2.4;
      }
    } else if (TriggerName[i] == "HLT_IsoTkMu22_v"){
      ExtraEtaCut1[i] = 2.4;
      if(channel == "mt") ExtraEtaCut2[i] = 2.3;
      ExtraPtCut1[i] = 23;
      ExtraPtCut2[i] = 20;
    } else if(TriggerName[i] == "HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v"){
      ExtraEtaCut1[i] = 2.1;
      ExtraEtaCut2[i] = 2.3;
      ExtraPtCut1[i]  = 20;
      ExtraPtCut2[i]  = 20;
    } else if(TriggerName[i] == "HLT_Ele25_eta2p1_WPTight_Gsf_v"){
      ExtraEtaCut1[i] = 2.1;
      ExtraEtaCut2[i] = 2.3;
      ExtraPtCut1[i] = 26;
      ExtraPtCut2[i] = 20;
    } else if(TriggerName[i] == "HLT_Ele27_eta2p1_WPLoose_Gsf_v"){
      ExtraEtaCut1[i] = 2.1;
      ExtraEtaCut2[i] = 2.3;
      ExtraPtCut1[i] = 28;
      ExtraPtCut2[i] = 20;
    } else if(TriggerName[i] == "HLT_Ele27_eta2p1_WPTight_Gsf_v"){
      ExtraEtaCut1[i] = 2.1;
      ExtraEtaCut2[i] = 2.3;
      ExtraPtCut1[i] = 28;
      ExtraPtCut2[i] = 20;
    } else if(TriggerName[i] == "HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v"){
      ExtraEtaCut1[i] = 2.1;
      ExtraEtaCut2[i] = 2.3;
      ExtraPtCut1[i] = 25;
      ExtraPtCut2[i] = 20;
    } else if(TriggerName[i] == "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v"){
      ExtraEtaCut1[i] = 2.4;
      ExtraEtaCut2[i] = 2.5;
      ExtraPtCut1[i] = 24;
      ExtraPtCut2[i] = 9;
    } else if(TriggerName[i] == "HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v"){
      ExtraEtaCut1[i] = 2.4;
      ExtraEtaCut2[i] = 2.5;
      ExtraPtCut1[i] = 9;
      ExtraPtCut2[i] = 24;
    } else if(TriggerName[i] == "HLT_DoubleMediumIsoPFTau32_Trk1_eta2p1_Reg_v"){
      ExtraEtaCut1[i] = 2.1;
      ExtraEtaCut2[i] = 2.1;
      ExtraPtCut1[i] = 40;
      ExtraPtCut2[i] = 40;
    } else if(TriggerName[i] == "HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau30_v"){
      ExtraEtaCut1[i] = 2.1;
      ExtraEtaCut2[i] = 2.1;
      ExtraPtCut1[i] = 25;
      ExtraPtCut2[i] = 40; //may have to change this cut need to look at efficiency curve for tau leg!
    } else if(TriggerName[i] == "HLT_Ele32_eta2p1_WPTight_Gsf_v_1pt45e34"){
      ExtraEtaCut1[i] = 2.1;
      ExtraEtaCut2[i] = 2.3;
      ExtraPtCut1[i] = 33;
      ExtraPtCut2[i] = 0;
    } else if(TriggerName[i] == "HLT_IsoMu21_eta2p1_LooseIsoPFTau20_SingleL1_v"){
      ExtraEtaCut1[i] = 2.1;
      ExtraEtaCut2[i] = 2.3;
      ExtraPtCut1[i]  = 22;
      ExtraPtCut2[i]  = 20;
    } else if (TriggerName[i] == "HLT_IsoMu22_eta2p1_v"){
      ExtraEtaCut2[i] = 2.3;
      ExtraPtCut2[i] = 20;
      ExtraPtCut1[i] = 23;
      ExtraEtaCut1[i] = 2.1;
    } else if (TriggerName[i] == "HLT_IsoMu27_v"){
      ExtraEtaCut2[i] = 2.3;
      ExtraPtCut2[i] = 20;
      ExtraPtCut1[i] = 28;
      ExtraEtaCut1[i] = 2.4;
    }
  }
  
  unsigned countTotal = 0;
  unsigned countTrigger[10] = { 0 };
  unsigned countCross = 0;
  
  for(unsigned i=0; i<nentries1; ++i){
    tIn->GetEntry(i);
    bool PassedBaseLine = false;
    if      ((channel == "et" || channel == "mt") && iso_1<0.1 && mva_olddm_tight_2 && antiele_2 && antimu_2 && !leptonveto) PassedBaseLine = true;
    else if (channel == "tt" && mva_olddm_vtight_1>0.5 && mva_olddm_vtight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto) PassedBaseLine = true;
    else if (channel == "em" && iso_1<0.15 && iso_2<0.15 && !leptonveto) PassedBaseLine = true;
    if(PassedBaseLine){
      countTotal++;
      bool AlreadyCounted = false;
      for(unsigned j=0; j<numberofTriggers; ++j){
        if(PassedTrigger[j] && pt_1 > ExtraPtCut1[j] && pt_2 > ExtraPtCut2[j] && std::fabs(eta_1) < ExtraEtaCut1[j] && std::fabs(eta_2) < ExtraEtaCut2[j]){
          countTrigger[j]++;
          if(!AlreadyCounted){
            countCross++;
            AlreadyCounted = true;
          }
        }
      }
    }
  }
  std::string ORname;
  std::cout << "Total events passing offline selection: " << countTotal << std::endl;
  for(unsigned i=0; i<numberofTriggers; ++i){
    std::cout << "Events passing " << TriggerName[i] << ": " << countTrigger[i] << std::endl;
    if(i==0) ORname = TriggerName[i];
    else ORname += (" OR "+TriggerName[i]);
  }
  
  if(numberofTriggers > 1){
    std::cout << "Events passing " << ORname << ": " << countCross << std::endl;
    unsigned maxCount = 0;
    for(unsigned i=0; i<10; ++i){
      if(countTrigger[i] > maxCount) maxCount = countTrigger[i];   
    }
    
    double gains = 100*((double)countCross/maxCount -1.);
    std::cout << "Gain from using OR trigger = " << gains << "%" << std::endl;
  }
  
  
  return 0;  

}


