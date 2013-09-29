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
#include "TLine.h"

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

void LegendStyle(TLegend *legend) {
  legend->SetBorderSize(1);
  legend->SetTextFont(42);
  legend->SetLineColor(0);
  legend->SetLineStyle(1);
  legend->SetLineWidth(1);
  legend->SetFillColor(0);
  legend->SetFillStyle(1001);
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

  if (postfit) {
    setup.ParsePulls(pulls_file);
    setup.ApplyPulls();
  } 

  // setup.SetIgnoreNuisanceCorrelations(true);
  // setup = setup.no_shapes();

  double sig_yield_before = setup.signals().GetRate();
  setup.WeightSoverB();
  double sig_yield_after = setup.signals().GetRate();
  double scale_all = sig_yield_before / sig_yield_after;

  // setup.VariableRebin({0., 20., 40., 60., 80., 100., 120., 140., 160., 180., 200., 250., 300., 350.});

  vector<string> samples = {"ZTT","QCD","W","ZL","ZJ","ZLL","VV","TT","Ztt","Fakes","EWK","ttbar"};

  vector<string> signal_procs = {"ggH", "qqH", "VH"};
  if (mssm) signal_procs = {"ggH","bbH"};

  Plot::SetHTTStyle();
  TCanvas canv;

  TH1F signal_hist = setup.process(signal_procs).GetShape();
  SetMCStackStyle(signal_hist, kRed);
  signal_hist.SetFillColor(kRed);
  signal_hist.SetFillStyle(3004);
  signal_hist.SetLineColor(kRed);
  signal_hist.SetLineWidth(2);
  signal_hist.SetTitle(("H("+plot.draw_signal_mass()+")#rightarrow#tau#tau").c_str());

  TH1F ztt_hist = setup.process({"ZTT","Ztt"}).GetShape();
  SetMCStackStyle(ztt_hist, kOrange - 4);
  ztt_hist.SetTitle("Z#rightarrow#tau#tau");

  TH1F qcd_hist = setup.process({"QCD","Fakes"}).GetShape();
  SetMCStackStyle(qcd_hist, kMagenta-10);
  qcd_hist.SetTitle("QCD");

  TH1F ewk_hist = setup.process({"W","ZL","ZJ","ZLL","VV","EWK"}).GetShape();
  SetMCStackStyle(ewk_hist, kRed    + 2);
  ewk_hist.SetTitle("electroweak");

  TH1F top_hist = setup.process({"TT","ttbar"}).GetShape();
  SetMCStackStyle(top_hist, kBlue   - 8);
  top_hist.SetTitle("t#bar{t}");

  // total_hist will be used to draw the background error on the main plot
  TH1F total_hist = setup.process({"ZTT","ZL","ZJ","ZLL","W","QCD","VV","TT","Ztt","Fakes","EWK","ttbar"}).GetShape();
  total_hist.SetMarkerSize(0);
  total_hist.SetFillColor(1);
  total_hist.SetFillStyle(3013);
  total_hist.SetLineWidth(1);
  total_hist.SetTitle("bkg. uncertainty");


  // copy_hist removes the bin errors from total_hist, and will be used
  // to determine the data-bkg histogram for the inset plot
  TH1F copy_hist = total_hist;
  for (int i = 1; i <= copy_hist.GetNbinsX(); ++i) copy_hist.SetBinError(i, 0);

  // err_hist removes the bin contents from total_hist, and will be used
  // to draw the error band on the inset plot
  TH1F err_hist = total_hist;
  for (int i = 1; i <= err_hist.GetNbinsX(); ++i) err_hist.SetBinContent(i, 0);
  err_hist.SetMarkerSize(0.0);
  err_hist.SetFillColor(1);
  err_hist.SetFillStyle(3013);
  err_hist.SetLineWidth(3);
  err_hist.SetTitle("bkg. uncertainty");

  TH1F data_hist = setup.GetObservedShape();
  SetDataStyle(data_hist);
  data_hist.SetTitle("observed");
  if (plot.blind()) BlindHistogram(&data_hist, plot.x_blind_min(), plot.x_blind_max());

  TH1F diff_hist = data_hist;
  diff_hist.Add(&copy_hist, -1.);
  SetDataStyle(diff_hist);
  diff_hist.SetTitle("observed - bkg.");

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
     drawn_hists[i]->Scale(scale_all);
     drawn_hists[i]->Scale(1.0, "width");
  }

  THStack thstack("stack","stack");
  thstack.Add(&qcd_hist, "HIST");
  thstack.Add(&top_hist, "HIST");
  thstack.Add(&ewk_hist, "HIST");
  thstack.Add(&ztt_hist, "HIST");
  thstack.Add(&signal_hist, "HIST");

  thstack.SetMaximum(thstack.GetMaximum()*1.1*plot.extra_pad());
  thstack.Draw();
  thstack.GetXaxis()->SetTitle(plot.x_axis_label().c_str());
  thstack.GetYaxis()->SetTitle(plot.y_axis_label().c_str());
  thstack.GetHistogram()->SetTitleSize  (0.055,"Y");
  thstack.GetHistogram()->SetTitleOffset(1.200,"Y");
  thstack.GetHistogram()->SetLabelOffset(0.014,"Y");
  thstack.GetHistogram()->SetLabelSize  (0.040,"Y");
  thstack.GetHistogram()->SetLabelFont  (42   ,"Y");
  thstack.GetHistogram()->SetTitleSize  (0.055,"X");
  thstack.GetHistogram()->SetTitleOffset(1.100,"X");
  thstack.GetHistogram()->SetLabelOffset(0.014,"X");
  thstack.GetHistogram()->SetLabelSize  (0.040,"X");
  thstack.GetHistogram()->SetLabelFont  (42   ,"X");

  // canv.Update();
  total_hist.Draw("SAMEE2");
  data_hist.Draw("SAME");

  TLegend *legend = new TLegend(0.7,0.20,0.9,0.40,"","brNDC");
  legend->AddEntry(&signal_hist, "", "F");
  legend->AddEntry(&data_hist, "", "LP");
  legend->AddEntry(&ztt_hist, "", "F");
  legend->AddEntry(&top_hist, "", "F");
  legend->AddEntry(&ewk_hist, "", "F");
  legend->AddEntry(&qcd_hist, "", "F");
  LegendStyle(legend);
  legend->Draw();


  TPad padBack("padBack","padBack",0.53,0.52,0.975,0.956);//TPad must be created after TCanvas otherwise ROOT crashes
  padBack.SetFillColor(0);

  TPad pad("diff","diff",0.45,0.45,0.9765,0.961);//TPad must be created after TCanvas otherwise ROOT crashes
  pad.cd();
  pad.SetFillColor(0);
  pad.SetFillStyle(0);
  err_hist.GetYaxis()->SetNdivisions(5);
  err_hist.GetYaxis()->SetLabelSize(0.05);
  err_hist.GetXaxis()->SetTitle("#bf{m_{#tau#tau} [GeV]}    ");
  err_hist.GetXaxis()->SetTitleColor(kBlack);
  err_hist.GetXaxis()->SetTitleSize(0.06);
  err_hist.GetXaxis()->SetTitleOffset(0.95);
  err_hist.GetXaxis()->SetLabelSize(0.05);
  err_hist.SetNdivisions(405);
  err_hist.Draw("E2");
  err_hist.GetXaxis()->SetRangeUser(40,199);
  // err_hist.GetYaxis()->SetRangeUser(-0.5,1.5);
  signal_hist.Draw("HISTSAME");
  diff_hist.Draw("SAME");
  pad.RedrawAxis();
  TLine line;
  line.DrawLine(40,0,200,0);
  TLegend *inlegend = new TLegend(0.60,0.75,0.9,0.90,"","brNDC");
  inlegend->AddEntry(&signal_hist, "", "F");
  inlegend->AddEntry(&diff_hist, "", "LP");
  inlegend->AddEntry(&err_hist, "", "F");
  LegendStyle(inlegend);
  inlegend->Draw();

  canv.cd();
  padBack.Draw();
  pad.Draw();
  canv.SaveAs("test_sob.pdf");


  return 0;
}

