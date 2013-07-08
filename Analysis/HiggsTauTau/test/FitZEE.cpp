
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

#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TextElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"


using namespace std;
using namespace ic;


int main(int argc, char* argv[]){

  ic::Plot::SetTdrStyle();
  TH1F data = GetFromTFile<TH1F>("datacard_m_vis_inclusive_et_2012.root", "/eleTau_inclusive", "data_obs");
  TH1F zee = GetFromTFile<TH1F>("datacard_m_vis_inclusive_et_2012.root", "/eleTau_inclusive", "ZL");

  double data_rate = data.Integral();
  double zee_init_rate = zee.Integral();

  RooRealVar x("x","mvis",data.GetBinLowEdge(1),data.GetBinLowEdge(data.GetNbinsX()+1));
  RooRealVar c1("c1","c1",-10,10);
  RooRealVar c2("c2","c2",-1,1);
  RooRealVar c3("c3","c3",-1,1);
  RooGenericPdf bkg("bkg","exp(c1*x+c2*x*x+c3*x*x*x)",RooArgSet(x,c1,c2,c3));


  RooDataHist zee_hist("zee_hist","zee_hist",RooArgSet(x),&zee);
  RooHistPdf zee_pdf("zee_pdf","zee_pdf",RooArgSet(x),zee_hist);


  ///Fit Data
  TCanvas C("canvas", "canvas", 800,800);

  RooRealVar yield("yield","yield",0.01,.9);
  RooAddPdf full_pdf("full_pdf","full_pdf",RooArgList(zee_pdf,bkg),RooArgList(yield));
  RooDataHist data_hist("data_hist","data_hist",RooArgSet(x),&data);
  RooChi2Var chi("chi","chi",full_pdf,data_hist,RooFit::DataError(RooAbsData::SumW2));
  RooMinuit minuit(chi);
  minuit.migrad();
  RooPlot* plot=x.frame();
  data_hist.plotOn(plot);
  full_pdf.plotOn(plot);
  full_pdf.plotOn(plot,RooFit::Components(zee_pdf),RooFit::LineColor(2));
  C.Clear();
  plot->SetTitle("Title");
  plot->GetYaxis()->SetTitle("");
  plot->GetXaxis()->SetTitle("m(e#tau)");
  plot->Draw();
  C.Print("plotZEE.ps");

  double fit_frac = yield.getVal();
  double zee_fit_rate = data_rate * fit_frac;
  std::cout << "Scale factor: " << zee_fit_rate / zee_init_rate << std::endl;

  return 0;
}

