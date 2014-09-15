#ifndef ICHiggsTauTau_CombineTools_MorphFunctions_h
#define ICHiggsTauTau_CombineTools_MorphFunctions_h
#include <algorithm>
#include <vector>
// #include <boost/range/begin.hpp>
// #include <boost/range/end.hpp>
// #include "boost/range/algorithm_ext/erase.hpp"
// #include "boost/algorithm/string.hpp"
// #include "boost/lexical_cast.hpp"
// #include "boost/regex.hpp"
// #include "boost/bind.hpp"
// #include "TFile.h"
// #include "TGraph.h"
#include "RooWorkspace.h"
#include "RooHistPdf.h"

#include "CombineTools/interface/CombineHarvester.h"
#include "CombineTools/interface/RooMorphingPdf.h"
#include "CombineTools/interface/RooHttYield.h"
#include "CombineTools/interface/VerticalInterpHistPdf.h"

namespace ch {

void BuildRooMorphing(RooWorkspace& ws, CombineHarvester& cb,
                      std::string const& mass_var, bool verbose);
}
#endif
