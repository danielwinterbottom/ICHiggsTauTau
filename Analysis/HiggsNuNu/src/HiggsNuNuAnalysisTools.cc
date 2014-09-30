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

namespace ic{
  
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
