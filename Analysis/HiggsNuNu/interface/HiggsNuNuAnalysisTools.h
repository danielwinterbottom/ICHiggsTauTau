#ifndef ICHiggsTauTau_HiggsNuNu_HiggsNuNuAnalysisTools_h
#define ICHiggsTauTau_HiggsNuNu_HiggsNuNuAnalysisTools_h
#include <vector>
#include <map>
#include "TH1.h"
#include "Utilities/interface/SimpleParamParser.h"
#include "Utilities/interface/FnRootTools.h"

namespace ic {
  double Integral(TH1F const* hist);
  double Integral(TH1F const* hist, int binmin, int binmax);
  double IntegralWithError(TH1F const* hist, int binmin, int binmax, double err);
  double Error(TH1F const* hist,int binmin,int binmax);
  std::string BuildCutString(std::string const& selection,
			     std::string const& category,
			     std::string const& weight);

  class HiggsNuNuAnalysis {
  public:
    HiggsNuNuAnalysis(int verbosity);
  };

  class TrigEffAnalysis {
  public:
    TrigEffAnalysis(int verbosity);
  };

}
#endif
