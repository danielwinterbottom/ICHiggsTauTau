#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTTriggerFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "boost/bind.hpp"
#include "boost/format.hpp"

namespace ic {

  HTTTriggerFilter::HTTTriggerFilter(std::string const& name) : ModuleBase(name), channel_(channel::zee), mc_(mc::summer12_53X), era_(era::data_2015), strategy_(strategy::mssmsummer16){
  }

  HTTTriggerFilter::~HTTTriggerFilter() {
    ;
  }

  int HTTTriggerFilter::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTTriggerFilter" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
    std::cout << boost::format(param_fmt()) % "channel"         % Channel2String(channel_);
    std::cout << boost::format(param_fmt()) % "mc"              % MC2String(mc_);
    std::cout << boost::format(param_fmt()) % "dilepton_label"  % pair_label_;
    std::cout << boost::format(param_fmt()) % "is_data"         % is_data_;
    std::cout << boost::format(param_fmt()) % "is_embedded"     % is_embedded_;
    std::cout << boost::format(param_fmt()) % "do_singletau"    % do_singletau_;
    std::cout << boost::format(param_fmt()) % "do_filter"       % do_filter_;
    return 0;
  }

  int HTTTriggerFilter::Execute(TreeEvent *event) {

    std::string trig_obj_label;
    std::string alt_trig_obj_label="";
    std::string alt_trig_obj_label_2;
    std::string alt_trig_obj_label_3;
    std::string leg1_filter;
    std::string leg2_filter;
    std::string alt_leg1_filter;
    std::string alt_jetleg1_filter;
    std::string alt_jetleg2_filter;
    std::string alt_leg1_filter_2;
    std::string alt_leg2_filter_2;
    std::string alt_leg1_filter_3;
    std::string alt_leg2_filter_3;
    std::string alt_leg2_filter;
    
    if (is_data_) { 
      EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");

      unsigned run = eventInfo->run();
      bool path_found     = false;
      auto const& triggerPathPtrVec = event->GetPtrVec<TriggerPath>("triggerPaths");

      for (unsigned i = 0; i < triggerPathPtrVec.size(); ++i) {
        std::string name = triggerPathPtrVec[i]->name();

        if (channel_ == channel::tttt || channel_ == channel::ettt || channel_ == channel::mttt || channel_ == channel::emtt || channel_ == channel::eett || channel_ == channel::mmtt){
          if (run >= 250985 && run <= 284044 && (name.find("HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg_v") != name.npos || name.find("HLT_DoubleMediumCombinedIsoPFTau35_Trk1_eta2p1_Reg_v") != name.npos)) path_found=true;
          if (run >= 294927 && run < 314472 && (name.find("HLT_DoubleTightChargedIsoPFTau35_Trk1_TightID_eta2p1_Reg_v") != name.npos || name.find("HLT_DoubleMediumChargedIsoPFTau40_Trk1_TightID_eta2p1_Reg_v") != name.npos || name.find("HLT_DoubleTightChargedIsoPFTau40_Trk1_eta2p1_Reg_v") != name.npos)) path_found=true;
          // only added vbf tau trg for 2018 here
          if (run >= 314472 && run < 317509 && (name.find("HLT_DoubleTightChargedIsoPFTau35_Trk1_TightID_eta2p1_Reg_v") != name.npos || name.find("HLT_DoubleMediumChargedIsoPFTau40_Trk1_TightID_eta2p1_Reg_v") != name.npos || name.find("HLT_DoubleTightChargedIsoPFTau40_Trk1_eta2p1_Reg_v") != name.npos || name.find("HLT_VBF_DoubleLooseChargedIsoPFTau20_Trk1_eta2p1_v") != name.npos)) path_found=true;
          if (run >= 317509 /*&& run < xxxxx*/ && (name.find("HLT_DoubleMediumChargedIsoPFTauHPS35_Trk1_eta2p1_Reg_v") != name.npos || name.find("HLT_VBF_DoubleLooseChargedIsoPFTauHPS20_Trk1_eta2p1_v") != name.npos )) path_found=true;
        }

         if (path_found) break;
      }
      if (!path_found) return 1;

      if(channel_ == channel::tttt || channel_ == channel::ettt || channel_ == channel::mttt || channel_ == channel::emtt || channel_ == channel::eett || channel_ == channel::mmtt){
        if(run >= 271036 && run <= 284044){
          trig_obj_label = "triggerObjectsDoubleMediumTau35";
          leg1_filter = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
          leg2_filter = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
          if(strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16){
            alt_trig_obj_label = "triggerObjectsDoubleMediumCombinedIsoTau35Reg";
            alt_leg1_filter = "hltDoublePFTau35TrackPt1MediumCombinedIsolationDz02Reg";
            alt_leg2_filter = "hltDoublePFTau35TrackPt1MediumCombinedIsolationDz02Reg";
          }
        }
        if(run >= 294927 && run < 314472){
          trig_obj_label = "triggerObjectsDoubleTightIsoTau35";
          leg1_filter = "hltDoublePFTau35TrackPt1TightChargedIsolationAndTightOOSCPhotonsDz02Reg";
          leg2_filter = "hltDoublePFTau35TrackPt1TightChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_trig_obj_label = "triggerObjectsDoubleMediumIsoTau40";
          alt_leg1_filter = "hltDoublePFTau40TrackPt1MediumChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_leg2_filter = "hltDoublePFTau40TrackPt1MediumChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_trig_obj_label_2 = "triggerObjectsDoubleTightIsoTau40";
          alt_leg1_filter_2 = "hltDoublePFTau40TrackPt1TightChargedIsolationDz02Reg";
          alt_leg2_filter_2 = "hltDoublePFTau40TrackPt1TightChargedIsolationDz02Reg";
        }
        if(run >= 314472 && run < 317509){
          trig_obj_label = "triggerObjectsDoubleTightIsoTau35";
          leg1_filter = "hltDoublePFTau35TrackPt1TightChargedIsolationAndTightOOSCPhotonsDz02Reg";
          leg2_filter = "hltDoublePFTau35TrackPt1TightChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_trig_obj_label = "triggerObjectsDoubleMediumIsoTau40TightID";
          alt_leg1_filter = "hltDoublePFTau40TrackPt1MediumChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_leg2_filter = "hltDoublePFTau40TrackPt1MediumChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_trig_obj_label_2 = "triggerObjectsDoubleTightIsoTau40";
          alt_leg1_filter_2 = "hltDoublePFTau40TrackPt1TightChargedIsolationDz02Reg";
          alt_leg2_filter_2 = "hltDoublePFTau40TrackPt1TightChargedIsolationDz02Reg";
          alt_trig_obj_label_3 = "triggerObjectsVBFDoubleLooseChargedIsoPFTau20";
          alt_leg1_filter_3 = "hltDoublePFTau20TrackLooseChargedIsoAgainstMuon";
          alt_leg2_filter_3 = "hltDoublePFTau20TrackLooseChargedIsoAgainstMuon";
          alt_jetleg1_filter = "hltMatchedVBFOnePFJet2CrossCleanedFromDoubleLooseChargedIsoPFTau20"; // leading 
          //subleading from following (but contains leading as well)
          alt_jetleg2_filter = "hltMatchedVBFTwoPFJets2CrossCleanedFromDoubleLooseChargedIsoPFTau20"; 
        }
        if (run >= 317509) {
          trig_obj_label = "triggerObjectsDoubleMediumIsoTauHPS35";
          leg1_filter = "hltHpsDoublePFTau35TrackPt1MediumChargedIsolationDz02Reg";
          leg2_filter = "hltHpsDoublePFTau35TrackPt1MediumChargedIsolationDz02Reg";
          alt_trig_obj_label_3 = "triggerObjectsVBFDoubleLooseChargedIsoPFTauHPS20";
          alt_leg1_filter_3 = "hltHpsDoublePFTau20TrackLooseChargedIsoAgainstMuon";
          alt_leg2_filter_3 = "hltHpsDoublePFTau20TrackLooseChargedIsoAgainstMuon";
          alt_jetleg1_filter = "hltMatchedVBFOnePFJet2CrossCleanedFromDoubleLooseChargedIsoPFTauHPS20"; // leading 
          //subleading from following (but contains leading as well)
          alt_jetleg2_filter = "hltMatchedVBFTwoPFJets2CrossCleanedFromDoubleLooseChargedIsoPFTauHPS20"; 
        }
      }
      
      if (channel_ == channel::tttt || channel_ == channel::ettt || channel_ == channel::mttt || channel_ == channel::emtt || channel_ == channel::eett || channel_ == channel::mmtt){
       if (era_ == era::data_2016){
         trig_obj_label = "triggerObjectsDoubleMediumTau35";
         alt_trig_obj_label = "triggerObjectsDoubleMediumCombinedIsoTau35Reg";
         if(is_embedded_){ 
           alt_leg1_filter = "hltDoublePFTau35Reg";
           alt_leg2_filter = "hltDoublePFTau35Reg";
           leg1_filter = "hltDoublePFTau35Reg";
           leg2_filter = "hltDoublePFTau35Reg";
         } else {
           leg1_filter = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
           leg2_filter = "hltDoublePFTau35TrackPt1MediumIsolationDz02Reg";
           alt_leg1_filter = "hltDoublePFTau35TrackPt1MediumCombinedIsolationDz02Reg";
           alt_leg2_filter = "hltDoublePFTau35TrackPt1MediumCombinedIsolationDz02Reg";
         }
      } else if(era_ == era::data_2017){
          trig_obj_label = "triggerObjectsDoubleTightIsoTau35";
          leg1_filter = "hltDoublePFTau35TrackPt1TightChargedIsolationAndTightOOSCPhotonsDz02Reg";
          leg2_filter = "hltDoublePFTau35TrackPt1TightChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_trig_obj_label = "triggerObjectsDoubleMediumIsoTau40";
          alt_leg1_filter = "hltDoublePFTau40TrackPt1MediumChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_leg2_filter = "hltDoublePFTau40TrackPt1MediumChargedIsolationAndTightOOSCPhotonsDz02Reg";
          alt_trig_obj_label_2 = "triggerObjectsDoubleTightIsoTau40";
          alt_leg1_filter_2 = "hltDoublePFTau40TrackPt1TightChargedIsolationDz02Reg";
          alt_leg2_filter_2 = "hltDoublePFTau40TrackPt1TightChargedIsolationDz02Reg";
          if(is_embedded_) {
             leg1_filter="hltDoubleL2IsoTau26eta2p2";
             leg2_filter=leg1_filter;
             alt_leg1_filter=leg1_filter;
             alt_leg2_filter=leg1_filter;
             alt_leg1_filter_2=leg1_filter;
             alt_leg2_filter_2=leg1_filter;
          }
        } else if (era_ == era::data_2018) {
            trig_obj_label = "triggerObjectsDoubleMediumIsoTauHPS35"; //HPS only
            leg1_filter = "hltHpsDoublePFTau35TrackPt1MediumChargedIsolationDz02Reg";
            leg2_filter = "hltHpsDoublePFTau35TrackPt1MediumChargedIsolationDz02Reg";
            alt_trig_obj_label_3 = "triggerObjectsVBFDoubleMediumChargedIsoPFTauHPS20";
            alt_leg1_filter_3 = "hltHpsDoublePFTau20TrackMediumChargedIsoAgainstMuon";
            alt_leg2_filter_3 = "hltHpsDoublePFTau20TrackMediumChargedIsoAgainstMuon";
            alt_jetleg1_filter = "hltMatchedVBFOnePFJet2CrossCleanedFromDoubleMediumChargedIsoPFTauHPS20"; // leading 
            //subleading from following (but contains leading as well)
            alt_jetleg2_filter = "hltMatchedVBFTwoPFJets2CrossCleanedFromDoubleMediumChargedIsoPFTauHPS20"; 
            if(is_embedded_) {
               leg1_filter="hltDoubleL2IsoTau26eta2p2";
               leg2_filter=leg1_filter;
               alt_leg1_filter=leg1_filter;
               alt_leg2_filter=leg1_filter;
               alt_leg1_filter_2=leg1_filter;
               alt_leg2_filter_2=leg1_filter;
            }
        }
      }
    }

    std::vector<CompositeCandidate *> & dileptons = event->GetPtrVec<CompositeCandidate>(pair_label_);
    std::vector<TriggerObject *> const& objs = event->GetPtrVec<TriggerObject>(trig_obj_label);

    std::vector<CompositeCandidate *> dileptons_pass;
   
   bool passed_doubletau = false;
   bool passed_vbfdoubletau = false;
   bool match_l1_parts = false;
   bool jetleg1_match = false;
   bool jetleg2_match = false;
   if (channel_ == channel::tttt || channel_ == channel::ettt || channel_ == channel::mttt || channel_ == channel::emtt || channel_ == channel::eett || channel_ == channel::mmtt){
     for(unsigned i = 0; i < dileptons.size(); ++i){
       std::cout << dileptons[i]->At(0)->pt() << " " << dileptons[i]->At(1)->pt() << " " << dileptons[i]->At(2)->pt() << " " << dileptons[i]->At(3)->pt() << " " << std::endl;
       bool leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), objs, leg1_filter, 0.5).first;
       bool leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), objs, leg2_filter, 0.5).first;
       
       if (leg1_match && leg2_match){
         passed_doubletau = true;  
         dileptons_pass.push_back(dileptons[i]);
        } else if(alt_trig_obj_label!=""){
           std::vector<TriggerObject *> alt_objs = event->GetPtrVec<TriggerObject>(alt_trig_obj_label);  
           leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), alt_objs, alt_leg1_filter, 0.5).first;
           leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), alt_objs, alt_leg2_filter, 0.5).first;
           if (leg1_match && leg2_match){
             passed_doubletau = true;  
             dileptons_pass.push_back(dileptons[i]);
          }
        }
        if(!passed_doubletau && alt_trig_obj_label_2!=""){
            std::vector<TriggerObject *> alt_objs_2 = event->GetPtrVec<TriggerObject>(alt_trig_obj_label_2);
            leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), alt_objs_2, alt_leg1_filter_2, 0.5).first;
            leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), alt_objs_2, alt_leg2_filter_2, 0.5).first;
            if (leg1_match && leg2_match){
              passed_doubletau = true;  
              dileptons_pass.push_back(dileptons[i]);
            }
        }
        if(!passed_doubletau && alt_trig_obj_label_3!=""){
            std::vector<TriggerObject *> alt_objs_3 = event->GetPtrVec<TriggerObject>(alt_trig_obj_label_3);
            leg1_match = IsFilterMatchedWithIndex(dileptons[i]->At(0), alt_objs_3, alt_leg1_filter_3, 0.5).first;
            leg2_match = IsFilterMatchedWithIndex(dileptons[i]->At(1), alt_objs_3, alt_leg2_filter_3, 0.5).first;
            if (leg1_match && leg2_match){
              passed_vbfdoubletau = true;  
              dileptons_pass.push_back(dileptons[i]);
            }
        }
        
        if(era_ == era::data_2017){
          // For 2017 MC we have to match the taus to L1 iso taus with pT>32 GeV to fit with how the SFs were measured
              
          std::vector<ic::L1TObject*> l1taus = event->GetPtrVec<ic::L1TObject>("L1Taus");
          std::vector<ic::L1TObject*> passed_l1_taus;
          for(unsigned ta=0; ta<l1taus.size(); ++ta){
            if(l1taus[ta]->isolation()!=0 && l1taus[ta]->vector().Pt() >= 32.) passed_l1_taus.push_back(l1taus[ta]);  
          }
          std::vector<Candidate *> match_taus;
          match_taus.push_back(dileptons[i]->At(0));
          match_taus.push_back(dileptons[i]->At(1));
          match_l1_parts = (MatchByDR(match_taus,passed_l1_taus,0.5,true,true)).size() == 2;
          passed_doubletau = passed_doubletau && match_l1_parts;
        }
      }

      // 2018 vbf trg jet matching after double tau trg part
      if (era_ == era::data_2018 && passed_vbfdoubletau && alt_trig_obj_label_3 != "") {
        std::vector<TriggerObject *> alt_objs_3 = event->GetPtrVec<TriggerObject>(alt_trig_obj_label_3);  
        std::vector<PFJet *> jets = event->GetPtrVec<PFJet>("ak4PFJetsCHS");
        // first need to find jets with maximum mjj and cross clean from taus?
        if (jets.size() >= 2) {
          unsigned int i1 = 0;
          unsigned int j1 = 0;
          double mjj_max = 0;
          
          for (unsigned int ijet = 0; ijet < jets.size()-1; ++ijet){
            for (unsigned int jjet = ijet+1; jjet < jets.size(); ++jjet){
              double mjj_test = (jets[ijet]->vector()+jets[jjet]->vector()).M();

              if (mjj_test > mjj_max){
                mjj_max = mjj_test;
                i1 = ijet;
                j1 = jjet;
              }
            } 
          }

          // add VBF trig here, need to do jet matching
          jetleg1_match = IsFilterMatchedWithIndex(jets[i1], alt_objs_3, alt_jetleg1_filter, 0.5).first;
          jetleg2_match = IsFilterMatchedWithIndex(jets[j1], alt_objs_3, alt_jetleg2_filter, 0.5).first;
          if (jetleg1_match && jetleg2_match) passed_vbfdoubletau = true;
          else passed_vbfdoubletau = false;
        }
      }
    }
    event->Add("trg_doubletau", passed_doubletau);
    event->Add("trg_vbfdoubletau", passed_vbfdoubletau);
    
    if(!do_filter_){
      return 0;    
    }
    
    dileptons = dileptons_pass;
    if (dileptons.size() >= 1) {
      return 0;
    } else {
      return 1;
    }
}

  int HTTTriggerFilter::PostAnalysis() {
    return 0;
  }

  void HTTTriggerFilter::PrintInfo() {
    ;
  }
}
