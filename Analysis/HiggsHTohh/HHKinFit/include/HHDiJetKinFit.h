/*
 * HHDiJetKinFit.h
 *
 */

#ifndef HHKinFit_HHDIJETKINFIT_H_
#define HHKinFit_HHDIJETKINFIT_H_

#include "../include/HHV4Vector.h"
#include "../include/HHEventRecord.h"
#include <Rtypes.h>
#include <TH1F.h>

class HHDiJetKinFit{
public:
  HHDiJetKinFit(HHEventRecord* recrecord);
  ~HHDiJetKinFit();

  
  
  void FitNew();
  void Fit();
  void ConstrainE2(Int_t iv4, Int_t iv41, Int_t iv42);
  Double_t Chi2V4(Int_t iv4);

  //Getters
  Double_t GetChi2();
  Double_t GetChi2_b1();
  Double_t GetChi2_b2();

  Double_t GetConvergence();

  HHEventRecord* GetFitrecord();

  TLorentzVector GetFitJet1() { return GetFitJet(HHEventRecord::b1); }
  double GetPullE1() { return GetPullE(HHEventRecord::b1); }
  TLorentzVector GetFitJet2() { return GetFitJet(HHEventRecord::b2); }
  double GetPullE2() { return GetPullE(HHEventRecord::b2); }

  //Setters
  void SetPrintLevel(Int_t level);
  void SetGraphicsLevel(Int_t level);
  void SetMaxLoops(Int_t loops);

  void SetAdvancedBalance(Bool_t flag);
  void SetLogLevel(Int_t level);

  friend class HHDiJetKinFitMaster;
  
private:
  TLorentzVector GetFitJet(Int_t iv4) {
    HHV4Vector* fitobject = m_fitrecord->GetEntry(iv4);
    return TLorentzVector(fitobject->Px(), fitobject->Py(), fitobject->Pz(), fitobject->E());
  }
  double GetPullE(Int_t iv4);

  HHParticleList* m_particlelist;
  HHEventRecord* m_recrecord;
  HHEventRecord* m_fitrecord;

  Int_t m_logLevel;

  Double_t m_chi2;
  Double_t m_chi2_b1;
  Double_t m_chi2_b2;
  Int_t m_convergence;

  Int_t m_printlevel;
  Int_t m_graphicslevel;
  Int_t m_maxloops;
};



#endif /* HHDIJETKINFIT_H_ */
