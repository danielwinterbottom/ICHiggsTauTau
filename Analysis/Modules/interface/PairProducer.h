#ifndef ICHiggsTauTau_Module_PairProducer_h
#define ICHiggsTauTau_Module_PairProducer_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"

#include <string>
#include "boost/bind.hpp"

namespace ic {

template <class T>
class PairProducer : public ModuleBase {
 private:
  std::string input_name_;
  std::string prod_name_;

 public:
  PairProducer(std::string const& name, std::string input_name, std::string prod_name);
  virtual ~PairProducer();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

template <class T>
PairProducer<T>::PairProducer(std::string const& name, std::string input_name, std::string prod_name) : ModuleBase(name) {
  input_name_ = input_name;
  prod_name_ = prod_name;
}

template <class T>
PairProducer<T>::~PairProducer() {
  ;
}

template <class T>
int PairProducer<T>::PreAnalysis() {
  return 0;
}

template <class T>
int PairProducer<T>::Execute(TreeEvent *event) {
  std::vector<T *> & vec = event->GetPtrVec<T>(input_name_);
  //ic::erase_if(vec,!boost::bind(MinPtMaxEta, _1, min_pt_, max_eta_));
  std::vector<CompositeCandidate> cand_vec;
  for (unsigned i = 0; i < vec.size(); ++i) {
    for (unsigned j = i+1; j < vec.size(); ++j) {
      cand_vec.push_back(CompositeCandidate());
      CompositeCandidate & cand_ref = cand_vec.back();
      cand_ref.AddCandidate("lepton1", vec[i]);
      cand_ref.AddCandidate("lepton2", vec[j]);
    }
  }
  event->Add(prod_name_, cand_vec);
  return 0;
}

template <class T>
int PairProducer<T>::PostAnalysis() {
  return 0;
}

template <class T>
void PairProducer<T>::PrintInfo() {
  ;
}

}

#endif
