#ifndef ICHiggsTauTau_Module_OneCollCompositeProducer_h
#define ICHiggsTauTau_Module_OneCollCompositeProducer_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/interface/Objects.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {

template <class T>
class OneCollCompositeProducer : public ModuleBase {
 private:
  std::string input_label_;
  std::string candidate_name_first_;
  std::string candidate_name_second_;
  bool select_leading_pair_;
  std::string output_label_;

 public:
  OneCollCompositeProducer(std::string const& name);
  virtual ~OneCollCompositeProducer();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  
  OneCollCompositeProducer<T> & set_input_label(std::string const& input_label) {
    input_label_ = input_label;
    return *this;
  }

  OneCollCompositeProducer<T> & set_candidate_name_first(std::string const& candidate_name_first) {
    candidate_name_first_ = candidate_name_first;
    return *this;
  }

  OneCollCompositeProducer<T> & set_candidate_name_second(std::string const& candidate_name_second) {
    candidate_name_second_ = candidate_name_second;
    return *this;
  }


  OneCollCompositeProducer<T> & set_select_leading_pair(bool const& select_leading_pair) {
    select_leading_pair_ = select_leading_pair;
    return *this;
  }

  OneCollCompositeProducer<T> & set_output_label(std::string const& output_label) {
    output_label_ = output_label;
    return *this;
  }

};

template <class T>
OneCollCompositeProducer<T>::OneCollCompositeProducer(std::string const& name) : ModuleBase(name) {
  select_leading_pair_=false;
}

template <class T>
OneCollCompositeProducer<T>::~OneCollCompositeProducer() {
  ;
}

template <class T>
int OneCollCompositeProducer<T>::PreAnalysis() {
  return 0;
}

template <class T>
int OneCollCompositeProducer<T>::Execute(TreeEvent *event) {
  std::vector<T *> & vec_first = event->GetPtrVec<T>(input_label_);
  if (select_leading_pair_) std::sort(vec_first.begin(), vec_first.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
  std::vector< std::pair<T*,T*> > pairs = MakePairs(vec_first);
  std::vector<CompositeCandidate> vec_out;
  std::vector<CompositeCandidate *> ptr_vec_out;
  if (select_leading_pair_ && pairs.size()>0){
    vec_out.push_back(CompositeCandidate());
    CompositeCandidate & cand_ref = vec_out.back();
    cand_ref.AddCandidate(candidate_name_first_, pairs[0].first);
    cand_ref.AddCandidate(candidate_name_second_, pairs[0].second);
  }
  else {
    for (unsigned i = 0; i < pairs.size(); ++i) {
      vec_out.push_back(CompositeCandidate());
      CompositeCandidate & cand_ref = vec_out.back();
      cand_ref.AddCandidate(candidate_name_first_, pairs[i].first);
      cand_ref.AddCandidate(candidate_name_second_, pairs[i].second);
    }
  }
  event->Add(output_label_+"Product", vec_out);
  std::vector<CompositeCandidate> & vec_in = event->Get<std::vector<CompositeCandidate> >(output_label_+"Product");
  ptr_vec_out.resize(vec_in.size());
  for (unsigned i = 0; i < vec_in.size(); ++i) {
    ptr_vec_out[i] = &(vec_in[i]);
  }
  event->Add(output_label_, ptr_vec_out);
  return 0;
 }

template <class T>
int OneCollCompositeProducer<T>::PostAnalysis() {
  return 0;
}

template <class T>
void OneCollCompositeProducer<T>::PrintInfo() {
  ;
}

}

#endif
