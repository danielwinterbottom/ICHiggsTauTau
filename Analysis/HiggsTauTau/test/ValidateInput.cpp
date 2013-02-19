#include <iostream>
#include <vector>
#include <map>
#include <regex>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "boost/algorithm/string.hpp"
#include "TROOT.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TKey.h"

#include "TGraph.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"



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

vector<string> Directories(TDirectory const* dir) {
  TIter next(dir->GetListOfKeys());
  TKey *key;
  vector<string> ret;
  while ((key = (TKey*)next())) {
    TClass *cl = gROOT->GetClass(key->GetClassName());
    if (!cl->InheritsFrom("TDirectory")) continue;
    ret.push_back(key->GetName());
  }
  return ret;
}

vector<string> Histograms(TDirectory const* dir) {
  TIter next(dir->GetListOfKeys());
  TKey *key;
  vector<string> ret;
  while ((key = (TKey*)next())) {
    TClass *cl = gROOT->GetClass(key->GetClassName());
    if (!cl->InheritsFrom("TH1")) continue;
    ret.push_back(key->GetName());
  }
  return ret;
}



int main(int argc, char* argv[]){
  using namespace ic;
  using namespace std;

  string input = "";
  string append = "";
  string channel = ""; // muTau

  po::options_description config("Configuration");
  config.add_options()
    ("input",                 po::value<string>(&input)->required(), "input");
    // 145=135,125,115,105:
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::notify(vm);


  TFile fin(input.c_str(),"UPDATE");

  // vector<regex> signal_regex{ regex("^VH[0-9]*$"), regex("^ggH[0-9]*$"), regex("^qqH[0-9]*$") };

  map<string, vector<string>> backgrounds {
    {"eleTau_vbf"         , {"ZTT", "ZL", "ZJ", "W", "QCD", "VV", "TT"} },
    {"eleTau_boost_high"  , {"ZTT", "ZL", "ZJ", "W", "QCD", "VV", "TT"} },
    {"eleTau_boost_low"   , {"ZTT", "ZL", "ZJ", "W", "QCD", "VV", "TT"} },
    {"eleTau_0jet_high"   , {"ZTT", "ZL", "ZJ", "W", "QCD", "VV", "TT"} },
    {"eleTau_0jet_low"    , {"ZTT", "ZL", "ZJ", "W", "QCD", "VV", "TT"} },
    {"eleTau_inclusive"   , {"ZTT", "ZL", "ZJ", "W", "QCD", "VV", "TT"} },
    {"muTau_vbf"          , {"ZTT", "ZL", "ZJ", "W", "QCD", "VV", "TT"} },
    {"muTau_boost_high"   , {"ZTT", "ZL", "ZJ", "W", "QCD", "VV", "TT"} },
    {"muTau_boost_low"    , {"ZTT", "ZL", "ZJ", "W", "QCD", "VV", "TT"} },
    {"muTau_0jet_high"    , {"ZTT", "ZL", "ZJ", "W", "QCD", "VV", "TT"} },
    {"muTau_0jet_low"     , {"ZTT", "ZL", "ZJ", "W", "QCD", "VV", "TT"} },
    {"muTau_inclusive"    , {"ZTT", "ZL", "ZJ", "W", "QCD", "VV", "TT"} } };

  for (string &cat : Directories(&fin)) {
    if (backgrounds.count(cat)) {
      cout << "Entering category " << cat << endl;
      gDirectory->cd("/"+TString(cat));
      map<string, TH1F*> hists;
      for (string &h : Histograms(gDirectory)) {
        hists[h] = (TH1F*)gDirectory->Get(h.c_str());
      }
      for (int i = 1; i <= hists["data_obs"]->GetNbinsX(); ++i) {
        float data = hists["data_obs"]->GetBinContent(i);
        float background = 0.0;
        for (string &bkg : backgrounds[cat]) {
          background += hists[bkg]->GetBinContent(i);
        }
        if (data > 0.0 && background == 0) {
          std::cout << "Warnining, bin " << i << " in " << cat << " has data but no background" << std::endl;
          hists["data_obs"]->SetBinContent(i, 0.0);
          hists["data_obs"]->SetBinError(i, 0.0);
        }
      }
      hists["data_obs"]->Write("",TObject::kOverwrite);
    } else {
      cout << "Category " << cat << " not recognised, skipping!" << endl;
    }
  }

  fin.Close();

  return 0;
}
