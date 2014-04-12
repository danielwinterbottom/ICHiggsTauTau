#include "CombineTools/interface/HelperFunctions.h"
#include <iostream>
#include <vector>
#include "RooFitResult.h"
#include "RooRealVar.h"

namespace ch {

  std::vector<ch::Parameter> ExtractFitParameters(RooFitResult const& res) {
    std::vector<ch::Parameter> params;
    params.resize(res.floatParsFinal().getSize());
    for (int i = 0; i < res.floatParsFinal().getSize(); ++i) {
      RooRealVar const* var = dynamic_cast<RooRealVar const*>(res.floatParsFinal().at(i));
      params[i].set_name(std::string(var->GetName()));
      params[i].set_val(var->getVal());
      params[i].set_err_d(var->getErrorLo());
      params[i].set_err_u(var->getErrorHi());
    }
    return params;
  }

  std::vector<ch::Parameter> ExtractSampledFitParameters(RooFitResult const& res) {
    std::vector<ch::Parameter> params;
    params.resize(res.floatParsFinal().getSize());
    RooArgList const& rands = res.randomizePars();
    for (int i = 0; i < res.floatParsFinal().getSize(); ++i) {
      RooRealVar const* var = dynamic_cast<RooRealVar const*>(rands.at(i));
      params[i].set_name(std::string(var->GetName()));
      params[i].set_val(var->getVal());
      params[i].set_err_d(var->getErrorLo());
      params[i].set_err_u(var->getErrorHi());
    }
    return params;
  }

}
