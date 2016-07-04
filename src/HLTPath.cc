
#include "../interface/HLTPath.hh"

namespace ic {
  HLTPath::HLTPath() {}
  
  HLTPath::~HLTPath() {}
  
  void HLTPath::Print() const {
    std::cout << "HLT path name: " << path << " passed: " << pass << std::endl;  
  }
}