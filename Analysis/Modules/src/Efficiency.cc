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
                                          fwlite::TFileService *fs, std::string output_name, int effNum, std::string channel) : ModuleBase(name) {
    
    if(effNum == 1){
        l1eCut = 10;
        l1muCut = 10;
        l1tauCut = 10;
        l1jetCut = 20;
        l1MjjCut = 300;
        l1DeltaEtaCut = 2.5;
        l1METCut = 10;
        l1MHTCut = 40;
    }
    else if(effNum == 2){
        l1eCut = 15;
        l1muCut = 15;
        l1tauCut = 20;
        l1jetCut = 30;
        l1MjjCut = 400;
        l1DeltaEtaCut = 3.0;
        l1METCut = 20;
        l1MHTCut = 50;
    }
    else if(effNum == 3){
        l1eCut = 20;
        l1muCut = 20;
        l1tauCut = 30;
        l1jetCut = 40;
        l1MjjCut = 500;
        l1DeltaEtaCut = 3.5;
        l1METCut = 30;
        l1MHTCut = 60;
    }
    else if(effNum == 4){
        l1eCut = 25;
        l1muCut = 25;
        l1tauCut = 40;
        l1jetCut = 70;
        l1MjjCut = 600;
        l1DeltaEtaCut = 4.0;
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
    
    TFileDirectory subDir = fs->mkdir(output_name.c_str());
    
    h_mu_Mu_Efficiency = subDir.make<TH1D>("h_mu_Mu_Efficiency","h_mu_Mu_Efficiency",100, 0,100); 
    h_mu_Mu_Efficiency->GetXaxis()->SetTitle("Muon p_{T} [GeV] ");
    h_mu_Mu_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_mu_Total = subDir.make<TH1D>("h_mu_Total","h_mu_Total",100, 0,100);
    h_mu_Total->GetXaxis()->SetTitle("Muon p_{T} [GeV] ");
    h_mu_Total->GetYaxis()->SetTitle("Efficiency");
    
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
    
    h_e_NonIsoTau_Efficiency = subDir.make<TH1D>("h_e_NonIsoTau_Efficiency","h_e_NonIsoTau_Efficiency",100, 0,100); 
    h_e_NonIsoTau_Efficiency->GetXaxis()->SetTitle("Electron p_{T} [GeV] ");
    h_e_NonIsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
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
    
    h_tau_NonIsoTau_Efficiency = subDir.make<TH1D>("h_tau_NonIsoTau_Efficiency","h_tau_NonIsoTau_Efficiency",100, 0,100);
    h_tau_NonIsoTau_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_NonIsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_subtau_NonIsoTau_Efficiency = subDir.make<TH1D>("h_subtau_NonIsoTau_Efficiency","h_subtau_NonIsoTau_Efficiency",100, 0,100);
    h_subtau_NonIsoTau_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_subtau_NonIsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_IsoTau_Efficiency = subDir.make<TH1D>("h_tau_IsoTau_Efficiency","h_tau_IsoTau_Efficiency",100, 0,100);
    h_tau_IsoTau_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_IsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_subtau_IsoTau_Efficiency = subDir.make<TH1D>("h_subtau_IsoTau_Efficiency","h_subtau_IsoTau_Efficiency",100, 0,100);
    h_subtau_IsoTau_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_subtau_IsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_Jet_Efficiency = subDir.make<TH1D>("h_tau_Jet_Efficiency","h_tau_Jet_Efficiency",100, 0,100);
    h_tau_Jet_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV]");
    h_tau_Jet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_IsoTau_PtDiff = subDir.make<TH1D>("h_tau_IsoTau_PtDiff","h_tau_IsoTau_PtDiff",100, -50,20); 
    h_tau_IsoTau_PtDiff->GetXaxis()->SetTitle("Tau (L1 p_{T} - Gen p_{T}) [GeV] ");
    h_tau_IsoTau_PtDiff->GetYaxis()->SetTitle("Entries");
    
    h_tau_Tau_PtDiff = subDir.make<TH1D>("h_tau_Tau_PtDiff","h_tau_Tau_PtDiff",100, -50,20); 
    h_tau_Tau_PtDiff->GetXaxis()->SetTitle("Tau (L1 p_{T} - Gen p_{T}) [GeV] ");
    h_tau_Tau_PtDiff->GetYaxis()->SetTitle("Entries");
    
    h_tau_IsoTau_PtRes = subDir.make<TH1D>("h_tau_IsoTau_PtRes","h_tau_IsoTau_PtRes",100, -2,2); 
    h_tau_IsoTau_PtRes->GetXaxis()->SetTitle("Iso-Tau (L1 p_{T} - Gen p_{T})/(Gen p_{T}) ");
    h_tau_IsoTau_PtRes->GetYaxis()->SetTitle("Entries");
    
    h_tau_Tau_PtRes = subDir.make<TH1D>("h_tau_Tau_PtRes","h_tau_Tau_PtRes",100, -2,2); 
    h_tau_Tau_PtRes->GetXaxis()->SetTitle("Tau (L1 p_{T} - Gen p_{T})/(Gen p_{T}) ");
    h_tau_Tau_PtRes->GetYaxis()->SetTitle("Entries");
    
    h_tau_Tau_DeltaRRes = subDir.make<TH1D>("h_tau_Tau_DeltaRRes","h_tau_Tau_DeltaRRes",100, 0,0.3);
    h_tau_Tau_DeltaRRes->GetXaxis()->SetTitle("L1-Gen Tau |#DeltaR| ");
    h_tau_Tau_DeltaRRes->GetYaxis()->SetTitle("Entries");
    
    h_tau_IsoTau_DeltaRRes = subDir.make<TH1D>("h_tau_IsoTau_DeltaRRes","h_tau_IsoTau_DeltaRRes",100, 0,0.3); 
    h_tau_IsoTau_DeltaRRes->GetXaxis()->SetTitle("L1-Gen Iso-Tau |#DeltaR| ");
    h_tau_IsoTau_DeltaRRes->GetYaxis()->SetTitle("Entries");
    
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
    
    h_jet_Tau_Efficiency = subDir.make<TH1D>("h_jet_Tau_Efficiency","h_jet_Tau_Efficiency",100, 0,100);
    h_jet_Tau_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_Tau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_NonIsoTau_Efficiency = subDir.make<TH1D>("h_jet_NonIsoTau_Efficiency","h_jet_NonIsoTau_Efficiency",100, 0,100);
    h_jet_NonIsoTau_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_NonIsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_IsoTau_Efficiency = subDir.make<TH1D>("h_jet_IsoTau_Efficiency","h_jet_IsoTau_Efficiency",100, 0,100); 
    h_jet_IsoTau_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_IsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_Jet_Efficiency = subDir.make<TH1D>("h_jet_Jet_Efficiency","h_jet_Jet_Efficiency",100, 0,100);
    h_jet_Jet_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_jet_Jet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_TauEG_Efficiency = subDir.make<TH1D>("h_jet_TauEG_Efficiency","h_jet_TauEG_Efficiency",100, 0,100); 
    h_jet_TauEG_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_jet_TauEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_IsoTauIsoEG_Efficiency = subDir.make<TH1D>("h_jet_IsoTauIsoEG_Efficiency","h_jet_IsoTauIsoEG_Efficiency",100, 0,100); 
    h_jet_IsoTauIsoEG_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_jet_IsoTauIsoEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_Jet_PtRes = subDir.make<TH1D>("h_jet_Jet_PtRes","h_jet_Jet_PtRes",100, -1,2);
    h_jet_Jet_PtRes->GetXaxis()->SetTitle("Jet (L1 p_{T} - Gen p_{T})/(Gen p_{T}) ");
    h_jet_Jet_PtRes->GetYaxis()->SetTitle("Entries");
    
    h_jet_Jet_PtDiff = subDir.make<TH1D>("h_jet_Jet_PtDiff","h_jet_Jet_PtDiff",100, -30,30); 
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
      
      for(unsigned i=0; i < n_electrons_ && i < nPromptElectrons; i++){
                  
          bool eProceed = false;
                
          if(std::fabs(electrons[i]->vector().Rapidity()) < 2.4) eProceed = true;
          else eProceed = false;

          if(eProceed){
                  
              h_e_Total->Fill(electrons[i]->vector().Pt());
              
              // Check if electron fired EG trigger.
              
              bool MatchedL1 = false;
              double PtDiffelectron = 10000;
              int eL1Index = -1;
          
              for(unsigned j=0; j < n_l1electrons_; j++){
              
                  double DeltaR = sqrt(pow(l1electrons[j]->vector().Phi()-electrons[i]->vector().Phi(),2) + pow(l1electrons[j]->vector().Rapidity()-electrons[i]->vector().Rapidity(),2));
                  double PtDiff = std::fabs(l1electrons[j]->vector().Pt() - electrons[i]->vector().Pt());
       
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
                  if(L1EPtFired && l1electrons[eL1Index]->isolation == 1) h_e_IsoEG_Efficiency->Fill(electrons[i]->vector().Pt());
              }
             
              // Check if electron fired tau and iso-tau triggers.
              
              bool MatchedTauL1 = false;
              bool MatchedIsoTauL1 = false;
              double PtDiffelectronTau = 10000;
              double PtDiffelectronIsoTau = 10000;
              int eTauL1Index = -1;
              int eIsoTauL1Index = -1;
          
              for(unsigned j=0; j < n_l1taus_; j++){
              
                  double DeltaR = sqrt(pow(l1taus[j]->vector().Phi() - electrons[i]->vector().Phi(),2) + pow(l1taus[j]->vector().Rapidity()-electrons[i]->vector().Rapidity(),2));
                  double PtDiff = std::fabs(l1taus[j]->vector().Pt() - electrons[i]->vector().Pt());
                  int tauIso = l1taus[j]->isolation;
                  bool IsoTau = false;
                  if(tauIso == 1) IsoTau = true;
                  if(DeltaR < 0.2 && PtDiff < PtDiffelectronTau && !IsoTau){
                       PtDiffelectronTau = PtDiff;
                       MatchedTauL1 = true;
                       eTauL1Index = j;
                  }
                  
                  if(DeltaR < 0.2 && PtDiff < PtDiffelectronIsoTau && IsoTau){
                       PtDiffelectronIsoTau = PtDiff;
                       MatchedIsoTauL1 = true;
                       eIsoTauL1Index = j;
                  }

              }
              
              if(MatchedTauL1){ 
                  bool L1TauPtFired = false;
                  double L1TauPt = l1taus[eTauL1Index]->vector().Pt();
                  if(L1TauPt >= l1tauCut) L1TauPtFired = true;
                  if(L1TauPtFired) h_e_Tau_Efficiency->Fill(electrons[i]->vector().Pt());
                  if(L1TauPtFired) h_e_NonIsoTau_Efficiency->Fill(electrons[i]->vector().Pt());
              }
              
              if(MatchedIsoTauL1){ 
                  bool L1IsoTauPtFired = false;
                  double L1IsoTauPt = l1taus[eIsoTauL1Index]->vector().Pt();
                  if(L1IsoTauPt >= l1tauCut) L1IsoTauPtFired = true;
                  if(L1IsoTauPtFired) h_e_IsoTau_Efficiency->Fill(electrons[i]->vector().Pt());
                  if(L1IsoTauPtFired) h_e_Tau_Efficiency->Fill(electrons[i]->vector().Pt());
              }
              
              // Check if electron fired Jet trigger.

              bool MatchedJetL1 = false;
              double PtDiffelectronJet = 10000;
              //double DeltaRDiffelectronJet = 10000;
              int eJetL1Index = -1;
          
              for(unsigned j=0; j < n_l1jets_; j++){
              
                  double DeltaR = sqrt(pow(l1jets[j]->vector().Phi()-electrons[i]->vector().Phi(),2) + pow(l1jets[j]->vector().Rapidity()-electrons[i]->vector().Rapidity(),2));
                  double PtDiff = std::fabs(l1jets[j]->vector().Pt() - electrons[i]->vector().Pt());
              
                  if(DeltaR < 0.2 && PtDiff < PtDiffelectronJet){
                       PtDiffelectronJet = PtDiff;
                       MatchedJetL1 = true;
                       eJetL1Index = j;
                  }
                  
              }
              
              if(MatchedJetL1){ 
                  
                  bool L1JetPtFired = false;
                  double L1JetPt = l1jets[eJetL1Index]->vector().Pt();
                  if(L1JetPt >= l1jetCut) L1JetPtFired = true;
                  if(L1JetPtFired) h_e_Jet_Efficiency->Fill(electrons[i]->vector().Pt());
              }
          }
      }
      
      // Taus
      
      for(unsigned i=0; i < n_taus_ && i < nPromptTaus; i++){
                  
          bool tauProceed = false;

          if(std::fabs(taus[i]->vector().Rapidity()) < 2.3) tauProceed = true;
          else tauProceed = false;
          
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
              double PtDifftau = 10000;
              int tauL1Index = -1;

          
              for(unsigned j=0; j < n_l1electrons_; j++){
              
                  double DeltaR = sqrt(pow(l1electrons[j]->vector().Phi()-taus[i]->vector().Phi(),2) + pow(l1electrons[j]->vector().Rapidity()-taus[i]->vector().Rapidity(),2));
                  double PtDiff = std::fabs(l1electrons[j]->vector().Pt() - taus[i]->vector().Pt());
              
                  if(DeltaR < 0.2 && PtDiff < PtDifftau){
                       PtDifftau = PtDiff;
                       MatchedL1 = true;
                       tauL1Index = j;
                  }
                  
              }
              
              if(MatchedL1){ 

                  bool L1EPtFired = false;
                  double L1EPt = l1electrons[tauL1Index]->vector().Pt();
                  if(L1EPt >= l1eCut) L1EPtFired = true;
                  if(L1EPtFired) h_tau_EG_Efficiency->Fill(taus[i]->vector().Pt());
                  if(L1EPtFired && l1electrons[tauL1Index]->isolation == 1) h_tau_IsoEG_Efficiency->Fill(taus[i]->vector().Pt());
              }
             
              // Check if tau fired tau and iso-tau triggers.
              
              bool MatchedTauL1 = false;
              bool MatchedIsoTauL1 = false;
              double PtDifftauTau = 10000;
              double PtDifftauIsoTau = 10000;
              int tauTauL1Index = -1;
              int tauIsoTauL1Index = -1;
          
              for(unsigned j=0; j < n_l1taus_; j++){
              
                  double DeltaR = sqrt(pow(l1taus[j]->vector().Phi()-taus[i]->vector().Phi(),2) + pow(l1taus[j]->vector().Rapidity()-taus[i]->vector().Rapidity(),2));
                  double PtDiff = std::fabs(l1taus[j]->vector().Pt() - taus[i]->vector().Pt());
                  int tauIso = l1taus[j]->isolation;
                  bool IsoTau = false;
                  if(tauIso == 1) IsoTau = true;
                  if(DeltaR < 0.2 && PtDiff < PtDifftauTau && !IsoTau){
                       PtDifftauTau = PtDiff;
                       MatchedTauL1 = true;
                       tauTauL1Index = j;
                  }
                  
                  if(DeltaR < 0.2 && PtDiff < PtDifftauIsoTau && IsoTau){
                       PtDifftauIsoTau = PtDiff;
                       MatchedIsoTauL1 = true;
                       tauIsoTauL1Index = j;
                  }

              }
              
              if(MatchedTauL1){ 
                  bool L1TauPtFired = false;
                  double L1TauPt = l1taus[tauTauL1Index]->vector().Pt();
                  if(L1TauPt >= l1tauCut) L1TauPtFired = true;
                  if(i==0){
                      if(L1TauPtFired) h_tau_Tau_Efficiency->Fill(taus[i]->vector().Pt());
                      if(L1TauPtFired) h_tau_NonIsoTau_Efficiency->Fill(taus[i]->vector().Pt());
                  }
                  else if(i==1){
                      if(L1TauPtFired) h_subtau_Tau_Efficiency->Fill(taus[i]->vector().Pt());
                      if(L1TauPtFired) h_subtau_NonIsoTau_Efficiency->Fill(taus[i]->vector().Pt());
                  }
              }
              
              if(MatchedIsoTauL1){ 
                  bool L1IsoTauPtFired = false;
                  double L1IsoTauPt = l1taus[tauIsoTauL1Index]->vector().Pt();
                  if(L1IsoTauPt >= l1tauCut) L1IsoTauPtFired = true;
                  if(i==0){
                      if(L1IsoTauPtFired) h_tau_IsoTau_Efficiency->Fill(taus[i]->vector().Pt());
                      if(L1IsoTauPtFired) h_tau_Tau_Efficiency->Fill(taus[i]->vector().Pt());
                  }
                  else if(i==1){
                      if(L1IsoTauPtFired) h_subtau_IsoTau_Efficiency->Fill(taus[i]->vector().Pt());
                      if(L1IsoTauPtFired) h_subtau_Tau_Efficiency->Fill(taus[i]->vector().Pt());
                  }
              }
              
              // Check if tau fired Jet trigger.

              bool MatchedJetL1 = false;
              double PtDifftauJet = 10000;
              int tauJetL1Index = -1;
          
              for(unsigned j=0; j < n_l1jets_; j++){
              
                  double DeltaR = sqrt(pow(l1jets[j]->vector().Phi()-taus[i]->vector().Phi(),2) + pow(l1jets[j]->vector().Rapidity()-taus[i]->vector().Rapidity(),2));
                  double PtDiff = std::fabs(l1jets[j]->vector().Pt() - taus[i]->vector().Pt());
              
                  if(DeltaR < 0.2 && PtDiff < PtDifftauJet){
                       PtDifftauJet = PtDiff;
                       MatchedJetL1 = true;
                       tauJetL1Index = j;
                  }
                  
              }
              
              if(MatchedJetL1){ 
                  
                  bool L1JetPtFired = false;
                  double L1JetPt = l1jets[tauJetL1Index]->vector().Pt();
                  if(L1JetPt >= l1jetCut) L1JetPtFired = true;
                  if(L1JetPtFired) h_tau_Jet_Efficiency->Fill(taus[i]->vector().Pt());
              }
              
              // Check if tau fired Muon trigger.
              
              bool MatchedL1Mu = false;
              double PtDifftauMu = 10000;
              int tauMuL1Index = -1;
          
              for(unsigned j=0; j < n_l1muons_; j++){
              
                  double DeltaR = sqrt(pow(l1muons[j]->vector().Phi()-taus[i]->vector().Phi(),2) + pow(l1muons[j]->vector().Rapidity()-taus[i]->vector().Rapidity(),2));
                  double PtDiff = std::fabs(l1muons[j]->vector().Pt() - taus[i]->vector().Pt());
              
                  if(DeltaR < 0.2 && PtDiff < PtDifftauMu){
                      PtDifftauMu = PtDiff;
                      MatchedL1Mu = true;
                      tauMuL1Index = j;
                  }
                  
              }
              
              if(MatchedL1Mu){ 
                  
                  bool L1MuPtFired = false;
                  double L1MuPt = l1muons[tauMuL1Index]->vector().Pt();
                  if(L1MuPt >= l1muCut) L1MuPtFired = true;
                  if(L1MuPtFired) h_tau_Mu_Efficiency->Fill(taus[i]->vector().Pt());
              }
          
          }
          
      }
      
      //Muons
      
      for(unsigned i=0; i < n_muons_ && i< nPromptMuons; i++){
                  
          bool muProceed = false;

          if(std::fabs(muons[i]->vector().Rapidity()) < 2.4) muProceed = true;
          else muProceed = false;

          if(muProceed){
                  
              h_mu_Total->Fill(muons[i]->vector().Pt());
              
              // Check if muon fired Muon trigger.
              
              bool MatchedL1 = false;
              double PtDiffmuon = 10000;
              int muL1Index = -1;
          
              for(unsigned j=0; j < n_l1muons_; j++){
              
                  double DeltaR = sqrt(pow(l1muons[j]->vector().Phi()-muons[i]->vector().Phi(),2) + pow(l1muons[j]->vector().Rapidity()-muons[i]->vector().Rapidity(),2));
                  double PtDiff = std::fabs(l1muons[j]->vector().Pt() - muons[i]->vector().Pt());
              
                  if(DeltaR < 0.3 && PtDiff < PtDiffmuon){
                       PtDiffmuon = PtDiff;
                       MatchedL1 = true;
                       muL1Index = j;
                  }
                  
              }
              
              if(MatchedL1){ 
                  bool L1MuPtFired = false;
                  double L1MuPt = l1muons[muL1Index]->vector().Pt();
                  if(L1MuPt >= l1muCut) L1MuPtFired = true;
                  if(L1MuPtFired) h_mu_Mu_Efficiency->Fill(muons[i]->vector().Pt());
              }  
          }
      }
      
      // Jets
      
      for(unsigned i=0; i < n_jets_ && i < 2; i++){
                  
          bool jetProceed = false;
          if(std::fabs(jets[i]->vector().Rapidity()) < 5) jetProceed = true;
          else jetProceed = false;
          
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

              h_jet_Total->Fill(jets[i]->vector().Pt());
              
              // Check if jet fired EG trigger.
              
              bool MatchedL1 = false;
              double PtDiffjet = 10000;
              int jetL1Index = -1;

          
              for(unsigned j=0; j < n_l1electrons_; j++){
              
                  double DeltaR = sqrt(pow(l1electrons[j]->vector().Phi()-jets[i]->vector().Phi(),2) + pow(l1electrons[j]->vector().Rapidity()-jets[i]->vector().Rapidity(),2));
                  double PtDiff = std::fabs(l1electrons[j]->vector().Pt() - jets[i]->vector().Pt());
              
                  if(DeltaR < 0.5 && PtDiff < PtDiffjet){
                       PtDiffjet = PtDiff;
                       MatchedL1 = true;
                       jetL1Index = j;
                  }
                  
              }
              
              if(MatchedL1){ 

                  bool L1EPtFired = false;
                  double L1EPt = l1electrons[jetL1Index]->vector().Pt();
                  if(L1EPt >= l1eCut) L1EPtFired = true;
                  if(L1EPtFired) h_jet_EG_Efficiency->Fill(jets[i]->vector().Pt());
                  if(L1EPtFired && l1electrons[jetL1Index]->isolation == 1) h_jet_IsoEG_Efficiency->Fill(jets[i]->vector().Pt());
              }
             
              // Check if jet fired tau and iso-tau triggers.
              
              bool MatchedTauL1 = false;
              bool MatchedIsoTauL1 = false;
              double PtDiffjetTau = 10000;
              double PtDiffjetIsoTau = 10000;
              int jetTauL1Index = -1;
              int jetIsoTauL1Index = -1;
          
              for(unsigned j=0; j < n_l1taus_; j++){
              
                  double DeltaR = sqrt(pow(l1taus[j]->vector().Phi()-jets[i]->vector().Phi(),2) + pow(l1taus[j]->vector().Rapidity()-jets[i]->vector().Rapidity(),2));
                  double PtDiff = std::fabs(l1taus[j]->vector().Pt() - jets[i]->vector().Pt());
                  int tauIso = l1taus[j]->isolation;
                  bool IsoTau = false;
                  if(tauIso == 1) IsoTau = true;
                  if(DeltaR < 0.5 && PtDiff < PtDiffjetTau && !IsoTau){
                       PtDiffjetTau = PtDiff;
                       MatchedTauL1 = true;
                       jetTauL1Index = j;
                  }
                  
                  if(DeltaR < 0.5 && PtDiff < PtDiffjetIsoTau && IsoTau){
                       PtDiffjetIsoTau = PtDiff;
                       MatchedIsoTauL1 = true;
                       jetIsoTauL1Index = j;
                  }

              }
              
              if(MatchedTauL1){ 
                  bool L1TauPtFired = false;
                  double L1TauPt = l1taus[jetTauL1Index]->vector().Pt();
                  if(L1TauPt >= l1tauCut) L1TauPtFired = true;
                  if(L1TauPtFired) h_jet_Tau_Efficiency->Fill(jets[i]->vector().Pt());
                  if(L1TauPtFired) h_jet_NonIsoTau_Efficiency->Fill(jets[i]->vector().Pt());
              }
              
              if(MatchedIsoTauL1){ 
                  bool L1IsoTauPtFired = false;
                  double L1IsoTauPt = l1taus[jetIsoTauL1Index]->vector().Pt();
                  if(L1IsoTauPt >= l1tauCut) L1IsoTauPtFired = true;
                  if(L1IsoTauPtFired) h_jet_IsoTau_Efficiency->Fill(jets[i]->vector().Pt());
                  if(L1IsoTauPtFired) h_jet_Tau_Efficiency->Fill(jets[i]->vector().Pt());
              }
              
              if(MatchedTauL1 && MatchedL1){ 
                  bool L1TauPtFired = false;
                  bool L1ElectronPtFired = false;
                  double L1TauPt = l1taus[jetTauL1Index]->vector().Pt();
                  if(L1TauPt >= l1tauCut) L1TauPtFired = true;
                  double L1ElectronPt = l1electrons[jetL1Index]->vector().Pt();
                  if(L1ElectronPt >= l1eCut) L1ElectronPtFired = true;
                  if(L1TauPtFired && L1ElectronPtFired) h_jet_TauEG_Efficiency->Fill(jets[i]->vector().Pt());
              }
              
              if(MatchedIsoTauL1 && MatchedL1 && l1electrons[jetL1Index]->isolation == 1){ 
                  bool L1IsoTauPtFired = false;
                  bool L1IsoElectronPtFired = false;
                  double L1IsoTauPt = l1taus[jetIsoTauL1Index]->vector().Pt();
                  if(L1IsoTauPt >= l1tauCut) L1IsoTauPtFired = true;
                  double L1IsoElectronPt = l1electrons[jetL1Index]->vector().Pt();
                  if(L1IsoElectronPt >= l1eCut) L1IsoElectronPtFired = true;
                  if(L1IsoTauPtFired && L1IsoElectronPtFired) h_jet_IsoTauIsoEG_Efficiency->Fill(jets[i]->vector().Pt());
                  if(L1IsoTauPtFired && L1IsoElectronPtFired) h_jet_TauEG_Efficiency->Fill(jets[i]->vector().Pt());
              }
              
              // Check if jet fired Jet trigger.

              bool MatchedJetL1 = false;
              double PtDiffjetJet = 10000;
              int jetJetL1Index = -1;
          
              for(unsigned j=0; j < n_l1jets_; j++){
              
                  double DeltaR = sqrt(pow(l1jets[j]->vector().Phi()- jets[i]->vector().Phi(),2) + pow(l1jets[j]->vector().Rapidity()-jets[i]->vector().Rapidity(),2));
                  double PtDiff = std::fabs(l1jets[j]->vector().Pt() - jets[i]->vector().Pt());
              
                  if(DeltaR < 0.5 && PtDiff < PtDiffjetJet){
                       PtDiffjetJet = PtDiff;
                       MatchedJetL1 = true;
                       jetJetL1Index = j;
                  }
                  
              }
              
              if(MatchedJetL1){ 
                  bool L1JetPtFired = false;
                  double L1JetPt = l1jets[jetJetL1Index]->vector().Pt();
                  if(L1JetPt >= l1jetCut) L1JetPtFired = true;
                  if(L1JetPtFired) h_jet_Jet_Efficiency->Fill(jets[i]->vector().Pt());
              }
              
              // Check if jet fired Muon trigger.
              
              bool MatchedL1Mu = false;
              double PtDiffjetMu = 10000;
              int jetMuL1Index = -1;
          
              for(unsigned j=0; j < n_l1muons_; j++){
              
                  double DeltaR = sqrt(pow(l1muons[j]->vector().Phi()-jets[i]->vector().Phi(),2) + pow(l1muons[j]->vector().Rapidity()-jets[i]->vector().Rapidity(),2));
                  double PtDiff = std::fabs(l1muons[j]->vector().Pt() - jets[i]->vector().Pt());
              
                  if(DeltaR < 0.5 && PtDiff < PtDiffjetMu){
                      PtDiffjetMu = PtDiff;
                      MatchedL1Mu = true;
                      jetMuL1Index = j;
                  }
                  
              }
              
              if(MatchedL1Mu){ 
                  bool L1MuPtFired = false;
                  double L1MuPt = l1muons[jetMuL1Index]->vector().Pt();
                  if(L1MuPt >= l1muCut) L1MuPtFired = true;
                  if(L1MuPtFired) h_jet_Mu_Efficiency->Fill(jets[i]->vector().Pt());
              }
          
          }
          
      }
      
      // Di-Jets

      
      for(unsigned i=0; i < n_jets_ && i < 2; i++){

          bool jet1IsLepton = false;
          
          for(unsigned k=0; k < n_taus_; k++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-taus[k]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-taus[k]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jet1IsLepton = true;
          }
          for(unsigned k=0; k < n_muons_; k++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-muons[k]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-muons[k]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jet1IsLepton = true;
          }
          for(unsigned k=0; k < n_electrons_; k++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-electrons[k]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-electrons[k]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jet1IsLepton = true;
          }
    
          for(unsigned j=0; j<n_jets_ && j < 2; j++){

              bool jet2IsLepton = false;
              
              for(unsigned k=0; k < n_taus_; k++){
                  double DeltaR = sqrt(pow(jets[j]->vector().Phi()-taus[k]->vector().Phi(),2) + pow(jets[j]->vector().Rapidity()-taus[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.1) jet2IsLepton = true;
              }
              for(unsigned k=0; k < n_muons_; k++){
                  double DeltaR = sqrt(pow(jets[j]->vector().Phi()-muons[k]->vector().Phi(),2) + pow(jets[j]->vector().Rapidity()-muons[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.1) jet2IsLepton = true;
              }
              for(unsigned k=0; k < n_electrons_; k++){
                  double DeltaR = sqrt(pow(jets[j]->vector().Phi()-electrons[k]->vector().Phi(),2) + pow(jets[j]->vector().Rapidity()-electrons[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.1) jet2IsLepton = true;
              }
           
              if(i!=j && !jet1IsLepton && !jet2IsLepton && std::fabs(jets[i]->vector().Rapidity()) < 5 && std::fabs(jets[j]->vector().Rapidity()) < 5){
                  
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

          bool jet1IsLepton = false;
          
          for(unsigned k=0; k < n_taus_; k++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-taus[k]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-taus[k]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jet1IsLepton = true;
          }
          for(unsigned k=0; k < n_muons_; k++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-muons[k]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-muons[k]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jet1IsLepton = true;
          }
          for(unsigned k=0; k < n_electrons_; k++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-electrons[k]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-electrons[k]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jet1IsLepton = true;
          }
    
          for(unsigned j=0; j<n_taus_ && j <nPromptTaus; j++){

              bool tauIsLepton = false;

              for(unsigned k=0; k < n_muons_; k++){
                  double DeltaR = sqrt(pow(taus[j]->vector().Phi()-muons[k]->vector().Phi(),2) + pow(taus[j]->vector().Rapidity()-muons[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.1) tauIsLepton = true;
              }
              for(unsigned k=0; k < n_electrons_; k++){
                  double DeltaR = sqrt(pow(taus[j]->vector().Phi()-electrons[k]->vector().Phi(),2) + pow(taus[j]->vector().Rapidity()-electrons[k]->vector().Rapidity(),2));
                  if(DeltaR < 0.1) tauIsLepton = true;
              }
           
              if(!jet1IsLepton && !tauIsLepton && std::fabs(jets[i]->vector().Rapidity()) < 5 && std::fabs(taus[j]->vector().Rapidity()) < 5){

                  
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

          bool jet1IsLepton = false;
          
          for(unsigned k=0; k < n_taus_; k++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-taus[k]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-taus[k]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jet1IsLepton = true;
          }
          for(unsigned k=0; k < n_muons_; k++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-muons[k]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-muons[k]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jet1IsLepton = true;
          }
          for(unsigned k=0; k < n_electrons_; k++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()-electrons[k]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-electrons[k]->vector().Rapidity(),2));
              if(DeltaR < 0.1) jet1IsLepton = true;
          }
    
          for(unsigned j=0; j<n_electrons_ && j <nPromptElectrons; j++){

           
              if(!jet1IsLepton && std::fabs(jets[i]->vector().Rapidity()) < 5 && std::fabs(electrons[j]->vector().Rapidity()) < 5){

                  
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
          
          for(unsigned k=0; k < GenParticles[j]->daughters().size(); k++) {
              if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 11 || std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 13) isPrompt = false;
              if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 16) GenParticles[j]->set_vector(GenParticles[j]->vector() - GenParticles[GenParticles[j]->daughters().at(k)]->vector());
          }
          
          if(genID == 15 && isPrompt && std::fabs(GenParticles[j]->vector().Rapidity()) < 2.3){
              
              h_gentau_Total->Fill(GenParticles[j]->vector().Pt());
              
              bool MatchedL1 = false;
              bool MatchedIsoL1 = false;
              
              for(unsigned k=0; k<n_l1taus_; k++){
                  double DeltaR = sqrt(pow(l1taus[k]->vector().Phi()- GenParticles[j]->vector().Phi(),2) + pow(l1taus[k]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
                  if(DeltaR < 0.5){
                      MatchedL1 = true;
                      if(l1taus[k]->isolation == 1) MatchedIsoL1 = true;
                  }
              }
              
              if(MatchedL1){
                  h_gentau_Tau_Efficiency->Fill(GenParticles[j]->vector().Pt());
              
                  if(MatchedIsoL1){
                      h_gentau_IsoTau_Efficiency->Fill(GenParticles[j]->vector().Pt());
                  }
              }
          }
   }
      
      return 0;
  }

  int Efficiency::PostAnalysis(){
      h_mu_Mu_Efficiency->Divide(h_mu_Total);  
      
      h_e_EG_Efficiency->Divide(h_e_Total);
      h_e_IsoEG_Efficiency->Divide(h_e_Total);
      h_e_Tau_Efficiency->Divide(h_e_Total);
      h_e_IsoTau_Efficiency->Divide(h_e_Total);
      h_e_NonIsoTau_Efficiency->Divide(h_e_Total);
      h_e_Jet_Efficiency->Divide(h_e_Total);
      
      h_tau_EG_Efficiency->Divide(h_tau_Total);
      h_tau_IsoEG_Efficiency->Divide(h_tau_Total);
      h_tau_Mu_Efficiency->Divide(h_tau_Total);
      h_tau_Tau_Efficiency->Divide(h_tau_Total);
      h_tau_NonIsoTau_Efficiency->Divide(h_tau_Total);
      h_tau_IsoTau_Efficiency->Divide(h_tau_Total);
      h_tau_Jet_Efficiency->Divide(h_tau_Total);
      
      h_subtau_IsoTau_Efficiency->Divide(h_subtau_Total);
      h_subtau_Tau_Efficiency->Divide(h_subtau_Total);
      h_subtau_NonIsoTau_Efficiency->Divide(h_subtau_Total);
      
      h_gentau_Tau_Efficiency->Divide(h_gentau_Total);
      h_gentau_IsoTau_Efficiency->Divide(h_gentau_Total);

      h_jet_EG_Efficiency->Divide(h_jet_Total);
      h_jet_IsoEG_Efficiency->Divide(h_jet_Total);
      h_jet_Mu_Efficiency->Divide(h_jet_Total);
      h_jet_Tau_Efficiency->Divide(h_jet_Total);
      h_jet_NonIsoTau_Efficiency->Divide(h_jet_Total);
      h_jet_IsoTau_Efficiency->Divide(h_jet_Total);
      h_jet_Jet_Efficiency->Divide(h_jet_Total);
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


