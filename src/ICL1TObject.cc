#include "UserCode/ICHiggsTauTau/interface/ICL1TObject.hh"

namespace ic {
  ICL1TObject::ICL1TObject() {;}
  ICL1TObject::~ICL1TObject() {;}
  
  void ICL1TObject::Print() const {Candidate::Print();}
  
}