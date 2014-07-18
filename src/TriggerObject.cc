#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"

namespace ic {
TriggerObject::TriggerObject() {}

TriggerObject::~TriggerObject() {}

void TriggerObject::Print() const { Candidate::Print(); }
}
