#ifndef ICHiggsTauTau_Utilities_HistoSet_h
#define ICHiggsTauTau_Utilities_HistoSet_h

#include "TH1F.h"
#include "TH2F.h"

#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"
#include "UserCode/ICHiggsTauTau/interface/Objects.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"


#include <string>
#include <iostream>

#include "TTree.h"

namespace ic {

//! An abstract base class for the automatic handling of branches in a TTree
/*! 
 Some text. 
 */


 class HistoSet {
  private:

  public:
    HistoSet() {
      ;
    }
 };

 class HttPlots : public HistoSet{
  private:
    // General event-level plots
    TH1F* n_good_pv_noweight;
    TH1F* n_good_pv;

    // MVA VBF plots
    TH1F* vbf_mva_val;
    TH1F* vbf_mjj;
    TH1F* vbf_dEta;
    TH1F* vbf_dPhi;
    TH1F* vbf_vDiTau;
    TH1F* vbf_vDiJet;
    TH1F* vbf_dphi_hj;
    TH1F* vbf_C1;
    TH1F* vbf_C2;
    TH2F* vbf_mjj_DEta_mva_Y;
    TH2F* mjj_DEta;
    TH2F* vbf_mjj_DEta_mva_N_cut_Y;

    // General lepton-pair plots
    TH1F* lep1_pt;
    TH1F* lep1_eta;
    TH1F* lep2_pt;
    TH1F* lep2_eta;
    TH1F* pair_vis_mass;
    TH1F* svfit_mass;
    TH1F* svfit_dc;
    TH1F* svfit_sm_fine;
    TH1F* svfit_mssm;
    TH1F* svfit_mssm_fine;

    TH1F* pair_vis_mass_40_40;
    TH1F* pair_vis_mass_40_40_MET_20;
    TH1F* pair_vis_mass_30_40;
    TH1F* pair_vis_mass_30_40_MET_20;
    TH1F* pair_vis_mass_20_40;
    TH1F* pair_vis_mass_20_40_MET_20;

    TH1F* pair_vis_mass_mode0;
    TH1F* pair_vis_mass_mode1;
    TH1F* pair_vis_mass_mode0_EB;
    TH1F* pair_vis_mass_mode1_EB;
    TH1F* pair_vis_mass_mode0_EE;
    TH1F* pair_vis_mass_mode1_EE;

    TH1F* svfit_mode0;
    TH1F* svfit_mode1;
    TH1F* svfit_mode0_EB;
    TH1F* svfit_mode1_EB;
    TH1F* svfit_mode0_EE;
    TH1F* svfit_mode1_EE;

    TH1F* vis_one_gev;
    TH1F* svfit_one_gev;

    TH1F* vis_sm;
    TH1F* vis_sm_fine;
    TH1F* vis_mssm;
    TH1F* vis_mssm_fine;

    TH1F* npartons;

    // Tau specific plots
    TH1F* tau_mva_iso_val;

    // Pair-MET plots
    TH1F* lep1_mvamet_mt;
    TH1F* lep1_mvamet_mt_all;
    TH1F* lep2_mvamet_mt;
    TH1F* lep1_mvamet_pzeta_0p85;
    TH1F* lep1_mvamet_pzeta_0p5;

    //Jet Plots
    TH1F* n_jets;
    TH1F* lead_jet_pt;
    TH1F* lead_jet_eta;
    TH1F* sublead_jet_pt;
    TH1F* sublead_jet_eta;
    TH1F* dijet_mass;
    TH1F* dijet_delta_eta;

    //b-jet Plots
    TH1F* n_pt20jets;
    TH1F* lead_pt20jet_pt;
    TH1F* lead_pt20jet_eta;
    TH1F* sublead_pt20jet_pt;
    TH1F* sublead_pt20jet_eta;
    TH1F* n_bjets;
    TH1F* lead_bjet_pt;
    TH1F* lead_bjet_eta;
    TH1F* lead_pt20jet_btag_val;
    TH1F* sublead_pt20jet_btag_val;
    TH1F* all_pt20jet_btag_val;

    //MET Plots
    TH1F* mvamet_et;
    TH1F* mvamet_phi;

    //BTag Eff
    TH1F* beff_all;
    TH1F* beff_pass;

  public:
    HttPlots(TFileDirectory const& dir);
    void FillVertexPlots(std::vector<Vertex *> const& vertices, double wt = 1.0);
    void FillVertexPlots(unsigned vertices, double wt = 1.0);

    void FillSVFitMassPlot(double const& mass, double wt = 1.0);

    void FillSVFitMassModePlots(CompositeCandidate const& dilepton, double const& mass, double wt = 1.0);

    void FillVbfMvaPlots( double const& mjj,
                                    double const& dEta,
                                    double const& dPhi,
                                    double const& vDiTau,
                                    double const& vDiJet,
                                    double const& dphi_hj,
                                    double const& C1,
                                    double const& C2,
                                    double const& mva_value, double wt = 1.0);

    void FillVbfMvaPlotsDummy( double wt = 1.0);
    void FillVbfMvaPlots( double const& mjj,
                                    double const& dEta,
                                    bool pass_mva,
                                    bool pass_cuts,
                                    double wt = 1.0);
    void FillLeptonMetPlots(CompositeCandidate const& dilepton, Met const& met, double wt = 1.0);
    void FillIncMT(CompositeCandidate const& dilepton, Met const& met, double wt = 1.0);
    void FillJetPlots(std::vector<PFJet *> jets, double wt = 1.0);
    void FillPt20JetPlots(std::vector<PFJet *> jets, double wt, double btag_weight);
    void FillJetPlotsDummy(std::vector<PFJet *> jets, double wt = 1.0);
    void FillPt20JetPlotsDummy(std::vector<PFJet *> jets, double wt = 1.0);
    void FillBEfficiency(std::vector<PFJet *> jets, double wt = 1.0);

    void FillNPartons(std::vector<GenParticle *> parts, double wt = 1.0);

 };







class DynamicHistoSet : public HistoSet{
  private:
    std::map<std::string, TH1F *> hmap_;
    TFileDirectory dir_;

  public:
    DynamicHistoSet(TFileDirectory const& dir) : HistoSet(), dir_(dir) {
    }

    void Create(std::string name, unsigned bins, double min, double max) {
      if (hmap_.count(name)) return;
      hmap_[name] = dir_.make<TH1F>(name.c_str(),name.c_str(),bins,min,max);
    }

    void Fill(std::string name, double value, double weight = 1.0) {
      if (!hmap_.count(name)) return;
      hmap_[name]->Fill(value,weight);
    }
    
    TH1F* Get_Histo(std::string name) {
      if(!hmap_.count(name)) { 
        std::cerr << "Histogram does not exist" << std::endl;
        throw;
      }
      else return hmap_[name];
    }
 };


 class Dynamic2DHistoSet : public HistoSet{
   private:
     std::map<std::string, TH2F *> hmap_;
     TFileDirectory dir_;

   public:
     Dynamic2DHistoSet(TFileDirectory const& dir) : HistoSet(), dir_(dir) {
     }

     void Create(std::string name, unsigned binsx, double minx, double maxx, unsigned binsy, double miny, double maxy) {
       if (hmap_.count(name)) return;
       hmap_[name] = dir_.make<TH2F>(name.c_str(),name.c_str(),binsx,minx,maxx,binsy,miny,maxy);
     }

     void Create(std::string name, unsigned binsx, const double* xarr, unsigned binsy, const double* yarr) {
       if (hmap_.count(name)) return;
       hmap_[name] = dir_.make<TH2F>(name.c_str(),name.c_str(),binsx,xarr,binsy,yarr);
     }

     void Fill(std::string name, double valuex, double valuey, double weight = 1.0) {
       if (!hmap_.count(name)) return;
       hmap_[name]->Fill(valuex, valuey, weight);
     }
     TH2F* Get_Histo(std::string name) {
       if(!hmap_.count(name)) { 
         throw;
         std::cerr << "Histogram does not exist" << std::endl;
       }
       else return hmap_[name];
     }
  };

}


#endif
