#ifndef ICHiggsTauTau_Module_CompositeProducer_h
#define ICHiggsTauTau_Module_CompositeProducer_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/interface/Objects.hh"
#include <string>

namespace ic {

template <class T, class U>
class CompositeProducer : public ModuleBase {
 private:
  std::string input_label_first_;
  std::string input_label_second_;
  std::string candidate_name_first_;
  std::string candidate_name_second_;
  std::string output_label_;

 public:
  CompositeProducer(std::string const& name);
  virtual ~CompositeProducer();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  
  CompositeProducer<T, U> & set_input_label_first(std::string const& input_label_first) {
    input_label_first_ = input_label_first;
    return *this;
  }

  CompositeProducer<T, U> & set_input_label_second(std::string const& input_label_second) {
    input_label_second_ = input_label_second;
    return *this;
  }

  CompositeProducer<T, U> & set_candidate_name_first(std::string const& candidate_name_first) {
    candidate_name_first_ = candidate_name_first;
    return *this;
  }

  CompositeProducer<T, U> & set_candidate_name_second(std::string const& candidate_name_second) {
    candidate_name_second_ = candidate_name_second;
    return *this;
  }

  CompositeProducer<T, U> & set_output_label(std::string const& output_label) {
    output_label_ = output_label;
    return *this;
  }
};

template <class T, class U>
CompositeProducer<T, U>::CompositeProducer(std::string const& name) : ModuleBase(name) {
}

template <class T, class U>
CompositeProducer<T, U>::~CompositeProducer() {
  ;
}

template <class T, class U>
int CompositeProducer<T, U>::PreAnalysis() {
  return 0;
}

template <class T, class U>
int CompositeProducer<T, U>::Execute(TreeEvent *event) {
  std::vector<T *> const& vec_first = event->GetPtrVec<T>(input_label_first_);
  std::vector<U *> const& vec_second = event->GetPtrVec<U>(input_label_second_);
  std::vector< std::pair<T*,U*> > pairs = MakePairs(vec_first, vec_second);
  std::vector<CompositeCandidate> vec_out;
  std::vector<CompositeCandidate *> ptr_vec_out;
  for (unsigned i = 0; i < pairs.size(); ++i) {
    vec_out.push_back(CompositeCandidate());
    CompositeCandidate & cand_ref = vec_out.back();
    cand_ref.AddCandidate(candidate_name_first_, pairs[i].first);
    cand_ref.AddCandidate(candidate_name_second_, pairs[i].second);
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

template <class T, class U>
int CompositeProducer<T, U>::PostAnalysis() {
  return 0;
}

template <class T, class U>
void CompositeProducer<T, U>::PrintInfo() {
  ;
}

}

#endif
