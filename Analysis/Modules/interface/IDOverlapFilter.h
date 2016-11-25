#ifndef ICHiggsTauTau_Module_IDOverlapFilter_h
#define ICHiggsTauTau_Module_IDOverlapFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/Objects.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/SimpleCounter.h"
#include <string>

namespace ic {

template <class T, class U>
class IDOverlapFilter : public ModuleBase {
 private:
  std::string input_label_;
  std::string reference_label_;

 public:
  IDOverlapFilter(std::string const& name);
  virtual ~IDOverlapFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  
  IDOverlapFilter<T, U> & set_input_label(std::string const& input_label) {
    input_label_ = input_label;
    return *this;
  }

  IDOverlapFilter<T, U> & set_reference_label(std::string const& reference_label) {
    reference_label_ = reference_label;
    return *this;
  }

};

template <class T, class U>
IDOverlapFilter<T, U>::IDOverlapFilter(std::string const& name) : ModuleBase(name) {
}

template <class T, class U>
IDOverlapFilter<T, U>::~IDOverlapFilter() {
  ;
}

template <class T, class U>
int IDOverlapFilter<T, U>::PreAnalysis() {
  return 0;
}

template <class T, class U>
int IDOverlapFilter<T, U>::Execute(TreeEvent *event) {
  // Get the input collection
  std::vector<T *> & vec = event->GetPtrVec<T>(input_label_);
  // Get the reference input collection
  std::vector<U *> const& ref_vec = event->GetPtrVec<U>(reference_label_);
  ic::erase_if(vec, boost::bind(FoundIdInCollection<T, U>, _1, ref_vec));
  return 0;
}

template <class T, class U>
int IDOverlapFilter<T, U>::PostAnalysis() {
  return 0;
}

template <class T, class U>
void IDOverlapFilter<T, U>::PrintInfo() {
  ;
}

}

#endif
