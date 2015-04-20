#include "../interface/TriggerPath.hh"
#include <iostream>
#include "boost/format.hpp"

namespace ic {
TriggerPath::TriggerPath()
    : name_(""), accept_(false), prescale_(0), id_(0), version_(0) {}

TriggerPath::~TriggerPath() {}

void TriggerPath::Print() const {
  std::string name = name_ != "" ? name_ : "<path name not saved>";
  std::cout << boost::format(
                   "%-65s accept=%-3i prescale=%-3i id=%-21i version=%-2i\n") %
                   name % accept_ % prescale_ % id_ % version_;
}
}
