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
    
    h_tautau_DeltaR = new TH1D("h_tautau_DeltaR","h_tautau_DeltaR",100, 0,0.3); h_tautau_DeltaR->SetDirectory(fEff);
    h_tautau_DeltaR->GetXaxis()->SetTitle("#Delta R resolution ");
    h_tautau_DeltaR->GetYaxis()->SetTitle("Entries");
    
    h_mu_Mu_Efficiency = new TH1D("h_mu_Mu_Efficiency","h_mu_Mu_Efficiency",100, 0,100); h_mu_Mu_Efficiency->SetDirectory(fEff);
    h_mu_Mu_Efficiency->GetXaxis()->SetTitle("Muon p_{T} [GeV] ");
    h_mu_Mu_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_mu_Total = new TH1D("h_mu_Total","h_mu_Total",100, 0,100); h_mu_Total->SetDirectory(fEff);
    h_mu_Total->GetXaxis()->SetTitle("Muon p_{T} [GeV] ");
    h_mu_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_mu_Mu_PtRes = new TH1D("h_mu_Mu_PtRes","h_mu_Mu_PtRes",100, -1,2); h_mu_Mu_PtRes->SetDirectory(fEff);
    h_mu_Mu_PtRes->GetXaxis()->SetTitle("Muon (L1 p_{T} - Gen p_{T})/(Gen p_{T}) ");
    h_mu_Mu_PtRes->GetYaxis()->SetTitle("Entries");
    
    h_mu_Mu_PtDiff = new TH1D("h_mu_Mu_PtDiff","h_mu_Mu_PtDiff",100, -10,30); h_mu_Mu_PtDiff->SetDirectory(fEff);
    h_mu_Mu_PtDiff->GetXaxis()->SetTitle("Muon (L1 p_{T} - Gen p_{T}) [GeV] ");
    h_mu_Mu_PtDiff->GetYaxis()->SetTitle("Entries");
    
    h_mu_Mu_DeltaRRes = new TH1D("h_mu_Mu_DeltaRRes","h_mu_Mu_DeltaRRes",100, 0,0.3); h_mu_Mu_DeltaRRes->SetDirectory(fEff);
    h_mu_Mu_DeltaRRes->GetXaxis()->SetTitle("L1-Gen Muon |#DeltaR| ");
    h_mu_Mu_DeltaRRes->GetYaxis()->SetTitle("Entries");
    
    h_e_EG_Efficiency = new TH1D("h_e_EG_Efficiency","h_e_EG_Efficiency",100, 0,100); h_e_EG_Efficiency->SetDirectory(fEff);
    h_e_EG_Efficiency->GetXaxis()->SetTitle("Electron p_{T} [GeV] ");
    h_e_EG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_e_IsoEG_Efficiency = new TH1D("h_e_IsoEG_Efficiency","h_e_IsoEG_Efficiency",100, 0,100); h_e_IsoEG_Efficiency->SetDirectory(fEff);
    h_e_IsoEG_Efficiency->GetXaxis()->SetTitle("Electron p_{T} [GeV] ");
    h_e_IsoEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_e_Total = new TH1D("h_e_Total","h_e_Total",100, 0,100); h_e_Total->SetDirectory(fEff);
    h_e_Total->GetXaxis()->SetTitle("Electron p_{T} [GeV] ");
    h_e_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_e_Tau_Efficiency = new TH1D("h_e_Tau_Efficiency","h_e_Tau_Efficiency",100, 0,100); h_e_Tau_Efficiency->SetDirectory(fEff);
    h_e_Tau_Efficiency->GetXaxis()->SetTitle("Electron p_{T} [GeV] ");
    h_e_Tau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_e_NonIsoTau_Efficiency = new TH1D("h_e_NonIsoTau_Efficiency","h_e_NonIsoTau_Efficiency",100, 0,100); h_e_NonIsoTau_Efficiency->SetDirectory(fEff);
    h_e_NonIsoTau_Efficiency->GetXaxis()->SetTitle("Electron p_{T} [GeV] ");
    h_e_NonIsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_e_IsoTau_Efficiency = new TH1D("h_e_IsoTau_Efficiency","h_e_IsoTau_Efficiency",100, 0,100); h_e_IsoTau_Efficiency->SetDirectory(fEff);
    h_e_IsoTau_Efficiency->GetXaxis()->SetTitle("Electron p_{T} [GeV] ");
    h_e_IsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_e_Jet_Efficiency = new TH1D("h_e_Jet_Efficiency","h_e_Jet_Efficiency",100, 0,100); h_e_Jet_Efficiency->SetDirectory(fEff);
    h_e_Jet_Efficiency->GetXaxis()->SetTitle("Electron p_{T} [GeV] ");
    h_e_Jet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_e_EG_PtRes = new TH1D("h_e_EG_PtRes","h_e_EG_PtRes",100, -1,1); h_e_EG_PtRes->SetDirectory(fEff);
    h_e_EG_PtRes->GetXaxis()->SetTitle("Electron (L1 p_{T} - Gen p_{T})/(Gen p_{T}) ");
    h_e_EG_PtRes->GetYaxis()->SetTitle("Entries");
    
    h_e_EG_PtDiff = new TH1D("h_e_EG_PtDiff","h_e_EG_PtDiff",100, -10,10); h_e_EG_PtDiff->SetDirectory(fEff);
    h_e_EG_PtDiff->GetXaxis()->SetTitle("Electron (L1 p_{T} - Gen p_{T}) [GeV] ");
    h_e_EG_PtDiff->GetYaxis()->SetTitle("Entries");
    
    h_e_EG_DeltaRRes = new TH1D("h_e_EG_DeltaRRes","h_e_EG_DeltaRRes",100, 0,0.2); h_e_EG_DeltaRRes->SetDirectory(fEff);
    h_e_EG_DeltaRRes->GetXaxis()->SetTitle("L1-Gen Electron |#DeltaR| ");
    h_e_EG_DeltaRRes->GetYaxis()->SetTitle("Entries");
    
    h_tau_EG_Efficiency = new TH1D("h_tau_EG_Efficiency","h_tau_EG_Efficiency",100, 0,100); h_tau_EG_Efficiency->SetDirectory(fEff);
    h_tau_EG_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_EG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_IsoEG_Efficiency = new TH1D("h_tau_IsoEG_Efficiency","h_tau_IsoEG_Efficiency",100, 0,100); h_tau_IsoEG_Efficiency->SetDirectory(fEff);
    h_tau_IsoEG_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_IsoEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_Mu_Efficiency = new TH1D("h_tau_Mu_Efficiency","h_tau_Mu_Efficiency",100, 0,100); h_tau_Mu_Efficiency->SetDirectory(fEff);
    h_tau_Mu_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_Mu_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_Total = new TH1D("h_tau_Total","h_tau_Total",100, 0,100); h_tau_Total->SetDirectory(fEff);
    h_tau_Total->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_Tau_Efficiency = new TH1D("h_tau_Tau_Efficiency","h_tau_Tau_Efficiency",100, 0,100); h_tau_Tau_Efficiency->SetDirectory(fEff);
    h_tau_Tau_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_Tau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_NonIsoTau_Efficiency = new TH1D("h_tau_NonIsoTau_Efficiency","h_tau_NonIsoTau_Efficiency",100, 0,100); h_tau_NonIsoTau_Efficiency->SetDirectory(fEff);
    h_tau_NonIsoTau_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_NonIsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_IsoTau_Efficiency = new TH1D("h_tau_IsoTau_Efficiency","h_tau_IsoTau_Efficiency",100, 0,100); h_tau_IsoTau_Efficiency->SetDirectory(fEff);
    h_tau_IsoTau_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV] ");
    h_tau_IsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_Jet_Efficiency = new TH1D("h_tau_Jet_Efficiency","h_tau_Jet_Efficiency",100, 0,100); h_tau_Jet_Efficiency->SetDirectory(fEff);
    h_tau_Jet_Efficiency->GetXaxis()->SetTitle("Tau p_{T} [GeV]");
    h_tau_Jet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_tau_IsoTau_PtDiff = new TH1D("h_tau_IsoTau_PtDiff","h_tau_IsoTau_PtDiff",100, -30,15); h_tau_IsoTau_PtDiff->SetDirectory(fEff);
    h_tau_IsoTau_PtDiff->GetXaxis()->SetTitle("Tau (L1 p_{T} - Gen p_{T}) [GeV] ");
    h_tau_IsoTau_PtDiff->GetYaxis()->SetTitle("Entries");
    
    h_tau_Tau_PtDiff = new TH1D("h_tau_Tau_PtDiff","h_tau_Tau_PtDiff",100, -30,15); h_tau_Tau_PtDiff->SetDirectory(fEff);
    h_tau_Tau_PtDiff->GetXaxis()->SetTitle("Tau (L1 p_{T} - Gen p_{T}) [GeV] ");
    h_tau_Tau_PtDiff->GetYaxis()->SetTitle("Entries");
    
    h_tau_IsoTau_PtRes = new TH1D("h_tau_IsoTau_PtRes","h_tau_IsoTau_PtRes",100, -2,2); h_tau_IsoTau_PtRes->SetDirectory(fEff);
    h_tau_IsoTau_PtRes->GetXaxis()->SetTitle("Iso-Tau (L1 p_{T} - Gen p_{T})/(Gen p_{T}) ");
    h_tau_IsoTau_PtRes->GetYaxis()->SetTitle("Entries");
    
    h_tau_Tau_PtRes = new TH1D("h_tau_Tau_PtRes","h_tau_Tau_PtRes",100, -2,2); h_tau_Tau_PtRes->SetDirectory(fEff);
    h_tau_Tau_PtRes->GetXaxis()->SetTitle("Tau (L1 p_{T} - Gen p_{T})/(Gen p_{T}) ");
    h_tau_Tau_PtRes->GetYaxis()->SetTitle("Entries");
    
    h_tau_Tau_DeltaRRes = new TH1D("h_tau_Tau_DeltaRRes","h_tau_Tau_DeltaRRes",100, 0,0.3); h_tau_Tau_DeltaRRes->SetDirectory(fEff);
    h_tau_Tau_DeltaRRes->GetXaxis()->SetTitle("L1-Gen Tau |#DeltaR| ");
    h_tau_Tau_DeltaRRes->GetYaxis()->SetTitle("Entries");
    
    h_tau_IsoTau_DeltaRRes = new TH1D("h_tau_IsoTau_DeltaRRes","h_tau_IsoTau_DeltaRRes",100, 0,0.3); h_tau_IsoTau_DeltaRRes->SetDirectory(fEff);
    h_tau_IsoTau_DeltaRRes->GetXaxis()->SetTitle("L1-Gen Iso-Tau |#DeltaR| ");
    h_tau_IsoTau_DeltaRRes->GetYaxis()->SetTitle("Entries");

    
    h_jet_EG_Efficiency = new TH1D("h_jet_EG_Efficiency","h_jet_EG_Efficiency",100, 0,100); h_jet_EG_Efficiency->SetDirectory(fEff);
    h_jet_EG_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_EG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_IsoEG_Efficiency = new TH1D("h_jet_IsoEG_Efficiency","h_jet_IsoEG_Efficiency",100, 0,100); h_jet_IsoEG_Efficiency->SetDirectory(fEff);
    h_jet_IsoEG_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_IsoEG_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_Mu_Efficiency = new TH1D("h_jet_Mu_Efficiency","h_jet_Mu_Efficiency",100, 0,100); h_jet_Mu_Efficiency->SetDirectory(fEff);
    h_jet_Mu_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_Mu_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_Total = new TH1D("h_jet_Total","h_jet_Total",100, 0,100); h_jet_Total->SetDirectory(fEff);
    h_jet_Total->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_Tau_Efficiency = new TH1D("h_jet_Tau_Efficiency","h_jet_Tau_Efficiency",100, 0,100); h_jet_Tau_Efficiency->SetDirectory(fEff);
    h_jet_Tau_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_Tau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_NonIsoTau_Efficiency = new TH1D("h_jet_NonIsoTau_Efficiency","h_jet_NonIsoTau_Efficiency",100, 0,100); h_jet_NonIsoTau_Efficiency->SetDirectory(fEff);
    h_jet_NonIsoTau_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_NonIsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_IsoTau_Efficiency = new TH1D("h_jet_IsoTau_Efficiency","h_jet_IsoTau_Efficiency",100, 0,100); h_jet_IsoTau_Efficiency->SetDirectory(fEff);
    h_jet_IsoTau_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV] ");
    h_jet_IsoTau_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_Jet_Efficiency = new TH1D("h_jet_Jet_Efficiency","h_jet_Jet_Efficiency",100, 0,100); h_jet_Jet_Efficiency->SetDirectory(fEff);
    h_jet_Jet_Efficiency->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
    h_jet_Jet_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jet_Jet_PtRes = new TH1D("h_jet_Jet_PtRes","h_jet_Jet_PtRes",100, -1,2); h_jet_Jet_PtRes->SetDirectory(fEff);
    h_e_EG_PtRes->GetXaxis()->SetTitle("Jet (L1 p_{T} - Gen p_{T})/(Gen p_{T}) ");
    h_e_EG_PtRes->GetYaxis()->SetTitle("Entries");
    
    h_jet_Jet_PtDiff = new TH1D("h_jet_Jet_PtDiff","h_jet_Jet_PtDiff",100, -10,30); h_jet_Jet_PtDiff->SetDirectory(fEff);
    h_jet_Jet_PtDiff->GetXaxis()->SetTitle("Jet (L1 p_{T} - Gen p_{T}) [GeV] ");
    h_jet_Jet_PtDiff->GetYaxis()->SetTitle("Entries");
    
    h_jet_Jet_DeltaRRes = new TH1D("h_jet_Jet_DeltaRRes","h_jet_Jet_DeltaRRes",100, 0,0.5); h_jet_Jet_DeltaRRes->SetDirectory(fEff);
    h_jet_Jet_DeltaRRes->GetXaxis()->SetTitle("L1-Gen Jet |#DeltaR| ");
    h_jet_Jet_DeltaRRes->GetYaxis()->SetTitle("Entries");
    
    ///
    
    h_jetjet_DeltaEta_Efficiency = new TH1D("h_h_jetjet_DeltaEta_Efficiency","h_jetjet_DeltaEta_Efficiency",100, 0,5); h_jetjet_DeltaEta_Efficiency->SetDirectory(fEff);
    h_jetjet_DeltaEta_Efficiency->GetXaxis()->SetTitle("#Delta#eta");
    h_jetjet_DeltaEta_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jetjet_DeltaEta_Total = new TH1D("h_h_jetjet_DeltaEta_Total","h_jetjet_DeltaEta_Total",100, 0,5); h_jetjet_DeltaEta_Total->SetDirectory(fEff);
    h_jetjet_DeltaEta_Total->GetXaxis()->SetTitle("#Delta#eta");
    h_jetjet_DeltaEta_Total->GetYaxis()->SetTitle("Efficiency");
    
    h_jetjet_Mjj_Efficiency = new TH1D("h_jetjet_Mjj_Efficiency","h_jetjet_Mjj_Efficiency",100, 0,1000); h_jetjet_Mjj_Efficiency->SetDirectory(fEff);
    h_jetjet_Mjj_Efficiency->GetXaxis()->SetTitle("m_{jj} [GeV]");
    h_jetjet_Mjj_Efficiency->GetYaxis()->SetTitle("Efficiency");
    
    h_jetjet_Mjj_Total = new TH1D("h_jetjet_Mjj_Total","h_jetjet_Mjj_Total",100, 0,1000); h_jetjet_Mjj_Total->SetDirectory(fEff);
    h_jetjet_Mjj_Total->GetXaxis()->SetTitle("m_{jj} [GeV]");
    h_jetjet_Mjj_Total->GetYaxis()->SetTitle("Efficiency");
    ///
    
    h_tauJetComp = new TH1D("h_tauJetComp","h_tauJetComp",4, 0,4); h_tauJetComp->SetDirectory(fEff);
    h_tauJetComp->GetXaxis()->SetTitle("");
    h_tauJetComp->GetYaxis()->SetTitle("%");
    h_tauJetComp->GetXaxis()->SetBinLabel(1,"jj");
    h_tauJetComp->GetXaxis()->SetBinLabel(2,"j#tau");
    h_tauJetComp->GetXaxis()->SetBinLabel(3,"#tau#tau");
    h_tauJetComp->GetXaxis()->SetBinLabel(4,"other");
    
    h_l1tauJetComp = new TH1D("h_l1tauJetComp","h_l1tauJetComp",3, 0,3); h_l1tauJetComp->SetDirectory(fEff);
    h_l1tauJetComp->GetXaxis()->SetTitle("");
    h_l1tauJetComp->GetYaxis()->SetTitle("%");
    h_l1tauJetComp->GetXaxis()->SetBinLabel(1,"0 L1 #tau");
    h_l1tauJetComp->GetXaxis()->SetBinLabel(2,"1 L1 #tau");
    h_l1tauJetComp->GetXaxis()->SetBinLabel(3,"2 L1 #tau's");
    
    
    h_gentau_Tau = new TH1D("h_gentau_Tau","h_gentau_Tau",100, 0,100); h_gentau_Tau->SetDirectory(fEff);
    h_gentau_Tau->GetXaxis()->SetTitle("Gen Tau p_{T} [GeV] ");
    h_gentau_Tau->GetYaxis()->SetTitle("Efficiency");
    
    h_gentau_IsoTau = new TH1D("h_gentau_IsoTau","h_gentau_IsoTau",100, 0,100); h_gentau_IsoTau->SetDirectory(fEff);
    h_gentau_IsoTau->GetXaxis()->SetTitle("Gen Tau p_{T} [GeV] ");
    h_gentau_IsoTau->GetYaxis()->SetTitle("Efficiency");
    
    h_gentau_Jet = new TH1D("h_gentau_Jet","h_gentau_Jet",100, 0,100); h_gentau_Jet->SetDirectory(fEff);
    h_gentau_Jet->GetXaxis()->SetTitle("Gen Tau p_{T} [GeV] ");
    h_gentau_Jet->GetYaxis()->SetTitle("Efficiency");
    
    h_gentau_EG = new TH1D("h_gentau_EG","h_gentau_EG",100, 0,100); h_gentau_Tau->SetDirectory(fEff);
    h_gentau_EG->GetXaxis()->SetTitle("Gen Tau p_{T} [GeV] ");
    h_gentau_EG->GetYaxis()->SetTitle("Efficiency");
    
    h_gentau_IsoEG = new TH1D("h_gentau_IsoEG","h_gentau_IsoEG",100, 0,100); h_gentau_IsoEG->SetDirectory(fEff);
    h_gentau_IsoEG->GetXaxis()->SetTitle("Gen Tau p_{T} [GeV] ");
    h_gentau_IsoEG->GetYaxis()->SetTitle("Efficiency");
    
    
    
    h_gentau_IsoTauIsoEG = new TH1D("h_gentau_IsoTauIsoEG","h_gentau_IsoTauIsoEG",100, 0,100); h_gentau_IsoTauIsoEG->SetDirectory(fEff);
    h_gentau_IsoTauIsoEG->GetXaxis()->SetTitle("Gen Tau p_{T} [GeV] ");
    h_gentau_IsoTauIsoEG->GetYaxis()->SetTitle("Efficiency");
    
    h_gentau_TauEG = new TH1D("h_gentau_TauEG","h_gentau_TauEG",100, 0,100); h_gentau_TauEG->SetDirectory(fEff);
    h_gentau_TauEG->GetXaxis()->SetTitle("Gen Tau p_{T} [GeV] ");
    h_gentau_TauEG->GetYaxis()->SetTitle("Efficiency");
    
    h_gentau_IsoTauJet = new TH1D("h_gentau_IsoTauJet","h_gentau_IsoTauJet",100, 0,100); h_gentau_IsoTauJet->SetDirectory(fEff);
    h_gentau_IsoTauJet->GetXaxis()->SetTitle("Gen Tau p_{T} [GeV] ");
    h_gentau_IsoTauJet->GetYaxis()->SetTitle("Efficiency");
    
    h_gentau_TauJet = new TH1D("h_gentau_TauJet","h_gentau_TauJet",100, 0,100); h_gentau_TauJet->SetDirectory(fEff);
    h_gentau_TauJet->GetXaxis()->SetTitle("Gen Tau p_{T} [GeV] ");
    h_gentau_TauJet->GetYaxis()->SetTitle("Efficiency");
    
    h_gentau_IsoTauIsoEGJet = new TH1D("h_gentau_IsoTauIsoEGJet","h_gentau_IsoTauIsoEGJet",100, 0,100); h_gentau_IsoTauIsoEGJet->SetDirectory(fEff);
    h_gentau_IsoTauIsoEGJet->GetXaxis()->SetTitle("Gen Tau p_{T} [GeV] ");
    h_gentau_IsoTauIsoEGJet->GetYaxis()->SetTitle("Efficiency");
    
    h_gentau_TauEGJet = new TH1D("h_gentau_TauEGJet","h_gentau_TauEGJet",100, 0,100); h_gentau_TauEGJet->SetDirectory(fEff);
    h_gentau_TauEGJet->GetXaxis()->SetTitle("Gen Tau p_{T} [GeV] ");
    h_gentau_TauEGJet->GetYaxis()->SetTitle("Efficiency");
    
    h_gentau_total = new TH1D("h_gentau_total","h_gentau_total",100, 0,100); h_gentau_total->SetDirectory(fEff);
    h_gentau_total->GetXaxis()->SetTitle("Gen Tau p_{T} [GeV] ");
    h_gentau_total->GetYaxis()->SetTitle("Efficiency");
    
    ///
    
    h_genelectron_Tau = new TH1D("h_genelectron_Tau","h_genelectron_Tau",100, 0,100); h_genelectron_Tau->SetDirectory(fEff);
    h_genelectron_Tau->GetXaxis()->SetTitle("Gen Electron p_{T} [GeV] ");
    h_genelectron_Tau->GetYaxis()->SetTitle("Efficiency");
    
    h_genelectron_IsoTau = new TH1D("h_genelectron_IsoTau","h_genelectron_IsoTau",100, 0,100); h_genelectron_IsoTau->SetDirectory(fEff);
    h_genelectron_IsoTau->GetXaxis()->SetTitle("Gen Electron p_{T} [GeV] ");
    h_genelectron_IsoTau->GetYaxis()->SetTitle("Efficiency");
    
    h_genelectron_Jet = new TH1D("h_genelectron_Jet","h_genelectron_Jet",100, 0,100); h_genelectron_Jet->SetDirectory(fEff);
    h_genelectron_Jet->GetXaxis()->SetTitle("Gen Electron p_{T} [GeV] ");
    h_genelectron_Jet->GetYaxis()->SetTitle("Efficiency");
    
    h_genelectron_EG = new TH1D("h_genelectron_EG","h_genelectron_EG",100, 0,100); h_genelectron_Tau->SetDirectory(fEff);
    h_genelectron_EG->GetXaxis()->SetTitle("Gen Electron p_{T} [GeV] ");
    h_genelectron_EG->GetYaxis()->SetTitle("Efficiency");
    
    h_genelectron_IsoEG = new TH1D("h_genelectron_IsoEG","h_genelectron_IsoEG",100, 0,100); h_genelectron_IsoEG->SetDirectory(fEff);
    h_genelectron_IsoEG->GetXaxis()->SetTitle("Gen Electron p_{T} [GeV] ");
    h_genelectron_IsoEG->GetYaxis()->SetTitle("Efficiency");
    
    
    
    h_genelectron_IsoTauIsoEG = new TH1D("h_genelectron_IsoTauIsoEG","h_genelectron_IsoTauIsoEG",100, 0,100); h_genelectron_IsoTauIsoEG->SetDirectory(fEff);
    h_genelectron_IsoTauIsoEG->GetXaxis()->SetTitle("Gen Electron p_{T} [GeV] ");
    h_genelectron_IsoTauIsoEG->GetYaxis()->SetTitle("Efficiency");
    
    h_genelectron_TauEG = new TH1D("h_genelectron_TauEG","h_genelectron_TauEG",100, 0,100); h_genelectron_TauEG->SetDirectory(fEff);
    h_genelectron_TauEG->GetXaxis()->SetTitle("Gen Electron p_{T} [GeV] ");
    h_genelectron_TauEG->GetYaxis()->SetTitle("Efficiency");
    
    h_genelectron_IsoTauJet = new TH1D("h_genelectron_IsoTauJet","h_genelectron_IsoTauJet",100, 0,100); h_genelectron_IsoTauJet->SetDirectory(fEff);
    h_genelectron_IsoTauJet->GetXaxis()->SetTitle("Gen Electron p_{T} [GeV] ");
    h_genelectron_IsoTauJet->GetYaxis()->SetTitle("Efficiency");
    
    h_genelectron_TauJet = new TH1D("h_genelectron_TauJet","h_genelectron_TauJet",100, 0,100); h_genelectron_TauJet->SetDirectory(fEff);
    h_genelectron_TauJet->GetXaxis()->SetTitle("Gen Electron p_{T} [GeV] ");
    h_genelectron_TauJet->GetYaxis()->SetTitle("Efficiency");
    
    h_genelectron_IsoTauIsoEGJet = new TH1D("h_genelectron_IsoTauIsoEGJet","h_genelectron_IsoTauIsoEGJet",100, 0,100); h_genelectron_IsoTauIsoEGJet->SetDirectory(fEff);
    h_genelectron_IsoTauIsoEGJet->GetXaxis()->SetTitle("Gen Electron p_{T} [GeV] ");
    h_genelectron_IsoTauIsoEGJet->GetYaxis()->SetTitle("Efficiency");
    
    h_genelectron_TauEGJet = new TH1D("h_genelectron_TauEGJet","h_genelectron_TauEGJet",100, 0,100); h_genelectron_TauEGJet->SetDirectory(fEff);
    h_genelectron_TauEGJet->GetXaxis()->SetTitle("Gen Electron p_{T} [GeV] ");
    h_genelectron_TauEGJet->GetYaxis()->SetTitle("Efficiency");
    
    h_genelectron_total = new TH1D("h_genelectron_total","h_genelectron_total",100, 0,100); h_genelectron_total->SetDirectory(fEff);
    h_genelectron_total->GetXaxis()->SetTitle("Gen Electron p_{T} [GeV] ");
    h_genelectron_total->GetYaxis()->SetTitle("Efficiency");
    
    h_jet2_tt = new TH1D("h_jet2_tt","h_jet2_tt",3, 0,3); h_jet2_tt->SetDirectory(fEff);
    h_jet2_tt->GetXaxis()->SetTitle("");
    h_jet2_tt->GetYaxis()->SetTitle("%");
    h_jet2_tt->GetXaxis()->SetBinLabel(1,"j");
    h_jet2_tt->GetXaxis()->SetBinLabel(2,"e");
    h_jet2_tt->GetXaxis()->SetBinLabel(3,"#tau");
    
    h_jet2_mt = new TH1D("h_jet2_mt","h_jet2_mt",3, 0,3); h_jet2_mt->SetDirectory(fEff);
    h_jet2_mt->GetXaxis()->SetTitle("");
    h_jet2_mt->GetYaxis()->SetTitle("%");
    h_jet2_mt->GetXaxis()->SetBinLabel(1,"j");
    h_jet2_mt->GetXaxis()->SetBinLabel(2,"e");
    h_jet2_mt->GetXaxis()->SetBinLabel(3,"#tau");
    
    h_jet2_et = new TH1D("h_jet2_et","h_jet2_et",3, 0,3); h_jet2_et->SetDirectory(fEff);
    h_jet2_et->GetXaxis()->SetTitle("");
    h_jet2_et->GetYaxis()->SetTitle("%");
    h_jet2_et->GetXaxis()->SetBinLabel(1,"j");
    h_jet2_et->GetXaxis()->SetBinLabel(2,"e");
    h_jet2_et->GetXaxis()->SetBinLabel(3,"#tau");
    
    h_jet2_em = new TH1D("h_jet2_em","h_jet2_em",3, 0,3); h_jet2_em->SetDirectory(fEff);
    h_jet2_em->GetXaxis()->SetTitle("");
    h_jet2_em->GetYaxis()->SetTitle("%");
    h_jet2_em->GetXaxis()->SetBinLabel(1,"j");
    h_jet2_em->GetXaxis()->SetBinLabel(2,"e");
    h_jet2_em->GetXaxis()->SetBinLabel(3,"#tau");
    
    h_jet1_tt = new TH1D("h_jet1_tt","h_jet1_tt",3, 0,3); h_jet1_tt->SetDirectory(fEff);
    h_jet1_tt->GetXaxis()->SetTitle("");
    h_jet1_tt->GetYaxis()->SetTitle("%");
    h_jet1_tt->GetXaxis()->SetBinLabel(1,"j");
    h_jet1_tt->GetXaxis()->SetBinLabel(2,"e");
    h_jet1_tt->GetXaxis()->SetBinLabel(3,"#tau");
    
    h_jet1_mt = new TH1D("h_jet1_mt","h_jet1_mt",3, 0,3); h_jet1_mt->SetDirectory(fEff);
    h_jet1_mt->GetXaxis()->SetTitle("");
    h_jet1_mt->GetYaxis()->SetTitle("%");
    h_jet1_mt->GetXaxis()->SetBinLabel(1,"j");
    h_jet1_mt->GetXaxis()->SetBinLabel(2,"e");
    h_jet1_mt->GetXaxis()->SetBinLabel(3,"#tau");
    
    h_jet1_et = new TH1D("h_jet1_et","h_jet1_et",3, 0,4); h_jet1_et->SetDirectory(fEff);
    h_jet1_et->GetXaxis()->SetTitle("");
    h_jet1_et->GetYaxis()->SetTitle("%");
    h_jet1_et->GetXaxis()->SetBinLabel(1,"j");
    h_jet1_et->GetXaxis()->SetBinLabel(2,"e");
    h_jet1_et->GetXaxis()->SetBinLabel(3,"#tau");
    h_jet1_et->GetXaxis()->SetBinLabel(4,"other");
    
    h_jet1_em = new TH1D("h_jet1_em","h_jet1_em",4, 0,4); h_jet1_em->SetDirectory(fEff);
    h_jet1_em->GetXaxis()->SetTitle("");
    h_jet1_em->GetYaxis()->SetTitle("%");
    h_jet1_em->GetXaxis()->SetBinLabel(1,"j");
    h_jet1_em->GetXaxis()->SetBinLabel(2,"e");
    h_jet1_em->GetXaxis()->SetBinLabel(3,"#tau");
    h_jet1_em->GetXaxis()->SetBinLabel(4,"other");
    
    h_DeltaEta_tt = new TH1D("h_DeltaEta_tt","h_DeltaEta_tt",6, 0,6); h_DeltaEta_tt->SetDirectory(fEff);
    h_DeltaEta_tt->GetXaxis()->SetTitle("");
    h_DeltaEta_tt->GetYaxis()->SetTitle("%");
    h_DeltaEta_tt->GetXaxis()->SetBinLabel(1,"jj");
    h_DeltaEta_tt->GetXaxis()->SetBinLabel(2,"j#tau");
    h_DeltaEta_tt->GetXaxis()->SetBinLabel(3,"je");
    h_DeltaEta_tt->GetXaxis()->SetBinLabel(4,"#tau#tau");
    h_DeltaEta_tt->GetXaxis()->SetBinLabel(5,"ee");
    h_DeltaEta_tt->GetXaxis()->SetBinLabel(6,"e#tau");
    
    h_DeltaEta_et = new TH1D("h_DeltaEta_et","h_DeltaEta_et",6, 0,6); h_DeltaEta_et->SetDirectory(fEff);
    h_DeltaEta_et->GetXaxis()->SetTitle("");
    h_DeltaEta_et->GetYaxis()->SetTitle("%");
    h_DeltaEta_et->GetXaxis()->SetBinLabel(1,"jj");
    h_DeltaEta_et->GetXaxis()->SetBinLabel(2,"j#tau");
    h_DeltaEta_et->GetXaxis()->SetBinLabel(3,"je");
    h_DeltaEta_et->GetXaxis()->SetBinLabel(4,"#tau#tau");
    h_DeltaEta_et->GetXaxis()->SetBinLabel(5,"ee");
    h_DeltaEta_et->GetXaxis()->SetBinLabel(6,"e#tau");
    
    h_DeltaEta_mt = new TH1D("h_DeltaEta_mt","h_DeltaEta_mt",6, 0,6); h_DeltaEta_mt->SetDirectory(fEff);
    h_DeltaEta_mt->GetXaxis()->SetTitle("");
    h_DeltaEta_mt->GetYaxis()->SetTitle("%");
    h_DeltaEta_mt->GetXaxis()->SetBinLabel(1,"jj");
    h_DeltaEta_mt->GetXaxis()->SetBinLabel(2,"j#tau");
    h_DeltaEta_mt->GetXaxis()->SetBinLabel(3,"je");
    h_DeltaEta_mt->GetXaxis()->SetBinLabel(4,"#tau#tau");
    h_DeltaEta_mt->GetXaxis()->SetBinLabel(5,"ee");
    h_DeltaEta_mt->GetXaxis()->SetBinLabel(6,"e#tau");

    h_DeltaEta_em = new TH1D("h_DeltaEta_em","h_DeltaEta_em",6, 0,6); h_DeltaEta_em->SetDirectory(fEff);
    h_DeltaEta_em->GetXaxis()->SetTitle("");
    h_DeltaEta_em->GetYaxis()->SetTitle("%");
    h_DeltaEta_em->GetXaxis()->SetBinLabel(1,"jj");
    h_DeltaEta_em->GetXaxis()->SetBinLabel(2,"j#tau");
    h_DeltaEta_em->GetXaxis()->SetBinLabel(3,"je");
    h_DeltaEta_em->GetXaxis()->SetBinLabel(4,"#tau#tau");
    h_DeltaEta_em->GetXaxis()->SetBinLabel(5,"ee");
    h_DeltaEta_em->GetXaxis()->SetBinLabel(6,"e#tau");
    
    h_Mjj_tt = new TH1D("h_Mjj_tt","h_Mjj_tt",6, 0,6); h_Mjj_tt->SetDirectory(fEff);
    h_Mjj_tt->GetXaxis()->SetTitle("");
    h_Mjj_tt->GetYaxis()->SetTitle("%");
    h_Mjj_tt->GetXaxis()->SetBinLabel(1,"jj");
    h_Mjj_tt->GetXaxis()->SetBinLabel(2,"j#tau");
    h_Mjj_tt->GetXaxis()->SetBinLabel(3,"je");
    h_Mjj_tt->GetXaxis()->SetBinLabel(4,"#tau#tau");
    h_Mjj_tt->GetXaxis()->SetBinLabel(5,"ee");
    h_Mjj_tt->GetXaxis()->SetBinLabel(6,"e#tau");
    
    h_Mjj_et = new TH1D("h_Mjj_et","h_Mjj_et",6, 0,6); h_Mjj_et->SetDirectory(fEff);
    h_Mjj_et->GetXaxis()->SetTitle("");
    h_Mjj_et->GetYaxis()->SetTitle("%");
    h_Mjj_et->GetXaxis()->SetBinLabel(1,"jj");
    h_Mjj_et->GetXaxis()->SetBinLabel(2,"j#tau");
    h_Mjj_et->GetXaxis()->SetBinLabel(3,"je");
    h_Mjj_et->GetXaxis()->SetBinLabel(4,"#tau#tau");
    h_Mjj_et->GetXaxis()->SetBinLabel(5,"ee");
    h_Mjj_et->GetXaxis()->SetBinLabel(6,"e#tau");
    
    h_Mjj_mt = new TH1D("h_Mjj_mt","h_Mjj_mt",6, 0,6); h_Mjj_mt->SetDirectory(fEff);
    h_Mjj_mt->GetXaxis()->SetTitle("");
    h_Mjj_mt->GetYaxis()->SetTitle("%");
    h_Mjj_mt->GetXaxis()->SetBinLabel(1,"jj");
    h_Mjj_mt->GetXaxis()->SetBinLabel(2,"j#tau");
    h_Mjj_mt->GetXaxis()->SetBinLabel(3,"je");
    h_Mjj_mt->GetXaxis()->SetBinLabel(4,"#tau#tau");
    h_Mjj_mt->GetXaxis()->SetBinLabel(5,"ee");
    h_Mjj_mt->GetXaxis()->SetBinLabel(6,"e#tau");
    
    h_Mjj_em = new TH1D("h_Mjj_em","h_Mjj_em",6, 0,6); h_Mjj_em->SetDirectory(fEff);
    h_Mjj_em->GetXaxis()->SetTitle("");
    h_Mjj_em->GetYaxis()->SetTitle("%");
    h_Mjj_em->GetXaxis()->SetBinLabel(1,"jj");
    h_Mjj_em->GetXaxis()->SetBinLabel(2,"j#tau");
    h_Mjj_em->GetXaxis()->SetBinLabel(3,"je");
    h_Mjj_em->GetXaxis()->SetBinLabel(4,"#tau#tau");
    h_Mjj_em->GetXaxis()->SetBinLabel(5,"ee");
    h_Mjj_em->GetXaxis()->SetBinLabel(6,"e#tau");
    
  }

    Efficiency::~Efficiency(){
      
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
      
      h_jet_EG_Efficiency->Divide(h_jet_Total);
      h_jet_IsoEG_Efficiency->Divide(h_jet_Total);
      h_jet_Mu_Efficiency->Divide(h_jet_Total);
      h_jet_Tau_Efficiency->Divide(h_jet_Total);
      h_jet_NonIsoTau_Efficiency->Divide(h_jet_Total);
      h_jet_IsoTau_Efficiency->Divide(h_jet_Total);
      h_jet_Jet_Efficiency->Divide(h_jet_Total);
      
      h_jetjet_Mjj_Efficiency->Divide(h_jetjet_Mjj_Total);
      h_jetjet_DeltaEta_Efficiency->Divide(h_jetjet_DeltaEta_Total);
      
      h_l1tauJetComp->Scale(100/h_l1tauJetComp->GetEntries());
      h_tauJetComp->Scale(100/h_tauJetComp->GetEntries());
      
      h_gentau_Jet->Divide(h_gentau_total);
      h_gentau_EG->Divide(h_gentau_total);
      h_gentau_Tau->Divide(h_gentau_total);
      h_gentau_IsoEG->Divide(h_gentau_total);
      h_gentau_IsoTau->Divide(h_gentau_total);
      h_gentau_IsoTauIsoEG->Divide(h_gentau_total);
      h_gentau_TauEG->Divide(h_gentau_total);
      h_gentau_IsoTauJet->Divide(h_gentau_total);
      h_gentau_TauJet->Divide(h_gentau_total);
      h_gentau_IsoTauIsoEGJet->Divide(h_gentau_total);
      h_gentau_TauEGJet->Divide(h_gentau_total);
      
      h_genelectron_Jet->Divide(h_genelectron_total);
      h_genelectron_EG->Divide(h_genelectron_total);
      h_genelectron_Tau->Divide(h_genelectron_total);
      h_genelectron_IsoEG->Divide(h_genelectron_total);
      h_genelectron_IsoTau->Divide(h_genelectron_total);
      h_genelectron_IsoTauIsoEG->Divide(h_genelectron_total);
      h_genelectron_TauEG->Divide(h_genelectron_total);
      h_genelectron_IsoTauJet->Divide(h_genelectron_total);
      h_genelectron_TauJet->Divide(h_genelectron_total);
      h_genelectron_IsoTauIsoEGJet->Divide(h_genelectron_total);
      h_genelectron_TauEGJet->Divide(h_genelectron_total);
      
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
      h_jet2_em->Scale(100/h_jet2_em->GetEntries());
      h_jet2_et->Scale(100/h_jet2_et->GetEntries());
      h_jet2_mt->Scale(100/h_jet2_mt->GetEntries());
      h_jet2_tt->Scale(100/h_jet2_tt->GetEntries());
      
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
      
      double l1eCut = 25;
      double l1muCut = 25;
      double l1tauCut = 30;
      double l1jetCut = 70;
      //double l1MjjCut = 200;
      //double l1DeltaEtaCut = 2.5;
      //double l1METCut = 20;
      //double l1MHTCut = 50;
      
      for(unsigned i=0; i < n_electrons_; i++){
                  
          bool eProceed = false;
                  
          //double electroniso =  electrons[i]->dr04_pfiso_charged_all() + std::max(electrons[i]->dr04_pfiso_neutral() + electrons[i]->dr04_pfiso_gamma() - 0.5 * electrons[i]->dr04_pfiso_pu(), 0.0);
          //electroniso = electroniso / electrons[i]->vector().Pt();
          if(/*electroniso <= 0.1 && std::fabs(electrons[i]->dxy_vertex()) < 0.045 && std::fabs(electrons[i]->dz_vertex()) < 0.2 &&*/ std::fabs(electrons[i]->vector().Rapidity()) < 2.1) eProceed = true;
          else eProceed = false;

          if(eProceed){
              
              int eGenIndex = -1;
              bool MatchedOffline = false;
              double PtDiffelectron = 10000;
              
              for(unsigned j=0; j< n_genParticles_; j++){

                  int genID = std::fabs(GenParticles[j]->pdgid());
                  bool isPrompt = false;
                  //if((GenParticles[j]->statusFlags().at(0) == true || GenParticles[j]->statusFlags().at(5) == true) && genID == 11) isPrompt = true;
                  
                  for(unsigned k=0; k < GenParticles[j]->mothers().size(); k++) {
                      if(std::fabs(GenParticles[GenParticles[j]->mothers().at(k)]->pdgid()) == 15){
                          int ParentgenIndex = GenParticles[j]->mothers().at(k);
                          int GPgenID = std::fabs(GenParticles[GenParticles[ParentgenIndex]->mothers().at(0)]->pdgid());
                          if(genID == 11 && GPgenID == 25) isPrompt = true;
                      }
                  }
                 
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
                  double EDeltaRRes;
                  double EPtRes;
              
                  for(unsigned j=0; j < n_l1electrons_; j++){
                  
                      double DeltaR = sqrt(pow(l1electrons[j]->vector().Phi()-GenParticles[eGenIndex]->vector().Phi(),2) + pow(l1electrons[j]->vector().Rapidity()-GenParticles[eGenIndex]->vector().Rapidity(),2));
                      double PtDiff = std::fabs(l1electrons[j]->vector().Pt() - GenParticles[eGenIndex]->vector().Pt());
                  
                      if(DeltaR < 0.2 && PtDiff < PtDiffelectron){
                           PtDiffelectron = PtDiff;
                           MatchedL1 = true;
                           eL1Index = j;
                           EDeltaRRes = DeltaR;
                           EPtRes = l1electrons[j]->vector().Pt() - GenParticles[eGenIndex]->vector().Pt();
                      }
                      
                  }
                  
                  if(MatchedL1){ 
                      h_e_EG_PtDiff->Fill(EPtRes);
                      h_e_EG_PtRes->Fill(EPtRes/GenParticles[eGenIndex]->vector().Pt());
                      h_e_EG_DeltaRRes->Fill(EDeltaRRes);
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
                  
                      double DeltaR = sqrt(pow(l1taus[j]->vector().Phi()-GenParticles[eGenIndex]->vector().Phi(),2) + pow(l1taus[j]->vector().Rapidity()-GenParticles[eGenIndex]->vector().Rapidity(),2));
                      double PtDiff = std::fabs(l1taus[j]->vector().Pt() - GenParticles[eGenIndex]->vector().Pt());
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
                  
                      double DeltaR = sqrt(pow(l1jets[j]->vector().Phi()-GenParticles[eGenIndex]->vector().Phi(),2) + pow(l1jets[j]->vector().Rapidity()-GenParticles[eGenIndex]->vector().Rapidity(),2));
                      double PtDiff = std::fabs(l1jets[j]->vector().Pt() - GenParticles[eGenIndex]->vector().Pt());
                  
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
      }
      
      // Taus
      
      for(unsigned i=0; i < n_taus_; i++){
                  
          bool tauProceed = false;
                  
          //double tauiso = taus[i]->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
          if(/*tauiso < 0.5 &&*/  /*std::fabs(taus[i]->lead_dz_vertex()) < 0.2 &&*/ std::fabs(taus[i]->vector().Rapidity()) < 2.3) tauProceed = true;
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
              
              int tauGenIndex = -1;
              bool MatchedOffline = false;
              double PtDifftau = 10000;
              
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
                 
                  double DeltaR = sqrt(pow(taus[i]->vector().Phi()-GenParticles[j]->vector().Phi(),2) + pow(taus[i]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
                  double PtDiff = std::fabs(taus[i]->vector().Pt() - GenParticles[j]->vector().Pt());
                  
                  if(DeltaR < 0.2 && PtDiff < PtDifftau && isPrompt){
                       PtDifftau = PtDiff;
                       MatchedOffline = true;
                       tauGenIndex = j;
                  }
              }
              
              if(MatchedOffline){
                  
                  h_tau_Total->Fill(taus[i]->vector().Pt());
                  
                  // Check if tau fired EG trigger.
                  
                  bool MatchedL1 = false;
                  double PtDifftau = 10000;
                  int tauL1Index = -1;

              
                  for(unsigned j=0; j < n_l1electrons_; j++){
                  
                      double DeltaR = sqrt(pow(l1electrons[j]->vector().Phi()-GenParticles[tauGenIndex]->vector().Phi(),2) + pow(l1electrons[j]->vector().Rapidity()-GenParticles[tauGenIndex]->vector().Rapidity(),2));
                      double PtDiff = std::fabs(l1electrons[j]->vector().Pt() - GenParticles[tauGenIndex]->vector().Pt());
                  
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
                  double TauDeltaRRes;
                  double IsoTauDeltaRRes;
                  double TauPtRes;
                  double IsoTauPtRes;
              
                  for(unsigned j=0; j < n_l1taus_; j++){
                  
                      double DeltaR = sqrt(pow(l1taus[j]->vector().Phi()-GenParticles[tauGenIndex]->vector().Phi(),2) + pow(l1taus[j]->vector().Rapidity()-GenParticles[tauGenIndex]->vector().Rapidity(),2));
                      double PtDiff = std::fabs(l1taus[j]->vector().Pt() - GenParticles[tauGenIndex]->vector().Pt());
                      int tauIso = l1taus[j]->isolation;
                      bool IsoTau = false;
                      if(tauIso == 1) IsoTau = true;
                      if(DeltaR < 0.2 && PtDiff < PtDifftauTau && !IsoTau){
                           PtDifftauTau = PtDiff;
                           MatchedTauL1 = true;
                           tauTauL1Index = j;
                           TauDeltaRRes = DeltaR;
                           TauPtRes = l1taus[j]->vector().Pt() - GenParticles[tauGenIndex]->vector().Pt();
                      }
                      
                      if(DeltaR < 0.2 && PtDiff < PtDifftauIsoTau && IsoTau){
                           PtDifftauIsoTau = PtDiff;
                           MatchedIsoTauL1 = true;
                           tauIsoTauL1Index = j;
                           IsoTauDeltaRRes = DeltaR;
                           IsoTauPtRes = l1taus[j]->vector().Pt() - GenParticles[tauGenIndex]->vector().Pt();
                      }

                  }
                  
                  if(MatchedTauL1){ 
                      h_tau_Tau_PtDiff->Fill(TauPtRes);
                      h_tau_Tau_PtRes->Fill(TauPtRes/GenParticles[tauGenIndex]->vector().Pt());
                      h_tau_Tau_DeltaRRes->Fill(TauDeltaRRes);
                      bool L1TauPtFired = false;
                      double L1TauPt = l1taus[tauTauL1Index]->vector().Pt();
                      if(L1TauPt >= l1tauCut) L1TauPtFired = true;
                      if(L1TauPtFired) h_tau_Tau_Efficiency->Fill(taus[i]->vector().Pt());
                      if(L1TauPtFired) h_tau_NonIsoTau_Efficiency->Fill(taus[i]->vector().Pt());
                  }
                  
                  if(MatchedIsoTauL1){ 
                      h_tau_Tau_PtRes->Fill(IsoTauPtRes/GenParticles[tauGenIndex]->vector().Pt());
                      h_tau_IsoTau_PtRes->Fill(IsoTauPtRes/GenParticles[tauGenIndex]->vector().Pt());
                      h_tau_Tau_PtDiff->Fill(IsoTauPtRes);
                      h_tau_IsoTau_PtDiff->Fill(IsoTauPtRes);
                      h_tau_Tau_DeltaRRes->Fill(IsoTauDeltaRRes);
                      h_tau_IsoTau_DeltaRRes->Fill(IsoTauDeltaRRes);
                      bool L1IsoTauPtFired = false;
                      double L1IsoTauPt = l1taus[tauIsoTauL1Index]->vector().Pt();
                      if(L1IsoTauPt >= l1tauCut) L1IsoTauPtFired = true;
                      if(L1IsoTauPtFired) h_tau_IsoTau_Efficiency->Fill(taus[i]->vector().Pt());
                      if(L1IsoTauPtFired) h_tau_Tau_Efficiency->Fill(taus[i]->vector().Pt());
                  }
                  
                  // Check if tau fired Jet trigger.

                  bool MatchedJetL1 = false;
                  double PtDifftauJet = 10000;
                  //double DeltaRDiffelectronJet = 10000;
                  int tauJetL1Index = -1;
              
                  for(unsigned j=0; j < n_l1jets_; j++){
                  
                      double DeltaR = sqrt(pow(l1jets[j]->vector().Phi()-GenParticles[tauGenIndex]->vector().Phi(),2) + pow(l1jets[j]->vector().Rapidity()-GenParticles[tauGenIndex]->vector().Rapidity(),2));
                      double PtDiff = std::fabs(l1jets[j]->vector().Pt() - GenParticles[tauGenIndex]->vector().Pt());
                  
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
                  
                      double DeltaR = sqrt(pow(l1muons[j]->vector().Phi()-GenParticles[tauGenIndex]->vector().Phi(),2) + pow(l1muons[j]->vector().Rapidity()-GenParticles[tauGenIndex]->vector().Rapidity(),2));
                      double PtDiff = std::fabs(l1muons[j]->vector().Pt() - GenParticles[tauGenIndex]->vector().Pt());
                  
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
          
      }
      
      //Muons
      
      for(unsigned i=0; i < n_muons_; i++){
                  
          bool muProceed = false;
                  
          //double muoniso =  muons[i]->dr04_pfiso_charged_all() + std::max(muons[i]->dr04_pfiso_neutral() + muons[i]->dr04_pfiso_gamma() - 0.5 * muons[i]->dr04_pfiso_pu(), 0.0);
          //muoniso = muoniso / muons[i]->vector().Pt();
          if(/*muoniso <= 0.1 && std::fabs(muons[i]->dxy_vertex()) < 0.045 && std::fabs(muons[i]->dz_vertex()) < 0.2 &&*/ std::fabs(muons[i]->vector().Rapidity()) < 2.1) muProceed = true;
          else muProceed = false;

          if(muProceed){
              
              int muGenIndex = -1;
              bool MatchedOffline = false;
              double PtDiffmuon = 10000;
              
              for(unsigned j=0; j< n_genParticles_; j++){

                  int genID = std::fabs(GenParticles[j]->pdgid());
                  bool isPrompt = false;
                  //if((GenParticles[j]->statusFlags().at(0) == true || GenParticles[j]->statusFlags().at(5) == true) && genID == 13) isPrompt = true;
                  for(unsigned k=0; k < GenParticles[j]->mothers().size(); k++) {
                      if(std::fabs(GenParticles[GenParticles[j]->mothers().at(k)]->pdgid()) == 15){
                          int ParentgenIndex = GenParticles[j]->mothers().at(k);
                          int GPgenID = std::fabs(GenParticles[GenParticles[ParentgenIndex]->mothers().at(0)]->pdgid());
                          if(genID == 13 && GPgenID == 25) isPrompt = true;
                      }
                  }
                  
                  double DeltaR = sqrt(pow(muons[i]->vector().Phi()-GenParticles[j]->vector().Phi(),2) + pow(muons[i]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
                  double PtDiff = std::fabs(muons[i]->vector().Pt() - GenParticles[j]->vector().Pt());
                  
                  if(DeltaR < 0.1 && PtDiff < PtDiffmuon && isPrompt){
                       PtDiffmuon = PtDiff;
                       MatchedOffline = true;
                       muGenIndex = j;
                  }
              }
              
              if(MatchedOffline){
                  
                  h_mu_Total->Fill(muons[i]->vector().Pt());
                  
                  // Check if muon fired Muon trigger.
                  
                  bool MatchedL1 = false;
                  double PtDiffmuon = 10000;
                  int muL1Index = -1;
                  double MuDeltaRRes;
                  double MuPtRes;
              
                  for(unsigned j=0; j < n_l1muons_; j++){
                  
                      double DeltaR = sqrt(pow(l1muons[j]->vector().Phi()-GenParticles[muGenIndex]->vector().Phi(),2) + pow(l1muons[j]->vector().Rapidity()-GenParticles[muGenIndex]->vector().Rapidity(),2));
                      double PtDiff = std::fabs(l1muons[j]->vector().Pt() - GenParticles[muGenIndex]->vector().Pt());
                  
                      if(DeltaR < 0.2 && PtDiff < PtDiffmuon){
                           PtDiffmuon = PtDiff;
                           MatchedL1 = true;
                           muL1Index = j;
                           MuDeltaRRes = DeltaR;
                           MuPtRes = l1muons[j]->vector().Pt() - GenParticles[muGenIndex]->vector().Pt();
                      }
                      
                  }
                  
                  if(MatchedL1){ 
                      h_mu_Mu_PtRes->Fill(MuPtRes/GenParticles[muGenIndex]->vector().Pt());
                      h_mu_Mu_PtDiff->Fill(MuPtRes);
                      h_mu_Mu_DeltaRRes->Fill(MuDeltaRRes);
                      bool L1MuPtFired = false;
                      double L1MuPt = l1muons[muL1Index]->vector().Pt();
                      if(L1MuPt >= l1muCut) L1MuPtFired = true;
                      if(L1MuPtFired) h_mu_Mu_Efficiency->Fill(muons[i]->vector().Pt());
                  }
                 
                  
              }
          }
      }
      
      // Jets
      
      for(unsigned i=0; i < n_jets_; i++){
                  
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
              
              int jetGenIndex = -1;
              bool MatchedOffline = false;
              double PtDiffjet = 10000;
              
              for(unsigned j=0; j<n_genJets_; j++){
                  
                  double DeltaR = sqrt(pow(jets[i]->vector().Phi()-genjets[j]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-genjets[j]->vector().Rapidity(),2));
                  double PtDiff = std::fabs(jets[i]->vector().Pt() - genjets[j]->vector().Pt());
                  
                  if(DeltaR < 0.4 && PtDiff < PtDiffjet){
                      PtDiffjet = PtDiff;
                      MatchedOffline = true;
                      jetGenIndex = j;
                  }
                  
              }
              
              for(unsigned j=0; j< n_genParticles_; j++){

                  int genID = std::fabs(GenParticles[j]->pdgid());
                  bool isPrompt = false;
                  if((GenParticles[j]->statusFlags().at(0) == true || GenParticles[j]->statusFlags().at(5) == true) && (genID == 11 || genID == 13)) isPrompt = true;
                  
                  for(unsigned k=0; k < GenParticles[j]->mothers().size(); k++) {
                      if(std::fabs(GenParticles[GenParticles[j]->mothers().at(k)]->pdgid()) == 25){
                          if(genID == 15) isPrompt = true;
                      }
                  }
                  
                  for(unsigned k=0; k < GenParticles[j]->daughters().size(); k++) {
                      if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 11 || std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 13) isPrompt = false;
                      if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 16) GenParticles[j]->set_vector(GenParticles[j]->vector() - GenParticles[GenParticles[j]->daughters().at(k)]->vector());
                  }
                 
                  double DeltaR = sqrt(pow(jets[i]->vector().Phi()-GenParticles[j]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
                  
                  if(DeltaR < 0.2 && isPrompt){
                       MatchedOffline = false;
                  }
              }
              
              if(MatchedOffline){
                  
                  h_jet_Total->Fill(jets[i]->vector().Pt());
                  
                  // Check if jet fired EG trigger.
                  
                  bool MatchedL1 = false;
                  double PtDiffjet = 10000;
                  int jetL1Index = -1;

              
                  for(unsigned j=0; j < n_l1electrons_; j++){
                  
                      double DeltaR = sqrt(pow(l1electrons[j]->vector().Phi()-genjets[jetGenIndex]->vector().Phi(),2) + pow(l1electrons[j]->vector().Rapidity()-genjets[jetGenIndex]->vector().Rapidity(),2));
                      double PtDiff = std::fabs(l1electrons[j]->vector().Pt() - genjets[jetGenIndex]->vector().Pt());
                  
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
                  
                      double DeltaR = sqrt(pow(l1taus[j]->vector().Phi()-genjets[jetGenIndex]->vector().Phi(),2) + pow(l1taus[j]->vector().Rapidity()-genjets[jetGenIndex]->vector().Rapidity(),2));
                      double PtDiff = std::fabs(l1taus[j]->vector().Pt() - genjets[jetGenIndex]->vector().Pt());
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
                  
                  // Check if jet fired Jet trigger.

                  bool MatchedJetL1 = false;
                  double PtDiffjetJet = 10000;
                  int jetJetL1Index = -1;
                  double JetDeltaRRes;
                  double JetPtRes;
              
                  for(unsigned j=0; j < n_l1jets_; j++){
                  
                      double DeltaR = sqrt(pow(l1jets[j]->vector().Phi()- genjets[jetGenIndex]->vector().Phi(),2) + pow(l1jets[j]->vector().Rapidity()-genjets[jetGenIndex]->vector().Rapidity(),2));
                      double PtDiff = std::fabs(l1jets[j]->vector().Pt() - genjets[jetGenIndex]->vector().Pt());
                  
                      if(DeltaR < 0.5 && PtDiff < PtDiffjetJet){
                           PtDiffjetJet = PtDiff;
                           MatchedJetL1 = true;
                           jetJetL1Index = j;
                           JetDeltaRRes = DeltaR;
                           JetPtRes = l1jets[j]->vector().Pt() - genjets[jetGenIndex]->vector().Pt();
                      }
                      
                  }
                  
                  if(MatchedJetL1){ 
                      h_jet_Jet_PtRes->Fill(JetPtRes/genjets[jetGenIndex]->vector().Pt());
                      h_jet_Jet_PtDiff->Fill(JetPtRes);
                      h_jet_Jet_DeltaRRes->Fill(JetDeltaRRes);
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
                  
                      double DeltaR = sqrt(pow(l1muons[j]->vector().Phi()-genjets[jetGenIndex]->vector().Phi(),2) + pow(l1muons[j]->vector().Rapidity()-genjets[jetGenIndex]->vector().Rapidity(),2));
                      double PtDiff = std::fabs(l1muons[j]->vector().Pt() - genjets[jetGenIndex]->vector().Pt());
                  
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
          
      }
      
       // Di-Jets
      
      /*for(unsigned i=0; i < n_jets_; i++){
          
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
    
          for(unsigned j=0; j<n_jets_; i++){
              
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
                  
                  int jet1GenIndex = -1;
                  bool Matched1Offline = false;
                  double PtDiffjet1 = 10000;
              
                  for(unsigned k=0; k<n_genJets_; k++){
                  
                      double DeltaR = sqrt(pow(jets[i]->vector().Phi()-genjets[k]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-genjets[k]->vector().Rapidity(),2));
                      double PtDiff = std::fabs(jets[i]->vector().Pt() - genjets[k]->vector().Pt());
                  
                      if(DeltaR < 0.4 && PtDiff < PtDiffjet1){
                          PtDiffjet1 = PtDiff;
                          Matched1Offline = true;
                          jet1GenIndex = k;
                      }
                  
                  }
                  
                  int jet2GenIndex = -1;
                  bool Matched2Offline = false;
                  double PtDiffjet2 = 10000;
                  
                  for(unsigned k=0; k<n_genJets_; k++){
                  
                      double DeltaR = sqrt(pow(jets[j]->vector().Phi()-genjets[k]->vector().Phi(),2) + pow(jets[j]->vector().Rapidity()-genjets[k]->vector().Rapidity(),2));
                      double PtDiff = std::fabs(jets[j]->vector().Pt() - genjets[k]->vector().Pt());
                  
                      if(DeltaR < 0.4 && PtDiff < PtDiffjet2){
                          PtDiffjet2 = PtDiff;
                          Matched2Offline = true;
                          jet2GenIndex = k;
                      }
                  
                  }
                  
                  if(jet1GenIndex != jet1GenIndex && Matched1Offline && Matched2Offline){

                      h_jetjet_Mjj_Total->Fill((jets[i]->vector() + jets[j]->vector()).M());
                      h_jetjet_DeltaEta_Total->Fill(std::fabs(jets[i]->vector().Rapidity() - jets[j]->vector().Rapidity()));
                      
                      bool Matched1JetL1 = false;
                      double PtDiffjet1Jet = 10000;
                      int jet1JetL1Index = -1;
              
                      for(unsigned k=0; k < n_l1jets_; k++){
                  
                          double DeltaR = sqrt(pow(l1jets[k]->vector().Phi()- genjets[jet1GenIndex]->vector().Phi(),2) + pow(l1jets[k]->vector().Rapidity()-genjets[jet1GenIndex]->vector().Rapidity(),2));
                          double PtDiff = std::fabs(l1jets[k]->vector().Pt() - genjets[jet1GenIndex]->vector().Pt());
                  
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
                  
                          double DeltaR = sqrt(pow(l1jets[k]->vector().Phi()- genjets[jet2GenIndex]->vector().Phi(),2) + pow(l1jets[k]->vector().Rapidity()-genjets[jet2GenIndex]->vector().Rapidity(),2));
                          double PtDiff = std::fabs(l1jets[k]->vector().Pt() - genjets[jet2GenIndex]->vector().Pt());
                  
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
      }*/
      
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
          
          bool electronMatched1 = false;
          bool electronMatched2 = false;
          bool tauMatched1 = false;
          bool tauMatched2 = false;
          bool jetMatched1 = false;
          bool jetMatched2 = false;
          
          for(unsigned i =0; i<n_electrons_; i++){
              double DeltaR = sqrt(pow(electrons[i]->vector().Phi()- l1jets[0]->vector().Phi(),2) + pow(electrons[i]->vector().Rapidity()-l1jets[0]->vector().Rapidity(),2));
              if(DeltaR < 0.4) electronMatched1 = true;
              DeltaR = sqrt(pow(electrons[i]->vector().Phi()- l1jets[1]->vector().Phi(),2) + pow(electrons[i]->vector().Rapidity()-l1jets[1]->vector().Rapidity(),2));
              if(DeltaR < 0.4) electronMatched2 = true;
          }
          
          for(unsigned i =0; i<n_taus_; i++){
              double DeltaR = sqrt(pow(taus[i]->vector().Phi()- l1jets[0]->vector().Phi(),2) + pow(taus[i]->vector().Rapidity()-l1jets[0]->vector().Rapidity(),2));
              if(DeltaR < 0.4) tauMatched1 = true;
              DeltaR = sqrt(pow(taus[i]->vector().Phi()- l1jets[1]->vector().Phi(),2) + pow(taus[i]->vector().Rapidity()-l1jets[1]->vector().Rapidity(),2));
              if(DeltaR < 0.4) tauMatched2 = true;
          }
          
           for(unsigned i=0; i<n_jets_; i++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()- l1jets[0]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-l1jets[0]->vector().Rapidity(),2));
              if(DeltaR < 0.4) jetMatched1 = true;
              DeltaR = sqrt(pow(jets[i]->vector().Phi()- l1jets[1]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-l1jets[1]->vector().Rapidity(),2));
              if(DeltaR < 0.4) jetMatched2 = true;
          }
          
          if(electronMatched1){
              if(nElectrons == 1 && nMuons == 1)   h_jet1_em->Fill(2);
              if(nElectrons == 1 && nHadTaus == 1) h_jet1_et->Fill(2); 
              if(nHadTaus == 1 && nMuons == 1)     h_jet1_mt->Fill(2); 
              if(nHadTaus == 2)                    h_jet1_tt->Fill(2); 
          }
          if(tauMatched1){
              if(nElectrons == 1 && nMuons == 1)   h_jet1_em->Fill(1);
              if(nElectrons == 1 && nHadTaus == 1) h_jet1_et->Fill(1); 
              if(nHadTaus == 1 && nMuons == 1)     h_jet1_mt->Fill(1); 
              if(nHadTaus == 2)                    h_jet1_tt->Fill(1); 
          }
          else if(jetMatched1){
              if(nElectrons == 1 && nMuons == 1)   h_jet1_em->Fill(0);
              if(nElectrons == 1 && nHadTaus == 1) h_jet1_et->Fill(0); 
              if(nHadTaus == 1 && nMuons == 1)     h_jet1_mt->Fill(0); 
              if(nHadTaus == 2)                    h_jet1_tt->Fill(0); 
          }
          else{
              if(nElectrons == 1 && nMuons == 1)   h_jet1_em->Fill(4);
              if(nElectrons == 1 && nHadTaus == 1) h_jet1_et->Fill(4); 
              if(nHadTaus == 1 && nMuons == 1)     h_jet1_mt->Fill(4); 
              if(nHadTaus == 2)                    h_jet1_tt->Fill(4);
          }
          
          if(electronMatched2){
              if(nElectrons == 1 && nMuons == 1)   h_jet2_em->Fill(2);
              if(nElectrons == 1 && nHadTaus == 1) h_jet2_et->Fill(2); 
              if(nHadTaus == 1 && nMuons == 1)     h_jet2_mt->Fill(2); 
              if(nHadTaus == 2)                    h_jet2_tt->Fill(2); 
          }
          if(tauMatched2){
              if(nElectrons == 1 && nMuons == 1)   h_jet2_em->Fill(1);
              if(nElectrons == 1 && nHadTaus == 1) h_jet2_et->Fill(1); 
              if(nHadTaus == 1 && nMuons == 1)     h_jet2_mt->Fill(1); 
              if(nHadTaus == 2)                    h_jet2_tt->Fill(1); 
          }
          else if(jetMatched2){
              if(nElectrons == 1 && nMuons == 1)   h_jet2_em->Fill(0);
              if(nElectrons == 1 && nHadTaus == 1) h_jet2_et->Fill(0); 
              if(nHadTaus == 1 && nMuons == 1)     h_jet2_mt->Fill(0); 
              if(nHadTaus == 2)                    h_jet2_tt->Fill(0); 
          }
          else{
              if(nElectrons == 1 && nMuons == 1)   h_jet2_em->Fill(4);
              if(nElectrons == 1 && nHadTaus == 1) h_jet2_et->Fill(4); 
              if(nHadTaus == 1 && nMuons == 1)     h_jet2_mt->Fill(4); 
              if(nHadTaus == 2)                    h_jet2_tt->Fill(4); 
          }
          
          bool MjjelectronMatched1 = false;
          bool MjjelectronMatched2 = false;
          bool MjjtauMatched1 = false;
          bool MjjtauMatched2 = false;
          
          for(unsigned i=0; i<n_l1electrons_; i++){
              double DeltaR = sqrt(pow(l1electrons[i]->vector().Phi()- l1jets[Mjjjet1Index]->vector().Phi(),2) + pow(l1electrons[i]->vector().Rapidity()-l1jets[Mjjjet1Index]->vector().Rapidity(),2));
              if(DeltaR < 0.4) MjjelectronMatched1 = true;
              DeltaR = sqrt(pow(l1electrons[i]->vector().Phi()- l1jets[Mjjjet2Index]->vector().Phi(),2) + pow(l1electrons[i]->vector().Rapidity()-l1jets[Mjjjet2Index]->vector().Rapidity(),2));
              if(DeltaR < 0.4) MjjelectronMatched2 = true;
          }
          
          for(unsigned i=0; i<n_l1taus_; i++){
              double DeltaR = sqrt(pow(l1taus[i]->vector().Phi()- l1jets[Mjjjet1Index]->vector().Phi(),2) + pow(l1taus[i]->vector().Rapidity()-l1jets[Mjjjet1Index]->vector().Rapidity(),2));
              if(DeltaR < 0.4) MjjtauMatched1 = true;
              DeltaR = sqrt(pow(l1taus[i]->vector().Phi()- l1jets[Mjjjet2Index]->vector().Phi(),2) + pow(l1taus[i]->vector().Rapidity()-l1jets[Mjjjet2Index]->vector().Rapidity(),2));
              if(DeltaR < 0.4) MjjtauMatched2 = true;
          }
          
          if(MjjelectronMatched1 && MjjelectronMatched2){
              if(nElectrons == 1 && nMuons == 1 && nHadTaus == 0)   h_Mjj_em->Fill(4);
              if(nElectrons == 1 && nHadTaus == 1 && nMuons == 0) h_Mjj_et->Fill(4); 
              if(nHadTaus == 1 && nMuons == 1 && nElectrons == 0)     h_Mjj_mt->Fill(4); 
              if(nHadTaus == 2 && nMuons == 0 && nElectrons == 0)                    h_Mjj_tt->Fill(4); 
          }
          else if((MjjelectronMatched1 && !MjjelectronMatched2 && MjjtauMatched2) || (MjjelectronMatched2 && !MjjelectronMatched1 && MjjtauMatched1)){
              if(nElectrons == 1 && nMuons == 1 && nHadTaus == 0)   h_Mjj_em->Fill(5);
              if(nElectrons == 1 && nHadTaus == 1 && nMuons == 0) h_Mjj_et->Fill(5); 
              if(nHadTaus == 1 && nMuons == 1 && nElectrons == 0)     h_Mjj_mt->Fill(5); 
              if(nHadTaus == 2 && nMuons == 0 && nElectrons == 0)                    h_Mjj_tt->Fill(5);
          }
          else if((MjjelectronMatched1 && !MjjtauMatched2 && !MjjelectronMatched2) || (MjjelectronMatched2 && !MjjtauMatched1 && !MjjelectronMatched1)){
              if(nElectrons == 1 && nMuons == 1 && nHadTaus == 0)   h_Mjj_em->Fill(2);
              if(nElectrons == 1 && nHadTaus == 1 && nMuons == 0) h_Mjj_et->Fill(2); 
              if(nHadTaus == 1 && nMuons == 1 && nElectrons == 0)     h_Mjj_mt->Fill(2); 
              if(nHadTaus == 2 && nMuons == 0 && nElectrons == 0)                    h_Mjj_tt->Fill(2);
          }
          else if(!MjjelectronMatched1 && !MjjelectronMatched2 && MjjtauMatched1 && MjjtauMatched2){
              if(nElectrons == 1 && nMuons == 1 && nHadTaus == 0)   h_Mjj_em->Fill(3);
              if(nElectrons == 1 && nHadTaus == 1 && nMuons == 0) h_Mjj_et->Fill(3); 
              if(nHadTaus == 1 && nMuons == 1 && nElectrons == 0)     h_Mjj_mt->Fill(3); 
              if(nHadTaus == 2 && nMuons == 0 && nElectrons == 0)                    h_Mjj_tt->Fill(3);
          }
          else if(!MjjelectronMatched1 && !MjjelectronMatched2 && (MjjtauMatched1 || MjjtauMatched2)){
              if(nElectrons == 1 && nMuons == 1 && nHadTaus == 0)   h_Mjj_em->Fill(1);
              if(nElectrons == 1 && nHadTaus == 1 && nMuons == 0) h_Mjj_et->Fill(1); 
              if(nHadTaus == 1 && nMuons == 1 && nElectrons == 0)     h_Mjj_mt->Fill(1); 
              if(nHadTaus == 2 && nMuons == 0 && nElectrons == 0)                    h_Mjj_tt->Fill(1);
          }
          else{
              if(nElectrons == 1 && nMuons == 1 && nHadTaus == 0)   h_Mjj_em->Fill(0);
              if(nElectrons == 1 && nHadTaus == 1 && nMuons == 0) h_Mjj_et->Fill(0); 
              if(nHadTaus == 1 && nMuons == 1 && nElectrons == 0)     h_Mjj_mt->Fill(0); 
              if(nHadTaus == 2 && nMuons == 0 && nElectrons == 0)                    h_Mjj_tt->Fill(0);
          }
          
          bool DeltaEtaelectronMatched1 = false;
          bool DeltaEtaelectronMatched2 = false;
          bool DeltaEtatauMatched1 = false;
          bool DeltaEtatauMatched2 = false;
          bool DeltaEtajetMatched1 = false;
          bool DeltaEtajetMatched2 = false;
          
          for(unsigned i=0; i<n_electrons_; i++){
              double DeltaR = sqrt(pow(electrons[i]->vector().Phi()- l1jets[DeltaEtajet1Index]->vector().Phi(),2) + pow(electrons[i]->vector().Rapidity()-l1jets[DeltaEtajet1Index]->vector().Rapidity(),2));
              if(DeltaR < 0.4){
                  DeltaEtaelectronMatched1 = true;
              }
              DeltaR = sqrt(pow(electrons[i]->vector().Phi()- l1jets[DeltaEtajet2Index]->vector().Phi(),2) + pow(electrons[i]->vector().Rapidity()-l1jets[DeltaEtajet2Index]->vector().Rapidity(),2));
              if(DeltaR < 0.4) DeltaEtaelectronMatched2 = true;
          }
          
          for(unsigned i=0; i<n_taus_; i++){
              double DeltaR = sqrt(pow(taus[i]->vector().Phi()- l1jets[DeltaEtajet1Index]->vector().Phi(),2) + pow(taus[i]->vector().Rapidity()-l1jets[DeltaEtajet1Index]->vector().Rapidity(),2));
              if(DeltaR < 0.4) DeltaEtatauMatched1 = true;
              DeltaR = sqrt(pow(taus[i]->vector().Phi()- l1jets[DeltaEtajet2Index]->vector().Phi(),2) + pow(taus[i]->vector().Rapidity()-l1jets[DeltaEtajet2Index]->vector().Rapidity(),2));
              if(DeltaR < 0.4) DeltaEtatauMatched2 = true;
          }
          
          for(unsigned i=0; i<n_jets_; i++){
              double DeltaR = sqrt(pow(jets[i]->vector().Phi()- l1jets[DeltaEtajet1Index]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-l1jets[DeltaEtajet1Index]->vector().Rapidity(),2));
              if(DeltaR < 0.4) DeltaEtajetMatched1 = true;
              DeltaR = sqrt(pow(jets[i]->vector().Phi()- l1jets[DeltaEtajet2Index]->vector().Phi(),2) + pow(jets[i]->vector().Rapidity()-l1jets[DeltaEtajet2Index]->vector().Rapidity(),2));
              if(DeltaR < 0.4) DeltaEtajetMatched2 = true;
          }
          
          if(DeltaEtaelectronMatched1 && DeltaEtaelectronMatched2){
              if(nElectrons == 1 && nMuons == 1)   h_DeltaEta_em->Fill(4);
              if(nElectrons == 1 && nHadTaus == 1) h_DeltaEta_et->Fill(4); 
              if(nHadTaus == 1 && nMuons == 1)     h_DeltaEta_mt->Fill(4); 
              if(nHadTaus == 2)                    h_DeltaEta_tt->Fill(4); 
          }
          else if((DeltaEtaelectronMatched1 && !DeltaEtaelectronMatched2 && DeltaEtatauMatched2) || (DeltaEtaelectronMatched2 && !DeltaEtaelectronMatched1 && DeltaEtatauMatched1)){
              if(nElectrons == 1 && nMuons == 1)   h_DeltaEta_em->Fill(5);
              if(nElectrons == 1 && nHadTaus == 1) h_DeltaEta_et->Fill(5); 
              if(nHadTaus == 1 && nMuons == 1)     h_DeltaEta_mt->Fill(5); 
              if(nHadTaus == 2)                    h_DeltaEta_tt->Fill(5);
          }
          else if((DeltaEtaelectronMatched1 && !DeltaEtatauMatched2 && !DeltaEtaelectronMatched2 && DeltaEtajetMatched2) || (DeltaEtaelectronMatched2 && !DeltaEtatauMatched1 && !DeltaEtaelectronMatched1 && DeltaEtajetMatched1)){
              if(nElectrons == 1 && nMuons == 1)   h_DeltaEta_em->Fill(2);
              if(nElectrons == 1 && nHadTaus == 1) h_DeltaEta_et->Fill(2); 
              if(nHadTaus == 1 && nMuons == 1)     h_DeltaEta_mt->Fill(2); 
              if(nHadTaus == 2)                    h_DeltaEta_tt->Fill(2);
          }
          else if(!DeltaEtaelectronMatched1 && !DeltaEtaelectronMatched2 && DeltaEtatauMatched1 && DeltaEtatauMatched2){
              if(nElectrons == 1 && nMuons == 1)   h_DeltaEta_em->Fill(3);
              if(nElectrons == 1 && nHadTaus == 1) h_DeltaEta_et->Fill(3); 
              if(nHadTaus == 1 && nMuons == 1)     h_DeltaEta_mt->Fill(3); 
              if(nHadTaus == 2)                    h_DeltaEta_tt->Fill(3);
          }
          else if(!DeltaEtaelectronMatched1 && !DeltaEtaelectronMatched2 && ((DeltaEtatauMatched1 && DeltaEtajetMatched2)|| (DeltaEtatauMatched2 && DeltaEtajetMatched1))){
              if(nElectrons == 1 && nMuons == 1)   h_DeltaEta_em->Fill(1);
              if(nElectrons == 1 && nHadTaus == 1) h_DeltaEta_et->Fill(1); 
              if(nHadTaus == 1 && nMuons == 1)     h_DeltaEta_mt->Fill(1); 
              if(nHadTaus == 2)                    h_DeltaEta_tt->Fill(1);
          }
          else if (DeltaEtajetMatched1 && DeltaEtajetMatched2){
              if(nElectrons == 1 && nMuons == 1)   h_DeltaEta_em->Fill(0);
              if(nElectrons == 1 && nHadTaus == 1) h_DeltaEta_et->Fill(0); 
              if(nHadTaus == 1 && nMuons == 1)     h_DeltaEta_mt->Fill(0); 
              if(nHadTaus == 2)                    h_DeltaEta_tt->Fill(0);
          }
      }
              
              
 
     if(nHadTaus == 2){
      
          double LargestMjj = 0;
          bool TwoL1Jets = false;
          int jet1Index;
          int jet2Index;
          for(unsigned i=0; i<n_l1jets_; i++){
          
              for(unsigned j=0; j <n_l1jets_; j++){
           
                  if(i!=j && l1jets[i]->vector().Pt() >=30 && l1jets[j]->vector().Pt() >=30){
                      double Mjj = (l1jets[i]->vector() + l1jets[j]->vector()).M();
                      TwoL1Jets = true;
                      if(Mjj > LargestMjj) LargestMjj = Mjj;
                      jet1Index = i;
                      jet2Index = j;
                  }
              }
          }
          bool l1jet1IsTau = false;
          bool l1jet2IsTau = false;
          bool l1jet1IsJet = false;
          bool l1jet2IsJet = false;
          //bool l1jet1IsOther = false;
          //bool l1jet2IsOther = false;
          
          if(TwoL1Jets){
              double PtDifftau = 100000;
              for(unsigned j=0; j< n_genParticles_; j++){

              int genID = std::fabs(GenParticles[j]->pdgid());
              bool isPrompt = false;
                  
              for(unsigned k=0; k < GenParticles[j]->mothers().size(); k++) {
                  if(std::fabs(GenParticles[GenParticles[j]->mothers().at(k)]->pdgid()) == 25){
                      if(genID == 15){
                          //hadTauCount++;
                          isPrompt = true;
                      }
                  }
              }
                  
              
              for(unsigned k=0; k < GenParticles[j]->daughters().size(); k++) {
                  if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 11 || std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 13){
                      isPrompt = false;
                      //hadTauCount--;
                  }
                  if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 16) GenParticles[j]->set_vector(GenParticles[j]->vector() - GenParticles[GenParticles[j]->daughters().at(k)]->vector());
              }
                 
              double DeltaR = sqrt(pow(l1jets[jet1Index]->vector().Phi()-GenParticles[j]->vector().Phi(),2) + pow(l1jets[jet1Index]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
              double PtDiff = std::fabs(l1jets[jet1Index]->vector().Pt() - GenParticles[j]->vector().Pt());
                  
              if(DeltaR < 0.2 && PtDiff < PtDifftau && isPrompt){
                  PtDifftau = PtDiff;
                  l1jet1IsTau = true;
                  //tauGenIndex = j;
                  l1jet1IsTau = true;
                  //else l1jet1IsTau = false;
                  //if(!isPrompt) l1jet1IsOther = true;
                  //else l1jet1IsOther = false;
              }
              }
              
              PtDifftau = 100000;
              
              for(unsigned j=0; j< n_genParticles_; j++){

              int genID = std::fabs(GenParticles[j]->pdgid());
              bool isPrompt = false;
                  
              for(unsigned k=0; k < GenParticles[j]->mothers().size(); k++) {
                  if(std::fabs(GenParticles[GenParticles[j]->mothers().at(k)]->pdgid()) == 25){
                      if(genID == 15){
                          //hadTauCount++;
                          isPrompt = true;
                      }
                  }
              }
                  
              
              for(unsigned k=0; k < GenParticles[j]->daughters().size(); k++) {
                  if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 11 || std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 13){
                      isPrompt = false;
                      //hadTauCount--;
                  }
                  if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 16) GenParticles[j]->set_vector(GenParticles[j]->vector() - GenParticles[GenParticles[j]->daughters().at(k)]->vector());
              }
                 
              double DeltaR = sqrt(pow(l1jets[jet2Index]->vector().Phi()-GenParticles[j]->vector().Phi(),2) + pow(l1jets[jet2Index]->vector().Rapidity()-GenParticles[j]->vector().Rapidity(),2));
              double PtDiff = std::fabs(l1jets[jet2Index]->vector().Pt() - GenParticles[j]->vector().Pt());
                  
              if(DeltaR < 0.2 && PtDiff < PtDifftau && isPrompt){
                  PtDifftau = PtDiff;
                  //tauGenIndex = j;
                  l1jet2IsTau = true;
                  //else l1jet2IsTau = false;
                  //if(!isPrompt) l1jet2IsOther = true;
                  //else l1jet2IsOther = false;
              }
              
          }
          
          if(!l1jet1IsTau){
              
              for(unsigned j=0; j<n_genJets_; j++){
                  
                  double DeltaR = sqrt(pow(l1jets[jet1Index]->vector().Phi()-genjets[j]->vector().Phi(),2) + pow(l1jets[jet1Index]->vector().Rapidity()-genjets[j]->vector().Rapidity(),2));
                  
                  if(DeltaR < 0.4){
                      l1jet1IsJet = true;
                  }
                  
              }
                  
                  
          }
          
          if(!l1jet2IsTau){
              
              for(unsigned j=0; j<n_genJets_; j++){
                  
                  double DeltaR = sqrt(pow(l1jets[jet2Index]->vector().Phi()-genjets[j]->vector().Phi(),2) + pow(l1jets[jet2Index]->vector().Rapidity()-genjets[j]->vector().Rapidity(),2));
                  
                  if(DeltaR < 0.4){
                      l1jet2IsJet = true;
                  }
                  
              }
                  
                  
          }
          
          if(l1jet1IsJet && l1jet2IsJet) h_tauJetComp->Fill(0);
          else if((l1jet1IsTau && l1jet2IsJet) || (l1jet2IsTau && l1jet1IsJet)) h_tauJetComp->Fill(1); 
          else if(l1jet1IsTau && l1jet2IsTau) h_tauJetComp->Fill(2);
          else h_tauJetComp->Fill(3);
          
          
      
      }
      
          if(n_l1jets_ >=2 && l1jets[0]->vector().Pt() >= 30 && l1jets[1]->vector().Pt() >= 30){
              
              bool jet1TauMatched = false;
              bool jet2TauMatched = false;
              
              for(unsigned i=0; i<n_l1taus_; i++){
                  double DeltaR1 = sqrt(pow(l1jets[0]->vector().Phi()-l1taus[i]->vector().Phi(),2) + pow(l1jets[0]->vector().Rapidity()-l1taus[i]->vector().Rapidity(),2));
                  if(DeltaR1 < 0.4 && l1taus[i]->vector().Pt() >=30) jet1TauMatched = true;
                  double DeltaR2 = sqrt(pow(l1jets[1]->vector().Phi()-l1taus[i]->vector().Phi(),2) + pow(l1jets[1]->vector().Rapidity()-l1taus[i]->vector().Rapidity(),2));
                  if(DeltaR2 < 0.4 && l1taus[i]->vector().Pt() >=30) jet2TauMatched = true;
              }
              
              if(!jet1TauMatched && !jet2TauMatched) h_l1tauJetComp->Fill(0);
              if((jet1TauMatched && !jet2TauMatched) || (jet2TauMatched && !jet1TauMatched)) h_l1tauJetComp->Fill(1);
              if(jet1TauMatched && jet2TauMatched) h_l1tauJetComp->Fill(2);
              
          }
          
          
     }
     
     for(unsigned j=0; j< n_genParticles_; j++){

         int genID = std::fabs(GenParticles[j]->pdgid());
         bool isPrompt = false;
         bool fromHiggs = false;
         
         for(unsigned k=0; k < GenParticles[j]->mothers().size(); k++) {
              if(std::fabs(GenParticles[GenParticles[j]->mothers().at(k)]->pdgid()) == 15){
                  int ParentgenIndex = GenParticles[j]->mothers().at(k);
                  int GPgenID = std::fabs(GenParticles[GenParticles[ParentgenIndex]->mothers().at(0)]->pdgid());
                  if(GPgenID == 25) fromHiggs = true;
              }
         }
                  
         bool MatchedOffline = false;
         for(unsigned k=0; k< n_electrons_; k++){
             double DeltaR = sqrt(pow(GenParticles[j]->vector().Phi()-electrons[k]->vector().Phi(),2) + pow(GenParticles[j]->vector().Rapidity()-electrons[k]->vector().Rapidity(),2)); 
             if(DeltaR < 0.2) MatchedOffline = true;
         }
              
         if(fromHiggs && genID == 11 && MatchedOffline) isPrompt = true;
              
         if(isPrompt && std::fabs(GenParticles[j]->vector().Rapidity()) < 2.1){
             bool l1TauMatched = false;
             bool l1IsoTauMatched = false;
             bool l1JetMatched = false;
             bool l1EGMatched = false;
             bool l1IsoEGMatched = false;
             h_genelectron_total->Fill(GenParticles[j]->vector().Pt());
             for(unsigned k=0; k <n_l1taus_; k++){
                 double DeltaR = sqrt(pow(GenParticles[j]->vector().Phi()-l1taus[k]->vector().Phi(),2) + pow(GenParticles[j]->vector().Rapidity()-l1taus[k]->vector().Rapidity(),2));
                 if(DeltaR < 0.3) l1TauMatched = true;
             }
             for(unsigned k=0; k <n_l1taus_; k++){
                 double DeltaR = sqrt(pow(GenParticles[j]->vector().Phi()-l1taus[k]->vector().Phi(),2) + pow(GenParticles[j]->vector().Rapidity()-l1taus[k]->vector().Rapidity(),2));
                 if(DeltaR < 0.3 && l1taus[k]->isolation == 1) l1IsoTauMatched = true;
             }
             for(unsigned k=0; k <n_l1jets_; k++){
                 double DeltaR = sqrt(pow(GenParticles[j]->vector().Phi()-l1jets[k]->vector().Phi(),2) + pow(GenParticles[j]->vector().Rapidity()-l1jets[k]->vector().Rapidity(),2));
                 if(DeltaR < 0.4) l1JetMatched = true;
             }
             for(unsigned k=0; k <n_l1electrons_; k++){
                 double DeltaR = sqrt(pow(GenParticles[j]->vector().Phi()-l1electrons[k]->vector().Phi(),2) + pow(GenParticles[j]->vector().Rapidity()-l1electrons[k]->vector().Rapidity(),2));
                 if(DeltaR < 0.2) l1EGMatched = true;
             }
             for(unsigned k=0; k <n_l1electrons_; k++){
                 double DeltaR = sqrt(pow(GenParticles[j]->vector().Phi()-l1electrons[k]->vector().Phi(),2) + pow(GenParticles[j]->vector().Rapidity()-l1electrons[k]->vector().Rapidity(),2));
                 if(DeltaR < 0.2 && l1electrons[k]->isolation == 1) l1IsoEGMatched = true;
             }
             if(l1TauMatched) h_genelectron_Tau->Fill(GenParticles[j]->vector().Pt());
             if(l1IsoTauMatched) h_genelectron_IsoTau->Fill(GenParticles[j]->vector().Pt());
             if(l1JetMatched) h_genelectron_Jet->Fill(GenParticles[j]->vector().Pt());
             if(l1EGMatched) h_genelectron_EG->Fill(GenParticles[j]->vector().Pt());
             if(l1IsoEGMatched) h_genelectron_IsoEG->Fill(GenParticles[j]->vector().Pt());
                  
             if(l1IsoEGMatched && l1IsoTauMatched) h_genelectron_IsoTauIsoEG->Fill(GenParticles[j]->vector().Pt());
             if(l1EGMatched && l1TauMatched) h_genelectron_TauEG->Fill(GenParticles[j]->vector().Pt());
             if(l1JetMatched && l1IsoTauMatched) h_genelectron_IsoTauJet->Fill(GenParticles[j]->vector().Pt());
             if(l1JetMatched && l1TauMatched) h_genelectron_TauJet->Fill(GenParticles[j]->vector().Pt());
             if(l1IsoEGMatched && l1IsoTauMatched && l1JetMatched) h_genelectron_IsoTauIsoEGJet->Fill(GenParticles[j]->vector().Pt());
             if(l1EGMatched && l1TauMatched && l1JetMatched) h_genelectron_TauEGJet->Fill(GenParticles[j]->vector().Pt());
         }
              
     }
     
          for(unsigned j=0; j< n_genParticles_; j++){

         int genID = std::fabs(GenParticles[j]->pdgid());
         bool isPrompt = false;
         bool fromHiggs = false;
         bool isLepton = false;
                  
         for(unsigned k=0; k < GenParticles[j]->mothers().size(); k++) {
             if(std::fabs(GenParticles[GenParticles[j]->mothers().at(k)]->pdgid()) == 25) fromHiggs = true;
         }
                  
              
         for(unsigned k=0; k < GenParticles[j]->daughters().size(); k++) {
             if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 11 || std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 13) isLepton = true;
    
             if(std::fabs(GenParticles[GenParticles[j]->daughters().at(k)]->pdgid()) == 16) GenParticles[j]->set_vector(GenParticles[j]->vector() - GenParticles[GenParticles[j]->daughters().at(k)]->vector());

         }
         bool MatchedOffline = false;
         for(unsigned k=0; k< n_taus_; k++){
             double DeltaR = sqrt(pow(GenParticles[j]->vector().Phi()-taus[k]->vector().Phi(),2) + pow(GenParticles[j]->vector().Rapidity()-taus[k]->vector().Rapidity(),2)); 
             if(DeltaR < 0.3) MatchedOffline = true;
         }
         
         for(unsigned k=0; k< n_electrons_; k++){
             double DeltaR = sqrt(pow(GenParticles[j]->vector().Phi()-electrons[k]->vector().Phi(),2) + pow(GenParticles[j]->vector().Rapidity()-electrons[k]->vector().Rapidity(),2)); 
             if(DeltaR < 0.2) MatchedOffline = false;
         }
         for(unsigned k=0; k< n_muons_; k++){
             double DeltaR = sqrt(pow(GenParticles[j]->vector().Phi()-muons[k]->vector().Phi(),2) + pow(GenParticles[j]->vector().Rapidity()-muons[k]->vector().Rapidity(),2)); 
             if(DeltaR < 0.2) MatchedOffline = false;
         }
              
         if(fromHiggs && !isLepton && genID == 15 && MatchedOffline) isPrompt = true;
              
         if(isPrompt && std::fabs(GenParticles[j]->vector().Rapidity()) < 2.3){
             bool l1TauMatched = false;
             bool l1IsoTauMatched = false;
             bool l1JetMatched = false;
             bool l1EGMatched = false;
             bool l1IsoEGMatched = false;
             h_gentau_total->Fill(GenParticles[j]->vector().Pt());
             for(unsigned k=0; k <n_l1taus_; k++){
                 double DeltaR = sqrt(pow(GenParticles[j]->vector().Phi()-l1taus[k]->vector().Phi(),2) + pow(GenParticles[j]->vector().Rapidity()-l1taus[k]->vector().Rapidity(),2));
                 if(DeltaR < 0.3) l1TauMatched = true;
             }
             for(unsigned k=0; k <n_l1taus_; k++){
                 double DeltaR = sqrt(pow(GenParticles[j]->vector().Phi()-l1taus[k]->vector().Phi(),2) + pow(GenParticles[j]->vector().Rapidity()-l1taus[k]->vector().Rapidity(),2));
                 if(DeltaR < 0.3 && l1taus[k]->isolation == 1) l1IsoTauMatched = true;
             }
             for(unsigned k=0; k <n_l1jets_; k++){
                 double DeltaR = sqrt(pow(GenParticles[j]->vector().Phi()-l1jets[k]->vector().Phi(),2) + pow(GenParticles[j]->vector().Rapidity()-l1jets[k]->vector().Rapidity(),2));
                 if(DeltaR < 0.4) l1JetMatched = true;
             }
             for(unsigned k=0; k <n_l1electrons_; k++){
                 double DeltaR = sqrt(pow(GenParticles[j]->vector().Phi()-l1electrons[k]->vector().Phi(),2) + pow(GenParticles[j]->vector().Rapidity()-l1electrons[k]->vector().Rapidity(),2));
                 if(DeltaR < 0.2) l1EGMatched = true;
             }
             for(unsigned k=0; k <n_l1electrons_; k++){
                 double DeltaR = sqrt(pow(GenParticles[j]->vector().Phi()-l1electrons[k]->vector().Phi(),2) + pow(GenParticles[j]->vector().Rapidity()-l1electrons[k]->vector().Rapidity(),2));
                 if(DeltaR < 0.2 && l1electrons[k]->isolation == 1) l1IsoEGMatched = true;
             }
             if(l1TauMatched) h_gentau_Tau->Fill(GenParticles[j]->vector().Pt());
             if(l1IsoTauMatched) h_gentau_IsoTau->Fill(GenParticles[j]->vector().Pt());
             if(l1JetMatched) h_gentau_Jet->Fill(GenParticles[j]->vector().Pt());
             if(l1EGMatched) h_gentau_EG->Fill(GenParticles[j]->vector().Pt());
             if(l1IsoEGMatched) h_gentau_IsoEG->Fill(GenParticles[j]->vector().Pt());
                  
             if(l1IsoEGMatched && l1IsoTauMatched) h_gentau_IsoTauIsoEG->Fill(GenParticles[j]->vector().Pt());
             if(l1EGMatched && l1TauMatched) h_gentau_TauEG->Fill(GenParticles[j]->vector().Pt());
             if(l1JetMatched && l1IsoTauMatched) h_gentau_IsoTauJet->Fill(GenParticles[j]->vector().Pt());
             if(l1JetMatched && l1TauMatched) h_gentau_TauJet->Fill(GenParticles[j]->vector().Pt());
             if(l1IsoEGMatched && l1IsoTauMatched && l1JetMatched) h_gentau_IsoTauIsoEGJet->Fill(GenParticles[j]->vector().Pt());
             if(l1EGMatched && l1TauMatched && l1JetMatched) h_gentau_TauEGJet->Fill(GenParticles[j]->vector().Pt());
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


