#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/L1TFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "TVector3.h"


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
    
    l1jets_label_ = "L1Jets"; 
    l1electrons_label_ = "L1EGammas";
    l1muons_label_ = L1MuTag;
    l1taus_label_ = "L1Taus";  
    
    TFileDirectory subDir = fs->mkdir(subDirName.c_str());

    h_L1Filter_Efficiency = subDir.make<TH1D>("h_L1Filter_Efficiency","h_L1Filter_Efficiency",2000000,0,2000000); 
    
    h_SignalEfficiency = subDir.make<TH1D>("h_SignalEfficiency","h_SignalEfficiency",16, 0,16); 
    h_SignalEfficiency->GetXaxis()->SetTitle("");
    h_SignalEfficiency->GetYaxis()->SetTitle("Entries");
    
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
    
  }

  L1TFilter::~L1TFilter() {
      ;
  }

  int L1TFilter::PreAnalysis() {

    return 0;
  }

  int L1TFilter::Execute(TreeEvent *event) {
      
      ic::EventInfo* EventInfo = event->GetPtr<ic::EventInfo>("eventInfo");
      
      if(isZB == 1) if(EventInfo->run() != 259721) return 1;
      
      std::vector<ic::L1TJet*>  l1jets = event->GetPtrVec<ic::L1TJet>(l1jets_label_);
      std::vector<ic::L1TEGamma*> l1electrons = event->GetPtrVec<ic::L1TEGamma>(l1electrons_label_);
      std::vector<ic::L1TMuon*> l1muons = event->GetPtrVec<ic::L1TMuon>(l1muons_label_);
      std::vector<ic::L1TTau*> l1taus = event->GetPtrVec<ic::L1TTau>(l1taus_label_);
      
      std::sort(l1taus.begin(),l1taus.end(),greater_Candidate());
      std::sort(l1electrons.begin(),l1electrons.end(),greater_Candidate());
      std::sort(l1muons.begin(),l1muons.end(),greater_Candidate());
      std::sort(l1jets.begin(),l1jets.end(),greater_Candidate());
      
      n_l1jets_ = l1jets.size();
      n_l1electrons_ = l1electrons.size();
      n_l1muons_ = l1muons.size();
      n_l1taus_ = l1taus.size();
      
      if(n_l1jets_>1){
         for(unsigned i=0; i< n_l1jets_; i++){

             for(unsigned j=i+1; j< n_l1jets_; j++){
                 h_l1jj_VecPt->Fill((l1jets[i]->vector() + l1jets[j]->vector()).Pt());
                 h_l1jj_DiffOverSum->Fill((l1jets[i]->vector().Pt() - l1jets[j]->vector().Pt())/(l1jets[i]->vector().Pt() + l1jets[j]->vector().Pt()));
                 h_l1jj_Diff->Fill(std::fabs(l1jets[i]->vector().Pt() - l1jets[j]->vector().Pt()));
             }
         }
      }
      
      for(unsigned i=0; i< n_l1taus_; i++) if(l1taus[i]->vector().Pt() >= 20 && l1taus[i]->isolation == 1) h_tauEta_Before->Fill(l1taus[i]->vector().Rapidity());    
      
      // Filter Non-iso Particles is option is selected
      
      /*if(n_l1taus_ > 0) if(l1taus[0]->vector().Pt() > 20){
          std::cout << "Taus Pt, iso: " << std::endl;
          for(unsigned i=0; i <n_l1taus_; i++){
              std::cout << l1taus[i]->vector().Pt() << "  " << l1taus[i]->isolation<<std::endl;   
          }
      }*/
      
      if(n_l1taus_ > 1) h_DoubleTauPt->Fill(l1taus[1]->vector().Pt());
      else h_DoubleTauPt->Fill(0);
      if(n_l1electrons_ > 0) h_SingleEGPt->Fill(l1electrons[0]->vector().Pt());
      else h_SingleEGPt->Fill(0);
      if(n_l1muons_ > 0) h_SingleMuPt->Fill(l1muons[0]->vector().Pt());
      else h_SingleMuPt->Fill(0);
      if(n_l1jets_ > 0) h_SingleJetPt->Fill(l1jets[0]->vector().Pt());
      else h_SingleJetPt->Fill(0);
      if(n_l1jets_ > 1) h_DoubleJetPt->Fill(l1jets[1]->vector().Pt());
      else h_DoubleJetPt->Fill(0);
      if(n_l1jets_ > 1) h_DoubleJetMjj->Fill((l1jets[0]->vector()+l1jets[1]->vector()).M());
      else h_DoubleJetMjj->Fill(0);
      if(n_l1jets_ > 1) h_DoubleJetDeltaEta->Fill(std::fabs(l1jets[0]->vector().Rapidity()-l1jets[1]->vector().Rapidity()));
      else h_DoubleJetDeltaEta->Fill(0);
      if(n_l1jets_ > 1) h_DoubleJetAvePt->Fill((l1jets[0]->vector().Pt()+l1jets[1]->vector().Pt())/2);
      else h_DoubleJetAvePt->Fill(0);
      if(n_l1jets_ > 1) h_DoubleJetVecPt->Fill((l1jets[0]->vector()+l1jets[1]->vector()).Pt());
      else h_DoubleJetVecPt->Fill(0);
      
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
      
      std::sort(l1taus.begin(),l1taus.end(),greater_Candidate());
      std::sort(l1electrons.begin(),l1electrons.end(),greater_Candidate());
      std::sort(l1muons.begin(),l1muons.end(),greater_Candidate());
      
      if(n_l1taus_ > 1) h_DoubleIsoTauPt->Fill(l1taus[1]->vector().Pt());
      else h_DoubleIsoTauPt->Fill(0);
      if(n_l1electrons_ > 0) h_SingleIsoEGPt->Fill(l1electrons[0]->vector().Pt());
      else h_SingleIsoEGPt->Fill(0);
      if(n_l1muons_ > 0) h_SingleIsoMuPt->Fill(l1muons[0]->vector().Pt());
      else h_SingleIsoMuPt->Fill(0);
      
      /*if(n_l1taus_ > 0) if(l1taus[0]->vector().Pt() > 20){
          std::cout << "IsoTaus Pt, iso: " << std::endl;
          for(unsigned i=0; i <n_l1taus_; i++){
              std::cout << l1taus[i]->vector().Pt() << "  " << l1taus[i]->isolation<<std::endl;     
          }
      }*/

      // Filter jest matched to electrons/taus
      
      // Lepton selections
          
      bool Filter = false;
      double EtaCut = 0;
      
      h_SignalEfficiency->Fill(0);
      
      if(channel_ == "em"){
          
          //if(JetFilter) EtaCut = 2.1;
          
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
              if(n_l1muons_ > 0) if(l1muons[0]->vector().Pt() < MuPtCut) Filter = true;
          }
          if(!Filter) h_SignalEfficiency->Fill(1);
          if(EGPtCut > 0){
              if(n_l1electrons_ < 1) Filter = true;
              if(n_l1electrons_ > 0) if(l1electrons[0]->vector().Pt() < EGPtCut) Filter = true;
          }
          if(!Filter) h_SignalEfficiency->Fill(2);

      }

      if(channel_ == "et"){
          
          if(JetFilter) EtaCut = 2.1;
          
          /*if(JetFilter){
              for(int i= n_l1taus_-1; i>=0; i--) if(std::fabs(l1taus[i]->vector().Rapidity()) > 0) l1taus.erase(l1taus.begin()+i);
              n_l1taus_ = l1taus.size();
          }
          std::sort(l1taus.begin(),l1taus.end(),greater_Candidate());
          */
          /*if(JetFilter){
              for(int i= n_l1electrons_-1; i>=0; i--) if(std::fabs(l1electrons[i]->vector().Rapidity()) > 2.1) l1electrons.erase(l1electrons.begin()+i);
              n_l1electrons_ = l1electrons.size();
          }
          std::sort(l1electrons.begin(),l1electrons.end(),greater_Candidate());*/
          
          if(JetFilter){
              for(int i= n_l1electrons_-1; i>=0; i--) if(std::fabs(l1electrons[i]->vector().Rapidity()) > 2.1) l1electrons.erase(l1electrons.begin()+i);
              n_l1electrons_ = l1electrons.size();
              std::sort(l1electrons.begin(),l1electrons.end(),greater_Candidate());
              
              /*for(int i= n_l1jets_-1; i>=0; i--){
                  double DeltaR = sqrt(pow(l1electrons[0]->vector().Phi()-l1jets[i]->vector().Phi(),2) + pow(l1electrons[0]->vector().Rapidity()-l1jets[i]->vector().Rapidity(),2));
                  if(DeltaR < 0.5) l1jets.erase(l1jets.begin()+i);
              }
              n_l1jets_ = l1jets.size();
              std::sort(l1jets.begin(),l1jets.end(),greater_Candidate());*/
          }
          
          
          /*if(EGPtCut > 0 && Tau1PtCut > 0){
              bool dontFilter = false;
              
              for(unsigned i=0; i<n_l1electrons_; i++){
                  for(unsigned j=0; j<n_l1taus_; j++){
                      double EtaSep = std::fabs(l1electrons[i]->vector().Rapidity() - l1taus[i]->vector().Rapidity());
                      if(l1electrons[i]->vector().Pt() >= EGPtCut && l1taus[j]->vector().Pt() >= Tau1PtCut && EtaSep > 0.2) dontFilter = true;   
                  }
              }
              if(!dontFilter) Filter = true;
          }*/
          
          if(EGPtCut > 0){
              if(n_l1electrons_ < 1) Filter = true;
              if(n_l1electrons_ > 0) if(l1electrons[0]->vector().Pt() < EGPtCut) Filter = true;
          }
          if(!Filter) h_SignalEfficiency->Fill(1);
          if(Tau1PtCut > 0){
              if(n_l1taus_ < 1) Filter = true;
              if(n_l1taus_ > 0) if(l1taus[0]->vector().Pt() < Tau1PtCut) Filter = true;
          }
          if(!Filter) h_SignalEfficiency->Fill(2);
          
      }
      
      if(channel_ == "mt"){
          
          if(JetFilter) EtaCut = 2.1;
          
          if(JetFilter){
              for(int i= n_l1muons_-1; i>=0; i--) if(std::fabs(l1muons[i]->vector().Rapidity()) > 2.1) l1muons.erase(l1muons.begin()+i);
              n_l1muons_ = l1muons.size();
          }
          std::sort(l1muons.begin(),l1muons.end(),greater_Candidate());
          
          if(JetFilter){
              for(int i= n_l1taus_-1; i>=0; i--) if(std::fabs(l1taus[i]->vector().Rapidity()) > 2.1) l1taus.erase(l1taus.begin()+i);
              n_l1taus_ = l1taus.size();
          }
          std::sort(l1taus.begin(),l1taus.end(),greater_Candidate());
                    
          if(MuPtCut > 0){
              if(n_l1muons_ < 1) Filter = true;
              if(n_l1muons_ > 0) if(l1muons[0]->vector().Pt() < MuPtCut) Filter = true;
          }
          if(!Filter) h_SignalEfficiency->Fill(1);
          if(Tau1PtCut > 0){
              if(n_l1taus_ < 1) Filter = true;
              if(n_l1taus_ > 0) if(l1taus[0]->vector().Pt() < Tau1PtCut) Filter = true;
          }
          if(!Filter) h_SignalEfficiency->Fill(2);
      }
      
      if(channel_ == "tt"){
          
          if(JetFilter) EtaCut = 2.1;
          
          if(JetFilter){
              for(int i= n_l1taus_-1; i>=0; i--) if(std::fabs(l1taus[i]->vector().Rapidity()) > 2.1) l1taus.erase(l1taus.begin()+i);
              n_l1taus_ = l1taus.size();
          }
          std::sort(l1taus.begin(),l1taus.end(),greater_Candidate());
        
          if(Tau1PtCut > 0){
              if(n_l1taus_ < 1) Filter = true;
              if(n_l1taus_ > 0) if(l1taus[0]->vector().Pt() < Tau1PtCut) Filter = true;
          }
          if(!Filter) h_SignalEfficiency->Fill(1);
          if(Tau2PtCut > 0){
              if(n_l1taus_ < 2) Filter = true;
              if(n_l1taus_ > 1) if(l1taus[1]->vector().Pt() < Tau2PtCut) Filter = true;
          }
          if(!Filter) h_SignalEfficiency->Fill(2);
     }
      
      
      // VBF Cuts
      
      bool dontFilter = true;
      bool doJetFilterLoop = false;

      if(Jet1PtCut > 0 || Jet2PtCut > 0 || MjjCut > 0 || DeltaEtaCut > 0) doJetFilterLoop = true;
      if(doJetFilterLoop && n_l1jets_ < 2) Filter = true;
   
      if(n_l1jets_>1 && doJetFilterLoop){

         dontFilter = false;
      
         for(unsigned i=0; i< n_l1jets_; i++){
       
             for(unsigned j=i+1; j< n_l1jets_; j++){
        
                 double Mjj = (l1jets[i]->vector() + l1jets[j]->vector()).M();
                 double DeltaEta = std::fabs(l1jets[i]->vector().Rapidity() - l1jets[j]->vector().Rapidity());
                 
                 if(Mjj >= MjjCut && DeltaEta >= DeltaEtaCut && (l1jets[i]->vector().Pt()+l1jets[j]->vector().Pt())/2 >= AvePtCut && (std::fabs(l1jets[i]->vector().Rapidity()) >= EtaCut || std::fabs(l1jets[j]->vector().Rapidity()) >= EtaCut) && (l1jets[i]->vector()+l1jets[j]->vector()).Pt() >= VecPtCut && l1jets[i]->vector().Pt() >= Jet1PtCut && l1jets[j]->vector().Pt() >= Jet2PtCut){
                     dontFilter = true;
                     h_jetsMjj->Fill(Mjj);
                     h_jetsDeltaEta->Fill(DeltaEta); 
                     h_jetsAvePt->Fill((l1jets[i]->vector().Pt()+l1jets[j]->vector().Pt())/2);
                     h_jetsVecPt->Fill((l1jets[i]->vector()+l1jets[j]->vector()).Pt());
                     double Eta1 = std::fabs(l1jets[i]->vector().Rapidity());
                     double Eta2 = std::fabs(l1jets[j]->vector().Rapidity());
                     h_jet1Eta->Fill(std::min(Eta1,Eta2));
                     h_jet2Eta->Fill(std::max(Eta1,Eta2));
                 }
             }
         }    
     }
     
     if(!dontFilter) Filter = true; 
     
     ic::EventInfo const* eventInfo = event->GetPtr<ic::EventInfo>("eventInfo");
     //1.06382e+09
     
     if(!Filter){
         h_SignalEfficiency->Fill(3);
         h_L1Filter_Efficiency->Fill(eventInfo->event());
         for(unsigned i=0; i< n_l1taus_; i++) if(l1taus[i]->vector().Pt() >= 20 && l1taus[i]->isolation == 1) h_tauEta_After->Fill(l1taus[i]->vector().Rapidity());
         
         if(n_l1taus_ > 0) h_tau1Eta->Fill(l1taus[0]->vector().Rapidity());
         if(n_l1taus_ > 1) h_tau2Eta->Fill(l1taus[1]->vector().Rapidity()); 
         if(n_l1taus_ > 0) h_tau1Pt->Fill(l1taus[0]->vector().Pt()); 
         if(n_l1taus_ > 1) h_tau2Pt->Fill(l1taus[1]->vector().Pt()); 
         
         if(n_l1electrons_ > 0) h_electron1Eta->Fill(l1electrons[0]->vector().Rapidity());
         if(n_l1electrons_ > 1) h_electron2Eta->Fill(l1electrons[1]->vector().Rapidity()); 
         if(n_l1electrons_ > 0) h_electron1Pt->Fill(l1electrons[0]->vector().Pt()); 
         if(n_l1electrons_ > 1) h_electron2Pt->Fill(l1electrons[1]->vector().Pt());
         
         if(n_l1muons_ > 0) h_muon1Eta->Fill(l1muons[0]->vector().Rapidity());
         if(n_l1muons_ > 1) h_muon2Eta->Fill(l1muons[1]->vector().Rapidity()); 
         if(n_l1muons_ > 0) h_muon1Pt->Fill(l1muons[0]->vector().Pt()); 
         if(n_l1muons_ > 1) h_muon2Pt->Fill(l1muons[1]->vector().Pt());
    }   

     if(Filter) return 1;
     else return 0;
      
  }
  
  int L1TFilter::PostAnalysis() {
    return 0;
  }

  void L1TFilter::PrintInfo() {
    ;
  }
}

