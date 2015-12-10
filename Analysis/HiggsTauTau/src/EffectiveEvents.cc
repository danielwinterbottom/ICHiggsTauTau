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
outtree_->Branch("gen_ht",&gen_ht_);
return 0;
}

int EffectiveEvents::Execute(TreeEvent *event){
 EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");

  //if (eventInfo->weight_defined("wt_mc_sign")) mcsign_ = eventInfo->weight("wt_mc_sign"); else mcsign_ = 1.0;
  if (eventInfo->weight_defined("wt_mc_sign")) mcsign_ = eventInfo->weight("wt_mc_sign"); else mcsign_= 1;
  gen_ht_ = eventInfo->gen_ht();
//  mcsign_ = eventInfo->weight;
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

