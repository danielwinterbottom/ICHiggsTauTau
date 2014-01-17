#ifndef ICHiggsTauTau_Analysis_BranchHandler_h
#define ICHiggsTauTau_Analysis_BranchHandler_h

#include "Core/interface/BranchHandlerBase.h"
#include <string>
#include <iostream>
#include "TTree.h"
class TBranch;

namespace ic {

//! A class for the automatic handling of branches in a TTree
/*! 
 This template class manages the retrieval of objects of a specific type from the TBranch
 of a TTree.
  */
  template <class T>
    class BranchHandler : public BranchHandlerBase{
      private:
        T* ptr_;

      public:
        BranchHandler(){
          SetBranchPtr(0);
          ptr_ = 0;
        }
        BranchHandler(TTree* tree, std::string const& branch_name){
          TBranch *branch_ptr = tree->GetBranch(branch_name.c_str());
          if (!branch_ptr) {
            std::cerr << "Error in <BranchHandler>: Branch \"" << branch_name <<
              "\" not found in tree, an exception will be thrown." << std::endl;
            throw;
          }

          SetBranchPtr(tree->GetBranch(branch_name.c_str()));
          ptr_ = 0;
        }
        void SetAddress(){
          GetBranchPtr()->SetAddress(&ptr_);
        }
        T* & GetPtr(){
          return ptr_;
        }

        virtual ~BranchHandler() {
          delete ptr_;
        }
    };

}


#endif
