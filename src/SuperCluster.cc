#include "UserCode/ICHiggsTauTau/interface/SuperCluster.hh"

namespace ic {
// Constructors/Destructors
SuperCluster::SuperCluster()
    : id_(0), energy_(0.), raw_energy_(0.), is_barrel_(false) {}

SuperCluster::~SuperCluster() {}

void SuperCluster::Print() const {}
}
