#ifndef ICHiggsTauTau_Module_GenericModule_h
#define ICHiggsTauTau_Module_GenericModule_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "boost/function.hpp"

#include <string>

namespace ic {

class GenericModule : public ModuleBase {
 private:
  CLASS_MEMBER(GenericModule, boost::function<int(ic::TreeEvent *)>, function)

 public:
  GenericModule(std::string const& name);
  virtual ~GenericModule();

  virtual int PreAnalysis();
  virtual int Execute(ic::TreeEvent* evt);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

GenericModule::GenericModule(std::string const& name) : ModuleBase(name) {
}

GenericModule::~GenericModule() {
  ;
}

int GenericModule::PreAnalysis() {
  return 0;
}


int GenericModule::Execute(ic::TreeEvent* event) {
  return function_(event);
}


int GenericModule::PostAnalysis() {
  return 0;
}

void GenericModule::PrintInfo() {
  ;
}





}

#endif
