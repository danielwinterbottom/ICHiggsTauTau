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
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/Photon.hh"
#include "Utilities/interface/FnPairs.h"

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
  CLASS_MEMBER(TagAndProbe, std::string, tag_add_trg_objects)
  CLASS_MEMBER(TagAndProbe, std::string, tag_add_trg_filters)
  CLASS_MEMBER(TagAndProbe, std::string, probe_trg_objects)
  CLASS_MEMBER(TagAndProbe, std::string, probe_trg_filters)
  CLASS_MEMBER(TagAndProbe, std::function<bool(T)>, probe_id)
  CLASS_MEMBER(TagAndProbe, std::function<bool(T)>, tag_id)
  CLASS_MEMBER(TagAndProbe, double, extra_l1_tag_pt)
  CLASS_MEMBER(TagAndProbe, double, extra_l1_probe_pt)
  CLASS_MEMBER(TagAndProbe, double, extra_l1_iso_probe_pt)
  CLASS_MEMBER(TagAndProbe, double, extra_hlt_probe_pt)
  CLASS_MEMBER(TagAndProbe, std::vector<double>, extra_hlt_probe_pt_vec)
  CLASS_MEMBER(TagAndProbe, bool, loose_iso_trgprobe)
  CLASS_MEMBER(TagAndProbe, bool, do_dzmass)
  CLASS_MEMBER(TagAndProbe, bool, do_extra)
 
  TTree *outtree_;
  
  unsigned long long event_;
  double pt_1_;
  double pt_2_;
  double online_pt_1_;
  double online_pt_2_;
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
  bool iso_vloose_;
  bool iso_loose_; 
  bool iso_medium_;
  bool iso_tight_;
  bool pass_antilep_;
  bool lepton_veto_;
  int dm_;
  double mt_1_;
  unsigned n_bjets_;
  double pzeta_;
  unsigned gen_match_1_;
  unsigned gen_match_2_;
  bool pass_dz_;
  bool pass_mass8_;
  bool pass_dimu_;
  bool passed_extra_1_;
  bool passed_extra_2_;
 
  bool trg_probe_2_1_;
  bool trg_probe_2_2_;
  bool trg_probe_2_3_;
  
  bool pass_FSR_condition_;
  double m_gamma_muons_;

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
  extra_l1_tag_pt_ = 34.0;
  extra_l1_probe_pt_ = 0.;
  extra_hlt_probe_pt_ = 0.;
  extra_hlt_probe_pt_vec_ = {};
  tag_add_trg_objects_="";
  tag_add_trg_filters_="";
  loose_iso_trgprobe_=false;
  extra_l1_iso_probe_pt_=0.;
  do_extra_=false;
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
    outtree_->Branch("online_pt_1"  , &online_pt_1_  );
    outtree_->Branch("online_pt_2"  , &online_pt_2_  );
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
    outtree_->Branch("trg_probe_2_1" , &trg_probe_2_1_    );
    outtree_->Branch("trg_probe_2_2" , &trg_probe_2_2_    );
    outtree_->Branch("trg_probe_2_3" , &trg_probe_2_3_    );
    outtree_->Branch("passed_extra_1" , &passed_extra_1_    );
    outtree_->Branch("passed_extra_2" , &passed_extra_2_    );
    outtree_->Branch("trg_tag_1" , &trg_tag_1_    );
    outtree_->Branch("trg_tag_2" , &trg_tag_2_    );
    outtree_->Branch("gen_match_1", &gen_match_1_);
    outtree_->Branch("gen_match_2", &gen_match_2_);
    outtree_->Branch("m_gamma_muons", &m_gamma_muons_);
    outtree_->Branch("pass_FSR_condition",&pass_FSR_condition_);


    if(channel_ == channel::tpmt){
      outtree_->Branch("iso_vloose" , &iso_vloose_);
      outtree_->Branch("iso_loose"  , &iso_loose_ );
      outtree_->Branch("iso_medium" , &iso_medium_);
      outtree_->Branch("iso_tight"  , &iso_tight_ );     
      outtree_->Branch("pass_antilep"  , &pass_antilep_ );  
      outtree_->Branch("lepton_veto"  , &lepton_veto_ );
      outtree_->Branch("dm"  , &dm_ );
      outtree_->Branch("mt_1"  , &mt_1_ );
      outtree_->Branch("n_bjets"  , &n_bjets_ );
      outtree_->Branch("pzeta"  , &pzeta_ );
    }
    if(do_dzmass_){
      outtree_->Branch("pass_dz" , &pass_dz_    );
      outtree_->Branch("pass_mass8", &pass_mass8_);
      outtree_->Branch("pass_dimu", &pass_dimu_);
    }
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
  
  if(event->Exists("gen_match_1")) gen_match_1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
  if(event->Exists("gen_match_2")) gen_match_2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));
  
  trg_tag_1_ = false;
  trg_tag_2_ = false;
  trg_probe_1_ = false;
  trg_probe_2_ = false;
  
  std::vector<std::string> tag_objs = SplitString(tag_trg_objects_);
  std::vector<std::string> tag_filts = SplitString(tag_trg_filters_);  
  std::vector<std::string> tag_add_objs = SplitString(tag_add_trg_objects_);
  std::vector<std::string> tag_add_filts = SplitString(tag_add_trg_filters_);  
  std::vector<std::string> probe_objs = SplitString(probe_trg_objects_);
  std::vector<std::string> probe_filts = SplitString(probe_trg_filters_);
  
  if(tag_objs.size() != tag_filts.size()){std::cout << "Number of tag trigger objects does not match the number of tag filters, throwing excpetion!" << std::endl; throw;}
  if(probe_objs.size() != probe_filts.size()){std::cout << "Number of probe trigger objects does not match the number of probe filters, throwing excpetion!" << std::endl; throw;}
 
 
  std::vector<TriggerObject *> objs_tag;
  for(unsigned i=0; i<tag_objs.size(); ++i){
    objs_tag = event->GetPtrVec<TriggerObject>(tag_objs[i]);
    trg_tag_1_ = IsFilterMatched(ditau->At(0), objs_tag, tag_filts[i], 0.5) || trg_tag_1_;
    trg_tag_2_ = IsFilterMatched(ditau->At(1), objs_tag, tag_filts[i], 0.5) || trg_tag_2_;
    
    //// added this bit for DZ filter! 
    //std::size_t hash = CityHash64(tag_filts[i]);
    //for (unsigned j = 0; j < objs_tag.size(); ++j) {
    //  std::vector<std::size_t> const& labels = objs_tag[j]->filters();
    //  if (std::find(labels.begin(),labels.end(), hash) == labels.end()) continue;
    //  trg_tag_1_ = true;
    //}
    ///////
  }
  if(tag_add_trg_objects_!=""){
    bool add_trg_tag_1 = false; 
    bool add_trg_tag_2 = false;
    std::vector<TriggerObject *> add_objs_tag;
    for(unsigned i=0; i<tag_add_objs.size(); ++i){
      add_objs_tag = event->GetPtrVec<TriggerObject>(tag_add_objs[i]);
      add_trg_tag_1 = IsFilterMatched(ditau->At(0), add_objs_tag, tag_add_filts[i], 0.5) || add_trg_tag_1;
      add_trg_tag_2 = IsFilterMatched(ditau->At(1), add_objs_tag, tag_add_filts[i], 0.5) || add_trg_tag_2;
    }
    trg_tag_1_ = trg_tag_1_ && add_trg_tag_1;
    trg_tag_2_ = trg_tag_2_ && add_trg_tag_2;
  }
 
  online_pt_1_=-9999;
  online_pt_2_=-9999; 
  trg_probe_2_1_ = false;
  trg_probe_2_2_ = false;
  trg_probe_2_3_ = false;
  trg_probe_1_ = false;
  trg_probe_2_ = false;
  std::vector<TriggerObject *> objs_probe;
  for(unsigned i=0; i<probe_objs.size(); ++i){
    objs_probe = event->GetPtrVec<TriggerObject>(probe_objs[i]);
    bool pass_extra_filter_1 = true;
    bool pass_extra_filter_2 = true;
    if(probe_objs[i] == "triggerObjectsEle32L1DoubleEG"){
      std::vector<TriggerObject *> extra_objs_probe = event->GetPtrVec<TriggerObject>("triggerObjectsEle35");
      pass_extra_filter_1 = IsFilterMatched(ditau->At(0), extra_objs_probe, "hltEGL1SingleEGOrFilter", 0.5);
      pass_extra_filter_2 = IsFilterMatched(ditau->At(1), extra_objs_probe, "hltEGL1SingleEGOrFilter", 0.5);
    }
    bool trg_probe_temp_1 = (IsFilterMatched(ditau->At(0), objs_probe, probe_filts[i], 0.5)&&pass_extra_filter_1);
    bool trg_probe_temp_2 = (IsFilterMatched(ditau->At(1), objs_probe, probe_filts[i], 0.5)&&pass_extra_filter_2);
 
    //if(trg_probe_temp_2 && gen_match_2_==5) std::cout << pt_2_ << "    " << gen_match_2_ << "    " << probe_filts[i] << std::endl;
   
    //// added this bit for DZ filter! 
    //std::size_t hash = CityHash64(probe_filts[i]);
    //for (unsigned j = 0; j < objs_probe.size(); ++j) {
    //  std::vector<std::size_t> const& labels = objs_probe[j]->filters();
    //  if (std::find(labels.begin(),labels.end(), hash) == labels.end()) continue;
    //  trg_probe_1_ = true;
    //}
    ////
     
    if(trg_probe_temp_1) { 
      unsigned leg1_match_index_1 = IsFilterMatchedWithIndex(ditau->At(0), objs_probe, probe_filts[i], 0.5).second;
      online_pt_1_ = objs_probe[leg1_match_index_1]->pt();
      if(extra_hlt_probe_pt_>0) trg_probe_temp_1 = trg_probe_temp_1 && objs_probe[leg1_match_index_1]->pt() > extra_hlt_probe_pt_;
      if(extra_hlt_probe_pt_vec_.size()>i){
        double pt_cut = extra_hlt_probe_pt_vec_[i];
        trg_probe_temp_1 = trg_probe_temp_1 && objs_probe[leg1_match_index_1]->pt() > pt_cut;
      } 
    }
    if(trg_probe_temp_2){
      unsigned leg1_match_index_2 = IsFilterMatchedWithIndex(ditau->At(1), objs_probe, probe_filts[i], 0.5).second;
      online_pt_2_ = objs_probe[leg1_match_index_2]->pt();
      if(extra_hlt_probe_pt_>0) trg_probe_temp_2 = trg_probe_temp_2 && objs_probe[leg1_match_index_2]->pt() > extra_hlt_probe_pt_;
      if(extra_hlt_probe_pt_vec_.size()>i){
        double pt_cut = extra_hlt_probe_pt_vec_[i];
        trg_probe_temp_2 = trg_probe_temp_2 && objs_probe[leg1_match_index_2]->pt() > pt_cut;
        //if(gen_match_2_==5) std::cout << trg_probe_temp_2 << "    " <<  objs_probe[leg1_match_index_2]->pt() << "    " <<  pt_cut <<std::endl;
      }
    }
    trg_probe_1_ = trg_probe_temp_1  || trg_probe_1_;
    trg_probe_2_ = trg_probe_temp_2  || trg_probe_2_;    

    if(i==0 && trg_probe_temp_2) trg_probe_2_1_ = true;
    if(i==1 && trg_probe_temp_2) trg_probe_2_2_ = true;
    if(i==2 && trg_probe_temp_2) trg_probe_2_3_ = true; 

  }
  
 //Add photons to check Final State Radiation in zmm channel in isolation eff.
    if(event->ExistsInTree("photons")){
        std::vector<Photon*> gammas = event->GetPtrVec<Photon>("photons");
        ic::erase_if(gammas, !boost::bind(MinPtMaxEta, _1, 10.0, 999));
        std::vector<Candidate const*> muon_vec_1 = {lep1};
        std::vector<Candidate const*> muon_vec_2 = {lep2};
        std::vector<std::pair <Photon*,Candidate const*> > input1 = MatchByDR(gammas, muon_vec_1, 0.4, false, false);   
        std::vector<std::pair <Photon*,Candidate const*> > input2 = MatchByDR(gammas, muon_vec_2, 0.4, false, false);    
        std::sort(input1.begin(), input1.end(), [](std::pair <Photon*,Candidate const*> a,std::pair <Photon*,Candidate const*>  b) {return a.first->pt() > b.first->pt();} );
        std::sort(input2.begin(), input2.end(), [](std::pair <Photon*,Candidate const*> a,std::pair <Photon*,Candidate const*>  b) {return a.first->pt() > b.first->pt();} );
        
        m_gamma_muons_=-999.0;
        pass_FSR_condition_ = false;
        auto sum_vec = lep1->vector() + lep2->vector();
        if (input1.size()>0){
            sum_vec +=  input1[0].first->vector();
            pass_FSR_condition_=true;
        }
        if (input2.size()>0){
            sum_vec += input2[0].first->vector();
            pass_FSR_condition_=true;
        }
        m_gamma_muons_= sum_vec.M();
    }
  if(channel_ == channel::tpzmm){
    if(strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18){
      T muon1 = dynamic_cast<T>(lep1);
      T muon2 = dynamic_cast<T>(lep2);
      iso_1_ = PF04IsolationVal(muon1, 0.5, 0);
      iso_2_ = PF04IsolationVal(muon2, 0.5, 0);
      if(loose_iso_trgprobe_){
        trg_probe_1_ = trg_probe_1_ && MuonTkIsoVal(dynamic_cast<Muon const*>(lep1)) < 0.4;   
        trg_probe_2_ = trg_probe_2_ && MuonTkIsoVal(dynamic_cast<Muon const*>(lep1)) < 0.4;
      }
      id_tag_1_ = tag_id_(muon1);
      id_tag_2_ = tag_id_(muon2);
      id_probe_1_ = probe_id_(muon1);
      id_probe_2_ = probe_id_(muon2);
   
      if(do_extra_){
        //put any extra condition your require the events to pass here
        std::vector<ic::L1TObject*> l1taus = event->GetPtrVec<ic::L1TObject>("L1Taus");
        bool passed_extra_1 = false;
        bool passed_extra_2 = false;
        for(unsigned ta=0; ta<l1taus.size(); ++ta){
          if(l1taus[ta]->vector().Pt()>24 && fabs(l1taus[ta]->eta())<2.1)  {
            double dR_1 = std::fabs(ROOT::Math::VectorUtil::DeltaR(l1taus[ta]->vector(),lep1->vector()));
            double dR_2 = std::fabs(ROOT::Math::VectorUtil::DeltaR(l1taus[ta]->vector(),lep2->vector()));
            if(dR_1>0.5) passed_extra_1 = true;
            if(dR_2>0.5) passed_extra_2 = true;
          }
         }
         trg_tag_1_ = trg_tag_1_ && passed_extra_2;
         trg_tag_2_ = trg_tag_2_ && passed_extra_1;
      }
 
    }
    if(extra_l1_probe_pt_>0){
      std::vector<ic::L1TObject*> l1muons = event->GetPtrVec<ic::L1TObject>("L1Muons");
      bool found_match_probe_1 = false;
      bool found_match_probe_2 = false;
      for(unsigned mu=0; mu<l1muons.size(); ++mu){
        if(l1muons[mu]->vector().Pt()>extra_l1_probe_pt_){
          // must pass L1 pT cut and be matched by DR to the tagging muon
          if(DR(l1muons[mu],lep1)<0.5) found_match_probe_1 = true;
          if(DR(l1muons[mu],lep2)<0.5) found_match_probe_2 = true;
        }
      }
      trg_probe_1_ = trg_probe_1_ && found_match_probe_1;
      trg_probe_2_ = trg_probe_2_ && found_match_probe_2;
    }

    if(do_dzmass_){
      pass_dz_ = false;
      pass_mass8_ = false;
      pass_dimu_ =false;

      std::vector<TriggerObject *> objs_dz = event->GetPtrVec<TriggerObject>("triggerObjectsMu17Mu8DZ");
      if(strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18){ 
        std::vector<TriggerObject *> objs_dzmass = event->GetPtrVec<TriggerObject>("triggerObjectsMu17Mu8DZmass8");
        bool leg_1_pass = IsFilterMatchedWithIndex(ditau->At(0), objs_dzmass, "hltDiMuon178RelTrkIsoFiltered0p4", 0.5).first || IsFilterMatchedWithIndex(ditau->At(0), objs_dz, "hltDiMuon178RelTrkIsoFiltered0p4", 0.5).first;
        if(leg_1_pass) leg_1_pass = leg_1_pass && (objs_dzmass[IsFilterMatchedWithIndex(ditau->At(0), objs_dzmass, "hltDiMuon178RelTrkIsoFiltered0p4", 0.5).second]->pt()>17 || objs_dz[IsFilterMatchedWithIndex(ditau->At(0), objs_dz, "hltDiMuon178RelTrkIsoFiltered0p4", 0.5).second]->pt()>17);
        bool leg_2_pass = IsFilterMatchedWithIndex(ditau->At(1), objs_dzmass, "hltDiMuon178RelTrkIsoFiltered0p4", 0.5).first || IsFilterMatchedWithIndex(ditau->At(1), objs_dz, "hltDiMuon178RelTrkIsoFiltered0p4", 0.5).first; 

        bool leg_1_pass_dz = IsFilterMatchedWithIndex(ditau->At(0), objs_dzmass, "hltDiMuon178RelTrkIsoFiltered0p4DzFiltered0p2", 0.5).first || IsFilterMatchedWithIndex(ditau->At(0), objs_dz, "hltDiMuon178RelTrkIsoFiltered0p4DzFiltered0p2", 0.5).first;
        bool leg_2_pass_dz = IsFilterMatchedWithIndex(ditau->At(1), objs_dzmass, "hltDiMuon178RelTrkIsoFiltered0p4DzFiltered0p2", 0.5).first || IsFilterMatchedWithIndex(ditau->At(1), objs_dz, "hltDiMuon178RelTrkIsoFiltered0p4DzFiltered0p2", 0.5).first;

        bool leg_1_pass_mass = IsFilterMatchedWithIndex(ditau->At(0), objs_dzmass, "hltDiMuon178Mass8Filtered", 0.5).first || IsFilterMatchedWithIndex(ditau->At(0), objs_dz, "hltDiMuon178Mass8Filtered", 0.5).first;
        bool leg_2_pass_mass = IsFilterMatchedWithIndex(ditau->At(1), objs_dzmass, "hltDiMuon178Mass8Filtered", 0.5).first || IsFilterMatchedWithIndex(ditau->At(1), objs_dz, "hltDiMuon178Mass8Filtered", 0.5).first;

        pass_dimu_ = leg_1_pass && leg_2_pass;
        pass_dz_ = pass_dimu_ && leg_1_pass_dz && leg_2_pass_dz;
        pass_mass8_ = pass_dz_&& leg_1_pass_mass && leg_2_pass_mass;
      } else{
        std::vector<TriggerObject *> objs_dz_alt = event->GetPtrVec<TriggerObject>("triggerObjectsMu17TkMu8DZ");
        bool leg_1_pass = IsFilterMatchedWithIndex(ditau->At(0), objs_dz_alt, "hltDiMuonGlb17Trk8RelTrkIsoFiltered0p4", 0.5).first || IsFilterMatchedWithIndex(ditau->At(0), objs_dz, "hltDiMuonGlb17Glb8RelTrkIsoFiltered0p4", 0.5).first;
        if(leg_1_pass) leg_1_pass = leg_1_pass && (objs_dz_alt[IsFilterMatchedWithIndex(ditau->At(0), objs_dz_alt, "hltDiMuonGlb17Trk8RelTrkIsoFiltered0p4", 0.5).second]->pt()>17 || objs_dz[IsFilterMatchedWithIndex(ditau->At(0), objs_dz, "hltDiMuonGlb17Glb8RelTrkIsoFiltered0p4", 0.5).second]->pt()>17);
        bool leg_2_pass = IsFilterMatchedWithIndex(ditau->At(1), objs_dz_alt, "hltDiMuonGlb17Trk8RelTrkIsoFiltered0p4", 0.5).first || IsFilterMatchedWithIndex(ditau->At(1), objs_dz, "hltDiMuonGlb17Glb8RelTrkIsoFiltered0p4", 0.5).first;

        bool leg_1_pass_dz = IsFilterMatchedWithIndex(ditau->At(0), objs_dz_alt, "hltDiMuonGlb17Trk8RelTrkIsoFiltered0p4DzFiltered0p2", 0.5).first || IsFilterMatchedWithIndex(ditau->At(0), objs_dz, "hltDiMuonGlb17Glb8RelTrkIsoFiltered0p4DzFiltered0p2", 0.5).first;
        bool leg_2_pass_dz = IsFilterMatchedWithIndex(ditau->At(1), objs_dz_alt, "hltDiMuonGlb17Trk8RelTrkIsoFiltered0p4DzFiltered0p2", 0.5).first || IsFilterMatchedWithIndex(ditau->At(1), objs_dz, "hltDiMuonGlb17Glb8RelTrkIsoFiltered0p4DzFiltered0p2", 0.5).first;
        pass_dimu_ = leg_1_pass && leg_2_pass;
        pass_dz_ = pass_dimu_ && leg_1_pass_dz && leg_2_pass_dz;
      }

    }
    
  }
  if(channel_ == channel::tpzee){
    if(strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18){
      T elec1 = dynamic_cast<T>(lep1);
      T elec2 = dynamic_cast<T>(lep2);
      if(strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18 || strategy_ == strategy::legacy16) {
        iso_1_ = PF03EAIsolationVal(elec1, eventInfo->jet_rho()); //lepton_rho
        iso_2_ = PF03EAIsolationVal(elec2, eventInfo->jet_rho());  
      } else {
        iso_1_ = PF03IsolationVal(elec1, 0.5, 0);
        iso_2_ = PF03IsolationVal(elec2, 0.5, 0);
      }
      id_tag_1_ = tag_id_(elec1);
      id_tag_2_ = tag_id_(elec2);
      id_probe_1_ = probe_id_(elec1);
      id_probe_2_ = probe_id_(elec2);
      
      Electron *elec1_1 = dynamic_cast<Electron*>(ditau->GetCandidate("lepton1"));
      Electron *elec2_1 = dynamic_cast<Electron*>(ditau->GetCandidate("lepton2"));
      eta_1_ = elec1_1->sc_eta();
      eta_2_ = elec2_1->sc_eta();

      if(do_extra_){
        //put any extra condition your require the events to pass here
        std::vector<ic::L1TObject*> l1taus = event->GetPtrVec<ic::L1TObject>("L1Taus");
        bool passed_extra_1 = false;
        bool passed_extra_2 = false;
        for(unsigned ta=0; ta<l1taus.size(); ++ta){
          if(l1taus[ta]->vector().Pt()>26 && fabs(l1taus[ta]->eta())<2.1 && l1taus[ta]->isolation()>0)  {
            double dR_1 = std::fabs(ROOT::Math::VectorUtil::DeltaR(l1taus[ta]->vector(),lep1->vector()));
            double dR_2 = std::fabs(ROOT::Math::VectorUtil::DeltaR(l1taus[ta]->vector(),lep2->vector()));
            double deta_1 = std::fabs(l1taus[ta]->eta()-lep1->eta());
            double deta_2 = std::fabs(l1taus[ta]->eta()-lep2->eta());
            if(deta_1>0.2&&dR_1>0.5) passed_extra_1 = true;
            if(deta_2>0.2&&dR_2>0.5) passed_extra_2 = true;
          }
         }
         trg_tag_1_ = trg_tag_1_ && passed_extra_2;
         trg_tag_2_ = trg_tag_2_ && passed_extra_1;
      }

      if(strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18 || strategy_ == strategy::legacy16){
        // we have to do this here so that the ID is compted before the smear and scale shift
        float  preCorr_1 = elec1_1->ecalTrkEnergyPreCorr();
        float postCorr_1 = elec1_1->ecalTrkEnergyPostCorr();
        float shift_1 = postCorr_1/preCorr_1;
        elec1_1->set_pt(elec1_1->pt() * shift_1);
        elec1_1->set_energy(elec1_1->energy() * shift_1);
        float  preCorr_2 = elec2_1->ecalTrkEnergyPreCorr();
        float postCorr_2 = elec2_1->ecalTrkEnergyPostCorr();
        float shift_2 = postCorr_2/preCorr_2;
        elec2_1->set_pt(elec2_1->pt() * shift_2);
        elec2_1->set_energy(elec2_1->energy() * shift_2);

        pt_1_ = elec1_1->pt();
        pt_2_ = elec2_1->pt();
        m_vis_ = (elec1_1->vector()+elec2_1->vector()).M();
      }
      
    }
    if(extra_l1_probe_pt_>0 || extra_l1_iso_probe_pt_>0){
      std::vector<ic::L1TObject*> l1electrons = event->GetPtrVec<ic::L1TObject>("L1EGammas");
      bool found_match_probe_1 = false;
      bool found_match_probe_2 = false;
      for(unsigned eg=0; eg<l1electrons.size(); ++eg){
        if((l1electrons[eg]->vector().Pt()>extra_l1_probe_pt_||extra_l1_probe_pt_==0) || (extra_l1_iso_probe_pt_==0 || (l1electrons[eg]->vector().Pt()>extra_l1_iso_probe_pt_ && l1electrons[eg]->isolation()!=0))){
        if(DR(l1electrons[eg],lep1)<0.5) found_match_probe_1 = true;
        if(DR(l1electrons[eg],lep2)<0.5) found_match_probe_2 = true;
        }
      }
      trg_probe_1_ = trg_probe_1_ && found_match_probe_1;
      trg_probe_2_ = trg_probe_2_ && found_match_probe_2;
    }

    if(extra_l1_tag_pt_>0){
      std::vector<ic::L1TObject*> l1electrons = event->GetPtrVec<ic::L1TObject>("L1EGammas");
      bool found_match_tag_1 = false;
      bool found_match_tag_2 = false;
      int isocut=1;
      if(strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) isocut=3;
   
      for(unsigned eg=0; eg<l1electrons.size(); ++eg){
        if(std::fabs(l1electrons[eg]->vector().Rapidity()) < 2.17&&l1electrons[eg]->vector().Pt()>extra_l1_tag_pt_&&l1electrons[eg]->isolation()>=isocut){
          // must pass L1 eta, pT and iso cuts and be matched by DR to the tagging electron
          if(DR(l1electrons[eg],lep1)<0.5) found_match_tag_1 = true;
          if(DR(l1electrons[eg],lep2)<0.5) found_match_tag_2 = true;
        }
      }
      trg_tag_1_ = trg_tag_1_ && found_match_tag_1;
      trg_tag_2_ = trg_tag_2_ && found_match_tag_2;
    }
  }
  
  if(channel_ == channel::tpmt){
    // add extra lepton veto!  
    if(strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 ||  strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18){
      T muon = dynamic_cast<T>(lep1);
      Tau const* tau = dynamic_cast<Tau const*>(lep2);
      iso_1_ = PF04IsolationVal(muon, 0.5, 0);
      id_tag_1_ = tag_id_(muon);
      id_tag_2_ = 0;
      id_probe_1_ = 0;
      id_probe_2_ = tau->GetTauID("decayModeFinding");
      pass_antilep_ = tau->GetTauID("againstMuonTight3") && tau->GetTauID("againstElectronVLooseMVA6");
      if( strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) {
        iso_vloose_ = tau->GetTauID("byVLooseIsolationMVArun2017v2DBoldDMwLT2017");
        iso_loose_ = tau->GetTauID("byLooseIsolationMVArun2017v2DBoldDMwLT2017");
        iso_medium_ = tau->GetTauID("byMediumIsolationMVArun2017v2DBoldDMwLT2017");
        iso_tight_ = tau->GetTauID("byTightIsolationMVArun2017v2DBoldDMwLT2017");
      } else {
        iso_vloose_ = tau->GetTauID("byVLooseIsolationMVArun2v1DBoldDMwLT");
        iso_loose_ = tau->GetTauID("byLooseIsolationMVArun2v1DBoldDMwLT");
        iso_medium_ = tau->GetTauID("byMediumIsolationMVArun2v1DBoldDMwLT");
        iso_tight_ = tau->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT");
      }
      dm_ = tau->decay_mode();
      
      lepton_veto_ = event->Get<bool>("dimuon_veto") || event->Get<bool>("extra_elec_veto") || event->Get<bool>("extra_muon_veto");
    }
    if(extra_l1_probe_pt_>0){
      std::vector<ic::L1TObject*> l1taus = event->GetPtrVec<ic::L1TObject>("L1Taus");
      bool found_match_probe = false;
      for(unsigned ta=0; ta<l1taus.size(); ++ta){
        //if(l1taus[ta]->vector().Pt()>extra_l1_probe_pt_ && l1taus[ta]->isolation()>0 && fabs(l1taus[ta]->eta())<2.17){
        if(l1taus[ta]->vector().Pt()>extra_l1_probe_pt_ && fabs(l1taus[ta]->eta())<2.17){ // this for non isolated taus
          // must pass L1 pT cut and be matched by DR to the tau
          if(DR(l1taus[ta],lep2)<0.5) found_match_probe = true;
        }
      }
      trg_probe_2_ = trg_probe_2_ && found_match_probe;
      trg_probe_2_1_ = trg_probe_2_1_ && found_match_probe;
      trg_probe_2_2_ = trg_probe_2_2_ && found_match_probe;
      trg_probe_2_3_ = trg_probe_2_3_ && found_match_probe;
  
    }
    Met const* mets = NULL;
    mets = event->GetPtr<Met>("pfMET");
    mt_1_ = MT(lep1, mets);
    
    pzeta_ = PZeta(ditau, mets, 0.85);
    
    std::string jets_label = "ak4PFJetsCHS";
    std::string btag_label = "pfCombinedInclusiveSecondaryVertexV2BJetTags";
    double btag_wp = 0.8484;
    std::vector<PFJet*> bjets = event->GetPtrVec<PFJet>(jets_label);
    ic::erase_if(bjets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    if (event->Exists("retag_result")) {
        auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result"); 
        ic::erase_if(bjets, !boost::bind(IsReBTagged, _1, retag_result));
      } else{ 
        ic::erase_if(bjets, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) < btag_wp);
      } 
    
    n_bjets_ = bjets.size();
  }
  if(channel_ == channel::tpem){
    // add extra lepton veto!  
    if(strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18){
      Electron const* elec = dynamic_cast<Electron const*>(lep1);
      //Muon const* muon = dynamic_cast<Muon const*>(lep2);
      T muon = dynamic_cast<T>(lep2);
      if(strategy_ == strategy::legacy16)
        iso_1_ = PF03EAIsolationVal(elec, eventInfo->jet_rho());
      else
        iso_1_ = PF03IsolationVal(elec, 0.5, 0);      
      iso_2_ = PF04IsolationVal(muon, 0.5, 0);
      id_tag_1_ = 0;
      id_tag_2_ = tag_id_(muon);
      if(strategy_ == strategy::legacy16)
        id_probe_1_ = ElectronHTTIdFall17V2(elec, true);
      else
        id_probe_1_ = ElectronHTTIdSpring16(elec, false);
      id_probe_2_ = 0;
    }

    Met const* mets = NULL;
    mets = event->GetPtr<Met>("pfMET");
    mt_1_ = MT(lep1, mets);
    
    pzeta_ = PZeta(ditau, mets, 0.85);
    
    std::string jets_label = "ak4PFJetsCHS";
    std::string btag_label = "pfCombinedInclusiveSecondaryVertexV2BJetTags";
    double btag_wp = 0.8484;
    std::vector<PFJet*> bjets = event->GetPtrVec<PFJet>(jets_label);
    ic::erase_if(bjets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    if (event->Exists("retag_result")) {
        auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result"); 
        ic::erase_if(bjets, !boost::bind(IsReBTagged, _1, retag_result));
      } else{ 
        ic::erase_if(bjets, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) < btag_wp);
      } 
    
    n_bjets_ = bjets.size();
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
