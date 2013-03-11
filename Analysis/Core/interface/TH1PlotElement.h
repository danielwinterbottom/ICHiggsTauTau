#ifndef AGilbert_Analysis_TH1PlotElement_h
#define AGilbert_Analysis_TH1PlotElement_h

#include <string>
#include "TFile.h"
#include "TH1F.h"

namespace ic {

  class TH1PlotElement {
    private:
      std::string name_;
      TH1F* hist_ptr_;
      std::string legend_text_;
      bool in_stack_;
      //Fill Settings
      bool draw_fill_;
      bool draw_fill_in_legend_;
      int fill_color_;
      int fill_style_;
      //Line settings
      bool draw_line_;
      int line_color_;
      int line_style_;
      int line_width_;
      //Marker settings
      bool draw_marker_;
      int marker_color_;
      int marker_style_;
      double marker_size_;
      //Error bar settings
      bool draw_stat_error_y_;
      bool draw_bin_width_x_;
      //Scaling & rebin settings
      double scale_factor_;
      bool draw_normalised_;
      unsigned rebin_factor_;
      bool smooth_curve_;
      std::string draw_options_;

    public:
      TH1PlotElement(std::string name = "no_name");

      TH1PlotElement(std::string name, TFile* source_file,
          std::string hist_path, std::string hist_name,
          std::string legend_text = "");

      TH1PlotElement(std::string name, TH1F* hist_ptr,
          std::string legend_text = "");

      void SetElement(TFile* source_file, std::string hist_path,
          std::string hist_name);

      void SetElement(TH1F* hist_ptr);

      void SetDefaults();
      
      //Getters
      std::string name() const { return name_; }
      TH1F* hist_ptr() const { return hist_ptr_; }
      std::string legend_text() const { return legend_text_; }
      bool in_stack() const { return in_stack_; }
      bool draw_fill() const { return draw_fill_; }
      bool draw_fill_in_legend() const { return draw_fill_in_legend_; }

      int fill_color() const { return fill_color_;}
      int fill_style() const { return fill_style_;}
      bool draw_line() const { return draw_line_; }
      int line_color() const { return line_color_; }
      int line_style() const { return line_style_; }
      int line_width() const { return line_width_; }
      bool draw_marker() const { return draw_marker_; }
      int marker_color() const { return marker_color_; }
      int marker_style() const { return marker_style_; }
      double marker_size() const { return marker_size_; }
      bool draw_stat_error_y() const { return draw_stat_error_y_; }
      bool draw_bin_width_x() const { return draw_bin_width_x_; }
      double scale_factor() const { return scale_factor_; }
      bool draw_normalised() const { return draw_normalised_; }
      bool smooth_curve() const { return smooth_curve_; }
      std::string draw_options() const { return draw_options_; }

      unsigned rebin_factor() const { return rebin_factor_; }

      //Setters
      void set_name(std::string name) { name_ = name; }
      void set_hist_ptr(TH1F* hist_ptr) { hist_ptr_ = hist_ptr; }
      void set_legend_text(std::string legend_text) { 
        legend_text_ = legend_text; 
      }
      void set_in_stack(bool in_stack) { in_stack_ = in_stack; }
      void set_draw_fill(bool draw_fill) { draw_fill_ = draw_fill; }
      void set_draw_fill_in_legend(bool draw_fill_in_legend) { draw_fill_in_legend_ = draw_fill_in_legend; }
      void set_fill_color(int fill_color) { fill_color_ = fill_color; }
      void set_fill_style(int fill_style) { fill_style_ = fill_style; }
      void set_draw_line(bool draw_line) { draw_line_ = draw_line; }
      void set_smooth_curve(bool smooth_curve) { smooth_curve_ = smooth_curve; }
      void set_line_color(int line_color) { line_color_ = line_color; }
      void set_line_style(int line_style) { line_style_ = line_style; }
      void set_line_width(int line_width) { line_width_ = line_width; }
      void set_draw_marker(bool draw_marker) { draw_marker_ = draw_marker; }
      void set_marker_color(int marker_color) { marker_color_ = marker_color; }
      void set_marker_style(int marker_style) { marker_style_ = marker_style; }
      void set_marker_size(double marker_size) { marker_size_ = marker_size; }
      void set_draw_stat_error_y(bool draw_stat_error_y) { 
        draw_stat_error_y_ =  draw_stat_error_y; 
      }
      void set_draw_bin_width_x(bool draw_bin_width_x) { 
        draw_bin_width_x_ = draw_bin_width_x;
      }
      void set_scale_factor(double scale_factor) { 
        scale_factor_ = scale_factor;
      }
      void set_draw_normalised(bool draw_normalised) { 
        draw_normalised_ = draw_normalised;
      }
      void set_rebin_factor(unsigned rebin_factor) { 
        rebin_factor_ = rebin_factor;
      }
      void set_draw_options(std::string draw_options) { 
        draw_options_ = draw_options; 
      }
  };
  
}

#endif







