#ifndef ICHiggsTauTau_HiggsTauTau_TagAndProbe_h
#define ICHiggsTauTau_HiggsTauTau_TagAndProbe_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"

#include <string>

namespace ic {
template <class T>  
class TagAndProbe : public ModuleBase {
 private:
  CLASS_MEMBER(TagAndProbe, std::string, ditau_label)
  CLASS_MEMBER(TagAndProbe, ic::channel, channel)
  CLASS_MEMBER(TagAndProbe, ic::strategy, strategy)
  CLASS_MEMBER(TagAndProbe, fwlite::TFileService*, fs)
  CLASS_MEMBER(TagAndProbe, std::string, tag_trg_objects)
  CLASS_MEMBER(TagAndProbe, std::string, tag_trg_filters)
  CLASS_MEMBER(TagAndProbe, std::string, probe_trg_objects)
  CLASS_MEMBER(TagAndProbe, std::string, probe_trg_filters)
  CLASS_MEMBER(TagAndProbe, std::function<bool(T)>, probe_id)
  CLASS_MEMBER(TagAndProbe, std::function<bool(T)>, tag_id)
  
  TTree *outtree_;
  
  unsigned long long event_;
  double pt_1_;
  double pt_2_;
  double eta_1_;
  double eta_2_;
  double phi_1_;
  double phi_2_;
  double iso_1_;
  double iso_2_;
  double dR_;
  double dphi_;
  double deta_;
  double m_vis_;
  double wt_;
  bool os_;
  bool trg_tag_1_;
  bool trg_tag_2_;
  bool trg_probe_1_;
  bool trg_probe_2_;
  bool id_probe_1_;
  bool id_probe_2_;
  bool id_tag_1_;
  bool id_tag_2_;
  
  std::vector<std::string> SplitString(std::string instring){
    std::vector<std::string> outstrings;
    std::stringstream ss(instring);   
    std::string splitstring;  
    while(std::getline(ss, splitstring, ',')) outstrings.push_back(splitstring); 
    return outstrings;
  }

 public:  
  TagAndProbe(std::string const& name);
  virtual ~TagAndProbe();
  
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  
};

template <class T> 
TagAndProbe<T>::TagAndProbe(std::string const& name) : ModuleBase(name),
  channel_(channel::tpzmm), 
  strategy_(strategy::mssmsummer16) {
  ditau_label_ = "ditau";
  fs_ = NULL;
}

template <class T>
TagAndProbe<T>::~TagAndProbe() {
  ;
}

template <class T> 
int TagAndProbe<T>::PreAnalysis() {
  if(fs_){  
    outtree_ = fs_->make<TTree>("tagandprobe","tagandprobe");
    outtree_->Branch("event" , &event_ );
    outtree_->Branch("wt"    , &wt_    );
    outtree_->Branch("pt_1"  , &pt_1_  );
    outtree_->Branch("pt_2"  , &pt_2_  );
    outtree_->Branch("iso_1"  , &iso_1_ );
    outtree_->Branch("iso_2"  , &iso_2_ );
    outtree_->Branch("id_probe_1"  , &id_probe_1_  );
    outtree_->Branch("id_probe_2"  , &id_probe_2_  );
    outtree_->Branch("id_tag_1"  , &id_tag_1_  );
    outtree_->Branch("id_tag_2"  , &id_tag_2_  );
    outtree_->Branch("eta_1" , &eta_1_ );
    outtree_->Branch("eta_2" , &eta_2_ );
    outtree_->Branch("phi_1" , &phi_1_ );
    outtree_->Branch("phi_2" , &phi_2_ );
    outtree_->Branch("m_vis" , &m_vis_ );
    outtree_->Branch("dR"    , &dR_    );
    outtree_->Branch("dphi"  , &dphi_  );
    outtree_->Branch("deta"  , &deta_  );
    outtree_->Branch("os"    , &os_    );
    outtree_->Branch("trg_probe_1" , &trg_probe_1_    );
    outtree_->Branch("trg_probe_2" , &trg_probe_2_    );
    outtree_->Branch("trg_tag_1" , &trg_tag_1_    );
    outtree_->Branch("trg_tag_2" , &trg_tag_2_    );
  }    
  return 0;
}

template <class T>
int TagAndProbe<T>::Execute(TreeEvent *event){
    
  EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
  event_ = (unsigned long long) eventInfo->event();
  wt_ = eventInfo->total_weight();
  
  std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>(ditau_label_);
  CompositeCandidate const* ditau = ditau_vec.at(0);
  Candidate const* lep1 = ditau->GetCandidate("lepton1");
  Candidate const* lep2 = ditau->GetCandidate("lepton2");
  
  m_vis_ = ditau->M();
  pt_1_ = lep1->pt();
  pt_2_ = lep2->pt();
  eta_1_ = lep1->eta();
  eta_2_ = lep2->eta();
  phi_1_ = lep1->phi();
  phi_2_ = lep2->phi();
  dphi_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(lep1->vector(),lep2->vector()));
  deta_ = eta_1_-eta_2_;
  dR_ = std::fabs(ROOT::Math::VectorUtil::DeltaR(lep1->vector(),lep2->vector()));
  os_ = PairOppSign(ditau);
  
  trg_tag_1_ = false;
  trg_tag_2_ = false;
  trg_probe_1_ = false;
  trg_probe_2_ = false;
  
  std::vector<std::string> tag_objs = SplitString(tag_trg_objects_);
  std::vector<std::string> tag_filts = SplitString(tag_trg_filters_);    
  std::vector<std::string> probe_objs = SplitString(probe_trg_objects_);
  std::vector<std::string> probe_filts = SplitString(probe_trg_filters_);
  
  if(tag_objs.size() != tag_filts.size()){std::cout << "Number of tag trigger objects does not match the number of tag filters, throwing excpetion!" << std::endl; throw;}
  if(probe_objs.size() != probe_filts.size()){std::cout << "Number of probe trigger objects does not match the number of probe filters, throwing excpetion!" << std::endl; throw;}
  
  std::vector<TriggerObject *> objs_tag;
  for(unsigned i=0; i<tag_objs.size(); ++i){
    objs_tag = event->GetPtrVec<TriggerObject>(tag_objs[i]);
    trg_tag_1_ = IsFilterMatched(ditau->At(0), objs_tag, tag_filts[i], 0.5) || trg_tag_1_;
    trg_tag_2_ = IsFilterMatched(ditau->At(1), objs_tag, tag_filts[i], 0.5) || trg_tag_2_;
  }
  
  std::vector<TriggerObject *> objs_probe;
  for(unsigned i=0; i<probe_objs.size(); ++i){
    objs_probe = event->GetPtrVec<TriggerObject>(probe_objs[i]);
    trg_probe_1_ = IsFilterMatched(ditau->At(0), objs_probe, probe_filts[i], 0.5) || trg_probe_1_;
    trg_probe_2_ = IsFilterMatched(ditau->At(1), objs_probe, probe_filts[i], 0.5) || trg_probe_2_;
  }
  
  if(channel_ == channel::tpzmm){
    if(strategy_ == strategy::mssmsummer16){
      T muon1 = dynamic_cast<T>(lep1);
      T muon2 = dynamic_cast<T>(lep2);
      iso_1_ = PF04IsolationVal(muon1, 0.5, 0);
      iso_2_ = PF04IsolationVal(muon2, 0.5, 0);
      id_tag_1_ = tag_id_(muon1);
      id_tag_2_ = tag_id_(muon2);
      id_probe_1_ = probe_id_(muon1);
      id_probe_2_ = probe_id_(muon2);
    }
  }
  if(channel_ == channel::tpzee){
    if(strategy_ == strategy::mssmsummer16){
      T elec1 = dynamic_cast<T>(lep1);
      T elec2 = dynamic_cast<T>(lep2);
      iso_1_ = PF03IsolationVal(elec1, 0.5, 0);
      iso_2_ = PF03IsolationVal(elec2, 0.5, 0);
      id_tag_1_ = tag_id_(elec1);
      id_tag_2_ = tag_id_(elec2);
      id_probe_1_ = probe_id_(elec1);
      id_probe_2_ = probe_id_(elec2);
    } 
  }
  
  if(fs_) outtree_->Fill();
  
  return 0;
}
template <class T>
int TagAndProbe<T>::PostAnalysis(){  
  return 0;
}
template <class T>
void TagAndProbe<T>::PrintInfo(){  
  ;
}


}

#endif
