#ifndef ICHiggsTauTau_HiggsHTohh_JetTauFakeRate_h
#define ICHiggsTauTau_HiggsHTohh_JetTauFakeRate_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"

#include <string>

namespace ic {

class JetTauFakeRate : public ModuleBase {
 private:
  
  CLASS_MEMBER(JetTauFakeRate, bool, write_tree)
  CLASS_MEMBER(JetTauFakeRate, bool, write_plots)
  CLASS_MEMBER(JetTauFakeRate, fwlite::TFileService*, fs)
  
  TTree *outtree_;
	TH1F* jetpt_;
	TH1F* jeteta_;
	TH1F* jetflavour_;
	TH1F* jetphi_;
	TH1F* taupt_;
	TH1F* taueta_;
	TH1F* tauphi_;
	TH1F* loose_taupt_;
	TH1F* loose_taueta_;
	TH1F* loose_tauphi_;
	TH1F* medium_taupt_;
	TH1F* medium_taueta_;
	TH1F* medium_tauphi_;
	TH1F* tight_taupt_;
	TH1F* tight_taueta_;
	TH1F* tight_tauphi_;
	TH1F* deltar_;
	TH1F* loose_deltar_;
	TH1F* medium_deltar_;
	TH1F* tight_deltar_;
  
  double jpt_;     
  double jeta_;     
	double deltaR_;
	double deltaR_calc_;
 
 public:
  JetTauFakeRate(std::string const& name);
  virtual ~JetTauFakeRate();
  
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
	double GetDeltaR(Tau *tau_sig,std::vector<PFJet*> jet_vector);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}


#endif
