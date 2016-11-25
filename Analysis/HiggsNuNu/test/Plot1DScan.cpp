#include <vector>
#include <string>
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLine.h"
#include "TLegend.h"
#include "TString.h"
#include "TLatex.h"

#include "Core/interface/Plot.h"

    void DrawCMSLogoTest(TPad* pad, TString cmsText, TString extraText, int iPosX,
		   float relPosX, float relPosY, float relExtraDY) {
    TVirtualPad *pad_backup = gPad;
    pad->cd();
    float cmsTextFont = 61;  // default is helvetic-bold

    bool writeExtraText = extraText.Length() > 0;
    float extraTextFont = 52;  // default is helvetica-italics

    // text sizes and text offsets with respect to the top frame
    // in unit of the top margin size
    TString lumiText;
    float lumiTextOffset = 0.2;
    float cmsTextSize = 0.8;
    float lumiTextSize = 0.6;
    // float cmsTextOffset    = 0.1;  // only used in outOfFrame version

    // ratio of "CMS" and extra text size
    float extraOverCmsTextSize = 0.76;

    //!!MAKE CHOICE CONFIGURABLE
    TString lumi_13TeV = "20.1 fb^{-1}";
    TString lumi_8TeV = "18.9-19.7 fb^{-1}";
    TString lumi_7TeV = "0-4.9 fb^{-1}";

    lumiText +=lumi_8TeV;
    lumiText +=" (8 TeV) + ";
    lumiText +=lumi_7TeV;
    lumiText +=" (7 TeV)";



    bool outOfFrame = false;
    if (iPosX / 10 == 0) {
      outOfFrame = true;
    }
    int alignY_ = 3;
    int alignX_ = 2;
    if (iPosX / 10 == 0) alignX_ = 1;
    if (iPosX == 0) alignX_ = 1;
    if (iPosX == 0) alignY_ = 1;
    if (iPosX / 10 == 1) alignX_ = 1;
    if (iPosX / 10 == 2) alignX_ = 2;
    if (iPosX / 10 == 3) alignX_ = 3;
    if (iPosX == 0) relPosX = 0.14;
    int align_ = 10 * alignX_ + alignY_;

    float l = pad->GetLeftMargin();
    float t = pad->GetTopMargin();
    float r = pad->GetRightMargin();
    float b = pad->GetBottomMargin();

    TLatex latex;
    latex.SetNDC();
    latex.SetTextAngle(0);
    latex.SetTextColor(kBlack);

    float extraTextSize = extraOverCmsTextSize * cmsTextSize;
    float pad_ratio = (static_cast<float>(pad->GetWh()) * pad->GetAbsHNDC()) /
      (static_cast<float>(pad->GetWw()) * pad->GetAbsWNDC());
    if (pad_ratio < 1.) pad_ratio = 1.;

    latex.SetTextFont(42);
    latex.SetTextAlign(31); 
    latex.SetTextSize(lumiTextSize*t*pad_ratio);    
    latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);

    if (outOfFrame) {
      latex.SetTextFont(cmsTextFont);
      latex.SetTextAlign(11);
      latex.SetTextSize(cmsTextSize * t * pad_ratio);
      latex.DrawLatex(l, 1 - t + lumiTextOffset * t, cmsText);
    }


    float posX_ = 0;
    if (iPosX % 10 <= 1) {
      posX_ = l + relPosX * (1 - l - r);
    } else if (iPosX % 10 == 2) {
      posX_ = l + 0.5 * (1 - l - r);
    } else if (iPosX % 10 == 3) {
      posX_ = 1 - r - relPosX * (1 - l - r);
    }
    float posY_ = 1 - t - relPosY * (1 - t - b);
    if (!outOfFrame) {
      latex.SetTextFont(cmsTextFont);
      latex.SetTextSize(cmsTextSize * t * pad_ratio);
      latex.SetTextAlign(align_);
      latex.DrawLatex(posX_, posY_, cmsText);
      if (writeExtraText) {
	latex.SetTextFont(extraTextFont);
	latex.SetTextAlign(align_);
	latex.SetTextSize(extraTextSize * t * pad_ratio);
	latex.DrawLatex(posX_, posY_ - relExtraDY * cmsTextSize * t, extraText);
      }
    } else if (writeExtraText) {
      if (iPosX == 0) {
	posX_ = l + relPosX * (1 - l - r);
	posY_ = 1 - t + lumiTextOffset * t;
      }
      latex.SetTextFont(extraTextFont);
      latex.SetTextSize(extraTextSize * t * pad_ratio);
      latex.SetTextAlign(align_);
      latex.DrawLatex(posX_, posY_, extraText);
    }
    pad_backup->cd();
  }

  void DrawCMSLogoTest(TPad* pad, TString cmsText, TString extraText, int iPosX) {
    DrawCMSLogoTest(pad, cmsText, extraText, iPosX, 0.045, 0.035, 1.2);
  }

TGraph ExtractGraph(TTree *t, double & bestFit) {
  TGraph g(t->GetEntries());
  float r = 0;
  float deltaNLL = 0;
  t->SetBranchAddress("r", &r);
  t->SetBranchAddress("deltaNLL", &deltaNLL);
  for (unsigned i = 0; i < t->GetEntries(); ++i) {
    t->GetEntry(i);
    if (deltaNLL == 0) bestFit = r;
    g.SetPoint(i, r, 2.0*deltaNLL);
  }
  g.Sort();
  return g;
}

std::vector<double> GetCrossings(TGraph const& g, double cross) {
  std::vector<double> result;
  unsigned n = g.GetN();
  double x1 = 0;
  double y1 = 0;
  double x2 = 0;
  double y2 = 0;
  for (unsigned i = 0; i < (n-1); ++i) {
    g.GetPoint(i, x1, y1);
    g.GetPoint(i+1, x2, y2);
    if ( (y2-cross)*(y1-cross) < 0.0 ) {
      double m = (y2-y1)/(x2-x1);
      double c = (y1 - m * x1);
      double xc = (cross - c) / m;
      result.push_back(xc);
      std::cout << "Crossing at " << xc << std::endl;
    }
  }
  return result;
}

struct Scan {
  std::string file;
  std::string label;
  int color;
  TGraph *gr;
};

int main() {
  ic::Plot::SetHTTStyle();

  std::string dir="/vols/cms04/pjd12/invcmssws/CMSSW_7_1_5/src/HiggsAnalysis/CombinedLimit/exocombcards/";

  std::vector<Scan> scans;
  //  scans.push_back({"higgsCombinefullScan.MultiDimFit.mH125.root", "Observed", 1, nullptr});
  //  scans.push_back({"higgsCombineexpected.MultiDimFit.mH125.root", "Exp. for SM H", 32, nullptr});
//   scans.push_back({"higgsCombinenoBBBScan.MultiDimFit.mH125.root", "no bbb syst.", 38, nullptr});
  scans.push_back({dir+"higgsCombineCombExp.MultiDimFit.mH125.root", "Exp. for SM H", 1, nullptr});
  scans.push_back({dir+"higgsCombineCombObs.MultiDimFit.mH125.root", "Obs. for SM H", 1, nullptr});

  TCanvas c1("canvas","canvas");

  std::vector<TLine *> lines;


  TLegend *leg = new TLegend(0.65,0.75,0.9,0.9,"","brNDC");

  unsigned counter = 0;
  for (auto & sc : scans) {
    TFile f1(sc.file.c_str());
    TTree *t1 = dynamic_cast<TTree*>(f1.Get("limit"));
    double best1 = 0.0;
    TString res;
    sc.gr = new TGraph(ExtractGraph(t1, best1));
    if(counter==1){
      auto x1 = GetCrossings(*(sc.gr), 1.0);
      auto x2 = GetCrossings(*(sc.gr), 3.84);
      lines.push_back(new TLine(x1[0],0,x1[0],1.0));
      lines.back()->SetLineColor(2);
      lines.back()->SetLineWidth(2);
      lines.push_back(new TLine(x2[0],0,x2[0],3.84));
      lines.back()->SetLineColor(2);
      lines.back()->SetLineWidth(2);
    }
    sc.gr->SetLineColor(sc.color);
    sc.gr->SetLineWidth(3);
    sc.gr->Draw(counter ? "LSAME" : "AL");
    TString leg_text = "#splitline{"+sc.label+"}{"+res+"}";
    leg->AddEntry(sc.gr, leg_text, "L");
    counter++;
  }
  // c1.cd();
  // // g1.Print();
  // g1.SetLineColor(1);
  // g1.SetLineWidth(2);
  // // g1.SetMarkerColor(7);
  // g1.Draw("AC");
  scans[0].gr->SetMaximum(9);
  scans[0].gr->SetMinimum(0.);
  scans[0].gr->GetXaxis()->SetRangeUser(0., 0.9);
  scans[0].gr->GetXaxis()->SetTitle("BR_{inv}");
  scans[0].gr->GetXaxis()->SetTitleOffset(1.1);
  scans[0].gr->GetXaxis()->SetNdivisions(1005,true);
  scans[0].gr->GetXaxis()->SetLabelSize(0.05);
  scans[0].gr->GetYaxis()->SetLabelSize(0.05);
  scans[0].gr->GetXaxis()->SetLabelOffset(0.02);
  scans[0].gr->GetYaxis()->SetLabelOffset(0.02);
  scans[0].gr->GetYaxis()->SetTitle("-2 #Delta ln L");
  scans[0].gr->SetLineStyle(2);
  scans[0].gr->SetLineColor(1);
  leg->SetBorderSize(1);
  leg->SetTextFont(42);
  leg->SetTextSize(0.03);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  leg->Draw();
  lines.push_back(new TLine(0.,1,0.9,1.0));
  lines.back()->SetLineColor(2);
  lines.push_back(new TLine(0.,3.84,0.9,3.84));
  lines.back()->SetLineColor(2);
  //  for (auto l : lines) l->Draw();

  DrawCMSLogoTest(&c1,"CMS","preliminary",10);

  TLatex lat = TLatex();
  lat.SetNDC();
  lat.SetTextSize(0.04);                                                                                                                                    
  lat.SetTextFont(42);

  TLatex lat2 = TLatex();
  lat2.SetNDC();
  lat2.SetTextSize(0.03);
  lat2.SetTextFont(42);

  lat.DrawLatex(0.2,0.73,"Combination of all");
  lat.DrawLatex(0.2,0.68,"H#rightarrow inv. channels");


  c1.Update();
  c1.SaveAs("scan.pdf");
  return 0;
}
