#ifndef ICHiggsTauTau_Module_SimpleCounter_h
#define ICHiggsTauTau_Module_SimpleCounter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "boost/function.hpp"
#include <string>

namespace ic {

template <class T>
class SimpleCounter : public ModuleBase {
 private:
  CLASS_MEMBER(SimpleCounter<T>, boost::function<bool (T const*)>, predicate)
  CLASS_MEMBER(SimpleCounter<T>, std::string, input_label)
  CLASS_MEMBER(SimpleCounter<T>, unsigned, min)
  CLASS_MEMBER(SimpleCounter<T>, unsigned, max)

 public:
  SimpleCounter(std::string const& name);
  virtual ~SimpleCounter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

template <class T>
SimpleCounter<T>::SimpleCounter(std::string const& name) : ModuleBase(name) {
  min_ = 0;
  max_ = 9999;
}

template <class T>
SimpleCounter<T>::~SimpleCounter() {
  ;
}

template <class T>
int SimpleCounter<T>::PreAnalysis() {
  return 0;
}

template <class T>
int SimpleCounter<T>::Execute(TreeEvent *event) {
  std::vector<T *> const& vec = event->GetPtrVec<T>(input_label_);
  unsigned n_pass = std::count_if(vec.begin(), vec.end(), predicate_);
  if (n_pass >= min_ && n_pass <= max_) {
    return 0;
  } else {
    return 1;
  }
}

// template <>
// int SimpleCounter<CompositeCandidate>::Execute(TreeEvent *event) {
//   std::vector<CompositeCandidate> & vec = event->Get<std::vector<CompositeCandidate> >(input_label_);
//   std::vector<CompositeCandidate *> tmp;
//   tmp.resize(vec.size());
//   for (unsigned i = 0; i < vec.size(); ++i) {
//     tmp[i] = &(vec[i]);
//   }
//   unsigned n_pass = std::count_if(tmp.begin(), tmp.end(), predicate_);
//   if (n_pass >= min_ && n_pass <= max_) {
//     return 0;
//   } else {
//     return 1;
//   }
// }

template <class T>
int SimpleCounter<T>::PostAnalysis() {
  return 0;
}

template <class T>
void SimpleCounter<T>::PrintInfo() {
  ;
}



}

#endif
