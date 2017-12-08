#ifndef ICHiggsTauTau_HiggsTauTau_HTTGenEventInfo_h
#define ICHiggsTauTau_HiggsTauTau_HTTGenEventInfo_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"

#include <string>

namespace ic {
  
class HTTGenEventInfo : public ModuleBase {
 private:

  CLASS_MEMBER(HTTGenEventInfo, std::string, theta);  
  CLASS_MEMBER(HTTGenEventInfo, std::string, process_dir);
  CLASS_MEMBER(HTTGenEventInfo, std::string, param_card);
  CLASS_MEMBER(HTTGenEventInfo, std::string, param_card_sample);
  CLASS_MEMBER(HTTGenEventInfo, double, theta_sample);
     
  std::vector<std::pair<std::string,double>> theta_vec_;

 public:
  HTTGenEventInfo(std::string const& name);
  virtual ~HTTGenEventInfo();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo(); 


};

}

#endif
