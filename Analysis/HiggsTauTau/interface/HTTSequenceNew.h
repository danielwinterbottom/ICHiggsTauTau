#ifndef ICHiggsTauTau_HiggsTauTau_HTTSequenceNew_h
#define ICHiggsTauTau_HiggsTauTau_HTTSequenceNew_h

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

class HTTSequenceNew {
 private:
 fwlite::TFileService *fs;
 std::string channel_str;
 std::vector<std::shared_ptr<ic::ModuleBase>> seq;
 Json::Value js;
 
 public:
  typedef std::vector<std::shared_ptr<ic::ModuleBase>> ModuleSequence;
  HTTSequenceNew(std::string& chan,std::string& var, Json::Value const& js);
  ~HTTSequenceNew();
  ModuleSequence getSequence(){return seq;}
  void BuildSequence();
  void BuildETPairs();
  void BuildMTPairs();
  void BuildEMPairs();

  void BuildTauSelection();

  void BuildDiElecVeto();
  void BuildDiMuonVeto();

  void BuildExtraMuonVeto();
  void BuildExtraElecVeto();

  template<class T>
  void BuildModule(T const& mod) {
     seq.push_back(std::shared_ptr<ModuleBase>(new T(mod)));
  }
};
}

#endif
