#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/L1TFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "TVector3.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisEventDataFormat.h"
#include "UserCode/ICHiggsTauTau/interface/VBFHiggsGenAnalysisDataFormat.h"


struct greater_Candidate{
  bool operator() (const ic::Candidate *a,const ic::Candidate *b) {
    return (a->pt() > b->pt());
  }
};

namespace ic {

  L1TFilter::L1TFilter(std::string const& name, std::string channel, fwlite::TFileService *fs, struct L1Cuts l1Cuts, std::string L1MuTag, std::string subDirName, unsigned isZeroBias) : ModuleBase(name) {
    
    channel_ = channel;
    isZB = isZeroBias;
    
    Jet1PtCut = l1Cuts.Jet1Pt;
    Jet2PtCut = l1Cuts.Jet2Pt;
    DeltaEtaCut = l1Cuts.DeltaEta;
    MjjCut = l1Cuts.Mjj;
    EGPtCut = l1Cuts.EGPt;
    Tau1PtCut = l1Cuts.Tau1Pt;
    Tau2PtCut = l1Cuts.Tau2Pt;
    MuPtCut = l1Cuts.MuPt;
    IsoTauFilter = l1Cuts.IsoTau;
    IsoEGFilter = l1Cuts.IsoEG;
    IsoMuFilter = l1Cuts.IsoMu;
    JetFilter = l1Cuts.JetFilter;
    AvePtCut = l1Cuts.AvePt;
    VecPtCut = l1Cuts.VecPt;
    DeltaRJetFilter = l1Cuts.DeltaRFilter;
    HtCut = l1Cuts.Ht;
    
    l1jets_label_ = "L1Jets"; 
    l1electrons_label_ = "L1EGammas";
    l1muons_label_ = L1MuTag;
    l1taus_label_ = "L1Taus";  
    
    TFileDirectory subDir = fs->mkdir(subDirName.c_str());

    h_TotalEvents = subDir.make<TH1D>("h_TotalEvents","h_TotalEvents",1,0,1); 
    h_EventsPassedTrigger = subDir.make<TH1D>("h_EventsPassedTrigger","h_EventsPassedTrigger",1, 0,1); 
    h_EventsPassedTriggerPure = subDir.make<TH1D>("h_EventsPassedTriggerPure","h_EventsPassedTriggerPure",2, 0,2);
    
    h_TriggersPassed = subDir.make<TH1D>("h_TriggersPassed", "h_TriggersPassed", 15,0,15);
    
    h_SingleTauPt = subDir.make<TH1D>("h_SingleTauPt","h_SingleTauPt",200,0,200);
    h_SingleIsoTauPt = subDir.make<TH1D>("h_SingleIsoTauPt","h_SingleIsoTauPt",200,0,200);
    h_DoubleTauPt = subDir.make<TH1D>("h_DoubleTauPt","h_DoubleTauPt",200,0,200);
    h_DoubleIsoTauPt = subDir.make<TH1D>("h_DoubleIsoTauPt","h_DoubleIsoTauPt",200,0,200);
    h_SingleEGPt = subDir.make<TH1D>("h_SingleEGPt","h_SingleEGPt",200,0,200);
    h_SingleIsoEGPt = subDir.make<TH1D>("h_SingleIsoEGPt","h_SingleIsoEGPt",200,0,200);
    h_SingleMuPt = subDir.make<TH1D>("h_SingleMuPt","h_SingleMuPt",200,0,200);
    h_SingleIsoMuPt = subDir.make<TH1D>("h_SingleIsoMuPt","h_SingleIsoMuPt",200,0,200);
    h_SingleJetPt = subDir.make<TH1D>("h_SingleJetPt","h_SingleJetPt",200,0,200);
    h_DoubleJetPt = subDir.make<TH1D>("h_DoubleJetPt","h_DoubleJetPt",200,0,200);
    h_DoubleJetMjj = subDir.make<TH1D>("h_DoubleJetMjj","h_DoubleJetMjj",200,0,1000);
    h_DoubleJetDeltaEta = subDir.make<TH1D>("h_DoubleJetDeltaEta","h_DoubleJetDeltaEta",200,0,10);
    h_DoubleJetAvePt = subDir.make<TH1D>("h_DoubleJetAvePt","h_DoubleJetAvePt",200,0,200);
    h_DoubleJetVecPt = subDir.make<TH1D>("h_DoubleJetVecPt","h_DoubleJetVecPt",200,0,200);
    
    h_DoubleJet30DeltaEta3pt25_Mu5_Mjj = subDir.make<TH1D>("h_DoubleJet30DeltaEta3pt25_Mu5_Mjj","h_DoubleJet30DeltaEta3pt25_Mu5_Mjj",200,0,1000);
    h_DoubleJet30DeltaEta3pt25_Mjj = subDir.make<TH1D>("h_DoubleJet30DeltaEta3pt25_Mjj","h_DoubleJet30DeltaEta3pt25_Mjj",200,0,1000);
    
    h_DoubleTauPt2D = subDir.make<TH2D>("h_DoubleTauPt2D","h_DoubleTauPt2D",200,0,200, 200, 0, 200);
    h_DoubleIsoTauPt2D = subDir.make<TH2D>("h_DoubleIsoTauPt2D","h_DoubleIsoTauPt2D",200,0,200, 200, 0, 200);
    
    h_l1jj_VecPt = subDir.make<TH1D>("h_l1jj_VecPt","h_l1jj_VecPt",150, 0,300); 
    h_l1jj_VecPt->GetXaxis()->SetTitle("Di-jet p_{T}_{jj} [GeV]");
    h_l1jj_VecPt->GetYaxis()->SetTitle("Entries");
    
    h_l1jj_DiffOverSum = subDir.make<TH1D>("h_l1jj_DiffOverSum","h_l1jj_DiffOverSum",100, 0,1); 
    h_l1jj_DiffOverSum->GetXaxis()->SetTitle("Di-jet #Delta p_{T}/#Sigma p_{T} [GeV]");
    h_l1jj_DiffOverSum->GetYaxis()->SetTitle("Entries");
    
    h_l1jj_Diff = subDir.make<TH1D>("h_l1jj_Diff","h_l1jj_Diff",100, 0,150); 
    h_l1jj_Diff->GetXaxis()->SetTitle("Di-jet #Delta p_{T} [GeV]");
    h_l1jj_Diff->GetYaxis()->SetTitle("Entries");
    
    h_tauEta_Before = subDir.make<TH1D>("h_tauEta_Before","h_tauEta_Before",100, -5,5); 
    h_tauEta_Before->GetXaxis()->SetTitle("L1 tau #eta");
    h_tauEta_Before->GetYaxis()->SetTitle("Entries");
    
    h_tauEta_After = subDir.make<TH1D>("h_tauEta_After","h_tauEta_After",100, -5,5); 
    h_tauEta_After->GetXaxis()->SetTitle("L1 tau #eta");
    h_tauEta_After->GetYaxis()->SetTitle("Entries");
    
    h_tau1Eta = subDir.make<TH1D>("h_tau1Eta","h_tau1Eta",100, -5,5);
    h_tau2Eta = subDir.make<TH1D>("h_tau2Eta","h_tau2Eta",100, -5,5); 
    h_electron1Eta = subDir.make<TH1D>("h_electron1Eta","h_electron1Eta",100, -5,5);
    h_electron2Eta = subDir.make<TH1D>("h_electron2Eta","h_electron2Eta",100, -5,5);
    h_muon1Eta = subDir.make<TH1D>("h_muon1Eta","h_muon1Eta",100, -5,5);
    h_muon2Eta = subDir.make<TH1D>("h_muon2Eta","h_muon2Eta",100, -5,5);
    h_jet1Eta = subDir.make<TH1D>("h_jet1Eta","h_jet1Eta",100, 0,5); 
    h_jet2Eta = subDir.make<TH1D>("h_jet2Eta","h_jet2Eta",100, 0,5); 
    h_tau1Pt = subDir.make<TH1D>("h_tau1Pt","h_tau1Pt",100, 0,200); 
    h_tau2Pt = subDir.make<TH1D>("h_tau2Pt","h_tau2Pt",100, 0,200); 
    h_electron1Pt = subDir.make<TH1D>("h_electron1Pt","h_electron1Pt",100, 0,200); 
    h_electron2Pt = subDir.make<TH1D>("h_electron2Pt","h_electron2Pt",100, 0,200);
    h_muon1Pt = subDir.make<TH1D>("h_muon1Pt","h_muon1Pt",100, 0,200); 
    h_muon2Pt = subDir.make<TH1D>("h_muon2Pt","h_muon2Pt",100, 0,200);
    h_jet1Pt = subDir.make<TH1D>("h_jet1Pt","h_jet1Pt",100, 0,200); 
    h_jet2Pt = subDir.make<TH1D>("h_jet2Pt","h_jet2Pt",100, 0,200);
    h_jetsMjj = subDir.make<TH1D>("h_jetsMjj","h_jetsMjj",100, 0,1000); 
    h_jetsDeltaEta = subDir.make<TH1D>("h_jetsDeltaEta","h_jetsDeltaEta",100, 0,10); 
    h_jetsAvePt = subDir.make<TH1D>("h_jetsAvePt","h_jetsAvePt",100, 0,200); 
    h_jetsVecPt = subDir.make<TH1D>("h_jetsVecPt","h_jetsVecPt",100, 0,200);
    h_JetMultiplicity = subDir.make<TH1D>("h_JetMultiplicity","h_JetMultiplicity",10, 0,10);
    h_Mjj500Multiplicity = subDir.make<TH1D>("h_Mjj500Multiplicity","h_Mjj500Multiplicity",10, 0,10);
    h_Tau1Multiplicity = subDir.make<TH1D>("h_Tau1Multiplicity","h_Tau1Multiplicity",10, 0,10);
    h_Tau2Multiplicity = subDir.make<TH1D>("h_Tau2Multiplicity","h_Tau2Multiplicity",10, 0,10);
    h_MuonMultiplicity = subDir.make<TH1D>("h_MuonMultiplicity","h_MuonMultiplicity",10, 0,10);
    h_EGMultiplicity = subDir.make<TH1D>("h_EGMultiplicity","h_EGMultiplicity",10, 0,10);
    
    h_LargestMjj = subDir.make<TH1D>("h_LargestMjj","h_LargestMjj",500, 0,5000);
    h_LargestMjj_LeadPt = subDir.make<TH1D>("h_LargestMjj_LeadPt","h_LargestMjj_LeadPt",300, 0,300);
    h_LargestMjj_SubLeadPt = subDir.make<TH1D>("h_LargestMjj_SubLeadPt","h_LargestMjj_SubLeadPt",200, 0,200);
    h_LargestMjj_AvePt = subDir.make<TH1D>("h_LargestMjj_AvePt","h_LargestMjj_AvePt",200, 0,300);
    h_LargestMjj_VecPt = subDir.make<TH1D>("h_LargestMjj_VecPt","h_LargestMjj_VecPt",500, 0,200);
    h_LargestMjj_DeltaEta = subDir.make<TH1D>("h_LargestMjj_DeltaEta","h_LargestMjj_DeltaEta",200, 0,10);
    
    h_Ht = subDir.make<TH1D>("h_Ht","h_Ht",200, 0,1000);
    h_HtEta3 = subDir.make<TH1D>("h_HtEta3","h_HtEta3",200, 0,1000);
    h_Et = subDir.make<TH1D>("h_Et","h_Et",200, 0,1000);    
    h_EtEta3 = subDir.make<TH1D>("h_EtEta3","h_EtEta3",200, 0,1000); 
    h_HtMinusJets = subDir.make<TH1D>("h_HtMinusJets","h_HtMinusJets",400, -1000,1000);
    h_EtMinusJets = subDir.make<TH1D>("h_EtMinusJets","h_EtMinusJets",400, -1000,1000);
    h_JetsOverHt = subDir.make<TH1D>("h_JetsOverHt","h_JetsOverHt",200, 0,1);
    h_JetsOverEt = subDir.make<TH1D>("h_JetsOverEt","h_JetsOverEt",200, 0,1);
    h_JetsOverHtMin = subDir.make<TH1D>("h_JetsOverHtMin","h_JetsOverHtMin",200, 0,1);
    h_HtMinusJetsMin = subDir.make<TH1D>("h_HtMinusJetsMin","h_HtMinusJetsMin",400, -1000,1000);
    h_JetsOverHtMax = subDir.make<TH1D>("h_JetsOverHtMax","h_JetsOverHtMax",200, 0,1);
    h_HtMinusJetsMax = subDir.make<TH1D>("h_HtMinusJetsMax","h_HtMinusJetsMax",400, -1000,1000);
    
    h_l1jetDeltaPhi_Before = subDir.make<TH1D>("h_l1jetDeltaPhi_Before","h_l1jetDeltaPhi_Before",200, 0,3.15);
    h_l1jetDeltaPhi_Before->GetXaxis()->SetTitle("L1 jets #Delta#phi");
    h_l1jetDeltaPhi_Before->GetYaxis()->SetTitle("Entries");
    
    h_l1jetDeltaPhi_BeforeBefore = subDir.make<TH1D>("h_l1jetDeltaPhi_BeforeBefore","h_l1jetDeltaPhi_BeforeBefore",200, 0,3.15);
    h_l1jetDeltaPhi_BeforeBefore->GetXaxis()->SetTitle("L1 jets #Delta#phi");
    h_l1jetDeltaPhi_BeforeBefore->GetYaxis()->SetTitle("Entries");
    
    h_l1jetDeltaPhi_After = subDir.make<TH1D>("h_l1jetDeltaPhi_After","h_l1jetDeltaPhi_After",200, 0,3.15);
    h_l1jetDeltaPhi_After->GetXaxis()->SetTitle("L1 jets #Delta#phi");
    h_l1jetDeltaPhi_After->GetYaxis()->SetTitle("Entries");
    
    h_MET_NOHF = subDir.make<TH1D>("h_MET_NOHF","h_MET_NOHF",200, 0,200);
    h_MET_HF = subDir.make<TH1D>("h_MET_HF","h_MET_HF",200, 0,200);
    h_MHT_HF = subDir.make<TH1D>("h_MHT_HF","h_MHT_HF",200, 0,200);
    
    h_EtaProduct = subDir.make<TH1D>("h_EtaProduct","h_EtaProduct",200, -25,25);
    
    h_mvis = subDir.make<TH1D>("h_mvis","h_mvis",200,0,1000);
    h_mvis_ISO = subDir.make<TH1D>("h_mvis_ISO","h_mvis_ISO",200,0,1000);
    
    h_Mjj_AOCA = subDir.make<TH1D>("h_Mjj_AOCA","h_Mjj_AOCA",200,0,1000);
    h_DeltaEta_AOCA = subDir.make<TH1D>("h_DeltaEta_AOCA","h_DeltaEta_AOCA",200,0,10);
    h_AvgPt_AOCA = subDir.make<TH1D>("h_AvgPt_AOCA","h_AvgPt_AOCA",200,0,200);
    h_VecPt_AOCA = subDir.make<TH1D>("h_VecPt_AOCA","h_VecPt_AOCA",200,0,200);
    h_jPt1_AOCA = subDir.make<TH1D>("h_jPt1_AOCA","h_jPt1_AOCA",200,0,200);
    h_jPt2_AOCA = subDir.make<TH1D>("h_jPt2_AOCA","h_jPt2_AOCA",200,0,200);
    
    h_MHTHFVecPtRes = subDir.make<TH1D>("h_MHTHFVecPtRes","h_MHTHFVecPtRes",100,-2,2);
    
    h_VecPtVsETM = subDir.make<TH2D>("h_VecPtVsETM","h_VecPtVsETM",200,0,200,200,0,200);
    
    h_partonsmatched = subDir.make<TH1D>("h_partonsmatched","h_partonsmatched",2,0,2);
    
    h_tau_AvgPt = subDir.make<TH1D>("h_tau_AvgPt","h_tau_AvgPt",100,0,100);
    h_isotau_AvgPt = subDir.make<TH1D>("h_isotau_AvgPt","h_isotau_AvgPt",100,0,100);
    
  }

  L1TFilter::~L1TFilter() {
      ;
  }

  int L1TFilter::PreAnalysis() {

    return 0;
  }

  int L1TFilter::Execute(TreeEvent *event) {
      
      std::vector<ic::L1TJet*>  l1jets = event->GetPtrVec<ic::L1TJet>(l1jets_label_);
      std::vector<ic::L1TEGamma*> l1electrons = event->GetPtrVec<ic::L1TEGamma>(l1electrons_label_);
      std::vector<ic::L1TMuon*> l1muons = event->GetPtrVec<ic::L1TMuon>(l1muons_label_);
      std::vector<ic::L1TTau*> l1taus = event->GetPtrVec<ic::L1TTau>(l1taus_label_);
      std::vector<ic::L1TSum*> l1sums = event->GetPtrVec<ic::L1TSum>("L1SumsEtaRange5pt0");
      std::vector<ic::L1TSum*> l1sumsnohf = event->GetPtrVec<ic::L1TSum>("L1Sums");
      
      if(true){
        l1jets = event->GetPtrVec<ic::L1TJet>(l1jets_label_);
        l1electrons = event->GetPtrVec<ic::L1TEGamma>(l1electrons_label_);
        l1muons = event->GetPtrVec<ic::L1TMuon>(l1muons_label_);
        l1taus = event->GetPtrVec<ic::L1TTau>(l1taus_label_);
        l1sums = event->GetPtrVec<ic::L1TSum>("L1SumsEtaRange5pt0");
        l1sumsnohf = event->GetPtrVec<ic::L1TSum>("L1Sums");
      } 
      
      std::sort(l1electrons.begin(),l1electrons.end(),greater_Candidate());
      std::sort(l1muons.begin(),l1muons.end(),greater_Candidate());
      std::sort(l1taus.begin(),l1taus.end(),greater_Candidate());
      std::sort(l1jets.begin(),l1jets.end(),greater_Candidate());
      
      n_l1jets_ = l1jets.size();
      n_l1electrons_ = l1electrons.size();
      n_l1muons_ = l1muons.size();
      n_l1taus_ = l1taus.size();
      
      for(int i= n_l1muons_-1; i>=0; i--){
          if(l1muons[i]->quality < 12) l1muons.erase(l1muons.begin()+i);
      }
      n_l1muons_ = l1muons.size();
      
      if(IsoTauFilter){
          for(int i= n_l1taus_-1; i>=0; i--) if(l1taus[i]->isolation == 0) l1taus.erase(l1taus.begin()+i);
          n_l1taus_ = l1taus.size();
      }
      
      if(IsoEGFilter){
          for(int i= n_l1electrons_-1; i>=0; i--) if(l1electrons[i]->isolation == 0) l1electrons.erase(l1electrons.begin()+i);
          n_l1electrons_ = l1electrons.size();
      }
      if(IsoMuFilter){
          for(int i= n_l1muons_-1; i>=0; i--) if(l1muons[i]->isolation == 0) l1muons.erase(l1muons.begin()+i);
          n_l1muons_ = l1muons.size();
      }
      
      n_l1electrons_ = l1electrons.size();
      n_l1muons_ = l1muons.size();
      n_l1taus_ = l1taus.size();
      
      // Lepton selections
          
      bool Filter = false;
      
      if(channel_ == "em"){
          
          for(int i= n_l1electrons_-1; i>=0; i--) if(std::fabs(l1electrons[i]->vector().Pt()) < EGPtCut) l1electrons.erase(l1electrons.begin()+i);
          n_l1electrons_ = l1electrons.size();
          
          for(int i= n_l1muons_-1; i>=0; i--) if(std::fabs(l1muons[i]->vector().Pt()) < MuPtCut) l1muons.erase(l1muons.begin()+i);
          n_l1muons_ = l1muons.size();
          
          if(JetFilter){
              for(int i= n_l1muons_-1; i>=0; i--) if(std::fabs(l1muons[i]->vector().Rapidity()) > 2.1) l1muons.erase(l1muons.begin()+i);
              n_l1muons_ = l1muons.size();
          }
          std::sort(l1muons.begin(),l1muons.end(),greater_Candidate());
          
          if(JetFilter){
              for(int i= n_l1electrons_-1; i>=0; i--) if(std::fabs(l1electrons[i]->vector().Rapidity()) > 2.1) l1electrons.erase(l1electrons.begin()+i);
              n_l1electrons_ = l1electrons.size();
              std::sort(l1electrons.begin(),l1electrons.end(),greater_Candidate());
          }
          
          if(MuPtCut > 0){
              if(n_l1muons_ < 1) Filter = true;
          }

          if(EGPtCut > 0){
              if(n_l1electrons_ < 1) Filter = true;
          }

      }

      if(channel_ == "et"){
          
          for(int i= n_l1electrons_-1; i>=0; i--) if(std::fabs(l1electrons[i]->vector().Pt()) < EGPtCut) l1electrons.erase(l1electrons.begin()+i);
          n_l1electrons_ = l1electrons.size();
          
          for(int i= n_l1taus_-1; i>=0; i--) if(std::fabs(l1taus[i]->vector().Pt()) < Tau1PtCut) l1taus.erase(l1taus.begin()+i);
          n_l1taus_ = l1taus.size();
          
          if(JetFilter){
              for(int i= n_l1electrons_-1; i>=0; i--) if(std::fabs(l1electrons[i]->vector().Rapidity()) > 2.1) l1electrons.erase(l1electrons.begin()+i);
              n_l1electrons_ = l1electrons.size();
          }
          
          if(n_l1electrons_ > 0 && DeltaRJetFilter){
              
              for(int i= n_l1jets_-1; i>=0; i--){
          
                  double DeltaR = sqrt(pow(l1electrons[0]->vector().Phi()-l1jets[i]->vector().Phi(),2) + pow(l1electrons[0]->vector().Rapidity()-l1jets[i]->vector().Rapidity(),2));
                  if(DeltaR < 0.4 || l1jets[i]->vector().Pt() < Jet1PtCut){
                      l1jets.erase(l1jets.begin()+i);
                  }
              }
          }
          n_l1jets_ = l1jets.size();
          
          if(EGPtCut > 0){
              if(n_l1electrons_ < 1) Filter = true;
          }
          if(Tau1PtCut > 0){
              if(n_l1taus_ < 1) Filter = true;
          }
          
      }
      
      if(channel_ == "mt"){
          
          for(int i= n_l1muons_-1; i>=0; i--) if(std::fabs(l1muons[i]->vector().Pt()) < MuPtCut) l1muons.erase(l1muons.begin()+i);
          n_l1muons_ = l1muons.size();
          
          for(int i= n_l1taus_-1; i>=0; i--) if(std::fabs(l1taus[i]->vector().Pt()) < Tau1PtCut) l1taus.erase(l1taus.begin()+i);
          n_l1taus_ = l1taus.size();
          
          if(JetFilter){
              for(int i= n_l1muons_-1; i>=0; i--) if(std::fabs(l1muons[i]->vector().Rapidity()) > 2.1) l1muons.erase(l1muons.begin()+i);
              n_l1muons_ = l1muons.size();
          }
          
          if(JetFilter){
              for(int i= n_l1taus_-1; i>=0; i--) if(std::fabs(l1taus[i]->vector().Rapidity()) > 2.1) l1taus.erase(l1taus.begin()+i);
              n_l1taus_ = l1taus.size();
          }
                    
          if(MuPtCut > 0){
              if(n_l1muons_ < 1) Filter = true;
          }
          if(Tau1PtCut > 0){
              if(n_l1taus_ < 1) Filter = true;
          }
      }
      
      if(channel_ == "tt"){
          
          if(JetFilter){
              for(int i= n_l1taus_-1; i>=0; i--) if(std::fabs(l1taus[i]->vector().Rapidity()) > 2.1) l1taus.erase(l1taus.begin()+i);
              n_l1taus_ = l1taus.size();
          }
          
          std::vector<ic::L1TTau*> l1leadtaus;
          for(int i= n_l1taus_-1; i>=0; i--){ 
              if(std::fabs(l1taus[i]->vector().Pt()) < Tau2PtCut) l1taus.erase(l1taus.begin()+i);
              if(std::fabs(l1taus[i]->vector().Pt()) >= Tau1PtCut) l1leadtaus.push_back(l1taus[i]);
          }
          
          n_l1taus_ = l1taus.size();
          n_l1leadtaus_ = l1leadtaus.size();
          std::sort(l1leadtaus.begin(),l1leadtaus.end(),greater_Candidate());
        
          if(Tau1PtCut > 0){
              if(n_l1leadtaus_ < 1) Filter = true;
          }
          if(Tau2PtCut > 0){
              if(n_l1taus_ < 2) Filter = true;
          }
          
          if(n_l1leadtaus_ > 0 && DeltaRJetFilter){
              
              for(int i= n_l1jets_-1; i>=0; i--){
          
                  double DeltaR = sqrt(pow(l1leadtaus[0]->vector().Phi()-l1jets[i]->vector().Phi(),2) + pow(l1leadtaus[0]->vector().Rapidity()-l1jets[i]->vector().Rapidity(),2));
                  if(DeltaR < 0.4 || l1jets[i]->vector().Pt() < Jet1PtCut){
                      l1jets.erase(l1jets.begin()+i);
                  }
              }
          }
          n_l1jets_ = l1jets.size();
     }
     
      
      // VBF Cuts
      
      bool dontFilter = true;
      bool doJetFilterLoop = false;

      if(Jet1PtCut > 0 || Jet2PtCut > 0 || MjjCut > 0 || DeltaEtaCut > 0 || AvePtCut > 0 || VecPtCut > 0) doJetFilterLoop = true;
      
      n_l1jets_ = l1jets.size();
      std::sort(l1jets.begin(),l1jets.end(),greater_Candidate());
      
      if(n_l1jets_>0 && doJetFilterLoop) if(l1jets[0]->vector().Pt() < Jet1PtCut) Filter = true;
      if(doJetFilterLoop && n_l1jets_ < 2) Filter = true;

      for(int i= n_l1jets_-1; i>=0; i--) if(std::fabs(l1jets[i]->vector().Pt()) < Jet2PtCut) l1jets.erase(l1jets.begin()+i);
      n_l1jets_ = l1jets.size();
      
      if(n_l1jets_ < 2 && doJetFilterLoop) Filter = true; 
      
   
      if(n_l1jets_>1){
          
         if(l1jets[0]->vector().Pt() < Jet1PtCut) Filter = true;
         dontFilter = false;
         
         for(unsigned i=0; i< n_l1jets_; i++){
             for(unsigned j=i+1; j< n_l1jets_; j++){
        
                 double Mjj = (l1jets[i]->vector() + l1jets[j]->vector()).M();
                 double DeltaEta = std::fabs(l1jets[i]->vector().Rapidity() - l1jets[j]->vector().Rapidity());
                 double AvgPt = (l1jets[i]->vector().Pt()+l1jets[j]->vector().Pt())/2;
                 double VecPt =  (l1jets[i]->vector()+l1jets[j]->vector()).Pt();
                 
                 if(Mjj >= MjjCut && DeltaEta >= DeltaEtaCut && AvgPt >= AvePtCut && VecPt >= VecPtCut && l1jets[i]->vector().Pt() >= Jet2PtCut && l1jets[j]->vector().Pt() >= Jet2PtCut){
                     dontFilter = true;
                 }
             }
         }
     }
     
     if(!dontFilter) Filter = true;
      
     double METNoHF = -1;
     double METHF = -1;
     double MHTHF = -1;
     double HTT = -1;
     
     for(unsigned i=0; i< l1sumsnohf.size(); i++){
         if(l1sumsnohf[i]->sumType==2) METNoHF = l1sumsnohf[i]->vector().Pt();   
         if(l1sumsnohf[i]->sumType==1) HTT   = l1sumsnohf[i]->vector().Pt();
     }
     
     for(unsigned i=0; i< l1sums.size(); i++){
         if(l1sums[i]->sumType==2) METHF = l1sums[i]->vector().Pt();    
         if(l1sums[i]->sumType==3) MHTHF = l1sums[i]->vector().Pt();
     }
     
     h_TotalEvents->Fill(0);
     
     /*Filter = false;
     //bool PassedTripleJetVBF = false;
     
     if(n_l1jets_ < 3) Filter = true;
     if(n_l1jets_ >=3){    
         if(l1jets[0]->vector().Pt() < 80 && l1jets[1]->vector().Pt() <50 && l1jets[2]->vector().Pt() < 25) Filter = true;
     }
     
     double LargestMjj = -1;
     for(unsigned i=0; i<n_l1jets_; ++i){
         for(unsigned j=0; j<n_l1jets_; ++j) {
             if(l1jets[i]->vector().Pt() >= 25 && l1jets[j]->vector().Pt() >=25){
               double Mjj = (l1jets[i]->vector()+l1jets[j]->vector()).M(); 
               if(Mjj > LargestMjj) LargestMjj = Mjj;
             }
         }
     }
     if(LargestMjj < 540) Filter = true;*/
     
     /*for(unsigned i=0; i<n_l1jets_;++i){
         for(unsigned j=i+1; j<n_l1jets_;++j){
             for(unsigned k=j+1; k<n_l1jets_;++k){
                 
                 if(l1jets[i]->vector().Pt() >= 80 && l1jets[j]->vector().Pt() >=50 && l1jets[k]->vector().Pt() >=25){
                    double Mjj[3];
                    Mjj[0] = (l1jets[i]->vector()+l1jets[j]->vector()).M();
                    Mjj[1] = (l1jets[i]->vector()+l1jets[k]->vector()).M();
                    Mjj[2] = (l1jets[j]->vector()+l1jets[k]->vector()).M();
                    if(Mjj[0] >= 540 || Mjj[1] >= 540 || Mjj[2] >= 540) PassedTripleJetVBF = true;
                 }
             }
         }
         
     }
     if(!PassedTripleJetVBF) Filter = true;*/

     
     if(!Filter){

         for(unsigned i=0; i< n_l1taus_; i++) if(l1taus[i]->vector().Pt() >= 20 && l1taus[i]->isolation != 0) h_tauEta_After->Fill(l1taus[i]->vector().Rapidity());
         
         h_MET_HF   ->Fill(METHF);
         h_MET_NOHF ->Fill(METNoHF);
         h_MHT_HF   ->Fill(MHTHF);

         //Check if event fired any other trigger
         
         //SingleJet170
         bool PassedSingleJet170 = false;
         if(n_l1jets_ >= 1){ 
             if(l1jets[0]->vector().Pt() >= 170) {
                 PassedSingleJet170 = true;  
                 h_TriggersPassed->Fill(0);
            }
         }
         //DoubleJetC100
         bool PassedDoubleJet100 = false;
         unsigned Cjetcount1=0;
         for(unsigned i = 0; i<n_l1jets_; i++){
             if(std::fabs(l1jets[i]->vector().Rapidity())<=3 && l1jets[i]->vector().Pt() >= 100) Cjetcount1++;    
         }
         if(Cjetcount1 >= 2) {
             PassedDoubleJet100 = true;
             h_TriggersPassed->Fill(1);
        }
        
         //QuadJetC40
         bool PassedQuadJetC50 = false;
         unsigned Cjetcount=0;
         for(unsigned i = 0; i<n_l1jets_; i++){
             if(std::fabs(l1jets[i]->vector().Rapidity())<=3 && l1jets[i]->vector().Pt() >= 50) Cjetcount++;    
         }
         if(Cjetcount >= 4) {
             PassedQuadJetC50 = true;
             h_TriggersPassed->Fill(2);
        }
        
         //DoubleIsoTau28er
         bool PassedDoubleIsoTau28er = false;
         unsigned taucount=0;
         for(unsigned i = 0; i< n_l1taus_; i++){
             if(std::fabs(l1taus[i]->vector().Rapidity())<=2.1 && l1taus[i]->vector().Pt() >= 28 && l1taus[i]->isolation !=0) taucount++;    
         }
         if(taucount >=2) {
             PassedDoubleIsoTau28er = true;  
             h_TriggersPassed->Fill(3);
        }
        
         //HTT300
         bool PassedHTT300 = false;
         if(HTT >= 300) {
             PassedHTT300 = true; 
             h_TriggersPassed->Fill(4);
        }

         //TripleJet_84_68_48
         bool PassedTripleJet_88_72_56 = false;
         std::vector<ic::L1TJet> centraljets;
         std::vector<ic::L1TJet> forwardsjets;
         for(unsigned i = 0; i<n_l1jets_; i++){
             if(std::fabs(l1jets[i]->vector().Rapidity()) <= 3.) centraljets.push_back(*l1jets[i]);
             else forwardsjets.push_back(*l1jets[i]);
         }
         if(centraljets.size() >=3){
             if(centraljets[0].vector().Pt() >= 88 && centraljets[1].vector().Pt() > 72 && centraljets[2].vector().Pt() >= 56) PassedTripleJet_88_72_56 = true;
         }
         if(centraljets.size() >=2 && forwardsjets.size() >=1){
             if     (centraljets[0].vector().Pt() >= 88 && centraljets[1].vector().Pt() > 72 && forwardsjets[0].vector().Pt() >= 56) PassedTripleJet_88_72_56 = true;
             else if(centraljets[0].vector().Pt() >= 88 && centraljets[1].vector().Pt() > 56 && forwardsjets[0].vector().Pt() >= 72) PassedTripleJet_88_72_56 = true;
             else if(centraljets[0].vector().Pt() >= 72 && centraljets[1].vector().Pt() > 56 && forwardsjets[0].vector().Pt() >= 88) PassedTripleJet_88_72_56 = true;
         }
         if(PassedTripleJet_88_72_56)  h_TriggersPassed->Fill(5);
         
         std::vector<ic::L1TMuon*>   l1muons2   = event->GetPtrVec<ic::L1TMuon>(l1muons_label_);
         std::vector<ic::L1TEGamma*> l1electrons2 = event->GetPtrVec<ic::L1TEGamma>(l1electrons_label_);
         std::sort(l1electrons2.begin(),l1electrons2.end(),greater_Candidate());
         std::sort(l1muons2.begin(),l1muons2.end(),greater_Candidate());
         
         //SingleMu18, SingleMu16er, Mu6_HTT200
         bool PassedSingleMu22 = false;
         bool PassedSingleMu20er = false;
         bool Mu6;
         for(unsigned i = 0; i<l1muons2.size(); i++){
             if(l1muons2[i]->vector().Pt() >= 22 && l1muons2[i]->quality>=12) PassedSingleMu22 = true;
             if(l1muons2[i]->vector().Pt() >= 20 && l1muons2[i]->quality>=12 && std::fabs(l1muons2[i]->vector().Rapidity()) <= 2.1) PassedSingleMu20er = true;
             if(l1muons2[i]->vector().Pt() >= 6 && l1muons2[i]->quality>=12) Mu6 = true;
         }
         if(PassedSingleMu22) h_TriggersPassed->Fill(6);
         if(PassedSingleMu20er)  h_TriggersPassed->Fill(7);
         
         //Mu6HTT200
         bool PassedMu6HTT200 = false;
         if(HTT >= 200 && Mu6) {
             PassedMu6HTT200 = true; 
             h_TriggersPassed->Fill(8);
          }
        
          //SingleEG34, SingleIsoEG28, SingleIsoEG26er, 
          
          bool PassedEG34 = false;
          bool PassedIsoEG28 = false;
          bool PassedIsoEG26er = false;
          
          for(unsigned i = 0; i<l1electrons2.size(); i++){
               if(l1electrons2[i]->vector().Pt() >= 34 ) PassedEG34 = true;
               if(l1electrons2[i]->vector().Pt() >= 26 && l1electrons2[i]->isolation != 0 ) PassedIsoEG28 = true;
               if(l1electrons2[i]->vector().Pt() >= 26 && l1electrons2[i]->isolation != 0 && std::fabs(l1electrons2[i]->vector().Rapidity()) <= 2.1) PassedIsoEG26er = true;
          }
          if(PassedEG34) h_TriggersPassed->Fill(9);
          if(PassedIsoEG28)  h_TriggersPassed->Fill(10);
          if(PassedIsoEG26er)  h_TriggersPassed->Fill(11);
          
          bool PassedTripleJetVBF = false;
          
          for(unsigned i=0; i<n_l1jets_;++i){
              for(unsigned j=i+1; j<n_l1jets_;++j){
                  for(unsigned k=j+1; k<n_l1jets_;++k){
                      
                      if(l1jets[i]->vector().Pt() >= 80 && l1jets[j]->vector().Pt() >=50 && l1jets[k]->vector().Pt() >=25){
                         double Mjj[3];
                         Mjj[0] = (l1jets[i]->vector()+l1jets[j]->vector()).M();
                         Mjj[1] = (l1jets[i]->vector()+l1jets[k]->vector()).M();
                         Mjj[2] = (l1jets[j]->vector()+l1jets[k]->vector()).M();
                         if(Mjj[0] >= 540 || Mjj[1] >= 540 || Mjj[2] >= 540) PassedTripleJetVBF = true;
                      }
                  }
              }
              
          }
          if(PassedTripleJetVBF) h_TriggersPassed->Fill(12);
          
          //bool PassedOtherTrigger = true;
          
          if(!(PassedSingleJet170 || PassedDoubleJet100 || PassedQuadJetC50 || PassedDoubleIsoTau28er || PassedHTT300 || PassedTripleJet_88_72_56 ||  PassedSingleMu22 || PassedSingleMu20er || PassedMu6HTT200 || PassedSingleMu22 || PassedSingleMu20er || PassedMu6HTT200 || PassedEG34 || PassedIsoEG26er || PassedIsoEG28)) h_EventsPassedTriggerPure->Fill(0);
          if(!(PassedSingleJet170 || PassedDoubleJet100 || PassedQuadJetC50 || PassedDoubleIsoTau28er || PassedHTT300 || PassedTripleJet_88_72_56 || PassedTripleJetVBF || PassedSingleMu22 || PassedSingleMu20er || PassedMu6HTT200 || PassedSingleMu22 || PassedSingleMu20er || PassedMu6HTT200 || PassedEG34 || PassedIsoEG26er || PassedIsoEG28)) h_EventsPassedTriggerPure->Fill(1);
         
          h_EventsPassedTrigger->Fill(0);
     
    }   

     if(Filter) return 1;
     else return 0;
      
  }
  
  int L1TFilter::PostAnalysis() {
    h_TriggersPassed->Scale(1./h_EventsPassedTrigger->GetBinContent(1));
    h_partonsmatched->Scale(1./h_partonsmatched->GetBinContent(1));
    return 0;
  }

  void L1TFilter::PrintInfo() {
    ;
  }
}

