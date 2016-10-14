#ifndef ICHiggsTauTau_HiggsTauTau_HTTPlotTools_h
#define ICHiggsTauTau_HiggsTauTau_HTTPlotTools_h
#include <iostream>
#include <vector>
#include <map>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TextElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTAnalysisTools.h"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/SOverBTools.h"

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

  struct PlotSigComponent {
    std::string name;
    std::string legend;
    std::vector<std::string> plots;
    int color;
    bool in_stack;
    PlotSigComponent(std::string n, std::string l, std::vector<std::string> p, int c, bool s) {
      name = n;
      legend = l;
      plots = p;
      color = c;
      in_stack = s;
    }
  }; 


  class HTTPlot {
    CLASS_MEMBER(HTTPlot,   bool,           use_htt_style)
    CLASS_MEMBER(HTTPlot,   std::string,    plot_name)
    CLASS_MEMBER(HTTPlot,   std::string,    x_axis_label)
    CLASS_MEMBER(HTTPlot,   std::string,    y_axis_label)
    CLASS_MEMBER(HTTPlot,   std::string,    x_axis_bin_labels)
    CLASS_MEMBER(HTTPlot,   bool,           custom_x_axis_range)
    CLASS_MEMBER(HTTPlot,   double,         x_axis_min)
    CLASS_MEMBER(HTTPlot,   double,         x_axis_max)
    CLASS_MEMBER(HTTPlot,   bool,           custom_y_axis_min)
    CLASS_MEMBER(HTTPlot,   double,         y_axis_min)
    CLASS_MEMBER(HTTPlot,   double,         extra_pad)
    CLASS_MEMBER(HTTPlot,   bool,           blind)
    CLASS_MEMBER(HTTPlot,   bool,           autoblind)
    CLASS_MEMBER(HTTPlot,   double,         x_blind_min)
    CLASS_MEMBER(HTTPlot,   double,         x_blind_max)
    CLASS_MEMBER(HTTPlot,   bool,           log_y)
    CLASS_MEMBER(HTTPlot,   bool,           draw_ratio)
    CLASS_MEMBER(HTTPlot,   bool,           norm_bins)
    CLASS_MEMBER(HTTPlot,   std::string,    cms_label)
    CLASS_MEMBER(HTTPlot,   std::string,    lumi_label)
    CLASS_MEMBER(HTTPlot,   std::string,    cms_extra)
    CLASS_MEMBER(HTTPlot,   std::string,    background_scheme)
    CLASS_MEMBER(HTTPlot,   std::string,    signal_scheme)
    CLASS_MEMBER(HTTPlot,   std::string,    draw_signal_mass) // if "" then don't draw
    CLASS_MEMBER(HTTPlot,   std::string,    draw_signal_tanb) // if "" then don't draw
    CLASS_MEMBER(HTTPlot,   unsigned,       signal_scale) // if "" then don't draw
    CLASS_MEMBER(HTTPlot,   double,         auto_error_band)
    CLASS_MEMBER(HTTPlot,   bool,           draw_error_band)
    CLASS_MEMBER(HTTPlot,   bool,           add_stat_error)
    CLASS_MEMBER(HTTPlot,   double,         ratio_min)
    CLASS_MEMBER(HTTPlot,   double,         ratio_max)
    CLASS_MEMBER(HTTPlot,   bool,           supress_output)
    CLASS_MEMBER(HTTPlot,   std::string,       sOverb_output_name)
    CLASS_MEMBER(HTTPlot,   bool,           ams_scan)
    CLASS_MEMBER(HTTPlot,    std::string,    ams_scan_output_name)

    // CLASS_MEMBER(HTTPlot, bool, ztt_by_decay_mode)
    // CLASS_MEMBER(HTTPlot, bool, shift_backgrounds)
    // CLASS_MEMBER(HTTPlot, bool, draw_error_band)

    public:
      HTTPlot();
      boost::program_options::options_description const& GenerateOptions(std::string const& prefix);
      void GeneratePlot(HTTAnalysis::HistValueMap hmap);
      static void SetMCStackStyle(TH1* hist_ptr, unsigned color);
      static void SetSignalStyle(TH1* hist_ptr, unsigned color);
      static void SetDataStyle(TH1* hist_ptr);
      inline std::string plot_name() const { return plot_name_; }
      inline std::string draw_signal_mass() const { return draw_signal_mass_; }
      inline std::string draw_signal_tanb() const { return draw_signal_tanb_; }
      void AddTextElement(ic::TextElement & ele);
      inline bool blind() const { return blind_; }
      inline bool autoblind() const { return autoblind_; }
      inline double x_blind_min() const { return x_blind_min_; }
      inline double x_blind_max() const { return x_blind_max_; }
      inline double extra_pad() const { return extra_pad_; }
      inline std::string x_axis_label() const { return x_axis_label_; }
      inline std::string y_axis_label() const { return y_axis_label_; }
      inline bool supress_output() const { return supress_output_; }
    private:
      boost::program_options::options_description config_;
      std::map<std::string, std::vector<PlotBkgComponent>> bkg_schemes_;
      std::map<std::string, std::vector<PlotSigComponent>> sig_schemes_;
      std::vector<ic::TextElement> text_;
  };

    
    // -- Need mapping from HistValueMap to PlotElements

    // i.e. take signal as ggH125+VH125+qqH125 and draw on top of stack
    // i.e. take W and VV and ZJ --> call this electroweak

    // background schemes:
    // et_standard
    // mt_standard
    // em_standard
    // mt_separate_zll
    // mt_ztt_decay_modes

    // signal schemes
    // split_qqh
    // standard
    // unstacked
  
 
}

#endif

