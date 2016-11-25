#include <iostream>
#include <vector>
#include <map>
#include "TGraph.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TextElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SimpleParamParser.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"

namespace po = boost::program_options;

using namespace std;
using namespace ic;

void SetTGraphStyle(TGraph * gr, int color) {
    gr->SetLineStyle(11.);
    gr->SetLineWidth( 3.);
    gr->SetLineColor(color);
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1.3);
    gr->SetMarkerColor(color);
}


int main(int /*argc*/, char* /*argv*/[]){

  string ch = "cmb";

  TGraph exp1 = GetFromTFile<TGraph>("pairs-plots/"+ch+"_limit.root", ch+"/", "expected");
  TGraph obs1 = GetFromTFile<TGraph>("pairs-plots/"+ch+"_limit.root", ch+"/", "observed");
  TGraph exp2 = GetFromTFile<TGraph>("presel-plots/"+ch+"_limit.root", ch+"/", "expected");
  TGraph obs2 = GetFromTFile<TGraph>("presel-plots/"+ch+"_limit.root", ch+"/", "observed");

  for (int i = 0; i < exp1.GetN(); ++i) {
    double diff = fabs(exp1.GetY()[i] - exp2.GetY()[i]) / exp1.GetY()[i];
    exp1.GetY()[i] = diff;
    std::cout << exp1.GetX()[i] << std::endl;
  }
  for (int i = 0; i < obs1.GetN(); ++i) {
    double diff = fabs(obs1.GetY()[i] - obs2.GetY()[i]) / obs1.GetY()[i];
    obs1.GetY()[i] = diff;
  }

  ic::Plot::SetHTTStyle();
  TCanvas canv("canv","canv",600,600);
  gStyle->SetNdivisions(505);
  canv.SetGridx(1);
  canv.SetGridy(1);
  SetTGraphStyle(&exp1, 4);
  exp1.GetXaxis()->SetLimits(exp1.GetX()[0]-.1, exp1.GetX()[exp1.GetN()-1]+.1);
  exp1.SetMinimum(0.0);
  exp1.SetMaximum(0.2);
  exp1.GetYaxis()->SetTitleSize  (0.055);
  exp1.GetYaxis()->SetTitleOffset(1.200);
  exp1.GetYaxis()->SetLabelOffset(0.014);
  exp1.GetYaxis()->SetLabelSize  (0.040);
  exp1.GetYaxis()->SetLabelFont  (42);
  exp1.GetXaxis()->SetTitleSize  (0.055);
  exp1.GetXaxis()->SetTitleOffset(1.100);
  exp1.GetXaxis()->SetLabelOffset(0.014);
  exp1.GetXaxis()->SetLabelSize  (0.040);
  exp1.GetXaxis()->SetLabelFont  (42);
  exp1.Draw("APL");

  exp1.GetYaxis()->SetTitleFont(42);
  exp1.GetXaxis()->SetTitleFont(42);   
  exp1.GetXaxis()->SetTitle("m_{H} [GeV]");
  exp1.GetYaxis()->SetTitle("|pairs - presel| / pairs");
  SetTGraphStyle(&obs1, 6);
  SetTGraphStyle(&exp2, 7);
  SetTGraphStyle(&obs2, 8);

  obs1.Draw("PLsame");
  // exp2.Draw("PLsame");
  // obs2.Draw("PLsame");

  TLegend *legend = new TLegend(0.17,0.7,0.47,0.85,"","brNDC");
  legend->AddEntry(&exp1, "Expected","lP");
  legend->AddEntry(&obs1, "Observed","lP");
  legend->SetBorderSize(1);
  legend->SetTextFont(42);
  legend->SetLineColor(0);
  legend->SetLineStyle(1);
  legend->SetLineWidth(1);
  legend->SetFillColor(0);
  legend->SetFillStyle(1001);
  legend->Draw();

  canv.SaveAs((ch+"_comparison.pdf").c_str());

  return 0;
}

