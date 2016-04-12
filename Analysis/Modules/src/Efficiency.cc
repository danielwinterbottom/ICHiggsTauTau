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
                                          fwlite::TFileService *fs, std::string output_name, int effNum, std::string channel, struct L1Cuts l1Cuts) : ModuleBase(name) {
    
    EffNum = effNum;
    if(EffNum == 1){
        l1eCut = 10;
        l1muCut = 10;
        l1tauCut = 10;
        l1jetCut = 20;
        l1MjjCut = 300;
        l1DeltaEtaCut = 2.5;
        l1METCut = 10;
        l1MHTCut = 40;
    }
    else if(EffNum == 2){
        l1eCut = 15;
        l1muCut = 15;
        l1tauCut = 20;
        l1jetCut = 30;
        l1MjjCut = 400;
        l1DeltaEtaCut = 3.0;
        l1METCut = 20;
        l1MHTCut = 50;
    }
    else if(EffNum == 3){
        l1eCut = 20;
        l1muCut = 20;
        l1tauCut = 30;
        l1jetCut = 40;
        l1MjjCut = 500;
        l1DeltaEtaCut = 3.5;
        l1METCut = 30;
        l1MHTCut = 60;
    }
    else if(EffNum == 4){
        l1eCut = 25;
        l1muCut = 25;
        l1tauCut = 40;
        l1jetCut = 70;
        l1MjjCut = 600;
        l1DeltaEtaCut = 4.0;
        l1METCut = 40;
        l1MHTCut = 70;
    }
    
    else if(EffNum == 5){
        l1eCut = l1Cuts.EGPt;
        l1muCut = l1Cuts.MuPt;
        l1tauCut = l1Cuts.Tau1Pt;
        l1jetCut = l1Cuts.Jet1Pt;
        l1MjjCut = l1Cuts.Mjj;
        l1DeltaEtaCut = l1Cuts.DeltaEta;
        l1METCut = 40;
        l1MHTCut = 70;
    }
    
    if(channel == "em"){
        nPromptElectrons = 1;
        nPromptMuons = 1;
        nPromptTaus = 0;
    }
    if(channel == "et"){
        nPromptElectrons = 1;
        nPromptMuons = 0;
        nPromptTaus = 1;
    }
    if(channel == "mt"){
        nPromptElectrons = 0;
        nPromptMuons = 1;
        nPromptTaus = 1;
    }
    if(channel == "tt"){
        nPromptElectrons = 0;
        nPromptMuons = 0;
        nPromptTaus = 2;
    }
                                              
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
    
    h_gene_IsoEG_Efficiency = subDir.make<TH1D>("h_gene_IsoEG_Efficiency","h_gene_IsoEG_Efficiency",100, 0,100); 
    h_gene_IsoEG_Efficiency->GetXaxis()->SetTitle("Gen-Electron p_{T} [GeV] ");
    h_gene_IsoEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_gene_Total = subDir.make<TH1D>("h_gene_Total","h_gene_Total",100, 0,100); 
    h_gene_Total->GetXaxis()->SetTitle("Gen-Electron p_{T} [GeV] ");
    h_gene_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_EG_Efficiency = subDir.make<TH1D>("h_tau_EG_Efficiency","h_tau_EG_Efficiency",100, 0,100); 
    h_tau_EG_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_EG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_IsoEG_Efficiency = subDir.make<TH1D>("h_tau_IsoEG_Efficiency","h_tau_IsoEG_Efficiency",100, 0,100); 
    h_tau_IsoEG_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_IsoEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_Mu_Efficiency = subDir.make<TH1D>("h_tau_Mu_Efficiency","h_tau_Mu_Efficiency",100, 0,100);
    h_tau_Mu_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_Mu_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_Total = subDir.make<TH1D>("h_tau_Total","h_tau_Total",100, 0,100); 
    h_tau_Total->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_subtau_Total = subDir.make<TH1D>("h_subtau_Total","h_subtau_Total",100, 0,100); 
    h_subtau_Total->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_subtau_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_Tau_Efficiency = subDir.make<TH1D>("h_tau_Tau_Efficiency","h_tau_Tau_Efficiency",100, 0,100);
    h_tau_Tau_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_Tau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_subtau_Tau_Efficiency = subDir.make<TH1D>("h_subtau_Tau_Efficiency","h_subtau_Tau_Efficiency",100, 0,100);
    h_subtau_Tau_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_subtau_Tau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_IsoTau_Efficiency = subDir.make<TH1D>("h_tau_IsoTau_Efficiency","h_tau_IsoTau_Efficiency",100, 0,100);
    h_tau_IsoTau_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_IsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_subtau_IsoTau_Efficiency = subDir.make<TH1D>("h_subtau_IsoTau_Efficiency","h_subtau_IsoTau_Efficiency",100, 0,100);
    h_subtau_IsoTau_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_subtau_IsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_Jet_Efficiency = subDir.make<TH1D>("h_tau_Jet_Efficiency","h_tau_Jet_Efficiency",100, 0,100);
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
    
    h_jet_EG_Efficiency = subDir.make<TH1D>("h_jet_EG_Efficiency","h_jet_EG_Efficiency",100, 0,100); 
    h_jet_EG_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_EG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_IsoEG_Efficiency = subDir.make<TH1D>("h_jet_IsoEG_Efficiency","h_jet_IsoEG_Efficiency",100, 0,100);
    h_jet_IsoEG_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_IsoEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_Mu_Efficiency = subDir.make<TH1D>("h_jet_Mu_Efficiency","h_jet_Mu_Efficiency",100, 0,100); 
    h_jet_Mu_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_Mu_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_Total = subDir.make<TH1D>("h_jet_Total","h_jet_Total",100, 0,100);
    h_jet_Total->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_subjet_Total = subDir.make<TH1D>("h_subjet_Total","h_subjet_Total",100, 0,100);
    h_subjet_Total->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_subjet_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_Tau_Efficiency = subDir.make<TH1D>("h_jet_Tau_Efficiency","h_jet_Tau_Efficiency",100, 0,100);
    h_jet_Tau_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_Tau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_IsoTau_Efficiency = subDir.make<TH1D>("h_jet_IsoTau_Efficiency","h_jet_IsoTau_Efficiency",100, 0,100); 
    h_jet_IsoTau_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_IsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_Jet_Efficiency = subDir.make<TH1D>("h_jet_Jet_Efficiency","h_jet_Jet_Efficiency",100, 0,100);
    h_jet_Jet_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_jet_Jet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_genjet_Jet_Efficiency = subDir.make<TH1D>("h_genjet_Jet_Efficiency","h_genjet_Jet_Efficiency",100, 0,100);
    h_genjet_Jet_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_genjet_Jet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_genjet_Total = subDir.make<TH1D>("h_genjet_Total","h_genjet_Total",100, 0,100);
    h_genjet_Total->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_genjet_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_subjet_Jet_Efficiency = subDir.make<TH1D>("h_subjet_Jet_Efficiency","h_subjet_Jet_Efficiency",100, 0,100);
    h_subjet_Jet_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_subjet_Jet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_TauEG_Efficiency = subDir.make<TH1D>("h_jet_TauEG_Efficiency","h_jet_TauEG_Efficiency",100, 0,100); 
    h_jet_TauEG_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_jet_TauEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_IsoTauIsoEG_Efficiency = subDir.make<TH1D>("h_jet_IsoTauIsoEG_Efficiency","h_jet_IsoTauIsoEG_Efficiency",100, 0,100); 
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
    
    h_jettau_DeltaEta_Efficiency = subDir.make<TH1D>("h_jettau_DeltaEta_Efficiency","h_jettau_DeltaEta_Efficiency",100, 0,5); 
    h_jettau_DeltaEta_Efficiency->GetXaxis()->SetTitle("#Delta#eta");
    h_jettau_DeltaEta_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jettau_DeltaEta_Total = subDir.make<TH1D>("h_jettau_DeltaEta_Total","h_jettau_DeltaEta_Total",100, 0,5); 
    h_jettau_DeltaEta_Total->GetXaxis()->SetTitle("#Delta#eta");
    h_jettau_DeltaEta_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_jettau_Mjj_Efficiency = subDir.make<TH1D>("h_jettau_Mjj_Efficiency","h_jettau_Mjj_Efficiency",100, 0,1000); 
    h_jettau_Mjj_Efficiency->GetXaxis()->SetTitle("m_{j#tau} [GeV]");
    h_jettau_Mjj_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jettau_Mjj_Total = subDir.make<TH1D>("h_jettau_Mjj_Total","h_jettau_Mjj_Total",100, 0,1000); 
    h_jettau_Mjj_Total->GetXaxis()->SetTitle("m_{j#tau} [GeV]");
    h_jettau_Mjj_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_jetelectron_DeltaEta_Efficiency = subDir.make<TH1D>("h_jetelectron_DeltaEta_Efficiency","h_jetelectron_DeltaEta_Efficiency",100, 0,5); 
    h_jetelectron_DeltaEta_Efficiency->GetXaxis()->SetTitle("#Delta#eta");
    h_jetelectron_DeltaEta_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jetelectron_DeltaEta_Total = subDir.make<TH1D>("h_jetelectron_DeltaEta_Total","h_jetelectron_DeltaEta_Total",100, 0,5); 
    h_jetelectron_DeltaEta_Total->GetXaxis()->SetTitle("#Delta#eta");
    h_jetelectron_DeltaEta_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_jetelectron_Mjj_Efficiency = subDir.make<TH1D>("h_jetelectron_Mjj_Efficiency","h_jetelectron_Mjj_Efficiency",100, 0,1000); 
    h_jetelectron_Mjj_Efficiency->GetXaxis()->SetTitle("m_{je} [GeV]");
    h_jetelectron_Mjj_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jetelectron_Mjj_Total = subDir.make<TH1D>("h_jetelectron_Mjj_Total","h_jetelectron_Mjj_Total",100, 0,1000); 
    h_jetelectron_Mjj_Total->GetXaxis()->SetTitle("m_{je} [GeV]");
    h_jetelectron_Mjj_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_METEfficiency = subDir.make<TH1D>("h_METEfficiency","h_METEfficiency",100, 0,100); 
    h_METEfficiency->GetXaxis()->SetTitle("MET [GeV]");
    h_METEfficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_MHTEfficiency = subDir.make<TH1D>("h_MHTEfficiency","h_GenMHTEfficiency",100, 0,200);
    h_MHTEfficiency->GetXaxis()->SetTitle("MHT [GeV]");
    h_MHTEfficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_GenMETEfficiency = subDir.make<TH1D>("h_GenMETEfficiency","h_GenMETEfficiency",100, 0,100); 
    h_GenMETEfficiency->GetXaxis()->SetTitle("MET [GeV]");
    h_GenMETEfficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_GenMHTEfficiency = subDir.make<TH1D>("h_GenMHTEfficiency","h_GenMHTEfficiency",100, 0,200); 
    h_GenMHTEfficiency->GetXaxis()->SetTitle("MHT [GeV]");
    h_GenMHTEfficiency->GetYaxis()->SetTitle("Efficiency");
    
  }

    Efficiency::~Efficiency(){
    ;
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
      std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
      std::vector<Electron*> electrons = event->GetPtrVec<Electron>(electrons_label_);
      std::vector<Muon*> muons = event->GetPtrVec<Muon>(muons_label_);
      std::vector<Tau*> taus = event->GetPtrVec<Tau>(taus_label_);
      std::vector<Met*> met_vec = event->GetPtrVec<Met>(met_label_);
      
      n_jets_ = jets.size();
      n_electrons_ = electrons.size();
      n_muons_ = muons.size();
      n_taus_ = taus.size();
      n_genParticles_ = GenParticles.size();
      n_genjets_ = genjets.size();      
      
      //Electrons
      
      for(unsigned i=0; i < n_electrons_ && i < nPromptElectrons; i++){
                  
          bool eProceed = false;
                
          if(std::fabs(electrons[i]->vector().Rapidity()) < 2.4) eProceed = true;
          
          for(unsigned j=0; j< n_muons_; j++){
              double DeltaR = sqrt(pow(electrons[i]->vector().Phi()-muons[j]->vector().Phi(),2) + pow(electrons[i]->vector().Rapidity()-muons[j]->vector().Rapidity(),2));
              if(DeltaR < 0.1) eProceed = false;
          }

          if(eProceed){
              
              bool MatchedL1 = false;
              bool MatchedIsoL1 = false;
              bool L1EGPtFired = false;
              
              for(unsigned j=0; j < n_l1electrons_; j++){
              
                  double DeltaR = sqrt(pow(l1electrons[j]->vector().Phi()-electrons[i]->vector().Phi(),2) + pow(l1electrons[j]->vector().Rapidity()-electrons[i]->vector().Rapidity(),2));
                  
                  int eIso = l1electrons[j]->isolation;
                  bool IsoEG = false;
                  
                  double L1EGPt = l1electrons[j]->vector().Pt();
                  if(L1EGPt >= l1eCut) L1EGPtFired = true;
                  
                  if(eIso == 1) IsoEG = true;
                  if(DeltaR < 0.5 && L1EGPtFired) MatchedL1 = true;
                  if(DeltaR < 0.5 && IsoEG && L1EGPtFired) MatchedIsoL1 = true;
              }

              h_e_Total->Fill(electrons[i]->vector().Pt());
              if(MatchedIsoL1){ 
                  h_e_EG_Efficiency->Fill(electrons[i]->vector().Pt());
                  h_e_IsoEG_Efficiency->Fill(electrons[i]->vector().Pt());
              }
              else if(!MatchedIsoL1 && MatchedL1) h_e_EG_Efficiency->Fill(electrons[i]->vector().Pt());
             
              // Check if electron fired tau and iso-tau triggers.
              
              bool MatchedTauL1 = false;
              bool MatchedIsoTauL1 = false;
              
              for(unsigned j=0; j < n_l1taus_; j++){
              
                  double DeltaR = sqrt(pow(l1taus[j]->vector().Phi() - electrons[i]->vector().Phi(),2) + pow(l1taus[j]->vector().Rapidity()-electrons[i]->vector().Rapidity(),2));

                  int tauIso = l1taus[j]->isolation;
                  bool IsoTau = false;
                  if(tauIso == 1) IsoTau = true;
                  bool L1TauPtFired = false;
                  double L1TauPt = l1taus[j]->vector().Pt();
                  if(L1TauPt >= l1tauCut) L1TauPtFired = true;
                  
                  if(DeltaR < 0.5 && L1TauPtFired) MatchedTauL1 = true;
                  if(DeltaR < 0.5 && L1TauPtFired && IsoTau) MatchedIsoTauL1 = true;
              }
              
              if(MatchedTauL1) h_e_Tau_Efficiency->Fill(electrons[i]->vector().Pt());
              if(MatchedIsoTauL1) h_e_IsoTau_Efficiency->Fill(electrons[i]->vector().Pt());

              // Check if electron fired Jet trigger.

              bool MatchedJetL1 = false;
          
              for(unsigned j=0; j < n_l1jets_; j++){
              
                  double DeltaR = sqrt(pow(l1jets[j]->vector().Phi()-electrons[i]->vector().Phi(),2) + pow(l1jets[j]->vector().Rapidity()-electrons[i]->vector().Rapidity(),2));

                  bool L1JetPtFired = false;
                  double L1JetPt = l1jets[j]->vector().Pt();
                  if(L1JetPt >= l1jetCut) L1JetPtFired = true;
              
                  if(DeltaR < 0.5 && L1JetPtFired) MatchedJetL1 = true;
                  
              }
              
              if(MatchedJetL1) h_e_Jet_Efficiency->Fill(electrons[i]->vector().Pt());
          }
      }
      
      // Taus
      
      for(unsigned i=0; i < n_taus_ && i < nPromptTaus; i++){
                  
          bool tauProceed = false;

          if(std::fabs(taus[i]->vector().Rapidity()) < 2.3) tauProceed = true;
          
          for(unsigned j=0; j< n_electrons_; j++){
              double DeltaR = sqrt(pow(taus[i]->vector().Phi()-electrons[j]->vector().Phi(),2) + pow(taus[i]->vector().Rapidity()-electrons[j]->vector().Rapidity(),2));
              if(DeltaR < 0.1) tauProceed = false;
          }
          
          for(unsigned j=0; j< n_muons_; j++){
              double DeltaR = sqrt(pow(taus[i]->vector().Phi()-muons[j]->vector().Phi(),2) + pow(taus[i]->vector().Rapidity()-muons[j]->vector().Rapidity(),2));
              if(DeltaR < 0.1) tauProceed = false;
          }

          if(tauProceed){
                  
              if(i == 0) h_tau_Total->Fill(taus[i]->vector().Pt());
              else if(i == 1) h_subtau_Total->Fill(taus[i]->vector().Pt());
              
              // Check if tau fired EG trigger.
              
              bool MatchedL1 = false;
              bool MatchedL1Iso = false;
              
              for(unsigned j=0; j < n_l1electrons_; j++){
              
                  double DeltaR = sqrt(pow(l1electrons[j]->vector().Phi()-taus[i]->vector().Phi(),2) + pow(l1electrons[j]->vector().Rapidity()-taus[i]->vector().Rapidity(),2));

                  bool L1EPtFired = false;
                  double L1EPt = l1electrons[j]->vector().Pt();
                  if(L1EPt >= l1eCut) L1EPtFired = true;
                  bool isIso = false;
                  if(l1electrons[j]->isolation == 1) isIso = true; 
              
                  if(DeltaR < 0.5 && L1EPtFired) MatchedL1 = true;
                  if(DeltaR < 0.5 && L1EPtFired && isIso) MatchedL1Iso = true;
                  
              }
              
              if(MatchedL1) h_tau_EG_Efficiency->Fill(taus[i]->vector().Pt());
              if(MatchedL1Iso) h_tau_IsoEG_Efficiency->Fill(taus[i]->vector().Pt());
    
             
              // Check if tau fired tau and iso-tau triggers.
              
              bool MatchedTauL1 = false;
              bool MatchedIsoTauL1 = false;

              bool L1TauPtFired = false;
              for(unsigned j=0; j < n_l1taus_; j++){
              
                  double DeltaR = sqrt(pow(l1taus[j]->vector().Phi()-taus[i]->vector().Phi(),2) + pow(l1taus[j]->vector().Rapidity()-taus[i]->vector().Rapidity(),2));
                  
                  int tauIso = l1taus[j]->isolation;
                  bool IsoTau = false;
                  
                  double L1TauPt = l1taus[j]->vector().Pt();
                  if(L1TauPt >= l1tauCut) L1TauPtFired = true;
                  
                  if(tauIso == 1) IsoTau = true;
                  if(DeltaR < 0.5 && L1TauPtFired) MatchedTauL1 = true;
                  
                  if(DeltaR < 0.5 && IsoTau && L1TauPtFired) MatchedIsoTauL1 = true;

              }
              
              if(MatchedIsoTauL1){ 
                  if(i==0){
                      h_tau_Tau_Efficiency->Fill(taus[i]->vector().Pt());
                      h_tau_IsoTau_Efficiency->Fill(taus[i]->vector().Pt());
                  }
                  else if(i==1){
                      h_subtau_Tau_Efficiency->Fill(taus[i]->vector().Pt());
                      h_subtau_IsoTau_Efficiency->Fill(taus[i]->vector().Pt());
                  }
              }
              else if(!MatchedIsoTauL1 && MatchedTauL1){
                  if(i==0){
                      h_tau_Tau_Efficiency->Fill(taus[i]->vector().Pt());
                  }
                  else if(i==1){
                      h_subtau_Tau_Efficiency->Fill(taus[i]->vector().Pt());
                  }
              }
              
              // Check if tau fired Jet trigger.

              bool MatchedJetL1 = false;
          
              for(unsigned j=0; j < n_l1jets_; j++){
              
                  double DeltaR = sqrt(pow(l1jets[j]->vector().Phi()-taus[i]->vector().Phi(),2) + pow(l1jets[j]->vector().Rapidity()-taus[i]->vector().Rapidity(),2));
              
                  bool L1JetPtFired = false;
                  double L1JetPt = l1jets[j]->vector().Pt();
                  if(L1JetPt >= l1jetCut) L1JetPtFired = true;
                  
                  if(DeltaR < 0.5 && L1JetPtFired) MatchedJetL1 = true;
                  
              }
              
              if(MatchedJetL1) h_tau_Jet_Efficiency->Fill(taus[i]->vector().Pt());
              
              // Check if tau fired Muon trigger.
              
              bool MatchedL1Mu = false;
          
              for(unsigned j=0; j < n_l1muons_; j++){
              
                  double DeltaR = sqrt(pow(l1muons[j]->vector().Phi()-taus[i]->vector().Phi(),2) + pow(l1muons[j]->vector().Rapidity()-taus[i]->vector().Rapidity(),2));
                  bool L1MuPtFired = false;
                  double L1MuPt = l1muons[j]->vector().Pt();
                  if(L1MuPt >= l1muCut) L1MuPtFired = true;
                  
                  if(DeltaR < 0.5 && L1MuPtFired) MatchedL1Mu = true;
              }
              
              if(MatchedL1Mu) h_tau_Mu_Efficiency->Fill(taus[i]->vector().Pt());
          
          }
          
      }
      
      //Muons
      
      for(unsigned i=0; i < n_muons_ && i< nPromptMuons; i++){
                  
          bool muProceed = false;

          if(std::fabs(muons[i]->vector().Rapidity()) < 2.4) muProceed = true;

          if(muProceed){
                  
              h_mu_Total->Fill(muons[i]->vector().Pt());
              
              // Check if muon fired Muon trigger.
              
              bool MatchedL1 = false;
              bool MatchedIsoL1 = false;
          
              for(unsigned j=0; j < n_l1muons_; j++){
              
                  double DeltaR = sqrt(pow(l1muons[j]->vector().Phi()-muons[i]->vector().Phi(),2) + pow(l1muons[j]->vector().Rapidity()-muons[i]->vector().Rapidity(),2));
              
                  bool L1MuPtFired = false;
                  double L1MuPt = l1muons[j]->vector().Pt();
                  if(L1MuPt >= l1muCut) L1MuPtFired = true;
                  
                  if(DeltaR < 0.5 && L1MuPtFired) MatchedL1 = true;
                  if(DeltaR < 0.5 && L1MuPtFired && l1muons[j]->isolation >0) MatchedIsoL1 = true;
              }
              
              if(MatchedL1) h_mu_Mu_Efficiency->Fill(muons[i]->vector().Pt());
              if(MatchedIsoL1) h_mu_IsoMu_Efficiency->Fill(muons[i]->vector().Pt());
          }
      }
      
      // Jets
      
      for(unsigned i=0; i < n_jets_ && i < 2; i++){
                  
          bool jetProceed = false;
          if(std::fabs(jets[i]->vector().Rapidity()) < 5) jetProceed = true;
          
          for(unsigned j=0; j< n_taus_; j++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-taus[j]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-taus[j]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jetProceed = false;
          }
          for(unsigned j=0; j< n_electrons_; j++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-electrons[j]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-electrons[j]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jetProceed = false;
          }
          
          for(unsigned j=0; j< n_muons_; j++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-muons[j]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-muons[j]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jetProceed = false;
          }

          if(jetProceed){

              if(i == 0) h_jet_Total->Fill(jets[i]->vector().Pt());
              if(i == 1) h_subjet_Total->Fill(jets[i]->vector().Pt());
              
              // Check if jet fired EG trigger.
              
              bool MatchedL1 = false;
              bool MatchedL1Iso = false;
          
              for(unsigned j=0; j < n_l1electrons_; j++){
              
                  double DeltaR = sqrt(pow(l1electrons[j]->vector().Phi()-jets[i]->vector().Phi(),2) + pow(l1electrons[j]->vector().Rapidity()-jets[i]->vector().Rapidity(),2));
                  
                  bool L1EPtFired = false;
                  double L1EPt = l1electrons[j]->vector().Pt();
                  if(L1EPt >= l1eCut) L1EPtFired = true;
                  
                  bool isIso = false;
                  if(l1electrons[j]->isolation == 1) isIso = true;
              
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
                  if(tauIso == 1) IsoTau = true;
                  
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
              
              if(MatchedJetL1){
                  if(i==0) h_jet_Jet_Efficiency->Fill(jets[i]->vector().Pt());
                  if(i==1) h_subjet_Jet_Efficiency->Fill(jets[i]->vector().Pt());
              }  
              
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

      
      for(unsigned i=0; i < n_jets_ && i < 2; i++){
          
          bool jetProceed = true;
          
          for(unsigned j=0; j< n_taus_; j++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-taus[j]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-taus[j]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jetProceed = false;
          }
          for(unsigned j=0; j< n_electrons_; j++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-electrons[j]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-electrons[j]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jetProceed = false;
          }
    
          for(unsigned j=0; j<n_jets_ && j < 2; j++){
              
              for(unsigned k=0; k< n_taus_; k++){
                  double DeltaR = sqrt(pow(jets[j]->vector().Phi()-taus[k]->vector().Phi(),2) + pow(jets[j]->vector().Rapidity()-taus[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.1) jetProceed = false;
              }
              for(unsigned k=0; k< n_electrons_; k++){
                  double DeltaR = sqrt(pow(jets[j]->vector().Phi()-electrons[k]->vector().Phi(),2) + pow(jets[j]->vector().Rapidity()-electrons[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.1) jetProceed = false;
              }
           
              if(i!=j && jetProceed && std::fabs(jets[i]->vector().Rapidity()) < 5 && std::fabs(jets[j]->vector().Rapidity()) < 5){
                  
                  if(jets[i]->vector().Pt() > 20 && jets[j]->vector().Pt() > 20){

                      h_jetjet_Mjj_Total->Fill((jets[i]->vector() + jets[j]->vector()).M());
                      h_jetjet_DeltaEta_Total->Fill(std::fabs(jets[i]->vector().Rapidity() - jets[j]->vector().Rapidity()));
                      
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
                      
                      
                      if(Matched1JetL1 && Matched2JetL1 && jet1JetL1Index != jet2JetL1Index){
                          double Mjj = (l1jets[jet1JetL1Index]->vector() + l1jets[jet2JetL1Index]->vector()).M(); 
                          if(Mjj >= l1MjjCut) h_jetjet_Mjj_Efficiency->Fill((jets[i]->vector() + jets[j]->vector()).M());
                          double DeltaEta = std::fabs(l1jets[jet1JetL1Index]->vector().Rapidity() - l1jets[jet2JetL1Index]->vector().Rapidity()); 
                          if(DeltaEta >= l1DeltaEtaCut) h_jetjet_DeltaEta_Efficiency->Fill(std::fabs(jets[i]->vector().Rapidity() - jets[j]->vector().Rapidity()));
                      }
                  }
              }
          }
      }
      
      //Jet-Tau
      
      for(unsigned i=0; i < n_jets_ && i < 2; i++){

          bool jetProceed = true;
          
          for(unsigned j=0; j< n_taus_; j++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-taus[j]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-taus[j]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jetProceed = false;
          }
          for(unsigned j=0; j< n_electrons_; j++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-electrons[j]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-electrons[j]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jetProceed = false;
          }

          for(unsigned j=0; j<n_taus_ && j <nPromptTaus; j++){
              
              for(unsigned k=0; k< n_electrons_; k++){
                  double DeltaR = sqrt(pow(taus[j]->vector().Phi()-electrons[k]->vector().Phi(),2) + pow(taus[j]->vector().Rapidity()-electrons[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.1) jetProceed = false;
              }

              if(jetProceed && std::fabs(jets[i]->vector().Rapidity()) < 5 && std::fabs(taus[j]->vector().Rapidity()) < 5){
                  
                  if(jets[i]->vector().Pt() > 20){

                      h_jettau_Mjj_Total->Fill((jets[i]->vector() + taus[j]->vector()).M());
                      h_jettau_DeltaEta_Total->Fill(std::fabs(jets[i]->vector().Rapidity() - taus[j]->vector().Rapidity()));
                      
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
                      double PtDifftauJet = 10000;
                      int tauJetL1Index = -1;
              
                      for(unsigned k=0; k < n_l1jets_; k++){
                          
                          double DeltaR = sqrt(pow(l1jets[k]->vector().Phi()- taus[j]->vector().Phi(),2) + pow(l1jets[k]->vector().Rapidity()-taus[j]->vector().Rapidity(),2));
                          double PtDiff = std::fabs(l1jets[k]->vector().Pt() - taus[j]->vector().Pt());
                  
                          if(DeltaR < 0.5 && PtDiff < PtDifftauJet){
                               PtDifftauJet = PtDiff;
                               Matched2JetL1 = true;
                               tauJetL1Index = k;
                          }
                      
                      }
                      
                      
                      if(Matched1JetL1 && Matched2JetL1 && jet1JetL1Index != tauJetL1Index){
                          double Mjj = (l1jets[jet1JetL1Index]->vector() + l1jets[tauJetL1Index]->vector()).M(); 
                          if(Mjj >= l1MjjCut) h_jettau_Mjj_Efficiency->Fill((jets[i]->vector() + taus[j]->vector()).M());
                          double DeltaEta = std::fabs(l1jets[jet1JetL1Index]->vector().Rapidity() - l1jets[tauJetL1Index]->vector().Rapidity()); 
                          if(DeltaEta >= l1DeltaEtaCut) h_jettau_DeltaEta_Efficiency->Fill(std::fabs(jets[i]->vector().Rapidity() - taus[j]->vector().Rapidity()));
                      }
                  }
              }
          }
      }
      
      // Jet-Electron
      
      for(unsigned i=0; i < n_jets_ && i < 2; i++){

          bool jetProceed = true;
          
          for(unsigned j=0; j< n_taus_; j++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-taus[j]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-taus[j]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jetProceed = false;
          }
          for(unsigned j=0; j< n_electrons_; j++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-electrons[j]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-electrons[j]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jetProceed = false;
          }
    
          for(unsigned j=0; j<n_electrons_ && j <nPromptElectrons; j++){
           
              if(jetProceed && std::fabs(jets[i]->vector().Rapidity()) < 5 && std::fabs(electrons[j]->vector().Rapidity()) < 5){
                          
                  if(jets[i]->vector().Pt() > 20){

                      h_jetelectron_Mjj_Total->Fill((jets[i]->vector() + electrons[j]->vector()).M());
                      h_jetelectron_DeltaEta_Total->Fill(std::fabs(jets[i]->vector().Rapidity() - electrons[j]->vector().Rapidity()));
                      
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
                      double PtDiffelectronJet = 10000;
                      int electronJetL1Index = -1;
              
                      for(unsigned k=0; k < n_l1jets_; k++){
                          
                          double DeltaR = sqrt(pow(l1jets[k]->vector().Phi()- electrons[j]->vector().Phi(),2) + pow(l1jets[k]->vector().Rapidity()-electrons[j]->vector().Rapidity(),2));
                          double PtDiff = std::fabs(l1jets[k]->vector().Pt() - electrons[j]->vector().Pt());
                  
                          if(DeltaR < 0.5 && PtDiff < PtDiffelectronJet){
                               PtDiffelectronJet = PtDiff;
                               Matched2JetL1 = true;
                               electronJetL1Index = k;
                          }
                      
                      }
                      
                      
                      if(Matched1JetL1 && Matched2JetL1 && jet1JetL1Index != electronJetL1Index){
                          double Mjj = (l1jets[jet1JetL1Index]->vector() + l1jets[electronJetL1Index]->vector()).M(); 
                          if(Mjj >= l1MjjCut) h_jetelectron_Mjj_Efficiency->Fill((jets[i]->vector() + electrons[j]->vector()).M());
                          double DeltaEta = std::fabs(l1jets[jet1JetL1Index]->vector().Rapidity() - l1jets[electronJetL1Index]->vector().Rapidity()); 
                          if(DeltaEta >= l1DeltaEtaCut) h_jetelectron_DeltaEta_Efficiency->Fill(std::fabs(jets[i]->vector().Rapidity() - electrons[j]->vector().Rapidity()));
                      }
                  }
              }
          }
      }
      
      //MET/MHT
      if(met_vec.size() > 0){
          double MET = met_vec[0]->vector().pt();
          h_GenMETEfficiency->Fill(MET);
          if(l1met_vec.size() > 1) if(l1met_vec[1]->et >= l1METCut) h_METEfficiency->Fill(MET);
      
          ic::Candidate *MHT = new ic::Candidate();
          for(unsigned int i=0; i <n_jets_; i++){
              if(jets[i]->vector().pt() > 35 && std::fabs(jets[i]->vector().Rapidity()) < 3) MHT->set_vector(MHT->vector() - jets[i]->vector());
          }
      
          h_GenMHTEfficiency->Fill(MHT->vector().pt());
          if(l1met_vec.size() > 2) if(l1met_vec[3]->et >= l1MHTCut) h_MHTEfficiency->Fill(MHT->vector().pt());
          
      }
      
      //Gen Taus
      
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
              
              h_gentau_Total->Fill(tausVis.vector().Pt());
              
              bool MatchedL1 = false;
              bool MatchedIsoL1 = false;
              
              for(unsigned k=0; k<n_l1taus_; k++){
                  double DeltaR = sqrt(pow(l1taus[k]->vector().Phi()- tausVis.vector().Phi(),2) + pow(l1taus[k]->vector().Rapidity()-tausVis.vector().Rapidity(),2));
                  double PtDiff = l1taus[k]->vector().Pt() - tausVis.vector().Pt();
                  if(DeltaR < 0.5){
                      MatchedL1 = true;
                      if(l1taus[k]->isolation == 1) MatchedIsoL1 = true;
                      h_tau_Tau_DeltaRRes->Fill(DeltaR);
                      h_tau_Tau_PtDiff->Fill(PtDiff);
                      h_tau_Tau_PtRes->Fill(PtDiff/tausVis.vector().Pt());
                  }
              }
              
              if(MatchedL1){
                  h_gentau_Tau_Efficiency->Fill(tausVis.vector().Pt());
                  if(MatchedIsoL1) h_gentau_IsoTau_Efficiency->Fill(tausVis.vector().Pt());
              }
          }
          
          if(genID == 11 && std::fabs(GenParticles[j]->vector().Rapidity()) < 2.4){
              
              h_gene_Total->Fill(GenParticles[j]->vector().Pt());
              
              bool MatchedL1 = false;
              bool MatchedIsoL1 = false;
              
              for(unsigned k=0; k<n_l1electrons_; k++){
                  double DeltaR = sqrt(pow(l1electrons[k]->vector().Phi()- GenParticles[j]->vector().Phi(),2) + pow(l1electrons[k]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
                  double PtDiff = l1electrons[k]->vector().Pt() - GenParticles[j]->vector().Pt();
                  if(DeltaR < 0.5){
                      MatchedL1 = true;
                      if(l1taus[k]->isolation == 1) MatchedIsoL1 = true;
                      h_e_EG_DeltaRRes->Fill(DeltaR);
                      h_e_EG_PtDiff->Fill(PtDiff);
                      h_e_EG_PtRes->Fill(PtDiff/GenParticles[j]->vector().Pt());
                  }
              }
              
              if(MatchedL1){
                  h_gene_EG_Efficiency->Fill(GenParticles[j]->vector().Pt());
              
                  if(MatchedIsoL1){
                      h_gene_IsoEG_Efficiency->Fill(GenParticles[j]->vector().Pt());
                  }
              }
          }
          
          if(genID == 13 && std::fabs(GenParticles[j]->vector().Rapidity()) < 2.4){
              
              h_genmu_Total->Fill(GenParticles[j]->vector().Pt());
              
              bool MatchedL1 = false;
              bool MatchedIsoL1 = false;
              
              for(unsigned k=0; k<n_l1muons_; k++){
                  double DeltaR = sqrt(pow(l1muons[k]->vector().Phi()- GenParticles[j]->vector().Phi(),2) + pow(l1muons[k]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
                  double PtDiff = l1muons[k]->vector().Pt() - GenParticles[j]->vector().Pt();
                  if(DeltaR < 0.5){
                      MatchedL1 = true;
                      if(l1muons[k]->isolation > 0) MatchedIsoL1 = true;
                      h_mu_Mu_DeltaRRes->Fill(DeltaR);
                      h_mu_Mu_PtDiff->Fill(PtDiff);
                      h_mu_Mu_PtRes->Fill(PtDiff/GenParticles[j]->vector().Pt());
                  }

              }
              
              if(MatchedL1) h_genmu_Mu_Efficiency->Fill(GenParticles[j]->vector().Pt());
              if(MatchedIsoL1) h_genmu_IsoMu_Efficiency->Fill(GenParticles[j]->vector().Pt());
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
           
           if(MatchedL1) h_genjet_Jet_Efficiency->Fill(genjets[j]->vector().Pt());
       }
       
   }


      return 0;
  }

  int Efficiency::PostAnalysis(){
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
      
      h_gene_EG_Efficiency->Divide(h_gene_Total);
      h_gene_IsoEG_Efficiency->Divide(h_gene_Total);
      
      h_genmu_Mu_Efficiency->Divide(h_genmu_Total);
      h_genmu_IsoMu_Efficiency->Divide(h_genmu_Total);
      
      h_genjet_Jet_Efficiency->Divide(h_genjet_Total);
      
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
      
      h_jettau_Mjj_Efficiency->Divide(h_jettau_Mjj_Total);
      h_jettau_DeltaEta_Efficiency->Divide(h_jettau_DeltaEta_Total);
      
      h_jetelectron_Mjj_Efficiency->Divide(h_jetelectron_Mjj_Total);
      h_jetelectron_DeltaEta_Efficiency->Divide(h_jetelectron_DeltaEta_Total);
      
      h_METEfficiency->Divide(h_GenMETEfficiency);
      h_MHTEfficiency->Divide(h_GenMHTEfficiency);
    return 0;
  }

  void Efficiency::PrintInfo(){
    ;
  }
  



}//namespace


