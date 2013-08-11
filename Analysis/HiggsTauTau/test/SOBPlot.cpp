#include <iostream>
#include <vector>
#include <map>
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraphErrors.h"

#include "TLatex.h"
#include "TH2F.h"
#include "TStyle.h"
#include "THStack.h"

#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TextElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTStatTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTPlotTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTAnalysisTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/mssm_xs_tools.h"

namespace po = boost::program_options;

using namespace std;
using namespace ic;

void SetMCStackStyle(TH1F & ele, unsigned color) {
  ele.SetFillColor(color);
  ele.SetFillStyle(1001);
  ele.SetLineWidth(2);
  return; 
}
void SetSignalStyle(TH1F & ele, unsigned color) {
  ele.SetFillStyle(1001);
  ele.SetLineStyle(11);
  ele.SetFillColor(0);
  ele.SetLineColor(color);
  ele.SetLineWidth(2);
  return;
}
void SetDataStyle(TH1F & ele) {
  ele.SetMarkerColor(1);
  ele.SetLineColor(1);
  ele.SetFillColor(1);
  ele.SetFillStyle(0);
  ele.SetLineWidth(2);
  ele.SetMarkerStyle(20);
  ele.SetMarkerSize(1.1);
  return;
}


int main(int argc, char* argv[]){
  string cfg;                                   // The configuration file
  string datacard_regex   = "";
  string root_file_regex  = "";
  string pulls_file       = "";
  string datacard_path    = "";
  string root_file_path   = "";
  bool postfit          = true;
  bool mssm             = false;

  po::options_description preconfig("Pre-Configuration");
  preconfig.add_options()("cfg", po::value<std::string>(&cfg)->required());
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
    ("help,h",  "print the help message")
    ("datacard_regex",       po::value<string>(&datacard_regex)->required(),   "[REQUIRED] regular expression for datacards to parse")
    ("root_file_regex",      po::value<string>(&root_file_regex)->required(),  "[REQUIRED] regular expression for root files to parse")
    ("datacard_path",        po::value<string>(&datacard_path)->required(),   "[REQUIRED] path to the folder containing datacard *.txt files")
    ("root_file_path",       po::value<string>(&root_file_path)->required(),  "[REQUIRED] path to the folder containing datacard *.root files")
    ("pulls_file",           po::value<string>(&pulls_file)->required(),      "[REQUIRED] path to the file containing the pulls from a maximum-likelihood fit")
    ("postfit",              po::value<bool>(&postfit)->required(),           "[REQUIRED] use the pulls file to make a post-fit plot")
    ("mssm",                 po::value<bool>(&mssm)->default_value(false),                   "input is an MSSM datacard");
  HTTPlot plot;
  config.add(plot.GenerateOptions("")); // The string here is a prefix for the options parameters
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config, true), vm);
  po::notify(vm);

  HTTSetup setup;

  boost::filesystem::path datacard_folder(datacard_path);
  boost::filesystem::directory_iterator dir_it(datacard_folder);
  boost::regex datacard_rgx(datacard_regex);
  for (; dir_it != boost::filesystem::directory_iterator(); ++dir_it) {
    std::string path = dir_it->path().filename().string();
    if (boost::regex_match(path, datacard_rgx)) {
      std::cout << "Parsing datacard: " << dir_it->path().string() << std::endl;
      setup.ParseDatacard(dir_it->path().string(), plot.draw_signal_mass());
    }
  }

  boost::filesystem::path file_folder(root_file_path);
  dir_it = boost::filesystem::directory_iterator(file_folder);
  boost::regex file_rgx(root_file_regex);
  for (; dir_it != boost::filesystem::directory_iterator(); ++dir_it) {
    std::string path = dir_it->path().filename().string();
    if (boost::regex_match(path, file_rgx)) {
      std::cout << "Parsing ROOT file: " << dir_it->path().string() << std::endl;
      setup.ParseROOTFile(dir_it->path().string());
    }
  }

  setup.WeightSoverB();

  setup.VariableRebin({0., 20., 40., 60., 80., 100., 120., 140., 160., 180., 200., 250., 300., 350.});

  setup.PrintAll();

  vector<string> samples = {"ZTT","QCD","W","ZL","ZJ","ZLL","VV","TT","Ztt","Fakes","EWK","ttbar"};

  vector<string> signal_procs = {"ggH", "qqH", "VH"};
  if (mssm) signal_procs = {"ggH","bbH"};

  Plot::SetHTTStyle();
  TCanvas canv;

  TH1F signal_hist = setup.process(signal_procs).GetShape();
  // ic::TH1PlotElement signal_shape("signal_shape", &signal_hist, "1#times H(125)#rightarrow#tau#tau");
  SetMCStackStyle(signal_hist, kRed);
  signal_hist.SetFillColor(kRed);
  signal_hist.SetFillStyle(3004);
  signal_hist.SetLineColor(kRed);
  signal_hist.SetLineWidth(2);

  TH1F ztt_hist = setup.process({"ZTT","Ztt"}).GetShape();
  // ic::TH1PlotElement ztt_shape("ztt_shape", &ztt_hist, "Z#rightarrow#tau#tau");
  SetMCStackStyle(ztt_hist, kOrange - 4);

  TH1F qcd_hist = setup.process({"QCD","Fakes"}).GetShape();
  // ic::TH1PlotElement qcd_shape("qcd_shape", &qcd_hist, "QCD");
  SetMCStackStyle(qcd_hist, kMagenta-10);

  TH1F ewk_hist = setup.process({"W","ZL","ZJ","ZLL","VV","EWK"}).GetShape();
  // ic::TH1PlotElement ewk_shape("ewk_shape", &ewk_hist, "electroweak");
  SetMCStackStyle(ewk_hist, kRed    + 2);

  TH1F top_hist = setup.process({"TT","ttbar"}).GetShape();
  // ic::TH1PlotElement top_shape("top_shape", &top_hist, "t#bar{t}");
  SetMCStackStyle(top_hist, kBlue   - 8);

  TH1F total_hist = setup.process({"ZTT","ZL","ZJ","ZLL","W","QCD","VV","TT","Ztt","Fakes","EWK","ttbar"}).GetShape();
  // ic::TH1PlotElement total_shape("total_shape", &total_hist,"bkg. uncertainty");


  total_hist.SetMarkerSize(0);
  total_hist.SetFillColor(1);
  total_hist.SetFillStyle(3013);
  total_hist.SetLineWidth(1);
  // total_shape.set_draw_options("e2");

  TH1F copy_hist = total_hist;
  for (int i = 1; i <= copy_hist.GetNbinsX(); ++i) copy_hist.SetBinError(i, 0);

  TH1F err_hist = total_hist;
  for (int i = 1; i <= err_hist.GetNbinsX(); ++i) err_hist.SetBinContent(i, 0);
  // ic::TH1PlotElement err_shape("err_shape", &err_hist,"bkg. uncertainty");
  err_hist.SetMarkerSize(0.0);
  err_hist.SetFillColor(1);
  err_hist.SetFillStyle(3013);
  err_hist.SetLineWidth(3);
  // err_hist.set_draw_options("HISTE2");

  TH1F data_hist = setup.GetObservedShape();
  SetDataStyle(data_hist);
  BlindHistogram(&data_hist, 100, 160);

  TH1F diff_hist = data_hist;
  diff_hist.Add(&copy_hist, -1.);
  // ic::TH1PlotElement diff_shape("diff_shape", &diff_hist,"Data - Background");
  SetDataStyle(diff_hist);
  

  vector<TH1F *> drawn_hists;
  drawn_hists.push_back(&qcd_hist);
  drawn_hists.push_back(&ewk_hist);
  drawn_hists.push_back(&ztt_hist);
  drawn_hists.push_back(&top_hist);
  drawn_hists.push_back(&signal_hist);
  drawn_hists.push_back(&data_hist);
  drawn_hists.push_back(&diff_hist);
  drawn_hists.push_back(&err_hist);
  drawn_hists.push_back(&total_hist);
 

  for (unsigned i = 0; i < drawn_hists.size(); ++i) {
     drawn_hists[i]->SetLineWidth(2);
     // drawn_hists[i]->hist_ptr()->Scale(sig_before/sig_after);
     drawn_hists[i]->Scale(1.0, "width");
  }

  THStack thstack("stack","stack");
  thstack.Add(&qcd_hist, "HIST");
  thstack.Add(&top_hist, "HIST");
  thstack.Add(&ewk_hist, "HIST");
  thstack.Add(&ztt_hist, "HIST");
  thstack.Add(&signal_hist, "HIST");

  thstack.Draw();
  total_hist.Draw("SAMEE2");
  data_hist.Draw("SAME");

  TPad padBack("padBack","padBack",0.53,0.52,0.975,0.956);//TPad must be created after TCanvas otherwise ROOT crashes
  padBack.SetFillColor(0);

  TPad pad("diff","diff",0.45,0.45,0.9765,0.961);//TPad must be created after TCanvas otherwise ROOT crashes
  pad.cd();
  pad.SetFillColor(0);
  pad.SetFillStyle(0);
  err_hist.GetYaxis()->SetNdivisions(5);
  err_hist.GetYaxis()->SetLabelSize(0.06);
  err_hist.GetXaxis()->SetTitle("#bf{m_{#tau#tau} [GeV]}    ");
  err_hist.GetXaxis()->SetTitleColor(kBlack);
  err_hist.GetXaxis()->SetTitleSize(0.07);
  err_hist.GetXaxis()->SetTitleOffset(0.95);
  err_hist.GetXaxis()->SetLabelSize(0.06);
  err_hist.SetNdivisions(505);
  err_hist.Draw("E2");
  err_hist.GetXaxis()->SetRangeUser(60,179);
  signal_hist.Draw("HISTSAME");
  diff_hist.Draw("SAME");
  pad.RedrawAxis();

  canv.cd();
  padBack.Draw();
  pad.Draw();
  canv.SaveAs("test_sob.pdf");

  // plot.GeneratePlot(hmap);


  // setup.ParsePulls("bin-by-bin/cmb/125/out/mlfit.txt");
  // setup.ApplyPulls();



  // ic::TH1PlotElement data_shape("data_shape", &data_hist, "observed");




  // plot.AddTH1PlotElement(qcd_shape);
  // plot.AddTH1PlotElement(top_shape);
  // plot.AddTH1PlotElement(ewk_shape);
  // plot.AddTH1PlotElement(ztt_shape);
  // plot.AddTH1PlotElement(signal_shape);
  // plot.AddTH1PlotElement(total_shape);
  // plot.AddTH1PlotElement(data_shape);
  // plot.legend_height = 0.045;
  // plot.x_axis_title = "M_{#tau#tau} [GeV]";
  // plot.y_axis_title = "S/B Weighted dN/dm_{#tau#tau} [1/GeV]";
  // plot.title_left = "CMS Preliminary, #sqrt{s} = 7-8 TeV, L = 24.3 fb^{-1}";
  // ic::TextElement text("e#mu, e#tau_{h}, #mu#tau_{h}, #tau_{h}#tau_{h}",0.04,0.20,0.85);
  // plot.AddTextElement(text);
  // plot.extra_pad = 1.2;
  // plot.GeneratePlot();

  // signal_shape.set_draw_options("][HIST");

  // ic::Plot diffplot;
  // signal_shape.set_in_stack(false);
  // diffplot.use_htt_style = true;
  // diffplot.output_filename = "test_diff_plot.pdf";
  // diffplot.AddTH1PlotElement(err_shape);
  // diffplot.AddTH1PlotElement(signal_shape);
  // diffplot.AddTH1PlotElement(diff_shape);
  // diffplot.legend_height = 0.045;
  // diffplot.x_axis_title = "M_{#tau#tau} [GeV]";
  // diffplot.y_axis_title = "";
  // diffplot.custom_x_axis_range = true;
  // diffplot.x_axis_min = 60;
  // diffplot.x_axis_max = 179;
  // // diffplot.custom_y_axis_range = true;
  // // diffplot.y_axis_min = -0.1;
  // // diffplot.y_axis_max = 0.1;
  // diffplot.extra_pad = 1.5;
  // diffplot.title_left = "CMS Preliminary, #sqrt{s} = 7-8 TeV, L = 24.3 fb^{-1}";
  // diffplot.GeneratePlot();


  return 0;
}

