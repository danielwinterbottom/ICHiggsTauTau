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
  bool deep_copy_ = false;


 public:
  CopyCollection(std::string const& name, std::string const& input_name, std::string const& copy_name);
  CopyCollection(std::string const& name, std::string const& input_name, std::string const& copy_name, bool const& deep_copy);
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
CopyCollection<T>::CopyCollection(std::string const& name, std::string const& input_name, std::string const& copy_name, bool const& deep_copy) : ModuleBase(name) {
  input_name_ = input_name;
  copy_name_ = copy_name;
  deep_copy_ = deep_copy;
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
  if(deep_copy_){
    for(unsigned i=0; i<vec.size(); ++i) vec.at(i) = new T(*vec.at(i));
  }
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
