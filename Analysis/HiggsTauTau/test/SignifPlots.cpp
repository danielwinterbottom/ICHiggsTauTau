#include <iostream>
#include <vector>
#include <map>
#include "TGraph.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TextElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SimpleParamParser.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"

namespace po = boost::program_options;

using namespace std;
using namespace ic;

void SetTGraphStyle(TGraph * gr, int color) {
    gr->SetLineStyle(11.);
    gr->SetLineWidth( 3.);
    gr->SetLineColor(color);
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1.3);
    gr->SetMarkerColor(color);
}


int main(int argc, char* argv[]){

  string ch = "mt-8TeV";
  bool limit = true;
  bool relative = false;

  vector<string> lines = { "def", "test" };
  vector<unsigned> cols = { 1 , 2 };
  vector<string> labels = { "Ignore H#rightarrowWW" , "H#rightarrow#tau#tau+H#rightarrowWW" };
  vector<string> types = { "jpt", "hpt", "jpt_0jet", "hpt_0jet" };
  // vector<string> lines = { "jpt", "hpt", "jpt_0jet", "hpt_0jet", "moriond" };
  vector<string> vals = { "30", "40", "60", "80" };
  vector<double> x_vals = { 30, 40, 60, 80 };

  // vector<string> lines = { "hpt_40", "hpt_40_0jet", "hpt_60", "hpt_60_0jet", "moriond" };
  // vector<string> lines = { "hpt_40", "hpt_40_0jet", "hpt_60", "hpt_60_0jet" };
  vector<TGraph> graphs;



  // for (auto t : types) {
  //   vector<double> y_vals;
  //   for (unsigned i = 0; i < vals.size(); ++i) {
  //     TGraph tmp = GetFromTFile<TGraph>("output/categories/"+t+"_"+vals[i]+"/"+ (limit ? (ch+"_limit") : "limits_significance")+".root",ch+"/","expected");
  //     double x, y;
  //     tmp.GetPoint(3, x, y);
  //     std::cout << t << "\t" << vals[i] << "\t" << x << "\t" << y << endl;
  //     y_vals.push_back(y);
  //   }
  //   graphs.push_back(TGraph(x_vals.size(), &(x_vals[0]), &(y_vals[0])));
  // }

  // vector<double> xm_vals = { 30 };
  // vector<double> y_vals;
  // TGraph tmp = GetFromTFile<TGraph>("output/categories/moriond/"+ (limit ? (ch+"_limit") : "limits_significance")+".root",ch+"/","expected");
  // double x, y;
  // tmp.GetPoint(3, x, y);
  // y_vals.push_back(y);
  // graphs.push_back(TGraph(xm_vals.size(), &(xm_vals[0]), &(y_vals[0])));


  for (auto l : lines) {
    graphs.push_back(GetFromTFile<TGraph>("output/sm-hww/"+ l + "/" + (limit ? (ch+"_limit") : "limits_significance") + ".root",ch+"/","expected"));
    double x, y;
    graphs.back().GetPoint(3, x, y);
    std::cout << l << "\t" << x << "\t" << y << endl;
  }

  ic::Plot::SetHTTStyle();
  TCanvas canv("canv","canv",600,600);
  gStyle->SetNdivisions(505);
  canv.SetGridx(1);
  canv.SetGridy(1);
  graphs[0].GetXaxis()->SetLimits(graphs[0].GetX()[0]-.1, graphs[0].GetX()[graphs[0].GetN()-1]+.1);
  graphs[0].SetMinimum(0.51);
  graphs[0].SetMaximum(5.0);
  if (relative) {
    graphs[0].SetMinimum(0.8);
    graphs[0].SetMaximum(1.6);    
  }
  graphs[0].GetYaxis()->SetTitleSize  (0.055);
  graphs[0].GetYaxis()->SetTitleOffset(1.200);
  graphs[0].GetYaxis()->SetLabelOffset(0.014);
  graphs[0].GetYaxis()->SetLabelSize  (0.040);
  graphs[0].GetYaxis()->SetLabelFont  (42);
  graphs[0].GetXaxis()->SetTitleSize  (0.055);
  graphs[0].GetXaxis()->SetTitleOffset(1.100);
  graphs[0].GetXaxis()->SetLabelOffset(0.014);
  graphs[0].GetXaxis()->SetLabelSize  (0.040);
  graphs[0].GetXaxis()->SetLabelFont  (42);
  graphs[0].Draw("APL");
  graphs[0].GetYaxis()->SetTitleFont(42);
  graphs[0].GetXaxis()->SetTitleFont(42);   
  graphs[0].GetXaxis()->SetTitle("m_{H} [GeV]");
  // graphs[0].GetXaxis()->SetTitle("Cut Value [GeV]");
  if (!limit) {
    graphs[0].GetYaxis()->SetTitle("Exp. Significance");
    if (relative) graphs[0].GetYaxis()->SetTitle("Exp. Signif. / Default");   
  } else {
    graphs[0].GetYaxis()->SetTitle("95\% CL Limit on #sigma/#sigma_{SM}");    
    if (relative) graphs[0].GetYaxis()->SetTitle("Exp. Limit / Default");   
  }
  TLegend *legend = new TLegend(0.17,0.65,0.47,0.85,"","brNDC");
  
  double benchmark = graphs[0].GetY()[0];
  for (unsigned i = 0; i < graphs.size(); ++i) {
    SetTGraphStyle(&(graphs[i]), cols[i]);
    if (relative) {
      for (int j = 0; j < graphs[i].GetN(); ++j) {
        graphs[i].GetY()[j] /= benchmark;
      }      
    }
    graphs[i].Draw("PLsame");
    legend->AddEntry(&(graphs)[i], labels[i].c_str(), "lP");

  }

  legend->SetBorderSize(1);
  legend->SetTextFont(42);
  legend->SetLineColor(0);
  legend->SetLineStyle(1);
  legend->SetLineWidth(1);
  legend->SetFillColor(0);
  legend->SetFillStyle(1001);
  legend->Draw();

  string outfile;
  outfile += ch;
  outfile += "_";
  outfile += (limit ? "limit" : "signif");
  outfile += "_";
  outfile += (relative ? "relative" : "absolute");  

  if (relative) {
    canv.SaveAs((outfile+".pdf").c_str());    
  } else {
    canv.SaveAs((outfile+".pdf").c_str());    
  }

  return 0;
}

