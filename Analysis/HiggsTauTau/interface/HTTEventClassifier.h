#ifndef ICHiggsTauTau_HiggsTauTau_HTTEventClassifier_h
#define ICHiggsTauTau_HiggsTauTau_HTTEventClassifier_h

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
  
class HTTEventClassifier : public ModuleBase {
 private:
  CLASS_MEMBER(HTTEventClassifier, fwlite::TFileService*, fs)
  CLASS_MEMBER(HTTEventClassifier, ic::channel, channel)
  CLASS_MEMBER(HTTCategories, ic::era, era)
  CLASS_MEMBER(HTTCategories, std::string, ditau_label)
  CLASS_MEMBER(HTTCategories, std::string, met_label)
  CLASS_MEMBER(HTTCategories, std::string, jets_label)
 
  TMVA::Reader *reader_even_;
  TMVA::Reader *reader_odd_;

 public:
  HTTEventClassifier(std::string const& name);
  virtual ~HTTEventClassifier();
  virtual std::vector<float> read_mva_scores(unsigned isEven, std::vector<float> vars);
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  unsigned isEven_;
  float event_;
  unsigned long long evt_;

  float dijetpt;
  float jdeta;
  float jpt_1;
  float m_sv;
  float m_vis;
  float met;
  float mjj;
  float n_jets;
  float pt_1;
  float pt_2;
  float pt_tt;
  float pt_vis;


  float var0, var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11;

};

}

#endif
