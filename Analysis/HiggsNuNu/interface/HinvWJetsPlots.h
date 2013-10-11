#ifndef ICHiggsTauTau_Module_HinvWJetsPlots_h
#define ICHiggsTauTau_Module_HinvWJetsPlots_h

#include <string>

#include "PhysicsTools/FWLite/interface/TFileService.h"

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"

#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"

#include "UserCode/ICHiggsTauTau/interface/TH2DAsymErr.h"
#include "TH1F.h"
#include "TH2F.h"


namespace ic {

  struct HinvWJetsControlPlots {
    TH1F *n_electrons;
    TH1F *n_muons;
    TH1F *n_taus;
    TH1F *mt_enu;
    TH1F *mt_munu;
    TH1F *mt_taunu;
    TH1F *zeppenfeld_enu;
    TH1F *zeppenfeld_munu;
    TH1F *ept_1;
    TH1F *ept_2;
    TH1F *eeta_1;
    TH1F *eeta_2;
    TH1F *mupt_1;
    TH1F *mupt_2;
    TH1F *mueta_1;
    TH1F *mueta_2;
    TH1F *taupt_1;
    TH1F *taupt_2;
    TH1F *taueta_1;
    TH1F *taueta_2;
    TH1F *dRmin_taujet;
    TH1F *dRmin_taujet1;
    TH1F *dRmin_taujet2;
    TH1F *taggingJetsFlavour;
    TH1F *edxy_all;
    TH1F *mudxy_all;
    TH1F *edz_all;
    TH1F *mudz_all;
    TH1F *eiso_all;
    TH1F *muiso_all;
    TH1F *met_noelectrons;
    TH1F *met_nomuons;

    HinvWJetsControlPlots(TFileDirectory const& dir);

  };


  class HinvWJetsPlots : public ModuleBase {

  private:

    CLASS_MEMBER(HinvWJetsPlots,fwlite::TFileService*, fs);
    CLASS_MEMBER(HinvWJetsPlots,std::string, met_label);
    CLASS_MEMBER(HinvWJetsPlots,std::string, met_nolep_label);
    CLASS_MEMBER(HinvWJetsPlots,std::string, electrons_label);
    CLASS_MEMBER(HinvWJetsPlots,std::string, muons_label);
    CLASS_MEMBER(HinvWJetsPlots,std::string, taus_label);
    CLASS_MEMBER(HinvWJetsPlots,std::string, dijet_label);
    CLASS_MEMBER(HinvWJetsPlots,std::string, sel_label);


    double yields_;
    HinvWJetsControlPlots* wjetsplots_;
    DynamicHistoSet * misc_plots_;
    Dynamic2DHistoSet * misc_2dplots_;

    // Event Properties
    double wt_;

    unsigned n_electrons_;
    unsigned n_muons_;
    unsigned n_taus_;
    double mt_enu_;
    double mt_munu_;
    double mt_taunu_;
    double zepp_e_;
    double zepp_mu_;
    double ept_1_;
    double ept_2_;
    double eeta_1_;
    double eeta_2_;
    double mupt_1_;
    double mupt_2_;
    double mueta_1_;
    double mueta_2_;
    double taupt_1_;
    double taupt_2_;
    double taueta_1_;
    double taueta_2_;
    double dRmin_taujet_;
    double dRmin_taujet1_;
    double dRmin_taujet2_;
    unsigned taggingJetsFlavour_;

    double met_noelectrons_;
    double met_nomuons_;

    unsigned counter_;
 
  public:
    HinvWJetsPlots(std::string const& name);
    virtual ~HinvWJetsPlots();
    
    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
 
    void FillYields();
    void InitPlots();
    void FillPlots();
   
    void Reset();

  };

}


#endif
