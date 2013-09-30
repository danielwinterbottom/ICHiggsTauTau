#ifndef ICHiggsTauTau_Module_HinvControlPlots_h
#define ICHiggsTauTau_Module_HinvControlPlots_h

#include <string>

#include "PhysicsTools/FWLite/interface/TFileService.h"

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"

#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"

#include "UserCode/ICHiggsTauTau/interface/TH2DAsymErr.h"
#include "TH1F.h"
#include "TH2F.h"


namespace ic {

  struct HinvCoreControlPlots {
    TH1F *n_vtx;
    TH1F *met;
    TH1F *met_noMuons;
    TH1F *met_phi;
    TH1F *n_jets;
    TH1F *n_jetsingap;
    TH1F *jpt_1;
    TH1F *jpt_2;
    TH1F *jeta_1;
    TH1F *jeta_2;
    TH1F *mjj;
    TH1F *detajj;
    TH1F *etaprodjj;
    TH1F *drjj;
    TH1F *dphijj;
    //TH1F *dphi_metj1;
    //TH1F *dphi_metj2;
    //TH1F *dphi_metj3;
    //TH1F *mindphi_metj;

    HinvCoreControlPlots(TFileDirectory const& dir);
    
  };

  struct HinvWeightPlots {
    TH1F *met_noW;
    TH1F *dphijj_noW;
    TH1F *n_jets_noW;
    TH1F *met_pu;
    TH1F *dphijj_pu;
    TH1F *n_jets_pu;
    TH1F *met_pu_trig;
    TH1F *dphijj_pu_trig;
    TH1F *n_jets_pu_trig;
    TH1F *met_pu_trig_idiso;
    TH1F *dphijj_pu_trig_idiso;
    TH1F *n_jets_pu_trig_idiso;

    HinvWeightPlots(TFileDirectory const& dir);

  };

  struct HinvSystPlots {
    TH1F *n_jets_puUp;
    TH1F *n_jets_puDown;
    TH1F *n_vtx_puUp;
    TH1F *n_vtx_puDown;
    TH1F *dphijj_puUp;
    TH1F *dphijj_puDown;

    HinvSystPlots(TFileDirectory const& dir);

  };

  // Plots for possible new variables based on topology of
  // the dijet and MET system
  struct HinvDijetMETPlots {
    TH1F *vecSumTriObjectPt;
    TH1F *scalSumTriObjectPt;
    TH1F *htMET;
    TH1F *dijetOverMetPt;
    TH1F *alphaT;
    TH1F *betaT;
    TH2F *vecSum_htMET;
    TH2F *dijetFrac_htMET;
    TH2F *alphaT_htMET;
    TH2F *betaT_htMET;
    
    HinvDijetMETPlots(TFileDirectory const& dir);
    
  };

  struct HinvHTPlots {
    TH1F *Ht;
    TH1F *SqrtHt;
    TH2F *MetHt;
    TH1F *MetHt0to10;
    TH1F *MetHt10to20;
    TH1F *MetHt20to30;
    TH1F *MetHt30to40;
    TH1F *MetHt40to50;
    TH1F *MetHt50to60;
    TH1F *MetHt60to70;
    TH1F *MetHt70to80;
    TH1F *MetHt80to90;
    TH1F *MetHt90to100;
    TH1F *MetHt100to110;
    TH1F *MetHt110to120;
    TH1F *MetHt120to130;
    TH2F *MetSqrtHt;
    TH1F *MetSqrtHt0to10;
    TH1F *MetSqrtHt10to20;
    TH1F *MetSqrtHt20to30;
    TH1F *MetSqrtHt30to40;
    TH1F *MetSqrtHt40to50;
    TH1F *MetSqrtHt50to60;
    TH1F *MetSqrtHt60to70;
    TH1F *MetSqrtHt70to80;
    TH1F *MetSqrtHt80to90;
    TH1F *MetSqrtHt90to100;
    TH1F *MetSqrtHt100to110;
    TH1F *MetSqrtHt110to120;
    TH1F *MetSqrtHt120to130;
    TH1F *unclusteredEt;
    TH1F *MHT;
    TH1F *dphimetMHT;
    TH1F *METminusMHT;
    HinvHTPlots(TFileDirectory const& dir);
    
  };

  struct HinvGenPlots {
    TH1F *taupt;
    TH1F *taueta;
    TH2F *tauptvseta;
    TH1F *mindR_gentau_tagjets;
    TH1F *dR_genjet_gentau;
    TH1F *dR_recotau_genjet;
    TH1F *dR_recotau_status3tau;
    TH1F *recotaupt;
    TH1F *recotaueta;
    TH1F *recotaupt_status3;
    TH1F *recotaueta_status3;

    HinvGenPlots(TFileDirectory const& dir);
  };
  

  class HinvControlPlots : public ModuleBase {

  private:

    CLASS_MEMBER(HinvControlPlots,fwlite::TFileService*, fs);
    CLASS_MEMBER(HinvControlPlots,std::string, met_label);
    CLASS_MEMBER(HinvControlPlots,std::string, dijet_label);
    CLASS_MEMBER(HinvControlPlots,std::string, sel_label);
    CLASS_MEMBER(HinvControlPlots,std::string, channel);


    double yields_;
    HinvCoreControlPlots *controlplots_;
    HinvWeightPlots      *weightplots_;
    HinvSystPlots        *systplots_;
    HinvDijetMETPlots    *dijetMETPlots_;
    HinvHTPlots          *HTPlots_;
    HinvGenPlots *genPlots_;

    DynamicHistoSet * misc_plots_;
    Dynamic2DHistoSet * misc_2dplots_;

  // Event Properties

    double wt_;

    unsigned n_vtx_;
    double met_;
    double met_noMuons_;
    double met_phi_;

    unsigned n_jets_;
    unsigned n_jetsingap_; // Defined if n_jets >= 2

    double jpt_1_;     // Defined if n_jets >= 1
    double jpt_2_;     // Defined if n_jets >= 2
    double jeta_1_;    // Defined if n_jets >= 1
    double jeta_2_;    // Defined if n_jets >= 2

    double mjj_;       // Defined if n_jets >= 2
    double detajj_;     // Defined if n_jets >= 2
    double etaprodjj_;     // Defined if n_jets >= 2
    double drjj_;     // Defined if n_jets >= 2
    double dphijj_;     // Defined if n_jets >= 2

 
  public:
    HinvControlPlots(std::string const& name);
    virtual ~HinvControlPlots();
    
    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
 
    void FillYields();
    void InitCoreControlPlots();
    void InitWeightPlots();
    void InitSystPlots();
    void InitDijetMETPlots();
    void InitHTPlots();
    void InitGenPlots();

    void FillCoreControlPlots();
    void FillWeightPlots(EventInfo * info);
    void FillSystPlots(EventInfo * info);
   
    void Reset();

  };

}


#endif
