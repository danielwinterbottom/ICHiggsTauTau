#include "../interface/deltaphi.h"

namespace mymath {
  double deltaPhi(double phi1,double phi2)
  {
    double result = phi1 - phi2;
    while (result > TMath::Pi()) result -= 2*TMath::Pi();
    while (result <= -TMath::Pi()) result += 2*TMath::Pi();
    return result;
  }
  double deltaR(double eta1,double phi1,double eta2,double phi2)
  {
    double deta = eta1 - eta2;
    double dphi = deltaPhi(phi1, phi2);
    return std::sqrt(deta*deta + dphi*dphi);
  }
  double deltaRmin(double eta1,double phi1,double eta2,double phi2,double eta3,double phi3)
  {
    return std::min(deltaR(eta1,phi1,eta3,phi3),deltaR(eta2,phi2,eta3,phi3));
  }

}
