#include <iostream>
#include <vector>
#include <map>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "boost/algorithm/string.hpp"
#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"

#include "TGraphErrors.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"


namespace po = boost::program_options;

using namespace std;


template<class T>
double Integral(T const* hist, int low_bin = -1, int high_bin = -1) {

  return hist->Integral((low_bin == -1 ? 0 : low_bin), (high_bin == -1 ? (hist->GetNbinsX() + 1) : high_bin));
}

template<class T>
double Error(T const* hist, int low_bin = -1, int high_bin = -1) {
  double err = 0.0;
  hist->IntegralAndError((low_bin == -1 ? 0 : low_bin), (high_bin == -1 ? (hist->GetNbinsX() + 1) : high_bin), err);
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



int main(int argc, char* argv[]){
  using namespace ic;
  using namespace std;

  ic::Plot::SetTdrStyle();

  std::string region = "barrel";

  std::string signal_file = "GluGluToHToTauTau_M-120_mt_2012_TauEff.root";
  std::string backgr_file = "Special_3_TauPlusX-2012C-24Aug2012-v1_mt_TauEff.root";
  // std::string signal_file = "SUSYGluGluToHToTauTau_M-500_mt_2012_TauEff.root";
  // std::string backgr_file = "QCD_2012_TauEff.root";

  // Cut > 0.795
  TH2F fake_barrel_mva1_num = GetFromTFile<TH2F>(backgr_file, "/TauEfficiency/", region+"_mva1_num");
  TH2F fake_barrel_mva1_den = GetFromTFile<TH2F>(backgr_file, "/TauEfficiency/", region+"_mva1_den");
  TH2F real_barrel_mva1_num = GetFromTFile<TH2F>(signal_file, "/TauEfficiency/", region+"_mva1_num");
  TH2F real_barrel_mva1_den = GetFromTFile<TH2F>(signal_file, "/TauEfficiency/", region+"_mva1_den");


  unsigned low_bin = -1;
  unsigned high_bin = -1;
  // unsigned low_bin = fake_barrel_mva1_num.GetXaxis()->FindBin(50);
  // unsigned high_bin = fake_barrel_mva1_den.GetXaxis()->FindBin(150);


  vector<double> mva1_sig_eff;
  vector<double> mva1_sig_err;
  vector<double> mva1_bkg_eff;
  vector<double> mva1_bkg_err;

  int mva1_bin = fake_barrel_mva1_num.GetYaxis()->FindBin(0.795);
  TGraphErrors *mva1_loose;
  for (int i = 1; i <= fake_barrel_mva1_num.GetNbinsY(); ++i) {
    TH1D *proj_fake_barrel_mva1_num = fake_barrel_mva1_num.ProjectionX("proj_fake_barrel_mva1_num",i,i,"");
    TH1D *proj_fake_barrel_mva1_den = fake_barrel_mva1_den.ProjectionX("proj_fake_barrel_mva1_den",i,i,"");
    TH1D *proj_real_barrel_mva1_num = real_barrel_mva1_num.ProjectionX("proj_real_barrel_mva1_num",i,i,"");
    TH1D *proj_real_barrel_mva1_den = real_barrel_mva1_den.ProjectionX("proj_real_barrel_mva1_den",i,i,"");
    mva1_bkg_eff.push_back(Integral(proj_fake_barrel_mva1_num,low_bin,high_bin) / Integral(proj_fake_barrel_mva1_den,low_bin,high_bin));
    mva1_bkg_err.push_back(Error(proj_fake_barrel_mva1_num,low_bin,high_bin)/Integral(proj_fake_barrel_mva1_den,low_bin,high_bin));
    mva1_sig_eff.push_back(Integral(proj_real_barrel_mva1_num,low_bin,high_bin) / Integral(proj_real_barrel_mva1_den,low_bin,high_bin));
    mva1_sig_err.push_back(Error(proj_real_barrel_mva1_num,low_bin,high_bin)/Integral(proj_real_barrel_mva1_den,low_bin,high_bin));
    std::cout << "MVA1 Cut: " << fake_barrel_mva1_num.GetYaxis()->GetBinLowEdge(i) << "  " << mva1_sig_eff.back() << " " << mva1_bkg_eff.back() << std::endl;
    if (i == mva1_bin) {
      mva1_loose = new TGraphErrors(1, new double(mva1_sig_eff.back()), new double(mva1_bkg_eff.back()), new double(mva1_sig_err.back()), new double(mva1_bkg_err.back()));
    }
    delete proj_fake_barrel_mva1_num;
    delete proj_fake_barrel_mva1_den;
    delete proj_real_barrel_mva1_num;
    delete proj_real_barrel_mva1_den;
  }


  // Cut > 0.85
  TH2F fake_barrel_mva2_num = GetFromTFile<TH2F>(backgr_file, "/TauEfficiency/", region+"_mva2_num");
  TH2F fake_barrel_mva2_den = GetFromTFile<TH2F>(backgr_file, "/TauEfficiency/", region+"_mva2_den");
  TH2F real_barrel_mva2_num = GetFromTFile<TH2F>(signal_file, "/TauEfficiency/", region+"_mva2_num");
  TH2F real_barrel_mva2_den = GetFromTFile<TH2F>(signal_file, "/TauEfficiency/", region+"_mva2_den");

  vector<double> mva2_sig_eff;
  vector<double> mva2_sig_err;
  vector<double> mva2_bkg_eff;
  vector<double> mva2_bkg_err;

  int mva2_bin = fake_barrel_mva2_num.GetYaxis()->FindBin(0.85);
  TGraphErrors *mva2_loose;

  for (int i = 1; i <= fake_barrel_mva2_num.GetNbinsY(); ++i) {
    TH1D *proj_fake_barrel_mva2_num = fake_barrel_mva2_num.ProjectionX("proj_fake_barrel_mva2_num",i,i,"");
    TH1D *proj_fake_barrel_mva2_den = fake_barrel_mva2_den.ProjectionX("proj_fake_barrel_mva2_den",i,i,"");
    TH1D *proj_real_barrel_mva2_num = real_barrel_mva2_num.ProjectionX("proj_real_barrel_mva2_num",i,i,"");
    TH1D *proj_real_barrel_mva2_den = real_barrel_mva2_den.ProjectionX("proj_real_barrel_mva2_den",i,i,"");
    mva2_bkg_eff.push_back(Integral(proj_fake_barrel_mva2_num,low_bin,high_bin) / Integral(proj_fake_barrel_mva2_den,low_bin,high_bin));
    mva2_bkg_err.push_back(Error(proj_fake_barrel_mva2_num,low_bin,high_bin)/Integral(proj_fake_barrel_mva2_den,low_bin,high_bin));
    mva2_sig_eff.push_back(Integral(proj_real_barrel_mva2_num,low_bin,high_bin) / Integral(proj_real_barrel_mva2_den,low_bin,high_bin));
    mva2_sig_err.push_back(Error(proj_real_barrel_mva2_num,low_bin,high_bin)/Integral(proj_real_barrel_mva2_den,low_bin,high_bin));
    std::cout << "MVA2 Cut: " << fake_barrel_mva2_num.GetYaxis()->GetBinLowEdge(i) << "  " << mva2_sig_eff.back() << " " << mva2_bkg_eff.back() << std::endl;
    if (i == mva2_bin) {
      mva2_loose = new TGraphErrors(1, new double(mva2_sig_eff.back()), new double(mva2_bkg_eff.back()), new double(mva2_sig_err.back()), new double(mva2_bkg_err.back()));
    }
    delete proj_fake_barrel_mva2_num;
    delete proj_fake_barrel_mva2_den;
    delete proj_real_barrel_mva2_num;
    delete proj_real_barrel_mva2_den;
  }

  // 3hits is < 2

  TH2F fake_barrel_hits_num = GetFromTFile<TH2F>(backgr_file, "/TauEfficiency/", region+"_3hit_num");
  TH2F fake_barrel_hits_den = GetFromTFile<TH2F>(backgr_file, "/TauEfficiency/", region+"_3hit_den");
  TH2F real_barrel_hits_num = GetFromTFile<TH2F>(signal_file, "/TauEfficiency/", region+"_3hit_num");
  TH2F real_barrel_hits_den = GetFromTFile<TH2F>(signal_file, "/TauEfficiency/", region+"_3hit_den");

  vector<double> hits_sig_eff;
  vector<double> hits_sig_err;
  vector<double> hits_bkg_eff;
  vector<double> hits_bkg_err;

  int hits_bin = fake_barrel_hits_num.GetYaxis()->FindBin(2);
  TGraphErrors *hits_loose;

  for (int i = 1; i <= fake_barrel_hits_num.GetNbinsY(); ++i) {
    TH1D *proj_fake_barrel_hits_num = fake_barrel_hits_num.ProjectionX("proj_fake_barrel_hits_num",i,i,"");
    TH1D *proj_fake_barrel_hits_den = fake_barrel_hits_den.ProjectionX("proj_fake_barrel_hits_den",i,i,"");
    TH1D *proj_real_barrel_hits_num = real_barrel_hits_num.ProjectionX("proj_real_barrel_hits_num",i,i,"");
    TH1D *proj_real_barrel_hits_den = real_barrel_hits_den.ProjectionX("proj_real_barrel_hits_den",i,i,"");
    hits_bkg_eff.push_back(Integral(proj_fake_barrel_hits_num,low_bin,high_bin) / Integral(proj_fake_barrel_hits_den,low_bin,high_bin));
    hits_bkg_err.push_back(Error(proj_fake_barrel_hits_num,low_bin,high_bin)/Integral(proj_fake_barrel_hits_den,low_bin,high_bin));
    hits_sig_eff.push_back(Integral(proj_real_barrel_hits_num,low_bin,high_bin) / Integral(proj_real_barrel_hits_den,low_bin,high_bin));
    hits_sig_err.push_back(Error(proj_real_barrel_hits_num,low_bin,high_bin)/Integral(proj_real_barrel_hits_den,low_bin,high_bin));
    std::cout << "3Hits Cut: " << fake_barrel_hits_num.GetYaxis()->GetBinLowEdge(i)+fake_barrel_hits_num.GetYaxis()->GetBinWidth(i) << "  " << hits_sig_eff.back() << " " << hits_bkg_eff.back() << std::endl;
    if (i == hits_bin) {
      hits_loose = new TGraphErrors(1, new double(hits_sig_eff.back()), new double(hits_bkg_eff.back()), new double(hits_sig_err.back()), new double(hits_bkg_err.back()));
    }
    delete proj_fake_barrel_hits_num;
    delete proj_fake_barrel_hits_den;
    delete proj_real_barrel_hits_num;
    delete proj_real_barrel_hits_den;
  }

  TGraphErrors mva1_graph(mva1_sig_eff.size(), &(mva1_sig_eff[0]), &(mva1_bkg_eff[0]), &(mva1_sig_err[0]), &(mva1_bkg_err[0]));
  TGraphErrors mva2_graph(mva2_sig_eff.size(), &(mva2_sig_eff[0]), &(mva2_bkg_eff[0]), &(mva2_sig_err[0]), &(mva2_bkg_err[0]));
  TGraphErrors hits_graph(hits_sig_eff.size(), &(hits_sig_eff[0]), &(hits_bkg_eff[0]), &(hits_sig_err[0]), &(hits_bkg_err[0]));
  mva1_graph.GetXaxis()->SetRangeUser(0.65, 0.85);
  mva1_graph.GetXaxis()->SetTitle("Signal Efficiency");
  mva1_graph.GetYaxis()->SetRangeUser(0., 0.15);
  mva1_graph.GetYaxis()->SetTitle("Fake Rate");

  mva1_graph.SetMarkerColor(kBlack);
  mva1_graph.SetLineColor(kBlack);
  mva1_graph.SetFillColor(kGray);
  mva1_graph.SetFillStyle(3001);
  mva2_graph.SetMarkerColor(kRed);
  mva2_graph.SetLineColor(kRed);
  mva2_graph.SetFillColor(kRed-9);
  mva2_graph.SetFillStyle(3001);
  hits_graph.SetMarkerColor(kBlue);
  hits_graph.SetLineColor(kBlue);
  hits_graph.SetFillColor(kBlue-9);
  hits_graph.SetFillStyle(3001);
  TCanvas mva_canv("mva_canv","mva_canv", 600, 600);
  mva1_graph.Draw("A3L");
  mva2_graph.Draw("3LSAME");
  hits_graph.Draw("3LSAME");
  if (mva1_loose) {
    // mva1_loose.SetMarkerColor(kRed);
    // mva1_loose.SetLineColor(kRed);
    mva1_loose->Draw("PSAME");

  }
  if (mva2_loose) {
    mva2_loose->SetMarkerColor(kRed+2);
    mva2_loose->SetLineColor(kRed+2);
    mva2_loose->Draw("PSAME");
  }
  if (hits_loose) {
    hits_loose->SetMarkerColor(kBlue+2);
    hits_loose->SetLineColor(kBlue+2);
    hits_loose->Draw("PSAME");
  }



  TLegend *legend = new TLegend(0.20,0.7,0.60,0.92,"","NDC");
  legend->AddEntry(&mva1_graph, "MVA1", "lf");
  legend->AddEntry(&mva2_graph, "MVA2", "lf");
  legend->AddEntry(&hits_graph, "3Hits", "lf");
  legend->AddEntry(mva1_loose, "Loose WP", "p");

  legend->SetBorderSize(1);
  legend->SetTextFont(42);
  legend->SetLineColor(0);
  legend->SetLineStyle(1);
  legend->SetLineWidth(1);
  legend->SetFillColor(0);
  legend->SetFillStyle(1001);
  legend->Draw();

  TLatex *title_latex = new TLatex();
  title_latex->SetNDC();
    title_latex->SetTextSize(0.03);
    title_latex->SetTextAlign(31);
    title_latex->DrawLatex(0.95,0.965,"#mu#tau_{h}");
    title_latex->SetTextAlign(11);
    title_latex->DrawLatex(0.16,0.965,"Signal: SM ggH120, Fakes: SS Data (0.2 < #mu^{iso} < 0.5)");
    // title_latex->DrawLatex(0.16,0.965,"Signal: MSSM ggH500, Fakes: QCD MC, 50 < p_{T}^{#tau} < 150 GeV");
    title_latex->SetTextSize(0.06);
    title_latex->DrawLatex(0.7,0.87,region.c_str());



  mva_canv.SetGrid(1,1);

  mva_canv.SaveAs(("tau_iso_"+region+".pdf").c_str());

  return 0;
}
