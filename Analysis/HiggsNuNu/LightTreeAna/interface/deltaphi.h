////////////////////////////////////////////////////////////////////////////////
//
// define deltaPhi & deltaR
// ------------------------
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DELTAPHI_H
#define DELTAPHI_H 1


#include <TMath.h>
#include <cmath>


namespace mymath {
  double deltaPhi(double phi1,double phi2);
  double deltaR(double eta1,double phi1,double eta2,double phi2);
  double deltaRmin(double eta1,double phi1,double eta2,double phi2,double eta3,double phi3);
}


#endif
