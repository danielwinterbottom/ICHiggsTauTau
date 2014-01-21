//Code to produce some plots overlaying turn on curves for the AN. There is some attempt to copy the style used previously by Joshua.

#include "TFile.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TH1.h"
#include "TGraphErrors.h"
//#include "triggerEff_mine.C"
#include "TF1.h"
#include "TMath.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TROOT.h"
#include "boost/lexical_cast.hpp"
#include <string>


int main(int argc, char* argv[]){

  // Give argument info to user
  std::cout << "Arguments passed to program: " << argc << std::endl;
  for (int i = 0; i < argc; ++i){
    std::cout << i << "\t" << argv[i] << std::endl;
  }
  if (argc != 10){
    std::cerr << "Need 8 args:<iselec> <B/E/Eb> <File1><label1> <File2><labelData2> <File3><label3> <Plot1or2or3>" << std::endl;
    exit(1);
  }

/*
    gStyle->SetLabelColor(1, "XYZ");
    gStyle->SetLabelFont(42, "XYZ");
    gStyle->SetLabelOffset(0.007, "XYZ");
    gStyle->SetLabelSize(0.04, "XYZ");
  // For the Pad:
    gStyle->SetPadBorderMode(0);
    // gStyle->SetPadBorderSize(Width_t size = 1);
    gStyle->SetPadColor(kWhite);
    gStyle->SetPadGridX(false);
    gStyle->SetPadGridY(false);
    gStyle->SetGridColor(0);
    gStyle->SetGridStyle(3);
   gStyle->SetGridWidth(1);

                                      // For the frame:
      gStyle->SetFrameBorderMode(0);
     gStyle->SetFrameBorderSize(10);
     gStyle->SetFrameFillColor(0);    
 
       gROOT->ForceStyle();
*/

    bool elec;
    std::string barrel;
    elec = boost::lexical_cast<bool>(argv[1]);
    barrel = argv[2];
    TFile* file2012A=new TFile(argv[3]);
    const char* labeldata1=argv[4];
    TFile* file2012B=new TFile(argv[5]);
    const char* labeldata2=argv[6];
    TFile* fileMC=new TFile(argv[7]);
    const char* labelMC=argv[8];
    int Plot2or3 = boost::lexical_cast<int>(argv[9]);

    TFile* output=NULL;
    if(elec && barrel=="B")
    {
        output=new TFile("ElectronBarrelTriggerPlots.root", "RECREATE");
    }
    if(elec && barrel=="E")
    {
        output=new TFile("ElectronEndcapTriggerPlots.root", "RECREATE");
    }
    if(!elec && barrel=="B")
    {
        output=new TFile("MuonEta08TriggerPlots.root", "RECREATE");
    }
    if(!elec && barrel=="E")
    {
        output=new TFile("MuonEta12TriggerPlots.root", "RECREATE");
    }
    if(!elec && barrel=="Eb")
    {
        output=new TFile("MuonEndcapTriggerPlots.root", "RECREATE");
    }
    
    output->cd();

    TGraphErrors* gr2012A=(TGraphErrors*)file2012A->Get("Graph");
    TGraphErrors* gr2012B = (TGraphErrors*)file2012B->Get("Graph");
    TGraphErrors* grMC = (TGraphErrors*)fileMC->Get("Graph");
    TF1* fit2012A;
    TF1* fit2012B;
    TF1* fitMC;
    
    
    if(barrel=="B")
    {
        fit2012A=(TF1*)file2012A->Get("fitGraph1");
        fit2012B = (TF1*)file2012B->Get("fitGraph1");
        fitMC = (TF1*)fileMC->Get("fitGraph1");
    }
    else if(barrel=="E")
    {
        fit2012A=(TF1*)file2012A->Get("fitGraph2");
        fit2012B = (TF1*)file2012B->Get("fitGraph2");
        fitMC = (TF1*)fileMC->Get("fitGraph2");
    }
    else
    {
        fit2012A=(TF1*)file2012A->Get("fitGraph3");
        fit2012B = (TF1*)file2012B->Get("fitGraph3");
        fitMC = (TF1*)fileMC->Get("fitGraph3");
    }
    
    TCanvas* canvas1 = new TCanvas("canvas1", "canvas1", 200,10, 700, 500);
    TH1F* base1 = new TH1F("base1", "base1" , 100, 0 ,50);
    if(elec)
    {    
        base1->GetXaxis()->SetTitle("Electron p_{T} (GeV)");
    }
    else base1->GetXaxis()->SetTitle("Muon p_{T} (GeV)");
    
    
    base1->GetXaxis()->SetLabelSize(0.045);
    base1->GetXaxis()->SetTitleSize(0.045);
    base1->GetXaxis()->SetTitleOffset(1.1);
    
    base1->GetYaxis()->SetTitle("Efficiency");
    base1->GetYaxis()->SetLabelSize(0.045);
    base1->GetYaxis()->SetTitleSize(0.045);
    base1->GetYaxis()->SetTitleOffset(1.0);
    base1->SetTitle(0);
    base1->SetStats(0);
    base1->Draw();
    gr2012A->SetMarkerColor(1);
    fit2012A->SetLineColor(1);
    if(Plot2or3==1)
    {
        gr2012A->SetMarkerColor(kBlue);
        fit2012A->SetLineColor(kBlue);
    }
    gr2012A->Draw("Psame");
    fit2012A->Draw("same");
    if(Plot2or3!=1)
    {
        gr2012B->SetMarkerColor(kBlue);
        gr2012B->SetMarkerStyle(33);
        fit2012B->SetLineColor(4);
        //fit2012B->SetLineWidth(1);
        gr2012B->Draw("Psame");
        fit2012B->Draw("same");
    }
   if(Plot2or3==3)
   {
        grMC->SetMarkerColor(kRed);
        grMC->SetMarkerStyle(21);
        fitMC->SetLineColor(kRed);
        //fitMC->SetLineWidth(1);
        grMC->Draw("Psame");
        fitMC->Draw("same");
   }
    
    TLegend * legend1 = new TLegend(0.50, 0.35, 0.75, 0.60);
    legend1->AddEntry(gr2012A, labeldata1, "p");
    if(Plot2or3!=1) legend1->AddEntry(gr2012B, labeldata2, "p");
 if(Plot2or3==3)  legend1->AddEntry(grMC, labelMC, "p");
//    legend1->AddEntry(grptdata, "2012 Data", "p");
    legend1->SetFillColor(0);
    legend1->SetTextSize(0.045);
    legend1->SetBorderSize(0);
    legend1->Draw();
    canvas1->Update();

    TLatex *title_latex = new TLatex();
    title_latex->SetNDC();
    title_latex->SetTextSize(0.045);
    title_latex->DrawLatex(0.14, 0.935, "CMS Preliminary 2012, #sqrt{s}=8 TeV, 19.4 fb^{-1}");        
    TLatex *label_latex = new TLatex();
    label_latex->SetNDC();
    label_latex->SetTextSize(0.045);
    if(barrel=="B" && !elec)
    {
        label_latex->DrawLatex(0.14, 0.8, "|#eta|<0.8");
        //label_latex->DrawLatex(0.14, 0.8, "0 < #eta < 0.8");
     //   label_latex->DrawLatex(0.14, 0.8, "-0.8 < #eta < 0");
    }
    if(barrel=="B" && elec)
    {
        label_latex->DrawLatex(0.14, 0.8, "Barrel");
    }
    if(barrel=="E" && !elec)
    {
        label_latex->DrawLatex(0.14, 0.8, "0.8<|#eta|<1.2");
        //label_latex->DrawLatex(0.14, 0.8, "0.8 < #eta < 1.2");
        //label_latex->DrawLatex(0.14, 0.8, "-1.2 < #eta < -0.8");
    }
    if(barrel=="E" && elec)
    {
        label_latex->DrawLatex(0.14, 0.8, "Endcap");
    }
    if(barrel=="Eb")
    {
        label_latex->DrawLatex(0.14, 0.8, "|#eta|>1.2");
        //label_latex->DrawLatex(0.14, 0.8, "#eta > 1.2");
        //label_latex->DrawLatex(0.14, 0.8, "#eta < -1.2");
    }
    
    
    canvas1->Update();

    canvas1->Write();    
    
    output->Close();


    return 0;
}


