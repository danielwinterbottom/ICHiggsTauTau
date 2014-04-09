#include "HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include "HiggsNuNu/interface/HinvVar.h"
#include <iostream>
#include <vector>

namespace ic{

  Var::Var(){
  };

  Var::Var(std::string name, std::string latex, std::string variable, std::string selection){
    name_=name;
    latex_=latex;
    variable_=variable;
    selection_=selection;
  };

  Rebinned1DVar::Rebinned1DVar(std::string name, std::string latex, std::string variable, std::string selection){
    name_=name;
    latex_=latex;
    variable_=variable;
    selection_=selection;
  };

  Rebinned1DVar::Rebinned1DVar(std::string name, std::string latex, std::string variable, std::string selection,std::vector<double> bins){
    name_=name;
    latex_=latex;
    variable_=variable;
    selection_=selection;
    bins_=bins;
  };


  Rebinned1DVar & Rebinned1DVar::binsPushBack(double newbin){
    this->bins_.push_back(newbin);
    return *this;
  };

  RebinnedNDVar::RebinnedNDVar(std::string name, std::string latex, std::string variable, std::string selection,int D){
    name_=name;
    latex_=latex;
    variable_=variable;
    selection_=selection;
    std::vector<std::vector<double> > binstmp(D);
    bins_=binstmp;
  };

  RebinnedNDVar & RebinnedNDVar::binsPushBack(int D, double newbin){
    this->bins_[D].push_back(newbin);
    return *this;
  };
}
