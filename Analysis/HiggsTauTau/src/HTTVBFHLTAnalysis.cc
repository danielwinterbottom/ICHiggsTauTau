#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTVBFHLTAnalysis.h"
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnRootTools.h"
#include "DataFormats/Math/interface/deltaR.h"


double L1pass;
double L1fail;
double L1failSecond;
double L1size0;
double L1size1;
double Calosize0;
double HLTsize0;
double CaloL1tot;
double CaloL1matched;
bool CaloL1;
double two_jet;
double three_jet;

double vbfpass;
double vbfl1pass;
double metmhtpass;
double isomupass;
bool data = true;



struct PtComparator{
    bool operator() (ic::Candidate a, ic::Candidate b) {
        return (a.vector().Pt() > b.vector().Pt());
    }
};





struct PtComparatorTriggerObj{
    bool operator() (ic::TriggerObject *a, ic::TriggerObject *b) {
        return (a->vector().Pt() > b->vector().Pt());
    }
};

struct PtComparatorOfflineObj{
    bool operator() (ic::PFJet *a, ic::PFJet *b) {
        return (a->vector().Pt() > b->vector().Pt());
    }
};

struct PtComparatorL1Obj{
    bool operator() (ic::L1TObject *a, ic::L1TObject *b) {
        return (a->vector().Pt() > b->vector().Pt());
    }
};

bool IsFilterMatchedWithName(ic::TriggerObject *objs, std::string const& filter){
    std::size_t hash = CityHash64(filter);
    
    std::vector<std::size_t> const& labels = objs->filters();
    if (std::find(labels.begin(),labels.end(), hash) == labels.end())  return false;
    return true;
}




namespace ic {
    
    HTTVBFHLTAnalysis::HTTVBFHLTAnalysis(std::string const& name) : ModuleBase(name) {
        fs_ = NULL;
    }
    
    HTTVBFHLTAnalysis::~HTTVBFHLTAnalysis() {
        ;
    }
    
    int HTTVBFHLTAnalysis::PreAnalysis() {
        
        if(fs_){
            outtree_ = fs_->make<TTree>("HLT_trigger_ntuple","HLT_trigger_ntuple");
            
            //VBF HLT properties
            outtree_->Branch("event"                     , &event_       );
            
            outtree_->Branch("l1_jpt_1"              , &l1_jpt_1_       );
            outtree_->Branch("l1_jpt_2"              , &l1_jpt_2_       );
            outtree_->Branch("l1_jpt_3"              , &l1_jpt_3_       );
            outtree_->Branch("l1_jpt_4"              , &l1_jpt_4_       );
            
            outtree_->Branch("l1_jeta_1"             , &l1_jeta_1_       );
            outtree_->Branch("l1_jeta_2"             , &l1_jeta_2_       );
            outtree_->Branch("l1_jeta_3"             , &l1_jeta_3_       );
            outtree_->Branch("l1_jeta_4"             , &l1_jeta_4_       );
            
            outtree_->Branch("l1_jpt_2d_plot"              , &l1_jpt_2d_plot_       );
            
            outtree_->Branch("l1_mjj"                , &l1_mjj_       );
            outtree_->Branch("l1_mjj30"              , &l1_mjj30_       );
            
            outtree_->Branch("l1_etajj"              , &l1_etajj_       );
            outtree_->Branch("l1_phijj"              , &l1_phijj_       );
            
            
            outtree_->Branch("vbf_l1_jpt_1"              , &vbf_l1_jpt_1_       );
            outtree_->Branch("vbf_l1_jpt_2"              , &vbf_l1_jpt_2_       );
            outtree_->Branch("vbf_l1_jpt_3"              , &vbf_l1_jpt_3_       );
            outtree_->Branch("vbf_l1_jpt_4"              , &vbf_l1_jpt_4_       );
            
            outtree_->Branch("vbf_l1_jpt_2d_plot"              , &vbf_l1_jpt_2d_plot_       );
            
            outtree_->Branch("vbf_l1_jeta_1"             , &vbf_l1_jeta_1_       );
            outtree_->Branch("vbf_l1_jeta_2"             , &vbf_l1_jeta_2_       );
            outtree_->Branch("vbf_l1_jeta_3"             , &vbf_l1_jeta_3_       );
            outtree_->Branch("vbf_l1_jeta_4"             , &vbf_l1_jeta_4_       );
            
            outtree_->Branch("vbf_l1_mjj"                , &vbf_l1_mjj_       );
            outtree_->Branch("vbf_l1_mjj30"              , &vbf_l1_mjj30_       );
            
            outtree_->Branch("vbf_l1_etajj"              , &vbf_l1_etajj_       );
            outtree_->Branch("vbf_l1_phijj"              , &vbf_l1_phijj_       );
            
            outtree_->Branch("vbf_pf_jpt_1"              , &vbf_pf_jpt_1_       );
            outtree_->Branch("vbf_pf_jpt_2"              , &vbf_pf_jpt_2_       );
            outtree_->Branch("vbf_pf_jpt_3"              , &vbf_pf_jpt_3_       );
            outtree_->Branch("vbf_pf_jpt_4"              , &vbf_pf_jpt_4_       );
            
            outtree_->Branch("vbf_pf_jpt_2d_plot"              , &vbf_pf_jpt_2d_plot_       );
            
            
            outtree_->Branch("vbf_pf_jeta_1"             , &vbf_pf_jeta_1_       );
            outtree_->Branch("vbf_pf_jeta_2"             , &vbf_pf_jeta_2_       );
            outtree_->Branch("vbf_pf_jeta_3"             , &vbf_pf_jeta_3_       );
            outtree_->Branch("vbf_pf_jeta_4"             , &vbf_pf_jeta_4_       );
            
            outtree_->Branch("vbf_pf_mjj"                , &vbf_pf_mjj_       );
            outtree_->Branch("vbf_pf_mjj30"              , &vbf_pf_mjj30_       );
            
            outtree_->Branch("vbf_calo_met"              , &vbf_calo_met_       );
            outtree_->Branch("vbf_noise_cleaned_calo_met", &vbf_noise_cleaned_calo_met       );
            outtree_->Branch("vbf_pf_met"                , &vbf_pf_met_       );
            
            //Offline properties
            outtree_->Branch("leading_jpt_1"             , &leading_jpt_1_       );
            outtree_->Branch("leading_jpt_2"             , &leading_jpt_2_       );
            outtree_->Branch("leading_jeta_1"            , &leading_jeta_1_       );
            outtree_->Branch("leading_jeta_2"            , &leading_jeta_2_       );
            outtree_->Branch("leading_mjj"               , &leading_mjj_       );
            
            
            outtree_->Branch("offline_jpt_1"             , &offline_jpt_1_       );
            outtree_->Branch("offline_jpt_2"             , &offline_jpt_2_       );
            outtree_->Branch("offline_jpt_3"             , &offline_jpt_3_       );
            
            outtree_->Branch("offline_jeta_1"            , &offline_jeta_1_       );
            outtree_->Branch("offline_jeta_2"            , &offline_jeta_2_       );
            outtree_->Branch("offline_jeta_3"            , &offline_jeta_3_       );
            
            outtree_->Branch("offline_detajj"            , &offline_detajj_       );
            outtree_->Branch("offline_dphijj"              , &offline_dphijj_       );
            
            outtree_->Branch("offline_mjj"               , &offline_mjj_       );
            outtree_->Branch("offline_mjj30"             , &offline_mjj30_       );
            
            outtree_->Branch("offline_deta"              , &offline_detajj_       );
            outtree_->Branch("offline_dphi"              , &offline_dphijj_       );
            
            outtree_->Branch("offline_matched_jpt_1"             , &offline_matched_jpt_1_       );
            outtree_->Branch("offline_matched_jpt_2"             , &offline_matched_jpt_2_       );
            outtree_->Branch("offline_matched_jpt_3"             , &offline_matched_jpt_3_       );
            
            outtree_->Branch("offline_matched_jeta_1"            , &offline_matched_jeta_1_       );
            outtree_->Branch("offline_matched_jeta_2"            , &offline_matched_jeta_2_       );
            outtree_->Branch("offline_matched_jeta_3"            , &offline_matched_jeta_3_       );
            
            outtree_->Branch("offline_matched_detajj"            , &offline_matched_detajj_       );
            outtree_->Branch("offline_matched_dphijj"              , &offline_matched_dphijj_       );
            
            outtree_->Branch("offline_matched_mjj"               , &offline_matched_mjj_       );
            outtree_->Branch("offline_matched_mjj30"             , &offline_matched_mjj30_       );
            
            outtree_->Branch("offline_matched_deta"              , &offline_matched_detajj_       );
            outtree_->Branch("offline_matched_dphi"              , &offline_matched_dphijj_       );
            
        
            
            outtree_->Branch("offline_met"               , &offline_met_       );
            
            outtree_->Branch("VBFPassed"               , &VBFPassed_       );
            outtree_->Branch("VBFL1Passed"               , &VBFL1Passed_       );
            outtree_->Branch("METMHTPassed"               , &METMHTPassed_       );
            outtree_->Branch("IsoMuPassed"               , &IsoMuPassed_       );
            
            
            
        }
       
        return 0;
    }
    
    
    int HTTVBFHLTAnalysis::Execute(TreeEvent *event) {
        
        
        std::vector<TriggerObject *> const& VBFobjs =    event->GetPtrVec<TriggerObject>("DiJet110_35_Mjj650_PFMET110");
        std::vector<TriggerObject *> const& VBFobjs3 =   event->GetPtrVec<TriggerObject>("TripleJet110_35_35_Mjj650_PFMET110");
        std::vector<TriggerObject *> const& METMHTobjs = event->GetPtrVec<TriggerObject>("PFMETNoMu120_PFMHTNoMu120_IDTight");
        std::vector<TriggerObject *> const& IsoMuobjs =  event->GetPtrVec<TriggerObject>("triggerObjectsIsoMu27");
        
        
        
        
        std::vector<TriggerObject *>  L1jets;
        std::vector<TriggerObject *>  Calojets;
        std::vector<TriggerObject *>  HLT_PF_jets;
        std::vector<TriggerObject *>  HLT_All_PF_jets;
        std::vector<TriggerObject *>  L1met;
        std::vector<TriggerObject *>  Calomet;
        std::vector<TriggerObject *>  HLTmet;
        
        vbf_l1_jpt_1_=-9999;
        vbf_l1_jpt_2_=-9999;
        vbf_l1_jpt_3_=-9999;
        vbf_l1_jpt_4_=-9999;
        
        vbf_l1_jpt_2d_plot_=-9999;
        l1_jpt_2d_plot_=-9999;
        
        vbf_l1_jeta_1_=-9999;
        vbf_l1_jeta_2_=-9999;
        vbf_l1_jeta_3_=-9999;
        vbf_l1_jeta_4_=-9999;
        
        vbf_l1_mjj_=-9999;
        vbf_l1_mjj30_=-9999;
        
        vbf_l1_etajj_=-9999;
        vbf_l1_phijj_=-9999;
        
        vbf_pf_jpt_2d_plot_ = -9999;
        
        vbf_pf_jpt_1_=-9999;
        vbf_pf_jpt_2_=-9999;
        vbf_pf_jpt_3_=-9999;
        vbf_pf_jpt_4_=-9999;
        
        vbf_pf_jeta_1_=-9999;
        vbf_pf_jeta_2_=-9999;
        vbf_pf_jeta_3_=-9999;
        vbf_pf_jeta_4_=-9999;
        
        vbf_pf_mjj_=-9999;
        vbf_pf_mjj30_=-9999;
        
        vbf_calo_met_ = -9999;
        vbf_noise_cleaned_calo_met = -9999;
        vbf_pf_met_ = -9999;
        
        leading_jpt_1_=-9999;
        leading_jpt_2_=-9999;
        leading_jeta_1_=-9999;
        leading_jeta_2_=-9999;
        leading_mjj_ = -9999;
        
        offline_jpt_1_=-9999;
        offline_jpt_2_=-9999;
        offline_jpt_3_=-9999;
        
        offline_jeta_1_=-9999;
        offline_jeta_2_=-9999;
        offline_jeta_3_=-9999;
        
        offline_mjj30_ = -9999;
        offline_mjj_ = -9999;
        offline_detajj_= -9999;
        offline_dphijj_= -9999;
        
        offline_matched_jpt_1_=-9999;
        offline_matched_jpt_2_=-9999;
        offline_matched_jpt_3_=-9999;
        
        offline_matched_jeta_1_=-9999;
        offline_matched_jeta_2_=-9999;
        offline_matched_jeta_3_=-9999;
        
        offline_matched_mjj30_ = -9999;
        offline_matched_mjj_ = -9999;
        offline_matched_detajj_= -9999;
        offline_matched_dphijj_= -9999;
        
        offline_met_ = -9999;
        
        vbfpass = false;
        vbfl1pass = false;
        metmhtpass = false;
        isomupass = false;
     
        int VBFcount = 0;
        int VBFL1count = 0;
        
        for (unsigned i = 0; i < VBFobjs.size(); ++i)
        {
            bool L1 = IsFilterMatchedWithName(VBFobjs[i], "hltL1DiJetVBF");
            bool PF = IsFilterMatchedWithName(VBFobjs[i], "hltL1PFJetCategories");
            bool AllPF = IsFilterMatchedWithName(VBFobjs[i],"hltParticleFlowNoMu");
            
            if (L1) {
                        VBFL1count++;
                        L1jets.push_back(VBFobjs[i]);
                    }
            if (PF) {
                        VBFcount++;
                        HLT_PF_jets.push_back(VBFobjs[i]);
                    }
            
            if (AllPF) {
                        HLT_All_PF_jets.push_back(VBFobjs[i]);
                       }
            
            if (IsFilterMatchedWithName(VBFobjs[i], "hltCaloMETOpen")) vbf_calo_met_  = VBFobjs[i]->vector().Pt();
            if (IsFilterMatchedWithName(VBFobjs[i], "hltCaloNoiseCleanedMET66")) vbf_noise_cleaned_calo_met  = VBFobjs[i]->vector().Pt();
            if (IsFilterMatchedWithName(VBFobjs[i], "hltPFMETVBF110")) vbf_pf_met_  = VBFobjs[i]->vector().Pt();
            
            
            
        }
        
        for (unsigned i = 0; i < VBFobjs3.size(); ++i)
        {
            bool PF = IsFilterMatchedWithName(VBFobjs3[i], "hlt1PFJetCategories3");
           
            if (PF){
                VBFcount++;
                HLT_PF_jets.push_back(VBFobjs3[i]);
             
            }
        }
     
        VBFPassed_ = (VBFcount>0);
        VBFL1Passed_ = (VBFL1count>0);
        
        if (VBFPassed_) std::cout<<VBFcount<<std::endl;
        
        int METMHTcount = 0;
        
        for (unsigned i = 0; i < METMHTobjs.size(); ++i)
        {
            if (IsFilterMatchedWithName(METMHTobjs[i], "hltPFMETNoMu120")){
            
               // if (METMHTobjs[i]->vector().Pt()<200) std::cout<<"METMHT problem: " <<METMHTobjs[i]->vector().Pt()<<std::endl;
            METMHTcount++;
            }
        }
        
        METMHTPassed_ = (METMHTcount>0);
        
        int IsoMuobjscount = 0;
        
        for (unsigned i = 0; i < IsoMuobjs.size(); ++i)
        {
            if (IsFilterMatchedWithName(IsoMuobjs[i],"hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07")) IsoMuobjscount++;
        }
        
        IsoMuPassed_ = (IsoMuobjscount>0);
        
       // std::cout<<"VBFpass: "<<VBFPassed_<<std::endl;
        //std::cout<<"VBFpassL1: "<<VBFL1Passed_<<std::endl;
        //std::cout<<"METMHTpass: "<<METMHTPassed_<<std::endl;
        //std::cout<<"IsoMupass: "<<IsoMuPassed_<<std::endl;
        
        /*if (VBFobjs.size()>0){
                std::cout<<"VBFobj size: "<<VBFobjs.size()<<std::endl;
                std::cout<<"L1obj size: "<<L1jets.size()<<std::endl;
                std::cout<<"jets_obj size: "<<HLT_PF_jets.size()<<std::endl;
        }*/
        
        
         //Adding offline met
         std::vector<Met *> pfMet_vec = event->GetPtrVec<Met>("pfMetFromSlimmed");
         offline_met_ = pfMet_vec.at(0)->vector().Pt();
         
         //Sorting PF (HLT) and L1 jets by Pt
         std::sort(HLT_PF_jets.begin(), HLT_PF_jets.end(), PtComparatorTriggerObj());
         std::sort(HLT_All_PF_jets.begin(), HLT_All_PF_jets.end(), PtComparatorTriggerObj());
         std::sort(L1jets.begin(), L1jets.end(), PtComparatorTriggerObj());
        
        
        
        //Add all L1 jets
        std::vector<L1TObject *> AllL1_jet_objs;
        
       if (!data) AllL1_jet_objs = event->GetPtrVec<L1TObject>("L1Jets");
        
        
         // Add offline jet variables
         std::vector<PFJet *> const& jet_objs = event->GetPtrVec<PFJet>("ak4PFJetsCHS");
         std::vector<PFJet *>  Offline_jet_objs;
         std::vector<PFJet *>  Offline_jet_objs_no_matching;
        
        
        for (unsigned i = 0; i < jet_objs.size(); ++i) {
         
         //if (IsFilterMatchedWithIndex(jet_objs[i], VBFobjs, "hltL1PFJetCategories", 0.5).first == true)
           //          {
         
                         Offline_jet_objs.push_back(jet_objs[i]);
         
         
             //        }
         //else break;
         
         }
        
        
        
        for (unsigned i = 0; i < jet_objs.size(); ++i) {
            Offline_jet_objs_no_matching.push_back(jet_objs[i]);
        }
        
        
         std::sort(Offline_jet_objs.begin(), Offline_jet_objs.end(), PtComparatorOfflineObj());
         std::sort(Offline_jet_objs_no_matching.begin(), Offline_jet_objs_no_matching.end(), PtComparatorOfflineObj());
         std::sort(AllL1_jet_objs.begin(), AllL1_jet_objs.end(), PtComparatorL1Obj());
        
        
    
        
        
        //Insert mjj for HLT and L1 jets
        
        double hlt_mjj = -9999;
        double hlt_mjj30 = -9999;
        double mindR2 = 1000000;
       
        for (unsigned i = 0; i < HLT_PF_jets.size(); ++i)
        { if (jet_objs.size()>0)
            if (reco::deltaR2(Offline_jet_objs[0]->vector(), HLT_PF_jets[i]->vector()) < mindR2 )
            {
                mindR2 =reco::deltaR2(Offline_jet_objs[0]->vector(), HLT_PF_jets[i]->vector());
                vbf_pf_jpt_2d_plot_ = HLT_PF_jets[i]->vector().Pt();
            }
            for (unsigned j = 0; j < HLT_PF_jets.size(); ++j)
            {
                
                if ((HLT_PF_jets.size()>1)&&(i!=j)){
                    
                    if ((HLT_PF_jets[i]->vector()+HLT_PF_jets[j]->vector()).M()>hlt_mjj)
                        hlt_mjj = (HLT_PF_jets[i]->vector()+HLT_PF_jets[j]->vector()).M();
                    
                    
                    if ((HLT_PF_jets[i]->vector().Pt()>30)&&(HLT_PF_jets[j]->vector().Pt()>30)&&((HLT_PF_jets[i]->vector()+HLT_PF_jets[j]->vector()).M()>hlt_mjj30))
                        hlt_mjj30 = (HLT_PF_jets[i]->vector()+HLT_PF_jets[j]->vector()).M();
                    
                    
                }
            }
        }
        
        //mjj for all L1 jets
        
        
        
        double allL1_mjj = -9999;
        double allL1_mjj30 = -9999;
        double mindR = 1000000;
        
        for (unsigned i = 0; i < AllL1_jet_objs.size(); ++i)
        { if (jet_objs.size()>0)
            if (reco::deltaR2(Offline_jet_objs[0]->vector(), AllL1_jet_objs[i]->vector()) < mindR )
                {
                    mindR =reco::deltaR2(Offline_jet_objs[0]->vector(), AllL1_jet_objs[i]->vector());
                    l1_jpt_2d_plot_ = AllL1_jet_objs[i]->vector().Pt();
                }
        for (unsigned j = 0; j < AllL1_jet_objs.size(); ++j)
        {
            
            if ((AllL1_jet_objs.size()>1)&&(i!=j)){
                
                if ((AllL1_jet_objs[i]->vector()+AllL1_jet_objs[j]->vector()).M()>allL1_mjj)
                allL1_mjj = (AllL1_jet_objs[i]->vector()+AllL1_jet_objs[j]->vector()).M();
                
                
                if ((AllL1_jet_objs[i]->vector().Pt()>30)&&(AllL1_jet_objs[j]->vector().Pt()>30)&&((AllL1_jet_objs[i]->vector()+AllL1_jet_objs[j]->vector()).M()>allL1_mjj30))
                allL1_mjj30 = (AllL1_jet_objs[i]->vector()+AllL1_jet_objs[j]->vector()).M();
                
                
            }
        }
        }
        
        //l1 seed mjj
        
        double l1_mjj = -9999;
        double l1_mjj30 = -9999;
        double mindR1 = 100000;
        
        for (unsigned i = 0; i < L1jets.size(); ++i)
        {if (jet_objs.size()>0)
            if (reco::deltaR2(Offline_jet_objs[0]->vector(), L1jets[i]->vector()) < mindR1 )
            {
                mindR1 =reco::deltaR2(Offline_jet_objs[0]->vector(), L1jets[i]->vector());
                vbf_l1_jpt_2d_plot_ = L1jets[i]->vector().Pt();
            }
            for (unsigned j = 0; j < L1jets.size(); ++j)
            {
                
                if ((L1jets.size()>1)&&(i!=j)){
                    
                    if ((L1jets[i]->vector()+L1jets[j]->vector()).M()>l1_mjj)
                    {
                        l1_mjj = (L1jets[i]->vector()+L1jets[j]->vector()).M();
                        vbf_l1_etajj_ = (L1jets[i]->vector()+L1jets[j]->vector()).eta();
                        vbf_l1_phijj_ = (L1jets[i]->vector()+L1jets[j]->vector()).phi();
                    
                    }
                    if ((L1jets[i]->vector().Pt()>30)&&(L1jets[j]->vector().Pt()>30)&&((L1jets[i]->vector()+L1jets[j]->vector()).M()>l1_mjj30))
                        l1_mjj30 = (L1jets[i]->vector()+L1jets[j]->vector()).M();
                    
                    
                }
            }
        }
     
        //      FOR RATES COMMENT OFFLINE OUT
         double offline_mjj = -9999;
         double offline_mjj30 = -9999;
         double minPhi = 1000000000000;
        double detajj_ = 9999;
        double dphijj_ = 9999;
         for (unsigned i = 0; i < Offline_jet_objs.size(); ++i)
         {
         if (Offline_jet_objs[i]->vector().Pt()>30)
         if (minPhi>TMath::Abs(Offline_jet_objs[i]->vector().phi()-pfMet_vec.at(0)->vector().phi()))
         minPhi =TMath::Abs(Offline_jet_objs[i]->vector().phi()-pfMet_vec.at(0)->vector().phi());
         
         
         for (unsigned j = 0; j < Offline_jet_objs.size(); ++j)
         {
         
         if ((Offline_jet_objs.size()>1)&&(i!=j)){
             
       /*  if (dphijj_>TMath::Abs(Offline_jet_objs[i]->vector().phi()-Offline_jet_objs[j]->vector().phi()))
            dphijj_ =TMath::Abs(Offline_jet_objs[i]->vector().phi()-Offline_jet_objs[j]->vector().phi());
             
         if (detajj_>TMath::Abs(Offline_jet_objs[i]->vector().eta()-Offline_jet_objs[j]->vector().eta()))
             detajj_ =TMath::Abs(Offline_jet_objs[i]->vector().eta()-Offline_jet_objs[j]->vector().eta());
    */
         
             if ((Offline_jet_objs[i]->vector()+Offline_jet_objs[j]->vector()).M()>offline_mjj){
         offline_mjj = (Offline_jet_objs[i]->vector()+Offline_jet_objs[j]->vector()).M();
                 dphijj_ =TMath::Abs(Offline_jet_objs[i]->vector().phi()-Offline_jet_objs[j]->vector().phi());
                 detajj_ =TMath::Abs(Offline_jet_objs[i]->vector().eta()-Offline_jet_objs[j]->vector().eta());
                 if(dphijj_>=2*ROOT::Math::Pi()) dphijj_ = dphijj_ - 2*ROOT::Math::Pi();
                 
             }
         
         if ((Offline_jet_objs[i]->vector().Pt()>30)&&(Offline_jet_objs[j]->vector().Pt()>30)&&((Offline_jet_objs[i]->vector()+Offline_jet_objs[j]->vector()).M()>offline_mjj30))
         {
             
         offline_mjj30 = (Offline_jet_objs[i]->vector()+Offline_jet_objs[j]->vector()).M();
             if (Offline_jet_objs[i]->vector().Pt()>Offline_jet_objs[j]->vector().Pt()){ offline_jpt_1_ = Offline_jet_objs[i]->vector().Pt();
                                                                                         offline_jeta_1_ = Offline_jet_objs[i]->vector().Eta();
                                                                                         offline_jpt_2_ = Offline_jet_objs[j]->vector().Pt();
                                                                                         offline_jeta_2_ = Offline_jet_objs[j]->vector().Eta();
             
                                                                                        }
             else {
                 offline_jpt_2_ = Offline_jet_objs[i]->vector().Pt();
                 offline_jeta_2_ = Offline_jet_objs[i]->vector().Eta();
                 offline_jpt_1_ = Offline_jet_objs[j]->vector().Pt();
                 offline_jeta_1_ = Offline_jet_objs[j]->vector().Eta();
             }
         
         }
         }
         
         }
         
         }

        offline_detajj_ =detajj_;
        offline_dphijj_ =dphijj_;
        
         
      
        if (HLT_PF_jets.size()>0)
        {
            vbf_pf_jpt_1_ = HLT_PF_jets[0]->vector().Pt();
            vbf_pf_jeta_1_ = HLT_PF_jets[0]->vector().Eta();
            
        }
        if (HLT_PF_jets.size()>1)
        {
            vbf_pf_jpt_2_ = HLT_PF_jets[1]->vector().Pt();
            vbf_pf_jeta_2_ = HLT_PF_jets[1]->vector().Eta();
        }
        if (HLT_PF_jets.size()>2)
        {
            vbf_pf_jpt_3_ = HLT_PF_jets[2]->vector().Pt();
            vbf_pf_jeta_3_ = HLT_PF_jets[2]->vector().Eta();	
        }
        if (HLT_PF_jets.size()>3)
        {
            vbf_pf_jpt_4_ = HLT_PF_jets[3]->vector().Pt(); 
            vbf_pf_jeta_4_ = HLT_PF_jets[3]->vector().Eta();	
        }
        
        
        if (L1jets.size()>0)
        {
            vbf_l1_jpt_1_ = L1jets[0]->vector().Pt();
            vbf_l1_jeta_1_ = L1jets[0]->vector().Eta();
            
        }
        if (L1jets.size()>1)
        {
            vbf_l1_jpt_2_ = L1jets[1]->vector().Pt();
            vbf_l1_jeta_2_ = L1jets[1]->vector().Eta();
        }
        if (L1jets.size()>2)
        {
            vbf_l1_jpt_3_ = L1jets[2]->vector().Pt();
            vbf_l1_jeta_3_ = L1jets[2]->vector().Eta();
        }
        if (L1jets.size()>3)
        {
            vbf_l1_jpt_4_ = L1jets[3]->vector().Pt();
            vbf_l1_jeta_4_ = L1jets[3]->vector().Eta();
        }
      
        
        /* if (Offline_jet_objs.size()>0)
            {
                offline_jpt_1_ = AllL1_jet_objs[0]->vector().Pt();
                offline_jeta_1_ = AllL1_jet_objs[0]->vector().Eta();
            }
         if (Offline_jet_objs.size()>1)
            {
                offline_jpt_2_ = AllL1_jet_objs[1]->vector().Pt();
                offline_jeta_2_ = AllL1_jet_objs[1]->vector().Eta();
            
            }*/
        
       if (Offline_jet_objs.size()>1)
        {
            leading_jpt_1_ = Offline_jet_objs[0]->vector().Pt();
            leading_jeta_1_ = Offline_jet_objs[0]->vector().Eta();
            leading_jpt_2_ = Offline_jet_objs[1]->vector().Pt();
            leading_jeta_2_ = Offline_jet_objs[1]->vector().Eta();
            if ((leading_jpt_2_>30)&&(leading_jpt_1_>30))
            leading_mjj_ =(Offline_jet_objs[0]->vector()+Offline_jet_objs[1]->vector()).M();
        }
        
         if (Offline_jet_objs.size()>2)
            {
                offline_jpt_3_ = Offline_jet_objs[2]->vector().Pt();
                offline_jeta_3_ = Offline_jet_objs[2]->vector().Eta();
            }
        
        
        if (AllL1_jet_objs.size()>0)
        {
            l1_jpt_1_ = AllL1_jet_objs[0]->vector().Pt();
            l1_jeta_1_ = AllL1_jet_objs[0]->vector().Eta();
        }
        if (AllL1_jet_objs.size()>1)
        {
            l1_jpt_2_ = AllL1_jet_objs[1]->vector().Pt();
            l1_jeta_2_ = AllL1_jet_objs[1]->vector().Eta();
            
        }
        if (AllL1_jet_objs.size()>2)
        {
            l1_jpt_3_ = AllL1_jet_objs[2]->vector().Pt();
            l1_jeta_3_ = AllL1_jet_objs[2]->vector().Eta();
        }
        
        
        
        
        
        
        vbf_pf_mjj_ = hlt_mjj;
        vbf_pf_mjj30_ = hlt_mjj30;
        
        vbf_l1_mjj_ = l1_mjj;
        vbf_l1_mjj30_ = l1_mjj30;
        
        l1_mjj_ = allL1_mjj;
        l1_mjj30_ = allL1_mjj30;
        
        offline_mjj_ = offline_mjj;
        offline_mjj30_ = offline_mjj30;
        
        
        if(fs_) outtree_->Fill();
        
        return 0;
        
        
        
    }
    
    int HTTVBFHLTAnalysis::PostAnalysis() {
        std::cout<<"Created flat tree for trigger studies "<<std::endl;
        return 0;
    }
    
    void HTTVBFHLTAnalysis::PrintInfo() {
        ;
    }
    
}
