#ifndef ICHiggsTauTau_Module_PtEtaFilter_h
#define ICHiggsTauTau_Module_PtEtaFilter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include <string>
#include "boost/bind.hpp"

namespace ic {

template <class T>
class PtEtaFilter : public ModuleBase {
 private:
  std::string input_name_;
  double min_pt_;
  double max_eta_;

 public:
  PtEtaFilter(std::string const& name, std::string input_name, double min_pt , double max_eta);
  virtual ~PtEtaFilter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

template <class T>
PtEtaFilter<T>::PtEtaFilter(std::string const& name, std::string input_name, double min_pt , double max_eta) : ModuleBase(name) {
  input_name_ = input_name;
  min_pt_ = min_pt;
  max_eta_ = max_eta;
}

template <class T>
PtEtaFilter<T>::~PtEtaFilter() {
  ;
}

template <class T>
int PtEtaFilter<T>::PreAnalysis() {
  return 0;
}

template <class T>
int PtEtaFilter<T>::Execute(TreeEvent *event) {
  std::vector<T *> & vec = event->GetPtrVec<T>(input_name_);
  ic::erase_if(vec,!boost::bind(MinPtMaxEta, _1, min_pt_, max_eta_));
  if (vec.size() >= 2) {
    return 0;
  } else {
    return 1;
  }
}

template <class T>
int PtEtaFilter<T>::PostAnalysis() {
  return 0;
}

template <class T>
void PtEtaFilter<T>::PrintInfo() {
  ;
}

}

#endif
