#ifndef ICHiggsTauTau_HiggsTauTau_RhoIDEmbedder_h
#define ICHiggsTauTau_HiggsTauTau_RhoIDEmbedder_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/PFCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <string>
#include "Utilities/interface/FnRootTools.h"
#include "TMVA/Reader.h"

namespace ic {
  
class RhoIDEmbedder : public ModuleBase {
 private:
  CLASS_MEMBER(RhoIDEmbedder, fwlite::TFileService*, fs)
  CLASS_MEMBER(RhoIDEmbedder, ic::channel, channel)
  
  TTree *outtree_;
  int wt_;
  TMVA::Reader *reader_;

 public:
  RhoIDEmbedder(std::string const& name);
  virtual ~RhoIDEmbedder();
  virtual double read_mva_score(std::vector<double> vars);
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  

  float var0, var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11, var12, var13;
  

};

}

#endif
