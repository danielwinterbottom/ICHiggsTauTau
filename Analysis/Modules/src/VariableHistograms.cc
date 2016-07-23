#include "Modules/interface/VariableHistograms.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"

#include <iostream>
#include <fstream>

#include "TVector3.h"

struct greater_Candidate{
  bool operator() (const ic::Candidate *a,const ic::Candidate *b) {
    return (a->pt() > b->pt());
  }
};

namespace ic {

  
  VariableHistograms::VariableHistograms(std::string const& name, 
                                         fwlite::TFileService *fs, std::string output_name, std::string channel) : ModuleBase(name) {
    jets_label_ = "ak4PFJetsCHS"; 
    electrons_label_ = "electrons";
    muons_label_ = "muons"; 
    taus_label_ = "taus";  
    ditau_label_ = "emtauCandidates";
    met_label_ = "pfMVAMet";
    
    if(channel == "tt") leadtaus_label = "leadingtaus";
    else leadtaus_label = "taus";

    
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
    h_JetsPtScalarSum->GetXaxis()->SetTitle("Jets <p_{T}> [GeV]");
    h_JetsPtScalarSum->GetYaxis()->SetTitle("# Entries");
    
    h_JetsPtVectorSum = subDir.make<TH1D>("JetsPtVectorSum","JetsPtVectorSum",100, 0,200); 
    h_JetsPtVectorSum->GetXaxis()->SetTitle("Jets P_{T} Vector Sum [GeV]");
    h_JetsPtVectorSum->GetYaxis()->SetTitle("# Entries");
    
    h_pTT = subDir.make<TH1D>("h_pTT","h_pTT",100, 0,300); 
    h_pTT->GetXaxis()->SetTitle("p_{TT} [GeV]");
    h_pTT->GetYaxis()->SetTitle("# Entries");
    
    h_pTTMinusVecPt = subDir.make<TH1D>("h_pTTMinusVecPt","h_pTTMinusVecPt",100, 0,300); 
    h_pTTMinusVecPt->GetXaxis()->SetTitle("p_{TT} - DiJetpt [GeV]");
    h_pTTMinusVecPt->GetYaxis()->SetTitle("# Entries");
    
    h_pTTMinusVecPtRes = subDir.make<TH1D>("h_pTTMinusVecPtRes","h_pTTMinusVecPtRes",100, 0,5); 
    h_pTTMinusVecPtRes->GetXaxis()->SetTitle("(p_{TT} - DiJetpt)/DiJetPt");
    h_pTTMinusVecPtRes->GetYaxis()->SetTitle("# Entries");
    
    h_mVis_LOOSE = subDir.make<TH1D>("h_mVis_LOOSE","h_mVis_LOOSE",100, 0,200); 
    h_mVis_LOOSE->GetXaxis()->SetTitle("m_{vis} [GeV]");
    h_mVis_LOOSE->GetYaxis()->SetTitle("# Entries");
    
    h_mVis_TIGHT = subDir.make<TH1D>("h_mVis_TIGHT","h_mVis_TIGHT",100, 0,200); 
    h_mVis_TIGHT->GetXaxis()->SetTitle("m_{vis} [GeV]");
    h_mVis_TIGHT->GetYaxis()->SetTitle("# Entries");
    
    h_pTTMinusVecPtResVsDiJetPt = subDir.make<TH2D>("h_pTTMinusVecPtResVsDiJetPt","h_pTTMinusVecPtResVsDiJetPt", 100, 0, 300, 100, -2,2); 
    h_pTTMinusVecPtResVsDiJetPt->GetXaxis()->SetTitle("DiJetPt [GeV]");
    h_pTTMinusVecPtResVsDiJetPt->GetYaxis()->SetTitle("(p_{TT} - DiJetpt)/DiJetPt");
    
    h_DiJetPTMinusPTTResVsPTT = subDir.make<TH2D>("h_DiJetPTMinusPTTResVsPTT","h_DiJetPTMinusPTTResVsPTT", 100, 0, 300, 100, -2,2); 
    h_DiJetPTMinusPTTResVsPTT->GetXaxis()->SetTitle("p_{TT} [GeV]");
    h_DiJetPTMinusPTTResVsPTT->GetYaxis()->SetTitle("(p_{TT} - DiJetpt)/p_{TT}");
    
    h_ElecPt_LOOSE = subDir.make<TH1D>("h_ElecPt_LOOSE","h_ElecPt_LOOSE",200, 0,200); 
    h_ElecPt_LOOSE->GetXaxis()->SetTitle("Electron p_{T} [GeV]");
    h_ElecPt_LOOSE->GetYaxis()->SetTitle("# Entries");
    
    h_ElecPt_TIGHT = subDir.make<TH1D>("h_ElecPt_TIGHT","h_ElecPt_TIGHT",200, 0,200); 
    h_ElecPt_TIGHT->GetXaxis()->SetTitle("Electron p_{T} [GeV]");
    h_ElecPt_TIGHT->GetYaxis()->SetTitle("# Entries");
    
    h_MuPt_LOOSE = subDir.make<TH1D>("h_MuPt_LOOSE","h_MuPt_LOOSE",200, 0,200); 
    h_MuPt_LOOSE->GetXaxis()->SetTitle("Muon p_{T} [GeV]");
    h_MuPt_LOOSE->GetYaxis()->SetTitle("# Entries");
    
    h_MuPt_TIGHT = subDir.make<TH1D>("h_MuPt_TIGHT","h_MuPt_TIGHT",200, 0,200); 
    h_MuPt_TIGHT->GetXaxis()->SetTitle("Muon p_{T} [GeV]");
    h_MuPt_TIGHT->GetYaxis()->SetTitle("# Entries");
    
    h_Tau1Pt_LOOSE = subDir.make<TH1D>("h_Tau1Pt_LOOSE","h_Tau1Pt_LOOSE",200, 0,200); 
    h_Tau1Pt_LOOSE->GetXaxis()->SetTitle("Lead tau p_{T} [GeV]");
    h_Tau1Pt_LOOSE->GetYaxis()->SetTitle("# Entries");
    
    h_Tau1Pt_TIGHT = subDir.make<TH1D>("h_Tau1Pt_TIGHT","h_Tau1Pt_TIGHT",200, 0,200); 
    h_Tau1Pt_TIGHT->GetXaxis()->SetTitle("Lead tau p_{T} [GeV]");
    h_Tau1Pt_TIGHT->GetYaxis()->SetTitle("# Entries");
    
    h_Tau2Pt_LOOSE = subDir.make<TH1D>("h_Tau2Pt_LOOSE","h_Tau2Pt_LOOSE",200, 0,200); 
    h_Tau2Pt_LOOSE->GetXaxis()->SetTitle("Sub-lead tau p_{T} [GeV]");
    h_Tau2Pt_LOOSE->GetYaxis()->SetTitle("# Entries");
    
    h_Tau2Pt_TIGHT = subDir.make<TH1D>("h_Tau2Pt_TIGHT","h_Tau2Pt_TIGHT",200, 0,200); 
    h_Tau2Pt_TIGHT->GetXaxis()->SetTitle("Sub-lead tau p_{T} [GeV]");
    h_Tau2Pt_TIGHT->GetYaxis()->SetTitle("# Entries");
    
    h_Jet1Pt_LOOSE = subDir.make<TH1D>("h_Jet1Pt_LOOSE","h_Jet1Pt_LOOSE",200, 0,200); 
    h_Jet1Pt_LOOSE->GetXaxis()->SetTitle("Lead jet p_{T} [GeV]");
    h_Jet1Pt_LOOSE->GetYaxis()->SetTitle("# Entries");
    
    h_Jet1Pt_TIGHT = subDir.make<TH1D>("h_Jet1Pt_TIGHT","h_Jet1Pt_TIGHT",200, 0,200); 
    h_Jet1Pt_TIGHT->GetXaxis()->SetTitle("Lead jet p_{T} [GeV]");
    h_Jet1Pt_TIGHT->GetYaxis()->SetTitle("# Entries");
      
    h_Jet2Pt_LOOSE = subDir.make<TH1D>("h_Jet2Pt_LOOSE","h_Jet2Pt_LOOSE",200, 0,200); 
    h_Jet2Pt_LOOSE->GetXaxis()->SetTitle("Sub-lead jet p_{T} [GeV]");
    h_Jet2Pt_LOOSE->GetYaxis()->SetTitle("# Entries");
      
    h_Jet2Pt_TIGHT = subDir.make<TH1D>("h_Jet2Pt_TIGHT","h_Jet2Pt_TIGHT",200, 0,200); 
    h_Jet2Pt_TIGHT->GetXaxis()->SetTitle("Sub-lead jet p_{T} [GeV]");
    h_Jet2Pt_TIGHT->GetYaxis()->SetTitle("# Entries");
    
    h_HPtRes = subDir.make<TH1D>("h_HPtRes","h_HPtRes",100, -5,5); 
    h_HPtRes->GetXaxis()->SetTitle("(HPt - gen HPt)/(gen HPt)");
    h_HPtRes->GetYaxis()->SetTitle("Entries");

    h_VBFPtRes = subDir.make<TH1D>("h_VBFPtRes","h_VBFPtRes",100, -5,5); 
    h_VBFPtRes->GetXaxis()->SetTitle("(VBFPt - gen VBFPt)/(gen VBFPt)");
    h_VBFPtRes->GetYaxis()->SetTitle("Entries");
    
    h_VBFPlusHPtRes = subDir.make<TH1D>("h_VBFPlusHPtRes","h_VBFPlusHPtRes",100, -5,5); 
    h_VBFPlusHPtRes->GetXaxis()->SetTitle("(VBFPlusHPt - gen VBFPlusHPt)/(gen VBFPlusHPt)");
    h_VBFPlusHPtRes->GetYaxis()->SetTitle("Entries");
    
    h_HadHadVBF = subDir.make<TH1D>("h_HadHadVBF","h_HadHadVBF",2, 0,2);
    
    h_bothtausmatched = subDir.make<TH1D>("h_bothtausmatched","h_bothtausmatched",2, 0,2);

  }

    VariableHistograms::~VariableHistograms(){
        ;
  }
  
  int VariableHistograms::PreAnalysis(){
    return 0;
  }

  int VariableHistograms::Execute(TreeEvent *event){
      
      std::vector<ic::L1TTau*> l1taus = event->GetPtrVec<ic::L1TTau>("L1Taus");
      std::sort(l1taus.begin(),l1taus.end(),greater_Candidate());
      
      //offline 
      
      std::vector<PFJet*> const& jets = event->GetPtrVec<PFJet>(jets_label_);
      std::vector<Electron*> electrons = event->GetPtrVec<Electron>(electrons_label_);
      std::vector<Muon*> muons = event->GetPtrVec<Muon>(muons_label_);
      std::vector<Tau*> taus = event->GetPtrVec<Tau>(taus_label_);
      std::vector<Tau*> leadingtaus = event->GetPtrVec<Tau>(leadtaus_label);
      std::vector<Met*> met_vec = event->GetPtrVec<Met>(met_label_);

      n_jets_ = jets.size();
      n_electrons_ = electrons.size();
      n_muons_ = muons.size();
      n_taus_ = taus.size();

      double MET = met_vec[0]->vector().pt();
      h_OfflineMET->Fill(MET);
      
      double Mij=-1;
      double eta_gap=-1;
      double LeadJPt=-1;
      double SubLeadJPt=-1;

      if(n_jets_ > 1){
          
           Mij = (jets[0]->vector() + jets[1]->vector()).M();
           eta_gap = fabs(jets[0]->vector().Rapidity()-jets[1]->vector().Rapidity());
          
          if(jets[0]->vector().Pt() > 30 && jets[0]->vector().Pt() >30){
              h_OfflineMjjInv->Fill(Mij);
          
              h_OfflineEtaGap->Fill(eta_gap);
              
              if(jets[0]->vector().Pt() >30 && jets[0]->vector().Pt() >30 && eta_gap > 3.5) h_VBFMjj->Fill(Mij);
            
          }
          
          double phi_gap = fabs(jets[0]->vector().Phi()-jets[1]->vector().Phi());
          h_OfflineDeltaPhijj->Fill(phi_gap);

          double R_gap = sqrt(pow(jets[0]->vector().Phi()-jets[1]->vector().Phi(),2) + pow(jets[0]->vector().Rapidity()-jets[1]->vector().Rapidity(),2));
          h_OfflineDeltaRjj->Fill(R_gap);
          
          double JetsPtVectorSum = (jets[0]->vector()+jets[1]->vector()).Pt();
          h_JetsPtVectorSum->Fill(JetsPtVectorSum);
      
          double JetsPtScalarSum = (jets[0]->vector().Pt() +jets[1]->vector().Pt())/2;
          h_JetsPtScalarSum->Fill(JetsPtScalarSum);
          
          SubLeadJPt = jets[1]->vector().Pt();
          h_OfflineSubLeadJetPt->Fill(SubLeadJPt);

          double SubLeadJEta = jets[1]->vector().Rapidity();
          h_OfflineSubLeadJetEta->Fill(SubLeadJEta);
  
          double SubLeadJPhi = jets[1]->vector().Phi();
          h_OfflineSubLeadJetPhi->Fill(SubLeadJPhi);
      
      }
      
      if(n_jets_ > 0){
          
          LeadJPt = jets[0]->vector().Pt();
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
      
      bool tau1matched = false;
      bool tau2matched = false;
      
      for(unsigned i = 0; i < l1taus.size(); i++){
          if(std::fabs(l1taus[i]->vector().Rapidity()) <= 2.1 && l1taus[i]->isolation !=0 && l1taus[i]->vector().Pt() >= 28){
              double DeltaR1 = std::fabs(pow(l1taus[i]->vector().Phi() - lep1->phi(),2) + pow(l1taus[i]->vector().Rapidity() - lep1->eta(),2));
              if(DeltaR1 < 0.5) tau1matched = true;
              double DeltaR2 = std::fabs(pow(l1taus[i]->vector().Phi() - lep2->phi(),2) + pow(l1taus[i]->vector().Rapidity() - lep2->eta(),2));
              if(DeltaR2 < 0.5) tau2matched = true;
          }
      }
      
      h_bothtausmatched->Fill(0);
      if(tau1matched && tau2matched) h_bothtausmatched->Fill(1);
      
      double Mtt_Vis = (lep1->vector() + lep2->vector()).M();
      h_OfflineMttVis->Fill(Mtt_Vis);
      
      double Ptt = (met_vec[0]->vector() + lep1->vector() + lep2->vector()).Pt();
      
      bool VBFLoose = false;
      bool VBFTight = false;
      bool HadHadVBF = false;
      
      if(eta_gap > 4 && Mij > 700 && Ptt > 100 && LeadJPt > 30 && SubLeadJPt >30) VBFTight = true;
      else if (eta_gap > 3.5 && Mij > 500 && LeadJPt > 30 && SubLeadJPt >30) VBFLoose = true;
      
      if(VBFTight) h_mVis_TIGHT->Fill(Mtt_Vis);
      else if (VBFLoose) h_mVis_LOOSE->Fill(Mtt_Vis);
      
      if(VBFTight || (VBFLoose && Ptt > 100)) HadHadVBF = true; 
      h_HadHadVBF->Fill(HadHadVBF);
      
      if(n_jets_ > 1){
          double pTTMinusVecPt = Ptt - (jets[0]->vector() + jets[1]->vector()).Pt();
          double pTTMinusVecPtRes = pTTMinusVecPt /(jets[0]->vector() + jets[1]->vector()).Pt();
          h_pTT->Fill(Ptt);
          h_pTTMinusVecPt->Fill(pTTMinusVecPt);
          h_pTTMinusVecPtRes->Fill(pTTMinusVecPtRes);
          h_pTTMinusVecPtResVsDiJetPt->Fill((jets[0]->vector() + jets[1]->vector()).Pt(), pTTMinusVecPtRes);
          h_DiJetPTMinusPTTResVsPTT->Fill(Ptt, (Ptt - (jets[0]->vector() + jets[1]->vector()).Pt())/(jets[0]->vector() + jets[1]->vector()).Pt());
          if(VBFLoose){
              h_Jet1Pt_LOOSE->Fill(jets[0]->vector().Pt());
              h_Jet2Pt_LOOSE->Fill(jets[1]->vector().Pt());
          }
          if(VBFTight){
              h_Jet1Pt_TIGHT->Fill(jets[0]->vector().Pt());
              h_Jet2Pt_TIGHT->Fill(jets[1]->vector().Pt());
          }
      }

      double DeltaPhi_tt = fabs(lep1->vector().Phi() - lep2->vector().Phi());
      h_OfflineDeltaPhitt->Fill(DeltaPhi_tt);

      double DeltaEta_tt = fabs(lep1->vector().Rapidity() - lep2->vector().Rapidity());
      h_OfflineDeltaEtatt->Fill(DeltaEta_tt);

      double DeltaR_tt = sqrt(pow(lep1->vector().Rapidity() - lep2->vector().Rapidity(),2) + pow(lep1->vector().Phi() - lep2->vector().Phi(),2));
      h_OfflineDeltaRtt->Fill(DeltaR_tt);

      if (n_electrons_ > 0){
        double electronPt = electrons[0]->vector().pt();
        if (electronPt == pt_1_ || electronPt == pt_2_){
            h_OfflineElectronPt->Fill(electronPt);
            if(VBFLoose) h_ElecPt_LOOSE->Fill(electronPt);
            if(VBFTight) h_ElecPt_TIGHT->Fill(electronPt);
        }
      }

      if (n_muons_ > 0){
        double muonPt = muons[0]->vector().pt();
        if (muonPt == pt_1_ || muonPt == pt_2_){
            h_OfflineMuonPt->Fill(muonPt);
            if(VBFLoose) h_MuPt_LOOSE->Fill(muonPt);
            if(VBFTight) h_MuPt_TIGHT->Fill(muonPt);
        }
      }

      if (n_taus_ > 0){
        double tauPt = leadingtaus[0]->vector().pt();
        h_OfflineLeadTauPt->Fill(tauPt);
        if(VBFLoose) h_Tau1Pt_LOOSE->Fill(tauPt);
        if(VBFTight) h_Tau1Pt_TIGHT->Fill(tauPt);
      }

      if (taus.size() > 1){
        double SubtauPt = taus[1]->vector().pt();
        h_OfflineSubLeadTauPt->Fill(SubtauPt);
        if(VBFLoose) h_Tau2Pt_LOOSE->Fill(SubtauPt);
        if(VBFTight) h_Tau2Pt_TIGHT->Fill(SubtauPt);
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
      
     std::vector<ic::GenParticle*> genParticles = event->GetPtrVec<ic::GenParticle>("genParticles");
     
     ic::Candidate VBFJets;
     ic::Candidate Higgs;
     
     for(unsigned i=0; i< genParticles.size(); i++){
         
         if(i==5 || i==6) VBFJets.set_vector(VBFJets.vector() + genParticles[i]->vector());
         if(std::fabs(genParticles[i]->pdgid())==25){
             bool isHiggs = false;
             for(unsigned j=0; j< genParticles[i]->daughters().size(); j++){
                 if(std::fabs(genParticles[genParticles[i]->daughters().at(j)]->pdgid()) == 15) isHiggs=true;
             }
             if(isHiggs) Higgs.set_vector(genParticles[i]->vector());
         }
     }
     
     double GenHPt = Higgs.vector().Pt();
     //double GenVBFPt = VBFJets.vector().Pt();
     double GenVBFPlusHPt = (Higgs.vector()+VBFJets.vector()).Pt();
     double HPt = (lep1->vector() + lep2->vector() + met_vec[0]->vector()).Pt();
     
     h_HPtRes->Fill((HPt-GenHPt)/GenHPt);
     
     if(n_jets_>=2){
         double VBFPt = (jets[0]->vector() +jets[1]->vector()).Pt();
         double VBFPlusHPt = (jets[0]->vector() +jets[1]->vector() + lep1->vector() + lep2->vector() + met_vec[0]->vector()).Pt();
         
         h_VBFPtRes->Fill((VBFPt-GenHPt)/GenHPt);
         h_VBFPlusHPtRes->Fill((VBFPlusHPt-GenVBFPlusHPt)/GenVBFPlusHPt);
     }
      
     /* ic::EventInfo const* eventInfo = event->GetPtr<ic::EventInfo>("eventInfo");
      
      std::ofstream outfile1;
      outfile1.open("PassedOffline.dat",std::ios_base::app);
      
      outfile1 << eventInfo->event() << '\n';
      
      outfile1.close();*/
    
      
      return 0;
  }

  int VariableHistograms::PostAnalysis(){
    return 0;
  }

  void VariableHistograms::PrintInfo(){
    ;
  }
  



}//namespace


