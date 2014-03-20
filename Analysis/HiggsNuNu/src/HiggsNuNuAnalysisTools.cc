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
  
  double IntegralWithError(TH1F const* hist, int binmin, int binmax, double err){
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
}
