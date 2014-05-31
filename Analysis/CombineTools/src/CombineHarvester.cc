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
  std::cout << "Doing full copy!\n";
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

CombineHarvester CombineHarvester::shallow_copy() {
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
//  void CombineHarvester::WeightSoverB() {
//    // First need to identify the unique set of (mass, era, channel, category) - these will be
//    // reweighted individually.
//    std::vector<CategoryKey> keys;
//    for (unsigned i = 0; i  < processes_.size(); ++i) {
//      CategoryKey a(processes_[i].GetKey());
//      if (std::find(keys.begin(), keys.end(), a) == keys.end()) keys.push_back(a);
//    }
//    for (unssigned i = 0; i < keys.size(); ++i) {
//      std::cout << "Calculating S/B weight for: " << keys[i].channel << " " << keys[i].era << " " << keys[i].category << std::endl;
//      TH1F sig_shape = this->key_match(keys[i]).signals().GetShape();
//      TH1F bkg_shape = this->key_match(keys[i]).backgrounds().GetShape();
//      // Find the range from the lowest edge containing 15.9% of the signal
//      double xmin = sig_shape.GetXaxis()->GetXmin();
//      double xmax = sig_shape.GetXaxis()->GetXmax();
//      unsigned steps = 350;
//      double step_size = (xmax-xmin)/double(steps);
//      double sig_tot = sig_shape.Integral();
//      double lower_limit = 0;
//      double upper_limit = 0;
//      for (unsigned j = 0; j < steps; ++j) {
//        double integral = IntegrateFloatRange(&sig_shape, xmin, xmin+(step_size*double(j)));
//        if (integral/sig_tot > 0.159) {
//          lower_limit = xmin+(step_size*double(j));
//          break;
//        }
//      }
//      for (unsigned j = 0; j < steps; ++j) {
//        double integral = IntegrateFloatRange(&sig_shape, xmax - (step_size*double(j)), xmax);
//        if (integral/sig_tot > 0.159) {
//          upper_limit = xmax - (step_size*double(j));
//          break;
//        }
//      }
//      std::cout << "Found 68\% limits at " << lower_limit << "," << upper_limit << std::endl;
//      double signal_yield = IntegrateFloatRange(&sig_shape, lower_limit, upper_limit);
//      double backgr_yield = IntegrateFloatRange(&bkg_shape, lower_limit, upper_limit);
//      double weight = signal_yield / backgr_yield;
//      std::cout << "S/B: " << weight << std::endl;
//      for (unsigned j = 0; j < obs_.size(); ++j) {
//        if (obs_[j].GetKey() == keys[i]) {
//          obs_[j].rate *= weight;
//          if (obs_[j].shape) obs_[j].shape->Scale(weight);
//          if (obs_[j].errors) {
//            for (int k = 0; k < obs_[j].errors->GetN(); ++k) {
//              double x;
//              double y;
//              obs_[j].errors->GetPoint(k, x, y);
//              obs_[j].errors->SetPoint(k, x, y*weight);
//              double err_y_up = weight * obs_[j].errors->GetErrorYhigh(k);
//              double err_y_dn = weight * obs_[j].errors->GetErrorYlow(k);
//              obs_[j].errors->SetPointEYhigh(k, err_y_up);
//              obs_[j].errors->SetPointEYlow(k, err_y_dn);
//            }
//          }
//        }
//      }
//      for (unsigned j = 0; j < processes_.size(); ++j) {
//        if (processes_[j].GetKey() == keys[i]) {
//          processes_[j].rate *= weight;
//          if (processes_[j].shape) processes_[j].shape->Scale(weight);
//        }
//      }
//    }
//  }
//  void CombineHarvester::VariableRebin(std::vector<double> bins) {
//    for (unsigned i = 0; i < processes_.size(); ++i) {
//      if (processes_[i].shape) {
//        processes_[i].shape = (TH1F*)processes_[i].shape->Rebin(bins.size()-1,"",&(bins[0]));
//      }
//    }
//    for (unsigned i = 0; i < obs_.size(); ++i) {
//      if (obs_[i].shape) {
//        obs_[i].shape = (TH1F*)obs_[i].shape->Rebin(bins.size()-1,"",&(bins[0]));
//        // If we rebin then recreate the errors from scratch
//        if (obs_[i].errors) delete obs_[i].errors;
//        obs_[i].errors = new TGraphAsymmErrors(BuildPoissonErrors(*(obs_[i].shape)));
//      }
//    }
//    for (unsigned i = 0; i < nuisances_.size(); ++i) {
//      if (nuisances_[i].shape) nuisances_[i].shape = (TH1F*)nuisances_[i].shape->Rebin(bins.size()-1,"",&(bins[0]));
//      if (nuisances_[i].shape_down) nuisances_[i].shape_down = (TH1F*)nuisances_[i].shape_down->Rebin(bins.size()-1,"",&(bins[0]));
//      if (nuisances_[i].shape_up) nuisances_[i].shape_up = (TH1F*)nuisances_[i].shape_up->Rebin(bins.size()-1,"",&(bins[0]));
//    }
//  }
//  bool CombineHarvester::HasProcess(std::string const& process) const {
//    for (unsigned i = 0; i < processes_.size(); ++i) {
//      if (processes_[i].process == process) return true;
//    }
//    return false;
//  }

//  void CombineHarvester::ScaleProcessByEra(std::string const& process, std::string const& era, double scale) {
//    for (unsigned i = 0; i < processes_.size(); ++i) {
//      if (processes_[i].process == process && processes_[i].era == era) {
//        processes_[i].rate *= scale;
//        if (processes_[i].shape) processes_[i].shape->Scale(scale);
//      }
//    }
//  }

//  std::pair<double, int> CombineHarvester::GetPullsChi2(bool splusb) const {
//    double tot = 0.0;
//    for (unsigned i = 0; i < pulls_.size(); ++i) {
//      tot += splusb ? (pulls_[i].splusb*pulls_[i].splusb) : (pulls_[i].bonly*pulls_[i].bonly);
//    }
//    return std::make_pair(tot, pulls_.size());
//  }

}
