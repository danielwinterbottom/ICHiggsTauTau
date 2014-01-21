#ifndef ICHiggsTauTau_Core_ModuleBase_h
#define ICHiggsTauTau_Core_ModuleBase_h

#include <string>
#include <iostream>                     // for operator<<, cout, ostream, etc
#include <vector>                       // for vector
#include "boost/bind.hpp"               // for bind
#include "boost/program_options.hpp"
#include "boost/function.hpp"
namespace ic { class TreeEvent; }

#define CLASS_MEMBER(classn,type,name)                                                \
    private:                                                                \
      type name##_;                                                         \
    public:                                                                 \
      virtual classn & set_##name(type const& name) {name##_ = name; return *this; }


namespace po = boost::program_options;

namespace ic {

	class ModuleBase {

 private:
   std::string module_name_;
   std::string config_file_;
   unsigned events_processed_;
   po::options_description params_;
   po::variables_map vm_;
   std::vector< boost::function<void (void)> > print_params_;

 public:
  ModuleBase(std::string const& name);
  ModuleBase(std::string const& name, std::string const& config);

  virtual ~ModuleBase();
  inline void IncreaseProcessedCount() { ++events_processed_; }
  inline unsigned EventsProcessed() { return events_processed_; }
  inline std::string ModuleName() { return module_name_; }

  virtual int PreAnalysis() = 0;
  virtual int Execute(ic::TreeEvent *) = 0;
  virtual int PostAnalysis() = 0;
  virtual void PrintInfo() = 0;

  template <class T>
  void AddParameter(std::string const& name, T & var);

  template <class T>
  void AddParameter(std::string const& name, T & var, T const& default_value);

  template <class T>
  void PrintParameter(std::string const& name);

  void FillParameters();
};



template <class T>
void ModuleBase::AddParameter(std::string const& name, T & var) {
  params_.add_options()
    ((module_name_+"."+name).c_str(), po::value<T>(&var)->required(),name.c_str());
  print_params_.push_back(boost::bind(&ModuleBase::PrintParameter<T>, this, name));
}

template <class T>
void ModuleBase::AddParameter(std::string const& name, T & var, T const& default_value) {
  params_.add_options()
    ((module_name_+"."+name).c_str(), po::value<T>(&var)->default_value(default_value),name.c_str());
  print_params_.push_back(boost::bind(&ModuleBase::PrintParameter<T>, this, name));

}

template <class T>
void ModuleBase::PrintParameter(std::string const& name) {
  std::cout << name << " = " << (vm_[(module_name_+"."+name).c_str()].as<T>());
  if (vm_[(module_name_+"."+name).c_str()].defaulted()) std::cout << " (default)";
  std::cout << std::endl;
}


}


#endif