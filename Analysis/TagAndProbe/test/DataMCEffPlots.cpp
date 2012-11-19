//code to plot data and MC efficiency plots in pt, eta and nvtx

#include "TFile.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TH1.h"
#include "TGraphErrors.h"
//#include "triggerEff_mine.C"
#include "TF1.h"
#include "TMath.h"
#include "TLegend.h"
#include "boost/lexical_cast.hpp"
#include <string>
#include <iostream>


int main(int argc, char* argv[]){

  // Give argument info to user
  std::cout << "Arguments passed to program: " << argc << std::endl;
  for (int i = 0; i < argc; ++i){
    std::cout << i << "\t" << argv[i] << std::endl;
  }
  if (argc != 4){
    std::cerr << "Need 2 args:<iselec> <filepathdata> <filepathMC>" << std::endl;
    exit(1);
  }

    bool elec;
    elec = boost::lexical_cast<bool>(argv[1]);
    TFile* fileData=new TFile(argv[2]);
    TFile* fileMC=new TFile(argv[3]);

    TFile* output;
    if(elec)
    {
        output=new TFile("ElectronBinnedPlots.root", "RECREATE");
    }
    else output = new TFile("MuonBinnedPlots.root", "RECREATE");
    output->cd();

    TGraphErrors* grptdata=(TGraphErrors*)fileData->Get("Graph1");
    TGraphErrors* gretadata = (TGraphErrors*)fileData->Get("Graph2");
    TGraphErrors* grvtxdata = (TGraphErrors*)fileData->Get("Graph3");
    TGraphErrors* grptMC=(TGraphErrors*)fileMC->Get("Graph1");
    TGraphErrors* gretaMC = (TGraphErrors*)fileMC->Get("Graph2");
    TGraphErrors* grvtxMC = (TGraphErrors*)fileMC->Get("Graph3");

    TCanvas* canvas1 = new TCanvas("canvas1", "canvas1", 200,10, 700, 500);
    TH1F* base1 = new TH1F("base1", "base1" , 100, 0 ,160);
    if(elec)
    {    
        base1->GetXaxis()->SetTitle("Electron p_{T} (GeV)");
    }
    else base1->GetXaxis()->SetTitle("Muon p_{T} (GeV)");
    base1->GetYaxis()->SetTitle("Efficiency");
    base1->SetTitle(0);
    base1->SetStats(0);
    base1->Draw();
    grptMC->SetMarkerColor(kRed);
    grptMC->SetLineColor(kRed);
    grptMC->Draw("Psame");
    grptdata->SetMarkerColor(1);
    grptdata->Draw("Psame");
    
    TLegend * legend1 = new TLegend(0.6, 0.45, 0.75, 0.60);
    legend1->AddEntry(grptMC, "2012 MC", "p");
    legend1->AddEntry(grptdata, "2012 Data", "p");
    legend1->SetFillColor(0);
    legend1->SetTextSize(0.04);
    legend1->SetBorderSize(0);
    legend1->Draw();
    canvas1->Update();

    TLatex *title_latex = new TLatex();
    title_latex->SetNDC();
    title_latex->SetTextSize(0.04);
    title_latex->DrawLatex(0.14, 0.935, "CMS Preliminary 2012, #sqrt{s}=8 TeV, 12 fb^{-1}");        
    canvas1->Update();

    canvas1->Write();    

    TCanvas* canvas2 = new TCanvas("canvas2", "canvas2", 200,10, 700, 500);
    TH1F* base2 = new TH1F("base2", "base2" , 100, -2.2 ,2.2);
    if(elec)
    {
        base2->GetXaxis()->SetTitle("Electron Eta");
    }
    else base2->GetXaxis()->SetTitle("Muon Eta");
    base2->GetYaxis()->SetTitle("Efficiency");
    base2->SetTitle(0);
    base2->SetStats(0);
    base2->Draw();
    gretaMC->SetMarkerColor(kRed);
    gretaMC->SetLineColor(kRed);
    gretaMC->Draw("Psame");
    gretadata->SetMarkerColor(1);
    gretadata->Draw("Psame");

    TLegend * legend2 = new TLegend(0.6, 0.35, 0.75, 0.50);
    legend2->AddEntry(gretaMC, "2012 MC", "p");
    legend2->AddEntry(gretadata, "2012 Data", "p");
    legend2->SetFillColor(0);
    legend2->SetTextSize(0.04);
    legend2->SetBorderSize(0);
    legend2->Draw();
    canvas2->Update();
    title_latex->DrawLatex(0.14, 0.935, "CMS Preliminary 2012, #sqrt{s}=8 TeV, 12 fb^{-1}");        
    canvas2->Update();
     
   
    canvas2->Write();
 
    TCanvas* canvas3 = new TCanvas("canvas3", "canvas3", 200,10, 700, 500);
    TH1F* base3 = new TH1F("base3", "base3" , 100,  0 ,30);
    if(elec)
    {
        base3->GetXaxis()->SetTitle("Nvtx");
    }
    else base3->GetXaxis()->SetTitle("Nvtx");
    base3->GetYaxis()->SetTitle("Efficiency");
    base3->SetTitle(0);
    base3->SetStats(0);
    base3->Draw();
    grvtxMC->SetMarkerColor(kRed);
    grvtxMC->SetLineColor(kRed);
    grvtxMC->Draw("Psame");
    grvtxdata->SetMarkerColor(1);
    grvtxdata->Draw("Psame");

    TLegend * legend3 = new TLegend(0.6, 0.35, 0.75, 0.50);
    legend3->AddEntry(grvtxMC, "2012 MC", "p");
    legend3->AddEntry(grvtxdata, "2012 Data", "p");
    legend3->SetFillColor(0);
    legend3->SetTextSize(0.04);
    legend3->SetBorderSize(0);
    legend3->Draw();
    canvas3->Update();
    title_latex->DrawLatex(0.14, 0.935, "CMS Preliminary 3012, #sqrt{s}=8 TeV, 12 fb^{-1}");        
    canvas3->Update();
     
   
    canvas3->Write();
    
    TCanvas* canvas4 = new TCanvas("canvas4", "canvas4", 200,10, 700, 500);
    TH1F* base4 = new TH1F("base4", "base4" , 100,  0 ,30);
    base4->GetXaxis()->SetTitle("Nvtx");
    base4->GetYaxis()->SetTitle("Data/MC Scale Factor");
    base4->SetTitle(0);
    base4->SetStats(0);
  

    double x1, y1, x2, y2, err_x1, err_y1, err_x2, err_y2;
 
  //Code to plot scale factor as function of Nvtx
    double sf[10];
    double err_sf[10];
    double x[10];
    double err_x[10];

    for(int i=0; i<10; i++)
    {
        grvtxdata->GetPoint(i, x1, y1);
        err_x1=grvtxdata->GetErrorX(i);
        err_y1=grvtxdata->GetErrorY(i);
        grvtxMC->GetPoint(i, x2, y2);
        err_x2=grvtxMC->GetErrorX(i);
        err_y2=grvtxMC->GetErrorY(i);
        std::cout << x1 << " " << x2 << " " << y1 << " " << y2 <<  " " << y1/y2 <<std::endl;
        x[i]=x1;
        err_x[i]=err_x1;
        sf[i]=y1/y2;
        err_sf[i]=sqrt((err_y1/y1)*(err_y1/y1) + (err_y2/y2)*(err_y2/y2));
    } 
    TGraphErrors *grsf = new TGraphErrors(10,x,sf,err_x,err_sf);
    grsf->SetMarkerColor(kRed);
    grsf->SetMarkerStyle(20);
   
    base4->Draw();
    grsf->Draw("Psame");
    canvas4->Update();
    canvas4->Write();

    output->Close();


    return 0;
}


