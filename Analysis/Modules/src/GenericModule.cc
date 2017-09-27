#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/GenericModule.h"

namespace ic {

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
