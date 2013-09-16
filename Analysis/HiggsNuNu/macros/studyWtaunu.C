#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <iomanip>

#include "TMath.h"
#include "TFile.h"
#include "TH1F.h" 
#include "TH2F.h" 
#include "TGraphErrors.h" 
#include "TStyle.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TGraphAsymmErrors.h"


int studyWtaunu(){//main

  std::vector<std::string> Wfiles;
  Wfiles.push_back("WJetsToLNu-v1_taunu");
  Wfiles.push_back("WJetsToLNu-v2_taunu");
  Wfiles.push_back("W1JetsToLNu_taunu");
  Wfiles.push_back("W2JetsToLNu_taunu");
  Wfiles.push_back("W3JetsToLNu_taunu");
  Wfiles.push_back("W4JetsToLNu_taunu");
  Wfiles.push_back("EWK-W2jminus_taunu");
  Wfiles.push_back("EWK-W2jplus_taunu");

  std::string folder = "output/nunu/Skim/";
  std::string suffix = "";

  const unsigned nWFiles = Wfiles.size();

  TFile *fW[nWFiles];

  double lLumi = 19619.;
  
  double w_normalisation[nWFiles];
  for (unsigned iN(0); iN<6;++iN){
    w_normalisation[iN] = 37509/76102995.0;
  }
  w_normalisation[6] = 4.09/4696648;
  w_normalisation[7] = 6.48/6776164;
  

  TCanvas *myc = new TCanvas("myc","",800,1000);
  gStyle->SetOptStat(0);
  //myc->Divide(2,3);

  std::ostringstream lPath;

  const unsigned nHists = 1;
  //std::string lHistName[nHists] = {"Gen/taupt","Gen/taueta","Gen/tauptvseta"};
  std::string lHistName[nHists] = {"Gen/tauptvseta"};
  std::vector<std::string> selections;
  selections.push_back("AN");
  selections.push_back("DEta");
  selections.push_back("MET");
  selections.push_back("TightMjj");
  selections.push_back("DPhiSIGNAL_noCJV");
  
  const unsigned nSel = selections.size();

  TH2F *hist[nHists];

  for (unsigned iS(0); iS<nSel; ++iS){//loop on selections
    lPath.str("");
    lPath << "../" << folder << "/ICfiles/MC_";
    
    for (unsigned iP(0); iP<nHists; ++iP){//loop on points
      
      //get histograms
      for (unsigned iW(0); iW<nWFiles; ++iW){//loop on w files
	fW[iW] = TFile::Open((lPath.str()+Wfiles[iW]+".root").c_str());
	//fW[iW]->cd((lSelection[iS]+"/weights/").c_str());
	//if (iP < 2) 
	fW[iW]->cd((selections[iS]+"/").c_str());
	//else fW[iW]->cd((selections[iS]+"/weights/").c_str());
	if (iW==0) {
	  hist[iP] = (TH2F*)gDirectory->Get(lHistName[iP].c_str())->Clone();
	  hist[iP]->Sumw2();
	  hist[iP]->Scale(lLumi*w_normalisation[iW]);
	}
	else {
	  TH2F *lTmpHist = (TH2F*)gDirectory->Get(lHistName[iP].c_str());
	  lTmpHist->Scale(lLumi*w_normalisation[iW]);
	  hist[iP]->Add(lTmpHist);
	}
      }//loop on w hists
      
      std::cout << " -- total entries = " << hist[iP]->Integral() 
		<< " + " << hist[iP]->Underflows() 
		<< " + " << hist[iP]->Overflows() 
		<< std::endl;

      myc->cd();
      //hist[iP]->RebinY(10);
      hist[iP]->GetYaxis()->SetRangeUser(0,200);
      hist[iP]->GetXaxis()->SetTitle("#eta (#tau genjet)");
      hist[iP]->GetYaxis()->SetTitle("p_{T}(#tau genjet) (GeV)");
      hist[iP]->SetTitle(("MC W#rightarrow #tau#nu, "+selections[iS]).c_str());
      hist[iP]->SetLineColor(1+iP);
      hist[iP]->SetMarkerColor(1+iP);
      hist[iP]->SetMarkerStyle(20+iP);
      //if (iP==0) 
      hist[iP]->Draw("colz");
      myc->Update();
      myc->Print(("Wtau_"+selections[iS]+"_"+lHistName[iP]+".pdf").c_str());
      //else hist[iP]->Draw("histsame");
    }
    
  }//loop on selections
  
  return 0;
    
    
}//main
