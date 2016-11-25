#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <iomanip>

#include "TMath.h"
#include "Math/Vector4D.h"
#include "Math/VectorUtil.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h" 
#include "TGraphErrors.h" 
#include "TStyle.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TGraphAsymmErrors.h"

int CheckTmvaOutputs() {//main

  const unsigned int nFiles = 2;
  TFile *f[nFiles];
  //f[0] = TFile::Open("../TMVApp_VBF_METSIG_pseudoData_signalTimes1.root");
  //f[1] = TFile::Open("../TMVApp_VBF_METSIG_realData.root");
  f[0] = TFile::Open("../TMVApp_pseudoData_signalTimes1.root");
  f[1] = TFile::Open("../TMVApp_realData.root");

  TCanvas *myc = new TCanvas("myc","",1);
  TCanvas *myc1 = new TCanvas("myc1","",1);
  gStyle->SetOptStat(0);
  myc->SetLogy(1);

  TH1F *hist[nFiles][4];

  for (unsigned iF(0);iF<nFiles;++iF){//loop on files
    if (!f[iF]){
      std::cout << "Cannot open input file." << std::endl;
      return 1;
    }
    f[iF]->cd();
    hist[iF][0] = (TH1F*)gDirectory->Get("MVA_BDT_data");
    hist[iF][1] = (TH1F*)gDirectory->Get("MVA_BDT_sig");
    hist[iF][2] = (TH1F*)gDirectory->Get("MVA_BDT_qcd");
    hist[iF][3] = (TH1F*)gDirectory->Get("MVA_BDT_ewk");
  }//loop on files

  int binx = hist[0][1]->FindBin(-0.35);
  double integral = hist[0][1]->Integral(binx,hist[0][1]->GetNbinsX()+1);
  double total = hist[0][1]->Integral(0,hist[0][1]->GetNbinsX()+1);

  std::cout << "Integrals between bin " << binx << " " << hist[0][1]->GetBinLowEdge(binx) << " and " << hist[0][1]->GetBinLowEdge(hist[0][1]->GetNbinsX()+1) << std::endl
	    << " Signal = " << integral << " " << total << " " << integral/total << std::endl;

  integral = hist[1][0]->Integral(binx,hist[0][1]->GetNbinsX()+1);
  total = hist[1][0]->Integral();
  std::cout << " Data = " << integral << " " << total << " " << integral/total << std::endl;

  integral = hist[0][2]->Integral(binx,hist[0][1]->GetNbinsX()+1);
  total = hist[0][2]->Integral();
  hist[0][2]->GetXaxis()->SetRangeUser(0.6,1.0);
  std::cout << " QCD = " << integral << " " << total << " " << integral/total << std::endl;
  integral = hist[0][3]->Integral(binx,hist[0][1]->GetNbinsX()+1);
  total = hist[0][3]->Integral();
  std::cout << " EWK = " << integral << " " << total << " " << integral/total << std::endl;

 
  myc->cd();
  double max = std::max(hist[0][2]->GetMaximum(),hist[1][0]->GetMaximum())*1.2;

  hist[0][2]->SetLineColor(3);
  hist[0][2]->GetXaxis()->SetTitle("QCD BDT");
  hist[0][2]->GetYaxis()->SetTitle("Events");
  hist[0][2]->GetXaxis()->SetRangeUser(-1.,0.2);
  hist[0][2]->SetMaximum(max);
  hist[0][2]->SetTitle("");
  hist[0][2]->Draw();
  hist[0][3]->SetLineColor(6);
  hist[0][3]->Draw("same");
  
  //signal
  hist[0][1]->SetLineColor(4);
  hist[0][1]->SetFillColor(4);
  hist[0][1]->SetFillStyle(3004);
  hist[0][1]->Draw("same");
  //data
  hist[1][0]->SetMarkerStyle(2);
  hist[1][0]->Draw("PEsame");
  
  TLegend *leg =new TLegend(0.6,0.7,1.0,1.0);
  leg->SetFillColor(10);
  leg->AddEntry(hist[1][0],"Data L=19.5 fb^{-1}","P");
  leg->AddEntry(hist[0][2],"QCD","L");
  leg->AddEntry(hist[0][3],"V+Top+VV","L");
  leg->AddEntry(hist[0][1],"Signal m_{H}=125 GeV","F");
  leg->Draw("same");

  myc->Update();
  myc->Print("TmvaPlots/QCDBDToutputData_log.pdf");

  myc1->SetLogy(0);
  myc1->cd();
  hist[0][1]->Scale(10);
  hist[0][2]->GetXaxis()->SetRangeUser(-1.,0.2);
  hist[0][2]->SetMaximum(6000);
  hist[0][2]->Draw();
  hist[0][3]->Draw("same");
  hist[0][1]->Draw("same");
  hist[1][0]->Draw("PEsame");

  TLegend *leg2 =new TLegend(0.5,0.7,1.0,1.0);
  leg2->SetFillColor(10);
  leg2->AddEntry(hist[1][0],"Data L=19.5 fb^{-1}","P");
  leg2->AddEntry(hist[0][2],"QCD","L");
  leg2->AddEntry(hist[0][3],"V+Top+VV","L");
  leg2->AddEntry(hist[0][1],"Signal m_{H}=125 GeV #times 10","L");
  //leg2->AddEntry(hist[0][1],"Signal m_{H}=125 GeV","L");
  leg2->Draw("same");


  myc1->Update();
  myc1->Print("TmvaPlots/QCDBDToutputData.pdf");


  return 0;
}//main
