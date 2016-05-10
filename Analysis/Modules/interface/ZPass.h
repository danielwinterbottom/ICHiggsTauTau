#ifndef ICHiggsTauTau_Module_ZPass_h
#define ICHiggsTauTau_Module_ZPass_h

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
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
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
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"

#include "Modules/interface/L1Cuts.h"

namespace ic {
    
class ZPass : public ModuleBase {
   public:
      ZPass(std::string const& name, std::string channel, fwlite::TFileService *fs, L1Cuts l1Cuts, std::string L1MuTag, std::string subDirName);
      virtual ~ZPass(); 
      
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
      bool IsoMuFilter;
      bool JetFilter;
      unsigned isZB;
      
      DynamicHistoSet * hists_;
      TFileDirectory subDir;
      
      TH1D *h_ZPass1;
      TH1D *h_ZPass2;
      TH1D *h_ZPass3;
      TH1D *h_ZPass4;
      TH1D *h_ZPass5;
      TH1D *h_ZPass6;
      TH1D *h_ZPass7;
      TH1D *h_ZPass8;
      TH1D *h_ZPass9;
      TH1D *h_ZPass10;
      TH1D *h_ZPass11;
      TH1D *h_ZPass12;
      TH1D *h_ZPass13;
      TH1D *h_ZPass14;
      TH1D *h_ZPass15;
      

};
}

#endif
