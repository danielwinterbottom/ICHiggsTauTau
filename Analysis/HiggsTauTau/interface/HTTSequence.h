#ifndef ICHiggsTauTau_HiggsTauTau_HTTSequence_h
#define ICHiggsTauTau_HiggsTauTau_HTTSequence_h

#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include "Core/interface/ModuleBase.h"
#include "Utilities/interface/json.h"
#include "HiggsTauTau/interface/HTTConfig.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"

namespace ic {

class HTTSequence {
 public:
  typedef std::vector<std::shared_ptr<ic::ModuleBase>> ModuleSequence;
  void BuildSequence(ModuleSequence* seq, ic::channel channel, fwlite::TFileService *fs,
                     Json::Value const& js);
  void BuildETPairs(ModuleSequence* seq, Json::Value const& js);
  void BuildMTPairs(ModuleSequence* seq, Json::Value const& js);
  void BuildEMPairs(ModuleSequence* seq, Json::Value const& js);

  void BuildTauSelection(ModuleSequence* seq, Json::Value const& js);

  void BuildDiElecVeto(ModuleSequence* seq, Json::Value const& js);
  void BuildDiMuonVeto(ModuleSequence* seq, Json::Value const& js);

  void BuildExtraMuonVeto(ModuleSequence* seq, Json::Value const& js);
  void BuildExtraElecVeto(ModuleSequence* seq, Json::Value const& js);

  template<class T>
  void BuildModule(ModuleSequence* seq, T const& mod) {
     seq->push_back(std::shared_ptr<ModuleBase>(new T(mod)));
  }
};
}

#endif
