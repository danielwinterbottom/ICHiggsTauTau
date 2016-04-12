#ifndef ICHiggsTauTau_Module_VBFPlots_h
#define ICHiggsTauTau_Module_VBFPlots_h

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

#include "Modules/interface/L1Cuts.h"


namespace ic {
    
class VBFPlots : public ModuleBase {
   public:
      VBFPlots(std::string const& name, fwlite::TFileService *fs, std::string output_name);
      virtual ~VBFPlots(); 
      
      virtual int PreAnalysis();
      virtual int Execute(TreeEvent *event);
      virtual int PostAnalysis();
      virtual void PrintInfo();

   private:
       
      unsigned n_genjets_;
      unsigned n_jets_;
      unsigned n_genParticles_;
      unsigned n_l1jets_;
      unsigned EventCount;
      unsigned n2quarks;
      unsigned ngluons;
      
      TH1D *h_q1_Pt;
      TH1D *h_q2_Pt;
      TH1D *h_qq_Mqq;
      TH1D *h_qq_DeltaEta;
      TH1D *h_genj1_Pt;
      TH1D *h_genj2_Pt;
      TH1D *h_genjj_Mqq;
      TH1D *h_genjj_DeltaEta;
      TH1D *h_j1_Pt;
      TH1D *h_j2_Pt;
      TH1D *h_jj_Mqq;
      TH1D *h_jj_DeltaEta;
      TH1D *h_l1j1_Pt;
      TH1D *h_l1j2_Pt;
      TH1D *h_l1jj_Mqq;
      TH1D *h_l1jj_DeltaEta;
      

};
}

#endif
