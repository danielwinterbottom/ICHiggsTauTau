#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/EffectiveEvents.h"

namespace ic {
EffectiveEvents::EffectiveEvents(std::string const& name) : ModuleBase(name){
fs_ = NULL;
do_qcd_scale_wts_=false;
}

EffectiveEvents::~EffectiveEvents(){
;
}

int EffectiveEvents::PreAnalysis(){
outtree_ = fs_->make<TTree>("effective","effective");
outtree_->Branch("wt",&mcsign_);
outtree_->Branch("gen_ht",&gen_ht_);
if(do_qcd_scale_wts_){
  outtree_->Branch("wt_mur1_muf1",    &scale1_);
  outtree_->Branch("wt_mur1_muf2",    &scale2_);
  outtree_->Branch("wt_mur1_muf0p5",  &scale3_);
  outtree_->Branch("wt_mur2_muf1",    &scale4_);
  outtree_->Branch("wt_mur2_muf2",    &scale5_);
  outtree_->Branch("wt_mur2_muf0p5",  &scale6_);
  outtree_->Branch("wt_mur0p5_muf1",  &scale7_);
  outtree_->Branch("wt_mur0p5_muf2",  &scale8_);
  outtree_->Branch("wt_mur0p5_muf0p5",&scale9_);
}
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
 if(do_qcd_scale_wts_){
   // note some of these labels may be generator dependent so need to make sure you check before using them
   if(eventInfo->weight_defined("1001")) scale1_ = eventInfo->weight("1001")*mcsign_; else scale1_=mcsign_;
   if(eventInfo->weight_defined("1002")) scale2_ = eventInfo->weight("1002")*mcsign_; else scale2_=mcsign_;
   if(eventInfo->weight_defined("1003")) scale3_ = eventInfo->weight("1003")*mcsign_; else scale3_=mcsign_;
   if(eventInfo->weight_defined("1004")) scale4_ = eventInfo->weight("1004")*mcsign_; else scale4_=mcsign_;
   if(eventInfo->weight_defined("1005")) scale5_ = eventInfo->weight("1005")*mcsign_; else scale5_=mcsign_;
   if(eventInfo->weight_defined("1006")) scale6_ = eventInfo->weight("1006")*mcsign_; else scale6_=mcsign_;
   if(eventInfo->weight_defined("1007")) scale7_ = eventInfo->weight("1007")*mcsign_; else scale7_=mcsign_;
   if(eventInfo->weight_defined("1008")) scale8_ = eventInfo->weight("1008")*mcsign_; else scale8_=mcsign_;
   if(eventInfo->weight_defined("1009")) scale9_ = eventInfo->weight("1009")*mcsign_; else scale9_=mcsign_;    
 }
 
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

