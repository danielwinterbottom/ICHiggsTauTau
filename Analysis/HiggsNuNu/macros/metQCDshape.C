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
#include "TLegend.h"

int metQCDshape(){//main

  //TFile * fData;

  std::vector<std::string> files;
  files.push_back("MC_QCD-Pt-30to50-pythia6");
  files.push_back("MC_QCD-Pt-50to80-pythia6");
  files.push_back("MC_QCD-Pt-80to120-pythia6");
  files.push_back("MC_QCD-Pt-120to170-pythia6");
  files.push_back("MC_QCD-Pt-170to300-pythia6");
  files.push_back("MC_QCD-Pt-300to470-pythia6");
  files.push_back("MC_QCD-Pt-470to600-pythia6");
  files.push_back("MC_QCD-Pt-600to800-pythia6");
  files.push_back("MC_QCD-Pt-800to1000-pythia6");
  files.push_back("MC_QCD-Pt-1000to1400-pythia6");
  files.push_back("MC_QCD-Pt-1400to1800-pythia6");
  files.push_back("MC_QCD-Pt-1800-pythia6");


  std::string folder = "output";
  std::string suffix = "";

  const unsigned nFiles = files.size();

  TFile *fBkg[nFiles];
  //TFile *output = new TFile(("QCDShapeFromMC"+suffix+".root").c_str(),"RECREATE");

  double lLumi = 19619.;
  
  int lColor[nFiles];
  for (unsigned iN(0); iN<nFiles;++iN){
    lColor[iN] = 9;
  }

  int lStyle = 1001;
  double normalisation[nFiles];
  normalisation[0] = 66285328./5910000.0;
  normalisation[1] = 8148778.0/5998860.0;
  normalisation[2] = 1033680.0/5994864.0;
  normalisation[3] = 156293.3/5955732.0;
  normalisation[4] = 34138.15/5814398.0;
  normalisation[5] = 1759.549/5978500.0;
  normalisation[6] = 113.8791/3964848.0;
  normalisation[7] = 26.9921/3996864.0;
  normalisation[8] = 3.550036/3998563.0;
  normalisation[9] = 0.737844/1904088.0;
  normalisation[10] = 0.03352235/1910062.0;
  normalisation[11] = 0.001829005/947586.0;

  TCanvas *myc[2];
  myc[0] = new TCanvas("myc0","",1);
  myc[1] = new TCanvas("myc1","",1);
  gStyle->SetOptStat(0);


  TLegend *legend = new TLegend(0.5,0.7,0.9,0.9);

  TH1F *met[nFiles][2][2];
  TH1F *metRange[nFiles][2][2];
  TH1F *metRangeTotal[2][2];

  std::ostringstream lPath,lHistName[2];

  lHistName[0].str("");
  lHistName[1].str("");
  lHistName[0] << "met_pu";
  lHistName[1] << "met_pu_trig";

  //lPath.str("");
  //lPath << "../output/nunu/MET0/Data.root";
  //fData = TFile::Open(lPath.str().c_str());

  std::string lSelection[2] = {"DPhiSIGNAL","DPhiQCD"};
  std::string lType[2] = {"noTrigWeight","trigWeight"};
  //define the binning
  //const unsigned nBins = 10;
  //double xmin[nBins+1] = {0,10,20,30,40,50,60,70,80,100,120};//,met[0]->GetXaxis()->GetBinUpEdge(met[0]->GetNbinsX())};
  const unsigned nBins = 60;
  double xmin[nBins+1];

  for (unsigned iT(0); iT<2; ++iT){//loop on type: with or without trigger weights
    myc[iT]->cd();
    for (unsigned iS(0); iS<2; ++iS){//loop on selection

      //get histograms
      for (unsigned iBkg(0); iBkg<nFiles; ++iBkg){//loop on bkg files
	lPath.str("");
	lPath << "../" << folder << "/nunu/MET0/" << files[iBkg] << ".root";
	fBkg[iBkg] = TFile::Open(lPath.str().c_str());
	fBkg[iBkg]->cd((lSelection[iS]+"/weights/").c_str());
	//fBkg[iBkg]->cd((lSelection[iS]+"/").c_str());
	met[iBkg][iS][iT] = (TH1F*)gDirectory->Get(lHistName[iT].str().c_str());
	met[iBkg][iS][iT]->Sumw2();
	met[iBkg][iS][iT]->Scale(lLumi*normalisation[iBkg]);

	std::ostringstream lName;
	lName.str("");
	lName << "metRange_" << iBkg << "" << iS << " " << iT;
	//metRange[iBkg][iS][iT] = new TH1F(lName.str().c_str(),";MET range (GeV);Sum bkg (events)",nBins,xmin);
	metRange[iBkg][iS][iT] = new TH1F(lName.str().c_str(),";MET (GeV);Sum bkg (events)",nBins,0,300);
	metRange[iBkg][iS][iT]->SetLineColor(iS+1);
	metRange[iBkg][iS][iT]->SetMarkerColor(iS+1);
	metRange[iBkg][iS][iT]->SetMarkerStyle(20+iS);
	metRange[iBkg][iS][iT]->SetFillStyle(lStyle);
      }//loop on bkg files
      //metRangeTotal[iS][iT] = new TH1F(("metRangeTotal_"+lSelection[iS][iT]).c_str(),";MET range (GeV);Sum bkg (events)",nBins,xmin);

      metRangeTotal[iS][iT] = new TH1F(("metRangeTotal_"+lSelection[iS]+"_"+lType[iT]).c_str(),";MET (GeV);Arb. unit",nBins,0,300);
      metRangeTotal[iS][iT]->SetLineColor(iS+1);
      metRangeTotal[iS][iT]->SetMarkerColor(iS+1);
      metRangeTotal[iS][iT]->SetMarkerStyle(20+iS);
      metRangeTotal[iS][iT]->SetFillStyle(lStyle);


      unsigned binMin = 0;
      unsigned binMax = met[0][iS][iT]->GetNbinsX();
  
      for (unsigned iB(0); iB<nBins+1; iB++){
	xmin[iB] = 5*iB;
      }
      for (unsigned iB(0); iB<nBins; iB++){
	for (int iX(0); iX<met[0][iS][iT]->GetNbinsX(); ++iX){
	  if (met[0][iS][iT]->GetBinLowEdge(iX+1) >= xmin[iB]){
	    binMin = iX+1;
	    break;
	  }
	}
	
	for (int iX(binMin); iX<met[0][iS][iT]->GetNbinsX(); ++iX){
	  if (met[0][iS][iT]->GetBinLowEdge(iX+1) >= xmin[iB+1]) {
	    binMax = iX;
	    break;
	  }
	}
	//if (iB==nBins-1) binMax = met[0][iS][iT]->GetNbinsX();
	
	std::cout << " Bin " << iB 
		  << " min = " <<  met[0][iS][iT]->GetBinLowEdge(binMin)
		  << " max = " <<  met[0][iS][iT]->GetXaxis()->GetBinUpEdge(binMax)
		  << std::endl;
      
	
	double nBkg = 0;
	double nBkgErr = 0;
	for (unsigned iBkg(0); iBkg<nFiles; ++iBkg){
	  double lBkgErr = 0;
	  double lBkg = met[iBkg][iS][iT]->IntegralAndError(binMin,binMax,lBkgErr);
	  if (lBkg != lBkg) lBkg=0;
	  if (lBkgErr != lBkgErr) lBkgErr=0;

	  nBkg += lBkg;
	  nBkgErr += lBkgErr*lBkgErr;
	  //fill new histos
	  metRange[iBkg][iS][iT]->SetBinContent(iB+1,lBkg);
	  metRange[iBkg][iS][iT]->SetBinError(iB+1,lBkgErr);
	  //std::cout << " - " << files[iBkg] << " = " <<  lBkg << " +/- " << lBkgErr << std::endl;
	}

	std::cout << " ---- Total bkg = " << nBkg << " +/- " << sqrt(nBkgErr) << std::endl;
	metRangeTotal[iS][iT]->SetBinContent(iB+1,nBkg);
	metRangeTotal[iS][iT]->SetBinError(iB+1,sqrt(nBkgErr));
      }

      metRangeTotal[iS][iT]->SetTitle(lType[iT].c_str());

      if (iT==0){
	if (iS==0) {
	  legend->AddEntry(metRangeTotal[iS][iT],"#Delta#phi_{jj} < 1.0","L");
	}
	else {
	  legend->AddEntry(metRangeTotal[iS][iT],"#Delta#phi_{jj} > 2.6","L");
	}
      }
      if (iS==0) metRangeTotal[iS][iT]->DrawNormalized("PE");
      else metRangeTotal[iS][iT]->DrawNormalized("PEsame");
      //thstack1.SetTitle("#Delta#phi_{jj} > 2.6");
      //thstack1.Draw();

      //output->Add(metRangeTotal[iS][iT]);

    }//loop on selection
      
    legend->Draw("same");
    myc[iT]->Update();
    myc[iT]->Print(("QCDshape"+suffix+"_"+lType[iT]+".pdf").c_str());
    
      
  }//loop on types

  //output->Write();
  //output->Close();

  return 0;

}//main
