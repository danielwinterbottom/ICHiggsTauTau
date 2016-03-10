#ifndef ICHiggsTauTau_Module_VariableHistograms_h
#define ICHiggsTauTau_Module_VariableHistograms_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>
#include <cmath>

#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TLorentzVector.h"

// ICHiggsTauTau Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TSum.hh"

namespace ic {

  class VariableHistograms : public ModuleBase {
  private:

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
    
    std::string output_name_;
    std::string output_name2_ = "L1_";

    int n_jets_;
    int n_electrons_;
    int n_muons_;
    int n_taus_;
    
    int n_l1jets_;
    int n_l1electrons_;
    int n_l1muons_;
    int n_l1taus_;

    TFile *fOut;
    
    TH1D  *h_EtaGap;
    TH1D  *h_MjjInv;
    TH1D  *h_DeltaRjj;
    TH1D  *h_DeltaPhijj;
    TH1D  *h_MttVis;
    TH1D  *h_DeltaPhitt;
    TH1D  *h_DeltaEtatt;
    TH1D  *h_DeltaRtt;
    TH1D  *h_LeadTauPt;
    TH1D  *h_SubLeadTauPt;
    TH1D  *h_ElectronPt;
    TH1D  *h_MuonPt;
    TH1D  *h_MET;
    TH1D  *h_LeadJetPt;
    TH1D  *h_LeadJetEta;
    TH1D  *h_LeadJetPhi;
    TH1D  *h_SubLeadJetPt;
    TH1D  *h_SubLeadJetEta;
    TH1D  *h_SubLeadJetPhi;
    TH1D  *h_MinPhi;
    
    TH1D  *h_L1EtaGap;
    TH1D  *h_L1MjjInv;
    TH1D  *h_L1DeltaRjj;
    TH1D  *h_L1DeltaPhijj;
    TH1D  *h_L1LeadTauPt;
    TH1D  *h_L1SubLeadTauPt;
    TH1D  *h_L1ElectronPt;
    TH1D  *h_L1MuonPt;
    TH1D  *h_L1MET;
    TH1D  *h_L1LeadJetPt;
    TH1D  *h_L1LeadJetEta;
    TH1D  *h_L1LeadJetPhi;
    TH1D  *h_L1SubLeadJetPt;
    TH1D  *h_L1SubLeadJetEta;
    TH1D  *h_L1SubLeadJetPhi;

  public:

    VariableHistograms(std::string const& name, std::string output_name);
    virtual ~VariableHistograms();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
  };

}//namespace


#endif
