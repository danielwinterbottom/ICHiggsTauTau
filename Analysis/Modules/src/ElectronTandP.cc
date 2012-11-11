#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/ElectronTandP.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/SuperCluster.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"


namespace ic {

  ElectronTandP::ElectronTandP(std::string const& name) : ModuleBase(name) {
  }

  ElectronTandP::~ElectronTandP() {
    delete fs_;
  }

  int ElectronTandP::PreAnalysis() {
    fs_ = new fwlite::TFileService("ElectronTandP_"+output_name_+".root");
    hists_ = new DynamicHistoSet(fs_->mkdir("/"));
    hists_->Create("tt_wp85_SC8", hist_bins_, min_mass_, max_mass_);
    hists_->Create("tf_wp85_SC8", hist_bins_, min_mass_, max_mass_);
    hists_->Create("tt_wp85_Ele8", hist_bins_, min_mass_, max_mass_);
    hists_->Create("tf_wp85_Ele8", hist_bins_, min_mass_, max_mass_);
    hists_->Create("tt_trig17_SC8", hist_bins_, min_mass_, max_mass_);
    hists_->Create("tf_trig17_SC8", hist_bins_, min_mass_, max_mass_);
    hists_->Create("tt_trig8_SC8", hist_bins_, min_mass_, max_mass_);
    hists_->Create("tf_trig8_SC8", hist_bins_, min_mass_, max_mass_);
    hists_->Create("tt_trig17_SC17", hist_bins_, min_mass_, max_mass_);
    hists_->Create("tf_trig17_SC17", hist_bins_, min_mass_, max_mass_);
    hists_->Create("tt_sc_SC8", hist_bins_, min_mass_, max_mass_);
    hists_->Create("tf_sc_SC8", hist_bins_, min_mass_, max_mass_);
    hists_->Create("tt_sc_SC17", hist_bins_, min_mass_, max_mass_);
    hists_->Create("tf_sc_SC17", hist_bins_, min_mass_, max_mass_);
    return 0;
  }

  int ElectronTandP::Execute(TreeEvent *event) {

    // Trigger:
    std::string hlt_path_Ele32SC17 = "HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_v";
    std::string lower_filter_Ele32SC17 = "hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17PixelMatchFilter";
    std::string upper_filter_Ele32SC17 = "hltEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17TrackIsolFilter" ;
    std::string obj_name_Ele32SC17 = "triggerObjectsEle32SC17";                       

    std::string hlt_path_Ele17Ele8 = "HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass30_v";
    std::string lower_filter_Ele17Ele8 = "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8PMMassFilter";
    std::string upper_filter_Ele17Ele8 = "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8TrackIsolFilter";
    std::string obj_name_Ele17Ele8 = "triggerObjectsEle17Ele8Mass30";   

    std::string hlt_path_Ele17SC8 = "HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v";
    std::string lower_filter_Ele17SC8 = "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8PMMassFilter";
    std::string upper_filter_Ele17SC8 = "hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8TrackIsolFilter";
    std::string obj_name_Ele17SC8 = "triggerObjectsEle17SC8Mass30";       

    // HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v
    std::string lower_filter_early = "hltEle17CaloIdIsoEle8CaloIdIsoPixelMatchDoubleFilter";
    std::string upper_filter_early = "hltEle17CaloIdLCaloIsoVLPixelMatchFilter";
    std::string obj_name_early = "triggerObjectsEle17Ele8Early";                       

    // HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v
    std::string lower_filter_late = "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolDoubleFilter";
    std::string upper_filter_late = "hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsolFilter";
    std::string obj_name_late = "triggerObjectsEle17Ele8Late";      

    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    unsigned run = eventInfo->run();                 
    std::string lower_filter_phys;
    std::string upper_filter_phys;
    std::string obj_name_phys; 
    if (run <= 170053) {
      lower_filter_phys = lower_filter_early;
      upper_filter_phys = upper_filter_early;
      obj_name_phys = obj_name_early; 
    } else {
      lower_filter_phys = lower_filter_late;
      upper_filter_phys = upper_filter_late;
      obj_name_phys = obj_name_late; 
    }
   
    bool path_found_Ele32SC17= false;
    bool path_found_Ele17Ele8 = false;
    bool path_found_Ele17SC8 = false;

    TriggerPathPtrVec const& triggerPathPtrVec = 
        event->GetPtrVec<TriggerPath>("triggerPathPtrVec","triggerPaths");
    for (unsigned i = 0; i < triggerPathPtrVec.size(); ++i) {
      std::string name = triggerPathPtrVec[i]->name();
      if (name.find(hlt_path_Ele32SC17) != name.npos) path_found_Ele32SC17 = true;
      if (name.find(hlt_path_Ele17Ele8) != name.npos) path_found_Ele17Ele8 = true;
      if (name.find(hlt_path_Ele17SC8) != name.npos) path_found_Ele17SC8 = true;
    }

    // Do basic tag selection:
    std::vector<Electron *> & elecs = event->GetPtrVec<Electron>("electrons");
    std::vector<Electron *> tag_cands = elecs;
    ic::erase_if(tag_cands,!boost::bind(MinPtMaxEta, _1, 25.0, 9999.0));
    ic::erase_if(tag_cands,!boost::bind(MaxEtaSC, _1, 2.1));
    ic::erase_if(tag_cands,boost::bind(InEcalGap, _1));
    ic::erase_if(tag_cands,!boost::bind(Electron2011WP85ID, _1));
    ic::erase_if(tag_cands,!boost::bind(Electron2011WP85Iso, _1));
    ic::erase_if(tag_cands,boost::bind(&Electron::charge, _1) != -1);
    std::vector<Electron *> probe_cands = elecs;
    ic::erase_if(probe_cands,!boost::bind(MinPtMaxEta, _1, 25.0, 9999.0));
    ic::erase_if(probe_cands,!boost::bind(MaxEtaSC, _1, 2.1));
    ic::erase_if(probe_cands,boost::bind(InEcalGap, _1));
    ic::erase_if(probe_cands,boost::bind(&Electron::charge, _1) != +1);

    if (tag_cands.size() == 0) return 0;
    

    std::vector<SuperCluster *> sc_cands = event->GetPtrVec<SuperCluster>("superClusters");

    std::vector<TriggerObject *> const& objs_Ele32SC17 = event->GetPtrVec<TriggerObject>(obj_name_Ele32SC17);
    std::vector<TriggerObject *> const& objs_Ele17Ele8 = event->GetPtrVec<TriggerObject>(obj_name_Ele17Ele8);
    std::vector<TriggerObject *> const& objs_Ele17SC8 = event->GetPtrVec<TriggerObject>(obj_name_Ele17SC8);
    std::vector<TriggerObject *> const& objs_phys = event->GetPtrVec<TriggerObject>(obj_name_phys);


    //----------------------------------------------------------------------
    //Do SC->GSF electron efficiency
    //Can use only SC8 or SC17 leg
    //----------------------------------------------------------------------

    if (path_found_Ele32SC17) {
      std::vector<Electron *> tags = tag_cands;
      ic::erase_if(tags,!boost::bind(IsFilterMatched, _1, objs_Ele32SC17, upper_filter_Ele32SC17, 0.3));
      
      std::vector<SuperCluster *> scs = sc_cands;
      ic::erase_if(scs,!boost::bind(MinPtMaxEtaSC, _1, 25.0, 2.1));
      ic::erase_if(scs,boost::bind(InEcalGapSC, _1));

      if (tags.size() > 0 && scs.size() > 0) {
        std::vector< std::pair<Electron*, SuperCluster*> > pairs = MakePairs(tags,scs);
        for (unsigned i = 0; i < pairs.size(); ++i) {
          double mass = (pairs[i].first->vector() + pairs[i].second->vector()).M();
          if (mass < 66.0 && mass > 116.0) continue;
          std::vector<SuperCluster *> probe_sc;
          probe_sc.push_back(pairs[i].second);
          bool pass_sc = false;
          if ((MatchByDR(probe_sc, elecs, 0.3, true, true)).size() > 0) pass_sc = true;
          if (pass_sc) {
            hists_->Fill("tt_sc_SC17", mass);
          }
          if (!pass_sc) {
            hists_->Fill("tf_sc_SC17", mass);
          }
        } // loop through tag-probe pairs
      } // If there's at least one tag-probe pair
    } // If Ele32SC17 path fired in the event

    if (path_found_Ele17SC8) {
      std::vector<Electron *> tags = tag_cands;
      ic::erase_if(tags,!boost::bind(IsFilterMatched, _1, objs_Ele17SC8, upper_filter_Ele17SC8, 0.3));
    
      std::vector<SuperCluster *> scs = sc_cands;
      ic::erase_if(scs,!boost::bind(MinPtMaxEtaSC, _1, 25.0, 2.1));
      ic::erase_if(scs,boost::bind(InEcalGapSC, _1));

      if (tags.size() > 0 && scs.size() > 0) {
        std::vector< std::pair<Electron*, SuperCluster*> > pairs = MakePairs(tags,scs);
        for (unsigned i = 0; i < pairs.size(); ++i) {
          double mass = (pairs[i].first->vector() + pairs[i].second->vector()).M();
          if (mass < 66.0 && mass > 116.0) continue;
          std::vector<SuperCluster *> probe_sc;
          probe_sc.push_back(pairs[i].second);
          bool pass_sc = false;
          if ((MatchByDR(probe_sc, elecs, 0.3, true, true)).size() > 0) pass_sc = true;
          if (pass_sc) {
            hists_->Fill("tt_sc_SC8", mass);
          }
          if (!pass_sc) {
            hists_->Fill("tf_sc_SC8", mass);
          }
        } // loop through tag-probe pairs
      } // If there's at least one tag-probe pair
    } // If Ele17SC8 path fired in the event

    //----------------------------------------------------------------------
    //Do WP85 electron efficiency
    //Use SC8 or Ele8 leg (could use SC17??)
    //----------------------------------------------------------------------
    if (path_found_Ele17SC8) {
      std::vector<Electron *> tags = tag_cands;
      ic::erase_if(tags,!boost::bind(IsFilterMatched, _1, objs_Ele17SC8, upper_filter_Ele17SC8, 0.3));

      std::vector<Electron *> probes = probe_cands;
      ic::erase_if(probes,!boost::bind(IsFilterMatched, _1, objs_Ele17SC8, lower_filter_Ele17SC8, 0.3));

      if (tags.size() > 0 && probes.size() > 0) {
        std::vector< std::pair<Electron*, Electron*> > pairs = MakePairs(tags,probes);
        for (unsigned i = 0; i < pairs.size(); ++i) {
          double mass = (pairs[i].first->vector() + pairs[i].second->vector()).M();
          if (mass < 66.0 && mass > 116.0) continue;
          bool pass_probe = Electron2011WP85ID(pairs[i].second) && Electron2011WP85Iso(pairs[i].second);
          if (pass_probe) {
            hists_->Fill("tt_wp85_SC8", mass);
          }
          if (!pass_probe) {
            hists_->Fill("tf_wp85_SC8", mass);
          }
        } // loop through tag-probe pairs
      } // If there's at least one tag-probe pair
    } // If Ele17SC8 path fired in the event

    if (path_found_Ele17Ele8) {
      std::vector<Electron *> tags = tag_cands;
      ic::erase_if(tags,!boost::bind(IsFilterMatched, _1, objs_Ele17Ele8, upper_filter_Ele17Ele8, 0.3));

      std::vector<Electron *> probes = probe_cands;
      ic::erase_if(probes,!boost::bind(IsFilterMatched, _1, objs_Ele17Ele8, lower_filter_Ele17Ele8, 0.3));

      if (tags.size() > 0 && probes.size() > 0) {
        std::vector< std::pair<Electron*, Electron*> > pairs = MakePairs(tags,probes);
        for (unsigned i = 0; i < pairs.size(); ++i) {
          double mass = (pairs[i].first->vector() + pairs[i].second->vector()).M();
          if (mass < 66.0 && mass > 116.0) continue;
          bool pass_probe = Electron2011WP85ID(pairs[i].second) && Electron2011WP85Iso(pairs[i].second);
          if (pass_probe) {
            hists_->Fill("tt_wp85_Ele8", mass);
          }
          if (!pass_probe) {
            hists_->Fill("tf_wp85_Ele8", mass);
          }
        } // loop through tag-probe pairs
      } // If there's at least one tag-probe pair
    } // If Ele17SC8 path fired in the event


    //----------------------------------------------------------------------
    //Do Trigger electron efficiency
    //Use SC8 or SC17 leg
    //----------------------------------------------------------------------
    if (path_found_Ele17SC8) {
      std::vector<Electron *> tags = tag_cands;
      ic::erase_if(tags,!boost::bind(IsFilterMatched, _1, objs_Ele17SC8, upper_filter_Ele17SC8, 0.3));

      std::vector<Electron *> probes = probe_cands;
      ic::erase_if(probes,!boost::bind(IsFilterMatched, _1, objs_Ele17SC8, lower_filter_Ele17SC8, 0.3));
      ic::erase_if(probes,!boost::bind(Electron2011WP85ID, _1));
      ic::erase_if(probes,!boost::bind(Electron2011WP85Iso, _1));

      if (tags.size() > 0 && probes.size() > 0) {
        std::vector< std::pair<Electron*, Electron*> > pairs = MakePairs(tags,probes);
        for (unsigned i = 0; i < pairs.size(); ++i) {
          double mass = (pairs[i].first->vector() + pairs[i].second->vector()).M();
          if (mass < 66.0 && mass > 116.0) continue;
          bool pass_probe = IsFilterMatched(pairs[i].second, objs_phys, upper_filter_phys, 0.3);
          if (pass_probe) {
            hists_->Fill("tt_trig17_SC8", mass);
          }
          if (!pass_probe) {
            hists_->Fill("tf_trig17_SC8", mass);
          }
          bool tag_tight_physics = IsFilterMatched(pairs[i].first, objs_phys, upper_filter_phys, 0.3);
          if (!tag_tight_physics) continue;
          bool pass_probe_low_phys = IsFilterMatched(pairs[i].second, objs_phys, lower_filter_phys, 0.3);
          if (pass_probe_low_phys) {
            hists_->Fill("tt_trig8_SC8", mass);
          }
          if (!pass_probe_low_phys) {
            hists_->Fill("tf_trig8_SC8", mass);
          }
        } // loop through tag-probe pairs
      } // If there's at least one tag-probe pair
    } // If Ele17SC8 path fired in the event

    if (path_found_Ele32SC17) {
      std::vector<Electron *> tags = tag_cands;
      ic::erase_if(tags,!boost::bind(IsFilterMatched, _1, objs_Ele32SC17, upper_filter_Ele32SC17, 0.3));

      std::vector<Electron *> probes = probe_cands;
      ic::erase_if(probes,!boost::bind(IsFilterMatched, _1, objs_Ele32SC17, lower_filter_Ele32SC17, 0.3));
      ic::erase_if(probes,!boost::bind(Electron2011WP85ID, _1));
      ic::erase_if(probes,!boost::bind(Electron2011WP85Iso, _1));

      if (tags.size() > 0 && probes.size() > 0) {
        std::vector< std::pair<Electron*, Electron*> > pairs = MakePairs(tags,probes);
        for (unsigned i = 0; i < pairs.size(); ++i) {
          double mass = (pairs[i].first->vector() + pairs[i].second->vector()).M();
          if (mass < 66.0 && mass > 116.0) continue;
          bool pass_probe = IsFilterMatched(pairs[i].second, objs_phys, upper_filter_phys, 0.3);
          if (pass_probe) {
            hists_->Fill("tt_trig17_SC17", mass);
          }
          if (!pass_probe) {
            hists_->Fill("tf_trig17_SC17", mass);
          } 
        } // loop through tag-probe pairs
      } // If there's at least one tag-probe pair
    } // If Ele17SC8 path fired in the event

    return 0;
  }

  int ElectronTandP::PostAnalysis() {
    return 0;
  }

  void ElectronTandP::PrintInfo() {
    ;
  }
}
