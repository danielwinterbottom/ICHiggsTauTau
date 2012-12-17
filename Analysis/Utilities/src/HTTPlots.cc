#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HTTPlots.h"




namespace ic {

  MassPlots::MassPlots(TFileDirectory const& dir) {
    TH1F::SetDefaultSumw2();
    m_sv = dir.make<TH1F>("m_sv","m_sv", 70, 0, 350); 
    m_vis = dir.make<TH1F>("m_vis","m_vis",  70, 0, 350); 
    double bins_sm[14] =        { 0., 20., 40., 60., 80., 100., 120., 140., 160., 180., 200., 250., 300., 350. };
    double bins_sm_fine[27] =   { 0., 10, 20., 30., 40., 50., 60., 70., 80., 90., 100., 110, 120., 130., 140., 150., 160., 170., 180., 190., 200., 225., 250., 275., 300., 325., 350. };
    double bins_mssm[19] =      {0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,1000,1500}; 
    double bins_mssm_fine[32] = {0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,1000,1500}; 
    m_sv_sm = dir.make<TH1F>("m_sv_sm","m_sv_sm", 13, bins_sm);
    m_sv_sm_fine = dir.make<TH1F>("m_sv_sm_fine","m_sv_sm_fine", 26, bins_sm_fine); 
    m_sv_mssm = dir.make<TH1F>("m_sv_mssm","m_sv_mssm", 18, bins_mssm); 
    m_sv_mssm_fine = dir.make<TH1F>("m_sv_mssm_fine","m_sv_mssm_fine", 31, bins_mssm_fine); 
    m_vis_sm = dir.make<TH1F>("m_vis_sm","m_vis_sm", 13, bins_sm); 
    m_vis_sm_fine = dir.make<TH1F>("m_vis_sm_fine","m_vis_sm_fine", 26, bins_sm_fine); 
    m_vis_mssm = dir.make<TH1F>("m_vis_mssm","m_vis_mssm", 18, bins_mssm); 
    m_vis_mssm_fine = dir.make<TH1F>("m_vis_mssm_fine","m_vis_mssm_fine", 31, bins_mssm_fine); 
  };

  CoreControlPlots::CoreControlPlots(TFileDirectory const& dir) {
    TH1F::SetDefaultSumw2();
    n_vtx = dir.make<TH1F>("n_vtx","n_vtx", 50, -0.5, 49.5); 
    mt_1 = dir.make<TH1F>("mt_1","mt_1", 80, 0, 160); 
    pzeta = dir.make<TH1F>("pzeta","pzeta", 100, -100, 100); 
    pt_1 = dir.make<TH1F>("pt_1","pt_1", 100, 0, 100); 
    pt_2 = dir.make<TH1F>("pt_2","pt_2", 100, 0, 100); 
    eta_1 = dir.make<TH1F>("eta_1","eta_1", 60, -3, 3); 
    eta_2 = dir.make<TH1F>("eta_2","eta_2", 60, -3, 3); 
    met = dir.make<TH1F>("met","met", 100, 0, 200); 
    met_phi = dir.make<TH1F>("met_phi","met_phi", 63, -3.15, 3.15); 
    n_jets = dir.make<TH1F>("n_jets","n_jets", 10, -0.5, 9.5); 
    n_bjets = dir.make<TH1F>("n_bjets","n_bjets", 5, -0.5, 4.5); 
    n_jetsingap = dir.make<TH1F>("n_jetsingap","n_jetsingap", 5, -0.5, 4.5); 
    jpt_1 = dir.make<TH1F>("jpt_1","jpt_1", 100, 0, 200); 
    jpt_2 = dir.make<TH1F>("jpt_2","jpt_2", 100, 0, 200); 
    jeta_1 = dir.make<TH1F>("jeta_1","jeta_1", 50, -5, 5); 
    jeta_2 = dir.make<TH1F>("jeta_2","jeta_2", 50, -5, 5); 
    bpt_1 = dir.make<TH1F>("bpt_1","bpt_1", 50, 0, 200); 
    beta_1 = dir.make<TH1F>("beta_1","beta_1", 60, -3, 3); 
    mjj = dir.make<TH1F>("mjj","mjj", 40, 0, 1000); 
    jdeta = dir.make<TH1F>("jdeta","jdeta", 80, 0, 8); 
  };

} //namespace
