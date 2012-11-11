#ifndef ICHiggsTauTau_Module_OverlapFilter_h
#define ICHiggsTauTau_Module_OverlapFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/Objects.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/SimpleCounter.h"
#include <string>

namespace ic {

template <class T, class U>
class OverlapFilter : public ModuleBase {
 private:
  std::string input_label_;
  std::string reference_label_;
  double min_dr_;

 public:
  OverlapFilter(std::string const& name);
  virtual ~OverlapFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  
  OverlapFilter<T, U> & set_input_label(std::string const& input_label) {
    input_label_ = input_label;
    return *this;
  }

  OverlapFilter<T, U> & set_reference_label(std::string const& reference_label) {
    reference_label_ = reference_label;
    return *this;
  }

  OverlapFilter<T, U> & set_min_dr(double const& min_dr) {
    min_dr_ = min_dr;
    return *this;
  }

};



template <class T>
class OverlapFilter<T, CompositeCandidate> : public ModuleBase {
 private:
  std::string input_label_;
  std::string reference_label_;
  double min_dr_;

 public:
  OverlapFilter(std::string const& name);
  virtual ~OverlapFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  
  OverlapFilter<T, CompositeCandidate> & set_input_label(std::string const& input_label) {
    input_label_ = input_label;
    return *this;
  }

  OverlapFilter<T, CompositeCandidate> & set_reference_label(std::string const& reference_label) {
    reference_label_ = reference_label;
    return *this;
  }

  OverlapFilter<T, CompositeCandidate> & set_min_dr(double const& min_dr) {
    min_dr_ = min_dr;
    return *this;
  }

};




template <class T, class U>
OverlapFilter<T, U>::OverlapFilter(std::string const& name) : ModuleBase(name) {
}

template <class T, class U>
OverlapFilter<T, U>::~OverlapFilter() {
  ;
}

template <class T, class U>
int OverlapFilter<T, U>::PreAnalysis() {
  return 0;
}

template <class T, class U>
int OverlapFilter<T, U>::Execute(TreeEvent *event) {
  // Get the input collection
  std::vector<T *> & vec = event->GetPtrVec<T>(input_label_);
  // Get the reference input collection
  std::vector<U *> const& ref_vec = event->GetPtrVec<U>(reference_label_);
  ic::erase_if(vec, !boost::bind(MinDRToCollection<U*>, _1, ref_vec, min_dr_));
  return 0;
}


template <class T, class U>
int OverlapFilter<T, U>::PostAnalysis() {
  return 0;
}

template <class T, class U>
void OverlapFilter<T, U>::PrintInfo() {
  ;
}


////////

template <class T>
OverlapFilter<T, CompositeCandidate>::OverlapFilter(std::string const& name) : ModuleBase(name) {
}

template <class T>
OverlapFilter<T, CompositeCandidate>::~OverlapFilter() {
  ;
}

template <class T>
int OverlapFilter<T, CompositeCandidate>::PreAnalysis() {
  return 0;
}

template <class T>
int OverlapFilter<T, CompositeCandidate>::Execute(TreeEvent *event) {
  // Get the input collection
  std::vector<T *> & vec = event->GetPtrVec<T>(input_label_);
  // Get the reference input collection
  std::vector<CompositeCandidate *> const& ref_vec = event->GetPtrVec<CompositeCandidate>(reference_label_);
  for (unsigned i = 0; i < ref_vec.size(); ++i) {
  ic::erase_if(vec, !boost::bind(MinDRToCollection<Candidate*>, _1, ref_vec[i]->AsVector(), min_dr_));
  }
  return 0;
}


template <class T>
int OverlapFilter<T, CompositeCandidate>::PostAnalysis() {
  return 0;
}

template <class T>
void OverlapFilter<T, CompositeCandidate>::PrintInfo() {
  ;
}





}

#endif
