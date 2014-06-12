#include "CombineTools/interface/CombineHarvester.h"
#include <iostream>
#include "CombineTools/interface/Observation.h"
#include "CombineTools/interface/Process.h"
#include "CombineTools/interface/Nuisance.h"
#include "CombineTools/interface/Parameter.h"

namespace ch {

CombineHarvester::CombineHarvester() { }

CombineHarvester::~CombineHarvester() { }

void swap(CombineHarvester& first, CombineHarvester& second) {
  using std::swap;
  swap(first.obs_, second.obs_);
  swap(first.procs_, second.procs_);
  swap(first.nus_, second.nus_);
  swap(first.params_, second.params_);
  swap(first.wspaces_, second.wspaces_);
}

CombineHarvester::CombineHarvester(CombineHarvester const& other)
    : obs_(other.obs_.size(), nullptr),
      procs_(other.procs_.size(), nullptr),
      nus_(other.nus_.size(), nullptr) {
  for (std::size_t i = 0; i < obs_.size(); ++i) {
    if (other.obs_[i]) {
      obs_[i] = std::make_shared<Observation>(*(other.obs_[i]));
    }
  }
  for (std::size_t i = 0; i < procs_.size(); ++i) {
    if (other.procs_[i]) {
      procs_[i] = std::make_shared<Process>(*(other.procs_[i]));
    }
  }
  for (std::size_t i = 0; i < nus_.size(); ++i) {
    if (other.nus_[i]) {
      nus_[i] = std::make_shared<Nuisance>(*(other.nus_[i]));
    }
  }
  for (auto const& it : other.params_) {
    if (it.second) {
      params_.insert({it.first, std::make_shared<Parameter>(*(it.second))});
    } else {
      params_.insert({it.first, nullptr});
    }
  }
  for (auto const& it : other.wspaces_) {
    if (it.second) {
      wspaces_.insert({it.first, std::make_shared<RooWorkspace>(*(it.second))});
    } else {
      wspaces_.insert({it.first, nullptr});
    }
  }
}

CombineHarvester::CombineHarvester(CombineHarvester&& other) {
  swap(*this, other);
}

CombineHarvester& CombineHarvester::operator=(CombineHarvester other) {
  swap(*this, other);
  return (*this);
}

// CombineHarvester CombineHarvester::shallow_copy() {
//   CombineHarvester cpy;
//   cpy.obs_      = obs_;
//   cpy.procs_    = procs_;
//   cpy.nus_      = nus_;
//   cpy.params_   = params_;
//   cpy.wspaces_  = wspaces_;
//   return cpy;
// }

CombineHarvester CombineHarvester::cp() {
  CombineHarvester cpy;
  cpy.obs_      = obs_;
  cpy.procs_    = procs_;
  cpy.nus_      = nus_;
  cpy.params_   = params_;
  cpy.wspaces_  = wspaces_;
  return cpy;
}

CombineHarvester & CombineHarvester::PrintAll() {
  std::cout << Observation::PrintHeader;
  for (unsigned i = 0; i < obs_.size(); ++i)
      std::cout << *(obs_[i]) << std::endl;
  std::cout << Process::PrintHeader;
  for (unsigned i = 0; i < procs_.size(); ++i)
      std::cout << *(procs_[i]) << std::endl;
  std::cout << Nuisance::PrintHeader;
  for (unsigned i = 0; i < nus_.size(); ++i)
      std::cout << *(nus_[i]) << std::endl;
  std::cout << Parameter::PrintHeader;
  for (auto const& it : params_) std::cout << *(it.second) << std::endl;
  return *this;
}

// void CombineHarvester::Validate() {
//   std::set<std::string> bins;
//   for (auto const p : procs_) {
//     bins.insert(p->bin());
//   }
//   for (auto b : bins) {
//     std::cout << "Checking bin: " << b << std::endl;
//     TH1F bkg =  this->shallow_copy().bin(true, {b}).backgrounds().GetShape();
//     TH1F sig =  this->shallow_copy().bin(true, {b}).signals().GetShape();
//     TH1F dat = this->shallow_copy().bin(true, {b}).GetObservedShape();
//     for (unsigned i = 1; i <= dat.GetNbinsX(); ++i) {
//       double y_dat = dat.GetBinContent(i);
//       double y_sig = sig.GetBinContent(i);
//       double y_bkg = bkg.GetBinContent(i);
//       if (y_sig > y_bkg) std::cout << "Histogram bin " << i << " has sig " << y_sig <<
//         ", bkg " << y_bkg << " and data " << y_dat << std::endl;
//     }
//   }
// }
//  /*

//  TGraphAsymmErrors BuildPoissonErrors(TH1F const& hist) {
//    TGraphAsymmErrors result(&hist);
//    double alpha = 1 - 0.6827;
//    for (int k = 0; k <result.GetN(); ++k) {
//      double x;
//      double y;
//      result.GetPoint(k, x, y);
//      // double err_y_up = 0.5 + std::sqrt(y+0.25);
//      // double err_y_dn = -0.5 + std::sqrt(y+0.25);
//      double L =  (y==0) ? 0  : (ROOT::Math::gamma_quantile(alpha/2,y,1.));
//      double U =  ROOT::Math::gamma_quantile_c(alpha/2,y+1,1) ;
//      result.SetPointEYhigh(k, U - y);
//      result.SetPointEYlow(k, y - L);
//    }
//    return result;
//  }


}
