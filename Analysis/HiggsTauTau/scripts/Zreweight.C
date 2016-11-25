#include <iostream>
#include <cmath>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TMinuit.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TStyle.h"
#include "TAttAxis.h"
#include "TFitResult.h"
#include "TLatex.h"
#include <fstream>
#include <TSystem.h>
#include "TLegend.h"
#include "TGaxis.h"


int main(){

std::string mvis[] = {"50","80","120","160","200","400","800"};

Float_t ybins[]={0,40,80,120,160,200,240,280,320,400,500,600,10000}; //pt   
Float_t xbins[]={0,50,80,120,160,200,400,800,10000}; //mvis  
TH2D *zptmass_histo = new TH2D("zptmass_histo","zptmass_histo",8,xbins,12,ybins);
TH2D *zptmass_histo_err = new TH2D("zptmass_histo_err","zptmass_histo_err",8,xbins,12,ybins);
TH2D *data_sub_2D = new TH2D("data_sub_2D","data_sub_2D",8,xbins,12,ybins);
TH2D *ZLL_2D = new TH2D("ZLL_2D","ZLL_2D",8,xbins,12,ybins);

for(int i=0; i<((sizeof(mvis)/sizeof(*mvis))-1); i++) {
	TFile *file1 = TFile::Open(("datacard_pt_tt_inclusive_zmm_2016_mvis"+mvis[i]+"to"+mvis[i+1]+".root").c_str());

    std::cout << "Reading file: " << "datacard_pt_tt_inclusive_zmm_2016_mvis"+mvis[i]+"to"+mvis[i+1]+".root" << std::endl;
    
    Float_t bins[] = {0,40,80,120,160,200,240,280,320,400,500,600};
    TH1F * data = new TH1F("data","data",11,bins);
    TH1F * W = new TH1F("W","W",11,bins);
    TH1F * QCD = new TH1F("QCD","QCD",11,bins);
    TH1F * VV = new TH1F("VV","VV",11,bins);
    TH1F * TT = new TH1F("TT","TT",11,bins);
    TH1F * ZLL = new TH1F("ZLL","ZLL",11,bins);
    TH1F * total_noDY = new TH1F("total_noDY","total_noDY",11,bins);
    TH1F * data_sub = new TH1F("data_sub","data_sub",11,bins);
    
    data = (TH1F*)file1->Get("Zmm_inclusive/data_obs"); 
    W = (TH1F*)file1->Get("Zmm_inclusive/W"); 
    QCD = (TH1F*)file1->Get("Zmm_inclusive/QCD"); 
    VV = (TH1F*)file1->Get("Zmm_inclusive/VV"); 
    ZLL = (TH1F*)file1->Get("Zmm_inclusive/ZLL"); 
    TT = (TH1F*)file1->Get("Zmm_inclusive/TT"); 
    total_noDY = (TH1F*)W->Clone();
    data_sub = (TH1F*)data->Clone();

    total_noDY->Add(VV,1);
    total_noDY->Add(TT,1);
    total_noDY->Add(QCD,1);

    data_sub->Add(total_noDY,-1);
    
    for(int h=1; h<=data->GetNbinsX(); h++) {
        double value = data_sub->GetBinContent(h);
        double err = value*sqrt( (pow((data->GetBinError(h)/data->GetBinContent(h)),2) + pow((total_noDY->GetBinError(h)/total_noDY->GetBinContent(h)),2) + pow((ZLL->GetBinError(h)/ZLL->GetBinContent(h)),2) )   )/ZLL->GetBinContent(h);
     
        std::cout << "Bin Index: " << h << ", mass range: " << data->GetXaxis()->GetBinLowEdge(h) << "-" <<  data->GetXaxis()->GetBinLowEdge(h+1) << ", data_sub: " << data_sub->GetBinContent(h) << " ,DY: " << ZLL->GetBinContent(h) <<  ", data/DY: " << data_sub->GetBinContent(h)/ZLL->GetBinContent(h) << " +/- " << err << std::endl;

    }

 
    for(int j=0; j<((sizeof(bins)/sizeof(*bins))-1); j++){
      int bin = int(stoi(mvis[i]) + (stoi(mvis[i+1])-stoi(mvis[i]))/2);
      std::cout << "Filling x bin: " << bin << ", and y bin: " << bins[j]+5 << ", with bin content: " << data_sub->GetBinContent(data_sub->GetXaxis()->FindBin(bins[j]+5)) << std::endl;
      data_sub_2D->Fill(bin, bins[j]+5, data_sub->GetBinContent(data_sub->GetXaxis()->FindBin(bins[j]+5))); 
    }
    for(int j=0; j<((sizeof(bins)/sizeof(*bins))-1); j++){
      int bin = int(stoi(mvis[i]) + (stoi(mvis[i+1])-stoi(mvis[i]))/2);
      std::cout << "Filling x bin: " << bin << ", and y bin: " << bins[j]+5 << ", with bin content: " << ZLL->GetBinContent(ZLL->GetXaxis()->FindBin(bins[j]+5)) << std::endl;
      ZLL_2D->Fill(bin, bins[j]+5, ZLL->GetBinContent(ZLL->GetXaxis()->FindBin(bins[j]+5))); 
    }
    for(int j=0; j<((sizeof(bins)/sizeof(*bins))-1); j++){
      double value = data_sub->GetBinContent(data_sub->GetXaxis()->FindBin(bins[j]+5));
      int bin = int(stoi(mvis[i]) + (stoi(mvis[i+1])-stoi(mvis[i]))/2);
      double err = value*sqrt( (pow((data->GetBinError(data->GetXaxis()->FindBin(bins[j]+5))/data->GetBinContent(data->GetXaxis()->FindBin(bins[j]+5))),2) + pow((total_noDY->GetBinError(total_noDY->GetXaxis()->FindBin(bins[j]+5))/total_noDY->GetBinContent(total_noDY->GetXaxis()->FindBin(bins[j]+5))),2) + pow((ZLL->GetBinError(ZLL->GetXaxis()->FindBin(bins[j]+5))/ZLL->GetBinContent(ZLL->GetXaxis()->FindBin(bins[j]+5))),2) )   )/ZLL->GetBinContent(ZLL->GetXaxis()->FindBin(bins[j]+5));
      std::cout << "Filling x bin: " << bin << ", and y bin: " << bins[j]+5 << ", scale factor: " << value/ZLL->GetBinContent(ZLL->GetXaxis()->FindBin(bins[j]+5)) << ", and error: " << err << std::endl;
     
      zptmass_histo_err->Fill(bin, bins[j]+5, err); 
    }

}

TFile *fileout = new TFile("zpt_weights.root","RECREATE");

data_sub_2D->Scale(1/data_sub_2D->Integral());
ZLL_2D->Scale(1/ZLL_2D->Integral());

zptmass_histo = (TH2D*)data_sub_2D->Clone("zptmass_histo");
zptmass_histo->Divide(ZLL_2D);
 
for (unsigned i=1;i<=8;++i){
   for(unsigned j=1;j<=12;++j){
     if(zptmass_histo->GetBinContent(i,j) < 0.1){
       zptmass_histo->SetBinContent(i,j,1);
     }
   }
 }
 
zptmass_histo->Write();
zptmass_histo_err->Write("zptmass_histo_err");
TCanvas canvas("canvas");
gStyle->SetPalette(1,0);
canvas.SetLogx();
zptmass_histo->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
zptmass_histo->GetXaxis()->SetTitleColor(1);
zptmass_histo->GetYaxis()->SetTitle("Z p_{T} (GeV)");
zptmass_histo->GetXaxis()->SetRangeUser(50,800);
zptmass_histo->GetYaxis()->SetRangeUser(0,600);
zptmass_histo->SetStats(0);
zptmass_histo->SetTitle(0);
zptmass_histo->Draw("colztext");
canvas.SaveAs("Zweights.pdf");
canvas.SaveAs("Zweights.png");

TCanvas canvas2("canvas2");
gStyle->SetPalette(1,0);
canvas2.SetLogx();
zptmass_histo_err->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
zptmass_histo_err->GetXaxis()->SetTitleColor(1);
zptmass_histo_err->GetYaxis()->SetTitle("Z p_{T} (GeV)");
zptmass_histo_err->GetXaxis()->SetRangeUser(50,800);
zptmass_histo_err->GetYaxis()->SetRangeUser(0,600);
zptmass_histo_err->SetStats(0);
zptmass_histo_err->SetTitle(0);
zptmass_histo_err->Draw("colztext");
canvas2.SaveAs("Zweightserr.pdf");
canvas2.SaveAs("Zweightserr.png");


fileout->Close();

return 0;


}
