#include <iostream>
#include <vector>
#include <map>
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraphErrors.h"

#include "TLatex.h"
#include "TH2F.h"
#include "TStyle.h"


#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TextElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTStatTools.h"


namespace po = boost::program_options;

using namespace std;
using namespace ic;

double Integral(TH1F const* hist) {
  return hist->Integral(0, hist->GetNbinsX() + 1);
}

double Error(TH1F const* hist) {
  double err = 0.0;
  hist->IntegralAndError(0, hist->GetNbinsX() + 1, err);
  return err;
}

void SetStyle(ic::TH1PlotElement & ele, unsigned color) {
  //ele.set_marker_color(color);
  //ele.set_line_color(color);
  ele.set_fill_color(color);
  ele.set_fill_style(1001);
  ele.set_draw_fill(true);
  ele.set_draw_marker(false);
  ele.set_draw_line(false);
  ele.set_line_width(3);
  ele.set_draw_stat_error_y(false);
  ele.set_in_stack(true);
  return;
}

void SetDataStyle(ic::TH1PlotElement & ele) {
  ele.set_marker_color(1);
  ele.set_line_color(1);
  ele.set_fill_color(1);
  ele.set_fill_style(0);
  ele.set_draw_fill(false);
  ele.set_line_width(3);
  ele.set_draw_marker(true);
  ele.set_draw_line(true);
  ele.set_marker_style(20);
  ele.set_draw_stat_error_y(true);
  ele.set_marker_size(1.3);

  //ele.set_draw_normalised(true);

  //ele.set_in_stack(true);
  return;
}

void SetStyle(ic::RatioPlotElement & ele, unsigned color) {
  ele.set_marker_color(color);
  ele.set_line_color(color);
  ele.set_draw_stat_error_y(true);
  ele.set_draw_line(false);
  ele.set_draw_marker(true);
  return;
}


int main(int argc, char* argv[]){


  // po::options_description preconfig("Pre-Configuration");
  // po::options_description config("Configuration");
  // preconfig.add_options()
  //   ("input1",               po::value<string>(&input1)->required(), "inputs")
  //   ("input2",               po::value<string>(&input2)->required(), "labels")
  //   ("name1",               po::value<string>(&name1)->default_value(""), "name1")
  //   ("name2",               po::value<string>(&name2)->default_value(""), "name2")
  //   ("splusb_1",             po::value<bool>(&splusb_1)->required(), "category")
  //   ("splusb_2",             po::value<bool>(&splusb_2)->required(), "output_file")
  //   ("bbb_treatment",        po::value<int>(&bbb_treatment)->required(), "output_file");

  
  // po::variables_map vm;
  // po::store(po::command_line_parser(argc, argv).
  //           options(preconfig).allow_unregistered().run(), vm);
  // po::notify(vm);

  HTTSetup setup;
  setup.ParseDatacard("./htt_et_3_8TeV-125.txt");
  setup.ParseDatacard("./htt_et_3_7TeV-125.txt");
  setup.ParseROOTFile("./htt_et.inputs-sm-8TeV.root");
  setup.ParseROOTFile("./htt_et.inputs-sm-7TeV.root");
  //{"ZTT","ZL","ZJ","ZLL","W","QCD","VV","TT"}
  std::cout << setup.process({"ggH","VH","qqH"}).PrintAll().GetRate() << std::endl;
  std::cout << setup.process({"ggH","VH","qqH"}).GetUncertainty() << std::endl;
  // setup = setup.process({"ZTT"});
  setup.ParsePulls("mlfit-em+et+mt+mm+tt-130209-combined.txt");
  setup.ApplyPulls();
  // setup.process({"ZTT"}).PrintAll();

  std::cout << setup.process({"ggH","VH","qqH"}).PrintAll().GetRate() << std::endl;
  std::cout << setup.process({"ggH","VH","qqH"}).GetUncertainty() << std::endl;


  
  return 0;
}

