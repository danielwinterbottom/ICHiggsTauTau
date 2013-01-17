#include <iostream>
#include <vector>
#include <map>
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TF1.h"

#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TextElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"


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

void SetDataStyle(ic::TH1PlotElement & ele, unsigned color) {
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


  std::string title = "Electron-Tau Pair Yields by Run, 2011";
  std::string yield_file = "Data_ElecTau_2011.root.runstats";
  std::string lumi_file = "lumi_data_2011.csv";
  double min_lumi = 20.0;
  std::string coded_runs = "Ele15_LooseIsoPFTau20,165088,167913:Ele15_TightIsoPFTau20,170249,173198:Ele18_MediumIsoPFTau20,173236,178380:Ele20_MediumIsoPFTau20,178420,180252";
  std::string output_name = "blah.pdf";

  po::options_description config("Configuration");
  config.add_options()
    ("title",                po::value<std::string>(&title)->required(), "title")
    ("yield_file",           po::value<std::string>(&yield_file)->required(), "yield_file")
    ("lumi_file",            po::value<std::string>(&lumi_file)->required(), "lumi_file")
    ("min_lumi",             po::value<double>(&min_lumi)->required(), "min_lumi")
    ("coded_runs",           po::value<std::string>(&coded_runs)->required(), "coded_runs")
    ("output_name",           po::value<std::string>(&output_name)->required(), "output_name");

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::notify(vm);


  ic::Plot::SetTdrStyle();



  std::vector<std::string> run_filters;
  boost::split(run_filters, coded_runs, boost::is_any_of(":"));
  std::vector<std::string> filter_labels;
  std::vector<std::pair<unsigned, unsigned> > filter_ranges;
  for (unsigned i = 0; i < run_filters.size(); ++i) {
    std::vector<std::string> tmp_split;
    boost::split(tmp_split, run_filters[i], boost::is_any_of(","));
    filter_labels.push_back(tmp_split.at(0));
    filter_ranges.push_back(std::make_pair(boost::lexical_cast<int>(tmp_split.at(1)), boost::lexical_cast<int>(tmp_split.at(2))));
    std::cout << filter_labels.back() << "\t" << filter_ranges.back().first << ":" << filter_ranges.back().second << std::endl;
  }




  std::vector<std::string> yield_lines = ParseFileLines(yield_file);
  std::vector<std::string> lumi_lines = ParseFileLines(lumi_file);

  std::map<unsigned, unsigned> yield_map;
  std::map<unsigned, double> lumi_map;

  std::vector<int> colors;
  colors.push_back(2);
  colors.push_back(4);
  colors.push_back(6);
  colors.push_back(8);
  colors.push_back(9);
  colors.push_back(42);






  double total_lumi = 0.0;

  for (unsigned i = 1; i < lumi_lines.size(); ++i) {
    std::vector<std::string> split_lumi_line;
    boost::split(split_lumi_line, lumi_lines[i], boost::is_any_of(","));
    std::string run_fill = split_lumi_line.at(0);
    std::vector<std::string> split_run_fill;
    boost::split(split_run_fill, run_fill, boost::is_any_of(":"));
    unsigned run = boost::lexical_cast<unsigned>(split_run_fill.at(0));
    std::string lumi_str = split_lumi_line.back();
    lumi_str.erase(lumi_str.size() - 1);
    double lumi = boost::lexical_cast<double>(lumi_str);
    lumi = lumi / (1000. * 1000.); // convert from inv. microbarns to inv. picobarns
    lumi_map[run] = lumi;
  }

  for (unsigned i = 0; i < yield_lines.size(); ++i) {
    std::vector<std::string> split_yield_line;
    boost::split(split_yield_line, yield_lines[i], boost::is_any_of(":"));
    unsigned run = boost::lexical_cast<unsigned>(split_yield_line.at(0));
    unsigned yield = boost::lexical_cast<unsigned>(split_yield_line.at(1));

    std::map<unsigned, double>::const_iterator it = lumi_map.find(run);

    // Now apply some conditions
    if (it == lumi_map.end()) {
      std::cout << "Warning, did not find lumi for run " << run << std::endl;
      continue;
    }
    if (it->second < min_lumi) continue;

    yield_map[run] = yield;
    total_lumi += lumi_map[run];
  }

  std::map<unsigned, double>::const_iterator lit;
  for (lit = lumi_map.begin(); lit != lumi_map.end(); ++lit) {
    if (lit->second < min_lumi) continue;
    if (yield_map.find(lit->first) == yield_map.end()) {
      std::cout << "Warning, found no events for run " << lit->first << " having a lumi of " << lit->second << std::endl;
    }
  }


  unsigned good_runs = yield_map.size();
  std::cout << "Plotting " << good_runs << " out of " << yield_lines.size() << std::endl;
  std::cout << "Total lumi is " << total_lumi << " /pb" << std::endl;

  std::vector<TH1F*> yield_plots(filter_labels.size(), NULL);
  for (unsigned i = 0; i < yield_plots.size(); ++i) {
    yield_plots[i] = new TH1F(filter_labels[i].c_str(), filter_labels[i].c_str(), good_runs, 0, good_runs);
    yield_plots[i]->SetStats(false);
  }

  //TH1F yield_plot("yield_plot","yield_plot", good_runs, 0, good_runs);
  std::vector<unsigned> yield_by_era(yield_plots.size(), 0);
  std::vector<double> lumi_by_era(yield_plots.size(), 0);

  std::map<unsigned, unsigned>::const_iterator it;
  int b = 1;
  for (it = yield_map.begin(); it != yield_map.end(); ++it, ++b) {
    unsigned run = it->first;
    double yield = it->second;
    double yield_n = it->second;
    double yield_err = sqrt(it->second);
    yield = yield / lumi_map[run];
    double lumi_n = lumi_map[run];

    yield_err = yield_err / lumi_map[run];
    for (unsigned j = 0; j < yield_plots.size(); ++j) {
      if (run >= filter_ranges[j].first && run <= filter_ranges[j].second) {
        yield_by_era[j] += yield_n;
        lumi_by_era[j] += lumi_n;
        yield_plots[j]->SetBinContent(b, yield);
        yield_plots[j]->SetBinError(b, yield_err);
      }
      yield_plots[j]->GetXaxis()->SetBinLabel(b, boost::lexical_cast<std::string>(run).c_str());
      //yield_plots[j]->GetXaxis()->SetBinLabel(b, (boost::lexical_cast<std::string>(int(lumi_map[run])) + " pb^{-1}").c_str());
    }
  }

  for (unsigned i = 0; i < yield_by_era.size(); ++i) {
    std::cout << i << "\t" << double(yield_by_era[i])/lumi_by_era[i] << std::endl;
  }

  TCanvas canv("canvas", "canvas", 1200,500);
  canv.SetBottomMargin(0.20);
  canv.SetLeftMargin(0.07);
  canv.SetRightMargin(0.02);
  canv.SetTopMargin(0.08);
  canv.SetGrid(0,1);

  canv.cd();
  TLegend *legend = 0;
  double legend_y1 = 0.89 - (yield_plots.size() * 0.055);
  double x1 = 0.74;
  double x2 = 0.95;
  double max_bin = 0.0;
  legend = new TLegend(x1,legend_y1,x2,0.89,"","brNDC");
  //yield_plots[0]->Draw();
  yield_plots[0]->GetYaxis()->SetTitle("Events / pb^{-1}");
  yield_plots[0]->GetYaxis()->SetTitleOffset(0.65);
  yield_plots[0]->GetXaxis()->SetTitle("Run (time-ordered)");
  yield_plots[0]->GetXaxis()->SetTitleOffset(1.50);
  yield_plots[0]->GetXaxis()->SetLabelSize(0.03);
  yield_plots[0]->GetXaxis()->LabelsOption("v");

  std::vector<TF1 *> fits;
  fits.resize(yield_plots.size());

  for (unsigned i = 0; i < yield_plots.size(); ++i) {
    if (yield_plots[i]->GetMaximum() > max_bin) max_bin = yield_plots[i]->GetMaximum();
    yield_plots[i]->SetMarkerColor(colors[i]);
    yield_plots[i]->SetLineColor(colors[i]);
    unsigned bins = yield_plots[i]->GetNbinsX();
    unsigned first_populated = 0;
    unsigned last_populated = 0;
    for (unsigned k = 1; k <= bins; ++k) {
      if (yield_plots[i]->GetBinContent(k) > 0. && first_populated == 0) first_populated = k;
      if (yield_plots[i]->GetBinContent(bins-(k-1)) > 0. && last_populated == 0) last_populated = bins-(k-1);
    }
    double min_fit = yield_plots[i]->GetBinLowEdge(first_populated);
    double max_fit = yield_plots[i]->GetBinLowEdge(last_populated);
    std::cout << min_fit << "\t" << max_fit << std::endl;
    string fit_name = ("f"+boost::lexical_cast<std::string>(i));
    fits[i] = new TF1((fit_name).c_str(), "pol0", min_fit, max_fit);
    fits[i]->SetLineColor(1);
    yield_plots[i]->Draw(i == 0 ? "" : "SAME");
    yield_plots[i]->Fit(fit_name.c_str(), "R", "SAME");
    legend->AddEntry(yield_plots[i], yield_plots[i]->GetTitle());
  }

  yield_plots[0]->SetMaximum(max_bin * 1.65);
  canv.Update();
  legend->SetBorderSize(1);
  legend->SetTextFont(42);
  legend->SetLineColor(0);
  legend->SetLineStyle(1);
  legend->SetLineWidth(1);
  legend->SetFillColor(0);
  legend->SetFillStyle(1001);
  legend->Draw();


  std::string title_right = "Showing Runs with Lumi > " + boost::lexical_cast<std::string>(min_lumi) + " pb^{-1} Only";
  TLatex *title_latex = new TLatex();
  title_latex->SetNDC();
  title_latex->SetTextSize(0.05);
  title_latex->SetTextAlign(31);
  title_latex->DrawLatex(0.98,0.945,title_right.c_str());
  title_latex->SetTextAlign(11);
  title_latex->DrawLatex(0.07,0.945,title.c_str());




    canv.SaveAs(output_name.c_str());



  
  return 0;
}

