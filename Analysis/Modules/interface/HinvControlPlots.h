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

    HinvCoreControlPlots(TFileDirectory const& dir);
    
  };


  class HinvControlPlots : public ModuleBase {

  private:

    CLASS_MEMBER(HinvControlPlots,fwlite::TFileService*, fs);
    CLASS_MEMBER(HinvControlPlots,std::string, met_label);
    CLASS_MEMBER(HinvControlPlots,std::string, dijet_label);
    CLASS_MEMBER(HinvControlPlots,std::string, sel_label);


    double yields_;
    HinvCoreControlPlots* controlplots_;
    DynamicHistoSet * misc_plots_;
    Dynamic2DHistoSet * misc_2dplots_;

  // Event Properties

    double wt_;

    unsigned n_vtx_;
    double met_;
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
    void FillCoreControlPlots();
   
    void Reset();

  };

}


#endif
