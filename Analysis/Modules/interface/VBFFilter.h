#ifndef ICHiggsTauTau_Module_VBFFilter_h
#define ICHiggsTauTau_Module_VBFFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>
#include <cmath>

#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TDirectory.h"

struct OfflineCuts {
  double ElPt = 0.;
  double MuPt = 0.;
  double Tau1Pt = 0.;
  double Tau2Pt = 0.;
  double Mjj = 0.;
  double Jet1Pt = 0.;
  double Jet2Pt = 0.;
  double DeltaEta = 0.;
};

namespace ic {
    
class VBFFilter : public ModuleBase {
   public:
      VBFFilter(std::string const& name, OfflineCuts offlineCuts);
      virtual ~VBFFilter(); 
      
      virtual int PreAnalysis();
      virtual int Execute(TreeEvent *event);
      virtual int PostAnalysis();
      virtual void PrintInfo();

   private:
      std::string jets_label_;

      unsigned n_jets_;
      
      double JetPtCut;
      double EtaCut;
      double DeltaEtaCut;
      double MjjCut;

};
}

#endif
