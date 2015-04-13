#ifndef ICHiggsTauTau_Module_Year1Proj_h
#define ICHiggsTauTau_Module_Year1Proj_h

#include <string>

#include "PhysicsTools/FWLite/interface/TFileService.h"

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"

#include "TTree.h"

namespace ic {
  class Year1Proj : public ModuleBase {

  private:

    CLASS_MEMBER(Year1Proj,fwlite::TFileService*, fs);
    CLASS_MEMBER(Year1Proj,std::string, met_label);
    CLASS_MEMBER(Year1Proj,std::string, dijet_label);
    CLASS_MEMBER(Year1Proj,std::string, sel_label);
    CLASS_MEMBER(Year1Proj,std::string, channel);
    CLASS_MEMBER(Year1Proj,bool, is_data);
    CLASS_MEMBER(Year1Proj,bool, is_embedded);
    CLASS_MEMBER(Year1Proj,std::string, trig_obj_label);
    CLASS_MEMBER(Year1Proj,std::string, trigger_path);

    TTree *outputTree_;
    

    double total_weight_;
    double jet1_pt_;
    double jet2_pt_;
    double jet1_eta_;
    double jet2_eta_;
    double jet1_phi_;
    double jet2_phi_;
    double jet1_E_;
    double jet2_E_;
    double dijet_M_;
    double dijet_deta_;
    double dijet_sumeta_;
    double dijet_dphi_;
    double met_;
    double met_x_;
    double met_y_;
    double met_phi_;
    double met_significance_;
    double sumet_;
    double ht_;
    double mht_;
    double sqrt_ht_;
    double unclustered_et_;
    double unclustered_phi_;
    double jet1met_dphi_;
    double jet2met_dphi_;
    double jetmet_mindphi_;
    double jetunclet_mindphi_;
    double metunclet_dphi_;
    double dijetmet_scalarSum_pt_;
    double dijetmet_vectorialSum_pt_;
    double dijetmet_ptfraction_;
    double jet1met_scalarprod_;
    double jet2met_scalarprod_;
    unsigned n_jets_cjv_30_;
    unsigned n_jets_cjv_20EB_30EE_;
    double passtrigger_;
    double passparkedtrigger1_;
    double passparkedtrigger2_;
    double l1met_;
    double metnomuons_;

    int nvetomuons_;
    int nselmuons_;
    int nvetoelectrons_;
    int nselelectrons_;
    int ntaus_;
    double m_mumu_;

    //1st year specific variables
    double mu1px_;
    double mu1py_;
    double mu1pz_;
    double mu1E_;
    double mu2px_;
    double mu2py_;
    double mu2pz_;
    double mu2E_;

  public:
    Year1Proj(std::string const& name);
    virtual ~Year1Proj();
    
    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();

   };

}


#endif
