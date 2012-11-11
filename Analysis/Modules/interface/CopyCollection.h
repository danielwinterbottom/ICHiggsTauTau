#ifndef ICHiggsTauTau_Module_CopyCollection_h
#define ICHiggsTauTau_Module_CopyCollection_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {

template <class T>
class CopyCollection : public ModuleBase {
 private:
  std::string input_name_;
  std::string copy_name_;


 public:
  CopyCollection(std::string const& name, std::string const& input_name, std::string const& copy_name);
  virtual ~CopyCollection();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

template <class T>
CopyCollection<T>::CopyCollection(std::string const& name, std::string const& input_name, std::string const& copy_name) : ModuleBase(name) {
  input_name_ = input_name;
  copy_name_ = copy_name;
}

template <class T>
CopyCollection<T>::~CopyCollection() {
  ;
}

template <class T>
int CopyCollection<T>::PreAnalysis() {
  return 0;
}

template <class T>
int CopyCollection<T>::Execute(TreeEvent *event) {
  std::vector<T *> vec = event->GetPtrVec<T>(input_name_);
  event->Add(copy_name_, vec);
  return 0;
}

template <class T>
int CopyCollection<T>::PostAnalysis() {
  return 0;
}

template <class T>
void CopyCollection<T>::PrintInfo() {
  ;
}

}

#endif
