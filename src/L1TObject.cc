#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"

namespace ic {
  L1TObject::L1TObject() {;}
  L1TObject::~L1TObject() {;}
  
  void L1TObject::Print() const {
    std::cout << " -- bx " << bx_ << " ";
    Candidate::Print();
  }
  
}
