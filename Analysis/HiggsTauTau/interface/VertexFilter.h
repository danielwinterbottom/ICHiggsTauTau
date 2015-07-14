#ifndef ICHiggsTauTau_Module_VertexFilter_h
#define ICHiggsTauTau_Module_VertexFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "boost/function.hpp"

#include <string>

namespace ic {

template <class T>
class VertexFilter : public ModuleBase {
 private:
  CLASS_MEMBER(VertexFilter<T>, boost::function<bool (T const*)>, predicate)
  CLASS_MEMBER(VertexFilter<T>, std::string, input_label)
  CLASS_MEMBER(VertexFilter<T>, unsigned, min)
  CLASS_MEMBER(VertexFilter<T>, unsigned, max)

 public:
  VertexFilter(std::string const& name);
  virtual ~VertexFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

template <class T>
VertexFilter<T>::VertexFilter(std::string const& name) : ModuleBase(name) {
  min_ = 1;
  max_ = 9999;
}

template <class T>
VertexFilter<T>::~VertexFilter() {
  ;
}

template <class T>
int VertexFilter<T>::PreAnalysis() {
  return 0;
}

template <class T>
int VertexFilter<T>::Execute(TreeEvent *event) {
  std::vector<T *> & vec = event->GetPtrVec<T>(input_label_);
  std::vector<ic::Vertex*> & vertex_vec = event->GetPtrVec<ic::Vertex>("vertices");
  if(vertex_vec.size()==0) return 0;
  double vtxz = vertex_vec[0]->vz();
  ic::erase_if(vec,!boost::bind(VertexDz,_1,vtxz));
  if (vec.size() >= min_ && vec.size() <= max_) {
    return 0;
  } else {
    return 1;
  }
}

// template <>
// int VertexFilter<CompositeCandidate>::Execute(TreeEvent *event) {
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
int VertexFilter<T>::PostAnalysis() {
  return 0;
}

template <class T>
void VertexFilter<T>::PrintInfo() {
  ;
}





}

#endif
