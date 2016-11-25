#include "../interface/PileupInfo.hh"

namespace ic {
// Constructors/Destructors
PileupInfo::PileupInfo()
    : num_interactions_(0), bunch_crossing_(0), true_num_interactions_(0.) {}

PileupInfo::~PileupInfo() {}

void PileupInfo::Print() const {}
}
