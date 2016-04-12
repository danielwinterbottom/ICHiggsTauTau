#include "Modules/interface/VariableHistograms.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

#include "TVector3.h"

namespace ic {

  
  VariableHistograms::VariableHistograms(std::string const& name, 
                                         fwlite::TFileService *fs, std::string output_name) : ModuleBase(name) {
    jets_label_ = "ak4PFJetsCHS"; 
    electrons_label_ = "electrons";
    muons_label_ = "muons"; 
    taus_label_ = "taus";  
    ditau_label_ = "emtauCandidates";
    met_label_ = "pfMet";
    

    
    TFileDirectory subDir = fs->mkdir(output_name.c_str());
    
    //offline histograms
    h_OfflineEtaGap = subDir.make<TH1D>("EtaGap","EtaGap",100, 0,10);
    h_OfflineEtaGap->GetXaxis()->SetTitle("Leading-Subleading Jets |#Delta#eta|");
    h_OfflineEtaGap->GetYaxis()->SetTitle("# Entries");

    h_OfflineMjjInv = subDir.make<TH1D>("Mjj","Mjj",100, 0,2500); 
    h_OfflineMjjInv->GetXaxis()->SetTitle("M_{jj} [GeV]");
    h_OfflineMjjInv->GetYaxis()->SetTitle("# Entries");
    
    h_VBFMjj = subDir.make<TH1D>("h_VBFMjj","h_VBFMjj",100, 0,2500); 
    h_VBFMjj->GetXaxis()->SetTitle("m_{jj} [GeV]");
    h_VBFMjj->GetYaxis()->SetTitle("# Entries");

    h_OfflineDeltaPhijj = subDir.make<TH1D>("DeltaPhijj","DeltaPhijj",100, 0,6.5); 
    h_OfflineDeltaPhijj->GetXaxis()->SetTitle("Leading-Subleading Jets |#Delta#phi|");
    h_OfflineDeltaPhijj->GetYaxis()->SetTitle("# Entries");

    h_OfflineDeltaRjj = subDir.make<TH1D>("DeltaRjj","DeltaRjj",100, 0,12); 
    h_OfflineDeltaRjj->GetXaxis()->SetTitle("Leading-Subleading Jets |#Delta R|");
    h_OfflineDeltaRjj->GetYaxis()->SetTitle("# Entries");

    h_OfflineMttVis = subDir.make<TH1D>("MttVis","MttVis",100, 0,200); 
    h_OfflineMttVis->GetXaxis()->SetTitle("Visible M_{#tau#tau} [GeV]");
    h_OfflineMttVis->GetYaxis()->SetTitle("# Entries");

    h_OfflineDeltaPhitt = subDir.make<TH1D>("DeltaPhitt","DeltaPhitt",100, 0,6.5); 
    h_OfflineDeltaPhitt->GetXaxis()->SetTitle("Tau Candidates |#Delta#phi|");
    h_OfflineDeltaPhitt->GetYaxis()->SetTitle("# Entries");

    h_OfflineDeltaRtt = subDir.make<TH1D>("DeltaRtt","DeltaRtt",100, 0,12); 
    h_OfflineDeltaRtt->GetXaxis()->SetTitle("Tau Candidates |#Delta R|");
    h_OfflineDeltaRtt->GetYaxis()->SetTitle("# Entries");

    h_OfflineDeltaEtatt = subDir.make<TH1D>("DeltaEtatt","DeltaEtatt",100, 0,10); 
    h_OfflineDeltaEtatt->GetXaxis()->SetTitle("Tau Candidates |#Delta#eta|");
    h_OfflineDeltaEtatt->GetYaxis()->SetTitle("# Entries");

    h_OfflineLeadTauPt = subDir.make<TH1D>("LeadTauPt","LeadTauPt",100, 0,200);
    h_OfflineLeadTauPt->GetXaxis()->SetTitle("Leading Tau P_{T} [GeV]");
    h_OfflineLeadTauPt->GetYaxis()->SetTitle("# Entries");

    h_OfflineSubLeadTauPt = subDir.make<TH1D>("SubLeadTauPt","SubLeadTauPt",100, 0,200); 
    h_OfflineSubLeadTauPt->GetXaxis()->SetTitle("Sub-leading Tau P_{T} [GeV]");
    h_OfflineSubLeadTauPt->GetYaxis()->SetTitle("# Entries");

    h_OfflineElectronPt = subDir.make<TH1D>("ElectronPt","ElectronPt",100, 0,200); 
    h_OfflineElectronPt->GetXaxis()->SetTitle("Electron P_{T} [GeV]");
    h_OfflineElectronPt->GetYaxis()->SetTitle("# Entries");

    h_OfflineMuonPt = subDir.make<TH1D>("MuonPt","MuonPt",100, 0,200);
    h_OfflineMuonPt->GetXaxis()->SetTitle("Muon P_{T} [GeV]");
    h_OfflineMuonPt->GetYaxis()->SetTitle("# Entries");

    h_OfflineMET = subDir.make<TH1D>("MET","MET",100, 0,200);
    h_OfflineMET->GetXaxis()->SetTitle("MET [GeV]");
    h_OfflineMET->GetYaxis()->SetTitle("# Entries");

    h_OfflineSubLeadJetPt = subDir.make<TH1D>("SubLeadJetPt","SubLeadJetPt",100, 0,200); 
    h_OfflineSubLeadJetPt->GetXaxis()->SetTitle("Sub-leading Jet P_{T} [GeV]");
    h_OfflineSubLeadJetPt->GetYaxis()->SetTitle("# Entries");

    h_OfflineSubLeadJetEta = subDir.make<TH1D>("SubLeadJetEta","SubLeadJetEta",100, -5,5); 
    h_OfflineSubLeadJetEta->GetXaxis()->SetTitle("Sub-leading Jet #eta");
    h_OfflineSubLeadJetEta->GetYaxis()->SetTitle("# Entries");

    h_OfflineSubLeadJetPhi = subDir.make<TH1D>("SubLeadJetPhi","SubLeadJetPhi",100, -3.5,3.5); 
    h_OfflineSubLeadJetPhi->GetXaxis()->SetTitle("Sub-leading Jet #phi");
    h_OfflineSubLeadJetPhi->GetYaxis()->SetTitle("# Entries");

    h_OfflineLeadJetPt = subDir.make<TH1D>("LeadJetPt","LeadJetPt",100, 0,200); 
    h_OfflineLeadJetPt->GetXaxis()->SetTitle("Leading Jet P_{T} [GeV]");
    h_OfflineLeadJetPt->GetYaxis()->SetTitle("# Entries");

    h_OfflineLeadJetEta = subDir.make<TH1D>("LeadJetEta","LeadJetEta",100, -5,5); 
    h_OfflineLeadJetEta->GetXaxis()->SetTitle("Leading Jet #eta");
    h_OfflineLeadJetEta->GetYaxis()->SetTitle("# Entries");

    h_OfflineLeadJetPhi = subDir.make<TH1D>("LeadJetPhi","LeadJetPhi",100, -3.5,3.5); 
    h_OfflineLeadJetPhi->GetXaxis()->SetTitle("Leading Jet #phi");
    h_OfflineLeadJetPhi->GetYaxis()->SetTitle("# Entries");

    h_OfflineMinPhi = subDir.make<TH1D>("MinPhi","MinPhi",100, 0,6.5); 
    h_OfflineMinPhi->GetXaxis()->SetTitle("Min #Delta#phi");
    h_OfflineMinPhi->GetYaxis()->SetTitle("# Entries");
    
    h_JetsPtScalarSum = subDir.make<TH1D>("JetsPtScalarSum","JetsPtScalarSum",100, 0,200); 
    h_JetsPtScalarSum->GetXaxis()->SetTitle("Jets P_{T} Scalar Sum [GeV]");
    h_JetsPtScalarSum->GetYaxis()->SetTitle("# Entries");
    
    h_JetsPtVectorSum = subDir.make<TH1D>("JetsPtVectorSum","JetsPtVectorSum",100, 0,200); 
    h_JetsPtVectorSum->GetXaxis()->SetTitle("Jets P_{T} Vector Sum [GeV]");
    h_JetsPtVectorSum->GetYaxis()->SetTitle("# Entries");


  }

    VariableHistograms::~VariableHistograms(){
        ;
  }
  
  int VariableHistograms::PreAnalysis(){
    return 0;
  }

  int VariableHistograms::Execute(TreeEvent *event){
      
      //offline 
      
      std::vector<PFJet*> const& jets = event->GetPtrVec<PFJet>(jets_label_);
      std::vector<Electron*> electrons = event->GetPtrVec<Electron>(electrons_label_);
      std::vector<Muon*> muons = event->GetPtrVec<Muon>(muons_label_);
      std::vector<Tau*> taus = event->GetPtrVec<Tau>(taus_label_);
      std::vector<Met*> met_vec = event->GetPtrVec<Met>(met_label_);

      n_jets_ = jets.size();
      n_electrons_ = electrons.size();
      n_muons_ = muons.size();
      n_taus_ = taus.size();

      double MET = met_vec[0]->vector().pt();
      h_OfflineMET->Fill(MET);

      if(n_jets_ > 1){
          
          if(jets[0]->vector().Pt() > 20 && jets[0]->vector().Pt() >20){
              double Mij = (jets[0]->vector() + jets[1]->vector()).M();
              h_OfflineMjjInv->Fill(Mij);
          
              double eta_gap = fabs(jets[0]->vector().Rapidity()-jets[1]->vector().Rapidity());
              h_OfflineEtaGap->Fill(eta_gap);
              
              if(jets[0]->vector().Pt() >40 && jets[0]->vector().Pt() >40 && eta_gap > 2) h_VBFMjj->Fill(Mij);
          }
          
          double phi_gap = fabs(jets[0]->vector().Phi()-jets[1]->vector().Phi());
          h_OfflineDeltaPhijj->Fill(phi_gap);

          double R_gap = sqrt(pow(jets[0]->vector().Phi()-jets[1]->vector().Phi(),2) + pow(jets[0]->vector().Rapidity()-jets[1]->vector().Rapidity(),2));
          h_OfflineDeltaRjj->Fill(R_gap);
          
          double JetsPtVectorSum = (jets[0]->vector()+jets[1]->vector()).Pt();
          h_JetsPtVectorSum->Fill(JetsPtVectorSum);
      
          double JetsPtScalarSum = jets[0]->vector().Pt() +jets[1]->vector().Pt();
          h_JetsPtScalarSum->Fill(JetsPtScalarSum);
          
          double SubLeadJPt = jets[1]->vector().Pt();
          h_OfflineSubLeadJetPt->Fill(SubLeadJPt);

          double SubLeadJEta = jets[1]->vector().Rapidity();
          h_OfflineSubLeadJetEta->Fill(SubLeadJEta);
  
          double SubLeadJPhi = jets[1]->vector().Phi();
          h_OfflineSubLeadJetPhi->Fill(SubLeadJPhi);
      
      }
      
      if(n_jets_ > 0){
          
          double LeadJPt = jets[0]->vector().Pt();
          h_OfflineLeadJetPt->Fill(LeadJPt);

          double LeadJEta = jets[0]->vector().Rapidity();
          h_OfflineLeadJetEta->Fill(LeadJEta);
  
          double LeadJPhi = jets[0]->vector().Phi();
          h_OfflineLeadJetPhi->Fill(LeadJPhi);
      }
      

      std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>(ditau_label_);
      CompositeCandidate const* ditau = ditau_vec.at(0);
      Candidate const* lep1 = ditau->GetCandidate("lepton1");
      Candidate const* lep2 = ditau->GetCandidate("lepton2");
      double pt_1_ = lep1->pt();
      double pt_2_ = lep2->pt();

      double Mtt_Vis = (lep1->vector() + lep2->vector()).M();
      h_OfflineMttVis->Fill(Mtt_Vis);

      double DeltaPhi_tt = fabs(lep1->vector().Phi() - lep2->vector().Phi());
      h_OfflineDeltaPhitt->Fill(DeltaPhi_tt);

      double DeltaEta_tt = fabs(lep1->vector().Rapidity() - lep2->vector().Rapidity());
      h_OfflineDeltaEtatt->Fill(DeltaEta_tt);

      double DeltaR_tt = sqrt(pow(lep1->vector().Rapidity() - lep2->vector().Rapidity(),2) + pow(lep1->vector().Phi() - lep2->vector().Phi(),2));
      h_OfflineDeltaRtt->Fill(DeltaR_tt);

      if (n_electrons_ > 0){
        double electronPt = electrons[0]->vector().pt();
        if (electronPt == pt_1_ || electronPt == pt_2_) h_OfflineElectronPt->Fill(electronPt);
      }

      if (n_muons_ > 0){
        double muonPt = muons[0]->vector().pt();
        if (muonPt == pt_1_ || muonPt == pt_2_) h_OfflineMuonPt->Fill(muonPt);
      }

      if (n_taus_ > 0){
        double tauPt = taus[0]->vector().pt();
        if (tauPt == pt_1_ || tauPt == pt_2_) h_OfflineLeadTauPt->Fill(tauPt);
      }

      if (n_taus_ > 1){
        double SubtauPt = taus[1]->vector().pt();
        if (SubtauPt == pt_1_ || SubtauPt == pt_2_) h_OfflineSubLeadTauPt->Fill(SubtauPt);
      }

      double DetaPhiTemp = 0;
      double MinDeltaPhi = 10000;

      if(n_jets_ > 1){
          for(int i=0; i<2; i++){
            DetaPhiTemp = fabs(lep1->vector().Phi() - jets[i]->vector().Phi());
            if(DetaPhiTemp < MinDeltaPhi) MinDeltaPhi = DetaPhiTemp;

            DetaPhiTemp = fabs(lep2->vector().Phi() - jets[i]->vector().Phi());
            if(DetaPhiTemp < MinDeltaPhi) MinDeltaPhi = DetaPhiTemp;
          }
          h_OfflineMinPhi->Fill(MinDeltaPhi);
      }
    
      
      return 0;
  }

  int VariableHistograms::PostAnalysis(){
    return 0;
  }

  void VariableHistograms::PrintInfo(){
    ;
  }
  



}//namespace


