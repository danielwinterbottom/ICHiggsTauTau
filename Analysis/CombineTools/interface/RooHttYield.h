#ifndef RooHttYield_h
#define RooHttYield_h
#include <map>
#include "RooHistPdf.h"
#include "RooDataHist.h"
#include "RooRealProxy.h"
#include "RooArgSet.h"
#include "RooAbsReal.h"
#include "TH1F.h"
#include "Rtypes.h"

// #include <memory>
// #include <string>
// #include "TH1.h"
// #include "CombineTools/interface/MakeUnique.h"

class RooHttYield : public RooAbsReal {
 private:
  mutable std::map<double, double> ymap_;
  typedef std::map<double, double>::const_iterator MapIter;
  RooRealProxy mh_;
  bool can_interpolate_;

 public:
  RooHttYield();
  RooHttYield(const char* name, const char* title, RooAbsReal& mh);
  RooHttYield(const RooHttYield& other, const char* name = 0);
  virtual ~RooHttYield() {}
  virtual TObject* clone(const char* newname) const {
    return new RooHttYield(*this, newname);
  }

  void AddPoint(double point, double rate);

  virtual Double_t evaluate() const;

 public:
  ClassDef(RooHttYield, 1);
};

#endif
