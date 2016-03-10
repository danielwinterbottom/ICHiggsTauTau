
#include "Modules/interface/VariableHistograms.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
//#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/VariableHistograms.h"

#include "TVector3.h"

namespace ic {

  
  VariableHistograms::VariableHistograms(std::string const& name, 
                                         std::string output_name) : ModuleBase(name) {
    
    l1jets_label_ = "L1Jets"; 
    l1electrons_label_ = "L1EGammas";
    l1muons_label_ = "L1Muon"; 
    l1taus_label_ = "L1Taus";  
    l1met_label_ = "L1Sums";
    
    output_name_ = output_name;
     

    fOut = new TFile((output_name2_+output_name_).c_str(),"RECREATE");

    //l1 histograms
    
    h_L1EtaGap = new TH1D("L1EtaGap","L1EtaGap",100, 0,10); h_L1EtaGap->SetDirectory(fOut);
    h_L1EtaGap->GetXaxis()->SetTitle("Leading-Subleading Jets |#Delta#eta|");
    h_L1EtaGap->GetYaxis()->SetTitle("# Entries");

    h_L1MjjInv = new TH1D("L1Mjj","L1Mjj",100, 0,2500); h_L1MjjInv->SetDirectory(fOut);
    h_L1MjjInv->GetXaxis()->SetTitle("M_{jj} [GeV]");
    h_L1MjjInv->GetYaxis()->SetTitle("# Entries");

    h_L1DeltaPhijj = new TH1D("L1DeltaPhijj","L1DeltaPhijj",100, 0,6.5); h_L1DeltaPhijj->SetDirectory(fOut);
    h_L1DeltaPhijj->GetXaxis()->SetTitle("Leading-Subleading Jets |#Delta#phi|");
    h_L1DeltaPhijj->GetYaxis()->SetTitle("# Entries");

    h_L1DeltaRjj = new TH1D("L1DeltaRjj","L1DeltaRjj",100, 0,12); h_L1DeltaRjj->SetDirectory(fOut);
    h_L1DeltaRjj->GetXaxis()->SetTitle("Leading-Subleading Jets |#Delta R|");
    h_L1DeltaRjj->GetYaxis()->SetTitle("# Entries");

    h_L1LeadTauPt = new TH1D("L1LeadTauPt","L1LeadTauPt",100, 0,200); h_L1LeadTauPt->SetDirectory(fOut);
    h_L1LeadTauPt->GetXaxis()->SetTitle("Leading Tau P_{T} [GeV]");
    h_L1LeadTauPt->GetYaxis()->SetTitle("# Entries");

    h_L1SubLeadTauPt = new TH1D("L1SubLeadTauPt","L1SubLeadTauPt",100, 0,200); h_L1SubLeadTauPt->SetDirectory(fOut);
    h_L1SubLeadTauPt->GetXaxis()->SetTitle("Sub-leading Tau P_{T} [GeV]");
    h_L1SubLeadTauPt->GetYaxis()->SetTitle("# Entries");

    h_L1ElectronPt = new TH1D("L1ElectronPt","L1ElectronPt",100, 0,200); h_L1ElectronPt->SetDirectory(fOut);
    h_L1ElectronPt->GetXaxis()->SetTitle("Electron P_{T} [GeV]");
    h_L1ElectronPt->GetYaxis()->SetTitle("# Entries");

    h_L1MuonPt = new TH1D("L1MuonPt","L1MuonPt",100, 0,200); h_L1MuonPt->SetDirectory(fOut);
    h_L1MuonPt->GetXaxis()->SetTitle("Muon P_{T} [GeV]");
    h_L1MuonPt->GetYaxis()->SetTitle("# Entries");

    h_L1MET = new TH1D("L1MET","L1MET",100, 0,200); h_L1MET->SetDirectory(fOut);
    h_L1MET->GetXaxis()->SetTitle("MET [GeV]");
    h_L1MET->GetYaxis()->SetTitle("# Entries");

    h_L1SubLeadJetPt = new TH1D("L1SubLeadJetPt","L1SubLeadJetPt",100, 0,200); h_L1SubLeadJetPt->SetDirectory(fOut);
    h_L1SubLeadJetPt->GetXaxis()->SetTitle("Sub-leading Jet P_{T} [GeV]");
    h_L1SubLeadJetPt->GetYaxis()->SetTitle("# Entries");

    h_L1SubLeadJetEta = new TH1D("L1SubLeadJetEta","L1SubLeadJetEta",100, -5,5); h_L1SubLeadJetEta->SetDirectory(fOut);
    h_L1SubLeadJetEta->GetXaxis()->SetTitle("Sub-leading Jet #eta");
    h_L1SubLeadJetEta->GetYaxis()->SetTitle("# Entries");

    h_L1SubLeadJetPhi = new TH1D("L1SubLeadJetPhi","L1SubLeadJetPhi",100, -3.5,3.5); h_L1SubLeadJetPhi->SetDirectory(fOut);
    h_L1SubLeadJetPhi->GetXaxis()->SetTitle("Sub-leading Jet #phi");
    h_L1SubLeadJetPhi->GetYaxis()->SetTitle("# Entries");

    h_L1LeadJetPt = new TH1D("L1LeadJetPt","L1LeadJetPt",100, 0,200); h_L1LeadJetPt->SetDirectory(fOut);
    h_L1LeadJetPt->GetXaxis()->SetTitle("Leading Jet P_{T} [GeV]");
    h_L1LeadJetPt->GetYaxis()->SetTitle("# Entries");

    h_L1LeadJetEta = new TH1D("L1LeadJetEta","L1LeadJetEta",100, -5,5); h_L1LeadJetEta->SetDirectory(fOut);
    h_L1LeadJetEta->GetXaxis()->SetTitle("Leading Jet #eta");
    h_L1LeadJetEta->GetYaxis()->SetTitle("# Entries");

    h_L1LeadJetPhi = new TH1D("L1LeadJetPhi","L1LeadJetPhi",100, -3.5,3.5); h_L1LeadJetPhi->SetDirectory(fOut);
    h_L1LeadJetPhi->GetXaxis()->SetTitle("Leading Jet #phi");
    h_L1LeadJetPhi->GetYaxis()->SetTitle("# Entries");

  }

    VariableHistograms::~VariableHistograms(){
      fOut->Write();
  }
  
  int VariableHistograms::PreAnalysis(){
    std::cout << "** Output Histograms for n_jet >= 2 catagory **" << std::endl;

    return 0;
  }

  int VariableHistograms::Execute(TreeEvent *event){
      
      //L1
      std::vector<ic::L1TJet*>  l1jets = event->GetPtrVec<ic::L1TJet>(l1jets_label_);
      std::vector<ic::L1TEGamma*> l1electrons = event->GetPtrVec<ic::L1TEGamma>(l1electrons_label_);
      std::vector<ic::L1TMuon*> l1muons = event->GetPtrVec<ic::L1TMuon>(l1muons_label_);
      std::vector<ic::L1TTau*> l1taus = event->GetPtrVec<ic::L1TTau>(l1taus_label_);
      std::vector<ic::L1TSum*> l1met_vec = event->GetPtrVec<ic::L1TSum>(l1met_label_);

      n_l1jets_ = l1jets.size();
      n_l1electrons_ = l1electrons.size();
      n_l1muons_ = l1muons.size();
      n_l1taus_ = l1taus.size();

      double l1MET = l1met_vec[1]->et;
      h_L1MET->Fill(l1MET);
        
      if(n_l1jets_ >= 2){
        double l1eta_gap = fabs(l1jets[0]->vector().Rapidity()-l1jets[1]->vector().Rapidity());
        h_L1EtaGap->Fill(l1eta_gap);

        double l1phi_gap = fabs(l1jets[0]->vector().Phi()-l1jets[1]->vector().Phi());
        h_L1DeltaPhijj->Fill(l1phi_gap);

        double l1R_gap = sqrt(pow(l1jets[0]->vector().Phi()-l1jets[1]->vector().Phi(),2) + pow(l1jets[0]->vector().Rapidity()-l1jets[1]->vector().Rapidity(),2));
        h_L1DeltaRjj->Fill(l1R_gap);

        double l1Mij = (l1jets[0]->vector() + l1jets[1]->vector()).M();
        h_L1MjjInv->Fill(l1Mij);
      }
      
      if(n_l1jets_ >= 1){

        double l1LeadJPt = l1jets[0]->vector().Pt();
        h_L1LeadJetPt->Fill(l1LeadJPt);

        double l1LeadJEta = l1jets[0]->vector().Rapidity();
        h_L1LeadJetEta->Fill(l1LeadJEta);
      
        double l1LeadJPhi = l1jets[0]->vector().Phi();
        h_L1LeadJetPhi->Fill(l1LeadJPhi);

        double l1SubLeadJPt = l1jets[1]->vector().Pt();
        h_L1SubLeadJetPt->Fill(l1SubLeadJPt);

        double l1SubLeadJEta = l1jets[1]->vector().Rapidity();
        h_L1SubLeadJetEta->Fill(l1SubLeadJEta);

        double l1SubLeadJPhi = l1jets[1]->vector().Phi();
        h_L1SubLeadJetPhi->Fill(l1SubLeadJPhi);
      }
      if(n_l1electrons_ >= 1){

        double l1electronPt = l1electrons[0]->vector().pt();
        h_L1ElectronPt->Fill(l1electronPt);
      }
      
      if(n_l1muons_ >= 1){
        double l1muonPt = l1muons[0]->vector().pt();
        h_L1MuonPt->Fill(l1muonPt);
      }

      if(n_l1taus_ >= 1){
          
        double l1tauPt = l1taus[0]->vector().pt();
        h_L1LeadTauPt->Fill(l1tauPt);
      }
      if(n_l1taus_ >= 2){
        double l1SubtauPt = l1taus[1]->vector().pt();
        h_L1SubLeadTauPt->Fill(l1SubtauPt);
      }
      
      /*std::cout << "Jets momentum order:" << std::endl;
      for(int i = 0; i<n_l1jets_; i++){
          std::cout << l1jets[i]->vector().Pt() << std::endl;
      }
      std::cout << "Taus momentum order:" <<std::endl;
      for(int i = 0; i<n_l1taus_;i++){
          std::cout << l1taus[i]->vector().Pt() << std::endl;
      }
      std::cout << "Muons momentum order:" <<std::endl;
      for(int i = 0; i<n_l1muons_;i++){
          std::cout << l1muons[i]->vector().Pt() << std::endl;
      }
      std::cout << "Electrons momentum order:" <<std::endl;
      for(int i = 0; i<n_l1electrons_;i++){
          std::cout << l1electrons[i]->vector().Pt() << std::endl;
      }*/
      
      return 0;
  }

  int VariableHistograms::PostAnalysis(){
    return 0;
  }

  void VariableHistograms::PrintInfo(){
    ;
  }
  



}//namespace


