#ifndef ICHiggsTauTau_Core_TreeEvent_h
#define ICHiggsTauTau_Core_TreeEvent_h

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <stdexcept>
#include <functional>
#include "boost/format.hpp"
#include "Core/interface/Event.h"
#include "Core/interface/BranchHandler.h"
#include "Core/interface/BranchHandlerBase.h"
class TTree;

namespace ic {

class TreeEvent : public Event {
 private:
  std::map<std::string, BranchHandlerBase*> handlers_;
  std::map<std::string, std::function<void(int64_t)> > cached_funcs_;

  std::vector<std::function<void(int64_t)> > auto_add_funcs_;
  TTree* tree_;
  int64_t event_;

  std::set<std::string> branch_names_;

  std::string FormMissingMessage(std::string const& name,
                                 std::string const& branch_name);

  template<class T>
  BranchHandler<T>* SetupHandler(std::string const& branch_name) {
    auto h_it = handlers_.find(branch_name);
    if (h_it == handlers_.end()) {
      BranchHandler<T>* handler =
          new BranchHandler<T>(tree_, branch_name);
      handler->SetAddress();
      handlers_[branch_name] = handler;
      return handler;
    } else {
      return dynamic_cast<BranchHandler<T>*>(h_it->second);
    }
  }

  template <class T>
  void CopyPtr(std::string const& prod_name, BranchHandler<T>* bh,
                   int64_t event) {
    bh->GetEntry(event);
    bh->SetNoOverwrite(true);
    Add(prod_name, bh->GetPtr());
  }

  template <class T>
  void CopyPtrVec(std::string const& prod_name,
                      BranchHandler<std::vector<T> >* bh,
                      int64_t event) {
    bh->GetEntry(event);
    bh->SetNoOverwrite(true);
    std::vector<T>* ptr = bh->GetPtr();
    std::vector<T*> temp_vec(ptr->size(), nullptr);
    for (unsigned i = 0; i < ptr->size(); ++i) {
      temp_vec[i] = &((*ptr)[i]);
    }
    Add(prod_name, temp_vec);
  }

  template <class T>
  void CopyIDMap(std::string const& prod_name,
                     BranchHandler<std::vector<T> >* bh, int64_t event) {
    bh->GetEntry(event);
    bh->SetNoOverwrite(true);
    std::vector<T>* ptr = bh->GetPtr();
    std::map<std::size_t, T*> temp_map;
    for (unsigned i = 0; i < ptr->size(); ++i) {
      temp_map[(*ptr)[i].id()] = &((*ptr)[i]);
    }
    Add(prod_name, temp_map);
  }

 public:
  TreeEvent();
  virtual ~TreeEvent();

  bool ExistsInTree(std::string const& branch_name);
  bool ExistsInEvent(std::string const& name);

  template <class T>
  void AutoAddPtr(std::string const& name, std::string branch_name = "") {
    // Check if a branch handler already exists with this branch_name
    if (branch_name == "") branch_name = name;
    if (ExistsInTree(branch_name)) {
      // Return the BranchHandler for this branch, creating it if
      // necessary (will fail if SetupHandler dynamic_cast fails)
      BranchHandler<T>* bh = SetupHandler<T>(branch_name);
      // Create and store a function to extract the product from the tree
      auto_add_funcs_.push_back(std::bind(&TreeEvent::CopyPtr<T>, this,
                                          name, bh, std::placeholders::_1));
    }
  }

  template <class T>
  void AutoAddPtrVec(std::string const& name,
                     std::string branch_name = "") {
    typedef std::vector<T> Vector_T;
    // Check if a branch handler already exists with this branch_name
    if (branch_name == "") branch_name = name;
    if (ExistsInTree(branch_name)) {
      // Return the BranchHandler for this branch, creating it if
      // necessary (will fail if SetupHandler dynamic_cast fails)
      BranchHandler<Vector_T>* bh = SetupHandler<Vector_T>(branch_name);
      // Creat and store a function to extract the product from the tree
      auto_add_funcs_.push_back(std::bind(&TreeEvent::CopyPtrVec<T>, this,
                                          name, bh, std::placeholders::_1));
    }
  }

  template <class T>
  void AutoAddIDMap(std::string const& name,
                    std::string branch_name = "") {
    typedef std::vector<T> Vector_T;
    // Check if a branch handler already exists with this branch_name
    if (branch_name == "") branch_name = name;
    if (ExistsInTree(branch_name)) {
      // Return the BranchHandler for this branch, creating it if
      // necessary (will fail if SetupHandler dynamic_cast fails)
      BranchHandler<Vector_T>* bh = SetupHandler<Vector_T>(branch_name);
      // Creat and store a function to extract the product from the tree
      auto_add_funcs_.push_back(std::bind(&TreeEvent::CopyIDMap<T>, this,
                                          name, bh, std::placeholders::_1));
    }
  }

  template <class T>
  T* GetPtr(std::string const& name, std::string branch_name = "") {
    // If the product is already in the event: just return it
    if (ExistsInEvent(name)) return Get<T*>(name);
    // If a function to extract the product from the tree exists:
    // Call the function to get the product in the event, then return it
    auto fn_it = cached_funcs_.find(name);
    if (fn_it != cached_funcs_.end()) {
      fn_it->second(event_);
      return Get<T*>(name);
    }
    // If the product is in the tree:
    if (branch_name == "") branch_name = name;
    if (ExistsInTree(branch_name)) {
      // Return the BranchHandler for this branch, creating it if
      // necessary (will fail if SetupHandler dynamic_cast fails)
      BranchHandler<T>* bh = SetupHandler<T>(branch_name);
      // Create and store a function to extract the product from the tree
      cached_funcs_[name] = std::bind(&TreeEvent::CopyPtr<T>, this, name,
                                      bh, std::placeholders::_1);
      // Call this new function for the current event
      cached_funcs_[name](event_);
      return Get<T*>(name);
    } else {
      // If we get to here we don't have the product, so throw an exception
      throw std::runtime_error(FormMissingMessage(name, branch_name));
    }
  }

  template <class T>
  std::vector<T*>& GetPtrVec(std::string const& name,
                             std::string branch_name = "") {
    typedef std::vector<T*> Vector_TP;
    typedef std::vector<T> Vector_T;
    // If the product is already in the event: just return it
    if (ExistsInEvent(name)) return Get<Vector_TP>(name);
    // If a function to extract the product from the tree exists:
    // Call the function to get the product in the event, then return it
    auto fn_it = cached_funcs_.find(name);
    if (fn_it != cached_funcs_.end()) {
      fn_it->second(event_);
      return Get<Vector_TP>(name);
    }
    // If the product is in the tree:
    if (branch_name == "") branch_name = name;
    if (ExistsInTree(branch_name)) {
      // Return the BranchHandler for this branch, creating it if
      // necessary (will fail if SetupHandler dynamic_cast fails)
      BranchHandler<Vector_T>* bh = SetupHandler<Vector_T>(branch_name);
      // Creat and store a function to extract the product from the tree
      cached_funcs_[name] = std::bind(&TreeEvent::CopyPtrVec<T>, this, name,
                                      bh, std::placeholders::_1);
      // Call this new function for the current event
      cached_funcs_[name](event_);
      return Get<Vector_TP>(name);
    } else {
      // If we get to here we don't have the product, so throw an exception
      throw std::runtime_error(FormMissingMessage(name, branch_name));
    }
  }

  template <class T>
  std::map<std::size_t, T*>& GetIDMap(std::string const& name,
                                      std::string branch_name = "") {
    typedef std::map<std::size_t, T*> Map_TP;
    typedef std::vector<T> Vector_T;
    // If the product is already in the event: just return it
    if (ExistsInEvent(name)) return Get<Map_TP>(name);
    // If a function to extract the product from the tree exists:
    // Call the function to get the product in the event, then return it
    auto fn_it = cached_funcs_.find(name);
    if (fn_it != cached_funcs_.end()) {
      fn_it->second(event_);
      return Get<Map_TP>(name);
    }
    // If the product is in the tree:
    if (branch_name == "") branch_name = name;
    if (ExistsInTree(branch_name)) {
      // Return the BranchHandler for this branch, creating it if
      // necessary (will fail if SetupHandler dynamic_cast fails)
      BranchHandler<Vector_T>* bh = SetupHandler<Vector_T>(branch_name);
      // Creat and store a function to extract the product from the tree
      cached_funcs_[name] = std::bind(&TreeEvent::CopyIDMap<T>, this, name,
                                      bh, std::placeholders::_1);
      // Call this new function for the current event
      cached_funcs_[name](event_);
      return Get<Map_TP>(name);
    } else {
      // If we get to here we don't have the product, so throw an exception
      throw std::runtime_error(FormMissingMessage(name, branch_name));
    }
  }

  void SetEvent(int64_t event);

  void SetTree(TTree* tree);
  void DeleteAndClearHandlers();

  virtual void List();
};
}

#endif
