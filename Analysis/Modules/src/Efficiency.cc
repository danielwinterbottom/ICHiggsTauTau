#include "Modules/interface/Efficiency.h"
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

  
  Efficiency::Efficiency(std::string const& name, 
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
    
    h_e_EG_Efficiency = new TH1D("h_e_EG_Efficiency","h_e_EG_Efficiency",100, 0,100); h_e_EG_Efficiency->SetDirectory(fEff);
    h_e_EG_Efficiency->GetXaxis()->SetTitle("Electron p_{T} [GeV]");
    h_e_EG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_e_Total = new TH1D("h_e_Total","h_e_Total",100, 0,100); h_e_Total->SetDirectory(fEff);
    h_e_Total->GetXaxis()->SetTitle("Electron p_{T} [GeV]");
    h_e_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_e_Tau_Efficiency = new TH1D("h_e_Tau_Efficiency","h_e_Tau_Efficiency",100, 0,100); h_e_Tau_Efficiency->SetDirectory(fEff);
    h_e_Tau_Efficiency->GetXaxis()->SetTitle("Electron p_{T} [GeV]");
    h_e_Tau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_e_IsoTau_Efficiency = new TH1D("h_e_IsoTau_Efficiency","h_e_IsoTau_Efficiency",100, 0,100); h_e_IsoTau_Efficiency->SetDirectory(fEff);
    h_e_IsoTau_Efficiency->GetXaxis()->SetTitle("Electron p_{T} [GeV]");
    h_e_IsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_e_Jet_Efficiency = new TH1D("h_e_Jet_Efficiency","h_e_Jet_Efficiency",100, 0,100); h_e_Jet_Efficiency->SetDirectory(fEff);
    h_e_Jet_Efficiency->GetXaxis()->SetTitle("Electron p_{T} [GeV]");
    h_e_Jet_Efficiency->GetYaxis()->SetTitle("Efficiency");

    
  }

    Efficiency::~Efficiency(){
        
      h_e_EG_Efficiency->Divide(h_e_Total);
      h_e_Tau_Efficiency->Divide(h_e_Total);
      h_e_IsoTau_Efficiency->Divide(h_e_Total);
      h_e_Jet_Efficiency->Divide(h_e_Total);
      
      fEff->Write();
      fEff->Close();

  }
  
  int Efficiency::PreAnalysis(){

      return 0;

  }

  int Efficiency::Execute(TreeEvent *event){

      
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
      
      double l1eCut = 20;
      //double l1muCut = 10;
      //double l1tauCut = 20;
      //double l1jetCut = 30;
      //double l1mjjCut = 200;
      //double l1deltaetaCut = 2.5;
      //double l1METCut = 20;
      //double l1MHTCut = 50;
      
      for(unsigned i=0; i < n_electrons_; i++){
                  
          bool eProceed = false;
                  
          double electroniso =  electrons[i]->dr04_pfiso_charged_all() + std::max(electrons[i]->dr04_pfiso_neutral() + electrons[i]->dr04_pfiso_gamma() - 0.5 * electrons[i]->dr04_pfiso_pu(), 0.0);
          electroniso = electroniso / electrons[i]->vector().Pt();
          if(electroniso <= 0.1 && std::fabs(electrons[i]->dxy_vertex()) < 0.045 && std::fabs(electrons[i]->dz_vertex()) < 0.2 && std::fabs(electrons[i]->vector().Rapidity()) < 2.1) eProceed = true;
          else eProceed = false;

          if(eProceed){
              
              int eGenIndex = -1;
              bool MatchedOffline = false;
              double PtDiffelectron = 10000;
              
              for(unsigned j=0; j< n_genParticles_; j++){

                  int genID = std::fabs(GenParticles[j]->pdgid());
                  bool isPrompt = false;
                  if((GenParticles[j]->statusFlags().at(0) == true || GenParticles[j]->statusFlags().at(5) == true) && genID == 11) isPrompt = true;
                  
                  /*for(unsigned k=0; k < GenParticles[j]->mothers().size(); k++) {
                      if(std::fabs(GenParticles[GenParticles[j]->mothers().at(k)]->pdgid()) == 15){
                          int ParentgenIndex = GenParticles[j]->mothers().at(k);
                          int GPgenID = std::fabs(GenParticles[GenParticles[ParentgenIndex]->mothers().at(0)]->pdgid());
                          if(genID == 11 && GPgenID == 25) isPrompt = true;
                      }
                  }*/
                 
                  double DeltaR = sqrt(pow(electrons[i]->vector().Phi()-GenParticles[j]->vector().Phi(),2) + pow(electrons[i]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
                  double PtDiff = std::fabs(electrons[i]->vector().Pt() - GenParticles[j]->vector().Pt());
                  
                  if(DeltaR < 0.1 && PtDiff < PtDiffelectron && isPrompt){
                       PtDiffelectron = PtDiff;
                       MatchedOffline = true;
                       eGenIndex = j;
                  }
              }
              
              if(MatchedOffline){
                  
                  h_e_Total->Fill(electrons[i]->vector().Pt());
                  
                  // Check if electron fired EG trigger.
                  
                  bool MatchedL1 = false;
                  double PtDiffelectron = 10000;
                  int eL1Index = -1;
              
                  for(unsigned j=0; j < n_l1electrons_; j++){
                  
                      double DeltaR = sqrt(pow(l1electrons[j]->vector().Phi()-GenParticles[eGenIndex]->vector().Phi(),2) + pow(l1electrons[j]->vector().Rapidity()-GenParticles[eGenIndex]->vector().Rapidity(),2));
                      double PtDiff = std::fabs(l1electrons[j]->vector().Pt() - GenParticles[eGenIndex]->vector().Pt());
                  
                      if(DeltaR < 0.2 && PtDiff < PtDiffelectron){
                           PtDiffelectron = PtDiff;
                           MatchedL1 = true;
                           eL1Index = j;
                      }
                      
                  }
                  
                  if(MatchedL1){ 
                      
                      bool L1EPtFired = false;
                      double L1EPt = l1electrons[eL1Index]->vector().Pt();
                      if(L1EPt >= l1eCut) L1EPtFired = true;
                      if(L1EPtFired) h_e_EG_Efficiency->Fill(electrons[i]->vector().Pt());
                  }
                 
                  // Check if electron fired tau and iso-tau triggers.
                  
                  bool MatchedTauL1 = false;
                  bool MatchedIsoTauL1 = false;
                  bool PtDiffelectronTau = 10000;
                  bool PtDiffelectronIsoTau = 10000;
                  int eTauL1Index = -1;
                  int eIsoTauL1Index = -1;
              
                  for(unsigned j=0; j < n_l1taus_; j++){
                  
                      double DeltaR = sqrt(pow(l1taus[j]->vector().Phi()-GenParticles[eGenIndex]->vector().Phi(),2) + pow(l1taus[j]->vector().Rapidity()-GenParticles[eGenIndex]->vector().Rapidity(),2));
                      double PtDiff = std::fabs(l1taus[j]->vector().Pt() - GenParticles[eGenIndex]->vector().Pt());
                      int tauIso = l1taus[j]->isolation;
                      bool IsoTau = false;
                      if(tauIso == 1) IsoTau = true;
                      if(DeltaR < 0.3 && PtDiff < PtDiffelectronTau && !IsoTau){
                           PtDiffelectronTau = PtDiff;
                           MatchedTauL1 = true;
                           eTauL1Index = j;
                      }
                      
                      if(DeltaR < 0.3 && PtDiff < PtDiffelectronIsoTau && IsoTau){
                           PtDiffelectronIsoTau = PtDiff;
                           MatchedIsoTauL1 = true;
                           eIsoTauL1Index = j;
                      }

                  }
                  
                  if(MatchedTauL1){ 
                      bool L1TauPtFired = false;
                      double L1TauPt = l1taus[eTauL1Index]->vector().Pt();
                      if(L1TauPt >= l1eCut) L1TauPtFired = true;
                      if(L1TauPtFired) h_e_Tau_Efficiency->Fill(electrons[i]->vector().Pt());
                  }
                  
                  if(MatchedIsoTauL1){ 
                      bool L1IsoTauPtFired = false;
                      double L1IsoTauPt = l1taus[eIsoTauL1Index]->vector().Pt();
                      if(L1IsoTauPt >= l1eCut) L1IsoTauPtFired = true;
                      if(L1IsoTauPtFired) h_e_IsoTau_Efficiency->Fill(electrons[i]->vector().Pt());
                  }
                  
                  // Check if electron fired Jet trigger.

                  bool MatchedJetL1 = false;
                  //double PtDiffelectronJet = 10000;
                  double DeltaRDiffelectronJet = 10000;
                  int eJetL1Index = -1;
              
                  for(unsigned j=0; j < n_l1jets_; j++){
                  
                      double DeltaR = sqrt(pow(l1jets[j]->vector().Phi()-GenParticles[eGenIndex]->vector().Phi(),2) + pow(l1jets[j]->vector().Rapidity()-GenParticles[eGenIndex]->vector().Rapidity(),2));
                      //double PtDiff = std::fabs(l1jets[j]->vector().Pt() - GenParticles[eGenIndex]->vector().Pt());
                  
                      if(DeltaR < 0.4 && /*PtDiff < PtDiffelectronJet*/ DeltaR < DeltaRDiffelectronJet){
                           //PtDiffelectronJet = PtDiff;
                           DeltaRDiffelectronJet = DeltaR;
                           MatchedJetL1 = true;
                           eJetL1Index = j;
                      }
                      
                  }
                  
                  if(MatchedJetL1){ 
                      
                      bool L1JetPtFired = false;
                      double L1JetPt = l1jets[eJetL1Index]->vector().Pt();
                      if(L1JetPt >= l1eCut) L1JetPtFired = true;
                      if(L1JetPtFired) h_e_Jet_Efficiency->Fill(electrons[i]->vector().Pt());
                  }
                  
              }
          }
      }
      
      return 0;
  }

  int Efficiency::PostAnalysis(){
    return 0;
  }

  void Efficiency::PrintInfo(){
    ;
  }
  



}//namespace


