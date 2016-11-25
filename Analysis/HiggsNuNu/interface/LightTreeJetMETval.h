#ifndef ICHiggsTauTau_Module_LightTreeJetMETval_h
#define ICHiggsTauTau_Module_LightTreeJetMETval_h

#include <string>

#include "PhysicsTools/FWLite/interface/TFileService.h"

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"

#include "TTree.h"
#include "TH2D.h"

namespace ic {
  class LightTreeJetMETval : public ModuleBase {

  private:

    CLASS_MEMBER(LightTreeJetMETval,fwlite::TFileService*, fs);
    CLASS_MEMBER(LightTreeJetMETval,std::string, met_label);
    CLASS_MEMBER(LightTreeJetMETval,std::string, jet_label);
    CLASS_MEMBER(LightTreeJetMETval,bool, is_data);
    CLASS_MEMBER(LightTreeJetMETval,bool, ignoreLeptons);
    CLASS_MEMBER(LightTreeJetMETval,bool, dotrigskim);
    CLASS_MEMBER(LightTreeJetMETval,bool, do_noskim);
    CLASS_MEMBER(LightTreeJetMETval,std::string, trig_obj_label);
    CLASS_MEMBER(LightTreeJetMETval,std::string, trigger_path);

    TTree *outputTree_;
    TH2D *hetavspt_genall_;
    TH2D *hetavspt_genmatched_;
    TH2D *hetavspt_recmatched_;
    
    unsigned run_;
    unsigned lumi_;
    unsigned event_;
    double weight_nolep_;
    double total_weight_lepveto_;
    double total_weight_leptight_;
    unsigned nJetsSave_;
    unsigned nJets_;

    std::vector<double> jet_pt_;
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
    std::vector<double> jet_genphi_;

    unsigned nGenJets_;
    std::vector<double> genjet_pt_;
    std::vector<double> genjet_eta_;
    std::vector<double> genjet_phi_;
    std::vector<double> genjet_jet_mindR_;
    std::vector<unsigned> genjet_recid_;
    std::vector<double> genjet_recpt_;
    std::vector<double> genjet_receta_;
    std::vector<double> genjet_recphi_;

    unsigned nMets_; 
    std::vector<std::string> metName_;
    std::vector<double> sumet_;
    std::vector<double> met_;
    std::vector<double> metphi_;
    std::vector<double> met_x_;
    std::vector<double> met_y_;

    double l1met_;
    double genmet_;
    double genmetphi_;
    double metnomuons_;
    double metnomu_x_;
    double metnomu_y_;
    double met_significance_;
    double metnomu_significance_;

    double pass_sigtrigger_;
    double pass_mettrigger_;
    double pass_controltrigger_;

    int nvetomuons_;
    int nselmuons_;
    int nvetoelectrons_;
    int nselelectrons_;
    int ntaus_;
    double m_mumu_;
    double m_ee_;
    double m_mumu_gen_;
    double m_ee_gen_;
    double lep_mt_;

    int n_vertices_;

  public:
    LightTreeJetMETval(std::string const& name);
    virtual ~LightTreeJetMETval();

    void getGenRecoMatches(const std::vector<PFJet*> & jets,
			   const std::vector<GenJet *> & genvec, 
			   std::vector<std::pair<unsigned,bool> > & recotogenmatch, 
			   std::vector<std::pair<unsigned,bool> > & gentorecomatch,
			   bool firstTime,
			   bool print=false);

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();

   };

}


#endif
