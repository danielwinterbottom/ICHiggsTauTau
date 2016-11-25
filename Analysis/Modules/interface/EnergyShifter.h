#ifndef ICHiggsTauTau_Module_EnergyShifter_h
#define ICHiggsTauTau_Module_EnergyShifter_h

#include <string>
#include <map>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"

namespace ic {

template <class T>
class EnergyShifter : public ModuleBase {
 private:
  CLASS_MEMBER(EnergyShifter, std::string, input_label)
  CLASS_MEMBER(EnergyShifter, double, shift)
  CLASS_MEMBER(EnergyShifter, bool, save_shifts)
  CLASS_MEMBER(EnergyShifter, std::string, shift_label)

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
  save_shifts_ = false;
  shift_label_ = "shifts_"+ModuleName();
}

template <class T>
EnergyShifter<T>::~EnergyShifter() {
  ;
}

template <class T>
int EnergyShifter<T>::PreAnalysis() {
  PrintHeader("EnergyShifter");
  PrintArg("input_label", input_label_);
  PrintArg("shift", shift_);
  PrintArg("save_shifts", save_shifts_);
  if (save_shifts_) {
    PrintArg("shift_label", shift_label_);
  }
  return 0;
}

template <class T>
int EnergyShifter<T>::Execute(TreeEvent *event) {
  std::vector<T *> & vec = event->GetPtrVec<T>(input_label_);
  std::map<std::size_t, ROOT::Math::PxPyPzEVector> shifts;
  for (unsigned i = 0; i < vec.size(); ++i) {
    ROOT::Math::PxPyPzEVector before(vec[i]->vector());
    vec[i]->set_pt(vec[i]->pt()*shift_);
    vec[i]->set_energy(vec[i]->energy()*shift_);
    if (save_shifts_) {
      ROOT::Math::PxPyPzEVector after(vec[i]->vector());
      shifts[vec[i]->id()] = after - before;
    }
  }
  if (save_shifts_) {
    event->Add(shift_label_, shifts);
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
