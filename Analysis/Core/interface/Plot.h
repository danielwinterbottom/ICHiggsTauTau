#ifndef ICHiggsTauTau_Core_Plot_h
#define ICHiggsTauTau_Core_Plot_h

#include <vector>
#include <string>
#include <map>
#include "TFile.h"
#include "TH1F.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TH1PlotElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/RatioPlotElement.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TextElement.h"


namespace ic {

  class Plot {
    
    public:
      Plot(){
        draw_ratio_hist = false;
        custom_x_axis_range = false;
        x_axis_min = -999999;
        x_axis_max = -1;
        y_axis_log = false;
        custom_ratio_y_axis_range = true;
        ratio_y_axis_min = 0;
        ratio_y_axis_max = 2;
        draw_y_gridlines = true;
        legend_height = 0.07;
        n_in_stack = 0;
        custom_y_axis_range = false;
        y_axis_min = -1;
        y_axis_max = -1;
        extra_pad = 1.0;
        draw_signif = false;
        legend_pos = 0;
        append = 0;
        legend_left = -1.;
        samples_for_band_ = "";
        draw_band_on_stack_ = false;
        band_size_fractional_ = 0.0;
        x_bin_labels_ = "";
      }
      ~Plot();
      int GeneratePlot();
      bool AddTH1PlotElement(ic::TH1PlotElement element);
      bool AddRatioPlotElement(ic::RatioPlotElement element);
      void AddTextElement(ic::TextElement element);
      void SetRatioYaxisRange(double min, double max);
      void SetXaxisRange(double min, double max);
      void SetYaxisRange(double min, double max);

      //Customise plot titles
      std::string title_left;
      std::string title_right;
      
      //Customise ratio plot
      bool draw_ratio_hist;
      bool draw_signif;
      bool draw_y_gridlines;
      std::string ratio_y_axis_title;
      bool custom_ratio_y_axis_range;
      double ratio_y_axis_min;
      double ratio_y_axis_max;

      //Customise x-axis
      std::string x_axis_title;
      bool custom_x_axis_range;
      double x_axis_min;
      double x_axis_max;
      bool custom_y_axis_range;
      double y_axis_min;
      double y_axis_max;     
      //Customise y-axis
      std::string y_axis_title;
      bool y_axis_log;
      double extra_pad;
      double legend_left;

      //Customise legend
      double legend_height;
      unsigned legend_pos;

      unsigned append;

      std::string samples_for_band_;
      bool draw_band_on_stack_;
      double band_size_fractional_;

      std::string x_bin_labels_;

      std::string output_filename;
      static void SetTdrStyle();

      
    private:
      std::vector<ic::TH1PlotElement> elements_;
      std::vector<ic::RatioPlotElement> ratios_;
      std::vector<ic::TextElement> texts_;
      std::map<std::string,ic::TH1PlotElement> element_map_;
      std::vector<std::string> output_formats_;
      unsigned n_in_stack;


  };


}

#endif
