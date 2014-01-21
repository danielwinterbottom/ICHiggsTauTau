#ifndef ICHiggsTauTau_Core_TreeEvent_h
#define ICHiggsTauTau_Core_TreeEvent_h

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include "boost/function.hpp"
#include "boost/bind.hpp"
#include "Core/interface/Event.h"
#include "Core/interface/BranchHandler.h"
#include "Core/interface/BranchHandlerBase.h"
class TTree;

namespace ic {

  class TreeEvent : public Event {

  private:
    std::map<std::string, BranchHandlerBase*> handlers_;
    std::map<std::string, boost::function<void (unsigned)> > cached_funcs_;


    std::vector<boost::function<void (unsigned)> > auto_add_funcs_;
    TTree *tree_;
    unsigned event_;

    template <class T>
    void Copy(std::string const& branch_name, std::string const& prod_name, unsigned event) {
      handlers_[branch_name]->GetEntry(event);
      T* ptr = (dynamic_cast<BranchHandler<T>* >(handlers_[branch_name]))->GetPtr();
      Add(prod_name, ptr);
    }

    template <class T>
    void CopyPtrVec(std::string const& branch_name, std::string const& prod_name, unsigned event) {
      handlers_[branch_name]->GetEntry(event);
      std::vector<T> *ptr = (dynamic_cast<BranchHandler<std::vector<T> >* >(handlers_[branch_name]))->GetPtr();
      std::vector<T *> temp_vec(ptr->size(), nullptr);
      for (unsigned i = 0; i < ptr->size(); ++i) {
        temp_vec[i] = &((*ptr)[i]);
      }
      Add(prod_name, temp_vec);
    }

    template <class T>
    void CopyIDMap(std::string const& branch_name, std::string const& prod_name, unsigned event) {
      handlers_[branch_name]->GetEntry(event);
      std::vector<T> *ptr = (dynamic_cast<BranchHandler<std::vector<T> >* >(handlers_[branch_name]))->GetPtr();
      std::map<std::size_t, T *> temp_map;
      for (unsigned i = 0; i < ptr->size(); ++i) {
        temp_map[(*ptr)[i].id()] = &((*ptr)[i]);
      }
      Add(prod_name, temp_map);
    }

  public:
    TreeEvent();
    virtual ~TreeEvent();

    template <class T>
    void AutoAddPtr(std::string const& branch_name, std::string prod_name = "") {
      // Check if a branch handler already exists with this branch_name
      if (handlers_.count(branch_name) == 0) {
        BranchHandler<T> * handler = new BranchHandler<T>(tree_, branch_name);
        handler->SetAddress();
        handlers_[branch_name] = handler;
      }
      if (prod_name == "") prod_name = branch_name;
      auto_add_funcs_.push_back(boost::bind(
        &TreeEvent::Copy<T>,this, branch_name, prod_name, _1));
    }

    template <class T>
    void AutoAddPtrVec(std::string const& branch_name, std::string prod_name = "") {
      // Check if a branch handler already exists with this branch_name
      if (handlers_.count(branch_name) == 0) {      
        BranchHandler<std::vector<T> > * handler = new BranchHandler<std::vector<T> >(tree_, branch_name);
        handler->SetAddress();
        handlers_[branch_name] = handler;
      }
      if (prod_name == "") prod_name = branch_name;
      auto_add_funcs_.push_back(boost::bind(
        &TreeEvent::CopyPtrVec<T>,this, branch_name, prod_name, _1));      
    }

    template <class T>
    void AutoAddIDMap(std::string const& branch_name, std::string prod_name = "") {
      // Check if a branch handler already exists with this branch_name
      if (handlers_.count(branch_name) == 0) {      
        BranchHandler<std::vector<T> > * handler = new BranchHandler<std::vector<T> >(tree_, branch_name);
        handler->SetAddress();
        handlers_[branch_name] = handler;
      }
      if (prod_name == "") prod_name = branch_name;
      auto_add_funcs_.push_back(boost::bind(
        &TreeEvent::CopyIDMap<T>,this, branch_name, prod_name, _1));      
    }


    template <class T>
    T & Get(std::string const& name, std::string branch_name = "") {
      //1. If the product already exists in the event, return it
      if (Exists(name)) {
        return Event::Get<T>(name);
      } else { //2. No - is a function cached for the product?
        if (cached_funcs_.count(name)) {
          cached_funcs_[name](event_);
          return Event::Get<T>(name);
        } else { //3. No - try and generate a cached function
          if (branch_name == "") branch_name = name;
          //4. If necessary, try and generate a branch handler first
          if (handlers_.count(branch_name) == 0) {
            BranchHandler<T> * handler = new BranchHandler<T>(tree_, branch_name);
            handler->SetAddress();
            handlers_[branch_name] = handler;
          }
          cached_funcs_[name] = boost::bind(
            &TreeEvent::Copy<T>,this, branch_name, name, _1);
          cached_funcs_[name](event_);
          return Event::Get<T>(name);
        }
      }
    }

    template <class T>
    T * GetPtr(std::string const& name, std::string branch_name = "") {
      //1. If the product already exists in the event, return it
      if (Exists(name)) {
        return Event::Get<T *>(name);
      } else { //2. No - is a function cached for the product?
        if (cached_funcs_.count(name)) {
          cached_funcs_[name](event_);
          return Event::Get<T *>(name);
        } else { //3. No - try and generate a cached function
          if (branch_name == "") branch_name = name;
          //4. If necessary, try and generate a branch handler first
          if (handlers_.count(branch_name) == 0) {
            BranchHandler<T> * handler = new BranchHandler<T>(tree_, branch_name);
            handler->SetAddress();
            handlers_[branch_name] = handler;
          }
          cached_funcs_[name] = boost::bind(
            &TreeEvent::Copy<T>,this, branch_name, name, _1);
          cached_funcs_[name](event_);
          return Event::Get<T *>(name);
        }
      }
    }

    template <class T>
    std::vector<T*> & GetPtrVec(std::string const& name, std::string branch_name = "") {
      //1. If the product already exists in the event, return it
      if (Exists(name)) {
        return Event::Get<std::vector<T*> >(name);
      } else { //2. No - is a function cached for the product?
        if (cached_funcs_.count(name)) {
          cached_funcs_[name](event_);
          return Event::Get<std::vector<T*> >(name);
        } else { //3. No - try and generate a cached function
          if (branch_name == "") branch_name = name;
          //4. If necessary, try and generate a branch handler first
          if (handlers_.count(branch_name) == 0) {
            BranchHandler<std::vector<T> > * handler = new BranchHandler<std::vector<T> >(tree_, branch_name);
            handler->SetAddress();
            handlers_[branch_name] = handler;
          }
          cached_funcs_[name] = boost::bind(
            &TreeEvent::CopyPtrVec<T>,this, branch_name, name, _1);
          cached_funcs_[name](event_);
          return Event::Get<std::vector<T*> >(name);
        }
      }
    }

    template <class T>
    std::map<std::size_t,T*> & GetIDMap(std::string const& name, std::string branch_name = "") {
      //1. If the product already exists in the event, return it
      if (Exists(name)) {
        return Event::Get<std::map<std::size_t,T*> >(name);
      } else { //2. No - is a function cached for the product?
        if (cached_funcs_.count(name)) {
          cached_funcs_[name](event_);
          return Event::Get<std::map<std::size_t,T*> >(name);
        } else { //3. No - try and generate a cached function
          if (branch_name == "") branch_name = name;
          //4. If necessary, try and generate a branch handler first
          if (handlers_.count(branch_name) == 0) {
            BranchHandler<std::vector<T> > * handler = new BranchHandler<std::vector<T> >(tree_, branch_name);
            handler->SetAddress();
            handlers_[branch_name] = handler;
          }
          cached_funcs_[name] = boost::bind(
            &TreeEvent::CopyIDMap<T>,this, branch_name, name, _1);
          cached_funcs_[name](event_);
          return Event::Get<std::map<std::size_t,T*> >(name);
        }
      }
    }


    void SetEvent(unsigned event);

    void SetTree(TTree *tree); 



  
    


  };
}

#endif
