#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/L1TFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "TVector3.h"


struct greater_Candidate{
  bool operator() (const ic::Candidate *a,const ic::Candidate *b) {
    return (a->pt() > b->pt());
  }
};

namespace ic {

  L1TFilter::L1TFilter(std::string const& name, std::string channel, fwlite::TFileService *fs, struct L1Cuts l1Cuts) : ModuleBase(name) {
    
    channel_ = channel;
    
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
    
    l1jets_label_ = "L1Jets"; 
    l1electrons_label_ = "L1EGammas";
    l1muons_label_ = "L1Muons"; 
    l1taus_label_ = "L1Taus";  
    l1met_label_ = "L1Sums";
    
    TFileDirectory subDir = fs->mkdir("L1TFilter");

    h_L1Filter_Efficiency = subDir.make<TH1D>("h_L1Filter_Efficiency","h_L1Filter_Efficiency",2, 0,2); 
    h_L1Filter_Efficiency->GetXaxis()->SetTitle("");
    h_L1Filter_Efficiency->GetYaxis()->SetTitle("Entries");
    h_L1Filter_Efficiency->GetXaxis()->SetBinLabel(1,"Total");
    h_L1Filter_Efficiency->GetXaxis()->SetBinLabel(2,"Not Filtered");

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
      std::vector<ic::L1TSum*> l1met_vec = event->GetPtrVec<ic::L1TSum>(l1met_label_);
      
      std::sort(l1taus.begin(),l1taus.end(),greater_Candidate());
      std::sort(l1electrons.begin(),l1electrons.end(),greater_Candidate());
      std::sort(l1muons.begin(),l1muons.end(),greater_Candidate());
      std::sort(l1jets.begin(),l1jets.end(),greater_Candidate());
      
      n_l1jets_ = l1jets.size();
      n_l1electrons_ = l1electrons.size();
      n_l1muons_ = l1muons.size();
      n_l1taus_ = l1taus.size();
      
      /*if(channel_ == "em"){
          Jet1PtCut = 30;
          Jet2PtCut = 30;
          DeltaEtaCut = 3;
          MjjCut = 400;
          EGPtCut = 10;
          Tau1PtCut = 0;
          Tau2PtCut = 0;
          MuPtCut = 10;
          IsoTauFilter = true;
          IsoEGFilter = true;
      }
      if(channel_ == "et"){
          Jet1PtCut = 30;
          Jet2PtCut = 30;
          DeltaEtaCut = 3;
          MjjCut = 400;
          EGPtCut = 10;
          Tau1PtCut = 10;
          Tau2PtCut = 0;
          MuPtCut = 0;
          IsoTauFilter = true;
          IsoEGFilter = true;
      }
      if(channel_ == "mt"){
          Jet1PtCut = 30;
          Jet2PtCut = 30;
          DeltaEtaCut = 3;
          MjjCut = 400;
          EGPtCut = 0;
          Tau1PtCut = 10;
          Tau2PtCut = 0;
          MuPtCut = 10;
          IsoTauFilter = true;
          IsoEGFilter = true;
      }
      if(channel_ == "tt"){
          Jet1PtCut = 30;
          Jet2PtCut = 30;
          DeltaEtaCut = 3;
          MjjCut = 400;
          EGPtCut = 0;
          Tau1PtCut = 10;
          Tau2PtCut = 10;
          MuPtCut = 0;
          IsoTauFilter = true;
          IsoEGFilter = true;
      }*/
      
      if(IsoTauFilter){
          for(int i= n_l1taus_-1; i>=0; i--) if(l1taus[i]->isolation == 0) l1taus.erase(l1taus.begin()+i);
          n_l1taus_ = l1taus.size();
      }
      if(IsoEGFilter){
          for(int i= n_l1electrons_-1; i>=0; i--) if(l1electrons[i]->isolation == 0) l1electrons.erase(l1electrons.begin()+i);
          n_l1electrons_ = l1electrons.size();
      }

      
      if(channel_ == "em"){
          
          for(int i= n_l1jets_-1; i>=0; i--){
    
              bool EraseJet = false;
          
              for(unsigned k=0; k<n_l1electrons_ && k<1; k++){
                  double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- l1electrons[k]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-l1electrons[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.4) EraseJet = true;
              }
          
              if(EraseJet) l1jets.erase(l1jets.begin()+i);
          }
          
          n_l1jets_ = l1jets.size();
      }
      
      if(channel_ == "et"){
          
          for(int i= n_l1taus_-1; i>=0; i--){
    
              bool EraseTau = false;
          
              for(unsigned k=0; k<n_l1electrons_ && k<1; k++){
                  double DeltaR = sqrt(pow(l1taus[i]->vector().Phi()- l1electrons[k]->vector().Phi(),2) + pow(l1taus[i]->vector().Rapidity()-l1electrons[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.4) EraseTau = true;
              }
          
              if(EraseTau) l1taus.erase(l1taus.begin()+i);
          }
          
          n_l1taus_ = l1taus.size();
          
          for(int i= n_l1jets_-1; i>=0; i--){
    
              bool EraseJet = false;

              for(unsigned k=0; k<n_l1taus_ && k<1; k++){
                  double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- l1taus[k]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-l1taus[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.4) EraseJet = true;
              }
          
              if(EraseJet) l1jets.erase(l1jets.begin()+i);
          }
          
          n_l1jets_ = l1jets.size();
          
          for(int i= n_l1jets_-1; i>=0; i--){
    
              bool EraseJet = false;
          
              for(unsigned k=0; k<n_l1electrons_ && k<1; k++){
                  double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- l1electrons[k]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-l1electrons[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.4) EraseJet = true;
              }
          
              if(EraseJet) l1jets.erase(l1jets.begin()+i);
          }
          
          n_l1jets_ = l1jets.size();
      }

      if(channel_ == "mt"){
          
          for(int i= n_l1jets_-1; i>=0; i--){
    
              bool EraseJet = false;
          
              for(unsigned k=0; k<n_l1taus_ && k<1; k++){
                  double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- l1taus[k]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-l1taus[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.4) EraseJet = true;
              }

              if(EraseJet) l1jets.erase(l1jets.begin()+i);
          }
          
          n_l1jets_ = l1jets.size();
      }
      
      if(channel_ == "tt"){
          
          for(int i= n_l1jets_-1; i>=0; i--){
    
              bool EraseJet = false;
          
              for(unsigned k=0; k<n_l1taus_ && k<2; k++){
                  double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- l1taus[k]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-l1taus[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.4) EraseJet = true;
              }
          
              if(EraseJet) l1jets.erase(l1jets.begin()+i);
          }
          
          n_l1jets_ = l1jets.size();
      }
          
      bool Filter = false;
      
      if(n_l1jets_ <=2) Filter = true;
    
      if(n_l1jets_>1){

          if(l1jets[0]->vector().Pt() < Jet1PtCut || l1jets[1]->vector().Pt() < Jet2PtCut) Filter = true;
        
          double LargestMjj = -1;
          double LargestDeltaEta = -1;
        
          for(unsigned i=0; i< n_l1jets_; i++){
            
              for(unsigned j=0; j< n_l1jets_; j++){
                
                  if(i!=j){
                  
                      double Mjj = (l1jets[i]->vector() + l1jets[j]->vector()).M();
                      if(Mjj > LargestMjj) LargestMjj = Mjj;
                    
                      double DeltaEta = std::fabs(l1jets[i]->vector().Rapidity() - l1jets[j]->vector().Rapidity());
                      if(DeltaEta > LargestDeltaEta) LargestDeltaEta = DeltaEta;
                  }
              }
          }
        
          if(LargestDeltaEta < DeltaEtaCut) Filter = true;
          if(LargestMjj < MjjCut) Filter = true;
      }
      
      if(n_l1electrons_ > 0) if(l1electrons[0]->vector().Pt() < EGPtCut) Filter = true;
      if(n_l1muons_ > 0) if(l1muons[0]->vector().Pt() < MuPtCut) Filter = true;
      if(n_l1taus_ > 0) if(l1taus[0]->vector().Pt() < Tau1PtCut) Filter = true;
      if(n_l1taus_ > 1) if(l1taus[1]->vector().Pt() < Tau2PtCut) Filter = true;
      
      h_L1Filter_Efficiency->Fill(0);
      if(!Filter) h_L1Filter_Efficiency->Fill(1);
      
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

