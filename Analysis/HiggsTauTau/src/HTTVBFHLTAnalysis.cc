//MINIMAL CODE VER.1.0.
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTVBFHLTAnalysis.h"
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnRootTools.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "TLorentzVector.h"

double vbfpass;
double vbfl1pass;
double metmhtpass;
double isomupass;



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
            //Offline properties
            outtree_->Branch("leading_jpt_1"             , &leading_jpt_1_       );
            outtree_->Branch("leading_jpt_2"             , &leading_jpt_2_       );
            outtree_->Branch("leading_jeta_1"            , &leading_jeta_1_       );
            outtree_->Branch("leading_jeta_2"            , &leading_jeta_2_       );
            outtree_->Branch("leading_jdphi"            , &leading_jdphi_       );
            outtree_->Branch("leading_jdeta"            , &leading_jdeta_       );
            outtree_->Branch("leading_mjj"               , &leading_mjj_       );
            
            outtree_->Branch("offline_met"               , &offline_met_       );
            
            outtree_->Branch("VBFPassed"               , &VBFPassed_       );
            outtree_->Branch("VBFL1Passed"               , &VBFL1Passed_       );
            outtree_->Branch("VBFCaloMETPassed"               , &VBFCaloMETPassed_       );
            outtree_->Branch("VBFCaloMETNCPassed"               , &VBFCaloMETNCPassed_       );
            outtree_->Branch("VBFPFMETPassed"               , &VBFPFMETPassed_       );
            outtree_->Branch("METMHTPassed"               , &METMHTPassed_       );
            outtree_->Branch("IsoMuPassed"               , &IsoMuPassed_       );
            outtree_->Branch("OneTightMuon"               , &OneTightMuon_       );
            
            
        }
        
        return 0;
    }
    
    
    int HTTVBFHLTAnalysis::Execute(TreeEvent *event) {
        
      
        std::vector<TriggerObject *> const& VBFobjs =    event->GetPtrVec<TriggerObject>("DiJet110_35_Mjj650_PFMET110");
        std::vector<TriggerObject *> const& VBFobjs3 =   event->GetPtrVec<TriggerObject>("TripleJet110_35_35_Mjj650_PFMET110");
        std::vector<TriggerObject *> const& METMHTobjs = event->GetPtrVec<TriggerObject>("PFMETNoMu120_PFMHTNoMu120_IDTight");
        std::vector<TriggerObject *> const& IsoMuobjs =  event->GetPtrVec<TriggerObject>("triggerObjectsIsoMu27");
      
        
        leading_jpt_1_=-9999;
        leading_jpt_2_=-9999;
        leading_jeta_1_=-9999;
        leading_jeta_2_=-9999;
        leading_mjj_ = -9999;
        leading_jdeta_= -9999;
        leading_jdphi_=-9999;
        
        offline_met_ = -9999;
    
        vbfpass = false;
        metmhtpass = false;
        isomupass = false;
        
        
        int VBFcount = 0;
        int VBFL1count = 0;
        int VBFCaloMETcount = 0;
        int VBFCaloMETNCcount = 0;
        int VBFPFMETcount = 0;
        
        for (unsigned i = 0; i < VBFobjs.size(); ++i){
            if (IsFilterMatchedWithName(VBFobjs[i], "hltL1PFJetCategories")) VBFcount++;
            if (IsFilterMatchedWithName(VBFobjs[i], "hltL1DiJetVBF")) VBFL1count++;
            if (IsFilterMatchedWithName(VBFobjs[i], "hltCaloMETOpen")) VBFCaloMETcount++;
            if (IsFilterMatchedWithName(VBFobjs[i], "hltCaloNoiseCleanedMET66")) VBFCaloMETNCcount++;
            if (IsFilterMatchedWithName(VBFobjs[i], "hltPFMETVBF110")) VBFPFMETcount++;
        
        }
        for (unsigned i = 0; i < VBFobjs3.size(); ++i)
        {
            if (IsFilterMatchedWithName(VBFobjs3[i], "hlt1PFJetCategories3"))  VBFcount++;
            if (IsFilterMatchedWithName(VBFobjs3[i], "hltL1DiJetVBF"))  VBFL1count++;
            if (IsFilterMatchedWithName(VBFobjs3[i], "hltCaloMETOpen")) VBFCaloMETcount++;
            if (IsFilterMatchedWithName(VBFobjs3[i], "hltCaloNoiseCleanedMET66")) VBFCaloMETNCcount++;
            if (IsFilterMatchedWithName(VBFobjs3[i], "hltPFMETVBF110")) VBFPFMETcount++;
        }
        
        VBFPassed_ = (VBFcount>0);
        VBFL1Passed_ = (VBFL1count>0);
        VBFCaloMETPassed_ = (VBFCaloMETcount>0);
        VBFCaloMETNCPassed_ = (VBFCaloMETNCcount>0);
        VBFPFMETPassed_ = (VBFPFMETcount>0);
        
        
        int METMHTcount = 0;
        
        for (unsigned i = 0; i < METMHTobjs.size(); ++i)
            if (IsFilterMatchedWithName(METMHTobjs[i], "hltPFMETNoMu120")) METMHTcount++;
         
        
        METMHTPassed_ = (METMHTcount>0);
        
    int IsoMuobjscount = 0;
        
        for (unsigned i = 0; i < IsoMuobjs.size(); ++i)
           if (IsFilterMatchedWithName(IsoMuobjs[i],"hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07")) IsoMuobjscount++;
        
        IsoMuPassed_ = (IsoMuobjscount>0);
        
        
        
        //Adding offline met
        std::vector<Met *> pfMet_vec = event->GetPtrVec<Met>("pfMetFromSlimmed");
        offline_met_ = pfMet_vec.at(0)->vector().Pt();
        
        // Add offline jet variables
        std::vector<PFJet *> const& jet_objs = event->GetPtrVec<PFJet>("ak4PFJetsCHS");
        std::vector<PFJet *>  Offline_jet_objs;
        
         std::vector<Muon *> const& muons = event->GetPtrVec<Muon>("muons");
         double offline_met_x = pfMet_vec.at(0)->vector().Px();
         double offline_met_y = pfMet_vec.at(0)->vector().Py();
        double OneTightMuon = 0;
        for (unsigned i = 0; i < muons.size(); ++i)
         {
             if (OneTightMuon>1) break;
             if (muons[i]->vector().Pt()>=30) OneTightMuon++;
             offline_met_x +=muons[i]->vector().Px();
             offline_met_y +=muons[i]->vector().Py();
         }
        
        OneTightMuon_ = (OneTightMuon<=1);
        offline_met_ = TMath::Sqrt(offline_met_x*offline_met_x+offline_met_y*offline_met_y);
         
        
        for (unsigned i = 0; i < jet_objs.size(); ++i)
        {
           unsigned int c = 0;
            for (unsigned j = 0; j < muons.size(); ++j)
                if ((reco::deltaR2(muons[j]->vector(), jet_objs[i]->vector()) < 0.5)) break;
                else c++;
                  if (c == muons.size())  Offline_jet_objs.push_back(jet_objs[i]);
        }
        std::sort(Offline_jet_objs.begin(), Offline_jet_objs.end(), PtComparatorOfflineObj());
        
        
        if (Offline_jet_objs.size()>1)
        {
            leading_jpt_1_ = Offline_jet_objs[0]->vector().Pt();
            leading_jeta_1_ = Offline_jet_objs[0]->vector().Eta();
            leading_jpt_2_ = Offline_jet_objs[1]->vector().Pt();
            leading_jeta_2_ = Offline_jet_objs[1]->vector().Eta();
            if ((leading_jpt_2_>30)&&(leading_jpt_1_>30))
                leading_mjj_ =(Offline_jet_objs[0]->vector()+Offline_jet_objs[1]->vector()).M();
            
            TLorentzVector jet1, jet2;
            jet1.SetPtEtaPhiM(Offline_jet_objs[0]->vector().Pt(),Offline_jet_objs[0]->vector().Eta(),Offline_jet_objs[0]->vector().Phi(),Offline_jet_objs[0]->vector().M());
            jet2.SetPtEtaPhiM(Offline_jet_objs[1]->vector().Pt(),Offline_jet_objs[1]->vector().Eta(),Offline_jet_objs[1]->vector().Phi(),Offline_jet_objs[1]->vector().M());
            
            leading_jdeta_ =TMath::Abs(Offline_jet_objs[0]->vector().eta()-Offline_jet_objs[1]->vector().eta());
            leading_jdphi_ = fabs(jet1.DeltaPhi(jet2));
            if (fabs(leading_jdphi_)>=ROOT::Math::Pi()) leading_jdphi_ =  2*ROOT::Math::Pi()-fabs(leading_jdphi_);
            
        }
        
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
