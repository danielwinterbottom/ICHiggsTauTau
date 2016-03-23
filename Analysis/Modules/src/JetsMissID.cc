#include "Modules/interface/JetsMissID.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "TVector3.h"

struct greater_Candidate{
  bool operator() (const ic::Candidate *a,const ic::Candidate *b) {
    return (a->pt() > b->pt());
  }
};

namespace ic {

  
  JetsMissID::JetsMissID(std::string const& name, 
                                         std::string output_name, std::string output_folder) : ModuleBase(name) {
    
    genParticles_label_ = "genParticles";
    jets_label_ = "ak4PFJetsCHS"; 
    electrons_label_ = "electrons";
    muons_label_ = "muons"; 
    taus_label_ = "taus";  
    met_label_ = "pfMet";
    l1jets_label_ = "L1Jets"; 
    l1electrons_label_ = "L1EGammas";
    l1muons_label_ = "L1Muons"; 
    l1taus_label_ = "L1Taus";  
    l1met_label_ = "L1Sums";
    genjets_label_ = "genJets";
    met_label_ = "pfMet";
    output_name_ = output_name;
    output_folder_ = output_folder;
    

    fEff = new TFile((output_name_).c_str(),"RECREATE");

    h_jet1_tt = new TH1D("h_jet1_tt","h_jet1_tt",3, 0,3); h_jet1_tt->SetDirectory(fEff);
    h_jet1_tt->GetXaxis()->SetTitle("");
    h_jet1_tt->GetYaxis()->SetTitle("%");
    h_jet1_tt->GetXaxis()->SetBinLabel(1,"2 Jets Matched");
    h_jet1_tt->GetXaxis()->SetBinLabel(2,"1 Jets Matched");
    h_jet1_tt->GetXaxis()->SetBinLabel(3," 0 Jets Matched");
    
    h_jet1_mt = new TH1D("h_jet1_mt","h_jet1_mt",3, 0,3); h_jet1_mt->SetDirectory(fEff);
    h_jet1_mt->GetXaxis()->SetTitle("");
    h_jet1_mt->GetYaxis()->SetTitle("%");
    h_jet1_mt->GetXaxis()->SetBinLabel(1,"2 Jets Matched");
    h_jet1_mt->GetXaxis()->SetBinLabel(2,"1 Jets Matched");
    h_jet1_mt->GetXaxis()->SetBinLabel(3,"0 Jets Matched");
    
    h_jet1_et = new TH1D("h_jet1_et","h_jet1_et",3, 0,3); h_jet1_et->SetDirectory(fEff);
    h_jet1_et->GetXaxis()->SetTitle("");
    h_jet1_et->GetYaxis()->SetTitle("%");
    h_jet1_et->GetXaxis()->SetBinLabel(1,"2 Jets Matched");
    h_jet1_et->GetXaxis()->SetBinLabel(2,"1 Jets Matched");
    h_jet1_et->GetXaxis()->SetBinLabel(3,"0 Jets Matched");

    
    h_jet1_em = new TH1D("h_jet1_em","h_jet1_em",3, 0,3); h_jet1_em->SetDirectory(fEff);
    h_jet1_em->GetXaxis()->SetTitle("");
    h_jet1_em->GetYaxis()->SetTitle("%");
    h_jet1_em->GetXaxis()->SetBinLabel(1,"2 Jets Matched");
    h_jet1_em->GetXaxis()->SetBinLabel(2,"1 Jets Matched");
    h_jet1_em->GetXaxis()->SetBinLabel(3,"0 Jets Matched");
    
    h_DeltaEta_tt = new TH1D("h_DeltaEta_tt","h_DeltaEta_tt",3, 0,3); h_DeltaEta_tt->SetDirectory(fEff);
    h_DeltaEta_tt->GetXaxis()->SetTitle("");
    h_DeltaEta_tt->GetYaxis()->SetTitle("%");
    h_DeltaEta_tt->GetXaxis()->SetBinLabel(1,"2 Jets Matched");
    h_DeltaEta_tt->GetXaxis()->SetBinLabel(2,"1 Jets Matched");
    h_DeltaEta_tt->GetXaxis()->SetBinLabel(3,"0 Jets Matched");
    
    h_DeltaEta_et = new TH1D("h_DeltaEta_et","h_DeltaEta_et",3, 0,3); h_DeltaEta_et->SetDirectory(fEff);
    h_DeltaEta_et->GetXaxis()->SetTitle("");
    h_DeltaEta_et->GetYaxis()->SetTitle("%");
    h_DeltaEta_et->GetXaxis()->SetBinLabel(1,"2 Jets Matched");
    h_DeltaEta_et->GetXaxis()->SetBinLabel(2,"1 Jets Matched");
    h_DeltaEta_et->GetXaxis()->SetBinLabel(3,"0 Jets Matched");
    
    h_DeltaEta_mt = new TH1D("h_DeltaEta_mt","h_DeltaEta_mt",3, 0,3); h_DeltaEta_mt->SetDirectory(fEff);
    h_DeltaEta_mt->GetXaxis()->SetTitle("");
    h_DeltaEta_mt->GetYaxis()->SetTitle("%");
    h_DeltaEta_mt->GetXaxis()->SetBinLabel(1,"2 Jets Matched");
    h_DeltaEta_mt->GetXaxis()->SetBinLabel(2,"1 Jets Matched");
    h_DeltaEta_mt->GetXaxis()->SetBinLabel(3,"0 Jets Matched");

    h_DeltaEta_em = new TH1D("h_DeltaEta_em","h_DeltaEta_em",3, 0,3); h_DeltaEta_em->SetDirectory(fEff);
    h_DeltaEta_em->GetXaxis()->SetTitle("");
    h_DeltaEta_em->GetYaxis()->SetTitle("%");
    h_DeltaEta_em->GetXaxis()->SetBinLabel(1,"2 Jets Matched");
    h_DeltaEta_em->GetXaxis()->SetBinLabel(2,"1 Jets Matched");
    h_DeltaEta_em->GetXaxis()->SetBinLabel(3,"0 Jets Matched");
    
    h_Mjj_tt = new TH1D("h_Mjj_tt","h_Mjj_tt",3, 0,3); h_Mjj_tt->SetDirectory(fEff);
    h_Mjj_tt->GetXaxis()->SetTitle("");
    h_Mjj_tt->GetYaxis()->SetTitle("%");
    h_Mjj_tt->GetXaxis()->SetBinLabel(1,"2 Jets Matched");
    h_Mjj_tt->GetXaxis()->SetBinLabel(2,"1 Jets Matched");
    h_Mjj_tt->GetXaxis()->SetBinLabel(3,"0 Jets Matched");
    
    h_Mjj_et = new TH1D("h_Mjj_et","h_Mjj_et",3, 0,3); h_Mjj_et->SetDirectory(fEff);
    h_Mjj_et->GetXaxis()->SetTitle("");
    h_Mjj_et->GetYaxis()->SetTitle("%");
    h_Mjj_et->GetXaxis()->SetBinLabel(1,"2 Jets Matched");
    h_Mjj_et->GetXaxis()->SetBinLabel(2,"1 Jets Matched");
    h_Mjj_et->GetXaxis()->SetBinLabel(3,"0 Jets Matched");
    
    h_Mjj_mt = new TH1D("h_Mjj_mt","h_Mjj_mt",3, 0,3); h_Mjj_mt->SetDirectory(fEff);
    h_Mjj_mt->GetXaxis()->SetTitle("");
    h_Mjj_mt->GetYaxis()->SetTitle("%");
    h_Mjj_mt->GetXaxis()->SetBinLabel(1,"2 Jets Matched");
    h_Mjj_mt->GetXaxis()->SetBinLabel(2,"1 Jets Matched");
    h_Mjj_mt->GetXaxis()->SetBinLabel(3,"0 Jets Matched");
    
    h_Mjj_em = new TH1D("h_Mjj_em","h_Mjj_em",3, 0,3); h_Mjj_em->SetDirectory(fEff);
    h_Mjj_em->GetXaxis()->SetTitle("");
    h_Mjj_em->GetYaxis()->SetTitle("%");
    h_Mjj_em->GetXaxis()->SetBinLabel(1,"2 Jets Matched");
    h_Mjj_em->GetXaxis()->SetBinLabel(2,"1 Jets Matched");
    h_Mjj_em->GetXaxis()->SetBinLabel(3,"0 Jets Matched");
    
  }

    JetsMissID::~JetsMissID(){
      
      
      h_Mjj_em->Scale(100/h_Mjj_em->GetEntries());
      h_Mjj_et->Scale(100/h_Mjj_et->GetEntries());
      h_Mjj_mt->Scale(100/h_Mjj_mt->GetEntries());
      h_Mjj_tt->Scale(100/h_Mjj_tt->GetEntries());
      h_DeltaEta_em->Scale(100/h_DeltaEta_em->GetEntries());
      h_DeltaEta_et->Scale(100/h_DeltaEta_et->GetEntries());
      h_DeltaEta_mt->Scale(100/h_DeltaEta_mt->GetEntries());
      h_DeltaEta_tt->Scale(100/h_DeltaEta_tt->GetEntries());
      h_jet1_em->Scale(100/h_jet1_em->GetEntries());
      h_jet1_et->Scale(100/h_jet1_et->GetEntries());
      h_jet1_mt->Scale(100/h_jet1_mt->GetEntries());
      h_jet1_tt->Scale(100/h_jet1_tt->GetEntries());
      
      fEff->Write();
      fEff->Close();

  }
  
  int JetsMissID::PreAnalysis(){

      return 0;

  }

  int JetsMissID::Execute(TreeEvent *event){
      
      std::vector<ic::GenJet*> genjets = event->GetPtrVec<ic::GenJet>(genjets_label_);
      
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
      
      std::vector<ic::GenParticle*> GenParticles = event->GetPtrVec<ic::GenParticle>(genParticles_label_);
      std::vector<PFJet*> const& jets = event->GetPtrVec<PFJet>(jets_label_);
      std::vector<Electron*> electrons = event->GetPtrVec<Electron>(electrons_label_);
      std::vector<Muon*> muons = event->GetPtrVec<Muon>(muons_label_);
      std::vector<Tau*> taus = event->GetPtrVec<Tau>(taus_label_);
      std::vector<Met*> met_vec = event->GetPtrVec<Met>(met_label_);

      n_jets_ = jets.size();
      n_electrons_ = electrons.size();
      n_muons_ = muons.size();
      n_taus_ = taus.size();
      n_genParticles_ = GenParticles.size();
      n_genJets_ = genjets.size();


      
      unsigned nHadTaus = 0;
      unsigned nElectrons = 0;
      unsigned nMuons = 0;
      for(unsigned j=0; j< n_genParticles_; j++){

          int genID = std::fabs(GenParticles[j]->pdgid());
              
              if(genID == 15){
              
              for(unsigned k=0; k < GenParticles[j]->mothers().size(); k++) {
                  if(std::fabs(GenParticles[GenParticles[j]->mothers().at(k)]->pdgid()) == 25){
                      bool isMuon = false;
                      bool isElectron = false;
            
                      for(unsigned l=0; l < GenParticles[j]->daughters().size(); l++) {
                          if(std::fabs(GenParticles[GenParticles[j]->daughters().at(l)]->pdgid()) == 11) isElectron = true;
                          if(std::fabs(GenParticles[GenParticles[j]->daughters().at(l)]->pdgid()) == 13) isMuon = true;
                      }
                      if(isElectron) nElectrons++;
                      else if(isMuon) nMuons++;
                      else nHadTaus++;
                  }
              }
          }
      }
      
      double LargestMjj = 0;
      double LargestDeltaEta = 0;
      int Mjjjet1Index = -1;
      int Mjjjet2Index = -1;
      int DeltaEtajet1Index = -1;
      int DeltaEtajet2Index = -1;
      
      for(unsigned i=0; i<n_l1jets_; i++){
          
          for(unsigned j=0; j <n_l1jets_; j++){
              
              if(i!=j){
                  
                  double Mjj = (l1jets[i]->vector() + l1jets[j]->vector()).M();
                  if(Mjj > LargestMjj){
                      LargestMjj = Mjj;
                      Mjjjet1Index = i;
                      Mjjjet2Index = j;
                  }
                  double DeltaEta = std::fabs(l1jets[i]->vector().Rapidity() - l1jets[j]->vector().Rapidity());
                  if(DeltaEta > LargestDeltaEta){
                      LargestDeltaEta = DeltaEta;
                      DeltaEtajet1Index = i;
                      DeltaEtajet2Index = j;
                  }
              }
              
          }
      }
      
      if(n_l1jets_ > 1){
          
          bool jetMatched1 = false;
          bool jetMatched2 = false;
          
          int jetsIndex[2] = {-1, -1};
          
          bool jetProceed = false;
          unsigned jetsCount = 0;
          
           
          for(unsigned i=0; i<n_jets_; i++){
              
              if(std::fabs(jets[i]->vector().Rapidity()) < 5) jetProceed = true;
              
              for(unsigned j=0; j<n_taus_; j++){
                  double DeltaR = sqrt(pow(jets[i]->vector().Phi()- taus[j]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-taus[j]->vector().Rapidity(),2));
                  if(DeltaR < 0.1) jetProceed = false; 
              }
              for(unsigned j=0; j<n_electrons_; j++){
                  double DeltaR = sqrt(pow(jets[i]->vector().Phi()- electrons[j]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-electrons[j]->vector().Rapidity(),2));
                  if(DeltaR < 0.1) jetProceed = false; 
              }
              for(unsigned j=0; j<n_muons_; j++){
                  double DeltaR = sqrt(pow(jets[i]->vector().Phi()- muons[j]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-muons[j]->vector().Rapidity(),2));
                  if(DeltaR < 0.1) jetProceed = false; 
              }
              
              if(jetsCount < 2 && jetProceed){
                  jetsIndex[jetsCount] = i;
                  jetsCount++;
              }
            
          }
          
          if(jetsCount >1){
              
              
              for(unsigned i=0; i< n_l1jets_ && i < 2; i++){
          
                  double DeltaR = sqrt(pow(jets[jetsIndex[0]]->vector().Phi()- l1jets[i]->vector().Phi(),2) + pow(jets[jetsIndex[0]]->vector().Rapidity()-l1jets[i]->vector().Rapidity(),2));
                  if(DeltaR < 0.4){
                      jetMatched1 = true;
                  }
                  DeltaR = sqrt(pow(jets[jetsIndex[1]]->vector().Phi()- l1jets[i]->vector().Phi(),2) + pow(jets[jetsIndex[1]]->vector().Rapidity()-l1jets[i]->vector().Rapidity(),2));
                  if(DeltaR < 0.4){
                      jetMatched2 = true;
                  }
              }
              
              if(jetMatched1 && jetMatched2){
                  if(nElectrons == 1 && nMuons == 1)   h_jet1_em->Fill(0);
                  if(nElectrons == 1 && nHadTaus == 1) h_jet1_et->Fill(0); 
                  if(nHadTaus == 1 && nMuons == 1)     h_jet1_mt->Fill(0); 
                  if(nHadTaus == 2)                    h_jet1_tt->Fill(0); 
              }
              else if((jetMatched1 && !jetMatched2) || (jetMatched2 && !jetMatched1)){
                  if(nElectrons == 1 && nMuons == 1)   h_jet1_em->Fill(1);
                  if(nElectrons == 1 && nHadTaus == 1) h_jet1_et->Fill(1); 
                  if(nHadTaus == 1 && nMuons == 1)     h_jet1_mt->Fill(1); 
                  if(nHadTaus == 2)                    h_jet1_tt->Fill(1); 
              }
              else if(!jetMatched1 && !jetMatched2){
                  if(nElectrons == 1 && nMuons == 1)   h_jet1_em->Fill(2);
                  if(nElectrons == 1 && nHadTaus == 1) h_jet1_et->Fill(2); 
                  if(nHadTaus == 1 && nMuons == 1)     h_jet1_mt->Fill(2); 
                  if(nHadTaus == 2)                    h_jet1_tt->Fill(2); 
              }
              
              bool MjjjetMatched1 = false;
              bool MjjjetMatched2 = false;
              
              for(unsigned i=0; i < 2; i++){
          
                  double DeltaR = sqrt(pow(jets[jetsIndex[i]]->vector().Phi()- l1jets[Mjjjet1Index]->vector().Phi(),2) + pow(jets[jetsIndex[i]]->vector().Rapidity()-l1jets[Mjjjet1Index]->vector().Rapidity(),2));
                  if(DeltaR < 0.4){
                      if(i==0) MjjjetMatched1 = true;
                      if(i==1) MjjjetMatched2 = true;
                  }
                  DeltaR = sqrt(pow(jets[jetsIndex[i]]->vector().Phi()- l1jets[Mjjjet2Index]->vector().Phi(),2) + pow(jets[jetsIndex[i]]->vector().Rapidity()-l1jets[Mjjjet2Index]->vector().Rapidity(),2));
                  if(DeltaR < 0.4){
                      if(i==0) MjjjetMatched1 = true;
                      if(i==1) MjjjetMatched2 = true;
                  }
              }
              
              if(MjjjetMatched1 && MjjjetMatched2){
                  if(nElectrons == 1 && nMuons == 1)   h_Mjj_em->Fill(0);
                  if(nElectrons == 1 && nHadTaus == 1) h_Mjj_et->Fill(0); 
                  if(nHadTaus == 1 && nMuons == 1)     h_Mjj_mt->Fill(0); 
                  if(nHadTaus == 2)                    h_Mjj_tt->Fill(0);
              }
              else if((MjjjetMatched1 && !MjjjetMatched2) || (MjjjetMatched2 && !MjjjetMatched1)){
                  if(nElectrons == 1 && nMuons == 1)   h_Mjj_em->Fill(1);
                  if(nElectrons == 1 && nHadTaus == 1) h_Mjj_et->Fill(1); 
                  if(nHadTaus == 1 && nMuons == 1)     h_Mjj_mt->Fill(1); 
                  if(nHadTaus == 2)                    h_Mjj_tt->Fill(1);
              }
              else if(!MjjjetMatched1 && !MjjjetMatched2){
                  if(nElectrons == 1 && nMuons == 1)   h_Mjj_em->Fill(2);
                  if(nElectrons == 1 && nHadTaus == 1) h_Mjj_et->Fill(2); 
                  if(nHadTaus == 1 && nMuons == 1)     h_Mjj_mt->Fill(2); 
                  if(nHadTaus == 2)                    h_Mjj_tt->Fill(2);
              }
              
              bool DeltaEtajetMatched1 = false;
              bool DeltaEtajetMatched2 = false;
              
              for(unsigned i=0; i < 2; i++){
          
                  double DeltaR = sqrt(pow(jets[jetsIndex[i]]->vector().Phi()- l1jets[DeltaEtajet1Index]->vector().Phi(),2) + pow(jets[jetsIndex[i]]->vector().Rapidity()-l1jets[DeltaEtajet1Index]->vector().Rapidity(),2));
                  if(DeltaR < 0.4){
                      if(i==0) DeltaEtajetMatched1 = true;
                      if(i==1) DeltaEtajetMatched2 = true;
                  }
                  DeltaR = sqrt(pow(jets[jetsIndex[i]]->vector().Phi()- l1jets[DeltaEtajet2Index]->vector().Phi(),2) + pow(jets[jetsIndex[i]]->vector().Rapidity()-l1jets[DeltaEtajet2Index]->vector().Rapidity(),2));
                  if(DeltaR < 0.4){
                      if(i==0) DeltaEtajetMatched1 = true;
                      if(i==1) DeltaEtajetMatched2 = true;
                  }
              }
              
              if(DeltaEtajetMatched1 && DeltaEtajetMatched2){
                  if(nElectrons == 1 && nMuons == 1)   h_DeltaEta_em->Fill(0);
                  if(nElectrons == 1 && nHadTaus == 1) h_DeltaEta_et->Fill(0); 
                  if(nHadTaus == 1 && nMuons == 1)     h_DeltaEta_mt->Fill(0); 
                  if(nHadTaus == 2)                    h_DeltaEta_tt->Fill(0);
              }
              else if((DeltaEtajetMatched1 && !DeltaEtajetMatched2) || (DeltaEtajetMatched2 && !DeltaEtajetMatched1)){
                  if(nElectrons == 1 && nMuons == 1)   h_DeltaEta_em->Fill(1);
                  if(nElectrons == 1 && nHadTaus == 1) h_DeltaEta_et->Fill(1); 
                  if(nHadTaus == 1 && nMuons == 1)     h_DeltaEta_mt->Fill(1); 
                  if(nHadTaus == 2)                    h_DeltaEta_tt->Fill(1);
              }
              else if(!DeltaEtajetMatched1 && !DeltaEtajetMatched2){
                  if(nElectrons == 1 && nMuons == 1)   h_DeltaEta_em->Fill(2);
                  if(nElectrons == 1 && nHadTaus == 1) h_DeltaEta_et->Fill(2); 
                  if(nHadTaus == 1 && nMuons == 1)     h_DeltaEta_mt->Fill(2); 
                  if(nHadTaus == 2)                    h_DeltaEta_tt->Fill(2);
              }
              
          }
              
      }

              
      return 0;
  }

  int JetsMissID::PostAnalysis(){
    return 0;
  }

  void JetsMissID::PrintInfo(){
    ;
  }
  



}//namespace


