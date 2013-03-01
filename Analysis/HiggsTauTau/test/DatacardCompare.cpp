#include <iostream>
#include <vector>
#include <map>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "TROOT.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TextElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SimpleParamParser.h"
#include <boost/algorithm/string.hpp>

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
  static bool first_time = true;
  static vector<int> colors;
  if (first_time) {
    colors.push_back(1);
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
    colors.push_back(1);
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

 // 00      $CHANNEL_incl_low             incl  pt-low
 // 01      $CHANNEL_incl_high            incl  pt-high
 // 02      $CHANNEL_boost_low            boost pt-low
 // 03      $CHANNEL_boost_high           boost pt-high
 // 04      $CHANNEL_2jet                 2jet
 // 05      $CHANNEL_vbf                  vbf
 // 06      $CHANNEL_btag_low             btag  pt-low
 // 07      $CHANNEL_btag_high            btag  pt-high
 // 08 inclusive!!
 // 09 dilepton!!

  vector<string> inputs;
  vector<string> labels;

  string category;
  std::string output_file;
  unsigned mode = 0;
  unsigned mssm_mode = 0;
  bool plot = true;
  bool print_err = false;
  bool show_errs = false;

  po::options_description preconfig("Pre-Configuration");
  po::options_description config("Configuration");
  preconfig.add_options()
    ("inputs",               po::value<std::vector<string> >(&inputs)->multitoken(), "inputs")
    ("labels",               po::value<std::vector<string> >(&labels)->multitoken(), "labels")
    ("category",               po::value<string>(&category)->required(), "category")
    ("show_errs",               po::value<bool>(&show_errs)->default_value(false), "category")
    ("output_file",               po::value<string>(&output_file)->required(), "output_file")
    ("mode",               po::value<unsigned>(&mode)->default_value(0), "mode")
    ("mssm_mode",               po::value<unsigned>(&mssm_mode)->default_value(0), "mssm_mode");

  
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
            options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);

  unsigned n = inputs.size();

  vector<double> data_obs_yield(n, 0.0);
  vector<double> ztt_yield(n, 0.0);
  vector<double> w_yield(n, 0.0);
  vector<double> qcd_yield(n, 0.0);
  vector<double> top_yield(n, 0.0);
  vector<double> vv_yield(n, 0.0);
  vector<double> zll_yield(n, 0.0);
  vector<double> zj_yield(n, 0.0);
  vector<double> zl_yield(n, 0.0);
  vector<double> tot_yield(n, 0.0);
  vector<double> vh_yield(n, 0.0);
  vector<double> ggh_yield(n, 0.0);
  vector<double> qqh_yield(n, 0.0);

  vector<double> data_obs_yield_err(n, 0.0);
  vector<double> ztt_yield_err(n, 0.0);
  vector<double> w_yield_err(n, 0.0);
  vector<double> qcd_yield_err(n, 0.0);
  vector<double> top_yield_err(n, 0.0);
  vector<double> vv_yield_err(n, 0.0);
  vector<double> zll_yield_err(n, 0.0);
  vector<double> zj_yield_err(n, 0.0);
  vector<double> zl_yield_err(n, 0.0);
  vector<double> tot_yield_err(n, 0.0);
  vector<double> vh_yield_err(n, 0.0);
  vector<double> ggh_yield_err(n, 0.0);
  vector<double> qqh_yield_err(n, 0.0);

  vector<string> comp_plots;
  if (mode != 2) {
    comp_plots.push_back("data_obs");
    comp_plots.push_back("ZTT");
    comp_plots.push_back("W");
    comp_plots.push_back("QCD");
    comp_plots.push_back("TT");
    comp_plots.push_back("VV");
    comp_plots.push_back("ZLL");
    comp_plots.push_back("ZL");
    comp_plots.push_back("ZJ");
  } else {
    comp_plots.push_back("data_obs");
    comp_plots.push_back("Ztt");
    comp_plots.push_back("Fakes");
    comp_plots.push_back("ttbar");
    comp_plots.push_back("EWK");
  }
  if (mssm_mode == 0) {
    comp_plots.push_back("VH90");
    comp_plots.push_back("ggH90");
    comp_plots.push_back("qqH90");
    comp_plots.push_back("VH105");
    comp_plots.push_back("ggH105");
    comp_plots.push_back("qqH105");
    comp_plots.push_back("VH125");
    comp_plots.push_back("ggH125");
    comp_plots.push_back("qqH125");
  } else {
    comp_plots.push_back("ggH120");
    comp_plots.push_back("bbH120");
  }

  vector<TFile *> tfiles(n, NULL);
  if (mssm_mode == 0) {
    if (mode != 2) {
    std::cout << boost::format("%-25s %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f\n") 
         % (category)  % ("data_obs") % ("ZTT") % ("W") % ("QCD")
                % ("TT") % ("VV") % ("ZLL") % ("ZJ") % ("ZL") % ("TOT") % ("DATA/MC") % ("VH") % ("ggH") % ("qqH");
    } else {
     std::cout << boost::format("%-25s %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f\n") 
          % (category)  % ("data_obs") % ("Ztt") % ("Fakes") % ("ttbar")
                 % ("EWK") % ("TOT") % ("DATA/MC")  % ("VH") % ("ggH") % ("qqH"); 
    }
  } else {
    if (mode != 2) {
    std::cout << boost::format("%-25s %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f\n") 
         % (category)  % ("data_obs") % ("ZTT") % ("W") % ("QCD")
                % ("TT") % ("VV") % ("ZLL") % ("ZJ") % ("ZL") % ("TOT") % ("DATA/MC") % ("ggH") % ("bbH");
    } else {
     std::cout << boost::format("%-25s %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f\n") 
          % (category)  % ("data_obs") % ("Ztt") % ("Fakes") % ("ttbar")
                 % ("EWK") % ("TOT") % ("DATA/MC")  % ("ggH") % ("bbH"); 
    }
  }
  for (unsigned i = 0; i < inputs.size(); ++i) {
    tfiles[i] = new TFile(inputs[i].c_str());
    if (!tfiles[i]) {
      cerr << "Error, file " << inputs[i] << " could not be opened, quitting." << endl;
      exit(1);
    }
    tfiles[i]->cd();
    std::string replacement = category;

    if (mode != 2) {
    if (category.find("eleTau") != category.npos && labels[i].find("MIT") != labels[i].npos) {
      boost::replace_all(replacement, "eleTau", "eTau");
    }
    }
    gDirectory->cd(replacement.c_str());

    if (mode !=2) {
      data_obs_yield[i] = (TH1F*)gDirectory->Get("data_obs") ? Integral((TH1F*)gDirectory->Get("data_obs")) : 0;
      ztt_yield[i] =      (TH1F*)gDirectory->Get("ZTT") ? Integral((TH1F*)gDirectory->Get("ZTT")) : 0;
      w_yield[i] =        (TH1F*)gDirectory->Get("W") ? Integral((TH1F*)gDirectory->Get("W")) : 0;
      qcd_yield[i] =      (TH1F*)gDirectory->Get("QCD") ? Integral((TH1F*)gDirectory->Get("QCD")) : 0;
      top_yield[i] =      (TH1F*)gDirectory->Get("TT") ? Integral((TH1F*)gDirectory->Get("TT")) : 0;
      vv_yield[i] =       (TH1F*)gDirectory->Get("VV") ? Integral((TH1F*)gDirectory->Get("VV")) : 0;
      zll_yield[i] =      (TH1F*)gDirectory->Get("ZLL") ? Integral((TH1F*)gDirectory->Get("ZLL")) : 0;
      zj_yield[i] =       (TH1F*)gDirectory->Get("ZJ") ? Integral((TH1F*)gDirectory->Get("ZJ")) : 0;
      zl_yield[i] =       (TH1F*)gDirectory->Get("ZL") ? Integral((TH1F*)gDirectory->Get("ZL")) : 0;
      vh_yield[i] =       (TH1F*)gDirectory->Get("VH125") ? Integral((TH1F*)gDirectory->Get("VH125")) : 0;
      ggh_yield[i] =       (TH1F*)gDirectory->Get("ggH125") ? Integral((TH1F*)gDirectory->Get("ggH125")) : 0;
      qqh_yield[i] =       (TH1F*)gDirectory->Get("qqH125") ? Integral((TH1F*)gDirectory->Get("qqH125")) : 0;
      data_obs_yield_err[i] = (TH1F*)gDirectory->Get("data_obs") ? Error((TH1F*)gDirectory->Get("data_obs")) : 0;
      ztt_yield_err[i] =      (TH1F*)gDirectory->Get("ZTT") ? Error((TH1F*)gDirectory->Get("ZTT")) : 0;
      w_yield_err[i] =        (TH1F*)gDirectory->Get("W") ? Error((TH1F*)gDirectory->Get("W")) : 0;
      qcd_yield_err[i] =      (TH1F*)gDirectory->Get("QCD") ? Error((TH1F*)gDirectory->Get("QCD")) : 0;
      top_yield_err[i] =      (TH1F*)gDirectory->Get("TT") ? Error((TH1F*)gDirectory->Get("TT")) : 0;
      vv_yield_err[i] =       (TH1F*)gDirectory->Get("VV") ? Error((TH1F*)gDirectory->Get("VV")) : 0;
      zll_yield_err[i] =      (TH1F*)gDirectory->Get("ZLL") ? Error((TH1F*)gDirectory->Get("ZLL")) : 0;
      zj_yield_err[i] =       (TH1F*)gDirectory->Get("ZJ") ? Error((TH1F*)gDirectory->Get("ZJ")) : 0;
      zl_yield_err[i] =       (TH1F*)gDirectory->Get("ZL") ? Error((TH1F*)gDirectory->Get("ZL")) : 0;
      vh_yield_err[i] =       (TH1F*)gDirectory->Get("VH125") ? Error((TH1F*)gDirectory->Get("VH125")) : 0;
      ggh_yield_err[i] =       (TH1F*)gDirectory->Get("ggH125") ? Error((TH1F*)gDirectory->Get("ggH125")) : 0;
      qqh_yield_err[i] =       (TH1F*)gDirectory->Get("qqH125") ? Error((TH1F*)gDirectory->Get("qqH125")) : 0;
    } else {
      data_obs_yield[i] = (TH1F*)gDirectory->Get("data_obs") ? Integral((TH1F*)gDirectory->Get("data_obs")) : 0;
      ztt_yield[i] =      (TH1F*)gDirectory->Get("Ztt") ? Integral((TH1F*)gDirectory->Get("Ztt")) : 0;
      qcd_yield[i] =      (TH1F*)gDirectory->Get("Fakes") ? Integral((TH1F*)gDirectory->Get("Fakes")) : 0;
      top_yield[i] =      (TH1F*)gDirectory->Get("ttbar") ? Integral((TH1F*)gDirectory->Get("ttbar")) : 0;
      vv_yield[i] =       (TH1F*)gDirectory->Get("EWK") ? Integral((TH1F*)gDirectory->Get("EWK")) : 0;
      vh_yield[i] =       (TH1F*)gDirectory->Get("VH125") ? Integral((TH1F*)gDirectory->Get("VH125")) : 0;
      ggh_yield[i] =       (TH1F*)gDirectory->Get("ggH125") ? Integral((TH1F*)gDirectory->Get("ggH125")) : 0;
      qqh_yield[i] =       (TH1F*)gDirectory->Get("qqH125") ? Integral((TH1F*)gDirectory->Get("qqH125")) : 0;
      data_obs_yield_err[i] = (TH1F*)gDirectory->Get("data_obs") ? Error((TH1F*)gDirectory->Get("data_obs")) : 0;
      ztt_yield_err[i] =      (TH1F*)gDirectory->Get("Ztt") ? Error((TH1F*)gDirectory->Get("Ztt")) : 0;
      qcd_yield_err[i] =      (TH1F*)gDirectory->Get("Fakes") ? Error((TH1F*)gDirectory->Get("Fakes")) : 0;
      top_yield_err[i] =      (TH1F*)gDirectory->Get("ttbar") ? Error((TH1F*)gDirectory->Get("ttbar")) : 0;
      vv_yield_err[i] =       (TH1F*)gDirectory->Get("EWK") ? Error((TH1F*)gDirectory->Get("EWK")) : 0;
      vh_yield_err[i] =       (TH1F*)gDirectory->Get("VH125") ? Error((TH1F*)gDirectory->Get("VH125")) : 0;
      ggh_yield_err[i] =       (TH1F*)gDirectory->Get("ggH125") ? Error((TH1F*)gDirectory->Get("ggH125")) : 0;
      qqh_yield_err[i] =       (TH1F*)gDirectory->Get("qqH125") ? Error((TH1F*)gDirectory->Get("qqH125")) : 0;
    }
    if (mssm_mode == 0) {
      vh_yield[i] =       (TH1F*)gDirectory->Get("VH125") ? Integral((TH1F*)gDirectory->Get("VH125")) : 0;
      ggh_yield[i] =       (TH1F*)gDirectory->Get("ggH125") ? Integral((TH1F*)gDirectory->Get("ggH125")) : 0;
      qqh_yield[i] =       (TH1F*)gDirectory->Get("qqH125") ? Integral((TH1F*)gDirectory->Get("qqH125")) : 0;
      vh_yield_err[i] =       (TH1F*)gDirectory->Get("VH125") ? Error((TH1F*)gDirectory->Get("VH125")) : 0;
      ggh_yield_err[i] =       (TH1F*)gDirectory->Get("ggH125") ? Error((TH1F*)gDirectory->Get("ggH125")) : 0;
      qqh_yield_err[i] =       (TH1F*)gDirectory->Get("qqH125") ? Error((TH1F*)gDirectory->Get("qqH125")) : 0;

    } else {
      ggh_yield[i] =       (TH1F*)gDirectory->Get("ggH120") ? Integral((TH1F*)gDirectory->Get("ggH120")) : 0;
      qqh_yield[i] =       (TH1F*)gDirectory->Get("bbH120") ? Integral((TH1F*)gDirectory->Get("bbH120")) : 0;
      ggh_yield_err[i] =       (TH1F*)gDirectory->Get("ggH120") ? Error((TH1F*)gDirectory->Get("ggH120")) : 0;
      qqh_yield_err[i] =       (TH1F*)gDirectory->Get("bbH120") ? Error((TH1F*)gDirectory->Get("bbH120")) : 0;

    }



    if (zll_yield[i] == 0) zll_yield[i] = zj_yield[i] + zl_yield[i];
    if (mode != 2) {
      tot_yield[i] = ztt_yield[i] + w_yield[i] + qcd_yield[i] + top_yield[i] + vv_yield[i] + zll_yield[i];
    } else {
      tot_yield[i] = ztt_yield[i]  + qcd_yield[i] + top_yield[i] + vv_yield[i];

    }

    if (mssm_mode == 0) {
      if (mode != 2) {
      std::cout << boost::format("%-25s %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f\n") 
           % (labels[i])  % data_obs_yield[i] % ztt_yield[i] % w_yield[i] % qcd_yield[i] 
                          % top_yield[i] % vv_yield[i] % zll_yield[i] % zj_yield[i] % zl_yield[i] % tot_yield[i] % (data_obs_yield[i]/tot_yield[i])
                          % vh_yield[i] % ggh_yield[i] % qqh_yield[i];
      if (print_err) {
        std::cout << boost::format("%-25s %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f\n") 
             % (labels[i])  % data_obs_yield_err[i] % ztt_yield_err[i] % w_yield_err[i] % qcd_yield_err[i] 
                            % top_yield_err[i] % vv_yield_err[i] % zll_yield_err[i] % zj_yield_err[i] % zl_yield_err[i] % tot_yield_err[i] % (data_obs_yield_err[i]/tot_yield_err[i])
                            % vh_yield_err[i] % ggh_yield_err[i] % qqh_yield_err[i];



      }
      } else {
        std::cout << boost::format("%-25s %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f\n") 
             % (labels[i])  % data_obs_yield[i] % ztt_yield[i] % qcd_yield[i] 
                            % top_yield[i] % vv_yield[i] % tot_yield[i] % (data_obs_yield[i]/tot_yield[i])
                            % vh_yield[i] % ggh_yield[i] % qqh_yield[i];
        if (print_err) {
          std::cout << boost::format("%-25s %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f\n") 
               % (labels[i])  % data_obs_yield_err[i] % ztt_yield_err[i] % qcd_yield_err[i] 
                              % top_yield_err[i] % vv_yield_err[i] % tot_yield_err[i] % (data_obs_yield_err[i]/tot_yield_err[i])
                              % vh_yield_err[i] % ggh_yield_err[i] % qqh_yield_err[i];

        }
      }
    } else {
      if (mode != 2) {
      std::cout << boost::format("%-25s %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f\n") 
           % (labels[i])  % data_obs_yield[i] % ztt_yield[i] % w_yield[i] % qcd_yield[i] 
                          % top_yield[i] % vv_yield[i] % zll_yield[i] % zj_yield[i] % zl_yield[i] % tot_yield[i] % (data_obs_yield[i]/tot_yield[i])
                          % ggh_yield[i] % qqh_yield[i];
      } else {
        std::cout << boost::format("%-25s %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f  %-9.2f %-9.2f\n") 
             % (labels[i])  % data_obs_yield[i] % ztt_yield[i] % qcd_yield[i] 
                            % top_yield[i] % vv_yield[i] % tot_yield[i] % (data_obs_yield[i]/tot_yield[i])
                            % ggh_yield[i] % qqh_yield[i];
      }
    }

  }
  std::cout << std::endl;

  if (plot) {

  for (unsigned j = 0; j < comp_plots.size(); ++j) {
    ic::Plot test_plot;
    test_plot.output_filename = output_file;
    test_plot.y_axis_title = "Events";
    test_plot.x_axis_title = "SVFit Mass (GeV)";
    test_plot.title_left = "Comparison: " + comp_plots[j];
    test_plot.title_right = category;


    if (j == 0) test_plot.append = 1;
    if (j > 0 && j < (comp_plots.size()-1)) test_plot.append = 2;
    if (j == (comp_plots.size() - 1)) test_plot.append = 3;

    std::vector<ic::TH1PlotElement> ele;
    std::vector<ic::RatioPlotElement> ratio;

    for (unsigned k = 0; k < inputs.size(); ++k) {
      std::string replacement = category;

      if (mode != 2) {
        if (category.find("eleTau") != category.npos && labels[k].find("MIT") != labels[k].npos) {
          boost::replace_first(replacement, "eleTau", "eTau");
        }
      }
      // std::cout << "Looking for plot: " << comp_plots[j] << std::endl;
      ele.push_back(ic::TH1PlotElement(labels[k],tfiles[k],replacement,comp_plots[j]));
      if (ele.back().hist_ptr()) {
      SetStyle(ele.back(), k);
      if (show_errs) {
        ele.back().set_draw_stat_error_y(true);
      }
      //drawn_ele[i]->hist_ptr()->Scale(1.0, "width");
      ele.back().hist_ptr()->Scale(1.0, "width");
      ele.back().set_legend_text(labels[k]);
      test_plot.AddTH1PlotElement(ele.back());
      }

      if (k > 0) {
        ratio.push_back(ic::RatioPlotElement("ratio"+boost::lexical_cast<string>(k), labels[k], labels[0]) );
        SetStyle(ratio.back(),k);
        test_plot.AddRatioPlotElement(ratio.back());
      }
    }
    test_plot.draw_ratio_hist = true;
    test_plot.ratio_y_axis_min = 0.8;
    test_plot.ratio_y_axis_max = 1.2;
    test_plot.GeneratePlot();
  }
}

  return 0;
}
