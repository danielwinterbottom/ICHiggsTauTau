#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "boost/program_options.hpp"


#include "TCanvas.h"
#include "TH1F.h"
#include "TFile.h"
#include "TROOT.h"
#include "TFrame.h"
#include "TLegend.h"
#include "THStack.h"
#include "TGraphAsymmErrors.h"
#include "boost/range/adaptor/reversed.hpp"
#include "boost/algorithm/string.hpp"

#include "Core/interface/Plotting.h"
#include "Core/interface/Plotting_Style.h"
#include "Utilities/interface/JsonTools.h"
#include "Utilities/interface/FnRootTools.h"
// #include "CombineTools/interface/TFileIO.h"


namespace po = boost::program_options;
using namespace std;

std::vector<double> BinsFromRange(std::string const& input) {
  std::set<double> val_set;
  std::vector<std::string> tokens;
  boost::split(tokens, input, boost::is_any_of(","));
  for (auto const& t : tokens) {
    std::vector<std::string> sub_tokens;
    boost::split(sub_tokens, t, boost::is_any_of("-:"));
    if (sub_tokens.size() == 1) {
      double val = boost::lexical_cast<double>(sub_tokens[0]);
      val_set.insert(val);
    } else if (sub_tokens.size() == 3) {
      double lo = boost::lexical_cast<double>(sub_tokens[0]);
      double hi = boost::lexical_cast<double>(sub_tokens[1]);
      double step = boost::lexical_cast<double>(sub_tokens[2]);
      if (hi <= lo)
        throw std::runtime_error(
            "[BinssFromRange] High mass is smaller than low mass!");
      double start = lo;
      while (start < hi + 1E-5) {
        val_set.insert(start);
        start += step;
      }
    }
  }
  std::vector<double> result;
  for (auto const& m : val_set) {
    result.push_back(m);
  }
  return result;
}



int main(int argc, char* argv[]) {
  // bool do_ratio = true;
  // bool do_logy = false;

  TH1::AddDirectory(0);
  ModTDRStyle();
  gStyle->SetEndErrorSize(0);


  Json::Value const js = ic::MergedJson(argc, argv);

  std::vector<TGraphAsymmErrors> graphs;

  Json::Value const& def = js["default"];

  for (unsigned g = 0; g < js["elements"].size(); ++g) {
    Json::Value const ele = js["elements"][g];
    TH1F h_num = ic::OpenFromTFile<TH1F>(ele["h_num"].asString() + js["output"].asString() + "_pass");
    TH1F h_den = ic::OpenFromTFile<TH1F>(ele["h_den"].asString() + js["output"].asString() + "_all");
    std::string rebin = ele.get("rebin", def["rebin"]).asString();
    std::cout << rebin << "\n";
    if (rebin != "") {
      std::vector<double> bins = BinsFromRange(rebin);
      h_num = *(TH1F*)(h_num.Rebin(bins.size()-1, "", &(bins[0])));
      h_den = *(TH1F*)(h_den.Rebin(bins.size()-1, "", &(bins[0])));
    }
    graphs.emplace_back(&h_num, &h_den);
    TGraphAsymmErrors & graph = graphs.back();
    graph.SetLineColor(ele.get("color", def["color"]).asUInt());
    graph.SetMarkerColor(ele.get("color", def["color"]).asUInt());
    graph.SetMarkerStyle(ele.get("marker", def["marker"]).asUInt());
  }

  TCanvas canv(js["output"].asCString(), js["output"].asCString());
  canv.cd();

  std::vector<TPad*> pads = OnePad();

  TH1 *axis = CreateAxisHist(&(graphs[0]), false);
  axis->Draw();
  axis->SetMinimum(js["y_axis_min"].asDouble());
  axis->SetMaximum(js["y_axis_max"].asDouble());
  pads[0]->SetLogy(js["log_y"].asBool());
  axis->GetXaxis()->SetTitle(js["x_axis_title"].asCString());
  axis->GetYaxis()->SetTitle(js["y_axis_title"].asCString());

  TLegend *legend = PositionedLegend(0.45, 0.18, 6, 0.03);
  for (unsigned g = 0; g < graphs.size(); ++g) {
    graphs[g].Draw("PSAME");
    legend->AddEntry(&(graphs[g]), js["elements"][g]["legend"].asCString());
  }
  pads[0]->SetGridx(true);
  pads[0]->SetGridy(true);
  DrawCMSLogo(pads[0], "CMS", "Simulation", 0);
  DrawTitle(pads[0], js["title_right"].asString(), 3);


  legend->Draw();

  canv.Print(".pdf");

  return 0;
}
