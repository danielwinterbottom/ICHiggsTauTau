#include <iostream>
#include <vector>
#include <map>
#include "TROOT.h"
#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLatex.h"



using namespace std;

int PlotExtrap(string input, string cat, string mc, string fn){
  using namespace std;

  // string input = "htt_et.inputs-sm-8TeV-lowsignal-real.root";
  // string cat = "eleTau_boost_high";
  // string mc = "ggH";
  TFile *input_dc = new TFile(input.c_str(),"UPDATE");

  // std::vector<string> cats;
  // cats.push_back(channel+"_vbf");
  // cats.push_back(channel+"_boost");
  // cats.push_back(channel+"_boost_high");
  // cats.push_back(channel+"_boost_low");
  // cats.push_back(channel+"_0jet_high");
  // cats.push_back(channel+"_0jet_low");
  // cats.push_back(channel+"_inclusive");


  std::string test_source_str = "145";
  std::string test_predict_str = "110";

  std::vector<std::string> masses;
  masses.push_back("90");
  masses.push_back("95");
  masses.push_back("100");
  masses.push_back("105");
  masses.push_back("110");
  masses.push_back("115");
  masses.push_back("120");
  masses.push_back("125");
  masses.push_back("130");
  masses.push_back("135");
  masses.push_back("140");
  masses.push_back("145");
  std::vector<double> massesd;
  massesd.push_back(90);
  massesd.push_back(95);
  massesd.push_back(100);
  massesd.push_back(105);
  massesd.push_back(110);
  massesd.push_back(115);
  massesd.push_back(120);
  massesd.push_back(125);
  massesd.push_back(130);
  massesd.push_back(135);
  massesd.push_back(140);
  massesd.push_back(145);

  std::vector<double> yield;
  std::vector<double> yield_err;
  std::vector<double> mass_err;

  if (!gDirectory->cd(("/"+cat).c_str())) return 1;
  std::vector<TH1F *> hists;
  for (unsigned i = 0; i < masses.size(); ++i) {
    hists.push_back((TH1F*)gDirectory->Get((mc+masses[i]).c_str()));
    double err = 0.0;
    hists.back()->IntegralAndError(0, hists.back()->GetNbinsX() + 1, err);
    yield.push_back(hists.back()->Integral());
    yield_err.push_back(err);
    mass_err.push_back(0);
  }

  // TF1 fitted("fit","-33.19+x*0.3415",90,145);
  TF1 fitted("fit",fn.c_str(),90,145);

  TGraphErrors gYield (12, &(massesd[0]), &(yield[0]), &(mass_err[0]), &(yield_err[0]));
  gYield.SetName("yield");
  TCanvas cYield("cYield","cYield",700,700);
  gYield.Draw("ap");
  fitted.Draw("SAME");
  gYield.SetMinimum(0.0);
  gYield.GetXaxis()->SetTitle("Mass Point");
  gYield.GetYaxis()->SetTitle("Distribution Yield");


  TLatex *title_latex = new TLatex();
  title_latex->SetNDC();
  title_latex->SetTextSize(0.03);
  title_latex->SetTextAlign(31);
  title_latex->DrawLatex(0.95,0.965, TString(cat) + ", " + TString(mc));

  cYield.SaveAs(("yield_"+cat+"_"+mc+".pdf").c_str());
  cYield.SaveAs(("yield_"+cat+"_"+mc+".png").c_str());




  input_dc->Close();




  return 0;
}
