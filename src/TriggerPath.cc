#include "../interface/TriggerPath.hh"

namespace ic {
TriggerPath::TriggerPath()
    : name_(""), accept_(false), prescale_(0), id_(0), version_(0) {}

TriggerPath::~TriggerPath() {}

void TriggerPath::Print() const {}
}
