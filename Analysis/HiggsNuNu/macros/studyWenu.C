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
#include "TGraphAsymmErrors.h"


int studyWenu(){//main

  //TFile * fData;

  std::vector<std::string> bkgfiles;
  bkgfiles.push_back("MC_TTJets");
  bkgfiles.push_back("MC_T-tW");
  bkgfiles.push_back("MC_Tbar-tW");
  bkgfiles.push_back("MC_SingleT-s-powheg-tauola");
  bkgfiles.push_back("MC_SingleTBar-s-powheg-tauola");
  bkgfiles.push_back("MC_SingleT-t-powheg-tauola");
  bkgfiles.push_back("MC_SingleTBar-t-powheg-tauola");
  bkgfiles.push_back("MC_WW-pythia6-tauola");
  bkgfiles.push_back("MC_WZ-pythia6-tauola");
  bkgfiles.push_back("MC_ZZ-pythia6-tauola");
  bkgfiles.push_back("MC_DYJJ01JetsToLL_M-50_MJJ-200");
  bkgfiles.push_back("MC_DYJetsToLL");
  bkgfiles.push_back("MC_DY1JetsToLL");
  bkgfiles.push_back("MC_DY2JetsToLL");
  bkgfiles.push_back("MC_DY3JetsToLL");
  bkgfiles.push_back("MC_DY4JetsToLL");
  bkgfiles.push_back("MC_ZJetsToNuNu_100_HT_200");
  bkgfiles.push_back("MC_ZJetsToNuNu_200_HT_400");
  bkgfiles.push_back("MC_ZJetsToNuNu_400_HT_inf");
  bkgfiles.push_back("MC_ZJetsToNuNu_50_HT_100");

  std::vector<std::string> Wfiles;
  Wfiles.push_back("MC_WJetsToLNu-v1_enu");
  Wfiles.push_back("MC_WJetsToLNu-v2_enu");
  Wfiles.push_back("MC_W1JetsToLNu_enu");
  Wfiles.push_back("MC_W2JetsToLNu_enu");
  Wfiles.push_back("MC_W3JetsToLNu_enu");
  Wfiles.push_back("MC_W4JetsToLNu_enu");


  std::vector<std::string> datafiles;
  datafiles.push_back("Data_MET-2012A-13Jul2012-v1");
  datafiles.push_back("Data_MET-2012A-06Aug2012-v1");
  datafiles.push_back("Data_MET-2012B-13Jul2012-v1");
  datafiles.push_back("Data_MET-2012C-24Aug2012-v1");
  datafiles.push_back("Data_MET-2012C-11Dec2012-v1"); 
  datafiles.push_back("Data_MET-2012C-PromptReco-v2");
  datafiles.push_back("Data_MET-2012D-PromptReco-v1");

  std::string folder = "output_mjj1100";
  std::string suffix = "_mjj1100";
  //std::string suffix = "metL1HLTnomu";//metL1noenomu

  const unsigned nBkgFiles = bkgfiles.size();
  const unsigned nWFiles = Wfiles.size();
  const unsigned nData = datafiles.size();

  TFile *fBkg[nBkgFiles];
  TFile *fW[nWFiles];
  TFile *fData[nData];

  //TFile *output = new TFile(("BkgForQCDEstimation"+suffix+".root").c_str(),"RECREATE");

  double lLumi = 19619.;
  
  double normalisation[nBkgFiles];
  normalisation[0] = 234./6923750.0;
  normalisation[1] = 11.1/497658.0;
  normalisation[2] = 11.1/493460.0;
  normalisation[3] = 3.79/259961.0;
  normalisation[4] = 1.76/139974.0;
  normalisation[5] = 56.4/3758227.0;
  normalisation[6] = 30.7/1935072.0;
  normalisation[7] = 54.838/10000431.0;
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

  double w_normalisation[nWFiles];
  for (unsigned iN(0); iN<nWFiles;++iN){
    w_normalisation[iN] = 37509/76102995.0;
  }

  TCanvas *myc = new TCanvas("myc","",800,1000);
  gStyle->SetOptStat(0);
  //myc->Divide(2,3);

  std::ostringstream lPath;

  std::string lHistName[3] = {"met","met","met_noelectrons"};
  //std::string lHistName[3] = {"met","met_pu","met_noelectrons"};
   //  std::string lSelection = "DPhiSIGNAL";
  std::vector<std::string> selections;
  selections.push_back("AN");
  selections.push_back("DEta");
  selections.push_back("MET");
  selections.push_back("TightMjj");
  selections.push_back("CJV");
  selections.push_back("DPhiQCD");
  selections.push_back("DPhiSIGNAL");
  
  const unsigned nSel = selections.size();

  TH1F *metBkg[3];
  TH1F *metW[3];
  TH1F *metData[3];
  TH1F *metDataminusBkg[3];

  std::string lRegion[3] = {"nunu","enu","enu"};
  //std::string lRegion[3] = {"nunu","enu-metL1HLTnomu","enu-metL1HLTnomu"};
  //std::string lRegion[3] = {"nunu","enu-metL1noenomu","enu-metL1noenomu"};

  for (unsigned iS(0); iS<nSel; ++iS){//loop on selections
 
    for (unsigned iP(0); iP<3; ++iP){//loop on points
      lPath.str("");
      lPath << "../" << folder << "/" << lRegion[iP] << "/MET0/";
      
      //get histograms
      for (unsigned iBkg(0); iBkg<nBkgFiles; ++iBkg){//loop on bkg files
	fBkg[iBkg] = TFile::Open((lPath.str()+bkgfiles[iBkg]+".root").c_str());
	//fBkg[iBkg]->cd((lSelection[iS]+"/weights/").c_str());
	//if (iP < 2) 
	fBkg[iBkg]->cd((selections[iS]+"/").c_str());
	//else  fBkg[iBkg]->cd((selections[iS]+"/weights/").c_str());

	TH1F *lTmpHist = (TH1F*)gDirectory->Get(lHistName[iP].c_str());
	lTmpHist->Sumw2();
	lTmpHist->Scale(lLumi*normalisation[iBkg]);
	if (iBkg==0) {
	  metBkg[iP] = lTmpHist;
	  metBkg[iP]->Sumw2();
	}
	else {
	  metBkg[iP]->Add(lTmpHist);
	}
	//double lErr=0;
	//double lContent = lTmpHist->IntegralAndError(131,metBkg[iP]->GetNbinsX()+1,lErr);
	//if (iS==nSel-1 && iP==2) std::cout << lTmpHist->GetBinLowEdge(131) << "-1000, Bkg " << iBkg << " " << lContent << " +/- " << lErr << std::endl;
      }//loop on bkg hists
      for (unsigned iW(0); iW<nWFiles; ++iW){//loop on w files
	fW[iW] = TFile::Open((lPath.str()+Wfiles[iW]+".root").c_str());
	//fW[iW]->cd((lSelection[iS]+"/weights/").c_str());
	//if (iP < 2) 
	fW[iW]->cd((selections[iS]+"/").c_str());
	//else fW[iW]->cd((selections[iS]+"/weights/").c_str());
	if (iW==0) {
	  metW[iP] = (TH1F*)gDirectory->Get(lHistName[iP].c_str())->Clone();
	  metW[iP]->Sumw2();
	  metW[iP]->Scale(lLumi*w_normalisation[iW]);
	}
	else {
	  TH1F *lTmpHist = (TH1F*)gDirectory->Get(lHistName[iP].c_str());
	  lTmpHist->Scale(lLumi*w_normalisation[iW]);
	  metW[iP]->Add(lTmpHist);
	}
      }//loop on w hists
      for (unsigned iD(0); iD<nData;++iD){//loop on data files
	fData[iD] = TFile::Open((lPath.str()+datafiles[iD]+".root").c_str());
	//if (iP < 2) 
	fData[iD]->cd((selections[iS]+"/").c_str());
	//else fData[iD]->cd((selections[iS]+"/weights/").c_str());
	if (iD==0) {
	  metData[iP] = (TH1F*)gDirectory->Get(lHistName[iP].c_str())->Clone();
	  metData[iP]->Sumw2();
	}
	else {
	  TH1F *lTmpHist = (TH1F*)gDirectory->Get(lHistName[iP].c_str());
	  metData[iP]->Add(lTmpHist);
	}
      }//loop on data hists
      metDataminusBkg[iP] = (TH1F*)metData[iP]->Clone();
      metDataminusBkg[iP]->Sumw2();
      metDataminusBkg[iP]->Add(metBkg[iP],-1);

      myc->cd();
      metW[iP]->Rebin(25);
      metW[iP]->GetXaxis()->SetRangeUser(0,400);
      metW[iP]->GetXaxis()->SetTitle("MET (GeV)");
      metW[iP]->GetYaxis()->SetTitle("Events/25 GeV");
      metW[iP]->SetTitle(("MC W, "+selections[iS]).c_str());
      metW[iP]->SetLineColor(1+iP);
      metW[iP]->SetMarkerColor(1+iP);
      metW[iP]->SetMarkerStyle(20+iP);
      if (iP==0) metW[iP]->Draw("PE");
      else metW[iP]->Draw("histsame");
    }

    
    TLegend *legend = new TLegend(0.5,0.7,0.9,0.9);
    legend->SetFillColor(10);
    legend->AddEntry(metW[0],"Signal region","P");
    legend->AddEntry(metW[1],"e+#nu with MET","L");
    legend->AddEntry(metW[2],"e+#nu with MET+e","L");
    //legend->AddEntry(metData[1],"Data","P");
    //legend->AddEntry(metBkg[1],"Sum BKG","P");
    //legend->AddEntry(metDataminusBkg[1],"Data-bkg","P");

    myc->cd();
    legend->Draw("same");

    myc->Update();
    myc->Print(("METWenu"+suffix+"_"+selections[iS]+"_MCW_MET.pdf").c_str());

    for (unsigned iP(0); iP<3; ++iP){//loop on points

      myc->cd();
      metData[iP]->Rebin(25);
      metData[iP]->SetLineColor(6);
      metData[iP]->SetMarkerColor(6);
      metData[iP]->SetMarkerStyle(25);
      metBkg[iP]->Rebin(25);
      metBkg[iP]->SetLineColor(7);
      metBkg[iP]->SetMarkerColor(7);
      metBkg[iP]->SetMarkerStyle(27);
      metDataminusBkg[iP]->Rebin(25);
      metDataminusBkg[iP]->GetXaxis()->SetRangeUser(0,400);
      //metDataminusBkg[iP]->GetYaxis()->SetRangeUser(-50,100);
      metDataminusBkg[iP]->GetXaxis()->SetTitle("MET (GeV)");
      metDataminusBkg[iP]->GetYaxis()->SetTitle("Data - BKG");
      metDataminusBkg[iP]->SetTitle(("Data - MC Bkg, "+selections[iS]).c_str());
      metDataminusBkg[iP]->SetLineColor(1+iP);
      metDataminusBkg[iP]->SetMarkerColor(1+iP);
      metDataminusBkg[iP]->SetMarkerStyle(20+iP);
      if (iP==1) metDataminusBkg[iP]->Draw("PE");
      else metDataminusBkg[iP]->Draw("PEsame");
      //metData[iP]->Draw("PEsame");
      //metBkg[iP]->Draw("PEsame");
      

    }//loop on points


    myc->Update();
    myc->Print(("METWenu"+suffix+"_"+selections[iS]+"_DATAminusBKG.pdf").c_str());

    //myc->cd(2);
    //legend->Draw("same");
   

    TH1F *grRatio[2];
    TH1F *grRatioBkg[2];

  
    grRatio[0] = (TH1F*)metW[0]->Clone();
    grRatio[0]->Sumw2();
    grRatio[0]->Divide(metW[1]);
    grRatio[0]->SetLineColor(2);
    grRatio[0]->SetMarkerColor(2);
    grRatio[0]->SetMarkerStyle(21);
    grRatio[1] = (TH1F*)metW[0]->Clone();
    grRatio[1]->Sumw2();
    grRatio[1]->Divide(metW[2]);
    grRatio[1]->SetLineColor(3);
    grRatio[1]->SetMarkerColor(3);
    grRatio[1]->SetMarkerStyle(22);


    grRatioBkg[0] = (TH1F*)metW[0]->Clone();
    grRatioBkg[0]->Sumw2();
    //grRatioBkg[0]->Divide(metW[1]);
    //grRatioBkg[0]->Multiply(metDataminusBkg[1]);
    grRatioBkg[0]->SetLineColor(2);
    grRatioBkg[0]->SetMarkerColor(2);
    grRatioBkg[0]->SetMarkerStyle(21);
    grRatioBkg[1] = (TH1F*)metW[0]->Clone();
    grRatioBkg[1]->Sumw2();
    //grRatioBkg[1]->Divide(metW[2]);
    //grRatioBkg[1]->Multiply(metDataminusBkg[2]);
    grRatioBkg[1]->SetLineColor(3);
    grRatioBkg[1]->SetMarkerColor(3);
    grRatioBkg[1]->SetMarkerStyle(22);

    for (unsigned iB(0); iB<static_cast<unsigned>(grRatioBkg[0]->GetNbinsX()); ++iB){//loop on bins
      double lErr1=0,lErr2=0,lErr3=0,lErr4=0;
      double lContent1 = metW[0]->IntegralAndError(iB+1,metW[0]->GetNbinsX()+1,lErr1);
      double lContent2 = metW[1]->IntegralAndError(iB+1,metW[1]->GetNbinsX()+1,lErr2);
      double lContent3 = metData[1]->IntegralAndError(iB+1,metData[1]->GetNbinsX()+1,lErr3);
      double lContent4 = metBkg[1]->IntegralAndError(iB+1,metBkg[1]->GetNbinsX()+1,lErr4);

      if (lContent1 != lContent1) lContent1=0;
      if (lContent2 != lContent2) lContent2=0;
      if (lContent3 != lContent3) lContent3=0;
      if (lContent4 != lContent4) lContent4=0;
      if (lErr1 != lErr1) lErr1=0;
      if (lErr2 != lErr2) lErr2=0;
      if (lErr3 != lErr3) lErr3=0;
      if (lErr4 != lErr4) lErr4=0;
      double lContent = 0;
      if (lContent2 != 0) lContent = lContent1*(lContent3-lContent4)/lContent2;
      grRatioBkg[0]->SetBinContent(iB+1,std::max(lContent,0.));
      double lErr = lContent*sqrt(pow(lErr1/lContent1,2)+pow(lErr2/lContent2,2)+pow(sqrt(lErr3*lErr3+lErr4*lErr4)/(lContent3-lContent4),2));
      if (lErr != lErr) lErr = 0;
      grRatioBkg[0]->SetBinError(iB+1,lErr);

 
      lContent2 = metW[2]->IntegralAndError(iB+1,metW[2]->GetNbinsX()+1,lErr2);
      lContent3 = metData[2]->IntegralAndError(iB+1,metData[2]->GetNbinsX()+1,lErr3);
      lContent4 = metBkg[2]->IntegralAndError(iB+1,metBkg[2]->GetNbinsX()+1,lErr4);
      if (lContent2 != lContent2) lContent2=0;
      if (lContent3 != lContent3) lContent3=0;
      if (lContent4 != lContent4) lContent4=0;
      if (lErr2 != lErr2) lErr2=0;
      if (lErr3 != lErr3) lErr3=0;
      if (lErr4 != lErr4) lErr4=0;
      if (lContent3 != 0) lContent = lContent1*(lContent3-lContent4)/lContent2;
      grRatioBkg[1]->SetBinContent(iB+1,lContent);
      lErr = lContent*sqrt(pow(lErr1/lContent1,2)+pow(lErr2/lContent2,2)+pow(sqrt(lErr3*lErr3+lErr4*lErr4)/(lContent3-lContent4),2));
      if (lErr != lErr) lErr = 0;
      grRatioBkg[1]->SetBinError(iB+1,lErr);

//       if (iB < 20) {
// 	std::cout << " - Bin " << iB+1 << "[" << metW[0]->GetBinLowEdge(iB+1) << "," << metW[0]->GetBinLowEdge(metW[0]->GetNbinsX()+1) << "]" << std::endl;
// 	std::cout << " W nunu " << lContent1 << " +/- " << lErr1 << " " << lErr1/lContent1 << std::endl;
// 	std::cout << " W enu " << lContent2 << " +/- " << lErr2<< " " << lErr2/lContent2 << std::endl;
// 	std::cout << " Data enu " << lContent3 << " +/- " << lErr3 << std::endl;
// 	std::cout << " bkg enu " << lContent4 << " +/- " << lErr4 << std::endl;
// 	std::cout << " Data-bkg = " << (lContent3-lContent4)<< " +/- " << sqrt(lErr3*lErr3+lErr4*lErr4) << std::endl;
// 	std::cout << " Result = " << lContent << " +/- " << lErr << std::endl;
//       }
    }


    myc->cd();
    grRatio[0]->GetYaxis()->SetRangeUser(0,10);
    grRatio[0]->GetYaxis()->SetTitle("N^{S}_{MC}/N^{C}_{MC}(MET)");
    grRatio[0]->SetTitle("");
    grRatio[0]->Draw("PE");

    myc->Update();
    myc->Print(("METWenu"+suffix+"_"+selections[iS]+"_ratioMC_MET.pdf").c_str());

    myc->cd();
    grRatio[1]->GetYaxis()->SetRangeUser(0,10);
    grRatio[1]->GetYaxis()->SetTitle("N^{S}_{MC}/N^{C}_{MC}(MET+e)");
    grRatio[1]->SetTitle("");
    grRatio[1]->Draw("PE");

    myc->Update();
    myc->Print(("METWenu"+suffix+"_"+selections[iS]+"_ratioMC_METnoE.pdf").c_str());


    myc->cd();
    //grRatioBkg[0]->GetYaxis()->SetRangeUser(0,100);
    grRatioBkg[0]->GetYaxis()->SetTitle("N^{S}_{WMC}/N^{C}_{WMC}#times N^{C}_{WData}(MET)");
    grRatioBkg[0]->GetXaxis()->SetTitle("minimum MET (GeV)");
    grRatioBkg[0]->SetTitle("");
    grRatioBkg[0]->Draw("PE");
    grRatioBkg[1]->Draw("PEsame");
    legend->Draw("same");
    myc->Update();
    myc->Print(("METWenu"+suffix+"_"+selections[iS]+"_Westimate.pdf").c_str());

 //    myc->cd();
//     //grRatioBkg[1]->GetYaxis()->SetRangeUser(0,100);
//     grRatioBkg[1]->GetYaxis()->SetTitle("N^{S}_{WMC}/N^{C}_{WMC}#times N^{C}_{WData} (MET+e)");
//     grRatioBkg[1]->GetXaxis()->SetTitle("minimum MET+e (GeV)");
//     grRatioBkg[1]->SetTitle("");
//     grRatioBkg[1]->Draw("PE");

//     myc->Update();
//     myc->Print(("METWenu"+suffix+"_"+selections[iS]+"_Westimate_METnoE.pdf").c_str());


    //myc->Update();
    //myc->Print(("METWenu"+suffix+"_"+selections[iS]+".pdf").c_str());
  }//loop on selections
  
  return 0;

  
}//main
