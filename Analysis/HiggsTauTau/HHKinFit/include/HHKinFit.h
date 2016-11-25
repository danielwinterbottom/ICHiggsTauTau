/*
 *HHKinFit.h
 *
 *  Created on: Jun 17, 2014
 */

#ifndef HHKINFIT_H_
#define HHKINFIT_H_

#include "HHV4Vector.h"
#include "HHEventRecord.h"
#include <map>
#include <utility>
#include <Rtypes.h>
#include <TH1F.h>

typedef std::map<std::pair<double, double>, double> Chi2Map;

class HHKinFit{
public:
  HHKinFit(HHEventRecord* recrecord);
  ~HHKinFit();

  void Fit();
  void ConstrainE2(Int_t iv4, Int_t iv41, Int_t iv42);
  Double_t Chi2V4(Int_t iv4);
  Double_t Chi2Balance();
  Chi2Map CreateChi2Map(int NbVariations, int NtauVariations);
  Chi2Map CreateChi2MapAroundStartvalues(int NbVariations, int NtauVariations);

  //Getters
  Double_t GetChi2();
  Double_t GetChi2_b1();
  Double_t GetChi2_b2();
  Double_t GetChi2_balance();

  Double_t GetConvergence();
  Double_t GetFittedMH();

  HHEventRecord* GetFitrecord();
  double GetPullE(Int_t iv4);
  double GetPullBalance();
  double GetPullBalanceX();
  double GetPullBalanceY();

  TLorentzVector GetFitParticle(Int_t iv4) { 
    HHV4Vector* fitobject = m_fitrecord->GetEntry(iv4);
    return TLorentzVector(fitobject->Px(), fitobject->Py(), fitobject->Pz(), fitobject->E());
  }
  
  //Setters
  void SetPrintLevel(Int_t level);
  void SetGraphicsLevel(Int_t level);
  void SetMaxLoops(Int_t loops);

  void SetAdvancedBalance(Bool_t flag);
  void SetLogLevel(Int_t level);

  bool m_fixedCovMatrix;
private:
  Double_t m_chi2;
  Double_t m_chi2_b1;
  Double_t m_chi2_b2;
  Double_t m_chi2_balance;
  Int_t m_convergence;
  Double_t m_fittedmH;
  TMatrixD m_covRecoil;
  Int_t m_printlevel;
  Int_t m_graphicslevel;

  Int_t m_maxloops;
  Bool_t m_advancedBalance;
  Bool_t m_keepMassesConst;
  Int_t m_logLevel;

  HHEventRecord* m_recrecord;
  HHEventRecord* m_fitrecord;
  HHParticleList* m_particlelist;

  //Just for Chi2Maps
  Double_t m_tauStartEnergy;
  Double_t m_bStartEnergy;

};



#endif /* HHKINFIT_H_ */
