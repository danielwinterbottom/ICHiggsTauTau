
#include <iostream>
#include <vector>
#include <map>
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TF1.h"
#include "RooRealVar.h"
#include "RooGenericPdf.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooAddPdf.h"
#include "RooChi2Var.h"
#include "RooMinuit.h"
#include "RooPlot.h"
#include "RooMCStudy.h"
#include "RooGaussian.h"

#include "RooFitResult.h"

#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TextElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"


using namespace std;
using namespace ic;


int main(int /*argc*/, char* /*argv*/[]){


unsigned nsig = 50;
unsigned nbkg = 500;

RooRealVar x("x","x",-5,5);
RooRealVar lM("m","m" ,300,1500);
TH1F ggh = GetFromTFile<TH1F>("htt_em.inputs-mssm-8TeV-0.root", "/emu_nobtag", "ggH800_fine_binning");
ggh.Rebin(10);
RooDataHist ggh_hist("ggh_hist","ggh_hist",RooArgSet(lM),&ggh);
RooHistPdf ggh_pdf("ggh_pdf","ggh_pdf",RooArgSet(lM),ggh_hist);

RooRealVar mean("mean","mean of gaussian",500); 
RooRealVar sigma("sigma","width of gaussian",70); 
RooGaussian gauss("gauss","gaussian PDF",lM,mean,sigma);

RooRealVar lA("a","a" ,68.3899,  0.1,200);
RooRealVar lB("b","b" ,0.0785294 , -10.5,10.5);
RooGenericPdf *lFit = new RooGenericPdf("genPdf","exp(-m/(a+b*m))",RooArgList(lM,lA,lB));

RooRealVar lNB("nb","nb",nbkg,0,1000000);
RooRealVar lNSig("nsig","nsig",nsig,-1000,100000);
// RooAddPdf    *lGenMod = new RooAddPdf  ("genmod","genmod",RooArgList(*lFit ,gauss),RooArgList(lNB,lNSig));
RooAddPdf    *lGenMod = new RooAddPdf  ("genmod","genmod",RooArgList(*lFit ,ggh_pdf),RooArgList(lNB,lNSig));



RooRealVar lA1("a1","a1" ,50,  0.1,1000);
RooRealVar lB1("b1","b1" ,0.0 , -30.5,30.5); 
RooGenericPdf *lTest  = 0; 
// lFit = new RooGenericPdf("genPdf","exp(-m/(a+b*m))",RooArgList(lM,lA,lB));
lTest = new RooGenericPdf("genPdf","exp(-a*pow(m,b))",RooArgList(lM,lA,lB));
lA1.setVal(0.3); lB1.setVal(0.5);
// lTest = new RooGenericPdf("genPdf","a*exp(b*m)",RooArgList(lM,lA,lB));
// lTest = new RooGenericPdf("genPdf","a/pow(m,b)",RooArgList(lM,lA,lB));
RooAddPdf    *lFitMod = new RooAddPdf  ("fitmod","fitmod",RooArgList(*lTest,ggh_pdf),RooArgList(lNB,lNSig));



// RooDataHist *dataHist = gauss.generateBinned(x, 10, RooFit::ExpectedData()); 
// TH1 *hist = dataHist->createHistogram("hist",x,RooFit::Binning(20,-5,5));
// RooDataHist dataHist2("dataHist2","dataHist2",RooArgSet(x),hist);
// RooHistPdf pdfHist("gauss_binned","gauss_binned",RooArgSet(x),dataHist2);
RooPlot* plot=lM.frame();
lGenMod->plotOn(plot);
lGenMod->plotOn(plot,RooFit::Components(ggh_pdf),RooFit::LineColor(2));
// pdfHist.plotOn(plot);
TCanvas* lC01 = new TCanvas("pdf","pdf",600,600) ;
lC01->cd();
plot->Draw();
lC01->SaveAs("binned.pdf");




RooMCStudy mgr(*lGenMod,lM,RooFit::FitModel(*lFitMod),RooFit::Silence(),RooFit::Extended(true),RooFit::Verbose(1),RooFit::Binned(kTRUE),RooFit::FitOptions(RooFit::Save(kTRUE),RooFit::Strategy(0)));
mgr.generateAndFit(3000,nsig+nbkg);
std::cout << mean.getVal() << std::endl;
RooPlot* lFrame1 = mgr.plotPull(lNSig,-1.5,1.5,50,true);
// RooPlot* lFrame1 = mgr.plotParam(lNSig,RooFit::FrameRange(-200, 200),RooFit::FrameBins(100));
TCanvas* lC00 = new TCanvas("pulls","pulls",600,600) ;
lC00->cd();
lFrame1->Draw();
mgr.fitResult(0)->Print();
lC00->SaveAs("test.pdf");


  // ic::Plot::SetTdrStyle();
  // TH1F data = GetFromTFile<TH1F>("datacard_m_vis_inclusive_et_2012.root", "/eleTau_inclusive", "data_obs");
  // TH1F zee = GetFromTFile<TH1F>("datacard_m_vis_inclusive_et_2012.root", "/eleTau_inclusive", "ZL");

  // double data_rate = data.Integral();
  // double zee_init_rate = zee.Integral();

  // RooRealVar x("x","mvis",data.GetBinLowEdge(1),data.GetBinLowEdge(data.GetNbinsX()+1));
  // RooRealVar c1("c1","c1",-10,10);
  // RooRealVar c2("c2","c2",-1,1);
  // RooRealVar c3("c3","c3",-1,1);
  // RooGenericPdf bkg("bkg","exp(c1*x+c2*x*x+c3*x*x*x)",RooArgSet(x,c1,c2,c3));


  // RooDataHist zee_hist("zee_hist","zee_hist",RooArgSet(x),&zee);
  // RooHistPdf zee_pdf("zee_pdf","zee_pdf",RooArgSet(x),zee_hist);


  // ///Fit Data
  // TCanvas C("canvas", "canvas", 800,800);

  // RooRealVar yield("yield","yield",0.01,.9);
  // RooAddPdf full_pdf("full_pdf","full_pdf",RooArgList(zee_pdf,bkg),RooArgList(yield));
  // RooDataHist data_hist("data_hist","data_hist",RooArgSet(x),&data);
  // RooChi2Var chi("chi","chi",full_pdf,data_hist,RooFit::DataError(RooAbsData::SumW2));
  // RooMinuit minuit(chi);
  // minuit.migrad();
  // RooPlot* plot=x.frame();
  // data_hist.plotOn(plot);
  // full_pdf.plotOn(plot);
  // full_pdf.plotOn(plot,RooFit::Components(zee_pdf),RooFit::LineColor(2));
  // C.Clear();
  // plot->SetTitle("Title");
  // plot->GetYaxis()->SetTitle("");
  // plot->GetXaxis()->SetTitle("m(e#tau)");
  // plot->Draw();
  // C.Print("plotZEE.ps");

  // double fit_frac = yield.getVal();
  // double zee_fit_rate = data_rate * fit_frac;
  // std::cout << "Scale factor: " << zee_fit_rate / zee_init_rate << std::endl;

  return 0;
}

