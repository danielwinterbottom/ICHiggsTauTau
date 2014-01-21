#include "Core/interface/ModuleBase.h"

namespace ic {
	
	ModuleBase::ModuleBase(std::string const& name) {
		module_name_ = name;
		events_processed_ = 0;
	}

  ModuleBase::ModuleBase(std::string const& name, std::string const& config) {
    module_name_ = name;
    config_file_ = config;
    events_processed_ = 0;
  }

	ModuleBase::~ModuleBase() {
		;
	}

  void ModuleBase::FillParameters() {
    po::store(po::parse_config_file<char>(config_file_.c_str(), params_, true), vm_);
    po::notify(vm_);
    for (unsigned i = 0; i < print_params_.size(); ++i) {
      print_params_[i]();
    }
  }
}