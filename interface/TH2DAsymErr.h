//----------------------------------------------------------------------------
// $Id: TH2DAsymErr.h,v 1.1 2012/11/11 11:41:18 agilbert Exp $
//
// TH2DAsymErr
//
// Histogram that stores separate asymmetric statistical and systematic errors.
// It is
// derived from TH2D.
//
// Authors: S.Xie
//----------------------------------------------------------------------------

#ifndef HIST2DASYMERROR_H
#define HIST2DASYMERROR_H

#include <TH2D.h>
#include "Rtypes.h"

namespace mithep {
class TH2DAsymErr : public TH2D {
 public:
  enum EErrType {
    kStatErrLow = 0,
    kStatErrHigh,
    kSysErrLow,
    kSysErrHigh
  };

  TH2DAsymErr() {}
  TH2DAsymErr(const char *name, const char *title, Int_t nbinsx, Double_t xlow,
              Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup);
  TH2DAsymErr(const TH2D &h2d);
  ~TH2DAsymErr() {}

  using TH2D::GetBinContent;
  Double_t GetError(Double_t x, Double_t y, EErrType t);
  virtual Double_t GetBinError(Int_t b, Int_t c, EErrType t);
  using TH1::GetBinError;
  Double_t GetStatErrorLow(Double_t x, Double_t y) {
    return GetError(x, y, kStatErrLow);
  }
  Double_t GetBinStatErrorLow(Int_t b, Int_t c) {
    return GetBinError(b, c, kStatErrLow);
  }
  Double_t GetStatErrorHigh(Double_t x, Double_t y) {
    return GetError(x, y, kStatErrHigh);
  }
  Double_t GetBinStatErrorHigh(Int_t b, Int_t c) {
    return GetBinError(b, c, kStatErrHigh);
  }
  Double_t GetSysErrorLow(Double_t x, Double_t y) {
    return GetError(x, y, kSysErrLow);
  }
  Double_t GetBinSysErrorLow(Int_t b, Int_t c) {
    return GetBinError(b, c, kSysErrLow);
  }
  Double_t GetSysErrorHigh(Double_t x, Double_t y) {
    return GetError(x, y, kSysErrHigh);
  }
  Double_t GetBinSysErrorHigh(Int_t b, Int_t c) {
    return GetBinError(b, c, kSysErrHigh);
  }
  using TH2D::SetBinContent;
  void SetBinContent(Int_t binx, Int_t biny, Double_t value,
                     Double_t statErrorLow, Double_t statErrorHigh,
                     Double_t sysErrorLow, Double_t sysErrorHigh);
  using TH2D::SetBinError;
  void SetBinError(Int_t binx, Int_t biny, Double_t statErrorLow,
                   Double_t statErrorHigh, Double_t sysErrorLow,
                   Double_t sysErrorHigh);

 protected:
  TArrayD fStatErrorLow;  // array to store statistical low errors
  TArrayD fStatErrorHigh;  // array to store statistical high errors
  TArrayD fSysErrorLow;  // array to store systematic low errors
  TArrayD fSysErrorHigh;  // array to store systematic high errors

  ClassDef(TH2DAsymErr,
           1)  // Histogram for storage of seperate asymmetric errors
};
}
#endif
