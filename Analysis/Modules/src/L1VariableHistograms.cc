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
                                         fwlite::TFileService *fs, std::string output_name) : ModuleBase(name) {
    
    l1jets_label_ = "L1Jets"; 
    l1electrons_label_ = "L1EGammas";
    l1muons_label_ = "L1Muons"; 
    l1taus_label_ = "L1Taus";  
    l1met_label_ = "L1Sums";
    
    TFileDirectory subDir = fs->mkdir(output_name.c_str());

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
    
    h_L1TauMultiplicity = subDir.make<TH1D>("h_L1TauMultiplicity","h_L1TauMultiplicity",10, 0,10); 
    h_L1TauMultiplicity->GetXaxis()->SetTitle("L1 Tau Multiplicity");
    h_L1TauMultiplicity->GetYaxis()->SetTitle("# Entries");
    
    h_L1IsoTauMultiplicity = subDir.make<TH1D>("h_L1IsoTauMultiplicity","h_L1IsoTauMultiplicity",10, 0,10); 
    h_L1IsoTauMultiplicity->GetXaxis()->SetTitle("L1 Iso-Tau Multiplicity");
    h_L1IsoTauMultiplicity->GetYaxis()->SetTitle("# Entries");
    
    h_TauPtVsJetPt = subDir.make<TH2D>("h_TauPtVsJetPt","h_TauPtVsJetPt",100, 0,200, 100,0,200); 
    h_TauPtVsJetPt->GetXaxis()->SetTitle("L1 Iso-Tau p_{T}");
    h_TauPtVsJetPt->GetYaxis()->SetTitle("L1 Jet p_{T}");
    
    h_TauPtToNormalize = subDir.make<TH2D>("h_TauPtToNormalize","h_TauPtToNormalize",100,0,200,100,0,200); 
    h_TauPtToNormalize->GetXaxis()->SetTitle("L1 Iso-Tau p_{T}");
    h_TauPtToNormalize->GetYaxis()->SetTitle("Entries");
    
    h_LeadIsoTauIDProb = subDir.make<TH1D>("h_LeadIsoTauIDProb","h_LeadIsoTauIDProb",100, 0,200); 
    h_LeadIsoTauIDProb->GetXaxis()->SetTitle("L1 iso-tau p_{T}");
    h_LeadIsoTauIDProb->GetYaxis()->SetTitle("Probability is prompt tau");
    
    h_LeadIsoTauIDProbTotal = subDir.make<TH1D>("h_LeadIsoTauIDProbTotal","h_LeadIsoTauIDProbTotal",100, 0,200); 
    h_LeadIsoTauIDProbTotal->GetXaxis()->SetTitle("L1 iso-tau p_{T}");
    h_LeadIsoTauIDProbTotal->GetYaxis()->SetTitle("Probability is prompt tau");
    
    h_PtDiffGenMatched = subDir.make<TH1D>("h_PtDiffGenMatched","h_PtDiffGenMatched",200, -100,100); 
    h_PtDiffGenMatched->GetXaxis()->SetTitle("L1 iso-tau p_{T} - L1 jet p_{T}");
    h_PtDiffGenMatched->GetYaxis()->SetTitle("Entries");
    
    h_PtResGenMatched = subDir.make<TH1D>("h_PtResGenMatched","h_PtResGenMatched",100, -5,5); 
    h_PtResGenMatched->GetXaxis()->SetTitle("(L1 iso-tau p_{T} - L1 jet p_{T}) / L1 iso-tau p_{T}");
    h_PtResGenMatched->GetYaxis()->SetTitle("Entries");
    
    h_DeltaRResGenMatched = subDir.make<TH1D>("h_DeltaRResGenMatched","h_DeltaRResGenMatched",100, 0,1); 
    h_DeltaRResGenMatched->GetXaxis()->SetTitle("#DeltaR L1 iso-tau p_{T} - L1 jet p_{T}");
    h_DeltaRResGenMatched->GetYaxis()->SetTitle("Entries");
    
    h_PtDiffNotMatched = subDir.make<TH1D>("h_PtDiffNotMatched","h_PtDiffNotMatched",200, -100,100); 
    h_PtDiffNotMatched->GetXaxis()->SetTitle("L1 iso-tau p_{T} - L1 jet p_{T}");
    h_PtDiffNotMatched->GetYaxis()->SetTitle("Entries");
    
    h_PtResNotMatched = subDir.make<TH1D>("h_PtResNotMatched","h_PtResNotMatched",100, -5,5); 
    h_PtResNotMatched->GetXaxis()->SetTitle("(L1 iso-tau p_{T} - L1 jet p_{T}) / L1 iso-tau p_{T}");
    h_PtResNotMatched->GetYaxis()->SetTitle("Entries");
    
    h_DeltaRResNotMatched = subDir.make<TH1D>("h_DeltaRResNotMatched","h_DeltaRResNotMatched",100, 0,1); 
    h_DeltaRResNotMatched->GetXaxis()->SetTitle("#DeltaR L1 iso-tau p_{T} - L1 jet p_{T}");
    h_DeltaRResNotMatched->GetYaxis()->SetTitle("Entries");
    
    h_DeltaRMissID = subDir.make<TH1D>("h_DeltaRMissID","h_DeltaRMissID",100, 0,1); 
    h_DeltaRMissID->GetXaxis()->SetTitle("#DeltaR L1 iso-tau p_{T} - L1 jet p_{T}");
    h_DeltaRMissID->GetYaxis()->SetTitle("Entries");

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
      
      unsigned nL1Taus10=0;
      unsigned nL1IsoTaus10=0;
      
      for(int i=0; i<n_l1taus_; i++){
          if(l1taus[i]->vector().Pt() >= 10) nL1Taus10++;
          if(l1taus[i]->vector().Pt() >= 10 && l1taus[i]->isolation == 1) nL1IsoTaus10++;
      }
      h_L1TauMultiplicity->Fill(nL1Taus10);
      h_L1IsoTauMultiplicity->Fill(nL1IsoTaus10);
      
      if(l1met_vec.size() >0){
          double l1MET = l1met_vec[2]->et;
          h_L1TL1MET->Fill(l1MET);
      }
        
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
        if(n_l1jets_ > 1){
            double l1SubLeadJPt = l1jets[1]->vector().Pt();
            h_L1TL1SubLeadJetPt->Fill(l1SubLeadJPt);

            double l1SubLeadJEta = l1jets[1]->vector().Rapidity();
            h_L1TL1SubLeadJetEta->Fill(l1SubLeadJEta);

            double l1SubLeadJPhi = l1jets[1]->vector().Phi();
            h_L1TL1SubLeadJetPhi->Fill(l1SubLeadJPhi);
        }
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
      
      std::vector<ic::GenParticle*> GenParticles = event->GetPtrVec<ic::GenParticle>("genParticles");
      unsigned n_genParticles_ = GenParticles.size();
      
      bool FoundLeadIsoTau = false;
      
      for(int i=0; i< n_l1taus_; i++){
          
          bool MatchedOffline = false;
          bool MatchedL1Jet = false;
          double DeltaRMax = 100000;
          int l1jetIndex = -1;
          int l1jetIndex2 = -1;
          
          
          for(unsigned j=0; j< n_genParticles_; j++){
         
              int genID = std::fabs(GenParticles[j]->pdgid());
              bool isPrompt = false;
         
              for(unsigned k=0; k < GenParticles[j]->mothers().size(); k++) {
                  if(std::fabs(GenParticles[GenParticles[j]->mothers().at(k)]->pdgid()) == 25){
                      if(genID == 15) isPrompt = true;
                  }
              }
              
              for(unsigned k=0; k < GenParticles[j]->daughters().size(); k++) {
                  if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 11 || std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 13) isPrompt = false;
                  if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 16) GenParticles[j]->set_vector(GenParticles[j]->vector() - GenParticles[GenParticles[j]->daughters().at(k)]->vector());
              }
             
              double DeltaR = sqrt(pow(l1taus[i]->vector().Phi()-GenParticles[j]->vector().Phi(),2) + pow(l1taus[i]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
              
              if(DeltaR < 0.5 && isPrompt) MatchedOffline = true;
          }
          
          for(int j=0; j< n_l1jets_; j++){
              double DeltaR = sqrt(pow(l1taus[i]->vector().Phi()-l1jets[j]->vector().Phi(),2) + pow(l1taus[i]->vector().Rapidity()-l1jets[j]->vector().Rapidity(),2));
              if(DeltaR < 0.5 && DeltaR < DeltaRMax){
                  MatchedL1Jet = true;
                  DeltaRMax = DeltaR;
                  l1jetIndex2 = l1jetIndex;
                  l1jetIndex = j;
              }
          }
          
          if(MatchedL1Jet && MatchedOffline && l1taus[i]->isolation==1){ 
              h_TauPtVsJetPt->Fill(l1taus[i]->vector().Pt(), l1jets[l1jetIndex]->vector().Pt());
              for(unsigned j=0;j <=100; j++){
                 double temp = j*2;
                  h_TauPtToNormalize->Fill(l1taus[i]->vector().Pt(),temp);
              }
          }
              
          if(!FoundLeadIsoTau && l1taus[i]->isolation==1){
              
              FoundLeadIsoTau = true;
              
              if(l1jetIndex2 != -1){
                  double DeltaR = sqrt(pow(l1taus[i]->vector().Phi()-l1jets[l1jetIndex2]->vector().Phi(),2) + pow(l1taus[i]->vector().Rapidity()-l1jets[l1jetIndex2]->vector().Rapidity(),2));
                  h_DeltaRMissID->Fill(DeltaR);
              }
              
              h_LeadIsoTauIDProbTotal->Fill(l1taus[i]->vector().Pt());
              if(MatchedOffline) h_LeadIsoTauIDProb->Fill(l1taus[i]->vector().Pt());  
                  
                  if(MatchedOffline && MatchedL1Jet){ 
                      double DeltaRRes = sqrt(pow(l1taus[i]->vector().Phi()-l1jets[l1jetIndex]->vector().Phi(),2) + pow(l1taus[i]->vector().Rapidity()-l1jets[l1jetIndex]->vector().Rapidity(),2));
                      double PtDiff = l1taus[i]->vector().Pt()-l1jets[l1jetIndex]->vector().Pt();
                      double PtRes = PtDiff / l1taus[i]->vector().Pt();
                      h_PtDiffGenMatched->Fill(PtDiff);
                      h_PtResGenMatched->Fill(PtRes);
                      h_DeltaRResGenMatched->Fill(DeltaRRes);
                  }
                  else if(!MatchedOffline && MatchedL1Jet){
                      double DeltaRRes = sqrt(pow(l1taus[i]->vector().Phi()-l1jets[l1jetIndex]->vector().Phi(),2) + pow(l1taus[i]->vector().Rapidity()-l1jets[l1jetIndex]->vector().Rapidity(),2));
                      double PtDiff = l1taus[i]->vector().Pt()-l1jets[l1jetIndex]->vector().Pt();
                      double PtRes = PtDiff / l1taus[i]->vector().Pt();
                      h_PtDiffNotMatched->Fill(PtDiff);
                      h_PtResNotMatched->Fill(PtRes);
                      h_DeltaRResNotMatched->Fill(DeltaRRes);
                  }
          }
          
      }
          
      
      
      return 0;
  }

  int L1VariableHistograms::PostAnalysis(){
    h_TauPtVsJetPt->Divide(h_TauPtToNormalize);
    h_LeadIsoTauIDProb->Divide(h_LeadIsoTauIDProbTotal);
    
    return 0;
  }

  void L1VariableHistograms::PrintInfo(){
    ;
  }
  

}//namespace


