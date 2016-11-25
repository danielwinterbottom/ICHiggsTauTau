 #ifndef ICHiggsTauTau_Module_ComplexFilter_h
#define ICHiggsTauTau_Module_ComplexFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "boost/function.hpp"

#include <string>

namespace ic {

template <class T, class U, class V>
class ComplexFilter : public ModuleBase {
 private:
    boost::function<bool (T const*,V const&)> predicate_;
    boost::function<V (U const*)> secondary_predicate_;
    std::string primary_input_label_;
    std::string secondary_input_label_;
    unsigned min_;
    unsigned max_;

 public:
    virtual ComplexFilter<T,U,V> & set_predicate(boost::function<bool (T const*,const V)> const& predicate){predicate_=predicate; return *this; }
    virtual ComplexFilter<T,U,V> & set_secondary_predicate(boost::function<V (U const*)> const& secondary_predicate){secondary_predicate_=secondary_predicate; return *this; }
    virtual ComplexFilter<T,U,V> & set_primary_input_label(std::string const& primary_input_label){primary_input_label_=primary_input_label;return *this; }
    virtual ComplexFilter<T,U,V> & set_secondary_input_label(std::string const& secondary_input_label){secondary_input_label_=secondary_input_label;return *this; }
    virtual ComplexFilter<T,U,V> & set_min(unsigned const& min){min_=min; return *this; }
    virtual ComplexFilter<T,U,V> & set_max(unsigned const& max){max_=max; return *this; }
    
  ComplexFilter(std::string const& name);
  virtual ~ComplexFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

template <class T, class U, class V>
ComplexFilter<T,U,V>::ComplexFilter(std::string const& name) : ModuleBase(name) {
  min_ = 0;
  max_ = 9999;
}

template <class T,class U,class V>
ComplexFilter<T,U,V>::~ComplexFilter() {
  ;
}

template <class T,class U,class V>
int ComplexFilter<T,U,V>::PreAnalysis() {
  return 0;
}

template <class T,class U,class V>
int ComplexFilter<T,U,V>::Execute(TreeEvent *event) {
  std::vector<T *> & vec = event->GetPtrVec<T>(primary_input_label_);
  U const* secondary = event->GetPtr<U>(secondary_input_label_);

  const V extrainfo=secondary_predicate_(secondary);
  
  boost::function<bool (T const*)> final_predicate = boost::bind(predicate_,_1,extrainfo);
  ic::erase_if(vec,!boost::bind(final_predicate,_1));
  if (vec.size() >= min_ && vec.size() <= max_) {
    return 0;
  } else {
    return 1;
  }
}

// template <>
// int ComplexFilter<CompositeCandidate>::Execute(TreeEvent *event) {
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

template <class T,class U,class V>
int ComplexFilter<T,U,V>::PostAnalysis() {
  return 0;
}

template <class T,class U,class V>
void ComplexFilter<T,U,V>::PrintInfo() {
  ;
}





}

#endif
