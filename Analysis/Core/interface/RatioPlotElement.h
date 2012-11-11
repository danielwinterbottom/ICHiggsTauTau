#ifndef ICHiggsTauTau_Core_RatioPlotElement_h
#define ICHiggsTauTau_Core_RatioPlotElement_h

#include <string>
#include "TFile.h"
#include "TH1F.h"

namespace ic {

  class RatioPlotElement {
    private:
      std::string name_;
      std::string hist_numerator_;
      std::string hist_denominator_;
      //Fill Settings
      bool draw_fill_;
      int fill_color_;
      int fill_style_;
      //Line Settings
      bool draw_line_;
      int line_color_;
      int line_style_;
      int line_width_;
      //Marker Settings
      bool draw_marker_;
      int marker_color_;
      int marker_style_;
      double marker_size_;
      //Error bar settings
      bool draw_stat_error_y_;
      bool draw_bin_width_x_;
      bool multi_mode_;

    public:
      RatioPlotElement(std::string name = "no_name");

      RatioPlotElement(std::string name, std::string num, std::string den);

      void SetDefaults();

      //Getters
      std::string name() const { return name_; }
      std::string hist_numerator() const { return hist_numerator_; }
      std::string hist_denominator() const { return hist_denominator_; }
      bool draw_fill() const { return draw_fill_; }
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
      bool multi_mode() const { return multi_mode_; }

      //Setters
      void set_name(std::string name) { name_ = name; }
      void set_hist_numerator(std::string num) { hist_numerator_ = num; }
      void set_hist_denominator(std::string den) { hist_denominator_ = den; }
      void set_draw_fill(bool draw_fill) { draw_fill_ = draw_fill; }
      void set_fill_color(int fill_color) { fill_color_ = fill_color; }
      void set_fill_style(int fill_style) { fill_style_ = fill_style; }
      void set_draw_line(bool draw_line) { draw_line_ = draw_line; }
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
      void set_multi_mode(bool multi_mode) { multi_mode_ = multi_mode; }


  };
}
#endif
