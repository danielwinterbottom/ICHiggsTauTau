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
      TriggerName[1] = "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v"; 
      TriggerName[0] = "HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v"; 
    } else if(channel == "tt") {
      TriggerName[0] = "HLT_DoubleMediumIsoPFTau32_Trk1_eta2p1_Reg_v"; 
      TriggerName[1] = "HLT_DoubleMediumIsoPFTau32_Trk1_eta2p1_Reg_v"; 
    }
  }
  else if(argc < 6){
    TriggerName[0] = argv[4];
    if(channel == "mt")      TriggerName[1] = "HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v";
    else if(channel == "et") TriggerName[1] = "HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v" ;
    else if(channel == "em") TriggerName[1] = "HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v";
    else if(channel == "tt") TriggerName[1] = "HLT_DoubleMediumIsoPFTau32_Trk1_eta2p1_Reg_v";
  }
  else {
   TriggerName[0] = argv[4];
   TriggerName[1] = argv[5];   
  }
  
  double ExtraEtaCut1[2] = {};
  double ExtraEtaCut2[2] = {};
  for(unsigned i = 0; i < 2 ; ++i){
    if     (TriggerName[i] == "HLT_IsoMu22_v"){
      ExtraEtaCut1[i] = 2.4;
      ExtraEtaCut2[i] = 2.3;
    } else if(TriggerName[i] == "HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v"){
      ExtraEtaCut1[i] = 2.1;
      ExtraEtaCut2[i] = 2.3;
    } else if(TriggerName[i] == "HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v"){
      ExtraEtaCut1[i] = 2.1;
      ExtraEtaCut2[i] = 2.3;
    } else if(TriggerName[i] == "HLT_Ele25_eta2p1_WPTight_Gsf_v"){
      ExtraEtaCut1[i] = 2.1;
      ExtraEtaCut2[i] = 2.3;
    } else if(TriggerName[i] == "HLT_Ele27_eta2p1_WPLoose_Gsf_v"){
      ExtraEtaCut1[i] = 2.1;
      ExtraEtaCut2[i] = 2.3;
    } else if(TriggerName[i] == "HLT_Ele27_eta2p1_WPTight_Gsf_v"){
      ExtraEtaCut1[i] = 2.1;
      ExtraEtaCut2[i] = 2.3;
    } else if(TriggerName[i] == "HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v"){
      ExtraEtaCut1[i] = 2.1;
      ExtraEtaCut2[i] = 2.3;
    } else if(TriggerName[i] == "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v"){
      ExtraEtaCut1[i] = 2.4;
      ExtraEtaCut2[i] = 2.5;
    } else if(TriggerName[i] == "HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v"){
      ExtraEtaCut1[i] = 2.4;
      ExtraEtaCut2[i] = 2.5;
    } else if(TriggerName[i] == "HLT_DoubleMediumIsoPFTau32_Trk1_eta2p1_Reg_v"){
      ExtraEtaCut1[i] = 2.1;
      ExtraEtaCut2[i] = 2.1;
    }
  }
 // 
 // ExtraPtCut1[0] = 0;
 // ExtraPtCut2[0] = 0;
 // ExtraPtCut1[1] = 0;
 // ExtraPtCut2[1] = 0;
  
  std::cout << "Channel: "            << channel        << std::endl;
  std::cout << "Output Directory: = " << outputDir      << std::endl;
  std::cout << "Signal Sample: "      << signalType     << std::endl;
  std::cout << "Trigger 1: "          << TriggerName[0] << std::endl;
  std::cout << "Trigger 2: "          << TriggerName[1] << std::endl;
  
  std::string filename1;
  if(signalType == "GluGlu"){
    filename1 = "output/Jul03/LegsSeperate2/GluGluHToTauTau_M-125_"+channel+"_2015.root";
  } else if (signalType == "VBF") {
    filename1 = "output/Jul03/LegsSeperate2/VBFHToTauTau_M-125_"+channel+"_2015.root";
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
  
  bool PassedTrigger1;
  bool PassedTrigger2;
  bool PassedTrigger1_leg1;
  bool PassedTrigger2_leg1;
  bool PassedTrigger1_leg2;
  bool PassedTrigger2_leg2;

  tIn->SetBranchAddress(TriggerName[0].c_str(),&PassedTrigger1);
  tIn->SetBranchAddress((TriggerName[0]+"_leg1_match").c_str(),&PassedTrigger1_leg1);
  tIn->SetBranchAddress((TriggerName[0]+"_leg2_match").c_str(),&PassedTrigger1_leg2);
  
  tIn->SetBranchAddress(TriggerName[1].c_str(),&PassedTrigger2);
  tIn->SetBranchAddress((TriggerName[1]+"_leg1_match").c_str(),&PassedTrigger2_leg1);
  tIn->SetBranchAddress((TriggerName[1]+"_leg2_match").c_str(),&PassedTrigger2_leg2);
  
  unsigned countTotal = 0;
  unsigned countTrigger1 = 0;
  unsigned countTrigger2 = 0;
  unsigned countCross = 0;
  
  std::string outfilename = outputDir+"/"+signalType+"_"+channel+".root";
  TFile *fOut = new TFile(outfilename.c_str(),"UPDATE");
  
  //TStyle *myStyle = (TStyle*)gROOT->GetListOfSpecials()->FindObject("myStyle");
  
  TCanvas *c1 = new TCanvas("c1");
  c1->SetTickx(1);
  c1->SetTicky(1);
  
  float bins[21] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,30,34,40,48,56,70,100};
  int   binnum = sizeof(bins)/sizeof(float) - 1;
  
  
  TH1D *h_lep1pt                = new TH1D("h_lep1pt", "h_lep1pt", binnum, bins);
  TH1D *h_lep2pt                = new TH1D("h_lep2pt", "h_lep2pt", binnum, bins);
  TH1D *h_lep1pt_denum_trigger1 = new TH1D("h_lep1pt_denum_trigger1", "h_lep1pt_denum_trigger1", binnum, bins);
  TH1D *h_lep2pt_denum_trigger1 = new TH1D("h_lep2pt_denum_trigger1", "h_lep2pt_denum_trigger1", binnum, bins);
  TH1D *h_lep1pt_denum_trigger2 = new TH1D("h_lep1pt_denum_trigger2", "h_lep1pt_denum_trigger2", binnum, bins);
  TH1D *h_lep2pt_denum_trigger2 = new TH1D("h_lep2pt_denum_trigger2", "h_lep2pt_denum_trigger2", binnum, bins);
  TH1D *h_lep1pt_denum_ORtrigger = new TH1D("h_lep1pt_denum_ORtrigger", "h_lep1pt_denum_ORtrigger", binnum, bins);
  TH1D *h_lep2pt_denum_ORtrigger = new TH1D("h_lep2pt_denum_ORtrigger", "h_lep2pt_denum_ORtrigger", binnum, bins);
  TH1D *h_lep1_eff_trigger1     = new TH1D("h_lep1_eff_trigger1", "h_lep1_eff_trigger1", binnum, bins);
  TH1D *h_lep2_eff_trigger1     = new TH1D("h_lep2_eff_trigger1", "h_lep2_eff_trigger1", binnum, bins);
  TH1D *h_lep1_eff_trigger2     = new TH1D("h_lep1_eff_trigger2", "h_lep1_eff_trigger2", binnum, bins);
  TH1D *h_lep2_eff_trigger2     = new TH1D("h_lep2_eff_trigger2", "h_lep2_eff_trigger2", binnum, bins);
  TH1D *h_lep1_eff_ORtrigger = new TH1D("h_lep1_eff_ORtrigger", "h_lep1_eff_ORtrigger", binnum, bins);
  TH1D *h_lep2_eff_ORtrigger = new TH1D("h_lep2_eff_ORtrigger", "h_lep2_eff_ORtrigger", binnum, bins);
  
  h_lep1pt_denum_trigger1->Sumw2();
  h_lep2pt_denum_trigger1->Sumw2();
  h_lep1pt_denum_trigger2->Sumw2();
  h_lep2pt_denum_trigger2->Sumw2();
  h_lep1pt_denum_ORtrigger->Sumw2();
  h_lep2pt_denum_ORtrigger->Sumw2();
  h_lep1_eff_trigger1->Sumw2();
  h_lep2_eff_trigger1->Sumw2();
  h_lep1_eff_trigger2->Sumw2();
  h_lep2_eff_trigger2->Sumw2();
  h_lep1_eff_ORtrigger->Sumw2();
  h_lep2_eff_ORtrigger->Sumw2();
  h_lep1_eff_trigger1->SetStats(0);
  h_lep2_eff_trigger1->SetStats(0);
  h_lep1_eff_trigger2->SetStats(0);
  h_lep2_eff_trigger2->SetStats(0);
  h_lep1_eff_ORtrigger->SetStats(0);
  h_lep2_eff_ORtrigger->SetStats(0);
  h_lep1_eff_trigger1->GetYaxis()->SetTitle("Efficiency");
  h_lep2_eff_trigger1->GetYaxis()->SetTitle("Efficiency");
  h_lep1_eff_trigger2->GetYaxis()->SetTitle("Efficiency");
  h_lep2_eff_trigger2->GetYaxis()->SetTitle("Efficiency");
  h_lep1_eff_ORtrigger->GetYaxis()->SetTitle("Efficiency");
  h_lep2_eff_ORtrigger->GetYaxis()->SetTitle("Efficiency");
  h_lep1_eff_trigger1->GetXaxis()->SetTitle("lep1_pt [GeV]");
  h_lep2_eff_trigger1->GetXaxis()->SetTitle("lep2_pt [GeV]");
  h_lep1_eff_trigger2->GetXaxis()->SetTitle("lep1_pt [GeV]");
  h_lep2_eff_trigger2->GetXaxis()->SetTitle("lep2_pt [GeV]");
  h_lep1_eff_ORtrigger->GetXaxis()->SetTitle("lep1_pt [GeV]");
  h_lep2_eff_ORtrigger->GetXaxis()->SetTitle("lep2_pt [GeV]");
  
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
      if(PassedTrigger1) countTrigger1++;
      if(PassedTrigger2) countTrigger2++;
      if(PassedTrigger1 || PassedTrigger2) countCross++;
      
      if(channel == "tt"){
        if(eta_1  < ExtraEtaCut1[0] && eta_2  < ExtraEtaCut2[0] && PassedTrigger1_leg2) h_lep1pt_denum_trigger1->Fill(pt_1);
        if(eta_1  < ExtraEtaCut1[1] && eta_2  < ExtraEtaCut2[1] && PassedTrigger2_leg2) h_lep1pt_denum_trigger2->Fill(pt_1);
      } else{
        if(eta_1  < ExtraEtaCut1[0] && eta_2  < ExtraEtaCut1[0]) h_lep1pt_denum_trigger1->Fill(pt_1);
        if(eta_1  < ExtraEtaCut1[1] && eta_2  < ExtraEtaCut1[1]) h_lep1pt_denum_trigger2->Fill(pt_1);
      }
      if(eta_2  < ExtraEtaCut2[0] && eta_1  < ExtraEtaCut1[0] && PassedTrigger1_leg1) h_lep2pt_denum_trigger1->Fill(pt_2);
      if(eta_2  < ExtraEtaCut2[1] && eta_1  < ExtraEtaCut1[1] && PassedTrigger2_leg1) h_lep2pt_denum_trigger2->Fill(pt_2);
      if((eta_1  < ExtraEtaCut1[0] && eta_2  < ExtraEtaCut2[0]) || (eta_1  < ExtraEtaCut1[1] && eta_2  < ExtraEtaCut2[1])) h_lep1pt_denum_ORtrigger->Fill(pt_1);
      if((eta_2  < ExtraEtaCut2[0] && eta_1  < ExtraEtaCut1[0] && PassedTrigger1_leg1) || (eta_2  < ExtraEtaCut2[1] && eta_1  < ExtraEtaCut1[1] && PassedTrigger2_leg1)) h_lep2pt_denum_ORtrigger->Fill(pt_2);

        if(channel == "tt"){
          if(eta_2  < ExtraEtaCut2[0] && eta_1  < ExtraEtaCut1[0] && PassedTrigger1_leg1 && PassedTrigger1_leg2) h_lep1_eff_trigger1->Fill(pt_1);
          if(eta_2  < ExtraEtaCut2[1] && eta_1  < ExtraEtaCut1[1] && PassedTrigger2_leg1 && PassedTrigger2_leg2) h_lep1_eff_trigger2->Fill(pt_1);
        } else {
          if(eta_2  < ExtraEtaCut2[0] && eta_1  < ExtraEtaCut1[0] && PassedTrigger1_leg1) h_lep1_eff_trigger1->Fill(pt_1);
          if(eta_2  < ExtraEtaCut2[1] && eta_1  < ExtraEtaCut1[1] && PassedTrigger2_leg1) h_lep1_eff_trigger2->Fill(pt_1);
        }
        if(eta_2  < ExtraEtaCut2[0] && eta_1  < ExtraEtaCut1[0] && PassedTrigger1_leg1 && PassedTrigger1_leg2) h_lep2_eff_trigger1->Fill(pt_2);
        if(eta_2  < ExtraEtaCut2[1] && eta_1  < ExtraEtaCut1[1] && PassedTrigger2_leg1 && PassedTrigger2_leg2) h_lep2_eff_trigger2->Fill(pt_2);

        if((eta_1  < ExtraEtaCut1[0] && eta_2  < ExtraEtaCut2[0] && PassedTrigger1_leg1) || (eta_1  < ExtraEtaCut1[1] && eta_2  < ExtraEtaCut2[1] && PassedTrigger2_leg1)) h_lep1_eff_ORtrigger->Fill(pt_1);
        if((eta_2  < ExtraEtaCut2[0] && eta_1  < ExtraEtaCut1[0] && PassedTrigger1_leg1 && PassedTrigger1_leg1) || (eta_2  < ExtraEtaCut2[1] && eta_1  < ExtraEtaCut1[1] && PassedTrigger2_leg1 && PassedTrigger2_leg2)) h_lep2_eff_ORtrigger->Fill(pt_2);
    }
  }
  
  std::cout << "Total events passing offline selection: " << countTotal << std::endl;
  std::cout << "Events passing " << TriggerName[0] << ": " << countTrigger1 << std::endl;
  std::cout << "Events passing " << TriggerName[1] << ": " << countTrigger2 << std::endl;
  std::cout << "Events passing " << TriggerName[0] << " OR " << TriggerName[1] << ": " << countCross << std::endl;
  double gains = 100*((double)countCross/std::max(countTrigger1,countTrigger2)-1.);
  std::cout << "Gain from using OR trigger = " << gains << "%" << std::endl;
  
  std::string outName1 = signalType+"_"+channel+"_"+TriggerName[0];
  std::string outName2 = signalType+"_"+channel+"_"+TriggerName[1];
  std::string outName3 = signalType+"_"+channel+"_"+TriggerName[0]+"_OR_"+TriggerName[1];
  
  TGraphAsymmErrors* gr1;
  TGraphAsymmErrors* gr2;
  TGraphAsymmErrors* gr3;
  TGraphAsymmErrors* gr4;
  TGraphAsymmErrors* gr5;
  TGraphAsymmErrors* gr6;

  std::string gr_name;
  std::string x_name_lep1;
  std::string x_name_lep2;
  
  if        (channel == "mt"){
    x_name_lep1 = "muon p_{T} [GeV]";    
    x_name_lep2 = "tau p_{T} [GeV]";
  } else if (channel == "et"){
    x_name_lep1 = "electron p_{T} [GeV]";    
    x_name_lep2 = "tau p_{T} [GeV]";
  } else if (channel == "em"){
    x_name_lep1 = "electron p_{T} [GeV]";    
    x_name_lep2 = "muon p_{T} [GeV]";
  } else if (channel == "tt"){
    x_name_lep1 = "leading tau p_{T} [GeV]";    
    x_name_lep2 = "sub-leading tau p_{T} [GeV]";
  }
  
  fOut->cd();
  
  double xmax;
  if(channel != "tt") {
    
    gr1 = new TGraphAsymmErrors(binnum);
    gr1->Divide(h_lep1_eff_trigger1, h_lep1pt_denum_trigger1);
    gr1->SetObjectStat(0);
    gr1->GetYaxis()->SetTitle("Efficiency");
    gr1->GetYaxis()->SetRangeUser(0,1.1);
    gr_name = outName1+"_lep1";
    gr1->SetTitle(TriggerName[0].c_str());
    gr1->GetXaxis()->SetTitle(x_name_lep1.c_str());
    xmax = h_lep1_eff_trigger1->GetXaxis()->GetXmax();
    gr1->GetXaxis()->SetRangeUser(0,xmax);
    gr1->Write(gr_name.c_str());
    delete gr1;
    h_lep1_eff_trigger1->GetYaxis()->SetRangeUser(0,1.1);
    h_lep1_eff_trigger1->Divide(h_lep1pt_denum_trigger1);
    h_lep1_eff_trigger1->Draw();
    c1->Print((gr_name+".png").c_str());
    
    gr2 = new TGraphAsymmErrors(binnum);
    gr2->Divide(h_lep2_eff_trigger1, h_lep2pt_denum_trigger1);
    gr2->SetObjectStat(0);
    gr2->GetYaxis()->SetTitle("Efficiency");
    gr2->GetYaxis()->SetRangeUser(0,1.1);
    gr_name = outName1+"_lep2";
    gr2->SetTitle(TriggerName[0].c_str());
    gr2->GetXaxis()->SetTitle(x_name_lep2.c_str());
    xmax = h_lep2_eff_trigger1->GetXaxis()->GetXmax();
    gr2->GetXaxis()->SetRangeUser(0,xmax);
    gr2->Write(gr_name.c_str());
    delete gr2;
    h_lep2_eff_trigger1->GetYaxis()->SetRangeUser(0,1.1);
    h_lep2_eff_trigger1->Divide(h_lep2pt_denum_trigger1);
    h_lep2_eff_trigger1->Draw();
    c1->Print((gr_name+".png").c_str());
  }

  //h_lep1_eff_trigger2->Draw();
  //c1->Print("leadtau_numerator.png");
  //h_lep1pt_denum_trigger2->Draw();
  //c1->Print("leadtau_denumerator.png");
  //h_lep1_eff_trigger2->Divide(h_lep1pt_denum_trigger2);
  //h_lep1_eff_trigger2->Draw();
  //c1->Print("leadtau_eff.png");
  gr3 = new TGraphAsymmErrors(binnum);
  gr3->Divide(h_lep1_eff_trigger2, h_lep1pt_denum_trigger2);
  gr3->SetObjectStat(0);
  gr3->GetYaxis()->SetTitle("Efficiency");
  gr3->GetYaxis()->SetRangeUser(0,1.1);
  gr_name = outName2+"_lep1";
  gr3->SetTitle(TriggerName[1].c_str());
  gr3->GetXaxis()->SetTitle(x_name_lep1.c_str());
  xmax = h_lep1_eff_trigger2->GetXaxis()->GetXmax();
  gr3->GetXaxis()->SetRangeUser(0,xmax);
  gr3->Write(gr_name.c_str());
  delete gr3;
  h_lep1_eff_trigger2->GetYaxis()->SetRangeUser(0,1.1);
  h_lep1_eff_trigger2->Divide(h_lep1pt_denum_trigger2);
  h_lep1_eff_trigger2->Draw();
  c1->Print((gr_name+".png").c_str());
  
  //h_lep2_eff_trigger2->Draw();
  //c1->Print("subleadtau_numerator.png");
  //h_lep2pt_denum_trigger2->Draw();
  //c1->Print("subleadtau_denumerator.png");
  //h_lep2_eff_trigger2->Divide(h_lep2pt_denum_trigger2);
  //h_lep2_eff_trigger2->Draw();
  //c1->Print("subleadtau_eff.png");
  gr4 = new TGraphAsymmErrors(binnum);
  gr4->Divide(h_lep2_eff_trigger2, h_lep2pt_denum_trigger2);
  gr4->SetObjectStat(0);
  gr4->GetYaxis()->SetTitle("Efficiency");
  gr4->GetYaxis()->SetRangeUser(0,1.1);
  gr_name = outName2+"_lep2";
  gr4->SetTitle(TriggerName[1].c_str());
  gr4->GetXaxis()->SetTitle(x_name_lep2.c_str());
  xmax = h_lep2_eff_trigger2->GetXaxis()->GetXmax();
  gr4->GetXaxis()->SetRangeUser(0,xmax);
  gr4->Write(gr_name.c_str());
  delete gr4;
  h_lep2_eff_trigger2->GetYaxis()->SetRangeUser(0,1.1);
  h_lep2_eff_trigger2->Divide(h_lep2pt_denum_trigger2);
  h_lep2_eff_trigger2->Draw();
  c1->Print((gr_name+".png").c_str());
  
  if(channel != "tt") {
    gr5 = new TGraphAsymmErrors(binnum);
    gr5->Divide(h_lep1_eff_ORtrigger, h_lep1pt_denum_ORtrigger);
    gr5->SetObjectStat(0);
    gr5->GetYaxis()->SetTitle("Efficiency");
    gr5->GetYaxis()->SetRangeUser(0,1.1);
    gr_name = outName3+"_lep1";
    gr5->SetTitle((TriggerName[0]+"_OR_"+TriggerName[1]).c_str());
    gr5->GetXaxis()->SetTitle(x_name_lep1.c_str());
    xmax = h_lep1_eff_ORtrigger->GetXaxis()->GetXmax();
    gr5->GetXaxis()->SetRangeUser(0,xmax);
    gr5->Write(gr_name.c_str());
    delete gr5;
    h_lep1_eff_ORtrigger->GetYaxis()->SetRangeUser(0,1.1);
    h_lep1_eff_ORtrigger->Divide(h_lep1pt_denum_ORtrigger);
    h_lep1_eff_ORtrigger->Draw();
    c1->Print((gr_name+".png").c_str());
    
    gr6 = new TGraphAsymmErrors(binnum);
    gr6->Divide(h_lep2_eff_ORtrigger, h_lep2pt_denum_ORtrigger);
    gr6->SetObjectStat(0);
    gr6->GetYaxis()->SetTitle("Efficiency");
    gr6->GetYaxis()->SetRangeUser(0,1.1);
    gr_name = outName3+"_lep2";
    gr6->SetTitle((TriggerName[0]+"_OR_"+TriggerName[1]).c_str());
    gr6->GetXaxis()->SetTitle(x_name_lep2.c_str());
    xmax = h_lep2_eff_ORtrigger->GetXaxis()->GetXmax();
    gr6->GetXaxis()->SetRangeUser(0,xmax);
    gr6->Write(gr_name.c_str());
    delete gr6;
    h_lep2_eff_ORtrigger->GetYaxis()->SetRangeUser(0,1.1);
    h_lep2_eff_ORtrigger->Divide(h_lep2pt_denum_ORtrigger);
    h_lep2_eff_ORtrigger->Draw();
    c1->Print((gr_name+".png").c_str());
    
  }
  
  fOut->Write();
  
  return 0;  

}


