//this code is a hack to plot the scale factor for the trigger as a function of Nvtx
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
        output=new TFile("ElectronTriggerBinnedPlots.root", "RECREATE");
    }
    else output = new TFile("MuonTriggerBinnedPlots.root", "RECREATE");
    output->cd();

    TGraphErrors* grvtxdata = (TGraphErrors*)fileData->Get("Graph4");
    TGraphErrors* grvtxMC = (TGraphErrors*)fileMC->Get("Graph4");

    
    TCanvas* canvas4 = new TCanvas("canvas4", "canvas4", 200,10, 700, 500);
    TH1F* base4 = new TH1F("base4", "base4" , 100,  0 ,100);
    base4->GetXaxis()->SetTitle("Nvtx");
    base4->GetYaxis()->SetTitle("Data/MC Scale Factor");
    base4->SetTitle(0);
    base4->SetStats(0);
  

    double x1, y1, x2, y2, err_x1, err_y1, err_x2, err_y2;
  
    double sf[10];
    double err_sf[10];
    double x[10];
    double err_x[10];

    for(int i=0; i<3; i++)
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
    TGraphErrors *grsf = new TGraphErrors(3,x,sf,err_x,err_sf);
    grsf->SetMarkerColor(kRed);
    grsf->SetMarkerStyle(20);
   
    base4->Draw();
    grsf->Draw("Psame");
    canvas4->Update();
    canvas4->Write();

    output->Close();


    return 0;
}


