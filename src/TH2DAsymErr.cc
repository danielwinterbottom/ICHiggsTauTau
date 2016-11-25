// $Id: TH2DAsymErr.cc,v 1.1 2012/09/14 08:16:14 ic Exp $

#include "../interface/TH2DAsymErr.h"
#include <TList.h>
#include <iostream>

//ClassImp(mithep::TH2DAsymErr)

using namespace mithep;

//--------------------------------------------------------------------------------------------------
TH2DAsymErr::TH2DAsymErr(const char *name, const char *title, 
                         Int_t nbinsx, Double_t xlow, Double_t xup
                         ,Int_t nbinsy, Double_t ylow, Double_t yup) : 
  TH2D(name, title, nbinsx, xlow, xup , nbinsy, ylow, yup),
  fStatErrorLow(fNcells),
  fStatErrorHigh(fNcells),
  fSysErrorLow(fNcells),
  fSysErrorHigh(fNcells)
{
  // Constructor.
}

TH2DAsymErr::TH2DAsymErr(const TH2D &h2d) : 
  TH2D(h2d),
  fStatErrorLow(fNcells),
  fStatErrorHigh(fNcells),
  fSysErrorLow(fNcells),
  fSysErrorHigh(fNcells)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
Double_t TH2DAsymErr::GetError(Double_t x, Double_t y, EErrType t)
{
  // Get error corresponding to x and y for given error type.

  Int_t nx = fXaxis.GetNbins()+2;
  Int_t binx = fXaxis.FindFixBin(x);
  Int_t biny = fYaxis.FindFixBin(y);
  Int_t bin = binx + nx*biny; 

  switch (t) {
    case kStatErrLow:
      return fStatErrorLow.fArray[bin];
      break;
    case kStatErrHigh:
      return fStatErrorHigh.fArray[bin];
      break;
    case kSysErrLow:
      return fSysErrorLow.fArray[bin];
      break;
    case kSysErrHigh:
      return fSysErrorHigh.fArray[bin];
      break;
  }
  return 0;
}

//--------------------------------------------------------------------------------------------------
Double_t TH2DAsymErr::GetBinError(Int_t b, Int_t c, EErrType t)
{
  // Get error corresponding to bin (b,c) for given error type.

  Int_t nx = fXaxis.GetNbins()+2;
  Int_t bin = b + nx*c;

  switch (t) {
    case kStatErrLow:
      return fStatErrorLow.fArray[bin];
      break;
    case kStatErrHigh:
      return fStatErrorHigh.fArray[bin];
      break;
    case kSysErrLow:
      return fSysErrorLow.fArray[bin];
      break;
    case kSysErrHigh:
      return fSysErrorHigh.fArray[bin];
      break;
  }
  return 0;
}


//--------------------------------------------------------------------------------------------------
void TH2DAsymErr::SetBinContent(Int_t binx, Int_t biny, Double_t value,
                                Double_t statErrorLow, Double_t statErrorHigh,
                                Double_t sysErrorLow, Double_t sysErrorHigh)
{
  // Set bin content for given bin and value.

  Int_t bin = GetBin(binx,biny);
  SetBinContent(bin,value);

  fStatErrorLow.fArray[bin]  = statErrorLow;
  fStatErrorHigh.fArray[bin] = statErrorHigh;
  fSysErrorLow.fArray[bin]   = sysErrorLow;
  fSysErrorHigh.fArray[bin]  = sysErrorHigh;
  return;
}

//--------------------------------------------------------------------------------------------------
void TH2DAsymErr::SetBinError(Int_t binx, Int_t biny,
                              Double_t statErrorLow, Double_t statErrorHigh,
                              Double_t sysErrorLow, Double_t sysErrorHigh)
{
  // Set bin error for asymetric errors.

  Int_t bin = GetBin(binx,biny);

  fStatErrorLow.fArray[bin] = statErrorLow;
  fStatErrorHigh.fArray[bin] = statErrorHigh;
  fSysErrorLow.fArray[bin] = sysErrorLow;
  fSysErrorHigh.fArray[bin] = sysErrorHigh;
  return;
}
