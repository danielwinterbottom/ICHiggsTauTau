#include <iostream>
#include <vector>
#include <map>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TextElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SimpleParamParser.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

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
  ele.set_line_width(2);
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
  ele.set_marker_style(20);

  return;
}


int main(int argc, char* argv[]){

  std::cout << argc << " arguments passed to program: " << std::endl;
  for (int i = 0; i < argc; ++i) {
    std::cout << "-- " << argv[i] << std::endl;
  }

  // ic::channel channel = channel::zee;
  ic::channel channel = channel::zmm;

  std::string channel_str = Channel2String(channel);
  channel_str += "_NewJEC";

  double data_lumi = 18048;
  std::string lumi_str = "18 fb^{-1}";
  std::string plotname = "jpt_1";
  std::string plotfolder = "inclusive";
  // std::string x_axis_label = "M_{ll} [GeV]";
  // std::string x_axis_label = "Leading Jet #eta";
  std::string x_axis_label = "Leading Jet p_{T} [GeV]";

  double dy_evt   = 30459503;
  double dy_xs    = 3503.7;
  double tt_evt   = 6923750;
  double tt_xs    = 225.2;

  bool log_y = false;

  TFile f_data(("output/ZJets/Data_Moriond_"+channel_str+"_2012.root").c_str());
  TFile f_dy(("output/ZJets/DYJetsToLL_"+channel_str+"_2012.root").c_str());
  TFile f_tt(("output/ZJets/TTJets_"+channel_str+"_2012.root").c_str());

  ic::TH1PlotElement data("data",&f_data, "/"+plotfolder, plotname, "Data");
  ic::TH1PlotElement dy("dy",&f_dy, "/"+plotfolder, plotname, "Z #rightarrow ll");
  ic::TH1PlotElement tt("tt",&f_tt, "/"+plotfolder, plotname, "t#bar{t}");

  dy.hist_ptr()->Scale( data_lumi * dy_xs / dy_evt );
  tt.hist_ptr()->Scale( data_lumi * tt_xs / tt_evt );

  double data_norm = Integral(data.hist_ptr());
  double dy_norm = Integral(dy.hist_ptr());
  double tt_norm = Integral(tt.hist_ptr());

  // double data_norm = data.hist_ptr()->Integral(data.hist_ptr()->FindBin(-2.0), data.hist_ptr()->FindBin(2.0));
  // double dy_norm = dy.hist_ptr()->Integral(dy.hist_ptr()->FindBin(-2.0), dy.hist_ptr()->FindBin(2.0));
  // double tt_norm = tt.hist_ptr()->Integral(tt.hist_ptr()->FindBin(-2.0), tt.hist_ptr()->FindBin(2.0));


  double sf = data_norm / (dy_norm + tt_norm);
  std::cout << "Data Yield: " << data_norm << std::endl;
  std::cout << "MC SF: " << sf << std::endl;

  dy.hist_ptr()->Scale( sf );
  tt.hist_ptr()->Scale( sf );

  ic::Plot plot;

  plot.output_filename = "output/ZJets/"+plotfolder+"_"+plotname+"_" +channel_str+"_2012"+".pdf";
  if (log_y) plot.output_filename = "output/ZJets/"+plotfolder+"_"+plotname+"_" +channel_str+"_2012"+".pdf";;
  
  SetStyle(dy, kBlue  - 6);
  SetStyle(tt, kCyan  - 6);
  SetDataStyle(data);

  plot.AddTH1PlotElement(tt);
  plot.AddTH1PlotElement(dy);
  plot.AddTH1PlotElement(data);
  // plot.extra_pad = extra_pad;
  // plot.custom_x_axis_range = custom_x_axis_range;
  // if (custom_x_axis_range){
  //   plot.x_axis_min = x_axis_min;
  //   plot.x_axis_max = x_axis_max;
  // }
  // plot.legend_height = 0.045;
  plot.x_axis_title = x_axis_label;
  plot.y_axis_title = "Events";
  plot.title_left = "CMS Preliminary 2012, #sqrt{s} = 8 TeV, "+ lumi_str;
  plot.title_right = channel_str+", "+plotfolder;

  plot.legend_left = 0.72;

  plot.draw_ratio_hist = true;
  ic::RatioPlotElement ratio("Ratio","data","dy+tt");
  SetStyle(ratio,1);
  ratio.set_multi_mode(true);
  plot.ratio_y_axis_title = "Data/MC";
  plot.AddRatioPlotElement(ratio);
  plot.custom_ratio_y_axis_range = true;
  plot.ratio_y_axis_min = 0.5;
  plot.ratio_y_axis_max = 1.5;
  
  if (log_y) plot.y_axis_log = true;
  // if (y_axis_min > 0) plot.y_axis_min = y_axis_min;
  
  // // plot.y_axis_min = 5.0;
  // if (blind) {
  //   agilbert::TextElement text1("Data in ["+boost::lexical_cast<std::string>(x_blind_min)+","+boost::lexical_cast<std::string>(x_blind_max)+"] blinded",0.03, 0.16,0.89);
  //   plot.AddTextElement(text1);
  // }
  // ic::TextElement ss_text("Same-sign",0.04,0.19,0.89);
  // if (draw_ss) plot.AddTextElement(ss_text);

  plot.GeneratePlot();






  return 0;
}

