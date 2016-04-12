#ifndef ICHiggsTauTau_Module_L1Cuts_h
#define ICHiggsTauTau_Module_L1Cuts_h

struct L1Cuts {
  double EGPt = 0.;
  double MuPt = 0.;
  double Tau1Pt = 0.;
  double Tau2Pt = 0.;
  bool IsoEG = false;
  bool IsoMu = false;
  bool IsoTau = false;
  double Jet1Pt = 0.;
  double Jet2Pt = 0.;
  double Mjj = 0.;
  double DeltaEta = 0.;
  bool JetFilter = false;
};


#endif