#ifndef ICHiggsTauTau_Module_HLTID_h
#define ICHiggsTauTau_Module_HLTID_h

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

namespace ic {

  class Efficiency : public ModuleBase {
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
 
    std::string output_name_;
    std::string output_name2_ = "Efficiency";
    std::string output_folder_;
    
    
    unsigned n_genParticles_;
    unsigned n_genJets_;
    unsigned n_jets_;
    unsigned n_electrons_;
    unsigned n_muons_;
    unsigned n_taus_;
    
    unsigned n_l1jets_;
    unsigned n_l1electrons_;
    unsigned n_l1muons_;
    unsigned n_l1taus_;
    
    TH1D *h_mu_Mu_Efficiency;
    TH1D *h_mu_Total;
    TH1D *h_mu_Mu_PtDiff;
    TH1D *h_mu_Mu_PtRes;
    TH1D *h_mu_Mu_DeltaRRes;
    
    TH1D *h_e_EG_Efficiency;
    TH1D *h_e_IsoEG_Efficiency;
    TH1D *h_e_Total;
    TH1D *h_e_Tau_Efficiency;
    TH1D *h_e_IsoTau_Efficiency;
    TH1D *h_e_NonIsoTau_Efficiency;
    TH1D *h_e_Jet_Efficiency;
    TH1D *h_e_EG_PtRes;
    TH1D *h_e_EG_DeltaRRes;
    TH1D *h_e_EG_PtDiff;

    TH1D *h_tau_EG_Efficiency;
    TH1D *h_tau_IsoEG_Efficiency;
    TH1D *h_tau_Total;
    TH1D *h_tau_Tau_Efficiency;
    TH1D *h_tau_Mu_Efficiency;
    TH1D *h_tau_NonIsoTau_Efficiency;
    TH1D *h_tau_IsoTau_Efficiency;
    TH1D *h_tau_Jet_Efficiency;
    TH1D *h_tau_Tau_PtRes;
    TH1D *h_tau_IsoTau_PtRes;
    TH1D *h_tau_Tau_PtDiff;
    TH1D *h_tau_IsoTau_PtDiff;
    TH1D *h_tau_Tau_DeltaRRes;
    TH1D *h_tau_IsoTau_DeltaRRes;
    TH1D *h_tautau_DeltaR;
    
    TH1D *h_jet_EG_Efficiency;
    TH1D *h_jet_IsoEG_Efficiency;
    TH1D *h_jet_Total;
    TH1D *h_jet_Tau_Efficiency;
    TH1D *h_jet_Mu_Efficiency;
    TH1D *h_jet_NonIsoTau_Efficiency;
    TH1D *h_jet_IsoTau_Efficiency;
    TH1D *h_jet_Jet_Efficiency;
    TH1D *h_jet_Jet_PtRes;
    TH1D *h_jet_Jet_DeltaRRes;
    TH1D *h_jet_Jet_PtDiff;
    
    TH1D *h_jetjet_Mjj_Efficiency;
    TH1D *h_jetjet_Mjj_Total;
    TH1D *h_jetjet_DeltaEta_Efficiency;
    TH1D *h_jetjet_DeltaEta_Total;
    
    TH1D *h_tauJetComp;
    TH1D *h_l1tauJetComp;
    
    TH1D *h_gentau_total;
    TH1D *h_gentau_Tau;
    TH1D *h_gentau_IsoTau;
    TH1D *h_gentau_Jet;
    TH1D *h_gentau_EG;
    TH1D *h_gentau_IsoEG;
    TH1D *h_gentau_IsoTauIsoEG;
    TH1D *h_gentau_TauEG;
    TH1D *h_gentau_IsoTauJet;
    TH1D *h_gentau_TauJet;
    TH1D *h_gentau_IsoTauIsoEGJet;
    TH1D *h_gentau_TauEGJet;
    
    TH1D *h_genelectron_total;
    TH1D *h_genelectron_Tau;
    TH1D *h_genelectron_IsoTau;
    TH1D *h_genelectron_Jet;
    TH1D *h_genelectron_EG;
    TH1D *h_genelectron_IsoEG;
    TH1D *h_genelectron_IsoTauIsoEG;
    TH1D *h_genelectron_TauEG;
    TH1D *h_genelectron_IsoTauJet;
    TH1D *h_genelectron_TauJet;
    TH1D *h_genelectron_IsoTauIsoEGJet;
    TH1D *h_genelectron_TauEGJet;
    
    TH1D *h_Mjj_em;
    TH1D *h_Mjj_et;
    TH1D *h_Mjj_mt;
    TH1D *h_Mjj_tt;
    
    TH1D *h_DeltaEta_em;
    TH1D *h_DeltaEta_et;
    TH1D *h_DeltaEta_mt;
    TH1D *h_DeltaEta_tt;
    
    TH1D *h_jet1_em;
    TH1D *h_jet1_et;
    TH1D *h_jet1_mt;
    TH1D *h_jet1_tt;
    
    TH1D *h_jet2_em;
    TH1D *h_jet2_et;
    TH1D *h_jet2_mt;
    TH1D *h_jet2_tt;


  public:

    Efficiency(std::string const& name, std::string output_name, std::string output_folder);
    virtual ~Efficiency();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
    TFile *fEff;
    
  };

}//namespace


#endif
