#include <iostream>
#include <vector>
#include <map>

#include "TCanvas.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TH2F.h"
#include "TStyle.h"

#include "boost/regex.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/program_options.hpp"

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTStatTools.h"

namespace po = boost::program_options;

using namespace std;
using namespace ic;

int main(int argc, char* argv[]){

  // Define and parse arguments 
  string input1, name1;
  bool splusb_1;
  string input2, name2;
  bool splusb_2;
  bool draw_difference;
  vector<string> filter_regex_str;

  po::options_description config("Configuration");
  config.add_options()
    ("input1",                po::value<string>(&input1)->required(), "input1")
    ("input2",                po::value<string>(&input2)->required(), "input2")
    ("label1",                po::value<string>(&name1)->default_value("input1"), "label1")
    ("label2",                po::value<string>(&name2)->default_value("input2"), "label2")
    ("splusb_1",              po::value<bool>(&splusb_1)->required(), "category")
    ("draw_difference",       po::value<bool>(&draw_difference)->default_value(true), "draw difference")
    ("splusb_2",              po::value<bool>(&splusb_2)->required(), "output_file")
    ("filter_regex",          po::value<vector<string>>(&filter_regex_str), "filter_regex");
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::notify(vm);

  // Build a list of regular expressions to filter nuisances
  std::vector<boost::regex> filter_regex;
  for (unsigned i = 0; i < filter_regex_str.size(); ++i) {
    filter_regex.push_back(boost::regex(filter_regex_str[i]));
    std::cout << "Filter nuisances with regex: " << filter_regex_str[i] << std::endl;
  }

  // Set a nice drawing style  
  ic::Plot::SetTdrStyle();

  // Extract pulls from the two input files
  std::vector<ic::Pull> pulls1;
  std::vector<ic::Pull> pulls2;
  PullsFromFile(input1, pulls1, false);
  PullsFromFile(input2, pulls2, false);

  // Build new lists of the pulls common to both inputs, and in the same order
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

  // Build a final vector of pulls taking b-only or s+b for each input respectively
  // Skip pulls that match one of the filter regex
  std::vector<ic::Pull> final;
  for (unsigned i = 0; i < pulls1sorted.size(); ++i) {
    bool skip = false;
    for (unsigned j = 0; j < filter_regex.size(); ++j) {
      if (boost::regex_match(pulls1sorted[i].name, filter_regex[j])) skip = true;
    }
    if (skip) continue;
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


  // Prepare the canvas
  TCanvas canv("canvas", "canvas", draw_difference ? 1200 : 800 , 1200);
  canv.cd();
  canv.Update();
  TPad* pad1 = NULL;
  TPad* pad2 = NULL;
  if (draw_difference) {
    pad1 = new TPad("pad1","pad1",0, 0, 0.85, 1);
    pad2 = new TPad("pad2","pad2",0.85, 0, 1, 1);
    pad1->Draw();
    pad2->Draw();
  } else {
    pad1 = new TPad("pad1","pad1",0, 0, 1, 1);
  }
  pad1->SetBottomMargin(0.07);
  pad1->SetLeftMargin(0.45);
  pad1->SetRightMargin(0.03);
  pad1->SetTopMargin(0.05);
  pad1->SetGrid(1,0);
  pad1->Draw();
  pad1->cd();

  // Create a dummy TH2F to attach the nuisance labels
  // and TGraphErrors for each set of pulls
  TH2F *hpulls = new TH2F("pulls","pulls", 6, -3, 3, npulls, 0, npulls);
  TGraphErrors vals(npulls);
  TGraphErrors vals2(npulls);
  float ts1 = 30./(pad1->GetWw()*pad1->GetAbsWNDC());
  for (unsigned i = 0; i < final.size(); ++i) {
    vals.SetPoint(i, final[i].bonly, double(i) + 0.5);
    vals.SetPointError(i, final[i].bonly_err, 0);
    vals2.SetPoint(i, final[i].splusb, double(i) + 0.5);
    vals2.SetPointError(i, final[i].splusb_err, 0);
    hpulls->GetYaxis()->SetBinLabel(i+1, final[i].name.c_str());
  }
  hpulls->GetYaxis()->LabelsOption("v");
  hpulls->SetStats(false);
  hpulls->GetYaxis()->SetLabelSize(0.03);
  gStyle->SetEndErrorSize(5);
  hpulls->GetXaxis()->SetTitle("Pull (#sigma)");
  hpulls->GetXaxis()->CenterTitle();
  hpulls->GetXaxis()->SetTitleSize(0.04);
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

  TGraphErrors valsd;
  if (pad2) {
    pad2->cd();
    pad2->SetBottomMargin(0.07);
    pad2->SetLeftMargin(0.1);
    pad2->SetRightMargin(0.2);
    pad2->SetTopMargin(0.05);
    pad2->SetGrid(1,0);
    TH2F *hpulldiff = new TH2F("pulldiff","pulldiff", 2, -0.5, 0.5, npulls, 0, npulls);
    hpulldiff->GetYaxis()->SetLabelSize(0.0);
    hpulldiff->GetYaxis()->SetTitleSize(0.0);
    valsd = TGraphErrors(npulls);
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
    canv.Update();
  }
  canv.cd();
  TLatex *title_latex = new TLatex();
  title_latex->SetNDC();
  title_latex->SetTextSize(0.03);
  title_latex->SetTextAlign(31);
  title_latex->DrawLatex(0.95,0.965, (std::string(name1) + (splusb_1?"(s+b)":"(b-only)") + " vs. #color[4]{" + name2 + "}" + (splusb_2?"(s+b)":"(b-only)")).c_str());
  canv.Update();
  canv.SaveAs(("compare_pulls_"+name1+(splusb_1?"_sb_":"_b_")+name2+(splusb_2?"_sb":"_b")+".pdf").c_str());
  canv.SaveAs(("compare_pulls_"+name1+(splusb_1?"_sb_":"_b_")+name2+(splusb_2?"_sb":"_b")+".png").c_str());

  return 0;
}

