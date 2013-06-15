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
  ele.set_draw_line(false);
  ele.set_line_width(1);
  ele.set_draw_fill(false);
  ele.set_draw_stat_error_y(true);
  ele.set_marker_style(20);
  ele.set_marker_size(1.2);
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
  string mssm_file = "";
  string append = "";
  string channel = ""; // muTau
  string prod = "";
  // string fit_input = "";
  // string extrap = "";
  // bool   no_rms = false;
  // bool   force_yield = false;

  po::options_description config("Configuration");
  config.add_options()
    ("input",                 po::value<string>(&input)->required(), "input")
    ("mssm_file",             po::value<string>(&mssm_file)->required(), "mssm_file")
    ("append",                po::value<string>(&append)->default_value(""), "append")
    ("channel",               po::value<string>(&channel)->required(), "channel")
    ("prod",                  po::value<string>(&prod)->required(), "prod");
    // ("fit_input",             po::value<string>(&fit_input)->default_value("110,115,120,125,130,135,140,145"), "fit_input")
    // ("no_rms",                po::value<bool>(&no_rms)->default_value(false), "fit_input")
    // ("force_yield",           po::value<bool>(&force_yield)->default_value(false), "force_yield")
    // ("extrap",                po::value<std::string>(&extrap)->required(), "extrap");
    // 145=135,125,115,105:
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::notify(vm);



  Plot::SetTdrStyle();

  TFile *input_dc = new TFile(input.c_str(),"UPDATE");
  cout << "Updating file: " << input << endl;
  cout << "Using channel: " << channel << endl;
  // cout << "Applying to signal samples: " << prod << endl;
  // cout << "Fit points:    " << fit_input << endl;
  cout << "Appending:     " << append << endl;

  std::vector<string> cats;
  cats.push_back(channel+"_vbf");
  cats.push_back(channel+"_boost");
  cats.push_back(channel+"_boost_high");
  cats.push_back(channel+"_boost_low");
  cats.push_back(channel+"_0jet_high");
  cats.push_back(channel+"_0jet_low");
  cats.push_back(channel+"_inclusive");

  std::vector<string> cat_src;
  cat_src.push_back("vbf_loose_os_sel");
  cat_src.push_back("1jet_os_sel");
  cat_src.push_back("1jet_high_os_sel");
  cat_src.push_back("1jet_low_os_sel");
  cat_src.push_back("0jet_high_os_sel");
  cat_src.push_back("0jet_low_os_sel");
  cat_src.push_back("inclusive_os_sel");

  std::vector<string> hist_nm;
  hist_nm.push_back("m_sv_sm");
  hist_nm.push_back("m_sv_sm_fine");
  hist_nm.push_back("m_sv_sm_fine");
  hist_nm.push_back("m_sv_sm_fine");
  hist_nm.push_back("m_sv_sm_fine");
  hist_nm.push_back("m_sv_sm_fine");
  hist_nm.push_back("m_sv_sm_fine");


  TFile *input_mssm = new TFile(mssm_file.c_str());


  for (unsigned i = 0; i < cats.size(); ++i) {
      input_mssm->cd();
      if (!gDirectory->cd(("/"+cat_src[i]).c_str())) continue;
      TH1F * mssm = (TH1F*)gDirectory->Get((hist_nm[i]).c_str());
      input_dc->cd();
      if (!gDirectory->cd(("/"+cats[i]).c_str())) continue;
      TH1F * ref = (TH1F*)gDirectory->Get((prod+"90"+append).c_str());
      mssm->Scale(ref->Integral() / mssm->Integral());

      ic::Plot compare;
      compare.output_filename = "mssm_test_"+cats[i]+"_"+prod+append+".pdf";
      ic::TH1PlotElement p1("extrap", ref, "Extrapolated");
      ic::TH1PlotElement p2("mssm", mssm , "MSSM");

      SetStyle(p1, 1);
      SetStyle(p2, 2);
      compare.AddTH1PlotElement(p1);
      compare.AddTH1PlotElement(p2);
      compare.x_axis_title = "M_{#tau#tau} [GeV]";
      compare.GeneratePlot();
    
      mssm->SetName((prod+"90"+append).c_str());
      mssm->SetTitle((prod+"90"+append).c_str());
      mssm->Write("",TObject::kOverwrite);
  }



  input_dc->Close();




  return 0;
}
