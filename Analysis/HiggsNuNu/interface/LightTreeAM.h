#ifndef ICHiggsTauTau_Module_LightTreeAM_h
#define ICHiggsTauTau_Module_LightTreeAM_h

#include <string>

#include "PhysicsTools/FWLite/interface/TFileService.h"

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"

#include "TTree.h"
#include "TVector3.h"
#include "Math/VectorUtil.h"

namespace ic {
  class LightTreeAM : public ModuleBase {

  private:

    CLASS_MEMBER(LightTreeAM,fwlite::TFileService*, fs);
    CLASS_MEMBER(LightTreeAM,std::string, met_label);
    CLASS_MEMBER(LightTreeAM,std::string, jet_label);
    CLASS_MEMBER(LightTreeAM,std::string, dijet_label);
    CLASS_MEMBER(LightTreeAM,bool, is_data);
    CLASS_MEMBER(LightTreeAM,bool, ignoreLeptons);
    CLASS_MEMBER(LightTreeAM,bool, do_trigskim);
    CLASS_MEMBER(LightTreeAM,bool, do_promptskim);
    CLASS_MEMBER(LightTreeAM,bool, do_noskim);

    unsigned countZmumu_;
    unsigned countZee_;

    TTree *outputTree_;
    
    unsigned run_;
    unsigned lumi_;
    unsigned event_;
    double weight_nolep_;
    double total_weight_lepveto_;
    double total_weight_leptight_;
    double puweight_up_scale_;
    double puweight_down_scale_;

    unsigned nJetsSave_;
    unsigned nJets_;
    unsigned nGenJets_;

    /*std::vector<double> jet_pt_;
    std::vector<double> jet_E_;
    std::vector<double> jet_eta_;
    std::vector<double> jet_phi_;
    std::vector<double> jet_csv_;
    std::vector<double> jet_jetid_;
    std::vector<double> jet_puid_;
    std::vector<int> jet_flavour_;

    std::vector<double> jet_genjet_mindR_;
    std::vector<unsigned> jet_genid_;
    std::vector<double> jet_genpt_;
    std::vector<double> jet_geneta_;
    std::vector<double> jet_genphi_;*/

    double* jet_pt_;
    double* jet_E_;
    double* jet_eta_;
    double* jet_phi_;
    double* jet_csv_;
    double* jet_jetid_;
    double* jet_puid_;
    int* jet_flavour_;

    double* jet_genjet_mindR_;
    unsigned* jet_genid_;
    double* jet_genpt_;
    double* jet_geneta_;
    double* jet_genphi_;

    unsigned n_jets_cjv_30_;
    unsigned n_jets_cjv_20EB_30EE_;
    unsigned n_jets_15_;
    unsigned n_jets_30_;
    double cjvjetpt_;

    double dijet_M_;
    double dijet_deta_;
    double dijet_sumeta_;
    double dijet_dphi_;
    double forward_tag_eta_;
    double central_tag_eta_;
    
    double sumet_;

    double l1met_;
    double met_;
    double genmet_;
    double genmetphi_;
    double metnomuons_;
    double met_x_;
    double met_y_;
    double metnomu_x_;
    double metnomu_y_;
    double met_significance_;
    double metnomu_significance_;
 
    double ht_;
    double ht30_;
    double mht_;
    double sqrt_ht_;

    double unclustered_et_;
    double jetunclet_mindphi_;
    double metnomuunclet_dphi_;

    double alljetsmetnomu_mindphi_;
    double jetmetnomu_mindphi_;

    double dijetmetnomu_scalarSum_pt_;
    double dijetmetnomu_vectorialSum_pt_;
    double dijetmetnomu_ptfraction_;
    double jet1metnomu_scalarprod_;
    double jet2metnomu_scalarprod_;

    double pass_sigtrigger_;
    double pass_mettrigger_;
    double pass_controltrigger_;

    int nvetomuons_;
    int nselmuons_;
    int nvetoelectrons_;
    int nselelectrons_;
    int ntaus_;
    int nloosephotons_;
    int nmediumphotons_;
    int ntightphotons_;

    double m_mumu_;
    double m_ee_;
    double m_mumu_gen_;
    double m_ee_gen_;
    double lep_mt_;

    double mu1_pt_;
    double mu1_eta_;
    double mu1_phi_;
    double mu1_genmindR_;
    double mu1_genpt_;
    double mu1_geneta_;
    double mu1_genphi_;
    double mu2_pt_;
    double mu2_eta_;
    double mu2_phi_;
    double mu2_genmindR_;
    double mu2_genpt_;
    double mu2_geneta_;
    double mu2_genphi_;

    double ele1_pt_;
    double ele1_eta_;
    double ele1_phi_;
    double ele1_genmindR_;
    double ele1_genpt_;
    double ele1_geneta_;
    double ele1_genphi_;

    double tau1_pt_;
    double tau1_eta_;
    double tau1_phi_;
    double tau1_genmindR_;
    double tau1_genpt_;
    double tau1_geneta_;
    double tau1_genphi_;

    double gamma1_pt_;
    double gamma1_eta_;
    double gamma1_phi_;
    double gamma1_genmindR_;
    double gamma1_genpt_;
    double gamma1_geneta_;
    double gamma1_genphi_;

    int n_vertices_;

  public:
    LightTreeAM(std::string const& name);
    virtual ~LightTreeAM();

    void resetAllTreeVariables();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();

   };


}


#endif
