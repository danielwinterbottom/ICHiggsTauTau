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


int studyWembedding(){//main

  std::vector<std::string> Wfiles;
  Wfiles.push_back("WJetsToLNu-v1_taunu");
  Wfiles.push_back("WJetsToLNu-v2_taunu");
  Wfiles.push_back("W1JetsToLNu_taunu");
  Wfiles.push_back("W2JetsToLNu_taunu");
  Wfiles.push_back("W3JetsToLNu_taunu");
  Wfiles.push_back("W4JetsToLNu_taunu");
  Wfiles.push_back("EWK-W2jminus_taunu");
  Wfiles.push_back("EWK-W2jplus_taunu");

  const unsigned nFS = 2;
  std::string fsStr[nFS] = {"nunu","taunu"};
  std::string folder[nFS] = {
    "output_tauEmbed/nunu/MET130/",
    "output/tauEmbed/taunu/MET130/"
  };

  std::string suffix = "";
  
  const unsigned nWFiles = Wfiles.size();
  

  double lLumi = 19619.;
  double lLumiEmbedded = 9964.8*0.648; //correct for tau BR to hadronic.

  double w_normalisation[nWFiles];
  for (unsigned iN(0); iN<6;++iN){
    w_normalisation[iN] = 37509/76102995.0;
  }
  w_normalisation[6] = 4.09/4696648;
  w_normalisation[7] = 6.48/6776164;
  

  //gStyle->SetOptStat("euoi");

  std::ostringstream lPath;

  const unsigned nHists = 7;
  TCanvas *myc[nHists];
  for (unsigned(iC); iC<nHists;++iC){
    std::ostringstream lName;
    lName << "myc" << iC;
    myc[iC] = new TCanvas(lName.str().c_str(),"",4);
  }
  
  //std::string lHistName[nHists] = {"Gen/taupt","Gen/taueta","Gen/tauptvseta"};
  std::string lHistName[nHists] = {"n_jets","n_jetsingap","met","jpt_1","jpt_2","taupt_1","taueta_1"};
  std::string histStr[nHists] = {"n_jets","n_jetsingap","met","jpt_1","jpt_2","taupt_1","taueta_1"};

  std::string xTitle[nHists] = {"n_jets","n_jetsingap","met","jpt_1","jpt_2","taupt_1","taueta_1"};
  std::string yTitle[nHists] = {"Events","Events","Events","Events","Events","Events","Events"};

  double minX[nHists] = {0,0,0,0,0,0,-3};
  double maxX[nHists] = {10,10,300,250,250,250,3};
  int logy[nHists] = {1,1,1,1,1,1,0};
  int rebin[nHists] = {1,1,10,5,5,5,2};

  std::vector<std::string> selections;
  selections.push_back("JetPair");
  selections.push_back("VBF");
  //selections.push_back("tauID");
  
  const unsigned nSel = selections.size();

  for (unsigned iFS(0); iFS<nFS-1; ++iFS){

    std::cout << " -- Processing final state: " << fsStr[iFS] << std::endl;
    
    TFile *fW[nWFiles];
    TFile *fData[2];

    const unsigned nSamples=3;
    std::string sampleStr[nSamples] = {"MC","DataEmbedded","Data"};
    TH1 *hist[nHists][nSamples];
    TH1 *hist_ewk[nHists];
    TLegend *leg;
    double error[nHists][nSamples];
    double integral[nHists][nSamples];
    double error_ewk[nHists];
    double integral_ewk[nHists];

    std::ofstream lOutfile;
    lOutfile.open("EventNumbers_taunu_embed.txt");
    
    
    for (unsigned iS(0); iS<nSel; ++iS){//loop on selections
      lPath.str("");
      lPath << "../" << folder[iFS] << "/MC_";
      
      lOutfile << selections[iS] << " & $ " ;
      
      for (unsigned iP(0); iP<nHists; ++iP){//loop on points
	//get histograms
	for (unsigned iW(0); iW<nWFiles; ++iW){//loop on w files
	  fW[iW] = TFile::Open((lPath.str()+Wfiles[iW]+".root").c_str());
	  //fW[iW]->cd((lSelection[iS]+"/weights/").c_str());
	  //if (iP < 2) 
	  fW[iW]->cd((selections[iS]+"/").c_str());
	  //else fW[iW]->cd((selections[iS]+"/weights/").c_str());
	  if (iW==0) {
	    hist[iP][0] = (TH1F*)gDirectory->Get(lHistName[iP].c_str())->Clone();
	    hist[iP][0]->Scale(lLumi*w_normalisation[iW]);
	  }
	  if (iW==6) {
	    hist_ewk[iP] = (TH1F*)gDirectory->Get(lHistName[iP].c_str())->Clone();
	    hist_ewk[iP]->Scale(lLumi*w_normalisation[iW]);
	  }
	  if (iW>0) {
	    TH1 *lTmpHist = 0;
	    lTmpHist = (TH1F*)gDirectory->Get(lHistName[iP].c_str());
	    lTmpHist->Scale(lLumi*w_normalisation[iW]);
	    hist[iP][0]->Add(lTmpHist);
	    if (iW==7) {
	      hist_ewk[iP]->Add(lTmpHist);
	    }
	  }
	}//loop on w hists
      
	fData[0] = TFile::Open(("../"+folder[iFS]+"/DataEmbedded_METembedded-9965pb.root").c_str());
	fData[0]->cd((selections[iS]+"/").c_str());
	hist[iP][1] = (TH1F*)gDirectory->Get(lHistName[iP].c_str())->Clone();
	hist[iP][1]->Scale(lLumi/lLumiEmbedded);
	fData[1] = TFile::Open(("../"+folder[iFS]+"/Data_MET-2012-all.root").c_str());
	fData[1]->cd((selections[iS]+"/").c_str());
	hist[iP][2] = (TH1F*)gDirectory->Get(lHistName[iP].c_str())->Clone();
	hist[iP][2]->Scale(1.);


	//std::cout << " -- total entries = " << hist[iP]->Integral()
	//<< std::endl;

	error_ewk[iP] = 0;
	integral_ewk[iP] = hist_ewk[iP]->IntegralAndError(0,hist_ewk[iP]->GetNbinsX(),error_ewk[iP]);

	myc[iP]->cd();

	for (unsigned iD(0); iD<nSamples; ++iD){//loop on samples
	  error[iP][iD] = 0;
	  integral[iP][iD] = hist[iP][iD]->IntegralAndError(0,hist[iP][iD]->GetNbinsX()+1,error[iP][iD]);
	  std::cout << selections[iS] << " " << sampleStr[iD] << " " << lHistName[iP] << " : " << integral[iP][iD] << " +/- " << error[iP][iD] << std::endl; 
	  
	  
	  hist[iP][iD]->Rebin(rebin[iP]);
	  hist[iP][iD]->GetXaxis()->SetTitle(xTitle[iP].c_str());
	  hist[iP][iD]->GetYaxis()->SetTitle(yTitle[iP].c_str());
	  hist[iP][iD]->SetTitle(selections[iS].c_str());
	  hist[iP][iD]->SetLineColor(iD+1);
	  hist[iP][iD]->SetMarkerColor(iD+1);
	  hist[iP][iD]->SetMarkerStyle(21+iD);
	  hist_ewk[iP]->SetLineColor(6);
	  hist_ewk[iP]->SetFillColor(6);
	  hist_ewk[iP]->SetMarkerColor(6);
	  hist_ewk[iP]->SetMarkerStyle(28);
	  gPad->SetLogy(logy[iP]);
	  hist[iP][iD]->GetXaxis()->SetRangeUser(minX[iP],maxX[iP]);
	  if (iD==0) hist[iP][iD]->Draw("PE");
	  else hist[iP][iD]->Draw("PEsame");
	  //hist_ewk[iP]->Draw("same");
	  
	}//loop on samples

	leg = new TLegend(0.3,0.7,0.6,0.89);
	leg->SetFillColor(10);
	leg->AddEntry(hist[iP][0],"MC W#rightarrow #tau#nu QCD+EWK","P");
	//leg->AddEntry(hist_ewk[iP],"W#rightarrow #tau#nu EWK","F");
	leg->AddEntry(hist[iP][1],"Data embedded","P");
	leg->AddEntry(hist[iP][2],"Data","P");
	
	leg->Draw("same");
	myc[iP]->Update();
	myc[iP]->Print(("PLOTS/WtauEmbedding_"+selections[iS]+"_"+histStr[iP]+".pdf").c_str());
  
      }//loop on points
      
      //gStyle->SetStatX(0.89);
      //gStyle->SetStatY(0.89);
      
    }//loop on selections
    
    lOutfile.close();
    
  }//loop on final state
  
  return 0;
      
}//main
