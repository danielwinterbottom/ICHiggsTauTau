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
#include "TColor.h"


int qcdMethod2() {//main
  
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

  const unsigned nFiles = files.size();

  TFile *fBkg[nFiles];

  double lLumi = 19619.;
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


  TFile *fData;
  
  const unsigned nSel = 4;
  std::string lSelection[nSel] = {"DPhiQCD-noCJV","DPhiQCD","DPhiSIGNAL-noCJV","DPhiSIGNAL"};
  std::string lSelectionName[nSel] = {"#Delta#phi_{jj}>2.6, no CJV","#Delta#phi_{jj}>2.6, CJV","#Delta#phi_{jj}<1.0, no CJV","#Delta#phi_{jj}<1.0, CJV"};
  
  TCanvas *myc = new TCanvas("myc","myc",1200,1200);
  myc->Divide(2,2);
  gStyle->SetOptStat(0);
  
  std::ostringstream lPath,lHistName;
  
  std::string folder = "output";

  lHistName.str("");
  lHistName << "met";
  
  TH1F *hMET[nSel];
  TH1F *hMETBKG[nSel];
  TH1F *hMETQCD[nSel];

  TLegend *leg = new TLegend(0.6,0.12,0.9,0.42);
  leg->SetFillColor(10);
  
  for (unsigned iS(0); iS<nSel; ++iS){//loop on selection
    fData = TFile::Open("../output/nunu/MET0/Data.root");
    fData->cd(lSelection[iS].c_str());
    hMET[iS] = (TH1F*)gDirectory->Get(lHistName.str().c_str())->Clone();
  }

  std::cout << " -- Data files uploaded..." << std::endl;

  for (unsigned iS(0); iS<nSel; ++iS){//loop on selection
   //get histograms
    for (unsigned iBkg(0); iBkg<nFiles; ++iBkg){//loop on bkg files
      lPath.str("");
      lPath << "../" << folder << "/nunu/MET0/" << files[iBkg] << ".root";
      fBkg[iBkg] = TFile::Open(lPath.str().c_str());
      fBkg[iBkg]->cd(lSelection[iS].c_str());
      TH1F *lTmp = (TH1F*)gDirectory->Get(lHistName.str().c_str());
      //met[iBkg][iS]->Sumw2();
      lTmp->Scale(lLumi*normalisation[iBkg]);
      if (iBkg==0) hMETBKG[iS] = (TH1F*)lTmp->Clone();
      else hMETBKG[iS]->Add(lTmp);
      
    }//loop on bkg files

    hMETQCD[iS] = (TH1F*)hMET[iS]->Clone();
    hMETQCD[iS]->Add(hMETBKG[iS],-1);

  }//loop on selection

  std::cout << " -- BKG files uploaded..." << std::endl;

  int lColor[nSel] = {kGray+1,kRed-4,kGreen-9,kBlue-4};

  for (unsigned iS(0); iS<nSel; ++iS){//loop on selection
    
    hMET[iS]->SetLineColor(iS+1);
    hMET[iS]->SetMarkerColor(iS+1);
    hMET[iS]->SetMarkerStyle(20+iS);  
    hMET[iS]->Rebin(5);
    hMET[iS]->GetXaxis()->SetRangeUser(0,300);
    hMET[iS]->GetYaxis()->SetRangeUser(0.01,50000);
    hMET[iS]->SetTitle("");
    hMET[iS]->GetXaxis()->SetTitle("MET (GeV)");
    hMET[iS]->GetYaxis()->SetTitle("Entries / 5 GeV");

    hMETBKG[iS]->SetLineColor(iS+1);
    hMETBKG[iS]->SetFillColor(lColor[iS]);
    hMETBKG[iS]->SetFillStyle(1001);
    //hMETBKG[iS]->SetMarkerStyle(20+iS);  
    hMETBKG[iS]->Rebin(5);
    hMETBKG[iS]->GetXaxis()->SetRangeUser(0,300);
    hMETBKG[iS]->GetYaxis()->SetRangeUser(0.01,50000);
    hMETBKG[iS]->SetTitle("");
    hMETBKG[iS]->GetXaxis()->SetTitle("MET (GeV)");
    hMETBKG[iS]->GetYaxis()->SetTitle("Entries / 5 GeV");

    myc->cd(1);
    gPad->SetLogy(1);
    gPad->SetGridx(1);
    if (iS==0) hMETBKG[iS]->Draw("hist");
    else hMETBKG[iS]->Draw("histsame");
  }
  for (unsigned iS(0); iS<nSel; ++iS){//loop on selection
    myc->cd(1);
    hMET[iS]->Draw("PEsame");

    leg->AddEntry(hMET[iS],lSelectionName[iS].c_str(),"P");
    
    myc->cd(2);
    gPad->SetLogy(0);
    gPad->SetGridx(1);
    hMETQCD[iS]->SetLineColor(iS+1);
    hMETQCD[iS]->SetMarkerColor(iS+1);
    hMETQCD[iS]->SetMarkerStyle(20+iS);  
    hMETQCD[iS]->Rebin(5);
    hMETQCD[iS]->GetXaxis()->SetRangeUser(0,130);
    hMETQCD[iS]->SetTitle("");
    hMETQCD[iS]->GetXaxis()->SetTitle("MET (GeV)");
    hMETQCD[iS]->GetYaxis()->SetTitle("Arb. Units");
 
    //if (iS==0) hMETQCD[iS]->DrawNormalized("PE");
    //else hMETQCD[iS]->DrawNormalized("PEsame");
    double integral = 1;//hMETQCD[0]->Integral(0,hMETQCD[0]->FindBin(130));
    hMETQCD[iS]->Sumw2();
    hMETQCD[iS]->Scale(integral/hMETQCD[iS]->Integral());

    hMETQCD[iS]->GetYaxis()->SetRangeUser(0,0.2);


    if (iS==0) hMETQCD[iS]->Draw("PE");
    else hMETQCD[iS]->Draw("PEsame");

  }//loop on selection

  myc->cd(1);
  leg->Draw("same");
  
  myc->cd(2);
  //leg->Draw("same");


  myc->Update();
  myc->Print("METshapes_method2.pdf");
  
  return 0;
  
}//main
