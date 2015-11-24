#ifndef ICHiggsTauTau_Module_LightTreeTrig_h
#define ICHiggsTauTau_Module_LightTreeTrig_h

#include <string>

#include "PhysicsTools/FWLite/interface/TFileService.h"

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"

#include "TTree.h"

namespace ic {
  class LightTreeTrig : public ModuleBase {

  private:

    CLASS_MEMBER(LightTreeTrig,fwlite::TFileService*, fs);
    CLASS_MEMBER(LightTreeTrig,std::string, met_label);
    CLASS_MEMBER(LightTreeTrig,std::string, jet_label);
    CLASS_MEMBER(LightTreeTrig,std::string, dijet_label);
    CLASS_MEMBER(LightTreeTrig,bool, is_data);
    CLASS_MEMBER(LightTreeTrig,bool, dotrigskim);
    CLASS_MEMBER(LightTreeTrig,bool, do_noskim);
    CLASS_MEMBER(LightTreeTrig,std::string, trig_obj_label);
    CLASS_MEMBER(LightTreeTrig,std::string, cont_trig_obj_label);
    CLASS_MEMBER(LightTreeTrig,std::string, trigger_path);

    unsigned nGenJetsSave_;
    unsigned nTrigJetsSave_;

    TTree *outputTree_;
    unsigned processed_;
    
    unsigned run_;
    unsigned lumi_;
    unsigned event_;

    double* trigjet_jet_mindR_;
    int* trigjet_trigMatched_;
    double* trigjet_pt_;
    double* trigjet_eta_;
    double* trigjet_phi_;
    double* trigjet_E_;

    double sigtrigcalomet_;
    double sigtrigpfmet_;
    double conttrigcalomet_;
    double conttrigpfmet_;      

    double jet1met_dphi_;
    double jet2met_dphi_;
    double jet1metnomu_dphi_;
    double jet2metnomu_dphi_;
    double jetmet_mindphi_;
    double alljetsmet_mindphi_;

    double dijetmet_scalarSum_pt_;
    double dijetmet_vectorialSum_pt_;
    double dijetmet_ptfraction_;
    double jet1met_scalarprod_;
    double jet2met_scalarprod_;

    double pass_muontrigger_;
    double pass_singlejettrigger_;
    double pass_htquadjettrigger_;

    double* genjetptordered_pt_;
    double* genjetptordered_eta_;
    double* genjetptordered_phi_;
    double* genjetptordered_E_;

    double* trigjetptordered_pt_;
    double* trigjetptordered_eta_;
    double* trigjetptordered_phi_;
    double* trigjetptordered_E_;

    double digenjet_M_;
    double digenjet_deta_;
    double digenjet_dphi_;

    double ditrigjet_M_;
    double ditrigjet_deta_;
    double ditrigjet_dphi_;

    double genlep1_pt_;
    double genlep1_eta_;
    double genlep1_phi_;
    double genlep1_id_;
    double genlep2_pt_;
    double genlep2_eta_;
    double genlep2_phi_;
    double genlep2_id_;

  public:
    LightTreeTrig(std::string const& name);
    virtual ~LightTreeTrig();

    void resetAllTreeVariables();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();

   };

}


#endif
