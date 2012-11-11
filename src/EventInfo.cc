#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"

namespace ic {
  //Constructors/Destructors
  EventInfo::EventInfo() {
    is_data_ = false;
    event_ = 0;
    run_ = 0;
    lumi_block_ = 0;
    bunch_crossing_ = 0;
  }

  EventInfo::~EventInfo() {
  }

  void EventInfo::Print() const {
  }
}
    