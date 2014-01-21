#ifndef ICHiggsTauTau_Analysis_BranchHandlerBase_h
#define ICHiggsTauTau_Analysis_BranchHandlerBase_h

#include "TBranch.h"

namespace ic {

  //! An abstract base class for the automatic handling of branches in a TTree
  /*! 
   This class holds a pointer to a TBranch and is the base class for BranchHander which is templated
   on the type of the class stored in the branch.
    */
  class BranchHandlerBase{
    private:
      TBranch* branch_ptr_;

    public:
      virtual void SetAddress() = 0;
      void GetEntry(unsigned i){
        branch_ptr_->GetEntry(i);
      }
      void SetBranchPtr(TBranch* ptr){
        branch_ptr_ = ptr;
      }

      TBranch* GetBranchPtr(){
        return branch_ptr_;
      }

      virtual ~BranchHandlerBase();

  };
}

#endif
