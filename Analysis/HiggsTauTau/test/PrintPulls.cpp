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
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/Pull.h"


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

  string input1, name1;
  bool splusb_1;
  string input2, name2;
  bool splusb_2;
  int bbb_treatment; // 0=show all, 0=no bbb, 1 = bbb only

  po::options_description preconfig("Pre-Configuration");
  po::options_description config("Configuration");
  preconfig.add_options()
    ("input1",               po::value<string>(&input1)->required(), "inputs")
    ("input2",               po::value<string>(&input2)->required(), "labels")
    ("name1",               po::value<string>(&name1)->default_value(""), "name1")
    ("name2",               po::value<string>(&name2)->default_value(""), "name2")
    ("splusb_1",             po::value<bool>(&splusb_1)->required(), "category")
    ("splusb_2",             po::value<bool>(&splusb_2)->required(), "output_file")
    ("bbb_treatment",        po::value<int>(&bbb_treatment)->required(), "output_file");

  
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
            options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);

  ic::Plot::SetTdrStyle();

  std::vector<ic::Pull> pulls1;
  std::vector<ic::Pull> pulls2;
  PullsFromFile(input1, pulls1, true);
  PullsFromFile(input2, pulls2, true);

  std::vector<ic::Pull> pulls1sorted;
  std::vector<ic::Pull> pulls2sorted;
  for (unsigned i = 0; i < pulls1.size(); ++i) {
    bool found = false;
    for (unsigned j = 0; j < pulls2.size(); ++j) {
      if (pulls1[i].name == pulls2[j].name) {
        found = true;
        pulls2sorted.push_back(pulls2[j]);
        continue;
      }
    }
    if (found == true) pulls1sorted.push_back(pulls1[i]);
  }

  std::vector<ic::Pull> final;
  for (unsigned i = 0; i < pulls1sorted.size(); ++i) {
    if (bbb_treatment == 1) {
      if (pulls1sorted[i].name.find("_bin_") != pulls1sorted[i].name.npos) continue;
    }
    if (bbb_treatment == 2) {
      if (pulls1sorted[i].name.find("_bin_") == pulls1sorted[i].name.npos) continue;
    }
    ic::Pull pull;
    pull.name = pulls1sorted[i].name;
    pull.bonly = splusb_1 ? pulls1sorted[i].splusb : pulls1sorted[i].bonly ;
    pull.bonly_err = splusb_1 ? pulls1sorted[i].splusb_err : pulls1sorted[i].bonly_err ;
    pull.splusb = splusb_2 ? pulls2sorted[i].splusb : pulls2sorted[i].bonly ;
    pull.splusb_err = splusb_2 ? pulls2sorted[i].splusb_err : pulls2sorted[i].bonly_err ;
    final.push_back(pull);

  }

  std::sort(final.begin(), final.end(), BvsSBComparator);

  unsigned npulls = final.size();

  bool draw_second = true;

  TCanvas canv("canvas", "canvas", draw_second ? 1200 : 800 , 1200);
  canv.cd();



  canv.Update();
  TPad* pad1 = NULL;
  TPad* pad2 = NULL;
  if (draw_second) {
    pad1 = new TPad("pad1","pad1",0, 0, 0.85, 1);
    pad2 = new TPad("pad2","pad2",0.85, 0, 1, 1);
    pad1->Draw();
    pad2->Draw();
  } else {
    pad1 = new TPad("pad1","pad1",0, 0, 1, 1);
  }


  pad1->SetBottomMargin(0.05);
  pad1->SetLeftMargin(0.45);
  pad1->SetRightMargin(0.03);
  pad1->SetTopMargin(0.05);


  pad1->SetGrid(1,0);
  pad1->Draw();
  pad1->cd();

  TH2F *hpulls = new TH2F("pulls","pulls", 6, -3, 3, npulls, 0, npulls);
  TGraphErrors vals(npulls);
  TGraphErrors vals2(npulls);

  float ts1 = 30./(pad1->GetWw()*pad1->GetAbsWNDC());

  for (unsigned i = 0; i < final.size(); ++i) {
    vals.SetPoint(i, final[i].bonly, double(i) + 0.5);
    vals.SetPointError(i, final[i].bonly_err, 0);
    vals2.SetPoint(i, final[i].splusb, double(i) + 0.5);
    vals2.SetPointError(i, final[i].splusb_err, 0);
    // hpulls->SetBinContent(i+1, pulls[i].bonly);
    // hpulls->SetBinError(i+1, pulls[i].bonly_err);
    hpulls->GetYaxis()->SetBinLabel(i+1, final[i].name.c_str());
  }

  hpulls->GetYaxis()->LabelsOption("v");

  // hpulls->GetXaxis()->SetRangeUser(-3.0, 3.0);
  hpulls->SetStats(false);
  hpulls->GetYaxis()->SetLabelSize(0.03);
  
  // gStyle->SetErrorX(0.0);
  gStyle->SetEndErrorSize(5);

  hpulls->Draw("");
  hpulls->GetXaxis()->SetLabelSize(ts1);
  vals.Draw("pSAME");
  vals.SetLineWidth(2);
  vals2.SetLineWidth(2);
  vals.SetMarkerStyle(20);
  vals2.SetMarkerStyle(24);
  vals2.SetLineColor(4);
  vals2.SetMarkerColor(4);
  vals2.Draw("pSAME");

  canv.Update();

  if (pad2) {
    pad2->cd();
    pad2->SetBottomMargin(0.05);
    pad2->SetLeftMargin(0.1);
    pad2->SetRightMargin(0.2);
    pad2->SetTopMargin(0.05);
    pad2->SetGrid(1,0);

    TH2F *hpulldiff = new TH2F("pulldiff","pulldiff", 2, -0.5, 0.5, npulls, 0, npulls);
    hpulldiff->GetYaxis()->SetLabelSize(0.0);
    hpulldiff->GetYaxis()->SetTitleSize(0.0);
    TGraphErrors valsd(npulls);

    for (unsigned i = 0; i < final.size(); ++i) {
      valsd.SetPoint(i, final[i].splusb-final[i].bonly, double(i) + 0.5);
      valsd.SetPointError(i, 0, 0);
    }
    hpulldiff->SetStats(false);

    if (name1 == "") name1 = input1;
    if (name2 == "") name2 = input2;


    float ts2 = 30./(pad2->GetWw()*pad2->GetAbsWNDC());
    hpulldiff->GetXaxis()->SetLabelSize(ts2);
    hpulldiff->GetXaxis()->SetLabelOffset(-0.105);
    hpulldiff->GetXaxis()->SetNdivisions(502);
    hpulldiff->Draw("");
    valsd.Draw("pSAME");
    valsd.SetMarkerStyle(20);
    valsd.SetMarkerColor(4);
    canv.cd();
    TLatex *title_latex = new TLatex();
    title_latex->SetNDC();
    title_latex->SetTextSize(0.03);
    title_latex->SetTextAlign(31);
    title_latex->DrawLatex(0.95,0.965, (std::string(name1) + (splusb_1?"(s+b)":"(b)") + " vs. #color[4]{" + name2 + "}" + (splusb_2?"(s+b)":"(b)")).c_str());
    canv.Update();
    canv.SaveAs((name1+(splusb_1?"_sb_":"_b_")+"_"+name2+(splusb_2?"_sb_":"_b_")+".pdf").c_str());

  }




  
  return 0;
}

