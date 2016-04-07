#ifndef ICHiggsTauTau_Module_L1VariableHistograms_h
#define ICHiggsTauTau_Module_L1VariableHistograms_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
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

namespace ic {

  class L1VariableHistograms : public ModuleBase {
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

    int n_jets_;
    int n_electrons_;
    int n_muons_;
    int n_taus_;
    
    int n_l1jets_;
    int n_l1electrons_;
    int n_l1muons_;
    int n_l1taus_;
    
    TH1D  *h_L1TEtaGap;
    TH1D  *h_L1TMjjInv;
    TH1D  *h_L1TDeltaRjj;
    TH1D  *h_L1TDeltaPhijj;
    TH1D  *h_L1TMttVis;
    TH1D  *h_L1TDeltaPhitt;
    TH1D  *h_L1TDeltaEtatt;
    TH1D  *h_L1TDeltaRtt;
    TH1D  *h_L1TLeadTauPt;
    TH1D  *h_L1TSubLeadTauPt;
    TH1D  *h_L1TElectronPt;
    TH1D  *h_L1TMuonPt;
    TH1D  *h_L1TMET;
    TH1D  *h_L1TLeadJetPt;
    TH1D  *h_L1TLeadJetEta;
    TH1D  *h_L1TLeadJetPhi;
    TH1D  *h_L1TSubLeadJetPt;
    TH1D  *h_L1TSubLeadJetEta;
    TH1D  *h_L1TSubLeadJetPhi;
    TH1D  *h_L1TMinPhi;
    
    TH1D  *h_L1TL1EtaGap;
    TH1D  *h_L1TL1MjjInv;
    TH1D  *h_L1TL1DeltaRjj;
    TH1D  *h_L1TL1DeltaPhijj;
    TH1D  *h_L1TL1LeadTauPt;
    TH1D  *h_L1TL1SubLeadTauPt;
    TH1D  *h_L1TL1ElectronPt;
    TH1D  *h_L1TL1MuonPt;
    TH1D  *h_L1TL1MET;
    TH1D  *h_L1TL1LeadJetPt;
    TH1D  *h_L1TL1LeadJetEta;
    TH1D  *h_L1TL1LeadJetPhi;
    TH1D  *h_L1TL1SubLeadJetPt;
    TH1D  *h_L1TL1SubLeadJetEta;
    TH1D  *h_L1TL1SubLeadJetPhi;
    TH1D  *h_L1TauMultiplicity;
    TH1D  *h_L1IsoTauMultiplicity;
    
    TH1D *h_LeadIsoTauIDProb;
    TH1D *h_LeadIsoTauIDProbTotal;
    TH1D *h_PtDiffGenMatched;
    TH1D *h_PtResGenMatched;
    TH1D *h_DeltaRResGenMatched;
    TH1D *h_PtDiffNotMatched;
    TH1D *h_PtResNotMatched;
    TH1D *h_DeltaRResNotMatched;
    TH1D *h_DeltaRMissID;
    
    TH2D  *h_TauPtVsJetPt;
    TH2D *h_TauPtToNormalize;

  public:

    L1VariableHistograms(std::string const& name, fwlite::TFileService *fs, std::string output_name);
    virtual ~L1VariableHistograms();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
  };

}//namespace


#endif
