#include "Core/interface/Event.h"

#include <typeinfo>
#include <utility>
#include <string>
#include <map>
#include <cxxabi.h>
#include "boost/format.hpp"

namespace ic {

  Event::Event() {
    ;
  }

  Event::~Event() {
    ;
  }

  bool Event::Exists(std::string const& name) {
    if (products_.count(name)) {
      return true;
    } else {
      return false;
    }
  }

  void Event::List() {
    std::map<std::string, boost::any>::const_iterator it;
    for (it = products_.begin(); it != products_.end(); ++it) {
      int status;
      std::string realname = abi::__cxa_demangle(it->second.type().name(), 0, 0, &status);
      std::cout << boost::format("%-30s %-30s\n") % it->first % realname;
    }
  }

  void Event::Clear() {
    products_.clear();
  }

  unsigned int Event::Remove(std::string const& name) {
    if (!Exists(name)) {
      return 1;
    } else {
      products_.erase(name);
      return 0;
    }
  }
 
}