#include "CombineTools/interface/CombineHarvester.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <utility>
#include <set>
#include <fstream>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/range/algorithm_ext/erase.hpp"
#include "boost/range/algorithm/find.hpp"
#include "boost/format.hpp"
#include "TDirectory.h"
#include "TH1.h"
#include "Utilities/interface/FnRootTools.h"
#include "Utilities/interface/FnPredicates.h"
#include "CombineTools/interface/Observation.h"
#include "CombineTools/interface/Process.h"
#include "CombineTools/interface/Nuisance.h"
#include "CombineTools/interface/Parameter.h"
#include "CombineTools/interface/MakeUnique.h"
#include "CombineTools/interface/HelperFunctions.h"

// #include "TMath.h"
// #include "boost/format.hpp"
// #include "Utilities/interface/FnPredicates.h"
// #include "Math/QuantFuncMathCore.h"

namespace ch {

CombineHarvester::ProcNusMap CombineHarvester::GenerateProcNusMap() {
  ProcNusMap lookup(procs_.size());
  for (unsigned i = 0; i < nus_.size(); ++i) {
    for (unsigned j = 0; j < procs_.size(); ++j) {
      if (MatchingProcess(*(nus_[i]), *(procs_[j]))) {
        lookup[j].push_back(nus_[i].get());
      }
    }
  }
  return lookup;
}

double CombineHarvester::GetUncertainty() {
  auto lookup = GenerateProcNusMap();
  double err_sq = 0.0;
  for (auto param_it : params_) {
    double backup = param_it.second->val();
    param_it.second->set_val(backup+param_it.second->err_d());
    double rate_d = this->GetRateInternal(lookup, param_it.first);
    param_it.second->set_val(backup+param_it.second->err_u());
    double rate_u = this->GetRateInternal(lookup, param_it.first);
    double err = std::fabs(rate_u-rate_d) / 2.0;
    err_sq += err * err;
    param_it.second->set_val(backup);
  }
  return std::sqrt(err_sq);
}

double CombineHarvester::GetUncertainty(RooFitResult const* fit, unsigned n_samples) {
  auto lookup = GenerateProcNusMap();
  double rate = GetRateInternal(lookup);
  double err_sq = 0.0;
  auto backup = GetParameters();
  for (unsigned i = 0; i < n_samples; ++i) {
    // std::cout << "Sampling " << i << "/" << n_samples << std::endl;
    this->UpdateParameters(ch::ExtractSampledFitParameters(*(fit)));
    double rand_rate = this->GetRateInternal(lookup);
    double err = std::fabs(rand_rate-rate);
    err_sq += (err*err);
  }
  this->UpdateParameters(backup);
  return std::sqrt(err_sq/double(n_samples));
}

TH1F CombineHarvester::GetShapeWithUncertainty() {
  auto lookup = GenerateProcNusMap();
  TH1F shape = GetShape();
  for (int i = 1; i <= shape.GetNbinsX(); ++i) {
    shape.SetBinError(i, 0.0);
  }
  for (auto param_it : params_) {
    double backup = param_it.second->val();
    param_it.second->set_val(backup+param_it.second->err_d());
    TH1F shape_d = this->GetShapeInternal(lookup, param_it.first);
    param_it.second->set_val(backup+param_it.second->err_u());
    TH1F shape_u = this->GetShapeInternal(lookup, param_it.first);
    for (int i = 1; i <= shape.GetNbinsX(); ++i) {
      double err = std::fabs(shape_u.GetBinContent(i)-shape_d.GetBinContent(i)) / 2.0;
      shape.SetBinError(i, err*err + shape.GetBinError(i));
    }
    param_it.second->set_val(backup);
  }
  for (int i = 1; i <= shape.GetNbinsX(); ++i) {
    shape.SetBinError(i, std::sqrt(shape.GetBinError(i)));
  }
  return shape;
}

TH1F CombineHarvester::GetShapeWithUncertainty(RooFitResult const* fit, unsigned n_samples) {
  auto lookup = GenerateProcNusMap();
  TH1F shape = GetShapeInternal(lookup);
  for (int i = 1; i <= shape.GetNbinsX(); ++i) {
    shape.SetBinError(i, 0.0);
  }
  auto backup = GetParameters();
  for (unsigned i = 0; i < n_samples; ++i) {
    // std::cout << "Sampling " << i << "/" << n_samples << std::endl;
    this->UpdateParameters(ch::ExtractSampledFitParameters(*(fit)));
    TH1F rand_shape = this->GetShapeInternal(lookup);
    for (int i = 1; i <= shape.GetNbinsX(); ++i) {
      double err = std::fabs(rand_shape.GetBinContent(i)-shape.GetBinContent(i));
      shape.SetBinError(i, err*err + shape.GetBinError(i));
    }
  }
  for (int i = 1; i <= shape.GetNbinsX(); ++i) {
    shape.SetBinError(i, std::sqrt(shape.GetBinError(i)/double(n_samples)));
  }
  this->UpdateParameters(backup);
  return shape;
}

double CombineHarvester::GetRate() {
  auto lookup = GenerateProcNusMap();
  return GetRateInternal(lookup);
}

TH1F CombineHarvester::GetShape() {
  auto lookup = GenerateProcNusMap();
  return GetShapeInternal(lookup);
}


double CombineHarvester::GetRateInternal(ProcNusMap const& lookup,
    std::string const& single_nus) {
  double rate = 0.0;
  // TH1F tot_shape
  for (unsigned i = 0; i < procs_.size(); ++i) {
    double p_rate = procs_[i]->rate();
    // If we are evaluating the effect of a single parameter
    // check the list of associated nuisances and skip if
    // this "single_nus" is not in the list
    if (single_nus != "") {
      if (!ch::any_of(lookup[i], [&](Nuisance const* nus) {
        return nus->name() == single_nus;
      })) continue;
    }
    for (auto nus_it : lookup[i]) {
      double x = params_[nus_it->name()]->val();
      if (nus_it->asymm()) {
        if (x >= 0) {
          p_rate *= std::pow(nus_it->value_u(), x);
        } else {
          p_rate *= std::pow(nus_it->value_d(), -1.0*x);
        }
      } else {
        p_rate *= std::pow(nus_it->value_u(), x);
      }
    }
    rate += p_rate;
  }
  return rate;
}

TH1F CombineHarvester::GetShapeInternal(ProcNusMap const& lookup,
    std::string const& single_nus) {
  TH1F shape = *((TH1F*)(procs_[0]->shape()));
  shape.Reset();

  for (unsigned i = 0; i < procs_.size(); ++i) {
    // Might be able to skip if only interested in one nuisance
    if (single_nus != "") {
      if (!ch::any_of(lookup[i], [&](Nuisance const* nus) {
        return nus->name() == single_nus;
      })) continue;
    }

    double p_rate = procs_[i]->rate();
    TH1F proc_shape = *((TH1F*)(procs_[i]->shape()));

    for (auto nus_it : lookup[i]) {
      double x = params_[nus_it->name()]->val();
      if (nus_it->asymm()) {
        if (x >= 0) {
          p_rate *= std::pow(nus_it->value_u(), x);
        } else {
          p_rate *= std::pow(nus_it->value_d(), -1.0*x);
        }
        if (nus_it->type() == "shape") {
          TH1F diff = ShapeDiff(x, procs_[i]->shape(),
            nus_it->shape_d(), nus_it->shape_u());
          proc_shape.Add(&diff);
        }
      } else {
        p_rate *= std::pow(nus_it->value_u(), x);
      }
    }
    proc_shape.Scale(p_rate);
    shape.Add(&proc_shape);
  }
  return shape;
}

double CombineHarvester::GetObservedRate() {
  double rate = 0.0;
  for (unsigned i = 0; i < obs_.size(); ++i) {
    rate += obs_[i]->rate();
  }
  return rate;
}

TH1F CombineHarvester::GetObservedShape() {
  TH1F shape = *((TH1F*)(obs_[0]->shape()));
  shape.Reset();
  for (unsigned i = 0; i < obs_.size(); ++i) {
    TH1F proc_shape = *((TH1F*)(obs_[i]->shape()));
    double p_rate = obs_[i]->rate();
    proc_shape.Scale(p_rate);
    shape.Add(&proc_shape);
  }
  return shape;
}

TH1F CombineHarvester::ShapeDiff(double x,
    TH1 const* nom,
    TH1 const* low,
    TH1 const* high) {
  TH1F diff = *(TH1F*)(high);
  diff.Add(low, -1.0);
  TH1F sum = *(TH1F*)(high);
  sum.Add(low, 1.0);
  sum.Add(nom, -2.0);
  sum.Scale(smoothStepFunc(x));
  sum.Add(&diff);
  sum.Scale(0.5*x);
  return sum;
}

void CombineHarvester::SetParameters(std::vector<ch::Parameter> params) {
  params_.clear();
  for (unsigned i = 0; i < params.size(); ++i) {
    params_[params[i].name()] = std::make_shared<ch::Parameter>(params[i]);
  }
}

void CombineHarvester::UpdateParameters(std::vector<ch::Parameter> params) {
  for (unsigned i = 0; i < params.size(); ++i) {
    auto it = params_.find(params[i].name());
    if (it != params_.end()) {
      it->second->set_val(params[i].val());
      it->second->set_err_d(params[i].err_d());
      it->second->set_err_u(params[i].err_u());
    }
  }
}

std::vector<ch::Parameter> CombineHarvester::GetParameters() const {
  std::vector<ch::Parameter> params;
  for (auto const& it : params_) {
    params.push_back(*(it.second));
  }
  return params;
}

}
