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

  void TH1PlotElement::AddOverflows() {

    //modify hist_ptr to add under/overflow bins
    Int_t nx    = hist_ptr_->GetNbinsX()+2;
    Double_t *xbins= new Double_t[nx+1];
    for (Int_t i=1;i<nx;i++)
      xbins[i]=hist_ptr_->GetBinLowEdge(i);
    
    xbins[0]=xbins[1]-hist_ptr_->GetBinWidth(0);
    xbins[nx]=xbins[nx-1]+hist_ptr_->GetBinWidth(nx-1);

    char *tempName= new char[strlen(hist_ptr_->GetName())+10];
    sprintf(tempName,"%swtOverFlow",hist_ptr_->GetName());
    // Book a temporary histogram having ab extra bin for overflows
    TH1F *htmp = new TH1F(tempName, hist_ptr_->GetTitle(), nx, xbins);
    
    // Fill the new hitogram including the extra bin for overflows
    for (Int_t i=1; i<=nx; i++){
      htmp->SetBinContent(i,hist_ptr_->GetBinContent(i-1));
      htmp->SetBinError(i,hist_ptr_->GetBinError(i-1));
    }
    // Restore the number of entries
    htmp->SetEntries(hist_ptr_->GetEntries());

    hist_ptr_ = htmp;
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
    draw_options_       = "";
  }
}
