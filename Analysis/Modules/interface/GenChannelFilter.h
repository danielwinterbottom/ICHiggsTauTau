#ifndef ICHiggsTauTau_Module_GenChannelFilter_h
#define ICHiggsTauTau_Module_GenChannelFilter_h

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


namespace ic {
    
class GenChannelFilter : public ModuleBase {
   public:
      GenChannelFilter(std::string const& name, std::string channel);
      virtual ~GenChannelFilter(); 
      
      virtual int PreAnalysis();
      virtual int Execute(TreeEvent *event);
      virtual int PostAnalysis();
      virtual void PrintInfo();

   private:
      std::string genParticles_label_; 
      std::string channel_;

      unsigned n_genParticles_;    

};
}

#endif
