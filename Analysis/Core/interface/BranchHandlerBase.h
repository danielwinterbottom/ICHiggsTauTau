#ifndef ICHiggsTauTau_Analysis_BranchHandlerBase_h
#define ICHiggsTauTau_Analysis_BranchHandlerBase_h
#include "TBranch.h"

namespace ic {

/**
 * @brief An abstract base class for TBranch management
 * @details This class holds a pointer to a TBranch and is the base class for
 * BranchHander which is templated on the type of the class stored in the
 * branch.
 */
class BranchHandlerBase {
 public:
  BranchHandlerBase();
  virtual ~BranchHandlerBase();
  virtual void SetAddress() = 0;
  inline void GetEntry(int64_t i) {
    if ((!no_overwrite_) || (no_overwrite_ && i != current_)) {
      branch_ptr_->GetEntry(i);
    }
    current_ = i;
  }
  inline void SetBranchPtr(TBranch* ptr) { branch_ptr_ = ptr; }
  inline TBranch* GetBranchPtr() { return branch_ptr_; }
  inline void SetNoOverwrite(bool const& flag) { no_overwrite_ = flag; }

 private:
  TBranch* branch_ptr_;
  int64_t current_;
  bool no_overwrite_;
};
}

#endif
