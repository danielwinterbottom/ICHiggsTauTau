#include "../interface/RooHttYield.h"
// #include <iostream>
// #include "boost/format.hpp"
#include "RooHistPdf.h"
#include "RooDataHist.h"
#include "RooRealProxy.h"
#include "RooArgSet.h"
#include "RooAbsReal.h"
#include "TH1F.h"

RooHttYield::RooHttYield() : can_interpolate_(false) {}

RooHttYield::RooHttYield(const char* name, const char* title,
                         RooAbsReal& mh)
    : RooAbsReal(name, title),
      mh_("MH", "MH", this, mh),
      can_interpolate_(true) {
  // evaluate();
}

RooHttYield::RooHttYield(const RooHttYield& other, const char* name)
    : RooAbsReal(other, name),
      // ymap_(other.ymap_),
      mh_("MH", this, other.mh_),
      can_interpolate_(other.can_interpolate_) {
  TH1::AddDirectory(false);
  ymap_ = other.ymap_;
}

void RooHttYield::AddPoint(double point, double rate) {
  ymap_[point] = rate;
}

Double_t RooHttYield::evaluate() const {
  double mh = mh_;
  if (ymap_.empty()) return 1.0;
  if (ymap_.size() == 1) {
    return ymap_.begin()->second;
  }
  MapIter upper = ymap_.lower_bound(mh);
  if (upper == ymap_.begin()) {
    return upper->second;
  } else if (upper == ymap_.end()) {
    --upper;
    return upper->second;
  } else {
    MapIter lower = upper;
    --lower;
    if (can_interpolate_) {
      return (mh - lower->first) * ((upper->second - lower->second) /
                                    (upper->first - lower->first)) +
             lower->second;
    } else {
      if (fabs(upper->first - mh) <= fabs(mh - lower->first)) {
        return upper->second;
      } else {
        return lower->second;
      }
    }
  }
}