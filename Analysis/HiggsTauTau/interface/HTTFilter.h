#ifndef ICHiggsTauTau_Module_HTTFilter_h
#define ICHiggsTauTau_Module_HTTFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "boost/function.hpp"

#include <string>

namespace ic {

template <class T>
class HTTFilter : public ModuleBase {
 private:
  CLASS_MEMBER(HTTFilter<T>, boost::function<bool (T const*)>, predicate)
  CLASS_MEMBER(HTTFilter<T>, std::string, input_label)
  CLASS_MEMBER(HTTFilter<T>, unsigned, min)
  CLASS_MEMBER(HTTFilter<T>, unsigned, max)
  //Option to store the result of the filter predicate without actually applying the filter
	CLASS_MEMBER(HTTFilter<T>, bool, no_filter)
  //Name under which the predicate result is stored
	CLASS_MEMBER(HTTFilter<T>, std::string, veto_name)

 public:
  HTTFilter(std::string const& name);
  virtual ~HTTFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

template <class T>
HTTFilter<T>::HTTFilter(std::string const& name) : ModuleBase(name) {
  min_ = 0;
  max_ = 9999;
	no_filter_ = false;
	veto_name_ = "veto";
}

template <class T>
HTTFilter<T>::~HTTFilter() {
  ;
}

template <class T>
int HTTFilter<T>::PreAnalysis() {
  return 0;
}

template <class T>
int HTTFilter<T>::Execute(TreeEvent *event) {
  std::vector<T *> & vec = event->GetPtrVec<T>(input_label_);
  ic::erase_if(vec,!boost::bind(predicate_,_1));
	if(no_filter_){
	  if(vec.size() >= min_ && vec.size() <= max_){
		  event->Add(veto_name_,false);
			return 0;
		} else {
		  event->Add(veto_name_,true);
			return 0;
		}
 } else {
    if (vec.size() >= min_ && vec.size() <= max_) {
      return 0;
    } else {
      return 1;
	  }
  }
}

// template <>
// int HTTFilter<CompositeCandidate>::Execute(TreeEvent *event) {
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
int HTTFilter<T>::PostAnalysis() {
  return 0;
}

template <class T>
void HTTFilter<T>::PrintInfo() {
  ;
}





}

#endif
