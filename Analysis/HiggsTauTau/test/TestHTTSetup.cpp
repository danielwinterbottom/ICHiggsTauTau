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
  ele.set_marker_size(1.0);
  return;
}

void SetStyle(ic::RatioPlotElement & ele, unsigned color) {
  ele.set_marker_color(color);
  ele.set_line_color(color);
  ele.set_draw_stat_error_y(true);
  ele.set_draw_line(false);
  ele.set_draw_marker(true);
  ele.set_marker_size(1.3);
  ele.set_marker_style(20);
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
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_mt_2_8TeV.txt","mt",2,"8TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_mt_2_7TeV.txt","mt",2,"7TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_et_2_8TeV.txt","et",2,"8TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_et_2_7TeV.txt","et",2,"7TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_em_2_8TeV.txt","em",2,"8TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_em_2_7TeV.txt","em",2,"7TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_mt_3_8TeV.txt","mt",3,"8TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_mt_3_7TeV.txt","mt",3,"7TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_et_3_8TeV.txt","et",3,"8TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_et_3_7TeV.txt","et",3,"7TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_em_3_8TeV.txt","em",3,"8TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_em_3_7TeV.txt","em",3,"7TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_mt_5_8TeV.txt","mt",5,"8TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_mt_5_7TeV.txt","mt",5,"7TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_et_5_8TeV.txt","et",5,"8TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_et_5_7TeV.txt","et",5,"7TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_em_5_8TeV.txt","em",5,"8TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_em_5_7TeV.txt","em",5,"7TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_tt_0_8TeV.txt","tt",0,"8TeV","125");
  setup.ParseDatacard("bin-by-bin/cmb/125/htt_tt_1_8TeV.txt","tt",1,"8TeV","125");

  setup.ParseROOTFile("bin-by-bin/cmb/common/htt_mt.input_8TeV.root","mt","8TeV");
  setup.ParseROOTFile("bin-by-bin/cmb/common/htt_mt.input_7TeV.root","mt","7TeV");
  setup.ParseROOTFile("bin-by-bin/cmb/common/htt_et.input_8TeV.root","et","8TeV");
  setup.ParseROOTFile("bin-by-bin/cmb/common/htt_et.input_7TeV.root","et","7TeV");
  setup.ParseROOTFile("bin-by-bin/cmb/common/htt_em.input_8TeV.root","em","8TeV");
  setup.ParseROOTFile("bin-by-bin/cmb/common/htt_em.input_7TeV.root","em","7TeV");
  setup.ParseROOTFile("bin-by-bin/cmb/common/htt_tt.input_8TeV.root","tt","8TeV");

  setup.VariableRebin({0., 20., 40., 60., 80., 100., 120., 140., 160., 180., 200., 250., 300., 350.});
  setup.ParsePulls("bin-by-bin/cmb/125/out/mlfit.txt");
  setup.ApplyPulls();
  std::cout << boost::format("%-20s %-10.1f +/- %-10.1f\n") % "Z->TauTau" 
    % setup.process({"ZTT","Ztt"}).GetRate() 
    % setup.process({"ZTT","Ztt"}).GetUncertainty();

  std::cout << boost::format("%-20s %-10.1f +/- %-10.1f\n") % "QCD" 
    % setup.process({"QCD","Fakes"}).GetRate() 
    % setup.process({"QCD","Fakes"}).GetUncertainty();

  std::cout << boost::format("%-20s %-10.1f +/- %-10.1f\n") % "EWK" 
    % setup.process({"W","ZL","ZJ","ZLL","VV","EWK"}).GetRate() 
    % setup.process({"W","ZL","ZJ","ZLL","VV","EWK"}).GetUncertainty();

  std::cout << boost::format("%-20s %-10.1f +/- %-10.1f\n") % "ttbar" 
    % setup.process({"TT","ttbar"}).GetRate() 
    % setup.process({"TT","ttbar"}).GetUncertainty();

  std::cout << boost::format("%-20s %-10.1f +/- %-10.1f\n") % "Total" 
    % setup.process({"ZTT","ZL","ZJ","ZLL","W","QCD","VV","TT","Ztt","Fakes","EWK","ttbar"}).GetRate() 
    % setup.process({"ZTT","ZL","ZJ","ZLL","W","QCD","VV","TT","Ztt","Fakes","EWK","ttbar"}).GetUncertainty();

  std::cout << boost::format("%-20s %-10.1f +/- %-10.1f\n") % "H->TauTau" 
    % setup.process({"ggH","VH","qqH"}).GetRate() 
    % setup.process({"ggH","VH","qqH"}).GetUncertainty();

    std::cout << boost::format("%-20s %-10.1f \n\n") % "Observed" 
      % setup.GetObservedRate();
  double sig_before = setup.signals().GetRate();
  setup.WeightSoverB();
  double sig_after = setup.signals().GetRate();




  ic::Plot plot;
  plot.use_htt_style = true;
  plot.output_filename = "test_plot.pdf";

  TH1F signal_hist = setup.process({"ggH","VH","qqH"}).GetShape();
  ic::TH1PlotElement signal_shape("signal_shape", &signal_hist, "1#times H(125)#rightarrow#tau#tau");
  SetStyle(signal_shape, kRed);
  // signal_shape.set_line_style(11);
  signal_shape.set_fill_color(kRed);
  signal_shape.set_fill_style(3004);
  signal_shape.set_line_color(kRed);
  signal_shape.set_line_width(2);

  TH1F ztt_hist = setup.process({"ZTT","Ztt"}).GetShape();
  ic::TH1PlotElement ztt_shape("ztt_shape", &ztt_hist, "Z#rightarrow#tau#tau");
  SetStyle(ztt_shape, kOrange - 4);
  std::cout << ztt_hist.Integral() << std::endl;

  TH1F qcd_hist = setup.process({"QCD","Fakes"}).GetShape();
  ic::TH1PlotElement qcd_shape("qcd_shape", &qcd_hist, "QCD");
  SetStyle(qcd_shape, kMagenta-10);


  TH1F ewk_hist = setup.process({"W","ZL","ZJ","ZLL","VV","EWK"}).GetShape();
  ic::TH1PlotElement ewk_shape("ewk_shape", &ewk_hist, "electroweak");
  SetStyle(ewk_shape, kRed    + 2);


  TH1F top_hist = setup.process({"TT","ttbar"}).GetShape();
  ic::TH1PlotElement top_shape("top_shape", &top_hist, "t#bar{t}");
  SetStyle(top_shape, kBlue   - 8);

  TH1F total_hist = setup.process({"ZTT","ZL","ZJ","ZLL","W","QCD","VV","TT","Ztt","Fakes","EWK","ttbar"}).GetShape();
  ic::TH1PlotElement total_shape("total_shape", &total_hist,"bkg. uncertainty");

  TH1F copy_hist = total_hist;
  for (int i = 1; i <= copy_hist.GetNbinsX(); ++i) copy_hist.SetBinError(i, 0);

  TH1F err_hist = total_hist;
  for (int i = 1; i <= err_hist.GetNbinsX(); ++i) err_hist.SetBinContent(i, 0);
  ic::TH1PlotElement err_shape("err_shape", &err_hist,"bkg. uncertainty");

  TH1F data_hist = setup.GetObservedShape();

  ic::TH1PlotElement data_shape("data_shape", &data_hist, "observed");
  TH1F diff_hist = data_hist;
  diff_hist.Add(&copy_hist, -1.);
  ic::TH1PlotElement diff_shape("diff_shape", &diff_hist,"Data - Background");
  SetDataStyle(data_shape);
  SetDataStyle(diff_shape);

  std::vector<ic::TH1PlotElement *> drawn_ele;
  drawn_ele.push_back(&qcd_shape);
  drawn_ele.push_back(&ewk_shape);
  drawn_ele.push_back(&ztt_shape);
  drawn_ele.push_back(&top_shape);
  drawn_ele.push_back(&signal_shape);
  drawn_ele.push_back(&data_shape);
  drawn_ele.push_back(&diff_shape);
  drawn_ele.push_back(&err_shape);
  drawn_ele.push_back(&total_shape);

  for (unsigned i = 0; i < drawn_ele.size(); ++i) {
     drawn_ele[i]->set_line_width(2);
     drawn_ele[i]->hist_ptr()->Scale(sig_before/sig_after);
     drawn_ele[i]->hist_ptr()->Scale(1.0, "width");

  }
  total_shape.set_marker_size(0);
  total_shape.set_fill_color(1);
  total_shape.set_fill_style(3013);
  total_shape.set_line_width(1);
  total_shape.set_draw_stat_error_y(true);
  total_shape.set_draw_fill(true);
  total_shape.set_draw_line(false);
  total_shape.set_draw_marker(false);
  total_shape.set_draw_options("e2");

  err_shape.set_marker_size(0.0);
  err_shape.set_fill_color(1);
  err_shape.set_fill_style(3013);
  err_shape.set_line_width(3);
  err_shape.set_draw_stat_error_y(false);
  err_shape.set_draw_fill(true);
  err_shape.set_draw_line(false);
  err_shape.set_draw_marker(false);
  err_shape.set_draw_options("HISTE2");

  plot.AddTH1PlotElement(qcd_shape);
  plot.AddTH1PlotElement(top_shape);
  plot.AddTH1PlotElement(ewk_shape);
  plot.AddTH1PlotElement(ztt_shape);
  plot.AddTH1PlotElement(signal_shape);
  plot.AddTH1PlotElement(total_shape);
  plot.AddTH1PlotElement(data_shape);
  plot.legend_height = 0.045;
  plot.x_axis_title = "M_{#tau#tau} [GeV]";
  plot.y_axis_title = "S/B Weighted dN/dm_{#tau#tau} [1/GeV]";
  plot.title_left = "CMS Preliminary, #sqrt{s} = 7-8 TeV, L = 24.3 fb^{-1}";
  ic::TextElement text("e#mu, e#tau_{h}, #mu#tau_{h}, #tau_{h}#tau_{h}",0.04,0.20,0.85);
  plot.AddTextElement(text);
  plot.extra_pad = 1.2;
  plot.GeneratePlot();

  signal_shape.set_draw_options("][HIST");

  ic::Plot diffplot;
  signal_shape.set_in_stack(false);
  diffplot.use_htt_style = true;
  diffplot.output_filename = "test_diff_plot.pdf";
  diffplot.AddTH1PlotElement(err_shape);
  diffplot.AddTH1PlotElement(signal_shape);
  diffplot.AddTH1PlotElement(diff_shape);
  diffplot.legend_height = 0.045;
  diffplot.x_axis_title = "M_{#tau#tau} [GeV]";
  diffplot.y_axis_title = "";
  diffplot.custom_x_axis_range = true;
  diffplot.x_axis_min = 60;
  diffplot.x_axis_max = 179;
  // diffplot.custom_y_axis_range = true;
  // diffplot.y_axis_min = -0.1;
  // diffplot.y_axis_max = 0.1;
  diffplot.extra_pad = 1.5;
  diffplot.title_left = "CMS Preliminary, #sqrt{s} = 7-8 TeV, L = 24.3 fb^{-1}";
  diffplot.GeneratePlot();


  return 0;
}

