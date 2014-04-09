#ifndef ICHiggsTauTau_HiggsNuNu_HinvVar_h
#define ICHiggsTauTau_HiggsNuNu_HinvVar_h
#include <vector>
#include "HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"

namespace ic{
  class Var{
    CLASS_MEMBER(Var,std::string,name)
    CLASS_MEMBER(Var,std::string,latex)
    CLASS_MEMBER(Var,std::string,variable)
    CLASS_MEMBER(Var,std::string,selection)
    public:
    Var();
    Var(std::string, std::string, std::string, std::string);
    const char* namecstr() {return name_.c_str();};
    const char* latexcstr() {return latex_.c_str();};
    const char* variablecstr() {return variable_.c_str();};
    const char* selectioncstr() {return selection_.c_str();};
  };

  class Rebinned1DVar: public Var {
    CLASS_MEMBER(Rebinned1DVar,std::vector<double>,bins)
    public:
    Rebinned1DVar(std::string, std::string, std::string,std::string);
    Rebinned1DVar(std::string, std::string, std::string, std::string,std::vector<double>);
    Rebinned1DVar & binsPushBack(double);
  };

  class RebinnedNDVar: public Var {
    CLASS_MEMBER(RebinnedNDVar,std::vector<std::vector<double> >,bins)
    public:
    RebinnedNDVar(std::string, std::string, std::string,std::string,int);
    RebinnedNDVar & binsPushBack(int,double);
  };
  
}
#endif
