#include "HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include "HiggsNuNu/interface/HinvVar.h"
#include <iostream>
#include <vector>

namespace ic{
  Var::Var(std::string name, std::string latex, std::string variable, std::string extraselection,std::vector<double> bins){
    name_=name;
    latex_=latex;
    variable_=variable;
    extraselection_=extraselection;
    bins_=bins;
  };

  Var::Var(std::string name, std::string latex, std::string variable, std::string extraselection){
    name_=name;
    latex_=latex;
    variable_=variable;
    extraselection_=extraselection;
  };

  Var & Var::binsPushBack(double newbin){
    this->bins_.push_back(newbin);
    return *this;
  };
}
