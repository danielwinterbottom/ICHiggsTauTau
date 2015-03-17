#ifndef ICHiggsTauTau_Module_HTTPairStudy_h
#define ICHiggsTauTau_Module_HTTPairStudy_h

#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Utilities/interface/HistoSet.h"
#include "HiggsTauTau/interface/HTTConfig.h"
#include "boost/range/algorithm_ext/erase.hpp"

#include <string>
#include <functional>


namespace ic {

template <class Container, class Pred>
Container copy_keep_if(Container& target, Pred pred) {
  Container res = target;
  keep_if(res, pred);
  return res;
}

template <class Container, class Pred>
Container& keep_if(Container& target, Pred pred) {
  return boost::remove_erase_if(target, [&](typename Container::value_type const& x) {
    return !pred(x);
  });
}


class HTTPairStudy : public ModuleBase {
 private:
  unsigned et_loose_pairs = 0;
  unsigned et_loose_pairs_could = 0;
  unsigned et_loose_matched = 0;
  unsigned et_loose_passed = 0;

 public:
  HTTPairStudy(std::string const& name);
  virtual ~HTTPairStudy();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}

#endif
