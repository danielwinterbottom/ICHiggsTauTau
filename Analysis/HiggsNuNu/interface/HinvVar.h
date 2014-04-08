#ifndef ICHiggsTauTau_HiggsNuNu_HinvVar_h
#define ICHiggsTauTau_HiggsNuNu_HinvVar_h
#include <vector>
#include "HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"

namespace ic{
class Var{
  CLASS_MEMBER(Var,std::string,name)
  CLASS_MEMBER(Var,std::string,latex)
  CLASS_MEMBER(Var,std::string,variable)
  CLASS_MEMBER(Var,std::string,extraselection)
  CLASS_MEMBER(Var,std::vector<double>,bins)
  public:
  Var(std::string, std::string, std::string, std::string,std::vector<double>);
  Var(std::string, std::string, std::string, std::string);
  Var & binsPushBack(double);
  const char* namecstr() {return name_.c_str();};
  const char* latexcstr() {return latex_.c_str();};
  const char* variablecstr() {return variable_.c_str();};
  const char* extraselectioncstr() {return extraselection_.c_str();};
};
}
#endif
