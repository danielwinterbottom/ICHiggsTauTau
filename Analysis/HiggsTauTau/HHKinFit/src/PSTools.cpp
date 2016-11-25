#include "../include/PSTools.h"

// #include <RooEllipse.h>
#include <Rtypes.h>
#include <TH1.h>
#include <TLatex.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TVirtualPad.h>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string>

//  Tools by Peter Schleper

PSTools::PSTools(int size)
{
  gROOT->Reset();
  Pcanvas(size);
  Pstyle();
  //  Platex (0.3, 0.7, 0.05); 
}

void
PSTools::ListHistos()
{        // List histograms in memory (-m) or on disk (-d)
  gDirectory->ls("-m");
}

TH1F*
PSTools::HistPointer(const char* histname = "") // Get pointer from name
{
  TH1F *myHist = (TH1F*) gDirectory->Get(histname);
  std::cout << histname << std::endl;
  std::cout << myHist << std::endl;
  return myHist;
}

void
PSTools::wait(TPad* c1)  // wait during execution of script until key it hit
{
  std::cout << " wait: To continue  activate pad window and Hit a key "
      << std::endl;
  c1->Update();
  c1->WaitPrimitive();
  //gets(s);
  return;
}

void
PSTools::next(TPad* c1)
{       // select next pad on canvas for c1->Divide(nx,ny)
  char *s = new char[1];
  Int_t oldpad = gPad->GetNumber();
  if (oldpad < 1)             // no previous sub-pad defined
      {
    std::cout << "next: oldpad = " << oldpad << std::endl;
    wait(c1);
    c1->Clear("D");
    c1->cd(1);
  }
  else {
    c1->cd(oldpad + 1);
    Int_t newpad = gPad->GetNumber();
    if (newpad == oldpad)   // final pad exceeded
        {
      std::cout << "next: newpad = " << oldpad << std::endl;
      wait(c1);
      c1->Clear("D");
      c1->cd(1);
    }
  };
  Int_t newpad = gPad->GetNumber();
  std::cout << "next:  oldpad = " << oldpad << "  newpad = " << newpad
      << std::endl;
  //  std::cout << "next = " << gPad->GetNumber() << std::endl;
  return;
}

TCanvas*
PSTools::Pcanvas(int flag)
{
  TCanvas *c1;
  // Create a new canvas.
  if (flag == 0)
    c1 = new TCanvas("c1", "Canvas c1", 1280, 600);          // im Buero
  if (flag == 1)
    c1 = new TCanvas("c1", "Canvas c1", 0, 0, 1150, 850);    // zu Hause
  if (flag == 2)
    c1 = new TCanvas("c1", "Canvas c1", 380, 0, 900, 500);    // Laptop
  //  c1->SetFillColor(42);
  //  c1->GetFrame()->SetFillColor(11);
  //  c1->GetFrame()->SetBorderSize(6);
  //  c1->GetFrame()->SetBorderMode(-1);
  c1->Divide(2, 2);
  return c1;
}

void
PSTools::Platex(Float_t x, Float_t y, Float_t size)
{
  std::cout << "Platex" << std::endl;
  TLatex *t = new TLatex();
  t->SetTextFont(1);
  t->SetTextColor(1);
  t->SetTextSize(size);
  t->SetTextAlign(11);
  t->DrawLatex(x, y, "M.Tokarev, E.Potrebenikova ");
}

void
PSTools::Pstyle()
{
  gPad->SetTickx(1);
  gPad->SetTicky(1);
}

void
PSTools::Pprint(const char* filename, TCanvas* c1)
{
  TString text;
  if (filename == "")
    filename = "temp";
  text = filename;
  text = text + ".pdf";
  //  std::cout << "Pprint " << text << std::endl;
  c1->Print(text, "pdf");
}

void
PSTools::Pcolor(int icol)
{
  // set all objects to the same colour 
  // canvas, pads, etc.,  are unchanged

  gStyle->SetFillColor(icol);
  gStyle->SetTextColor(icol);
  gStyle->SetMarkerColor(icol);
  gStyle->SetLineColor(icol);
  gStyle->SetHistFillColor(icol);
  gStyle->SetHistLineColor(icol);
  gStyle->SetFuncColor(icol);
  gStyle->SetLegendFillColor(icol);
  gStyle->SetLabelColor(icol);

  //SetColorModelPS
  //SetAxisColor
  //SetCanvasColor
  //SetGridColor
  //SetPadColor
  //SetFrameFillColor
  //SetFrameLineColor
  //SetTitleColor
  //SetTitleFillColor
  //SetTitleTextColor
  //SetStatColor
  //SetStatTextColor
}

void
PSTools::coutf(int w, TString s)
{    //  tools for formated std::cout
  std::string s1 = std::string(s.Data());
  coutf(w, s1);
}

void
PSTools::coutf(int w, int i)
{
  std::cout << std::setw(w)   << i ;
}

//void std::coutf(int w,string s) {std::cout << std::setw(w) << s ;}

void
PSTools::coutf(int w, std::string s)
{    // emulate "left" (bug in ROOT?
  int l = s.length();
  const std::string blank = "                       ";
  if (l <= w) {
    std::cout << s.substr(0, l);
    std::cout << blank.substr(0, w - l);
  }
  else {
    std::cout << s.substr(0, w);
  }
}

void
PSTools::coutf(int w, double x)
{
  std::cout << std::setw(w) << x;
}

void
PSTools::coutf(int w, int p, double x)
{
  printf("%*.*f ", w, p, x);
  //std::cout << std::setiosflags(std::ios::fixed) << std::setw(w) << std::setprecision(p) << x ; 
  //std::cout << std::setprecision(6) << std::resetiosflags(std::ios::fixed); 
}

void
PSTools::PSDrawErrorEllipse(Double_t x, Double_t y, Double_t sx, Double_t sy,
                            Double_t rho, Int_t nsigma)
{
//   //  TH2F  * h2 = new TH2F("","",20,-10,10,20,-10,10);
//   //  h2->Draw();
//   const Int_t ncolor = 5;
//   const Int_t base = kGray;
//   //  const Int_t base   = kBlue ;
//   Int_t color[ncolor] = { base + 1, base - 4, base - 7, base - 9, base - 10 };
//   RooEllipse *E1 = new RooEllipse("PTmiss", x, y, 1. * sx, 1. * sy, rho, 100);
//   RooEllipse *E2 = new RooEllipse("PTmiss", x, y, 2. * sx, 2. * sy, rho, 100);
//   RooEllipse *E3 = new RooEllipse("PTmiss", x, y, 3. * sx, 3. * sy, rho, 100);
// 
//   if (nsigma > 2) {
//     E3->SetFillColor(color[3]);
//     E3->Draw("F");
//     E3->SetLineColor(color[0]);
//     E3->Draw();
//   }
//   if (nsigma > 1) {
//     E2->SetFillColor(color[2]);
//     E2->Draw("F");
//     E2->SetLineColor(color[0]);
//     E2->Draw();
//   }
//   E1->SetFillColor(color[1]);
//   E1->Draw("F");
//   E1->SetLineColor(color[0]);
//   E1->Draw();

}

//Double_t PStools::fmax(Double_t a, Double_t b) {
//  if (a<b) return b;
//  else return a;
//}
//
//Double_t PStools::fmin(Double_t a, Double_t b) {
//  if (a>b) return b;
//  else return a;
//}
//
//void PStools::fsortup(Double_t* a, Double_t* b) {  // sort upward
//  if (a<b) return ;
//  else {
//    Double_t temp;
//    temp=a;
//    a=b;
//    b=temp;
//    return ;
//  }
//}
