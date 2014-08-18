#ifndef ICHiggsTauTau_Module_CheckEvents_h
#define ICHiggsTauTau_Module_CheckEvents_h

#include <string>
#include <set>
#include "Core/interface/ModuleBase.h"

namespace ic {

class TreeEvent;

class CheckEvents : public ModuleBase {
 private:
  std::set<uint64_t> events_;
  CLASS_MEMBER(CheckEvents, bool, skip_events)
  CLASS_MEMBER(CheckEvents, std::string, input)

 public:
  CheckEvents(std::string const& name);
  virtual ~CheckEvents();

  virtual int Execute(TreeEvent* event);

  void CheckEvent(uint64_t evt);
};
}

#endif
