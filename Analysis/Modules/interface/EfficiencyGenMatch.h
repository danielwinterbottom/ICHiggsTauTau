#ifndef ICHiggsTauTau_Module_EfficiencyGenMatch_h
#define ICHiggsTauTau_Module_EfficiencyGenMatch_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include <string>
#include <cmath>

#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TDirectory.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"

// ICHiggsTauTau Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TSum.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

#include "Modules/interface/L1Cuts.h"

namespace ic {

  class EfficiencyGenMatch : public ModuleBase {
  private:

    std::string genParticles_label_; 
    
    std::string jets_label_;
    std::string electrons_label_;
    std::string muons_label_;
    std::string taus_label_;
    std::string ditau_label_;
    std::string met_label_;
    
    std::string l1jets_label_;
    std::string l1electrons_label_;
    std::string l1muons_label_;
    std::string l1taus_label_;
    std::string l1met_label_;
    
    std::string genjets_label_;
    
    unsigned n_genParticles_;
    unsigned n_genjets_;
    unsigned n_jets_;
    unsigned n_electrons_;
    unsigned n_muons_;
    unsigned n_taus_;
    
    unsigned n_l1jets_;
    unsigned n_l1electrons_;
    unsigned n_l1muons_;
    unsigned n_l1taus_;
    
    double l1eCut;
    double l1muCut;
    double l1tauCut;
    double l1jetCut;
    double l1MjjCut;
    double l1DeltaEtaCut;
    double l1METCut;
    double l1MHTCut;
    int EffNum;
    double AvePtCut;
    
    unsigned nPromptElectrons;
    unsigned nPromptMuons;
    unsigned nPromptTaus;
    
    std::string channel_;
    
    TH1D *h_mu_Mu_Efficiency;
    TH1D *h_mu_IsoMu_Efficiency;
    TH1D *h_mu_Total;
    TH1D *h_mu_Mu_PtDiff;
    TH1D *h_mu_Mu_PtRes;
    TH1D *h_mu_Mu_DeltaRRes;
    
    TH1D *h_e_EG_Efficiency;
    TH1D *h_e_IsoEG_Efficiency;
    TH1D *h_e_Total;
    TH1D *h_e_Tau_Efficiency;
    TH1D *h_e_IsoTau_Efficiency;
    TH1D *h_e_Jet_Efficiency;
    TH1D *h_e_EG_PtRes;
    TH1D *h_e_EG_DeltaRRes;
    TH1D *h_e_EG_PtDiff;

    TH1D *h_tau_EG_Efficiency;
    TH1D *h_tau_IsoEG_Efficiency;
    TH1D *h_tau_Total;
    TH1D *h_subtau_Total;
    TH1D *h_tau_Tau_Efficiency;
    TH1D *h_subtau_Tau_Efficiency;
    TH1D *h_tau_Mu_Efficiency;
    TH1D *h_tau_IsoTau_Efficiency;
    TH1D *h_subtau_IsoTau_Efficiency;
    TH1D *h_tau_Jet_Efficiency;
    TH1D *h_tau_Tau_PtRes;
    TH1D *h_tau_Tau_PtDiff;
    TH1D *h_tau_Tau_DeltaRRes;
    
    TH1D *h_gentau_Tau_Efficiency;
    TH1D *h_gentau_IsoTau_Efficiency;
    TH1D *h_gentau_Total;
    
    TH1D *h_gene_EG_Efficiency;
    TH1D *h_gene_IsoEG_Efficiency;
    TH1D *h_gene_Total;
    
    TH1D *h_genmu_Mu_Efficiency;
    TH1D *h_genmu_IsoMu_Efficiency;
    TH1D *h_genmu_Total;
    
    TH1D *h_genjet_Jet_Efficiency;
    TH1D *h_genjet_Total;
    
    TH1D *h_jet_EG_Efficiency;
    TH1D *h_jet_IsoEG_Efficiency;
    TH1D *h_jet_Total;
    TH1D *h_subjet_Total;
    TH1D *h_jet_Tau_Efficiency;
    TH1D *h_jet_Mu_Efficiency;
    TH1D *h_jet_IsoTau_Efficiency;
    TH1D *h_jet_Jet_Efficiency;
    TH1D *h_subjet_Jet_Efficiency;
    TH1D *h_jet_Jet_PtRes;
    TH1D *h_jet_Jet_DeltaRRes;
    TH1D *h_jet_Jet_PtDiff;
    
    TH1D *h_jet_TauEG_Efficiency;
    TH1D *h_jet_IsoTauIsoEG_Efficiency;
    
    TH1D *h_jetjet_Mjj_Efficiency;
    TH1D *h_jetjet_Mjj_Total;
    TH1D *h_jetjet_DeltaEta_Efficiency;
    TH1D *h_jetjet_DeltaEta_Total;
    
    TH1D *h_jetjet_AvePt_Efficiency;
    TH1D *h_jetjet_AvePt_Total;
    
    TH1D *h_jettau_Mjj_Efficiency;
    TH1D *h_jettau_Mjj_Total;
    TH1D *h_jettau_DeltaEta_Efficiency;
    TH1D *h_jettau_DeltaEta_Total;
    
    TH1D *h_jetelectron_Mjj_Efficiency;
    TH1D *h_jetelectron_Mjj_Total;
    TH1D *h_jetelectron_DeltaEta_Efficiency;
    TH1D *h_jetelectron_DeltaEta_Total;
    
    TH1D  *h_METEfficiency;
    TH1D  *h_MHTEfficiency;
    
    TH1D  *h_GenMETEfficiency;
    TH1D  *h_GenMHTEfficiency;

  public:

    EfficiencyGenMatch(std::string const& name, fwlite::TFileService *fs, std::string output_name, int effNum, std::string channel, struct L1Cuts l1Cuts);
    virtual ~EfficiencyGenMatch();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
    TFile *fEff;
    
  };

}//namespace


#endif
