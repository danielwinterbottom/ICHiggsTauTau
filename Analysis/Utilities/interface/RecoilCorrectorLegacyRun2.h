#ifndef HTT_RecoilCorrectorLegacyRun2_h
#define HTT_RecoilCorrectorLegacyRun2_h

#include <TFile.h>
#include <TH1.h>
#include <TF1.h>
#include <TString.h>
#include <TRandom.h>
#include <TMath.h>
#include <assert.h>
#include "TVector2.h"
#include "TVector.h"
#include "Math/Vector2D.h"
#include "Math/VectorUtil.h"

class RecoilCorrectorLegacyRun2 {
  
 public:
  RecoilCorrectorLegacyRun2(TString fileName);
  ~RecoilCorrectorLegacyRun2();

  void Correct(float MetPx,
           float MetPy,
           float genZPx, 
           float genZPy,
           float diLepPx,
           float diLepPy,
           int njets,
           float & MetCorrPx,
           float & MetCorrPy);

  void CorrectWithHist(float MetPx,
           float MetPy,
           float genZPx, 
           float genZPy,
           float diLepPx,
           float diLepPy,
           int njets,
           float & MetCorrPx,
           float & MetCorrPy);

  void CorrectByMeanResolution(float MetPx,
                   float MetPy,
                   float genZPx, 
                   float genZPy,
                   float diLepPx,
                   float diLepPy,
                   int njets,
                   float & MetCorrPx,
                   float & MetCorrPy);

  
 private:

  int binNumber(float x, const std::vector<float> bins) const
  {
    for (size_t iB=0; iB<bins.size(); ++iB)
      if (x>=bins[iB]&&x<bins[iB+1])
    return iB;
    return 0;
  }

  int binNumber(float x, int nbins, const float * bins) {

    int binN = 0;

    for (int iB=0; iB<nbins; ++iB) {
      if (x>=bins[iB]&&x<bins[iB+1]) {
    binN = iB;
    break;
      }
    }
    
    return binN;

  }

  TString _fileName;

  void InitMEtWeights(TFile * file,
              TString  _perpZStr,
              TString  _paralZStr,
              int nZPtBins,
              float * ZPtBins,
              TString * _ZPtStr,
              int nJetsBins,
              TString * _nJetsStr);

  void InitMEtWeights(TFile * file,
              const std::vector<float>& ZPtBins,
              const std::string _perpZStr,
              const std::string _paralZStr,
              const std::vector<std::string>& _ZPtStr,
              const std::vector<std::string>& _nJetsStr);

  void CalculateU1U2FromMet(float MetPx,
                float MetPy,
                float genZPx,
                float genZPy,
                float diLepPx,
                float diLepPy,
                Double_t & U1,
                Double_t & U2,
                Double_t & metU1,
                Double_t & metU2);

  void CalculateMetFromU1U2(float U1,
                float U2,
                float genZPx,
                float genZPy,
                float diLepPx,
                float diLepPy,
                float & metPx,
                float & metPy);

  void  U1U2CorrectionsByWidth(Double_t & U1, Double_t & U2,
                   int nZptBin,
                   int njets);

  float CorrectionsBySampling(float x, TF1 * funcMC, TF1 * funcData);

  float rescale(float x,
        float meanData, 
        float meanMC,
        float resolutionData,
        float resolutionMC);
    

  //float * _ZPtBins;
  std::vector<float> _ZPtBins;

  Double_t     _epsrel;
  Double_t     _epsabs;
  Double_t       _error;
  float _range;

  int _nZPtBins;
  int _nJetsBins;

  TF1 * _metZParalData[10][3];
  TF1 * _metZPerpData[10][3];
  TF1 * _metZParalMC[10][3];
  TF1 * _metZPerpMC[10][3];

  TH1D * _metZParalDataHist[10][3];
  TH1D * _metZPerpDataHist[10][3];
  TH1D * _metZParalMCHist[10][3];
  TH1D * _metZPerpMCHist[10][3];

  float _meanMetZParalData[10][3];
  float _meanMetZParalMC[10][3];
  float _meanMetZPerpData[10][3];
  float _meanMetZPerpMC[10][3];
  
  float _rmsMetZParalData[10][3];
  float _rmsLeftMetZParalData[10][3];
  float _rmsRightMetZParalData[10][3];
  
  float _rmsMetZParalMC[10][3];
  float _rmsLeftMetZParalMC[10][3];
  float _rmsRightMetZParalMC[10][3];

  float _rmsMetZPerpData[10][3];
  float _rmsLeftMetZPerpData[10][3];
  float _rmsRightMetZPerpData[10][3];
  
  float _rmsMetZPerpMC[10][3];
  float _rmsLeftMetZPerpMC[10][3];
  float _rmsRightMetZPerpMC[10][3];
  
  float _xminMetZPerp[10][3];
  float _xmaxMetZPerp[10][3];

  float _xminMetZPerpData[10][3];
  float _xmaxMetZPerpData[10][3];

  float _xminMetZPerpMC[10][3];
  float _xmaxMetZPerpMC[10][3];

  float _xminMetZParal[10][3];
  float _xmaxMetZParal[10][3];

  float _xminMetZParalData[10][3];
  float _xmaxMetZParalData[10][3];

  float _xminMetZParalMC[10][3];
  float _xmaxMetZParalMC[10][3];

};

#endif
