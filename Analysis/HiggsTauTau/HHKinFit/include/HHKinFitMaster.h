#ifndef HHKinFitMaster_H
#define HHKinFitMaster_H

#include <Rtypes.h>
#include <stdio.h>
#include <TMatrixD.h>
#include <TGraph2D.h>
#include <TCanvas.h>
#include <map>
#include <utility>
#include <vector>
#include <sstream>

#include "TLorentzVector.h"

typedef std::map<std::pair<double, double>, double> Chi2Map;

class HHKinFitMaster
{
public:
  HHKinFitMaster( TLorentzVector* bjet1, TLorentzVector* bjet2, TLorentzVector* tauvis1, TLorentzVector* tauvis2, Bool_t truthinput=0, TLorentzVector* heavyhiggsgen=NULL);

  void doFullFit();
  
  //Setters
  void setAdvancedBalance(TLorentzVector* met, TMatrixD met_cov);
  void setSimpleBalance(Double_t balancePt, Double_t balanceUncert);
  
  //Getters for fit results
  Double_t getBestChi2FullFit();
  Double_t getBestMHFullFit();
  std::pair< Int_t, Int_t > getBestHypoFullFit();
  std::map< std::pair< Int_t, Int_t >, Double_t > getChi2FullFit();
  std::map< std::pair< Int_t, Int_t >, Double_t > getFitProbFullFit();
  std::map< std::pair< Int_t, Int_t >, Double_t > getMHFullFit();
  std::map< std::pair< Int_t, Int_t >, Double_t > getPullB1FullFit();
  std::map< std::pair< Int_t, Int_t >, Double_t > getPullB2FullFit();
  std::map< std::pair< Int_t, Int_t >, Double_t > getPullBalanceFullFit();
  std::map< std::pair< Int_t, Int_t >, Double_t > getPullBalanceFullFitX();
  std::map< std::pair< Int_t, Int_t >, Double_t > getPullBalanceFullFitY();
  std::map< std::pair< Int_t, Int_t >, Int_t > getConvergenceFullFit();

  std::map< std::pair< Int_t, Int_t >, Double_t > getChi2B1FullFit(){return m_fullFitResultChi2BJet1;}
  std::map< std::pair< Int_t, Int_t >, Double_t > getChi2B2FullFit(){return m_fullFitResultChi2BJet2;}
  std::map< std::pair< Int_t, Int_t >, Double_t > getChi2BalanceFullFit(){return m_fullFitResultChi2Balance;}
  //Hypotheses
  void addMh1Hypothesis(std::vector<Int_t> v);
  void addMh1Hypothesis(Double_t m1, Double_t m2=0, Double_t m3=0, Double_t m4=0, Double_t m5=0, Double_t m6=0, Double_t m7=0, Double_t m8=0, Double_t m9=0, Double_t m10=0);
  void addMh2Hypothesis(std::vector<Int_t> v);
  void addMh2Hypothesis(Double_t m1, Double_t m2=0, Double_t m3=0, Double_t m4=0, Double_t m5=0, Double_t m6=0, Double_t m7=0, Double_t m8=0, Double_t m9=0, Double_t m10=0);

  //Resolution  
  Double_t GetBjetResolution(Double_t eta, Double_t et);

  TLorentzVector m_bjet1_fitted;
  TLorentzVector m_bjet2_fitted;
  TLorentzVector m_tau1_fitted;
  TLorentzVector m_tau2_fitted;
  TLorentzVector m_bjet1_smeared;
  TLorentzVector m_bjet2_smeared;
  TLorentzVector m_met_smeared; 
  double m_bjet1Smear;
  double m_bjet2Smear;
  bool m_fixedCovMatrix;
private:
  //hypotheses
  std::vector< Int_t > m_mh1;
  std::vector< Int_t > m_mh2;

  //input vectors
  TLorentzVector* m_bjet1;
  TLorentzVector* m_bjet2;
  TLorentzVector* m_tauvis1;
  TLorentzVector* m_tauvis2;

  TLorentzVector* m_MET;
  TMatrixD m_MET_COV;

  //full event fit
  Bool_t m_truthInput;
  Bool_t m_advancedBalance;
  Double_t m_simpleBalancePt;
  Double_t m_simpleBalanceUncert;
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitResultChi2;
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitResultChi2BJet1;
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitResultChi2BJet2;
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitResultChi2Balance;
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitResultFitProb;
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitResultMH;
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitPullB1;
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitPullB2;
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitPullBalance;
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitPullBalanceX;
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitPullBalanceY;
  std::map< std::pair< Int_t, Int_t >, Int_t > m_fullFitConvergence;

  Double_t m_bestChi2FullFit;
  Double_t m_bestMHFullFit;
  std::pair< Int_t, Int_t > m_bestHypoFullFit;
};

#endif
