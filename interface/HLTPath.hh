#ifndef ICHiggsTauTau_HLTPath_hh
#define ICHiggsTauTau_HLTPath_hh
#include <vector>
#include "Rtypes.h"
#include <iostream>

namespace ic {

class HLTPath {

 public:
  HLTPath();
  virtual ~HLTPath();
  virtual void Print() const;

 #ifndef SKIP_CINT_DICT
 public:
  ClassDef(HLTPath, 1);
 #endif
  
 public:
  std::string path;  
  std::string leg1_filter;
  std::string leg2_filter;
  std::string leg2_extra;
  bool singleLepton_;
  bool pass;
  
};

typedef std::vector<ic::HLTPath> HLTPathCollection;
}

#endif
