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


int closureTestEmbedding() {//main

  const unsigned nFiles = 2;
  TFile *input[2];
  input[0] = TFile::Open("../output/W2jNormal.root");
  input[1] = TFile::Open("../output/W2jEmbedded.root");


  const unsigned nHists = 7;
  std::string histName[nHists] = {
    "Gen/taupt",
    "Gen/taueta",
    "n_taus",
    "taupt_1",
    "taueta_1",
    "dRmin_taujet",
    "met"
  };

    std::string histString[nHists] = {
    "Gentaupt",
    "Gentaueta",
    "n_taus",
    "taupt_1",
    "taueta_1",
    "dRmin_taujet",
    "met"
  };

    std::string xAxisLabel[nHists] = {
    "#tau genjet p_{T} (GeV)",
    "#tau genjet #eta",
    "#tau multiplicity",
    "PF #tau p_{T} (GeV)",
    "PF #tau #eta",
    "#Delta R_{min}(#tau,tag jet)",
    "MET (GeV)"
  };

  double minX[nHists] = {0,-4,0,0,-3,0,0};
  double maxX[nHists] = {200,4,5,200,3,6,300};
  int logy[nHists] = {0,0,0,0,0,1,0};
  int rebin[nHists] = {2,4,1,2,4,2,4};
  //std::string xTitle[nHists] = {};
  int color[nFiles] = {1,2};
  int marker[nFiles] = {22,23};


  TCanvas *myc[nHists];

  for (unsigned i(0); i<nHists;++i){
    std::ostringstream lCanTitle;
    lCanTitle << "myc" << i ;
    myc[i] = new TCanvas(lCanTitle.str().c_str(),histString[i].c_str(),1);
  }

  //gStyle->SetOptStat("euoi");
  gStyle->SetOptStat(0);

  std::vector<std::string> selections;
  selections.push_back("HLTMetClean");
  selections.push_back("WSelection");

  const unsigned nSel = selections.size();

  TH1F *hist[nHists][nFiles];
  TLegend *leg;
  
  for (unsigned iS(0); iS<nSel; ++iS){//loop on selections
    for (unsigned iP(0); iP<nHists; ++iP){//loop on points
      //get histograms
      myc[iP]->cd();
      for (unsigned iW(0); iW<nFiles; ++iW){//loop on w files
	input[iW]->cd((selections[iS]+"/").c_str());
	hist[iP][iW] = (TH1F*)gDirectory->Get(histName[iP].c_str())->Clone();
	hist[iP][iW]->GetXaxis()->SetTitle(xAxisLabel[iP].c_str());
	hist[iP][iW]->GetYaxis()->SetTitle("Events");
	hist[iP][iW]->SetTitle("");
	hist[iP][iW]->SetLineColor(color[iW]);
	hist[iP][iW]->SetMarkerColor(color[iW]);
	hist[iP][iW]->SetMarkerStyle(marker[iW]);
	gPad->SetLogy(logy[iP]);

	hist[iP][iW]->Rebin(rebin[iP]);
	hist[iP][iW]->GetXaxis()->SetRangeUser(minX[iP],maxX[iP]);
	if (iW==0) hist[iP][iW]->Draw("PE");
	else {
	  hist[iP][iW]->Scale(hist[iP][0]->GetEntries()/hist[iP][iW]->GetEntries());
	  hist[iP][iW]->Draw("PEsame");
	}
      }

      leg = new TLegend(0.3,0.9,1,1);
      leg->SetFillColor(10);

      std::ostringstream leglabel;
      leglabel << std::setprecision(3) << "Normal mean=" << hist[iP][0]->GetMean() << std::setprecision(1) << "\\pm" << hist[iP][0]->GetMeanError() << ", RMS=" << std::setprecision(3) << hist[iP][0]->GetRMS();
      leg->AddEntry(hist[iP][0],leglabel.str().c_str(),"P");
      leglabel.str("");
      leglabel << std::setprecision(3) << "Embedded mean=" << hist[iP][1]->GetMean() << "\\pm"  << std::setprecision(1)<< hist[iP][1]->GetMeanError()  << ", RMS=" << std::setprecision(3) << hist[iP][1]->GetRMS();
      leg->AddEntry(hist[iP][1],leglabel.str().c_str(),"P");
      leg->Draw("same");
      
      myc[iP]->Update();
      myc[iP]->Print(("PLOTS/Embedding_"+selections[iS]+"_"+histString[iP]+".pdf").c_str());
      
    }
  }


  return 0;
  

}//main
