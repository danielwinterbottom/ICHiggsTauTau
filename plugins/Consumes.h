#ifndef UserCode_ICHiggsTauTau_Consumes_h
#define UserCode_ICHiggsTauTau_Consumes_h
#include <memory>
#include <vector>
#include <string>
#include "FWCore/Utilities/interface/InputTag.h"

//Need this for CMSSW_5_3_X, which doesn't have a consumes method.
//In CMSSW 53X it won't actually do anything, except for making sure wwe don't 
//have to add #if CMSSW_MAJOR_VERSION ... in every single module.
#if CMSSW_MAJOR_VERSION < 6
template<typename T>
void consumes(edm::InputTag const& in){
//*omnomnomnom*
 } 

//Need this part to be able to use an edm::ConsumesCollector for consumes
//calls in helper structs in CMSSW 76X without crashing CMSSW 53X compilation
namespace edm{

struct ConsumesCollector
{
 template<typename T>
 void consumes(edm::InputTag const& in){
}
  };

}
edm::ConsumesCollector consumesCollector();
#endif
#if CMSSW_MAJOR_VERSION >= 6
#include "FWCore/Framework/interface/ConsumesCollector.h"
#endif
#endif
