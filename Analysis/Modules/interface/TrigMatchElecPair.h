#ifndef ICHiggsTauTau_Module_TrigMatchElecPair_h
#define ICHiggsTauTau_Module_TrigMatchElecPair_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"

#include <string>

namespace ic {


class TrigMatchElecPair : public ModuleBase {
 private:
  std::string input_name_;

 public:

  TrigMatchElecPair(std::string const& name, std::string input_name);
  virtual ~TrigMatchElecPair();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  bool IsMatched(CompositeCandidate const& cand, std::size_t const& hash) const;

};

}

#endif
