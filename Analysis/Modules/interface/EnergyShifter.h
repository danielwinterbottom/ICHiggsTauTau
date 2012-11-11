#ifndef ICHiggsTauTau_Module_EnergyShifter_h
#define ICHiggsTauTau_Module_EnergyShifter_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include <string>
#include "boost/bind.hpp"

namespace ic {

template <class T>
class EnergyShifter : public ModuleBase {
 private:
  CLASS_MEMBER(EnergyShifter, std::string, input_label)
  CLASS_MEMBER(EnergyShifter, double, shift)

 public:
  EnergyShifter(std::string const& name);
  virtual ~EnergyShifter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

template <class T>
EnergyShifter<T>::EnergyShifter(std::string const& name) : ModuleBase(name) {
  input_label_ = "";
  shift_ = 1.0;
}

template <class T>
EnergyShifter<T>::~EnergyShifter() {
  ;
}

template <class T>
int EnergyShifter<T>::PreAnalysis() {
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "PreAnalysis Info for EnergyShifter" << std::endl;
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "Input: [" << input_label_ << "] Shift: [" << shift_ << "]" << std::endl;
  return 0;
}

template <class T>
int EnergyShifter<T>::Execute(TreeEvent *event) {
  std::vector<T *> & vec = event->GetPtrVec<T>(input_label_);
  for (unsigned i = 0; i < vec.size(); ++i) {
    vec[i]->set_pt(vec[i]->pt()*shift_);
    vec[i]->set_energy(vec[i]->energy()*shift_);
  }
  return 0;
}

template <class T>
int EnergyShifter<T>::PostAnalysis() {
  return 0;
}

template <class T>
void EnergyShifter<T>::PrintInfo() {
  ;
}

}

#endif
