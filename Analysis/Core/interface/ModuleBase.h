#ifndef ICHiggsTauTau_Core_ModuleBase_h
#define ICHiggsTauTau_Core_ModuleBase_h

#include <string>
#include <iostream>                     // for operator<<, cout, ostream, etc
#include <vector>                       // for vector
#include "boost/bind.hpp"               // for bind
#include "boost/function.hpp"
namespace ic { class TreeEvent; }

#define CLASS_MEMBER(classn,type,name)                                                \
    private:                                                                          \
      type name##_;                                                                   \
    public:                                                                           \
      virtual classn & set_##name(type const& name) {name##_ = name; return *this; }  \
    private:

namespace ic {

class ModuleBase {
 private:
  std::string module_name_;
  unsigned events_processed_;

 public:
  ModuleBase(std::string const& name);
  virtual ~ModuleBase();

  inline void IncreaseProcessedCount() { ++events_processed_; }
  inline unsigned EventsProcessed() { return events_processed_; }
  inline std::string ModuleName() { return module_name_; }

  inline virtual int PreAnalysis() { return 0; }
  virtual int Execute(ic::TreeEvent*) = 0;
  inline virtual int PostAnalysis() { return 0; }
  inline virtual void PrintInfo() { return; }
};
}

#endif