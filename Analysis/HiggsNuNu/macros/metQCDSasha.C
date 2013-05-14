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
#include "TGraphErrors.h" 
#include "TStyle.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "THStack.h"

int metQCDSasha(){//main

  //TFile * fData;

  std::vector<std::string> files;
  files.push_back("MC_TTJets");
  files.push_back("MC_T-tW");
  files.push_back("MC_Tbar-tW");
  files.push_back("MC_SingleT-s-powheg-tauola");
  files.push_back("MC_SingleTBar-s-powheg-tauola");
  files.push_back("MC_SingleT-t-powheg-tauola");
  files.push_back("MC_SingleTBar-t-powheg-tauola");
  files.push_back("MC_WW-pythia6-tauola");
  files.push_back("MC_WZ-pythia6-tauola");
  files.push_back("MC_ZZ-pythia6-tauola");
  files.push_back("MC_DYJJ01JetsToLL_M-50_MJJ-200");
  files.push_back("MC_DYJetsToLL");
  files.push_back("MC_DY1JetsToLL");
  files.push_back("MC_DY2JetsToLL");
  files.push_back("MC_DY3JetsToLL");
  files.push_back("MC_DY4JetsToLL");
  files.push_back("MC_ZJetsToNuNu_100_HT_200");
  files.push_back("MC_ZJetsToNuNu_200_HT_400");
  files.push_back("MC_ZJetsToNuNu_400_HT_inf");
  files.push_back("MC_ZJetsToNuNu_50_HT_100");
  files.push_back("MC_WJetsToLNu-v1_enu");
  files.push_back("MC_WJetsToLNu-v2_enu");
  files.push_back("MC_W1JetsToLNu_enu");
  files.push_back("MC_W2JetsToLNu_enu");
  files.push_back("MC_W3JetsToLNu_enu");
  files.push_back("MC_W4JetsToLNu_enu");
  files.push_back("MC_WJetsToLNu-v1_munu");
  files.push_back("MC_WJetsToLNu-v2_munu");
  files.push_back("MC_W1JetsToLNu_munu");
  files.push_back("MC_W2JetsToLNu_munu");
  files.push_back("MC_W3JetsToLNu_munu");
  files.push_back("MC_W4JetsToLNu_munu");
  files.push_back("MC_WJetsToLNu-v1_taunu");
  files.push_back("MC_WJetsToLNu-v2_taunu");
  files.push_back("MC_W1JetsToLNu_taunu");
  files.push_back("MC_W2JetsToLNu_taunu");
  files.push_back("MC_W3JetsToLNu_taunu");
  files.push_back("MC_W4JetsToLNu_taunu");

  std::string folder = "output";
  std::string suffix = "_type01_v10sel_notrigWeight_bis";

  const unsigned nFiles = files.size();

  TFile *fBkg[nFiles];
  TFile *output = new TFile(("BkgForQCDEstimation"+suffix+".root").c_str(),"RECREATE");

  double lLumi = 19619.;
  
  int lColor[nFiles];
  for (unsigned iN(0); iN<7;++iN){
    lColor[iN] = 5;
  }
  lColor[7] = 4;
  lColor[8] = 4;
  lColor[9] = 4;
  for (unsigned iN(10); iN<20;++iN){
    lColor[iN] = 3;
  }

  int lStyle = 1001;
  double normalisation[nFiles];
  normalisation[0] = 234./6923750.0;
  normalisation[1] = 11.1/497658.0;
  normalisation[2] = 11.1/493460.0;
  normalisation[3] = 3.79/259961.0;
  normalisation[4] = 1.76/139974.0;
  normalisation[5] = 56.4/99876.0;
  normalisation[6] = 30.7/1935072.0;
  normalisation[7] = 54.838/9970431.0;
  normalisation[8] = 33.21/10000283.0;
  normalisation[9] = 17.654/9799908.0;
  normalisation[10] = (0.70/0.101*0.2)/510501.0;
  normalisation[11] = 3503.7/30459503.0;
  normalisation[12] = 3503.7/30459503.0;
  normalisation[13] = 3503.7/30459503.0;
  normalisation[14] = 3503.7/30459503.0;
  normalisation[15] = 3503.7/30459503.0;
  normalisation[16] = 381.2/4040980.0;
  normalisation[17] = 160.3/4416646.0;
  normalisation[18] = 41.49/5055885.0;
  normalisation[19] = 5.274/1006928.0;
  for (unsigned iN(20); iN<nFiles;++iN){
    normalisation[iN] = 37509/76102995.0;
    lColor[iN] = 6;
  }

  TCanvas *myc = new TCanvas("myc","",1);
  gStyle->SetOptStat(0);


  TH1F *met[nFiles][2];
  TH1F *metRange[nFiles][2];
  TH1F *metRangeTotal[2];

  THStack thstack0("stack0","stack0");
  THStack thstack1("stack1","stack1");

  unsigned n_added_to_stack[2] = {0,0};

  std::ostringstream lPath,lHistName;

  lHistName.str("");
  //lHistName << "met_pu_trig";
  lHistName << "met";

  //lPath.str("");
  //lPath << "../output/nunu/MET0/Data.root";
  //fData = TFile::Open(lPath.str().c_str());

  std::string lSelection[2] = {"DPhiSIGNAL","DPhiQCD"};

  //define the binning
  //const unsigned nBins = 10;
  //double xmin[nBins+1] = {0,10,20,30,40,50,60,70,80,100,120};//,met[0]->GetXaxis()->GetBinUpEdge(met[0]->GetNbinsX())};
  const unsigned nBins = 60;
  double xmin[nBins+1];

  for (unsigned iS(0); iS<2; ++iS){//loop on selection

    //fData->cd(lSelection[iS].c_str());
    //met[0][iS] = (TH1F*)gDirectory->Get(lHistName.str().c_str()); 
    //met[0][iS]->Sumw2();

    //get histograms
    for (unsigned iBkg(0); iBkg<nFiles; ++iBkg){//loop on bkg files
      lPath.str("");
      lPath << "../" << folder << "/nunu/MET0/" << files[iBkg] << ".root";
      fBkg[iBkg] = TFile::Open(lPath.str().c_str());
      //fBkg[iBkg]->cd((lSelection[iS]+"/weights/").c_str());
      fBkg[iBkg]->cd((lSelection[iS]+"/").c_str());
      met[iBkg][iS] = (TH1F*)gDirectory->Get(lHistName.str().c_str());
      met[iBkg][iS]->Sumw2();
      met[iBkg][iS]->Scale(lLumi*normalisation[iBkg]);

      std::ostringstream lName;
      lName.str("");
      lName << "metRange_" << iBkg ;
      //metRange[iBkg][iS] = new TH1F(lName.str().c_str(),";MET range (GeV);Sum bkg (events)",nBins,xmin);
      metRange[iBkg][iS] = new TH1F(lName.str().c_str(),";MET (GeV);Sum bkg (events)",nBins,0,300);
      metRange[iBkg][iS]->SetLineColor(lColor[iBkg]);
      metRange[iBkg][iS]->SetMarkerColor(lColor[iBkg]);
      metRange[iBkg][iS]->SetFillStyle(lStyle);
    }//loop on bkg files
    //metRangeTotal[iS] = new TH1F(("metRangeTotal_"+lSelection[iS]).c_str(),";MET range (GeV);Sum bkg (events)",nBins,xmin);
    metRangeTotal[iS] = new TH1F(("metRangeTotal_"+lSelection[iS]).c_str(),";MET (GeV);Sum bkg (events)",nBins,0,300);

    unsigned binMin = 0;
    unsigned binMax = met[0][iS]->GetNbinsX();
  
    for (unsigned iB(0); iB<nBins+1; iB++){
      xmin[iB] = 5*iB;
    }
    for (unsigned iB(0); iB<nBins; iB++){
      for (int iX(0); iX<met[0][iS]->GetNbinsX(); ++iX){
	if (met[0][iS]->GetBinLowEdge(iX+1) >= xmin[iB]){
	  binMin = iX+1;
	  break;
	}
      }
      
      for (int iX(binMin); iX<met[0][iS]->GetNbinsX(); ++iX){
	if (met[0][iS]->GetBinLowEdge(iX+1) >= xmin[iB+1]) {
	  binMax = iX;
	  break;
	}
      }
      //if (iB==nBins-1) binMax = met[0][iS]->GetNbinsX();
      
      std::cout << " Bin " << iB 
		<< " min = " <<  met[0][iS]->GetBinLowEdge(binMin)
		<< " max = " <<  met[0][iS]->GetXaxis()->GetBinUpEdge(binMax)
		<< std::endl;
      

      double nBkg = 0;
      double nBkgErr = 0;
      for (unsigned iBkg(0); iBkg<nFiles; ++iBkg){
	double lBkgErr = 0;
	double lBkg = met[iBkg][iS]->IntegralAndError(binMin,binMax,lBkgErr);
	if (lBkg != lBkg) lBkg=0;
	if (lBkgErr != lBkgErr) lBkgErr=0;

	nBkg += lBkg;
	nBkgErr += lBkgErr*lBkgErr;
	//fill new histos
	metRange[iBkg][iS]->SetBinContent(iB+1,lBkg);
	metRange[iBkg][iS]->SetBinError(iB+1,lBkgErr);
	std::cout << " - " << files[iBkg] << " = " <<  lBkg << " +/- " << lBkgErr << std::endl;
      }

      std::cout << " ---- Total bkg = " << nBkg << " +/- " << sqrt(nBkgErr) << std::endl;
      metRangeTotal[iS]->SetBinContent(iB+1,nBkg);
      metRangeTotal[iS]->SetBinError(iB+1,sqrt(nBkgErr));
    }

    //fill new histos
    for (unsigned iBkg(0); iBkg<nFiles; ++iBkg){//loop on bkg files
      if (iS==0){
	thstack0.Add(metRange[iBkg][iS],"Esame");
	if (n_added_to_stack[iS] == 0) thstack0.SetHistogram((TH1F*)metRange[iBkg][iS]->Clone());
      } else {
	thstack1.Add(metRange[iBkg][iS],"Esame");
	if (n_added_to_stack[iS] == 0) thstack1.SetHistogram((TH1F*)metRange[iBkg][iS]->Clone());
      }
      ++n_added_to_stack[iS];
    }//loop on bkg files


    //TGraphErrors *grSumBkg = new TGraphErrors(nBins,xaxis,yaxis,xaxisErr,yaxisErr);
    myc->cd();
    //grSumBkg->GetXaxis()->SetTitle("MET range (GeV)");
    //grSumBkg->GetYaxis()->SetTitle("Sum bkg (events)");
    //if (iS==0) grSumBkg->SetTitle("#Delta#phi_{jj} < 1.0");
    //else grSumBkg->SetTitle("#Delta#phi_{jj} > 2.6");
    //grSumBkg->SetMinimum(0);
    //grSumBkg->SetMaximum(maxVal*1.1);

    if (iS==0) {
      //thstack0.
      metRangeTotal[iS]->SetTitle("#Delta#phi_{jj} < 1.0");
    }
    else {
      metRangeTotal[iS]->SetTitle("#Delta#phi_{jj} > 2.6");
    }
    metRangeTotal[iS]->Draw("PE");

      //thstack1.SetTitle("#Delta#phi_{jj} > 2.6");
      //thstack1.Draw();

    myc->Update();
    myc->Print(("bkgQCDSasha"+suffix+"_"+lSelection[iS]+".pdf").c_str());
  
    output->Add(metRangeTotal[iS]);

  }//loop on selection

  output->Write();
  output->Close();

  return 0;

}//main
