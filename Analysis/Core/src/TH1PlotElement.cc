#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TH1PlotElement.h"

namespace ic {

  //Constructors
  TH1PlotElement::TH1PlotElement(std::string name) {
    name_ = name;
    SetDefaults();
  }

  TH1PlotElement::TH1PlotElement(std::string name, TFile* source_file,
                                  std::string hist_path, std::string hist_name,
                                  std::string legend_text) {
    name_ = name;
    SetDefaults();
    SetElement(source_file, hist_path, hist_name);
    legend_text_ = legend_text;
  }

  TH1PlotElement::TH1PlotElement(std::string name, TH1F* hist_ptr,
                                  std::string legend_text) {
    name_ = name;
    SetDefaults();
    SetElement(hist_ptr);
    legend_text_ = legend_text;
  }
  
  void TH1PlotElement::SetElement(TFile* source_file, std::string hist_path,
                                  std::string hist_name) {
    source_file->cd();
    gDirectory->cd(hist_path.c_str());
    hist_ptr_ = dynamic_cast<TH1F*>(gDirectory->Get(hist_name.c_str()));
    if (!hist_ptr_) hist_ptr_ = (TH1F*)(gDirectory->Get(hist_name.c_str()));

  }

  void TH1PlotElement::SetElement(TH1F* hist_ptr) {
    hist_ptr_ = hist_ptr;
  }

  void TH1PlotElement::SetDefaults() {
    hist_ptr_           = 0;
    legend_text_        = "";
    in_stack_           = false;
    draw_fill_          = false;
    draw_fill_in_legend_  = true;
    fill_color_         = 0;
    fill_style_         = 0;
    draw_line_          = true;
    line_color_         = 1;
    line_width_         = 1;
    line_style_         = 1;
    draw_marker_        = false;
    marker_color_       = 1;
    marker_style_       = 21;
    marker_size_        = 0.8;
    draw_stat_error_y_  = true;
    draw_bin_width_x_   = 0;
    scale_factor_       = 1.0;
    draw_normalised_    = false;
    rebin_factor_       = 1;
    smooth_curve_       = false;
  }
}
