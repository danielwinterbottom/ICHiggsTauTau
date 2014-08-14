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
#include "CombineTools/interface/HttSystematics.h"
#include "CombineTools/interface/RooHttSMPdf.h"
#include "CombineTools/interface/RooHttYield.h"
// #include "TH1F.h"
#include "RooFitResult.h"
#include "RooHistPdf.h"
#include "RooDataHist.h"
#include "RooRealVar.h"
#include "RooRealProxy.h"
#include "RooWorkspace.h"
#include "RooHistFunc.h"

using boost::bind;
using std::string;
using std::vector;
using std::pair;
using std::make_pair;

int main() {
  TH1::AddDirectory(false);

  TH1F qqH120 =
      ch::OpenFromTFile<TH1F>("htt_mt.input_8TeV.root:muTau_vbf_loose/qqH120");
  TH1F qqH125 =
      ch::OpenFromTFile<TH1F>("htt_mt.input_8TeV.root:muTau_vbf_loose/qqH125");
  // qqH120.Scale(1.0 / qqH120.Integral());
  // qqH125.Scale(1.0 / qqH125.Integral());
  std::cout << "120: " << qqH120.Integral() << "\n";
  std::cout << "125: " << qqH125.Integral() << "\n";
  RooRealVar mtt("CMS_th1x", "CMS_th1x", 0, 350);
  RooRealVar mh("MH", "MH", 122.5, 90, 145);
  RooDataHist scratch_hist("scratch_hist", "scratch_hist", RooArgList(mtt),
                          RooFit::Import(qqH125, false));
  RooDataHist qqH125_hist("qqH125", "qqH125", RooArgList(mtt),
                          RooFit::Import(qqH125, false));
  RooDataHist qqH120_hist("qqH120", "qqH120", RooArgList(mtt),
                          RooFit::Import(qqH120, false));
  RooHistPdf qqH125_pdf("qqH125_pdf", "qqH125_pdf", RooArgList(mtt),
                        qqH125_hist);
  RooHistPdf qqH120_pdf("qqH120_pdf", "qqH120_pdf", RooArgList(mtt),
                        qqH120_hist);
  RooHttSMPdf pdf("htt_pdf", "htt_pdf", RooArgList(mtt), scratch_hist, mh);
  pdf.AddPoint(120, qqH120);
  pdf.AddPoint(125, qqH125);
  RooHttYield yield("htt_yield", "htt_yield", mh);
  yield.AddPoint(120, qqH120.Integral());
  yield.AddPoint(125, qqH125.Integral());
  // std::cout << pdf.getVal() << std::endl;

  // for (int i = 0; i < mtt.numBins(); ++i) {
  //   mtt.setBin(i);
  //   std::cout << "120   " << i << ": " << qqH120_pdf.getVal() << std::endl;
  //   std::cout << "MyPdf " << i << ": " << pdf.getVal() << std::endl;
  //   std::cout << "125   " << i << ": " << qqH125_pdf.getVal() << std::endl;
  // }
  double x = 120.;
  for (unsigned i = 0; i < 100; ++i, x += 0.05) {
    mh.setVal(x);
    pdf.getVal();
    std::cout << x << ":  " << yield.getVal() << std::endl;
  }

  // RooRealVar mx("MX", "MX", 1, 0, 10);
  // TH1F test("test", "test", 10, -0.5, 9.5);
  // for (int i = 1; i <= test.GetNbinsX(); ++i) test.SetBinContent(i, float(i));
  // RooDataHist thist("thist", "thist", RooArgList(mx),
  //    RooFit::Import(test, false));
  // RooHistFunc func("func", "func", RooArgList(mx), thist, 1);
  // for (unsigned i = 0; i < 10; ++i) {
  //   mx.setVal(double(i));
  //   std::cout << i << " " << func.getVal() << std::endl;
  // }

  // RooWorkspace ws("htt_sig", "htt_sig");
  // ws.import(pdf);
  // ws.Print();

  // TFile f("test.root", "RECREATE");
  // ch::WriteToTFile(&ws, &f, "htt_sig");
  // f.Close();
}
