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
#include "TDirectory.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"


namespace ic {

  class VariableHistograms : public ModuleBase {
  private:

    std::string jets_label_;
    std::string electrons_label_;
    std::string muons_label_;
    std::string taus_label_;
    std::string ditau_label_;
    std::string met_label_;
    
    std::string HLT1_label_;
    std::string HLT2_label_;
    std::string HLT3_label_;
    std::string HLT4_label_;
    std::string HLT5_label_;
    std::string HLT6_label_;
    std::string HLT7_label_;
    std::string HLT8_label_;
    std::string HLT9_label_;
    std::string HLT10_label_;
    std::string HLT11_label_;
    std::string HLT12_label_;
    std::string HLT13_label_;
    std::string HLT14_label_;
    std::string HLT15_label_;
    
    std::string channel_;

    int n_jets_;
    int n_electrons_;
    int n_muons_;
    int n_taus_;
    
    TH1D  *h_OfflineEtaGap;
    TH1D  *h_OfflineMjjInv;
    TH1D  *h_OfflineDeltaRjj;
    TH1D  *h_OfflineDeltaPhijj;
    TH1D  *h_OfflineMttVis;
    TH1D  *h_OfflineDeltaPhitt;
    TH1D  *h_OfflineDeltaEtatt;
    TH1D  *h_OfflineDeltaRtt;
    TH1D  *h_OfflineLeadTauPt;
    TH1D  *h_OfflineSubLeadTauPt;
    TH1D  *h_OfflineElectronPt;
    TH1D  *h_OfflineMuonPt;
    TH1D  *h_OfflineMET;
    TH1D  *h_OfflineLeadJetPt;
    TH1D  *h_OfflineLeadJetEta;
    TH1D  *h_OfflineLeadJetPhi;
    TH1D  *h_OfflineSubLeadJetPt;
    TH1D  *h_OfflineSubLeadJetEta;
    TH1D  *h_OfflineSubLeadJetPhi;
    TH1D  *h_OfflineMinPhi;
    
    TH1D *h_JetsPtVectorSum;
    TH1D *h_JetsPtScalarSum;
    TH1D *h_VBFMjj;


  public:

    VariableHistograms(std::string const& name, fwlite::TFileService *fs, std::string output_name);
    virtual ~VariableHistograms();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
  };

}//namespace


#endif
