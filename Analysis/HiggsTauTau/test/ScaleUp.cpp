#include <iostream>
#include <vector>
#include <map>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "boost/algorithm/string.hpp"
#include "TROOT.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"

#include "TGraph.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"

#include "RooBinning.h"
#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooDataHist.h"
#include "RooAbsPdf.h"
#include "RooVoigtian.h"
#include "RooCBShape.h"
#include "RooBifurGauss.h"
#include "RooAddPdf.h"
#include "RooExponential.h"
#include "RooSimultaneous.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooFFTConvPdf.h"
#include "RooBreitWigner.h"
#include "boost/format.hpp"
#include "RooMsgService.h"

namespace po = boost::program_options;

using namespace std;


double Integral(TH1F const* hist) {
  return hist->Integral(0, hist->GetNbinsX() + 1);
}

double Error(TH1F const* hist) {
  double err = 0.0;
  hist->IntegralAndError(0, hist->GetNbinsX() + 1, err);
  return err;
}

void SetStyle(ic::TH1PlotElement & ele, unsigned color) {
  ele.set_marker_color(color);
  ele.set_line_color(color);
  ele.set_draw_line(true);
  ele.set_draw_fill(true);
  ele.set_line_width(3);
  ele.set_draw_stat_error_y(false);
  return;
}

void SetStyle(ic::RatioPlotElement & ele, unsigned color) {
  static bool first_time = true;
  static vector<int> colors;
  if (first_time) {
    colors.push_back(4);
    colors.push_back(2);
    colors.push_back(8);
    colors.push_back(9);
    colors.push_back(44);
    first_time = false;
  }
  unsigned color_idx = colors.at(color);
  ele.set_marker_color(color_idx);
  ele.set_line_color(color_idx);
  ele.set_draw_stat_error_y(true);
  ele.set_draw_line(false);
  ele.set_draw_marker(true);
  ele.set_marker_size(0.7);
  ele.set_marker_style(20);
  return;
}



int main(int argc, char* argv[]){
  using namespace ic;
  using namespace std;

  string input = "";
  string append = "";
  string channel = ""; // muTau

  po::options_description config("Configuration");
  config.add_options()
    ("input",                 po::value<string>(&input)->required(), "input")
    ("append",                po::value<string>(&append)->default_value(""), "append")
    ("channel",               po::value<string>(&channel)->required(), "channel");
    // 145=135,125,115,105:
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::notify(vm);


  Plot::SetTdrStyle();


  TFile *input_dc = new TFile(input.c_str(),"UPDATE");
  cout << "Updating file: " << input << endl;
  cout << "Using channel: " << channel << endl;
  cout << "Appending:     " << append << endl;


  std::vector<string> cats;
  cats.push_back(channel+"_vbf");
  cats.push_back(channel+"_boost");
  cats.push_back(channel+"_boost_high");
  cats.push_back(channel+"_boost_low");
  cats.push_back(channel+"_0jet_high");
  cats.push_back(channel+"_0jet_low");
  cats.push_back(channel+"_inclusive");

  for (unsigned i = 0; i < cats.size(); ++i) {
    if (!gDirectory->cd(("/"+cats[i]).c_str())) continue;
    TH1F *hist = (TH1F*)(gDirectory->Get(("ZTT"+append).c_str())->Clone());
    hist->Scale(1.05);
    hist->Write("",TObject::kOverwrite);
  }

  input_dc->Close();

  return 0;
}
