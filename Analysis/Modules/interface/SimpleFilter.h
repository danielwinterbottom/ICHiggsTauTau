#ifndef ICHiggsTauTau_Module_SimpleFilter_h
#define ICHiggsTauTau_Module_SimpleFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "boost/function.hpp"

#include <string>

namespace ic {

template <class T>
class SimpleFilter : public ModuleBase {
 private:
  CLASS_MEMBER(SimpleFilter<T>, boost::function<bool (T const*)>, predicate)
  CLASS_MEMBER(SimpleFilter<T>, std::string, input_label)
  CLASS_MEMBER(SimpleFilter<T>, unsigned, min)
  CLASS_MEMBER(SimpleFilter<T>, unsigned, max)

 public:
  SimpleFilter(std::string const& name);
  virtual ~SimpleFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

template <class T>
SimpleFilter<T>::SimpleFilter(std::string const& name) : ModuleBase(name) {
  min_ = 0;
  max_ = 9999;
}

template <class T>
SimpleFilter<T>::~SimpleFilter() {
  ;
}

template <class T>
int SimpleFilter<T>::PreAnalysis() {
  return 0;
}

template <class T>
int SimpleFilter<T>::Execute(TreeEvent *event) {
  std::vector<T *> & vec = event->GetPtrVec<T>(input_label_);
  ic::erase_if(vec,!boost::bind(predicate_,_1));
  if (vec.size() >= min_ && vec.size() <= max_) {
    return 0;
  } else {
    return 1;
  }
}

// template <>
// int SimpleFilter<CompositeCandidate>::Execute(TreeEvent *event) {
//   std::vector<CompositeCandidate> & vec = event->Get<std::vector<CompositeCandidate> >(input_label_);
//   std::vector<CompositeCandidate *> tmp;
//   tmp.resize(vec.size());
//   for (unsigned i = 0; i < vec.size(); ++i) {
//     tmp[i] = &(vec[i]);
//   }
//   ic::erase_if(tmp,!boost::bind(predicate_,_1));
//   std::vector<CompositeCandidate> copy;
//   copy.resize(tmp.size());
//   for (unsigned i = 0; i < tmp.size(); ++i) {
//     copy[i] = *(tmp[i]);
//   }
//   event->ForceAdd(input_label_, copy); 
//   if (tmp.size() >= min_ && tmp.size() <= max_) {
//     return 0;
//   } else {
//     return 1;
//   }
// }

template <class T>
int SimpleFilter<T>::PostAnalysis() {
  return 0;
}

template <class T>
void SimpleFilter<T>::PrintInfo() {
  ;
}





}

#endif
