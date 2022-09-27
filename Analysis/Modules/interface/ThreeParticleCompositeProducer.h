#ifndef ICHiggsTauTau_Module_ThreeParticleCompositeProducer_h
#define ICHiggsTauTau_Module_ThreeParticleCompositeProducer_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/interface/Objects.hh"
#include <string>

namespace ic {

template <class T, class U, class V>
class ThreeParticleCompositeProducer : public ModuleBase {
 private:
  std::string input_label_first_;
  std::string input_label_second_;
  std::string input_label_third_;
  std::string candidate_name_first_;
  std::string candidate_name_second_;
  std::string candidate_name_third_;
  std::string output_label_;

 public:
  ThreeParticleCompositeProducer(std::string const& name);
  virtual ~ThreeParticleCompositeProducer();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  
  ThreeParticleCompositeProducer<T, U, V> & set_input_label_first(std::string const& input_label_first) {
    input_label_first_ = input_label_first;
    return *this;
  }

  ThreeParticleCompositeProducer<T, U, V> & set_input_label_second(std::string const& input_label_second) {
    input_label_second_ = input_label_second;
    return *this;
  }

  ThreeParticleCompositeProducer<T, U, V> & set_input_label_third(std::string const& input_label_third) {
    input_label_third_ = input_label_third;
    return *this;
  }

  ThreeParticleCompositeProducer<T, U, V> & set_candidate_name_first(std::string const& candidate_name_first) {
    candidate_name_first_ = candidate_name_first;
    return *this;
  }

  ThreeParticleCompositeProducer<T, U, V> & set_candidate_name_second(std::string const& candidate_name_second) {
    candidate_name_second_ = candidate_name_second;
    return *this;
  }

  ThreeParticleCompositeProducer<T, U, V> & set_candidate_name_third(std::string const& candidate_name_third) {
    candidate_name_third_ = candidate_name_third;
    return *this;
  }

  ThreeParticleCompositeProducer<T, U, V> & set_output_label(std::string const& output_label) {
    output_label_ = output_label;
    return *this;
  }
};

template <class T, class U, class V>
ThreeParticleCompositeProducer<T, U, V>::ThreeParticleCompositeProducer(std::string const& name) : ModuleBase(name) {
}

template <class T, class U, class V>
ThreeParticleCompositeProducer<T, U, V>::~ThreeParticleCompositeProducer() {
  ;
}

template <class T, class U, class V>
int ThreeParticleCompositeProducer<T, U, V>::PreAnalysis() {
  return 0;
}

template <class T, class U, class V>
int ThreeParticleCompositeProducer<T, U, V>::Execute(TreeEvent *event) {
  std::vector<T *> const& vec_first = event->GetPtrVec<T>(input_label_first_);
  std::vector<U *> const& vec_second = event->GetPtrVec<U>(input_label_second_);
  std::vector<V *> const& vec_third = event->GetPtrVec<V>(input_label_third_);
  std::vector< std::tuple<T*,U*,V*> > products = MakeThreeParticles(vec_first, vec_second, vec_third);
  std::vector<CompositeCandidate> vec_out;
  std::vector<CompositeCandidate *> ptr_vec_out;
  for (unsigned i = 0; i < products.size(); ++i) {
    vec_out.push_back(CompositeCandidate());
    CompositeCandidate & cand_ref = vec_out.back();
    cand_ref.AddCandidate(candidate_name_first_, std::get<0>(products[i]));
    cand_ref.AddCandidate(candidate_name_second_, std::get<1>(products[i]));
    cand_ref.AddCandidate(candidate_name_third_, std::get<2>(products[i]));
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

template <class T, class U, class V>
int ThreeParticleCompositeProducer<T, U, V>::PostAnalysis() {
  return 0;
}

template <class T, class U, class V>
void ThreeParticleCompositeProducer<T, U, V>::PrintInfo() {
  ;
}

}

#endif
