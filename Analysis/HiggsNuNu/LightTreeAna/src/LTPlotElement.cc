#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LTPlotElement.h"

namespace ic{

  LTPlotElement::LTPlotElement(){
    unit_="GeV";
    in_stack_=false;
    is_inrationum_=false;
    is_inratioden_=false;
    has_dderrors_=0;
  };

  LTPlotElement::~LTPlotElement(){ ;};

  void LTPlotElement::ApplyStyle(){
    hist_ptr_->SetName(legname_.c_str());
    hist_ptr_->Scale(scale_);
    if(draw_marker_){
      hist_ptr_->SetMarkerColor(marker_color_);
      hist_ptr_->SetMarkerStyle(marker_style_);
      hist_ptr_->SetMarkerSize(marker_size_);
      drawopts_+="P";
      legopts_+="p";
    }
    if(draw_fill_){
      hist_ptr_->SetFillColor(fill_color_);
      hist_ptr_->SetFillStyle(fill_style_);
      hist_ptr_->SetLineColor(line_color_);
      drawopts_+="hist";
      legopts_+="f";
    }
    if(draw_line_){
      hist_ptr_->SetLineColor(line_color_);
      hist_ptr_->SetLineStyle(line_style_);
      hist_ptr_->SetLineWidth(line_width_);
      legopts_+="l";
    }
    if(draw_stat_error_y_==true){
      drawopts_+="E1";
      legopts_+="e";
    }
  };

}
