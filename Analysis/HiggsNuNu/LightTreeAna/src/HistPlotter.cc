#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/HistPlotter.h"
#include <iostream>
#include "TH1F.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include <map>
//#include "CommonTools/Utils/interface/TFileDirectory.h"
#include <boost/algorithm/string.hpp>
#include "TDirectory.h"
#include "TFile.h"
#include "TPad.h"
#include "TLatex.h"
#include "TLine.h"
#include "TBox.h"
#include "TASImage.h"

namespace ic{
  LTPlotElement::LTPlotElement(){
    unit_="GeV";
    in_stack_=false;
    is_inrationum_=false;
    is_inratioden_=false;
  };

  LTPlotElement::~LTPlotElement(){ ;};

  LTShapeElement::LTShapeElement(){
    dology_=false;
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
    ele->set_line_style(11);
    ele->set_fill_color(0);
    ele->set_line_color(ele->color());
    ele->set_line_width(2);
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
    do_ratio_=false;
    do_ratio_line_=false;
    add_underflows_=false;
    add_overflows_=false;
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

    //GET DIRECTORY TO WRITE TO
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
      THStack *stack=new THStack("stack","stacked plots");
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

	if (add_underflows_ || add_overflows_){
	  int tmpbins = histo->GetNbinsX();
	  double tmpmin = histo->GetXaxis()->GetBinLowEdge(1);
	  double tmpmax = histo->GetXaxis()->GetBinLowEdge(tmpbins+1);
	  double tmpsize = (tmpmax-tmpmin)/tmpbins;
	  if (add_underflows_) {
	    tmpbins+=1;
	    tmpmin = tmpmin-tmpsize;
	  }
	  if (add_overflows_){
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
	  if (add_overflows_) {
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

      //SETUP THE CANVAS
      TCanvas *c1=new TCanvas(shapes_[iShape].name().c_str(),shapes_[iShape].name().c_str());
      c1->cd();
	TPad* upper = nullptr;
	TPad* lower = nullptr;
      if(do_ratio_){
	  upper = new TPad("upper","pad",0, 0.26 ,1 ,1);
	  lower = new TPad("lower","pad",0, 0   ,1 ,0.26);
	  upper->SetBottomMargin(0.02);
	  upper->Draw();
	  upper->cd();
	  upper->SetLogy(shapes_[iShape].dology());
      }
      else c1->SetLogy(shapes_[iShape].dology());
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
	  stack->SetMaximum(ymax);
	  //stack->GetXaxis()->SetTitle("");
	  stack->Draw("hist");
	  c1->Update();
	  first=false;
	  if(do_ratio_){
	    stack->GetXaxis()->SetLabelOffset(999);
	    stack->GetXaxis()->SetLabelSize(0);
	    std::string ytitle;
	    //	    ytitle=histTitles_[iShape].substr(histTitles_[iShape].find(";")+1);
	    ytitle=shapes_[iShape].histtitle().substr(shapes_[iShape].histtitle().find(";")+1);
	    ytitle=ytitle.substr(ytitle.find(";")+1);
	    ytitle=ytitle.substr(0,ytitle.find(";"));
	    stack->SetTitle((";;"+ytitle).c_str());
	  }
	}
	else stack->Draw("histsame");
      }
      std::cout<<"    Drawing Unstacked.."<<std::endl;
      for(unsigned iElement=0;iElement<elements_.size();iElement++){
	if(!(elements_[iElement].in_stack())){
	  if(first){
	    elements_[iElement].hist_ptr()->Draw(elements_[iElement].drawopts().c_str());
	    elements_[iElement].hist_ptr()->GetYaxis()->SetRangeUser(0.,ymax+10);
	    elements_[iElement].hist_ptr()->Draw(elements_[iElement].drawopts().c_str());
	    c1->Update();
	    first=false;
	    if(do_ratio_){
	      elements_[iElement].hist_ptr()->GetXaxis()->SetLabelOffset(999);
	      elements_[iElement].hist_ptr()->GetXaxis()->SetLabelSize(0);
	      std::string ytitle;
	      ytitle=shapes_[iShape].histtitle().substr(shapes_[iShape].histtitle().find(";")+1);
	      ytitle=ytitle.substr(ytitle.find(";")+1);
	      ytitle=ytitle.substr(0,ytitle.find(";"));
	      elements_[iElement].hist_ptr()->GetYaxis()->SetTitle(ytitle.c_str());
	    }
	  }
	  else elements_[iElement].hist_ptr()->Draw(("same"+elements_[iElement].drawopts()).c_str());
	}
      }

      //SETUP AND DRAW THE LEGEND
      TLegend* leg =new TLegend(0.75,0.3,0.89,0.89);
      leg->SetFillStyle(0);
      leg->SetLineColor(10);
      for(unsigned iElement=0;iElement<elements_.size();iElement++){
	leg->AddEntry(elements_[iElement].hist_ptr(),elements_[iElement].hist_ptr()->GetName(),elements_[iElement].legopts().c_str());
      }
      leg->Draw("same");
      c1->Update();

      //DRAW RATIO PLOT
      if(do_ratio_){
	c1->cd();
	lower->SetTopMargin(0.03);
	lower->SetBottomMargin(0.2);
	lower->Draw();
	lower->cd();
	lower->SetGridy();

		
	bool firstnum=true;
	bool firstden=true;
	TH1F* num = 0;
	TH1F* den = 0;
	for(unsigned iElement=0;iElement<elements_.size();iElement++){
	  if(elements_[iElement].is_inrationum()){
	    //ADD TO num HIST
	    if(firstnum){
	      num=(TH1F*)(elements_[iElement].hist_ptr()->Clone("num"));
	      firstnum=false;
	    }
	    else num->Add(elements_[iElement].hist_ptr());
	  }
	  if(elements_[iElement].is_inratioden()){
	    //ADD TO den HIST
	    if(firstden){
	      den=(TH1F*)(elements_[iElement].hist_ptr()->Clone("den"));
	      firstden=false;
	    }
	    else den->Add(elements_[iElement].hist_ptr());
	  }
	}
	if(firstnum||firstden)std::cout<<"To draw ratio plot you must specify elements to be numerator and denominator! Ratio plot will be missing."<<std::endl;
	else{
	  //DIVIDE NUM BY DEN and put in ratio
	  TH1F* ratio;
	  ratio=(TH1F*)(num->Clone("ratio"));
	  ratio->GetXaxis()->SetLabelSize(0.1);
	  ratio->GetYaxis()->SetLabelSize(0.1);

	  std::string xtitle;
	  xtitle=shapes_[iShape].histtitle().substr(shapes_[iShape].histtitle().find(";")+1);
	  xtitle=xtitle.substr(0,xtitle.find(";"));
	  ratio->GetXaxis()->SetTitle(xtitle.c_str());//!!GET TITLE FOR X AXIS
	  ratio->GetXaxis()->SetTitleSize(0.1);
	  ratio->GetXaxis()->SetTitleOffset(0.8);
	  ratio->GetYaxis()->SetRangeUser(0,2.0);
	  ratio->SetTitle("");
	  ratio->GetYaxis()->SetTitle("data/MC");
	  ratio->GetYaxis()->SetTitleSize(0.1);
	  ratio->GetYaxis()->SetTitleOffset(0.3);
	  ratio->Divide(den);
	  gStyle->SetOptStat(0);
	  ratio->SetStats(0);
	  ratio->Draw("E1");
	  TLine *centerLine = new TLine(ratio->GetXaxis()->GetBinLowEdge(1),1.0,ratio->GetXaxis()->GetBinLowEdge(ratio->GetNbinsX()+1),1.0);
	  centerLine->SetLineWidth(1);
	  centerLine->SetLineColor(2);
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
      std::string tmpstr = file->GetName();
      tmpstr.erase(std::string(file->GetName()).find(".root"),5);
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

      //WRITE TO FILE
      writedir->cd();
      c1->Write();
      c1->Close();
    }
    writedir->Close();

    return 0;
  };

}
