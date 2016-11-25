#ifndef ICHiggsTauTau_Core_Event_h
#define ICHiggsTauTau_Core_Event_h

#include <stdexcept>
#include <map>
#include <string>
#include <iostream>
#include "boost/any.hpp"

namespace ic {

class Event {
 public:
  Event();
  virtual ~Event();

  template <class T>
  void Add(std::string name, T const& product) {
    if (!Exists(name)) {
      products_[name] = product;
    } else {
      throw std::runtime_error(
          "[ic::Event::Add] Product with name " + name + " already exists");
    }
  }

  template <class T>
  unsigned int ForceAdd(std::string name, T const& product) {
    products_[name] = product;
    return 0;
  }

  template <class T>
  T& Get(std::string const& name) {
    if (Exists(name)) {
      return boost::any_cast<T&>(products_[name]);
    } else {
      throw std::runtime_error(
          "[ic::Event::Get] No product with name " + name + " exists");
    }
  }

  virtual void List();

  void Clear();

  unsigned int Remove(std::string const& name);

  bool Exists(std::string const& name);

 private:
  std::map<std::string, boost::any> products_;
};
}

#endif
