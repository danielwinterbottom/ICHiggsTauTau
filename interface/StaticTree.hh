#ifndef ICHiggsTauTau_StaticTree_hh
#define ICHiggsTauTau_StaticTree_hh
#include "TTree.h"
#include "boost/functional/hash.hpp"

namespace ic {

/**
 * @brief Defines a small struct containing a static TTree pointer, thus
 *providing a mechanism for each producer to add a branch to the same tree.
 */
struct StaticTree {
  static TTree *tree_;
};
}

#endif
