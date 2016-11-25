#ifndef ICHiggsTauTau_Analysis_BranchHandler_h
#define ICHiggsTauTau_Analysis_BranchHandler_h
#include <type_traits>
#include <stdexcept>
#include <string>
#include "TTree.h"
#include "Core/interface/BranchHandlerBase.h"

class TBranch;

namespace ic {

/**
 * @brief A class that manages the retrieval of an object from a TBranch
 * @tparam T The type of the object that should be retrieved
 * @tparam X = std::is_fundamental<T>::value Allows for a specialised version of
 * the class when the branch stores a fundamental type
 */
template <class T, bool X = std::is_fundamental<T>::value>
class BranchHandler : public BranchHandlerBase {
 private:
  T* ptr_;

 public:
  BranchHandler() : BranchHandlerBase(), ptr_(nullptr) {}

  BranchHandler(TTree* tree, std::string const& branch_name)
      : BranchHandlerBase(), ptr_(nullptr) {
    if (tree) {
      TBranch* branch_ptr = tree->GetBranch(branch_name.c_str());
      if (branch_ptr) this->SetBranchPtr(branch_ptr);
    }
    if (!GetBranchPtr())
      throw std::runtime_error(
          "BranchHandler could not extract TBranch from TTree");
  }

  void SetAddress() { GetBranchPtr()->SetAddress(&ptr_); }
  T* GetPtr() { return ptr_; }

  virtual ~BranchHandler() { delete ptr_; }
};

template <class T>
class BranchHandler<T, true> : public BranchHandlerBase {
 private:
  T obj_;

 public:
  BranchHandler() : BranchHandlerBase(), obj_(T()) {
    SetBranchPtr(0);
  }

  BranchHandler(TTree* tree, std::string const& branch_name)
      : BranchHandlerBase(), obj_(T()) {
    if (tree) {
      TBranch* branch_ptr = tree->GetBranch(branch_name.c_str());
      if (branch_ptr) this->SetBranchPtr(branch_ptr);
    }
    if (!GetBranchPtr())
      throw std::runtime_error(
          "BranchHandler could not extract TBranch from TTree");
  }

  void SetAddress() { GetBranchPtr()->SetAddress(&obj_); }
  T* GetPtr() { return &obj_; }

  virtual ~BranchHandler() { }
};
}

#endif
