#ifndef HHTauInputKinFitMaster_H
#define HHTauInputKinFitMaster_H

#include <Rtypes.h>
#include <stdio.h>
#include <TMatrixD.h>
#include <map>
#include <utility>
#include <vector>

#include "TLorentzVector.h"

class HHTauInputKinFitMaster
{
public:
  HHTauInputKinFitMaster( TLorentzVector* bjet1, TLorentzVector* bjet2, TLorentzVector* tau1, TLorentzVector* tau2);

  void doFullFit();
    
  //Getters for fit results
  Double_t getBestChi2FullFit();
  Double_t getBestMHFullFit();
  std::pair< Int_t, Int_t > getBestHypoFullFit();
  std::map< std::pair< Int_t, Int_t >, Double_t > getChi2FullFit();
  std::map< std::pair< Int_t, Int_t >, Double_t > getFitProbFullFit();
  std::map< std::pair< Int_t, Int_t >, Double_t > getMHFullFit();
  std::map< std::pair< Int_t, Int_t >, Double_t > getPullB1FullFit();
  std::map< std::pair< Int_t, Int_t >, Double_t > getPullB2FullFit();
  std::map< std::pair< Int_t, Int_t >, Double_t > getPullTau1FullFit();
  std::map< std::pair< Int_t, Int_t >, Double_t > getPullTau2FullFit();
  std::map< std::pair< Int_t, Int_t >, Int_t > getConvergenceFullFit();

  //Hypotheses
  void addMh1Hypothesis(std::vector<Int_t> v);
  void addMh1Hypothesis(Double_t m1, Double_t m2=0, Double_t m3=0, Double_t m4=0, Double_t m5=0, Double_t m6=0, Double_t m7=0, Double_t m8=0, Double_t m9=0, Double_t m10=0);
  void addMh2Hypothesis(std::vector<Int_t> v);
  void addMh2Hypothesis(Double_t m1, Double_t m2=0, Double_t m3=0, Double_t m4=0, Double_t m5=0, Double_t m6=0, Double_t m7=0, Double_t m8=0, Double_t m9=0, Double_t m10=0);

  //Resolution  
  Double_t GetBjetResoultion(Double_t eta, Double_t et);
  Double_t GetTauResoultion(Double_t eta, Double_t et);

private:
  //hypotheses
  std::vector< Int_t > m_mh1;
  std::vector< Int_t > m_mh2;

  //input vectors
  TLorentzVector* m_bjet1;
  TLorentzVector* m_bjet2;
  TLorentzVector* m_tau1;
  TLorentzVector* m_tau2;

  //full event fit
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitResultChi2;
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitResultFitProb;
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitResultMH;
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitPullB1;
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitPullB2;
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitPullTau1;
  std::map< std::pair< Int_t, Int_t >, Double_t > m_fullFitPullTau2;
  std::map< std::pair< Int_t, Int_t >, Int_t > m_fullFitConvergence;

  Double_t m_bestChi2FullFit;
  Double_t m_bestMHFullFit;
  std::pair< Int_t, Int_t > m_bestHypoFullFit;
};

#endif
