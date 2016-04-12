#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/L1TFilterPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "TVector3.h"


struct greater_Candidate{
  bool operator() (const ic::Candidate *a,const ic::Candidate *b) {
    return (a->pt() > b->pt());
  }
};

namespace ic {

  L1TFilterPlots::L1TFilterPlots(std::string const& name, std::string channel, fwlite::TFileService *fs, struct L1Cuts l1Cuts, std::string dirName, int filterOpt) : ModuleBase(name) {
    
    channel_ = channel;
    
    CancelFilter = false;
    if(filterOpt == 1) CancelFilter = true;
    
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
    l1muons_label_ = "L1Muons";
    l1taus_label_ = "L1Taus";  
    l1met_label_ = "L1Sums";
    
    TFileDirectory subDir = fs->mkdir(dirName.c_str());

    h_L1Filter_Efficiency = subDir.make<TH1D>("h_L1Filter_Efficiency","h_L1Filter_Efficiency",2, 0,2); 
    h_L1Filter_Efficiency->GetXaxis()->SetTitle("");
    h_L1Filter_Efficiency->GetYaxis()->SetTitle("Entries");
    h_L1Filter_Efficiency->GetXaxis()->SetBinLabel(1,"Total");
    h_L1Filter_Efficiency->GetXaxis()->SetBinLabel(2,"Not Filtered");
    
    h_lead_l1Index_1 = subDir.make<TH1D>("h_lead_l1Index_1","h_lead_l1Index_1",11, -1,10); 
    h_lead_l1Index_1->GetXaxis()->SetTitle("");
    h_lead_l1Index_1->GetYaxis()->SetTitle("Entries");
    
    h_sublead_l1Index_1 = subDir.make<TH1D>("h_sublead_l1Index_1","h_sublead_l1Index_1",11, -1,10); 
    h_sublead_l1Index_1->GetXaxis()->SetTitle("");
    h_sublead_l1Index_1->GetYaxis()->SetTitle("Entries");
    
    h_lead_l1Index_2 = subDir.make<TH1D>("h_lead_l1Index_2","h_lead_l1Index_2",11, -1,10); 
    h_lead_l1Index_2->GetXaxis()->SetTitle("");
    h_lead_l1Index_2->GetYaxis()->SetTitle("Entries");
    
    h_sublead_l1Index_2 = subDir.make<TH1D>("h_sublead_l1Index_2","h_sublead_l1Index_2",11, -1,10); 
    h_sublead_l1Index_2->GetXaxis()->SetTitle("");
    h_sublead_l1Index_2->GetYaxis()->SetTitle("Entries");
    
    h_jets_l1Index_1 = subDir.make<TH2D>("h_jets_l1Index_1","h_jets_l1Index_1",11, -1, 10, 11, -1,10); 
    h_jets_l1Index_1->GetXaxis()->SetTitle("Lead-Jet Index");
    h_jets_l1Index_1->GetYaxis()->SetTitle("Sub-Lead Jet Index");
    
    h_jets_l1Index_2 = subDir.make<TH2D>("h_jets_l1Index_2","h_jets_l1Index_2",11, -1, 10, 11, -1,10); 
    h_jets_l1Index_2->GetXaxis()->SetTitle("Lead-Jet Index");
    h_jets_l1Index_2->GetYaxis()->SetTitle("Sub-Lead Jet Index");
    
    h_jets_l1IndexGenMatch_1 = subDir.make<TH2D>("h_jets_l1IndexGenMatch_1","h_jets_l1IndexGenMatch_1",11, -1, 10, 11, -1,10); 
    h_jets_l1IndexGenMatch_1->GetXaxis()->SetTitle("Lead-Jet Index");
    h_jets_l1IndexGenMatch_1->GetYaxis()->SetTitle("Sub-Lead Jet Index");
    
    h_jets_l1IndexGenMatch_2 = subDir.make<TH2D>("h_jets_l1IndexGenMatch_2","h_jets_l1IndexGenMatch_2",11, -1, 10, 11, -1,10); 
    h_jets_l1IndexGenMatch_2->GetXaxis()->SetTitle("Lead-Jet Index");
    h_jets_l1IndexGenMatch_2->GetYaxis()->SetTitle("Sub-Lead Jet Index");
    
    h_jets_OfflineIndexGenMatch_1 = subDir.make<TH2D>("h_jets_OfflineIndexGenMatch_1","h_jets_OfflineIndexGenMatch_1",11, -1, 10, 11, -1,10); 
    h_jets_OfflineIndexGenMatch_1->GetXaxis()->SetTitle("Lead-Jet Index");
    h_jets_OfflineIndexGenMatch_1->GetYaxis()->SetTitle("Sub-Lead Jet Index");
    
    h_SignalEfficiency = subDir.make<TH1D>("h_SignalEfficiency","h_SignalEfficiency",16, 0,16); 
    h_SignalEfficiency->GetXaxis()->SetTitle("");
    h_SignalEfficiency->GetYaxis()->SetTitle("Entries");
    
    h_CorrectVBFJets = subDir.make<TH1D>("h_CorrectVBFJets","h_CorrectVBFJets",5, 0,5); 
    h_CorrectVBFJets->GetXaxis()->SetTitle("");
    h_CorrectVBFJets->GetYaxis()->SetTitle("Entries");
    
    h_VBFMultiplicity = subDir.make<TH1D>("h_VBFMultiplicity","h_VBFMultiplicity",10, 0,10); 
    h_VBFMultiplicity->GetXaxis()->SetTitle("VBF Pairs Multiplicity");
    h_VBFMultiplicity->GetYaxis()->SetTitle("Entries");

  }

  L1TFilterPlots::~L1TFilterPlots() {
    ;
  }

  int L1TFilterPlots::PreAnalysis() {

    return 0;
  }

  int L1TFilterPlots::Execute(TreeEvent *event) {

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
      
      n_l1muons_ = l1muons.size();
      n_l1electrons_ = l1electrons.size();
      n_l1taus_ = l1taus.size();
      
      std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("ak4PFJetsCHS");
      unsigned n_jets_ = jets.size();
      
      std::vector<ic::GenJet*> genjets = event->GetPtrVec<ic::GenJet>("genJets");
      unsigned n_genjets_ = genjets.size();
      std::vector<ic::GenParticle*> GenParticles = event->GetPtrVec<ic::GenParticle>("genParticles");
      unsigned n_genParticles_ = GenParticles.size();
              
      for(int i= n_genjets_-1; i>=0; i--){
          
          bool EraseGenJet = false;
              
          for(unsigned j=0; j< n_genParticles_; j++){

              unsigned genID = std::fabs(GenParticles[j]->pdgid());
              bool isHadTau = false;
              bool isPrompt = false;

              if(genID == 15) isHadTau = true;
                  
              for(unsigned k=0; k < GenParticles[j]->mothers().size(); k++) {
                  if(std::fabs(GenParticles[GenParticles[j]->mothers().at(k)]->pdgid()) == 25) isPrompt = true;
                  else if(std::fabs(GenParticles[GenParticles[j]->mothers().at(k)]->pdgid()) == 15 && (genID == 11 || genID == 13)){
                      int ParentID = GenParticles[j]->mothers().at(k);
                      if(std::fabs(GenParticles[GenParticles[ParentID]->mothers().at(0)]->pdgid()) == 25) isPrompt = true;
                  }
              }
              ic::Candidate tausVis;
              tausVis.set_vector(GenParticles[j]->vector());
              
              if(genID == 15){
                  for(unsigned k=0; k < GenParticles[j]->daughters().size(); k++) {
                      if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 11 || std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 13) isHadTau = false;
                      if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 16) tausVis.set_vector(tausVis.vector() - GenParticles[GenParticles[j]->daughters().at(k)]->vector());
                  }
              }

              double DeltaR = sqrt(pow(genjets[i]->vector().Phi()-tausVis.vector().Phi(),2) + pow(genjets[i]->vector().Rapidity()-tausVis.vector().Rapidity(),2));
              if(DeltaR < 0.4 && (isHadTau || genID == 11 || genID == 13) && isPrompt) EraseGenJet = true;
          }
          
          if(EraseGenJet) genjets.erase(genjets.begin()+i);
      }
      n_genjets_ = genjets.size();
      
      if(n_genjets_ > 1) if(genjets[0]->vector().Pt() > 20 && genjets[1]->vector().Pt() > 20 && std::fabs(genjets[0]->vector().Rapidity()) < 5 && std::fabs(genjets[1]->vector().Rapidity()) < 5){
          
          double PtDiffMin1 = 10000;
          double PtDiffMin2 = 10000;
          int index1 = -1;
          int index2 = -1;
      
          for(unsigned i=0; i< n_l1jets_; i++){
              double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- genjets[0]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-genjets[0]->vector().Rapidity(),2));
              double PtDiff = std::fabs(l1jets[i]->vector().Pt() - genjets[0]->vector().Pt());
          
              if(DeltaR < 0.4 && PtDiff < PtDiffMin1){
                  index1 = i;
                  PtDiffMin1 = PtDiff;
              }
          }
      
          for(unsigned i=0; i< n_l1jets_; i++){
              double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- genjets[1]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-genjets[1]->vector().Rapidity(),2));
              double PtDiff = std::fabs(l1jets[i]->vector().Pt() - genjets[1]->vector().Pt());
          
              if(DeltaR < 0.4 && PtDiff < PtDiffMin2){
                  index2 = i;
                  PtDiffMin2 = PtDiff;
              }
          }

          h_jets_l1IndexGenMatch_1->Fill(index1,index2);
      
      }
      
      if(n_jets_ > 1) if(jets[0]->vector().Pt() > 20 && jets[1]->vector().Pt() > 20){
          
          double PtDiffMin1 = 10000;
          double PtDiffMin2 = 10000;
          int index1 = -1;
          int index2 = -1;
      
          for(unsigned i=0; i< n_genjets_; i++){
              double DeltaR = sqrt(pow(jets[0]->vector().Phi()- genjets[i]->vector().Phi(),2) + pow(jets[0]->vector().Rapidity()-genjets[i]->vector().Rapidity(),2));
              double PtDiff = std::fabs(jets[0]->vector().Pt() - genjets[i]->vector().Pt());
          
              if(DeltaR < 0.4 && PtDiff < PtDiffMin1 && std::fabs(genjets[i]->vector().Rapidity()) < 5){
                  index1 = i;
                  PtDiffMin1 = PtDiff;
              }
          }
      
          for(unsigned i=0; i< n_genjets_; i++){
              double DeltaR = sqrt(pow(jets[1]->vector().Phi()- genjets[i]->vector().Phi(),2) + pow(jets[1]->vector().Rapidity()-genjets[i]->vector().Rapidity(),2));
              double PtDiff = std::fabs(jets[1]->vector().Pt() - genjets[i]->vector().Pt());
          
              if(DeltaR < 0.4 && PtDiff < PtDiffMin2 && std::fabs(genjets[i]->vector().Rapidity()) < 5){
                  index2 = i;
                  PtDiffMin2 = PtDiff;
              }
          }

          h_jets_OfflineIndexGenMatch_1->Fill(index1,index2);
      
      }
      
      if(n_jets_ > 1) if(jets[0]->vector().Pt() > 20 && jets[1]->vector().Pt() > 20){
          
          double PtDiffMin1 = 10000;
          double PtDiffMin2 = 10000;
          int index1 = -1;
          int index2 = -1;
      
          for(unsigned i=0; i< n_l1jets_; i++){
              double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- jets[0]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-jets[0]->vector().Rapidity(),2));
              double PtDiff = std::fabs(l1jets[i]->vector().Pt() - jets[0]->vector().Pt());
          
              if(DeltaR < 0.4 && PtDiff < PtDiffMin1){
                  index1 = i;
                  PtDiffMin1 = PtDiff;
              }
          }
      
          h_lead_l1Index_1->Fill(index1);
      
          for(unsigned i=0; i< n_l1jets_; i++){
              double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- jets[1]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-jets[1]->vector().Rapidity(),2));
              double PtDiff = std::fabs(l1jets[i]->vector().Pt() - jets[1]->vector().Pt());
          
              if(DeltaR < 0.4 && PtDiff < PtDiffMin2){
                  index2 = i;
                  PtDiffMin2 = PtDiff;
              }
          }
      
          h_sublead_l1Index_1->Fill(index2);
          h_jets_l1Index_1->Fill(index1,index2);
      
      }

      
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
      
      if(n_genjets_ > 1) if(genjets[0]->vector().Pt() > 20 && genjets[1]->vector().Pt() > 20){
          
          double PtDiffMin1 = 10000;
          double PtDiffMin2 = 10000;
          int index1 = -1;
          int index2 = -1;
      
          for(unsigned i=0; i< n_l1jets_; i++){
              double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- genjets[0]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-genjets[0]->vector().Rapidity(),2));
              double PtDiff = std::fabs(l1jets[i]->vector().Pt() - genjets[0]->vector().Pt());
          
              if(DeltaR < 0.4 && PtDiff < PtDiffMin1){
                  index1 = i;
                  PtDiffMin1 = PtDiff;
              }
          }
      
          for(unsigned i=0; i< n_l1jets_; i++){
              double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- genjets[1]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-genjets[1]->vector().Rapidity(),2));
              double PtDiff = std::fabs(l1jets[i]->vector().Pt() - genjets[1]->vector().Pt());
          
              if(DeltaR < 0.4 && PtDiff < PtDiffMin2){
                  index2 = i;
                  PtDiffMin2 = PtDiff;
              }
          }

          h_jets_l1IndexGenMatch_2->Fill(index1,index2);
      
      }
      
      if(n_jets_ > 1) if(jets[0]->vector().Pt() > 20 && jets[1]->vector().Pt() > 20){
          
          double PtDiffMin1 = 10000;
          double PtDiffMin2 = 10000;
          int index1 = -1;
          int index2 = -1;
      
          for(unsigned i=0; i< n_l1jets_; i++){
              double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- jets[0]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-jets[0]->vector().Rapidity(),2));
              double PtDiff = std::fabs(l1jets[i]->vector().Pt() - jets[0]->vector().Pt());
          
              if(DeltaR < 0.4 && PtDiff < PtDiffMin1){
                  index1 = i;
                  PtDiffMin1 = PtDiff;
              }
          }
      
          h_lead_l1Index_2->Fill(index1);
      
          for(unsigned i=0; i< n_l1jets_; i++){
              double DeltaR = sqrt(pow(l1jets[i]->vector().Phi()- jets[1]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-jets[1]->vector().Rapidity(),2));
              double PtDiff = std::fabs(l1jets[i]->vector().Pt() - jets[1]->vector().Pt());
          
              if(DeltaR < 0.4 && PtDiff < PtDiffMin2){
                  index2 = i;
                  PtDiffMin2 = PtDiff;
              }
          }
      
          h_sublead_l1Index_2->Fill(index2);
          h_jets_l1Index_2->Fill(index1,index2);
      
      }
          
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
      
      if(Jet1PtCut > 0){
          if(n_l1jets_ <1) Filter = true;
          if(n_l1jets_ >1) if(l1jets[0]->vector().Pt() < Jet1PtCut) Filter = true;
      }
      if(!Filter) h_SignalEfficiency->Fill(3);
      
      if(Jet2PtCut > 0){
          if(n_l1jets_ <2) Filter = true;
          if(n_l1jets_ >1) if(l1jets[1]->vector().Pt() < Jet2PtCut) Filter = true;
      }
      if(!Filter) h_SignalEfficiency->Fill(4);
      
      bool dontFilter = false;
         
      if(n_l1jets_>1){

      bool OneJetsMatched = false;
      bool TwoJetsMatched = false;
      int njetsMatched = 0;
      double LargestMjj =0;
      int MjjIndex[2]= {-1,-1};
      int MjjMultiplicity = 0;
       
      for(unsigned i=0; i< n_l1jets_; i++){
    
          for(unsigned j=i+1; j< n_l1jets_; j++){
              
              njetsMatched = 0;
          
              double Mjj = (l1jets[i]->vector() + l1jets[j]->vector()).M();
              double DeltaEta = std::fabs(l1jets[i]->vector().Rapidity() - l1jets[j]->vector().Rapidity());
              
              if(Mjj >=MjjCut && DeltaEta >= DeltaEtaCut && ((l1jets[i]->vector().Pt() >= Jet2PtCut && l1jets[j]->vector().Pt()>=Jet1PtCut) || (l1jets[j]->vector().Pt() >= Jet2PtCut && l1jets[i]->vector().Pt()>=Jet1PtCut))){
                  dontFilter = true;
                  MjjMultiplicity++;
                  
                  if(Mjj > LargestMjj){
                      LargestMjj = Mjj;
                      MjjIndex[0] = i;
                      MjjIndex[1] = j;
                  }
                  
                  for(unsigned k=0; k < n_genjets_ && k < 2; k++){
                      double DeltaR = sqrt(pow(genjets[k]->vector().Phi()- l1jets[i]->vector().Phi(),2) + pow(genjets[k]->vector().Rapidity()-l1jets[i]->vector().Rapidity(),2));    
                      if(DeltaR < 0.5){
                          njetsMatched++;
                          break;
                      }
                  }
                  
                  for(unsigned k=0; k < n_genjets_ && k < 2; k++){
                      double DeltaR = sqrt(pow(genjets[k]->vector().Phi()- l1jets[j]->vector().Phi(),2) + pow(genjets[k]->vector().Rapidity()-l1jets[j]->vector().Rapidity(),2));    
                      if(DeltaR < 0.5){
                          njetsMatched++;
                          break;
                      }
                  }
              }
              
              if(njetsMatched == 2) TwoJetsMatched = true;
              else if(njetsMatched == 1) OneJetsMatched = true;
          }
      }
      
      h_VBFMultiplicity->Fill(MjjMultiplicity);
      
      
      if(dontFilter){
          h_CorrectVBFJets->Fill(0);
          if(TwoJetsMatched) h_CorrectVBFJets->Fill(3);
          else if(OneJetsMatched) h_CorrectVBFJets->Fill(2);
          else h_CorrectVBFJets->Fill(1);
          
          int njetsMatched1 = 0;
          int njetsMatched2 = 0;
          
          for(unsigned k=0; k < n_genjets_ && k < 2; k++){
              double DeltaR = sqrt(pow(genjets[k]->vector().Phi()- l1jets[MjjIndex[0]]->vector().Phi(),2) + pow(genjets[k]->vector().Rapidity()-l1jets[MjjIndex[0]]->vector().Rapidity(),2));    
              if(DeltaR < 0.5){
                  njetsMatched1++;
                  break;
              }
          }
          
          for(unsigned k=0; k < n_genjets_ && k < 2; k++){
              double DeltaR = sqrt(pow(genjets[k]->vector().Phi()- l1jets[MjjIndex[1]]->vector().Phi(),2) + pow(genjets[k]->vector().Rapidity()-l1jets[MjjIndex[1]]->vector().Rapidity(),2));    
              if(DeltaR < 0.5){
                  njetsMatched2++;
              break;
              }
          }
          
          if(njetsMatched1 > 0 && njetsMatched2 > 0) h_CorrectVBFJets->Fill(4);
      }
  
      if(!dontFilter) Filter = true;
      if(!Filter) h_SignalEfficiency->Fill(5);
      
      if(n_l1jets_ <3) Filter = true;
 
      if(n_l1jets_>2){
          if(l1jets[0]->vector().Pt() < Jet2PtCut || l1jets[1]->vector().Pt() < 1.5*Tau1PtCut || l1jets[2]->vector().Pt() < Jet1PtCut ) Filter = true;
          if(!Filter) h_SignalEfficiency->Fill(6);
      }
      
      if(n_l1jets_ <4) Filter = true;
 
      if(n_l1jets_>3){
          if(l1jets[0]->vector().Pt() < Jet2PtCut || l1jets[1]->vector().Pt() < 1.5*Tau1PtCut || l1jets[2]->vector().Pt() < 1.5*Tau1PtCut || l1jets[3]->vector().Pt() < Jet1PtCut) Filter = true;
          if(!Filter) h_SignalEfficiency->Fill(7);
      }
      
  }
 
 
 h_L1Filter_Efficiency->Fill(0);
 if(!Filter) h_L1Filter_Efficiency->Fill(1);
 
 if(CancelFilter) Filter = false;
 
 if(Filter) return 1;
 else return 0;
      
  }
  
  int L1TFilterPlots::PostAnalysis() {
    return 0;
  }

  void L1TFilterPlots::PrintInfo() {
    ;
  }
}

