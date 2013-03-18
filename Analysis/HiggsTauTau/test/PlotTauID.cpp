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

struct Yields {
  double data;
  double data_err;
  double ztt;
  double ztt_err;
  double zll;
  double zll_err;
  double w;
  double w_err;
  double vv;
  double vv_err;
  double top;
  double top_err;
  double qcd;
  double qcd_err;
};


int main(int argc, char* argv[]){
  using namespace ic;
  using namespace std;


  vector<string> lines = ParseFileLines("test.txt");

  double xbins[7] = { 20., 30., 40., 50., 75., 100., 200.};
  std::vector<Yields> yields;

  bool start_reading = false;
  for (unsigned i = 0; i < lines.size(); ++i) {
    vector<string> words;
    boost::split(words, lines[i], boost::is_any_of(" "), boost::token_compress_on);
    if (words.size() > 0) {
      if (words[0] == "category") 
        {
          start_reading = true;
          yields.push_back(Yields());
        }
    }
    if (start_reading) {
      if (words[0] == "Data") {
        yields.back().data = boost::lexical_cast<double>(words[1]);
        yields.back().data_err = boost::lexical_cast<double>(words[2]);
      }
      if (words[0] == "Top") {
        yields.back().top = boost::lexical_cast<double>(words[1]);
        yields.back().top_err = boost::lexical_cast<double>(words[2]);
      }
      if (words[0] == "VV") {
        yields.back().vv = boost::lexical_cast<double>(words[1]);
        yields.back().vv_err = boost::lexical_cast<double>(words[2]);
      }
      if (words[0] == "ZLL") {
        yields.back().zll = boost::lexical_cast<double>(words[1]);
        yields.back().zll_err = boost::lexical_cast<double>(words[2]);
      }
      if (words[0] == "ZTT") {
        yields.back().ztt = boost::lexical_cast<double>(words[1]);
        yields.back().ztt_err = boost::lexical_cast<double>(words[2]);
      }
      if (words[0] == "W") {
        yields.back().w = boost::lexical_cast<double>(words[1]);
        yields.back().w_err = boost::lexical_cast<double>(words[2]);
      }
      if (words[0] == "QCD") {
        yields.back().qcd = boost::lexical_cast<double>(words[1]);
        yields.back().qcd_err = boost::lexical_cast<double>(words[2]);
        start_reading = false;
      }
    }
  }

  std::cout << "Found entries: " << yields.size() << std::endl;
  TH1F result("result","result", 6, xbins);
  for (unsigned i = 0; i < yields.size(); ++i) {
    double data_ztt = yields[i].data - (
        yields[i].qcd +
        yields[i].w +
        yields[i].top +
        yields[i].vv +
        yields[i].zll
      );
    double data_err = 0.0;
    data_err += (
      yields[i].data_err * yields[i].data_err +
      yields[i].w_err * yields[i].w_err +
      yields[i].top_err * yields[i].top_err +
      yields[i].vv_err * yields[i].vv_err +
      yields[i].zll_err * yields[i].zll_err
      );
    data_err = sqrt(data_err);
    double mc_ztt = yields[i].ztt;
    double tot_err = (data_ztt/mc_ztt) * sqrt ( pow(data_err/data_ztt,2) + pow(yields[i].ztt_err/mc_ztt,2) );
    result.SetBinContent(i+1, data_ztt/mc_ztt);
    result.SetBinError(i+1, tot_err);
  }

  ic::Plot plot;
  plot.output_filename = "tau_id_pt.pdf";
  TH1PlotElement ele("ratio", &result);
  SetDataStyle(ele);
  plot.AddTH1PlotElement(ele);
  plot.GeneratePlot();
  // string input = "";
  // string append = "";
  // string channel = ""; // muTau
  // string prod = "";
  // string fit_input = "";
  // string extrap = "";
  // string patch_shape = "";
  // bool   no_rms = false;
  // bool   force_yield = false;

  // po::options_description config("Configuration");
  // config.add_options()
  //   ("input",                 po::value<string>(&input)->required(), "input")
  //   ("append",                po::value<string>(&append)->default_value(""), "append")
  //   ("channel",               po::value<string>(&channel)->required(), "channel")
  //   ("prod",                  po::value<string>(&prod)->required(), "prod")
  //   ("fit_input",             po::value<string>(&fit_input)->default_value("110,115,120,125,130,135,140,145"), "fit_input")
  //   ("patch_shape",           po::value<string>(&patch_shape)->default_value(""), "patch_shape")
  //   ("no_rms",                po::value<bool>(&no_rms)->default_value(false), "fit_input")
  //   ("force_yield",           po::value<bool>(&force_yield)->default_value(false), "force_yield")
  //   ("extrap",                po::value<std::string>(&extrap)->required(), "extrap");
  //   // 145=135,125,115,105:
  // po::variables_map vm;
  // po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  // po::notify(vm);


  // Plot::SetTdrStyle();

  // TFile *patch_file = new TFile(patch_shape.c_str());

  // TFile *input_dc = new TFile(input.c_str(),"UPDATE");
  // cout << "Updating file: " << input << endl;
  // cout << "Using channel: " << channel << endl;
  // cout << "Applying to signal samples: " << prod << endl;
  // cout << "Fit points:    " << fit_input << endl;
  // cout << "Appending:     " << append << endl;
  // if (patch_shape != "") {
  //   cout << "Patching shapes from file: " << patch_shape << endl;
  // }

  // vector<string> mcs;
  // boost::split(mcs, prod, boost::is_any_of(","));

  // vector<string> vfit_input;
  // boost::split(vfit_input, fit_input, boost::is_any_of(","));
  // vector<double> vfit_input_vals;
  // for (unsigned i = 0; i < vfit_input.size(); ++i) vfit_input_vals.push_back(boost::lexical_cast<double>(vfit_input[i]));

  // vector<string> vextraps;
  // vector<pair<double, vector<double> > > dextrap;
  // vector<pair<string, vector<string> > > sextrap;

  // boost::split(vextraps, extrap, boost::is_any_of(":"));
  // for (unsigned i = 0; i < vextraps.size(); ++i) {
  //   vector<string> tmp;
  //   boost::split(tmp, vextraps[i], boost::is_any_of("="));
  //   vector<string> tmp2;
  //   boost::split(tmp2, tmp.at(1), boost::is_any_of(","));
  //   std::cout << "Extrapolate from " << tmp.at(0) << " to: " << tmp.at(1) << endl;
  //   sextrap.push_back(make_pair(tmp.at(0),tmp2));
  //   double source = boost::lexical_cast<double>(tmp.at(0));
  //   vector<double> dtmp;
  //   for (unsigned j = 0; j < tmp2.size(); ++j) dtmp.push_back(boost::lexical_cast<double>(tmp2[j]));
  //   dextrap.push_back(make_pair(source, dtmp));
  // }

  // std::vector<string> cats;
  // cats.push_back(channel+"_vbf");
  // cats.push_back(channel+"_boost");
  // cats.push_back(channel+"_boost_high");
  // cats.push_back(channel+"_boost_low");
  // cats.push_back(channel+"_0jet_high");
  // cats.push_back(channel+"_0jet_low");
  // cats.push_back(channel+"_inclusive");


  // std::string test_source_str = "145";
  // std::string test_predict_str = "110";

  // for (unsigned i = 0; i < cats.size(); ++i) {
  //   for (unsigned j = 0; j < mcs.size(); ++j) {
  //     if (!gDirectory->cd(("/"+cats[i]).c_str())) continue;

  //     std::vector<TH1F *> hists;
  //     for (unsigned k = 0; k < vfit_input.size(); ++k) hists.push_back((TH1F*)gDirectory->Get((mcs[j]+vfit_input[k]+append).c_str()));
  //     bool should_skip = false;
  //     for (unsigned k = 0; k < vfit_input.size(); ++k) {
  //       if (!hists[k]) {
  //         cout << "Histogram not found, skipping!" << endl; 
  //         should_skip = true;
  //       }
  //     }
  //     if (should_skip) continue;
  //     // double mass[8] = {110, 115, 120, 125, 130, 135, 140, 145};
  //     double mean[hists.size()];
  //     double rms[hists.size()];
  //     double yield[hists.size()];
  //     for (unsigned k = 0; k < hists.size(); ++k) {
  //       mean[k] = hists[k]->GetMean();    
  //       rms[k] = hists[k]->GetRMS();    
  //       yield[k] = hists[k]->Integral();    
  //     }
  //     TGraph gMean (8, &(vfit_input_vals[0]), mean);
  //     gMean.SetName("mean");
  //     TGraph gRMS (8, &(vfit_input_vals[0]), rms);
  //     gRMS.SetName("rms");
  //     TGraph gYield (8, &(vfit_input_vals[0]), yield);
  //     gYield.SetName("yield");
      
  //     TF1 fitMean("fitmean","pol1",vfit_input_vals[0],vfit_input_vals.back());
  //     TF1 fitYield("fityield","pol1",vfit_input_vals[0],vfit_input_vals.back());
  //     TF1 fitRMS("fitrms","pol1",vfit_input_vals[0],vfit_input_vals.back());
  //     std::cout << "Fitting " << mcs[j] << " in category " << cats[i] << std::endl;
  //     gMean.Fit("fitmean","R","SAME");
  //     TCanvas cMean("cMean","cMean",700,700);
  //     gMean.Draw("ap");
  //     gMean.GetXaxis()->SetTitle("Mass Point");
  //     gMean.GetYaxis()->SetTitle("Distribution Mean");
  //     cMean.SaveAs(("fit_mean_"+cats[i]+"_"+mcs[j]+".pdf").c_str());
  //     cMean.SaveAs(("fit_mean_"+cats[i]+"_"+mcs[j]+".png").c_str());
  //     gYield.Fit("fityield","R","SAME");
  //     TCanvas cYield("cYield","cYield",700,700);
  //     gYield.Draw("ap");
  //     gYield.GetXaxis()->SetTitle("Mass Point");
  //     gYield.GetYaxis()->SetTitle("Distribution Yield");
  //     cYield.SaveAs(("fit_yield_"+cats[i]+"_"+mcs[j]+".pdf").c_str());
  //     cYield.SaveAs(("fit_yield_"+cats[i]+"_"+mcs[j]+".png").c_str());
  //     gRMS.Fit("fitrms","R","SAME");
  //     TCanvas cRMS("cRMS","cRMS",700,700);
  //     gRMS.Draw("ap");
  //     gRMS.GetXaxis()->SetTitle("Mass Point");
  //     gRMS.GetYaxis()->SetTitle("Distribution RMS");
  //     cRMS.SaveAs(("fit_rms_"+cats[i]+"_"+mcs[j]+".pdf").c_str());
  //     cRMS.SaveAs(("fit_rms_"+cats[i]+"_"+mcs[j]+".png").c_str());
  //     std::cout << "\n\n\n" << std::endl;

  //     unsigned test_idx = 0;
  //     for (unsigned p = 0; p < vfit_input.size(); ++p) {
  //       if (test_source_str == vfit_input[p]) {
  //         test_idx = p;
  //         break;
  //         if (p == vfit_input.size() - 1) throw;
  //       }
  //     }
  //     unsigned test_predict_idx = 0;
  //     for (unsigned p = 0; p < vfit_input.size(); ++p) {
  //       if (test_predict_str == vfit_input[p]) {
  //         test_predict_idx = p;
  //         break;
  //         if (p == vfit_input.size() - 1) throw;
  //       }
  //     }
  //     std::cout << "Found test idx: " << test_idx << endl;
  //     std::cout << "Found predict idx: " << test_predict_idx << endl;

  //     TH1F *test_input = hists[test_idx];
  //     RooBinning xbins (test_input->GetNbinsX(),((test_input->GetXaxis())->GetXbins())->GetArray()) ;
  //     double test_predict = boost::lexical_cast<double>(test_predict_str);
  //     double target_mean = fitMean.Eval(test_predict);
  //     double target_yield = fitYield.Eval(test_predict);
  //     double target_res = fitRMS.Eval(test_predict);
  //     double shift = target_mean - test_input->GetMean();
  //     double res = target_res / test_input->GetRMS();
  //     // std::cout << "res: " << res << std::endl;
  //     RooRealVar x("x","x",0,350) ; 
  //     RooDataHist dx("dx","dx",x,test_input); 
  //     RooRealVar a("a","a",(shift),-80,80) ; 
  //     RooRealVar b("b","b",(res),0,5) ; 
  //     RooRealVar nmean("m","m",(test_input->GetMean()),0,350) ; 
  //     RooFormulaVar xf("xf","m+((x-m-a)/b)",RooArgSet(x,a,b,nmean)) ; 
  //     RooHistPdf hx("hx","hx",xf,x,dx,1) ;
  //     TH1 * test_shifted = hx.createHistogram("test",x, RooFit::Binning(xbins));
  //     test_shifted->SetName((mcs[j]+"test").c_str());
  //     test_shifted->SetTitle((mcs[j]+"test").c_str());
  //     test_shifted->Scale(target_yield / test_shifted->Integral());
  //     double av_weight = ( test_input->Integral() / double(test_input->GetEntries()));
  //     test_shifted->Sumw2();
  //     for (int k = 1; k <= test_shifted->GetNbinsX(); ++k) test_shifted->SetBinError(k, test_shifted->GetBinError(k) * av_weight);
  //     test_shifted->Write("",TObject::kOverwrite);

  //     for (unsigned l = 0; l < sextrap.size(); ++l) {
  //       string src_point = sextrap[l].first;
  //       unsigned src_idx = 0;
  //       for (unsigned m = 0; m < vfit_input.size(); ++m) {
  //         if (src_point == vfit_input[m]) {
  //           src_idx = m;
  //           break;
  //           if (m == vfit_input.size() - 1) throw;
  //         }
  //       }
  //       TH1F *src_hist = hists[src_idx];
  //       RooBinning xbins (src_hist->GetNbinsX(),((src_hist->GetXaxis())->GetXbins())->GetArray()) ;

  //       for (unsigned e = 0; e < sextrap[l].second.size(); ++e) {
  //         double target_mean = fitMean.Eval(dextrap[l].second.at(e));
  //         double target_yield = fitYield.Eval(dextrap[l].second.at(e));
  //         double target_res = fitRMS.Eval(dextrap[l].second.at(e));
  //         double shift = target_mean - src_hist->GetMean();
  //         double res = target_res / src_hist->GetRMS();
  //         RooRealVar x("x","x",0,350) ; 
  //         RooDataHist dx("dx","dx",x,src_hist); 
  //         RooRealVar a("a","a",(shift),-80,80) ; 
  //         RooRealVar b("b","b",(res),0,5) ; 
  //         RooRealVar nmean("m","m",(src_hist->GetMean()),0,350) ; 
  //         RooFormulaVar xf("xf","m+((x-m-a)/b)",RooArgSet(x,a,b,nmean)) ; 
  //         RooFormulaVar xfnorms("xfnorms","x-a",RooArgSet(x,a)) ; 
  //         RooHistPdf hx("hx","hx",no_rms ? xfnorms : xf,x,dx,1) ;
  //         TH1 * shift_result = hx.createHistogram("test",x, RooFit::Binning(xbins));
  //         TH1 * patch_result = NULL;
  //         if (patch_shape != "") {
  //           patch_file->cd();
  //           if (!gDirectory->cd(("/"+cats[i]).c_str())) continue;
  //           if (gDirectory->Get((mcs[j]+sextrap[l].second.at(e)+append).c_str())) {
  //             cout << "TH1F appears to exist in patch file..." << endl;
  //             patch_result = (TH1*)(gDirectory->Get((mcs[j]+sextrap[l].second.at(e)+append).c_str())->Clone("dummy"));
  //           }
  //           // cout << "here2" << endl;
  //           input_dc->cd();
  //           if (!gDirectory->cd(("/"+cats[i]).c_str())) continue;
  //           if (patch_result) std::cout << "Shape " << (mcs[j]+sextrap[l].second.at(e)+append) << " patched from file " << patch_shape << endl; 
  //         }
  //         if (patch_result) shift_result = patch_result;
  //         if (src_hist->Integral() <= 0.0) {
  //           cout << "Extrapolation input histogram is empty... an empty histogram will be copied" << endl;
  //           shift_result = (TH1F*)src_hist->Clone();
  //         }
  //         if (no_rms && !force_yield) target_yield = src_hist->Integral();
  //         shift_result->SetName((mcs[j]+sextrap[l].second.at(e)+append).c_str());
  //         shift_result->SetTitle((mcs[j]+sextrap[l].second.at(e)+append).c_str());

  //         if (target_yield < 0.0) {
  //           cerr << "WARNING NEGATIVE YIELD!!!" << endl;
  //           target_yield = 0.01;
  //         }

  //         if (shift_result->Integral() > 0) {
  //           shift_result->Scale(target_yield / shift_result->Integral());
  //           double av_weight = ( src_hist->Integral() / double(src_hist->GetEntries()));
  //           shift_result->Sumw2();
  //           for (int k = 1; k <= shift_result->GetNbinsX(); ++k) shift_result->SetBinError(k, shift_result->GetBinError(k) * av_weight);
  //         }
  //         shift_result->Write("",TObject::kOverwrite);
  //         std::cout << "Transformed: " << cats[i] << ":" << mcs[j]+src_point+append << " to " << cats[i] << ":" << (mcs[j]+sextrap[l].second.at(e)+append) << std::endl;
  //       }
  //     }

  //     ic::Plot compare;
  //     compare.output_filename = cats[i]+"_"+mcs[j]+".pdf";
  //     cout << test_idx << " " << test_predict_idx << endl;
  //     ic::TH1PlotElement p1("orig", hists[test_idx], "Input Dist: m_{H} = "+test_source_str);
  //     ic::TH1PlotElement p2("start", hists[test_predict_idx] , "Reference m_{H} = "+test_predict_str);
  //     ic::TH1PlotElement p3("result", (TH1F*)test_shifted, "Result");
  //     SetStyle(p1, 1);
  //     SetStyle(p2, 2);
  //     SetStyle(p3, 4);
  //     compare.AddTH1PlotElement(p1);
  //     compare.AddTH1PlotElement(p2);
  //     compare.AddTH1PlotElement(p3);
  //     compare.x_axis_title = "M_{#tau#tau} [GeV]";
  //     compare.GeneratePlot();
  //   }

  // //   // input_dc->Write();
  // }



  // input_dc->Close();




  return 0;
}
