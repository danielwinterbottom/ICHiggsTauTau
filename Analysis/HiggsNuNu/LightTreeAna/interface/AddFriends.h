#ifndef ICHiggsTauTau_HiggsNuNu_AddFriends_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_AddFriends_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class AddFriends : public LTModule{
    CLASS_MEMBER(AddFriends,std::string,frienddir)
    CLASS_MEMBER(AddFriends,std::string,friendtreename)
    CLASS_MEMBER(AddFriends,std::vector<std::string>,sets)
  public:
    AddFriends(std::string);
    virtual ~AddFriends();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
