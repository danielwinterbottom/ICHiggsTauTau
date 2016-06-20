#include "../interface/Unhash.h"
#include <string>
#include <map>
#include "boost/format.hpp"
#include "boost/lexical_cast.hpp"

namespace ic {

std::map<std::size_t, std::string> Unhash::unhash_map_;

std::string Unhash::Get(std::size_t id) {
  auto it = unhash_map_.find(id);
  if (it != unhash_map_.end()) {
    return it->second;
  } else {
    return boost::lexical_cast<std::string>(id);
  }
}

void Unhash::SetMap(std::map<std::size_t, std::string> const& unhash_map) {
  unhash_map_ = unhash_map;
}



Unhash::~Unhash() { ; }


}