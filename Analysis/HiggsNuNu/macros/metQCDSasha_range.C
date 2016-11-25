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
#include "TGraphAsymmErrors.h" 
#include "TStyle.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TFitResult.h"
#include "TLegend.h"
#include "TF1.h"

int metQCDSasha_range(){//main

  //TFile * fData;

  std::vector<std::string> files;
  files.push_back("MC_TTJets");//0
  files.push_back("MC_T-tW");//1
  files.push_back("MC_Tbar-tW");
  files.push_back("MC_SingleT-s-powheg-tauola");
  files.push_back("MC_SingleTBar-s-powheg-tauola");
  files.push_back("MC_SingleT-t-powheg-tauola");
  files.push_back("MC_SingleTBar-t-powheg-tauola");
  files.push_back("MC_WW-pythia6-tauola");//7
  files.push_back("MC_WZ-pythia6-tauola");
  files.push_back("MC_ZZ-pythia6-tauola");
  files.push_back("MC_DYJJ01JetsToLL_M-50_MJJ-200");//10
  files.push_back("MC_DYJetsToLL");//11
  files.push_back("MC_DY1JetsToLL");
  files.push_back("MC_DY2JetsToLL");
  files.push_back("MC_DY3JetsToLL");
  files.push_back("MC_DY4JetsToLL");
  files.push_back("MC_ZJetsToNuNu_100_HT_200");//16
  files.push_back("MC_ZJetsToNuNu_200_HT_400");
  files.push_back("MC_ZJetsToNuNu_400_HT_inf");
  files.push_back("MC_ZJetsToNuNu_50_HT_100");
  files.push_back("MC_WJetsToLNu-v1_enu");//20
  files.push_back("MC_WJetsToLNu-v2_enu");
  files.push_back("MC_W1JetsToLNu_enu");
  files.push_back("MC_W2JetsToLNu_enu");
  files.push_back("MC_W3JetsToLNu_enu");
  files.push_back("MC_W4JetsToLNu_enu");
  files.push_back("MC_WJetsToLNu-v1_munu");//26
  files.push_back("MC_WJetsToLNu-v2_munu");
  files.push_back("MC_W1JetsToLNu_munu");
  files.push_back("MC_W2JetsToLNu_munu");
  files.push_back("MC_W3JetsToLNu_munu");
  files.push_back("MC_W4JetsToLNu_munu");
  files.push_back("MC_WJetsToLNu-v1_taunu");//32
  files.push_back("MC_WJetsToLNu-v2_taunu");
  files.push_back("MC_W1JetsToLNu_taunu");
  files.push_back("MC_W2JetsToLNu_taunu");
  files.push_back("MC_W3JetsToLNu_taunu");
  files.push_back("MC_W4JetsToLNu_taunu");

  const unsigned nSamples = 9;
  unsigned indices[nSamples+1] = {0,1,7,10,11,16,20,26,32,38};
  std::string samples[nSamples] = {"TTJets","single top","VV","EWK Z+2j","DYJets","Znunu","Wenu","Wmunu","Wtaunu"};

  std::vector<std::string> datafiles;
  datafiles.push_back("Data_MET-2012A-13Jul2012-v1");
  datafiles.push_back("Data_MET-2012A-06Aug2012-v1");
  datafiles.push_back("Data_MET-2012B-13Jul2012-v1");
  datafiles.push_back("Data_MET-2012C-24Aug2012-v1");
  datafiles.push_back("Data_MET-2012C-11Dec2012-v1"); 
  datafiles.push_back("Data_MET-2012C-PromptReco-v2");
  datafiles.push_back("Data_MET-2012D-PromptReco-v1");

  std::string folder = "output";
  std::string suffix = "_new_cjvnocjv";

  const unsigned nFiles = files.size();
  const unsigned nDataFiles = datafiles.size();

  TFile *fBkg[nFiles];
  TFile *fData[nDataFiles];

  TFile *output = new TFile(("BkgForQCDEstimation"+suffix+".root").c_str(),"RECREATE");

  double lLumi = 19619.;
  
  int lColor[nSamples];
  for (unsigned iN(0); iN<nSamples;++iN){
    lColor[iN] = iN+1;
  }

  int lStyle = 1001;
  double normalisation[nFiles];
  normalisation[0] = 234./6923750.0;
  normalisation[1] = 11.1/497658.0;
  normalisation[2] = 11.1/493460.0;
  normalisation[3] = 3.79/259961.0;
  normalisation[4] = 1.76/139974.0;
  normalisation[5] = 56.4/3758227.0;
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
  }

  TCanvas *myc = new TCanvas("myc","",1);
  gStyle->SetOptStat(0);
  
  
  const unsigned nSel = 4;

  TH1F *met[nFiles][nSel];
  TH1F *metRange[nSamples][nSel];
  TH1F *metRangeTotal[nSel];
  TH1F *datamet[nDataFiles][nSel];
  TH1F *datametRange[nDataFiles][nSel];
  TH1F *datametRangeTotal[nSel];
  TH1F *qcdmetRangeTotal[nSel];
  TH1F *qcdmetIntegratedTotal[nSel];


  THStack thstack0("stack0","stack0");
  THStack thstack1("stack1","stack1");
  THStack thstack2("stack2","stack2");
  THStack thstack3("stack3","stack3");

  unsigned n_added_to_stack[nSel] = {0,0,0,0};

  std::ostringstream lPath,lHistName;

  lHistName.str("");
  lHistName << "met";

  //lPath.str("");
  //lPath << "../output/nunu/MET0/Data.root";
  //fData = TFile::Open(lPath.str().c_str());
  
  std::string lSelection[nSel] = {"DPhiSIGNAL-noCJV","DPhiQCD-noCJV","DPhiSIGNAL","DPhiQCD"};
  //std::string lSelection[2] = {"DPhiSIGNAL-noCJV","DPhiQCD-noCJV"};

  //define the binning
  const unsigned nBins = 12;
  const unsigned nBinsInt = nBins-2;
  double xmin[nBins+1] = {0,10,20,30,40,50,60,70,80,90,100,110,120};//,met[0]->GetXaxis()->GetBinUpEdge(met[0]->GetNbinsX())};


  double xaxis[nBins] = {5,15,25,35,45,55,65,75,85,95,105,115};
  double yaxis[2][nBins];
  double yaxisInt[2][nBinsInt];
  double xaxisErr[nBins] = {5,5,5,5,5,5,5,5,5,5,5,5};
  double yaxisErr[2][nBins];
  double yaxisIntErr[2][nBinsInt];

  for (unsigned iS(0); iS<nSel; ++iS){//loop on selection

    //fData->cd(lSelection[iS].c_str());
    //met[0][iS] = (TH1F*)gDirectory->Get(lHistName.str().c_str()); 
    //met[0][iS]->Sumw2();

    //get histograms
    for (unsigned iBkg(0); iBkg<nFiles; ++iBkg){//loop on bkg files
      lPath.str("");
      lPath << "../" << folder << "/nunu/MET0/" << files[iBkg] << ".root";
      fBkg[iBkg] = TFile::Open(lPath.str().c_str());
      fBkg[iBkg]->cd(lSelection[iS].c_str());
      met[iBkg][iS] = (TH1F*)gDirectory->Get(lHistName.str().c_str());
      //met[iBkg][iS]->Sumw2();
      met[iBkg][iS]->Scale(lLumi*normalisation[iBkg]);

    }//loop on bkg files
    metRangeTotal[iS] = new TH1F(("metRangeTotal_"+lSelection[iS]).c_str(),";MET range (GeV);Sum bkg (events)",nBins,xmin);

    for (unsigned idx(0); idx<nSamples; ++idx){//loop on bkg files
      std::ostringstream lName;
      lName.str("");
      lName << "metRange_" << iS << "_" << idx;
      metRange[idx][iS] = new TH1F(lName.str().c_str(),(";MET range (GeV);"+samples[idx]+" (events/10GeV)").c_str(),nBins,xmin);
      //metRange[idx][iS]->SetLineColor(lColor[idx]);
      //metRange[idx][iS]->SetMarkerColor(lColor[idx]);
      //metRange[idx][iS]->SetFillStyle(lStyle);
    }


    for (unsigned iD(0); iD<nDataFiles; ++iD){//loop on bkg files
      lPath.str("");
      lPath << "../" << folder << "/nunu/MET0/" << datafiles[iD] << ".root";
      fData[iD] = TFile::Open(lPath.str().c_str());
      fData[iD]->cd(lSelection[iS].c_str());
      datamet[iD][iS] = (TH1F*)gDirectory->Get(lHistName.str().c_str());
      //datamet[iD][iS]->Sumw2();
      
      std::ostringstream lName;
      lName.str("");
      lName << "datametRange_" << iD ;
      datametRange[iD][iS] = new TH1F(lName.str().c_str(),";MET range (GeV);Data (events/10GeV)",nBins,xmin);
      datametRange[iD][iS]->SetLineColor(lColor[iD]);
      datametRange[iD][iS]->SetMarkerColor(lColor[iD]);
      datametRange[iD][iS]->SetFillStyle(lStyle);
    }//loop on bkg files

    datametRangeTotal[iS] = new TH1F(("datametRangeTotal_"+lSelection[iS]).c_str(),";MET range (GeV);Data (events/10GeV)",nBins,xmin);
    qcdmetRangeTotal[iS] = new TH1F(("qcdmetRangeTotal_"+lSelection[iS]).c_str(),";MET range (GeV);QCD (events/10GeV)",nBins,xmin);
    qcdmetIntegratedTotal[iS] = new TH1F(("qcdmetIntegratedTotal_"+lSelection[iS]).c_str(),";minimum MET (GeV);QCD (events/10GeV)",nBinsInt,xmin);

    unsigned binMin = 0;
    unsigned binMax = met[0][iS]->GetNbinsX();
    unsigned binMaxInt = binMax+1;

    //find bin with MET=value at nBinsInt
    //for (int iX(0); iX<met[0][iS]->GetNbinsX(); ++iX){
    //if (met[0][iS]->GetBinLowEdge(iX+1) >= xmin[nBinsInt]){
    //binMaxInt = iX;
    //break;
    //}
    //}

    std::cout << " -- Max integral up to bin " 
	      << binMaxInt 
	      << ": "
	      << met[0][iS]->GetBinLowEdge(binMaxInt) 
	      << "," 
	      << met[0][iS]->GetXaxis()->GetBinUpEdge(binMaxInt)
	      << std::endl;
      
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
      unsigned idx=0;
      double lBkgErr = 0;
      double lBkg = 0;
      double nBkgInt = 0;
      double nBkgIntErr = 0;
      for (unsigned iBkg(0); iBkg<nFiles; ++iBkg){
	double lErr = 0;
	double lNum = met[iBkg][iS]->IntegralAndError(binMin,binMax,lErr);
	nBkg += lNum;
	nBkgErr += lErr*lErr;
	double lErrInt = 0;
	double lNumInt = 0;
	if (iB<nBinsInt) lNumInt = met[iBkg][iS]->IntegralAndError(binMin,binMaxInt,lErrInt);
	nBkgInt += lNumInt;
	nBkgIntErr += lErrInt*lErrInt;
	if (iBkg == indices[idx]){
	  lBkgErr = 0;
	  lBkg = 0;
	  lBkg += lNum;
	  lBkgErr += lErr*lErr;
	  idx++;
	}
	else if (iBkg > indices[idx-1] && iBkg < indices[idx]) {
	  lBkg += lNum;
	  lBkgErr += lErr*lErr;
	}
	//fill new histos
	if (iBkg == indices[idx]-1) {
	  metRange[idx-1][iS]->SetBinContent(iB+1,lBkg);
	  metRange[idx-1][iS]->SetBinError(iB+1,sqrt(lBkgErr));
	  std::cout << samples[idx-1] << " " <<  lBkg << " " << sqrt(lBkgErr) << std::endl;
	}
      }

      std::cout << " ---- Total bkg = " << nBkg << " +/- " << sqrt(nBkgErr) << std::endl;
      metRangeTotal[iS]->SetBinContent(iB+1,nBkg);
      metRangeTotal[iS]->SetBinError(iB+1,sqrt(nBkgErr));

      double nData = 0;
      double nDataErr = 0;
      double nDataInt = 0;
      double nDataIntErr = 0;
       for (unsigned iData(0); iData<nDataFiles; ++iData){
	double lDataErr = 0;
	double lData = datamet[iData][iS]->IntegralAndError(binMin,binMax,lDataErr);
	nData += lData;
	nDataErr += lDataErr*lDataErr;
	double lDataIntErr = 0;
	double lDataInt = 0;
	if (iB<nBinsInt) lDataInt = datamet[iData][iS]->IntegralAndError(binMin,binMaxInt,lDataIntErr);
	nDataInt += lDataInt;
	nDataIntErr += lDataIntErr*lDataIntErr;
	//fill new histos
	datametRange[iData][iS]->SetBinContent(iB+1,lData);
	datametRange[iData][iS]->SetBinError(iB+1,lDataErr);
	//std::cout << " - " << datafiles[iData] << " = " <<  lData << " +/- " << lDataErr << std::endl;
       }
      
      std::cout << " ---- Total data = " << nData << " +/- " << sqrt(nDataErr) << std::endl;

      datametRangeTotal[iS]->SetBinContent(iB+1,nData);
      datametRangeTotal[iS]->SetBinError(iB+1,sqrt(nDataErr));
      qcdmetRangeTotal[iS]->SetBinContent(iB+1,nData-nBkg);
      qcdmetRangeTotal[iS]->SetBinError(iB+1,sqrt(nDataErr+nBkgErr));
      if (iB<nBinsInt) {
	qcdmetIntegratedTotal[iS]->SetBinContent(iB+1,nDataInt-nBkgInt);
	qcdmetIntegratedTotal[iS]->SetBinError(iB+1,sqrt(nDataIntErr+nBkgIntErr));
	if (iS%2==0) {
	  yaxisInt[iS/2][iB] = nDataInt-nBkgInt;
	  yaxisIntErr[iS/2][iB] = sqrt(nDataIntErr+nBkgIntErr);
	}
	else {
	double num = yaxisInt[iS/2][iB];
	double numErr = yaxisIntErr[iS/2][iB];
	double den = nDataInt-nBkgInt;
	double denErr = sqrt(nDataIntErr+nBkgIntErr);
	yaxisInt[iS/2][iB] = num/den;
	yaxisIntErr[iS/2][iB] = fabs(yaxisInt[iS/2][iB]*sqrt(pow(numErr/num,2)+pow(denErr/den,2)));
	}
      }

      if (iS%2==0) {
	yaxis[iS/2][iB] = nData-nBkg;
	yaxisErr[iS/2][iB] = sqrt(nDataErr+nBkgErr);
      }
      else {
	double num = yaxis[iS/2][iB];
	double numErr = yaxisErr[iS/2][iB];
	double den = nData-nBkg;
	double denErr = sqrt(nDataErr+nBkgErr);
	yaxis[iS/2][iB] = num/den;
	yaxisErr[iS/2][iB] = fabs(yaxis[iS/2][iB]*sqrt(pow(numErr/num,2)+pow(denErr/den,2)));
      }
      
      std::cout << " ---- Total bkg Int = " << nBkgInt << " +/- " << sqrt(nBkgIntErr) << std::endl;
      std::cout << " ---- Total data Int = " << nDataInt << " +/- " << sqrt(nDataIntErr) << std::endl;


    }

    //fill new histos
    for (unsigned iBkg(0); iBkg<nSamples; ++iBkg){//loop on bkg files
      if (iS==0){
	thstack0.Add(metRange[iBkg][iS],"same");
	if (n_added_to_stack[iS] == 0) thstack0.SetHistogram((TH1F*)metRange[iBkg][iS]->Clone());
      } else if (iS==1) {
	thstack1.Add(metRange[iBkg][iS],"same");
	if (n_added_to_stack[iS] == 0) thstack1.SetHistogram((TH1F*)metRange[iBkg][iS]->Clone());
      } else if (iS==2) {
	thstack2.Add(metRange[iBkg][iS],"same");
	if (n_added_to_stack[iS] == 0) thstack2.SetHistogram((TH1F*)metRange[iBkg][iS]->Clone());
      } else if (iS==3) {
	thstack3.Add(metRange[iBkg][iS],"same");
	if (n_added_to_stack[iS] == 0) thstack3.SetHistogram((TH1F*)metRange[iBkg][iS]->Clone());
      }
      ++n_added_to_stack[iS];
    }//loop on bkg files


    //TGraphErrors *grSumBkg = new TGraphErrors(nBins,xaxis,yaxis,xaxisErr,yaxisErr);
    myc->cd();
    myc->SetLogy(1);
    myc->SetGridx(0);
    myc->SetGridy(0);
    //gStyle->SetLogy(1);
    //grSumBkg->GetXaxis()->SetTitle("MET range (GeV)");
    //grSumBkg->GetYaxis()->SetTitle("Sum bkg (events)");
    //if (iS==0) grSumBkg->SetTitle("#Delta#phi_{jj} < 1.0");
    //else grSumBkg->SetTitle("#Delta#phi_{jj} > 2.6");
    //grSumBkg->SetMinimum(0);
    //grSumBkg->SetMaximum(maxVal*1.1);
    if (iS==0) thstack0.Draw();
    else if (iS==1) thstack1.Draw();
    else if (iS==2) thstack1.Draw();
    else if (iS==3) thstack1.Draw();

    myc->Update();
    myc->Print(("bkgQCDSasha"+suffix+"_"+lSelection[iS]+"_samples.pdf").c_str());
    

    myc->cd();
    myc->SetLogy(1);
    myc->SetGridx(0);
    myc->SetGridy(0);

    if (iS==0) {
      metRangeTotal[iS]->SetTitle("#Delta#phi_{jj} < 1.0");
    }
    else {
      metRangeTotal[iS]->SetTitle("#Delta#phi_{jj} > 2.6");
    }
    metRangeTotal[iS]->GetYaxis()->SetRangeUser(0.01,datametRangeTotal[iS]->GetMaximum());
    metRangeTotal[iS]->Draw("PE");
    datametRangeTotal[iS]->SetMarkerStyle(20);
    datametRangeTotal[iS]->SetMarkerColor(2);
    datametRangeTotal[iS]->SetLineColor(2);
    datametRangeTotal[iS]->Draw("PEsame");
    qcdmetRangeTotal[iS]->SetMarkerStyle(21);
    qcdmetRangeTotal[iS]->SetMarkerColor(3);
    qcdmetRangeTotal[iS]->SetLineColor(3);
    qcdmetRangeTotal[iS]->Draw("PEsame");
 
      //thstack1.SetTitle("#Delta#phi_{jj} > 2.6");
      //thstack1.Draw();

    myc->Update();
    myc->Print(("bkgQCDSasha"+suffix+"_"+lSelection[iS]+".pdf").c_str());
  
    output->Add(metRangeTotal[iS]);
    output->Add(datametRangeTotal[iS]);
    output->Add(qcdmetRangeTotal[iS]);
    output->Add(qcdmetIntegratedTotal[iS]);

    for (unsigned idx(0); idx<nSamples; ++idx){//loop on bkg files
      output->Add(metRange[idx][iS]);
    }


  }//loop on selection

  myc->cd();
  myc->SetLogy(0);
  myc->SetGridx(1);
  myc->SetGridy(1);
  TGraphErrors *ratio = new TGraphErrors(nBins,xaxis,yaxis[0],xaxisErr,yaxisErr[0]);
  TGraphErrors *ratio2 = new TGraphErrors(nBins,xaxis,yaxis[1],xaxisErr,yaxisErr[1]);
  ratio->GetXaxis()->SetTitle("MET (GeV)");
  ratio->GetYaxis()->SetTitle("qcd R");
  ratio->SetTitle("");
  ratio->SetMinimum(0);
  ratio->SetMaximum(0.05);
  ratio->SetMarkerStyle(23);
  ratio->SetMarkerColor(4);
  ratio->SetLineColor(4);
  ratio2->SetMarkerStyle(22);
  ratio2->SetMarkerColor(2);
  ratio2->SetLineColor(2);
  ratio->Draw("APE");
  ratio2->Draw("PEsame");

    
  TLegend *leg = new TLegend(0.2,0.65,0.4,0.85);
  leg->SetFillColor(10);
  leg->AddEntry(ratio,"no CJV","P");
  leg->AddEntry(ratio2,"CJV","P");
  leg->Draw("same");

  //TFitResultPtr r = ratio->Fit("pol1","+","same");
  //r->SetLineColor(2);
  //ratio->Fit("pol1","+","same",60,120);
  gStyle->SetOptFit(1111111);

  myc->Update();
  myc->Print(("qcdRatio"+suffix+".pdf").c_str());

  output->Add(ratio);
  output->Add(ratio2);



  myc->cd();
  myc->SetLogy(0);
  myc->SetGridx(1);
  myc->SetGridy(1);
  TGraphErrors *ratioInt = new TGraphErrors(nBinsInt,xaxis,yaxisInt[0],xaxisErr,yaxisIntErr[0]);
  TGraphErrors *ratioInt2 = new TGraphErrors(nBinsInt,xaxis,yaxisInt[1],xaxisErr,yaxisIntErr[1]);
  ratioInt->GetXaxis()->SetTitle("minimum MET (GeV)");
  ratioInt->GetYaxis()->SetTitle("qcd R");
  ratioInt->SetTitle("");
  ratioInt->SetMinimum(-0.02);
  ratioInt->SetMaximum(0.04);
  ratioInt->SetMarkerStyle(23);
  ratioInt->SetMarkerColor(4);
  ratioInt->SetLineColor(4);
  ratioInt2->SetMarkerStyle(22);
  ratioInt2->SetMarkerColor(2);
  ratioInt2->SetLineColor(2);
  ratioInt->Draw("APE");
  ratioInt2->Draw("PEsame");

  leg->Draw("same");

  //TFitResultPtr r = ratioInt->Fit("pol1","+","same");
  //r->SetLineColor(2);
  //ratioInt->Fit("pol1","","same",50,100);
  //ratioInt->Fit("pol1","","same",50,100);
  //ratioInt->Fit("pol1","","same",50,100);
  //TF1 *fit1 = ratioInt->GetFunction("pol1");
  //fit1->SetLineColor(4);
  //fit1->SetRange(0,130);
  //fit1->Draw("same");

  //ratioInt2->Fit("pol1","","same",50,100);
  //TF1 *fit2 = ratioInt2->GetFunction("pol1");
  //fit2->SetLineColor(2);
  //gStyle->SetOptFit(1111111);
  //fit2->SetRange(0,130);
  //fit2->Draw("same");

  myc->Update();
  myc->Print(("qcdRatioIntegrated"+suffix+".pdf").c_str());

  output->Add(ratioInt);
  output->Add(ratioInt2);

  output->Write();
  //output->Close();

  return 0;

}//main
