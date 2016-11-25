#include "Core/interface/ModuleBase.h"

namespace ic {

ModuleBase::ModuleBase(std::string const& name)
    : module_name_(name), events_processed_(0) {}

ModuleBase::~ModuleBase() { ; }

void ModuleBase::PrintHeader(std::string const& classname) {
  std::cout << "-------------------------------------\n";
  std::cout << classname << ": " << module_name_ << "\n";
  std::cout << "-------------------------------------\n";
}
}
