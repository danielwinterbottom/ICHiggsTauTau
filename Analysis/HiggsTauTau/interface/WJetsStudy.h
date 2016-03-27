#ifndef ICHiggsTauTau_Module_WJetsStudy_h
#define ICHiggsTauTau_Module_WJetsStudy_h

#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Utilities/interface/HistoSet.h"
#include "HiggsTauTau/interface/HTTConfig.h"
#include "HiggsTauTau/interface/HTTTriggerFilter.h"
#include "boost/range/algorithm_ext/erase.hpp"
#include "boost/lexical_cast.hpp"

#include <string>
#include <functional>

struct CandidateTreeVars {
  float pt = 0.;
  float eta = 0.;
  float phi = 0.;
  float m = 0.;
  int q = 0;

  virtual void SetTree(TTree *t, TString pre, TString post) {
    t->Branch(pre+"pt"+post, &pt);
    t->Branch(pre+"eta"+post, &eta);
    t->Branch(pre+"phi"+post, &phi);
    t->Branch(pre+"m"+post, &m);
    t->Branch(pre+"q"+post, &q);
  }

  virtual void SetVals(ic::Candidate const* c) {
    pt = c->pt();
    eta = c->eta();
    phi = c->phi();
    m = c->M();
    q = c->charge();
  }
};

struct WeightSetVars {
  std::vector<float> wts_;
  std::vector<std::string> strs_;
  unsigned start_ = 0;
  unsigned end_ = 0;

  virtual void SetTree(TTree *t, unsigned start, unsigned end, TString pre,
                       TString post) {
    if (start > end) return;
    start_ = start;
    end_ = end;
    wts_.resize((end_ - start_) + 1);
    strs_.resize((end_ - start_) + 1);
    for (unsigned i = 0; i < wts_.size(); ++i) {
      t->Branch(pre+boost::lexical_cast<std::string>(i)+post, &(wts_[i]));
      strs_[i] = boost::lexical_cast<std::string>(i+start_);
    };
  }

  virtual void SetVals(ic::EventInfo const* info) {
    for (unsigned i = 0; i < wts_.size(); ++i) {
      wts_[i] = info->weight(strs_[i]);
    }
  }
};


namespace ic {

class WJetsStudy : public ModuleBase {
 private:
  // CLASS_MEMBER(WJetsStudy, ic::channel, channel)
  CLASS_MEMBER(WJetsStudy, fwlite::TFileService*, fs)
  CLASS_MEMBER(WJetsStudy, std::string, genparticle_label)
  CLASS_MEMBER(WJetsStudy, std::string, sample_name)

  TTree * tree_;
  CandidateTreeVars lepton_;
  WeightSetVars scale_wts_;
  WeightSetVars pdf_wts_;
  float mt_1_ = 0;
  float wt_ = 1.0;
  unsigned n_jets_ = 0;
  bool do_lhe_weights_ = true;


 public:
  WJetsStudy(std::string const& name);
  virtual ~WJetsStudy();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
