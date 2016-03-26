#include "Modules/interface/L1VariableHistograms.h"
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

  
  L1VariableHistograms::L1VariableHistograms(std::string const& name, 
                                         fwlite::TFileService *fs) : ModuleBase(name) {
    
    l1jets_label_ = "L1Jets"; 
    l1electrons_label_ = "L1EGammas";
    l1muons_label_ = "L1Muons"; 
    l1taus_label_ = "L1Taus";  
    l1met_label_ = "L1Sums";
    
    TFileDirectory subDir = fs->mkdir("L1THistograms");

    //l1 histograms
    
    h_L1TL1EtaGap = subDir.make<TH1D>("L1EtaGap","L1EtaGap",100, 0,10);
    h_L1TL1EtaGap->GetXaxis()->SetTitle("Leading-Subleading Jets |#Delta#eta|");
    h_L1TL1EtaGap->GetYaxis()->SetTitle("# Entries");

    h_L1TL1MjjInv = subDir.make<TH1D>("L1Mjj","L1Mjj",100, 0,2500);
    h_L1TL1MjjInv->GetXaxis()->SetTitle("M_{jj} [GeV]");
    h_L1TL1MjjInv->GetYaxis()->SetTitle("# Entries");

    h_L1TL1DeltaPhijj = subDir.make<TH1D>("L1DeltaPhijj","L1DeltaPhijj",100, 0,6.5);
    h_L1TL1DeltaPhijj->GetXaxis()->SetTitle("Leading-Subleading Jets |#Delta#phi|");
    h_L1TL1DeltaPhijj->GetYaxis()->SetTitle("# Entries");

    h_L1TL1DeltaRjj = subDir.make<TH1D>("L1DeltaRjj","L1DeltaRjj",100, 0,12);
    h_L1TL1DeltaRjj->GetXaxis()->SetTitle("Leading-Subleading Jets |#Delta R|");
    h_L1TL1DeltaRjj->GetYaxis()->SetTitle("# Entries");

    h_L1TL1LeadTauPt = subDir.make<TH1D>("L1LeadTauPt","L1LeadTauPt",100, 0,200);
    h_L1TL1LeadTauPt->GetXaxis()->SetTitle("Leading Tau P_{T} [GeV]");
    h_L1TL1LeadTauPt->GetYaxis()->SetTitle("# Entries");

    h_L1TL1SubLeadTauPt = subDir.make<TH1D>("L1SubLeadTauPt","L1SubLeadTauPt",100, 0,200); 
    h_L1TL1SubLeadTauPt->GetXaxis()->SetTitle("Sub-leading Tau P_{T} [GeV]");
    h_L1TL1SubLeadTauPt->GetYaxis()->SetTitle("# Entries");

    h_L1TL1ElectronPt = subDir.make<TH1D>("L1ElectronPt","L1ElectronPt",100, 0,200); 
    h_L1TL1ElectronPt->GetXaxis()->SetTitle("Electron P_{T} [GeV]");
    h_L1TL1ElectronPt->GetYaxis()->SetTitle("# Entries");

    h_L1TL1MuonPt = subDir.make<TH1D>("L1MuonPt","L1MuonPt",100, 0,200); 
    h_L1TL1MuonPt->GetXaxis()->SetTitle("Muon P_{T} [GeV]");
    h_L1TL1MuonPt->GetYaxis()->SetTitle("# Entries");

    h_L1TL1MET = subDir.make<TH1D>("L1MET","L1MET",100, 0,200); 
    h_L1TL1MET->GetXaxis()->SetTitle("MET [GeV]");
    h_L1TL1MET->GetYaxis()->SetTitle("# Entries");

    h_L1TL1SubLeadJetPt = subDir.make<TH1D>("L1SubLeadJetPt","L1SubLeadJetPt",100, 0,200); 
    h_L1TL1SubLeadJetPt->GetXaxis()->SetTitle("Sub-leading Jet P_{T} [GeV]");
    h_L1TL1SubLeadJetPt->GetYaxis()->SetTitle("# Entries");

    h_L1TL1SubLeadJetEta = subDir.make<TH1D>("L1SubLeadJetEta","L1SubLeadJetEta",100, -5,5);
    h_L1TL1SubLeadJetEta->GetXaxis()->SetTitle("Sub-leading Jet #eta");
    h_L1TL1SubLeadJetEta->GetYaxis()->SetTitle("# Entries");

    h_L1TL1SubLeadJetPhi = subDir.make<TH1D>("L1SubLeadJetPhi","L1SubLeadJetPhi",100, -3.5,3.5); 
    h_L1TL1SubLeadJetPhi->GetXaxis()->SetTitle("Sub-leading Jet #phi");
    h_L1TL1SubLeadJetPhi->GetYaxis()->SetTitle("# Entries");

    h_L1TL1LeadJetPt = subDir.make<TH1D>("L1LeadJetPt","L1LeadJetPt",100, 0,200); 
    h_L1TL1LeadJetPt->GetXaxis()->SetTitle("Leading Jet P_{T} [GeV]");
    h_L1TL1LeadJetPt->GetYaxis()->SetTitle("# Entries");

    h_L1TL1LeadJetEta = subDir.make<TH1D>("L1LeadJetEta","L1LeadJetEta",100, -5,5); 
    h_L1TL1LeadJetEta->GetXaxis()->SetTitle("Leading Jet #eta");
    h_L1TL1LeadJetEta->GetYaxis()->SetTitle("# Entries");

    h_L1TL1LeadJetPhi = subDir.make<TH1D>("L1LeadJetPhi","L1LeadJetPhi",100, -3.5,3.5); 
    h_L1TL1LeadJetPhi->GetXaxis()->SetTitle("Leading Jet #phi");
    h_L1TL1LeadJetPhi->GetYaxis()->SetTitle("# Entries");

  }

    L1VariableHistograms::~L1VariableHistograms(){
      ;
  }
  
  int L1VariableHistograms::PreAnalysis(){
    return 0;
  }

  int L1VariableHistograms::Execute(TreeEvent *event){
      
      //L1
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

      double l1MET = l1met_vec[1]->et;
      h_L1TL1MET->Fill(l1MET);
        
      if(n_l1jets_ >= 2){
        double l1eta_gap = fabs(l1jets[0]->vector().Rapidity()-l1jets[1]->vector().Rapidity());
        h_L1TL1EtaGap->Fill(l1eta_gap);

        double l1phi_gap = fabs(l1jets[0]->vector().Phi()-l1jets[1]->vector().Phi());
        h_L1TL1DeltaPhijj->Fill(l1phi_gap);

        double l1R_gap = sqrt(pow(l1jets[0]->vector().Phi()-l1jets[1]->vector().Phi(),2) + pow(l1jets[0]->vector().Rapidity()-l1jets[1]->vector().Rapidity(),2));
        h_L1TL1DeltaRjj->Fill(l1R_gap);

        double l1Mij = (l1jets[0]->vector() + l1jets[1]->vector()).M();
        h_L1TL1MjjInv->Fill(l1Mij);
      }
      
      if(n_l1jets_ >= 1){

        double l1LeadJPt = l1jets[0]->vector().Pt();
        h_L1TL1LeadJetPt->Fill(l1LeadJPt);

        double l1LeadJEta = l1jets[0]->vector().Rapidity();
        h_L1TL1LeadJetEta->Fill(l1LeadJEta);
      
        double l1LeadJPhi = l1jets[0]->vector().Phi();
        h_L1TL1LeadJetPhi->Fill(l1LeadJPhi);

        double l1SubLeadJPt = l1jets[1]->vector().Pt();
        h_L1TL1SubLeadJetPt->Fill(l1SubLeadJPt);

        double l1SubLeadJEta = l1jets[1]->vector().Rapidity();
        h_L1TL1SubLeadJetEta->Fill(l1SubLeadJEta);

        double l1SubLeadJPhi = l1jets[1]->vector().Phi();
        h_L1TL1SubLeadJetPhi->Fill(l1SubLeadJPhi);
      }
      if(n_l1electrons_ >= 1){

        double l1electronPt = l1electrons[0]->vector().pt();
        h_L1TL1ElectronPt->Fill(l1electronPt);
      }
      
      if(n_l1muons_ >= 1){
        double l1muonPt = l1muons[0]->vector().pt();
        h_L1TL1MuonPt->Fill(l1muonPt);
      }

      if(n_l1taus_ >= 1){
          
        double l1tauPt = l1taus[0]->vector().pt();
        h_L1TL1LeadTauPt->Fill(l1tauPt);
      }
      if(n_l1taus_ >= 2){
        double l1SubtauPt = l1taus[1]->vector().pt();
        h_L1TL1SubLeadTauPt->Fill(l1SubtauPt);
      }
      
 
      
      return 0;
  }

  int L1VariableHistograms::PostAnalysis(){
    return 0;
  }

  void L1VariableHistograms::PrintInfo(){
    ;
  }
  

}//namespace


