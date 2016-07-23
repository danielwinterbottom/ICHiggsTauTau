#include "Modules/interface/EfficiencyGenMatch.h"
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

  
  EfficiencyGenMatch::EfficiencyGenMatch(std::string const& name, 
                                          fwlite::TFileService *fs, std::string output_name, int effNum, std::string channel, struct L1Cuts l1Cuts) : ModuleBase(name) {
    
    EffNum = effNum;
    if(EffNum == 1){
        l1eCut = 10;
        l1muCut = 5;
        l1tauCut = 10;
        l1jetCut = 30;
        l1MjjCut = 400;
        l1DeltaEtaCut = 2.5;
        l1METCut = 70;
        AvePtCut = 30;
        VecPtCut = 40;
    }
    else if(EffNum == 2){
        l1eCut = 15;
        l1muCut = 10;
        l1tauCut = 20;
        l1jetCut = 50;
        l1MjjCut = 500;
        l1DeltaEtaCut = 3.0;
        l1METCut = 80;
        AvePtCut = 40;
        VecPtCut = 50;
    }
    else if(EffNum == 3){
        l1eCut = 20;
        l1muCut = 15;
        l1tauCut = 30;
        l1jetCut = 90;
        l1MjjCut = 600;
        l1DeltaEtaCut = 3.5;
        l1METCut = 90;
        AvePtCut = 50;
        VecPtCut = 60;
    }
    else if(EffNum == 4){
        l1eCut = 0;
        l1muCut = 0;
        l1tauCut = 0;
        l1jetCut = 0;
        l1MjjCut = 0;
        l1DeltaEtaCut = 0;
        l1METCut = 0;
        AvePtCut = 0;
        VecPtCut = 0;
    }
    
    else if(EffNum == 5){
        l1eCut = l1Cuts.EGPt;
        l1muCut = l1Cuts.MuPt;
        l1tauCut = l1Cuts.Tau1Pt;
        l1jetCut = l1Cuts.Jet1Pt;
        l1MjjCut = l1Cuts.Mjj;
        l1DeltaEtaCut = l1Cuts.DeltaEta;
        l1METCut = 40;
    }
    
    nPromptElectrons = 1000;
    nPromptMuons = 1000;
    nPromptTaus = 1000;
                                              
    genParticles_label_ = "genParticles";
    jets_label_ = "ak4PFJetsCHS"; 
    electrons_label_ = "electrons";
    muons_label_ = "muons"; 
    taus_label_ = "taus";  
    met_label_ = "pfMVAMet";
    l1jets_label_ = "L1Jets"; 
    l1electrons_label_ = "L1EGammas";
    l1muons_label_ = "L1Muons"; 
    l1taus_label_ = "L1Taus";  
    l1metNoHF_label_ = "L1Sums";
    l1met_label_ = "L1SumsEtaRange5pt0";
    genjets_label_ = "genJets";
    channel_ = channel;
    
    TFileDirectory subDir = fs->mkdir(output_name.c_str());
    
    h_mu_Mu_Efficiency = subDir.make<TH1D>("h_mu_Mu_Efficiency","h_mu_Mu_Efficiency",100, 0,100); 
    h_mu_Mu_Efficiency->GetXaxis()->SetTitle("Muon p_{T} [GeV] ");
    h_mu_Mu_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_mu_IsoMu_Efficiency = subDir.make<TH1D>("h_mu_IsoMu_Efficiency","h_mu_IsoMu_Efficiency",100, 0,100); 
    h_mu_IsoMu_Efficiency->GetXaxis()->SetTitle("Muon p_{T} [GeV] ");
    h_mu_IsoMu_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_mu_Total = subDir.make<TH1D>("h_mu_Total","h_mu_Total",100, 0,100);
    h_mu_Total->GetXaxis()->SetTitle("Muon p_{T} [GeV] ");
    h_mu_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_genmu_Mu_Efficiency = subDir.make<TH1D>("h_genmu_Mu_Efficiency","h_genmu_Mu_Efficiency",100, 0,100); 
    h_genmu_Mu_Efficiency->GetXaxis()->SetTitle("Gen-Muon p_{T} [GeV] ");
    h_genmu_Mu_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_genmu_IsoMu_Efficiency = subDir.make<TH1D>("h_genmu_IsoMu_Efficiency","h_genmu_IsoMu_Efficiency",100, 0,100); 
    h_genmu_IsoMu_Efficiency->GetXaxis()->SetTitle("Gen-Muon p_{T} [GeV] ");
    h_genmu_IsoMu_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_genmu_Total = subDir.make<TH1D>("h_genmu_Total","h_genmu_Total",100, 0,100);
    h_genmu_Total->GetXaxis()->SetTitle("Gen-Muon p_{T} [GeV] ");
    h_genmu_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_mu_Mu_PtRes = subDir.make<TH1D>("h_mu_Mu_PtRes","h_mu_Mu_PtRes",100, -1,2); 
    h_mu_Mu_PtRes->GetXaxis()->SetTitle("Muon (L1 p_{T} - Gen p_{T})/(Gen p_{T}) ");
    h_mu_Mu_PtRes->GetYaxis()->SetTitle("Entries");
    
    h_mu_Mu_PtDiff = subDir.make<TH1D>("h_mu_Mu_PtDiff","h_mu_Mu_PtDiff",100, -10,30); 
    h_mu_Mu_PtDiff->GetXaxis()->SetTitle("Muon (L1 p_{T} - Gen p_{T}) [GeV] ");
    h_mu_Mu_PtDiff->GetYaxis()->SetTitle("Entries");
    
    h_mu_Mu_DeltaRRes = subDir.make<TH1D>("h_mu_Mu_DeltaRRes","h_mu_Mu_DeltaRRes",100, 0,0.3); 
    h_mu_Mu_DeltaRRes->GetXaxis()->SetTitle("L1-Gen Muon |#DeltaR| ");
    h_mu_Mu_DeltaRRes->GetYaxis()->SetTitle("Entries");
    
    h_e_EG_Efficiency = subDir.make<TH1D>("h_e_EG_Efficiency","h_e_EG_Efficiency",100, 0,100); 
    h_e_EG_Efficiency->GetXaxis()->SetTitle("Electron p_{T} [GeV] ");
    h_e_EG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_e_IsoEG_Efficiency = subDir.make<TH1D>("h_e_IsoEG_Efficiency","h_e_IsoEG_Efficiency",100, 0,100); 
    h_e_IsoEG_Efficiency->GetXaxis()->SetTitle("Electron p_{T} [GeV] ");
    h_e_IsoEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_e_Total = subDir.make<TH1D>("h_e_Total","h_e_Total",100, 0,100); 
    h_e_Total->GetXaxis()->SetTitle("Electron p_{T} [GeV] ");
    h_e_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_e_Tau_Efficiency = subDir.make<TH1D>("h_e_Tau_Efficiency","h_e_Tau_Efficiency",100, 0,100);
    h_e_Tau_Efficiency->GetXaxis()->SetTitle("Electron p_{T} [GeV] ");
    h_e_Tau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_e_IsoTau_Efficiency = subDir.make<TH1D>("h_e_IsoTau_Efficiency","h_e_IsoTau_Efficiency",100, 0,100);
    h_e_IsoTau_Efficiency->GetXaxis()->SetTitle("Electron p_{T} [GeV] ");
    h_e_IsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_e_Jet_Efficiency = subDir.make<TH1D>("h_e_Jet_Efficiency","h_e_Jet_Efficiency",100, 0,100);
    h_e_Jet_Efficiency->GetXaxis()->SetTitle("Electron p_{T} [GeV] ");
    h_e_Jet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_e_EG_PtRes = subDir.make<TH1D>("h_e_EG_PtRes","h_e_EG_PtRes",100, -1,1); 
    h_e_EG_PtRes->GetXaxis()->SetTitle("Electron (L1 p_{T} - Gen p_{T})/(Gen p_{T}) ");
    h_e_EG_PtRes->GetYaxis()->SetTitle("Entries");
    
    h_e_EG_PtDiff = subDir.make<TH1D>("h_e_EG_PtDiff","h_e_EG_PtDiff",100, -10,10); 
    h_e_EG_PtDiff->GetXaxis()->SetTitle("Electron (L1 p_{T} - Gen p_{T}) [GeV] ");
    h_e_EG_PtDiff->GetYaxis()->SetTitle("Entries");
    
    h_e_EG_DeltaRRes = subDir.make<TH1D>("h_e_EG_DeltaRRes","h_e_EG_DeltaRRes",100, 0,0.2); 
    h_e_EG_DeltaRRes->GetXaxis()->SetTitle("L1-Gen Electron |#DeltaR| ");
    h_e_EG_DeltaRRes->GetYaxis()->SetTitle("Entries");
    
    h_gene_EG_Efficiency = subDir.make<TH1D>("h_gene_EG_Efficiency","h_gene_EG_Efficiency",100, 0,100); 
    h_gene_EG_Efficiency->GetXaxis()->SetTitle("Gen-Electron p_{T} [GeV] ");
    h_gene_EG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_gene_EGTau_Efficiency = subDir.make<TH1D>("h_gene_EGTau_Efficiency","h_gene_EGTau_Efficiency",100, 0,100); 
    h_gene_EGTau_Efficiency->GetXaxis()->SetTitle("Gen-Electron p_{T} [GeV] ");
    h_gene_EGTau_Efficiency->GetYaxis()->SetTitle("Efficiency");

    h_gene_EGJet_Efficiency = subDir.make<TH1D>("h_gene_EGJet_Efficiency","h_gene_EGJet_Efficiency",100, 0,100); 
    h_gene_EGJet_Efficiency->GetXaxis()->SetTitle("Gen-Electron p_{T} [GeV] ");
    h_gene_EGJet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_gene_EGTauJet_Efficiency = subDir.make<TH1D>("h_gene_EGTauJet_Efficiency","h_gene_EGTauJet_Efficiency",100, 0,100); 
    h_gene_EGTauJet_Efficiency->GetXaxis()->SetTitle("Gen-Electron p_{T} [GeV] ");
    h_gene_EGTauJet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_gene_IsoEGIsoTau_Efficiency = subDir.make<TH1D>("h_gene_IsoEGIsoTau_Efficiency","h_gene_IsoEGIsoTau_Efficiency",100, 0,100); 
    h_gene_IsoEGIsoTau_Efficiency->GetXaxis()->SetTitle("Gen-Electron p_{T} [GeV] ");
    h_gene_IsoEGIsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");

    h_gene_IsoEGJet_Efficiency = subDir.make<TH1D>("h_gene_IsoEGJet_Efficiency","h_gene_IsoEGJet_Efficiency",100, 0,100); 
    h_gene_IsoEGJet_Efficiency->GetXaxis()->SetTitle("Gen-Electron p_{T} [GeV] ");
    h_gene_IsoEGJet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_gene_IsoEGIsoTauJet_Efficiency = subDir.make<TH1D>("h_gene_IsoEGIsoTauJet_Efficiency","h_gene_IsoEGIsoTauJet_Efficiency",100, 0,100); 
    h_gene_IsoEGIsoTauJet_Efficiency->GetXaxis()->SetTitle("Gen-Electron p_{T} [GeV] ");
    h_gene_IsoEGIsoTauJet_Efficiency->GetYaxis()->SetTitle("Efficiency");

    //
    h_gene_IsoEG_Efficiency = subDir.make<TH1D>("h_gene_IsoEG_Efficiency","h_gene_IsoEG_Efficiency",100, 0,100); 
    h_gene_IsoEG_Efficiency->GetXaxis()->SetTitle("Gen-Electron p_{T} [GeV] ");
    h_gene_IsoEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_gene_Total = subDir.make<TH1D>("h_gene_Total","h_gene_Total",100, 0,100); 
    h_gene_Total->GetXaxis()->SetTitle("Gen-Electron p_{T} [GeV] ");
    h_gene_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_EG_Efficiency = subDir.make<TH1D>("h_tau_EG_Efficiency","h_tau_EG_Efficiency",200, 0,200); 
    h_tau_EG_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_EG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_IsoEG_Efficiency = subDir.make<TH1D>("h_tau_IsoEG_Efficiency","h_tau_IsoEG_Efficiency",200, 0,200); 
    h_tau_IsoEG_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_IsoEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_Mu_Efficiency = subDir.make<TH1D>("h_tau_Mu_Efficiency","h_tau_Mu_Efficiency",200, 0,200);
    h_tau_Mu_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_Mu_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_subtau_Total = subDir.make<TH1D>("h_subtau_Total","h_subtau_Total",200, 0,200); 
    h_subtau_Total->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_subtau_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_Tau_Efficiency = subDir.make<TH1D>("h_tau_Tau_Efficiency","h_tau_Tau_Efficiency",200, 0,200);
    h_tau_Tau_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_Tau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_subtau_Tau_Efficiency = subDir.make<TH1D>("h_subtau_Tau_Efficiency","h_subtau_Tau_Efficiency",200, 0,200);
    h_subtau_Tau_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_subtau_Tau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_IsoTau_Efficiency = subDir.make<TH1D>("h_tau_IsoTau_Efficiency","h_tau_IsoTau_Efficiency",200, 0,200);
    h_tau_IsoTau_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_IsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_subtau_IsoTau_Efficiency = subDir.make<TH1D>("h_subtau_IsoTau_Efficiency","h_subtau_IsoTau_Efficiency",200, 0,200);
    h_subtau_IsoTau_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_subtau_IsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_Jet_Efficiency = subDir.make<TH1D>("h_tau_Jet_Efficiency","h_tau_Jet_Efficiency",200, 0,200);
    h_tau_Jet_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV]");
    h_tau_Jet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_Tau_PtDiff = subDir.make<TH1D>("h_tau_Tau_PtDiff","h_tau_Tau_PtDiff",100, -50,20); 
    h_tau_Tau_PtDiff->GetXaxis()->SetTitle("Tau (L1 p_{T} - Gen p_{T}) [GeV] ");
    h_tau_Tau_PtDiff->GetYaxis()->SetTitle("Entries");
    
    h_tau_Tau_PtRes = subDir.make<TH1D>("h_tau_Tau_PtRes","h_tau_Tau_PtRes",100, -2,2); 
    h_tau_Tau_PtRes->GetXaxis()->SetTitle("Tau (L1 p_{T} - Gen p_{T})/(Gen p_{T}) ");
    h_tau_Tau_PtRes->GetYaxis()->SetTitle("Entries");
    
    h_tau_Tau_DeltaRRes = subDir.make<TH1D>("h_tau_Tau_DeltaRRes","h_tau_Tau_DeltaRRes",100, 0,0.3);
    h_tau_Tau_DeltaRRes->GetXaxis()->SetTitle("L1-Gen Tau |#DeltaR| ");
    h_tau_Tau_DeltaRRes->GetYaxis()->SetTitle("Entries");
    
    h_gentau_Total = subDir.make<TH1D>("h_gentau_Total","h_gentau_Total",100, 0,100); 
    h_gentau_Total->GetXaxis()->SetTitle("Gen-Tau p_{T} [GeV] ");
    h_gentau_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_gentau_Tau_Efficiency = subDir.make<TH1D>("h_gentau_Tau_Efficiency","h_gentau_Tau_Efficiency",100, 0,100);
    h_gentau_Tau_Efficiency->GetXaxis()->SetTitle("Gen-Tau p_{T} [GeV] ");
    h_gentau_Tau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_gentau_IsoTau_Efficiency = subDir.make<TH1D>("h_gentau_IsoTau_Efficiency","h_gentau_IsoTau_Efficiency",100, 0,100);
    h_gentau_IsoTau_Efficiency->GetXaxis()->SetTitle("Gen-Tau p_{T} [GeV] ");
    h_gentau_IsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    ///
    h_gentau_TauEG_Efficiency = subDir.make<TH1D>("h_gentau_TauEG_Efficiency","h_gentau_TauEG_Efficiency",100, 0,100);
    h_gentau_TauEG_Efficiency->GetXaxis()->SetTitle("Gen-Tau p_{T} [GeV] ");
    h_gentau_TauEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_gentau_TauJet_Efficiency = subDir.make<TH1D>("h_gentau_TauJet_Efficiency","h_gentau_TauJet_Efficiency",100, 0,100);
    h_gentau_TauJet_Efficiency->GetXaxis()->SetTitle("Gen-Tau p_{T} [GeV] ");
    h_gentau_TauJet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_gentau_TauEGJet_Efficiency = subDir.make<TH1D>("h_gentau_TauEGJet_Efficiency","h_gentau_TauEGJet_Efficiency",100, 0,100);
    h_gentau_TauEGJet_Efficiency->GetXaxis()->SetTitle("Gen-Tau p_{T} [GeV] ");
    h_gentau_TauEGJet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_gentau_IsoTauIsoEG_Efficiency = subDir.make<TH1D>("h_gentau_IsoTauIsoEG_Efficiency","h_gentau_IsoTauIsoEG_Efficiency",100, 0,100);
    h_gentau_IsoTauIsoEG_Efficiency->GetXaxis()->SetTitle("Gen-Tau p_{T} [GeV] ");
    h_gentau_IsoTauIsoEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_gentau_IsoTauJet_Efficiency = subDir.make<TH1D>("h_gentau_IsoTauJet_Efficiency","h_gentau_IsoTauJet_Efficiency",100, 0,100);
    h_gentau_IsoTauJet_Efficiency->GetXaxis()->SetTitle("Gen-Tau p_{T} [GeV] ");
    h_gentau_IsoTauJet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_gentau_IsoTauIsoEGJet_Efficiency = subDir.make<TH1D>("h_gentau_IsoTauIsoEGJet_Efficiency","h_gentau_IsoTauIsoEGJet_Efficiency",100, 0,100);
    h_gentau_IsoTauIsoEGJet_Efficiency->GetXaxis()->SetTitle("Gen-Tau p_{T} [GeV] ");
    h_gentau_IsoTauIsoEGJet_Efficiency->GetYaxis()->SetTitle("Efficiency");    
    ///
    h_jet_EG_Efficiency = subDir.make<TH1D>("h_jet_EG_Efficiency","h_jet_EG_Efficiency",200, 0,200); 
    h_jet_EG_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_EG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_IsoEG_Efficiency = subDir.make<TH1D>("h_jet_IsoEG_Efficiency","h_jet_IsoEG_Efficiency",200, 0,200);
    h_jet_IsoEG_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_IsoEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_Mu_Efficiency = subDir.make<TH1D>("h_jet_Mu_Efficiency","h_jet_Mu_Efficiency",200, 0,200); 
    h_jet_Mu_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_Mu_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_Total = subDir.make<TH1D>("h_jet_Total","h_jet_Total",200, 0,200);
    h_jet_Total->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_subjet_Total = subDir.make<TH1D>("h_subjet_Total","h_subjet_Total",200, 0,200);
    h_subjet_Total->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_subjet_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_Tau_Efficiency = subDir.make<TH1D>("h_jet_Tau_Efficiency","h_jet_Tau_Efficiency",200, 0,200);
    h_jet_Tau_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_Tau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_IsoTau_Efficiency = subDir.make<TH1D>("h_jet_IsoTau_Efficiency","h_jet_IsoTau_Efficiency",200, 0,200); 
    h_jet_IsoTau_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_IsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_Jet_Efficiency = subDir.make<TH1D>("h_jet_Jet_Efficiency","h_jet_Jet_Efficiency",200, 0,200);
    h_jet_Jet_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_jet_Jet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_genjet_Jet_Efficiency = subDir.make<TH1D>("h_genjet_Jet_Efficiency","h_genjet_Jet_Efficiency",100, 0,100);
    h_genjet_Jet_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_genjet_Jet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_genjet_EG_Efficiency = subDir.make<TH1D>("h_genjet_EG_Efficiency","h_genjet_EG_Efficiency",100, 0,100);
    h_genjet_EG_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_genjet_EG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_genjet_Tau_Efficiency = subDir.make<TH1D>("h_genjet_Tau_Efficiency","h_genjet_Tau_Efficiency",100, 0,100);
    h_genjet_Tau_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_genjet_Tau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_genjet_TauEG_Efficiency = subDir.make<TH1D>("h_genjet_TauEG_Efficiency","h_genjet_TauEG_Efficiency",100, 0,100);
    h_genjet_TauEG_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_genjet_TauEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_genjet_IsoEG_Efficiency = subDir.make<TH1D>("h_genjet_IsoEG_Efficiency","h_genjet_IsoEG_Efficiency",100, 0,100);
    h_genjet_IsoEG_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_genjet_IsoEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_genjet_IsoTau_Efficiency = subDir.make<TH1D>("h_genjet_IsoTau_Efficiency","h_genjet_IsoTau_Efficiency",100, 0,100);
    h_genjet_IsoTau_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_genjet_IsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_genjet_IsoTauIsoEG_Efficiency = subDir.make<TH1D>("h_genjet_IsoTauIsoEG_Efficiency","h_genjet_IsoTauIsoEG_Efficiency",100, 0,100);
    h_genjet_IsoTauIsoEG_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_genjet_IsoTauIsoEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_genjet_Total = subDir.make<TH1D>("h_genjet_Total","h_genjet_Total",100, 0,100);
    h_genjet_Total->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_genjet_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_subjet_Jet_Efficiency = subDir.make<TH1D>("h_subjet_Jet_Efficiency","h_subjet_Jet_Efficiency",100, 0,100);
    h_subjet_Jet_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_subjet_Jet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_TauEG_Efficiency = subDir.make<TH1D>("h_jet_TauEG_Efficiency","h_jet_TauEG_Efficiency",200, 0,200); 
    h_jet_TauEG_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_jet_TauEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_IsoTauIsoEG_Efficiency = subDir.make<TH1D>("h_jet_IsoTauIsoEG_Efficiency","h_jet_IsoTauIsoEG_Efficiency",200, 0,200); 
    h_jet_IsoTauIsoEG_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_jet_IsoTauIsoEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_Jet_PtRes = subDir.make<TH1D>("h_jet_Jet_PtRes","h_jet_Jet_PtRes",100, -1,2);
    h_jet_Jet_PtRes->GetXaxis()->SetTitle("Jet (L1 p_{T} - Gen p_{T})/(Gen p_{T}) ");
    h_jet_Jet_PtRes->GetYaxis()->SetTitle("Entries");
    
    h_jet_Jet_PtDiff = subDir.make<TH1D>("h_jet_Jet_PtDiff","h_jet_Jet_PtDiff",100, -30,50); 
    h_jet_Jet_PtDiff->GetXaxis()->SetTitle("Jet (L1 p_{T} - Gen p_{T}) [GeV] ");
    h_jet_Jet_PtDiff->GetYaxis()->SetTitle("Entries");
    
    h_jet_Jet_DeltaRRes = subDir.make<TH1D>("h_jet_Jet_DeltaRRes","h_jet_Jet_DeltaRRes",100, 0,0.5); 
    h_jet_Jet_DeltaRRes->GetXaxis()->SetTitle("L1-Gen Jet |#DeltaR| ");
    h_jet_Jet_DeltaRRes->GetYaxis()->SetTitle("Entries");
    
    h_jetjet_DeltaEta_Efficiency = subDir.make<TH1D>("h_jetjet_DeltaEta_Efficiency","h_jetjet_DeltaEta_Efficiency",100, 0,5); 
    h_jetjet_DeltaEta_Efficiency->GetXaxis()->SetTitle("#Delta#eta");
    h_jetjet_DeltaEta_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jetjet_DeltaEta_Total = subDir.make<TH1D>("h_jetjet_DeltaEta_Total","h_jetjet_DeltaEta_Total",100, 0,5); 
    h_jetjet_DeltaEta_Total->GetXaxis()->SetTitle("#Delta#eta");
    h_jetjet_DeltaEta_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_jetjet_Mjj_Efficiency = subDir.make<TH1D>("h_jetjet_Mjj_Efficiency","h_jetjet_Mjj_Efficiency",100, 0,1000); 
    h_jetjet_Mjj_Efficiency->GetXaxis()->SetTitle("m_{jj} [GeV]");
    h_jetjet_Mjj_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jetjet_Mjj_Total = subDir.make<TH1D>("h_jetjet_Mjj_Total","h_jetjet_Mjj_Total",100, 0,1000);
    h_jetjet_Mjj_Total->GetXaxis()->SetTitle("m_{jj} [GeV]");
    h_jetjet_Mjj_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_jetjet_AvePt_Efficiency = subDir.make<TH1D>("h_jetjet_AvePt_Efficiency","h_jetjet_AvePt_Efficiency",100, 0,200); 
    h_jetjet_AvePt_Efficiency->GetXaxis()->SetTitle("Jets <p_{T}>_{jj} [GeV]");
    h_jetjet_AvePt_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jetjet_AvePt_Total = subDir.make<TH1D>("h_jetjet_AvePt_Total","h_jetjet_AvePt_Total",100, 0,200);
    h_jetjet_AvePt_Total->GetXaxis()->SetTitle("Jets <p_{T}>_{jj} [GeV]");
    h_jetjet_AvePt_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_jetjet_VecPt_Efficiency = subDir.make<TH1D>("h_jetjet_VecPt_Efficiency","h_jetjet_VecPt_Efficiency",100, 0,200); 
    h_jetjet_VecPt_Efficiency->GetXaxis()->SetTitle("Jets Vec p_{T} [GeV]");
    h_jetjet_VecPt_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jetjet_VecPt_Total = subDir.make<TH1D>("h_jetjet_VecPt_Total","h_jetjet_VecPt_Total",100, 0,200);
    h_jetjet_VecPt_Total->GetXaxis()->SetTitle("Jets Vec p_{T} [GeV]");
    h_jetjet_VecPt_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_METHF_Efficiency = subDir.make<TH1D>("h_METHF_Efficiency","h_METHF_Efficiency",200, 0,400); 
    h_METHF_Efficiency->GetXaxis()->SetTitle("MET (inc HF) [GeV]");
    h_METHF_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_METNoHF_Efficiency = subDir.make<TH1D>("h_METNoHF_Efficiency","h_METNoHF_Efficiency",200, 0,400);
    h_METNoHF_Efficiency->GetXaxis()->SetTitle("MET (no HF) [GeV]");
    h_METNoHF_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_METHF_Total = subDir.make<TH1D>("h_METHF_Total","h_METHF_Total",200, 0,400); 
    h_METHF_Total->GetXaxis()->SetTitle("MET (no HF) [GeV]");
    h_METHF_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_METNoHF_Total = subDir.make<TH1D>("h_METNoHF_Total","h_METNoHF_Total",200, 0,400); 
    h_METNoHF_Total->GetXaxis()->SetTitle("MET (no HF) [GeV]");
    h_METNoHF_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_Total = subDir.make<TH1D>("h_tau_Total","h_tau_Total",200, 0,200); 
    h_tau_Total->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_Total->GetYaxis()->SetTitle("Efficiency");
    
  }

    EfficiencyGenMatch::~EfficiencyGenMatch(){
    ;
  }
  
  int EfficiencyGenMatch::PreAnalysis(){
      return 0;
  }

  int EfficiencyGenMatch::Execute(TreeEvent *event){

      
      std::vector<ic::GenJet*> genjets = event->GetPtrVec<ic::GenJet>(genjets_label_);
      
      std::vector<ic::L1TJet*>  l1jets = event->GetPtrVec<ic::L1TJet>(l1jets_label_);
      std::vector<ic::L1TEGamma*> l1electrons = event->GetPtrVec<ic::L1TEGamma>(l1electrons_label_);
      std::vector<ic::L1TMuon*> l1muons = event->GetPtrVec<ic::L1TMuon>(l1muons_label_);
      std::vector<ic::L1TTau*> l1taus = event->GetPtrVec<ic::L1TTau>(l1taus_label_);
      std::vector<ic::L1TSum*> l1met_vec = event->GetPtrVec<ic::L1TSum>(l1met_label_);
      std::vector<ic::L1TSum*> l1sums = event->GetPtrVec<ic::L1TSum>(l1metNoHF_label_);
      
      std::sort(l1taus.begin(),l1taus.end(),greater_Candidate());
      std::sort(l1electrons.begin(),l1electrons.end(),greater_Candidate());
      std::sort(l1muons.begin(),l1muons.end(),greater_Candidate());
      std::sort(l1jets.begin(),l1jets.end(),greater_Candidate());
      
      n_l1jets_ = l1jets.size();
      n_l1electrons_ = l1electrons.size();
      n_l1muons_ = l1muons.size();
      n_l1taus_ = l1taus.size();
      
      std::vector<ic::GenParticle*> GenParticles = event->GetPtrVec<ic::GenParticle>(genParticles_label_);
      std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
      std::vector<Electron*> electrons = event->GetPtrVec<Electron>(electrons_label_);
      std::vector<Muon*> muons = event->GetPtrVec<Muon>(muons_label_);
      std::vector<Tau*> taus = event->GetPtrVec<Tau>(taus_label_);
      std::vector<ic::Met*> met_vec = event->GetPtrVec<ic::Met>(met_label_);
      
      n_jets_ = jets.size();
      n_electrons_ = electrons.size();
      n_muons_ = muons.size();
      n_taus_ = taus.size();
      n_genParticles_ = GenParticles.size();
      n_genjets_ = genjets.size();   
      
      for(int i= n_l1muons_-1; i>=0; i--){
          if(l1muons[i]->quality < 12) l1muons.erase(l1muons.begin()+i);
      }
      n_l1muons_ = l1muons.size();
      
      // Jets
      
      for(unsigned i=0; i < n_jets_; i++){
                  
          bool jetProceed = false;

          
          for(unsigned j=5; j< 7 && j< n_genParticles_; j++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-GenParticles[j]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
              if(DeltaR < 0.5) jetProceed = true;
          }
          
          if((std::fabs(jets[i]->vector().Rapidity()) > 4.7)) jetProceed = false;

          if(jetProceed){

              h_jet_Total->Fill(jets[i]->vector().Pt());
              
              // Check if jet fired EG trigger.
              
              bool MatchedL1 = false;
              bool MatchedL1Iso = false;
          
              for(unsigned j=0; j < n_l1electrons_; j++){
              
                  double DeltaR = sqrt(pow(l1electrons[j]->vector().Phi()-jets[i]->vector().Phi(),2) + pow(l1electrons[j]->vector().Rapidity()-jets[i]->vector().Rapidity(),2));
                  
                  bool L1EPtFired = false;
                  double L1EPt = l1electrons[j]->vector().Pt();
                  if(L1EPt >= l1eCut) L1EPtFired = true;
                  
                  bool isIso = false;
                  if(l1electrons[j]->isolation != 0) isIso = true;
              
                  if(DeltaR < 0.5 && L1EPtFired) MatchedL1 = true;
                  if(DeltaR < 0.5 && L1EPtFired && isIso) MatchedL1Iso = true;
              }
              
              if(MatchedL1) h_jet_EG_Efficiency->Fill(jets[i]->vector().Pt());
              if(MatchedL1Iso) h_jet_IsoEG_Efficiency->Fill(jets[i]->vector().Pt());
              
             
              // Check if jet fired tau and iso-tau triggers.
              
              bool MatchedTauL1 = false;
              bool MatchedIsoTauL1 = false;
          
              for(unsigned j=0; j < n_l1taus_; j++){
              
                  double DeltaR = sqrt(pow(l1taus[j]->vector().Phi()-jets[i]->vector().Phi(),2) + pow(l1taus[j]->vector().Rapidity()-jets[i]->vector().Rapidity(),2));

                  int tauIso = l1taus[j]->isolation;
                  bool IsoTau = false;
                  if(tauIso != 0) IsoTau = true;
                  
                  bool L1TauPtFired = false;
                  double L1TauPt = l1taus[j]->vector().Pt();
                  if(L1TauPt >= l1tauCut) L1TauPtFired = true;
                  
                  if(DeltaR < 0.5 && L1TauPtFired) MatchedTauL1 = true;
                  
                  if(DeltaR < 0.5 && L1TauPtFired && IsoTau) MatchedIsoTauL1 = true;
              }
              
              if(MatchedTauL1) h_jet_Tau_Efficiency->Fill(jets[i]->vector().Pt());
              
              if(MatchedIsoTauL1) h_jet_IsoTau_Efficiency->Fill(jets[i]->vector().Pt());
              
              if(MatchedTauL1 && MatchedL1) h_jet_TauEG_Efficiency->Fill(jets[i]->vector().Pt());
              
              if(MatchedIsoTauL1 && MatchedL1Iso) h_jet_IsoTauIsoEG_Efficiency->Fill(jets[i]->vector().Pt());

              // Check if jet fired Jet trigger.

              bool MatchedJetL1 = false;
          
              for(unsigned j=0; j < n_l1jets_; j++){
              
                  double DeltaR = sqrt(pow(l1jets[j]->vector().Phi()- jets[i]->vector().Phi(),2) + pow(l1jets[j]->vector().Rapidity()-jets[i]->vector().Rapidity(),2));
                  
                  bool L1JetPtFired = false;
                  double L1JetPt = l1jets[j]->vector().Pt();
                  if(L1JetPt >= l1jetCut) L1JetPtFired = true;
              
                  if(DeltaR < 0.5 && L1JetPtFired) MatchedJetL1 = true;
                  
              }
              
              if(MatchedJetL1) h_jet_Jet_Efficiency->Fill(jets[i]->vector().Pt());
              
              // Check if jet fired Muon trigger.
              
              bool MatchedL1Mu = false;
          
              for(unsigned j=0; j < n_l1muons_; j++){
              
                  double DeltaR = sqrt(pow(l1muons[j]->vector().Phi()-jets[i]->vector().Phi(),2) + pow(l1muons[j]->vector().Rapidity()-jets[i]->vector().Rapidity(),2));
                  
                  bool L1MuPtFired = false;
                  double L1MuPt = l1muons[j]->vector().Pt();
                  if(L1MuPt >= l1muCut) L1MuPtFired = true;
              
                  if(DeltaR < 0.5 && L1MuPtFired) MatchedL1Mu = true;
                  
              }
              
              if(MatchedL1Mu) h_jet_Mu_Efficiency->Fill(jets[i]->vector().Pt());
          
          }
          
      }
      
      // Di-Jets
      
      for(unsigned i=0; i < n_jets_; i++){
          
          bool jet1Proceed = false;
          bool jet2Proceed = false;
          
          for(unsigned j=5; j< 7 && j< n_genParticles_; j++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-GenParticles[j]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
              if(DeltaR < 0.5) jet1Proceed = true;
          }
    
          for(unsigned j=0; j<n_jets_; j++){
              
              for(unsigned k=5;k< 7 && k< n_genParticles_; k++){
                  double DeltaR = sqrt(pow(jets[j]->vector().Phi()-GenParticles[k]->vector().Phi(),2) + pow(jets[j]->vector().Rapidity()-GenParticles[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.5) jet2Proceed = true;
              }
           
              if(i!=j && jet1Proceed && jet2Proceed && std::fabs(jets[i]->vector().Rapidity()) < 4.7 && std::fabs(jets[j]->vector().Rapidity()) < 4.7){
                  
                  if(jets[i]->vector().Pt() > 30 && jets[j]->vector().Pt() > 30){
                      
                      bool Matched1JetL1 = false;
                      double PtDiffjet1Jet = 10000;
                      int jet1JetL1Index = -1;
              
                      for(unsigned k=0; k < n_l1jets_; k++){

                          double DeltaR = sqrt(pow(l1jets[k]->vector().Phi()- jets[i]->vector().Phi(),2) + pow(l1jets[k]->vector().Rapidity()-jets[i]->vector().Rapidity(),2));
                          double PtDiff = std::fabs(l1jets[k]->vector().Pt() - jets[i]->vector().Pt());
                  
                          if(DeltaR < 0.5 && PtDiff < PtDiffjet1Jet){
                               PtDiffjet1Jet = PtDiff;
                               Matched1JetL1 = true;
                               jet1JetL1Index = k;
                          }
                      
                      }
                      
                      bool Matched2JetL1 = false;
                      double PtDiffjet2Jet = 10000;
                      int jet2JetL1Index = -1;
              
                      for(unsigned k=0; k < n_l1jets_; k++){
                          
                          double DeltaR = sqrt(pow(l1jets[k]->vector().Phi()- jets[j]->vector().Phi(),2) + pow(l1jets[k]->vector().Rapidity()-jets[j]->vector().Rapidity(),2));
                          double PtDiff = std::fabs(l1jets[k]->vector().Pt() - jets[j]->vector().Pt());
                  
                          if(DeltaR < 0.5 && PtDiff < PtDiffjet2Jet){
                               PtDiffjet2Jet = PtDiff;
                               Matched2JetL1 = true;
                               jet2JetL1Index = k;
                          }
                      
                      }
                      
                      //if(l1jets[jet1JetL1Index]->vector().Pt() >= 30 && l1jets[jet2JetL1Index]->vector().Pt() >= 30){
                        h_jetjet_Mjj_Total->Fill((jets[i]->vector() + jets[j]->vector()).M());
                        h_jetjet_DeltaEta_Total->Fill(std::fabs(jets[i]->vector().Rapidity() - jets[j]->vector().Rapidity()));
                        
                        double AvePt = (jets[i]->vector().Pt()+jets[j]->vector().Pt())/2;
                        double VecPt = (jets[i]->vector()+jets[j]->vector()).Pt();
                        h_jetjet_AvePt_Total->Fill(AvePt);
                        h_jetjet_VecPt_Total->Fill(VecPt);
                        
                        if(Matched1JetL1 && Matched2JetL1) {
                            
                            if(Matched1JetL1 && Matched2JetL1 && jet1JetL1Index != jet2JetL1Index){
                                double Mjj = (l1jets[jet1JetL1Index]->vector() + l1jets[jet2JetL1Index]->vector()).M(); 
                                if(Mjj >= l1MjjCut) h_jetjet_Mjj_Efficiency->Fill((jets[i]->vector() + jets[j]->vector()).M());
                                double DeltaEta = std::fabs(l1jets[jet1JetL1Index]->vector().Rapidity() - l1jets[jet2JetL1Index]->vector().Rapidity()); 
                                if(DeltaEta >= l1DeltaEtaCut) h_jetjet_DeltaEta_Efficiency->Fill(std::fabs(jets[i]->vector().Rapidity() - jets[j]->vector().Rapidity()));
                                double AvePtL1 = (l1jets[jet1JetL1Index]->vector().Pt()+l1jets[jet2JetL1Index]->vector().Pt())/2;
                                double VecPtL1 = (l1jets[jet1JetL1Index]->vector()+l1jets[jet2JetL1Index]->vector()).Pt();
                                if(AvePtL1 >= AvePtCut) h_jetjet_AvePt_Efficiency->Fill(AvePt);
                                if(VecPtL1 >= VecPtCut) h_jetjet_VecPt_Efficiency->Fill(VecPt);
                            }
                        }
                     //}
                  }
              }
          }
      }
 
      //MET 
      
      double METNoHF = -1;
      double METHF = -1;
      
      double MET = met_vec[0]->vector().Pt();
      
      for(unsigned i=0; i< l1sums.size(); i++){
          if(l1sums[i]->sumType==2) METNoHF = l1sums[i]->vector().Pt();    
      }
      
      for(unsigned i=0; i< l1met_vec.size(); i++){
          if(l1met_vec[i]->sumType==2) METHF = l1met_vec[i]->vector().Pt();    
      }

      h_METHF_Total->Fill(MET);
      if(METHF >= l1METCut) h_METHF_Efficiency->Fill(MET);
      
      h_METNoHF_Total->Fill(MET);
      if(METNoHF >= l1METCut) h_METNoHF_Efficiency->Fill(MET);
          
      
      //Gen Taus
      
      double SubLeadTauPt =-1;
      double LeadTauPt    =-1;
      
      for(unsigned j=0; j< n_genParticles_; j++){
        
          int genID = std::fabs(GenParticles[j]->pdgid());
          bool isPrompt = false;
          
          for(unsigned k=0; k < GenParticles[j]->mothers().size(); k++) {
              if(std::fabs(GenParticles[GenParticles[j]->mothers().at(k)]->pdgid()) == 25){
                  if(genID == 15) isPrompt = true;
              }
          }
          
          ic::Candidate tausVis;
          
          for(unsigned k=0; k < GenParticles[j]->daughters().size(); k++) {
              if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 11 || std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 13) isPrompt = false;
              if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 16) tausVis.set_vector(GenParticles[j]->vector() - GenParticles[GenParticles[j]->daughters().at(k)]->vector());
          }
          
          if(genID == 15 && isPrompt && std::fabs(tausVis.vector().Rapidity()) < 2.3){
              
              double DeltaRMax = 100000;
              double tauPt = -1;
              bool MatchedOffline = false;
              
              for(unsigned k=0; k< n_taus_; k++){
                  double DeltaR = sqrt(pow(taus[k]->vector().Phi()- tausVis.vector().Phi(),2) + pow(taus[k]->vector().Rapidity()-tausVis.vector().Rapidity(),2));
                  if(DeltaR < 0.5 && DeltaR < DeltaRMax){
                      DeltaRMax = DeltaR;
                      tauPt = taus[k]->vector().Pt();
                      MatchedOffline = true;
                  }
                  
              }
              
              if(tauPt > LeadTauPt){
                  SubLeadTauPt = LeadTauPt;
                  LeadTauPt = tauPt;
              }
              else if (tauPt > SubLeadTauPt) SubLeadTauPt = tauPt;
              
              h_gentau_Total->Fill(tausVis.vector().Pt());
              if(MatchedOffline) h_tau_Total->Fill(tauPt); //do this for all tau histograms!!!!!
              
              bool MatchedL1 = false;
              bool MatchedIsoL1 = false;
              bool MatchedEG = false;
              bool MatchedIsoEG = false;
              bool MatchedJet = false;

              
              for(unsigned k=0; k<n_l1taus_; k++){
                  double DeltaR = sqrt(pow(l1taus[k]->vector().Phi()- tausVis.vector().Phi(),2) + pow(l1taus[k]->vector().Rapidity()-tausVis.vector().Rapidity(),2));
                  double PtDiff = l1taus[k]->vector().Pt() - tausVis.vector().Pt();
                  if(DeltaR < 0.5 && l1taus[k]->vector().Pt() >= l1tauCut){
                      MatchedL1 = true;
                      if(l1taus[k]->isolation != 0) MatchedIsoL1 = true;
                      h_tau_Tau_DeltaRRes->Fill(DeltaR);
                      h_tau_Tau_PtDiff->Fill(PtDiff);
                      h_tau_Tau_PtRes->Fill(PtDiff/tausVis.vector().Pt());
                  }
              }
              
              for(unsigned k=0; k<n_l1electrons_; k++){
                  double DeltaR = sqrt(pow(l1electrons[k]->vector().Phi()- tausVis.vector().Phi(),2) + pow(l1electrons[k]->vector().Rapidity()-tausVis.vector().Rapidity(),2));
                  if(DeltaR < 0.5 && l1electrons[k]->vector().Pt() >= l1eCut){
                      MatchedEG = true;
                      if(l1electrons[k]->isolation != 0) MatchedIsoEG = true;
                  }
              }
              
              
              for(unsigned k=0; k<n_l1jets_; k++){
                  double DeltaR = sqrt(pow(l1jets[k]->vector().Phi()- tausVis.vector().Phi(),2) + pow(l1jets[k]->vector().Rapidity()-tausVis.vector().Rapidity(),2));
                  if(DeltaR < 0.5 && l1jets[k]->vector().Pt() >= l1jetCut) MatchedJet = true;
              }
              
              if(MatchedL1){
                  h_gentau_Tau_Efficiency->Fill(tausVis.vector().Pt());
                  if(MatchedOffline) h_tau_Tau_Efficiency->Fill(tauPt);
                  if(MatchedIsoL1){
                      h_gentau_IsoTau_Efficiency->Fill(tausVis.vector().Pt());
                      if(MatchedOffline) h_tau_IsoTau_Efficiency->Fill(tauPt);
                  }
              }
              
              if(MatchedL1 && MatchedEG) h_gentau_TauEG_Efficiency->Fill(tausVis.vector().Pt());
              if(MatchedEG && MatchedOffline) h_tau_EG_Efficiency->Fill(tauPt);
              if(MatchedL1 && MatchedJet) h_gentau_TauJet_Efficiency->Fill(tausVis.vector().Pt());
              if(MatchedJet && MatchedOffline) h_tau_Jet_Efficiency->Fill(tauPt);
              if(MatchedL1 && MatchedJet && MatchedEG) h_gentau_TauEGJet_Efficiency->Fill(tausVis.vector().Pt());
                  
              if(MatchedIsoL1 && MatchedIsoEG) h_gentau_IsoTauIsoEG_Efficiency->Fill(tausVis.vector().Pt());
              if(MatchedIsoEG && MatchedOffline) h_tau_IsoEG_Efficiency->Fill(tauPt);
              if(MatchedIsoL1 && MatchedJet){
                  h_gentau_IsoTauJet_Efficiency->Fill(tausVis.vector().Pt());
                  
                  if(MatchedIsoL1 && MatchedJet && MatchedIsoEG) h_gentau_IsoTauIsoEGJet_Efficiency->Fill(tausVis.vector().Pt());

              }
          }
          
          //Gen-Electrons
          
          if(genID == 11 && std::fabs(GenParticles[j]->vector().Rapidity()) < 2.4 && GenParticles[j]->statusFlags().at(5)){
              
              //h_gene_Total->Fill(GenParticles[j]->vector().Pt());
              
              bool MatchedL1 = false;
              bool MatchedIsoL1 = false;
              bool MatchedTau = false;
              bool MatchedIsoTau = false;
              bool MatchedJet = false;
              
              for(unsigned k=0; k<n_l1electrons_; k++){
                  double DeltaR = sqrt(pow(l1electrons[k]->vector().Phi()- GenParticles[j]->vector().Phi(),2) + pow(l1electrons[k]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
                  double PtDiff = l1electrons[k]->vector().Pt() - GenParticles[j]->vector().Pt();
                  if(DeltaR < 0.5 && l1electrons[k]->vector().Pt()>= l1eCut){
                      MatchedL1 = true;
                      if(l1taus[k]->isolation != 0) MatchedIsoL1 = true;
                      h_e_EG_DeltaRRes->Fill(DeltaR);
                      h_e_EG_PtDiff->Fill(PtDiff);
                      h_e_EG_PtRes->Fill(PtDiff/GenParticles[j]->vector().Pt());
                  }
              }
              
              for(unsigned k=0; k<n_l1taus_; k++){
                  double DeltaR = sqrt(pow(l1taus[k]->vector().Phi()- GenParticles[j]->vector().Phi(),2) + pow(l1taus[k]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
                  if(DeltaR < 0.5 && l1taus[k]->vector().Pt() >= l1tauCut){
                      MatchedTau = true;
                      if(l1taus[k]->isolation != 0) MatchedIsoTau = true;
                  }
              }
              
              for(unsigned k=0; k<n_l1jets_; k++){
                  double DeltaR = sqrt(pow(l1jets[k]->vector().Phi()- GenParticles[j]->vector().Phi(),2) + pow(l1jets[k]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
                  if(DeltaR < 0.5 && l1jets[k]->vector().Pt() >= l1jetCut) MatchedJet = true;
              }
              
              double DeltaRMax = 100000;
              double ePt = 0;
              bool MatchedOffline = false;
              
              for(unsigned k=0; k< n_electrons_; k++){
                  double DeltaR = sqrt(pow(electrons[k]->vector().Phi()- GenParticles[j]->vector().Phi(),2) + pow(electrons[k]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
                  if(DeltaR < 0.5 && DeltaR < DeltaRMax){
                      DeltaRMax = DeltaR;
                      ePt = electrons[k]->vector().Pt();
                      MatchedOffline = true;
                  }
              }
              
              h_gene_Total->Fill(GenParticles[j]->vector().Pt());
              if(MatchedOffline) h_e_Total->Fill(ePt);
              
              if(MatchedL1){
                  h_gene_EG_Efficiency->Fill(GenParticles[j]->vector().Pt());
                  if(MatchedOffline) h_e_EG_Efficiency->Fill(ePt);
              
                  if(MatchedIsoL1){
                      h_gene_IsoEG_Efficiency->Fill(GenParticles[j]->vector().Pt());
                      if(MatchedOffline) h_e_IsoEG_Efficiency->Fill(ePt);
                  }
              }
              
              if(MatchedL1 && MatchedTau) h_gene_EGTau_Efficiency->Fill(GenParticles[j]->vector().Pt());
              if(MatchedTau && MatchedOffline) h_e_Tau_Efficiency->Fill(ePt);;

              if(MatchedL1 && MatchedJet) h_gene_EGJet_Efficiency->Fill(GenParticles[j]->vector().Pt());
              if(MatchedJet && MatchedOffline) h_e_Jet_Efficiency->Fill(ePt);
              if(MatchedL1 && MatchedJet && MatchedTau) h_gene_EGTauJet_Efficiency->Fill(GenParticles[j]->vector().Pt());
              if(MatchedIsoL1 && MatchedIsoTau) h_gene_IsoEGIsoTau_Efficiency->Fill(GenParticles[j]->vector().Pt());
              if(MatchedIsoTau && MatchedOffline) h_e_IsoTau_Efficiency->Fill(ePt);
              if(MatchedIsoL1 && MatchedJet) h_gene_IsoEGJet_Efficiency->Fill(GenParticles[j]->vector().Pt());
              if(MatchedIsoL1 && MatchedJet && MatchedIsoTau) h_gene_IsoEGIsoTauJet_Efficiency->Fill(GenParticles[j]->vector().Pt());
              
          }
          
          if(genID == 13 && std::fabs(GenParticles[j]->vector().Rapidity()) < 2.4 && GenParticles[j]->statusFlags().at(5)){
              
              double DeltaRMax = 100000;
              double muPt = 0;
              bool MatchedOffline = false;
              
              for(unsigned k=0; k< n_muons_; k++){
                  double DeltaR = sqrt(pow(muons[k]->vector().Phi()- GenParticles[j]->vector().Phi(),2) + pow(muons[k]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
                  if(DeltaR < 0.5 && DeltaR < DeltaRMax){
                      DeltaRMax = DeltaR;
                      muPt = muons[k]->vector().Pt();
                      MatchedOffline = true;
                  }
              }
              
              h_genmu_Total->Fill(GenParticles[j]->vector().Pt());
              if(MatchedOffline) h_mu_Total->Fill(muPt);
              
              bool MatchedL1 = false;
              bool MatchedIsoL1 = false;
              
              for(unsigned k=0; k<n_l1muons_; k++){
                  double DeltaR = sqrt(pow(l1muons[k]->vector().Phi()- GenParticles[j]->vector().Phi(),2) + pow(l1muons[k]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
                  double PtDiff = l1muons[k]->vector().Pt() - GenParticles[j]->vector().Pt();
                  if(DeltaR < 0.5 && l1muons[k]->vector().Pt() >= l1muCut){
                      MatchedL1 = true;
                      if(l1muons[k]->isolation > 0) MatchedIsoL1 = true;
                      h_mu_Mu_DeltaRRes->Fill(DeltaR);
                      h_mu_Mu_PtDiff->Fill(PtDiff);
                      h_mu_Mu_PtRes->Fill(PtDiff/GenParticles[j]->vector().Pt());
                  }

              }
              
              if(MatchedL1){
                  h_genmu_Mu_Efficiency->Fill(GenParticles[j]->vector().Pt());
                  if(MatchedOffline) h_mu_Mu_Efficiency->Fill(muPt);
              }
              if(MatchedIsoL1){
                  h_genmu_IsoMu_Efficiency->Fill(GenParticles[j]->vector().Pt());
                  if(MatchedOffline) h_mu_IsoMu_Efficiency->Fill(muPt);
              }
          }
   }
   
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
   
   for(unsigned j=0; j< n_genjets_; j++){
        
       if(std::fabs(genjets[j]->vector().Rapidity()) < 5){
           
           h_genjet_Total->Fill(genjets[j]->vector().Pt());
           
           bool MatchedL1 = false;
           bool MatchedTau = false;
           bool MatchedEG = false;
           bool MatchedIsoTau = false;
           bool MatchedIsoEG = false;
           
           for(unsigned k=0; k<n_l1jets_; k++){
               double DeltaR = sqrt(pow(l1jets[k]->vector().Phi()- genjets[j]->vector().Phi(),2) + pow(l1jets[k]->vector().Rapidity()-genjets[j]->vector().Rapidity(),2));
               double PtDiff = l1jets[k]->vector().Pt() - genjets[j]->vector().Pt();
               if(DeltaR < 0.5){
                   MatchedL1 = true;
                   h_jet_Jet_DeltaRRes->Fill(DeltaR);
                   h_jet_Jet_PtDiff->Fill(PtDiff);
                   h_jet_Jet_PtRes->Fill(PtDiff/genjets[j]->vector().Pt());
               }
           }
           
           for(unsigned k=0; k<n_l1electrons_; k++){
               double DeltaR = sqrt(pow(l1electrons[k]->vector().Phi()- genjets[j]->vector().Phi(),2) + pow(l1electrons[k]->vector().Rapidity()-genjets[j]->vector().Rapidity(),2));
               if(DeltaR < 0.5){
                   MatchedEG = true;
                   if(l1electrons[k]->isolation >0) MatchedIsoEG = true;
               }
           }
           
           for(unsigned k=0; k<n_l1taus_; k++){
               double DeltaR = sqrt(pow(l1taus[k]->vector().Phi()- genjets[j]->vector().Phi(),2) + pow(l1taus[k]->vector().Rapidity()-genjets[j]->vector().Rapidity(),2));
               if(DeltaR < 0.5){
                   MatchedTau = true;
                   if(l1taus[k]->isolation > 0) MatchedIsoTau = true;
               }
           }
           
           if(MatchedL1) h_genjet_Jet_Efficiency->Fill(genjets[j]->vector().Pt());
           if(MatchedTau && MatchedL1) h_genjet_Tau_Efficiency->Fill(genjets[j]->vector().Pt());
           if(MatchedEG && MatchedL1) h_genjet_EG_Efficiency->Fill(genjets[j]->vector().Pt());
           if(MatchedTau && MatchedEG && MatchedL1) h_genjet_TauEG_Efficiency->Fill(genjets[j]->vector().Pt());
           if(MatchedIsoTau && MatchedL1) h_genjet_IsoTau_Efficiency->Fill(genjets[j]->vector().Pt());
           if(MatchedIsoEG && MatchedL1) h_genjet_IsoEG_Efficiency->Fill(genjets[j]->vector().Pt());
           if(MatchedIsoTau && MatchedIsoEG && MatchedL1) h_genjet_IsoTauIsoEG_Efficiency->Fill(genjets[j]->vector().Pt());
    
       }
       
   }


      return 0;
  }

  int EfficiencyGenMatch::PostAnalysis(){
      h_mu_Mu_Efficiency->Divide(h_mu_Total);  
      h_mu_IsoMu_Efficiency->Divide(h_mu_Total); 
      
      h_e_EG_Efficiency->Divide(h_e_Total);
      h_e_IsoEG_Efficiency->Divide(h_e_Total);
      h_e_Tau_Efficiency->Divide(h_e_Total);
      h_e_IsoTau_Efficiency->Divide(h_e_Total);
      h_e_Jet_Efficiency->Divide(h_e_Total);
      
      h_tau_Tau_Efficiency->Divide(h_tau_Total);
      h_tau_IsoTau_Efficiency->Divide(h_tau_Total);
      h_subtau_IsoTau_Efficiency->Divide(h_subtau_Total);
      h_subtau_Tau_Efficiency->Divide(h_subtau_Total);
      
      h_tau_Total->Add(h_subtau_Total);
      
      h_tau_EG_Efficiency->Divide(h_tau_Total);
      h_tau_IsoEG_Efficiency->Divide(h_tau_Total);
      h_tau_Mu_Efficiency->Divide(h_tau_Total);
      h_tau_Jet_Efficiency->Divide(h_tau_Total);
      
      h_gentau_Tau_Efficiency->Divide(h_gentau_Total);
      h_gentau_IsoTau_Efficiency->Divide(h_gentau_Total);
      
      h_gentau_TauJet_Efficiency->Divide(h_gentau_Total);
      h_gentau_TauEG_Efficiency->Divide(h_gentau_Total);
      h_gentau_TauEGJet_Efficiency->Divide(h_gentau_Total);
      h_gentau_IsoTauIsoEG_Efficiency->Divide(h_gentau_Total);
      h_gentau_IsoTauJet_Efficiency->Divide(h_gentau_Total);
      h_gentau_IsoTauIsoEGJet_Efficiency->Divide(h_gentau_Total);
      
      h_gene_EG_Efficiency->Divide(h_gene_Total);
      h_gene_IsoEG_Efficiency->Divide(h_gene_Total);
      
      h_gene_EGJet_Efficiency->Divide(h_gene_Total);
      h_gene_EGTau_Efficiency->Divide(h_gene_Total);
      h_gene_EGTauJet_Efficiency->Divide(h_gene_Total);
      h_gene_IsoEGJet_Efficiency->Divide(h_gene_Total);
      h_gene_IsoEGIsoTau_Efficiency->Divide(h_gene_Total);
      h_gene_IsoEGIsoTauJet_Efficiency->Divide(h_gene_Total);
      
      h_genmu_Mu_Efficiency->Divide(h_genmu_Total);
      h_genmu_IsoMu_Efficiency->Divide(h_genmu_Total);
      
      h_genjet_Jet_Efficiency->Divide(h_genjet_Total);
      h_genjet_EG_Efficiency->Divide(h_genjet_Total);
      h_genjet_Tau_Efficiency->Divide(h_genjet_Total);
      h_genjet_TauEG_Efficiency->Divide(h_genjet_Total);
      h_genjet_IsoEG_Efficiency->Divide(h_genjet_Total);
      h_genjet_IsoTau_Efficiency->Divide(h_genjet_Total);
      h_genjet_IsoTauIsoEG_Efficiency->Divide(h_genjet_Total);
      
      h_jet_Jet_Efficiency->Divide(h_jet_Total);
      h_subjet_Jet_Efficiency->Divide(h_subjet_Total);

      h_jet_Total->Add(h_subjet_Total);
      
      h_jet_EG_Efficiency->Divide(h_jet_Total);
      h_jet_IsoEG_Efficiency->Divide(h_jet_Total);
      h_jet_Mu_Efficiency->Divide(h_jet_Total);
      h_jet_Tau_Efficiency->Divide(h_jet_Total);
      h_jet_IsoTau_Efficiency->Divide(h_jet_Total);
      h_jet_TauEG_Efficiency->Divide(h_jet_Total);
      h_jet_IsoTauIsoEG_Efficiency->Divide(h_jet_Total);
      
      h_jetjet_Mjj_Efficiency->Divide(h_jetjet_Mjj_Total);
      h_jetjet_DeltaEta_Efficiency->Divide(h_jetjet_DeltaEta_Total);
      h_jetjet_AvePt_Efficiency->Divide(h_jetjet_AvePt_Total);
      h_jetjet_VecPt_Efficiency->Divide(h_jetjet_VecPt_Total);

      
      h_METHF_Efficiency->Divide(h_METHF_Total);
      h_METNoHF_Efficiency->Divide(h_METNoHF_Total);
      
      return 0;
  }

  void EfficiencyGenMatch::PrintInfo(){
    ;
  }
  



}//namespace


