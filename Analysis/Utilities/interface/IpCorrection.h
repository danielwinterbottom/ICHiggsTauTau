#ifndef IpCorrection_h
#define IpCorrection_h

#include <TFile.h>
#include <TH1.h>
#include <TF1.h>
#include <TString.h>
#include <TRandom.h>
#include <TMath.h>
#include <assert.h>
#include "TVector3.h"
#include <Math/SMatrix.h>

typedef ROOT::Math::SMatrix<double,3,3, ROOT::Math::MatRepStd< double, 3, 3 >> CovMatrix;

class IpCorrection {

 public:
  IpCorrection();
  ~IpCorrection();
  void Init(TString fileName);
  double correctIp(int coordinate, double ip, double eta);
  double correctIp(int coordinate, double ip, double ipgen, double eta);
  TVector3 correctIp(TVector3 ip, TVector3 ipgen, double eta);
  CovMatrix correctIpCov(CovMatrix ipCov, double eta);
  enum Coordinate{Ipx=0, Ipy=1, Ipz=2};

 private:
  int binNumber(double x, const std::vector<double> bins) const
  {
    for (size_t iB=0; iB<bins.size(); ++iB)
      if (x>=bins[iB]&&x<bins[iB+1])
	return iB;
    return 0;
  }

  int binNumber(double x, int nbins, const double * bins) {
    int binN = 0;
    for (int iB=0; iB<nbins; ++iB) {
      if (x>=bins[iB]&&x<bins[iB+1]) {
	binN = iB;
	break;
      }
    }    
    return binN;
  }

  TFile * file = nullptr;
  TH1D * histIpData[3][4];
  TH1D * histIpMC[3][4];

  TH1D * histErrData[3][4];
  TH1D * histErrMC[3][4];


  std::vector<TString> IpNames = {"ipx","ipy","ipz"};
  std::vector<TString> IpErrNames = {"ipxErr","ipyErr","ipzErr"};
  std::vector<TString> EtaNames;
  int nEtaBins;
  std::vector<double>  EtaRanges;

  double applyQuantileMapping(TH1D * histMC, TH1D * histData, double var);

};

#endif

