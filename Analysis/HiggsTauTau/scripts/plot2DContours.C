#include "TMath.h"
#include "TFile.h"
#include "TH2F.h"
#include "TROOT.h"

void plot_bands() {
  TFile f("");
  TH2F *hist = (TH2F*)gDirectory->Get("");
  double contours[4];
  contours[0] = TMath::ChisquareQuantile(0.68,2)/2; //0.5;     //68% CL
  contours[1] = TMath::ChisquareQuantile(0.95,2)/2; //1.92;    //95% CL
  contours[2] = TMath::ChisquareQuantile(0.997,2)/2; //1.92;    //95% CL
  contours[3] = TMath::ChisquareQuantile(0.99993,2)/2; //1.92;    //95% CL
  hist->SetContour(4, contours);
  TCanvas canv;
  hist->Draw("CONT2");  // draw contours as filled regions, and save points
  canv.SaveAs(":
}
