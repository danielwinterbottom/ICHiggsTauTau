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
#include "boost/format.hpp"
#include "boost/bind.hpp"
#include "TMVA/Reader.h"

namespace ic {
  
class HTTEventClassifier : public ModuleBase {
 private:
  CLASS_MEMBER(HTTEventClassifier, fwlite::TFileService*, fs)
  CLASS_MEMBER(HTTEventClassifier, ic::channel, channel)
  CLASS_MEMBER(HTTEventClassifier, ic::era, era)
  CLASS_MEMBER(HTTEventClassifier, std::string, ditau_label)
  CLASS_MEMBER(HTTEventClassifier, std::string, met_label)
  CLASS_MEMBER(HTTEventClassifier, std::string, jets_label)
 
  TMVA::Reader *reader_even_;
  TMVA::Reader *reader_odd_;

 public:
  HTTEventClassifier(std::string const& name);
  virtual ~HTTEventClassifier();
  virtual std::vector<float> read_mva_scores(unsigned isEven, std::vector<float> vars);
  virtual std::pair<float,int> getMaxScoreWithIndex(std::vector<float> vec);
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  unsigned isEven_;
  float event_;
  unsigned long long evt_;

  double dijetpt_;
  double jdeta_;
  double jpt_1_;
  double m_sv_;
  double m_vis_;
  double met_;
  double mjj_;
  unsigned n_jets_;
  unsigned n_lowpt_jets_;
  double pt_1_;
  double pt_2_;
  double pt_tt_;
  double pt_vis_;


  float var0_, var1_, var2_, var3_, var4_, var5_, var6_, var7_, var8_, var9_, var10_, var11_;

};

}

#endif
