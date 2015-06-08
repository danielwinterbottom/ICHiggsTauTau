#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/HistPlotter.h"
#include <iostream>
#include "TH1F.h"
#include "TF1.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include <map>
#include "TVectorD.h"
//#include "CommonTools/Utils/interface/TFileDirectory.h"
#include <boost/algorithm/string.hpp>
#include "TDirectory.h"
#include "TFile.h"
#include "TPad.h"
#include "TLatex.h"
#include "TLine.h"
#include "TBox.h"
#include "TASImage.h"
#include "Math/QuantFuncMathCore.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"

namespace ic{

  void DrawCMSLogo(TPad* pad, TString cmsText, TString extraText, int iPosX,
		   float relPosX, float relPosY, float relExtraDY) {
    TVirtualPad *pad_backup = gPad;
    pad->cd();
    float cmsTextFont = 61;  // default is helvetic-bold

    bool writeExtraText = extraText.Length() > 0;
    float extraTextFont = 52;  // default is helvetica-italics

    // text sizes and text offsets with respect to the top frame
    // in unit of the top margin size
    TString lumiText;
    float lumiTextOffset = 0.2;
    float cmsTextSize = 0.8;
    float lumiTextSize = 0.6;
    // float cmsTextOffset    = 0.1;  // only used in outOfFrame version

    // ratio of "CMS" and extra text size
    float extraOverCmsTextSize = 0.76;

    //!!MAKE CHOICE CONFIGURABLE
    TString lumi_13TeV = "20.1 fb^{-1}";
    TString lumi_8TeV = "19.2 fb^{-1}";
    TString lumi_7TeV = "5.1 fb^{-1}";

    lumiText +=lumi_8TeV;
    lumiText +=" (8 TeV)";


    bool outOfFrame = false;
    if (iPosX / 10 == 0) {
      outOfFrame = true;
    }
    int alignY_ = 3;
    int alignX_ = 2;
    if (iPosX / 10 == 0) alignX_ = 1;
    if (iPosX == 0) alignX_ = 1;
    if (iPosX == 0) alignY_ = 1;
    if (iPosX / 10 == 1) alignX_ = 1;
    if (iPosX / 10 == 2) alignX_ = 2;
    if (iPosX / 10 == 3) alignX_ = 3;
    if (iPosX == 0) relPosX = 0.14;
    int align_ = 10 * alignX_ + alignY_;

    float l = pad->GetLeftMargin();
    float t = pad->GetTopMargin();
    float r = pad->GetRightMargin();
    float b = pad->GetBottomMargin();

    TLatex latex;
    latex.SetNDC();
    latex.SetTextAngle(0);
    latex.SetTextColor(kBlack);

    float extraTextSize = extraOverCmsTextSize * cmsTextSize;
    float pad_ratio = (static_cast<float>(pad->GetWh()) * pad->GetAbsHNDC()) /
      (static_cast<float>(pad->GetWw()) * pad->GetAbsWNDC());
    if (pad_ratio < 1.) pad_ratio = 1.;

    latex.SetTextFont(42);
    latex.SetTextAlign(31); 
    latex.SetTextSize(lumiTextSize*t*pad_ratio);    
    latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);

    if (outOfFrame) {
      latex.SetTextFont(cmsTextFont);
      latex.SetTextAlign(11);
      latex.SetTextSize(cmsTextSize * t * pad_ratio);
      latex.DrawLatex(l, 1 - t + lumiTextOffset * t, cmsText);
    }


    float posX_ = 0;
    if (iPosX % 10 <= 1) {
      posX_ = l + relPosX * (1 - l - r);
    } else if (iPosX % 10 == 2) {
      posX_ = l + 0.5 * (1 - l - r);
    } else if (iPosX % 10 == 3) {
      posX_ = 1 - r - relPosX * (1 - l - r);
    }
    float posY_ = 1 - t - relPosY * (1 - t - b);
    if (!outOfFrame) {
      latex.SetTextFont(cmsTextFont);
      latex.SetTextSize(cmsTextSize * t * pad_ratio);
      latex.SetTextAlign(align_);
      latex.DrawLatex(posX_, posY_, cmsText);
      if (writeExtraText) {
	latex.SetTextFont(extraTextFont);
	latex.SetTextAlign(align_);
	latex.SetTextSize(extraTextSize * t * pad_ratio);
	latex.DrawLatex(posX_, posY_ - relExtraDY * cmsTextSize * t, extraText);
      }
    } else if (writeExtraText) {
      if (iPosX == 0) {
	posX_ = l + relPosX * (1 - l - r);
	posY_ = 1 - t + lumiTextOffset * t;
      }
      latex.SetTextFont(extraTextFont);
      latex.SetTextSize(extraTextSize * t * pad_ratio);
      latex.SetTextAlign(align_);
      latex.DrawLatex(posX_, posY_, extraText);
    }
    pad_backup->cd();
  }

  void DrawCMSLogo(TPad* pad, TString cmsText, TString extraText, int iPosX) {
    DrawCMSLogo(pad, cmsText, extraText, iPosX, 0.045, 0.035, 1.2);
  }

  LTPlotElement::LTPlotElement(){
    unit_="GeV";
    in_stack_=false;
    is_inrationum_=false;
    is_inratioden_=false;
    has_dderrors_=0;
  };

  LTPlotElement::~LTPlotElement(){ ;};

  LTShapeElement::LTShapeElement(){
    dology_=false;
    axisrangemultiplier_=1.2;
    legleft_=0.67;
    legright_=0.89;
  };

  LTShapeElement::~LTShapeElement(){ ;};

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

  void HistPlotter::SetMCStackStyle(ic::LTPlotElement* ele) {
    ele->set_fill_color(ele->color());
    ele->set_fill_style(1001);
    ele->set_draw_fill(true);
    ele->set_draw_marker(false);
    ele->set_draw_line(false);
    ele->set_line_width(2);
    ele->set_draw_stat_error_y(false);
    ele->set_line_color(1);
    return;
  }
  void HistPlotter::SetSignalStyle(ic::LTPlotElement* ele) {
    ele->set_fill_style(1001);
    ele->set_draw_fill(true);
    ele->set_draw_marker(false);
    ele->set_draw_line(true);
    ele->set_draw_stat_error_y(false);
    ele->set_draw_fill_in_legend(false);
    ele->set_line_style(1);
    ele->set_fill_color(0);
    ele->set_line_color(ele->color());
    ele->set_line_width(3);
    return;
  }
  void HistPlotter::SetDataStyle(ic::LTPlotElement* ele) {
    ele->set_marker_color(1);
    ele->set_line_color(1);
    ele->set_fill_color(1);
    ele->set_fill_style(0);
    ele->set_draw_fill(false);
    ele->set_line_width(2);
    ele->set_draw_marker(true);
    ele->set_draw_line(true);
    ele->set_marker_style(20);
    ele->set_draw_stat_error_y(true);
    ele->set_marker_size(1.1);
    return;
  }

  HistPlotter::HistPlotter(std::string name) : LTModule(name){
    do_norm_=false;
    do_ratio_=false;
    do_ratio_line_=false;
    do_ratio_fitline_=false;
    add_underflows_=false;
    add_overflows_=false;
    outsuffix_="";
  };

  HistPlotter::~HistPlotter(){ ;};

  int HistPlotter::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;

    return 0;
  };

  int HistPlotter::Run(LTFiles* ){
    std::cout<<module_name_<<":"<<std::endl;
    TFile* file=fs_;
    gStyle->SetOptStat(0);
    //GET DIRECTORY TO WRITE TO
    TDirectory* stacksdir;
    TDirectory* numsdir;
    TDirectory* densdir;
    if(!fs_->GetDirectory("stacksdir")){
      stacksdir=file->mkdir("stacksdir");
      numsdir=file->mkdir("numsdir");
      densdir=file->mkdir("densdir");
    }
    else{
      int idir=2;
      while(fs_->GetDirectory(("stacksdir"+idir))){
	idir++;
      }
      stacksdir=file->mkdir(("stacksdir"+idir));
      numsdir=file->mkdir(("numsdir"+idir));
      densdir=file->mkdir(("densdir"+idir));
    }

    TDirectory* writedir;
    if(dirname_==""){
      writedir=file->mkdir("controlplots");
    }
    else if(!fs_->GetDirectory(dirname_.c_str())){
      writedir=file->mkdir(dirname_.c_str());
    }
    else{
      writedir=file->GetDirectory(dirname_.c_str());
    }
    writedir->cd();

    //SETUP STYLE
    for(unsigned iElement=0;iElement<elements_.size();iElement++){
    }

//     if(histTitles_.size()!=shapes_.size()){
//       std::cout<<"histTitles must be of the same size as shapes. Exiting with status 1"<<std::endl;
//       return 1;
//     }

    //LOOP OVER ALL THE VARIABLES TO PLOT
    for(unsigned iShape=0;iShape<shapes_.size();iShape++){
      std::cout<<"  Drawing plot for "<<shapes_[iShape].name()<<std::endl;
      THStack *stack=new THStack(("stack"+shapes_[iShape].name()).c_str(),("stacked "+shapes_[iShape].name()+" plots").c_str());
      bool stackempty=true;

      //      if(!do_ratio_) stack->SetTitle(histTitles_[iShape].name().c_str());
      if(!do_ratio_) stack->SetTitle(shapes_[iShape].histtitle().c_str());
      //stack->GetXaxis()->SetTitle(shapes_[iShape].c_str());

      //EXTRACT ALL THE HISTOS AND PUT THEM IN STACKED OR UNSTACKED GROUPS
      std::cout<<"    Getting histograms.."<<std::endl;
      for(unsigned iElement=0;iElement<elements_.size();iElement++){
	if(elements_[iElement].sample()==""){
	  std::cout<<"ERROR: Element with empty name exiting with status 1"<<std::endl;
	  return 1;
	}
	//std::cout<<"      "<<elements_[iElement].sample()<<std::endl;
	if(!fs_->GetDirectory(elements_[iElement].sample().c_str())){
	  std::cout<<"ERROR: No directory with name: "<<elements_[iElement].sample()<<std::endl;
	  std::cout<<"Exiting with status 1"<<std::endl;
	  return 1;
	}
	writedir->cd();
	TH1F* histo =dynamic_cast<TH1F*>(file->Get((elements_[iElement].sample()+"/"+shapes_[iShape].name()).c_str()));
	writedir->cd();

	for(unsigned iblindshape=0;iblindshape<elements_[iElement].blindvar().size();iblindshape++){
	  if(shapes_[iShape].name()==elements_[iElement].blindvar()[iblindshape]){
	    for (int j = 0; j < histo->GetNbinsX(); ++j) {
	      double low_edge = histo->GetBinLowEdge(j+1);
	      double high_edge = histo->GetBinWidth(j+1)+histo->GetBinLowEdge(j+1);
	      double x_blind_min_=elements_[iElement].blindrange()[iblindshape].first;
	      double x_blind_max_=elements_[iElement].blindrange()[iblindshape].second;
	      if ((low_edge > x_blind_min_ && low_edge < x_blind_max_) || (high_edge > x_blind_min_ && high_edge < x_blind_max_)) {
		histo->SetBinContent(j+1,0);
		histo->SetBinError(j+1,0);
	      }
	    }
	  }
	}
	if(do_norm_){
	  if(Integral(histo)!=0)histo->Scale(1/(Integral(histo)));
	}

	if (add_underflows_ || add_overflows_){
	  int tmpbins = histo->GetNbinsX();
	  double tmpmin = histo->GetXaxis()->GetBinLowEdge(1);
	  double tmpmax = histo->GetXaxis()->GetBinLowEdge(tmpbins+1);
	  double tmpsize = (tmpmax-tmpmin)/tmpbins;
	  if (add_underflows_) {
	    tmpbins+=1;
	    tmpmin = tmpmin-tmpsize;
	  }
	  if (add_overflows_&&shapes_[iShape].name()!="alljetsmetnomu_mindphi"){
	    tmpbins+=1;
	    tmpmax = tmpmax+tmpsize;
	  }
	  std::ostringstream tmpname;
	  tmpname << histo->GetName() << "_mod";
	  TH1F *histTmp = new TH1F(tmpname.str().c_str(),histo->GetTitle(),tmpbins,tmpmin,tmpmax);
	  histTmp->Sumw2();
	  if (add_underflows_) {
	    histTmp->SetBinContent(1,histo->GetBinContent(0));
	    histTmp->SetBinError(1,histo->GetBinError(0));
	    for (int ibin(1); ibin<histo->GetNbinsX()+1;++ibin){
	      histTmp->SetBinContent(ibin+1,histo->GetBinContent(ibin));
	      histTmp->SetBinError(ibin+1,histo->GetBinError(ibin));
	    }
	  }
	  else {
	    for (int ibin(1); ibin<histo->GetNbinsX()+1;++ibin){
	      histTmp->SetBinContent(ibin,histo->GetBinContent(ibin));
	      histTmp->SetBinError(ibin,histo->GetBinError(ibin));
	    }
	  }
	  if (add_overflows_&&shapes_[iShape].name()!="alljetsmetnomu_mindphi") {
	    histTmp->SetBinContent(tmpbins,histo->GetBinContent(histo->GetNbinsX()+1));
	    histTmp->SetBinError(tmpbins,histo->GetBinError(histo->GetNbinsX()+1));
	  }

	  elements_[iElement].set_hist_ptr(histTmp);
	}
	else 
	  elements_[iElement].set_hist_ptr(histo);
	
	//SETUP STYLE
	if(elements_[iElement].is_data()){
	  SetDataStyle(&(elements_[iElement]));
	}
	else if(elements_[iElement].in_stack()){
	  SetMCStackStyle(&(elements_[iElement]));
	  stackempty=false;
	}
	else{
	  SetSignalStyle(&(elements_[iElement]));
	}
	elements_[iElement].ApplyStyle();

	//ADD STACKED HISTOS TO STACK
	if((!elements_[iElement].is_data())&&elements_[iElement].in_stack()){
	  stack->Add(elements_[iElement].hist_ptr());
	}
      }
      stacksdir->cd();
      stack->Write();
    
      //SETUP THE CANVAS
      TCanvas *c1=new TCanvas(shapes_[iShape].name().c_str(),shapes_[iShape].name().c_str());
      c1->cd();
	TPad* upper = nullptr;
	TPad* lower = nullptr;
      if(do_ratio_){
	  upper = new TPad("upper","pad",0, 0.3 ,1 ,1);
	  lower = new TPad("lower","pad",0, 0   ,1 ,0.3);
	  upper->SetBottomMargin(0.03);
	  upper->Draw();
	  upper->cd();
	  upper->SetLogy(shapes_[iShape].dology());
      }
      else{
	upper = new TPad("upper","pad",0, 0. ,1 ,1);
	upper->SetBottomMargin(0.15);
	upper->Draw();
	upper->cd();
	upper->SetLogy(shapes_[iShape].dology());
      }
      bool first=true;
      double ymax=0;
      if(!stackempty) ymax=stack->GetMaximum();
      for(unsigned iElement=0;iElement<elements_.size();iElement++){
	if(elements_[iElement].hist_ptr()->GetMaximum()>ymax) ymax=elements_[iElement].hist_ptr()->GetMaximum();
      }
      
      //DRAW THE STACK AND ALL THE UNSTACKED HISTOS INCLUDING THE DATA
      if(!stackempty){ 
	std::cout<<"    Drawing Stack.."<<std::endl;
	if(first){
	  //std::cout<<"ymax: "<<ymax<<std::endl;//!!
	  if(ymax>=1){
	    stack->SetMaximum(shapes_[iShape].axisrangemultiplier()*(ymax+sqrt(ymax)));
	  }
	  else{
	    stack->SetMaximum(shapes_[iShape].axisrangemultiplier()*(ymax));
	  }
	  //stack->GetXaxis()->SetTitle("");
	  upper->cd();
	  stack->Draw("hist");
	  upper->Update();
	  c1->Update();
	  first=false;
	  stack->GetYaxis()->SetLabelSize(0.06);
 	  stack->GetYaxis()->SetTitleFont(62);
 	  stack->GetYaxis()->SetTitleSize(0.095);
 	  stack->GetYaxis()->SetTitleOffset(0.450);
	  if(do_ratio_){
	    stack->GetXaxis()->SetLabelOffset(999);
	    stack->GetXaxis()->SetLabelSize(0);
	  }
	  else{
 	    stack->GetXaxis()->SetLabelSize(0.06);
 	    stack->GetXaxis()->SetTitleFont(62);
 	    stack->GetXaxis()->SetTitleSize(0.095);
 	    stack->GetXaxis()->SetTitleOffset(0.7);
	    
	    std::string xtitle;
	    xtitle=shapes_[iShape].histtitle().substr(shapes_[iShape].histtitle().find(";")+1);
	    xtitle=xtitle.substr(0,xtitle.find(";"));
	    stack->GetXaxis()->SetTitle(xtitle.c_str());
	  }
	  std::string ytitle;
	  //	    ytitle=histTitles_[iShape].substr(histTitles_[iShape].find(";")+1);
	  ytitle=shapes_[iShape].histtitle().substr(shapes_[iShape].histtitle().find(";")+1);
	  ytitle=ytitle.substr(ytitle.find(";")+1);
	  ytitle=ytitle.substr(0,ytitle.find(";"));
	  if(do_ratio_) stack->SetTitle((";;"+ytitle).c_str());
	  else{
	    std::string xtitle;
            xtitle=shapes_[iShape].histtitle().substr(shapes_[iShape].histtitle().find(";")+1);
            xtitle=xtitle.substr(0,xtitle.find(";"));
            stack->GetXaxis()->SetTitle(xtitle.c_str());
	    stack->SetTitle((";"+xtitle+";"+ytitle).c_str());
	  }
	  c1->Update();
	}
	else stack->Draw("histsame");
      }
      std::cout<<"    Drawing Unstacked.."<<std::endl;
      for(unsigned iElement=0;iElement<elements_.size();iElement++){
	if(!(elements_[iElement].in_stack())){
	  if(!elements_[iElement].is_data()){
	    if(first){
	      if(!do_ratio_) elements_[iElement].hist_ptr()->SetTitle(shapes_[iShape].histtitle().c_str());
	      elements_[iElement].hist_ptr()->Draw(elements_[iElement].drawopts().c_str());
	      std::cout<<"scaling by: "<<shapes_[iShape].axisrangemultiplier()<<std::endl;//!!

	      if(ymax>=1){
		elements_[iElement].hist_ptr()->GetYaxis()->SetRangeUser(0,shapes_[iShape].axisrangemultiplier()*(ymax+sqrt(ymax)));
	      }
	      else{
		elements_[iElement].hist_ptr()->GetYaxis()->SetRangeUser(0,shapes_[iShape].axisrangemultiplier()*(ymax));
	      }

	      elements_[iElement].hist_ptr()->Draw(elements_[iElement].drawopts().c_str());
	      c1->Update();
	      first=false;
	      elements_[iElement].hist_ptr()->GetYaxis()->SetLabelSize(0.06);
	      elements_[iElement].hist_ptr()->GetYaxis()->SetTitleSize(0.095);
	      elements_[iElement].hist_ptr()->GetYaxis()->SetTitleFont(62);
	      if(do_ratio_){
		elements_[iElement].hist_ptr()->GetXaxis()->SetLabelOffset(999);
		elements_[iElement].hist_ptr()->GetXaxis()->SetLabelSize(0);
	      }
	      else{
		elements_[iElement].hist_ptr()->GetXaxis()->SetLabelSize(0.06);
		elements_[iElement].hist_ptr()->GetXaxis()->SetTitleFont(62);
		elements_[iElement].hist_ptr()->GetXaxis()->SetTitleSize(0.08);
		elements_[iElement].hist_ptr()->GetXaxis()->SetTitleOffset(0.600);
		std::string xtitle;
		xtitle=shapes_[iShape].histtitle().substr(shapes_[iShape].histtitle().find(";")+1);
		xtitle=xtitle.substr(0,xtitle.find(";"));
		elements_[iElement].hist_ptr()->GetXaxis()->SetTitle(xtitle.c_str());
	      }
	      std::string ytitle;
	      ytitle=shapes_[iShape].histtitle().substr(shapes_[iShape].histtitle().find(";")+1);
	      ytitle=ytitle.substr(ytitle.find(";")+1);
	      ytitle=ytitle.substr(0,ytitle.find(";"));
	      elements_[iElement].hist_ptr()->GetYaxis()->SetTitle(ytitle.c_str());
	      elements_[iElement].hist_ptr()->SetTitle("");
	    }
	    else{
	      elements_[iElement].hist_ptr()->Draw(("same"+elements_[iElement].drawopts()).c_str());
	    }
	  }
	  else{
	    //Do asymmetric data errors
	    const double alpha = 1 - 0.6827;
	    TGraphAsymmErrors * g = new TGraphAsymmErrors(elements_[iElement].hist_ptr());
	    g->SetMarkerSize(1.1);
	    g->SetMarkerStyle (20);

	    for (int i = 0; i < g->GetN(); ++i) {
	      int N = g->GetY()[i];
	      double L =  (N==0) ? 0  : (ROOT::Math::gamma_quantile(alpha/2,N,1.));
	      double U =  ROOT::Math::gamma_quantile_c(alpha/2,N+1,1) ;
	      g->SetPointEYlow(i, N-L);
	      g->SetPointEYhigh(i, U-N);
	    }
	    if(first){
	      if(!do_ratio_) g->SetTitle(shapes_[iShape].histtitle().c_str());
	      g->Draw("AP");
	      g->Draw(elements_[iElement].drawopts().c_str());
	      std::cout<<"scaling by: "<<shapes_[iShape].axisrangemultiplier()<<std::endl;//!!
	      g->GetYaxis()->SetRangeUser(0.,shapes_[iShape].axisrangemultiplier()*(ymax+sqrt(ymax)+1));
	      g->Draw(elements_[iElement].drawopts().c_str());
	      c1->Update();
	      first=false;
	      g->GetYaxis()->SetLabelSize(0.06);
	      g->GetYaxis()->SetTitleSize(0.095);
	      g->GetYaxis()->SetTitleFont(62);
	      if(do_ratio_){
		g->GetXaxis()->SetLabelOffset(999);
		g->GetXaxis()->SetLabelSize(0);
	      }
	      else{
		g->GetXaxis()->SetLabelSize(0.06);
                g->GetXaxis()->SetTitleFont(62);
                g->GetXaxis()->SetTitleSize(0.09);
                g->GetXaxis()->SetTitleOffset(0.600);
		std::string xtitle;
		xtitle=shapes_[iShape].histtitle().substr(shapes_[iShape].histtitle().find(";")+1);
		xtitle=xtitle.substr(0,xtitle.find(";"));
		g->GetXaxis()->SetTitle(xtitle.c_str());
	      }
	      std::string ytitle;
	      ytitle=shapes_[iShape].histtitle().substr(shapes_[iShape].histtitle().find(";")+1);
	      ytitle=ytitle.substr(ytitle.find(";")+1);
	      ytitle=ytitle.substr(0,ytitle.find(";"));
	      g->SetTitle(ytitle.c_str());
	    }
	    else g->Draw("sameP");

	  }
	}
      }

      //SETUP AND DRAW THE LEGEND
      double legleft=shapes_[iShape].legleft();
      double legright=shapes_[iShape].legright();
      double legbottom=0.3+((10-elements_.size())*(0.89-0.3)/10);
      if(legbottom<0.3||legbottom>0.89)legbottom=0.3;
      TLegend* leg =new TLegend(legleft,legbottom,legright,0.89);
      leg->SetName("thelegend");
      leg->SetTextSize(0.06);
      leg->SetFillStyle(0);
      leg->SetLineColor(10);
      for(unsigned iElement=0;iElement<elements_.size();iElement++){
	leg->AddEntry(elements_[iElement].hist_ptr(),elements_[iElement].hist_ptr()->GetName(),elements_[iElement].legopts().c_str());
      }
      leg->Draw("same");
      c1->Update();
      
      DrawCMSLogo(upper,"CMS","preliminary",10);

//       TLatex* lat=new TLatex();
//       lat->SetNDC();
// 	//lat->SetTextSize(0.06);
//       lat->SetTextFont(42);

//       TLatex* lat2 = new TLatex();
//       lat2->SetNDC();
//       lat2->SetTextSize(0.04);
//       lat2->SetTextFont(42);

//       lat->DrawLatex(0.14,0.85,"CMS Preliminary");
//       lat->DrawLatex(0.14,0.78,"VBF H #rightarrow invisible");


//       lat2->DrawLatex(0.14,0.665,"#sqrt{s} = 8 TeV, L = 19.2 fb^{-1}");

      c1->Update();

      //DRAW RATIO PLOT
      if(do_ratio_){

	c1->cd();
	lower->SetTopMargin(0.08);
	lower->SetBottomMargin(0.38);
	lower->Draw();
	lower->cd();
	lower->SetGridy();

		
	bool firstnum=true;
	bool firstden=true;
	TH1F* num = 0;
	TH1F* den = 0;
	double dentoterr=0;

	
	for(unsigned iElement=0;iElement<elements_.size();iElement++){
	std::cout<<elements_[iElement].hist_ptr()->GetName()<<std::endl;


	  if(elements_[iElement].is_inrationum()){
	    //ADD TO num HIST
	    if(firstnum){
	      num=(TH1F*)(elements_[iElement].hist_ptr()->Clone(("num"+shapes_[iShape].name()).c_str()));
	      firstnum=false;
	    }
	    else num->Add(elements_[iElement].hist_ptr());

	  }
	  if(elements_[iElement].is_inratioden()){
	    //ADD TO den HIST
	    if(firstden){
	      den=(TH1F*)(elements_[iElement].hist_ptr()->Clone(("den"+shapes_[iShape].name()).c_str()));
	      firstden=false;
	    }
	    else den->Add(elements_[iElement].hist_ptr());
	    //get error on this contribution
	    double thiselementfracerr;
	    double thiselementintegral=Integral(elements_[iElement].hist_ptr());
	    if(elements_[iElement].has_dderrors()==1){
	      TVectorD* dennormerr =(TVectorD*)file->Get((elements_[iElement].sample()+"/normerrs").c_str());//!!
	      thiselementfracerr=sqrt(pow((*dennormerr)[0],2)+pow((*dennormerr)[1],2));
	    }
	    else{

	      thiselementfracerr=Error(elements_[iElement].hist_ptr())/thiselementintegral;
	    }
	    if(thiselementintegral!=0){

	      dentoterr=sqrt(pow(dentoterr,2)+pow(thiselementfracerr*thiselementintegral,2));
	    }
	    //std::cout<<dentoterr<<std::endl


	  }
	}
	numsdir->cd();
	num->Write();
	densdir->cd();
	den->Write();
	lower->cd();

	if(firstnum||firstden)std::cout<<"To draw ratio plot you must specify elements to be numerator and denominator! Ratio plot will be missing."<<std::endl;
	else{
	  //Set den error to zero will take den error into account in error band!!
	  double denfracerr=dentoterr/Integral(den);
	  double numfracerr=1/sqrt(Integral(num));
	  //std::cout<<Integral(den);
	  //std::cout<<denfracerr<<std::endl;
	  TH1F* errorband=(TH1F*)(den->Clone("errorband"));
	  for(int bin=0;bin<=den->GetNbinsX()+1;bin++){
	    den->SetBinError(bin,0);
	    errorband->SetBinContent(bin,Integral(num)/Integral(den));
	    errorband->SetBinError(bin,sqrt(pow(denfracerr,2)+pow(numfracerr,2)));
	  }


	  
	  //DIVIDE NUM BY DEN and put in ratio
	  TH1F* ratio;
	  ratio=(TH1F*)(num->Clone("ratio"));
	  ratio->GetYaxis()->SetNdivisions(505);
	  ratio->GetYaxis()->SetNdivisions(505);
	  ratio->GetYaxis()->SetTitleFont(62);
	  ratio->GetXaxis()->SetLabelSize(0.14);
	  ratio->GetXaxis()->SetLabelOffset(0.005);
	  ratio->GetXaxis()->SetTitleFont(62);
	  ratio->GetXaxis()->SetTitleSize(0.19);
	  ratio->GetXaxis()->SetTitleOffset(0.750);
	  ratio->GetYaxis()->SetTitleSize(0.19);
	  ratio->GetYaxis()->SetTitleOffset(0.2);
	  ratio->GetYaxis()->SetLabelSize(0.14);

	  errorband->GetYaxis()->SetNdivisions(505);
	  errorband->GetXaxis()->SetLabelSize(0.14);
	  errorband->GetXaxis()->SetTitleFont(62);
	  errorband->GetYaxis()->SetTitleFont(62);
	  errorband->GetXaxis()->SetTitleSize(0.19);
	  errorband->GetYaxis()->SetTitleSize(0.19);
	  errorband->GetYaxis()->SetTitleOffset(0.460);
	  errorband->GetXaxis()->SetTitleOffset(0.750);
          errorband->GetYaxis()->SetLabelSize(0.14);

	  

	  std::string xtitle;
	  xtitle=shapes_[iShape].histtitle().substr(shapes_[iShape].histtitle().find(";")+1);
	  xtitle=xtitle.substr(0,xtitle.find(";"));
	  ratio->GetXaxis()->SetTitle(xtitle.c_str());//!!GET TITLE FOR X AXIS
	  //	  ratio->GetXaxis()->SetTitleSize(0.1);
	  //ratio->GetXaxis()->SetTitleOffset(0.8);
	  //std::cout<<ratio->GetMaximum();
	  double ratiomax=ratio->GetMaximum()+0.1;
	  if(ratiomax<2)ratiomax=2;
	  ratio->GetYaxis()->SetRangeUser(0,ratiomax);
	  ratio->SetTitle("");
	  ratio->GetYaxis()->SetTitle("Data/Bkg");
	  ratio->GetYaxis()->SetTitleSize(0.19);
	  ratio->Divide(den);
	  gStyle->SetOptStat(0);
	  ratio->SetStats(0);



	  errorband->SetMarkerSize(0);
	  errorband->SetFillColor(16);
	  errorband->SetFillStyle(1000);//3013);
	  errorband->SetLineWidth(1);
	  errorband->GetXaxis()->SetTitle(xtitle.c_str());//!!GET TITLE FOR X AXIS
	  //	  errorband->GetXaxis()->SetTitleSize(0.1);
	  //errorband->GetXaxis()->SetTitleOffset(0.8);
	  errorband->GetYaxis()->SetRangeUser(0,2.0);
	  errorband->SetTitle("");
	  errorband->GetYaxis()->SetTitle("data/MC");
	  //errorband->GetYaxis()->SetTitleSize(0.1);
	  //errorband->GetYaxis()->SetTitleOffset(0.3);
	  //errorband->GetXaxis()->SetLabelSize(0.1);
	  //errorband->GetYaxis()->SetLabelSize(0.1);
	  errorband->SetStats(0);



	  TF1* fiterrup;
	  TF1* fiterrdown;
	  TLine *averageLine;
	  TLine *averageupLine;
	  TLine *averagedownLine;
	  if(do_ratio_fitline_){
	    //ratio->Fit("pol0","E");
	    averageLine = new TLine(ratio->GetXaxis()->GetBinLowEdge(1),Integral(num)/Integral(den),ratio->GetXaxis()->GetBinLowEdge(ratio->GetNbinsX()+1),Integral(num)/Integral(den));
	    averageupLine = new TLine(ratio->GetXaxis()->GetBinLowEdge(1),(Integral(num)+sqrt(Integral(num)))/Integral(den),ratio->GetXaxis()->GetBinLowEdge(ratio->GetNbinsX()+1),(Integral(num)+sqrt(Integral(num)))/Integral(den));
	    averagedownLine = new TLine(ratio->GetXaxis()->GetBinLowEdge(1),(Integral(num)-sqrt(Integral(num)))/Integral(den),ratio->GetXaxis()->GetBinLowEdge(ratio->GetNbinsX()+1),(Integral(num)-sqrt(Integral(num)))/Integral(den));
	    averageLine->SetLineColor(2);
	    averageupLine->SetLineColor(3);
	    averagedownLine->SetLineColor(3);
	    averageupLine->SetLineStyle(3);
	    averagedownLine->SetLineStyle(3);
	    // fiterrup=new TF1("pol0up","pol0");
// 	    TF1 *fitresult=ratio->GetFunction("pol0");
// 	    //fiterrup->SetParameters(fitresult->GetParameter(0)+fitresult->GetParError(0),fitresult->GetParameter(1)+fitresult->GetParError(1));
// 	    fiterrup->SetParameter(0,fitresult->GetParameter(0)+fitresult->GetParError(0));                                                             
// 	    fiterrdown=new TF1("pol0up","pol0");
// 	    //fiterrdown->SetParameters(fitresult->GetParameter(0)-fitresult->GetParError(0),fitresult->GetParameter(1)-fitresult->GetParError(1));
// 	    fiterrdown->SetParameter(0,fitresult->GetParameter(0)-fitresult->GetParError(0));                                                            
// 	    fiterrup->SetLineStyle(2);
// 	    fiterrdown->SetLineStyle(2);
// 	    fiterrup->SetLineColor(2);
// 	    fiterrdown->SetLineColor(2);
	  }
	  if(do_ratio_fitline_){
	    //fiterrup->Draw("same");
	    //fiterrdown->Draw("same");
	    errorband->Draw("E2");
	    averageLine->Draw("same");
	    ratio->Draw("E1same");
	    //averageupLine->Draw("same");
	    //averagedownLine->Draw("same");
	  }
	  else 	  ratio->Draw("E1");
	  TLine *centerLine = new TLine(ratio->GetXaxis()->GetBinLowEdge(1),1.0,ratio->GetXaxis()->GetBinLowEdge(ratio->GetNbinsX()+1),1.0);
	  centerLine->SetLineWidth(1);
	  if(!do_ratio_fitline_)centerLine->SetLineColor(2);
	  else centerLine->SetLineColor(1);
	  centerLine->Draw("same");
	  if(do_ratio_line_){
	    TLine *lowerLine = new TLine(ratio->GetXaxis()->GetBinLowEdge(1),0.9,ratio->GetXaxis()->GetBinLowEdge(ratio->GetNbinsX()+1),0.9);
	    TLine *upperLine = new TLine(ratio->GetXaxis()->GetBinLowEdge(1),1.1,ratio->GetXaxis()->GetBinLowEdge(ratio->GetNbinsX()+1),1.1);
	    lowerLine->SetLineWidth(2);
	    upperLine->SetLineWidth(2);
	    lowerLine->SetLineColor(7);
	    upperLine->SetLineColor(7);
	    lowerLine->Draw();
	    upperLine->Draw();
	  }
	}
      }
      //save as PDF
      c1->Update();
      std::ostringstream lsave;
      std::ostringstream lsavepng;
      std::string tmpstr = file->GetName();
      tmpstr.erase(std::string(file->GetName()).find(".root"),5);
      tmpstr=tmpstr+outsuffix_;
      lsave << tmpstr ;
      lsave << ".pdf" ;
      if (iShape==0) {
	lsave << "[";
	c1->Print(lsave.str().c_str());//open the file
	lsave.str("");//reset for adding the first plot
	lsave << tmpstr ;
	lsave << ".pdf" ;
      }
      c1->Print(lsave.str().c_str());
      if (iShape==shapes_.size()-1) {
	lsave << "]";
	c1->Print(lsave.str().c_str());//close the file
      }

      lsave.str("");
      lsave << tmpstr << "_" << c1->GetName() << ".pdf" ;
      c1->Print((lsave.str()).c_str());
      lsavepng.str("");
      lsavepng << tmpstr << "_" << c1->GetName() << ".png" ;
      c1->Print((lsavepng.str()).c_str());

      //WRITE TO FILE
      writedir->cd();
      c1->Write();
      c1->Close();
    }
    writedir->Close();

    return 0;
  };

}
