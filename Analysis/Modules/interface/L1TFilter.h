#ifndef ICHiggsTauTau_Module_L1TFilter_h
#define ICHiggsTauTau_Module_L1TFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include <string>
#include <cmath>

#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TDirectory.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"

// ICHiggsTauTau Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TSum.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

struct L1Cuts {
  double EGPt = 0.;
  double MuPt = 0.;
  double Tau1Pt = 0.;
  double Tau2Pt = 0.;
  bool IsoEG = false;
  bool IsoTau = false;
  double Jet1Pt = 0.;
  double Jet2Pt = 0.;
  double Mjj = 0.;
  double DeltaEta = 0.;
};

namespace ic {
    
class L1TFilter : public ModuleBase {
   public:
      L1TFilter(std::string const& name, std::string channel, fwlite::TFileService *fs, L1Cuts l1Cuts);
      virtual ~L1TFilter(); 
      
      virtual int PreAnalysis();
      virtual int Execute(TreeEvent *event);
      virtual int PostAnalysis();
      virtual void PrintInfo();

   private:
      std::string channel_;
      std::string l1jets_label_;
      std::string l1electrons_label_;
      std::string l1muons_label_;
      std::string l1taus_label_;
      std::string l1met_label_;
    
      unsigned n_l1jets_;
      unsigned n_l1electrons_;
      unsigned n_l1muons_;
      unsigned n_l1taus_;
      
      double Jet1PtCut;
      double Jet2PtCut;
      double DeltaEtaCut;
      double MjjCut;
      double EGPtCut;
      double Tau1PtCut;
      double Tau2PtCut;
      double MuPtCut;
      
      bool IsoTauFilter;
      bool IsoEGFilter;
      
      TH1D *h_L1Filter_Efficiency;

};
}

#endif
