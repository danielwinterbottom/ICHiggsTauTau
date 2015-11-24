#include "HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <iostream>
#include <vector>
#include <map>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/range/algorithm_ext.hpp"
#include "boost/filesystem.hpp"
#include "Utilities/interface/SimpleParamParser.h"
#include "Utilities/interface/FnRootTools.h"
#include "TEfficiency.h"
#include "TH3F.h"
#include "TPad.h"
#include "TLatex.h"

namespace ic{

  void DrawCMSLogoTest(TPad* pad, TString cmsText, TString extraText, int iPosX,float relPosX, float relPosY, float relExtraDY) {
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
    TString lumi_13TeV = "2.11 fb^{-1}";
    TString lumi_8TeV = "19.2 fb^{-1}";
    TString lumi_7TeV = "5.1 fb^{-1}";

    lumiText +=lumi_13TeV;//lumi_8TeV;
    lumiText +=" (13 TeV)";


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

  void DrawCMSLogoTest(TPad* pad, TString cmsText, TString extraText, int iPosX) {
    DrawCMSLogoTest(pad, cmsText, extraText, iPosX, 0.045, 0.035, 1.2);
  }
  

  
  double Integral(TH1F const* hist) {
    if (hist) {
      double ltmp =hist->Integral(0, hist->GetNbinsX() + 1);
      if (ltmp<0 || ltmp != ltmp) {
	std::cout << " -- Warning: integral is " << ltmp << ". Setting to 0." << std::endl;
	ltmp=0;
      }
      return ltmp;
    }
  else return 0;
  }

  double Error(TH1F const* hist) {
    double err = 0.0;
    if (hist) {
      hist->IntegralAndError(0, hist->GetNbinsX()+1, err);
      if (err<0 || err != err) {
	std::cout << " -- Warning: error on integral is " << err << ". Setting to 0." << std::endl;
	err=0;
      }
    }
    return err;
  }
  
  double IntegralWithError(TH1F const* hist, int binmin, int binmax, double &err){
    if (hist) {
      double ltmp =hist->IntegralAndError(binmin, binmax, err);
      if (ltmp<0 || ltmp != ltmp) {
	std::cout << " -- Warning: integral is " << ltmp << ". Setting to 0." << std::endl;
	ltmp=0;
      }
      return ltmp;
    }
    else return 0;
  }

  double IntegralWithError(TH1F const* hist, double err){
    if (hist) {
      double ltmp =hist->IntegralAndError(0, hist->GetNbinsX()+1, err);
      if (ltmp<0 || ltmp != ltmp) {
	std::cout << " -- Warning: integral is " << ltmp << ". Setting to 0." << std::endl;
	ltmp=0;
      }
      return ltmp;
    }
    else return 0;
  }
  
  double Integral(TH1F const* hist, int binmin, int binmax){
    if (hist) {
      double ltmp =hist->Integral(binmin, binmax);
      if (ltmp<0 || ltmp != ltmp) {
	std::cout << " -- Warning: integral is " << ltmp << ". Setting to 0." << std::endl;
	ltmp=0;
      }
      return ltmp;
    }
    else return 0;
  }

  double Integral(TH3F const* hist, int xbinmin, int xbinmax, int ybinmin, int ybinmax, int zbinmin, int zbinmax){
    if (hist) {
      double ltmp =hist->Integral(xbinmin, xbinmax,ybinmin, ybinmax,zbinmin, zbinmax);
      if (ltmp<0 || ltmp != ltmp) {
	std::cout << " -- Warning: integral is " << ltmp << ". Setting to 0." << std::endl;
	ltmp=0;
      }
      return ltmp;
    }
    else return 0;
  }
  
  double Error(TH1F const* hist,int binmin,int binmax) {
    double err = 0.0;
    if (hist) {
      hist->IntegralAndError(binmin, binmax, err);
      if (err<0 || err != err) {
	std::cout << " -- Warning: error on integral is " << err << ". Setting to 0." << std::endl;
	err=0;
      }
    }
    return err;
  }

  double Error(TH3F const* hist,int xbinmin, int xbinmax, int ybinmin, int ybinmax, int zbinmin, int zbinmax) {
    double err = 0.0;
    if (hist) {
      hist->IntegralAndError(xbinmin, xbinmax,ybinmin, ybinmax,zbinmin, zbinmax, err);
      if (err<0 || err != err) {
	std::cout << " -- Warning: error on integral is " << err << ". Setting to 0." << std::endl;
	err=0;
      }
    }
    return err;
  }
  
  std::string BuildCutString(std::string const& selection,
			     std::string const& category,
			     std::string const& weight) {
    std::string full_selection;
    if (weight != "" && (selection != "" || category != "")) full_selection += "( ";
    
    if (selection != "")                    full_selection += ("(" + selection + ")");
    if (selection != "" && category != "")  full_selection += " && ";
    if (category != "")                     full_selection += ("(" + category + ")");
    if (weight != "" && (selection != "" || category != "")) full_selection += " ) * ";
    if (weight != "") full_selection += ("("+weight+")");
    return full_selection;
  }

  std::string BuildVarString(std::string const& variable) {
    std::string full_variable = variable;
    if (full_variable.find_last_of("(") != full_variable.npos
        && full_variable.find("[") == full_variable.npos
        && full_variable.find("]") == full_variable.npos) {
      full_variable.insert(full_variable.find_last_of("("),">>htemp");
    }
    return full_variable;
  }

  TH1F GetShape(std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, TTree* ttree){
    std::string full_variable= BuildVarString(variable);
    std::string full_selection = BuildCutString(selection, category, weight);
    TH1::AddDirectory(true);
    ttree->Draw(full_variable.c_str(), full_selection.c_str(), "goff");
    TH1::AddDirectory(false);
    TH1F* htemp = (TH1F*)gDirectory->Get("htemp");
    if (!htemp) {
      std::cout << " ERROR! Histogram " << full_variable.c_str() << " not found for selection " << full_selection.c_str() << std::endl;
      std::cout<<"Returning empty histogram!"<<std::endl;
      TH1F hshape;
      hshape.SetName("ERROR");
      return hshape;
    }
    TH1F hshape= (*htemp);
    gDirectory->Delete("htemp;*");
    return hshape;
  }

  TH3F GetShape3D(std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, TTree* ttree){
    std::string full_variable= BuildVarString(variable);
    std::string full_selection = BuildCutString(selection, category, weight);
    TH3::AddDirectory(true);
    ttree->Draw(full_variable.c_str(), full_selection.c_str(), "goff");
    TH3::AddDirectory(false);
    TH3F* htemp = (TH3F*)gDirectory->Get("htemp");
    if (!htemp) {
      std::cout << " ERROR! Histogram " << full_variable.c_str() << " not found for selection " << full_selection.c_str() << std::endl;
      std::cout<<"Returning empty histogram!"<<std::endl;
      TH3F hshape;
      hshape.SetName("ERROR");
      return hshape;
    }
    TH3F hshape= (*htemp);
    gDirectory->Delete("htemp;*");
    return hshape;
  }

}
