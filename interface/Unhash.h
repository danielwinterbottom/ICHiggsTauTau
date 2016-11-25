#ifndef ICHiggsTauTau_Objects_Unhash_h
#define ICHiggsTauTau_Objects_Unhash_h

#include <map>
#include <string>

namespace ic {

class Unhash {
 public:
  static std::string Get(std::size_t id);
  static void SetMap(std::map<std::size_t, std::string> const& unhash_map);
  ~Unhash();

 private:
  static std::map<std::size_t, std::string> unhash_map_;
};

}

#endif
