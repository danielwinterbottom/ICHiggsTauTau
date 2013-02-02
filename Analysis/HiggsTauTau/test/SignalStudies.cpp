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
#include "TF1.h"
#include "TCanvas.h"

#include "TGraph.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"

#include "RooBinning.h"
#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooDataHist.h"
#include "RooAbsPdf.h"
#include "RooVoigtian.h"
#include "RooCBShape.h"
#include "RooBifurGauss.h"
#include "RooAddPdf.h"
#include "RooExponential.h"
#include "RooSimultaneous.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooFFTConvPdf.h"
#include "RooBreitWigner.h"
#include "boost/format.hpp"
#include "RooMsgService.h"

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



int main(int argc, char* argv[]){
  using namespace ic;
  using namespace std;

  string input = "";
  string append = "";
  string channel = ""; // muTau
  string prod = "";
  string fit_input = "";
  string extrap = "";
  bool   no_rms = false;


  po::options_description config("Configuration");
  config.add_options()
    ("input",                 po::value<string>(&input)->required(), "input")
    ("append",                po::value<string>(&append)->default_value(""), "append")
    ("channel",               po::value<string>(&channel)->required(), "channel")
    ("prod",                  po::value<string>(&prod)->required(), "prod")
    ("fit_input",             po::value<string>(&fit_input)->default_value("110,115,120,125,130,135,140,145"), "fit_input")
    ("no_rms",                po::value<bool>(&no_rms)->default_value(false), "fit_input")
    ("extrap",                po::value<std::string>(&extrap)->required(), "extrap");
    // 145=135,125,115,105:
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::notify(vm);


  Plot::SetTdrStyle();

  TFile *input_dc = new TFile(input.c_str(),"UPDATE");
  cout << "Updating file: " << input << endl;
  cout << "Using channel: " << channel << endl;
  cout << "Applying to signal samples: " << prod << endl;
  cout << "Fit points:    " << fit_input << endl;
  cout << "Appending:     " << append << endl;

  vector<string> mcs;
  boost::split(mcs, prod, boost::is_any_of(","));

  vector<string> vfit_input;
  boost::split(vfit_input, fit_input, boost::is_any_of(","));
  vector<double> vfit_input_vals;
  for (unsigned i = 0; i < vfit_input.size(); ++i) vfit_input_vals.push_back(boost::lexical_cast<double>(vfit_input[i]));

  vector<string> vextraps;
  vector<pair<double, vector<double> > > dextrap;
  vector<pair<string, vector<string> > > sextrap;

  boost::split(vextraps, extrap, boost::is_any_of(":"));
  for (unsigned i = 0; i < vextraps.size(); ++i) {
    vector<string> tmp;
    boost::split(tmp, vextraps[i], boost::is_any_of("="));
    vector<string> tmp2;
    boost::split(tmp2, tmp.at(1), boost::is_any_of(","));
    std::cout << "Extrapolate from " << tmp.at(0) << " to: " << tmp.at(1) << endl;
    sextrap.push_back(make_pair(tmp.at(0),tmp2));
    double source = boost::lexical_cast<double>(tmp.at(0));
    vector<double> dtmp;
    for (unsigned j = 0; j < tmp2.size(); ++j) dtmp.push_back(boost::lexical_cast<double>(tmp2[j]));
    dextrap.push_back(make_pair(source, dtmp));
  }

  std::vector<string> cats;
  cats.push_back(channel+"_vbf");
  cats.push_back(channel+"_boost_high");
  cats.push_back(channel+"_boost_low");
  cats.push_back(channel+"_0jet_high");
  cats.push_back(channel+"_0jet_low");
  cats.push_back(channel+"_inclusive");


  std::string test_source_str = "145";
  std::string test_predict_str = "110";

  for (unsigned i = 0; i < cats.size(); ++i) {
    for (unsigned j = 0; j < mcs.size(); ++j) {
      if (!gDirectory->cd(("/"+cats[i]).c_str())) continue;

      std::vector<TH1F *> hists;
      for (unsigned k = 0; k < vfit_input.size(); ++k) hists.push_back((TH1F*)gDirectory->Get((mcs[j]+vfit_input[k]+append).c_str()));
      bool should_skip = false;
      for (unsigned k = 0; k < vfit_input.size(); ++k) {
        if (!hists[k]) {
          cout << "Histogram not found, skipping!" << endl; 
          should_skip = true;
        }
      }
      if (should_skip) continue;
      // double mass[8] = {110, 115, 120, 125, 130, 135, 140, 145};
      double mean[hists.size()];
      double rms[hists.size()];
      double yield[hists.size()];
      for (unsigned i = 0; i < hists.size(); ++i) {
        mean[i] = hists[i]->GetMean();    
        rms[i] = hists[i]->GetRMS();    
        yield[i] = hists[i]->Integral();    
      }
      TGraph gMean (8, &(vfit_input_vals[0]), mean);
      gMean.SetName("mean");
      TGraph gRMS (8, &(vfit_input_vals[0]), rms);
      gRMS.SetName("rms");
      TGraph gYield (8, &(vfit_input_vals[0]), yield);
      gYield.SetName("yield");
      
      TF1 fitMean("fitmean","pol1",vfit_input_vals[0],vfit_input_vals.back());
      TF1 fitYield("fityield","pol1",vfit_input_vals[0],vfit_input_vals.back());
      TF1 fitRMS("fitrms","pol1",vfit_input_vals[0],vfit_input_vals.back());
      std::cout << "Fitting " << mcs[j] << " in category " << cats[i] << std::endl;
      gMean.Fit("fitmean","R","SAME");
      TCanvas cMean("cMean","cMean",700,700);
      gMean.Draw("ap");
      gMean.GetXaxis()->SetTitle("Mass Point");
      gMean.GetYaxis()->SetTitle("Distribution Mean");
      cMean.SaveAs(("fit_mean_"+cats[i]+"_"+mcs[j]+".pdf").c_str());
      cMean.SaveAs(("fit_mean_"+cats[i]+"_"+mcs[j]+".png").c_str());
      gYield.Fit("fityield","R","SAME");
      TCanvas cYield("cYield","cYield",700,700);
      gYield.Draw("ap");
      gYield.GetXaxis()->SetTitle("Mass Point");
      gYield.GetYaxis()->SetTitle("Distribution Yield");
      cYield.SaveAs(("fit_yield_"+cats[i]+"_"+mcs[j]+".pdf").c_str());
      cYield.SaveAs(("fit_yield_"+cats[i]+"_"+mcs[j]+".png").c_str());
      gRMS.Fit("fitrms","R","SAME");
      TCanvas cRMS("cRMS","cRMS",700,700);
      gRMS.Draw("ap");
      gRMS.GetXaxis()->SetTitle("Mass Point");
      gRMS.GetYaxis()->SetTitle("Distribution RMS");
      cRMS.SaveAs(("fit_rms_"+cats[i]+"_"+mcs[j]+".pdf").c_str());
      cRMS.SaveAs(("fit_rms_"+cats[i]+"_"+mcs[j]+".png").c_str());
      std::cout << "\n\n\n" << std::endl;

      unsigned test_idx = 0;
      for (unsigned p = 0; p < vfit_input.size(); ++p) {
        if (test_source_str == vfit_input[p]) {
          test_idx = p;
          break;
          if (p == vfit_input.size() - 1) throw;
        }
      }
      unsigned test_predict_idx = 0;
      for (unsigned p = 0; p < vfit_input.size(); ++p) {
        if (test_predict_str == vfit_input[p]) {
          test_predict_idx = p;
          break;
          if (p == vfit_input.size() - 1) throw;
        }
      }
      std::cout << "Found test idx: " << test_idx << endl;
      std::cout << "Found predict idx: " << test_predict_idx << endl;

      TH1F *test_input = hists[test_idx];
      RooBinning xbins (test_input->GetNbinsX(),((test_input->GetXaxis())->GetXbins())->GetArray()) ;
      double test_predict = boost::lexical_cast<double>(test_predict_str);
      double target_mean = fitMean.Eval(test_predict);
      double target_yield = fitYield.Eval(test_predict);
      double target_res = fitRMS.Eval(test_predict);
      double shift = target_mean - test_input->GetMean();
      double res = target_res / test_input->GetRMS();
      // std::cout << "res: " << res << std::endl;
      RooRealVar x("x","x",0,350) ; 
      RooDataHist dx("dx","dx",x,test_input); 
      RooRealVar a("a","a",(shift),-80,80) ; 
      RooRealVar b("b","b",(res),0,5) ; 
      RooRealVar nmean("m","m",(test_input->GetMean()),0,350) ; 
      RooFormulaVar xf("xf","m+((x-m-a)/b)",RooArgSet(x,a,b,nmean)) ; 
      RooHistPdf hx("hx","hx",xf,x,dx,1) ;
      TH1 * test_shifted = hx.createHistogram("test",x, RooFit::Binning(xbins));
      test_shifted->SetName((mcs[j]+"test").c_str());
      test_shifted->SetTitle((mcs[j]+"test").c_str());
      test_shifted->Scale(target_yield / test_shifted->Integral());
      double av_weight = ( test_input->Integral() / double(test_input->GetEntries()));
      test_shifted->Sumw2();
      for (int k = 1; k <= test_shifted->GetNbinsX(); ++k) test_shifted->SetBinError(k, test_shifted->GetBinError(k) * av_weight);
      test_shifted->Write("",TObject::kOverwrite);

      for (unsigned l = 0; l < sextrap.size(); ++l) {
        string src_point = sextrap[l].first;
        unsigned src_idx = 0;
        for (unsigned m = 0; m < vfit_input.size(); ++m) {
          if (src_point == vfit_input[m]) {
            src_idx = m;
            break;
            if (m == vfit_input.size() - 1) throw;
          }
        }
        TH1F *src_hist = hists[src_idx];
        RooBinning xbins (src_hist->GetNbinsX(),((src_hist->GetXaxis())->GetXbins())->GetArray()) ;

        for (unsigned e = 0; e < sextrap[l].second.size(); ++e) {
          double target_mean = fitMean.Eval(dextrap[l].second.at(e));
          double target_yield = fitYield.Eval(dextrap[l].second.at(e));
          double target_res = fitRMS.Eval(dextrap[l].second.at(e));
          double shift = target_mean - src_hist->GetMean();
          double res = target_res / src_hist->GetRMS();
          RooRealVar x("x","x",0,350) ; 
          RooDataHist dx("dx","dx",x,src_hist); 
          RooRealVar a("a","a",(shift),-80,80) ; 
          RooRealVar b("b","b",(res),0,5) ; 
          RooRealVar nmean("m","m",(src_hist->GetMean()),0,350) ; 
          RooFormulaVar xf("xf","m+((x-m-a)/b)",RooArgSet(x,a,b,nmean)) ; 
          RooFormulaVar xfnorms("xfnorms","x-a",RooArgSet(x,a)) ; 
          RooHistPdf hx("hx","hx",no_rms ? xfnorms : xf,x,dx,1) ;
          TH1 * shift_result = hx.createHistogram("test",x, RooFit::Binning(xbins));
          shift_result->SetName((mcs[j]+sextrap[l].second.at(e)+append).c_str());
          shift_result->SetTitle((mcs[j]+sextrap[l].second.at(e)+append).c_str());
          if (no_rms) target_yield = src_hist->Integral();
          if (target_yield < 0.0) {
            cerr << "WARNING NEGATIVE YIELD!!!" << endl;
            target_yield = 0.01;
          }
          shift_result->Scale(target_yield / shift_result->Integral());
          double av_weight = ( src_hist->Integral() / double(src_hist->GetEntries()));
          shift_result->Sumw2();
          for (int k = 1; k <= shift_result->GetNbinsX(); ++k) shift_result->SetBinError(k, shift_result->GetBinError(k) * av_weight);
          shift_result->Write("",TObject::kOverwrite);
          std::cout << "Transformed: " << cats[i] << ":" << mcs[j]+src_point+append << " to " << cats[i] << ":" << (mcs[j]+sextrap[l].second.at(e)+append) << std::endl;
        }
      }

      ic::Plot compare;
      compare.output_filename = cats[i]+"_"+mcs[j]+".pdf";
      cout << test_idx << " " << test_predict_idx << endl;
      ic::TH1PlotElement p1("orig", hists[test_idx], "Input Dist: m_{H} = "+test_source_str);
      ic::TH1PlotElement p2("start", hists[test_predict_idx] , "Reference m_{H} = "+test_predict_str);
      ic::TH1PlotElement p3("result", (TH1F*)test_shifted, "Result");
      SetStyle(p1, 1);
      SetStyle(p2, 2);
      SetStyle(p3, 4);
      compare.AddTH1PlotElement(p1);
      compare.AddTH1PlotElement(p2);
      compare.AddTH1PlotElement(p3);
      compare.x_axis_title = "M_{#tau#tau} [GeV]";
      compare.GeneratePlot();
    }

  //   // input_dc->Write();
  }



  input_dc->Close();




  return 0;
}
