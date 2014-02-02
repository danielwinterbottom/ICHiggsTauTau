#ifndef ICHiggsTauTau_CombineTools_MakeUnique_h
#define ICHiggsTauTau_CombineTools_MakeUnique_h
#include <memory>

namespace ic {

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
}

#endif
