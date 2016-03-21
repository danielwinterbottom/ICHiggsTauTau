#ifndef ICHiggsTauTau_Module_HTTPairStudy_h
#define ICHiggsTauTau_Module_HTTPairStudy_h

#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Utilities/interface/HistoSet.h"
#include "HiggsTauTau/interface/HTTConfig.h"
#include "HiggsTauTau/interface/HTTTriggerFilter.h"
#include "boost/range/algorithm_ext/erase.hpp"

#include <string>
#include <functional>


namespace ic {

bool ETLooseEle(Electron const* e);
bool ETLooseTau(Tau const* t);
bool ETTightEle(Electron const* e);
bool ETTightTau(Tau const* t);

bool MTLooseMuo(Muon const* m);
bool MTLooseTau(Tau const* t);
bool MTTightMuo(Muon const* m);
bool MTTightTau(Tau const* t);

bool EMLooseEle(Electron const* e);
bool EMLooseMuo(Muon const* m);
bool EMTightEle(Electron const* e);
bool EMTightMuo(Muon const* m);

bool TTLooseTau(Tau const* t);
bool TTTightTau(Tau const* t);


bool SortByOSThenSumPt(CompositeCandidate const* c1,
                       CompositeCandidate const* c2);
bool SuperSortET(CompositeCandidate const* c1, CompositeCandidate const* c2);
bool SuperSortMT(CompositeCandidate const* c1, CompositeCandidate const* c2);
bool SuperSortEM(CompositeCandidate const* c1, CompositeCandidate const* c2);
bool SuperSortTT(CompositeCandidate const* c1, CompositeCandidate const* c2);

template <class T, class U>
std::vector<CompositeCandidate*> & CompositeProducer(
    TreeEvent* event, std::vector<T*> const& vec_first,
    std::vector<U*> const& vec_second, std::string output) {
  std::vector<std::pair<T*, U*> > pairs = MakePairs(vec_first, vec_second);
  std::vector<CompositeCandidate> vec_out;
  std::vector<CompositeCandidate*> ptr_vec_out;
  for (unsigned i = 0; i < pairs.size(); ++i) {
    vec_out.push_back(CompositeCandidate());
    CompositeCandidate& cand_ref = vec_out.back();
    cand_ref.AddCandidate("leg0", pairs[i].first);
    cand_ref.AddCandidate("leg1", pairs[i].second);
  }
  event->Add(output + "Product", vec_out);
  std::vector<CompositeCandidate>& vec_in =
      event->Get<std::vector<CompositeCandidate> >(output + "Product");
  ptr_vec_out.resize(vec_in.size());
  for (unsigned i = 0; i < vec_in.size(); ++i) {
    ptr_vec_out[i] = &(vec_in[i]);
  }
  event->Add(output, ptr_vec_out);
  return event->GetPtrVec<CompositeCandidate>(output);
}

class HTTPairStudy : public ModuleBase {
 private:
  CLASS_MEMBER(HTTPairStudy, ic::channel, channel)
  CLASS_MEMBER(HTTPairStudy, bool, do_loose)
  CLASS_MEMBER(HTTPairStudy, bool, do_gen_evt)
  CLASS_MEMBER(HTTPairStudy, fwlite::TFileService*, fs)

 private:
  unsigned loose_pairs;
  unsigned selected_pairs;
  unsigned selected_matched_pairs;
  unsigned selected_anymatched_pairs;

  HTTTriggerFilter trig_filter_;

  TH1F *h_mtt_vis;



 public:
  HTTPairStudy(std::string const& name);
  virtual ~HTTPairStudy();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#endif
