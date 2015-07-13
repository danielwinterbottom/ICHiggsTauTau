#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/EffectiveEvents.h"

namespace ic {
EffectiveEvents::EffectiveEvents(std::string const& name) : ModuleBase(name){
fs_ = NULL;
}

EffectiveEvents::~EffectiveEvents(){
;
}

int EffectiveEvents::PreAnalysis(){
outtree_ = fs_->make<TTree>("effective","effective");
outtree_->Branch("wt",&mcsign_);
return 0;
}

int EffectiveEvents::Execute(TreeEvent *event){
 EventInfo const* eventInfo;
  try{
eventInfo = event->GetPtr<EventInfo>("eventInfo");
 } catch(...){
  std::cout<<"eventInfo doesn't exist"<<std::endl;
  return 0;}

  //if (eventInfo->weight_defined("wt_mc_sign")) mcsign_ = eventInfo->weight("wt_mc_sign"); else mcsign_ = 1.0;
  mcsign_ = eventInfo->total_weight();
//  if (eventInfo->weight_defined("wt_mc_sign")) mcsign_ = eventInfo->weight("wt_mc_sign"); else mcsign_ = 1.0;
//std::cout<<mcsign_<<std::endl;
 
 outtree_->Fill();
 return 0;
 }

int EffectiveEvents::PostAnalysis(){
 return 0;
 }

 void EffectiveEvents::PrintInfo(){
;
}
}

