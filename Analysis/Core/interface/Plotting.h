#ifndef ICHiggsTauTau_Core_Plotting_h
#define ICHiggsTauTau_Core_Plotting_h

#include <map>
#include <string>
#include <iostream>
#include <vector>

#include "TStyle.h"
#include "TPad.h"
#include "TLatex.h"
#include "TAxis.h"
#include "TList.h"
#include "TH1F.h"

// tdrGrid: Turns the grid lines on (true) or off (false)


/*
See implementation of TPad::RedrawAxis for how to get hold of the histogram
that owns the axes:
http://root.cern.ch/root/html/src/TPad.cxx.html#E2wo0D
Would be useful to define a function like:

TH1 *GetAxisHisto(TPad *);

*/

void tdrGrid(bool gridOn) {
  gStyle->SetPadGridX(gridOn);
  gStyle->SetPadGridY(gridOn);
}

// fixOverlay: Redraws the axis

void fixOverlay() { gPad->RedrawAxis(); }



double GetPadYMax(TPad* pad) {
  TList *pad_obs = pad->GetListOfPrimitives();
  if (!pad_obs) return 0.;
  TIter next(pad_obs);
  double h_max = -99999;
  TObject *obj;
  while ((obj = next())) {
    if (obj->InheritsFrom(TH1::Class())) {
      TH1 *hobj = reinterpret_cast<TH1*>(obj);
      for (int j = 1; j < hobj->GetNbinsX(); ++j) {
        if (hobj->GetBinContent(j) + hobj->GetBinError(j) > h_max) {
          h_max = hobj->GetBinContent(j) + hobj->GetBinError(j);
        }
      }
    }
  }
  return h_max;
}

TH1* MakeRatioHist(TH1* num, TH1* den, bool num_err, bool den_err) {
  TH1* result = reinterpret_cast<TH1*>(num->Clone());
  if (!num_err) {
    for (int i = 1; i <= result->GetNbinsX(); ++i) result->SetBinError(i, 0.);
  }
  TH1* den_fix = reinterpret_cast<TH1*>(den->Clone());
  if (!den_err) {
    for (int i = 1; i <= den_fix->GetNbinsX(); ++i) den_fix->SetBinError(i, 0.);
  }
  result->Divide(den_fix);
  return result;
}



//----------------------------------------------------------------------------
// Functions for creating pad configuration
//----------------------------------------------------------------------------
// Creates two pads, split horizontally. The split will be at splitPoint, which
// is a fraction of the canvas that should be occupied by the lower pad. gapLow
// and gapHigh control the pad margins at the split point. This are specified as
// fractions of the input canvas height, not the resulting split pads. Note that
// the top margin of the upper pad and the bottom margin of the lower pad will
// be adjusted the maintain the same visual size as if there had only been one
// pad drawn on the canvas with the gStyle default.
std::vector<TPad*> TwoPadSplit(double splitPoint, double gapLow, double gapHigh) {
  TPad* upper = new TPad("upper", "upper", 0., splitPoint, 1., 1.);
  upper->SetTopMargin(gStyle->GetPadTopMargin() / (1 - splitPoint));
  upper->SetBottomMargin(0.5 * gapHigh / (1 - splitPoint));
  upper->Draw();
  TPad* lower = new TPad("lower", "lower", 0., 0., 1., splitPoint);
  lower->SetBottomMargin(gStyle->GetPadBottomMargin() / splitPoint);
  lower->SetTopMargin(0.5 * gapLow / splitPoint);
  lower->SetTickx(1);
  lower->SetTicky(1);
  // lower->SetGridy();
  lower->Draw();
  upper->cd();
  std::vector<TPad*> result;
  result.push_back(upper);
  result.push_back(lower);
  return result;
}

std::vector<TPad*> OnePad() {
  TPad* pad = new TPad("pad", "pad", 0., 0., 1., 1.);
  pad->Draw();
  pad->cd();
  std::vector<TPad*> result;
  result.push_back(pad);
  return result;
}

//----------------------------------------------------------------------------
// Functions for axis labelling
//----------------------------------------------------------------------------
void StandardAxes(TAxis* xaxis, TAxis* yaxis, TString var, TString units) {
  double width = xaxis->GetBinWidth(10);
  TString w_label = TString::Format("%.1f", width);
  std::cout << width << "\n";
  if (units == "") {
    xaxis->SetTitle(var);
    yaxis->SetTitle("Events / " + w_label);
  } else {
    xaxis->SetTitle(var + " (" + units + ")");
    yaxis->SetTitle("Events / " + w_label + " " + units);
  }
}

//----------------------------------------------------------------------------
// Functions for adding text to a pad
//----------------------------------------------------------------------------
// align: 1 (left), 2(centre), 3(right)
void drawTitle(TPad* pad, TString text, int align) {
  TVirtualPad *pad_backup = gPad;
  pad->cd();

  float t = pad->GetTopMargin();
  float l = pad->GetLeftMargin();
  float r = pad->GetRightMargin();

  float textSize = 0.6;
  float textOffset = 0.2;

  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);

  latex.SetTextFont(42);
  latex.SetTextSize(textSize * t);
  double y_off = 1 - t + textOffset * t;
  if (align == 1) latex.SetTextAlign(11);
  if (align == 1) latex.DrawLatex(l, y_off, text);
  if (align == 2) latex.SetTextAlign(21);
  if (align == 2) latex.DrawLatex(l + (1 - l - r) * 0.5, y_off, text);
  if (align == 3) latex.SetTextAlign(31);
  if (align == 3) latex.DrawLatex(1 - r, y_off, text);

  pad_backup->cd();
}


void drawCMSLogo(TPad* pad, TString cmsText, TString extraText, int iPosX, float relPosX, float relPosY, float relExtraDY) {
  TVirtualPad *pad_backup = gPad;
  pad->cd();
  float cmsTextFont = 61;  // default is helvetic-bold

  bool writeExtraText = extraText.Length() > 0;
  float extraTextFont = 52;  // default is helvetica-italics

  // text sizes and text offsets with respect to the top frame
  // in unit of the top margin size
  float lumiTextOffset = 0.2;
  float cmsTextSize = 0.75;
  // float cmsTextOffset    = 0.1;  // only used in outOfFrame version

  // ratio of "CMS" and extra text size
  float extraOverCmsTextSize = 0.76;

  TString lumi_13TeV = "20.1 fb^{-1}";
  TString lumi_8TeV = "19.7 fb^{-1}";
  TString lumi_7TeV = "5.1 fb^{-1}";

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
  if (iPosX == 0) relPosX = 0.12;
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

  if (outOfFrame) {
    latex.SetTextFont(cmsTextFont);
    latex.SetTextAlign(11);
    latex.SetTextSize(cmsTextSize * t);
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
    latex.SetTextSize(cmsTextSize * t);
    latex.SetTextAlign(align_);
    latex.DrawLatex(posX_, posY_, cmsText);
    if (writeExtraText) {
      latex.SetTextFont(extraTextFont);
      latex.SetTextAlign(align_);
      latex.SetTextSize(extraTextSize * t);
      latex.DrawLatex(posX_, posY_ - relExtraDY * cmsTextSize * t, extraText);
    }
  } else if (writeExtraText) {
    if (iPosX == 0) {
      posX_ = l + relPosX * (1 - l - r);
      posY_ = 1 - t + lumiTextOffset * t;
    }
    latex.SetTextFont(extraTextFont);
    latex.SetTextSize(extraTextSize * t);
    latex.SetTextAlign(align_);
    latex.DrawLatex(posX_, posY_, extraText);
  }
  pad_backup->cd();
}

void drawCMSLogo(TPad* pad, TString cmsText, TString extraText, int iPosX) {
  drawCMSLogo(pad, cmsText, extraText, iPosX, 0.045, 0.035, 1.2);
}

//----------------------------------------------------------------------------
// Functions to modify the global style
//----------------------------------------------------------------------------
// The TDR style unchanged from the reference here:
// https://ghm.web.cern.ch/ghm/plots/MacroExample/tdrstyle.C
void setTDRStyle() {
  // For the canvas:
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefH(600);  // Height of canvas
  gStyle->SetCanvasDefW(600);  // Width of canvas
  gStyle->SetCanvasDefX(0);    // POsition on screen
  gStyle->SetCanvasDefY(0);

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
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);

  // For the histo:
  // gStyle->SetHistFillColor(1);
  // gStyle->SetHistFillStyle(0);
  gStyle->SetHistLineColor(1);
  gStyle->SetHistLineStyle(0);
  gStyle->SetHistLineWidth(1);
  // gStyle->SetLegoInnerR(Float_t rad = 0.5);
  // gStyle->SetNumberContours(Int_t number = 20);

  gStyle->SetEndErrorSize(2);
  // gStyle->SetErrorMarker(20);
  // gStyle->SetErrorX(0.);

  gStyle->SetMarkerStyle(20);

  // For the fit/function:
  gStyle->SetOptFit(1);
  gStyle->SetFitFormat("5.4g");
  gStyle->SetFuncColor(2);
  gStyle->SetFuncStyle(1);
  gStyle->SetFuncWidth(1);

  // For the date:
  gStyle->SetOptDate(0);
  // gStyle->SetDateX(Float_t x = 0.01);
  // gStyle->SetDateY(Float_t y = 0.01);

  // For the statistics box:
  gStyle->SetOptFile(0);
  gStyle->SetOptStat(0);  // To display the mean and RMS:   SetOptStat("mr");
  gStyle->SetStatColor(kWhite);
  gStyle->SetStatFont(42);
  gStyle->SetStatFontSize(0.025);
  gStyle->SetStatTextColor(1);
  gStyle->SetStatFormat("6.4g");
  gStyle->SetStatBorderSize(1);
  gStyle->SetStatH(0.1);
  gStyle->SetStatW(0.15);
  // gStyle->SetStatStyle(Style_t style = 1001);
  // gStyle->SetStatX(Float_t x = 0);
  // gStyle->SetStatY(Float_t y = 0);

  // Margins:
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadRightMargin(0.02);

  // For the Global title:
  gStyle->SetOptTitle(0);
  gStyle->SetTitleFont(42);
  gStyle->SetTitleColor(1);
  gStyle->SetTitleTextColor(1);
  gStyle->SetTitleFillColor(10);
  gStyle->SetTitleFontSize(0.05);
  // gStyle->SetTitleH(0); // Set the height of the title box
  // gStyle->SetTitleW(0); // Set the width of the title box
  // gStyle->SetTitleX(0); // Set the position of the title box
  // gStyle->SetTitleY(0.985); // Set the position of the title box
  // gStyle->SetTitleStyle(Style_t style = 1001);
  // gStyle->SetTitleBorderSize(2);

  // For the axis titles:
  gStyle->SetTitleColor(1, "XYZ");
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetTitleSize(0.06, "XYZ");
  // Another way to set the size?
  // gStyle->SetTitleXSize(Float_t size = 0.02);
  // gStyle->SetTitleYSize(Float_t size = 0.02);
  gStyle->SetTitleXOffset(0.9);
  gStyle->SetTitleYOffset(1.25);
  // gStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:

  gStyle->SetLabelColor(1, "XYZ");
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetLabelOffset(0.007, "XYZ");
  gStyle->SetLabelSize(0.05, "XYZ");

  // For the axis:

  gStyle->SetAxisColor(1, "XYZ");
  gStyle->SetStripDecimals(kTRUE);
  gStyle->SetTickLength(0.03, "XYZ");
  gStyle->SetNdivisions(510, "XYZ");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

  // Change for log plots:
  gStyle->SetOptLogx(0);
  gStyle->SetOptLogy(0);
  gStyle->SetOptLogz(0);

  // Postscript options:
  gStyle->SetPaperSize(20., 20.);
  // gStyle->SetLineScalePS(Float_t scale = 3);
  // gStyle->SetLineStyleString(Int_t i, const char* text);
  // gStyle->SetHeaderPS(const char* header);
  // gStyle->SetTitlePS(const char* pstitle);

  // gStyle->SetBarOffset(Float_t baroff = 0.5);
  // gStyle->SetBarWidth(Float_t barwidth = 0.5);
  // gStyle->SetPaintTextFormat(const char* format = "g");
  // gStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // gStyle->SetTimeOffset(Double_t toffset);
  // gStyle->SetHistMinimumZero(kTRUE);

  gStyle->SetHatchesLineWidth(5);
  gStyle->SetHatchesSpacing(0.05);
}

// Some modifications to the TDR style, should call this function
// after setTDRStyle
void modTDRStyle() {
  setTDRStyle();


  // Increase the margins a bit
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.10);
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadRightMargin(0.04);

  gStyle->SetNdivisions(506, "XYZ");

  gStyle->SetMarkerColor(kBlack);
  gStyle->SetMarkerSize(1.0);
  gStyle->SetMarkerStyle(20);

  // Change the font from 42-->43 to set the size in absolute pixels
  // Only easy way to keep the same font size in pads with different
  // dimensions
  gStyle->SetLabelOffset(0.008, "XYZ");

  gStyle->SetTitleFont(43, "XYZ");
  gStyle->SetLabelFont(43, "XYZ");
  gStyle->SetTitleSize(0.04 * gStyle->GetCanvasDefH(), "XYZ");
  gStyle->SetLabelSize(0.03 * gStyle->GetCanvasDefH(), "XYZ");
  gStyle->SetTitleXOffset(3.3);
  gStyle->SetTitleYOffset(1.9);


  // Only draw ticks where we have an axis
  gStyle->SetPadTickX(0);
  gStyle->SetPadTickY(0);

  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendFont(43);
  gStyle->SetLegendFillColor(0);
  gStyle->SetFillColor(0);
}

#endif
