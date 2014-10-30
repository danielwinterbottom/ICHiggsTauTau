#ifndef CombineTools_HighLevelFunctions_h
#define CombineTools_HighLevelFunctions_h
#include <string>
#include <vector>
#include <set>
// #include <boost/range/begin.hpp>
// #include <boost/range/end.hpp>
// #include "boost/range/algorithm_ext/erase.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
// #include "boost/lexical_cast.hpp"
// #include "boost/regex.hpp"
// #include "boost/bind.hpp"
// #include "TFile.h"
// #include "TGraph.h"
// #include "RooFitResult.h"
// #include "RooHistPdf.h"
// #include "CombineTools/interface/Nuisance.h"
// #include "CombineTools/interface/Process.h"

#include "CombineTools/interface/CombineHarvester.h"
#include "CombineTools/interface/HelperFunctions.h"

namespace ch {
  void SetStandardBinNames(CombineHarvester * cb);

  std::vector<std::string> MassesFromRange(std::string const& input,
                                           std::string const& fmt = "%.0f");
}

#endif
