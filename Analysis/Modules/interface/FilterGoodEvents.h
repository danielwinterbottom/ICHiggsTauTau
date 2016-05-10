#ifndef ICHiggsTauTau_Module_FilterGoodEvents_h
#define ICHiggsTauTau_Module_FilterGoodEvents_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include <string>
#include <cmath>

#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TDirectory.h"


namespace ic {
    
class FilterGoodEvents : public ModuleBase {
   public:
      FilterGoodEvents(std::string const& name, std::string channel);
      virtual ~FilterGoodEvents(); 
      
      virtual int PreAnalysis();
      virtual int Execute(TreeEvent *event);
      virtual int PostAnalysis();
      virtual void PrintInfo();

   private:
      std::string channel_;
      unsigned n_genParticles_;    
      unsigned n_genjets_;
};
}

#endif
