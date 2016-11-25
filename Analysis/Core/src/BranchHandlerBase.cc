#include "Core/interface/BranchHandlerBase.h"

namespace ic {
BranchHandlerBase::BranchHandlerBase()
    : branch_ptr_(nullptr), current_(-1), no_overwrite_(false) {}

BranchHandlerBase::~BranchHandlerBase() {}
}