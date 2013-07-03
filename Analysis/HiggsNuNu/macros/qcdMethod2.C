
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


  

  /////////////////////////////////////////////////////////
  ///////// To be updated with additional histos /////////
  ////////////////////////////////////////////////////////
  const unsigned nHists = 20;
  //ADD MET BINNED HT PLOTS
  std::string lHistName[nHists] = {"met","n_jetsingap","Ht/Ht","Ht/SqrtHt","Ht/unclusteredEt","Ht/MHT","Ht/dphimetMHT","Ht/MetHt0to10","Ht/MetHt10to20","Ht/MetHt20to30","Ht/MetHt30to40","Ht/MetHt40to50","Ht/MetHt50to60","Ht/MetHt60to70","Ht/MetHt70to80","Ht/MetHt80to90","Ht/MetHt90to100","Ht/MetHt100to110","Ht/MetHt110to120","Ht/MetHt120to130"};

  bool doCJV[nHists] = {true,false,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true};
  bool lBlind[nHists] = {true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};

  //for plotting variables normalised to data luminosity
  unsigned lRebin[nHists] = {5,1,20,1,20,20,20,40,40,40,40,40,40,40,40,40,40,40,40,40};
  double xmin[nHists] = {0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  double xmax[nHists] = {300,10,800,35,400,500,6.3,800,800,800,800,800,800,800,800,800,800,800,800,800};
  double ymin[nHists] = {0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01};
  double ymax[nHists] = {50000,500000,30000,50000,60000,30000,30000,30000,30000,30000,30000,30000,30000,30000,30000,30000,30000,30000,30000,30000};
  std::string xAxisTitle[nHists] = {"MET (GeV)","n_{jets} p_{T}>30 GeV #eta_{1} < #eta < #eta_{2}","H_{T}/GeV","\sqrt_{H_{T}}/GeV^{1/2}","unclustered E_{T}/GeV","|mH_{T}|/GeV","#Delta#phi_{MET-MHT}","H_{T}/GeV","H_{T}/GeV","H_{T}/GeV","H_{T}/GeV","H_{T}/GeV","H_{T}/GeV","H_{T}/GeV","H_{T}/GeV","H_{T}/GeV","H_{T}/GeV","H_{T}/GeV","H_{T}/GeV","H_{T}/GeV"};
  std::string yAxisTitle[nHists] = {"Entries / 5 GeV","Entries","Entries","Entries","Entries","Entries","Entries","Entries","Entries","Entries","Entries","Entries","Entries","Entries","Entries","Entries","Entries","Entries","Entries","Entries"};
  int lLogY[nHists] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

  //for histos normalised to unity
  double xminNorm[nHists] = {0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  double xmaxNorm[nHists] = {130,10,800,35,400,500,6.3,800,800,800,800,800,800,800,800,800,800,800,800,800};
  double yminNorm[nHists] = {0,0.0001,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  double ymaxNorm[nHists] = {0.2,0.5,0.3,0.25,0.3,0.2,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.5};
  int lLogYNorm[nHists] = {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  //output file
  std::string lOutputPDF[nHists] = {"METshapes_method2.pdf","NjetsInGAP_method2.pdf","ht.pdf","sqrtht.pdf","unclusteredet.pdf","mht.pdf","dphimetmht.pdf","metht0to10.pdf","metht10to20.pdf","metht20to30.pdf","metht30to40.pdf","metht40to50.pdf","metht50to60.pdf","metht60to70.pdf","metht70to80.pdf","metht80to90.pdf","metht90to100.pdf","metht100to110.pdf","metht110to120.pdf","metht120to130.pdf"};

  TLegend *leg[nHists];
  leg[0] = new TLegend(0.55,0.6,0.89,0.89);
  leg[1] = new TLegend(0.55,0.7,0.89,0.89);
  leg[2] = new TLegend(0.55,0.7,0.89,0.89);
  leg[3] = new TLegend(0.55,0.7,0.89,0.89);
  leg[4] = new TLegend(0.55,0.7,0.89,0.89);
  leg[5] = new TLegend(0.55,0.7,0.89,0.89);
  leg[6] = new TLegend(0.55,0.7,0.89,0.89);
  leg[7] = new TLegend(0.55,0.7,0.89,0.89);
  leg[8] = new TLegend(0.55,0.7,0.89,0.89);
  leg[9] = new TLegend(0.55,0.7,0.89,0.89);
  leg[10] = new TLegend(0.55,0.7,0.89,0.89);
  leg[11] = new TLegend(0.55,0.7,0.89,0.89);
  leg[12] = new TLegend(0.55,0.7,0.89,0.89);
  leg[13] = new TLegend(0.55,0.7,0.89,0.89);
  leg[14] = new TLegend(0.55,0.7,0.89,0.89);
  leg[15] = new TLegend(0.55,0.7,0.89,0.89);
  leg[16] = new TLegend(0.55,0.7,0.89,0.89);
  leg[17] = new TLegend(0.55,0.7,0.89,0.89);
  leg[18] = new TLegend(0.55,0.7,0.89,0.89);
  leg[19] = new TLegend(0.55,0.7,0.89,0.89);

  //////////////////////////////////////////////////////
  ////// end section to modify /////////////////////////
  //////////////////////////////////////////////////////


  const unsigned nSel = 4;
  std::string lSelection[nSel] = {"DPhiQCD-noCJV","DPhiQCD","DPhiSIGNAL-noCJV","DPhiSIGNAL"};
  std::string lSelectionName[nSel] = {"#Delta#phi_{jj}>2.6, no CJV","#Delta#phi_{jj}>2.6, CJV","#Delta#phi_{jj}<1.0, no CJV","#Delta#phi_{jj}<1.0, CJV"};
  
  int lColor[nSel] = {kGray+1,kRed-4,kGreen-9,kBlue-4};
  
  TCanvas *myc = new TCanvas("myc","myc",1200,600);
  myc->Divide(2,1);
  gStyle->SetOptStat(0);
  
  std::ostringstream lPath;
    
  std::string folder = "output";


  for (unsigned iH(0); iH<nHists;++iH){//loop on hists

    TH1F *hMET[nSel];
    TH1F *hMETBKG[nSel];
    TH1F *hMETQCD[nSel];
    
    leg[iH]->SetFillColor(10);

    TFile *fData[nSel];
    
    for (unsigned iS(0); iS<nSel; ++iS){//loop on selection
      lPath.str("");
      lPath << "../" << folder  << "/nunu/MET0/Data.root" ;
      fData[iS] = new TFile(lPath.str().c_str());//TFile::Open(lPath.str().c_str());
      fData[iS]->cd(lSelection[iS].c_str());
      hMET[iS] = (TH1F*)gDirectory->Get(lHistName[iH].c_str())->Clone();
    }
    
    std::cout << " -- Data files uploaded..." << std::endl;

    TFile *fBkg[nSel][nFiles];
    
    for (unsigned iS(0); iS<nSel; ++iS){//loop on selection
      //get histograms
      for (unsigned iBkg(0); iBkg<nFiles; ++iBkg){//loop on bkg files
	lPath.str("");
	lPath << "../" << folder << "/nunu/MET0/" << files[iBkg] << ".root";
	fBkg[iS][iBkg] = new TFile(lPath.str().c_str());//TFile::Open(lPath.str().c_str());
	fBkg[iS][iBkg]->cd(lSelection[iS].c_str());
	TH1F *lTmp = (TH1F*)gDirectory->Get(lHistName[iH].c_str());
	//met[iBkg][iS]->Sumw2();
	lTmp->Scale(lLumi*normalisation[iBkg]);
	if (iBkg==0) hMETBKG[iS] = (TH1F*)lTmp->Clone();
	else hMETBKG[iS]->Add(lTmp);
	lTmp->Delete();
      }//loop on bkg files

      hMETQCD[iS] = (TH1F*)hMET[iS]->Clone();
      hMETQCD[iS]->Add(hMETBKG[iS],-1);

      if (iS>1 && lBlind[iH]) {
	//blind signal region in data
	for (unsigned iB(0); iB<hMET[iS]->GetNbinsX(); ++iB){
	  if (hMET[iS]->GetBinLowEdge(iB+1) > 130) {
	    hMET[iS]->SetBinContent(iB+1,0);
	    hMETQCD[iS]->SetBinContent(iB+1,0);
	    hMET[iS]->SetBinError(iB+1,0);
	    hMETQCD[iS]->SetBinError(iB+1,0);    
	  }
	}
      }
      
      
    }//loop on selection

    std::cout << " -- BKG files uploaded..." << std::endl;
    
    
    for (unsigned iS(0); iS<nSel; ++iS){//loop on selection
      if (!doCJV[iH] && iS%2==1) continue;
      hMET[iS]->SetLineColor(iS+1);
      hMET[iS]->SetMarkerColor(iS+1);
      hMET[iS]->SetMarkerStyle(20+iS);  
      hMET[iS]->Rebin(lRebin[iH]);
      hMET[iS]->GetXaxis()->SetRangeUser(xmin[iH],xmax[iH]);
      hMET[iS]->GetYaxis()->SetRangeUser(ymin[iH],ymax[iH]);
      hMET[iS]->SetTitle("");
      hMET[iS]->GetXaxis()->SetTitle(xAxisTitle[iH].c_str());
      hMET[iS]->GetYaxis()->SetTitle(yAxisTitle[iH].c_str());
    
      
      hMETBKG[iS]->SetLineColor(iS+1);
      hMETBKG[iS]->SetFillColor(lColor[iS]);
      hMETBKG[iS]->SetFillStyle(1001);
      //hMETBKG[iS]->SetMarkerStyle(20+iS);  
      hMETBKG[iS]->Rebin(lRebin[iH]);
      hMETBKG[iS]->GetXaxis()->SetRangeUser(xmin[iH],xmax[iH]);
      hMETBKG[iS]->GetYaxis()->SetRangeUser(ymin[iH],ymax[iH]);
      hMETBKG[iS]->SetTitle("");
      hMETBKG[iS]->GetXaxis()->SetTitle(xAxisTitle[iH].c_str());
      hMETBKG[iS]->GetYaxis()->SetTitle(yAxisTitle[iH].c_str());
      
      myc->cd(1);
      gPad->SetLogy(lLogY[iH]);
      gPad->SetGridx(1);
      if (iS==0) hMETBKG[iS]->Draw("hist");
      else hMETBKG[iS]->Draw("histsame");
    }
    for (unsigned iS(0); iS<nSel; ++iS){//loop on selection
      if (!doCJV[iH] && iS%2==1) continue;
      myc->cd(1);
      hMET[iS]->Draw("PEsame");

      leg[iH]->AddEntry(hMET[iS],lSelectionName[iS].c_str(),"P");
      
      myc->cd(2);
      gPad->SetLogy(lLogYNorm[iH]);
      gPad->SetGridx(1);
      hMETQCD[iS]->SetLineColor(iS+1);
      hMETQCD[iS]->SetMarkerColor(iS+1);
      hMETQCD[iS]->SetMarkerStyle(20+iS);  
      hMETQCD[iS]->Rebin(lRebin[iH]);
      hMETQCD[iS]->GetXaxis()->SetRangeUser(xminNorm[iH],xmaxNorm[iH]);
      hMETQCD[iS]->GetYaxis()->SetRangeUser(yminNorm[iH],ymaxNorm[iH]);
      hMETQCD[iS]->SetTitle("");
      hMETQCD[iS]->GetXaxis()->SetTitle(xAxisTitle[iH].c_str());
      hMETQCD[iS]->GetYaxis()->SetTitle("Arb. Units");
      
      //if (iS==0) hMETQCD[iS]->DrawNormalized("PE");
      //else hMETQCD[iS]->DrawNormalized("PEsame");
      double integral = 1;//hMETQCD[0]->Integral(0,hMETQCD[0]->FindBin(130));
      hMETQCD[iS]->Sumw2();
      hMETQCD[iS]->Scale(integral/hMETQCD[iS]->Integral());

      hMETQCD[iS]->GetYaxis()->SetRangeUser(yminNorm[iH],ymaxNorm[iH]);
      
      if (iS==0) hMETQCD[iS]->Draw("PE");
      else hMETQCD[iS]->Draw("PEsame");
      
    }//loop on selection

    myc->cd(1);
    leg[iH]->Draw("same");
    
    myc->cd(2);
    //leg[iH]->Draw("same");
    
    
    myc->Update();
    myc->Print(lOutputPDF[iH].c_str());

    for(int iS=0;iS<nSel;iS++){
      fData[iS]->Close();
      fData[iS]->Delete();
      for(int iBkg=0;iBkg<nFiles;iBkg++){
	fBkg[iS][iBkg]->Close();
	fBkg[iS][iBkg]->Delete();
      }
    }
  }//loop on hists

  return 0;
  
}//main
