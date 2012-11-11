#ifndef ICHiggsTauTau_StaticTree_hh
#define ICHiggsTauTau_StaticTree_hh
#include "TTree.h"
#include <boost/functional/hash.hpp>

namespace ic {

struct StaticTree{
    static TTree *tree_;
};


}

#endif