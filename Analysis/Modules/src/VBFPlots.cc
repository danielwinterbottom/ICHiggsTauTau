#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/VBFPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "TVector3.h"

struct greater_Candidate{
  bool operator() (const ic::Candidate *a,const ic::Candidate *b) {
    return (a->pt() > b->pt());
  }
};

namespace ic {

  VBFPlots::VBFPlots(std::string const& name, fwlite::TFileService *fs, std::string output_name) : ModuleBase(name) {
      
    TFileDirectory subDir = fs->mkdir(output_name.c_str());
      
    h_qq_DeltaEta = subDir.make<TH1D>("h_qq_DeltaEta","h_qq_DeltaEta",100, 0,10);
    h_qq_DeltaEta->GetXaxis()->SetTitle("|#Delta#eta_{qq}|");
    h_qq_DeltaEta->GetYaxis()->SetTitle("Entries");
    
    h_qq_Mqq = subDir.make<TH1D>("h_qq_Mqq","h_qq_Mqq",100, 0,5000); 
    h_qq_Mqq->GetXaxis()->SetTitle("m_{qq} [GeV]");
    h_qq_Mqq->GetYaxis()->SetTitle("Entries");

    h_q1_Pt = subDir.make<TH1D>("h_q1_Pt","h_q1_Pt",150, 0,300); 
    h_q1_Pt->GetXaxis()->SetTitle("Leading quark p_{T} [GeV]");
    h_q1_Pt->GetYaxis()->SetTitle("Entries");
    
    h_q2_Pt = subDir.make<TH1D>("h_q2_Pt","h_q2_Pt",150, 0,300); 
    h_q2_Pt->GetXaxis()->SetTitle("Sub-leading quark p_{T} [GeV]");
    h_q2_Pt->GetYaxis()->SetTitle("Entries");
    
    h_genjj_DeltaEta = subDir.make<TH1D>("h_genjj_DeltaEta","h_genjj_DeltaEta",100, 0,10);
    h_genjj_DeltaEta->GetXaxis()->SetTitle("|#Delta#eta_{jj}|");
    h_genjj_DeltaEta->GetYaxis()->SetTitle("Entries");
    
    h_genjj_Mqq = subDir.make<TH1D>("h_genjj_Mqq","h_genjj_Mqq",200, 0,5000); 
    h_genjj_Mqq->GetXaxis()->SetTitle("m_{jj} [GeV]");
    h_genjj_Mqq->GetYaxis()->SetTitle("Entries");

    h_genj1_Pt = subDir.make<TH1D>("h_genj1_Pt","h_genj1_Pt",150, 0,300); 
    h_genj1_Pt->GetXaxis()->SetTitle("Leading jet p_{T} [GeV]");
    h_genj1_Pt->GetYaxis()->SetTitle("Entries");
    
    h_genj2_Pt = subDir.make<TH1D>("h_genj2_Pt","h_genj2_Pt",150, 0,300); 
    h_genj2_Pt->GetXaxis()->SetTitle("Sub-leading jet p_{T} [GeV]");
    h_genj2_Pt->GetYaxis()->SetTitle("Entries");
    
    h_jj_Mqq = subDir.make<TH1D>("h_jj_Mjj","h_jj_Mjj",100, 0,5000); 
    h_jj_Mqq->GetXaxis()->SetTitle("m_{jj} [GeV]");
    h_jj_Mqq->GetYaxis()->SetTitle("Entries");
    
    h_jj_DeltaEta = subDir.make<TH1D>("h_jj_DeltaEta","h_jj_DeltaEta",100, 0,10);
    h_jj_DeltaEta->GetXaxis()->SetTitle("|#Delta#eta_{jj}|");
    h_jj_DeltaEta->GetYaxis()->SetTitle("Entries");

    h_j1_Pt = subDir.make<TH1D>("h_j1_Pt","h_j1_Pt",150, 0,300); 
    h_j1_Pt->GetXaxis()->SetTitle("Leading jet p_{T} [GeV]");
    h_j1_Pt->GetYaxis()->SetTitle("Entries");
    
    h_j2_Pt = subDir.make<TH1D>("h_j2_Pt","h_j2_Pt",150, 0,300); 
    h_j2_Pt->GetXaxis()->SetTitle("Sub-leading jet p_{T} [GeV]");
    h_j2_Pt->GetYaxis()->SetTitle("Entries");
    
    h_l1jj_Mqq = subDir.make<TH1D>("h_l1jj_l1Mjj","h_l1M_Mjj",100, 0,5000); 
    h_l1jj_Mqq->GetXaxis()->SetTitle("m_{jj} [GeV]");
    h_l1jj_Mqq->GetYaxis()->SetTitle("Entries");
    
    h_l1jj_DeltaEta = subDir.make<TH1D>("h_l1jj_DeltaEta","h_l1jj_DeltaEta",100, 0,10);
    h_l1jj_DeltaEta->GetXaxis()->SetTitle("|#Delta#eta_{jj}|");
    h_l1jj_DeltaEta->GetYaxis()->SetTitle("Entries");

    h_l1j1_Pt = subDir.make<TH1D>("h_l1j1_Pt","h_l1j1_Pt",150, 0,300); 
    h_l1j1_Pt->GetXaxis()->SetTitle("Leading jet p_{T} [GeV]");
    h_l1j1_Pt->GetYaxis()->SetTitle("Entries");
    
    h_l1j2_Pt = subDir.make<TH1D>("h_l1j2_Pt","h_l1j2_Pt",150, 0,300); 
    h_l1j2_Pt->GetXaxis()->SetTitle("Sub-leading jet p_{T} [GeV]");
    h_l1j2_Pt->GetYaxis()->SetTitle("Entries");
    
    EventCount=0;
      
  n2quarks = 0;
  ngluons = 0;

  }

  VBFPlots::~VBFPlots() {
    ;
  }

  int VBFPlots::PreAnalysis() {

    return 0;
  }

  int VBFPlots::Execute(TreeEvent *event) {
      
  std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("ak4PFJetsCHS");
  std::vector<ic::GenJet*> genjets = event->GetPtrVec<ic::GenJet>("genJets");
  std::vector<ic::GenParticle*> GenParticles = event->GetPtrVec<ic::GenParticle>("genParticles");
  std::vector<ic::L1TJet*>  l1jets = event->GetPtrVec<ic::L1TJet>("L1Jets");
  
  std::sort(l1jets.begin(),l1jets.end(),greater_Candidate());
   
  n_jets_ = jets.size();
  n_genjets_ = genjets.size();
  n_genParticles_ = GenParticles.size();
  n_l1jets_ = l1jets.size();

  EventCount++;
  
  if(std::fabs(GenParticles[5]->pdgid()) < 7 && std::fabs(GenParticles[6]->pdgid()) < 7) n2quarks++;
  if(std::fabs(GenParticles[7]->pdgid()) == 21) ngluons++; 

  if(GenParticles.size() > 6){
      
      double Mqq = (GenParticles[5]->vector() + GenParticles[6]->vector()).M();
      double DeltaEta = std::fabs(GenParticles[5]->vector().Rapidity() - GenParticles[6]->vector().Rapidity());
      double Pt1 = GenParticles[5]->vector().Pt();
      double Pt2 = GenParticles[6]->vector().Pt();
      if(Pt1 < Pt2){
          double PtTemp = Pt1;
          Pt1 = Pt2;
          Pt2 = PtTemp;
      }
      
      h_q1_Pt->Fill(Pt1);
      h_q2_Pt->Fill(Pt2);
      h_qq_DeltaEta->Fill(DeltaEta);
      h_qq_Mqq->Fill(Mqq);
  }
  
  double DeltaRMin1 = 100000;
  double DeltaRMin2 = 100000;
  int genjetIndex1 = -1;
  int genjetIndex2 = -1;
  
  for(unsigned i=0; i< n_genjets_; i++){
      
      double DeltaR1 = sqrt(pow(genjets[i]->vector().Phi()-GenParticles[5]->vector().Phi(),2) + pow(genjets[i]->vector().Rapidity()-GenParticles[5]->vector().Rapidity(),2));
      if(DeltaR1 < DeltaRMin1 && DeltaR1 < 0.5){
          DeltaRMin1 = DeltaR1;
          genjetIndex1 = i;
      }
      
      double DeltaR2 = sqrt(pow(genjets[i]->vector().Phi()-GenParticles[6]->vector().Phi(),2) + pow(genjets[i]->vector().Rapidity()-GenParticles[6]->vector().Rapidity(),2));
      if(DeltaR2 < DeltaRMin2 && DeltaR2 < 0.5){
          DeltaRMin2 = DeltaR2;
          genjetIndex2 = i;
      }
      
  }
  
  if(n_genjets_ > 1 && genjetIndex1 != -1 && genjetIndex2 != -1 && genjetIndex1 != genjetIndex2){
      
      double Mjj = (genjets[genjetIndex1]->vector() + genjets[genjetIndex2]->vector()).M();
      double DeltaEta = std::fabs(genjets[genjetIndex1]->vector().Rapidity() - genjets[genjetIndex2]->vector().Rapidity());
      double Pt1 = genjets[genjetIndex1]->vector().Pt();
      double Pt2 = genjets[genjetIndex2]->vector().Pt();
      if(Pt1 < Pt2){
          double PtTemp = Pt1;
          Pt1 = Pt2;
          Pt2 = PtTemp;
      }
      
      h_genj1_Pt->Fill(Pt1);
      h_genj2_Pt->Fill(Pt2);
      h_genjj_DeltaEta->Fill(DeltaEta);
      h_genjj_Mqq->Fill(Mjj);
      
  }
  
  DeltaRMin1 = 100000;
  DeltaRMin2 = 100000;
  int jetIndex1 = -1;
  int jetIndex2 = -1;
  
  if(n_jets_ > 1 && genjetIndex1 != -1 && genjetIndex2 != -1){
      
      for(unsigned i=0; i< n_jets_; i++){
          
          double DeltaR1 = sqrt(pow(jets[i]->vector().Phi()-genjets[genjetIndex1]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-genjets[genjetIndex1]->vector().Rapidity(),2));
          if(DeltaR1 < DeltaRMin1 && DeltaR1 < 0.5){
              DeltaRMin1 = DeltaR1;
              jetIndex1 = i;
          }
          
          double DeltaR2 = sqrt(pow(jets[i]->vector().Phi()-genjets[genjetIndex2]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-genjets[genjetIndex2]->vector().Rapidity(),2));
          if(DeltaR2 < DeltaRMin2 && DeltaR2 < 0.5){
              DeltaRMin2 = DeltaR2;
              jetIndex2 = i;
          }
          
      }
      
      if(jetIndex1 != -1 && jetIndex2 != -1 && jetIndex1 != jetIndex2){
          
          double Mjj = (jets[jetIndex1]->vector() + jets[jetIndex2]->vector()).M();
          double DeltaEta = std::fabs(jets[jetIndex1]->vector().Rapidity() - jets[jetIndex2]->vector().Rapidity());
          double Pt1 = jets[jetIndex1]->vector().Pt();
          double Pt2 = jets[jetIndex2]->vector().Pt();
          if(Pt1 < Pt2){
              double PtTemp = Pt1;
              Pt1 = Pt2;
              Pt2 = PtTemp;
          }
          
          h_j1_Pt->Fill(Pt1);
          h_j2_Pt->Fill(Pt2);
          h_jj_DeltaEta->Fill(DeltaEta);
          h_jj_Mqq->Fill(Mjj);
          
      }
  }
  
  DeltaRMin1 = 100000;
  DeltaRMin2 = 100000;
  int l1jetIndex1 = -1;
  int l1jetIndex2 = -1;
  
  if(n_l1jets_ > 1 && genjetIndex1 != -1 && genjetIndex2 != -1){
      
      for(unsigned i=0; i< n_l1jets_; i++){
          
          double DeltaR1 = sqrt(pow(l1jets[i]->vector().Phi()-genjets[genjetIndex1]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-genjets[genjetIndex1]->vector().Rapidity(),2));
          if(DeltaR1 < DeltaRMin1 && DeltaR1 < 0.5){
              DeltaRMin1 = DeltaR1;
              l1jetIndex1 = i;
          }
          
          double DeltaR2 = sqrt(pow(l1jets[i]->vector().Phi()-genjets[genjetIndex2]->vector().Phi(),2) + pow(l1jets[i]->vector().Rapidity()-genjets[genjetIndex2]->vector().Rapidity(),2));
          if(DeltaR2 < DeltaRMin2 && DeltaR2 < 0.5){
              DeltaRMin2 = DeltaR2;
              l1jetIndex2 = i;
          }
          
      }
      
      if(l1jetIndex1 != -1 && l1jetIndex2 != -1 && l1jetIndex1 != l1jetIndex2){
          
          double Mjj = (l1jets[l1jetIndex1]->vector() + l1jets[l1jetIndex2]->vector()).M();
          double DeltaEta = std::fabs(l1jets[l1jetIndex1]->vector().Rapidity() - l1jets[l1jetIndex2]->vector().Rapidity());
          double Pt1 = l1jets[l1jetIndex1]->vector().Pt();
          double Pt2 = l1jets[l1jetIndex2]->vector().Pt();
          if(Pt1 < Pt2){
              double PtTemp = Pt1;
              Pt1 = Pt2;
              Pt2 = PtTemp;
          }
          
          h_l1j1_Pt->Fill(Pt1);
          h_l1j2_Pt->Fill(Pt2);
          h_l1jj_DeltaEta->Fill(DeltaEta);
          h_l1jj_Mqq->Fill(Mjj);
          
      }
  }

  return 0;
      
  }
  
  int VBFPlots::PostAnalysis() {
      
    std::cout << "Total Events " << EventCount << std::endl;
    std::cout << "Total Events with 2 final state quarks" << n2quarks << std::endl;
    std::cout << "Total events with mysterious gluon " << ngluons << std::endl;
    return 0;
  }

  void VBFPlots::PrintInfo() {
    ;
  }
}

