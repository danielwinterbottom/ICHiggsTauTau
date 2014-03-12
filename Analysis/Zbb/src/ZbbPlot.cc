#include "Zbb/interface/ZbbPlot.h"
#include <iostream>
#include <vector>
#include <map>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "boost/range/algorithm.hpp"
#include "boost/range/algorithm_ext.hpp"
#include "Utilities/interface/SimpleParamParser.h"
#include "Utilities/interface/FnRootTools.h"
#include "TPad.h"
#include "TROOT.h"
#include "TColor.h"
#include "TEfficiency.h"


namespace ic {

  ZbbPlot::ZbbPlot() : config_("Config") {
    bkg_schemes_["all"] = {
      PlotBkgComponent("TT","t#bar{t}"  ,{"TT"}      , TColor::GetColor(255, 255, 84)),
      PlotBkgComponent("Zb","Z+b"       ,{"Zb"}      , TColor::GetColor(245, 77,  84)),
      PlotBkgComponent("Zc","Z+c"       ,{"Zc"}      , TColor::GetColor(91,  255, 84)),
      PlotBkgComponent("Zl","Z+l"       ,{"Zl"}      , TColor::GetColor(83,  73, 254))
    };
  }

  void ZbbPlot::SetMCStackStyle(ic::TH1PlotElement & ele, unsigned color) {
    ele.set_fill_color(color);
    ele.set_fill_style(1001);
    ele.set_draw_fill(true);
    ele.set_draw_marker(false);
    ele.set_draw_line(false);
    ele.set_line_width(2);
    ele.set_draw_stat_error_y(false);
    ele.set_in_stack(true);
    return;
  }
  void ZbbPlot::SetSignalStyle(ic::TH1PlotElement & ele, unsigned color) {
    ele.set_fill_style(1001);
    ele.set_draw_fill(true);
    ele.set_draw_marker(false);
    ele.set_draw_line(false);
    ele.set_draw_stat_error_y(false);
    ele.set_in_stack(true);
    ele.set_line_style(11);
    ele.set_fill_color(0);
    ele.set_line_color(color);
    ele.set_line_width(2);
    return;
  }
  void ZbbPlot::SetDataStyle(ic::TH1PlotElement & ele) {
    ele.set_marker_color(1);
    ele.set_line_color(1);
    ele.set_fill_color(1);
    ele.set_fill_style(0);
    ele.set_draw_fill(false);
    ele.set_line_width(2);
    ele.set_draw_marker(true);
    ele.set_draw_line(true);
    ele.set_marker_style(20);
    ele.set_draw_stat_error_y(true);
    ele.set_marker_size(1.1);
    return;
  }

  void ZbbPlot::SetRatioStyle(ic::RatioPlotElement & ele, unsigned color) {
    ele.set_marker_color(color);
    ele.set_line_color(color);
    ele.set_draw_stat_error_y(true);
    ele.set_draw_line(false);
    ele.set_draw_marker(true);
    ele.set_marker_size(1.1);
    ele.set_marker_style(20);
    return;
  }


  boost::program_options::options_description const& ZbbPlot::GenerateOptions(std::string const& prefix) {
    namespace po = boost::program_options;
    config_.add_options()
      ((prefix+"use_htt_style").c_str(),        po::value<bool>(&use_htt_style_)->default_value(false))
      ((prefix+"plot_name").c_str(),            po::value<std::string>(&plot_name_)->default_value(""))
      ((prefix+"x_axis_label").c_str(),         po::value<std::string>(&x_axis_label_)->default_value(""))
      ((prefix+"y_axis_label").c_str(),         po::value<std::string>(&y_axis_label_)->default_value("Events"))
      ((prefix+"x_axis_bin_labels").c_str(),    po::value<std::string>(&x_axis_bin_labels_)->default_value(""))
      ((prefix+"custom_y_axis_min").c_str(),    po::value<bool>(&custom_y_axis_min_)->default_value(false))
      ((prefix+"y_axis_min").c_str(),           po::value<double>(&y_axis_min_)->default_value(0.0))
      ((prefix+"custom_x_axis_range").c_str(),  po::value<bool>(&custom_x_axis_range_)->default_value(false))
      ((prefix+"x_axis_min").c_str(),           po::value<double>(&x_axis_min_)->default_value(0.0))
      ((prefix+"x_axis_max").c_str(),           po::value<double>(&x_axis_max_)->default_value(1.0))
      ((prefix+"extra_pad").c_str(),            po::value<double>(&extra_pad_)->default_value(1.0))
      ((prefix+"background_scheme").c_str(),    po::value<std::string>(&background_scheme_)->default_value(""))
      ((prefix+"title_left").c_str(),           po::value<std::string>(&title_left_)->default_value(""))
      ((prefix+"title_right").c_str(),          po::value<std::string>(&title_right_)->default_value(""))
      ((prefix+"log_y").c_str(),                po::value<bool>(&log_y_)->default_value(false))
      ((prefix+"draw_ratio").c_str(),           po::value<bool>(&draw_ratio_)->default_value(false))
      ((prefix+"norm_bins").c_str(),            po::value<bool>(&norm_bins_)->default_value(false))
      ((prefix+"blind").c_str(),                po::value<bool>(&blind_)->default_value(false))
      ((prefix+"x_blind_min").c_str(),          po::value<double>(&x_blind_min_)->default_value(0))
      ((prefix+"x_blind_max").c_str(),          po::value<double>(&x_blind_max_)->default_value(0))
      ((prefix+"auto_error_band").c_str(),      po::value<double>(&auto_error_band_)->default_value(-1.0))
      ((prefix+"draw_error_band").c_str(),      po::value<bool>(&draw_error_band_)->default_value(false))
      ((prefix+"add_stat_error").c_str(),       po::value<bool>(&add_stat_error_)->default_value(true))
      ((prefix+"ratio_min").c_str(),            po::value<double>(&ratio_min_)->default_value(0.68))
      ((prefix+"ratio_max").c_str(),            po::value<double>(&ratio_max_)->default_value(1.32));

    return config_;
  }

  void ZbbPlot::GeneratePlot(ZbbAnalysis::HistValueMap hmap) {

    ic::Plot plot;
    if (use_htt_style_) plot.use_htt_style = true;

    plot.output_filename = plot_name_+".pdf";
    if (log_y_) {
      plot.output_filename = plot_name_+"_log.pdf";
      plot.y_axis_log = true;
    }
    plot.x_bin_labels_ = x_axis_bin_labels_;

    plot.extra_pad = extra_pad_;
    plot.custom_x_axis_range = custom_x_axis_range_;
    if (custom_x_axis_range_){
      plot.x_axis_min = x_axis_min_;
      plot.x_axis_max = x_axis_max_;
    }
    if (custom_y_axis_min_) {
      plot.y_axis_min = y_axis_min_;
    }
    // if (mssm_mode == 1) plot.legend_height = 0.045;
    plot.x_axis_title = x_axis_label_;
    plot.y_axis_title = y_axis_label_;

    plot.draw_ratio_hist = draw_ratio_;

    plot.title_left = title_left_;
    plot.title_right = title_right_;

    // should check if data actually exists: we might want to make a plot
    // where it doesn't
    TH1PlotElement data_plot("data", &hmap["data_obs"].first, "Observed");
    if (norm_bins_) data_plot.hist_ptr()->Scale(1.0, "width");
    if (blind_) {
      TH1F *data_hist = &hmap["data_obs"].first;
      for (int j = 0; j < data_hist->GetNbinsX(); ++j) {
        double low_edge = data_hist->GetBinLowEdge(j+1);
        double high_edge = data_hist->GetBinWidth(j+1)+data_hist->GetBinLowEdge(j+1);
        if ((low_edge > x_blind_min_ && low_edge < x_blind_max_) || (high_edge > x_blind_min_ && high_edge < x_blind_max_)) {
          data_hist->SetBinContent(j+1,0);
          data_hist->SetBinError(j+1,0);
        }
      }
    }

    // should checkt the bakground scheme actually exists
    std::vector<PlotBkgComponent> bkg_scheme = bkg_schemes_[background_scheme_];
    std::vector<TH1PlotElement> bkg_elements;

    std::string bkr_list_for_ratio = "";


    for (unsigned i = 0; i < bkg_scheme.size(); ++i) {
      if (i == 0) bkr_list_for_ratio += bkg_scheme[i].name;
      if (i != 0) bkr_list_for_ratio += ("+"+bkg_scheme[i].name);
      bool valid_element = false;
      for (unsigned j = 0; j < bkg_scheme[i].plots.size(); ++j) {
        if (hmap.count(bkg_scheme[i].plots[j])) {
          if (!valid_element) {
            bkg_elements.push_back(TH1PlotElement(bkg_scheme[i].name, &(hmap[bkg_scheme[i].plots[j]].first), bkg_scheme[i].legend));            
            valid_element = true;
          } else {
            bkg_elements.back().hist_ptr()->Add(&hmap[bkg_scheme[i].plots[j]].first);          
          }
        }
      }
      if (valid_element) {
        ZbbPlot::SetMCStackStyle(bkg_elements.back(), bkg_scheme[i].color);
        if (norm_bins_) bkg_elements.back().hist_ptr()->Scale(1.0, "width");
        plot.AddTH1PlotElement(bkg_elements.back());        
      }
    }

    TH1F error_band;
    TH1F bkg_total;
    TH1PlotElement err_element;
    TH1PlotElement bkg_element;
    if (draw_error_band_) {
      if (auto_error_band_ <= 0.) {
        error_band = hmap["Bkg"].first;
        if (norm_bins_) error_band.Scale(1.0, "width");
      } else {
        error_band = *((TH1F*)bkg_elements[0].hist_ptr()->Clone());
        for (unsigned i = 1; i < bkg_elements.size(); ++i) {
          error_band.Add(bkg_elements[i].hist_ptr());
        }
        for (unsigned i = 1; i <= unsigned(error_band.GetNbinsX()); ++i) {
          if (add_stat_error_) {
            double from_frac = error_band.GetBinContent(i)*auto_error_band_;
            double from_bin = error_band.GetBinError(i);
            error_band.SetBinError(i, std::sqrt(from_frac*from_frac + from_bin*from_bin));
          } else {
            error_band.SetBinError(i, error_band.GetBinContent(i)*auto_error_band_);
          }
        }
      }
      bkg_total = error_band;
      for (unsigned i = 1; i <= unsigned(bkg_total.GetNbinsX()); ++i) {
        bkg_total.SetBinError(i, 0.0);
      } 
      err_element = TH1PlotElement("error_shape", &error_band,"Bkg. uncertainty");
      bkg_element = TH1PlotElement("bkg_shape", &bkg_total,"");

      err_element.set_marker_size(0);
      err_element.set_fill_color(13);
      err_element.set_fill_style(3013);
      err_element.set_line_width(1);
      err_element.set_draw_stat_error_y(true);
      err_element.set_draw_fill(true);
      err_element.set_draw_line(false);
      err_element.set_draw_marker(false);
      err_element.set_draw_options("e2");
      bkg_element.set_marker_size(0);
      bkg_element.set_fill_color(1);
      bkg_element.set_fill_style(0);
      bkg_element.set_line_width(1);
      bkg_element.set_draw_stat_error_y(true);
      bkg_element.set_draw_fill(true);
      bkg_element.set_draw_line(false);
      bkg_element.set_draw_marker(false);
      bkg_element.set_draw_options("e2");
      plot.AddTH1PlotElement(err_element);
      plot.AddTH1PlotElement(bkg_element);
    } else {
        bkg_total = *((TH1F*)bkg_elements[0].hist_ptr()->Clone());
        for (unsigned i = 1; i < bkg_elements.size(); ++i) {
          bkg_total.Add(bkg_elements[i].hist_ptr());
        }
        for (unsigned i = 1; i <= unsigned(bkg_total.GetNbinsX()); ++i) {
          bkg_total.SetBinError(i, 0.0);
        }
        bkg_element = TH1PlotElement("bkg_shape", &bkg_total,"");
        bkg_element.set_marker_size(0);
        bkg_element.set_fill_color(1);
        bkg_element.set_fill_style(0);
        bkg_element.set_line_width(1);
        bkg_element.set_draw_stat_error_y(true);
        bkg_element.set_draw_fill(true);
        bkg_element.set_draw_line(false);
        bkg_element.set_draw_marker(false);
        bkg_element.set_draw_options("e2");
        plot.AddTH1PlotElement(bkg_element);
    }

    ic::RatioPlotElement ratio("ratio","data","bkg_shape");
    ic::RatioPlotElement err_ratio("err_ratio","error_shape","bkg_shape");
    if (draw_error_band_) {
      err_ratio.set_marker_size(0);
      err_ratio.set_fill_color(13);
      err_ratio.set_fill_style(3013);
      err_ratio.set_line_width(1);
      err_ratio.set_draw_stat_error_y(true);
      err_ratio.set_draw_fill(false);
      err_ratio.set_draw_line(false);
      err_ratio.set_draw_marker(false);
      err_ratio.set_draw_options("e2");
      plot.AddRatioPlotElement(err_ratio);
    }
    ZbbPlot::SetRatioStyle(ratio,1);
    ratio.set_multi_mode(true);
    plot.ratio_y_axis_title = "Obs/Bkg";
    plot.AddRatioPlotElement(ratio);
    plot.custom_ratio_y_axis_range = true;
    plot.ratio_y_axis_min = ratio_min_;
    plot.ratio_y_axis_max = ratio_max_;
    ZbbPlot::SetDataStyle(data_plot);
    plot.AddTH1PlotElement(data_plot);
    plot.legend_height = 0.05;

    for (auto & ele : text_) plot.AddTextElement(ele);

    plot.GeneratePlot();
  }

  void ZbbPlot::AddTextElement(ic::TextElement & ele) {
    text_.push_back(ele);
  }




}

