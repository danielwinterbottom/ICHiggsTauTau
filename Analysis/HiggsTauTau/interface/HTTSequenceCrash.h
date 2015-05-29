#ifndef ICHiggsTauTau_HiggsTauTau_HTTSequenceCrash_h
#define ICHiggsTauTau_HiggsTauTau_HTTSequenceCrash_h

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

class HTTSequenceCrash {
 private:
  // We have to be very careful that fs isn't deleted before we've finished
  // running the analysis because it will close the file that it manages.
  // Therefore putting 'delete fs' in the destructor of this class is dangerous
  // because it could well be copied somewhere. Instead we'll use a shared_ptr
  // that will ensure it's persisted.
 std::shared_ptr<fwlite::TFileService> fs;
 std::vector<std::shared_ptr<ic::ModuleBase>> seq;
 //std::vector<std::shared_ptr<ic::ModuleBase>> *seq = new std::vector<std::shared_ptr<ic::ModuleBase>>;
 std::string channel_str;
 Json::Value js;
 
 public:
  typedef std::vector<std::shared_ptr<ic::ModuleBase>> ModuleSequence;
  HTTSequenceCrash(std::string& chan,std::string& var, Json::Value const& js);
  HTTSequenceCrash() = default; // no harm in having a default constructor
  ~HTTSequenceCrash();
  ModuleSequence* getSequence(){return &seq;}
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
