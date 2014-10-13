#ifndef ICHiggsTauTau_CombineTools_MorphFunctions_h
#define ICHiggsTauTau_CombineTools_MorphFunctions_h
#include <algorithm>
#include <vector>
#include "RooWorkspace.h"
#include "RooHistPdf.h"

#include "CombineTools/interface/CombineHarvester.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "HiggsAnalysis/CombinedLimit/interface/RooMorphingPdf.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooSpline1D.h"
#include "HiggsAnalysis/CombinedLimit/interface/VerticalInterpHistPdf.h"
#include "HiggsAnalysis/CombinedLimit/interface/AsymPow.h"
#pragma GCC diagnostic pop

namespace ch {

void BuildRooMorphing(RooWorkspace& ws, CombineHarvester& cb,
                      RooAbsReal& mass_var, bool verbose,
                      std::string norm_postfix = "_norm");
}
#endif
