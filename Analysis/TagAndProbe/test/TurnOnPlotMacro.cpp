#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TGraphErrors.h"
//#include "triggerEff_mine.C"
#include "TF1.h"
#include "TMath.h"
#include "TLegend.h"
#include "boost/lexical_cast.hpp"
#include <string>
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"

double efficiency(double m, double m0, double sigma, double alpha, double n, double norm)  
 { 
   const double sqrtPiOver2 = 1.2533141373;
   const double sqrt2 = 1.4142135624;

   double sig = fabs((double) sigma);
   
   double t = (m - m0)/sig ;
   
   if (alpha < 0)
     t = -t;

   double absAlpha = fabs(alpha / sig);
   double a = TMath::Power(n/absAlpha,n)*exp(-0.5*absAlpha*absAlpha);
   double b = absAlpha - n/absAlpha;

   //   if (a>=std::numeric_limits<double>::max()) return -1. ;

   double ApproxErf ;
   double arg = absAlpha / sqrt2 ;
   if (arg > 5.) ApproxErf = 1 ;
   else if (arg < -5.) ApproxErf = -1 ;
   else ApproxErf = TMath::Erf(arg) ;

   double leftArea = (1 + ApproxErf) * sqrtPiOver2 ;
   double rightArea = ( a * 1/TMath::Power(absAlpha - b,n-1)) / (n - 1);
   double area = leftArea + rightArea;

   if ( t <= absAlpha ){
     arg = t / sqrt2 ;
     if (arg > 5.) ApproxErf = 1 ;
     else if (arg < -5.) ApproxErf = -1 ;
     else ApproxErf = TMath::Erf(arg) ;
     return norm * (1 + ApproxErf) * sqrtPiOver2 / area ;
   }
   else{
     return norm * (leftArea +  a * (1/TMath::Power(t-b,n-1) - 1/TMath::Power(absAlpha - b,n-1)) / (1 - n)) / area ;
   }
  
 }

Double_t eff(Double_t *x, Double_t *par) {
  Double_t xx = x[0];
  Double_t m0 = par[0];
  Double_t sigma = par[1];
  Double_t alpha = par[2];
  Double_t n = par[3];
  Double_t norm = par[4];
 
  return efficiency(xx,m0,sigma,alpha,n,norm);
}

int main(int argc, char* argv[]){

  // Give argument info to user
  std::cout << "Arguments passed to program: " << argc << std::endl;
  for (int i = 0; i < argc; ++i){
    std::cout << i << "\t" << argv[i] << std::endl;
  }
  if (argc != 8){
    std::cerr << "Need 2 args: <filepath> <B/E/Eb> <par1> <par2> <par3> <par4> <par5>" << std::endl;
    exit(1);
  }

    ic::Plot::SetTdrStyle();
    std::string barrel=boost::lexical_cast<std::string>(argv[2]);

    TFile* file2=new TFile(argv[1]);

    TFile* output=new TFile("output.root", "RECREATE");
    output->cd();

    TF1 *myfit;
    
    if(barrel=="B")
    {
        myfit= (TF1*)file2->Get("fitGraph1");
    }
    if(barrel=="E")
    {
        myfit= (TF1*)file2->Get("fitGraph2");
    }
    if(barrel=="Eb")
    {
        myfit= (TF1*)file2->Get("fitGraph3");
    }

    //myfit->SetLineColor(kBlue);

    TF1* testfit;

    testfit = new TF1("testfit", eff, 10., 80., 5);
    double p1, p2, p3, p4, p5;
    p1=boost::lexical_cast<double>(argv[3]);
    p2=boost::lexical_cast<double>(argv[4]);
    p3=boost::lexical_cast<double>(argv[5]);
    p4=boost::lexical_cast<double>(argv[6]);
    p5=boost::lexical_cast<double>(argv[7]);
    testfit->SetParameters(p1, p2, p3, p4, p5);

    TCanvas* canvas = new TCanvas("canvas", "canvas", 200,10, 700, 500);
    TH1F* base = new TH1F("base", "base" , 100, 0 ,80);
    TGraphErrors* gr = (TGraphErrors*)file2->Get("Graph");
   
    base->GetXaxis()->SetTitle("p_{T} GeV");
    base->GetYaxis()->SetTitle("Efficiency");
    base->SetTitle(0);
    base->SetStats(0);
    base->Draw();
    testfit->SetLineColor(kRed);
    testfit->Draw("same");
    
    gr->SetMarkerColor(1);
    gr->Draw("Psame");

    myfit->SetLineColor(kGreen+1);
    myfit->Draw("same");
 
    TLegend * legend = new TLegend(0.6, 0.45, 0.75, 0.60);
    legend->AddEntry(testfit, "Test fit", "l");
    legend->AddEntry(myfit, "My fit", "l");
    legend->AddEntry(gr, "Data", "p");
    legend->Draw();
    canvas->Update();
    
    canvas->Write();
    output->Close();


    return 0;
}


