#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/L1TFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "TVector3.h"


struct greater_Candidate{
  bool operator() (const ic::Candidate *a,const ic::Candidate *b) {
    return (a->pt() > b->pt());
  }
};

namespace ic {

  L1TFilter::L1TFilter(std::string const& name, std::string channel, fwlite::TFileService *fs, struct L1Cuts l1Cuts, std::string L1MuTag, std::string subDirName) : ModuleBase(name) {
    
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
    IsoMuFilter = l1Cuts.IsoMu;
    JetFilter = l1Cuts.JetFilter;
    
    l1jets_label_ = "L1Jets"; 
    l1electrons_label_ = "L1EGammas";
    l1muons_label_ = L1MuTag;
    l1taus_label_ = "L1Taus";  
    l1met_label_ = "L1Sums";
    
    TFileDirectory subDir = fs->mkdir(subDirName.c_str());

    h_L1Filter_Efficiency = subDir.make<TH1D>("h_L1Filter_Efficiency","h_L1Filter_Efficiency",2, 0,2); 
    h_L1Filter_Efficiency->GetXaxis()->SetTitle("");
    h_L1Filter_Efficiency->GetYaxis()->SetTitle("Entries");
    h_L1Filter_Efficiency->GetXaxis()->SetBinLabel(1,"Total");
    h_L1Filter_Efficiency->GetXaxis()->SetBinLabel(2,"Not Filtered");
    
    h_SignalEfficiency = subDir.make<TH1D>("h_SignalEfficiency","h_SignalEfficiency",16, 0,16); 
    h_SignalEfficiency->GetXaxis()->SetTitle("");
    h_SignalEfficiency->GetYaxis()->SetTitle("Entries");
    
    h_METRates = subDir.make<TH1D>("h_METRates","h_METRates",100, 0,200); 
    h_METRates->GetXaxis()->SetTitle("MET [GeV]");
    h_METRates->GetYaxis()->SetTitle("Entries");


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
      
      h_METRates->Fill(l1met_vec[2]->et);
      
      // Filter Non-iso Particles is option is selected
      
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


      // Filter jest matched to electrons/taus
      
      if(channel_ == "em" && JetFilter == true){
          
          for(int i= n_l1jets_-1; i>=0; i--){
    
              bool EraseJet = false;
          
              for(unsigned k=0; k<n_l1electrons_ && k<1; k++){
                  double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- l1electrons[k]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-l1electrons[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.1) EraseJet = true;
              }
          
              if(EraseJet) l1jets.erase(l1jets.begin()+i);
          }
          
          n_l1jets_ = l1jets.size();
      }
      
      if(channel_ == "et" && JetFilter == true){
          
          for(int i= n_l1taus_-1; i>=0; i--){
    
              bool EraseTau = false;
          
              for(unsigned k=0; k<n_l1electrons_ && k<1; k++){
                  double DeltaR = sqrt(pow(l1taus[i]->vector().Phi()- l1electrons[k]->vector().Phi(),2) + pow(l1taus[i]->vector().Rapidity()-l1electrons[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.1) EraseTau = true;
              }
          
              if(EraseTau) l1taus.erase(l1taus.begin()+i);
          }
          
          n_l1taus_ = l1taus.size();
          
          for(int i= n_l1jets_-1; i>=0; i--){
    
              bool EraseJet = false;

              for(unsigned k=0; k<n_l1taus_ && k<1; k++){
                  double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- l1taus[k]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-l1taus[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.1) EraseJet = true;
              }
          
              if(EraseJet) l1jets.erase(l1jets.begin()+i);
          }
          
          n_l1jets_ = l1jets.size();
          
          for(int i= n_l1jets_-1; i>=0; i--){
    
              bool EraseJet = false;
          
              for(unsigned k=0; k<n_l1electrons_ && k<1; k++){
                  double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- l1electrons[k]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-l1electrons[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.1) EraseJet = true;
              }
          
              if(EraseJet) l1jets.erase(l1jets.begin()+i);
          }
          
          n_l1jets_ = l1jets.size();
      }

      if(channel_ == "mt" && JetFilter == true){
          
          for(int i= n_l1jets_-1; i>=0; i--){
    
              bool EraseJet = false;
          
              for(unsigned k=0; k<n_l1taus_ && k<1; k++){
                  double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- l1taus[k]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-l1taus[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.1) EraseJet = true;
              }

              if(EraseJet) l1jets.erase(l1jets.begin()+i);
          }
          
          n_l1jets_ = l1jets.size();
      }
      
      if(channel_ == "tt" && JetFilter == true){
          
          for(int i= n_l1jets_-1; i>=0; i--){
    
              bool EraseJet = false;
          
              for(unsigned k=0; k<n_l1taus_ && k<2; k++){
                  double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- l1taus[k]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-l1taus[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.1) EraseJet = true;
              }
          
              if(EraseJet) l1jets.erase(l1jets.begin()+i);
          }
          
          n_l1jets_ = l1jets.size();
      }
      
      // Lepton selections
          
      bool Filter = false;
      
      h_SignalEfficiency->Fill(0);
      
      if(channel_ == "em"){
          
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
          
          /*if(n_l1muons_ < 1 || n_l1electrons_ < 1) Filter = true;
          if(n_l1muons_ > 0 && n_l1electrons_ > 0){
              bool dontfilter = false;
              for(unsigned i =0; i< n_l1muons_; i++){
                  if((l1muons[0]->vector().Pt() >= 20 && l1electrons[0]->vector().Pt() >= 15 )||(l1muons[0]->vector().Pt() >= 5 && l1electrons[0]->vector().Pt() >= 20 )|| (l1muons[0]->vector().Pt() >= 5 && l1electrons[i]->vector().Pt() >= 18 && l1electrons[i]->isolation > 0)) dontfilter = true; 
                      //||(l1muons[0]->vector().Pt() >= 5 && l1electrons[i]->vector().Pt() >= 18 && l1electrons[i]->isolation > 0)) dontfilter = true; 
          }
              if(!dontfilter) Filter = true; 
          }
          
          
          if(!Filter) h_SignalEfficiency->Fill(1);
          if(!Filter) h_SignalEfficiency->Fill(2);*/

      }

      if(channel_ == "et"){
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
          
      /*if(n_l1electrons_ < 1) Filter = true;
          if(n_l1electrons_ > 0){
              bool dontfilter = false;
              for(unsigned i =0; i< n_l1electrons_; i++){
                   if((l1electrons[0]->vector().Pt() >= 40 )||(l1electrons[i]->vector().Pt() >= 22  && l1electrons[i]->isolation > 0)) dontfilter = true;    
              }

              if(!dontfilter) Filter = true; 
          }
          
          if(!Filter) h_SignalEfficiency->Fill(1);
          if(!Filter) h_SignalEfficiency->Fill(2);*/
      }
      
      if(channel_ == "mt"){
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
      
      bool dontFilter = false;
         
      if(n_l1jets_>1){
         
         for(unsigned i=0; i< n_l1jets_; i++){
       
             for(unsigned j=i+1; j< n_l1jets_; j++){
        
                 double Mjj = (l1jets[i]->vector() + l1jets[j]->vector()).M();
                 double DeltaEta = std::fabs(l1jets[i]->vector().Rapidity() - l1jets[j]->vector().Rapidity());
                     
                 if(Mjj >=MjjCut && DeltaEta >= DeltaEtaCut && (l1jets[i]->vector().Pt()+l1jets[i]->vector().Pt())/2 >= Jet1PtCut ) dontFilter = true;        
             }
      
         }

         
         if(!dontFilter) Filter = true;
         if(!Filter) h_SignalEfficiency->Fill(3);
         
     }
 
      
      
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

