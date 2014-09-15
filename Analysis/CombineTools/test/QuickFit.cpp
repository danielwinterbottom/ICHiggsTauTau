#include <string>
// #include "boost/filesystem.hpp"
// #include "boost/regex.hpp"
// #include "boost/format.hpp"
#include "boost/bind.hpp"
// #include "boost/assign/list_of.hpp"
#include "CombineTools/interface/CombineHarvester.h"
#include "CombineTools/interface/Observation.h"
#include "CombineTools/interface/Process.h"
#include "CombineTools/interface/HelperFunctions.h"
#include "CombineTools/interface/MorphFunctions.h"
#include "CombineTools/interface/HttSystematics.h"
#include "CombineTools/interface/RooHttSMPdf.h"
// #include "CombineTools/interface/RooHttYield.h"s
// #include "TH1F.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooGenericPdf.h"
#include "RooPlot.h"
#include "TCanvas.h"

using boost::bind;
using std::string;
using std::vector;
using std::pair;
using std::make_pair;
using std::map;
using std::set;

int main() {

  TH1F hist = ch::OpenFromTFile<TH1F>(
      "htt_mt.mssm.root:htt_mt_8_8TeV/data_obs");
      // "data/demo/htt_mt.inputs-mssm-8TeV-0.root:muTau_nobtag/TT_fine_binning");
  TH1F hist2("hist2", "hist2", hist.GetNbinsX(), 0,
             static_cast<float>(hist.GetNbinsX()));
  for (int i = 1; i <= hist.GetNbinsX(); ++i) {
    hist2.SetBinContent(i, hist.GetBinContent(i));
  }
  RooRealVar lM("m", "m", 0, 5000);  // lM.setBinning(lBinning);
  RooRealVar lA("a", "a", 50, 0.1, 200);
  RooRealVar lB("b", "b", 50, -10500, 10500);  // lB.setConstant(kTRUE);
  RooDataHist pH0("Data", "Data", RooArgList(lM), &hist2);
  double iFirst = 0.;
  double iLast = 260.;
  TString iStr = TString::Format("%.0f", iFirst);
  TString fn = "exp(-(m-" + iStr + ")/(a+0.001*b*(m-" + iStr + ")))";
  // TString fn = "exp(-(m)/(a+b*(m)))";
  std::cout << "fn = " << fn << std::endl;
  RooGenericPdf lFit("genPdf", fn, RooArgList(lM, lA, lB));
  RooFitResult  *lRFit = 0;
  lRFit = lFit.fitTo(pH0,RooFit::Save(kTRUE),RooFit::Range(iFirst,iLast),RooFit::Strategy(0));

  RooPlot *plot = lM.frame();
  pH0.plotOn(plot);
  lFit.plotOn(plot);

  TCanvas canv("canv", "canv", 600, 600);
  plot->Draw();
  canv.SaveAs("result.pdf");

}