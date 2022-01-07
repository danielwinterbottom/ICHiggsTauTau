#ifndef ICHiggsTauTau_Module_EnergyResShifter_h
#define ICHiggsTauTau_Module_EnergyResShifter_h

#include <string>
#include <map>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "TF1.h"

namespace ic {

template <class T>
class EnergyResShifter : public ModuleBase {
 private:
  CLASS_MEMBER(EnergyResShifter, std::string, input_label)
  CLASS_MEMBER(EnergyResShifter, std::string, input_label_gen)
  CLASS_MEMBER(EnergyResShifter, double, shift)
  CLASS_MEMBER(EnergyResShifter, bool, save_shifts)
  CLASS_MEMBER(EnergyResShifter, std::string, shift_label)

  TF1 *f;

 public:
  EnergyResShifter(std::string const& name);
  virtual ~EnergyResShifter();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

template <class T>
EnergyResShifter<T>::EnergyResShifter(std::string const& name) : ModuleBase(name) {
  input_label_ = "";
  input_label_gen_ = "";
  shift_ = 1.0;
  save_shifts_ = false;
  shift_label_ = "shifts_"+ModuleName();
}

template <class T>
EnergyResShifter<T>::~EnergyResShifter() {
  ;
}

template <class T>
int EnergyResShifter<T>::PreAnalysis() {
  PrintHeader("EnergyResShifter");
  PrintArg("input_label", input_label_);
  PrintArg("shift", shift_);
  PrintArg("save_shifts", save_shifts_);
  if (save_shifts_) {
    PrintArg("shift_label", shift_label_);
  }


  return 0;
}

template <class T>
int EnergyResShifter<T>::Execute(TreeEvent *event) {
  std::vector<T *> & vec = event->GetPtrVec<T>(input_label_);
  std::vector<ROOT::Math::PtEtaPhiEVector> & vec_gen = event->Get<std::vector<ROOT::Math::PtEtaPhiEVector>>(input_label_gen_);
  std::map<std::size_t, ROOT::Math::PxPyPzEVector> shifts;

  if(vec.size()!=vec_gen.size()) {
    std::cerr<< "(EnergyResShifter) Error: reco and gen container sizes don't match "<<std::endl;
    throw;
  }


  for (unsigned i = 0; i < vec.size(); ++i) {
    double shift = shift_;
    ROOT::Math::PtEtaPhiEVector gen_vec = vec_gen[i];
    double gen_pt = gen_vec.Pt(); 
    double reco_pt = vec[i]->pt();
    double reco_E = vec[i]->energy();
    double new_pt = (gen_pt-reco_pt)*shift + reco_pt;

    ROOT::Math::PxPyPzEVector before(vec[i]->vector());
    vec[i]->set_pt(new_pt);
    vec[i]->set_energy(reco_E*new_pt/reco_pt);

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
int EnergyResShifter<T>::PostAnalysis() {
  return 0;
}

template <class T>
void EnergyResShifter<T>::PrintInfo() {
  ;
}

}

#endif
