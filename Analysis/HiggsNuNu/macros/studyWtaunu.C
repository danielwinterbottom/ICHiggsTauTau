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

  std::string folder = "output/nunu/Skim/MET130/";
  //std::string folder = "output/nunu/Skim/MET130/tmp/";
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
  

  TCanvas *myc = new TCanvas("myc","",4);
  gStyle->SetOptStat("euoi");
  myc->Divide(2,3);

  std::ostringstream lPath;

  const unsigned nHists2D = 1;
  const unsigned nHists1D = 5;
  const unsigned nHists = nHists1D+nHists2D;

  //std::string lHistName[nHists] = {"Gen/taupt","Gen/taueta","Gen/tauptvseta"};
  std::string lHistName[nHists] = {"Gen/tauptvseta","Gen/taupt","Gen/taueta","n_jets","Gen/recotaupt","Gen/recotaueta"};

  double minX[nHists] = {-4,0,-4,0,0,-3};
  double maxX[nHists] = {4,250,4,10,250,3};
  int logy[nHists] = {0,1,0,1,1,1};
  int rebin[nHists] = {1,5,2,1,5,2};

  std::vector<std::string> selections;
  selections.push_back("HLTMetClean");
  selections.push_back("LeptonVeto");
  //selections.push_back("WSelection");
  selections.push_back("JetPair");
  //selections.push_back("AN");
  selections.push_back("DEta");
  selections.push_back("MET");
  selections.push_back("TightMjj");
  selections.push_back("DPhiSIGNAL_noCJV");
  //selections.push_back("DPhiQCD_noCJV");
  //selections.push_back("CJVfail");
  //selections.push_back("DPhiSIGNAL_CJVfail");
  //selections.push_back("DPhiQCD_CJVfail");
  selections.push_back("CJVpass");
  selections.push_back("DPhiSIGNAL_CJVpass");
  //selections.push_back("DPhiQCD_CJVpass");
  
  const unsigned nSel = selections.size();

  TH1 *hist[nHists];
  TH1 *hist_ewk[nHists];
  TLegend *leg;
  double error[nHists];
  double integral[nHists];
  double error_ewk[nHists];
  double integral_ewk[nHists];

  int bin_ptmin = 0;
  int bin_etamin = 0;
  int bin_etamax = 0;

  std::ofstream lOutfile;
  lOutfile.open("EventNumbers_taunu.txt");

  lOutfile << "Selection & n$_{\\tau}^{acc}$ & n$_{tot}$ & n$_{\\tau}^{acc}$/n$_{tot}$ & n$_{\\tau}^{reco}$ & n$_{\\tau}^{reco}$/n$_{tot}$ \\\\" << std::endl;

  for (unsigned iS(0); iS<nSel; ++iS){//loop on selections
    lPath.str("");
    lPath << "../" << folder << "/MC_";
    
    lOutfile << selections[iS] << " & $ " ;

    for (unsigned iP(0); iP<nHists; ++iP){//loop on points
      //get histograms
      for (unsigned iW(0); iW<nWFiles; ++iW){//loop on w files
	fW[iW] = TFile::Open((lPath.str()+Wfiles[iW]+".root").c_str());
	//fW[iW]->cd((lSelection[iS]+"/weights/").c_str());
	//if (iP < 2) 
	fW[iW]->cd((selections[iS]+"/").c_str());
	//else fW[iW]->cd((selections[iS]+"/weights/").c_str());
	if (iW==0 || iW==6) {
	  if (iP<nHists2D) {
	    if (iW==0) hist[iP] = (TH2F*)gDirectory->Get(lHistName[iP].c_str())->Clone();
	    if (iW==6) hist_ewk[iP] = (TH2F*)gDirectory->Get(lHistName[iP].c_str())->Clone();
	  } else {
	    if (iW==0) hist[iP] = (TH1F*)gDirectory->Get(lHistName[iP].c_str())->Clone();
	    if (iW==6) hist_ewk[iP] = (TH1F*)gDirectory->Get(lHistName[iP].c_str())->Clone();
	  }
	  //hist[iP]->Sumw2();
	  if (iW==0) hist[iP]->Scale(lLumi*w_normalisation[iW]);
	  if (iW==6) hist_ewk[iP]->Scale(lLumi*w_normalisation[iW]);
	}
	if (iW>0) {
	  TH1 *lTmpHist = 0;
	  if (iP<nHists2D) {
	    lTmpHist = (TH2F*)gDirectory->Get(lHistName[iP].c_str());
	  }
	  else {
	    lTmpHist = (TH1F*)gDirectory->Get(lHistName[iP].c_str());
	  }
	  lTmpHist->Scale(lLumi*w_normalisation[iW]);
	  hist[iP]->Add(lTmpHist);
	  if (iW==7) {
	    hist_ewk[iP]->Add(lTmpHist);
	  }
	}
      }//loop on w hists
      
      //std::cout << " -- total entries = " << hist[iP]->Integral()
      //<< std::endl;

      if (iP==0 && iS==0){
	for (int iX(0); iX<hist[iP]->GetNbinsX()+2; ++iX){
	  double eta = hist[iP]->GetXaxis()->GetBinLowEdge(iX);
	  if (eta <= -2.3) bin_etamin=iX;
	  else if (eta <= 2.3) {
	    bin_etamax=iX;
	  }
	  else break;
	}
	for (int iY(0); iY<hist[iP]->GetNbinsY()+2; ++iY){
	  double pT = hist[iP]->GetYaxis()->GetBinLowEdge(iY);
	  if (pT <= 20) {
	    bin_ptmin = iY;
	  }
	  else break;
	}
	std::cout << " -- Found bin edges : " 
		  << bin_etamin << " (" << hist[iP]->GetXaxis()->GetBinLowEdge(bin_etamin) << ") " << bin_etamax << "(" << hist[iP]->GetXaxis()->GetBinUpEdge(bin_etamax) << ") "
		  << bin_ptmin << " (" << hist[iP]->GetYaxis()->GetBinLowEdge(bin_ptmin) << ")"
		  << std::endl; 
      }

      error[iP] = 0;
      error_ewk[iP] = 0;
      if (iP>0) integral[iP] = hist[iP]->IntegralAndError(0,hist[iP]->GetNbinsX()+1,error[iP]);
      else integral[iP] = dynamic_cast<TH2F*>(hist[iP])->IntegralAndError(bin_etamin,bin_etamax,bin_ptmin,hist[iP]->GetNbinsY()+1,error[iP]);
      if (iP>0) integral_ewk[iP] = hist_ewk[iP]->IntegralAndError(0,hist_ewk[iP]->GetNbinsX(),error_ewk[iP]);
      else integral_ewk[iP] = dynamic_cast<TH2F*>(hist_ewk[iP])->IntegralAndError(bin_etamin,bin_etamax,bin_ptmin,hist[iP]->GetNbinsY()+1,error_ewk[iP]);
 
      std::cout << selections[iS] << " " << lHistName[iP] << " " << integral[iP] << " +/- " << error[iP] << std::endl;

      if (iP==0) lOutfile << std::setprecision(3)
			  << integral[iP] << " \\pm " << error[iP] ;
       //<< "$ & $ "
       //<< integral_ewk[iP] << " \\pm " << error_ewk[iP] ;

      if (iP==3){
	double ratio = integral[0]/integral[3];
	double ratioerr = ratio*sqrt(pow(error[0]/integral[0],2)+pow(error[3]/integral[3]-2*error[0]*error[3]/(integral[0]*integral[3]),2));
	lOutfile << " $ & $ "
		 << integral[iP] << " \\pm " << error[iP] 
		 << " $ & $ "
		 << ratio << " \\pm " << ratioerr ;
      }
      if (iP==4){
	double ratio = integral[4]/integral[3];
	double ratioerr = ratio*sqrt(pow(error[4]/integral[4],2)+pow(error[3]/integral[3]-2*error[4]*error[3]/(integral[4]*integral[3]),2));
	lOutfile << " $ & $ "
		 << integral[iP] << " \\pm " << error[iP] 
		 << " $ & $ "
		 << ratio << " \\pm " << ratioerr
		 << " $ \\\\" 
		 << std::endl;
      }

      myc->cd(iP+1);
      //hist[iP]->RebinY(10);
      if (iP==0) {
	hist[iP]->GetXaxis()->SetTitle("#eta (#tau genjet)");
	hist[iP]->GetYaxis()->SetTitle("p_{T}(#tau genjet) (GeV)");
      }
      else if (iP==1) {
	hist[iP]->GetXaxis()->SetTitle("p_{T}(#tau genjet) (GeV)");
      }
      else if (iP==2) {
	hist[iP]->GetXaxis()->SetTitle("#eta (#tau genjet)");
      }
      else if (iP==3) {
	hist[iP]->GetXaxis()->SetTitle("Jet multiplicity");
      }
      else if (iP==4) {
     	hist[iP]->GetXaxis()->SetTitle("p_{T}(#tau recojet) (GeV)");
      }
      else if (iP==5) {
	hist[iP]->GetXaxis()->SetTitle("#eta (#tau recojet)");
      }

      hist[iP]->SetTitle(("MC W#rightarrow #tau#nu, "+selections[iS]).c_str());
      hist[iP]->SetLineColor(1);
      hist[iP]->SetMarkerColor(1);
      hist[iP]->SetMarkerStyle(20);
      hist_ewk[iP]->SetLineColor(6);
      hist_ewk[iP]->SetMarkerColor(6);
      hist_ewk[iP]->SetMarkerStyle(22);
      gPad->SetLogy(logy[iP]);
      if (iP<nHists2D) {
	dynamic_cast<TH2F*>(hist[iP])->RebinX(2);
	dynamic_cast<TH2F*>(hist[iP])->RebinY(5);
	hist[iP]->GetYaxis()->SetRangeUser(0,250);
	hist[iP]->Draw("colz");
	//hist_ewk[iP]->Draw("same");
      }
      else {
	hist[iP]->Rebin(rebin[iP]);
	hist[iP]->GetXaxis()->SetRangeUser(minX[iP],maxX[iP]);
	hist[iP]->Draw("PE");
	hist_ewk[iP]->Draw("PEsame");

	if (iP==1) {
	  leg = new TLegend(0.3,0.7,0.6,0.89);
	  leg->SetFillColor(10);
	  leg->AddEntry(hist[iP],"W#rightarrow #tau#nu QCD+EWK","P");
	  leg->AddEntry(hist_ewk[iP],"W#rightarrow #tau#nu EWK","P");
	}
	leg->Draw("same");

      }
      //if (iP==0) 
      
      gStyle->SetStatX(0.89);
      gStyle->SetStatY(0.89);
      
      //else hist[iP]->Draw("histsame");
    }//loop on histos
    myc->Update();
    myc->Print(("Wtau_"+selections[iS]+".pdf").c_str());
    
  }//loop on selections
  

  lOutfile.close();

  return 0;
    
    
}//main
