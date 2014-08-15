#include "Modules/interface/CheckEvents.h"

#include <string>
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "Core/interface/TreeEvent.h"

namespace ic {

CheckEvents::CheckEvents(std::string const& name)
    : ModuleBase(name), skip_events_(false), input_("eventInfo") {}

CheckEvents::~CheckEvents() { ; }

void CheckEvents::CheckEvent(uint64_t evt) { events_.insert(evt); }

int CheckEvents::Execute(TreeEvent* event) {
  EventInfo const* eventInfo = event->GetPtr<EventInfo>(input_);
  if (events_.count(eventInfo->event())) {
    return 3;
  } else if (skip_events_) {
    return 1;
  } else {
    return 0;
  }
}
}
