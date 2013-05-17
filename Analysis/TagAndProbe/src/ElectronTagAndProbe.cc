#include "UserCode/ICHiggsTauTau/Analysis/TagAndProbe/interface/ElectronTagAndProbe.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/PileUpFunctions.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "boost/lexical_cast.hpp"
#include "boost/bind.hpp"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "TH1.h"
#include <string>
#include <map>
#include <iostream>
#include "UserCode/ICHiggsTauTau//interface/city.h"

namespace ic {

  ElectronTagAndProbe::ElectronTagAndProbe(std::string const& name) : ModuleBase(name) {
   min_mass_=0;
   max_mass_=200;
   hist_bins_=200;
   probe_match=false;
   fs_=NULL;
   run_low_=0;
   run_high_=10000000;
  }

  ElectronTagAndProbe::~ElectronTagAndProbe() {
  }
  
  bool ElectronTagAndProbe::hasL1MET(TreeEvent *event, std::vector<TriggerObject *> &etau_objs) {
    bool hasL1MET=false;
    if(data_)
    {
        std::string filter="hltL1sL1IsoEG12erETM36";
        std::size_t hash = CityHash64(filter);
        for (unsigned i = 0; i < etau_objs.size(); ++i)
        {
            std::vector<std::size_t> const& labels = etau_objs[i]->filters();
            if (std::find(labels.begin(),labels.end(), hash) != labels.end()) hasL1MET=true;
            std::vector<Candidate *> const& l1met = event->GetPtrVec<Candidate>("l1extraMET");
            if(hasL1MET && (l1met.at(0)->pt() < 36)) hasL1MET=false;
        }
    }
    else
    {
        std::vector<Candidate *> const& l1met = event->GetPtrVec<Candidate>("l1extraMET");
        hasL1MET = l1met.at(0)->pt() > 36.;
    }
    
    return hasL1MET;  
  }

  bool ElectronTagAndProbe::PassL1Lepton(TreeEvent *event, std::vector<TriggerObject *> &etau_objs, Electron* probeElectron) {
    ic::erase_if(etau_objs, !boost::bind(MinPtMaxEta, _1, 13.0, 2.1));
    std::vector<Candidate *> l1emiso = event->GetPtrVec<Candidate>("l1extraEmIsolated");
    ic::erase_if(l1emiso, !boost::bind(MinPtMaxEta, _1, 12.0, 2.17));
    std::vector<Candidate *> ele_as_vector;
    ele_as_vector.push_back(probeElectron);
    bool leg1_l1_match = MatchByDR(ele_as_vector, l1emiso, 0.5, false, false).size() > 0;
    return leg1_l1_match;
  }

  int ElectronTagAndProbe::PreAnalysis() {
    std::string dataormc;
    if(data_) dataormc="data";
    if(!data_) dataormc="MC";
    //fs_ = new fwlite::TFileService("eeTandP"+dataormc+era_+".root");
    hists_ = new DynamicHistoSet(fs_->mkdir("/"));
    
    object_count=0;
    passing_count=0;
    failing_count=0;

    // Set up necessary histograms depending on mode. These histograms are named such
    // that the fitting code will still work.
    std::string label;
    if(mode_==0 || mode_==10) label="id_";          //0=ID, 10=ID in finer bins
    if(mode_==1 || mode_==11) label="iso_";         //1=Iso, 11=iso in finer bins
    if(mode_==2 || mode_==12) label="idiso_";       //2=IDIso, 12=IDIso in finer bins
    if(mode_==3) label="";                          //3=Trigger efficiencies for etau channel
    if(mode_==4) label="";                          //4=Trigger efficiencies for etau+met channel

    if(mode_==0 || mode_==1 || mode_==2 || mode_==3 || mode_==4)
    {
        for(unsigned i=0; i<pt_bins_.size()-1; i++)
        {
            std::string s=boost::lexical_cast<std::string>(i+1);
            hists_->Create(label+"h_TP_"+s+"B", hist_bins_, min_mass_, max_mass_);
            hists_->Create(label+"h_TF_"+s+"B", hist_bins_, min_mass_, max_mass_);
            hists_->Create(label+"h_TP_"+s+"E", hist_bins_, min_mass_, max_mass_);
            hists_->Create(label+"h_TF_"+s+"E", hist_bins_, min_mass_, max_mass_);
        }
    }
    if(mode_==10 || mode_==11 || mode_==12)
    {
        for(unsigned i=0; i<pt_bins_.size()-1; i++)
        {
            std::string s=boost::lexical_cast<std::string>(i+1);
            hists_->Create(label+"h_TP_"+s+"pt", hist_bins_, min_mass_, max_mass_);
            hists_->Create(label+"h_TF_"+s+"pt", hist_bins_, min_mass_, max_mass_);
        }
        for(unsigned i=0; i<eta_bins_.size()-1; i++)
        {
            std::string s=boost::lexical_cast<std::string>(i+1);
            hists_->Create(label+"h_TP_"+s+"eta", hist_bins_, min_mass_, max_mass_);
            hists_->Create(label+"h_TF_"+s+"eta", hist_bins_, min_mass_, max_mass_);
        }
        for(unsigned i=0; i<vtx_bins_.size()-1; i++)
        {
            std::string s=boost::lexical_cast<std::string>(i+1);
            hists_->Create(label+"h_TP_"+s+"vtx", hist_bins_, min_mass_, max_mass_);
            hists_->Create(label+"h_TF_"+s+"vtx", hist_bins_, min_mass_, max_mass_);
        }
    }
    if(mode_==0) hists_->Create("h_PUWeights", 200, 0, 10);
    
    outFile.open((output_name_+"_"+dataormc+".txt").c_str());

    //std::map<int, int> nrun;
    //std::map<int, double> prescales;

    return 0;
  }

  int ElectronTagAndProbe::Execute(TreeEvent *event) {
    
    
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    int run = eventInfo->run();                 
 
    std::string tap_obj_label;
    std::string tag_filter_e;
    std::string probe_filter_e;
    std::string trigger_name_e;
    std::string etau_obj_label;
    std::string etau_filter;
    
    //Set trigger filter labels
    //Set etau trigger names if measuring trigger efficiencies.
    //In 2011 data the triggers used are split by run range. To make a correct efficiency measurement
    //the user must ensure only events within the run range are used.
    //for MC the trigger labels are fixed by "era".

    // 2011 Triggers
    if ( (data_ && run >= 160404 && run <= 163869) /*|| (!data_ && era_=="2011AMay10")*/)
    {
        etau_obj_label = "triggerObjectsEle15LooseTau15";
        etau_filter = "hltEle15CaloIdVTTrkIdTCaloIsoTTrkIsoTTrackIsolFilter";
    }
    if ( (data_ && run >= 165088 && run <= 167913) /*|| (!data_ && era_=="2011APrompt4")*/)
    {
        etau_obj_label = "triggerObjectsEle15LooseTau20";
        etau_filter = "hltEle15CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter";
    }
    if ( (data_ && run >= 170249 && run <= 173198) /*|| (!data_ && era_=="2011AAug05")*/)
    {
        etau_obj_label = "triggerObjectsEle15TightTau20";
        etau_filter = "hltEle15CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter";
    }
    if ( (data_ && run >= 173236 && run <= 178380) || (!data_ && (era_=="2011AOct03" || era_=="2011BPrompt1") ))
    {
        etau_obj_label = "triggerObjectsEle18MediumTau20";
        etau_filter = "hltEle18CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilter";
    }
    if (data_ && run >= 178420 && run <= 180252)
    {
        etau_obj_label = "triggerObjectsEle20MediumTau20";
        etau_filter = "hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20";
    }
    // 2012 Triggers
    if ((data_ && run >= 190456 && run <= 193751) || (!data_ && era_=="2012A"))
    {
        etau_obj_label = "triggerObjectsEle20RhoLooseTau20";
        etau_filter = "hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1IsoEG18OrEG20";
    }
    if ((data_ && run >= 193752/* && run <= xxxxx*/) ||
            (!data_ && (era_=="2012B"||era_=="2012C"||era_=="2012D" ) ))
    {
        etau_obj_label = "triggerObjectsEle22WP90RhoLooseTau20";
        etau_filter = "hltEle22WP90RhoTrackIsoFilter";
    }
    if(mode_==4 && data_)
    {    
        etau_obj_label = "triggerObjectsEle13LooseTau20L1ETM36";
        etau_filter = "hltEle13WP90RhoTrackIsoFilter";
    }
    if(mode_==4 && !data_)
    {    
        etau_obj_label = "triggerObjectsEle8";
        etau_filter = "hltEle8TightIdLooseIsoTrackIsoFilter";
    }

    //set tag and probe trigger names and filter labels
   
   if(data_ && run >= 190456)
    {
        tag_filter_e="hltEle27WP80TrackIsoFilter";
        trigger_name_e="HLT_Ele27_WP80_v";
        tap_obj_label="triggerObjectsEle27";
    } 
    else if(data_ && run >= 160329 && run <= 163869)
    {
        tag_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8TrackIsolFilter";
        probe_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8PMMassFilter";
        trigger_name_e="HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v";
        tap_obj_label="triggerObjectsEle17SC8";
    }
    else if(data_ && run >= 165071 && run <= 168437)
    {
        tag_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8TrackIsolFilter";
        probe_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8PMMassFilter";
        //probe_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8PMMassFilter"; 
       // tag_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8TrackIsolFilter";      
        trigger_name_e="HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass30_v";
        //trigger_name_e="HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v";
        tap_obj_label="triggerObjectsEle17Ele8";
    }
    else if(data_ && run >= 170053 && run <= 172619)
    {
        tag_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8TrackIsolFilter";
        probe_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8PMMassFilter";
     //   probe_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8PMMassFilter";  
      //  tag_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8TrackIsolFilter"; 
        trigger_name_e="HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass30_v";
        //trigger_name_e="HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v";
        tap_obj_label="triggerObjectsEle17Ele8";
    }
    else if(data_ && run >= 172620 && run <= 175770)
    {
        tag_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8TrackIsolFilter";
        probe_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8PMMassFilter";
//        probe_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8PMMassFilter";     
//        tag_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8TrackIsolFilter";   
        trigger_name_e="HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass30_v";
        //trigger_name_e="HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v";
        tap_obj_label="triggerObjectsEle17Ele8";
    }
    else if(data_ && run >= 175832 && run <= 180296)
    {
        tag_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8TrackIsolFilter";
        probe_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8PMMassFilter";
        //probe_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8PMMassFilter";     
       // tag_filter_e="hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8TrackIsolFilter";   
        trigger_name_e="HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass30_v";
        //trigger_name_e="HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v";
        tap_obj_label="triggerObjectsEle17Ele8";
    }

    std::vector<Electron *> & electrons = event->GetPtrVec<Electron>("electrons");
    std::vector<TriggerObject *> objs;
    if(data_) objs = event->GetPtrVec<TriggerObject>(tap_obj_label);
    std::vector<TriggerObject *> etau_objs = event->GetPtrVec<TriggerObject>(etau_obj_label);
    TriggerPathPtrVec triggerPathPtrVec;
    if(data_) triggerPathPtrVec = event->GetPtrVec<TriggerPath>("triggerPathPtrVec" , "triggerPaths");
    double vtx = eventInfo->good_vertices();
    std::vector<PileupInfo *> puInfo;
    if(!data_)  puInfo= event->GetPtrVec<PileupInfo>("pileupInfo"); 
  


   //Calculate PU weight for each event (if MC).

    double w=1.0;
    if(!data_)
    { 
        w = eventInfo->weight("pileup");
    }
    bool trigger=false;
    bool etau_trigger=false;
    int prescalenum;
    
    //See if trigger has fired (if data).

    if(data_)
    {
        for(unsigned k=0; k<triggerPathPtrVec.size(); k++)
        {
            std::string name=(triggerPathPtrVec[k])->name();
            if(name.find(trigger_name_e)!=name.npos)
            {
                trigger=true;
       //         prescalenum=triggerPathPtrVec[k]->prescale();
            }
        }
       /* for(unsigned k=0; k<triggerPathPtrVec.size(); k++)
        {
            std::string name=(triggerPathPtrVec[k])->name();
            if(name.find("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v")!=name.npos)
            {
                etau_trigger=true;
                prescalenum=triggerPathPtrVec[k]->prescale();
               if(etau_trigger && prescalenum >1) std::cout << prescalenum << std::endl;
            }
        }*/
    }
    else trigger=true;
/*    
    bool hasL1MET=false;

    std::string filter="hltL1sL1IsoEG12erETM36";
    std::size_t hash = CityHash64(filter);
    if(data_)
    {
        for (unsigned i = 0; i < etau_objs.size(); ++i)
        {
            std::vector<std::size_t> const& labels = etau_objs[i]->filters();
            if (std::find(labels.begin(),labels.end(), hash) != labels.end()) hasL1MET=true;
        }
    }
    else hasL1MET=true;
  */  

    std::vector<Electron *> tag_vector; 
    std::vector<Electron *> probe_vector; 
    bool good_tag=false;
    //Make vectors of tag and probe candidates.

    //std::cout << run_low_ << " " << run_high_ <<std::endl; 
    if(electrons.size()>1 && trigger && ((data_ && run>=run_low_ && run<=run_high_) || !data_))
    {
        //if(etau_trigger) prescales[run]+=prescalenum; 
        //if(etau_trigger) nrun[run]+=1;
        //std::cout << run << " " << etau_obj_label << std::endl;
        for(unsigned e1=0; e1<electrons.size(); e1++)
        {
            if( tag_predicate_((electrons[e1]))
                    && (!data_ || (data_ && IsFilterMatched((electrons)[e1], objs, tag_filter_e, 0.5)) ))
            { 
                tag_vector.push_back((electrons)[e1]);
                good_tag=true;
            } 
            if( probe_predicate_((electrons[e1])) 
                    && (!data_ || !probe_match || (data_ && IsFilterMatched((electrons)[e1], objs, probe_filter_e, 0.5)) )
                  && (!(mode_==4) || hasL1MET(event, etau_objs))  )
            {
                probe_vector.push_back((electrons)[e1]);
            }
        }
    }
  
    if(good_tag)
    {
        hists_->Fill("h_PUWeights",w);
    }


    //Save good tag/probe pairs.
    bool flag=false; 
    std::vector<std::pair <Electron *, Electron *> > pairs;
    for(unsigned i=0; i<tag_vector.size(); i++)
    {
        for(unsigned j=0; j<probe_vector.size(); j++)
        {
            if(((tag_vector[i]->vector()) + (probe_vector[j]->vector())).M()>60
                && ((tag_vector[i]->vector()) + (probe_vector[j]->vector())).M()<120
                && tag_vector[i]->charge()*probe_vector[j]->charge()<0)
            { 
                std::pair<Electron*,Electron*> p(tag_vector[i], probe_vector[j]);
                pairs.push_back(p);
                flag=true;
            }
        }
    }

    double mass;
    std::string label;
    if(mode_==0 || mode_==10) label="id_";
    if(mode_==1 || mode_==11) label="iso_";
    if(mode_==2 || mode_==12) label="idiso_";
    if(mode_==3) label="";
    if(mode_==4) label="";
    
    
    //Fill passing and failing histograms.
    if(flag)
    {
        for(unsigned k_final=0; k_final<pairs.size(); k_final++)
        {
            mass=((pairs[k_final].first)->vector()+(pairs[k_final].second)->vector()).M();
            for(unsigned i=0; i<pt_bins_.size(); i++)
            {
                std::string s=boost::lexical_cast<std::string>(i+1);
                if((!(mode_==3) && !(mode_==4) && passprobe_predicate_(pairs[k_final].second)) ||
                        ((mode_==3)  && IsFilterMatched(pairs[k_final].second,etau_objs,etau_filter,0.5 )) || 
                        ((mode_==4) && IsFilterMatched(pairs[k_final].second,etau_objs,etau_filter,0.5 ) && PassL1Lepton(event, etau_objs, pairs[k_final].second) ) )
                {
                    passing_count++;
                  //  if(run==203994 && eventInfo->lumi_block()>318){std::cout << "Passing: " << "Run= " << run << " Lumi= " << eventInfo->lumi_block() << " Event= " << eventInfo->event() << std::endl;}
                    if(mode_==0 || mode_==1 || mode_==2 || mode_==3 || mode_==4)
                    {
                        if(fabs((pairs[k_final].second)->sc_eta()) < eta_bins_[0])
                        {
                            if((pairs[k_final].second)->pt()>pt_bins_[i] 
                                    && (pairs[k_final].second)->pt()<=pt_bins_[i+1])
                            {
                                hists_->Fill((label+"h_TP_"+s+"B"), mass, w);
                            }
                        }
                        if(fabs((pairs[k_final].second)->sc_eta()) >= eta_bins_[0])
                        {
                            if((pairs[k_final].second)->pt()>pt_bins_[i] 
                                    && (pairs[k_final].second)->pt()<=pt_bins_[i+1])
                            {
                                hists_->Fill((label+"h_TP_"+s+"E"), mass, w);
                            }
                        }
                    }
                    if(mode_==10 || mode_==11 || mode_==12)
                    {
                        if((pairs[k_final].second)->pt()>pt_bins_[i] 
                                && (pairs[k_final].second)->pt()<=pt_bins_[i+1])
                        {
                            hists_->Fill((label+"h_TP_"+s+"pt"), mass, w);
                        } 
                    }
                }
                if ((!(mode_==3) && !(mode_==4) && !passprobe_predicate_(pairs[k_final].second))||
                        ((mode_==3) && !IsFilterMatched(pairs[k_final].second,etau_objs,etau_filter,0.5 )) ||
                        ((mode_==4) && (!IsFilterMatched(pairs[k_final].second,etau_objs,etau_filter,0.5 ) || !PassL1Lepton(event, etau_objs, pairs[k_final].second)) ) )
                {
                    //if(run==206210 && eventInfo->lumi_block() < 195){std::cout << "Failing: " << "Run= " << run << " Lumi= " << eventInfo->lumi_block() << " Event= " << eventInfo->event() << std::endl;}
                    failing_count++;
                    if(mode_==0 || mode_==1 || mode_==2 || mode_==3 || mode_==4)
                    {
                        if(fabs((pairs[k_final].second)->sc_eta()) < eta_bins_[0])
                        {
                            if((pairs[k_final].second)->pt()>pt_bins_[i] 
                                    && (pairs[k_final].second)->pt()<=pt_bins_[i+1])
                            {
                                hists_->Fill((label+"h_TF_"+s+"B"), mass, w);
                            }
                        }
                        if(fabs((pairs[k_final].second)->sc_eta()) >= eta_bins_[0])
                        {
                            if((pairs[k_final].second)->pt()>pt_bins_[i] 
                                    && (pairs[k_final].second)->pt()<=pt_bins_[i+1])
                            {
                                hists_->Fill((label+"h_TF_"+s+"E"), mass, w);
                            }
                        }
                    }
                    if(mode_==10 || mode_==11 || mode_==12)
                    {
                        if((pairs[k_final].second)->pt()>pt_bins_[i] 
                                && (pairs[k_final].second)->pt()<=pt_bins_[i+1])
                        {
                            hists_->Fill((label+"h_TF_"+s+"pt"), mass, w);
                        } 
                    }
                }
            }
            if(mode_==10 || mode_==11 || mode_==12)
            {
                for(unsigned i=0; i<eta_bins_.size(); i++)
                {
                    std::string s=boost::lexical_cast<std::string>(i+1);
                    if(passprobe_predicate_(pairs[k_final].second) )
                    {
                        if((pairs[k_final].second)->sc_eta()>eta_bins_[i] 
                                && (pairs[k_final].second)->sc_eta()<=eta_bins_[i+1])
                        {
                            hists_->Fill((label+"h_TP_"+s+"eta"), mass, w);
                        } 
                    }
                    else
                    {
                        if((pairs[k_final].second)->sc_eta()>eta_bins_[i] 
                                && (pairs[k_final].second)->sc_eta()<=eta_bins_[i+1])
                        {
                            hists_->Fill((label+"h_TF_"+s+"eta"), mass, w);
                        } 
                    }
                }
                for(unsigned i=0; i<vtx_bins_.size(); i++)
                {
                    std::string s=boost::lexical_cast<std::string>(i+1);
                    if(passprobe_predicate_(pairs[k_final].second) )
                    {
                        if(vtx>vtx_bins_[i] && vtx<=vtx_bins_[i+1])
                        {
                            hists_->Fill((label+"h_TP_"+s+"vtx"), mass, w);
                        } 
                    }
                    else
                    {
                        if(vtx>vtx_bins_[i] && vtx<=vtx_bins_[i+1])
                        {
                            hists_->Fill((label+"h_TF_"+s+"vtx"), mass, w);
                        } 
                    }
                }
            }
        }
    }

    return 0;
  }
  int ElectronTagAndProbe::PostAnalysis() {
    std::string label;
    if(mode_==0 || mode_==10) label="id_";
    if(mode_==1 || mode_==11) label="iso_";
    if(mode_==2 || mode_==12) label="idiso_";
    if(mode_==3) label="";
    if(mode_==4) label="";
     
    if(mode_==0 || mode_==1 || mode_==2 || mode_==3 || mode_==4)
    {
         std::cout << "====================="+label+"====================" << std::endl;
         for(unsigned i=0; i<pt_bins_.size()-1; i++)
         {
             std::string s=boost::lexical_cast<std::string>(i+1);
             outFile << pt_bins_[i] << " < pt < " << pt_bins_[i+1] << " Barrel: " << ((hists_->Get_Histo(label+"h_TP_"+s+"B")->Integral())) /
                    ( (hists_->Get_Histo(label+"h_TP_"+s+"B")->Integral()) +(hists_->Get_Histo(label+"h_TF_"+s+"B")->Integral())) << std::endl;
             outFile << pt_bins_[i] << " < pt < " << pt_bins_[i+1] << " Endcaps: " << ((hists_->Get_Histo(label+"h_TP_"+s+"E")->Integral())) /
                    ( (hists_->Get_Histo(label+"h_TP_"+s+"E")->Integral()) +(hists_->Get_Histo(label+"h_TF_"+s+"E")->Integral())) << std::endl;

             std::cout << pt_bins_[i] << " < pt < " << pt_bins_[i+1] << " Barrel: " << ((hists_->Get_Histo(label+"h_TP_"+s+"B")->Integral())) /
                    ( (hists_->Get_Histo(label+"h_TP_"+s+"B")->Integral()) +(hists_->Get_Histo(label+"h_TF_"+s+"B")->Integral())) << std::endl;
             std::cout << pt_bins_[i] << " < pt < " << pt_bins_[i+1] << " Endcaps: " << ((hists_->Get_Histo(label+"h_TP_"+s+"E")->Integral())) /
                    ( (hists_->Get_Histo(label+"h_TP_"+s+"E")->Integral()) +(hists_->Get_Histo(label+"h_TF_"+s+"E")->Integral())) << std::endl;
         }
             std::cout << "================================================" << std::endl;
    } 
    if(mode_==10 || mode_==11 || mode_==12)
    {
         std::cout << "====================="+label+"====================" << std::endl;
         for(unsigned i=0; i<pt_bins_.size()-1; i++)
         {
             std::string s=boost::lexical_cast<std::string>(i+1);
             outFile << pt_bins_[i] << " < pt < " << pt_bins_[i+1] << " all eta: " << ((hists_->Get_Histo(label+"h_TP_"+s+"pt")->Integral())) /
                    ( (hists_->Get_Histo(label+"h_TP_"+s+"pt")->Integral()) +(hists_->Get_Histo(label+"h_TF_"+s+"pt")->Integral())) << std::endl;

             std::cout << pt_bins_[i] << " < pt < " << pt_bins_[i+1] << " all eta: " << ((hists_->Get_Histo(label+"h_TP_"+s+"pt")->Integral())) /
                    ( (hists_->Get_Histo(label+"h_TP_"+s+"pt")->Integral()) +(hists_->Get_Histo(label+"h_TF_"+s+"pt")->Integral())) << std::endl;
         }
             std::cout << "================================================" << std::endl;
         for(unsigned i=0; i<eta_bins_.size()-1; i++)
         {
             std::string s=boost::lexical_cast<std::string>(i+1);
             outFile << eta_bins_[i] << " < eta < " << eta_bins_[i+1] << " all pt: " << ((hists_->Get_Histo(label+"h_TP_"+s+"eta")->Integral())) /
                    ( (hists_->Get_Histo(label+"h_TP_"+s+"eta")->Integral()) +(hists_->Get_Histo(label+"h_TF_"+s+"eta")->Integral())) << std::endl;

             std::cout << eta_bins_[i] << " < eta < " << eta_bins_[i+1] << " all pt: " << ((hists_->Get_Histo(label+"h_TP_"+s+"eta")->Integral())) /
                    ( (hists_->Get_Histo(label+"h_TP_"+s+"eta")->Integral()) +(hists_->Get_Histo(label+"h_TF_"+s+"eta")->Integral())) << std::endl;
         }
             std::cout << "================================================" << std::endl;
         for(unsigned i=0; i<vtx_bins_.size()-1; i++)
         {
             std::string s=boost::lexical_cast<std::string>(i+1);
             outFile << vtx_bins_[i] << " < vtx < " << vtx_bins_[i+1] << " all pt,eta: " << ((hists_->Get_Histo(label+"h_TP_"+s+"vtx")->Integral())) /
                    ( (hists_->Get_Histo(label+"h_TP_"+s+"vtx")->Integral()) +(hists_->Get_Histo(label+"h_TF_"+s+"vtx")->Integral())) << std::endl;

             std::cout << vtx_bins_[i] << " < vtx < " << vtx_bins_[i+1] << " all pt,eta: " << ((hists_->Get_Histo(label+"h_TP_"+s+"vtx")->Integral())) /
                    ( (hists_->Get_Histo(label+"h_TP_"+s+"vtx")->Integral()) +(hists_->Get_Histo(label+"h_TF_"+s+"vtx")->Integral())) << std::endl;
         }
             std::cout << "================================================" << std::endl;
     } 
     //outFile.close(); 

  /*  std::map<int,int>::const_iterator itr1;
    std::map<int,int>::const_iterator itr2;

    for(itr1=nrun.begin(); itr1!=nrun.end(); ++itr1)
    {
        for(itr2=prescales.begin(); itr2!=prescales.end(); ++itr2)
        {
            if((*itr1).first==(*itr2).first)
            {
                //std::cout << "RUN: " << (*itr1).first << " MEAN PRESCALE: " << ( (double((*itr2).second))/(double((*itr1).second)) ) << std::endl;
                std::cout << "RUN: " << (*itr1).first << " NRUNS: " << (*itr1).second << " SUMPRESCALE: " << (*itr2).second<< " MEANPRESCALE: " <<  ( (double((*itr2).second))/(double((*itr1).second)) ) << std::endl;
            }
        }
    }
    
*/


    return 0;
  }

  void ElectronTagAndProbe::PrintInfo() {
    ;
  }
}
