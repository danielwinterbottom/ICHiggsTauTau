#include "TString.h"
#include "TCanvas.h"
#include "TROOT.h"
#include <iostream>
#include "TFile.h"
#include "TH2F.h"
#include "TGraphAsymmErrors.h"
#include "TStyle.h"
#include "TBox.h"
#include "TLine.h"
#include "TLatex.h"
//#include "RooFitResult.h"
//#include "RooRealVar.h"


void htt_style() {
  TStyle *HttStyle = new TStyle("Htt-Style","The Perfect Style for Plots ;-)");
  gStyle = HttStyle;
  
  // Canvas
  HttStyle->SetCanvasColor     (0);
  HttStyle->SetCanvasBorderSize(10);
  HttStyle->SetCanvasBorderMode(0);
  HttStyle->SetCanvasDefH      (600);
  HttStyle->SetCanvasDefW      (600);
  HttStyle->SetCanvasDefX      (100);
  HttStyle->SetCanvasDefY      (100);
  
  // color palette for 2D temperature plots
  HttStyle->SetPalette(1,0);
  
  // Pads
  HttStyle->SetPadColor       (0);
  HttStyle->SetPadBorderSize  (10);
  HttStyle->SetPadBorderMode  (0);
  HttStyle->SetPadBottomMargin(0.13);
  HttStyle->SetPadTopMargin   (0.08);
  HttStyle->SetPadLeftMargin  (0.17);
  HttStyle->SetPadRightMargin (0.05);
  HttStyle->SetPadGridX       (0);
  HttStyle->SetPadGridY       (0);
  HttStyle->SetPadTickX       (1);
  HttStyle->SetPadTickY       (1);
  
  // Frames
  HttStyle->SetLineWidth(3);
  HttStyle->SetFrameFillStyle ( 0);
  HttStyle->SetFrameFillColor ( 0);
  HttStyle->SetFrameLineColor ( 1);
  HttStyle->SetFrameLineStyle ( 0);
  HttStyle->SetFrameLineWidth ( 2);
  HttStyle->SetFrameBorderSize(10);
  HttStyle->SetFrameBorderMode( 0);
  
  // Histograms
  HttStyle->SetHistFillColor(2);
  HttStyle->SetHistFillStyle(0);
  HttStyle->SetHistLineColor(1);
  HttStyle->SetHistLineStyle(0);
  HttStyle->SetHistLineWidth(3);
  HttStyle->SetNdivisions(510);
  
  // Functions
  HttStyle->SetFuncColor(1);
  HttStyle->SetFuncStyle(0);
  HttStyle->SetFuncWidth(2);
  
  // Various
  HttStyle->SetMarkerStyle(20);
  HttStyle->SetMarkerColor(kBlack);
  HttStyle->SetMarkerSize (1.1);
  
  HttStyle->SetTitleBorderSize(0);
  HttStyle->SetTitleFillColor (0);
  HttStyle->SetTitleX         (0.2);
  
  HttStyle->SetTitleSize  (0.055,"X");
  HttStyle->SetTitleOffset(1.200,"X");
  HttStyle->SetLabelOffset(0.005,"X");
  HttStyle->SetLabelSize  (0.040,"X");
  HttStyle->SetLabelFont  (42   ,"X");
  
  HttStyle->SetStripDecimals(kFALSE);
  HttStyle->SetLineStyleString(11,"20 10");
  
  HttStyle->SetTitleSize  (0.055,"Y");
  HttStyle->SetTitleOffset(1.600,"Y");
  HttStyle->SetLabelOffset(0.010,"Y");
  HttStyle->SetLabelSize  (0.040,"Y");
  HttStyle->SetLabelFont  (42   ,"Y");
  
  HttStyle->SetOptTitle(0);
  
  
  HttStyle->SetTextSize   (0.055);
  HttStyle->SetTextFont   (42);
  
  HttStyle->SetStatFont   (42);
  HttStyle->SetTitleFont  (42);
  HttStyle->SetTitleFont  (42,"X");
  HttStyle->SetTitleFont  (42,"Y");
  HttStyle->SetEndErrorSize(0);
  
  
  HttStyle->SetOptStat    (0);
  
  gROOT->ForceStyle();
  return;
  
}

TString betterName(TString name) {

 if (name == "htt_ee_0jet_0_7TeV") return TString("ee, 0-jet low, 7TeV");
 if (name == "htt_ee_0jet_1_7TeV") return TString("ee, 0-jet high, 7TeV");
 if (name == "htt_ee_1jet_2_7TeV") return TString("ee, 1-jet low, 7TeV");
 if (name == "htt_ee_1jet_3_7TeV") return TString("ee, 1-jet high, 7TeV");
 if (name == "htt_ee_vbf_4_7TeV")  return TString("ee, vbf, 7TeV");
 if (name == "htt_ee_0jet_0_8TeV") return TString("ee, 0-jet low, 8TeV");
 if (name == "htt_ee_0jet_1_8TeV") return TString("ee, 0-jet high, 8TeV");
 if (name == "htt_ee_1jet_2_8TeV") return TString("ee, 1-jet low, 8TeV");
 if (name == "htt_ee_1jet_3_8TeV") return TString("ee, 1-jet high, 8TeV");
 if (name == "htt_ee_vbf_4_8TeV")  return TString("ee, vbf, 8TeV");
 if (name == "htt_mm_0jet_0_7TeV") return TString("#mu#mu, 0-jet low, 7TeV");
 if (name == "htt_mm_0jet_1_7TeV") return TString("#mu#mu, 0-jet high, 7TeV");
 if (name == "htt_mm_1jet_2_7TeV") return TString("#mu#mu, 1-jet low, 7TeV");
 if (name == "htt_mm_1jet_3_7TeV") return TString("#mu#mu, 1-jet high, 7TeV");
 if (name == "htt_mm_vbf_4_7TeV")  return TString("#mu#mu, vbf, 7TeV");
 if (name == "htt_mm_0jet_0_8TeV") return TString("#mu#mu, 0-jet low, 8TeV");
 if (name == "htt_mm_0jet_1_8TeV") return TString("#mu#mu, 0-jet high, 8TeV");
 if (name == "htt_mm_1jet_2_8TeV") return TString("#mu#mu, 1-jet low, 8TeV");
 if (name == "htt_mm_1jet_3_8TeV") return TString("#mu#mu, 1-jet high, 8TeV");
 if (name == "htt_mm_vbf_4_8TeV")  return TString("#mu#mu, vbf, 8TeV");

 if (name == "htt_em_0jet_0_7TeV") return TString("e#mu, 0-jet low, 7TeV");
 if (name == "htt_em_0jet_1_7TeV") return TString("e#mu, 0-jet high, 7TeV");
 if (name == "htt_em_1jet_2_7TeV") return TString("e#mu, 1-jet low, 7TeV");
 if (name == "htt_em_1jet_3_7TeV") return TString("e#mu, 1-jet high, 7TeV");
 if (name == "htt_em_vbf_4_7TeV") return TString("e#mu, vbf, 7TeV");
 if (name == "htt_em_0jet_0_8TeV") return TString("e#mu, 0-jet low, 8TeV");
 if (name == "htt_em_0jet_1_8TeV") return TString("e#mu, 0-jet high, 8TeV");
 if (name == "htt_em_1jet_2_8TeV") return TString("e#mu, 1-jet low, 8TeV");
 if (name == "htt_em_1jet_3_8TeV") return TString("e#mu, 1-jet high, 8TeV");
 if (name == "htt_em_vbf_4_8TeV") return TString("e#mu, vbf loose, 8TeV");
 if (name == "htt_em_vbf_5_8TeV") return TString("e#mu, vbf tight, 8TeV");

 if (name == "htt_et_0jet_1_7TeV") return TString("e#tau_{h}, 0-jet low, 7TeV");
 if (name == "htt_et_0jet_2_7TeV") return TString("e#tau_{h}, 0-jet high, 7TeV");
 if (name == "htt_et_1jet_3_7TeV") return TString("e#tau_{h}, 1-jet low, 7TeV");
 if (name == "htt_et_1jet_5_7TeV") return TString("e#tau_{h}, 1-jet high boost, 7TeV");
 if (name == "htt_et_vbf_6_7TeV") return TString("e#tau_{h}, vbf, 7TeV");
 if (name == "htt_et_0jet_1_8TeV") return TString("e#tau_{h}, 0-jet low, 8TeV");
 if (name == "htt_et_0jet_2_8TeV") return TString("e#tau_{h}, 0-jet high, 8TeV");
 if (name == "htt_et_1jet_3_8TeV") return TString("e#tau_{h}, 1-jet low, 8TeV");
 if (name == "htt_et_1jet_5_8TeV") return TString("e#tau_{h}, 1-jet high boost, 8TeV");
 if (name == "htt_et_vbf_6_8TeV") return TString("e#tau_{h}, vbf loose, 8TeV");
 if (name == "htt_et_vbf_7_8TeV") return TString("e#tau_{h}, vbf tight, 8TeV");

 if (name == "htt_mt_0jet_1_7TeV") return TString("#mu#tau_{h}, 0-jet low, 7TeV");
 if (name == "htt_mt_0jet_2_7TeV") return TString("#mu#tau_{h}, 0-jet high, 7TeV");
 if (name == "htt_mt_1jet_3_7TeV") return TString("#mu#tau_{h}, 1-jet low, 7TeV");
 if (name == "htt_mt_1jet_4_7TeV") return TString("#mu#tau_{h}, 1-jet high, 7TeV");
 if (name == "htt_mt_1jet_5_7TeV") return TString("#mu#tau_{h}, 1-jet high boost, 7TeV");
 if (name == "htt_mt_vbf_6_7TeV") return TString("#mu#tau_{h}, vbf, 7TeV");
 if (name == "htt_mt_0jet_1_8TeV") return TString("#mu#tau_{h}, 0-jet low, 8TeV");
 if (name == "htt_mt_0jet_2_8TeV") return TString("#mu#tau_{h}, 0-jet high, 8TeV");
 if (name == "htt_mt_1jet_3_8TeV") return TString("#mu#tau_{h}, 1-jet low, 8TeV");
 if (name == "htt_mt_1jet_4_8TeV") return TString("#mu#tau_{h}, 1-jet high, 8TeV");
 if (name == "htt_mt_1jet_5_8TeV") return TString("#mu#tau_{h}, 1-jet high boost, 8TeV");
 if (name == "htt_mt_vbf_6_8TeV") return TString("#mu#tau_{h}, vbf loose, 8TeV");
 if (name == "htt_mt_vbf_7_8TeV") return TString("#mu#tau_{h}, vbf tight, 8TeV");

 if (name == "htt_tt_1jet_0_8TeV") return TString("#tau_{h}#tau_{h}, 1-jet high, 8TeV");
 if (name == "htt_tt_1jet_1_8TeV") return TString("#tau_{h}#tau_{h}, 1-jet high large boost, 8TeV");
 if (name == "htt_tt_vbf_2_8TeV") return TString("#tau_{h}#tau_{h}, vbf, 8TeV");
 
 if (name == "_ee_0jet_") return TString("ee, 0-jet");
 if (name == "_ee_1jet_") return TString("ee, 1-jet");
 if (name == "_ee_vbf_") return TString("ee, vbf");
 if (name == "_mm_0jet_") return TString("#mu#mu, 0-jet");
 if (name == "_mm_1jet_") return TString("#mu#mu, 1-jet");
 if (name == "_mm_vbf_") return TString("#mu#mu, vbf");
 if (name == "_mt_0jet_") return TString("#mu#tau_{h}, 0-jet");
 if (name == "_mt_1jet_") return TString("#mu#tau_{h}, 1-jet");
 if (name == "_mt_vbf_") return TString("#mu#tau_{h}, vbf");
 if (name == "_et_0jet_") return TString("e#tau_{h}, 0-jet");
 if (name == "_et_1jet_") return TString("e#tau_{h}, 1-jet");
 if (name == "_et_vbf_") return TString("e#tau_{h}, vbf");
 if (name == "_em_0jet_") return TString("e#mu, 0-jet");
 if (name == "_em_1jet_") return TString("e#mu, 1-jet");
 if (name == "_em_vbf_") return TString("e#mu, vbf");
 if (name == "_tt_1jet_") return TString("#tau_{h}#tau_{h}, 1-jet");
 if (name == "_tt_vbf_") return TString("#tau_{h}#tau_{h}, vbf");
 if (name == "vhtt_zh") return TString("#it{ll}+#it{LL}^{'}");
 if (name == "vhtt_wh_lep") return TString("#it{l}+#it{l}#tau_{h}");
 if (name == "vhtt_wh_had") return TString("#it{l}+#tau_{h}#tau_{h}");
 return name;
}
void cccPlot(const char *poi = "r", double rMin = -4, double rMax=4, TString filename="ccc.pdf") {
    htt_style();
    TCanvas *c1 = new TCanvas("c1","c1",700,700);
    c1->SetLeftMargin(0.2);
    c1->SetGridx(1);

    if (gFile == 0) { std::cout << "No input file open" << std::endl; return; }
    RooFitResult *fit_nominal   = (RooFitResult *) gFile->Get("fit_nominal");
    RooFitResult *fit_alternate = (RooFitResult *) gFile->Get("fit_alternate");
    if (fit_nominal == 0 || fit_alternate == 0) { std::cout << "Input file " << gFile->GetName() << " does not contain fit_nominal or fit_alternate" << std::endl; return; }
    RooRealVar *rFit = (RooRealVar *) fit_nominal->floatParsFinal().find(poi);
    if (rFit == 0)  { std::cout << "Nominal fit does not contain parameter " << poi << std::endl; return; }

    TString prefix =  TString::Format("_ChannelCompatibilityCheck_%s_", poi);


    int nChann = 0;
    TIterator *iter = fit_alternate->floatParsFinal().createIterator();
    for (RooAbsArg *a = (RooAbsArg *) iter->Next(); a != 0; a = (RooAbsArg *) iter->Next()) {
        if (TString(a->GetName()).Index(prefix) == 0) nChann++;
    }
    TH2F frame("frame",";Best Fit #sigma/#sigma_{SM};",1,TMath::Max(rFit->getMin(), rMin),TMath::Min(rFit->getMax(),rMax),nChann,0,nChann);
    frame.GetXaxis()->SetTitleFont(62);

    iter->Reset(); int iChann = 0; TGraphAsymmErrors points(nChann);
    for (RooAbsArg *a = (RooAbsArg *) iter->Next(); a != 0; a = (RooAbsArg *) iter->Next()) {
        if (TString(a->GetName()).Index(prefix) == 0) {
            RooRealVar *ri = (RooRealVar *) a;
            TString channel = a->GetName(); channel.ReplaceAll(prefix,"");
            channel = betterName(channel);
            //double symm = (ri->getAsymErrorHi()-ri->getAsymErrorLo())/2.0;
            points.SetPoint(iChann,       ri->getVal(), iChann+0.5);
            points.SetPointError(iChann, -ri->getAsymErrorLo(), ri->getAsymErrorHi(), 0, 0);
            //points.SetPointError(iChann, symm, symm, 0, 0);
            iChann++;
            channel = TString::Format("#bf{%s}",channel.Data());
            //channel = TString::Format("#bf{%s},  %+.2f#pm%.2f",channel.Data(), ri->getVal(), symm);
            frame.GetYaxis()->SetBinLabel(iChann, channel);
        }
    }
    points.SetLineColor(kRed);
    points.SetLineWidth(3);
    points.SetMarkerStyle(21);
    //frame.GetXaxis()->SetNdivisions(505);
    frame.GetXaxis()->SetTitleSize(0.05);
    frame.GetXaxis()->SetLabelSize(0.04);
    frame.GetYaxis()->SetLabelSize(0.04);
    frame.Draw(); gStyle->SetOptStat(0);
    TBox globalFitBand(rFit->getVal()+rFit->getAsymErrorLo(), 0, rFit->getVal()+rFit->getAsymErrorHi(), nChann);
    globalFitBand.SetFillStyle(3013);
    globalFitBand.SetFillColor(65);
    globalFitBand.SetLineStyle(0);
    globalFitBand.DrawClone();
    TLine globalFitLine(rFit->getVal(), 0, rFit->getVal(), nChann);
    globalFitLine.SetLineWidth(4);
    globalFitLine.SetLineColor(214);
    globalFitLine.DrawClone();
    points.Draw("P SAME");
    TLatex *tex = new TLatex(0.20,0.94,"CMS (unpublished), 4.9 fb^{-1} at 7 TeV, 19.7 fb^{-1} at 8 TeV");
    tex->SetNDC();
    tex->SetTextSize(0.03);
    tex->SetTextFont(62);
    tex->SetLineWidth(2);
    tex->Draw();
    c1->Print(filename+".pdf");
    c1->Print(filename+".png");
    c1->SaveAs(filename+".C");
    std::cout << "Channels: " << nChann << std::endl; 
}


