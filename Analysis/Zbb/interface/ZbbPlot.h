#ifndef ICHiggsTauTau_HiggsTauTau_ZbbPlot_h
#define ICHiggsTauTau_HiggsTauTau_ZbbPlot_h
#include <iostream>
#include <vector>
#include <map>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "Core/interface/Plot.h"
#include "Core/interface/TextElement.h"
#include "Core/interface/ModuleBase.h"
#include "Zbb/interface/ZbbAnalysisTools.h"
#include "boost/program_options.hpp"

namespace ic {

  struct PlotBkgComponent {
    std::string name;
    std::string legend;
    std::vector<std::string> plots;
    int color;
    PlotBkgComponent(std::string n, std::string l, std::vector<std::string> p, int c) {
      name = n;
      legend = l;
      plots = p;
      color = c;
    }
  }; 

  class ZbbPlot {
    CLASS_MEMBER(ZbbPlot,   bool,           use_htt_style)
    CLASS_MEMBER(ZbbPlot,   std::string,    plot_name)
    CLASS_MEMBER(ZbbPlot,   std::string,    x_axis_label)
    CLASS_MEMBER(ZbbPlot,   std::string,    y_axis_label)
    CLASS_MEMBER(ZbbPlot,   std::string,    x_axis_bin_labels)
    CLASS_MEMBER(ZbbPlot,   bool,           custom_x_axis_range)
    CLASS_MEMBER(ZbbPlot,   double,         x_axis_min)
    CLASS_MEMBER(ZbbPlot,   double,         x_axis_max)
    CLASS_MEMBER(ZbbPlot,   bool,           custom_y_axis_min)
    CLASS_MEMBER(ZbbPlot,   double,         y_axis_min)
    CLASS_MEMBER(ZbbPlot,   double,         legend_left)
    CLASS_MEMBER(ZbbPlot,   unsigned,       legend_pos)
    CLASS_MEMBER(ZbbPlot,   double,         extra_pad)
    CLASS_MEMBER(ZbbPlot,   bool,           blind)
    CLASS_MEMBER(ZbbPlot,   double,         x_blind_min)
    CLASS_MEMBER(ZbbPlot,   double,         x_blind_max)
    CLASS_MEMBER(ZbbPlot,   bool,           log_y)
    CLASS_MEMBER(ZbbPlot,   bool,           draw_ratio)
    CLASS_MEMBER(ZbbPlot,   bool,           norm_bins)
    CLASS_MEMBER(ZbbPlot,   std::string,    title_left)
    CLASS_MEMBER(ZbbPlot,   std::string,    title_right)
    CLASS_MEMBER(ZbbPlot,   std::string,    background_scheme)
    CLASS_MEMBER(ZbbPlot,   double,         auto_error_band)
    CLASS_MEMBER(ZbbPlot,   bool,           draw_error_band)
    CLASS_MEMBER(ZbbPlot,   bool,           add_stat_error)
    CLASS_MEMBER(ZbbPlot,   double,         ratio_min)
    CLASS_MEMBER(ZbbPlot,   double,         ratio_max)

    public:
      ZbbPlot();
      boost::program_options::options_description const& GenerateOptions(std::string const& prefix);
      void GeneratePlot(ZbbAnalysis::HistValueMap hmap);
      static void SetMCStackStyle(ic::TH1PlotElement & ele, unsigned color);
      static void SetSignalStyle(ic::TH1PlotElement & ele, unsigned color);
      static void SetDataStyle(ic::TH1PlotElement & ele);
      static void SetRatioStyle(ic::RatioPlotElement & ele, unsigned color);
      inline std::string plot_name() const { return plot_name_; }
      void AddTextElement(ic::TextElement & ele);
      inline bool blind() const { return blind_; }
      inline double x_blind_min() const { return x_blind_min_; }
      inline double x_blind_max() const { return x_blind_max_; }
      inline double extra_pad() const { return extra_pad_; }
      inline std::string x_axis_label() const { return x_axis_label_; }
      inline std::string y_axis_label() const { return y_axis_label_; }
      inline std::string title_left() const { return title_left_; }
      inline std::string title_right() const { return title_right_; }
    private:
      boost::program_options::options_description config_;
      std::map<std::string, std::vector<PlotBkgComponent>> bkg_schemes_;
      std::vector<ic::TextElement> text_;
  };
}

#endif

