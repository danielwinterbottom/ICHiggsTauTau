#include <vector>
#include <iostream>
#include <fstream>
#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TLegend.h"
#include "math.h"


int main(int argc, char *argv[]){
  TFile *centralfile = new TFile("../output/nunu/MET130/MC_W2JetsToLNu_enu.root");
  TFile *upfile = new TFile("../output/nunu/MET130/JERBETTER/MC_W2JetsToLNu_enu.root");
  TFile *downfile = new TFile("../output/nunu/MET130/JERWORSE/MC_W2JetsToLNu_enu.root");
  
  TH1F *metup;
  TH1F *metdown;
  TH1F *metcentral;

  TH1F *jet1ptup;
  TH1F *jet1ptdown;
  TH1F *jet1ptcentral;

  TH1F *jet2ptup;
  TH1F *jet2ptdown;
  TH1F *jet2ptcentral;
  

  centralfile->GetObject("HLTMetClean/met",metcentral);
  centralfile->GetObject("HLTMetClean/jpt_1",jet1ptcentral);
  centralfile->GetObject("HLTMetClean/jpt_2",jet2ptcentral);

  upfile->GetObject("HLTMetClean/met",metup);
  upfile->GetObject("HLTMetClean/jpt_1",jet1ptup);
  upfile->GetObject("HLTMetClean/jpt_2",jet2ptup);

  downfile->GetObject("HLTMetClean/met",metdown);
  downfile->GetObject("HLTMetClean/jpt_1",jet1ptdown);
  downfile->GetObject("HLTMetClean/jpt_2",jet2ptdown);

  TFile *outfile = new TFile("comphists.root","RECREATE");
  outfile->cd();

  TCanvas metcanvas;
  TLegend* metlegend = new TLegend(0.7,0.7,0.9,0.9);
  metlegend->AddEntry(metcentral,"Central","l");
  metlegend->AddEntry(metup,"JERBETTER","l");
  metlegend->AddEntry(metdown,"JERWORSE","l");

  metcentral->SetLineColor(1);
  metup->SetLineColor(2);
  metdown->SetLineColor(3);
  
  metcentral->Draw();
  metup->Draw("same");
  metdown->Draw("same");
  metlegend->Draw("same");

  metcanvas.SaveAs("metcomp.gif");
  metcanvas.Write();

  TCanvas jet1ptcanvas;
  TLegend* jet1ptlegend = new TLegend(0.7,0.7,0.9,0.9);
  jet1ptlegend->AddEntry(jet1ptcentral,"Central","l");
  jet1ptlegend->AddEntry(jet1ptup,"JERBETTER","l");
  jet1ptlegend->AddEntry(jet1ptdown,"JERWORSE","l");

  jet1ptcentral->SetLineColor(1);
  jet1ptup->SetLineColor(2);
  jet1ptdown->SetLineColor(3);
  
  jet1ptcentral->Draw();
  jet1ptup->Draw("same");
  jet1ptdown->Draw("same");
  jet1ptlegend->Draw("same");

  jet1ptcanvas.SaveAs("jet1ptcomp.gif");
  jet1ptcanvas.Write();

  TCanvas jet2ptcanvas;

  TLegend* jet2ptlegend = new TLegend(0.7,0.7,0.9,0.9);
  jet2ptlegend->AddEntry(jet2ptcentral,"Central","l");
  jet2ptlegend->AddEntry(jet2ptup,"JERBETTER","l");
  jet2ptlegend->AddEntry(jet2ptdown,"JERWORSE","l");

  jet2ptcentral->SetLineColor(1);
  jet2ptup->SetLineColor(2);
  jet2ptdown->SetLineColor(3);
  
  jet2ptcentral->Draw();
  jet2ptup->Draw("same");
  jet2ptdown->Draw("same");
  jet2ptlegend->Draw("same");

  jet2ptcanvas.SaveAs("jet2ptcomp.gif");
  jet2ptcanvas.Write();

  outfile->Close();

  return 0;
}
