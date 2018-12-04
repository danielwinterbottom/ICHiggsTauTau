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

bool ModuleBase::ProductExists(std::string const& name) {
  if (products_.count(name)) {
    return true;
  } else {
    return false;
  }
}

std::map<std::string, boost::any> ModuleBase::GetProducts() {
  return products_;
}




}
