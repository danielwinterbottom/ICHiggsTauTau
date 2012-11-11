#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"




namespace ic {

  HttPlots::HttPlots(TFileDirectory const& dir) : HistoSet() {
    // Vertex plots
    TH1F::SetDefaultSumw2();
    n_good_pv_noweight  = dir.make<TH1F>("n_good_pv_noweight","n_good_pv_noweight", 50, 0, 50);
    n_good_pv  = dir.make<TH1F>("n_good_pv","n_good_pv", 50, 0, 50);
    
    // VBF MVA plots
    vbf_mva_val  = dir.make<TH1F>("vbf_mva_val","vbf_mva_val", 50, -1 , 1);
    vbf_mjj = dir.make<TH1F>("vbf_mjj","vbf_mjj", 60, 0, 1500);
    vbf_dEta = dir.make<TH1F>("vbf_dEta","vbf_dEta", 80, 0, 8);

    vbf_mjj_DEta_mva_Y = dir.make<TH2F>("vbf_mjj_Deta_mva_Y","vbf_mjj_Deta_mva_Y", 60, 0, 1500, 80, 0, 8);
    vbf_mjj_DEta_mva_N_cut_Y = dir.make<TH2F>("vbf_mjj_DEta_mva_N_cut_Y","vbf_mjj_DEta_mva_N_cut_Y", 60, 0, 1500, 80, 0, 8);
    
    mjj_DEta = dir.make<TH2F>("mjj_DEta","mjj_DEta", 60, 0, 1500, 80, 0, 8);

    npartons = dir.make<TH1F>("npartons","npartons", 6, -0.5, 5.5);

    vbf_dPhi = dir.make<TH1F>("vbf_dPhi","vbf_dPhi", 63, 0, 3.15);
    vbf_vDiTau = dir.make<TH1F>("vbf_vDiTau","vbf_vDiTau", 200, 0, 200);
    vbf_vDiJet = dir.make<TH1F>("vbf_vDiJet","vbf_vDiJet", 200, 0, 200);
    vbf_dphi_hj = dir.make<TH1F>("vbf_dphi_hj","vbf_dphi_hj", 63, 0, 3.15);
    vbf_C1 = dir.make<TH1F>("vbf_C1","vbf_C1", 50, 0, 5);
    vbf_C2 = dir.make<TH1F>("vbf_C2","vbf_C2", 200, 0, 200);

    // General lepton-pair plots
    lep1_pt  = dir.make<TH1F>("lep1_pt","lep1_pt", 100, 0, 100);
    lep1_eta  = dir.make<TH1F>("lep1_eta","lep1_eta", 60, -3, 3);
    lep2_pt  = dir.make<TH1F>("lep2_pt","lep2_pt", 100, 0, 100);
    lep2_eta  = dir.make<TH1F>("lep2_eta","lep2_eta", 60, -3, 3);
    pair_vis_mass  = dir.make<TH1F>("pair_vis_mass","pair_vis_mass", 100, 0, 500);

    vis_one_gev  = dir.make<TH1F>("vis_one_gev","vis_one_gev", 300, 0, 300);
    svfit_one_gev  = dir.make<TH1F>("svfit_one_gev","svfit_one_gev", 300, 0, 300);
    

    pair_vis_mass_20_40 = dir.make<TH1F>("pair_vis_mass_20_40","pair_vis_mass_20_40", 300, 0, 300);
    pair_vis_mass_20_40_MET_20  = dir.make<TH1F>("pair_vis_mass_20_40_MET_20","pair_vis_mass_20_40_MET_20", 300, 0, 300);
    pair_vis_mass_30_40 = dir.make<TH1F>("pair_vis_mass_30_40","pair_vis_mass_30_40", 300, 0, 300);
    pair_vis_mass_30_40_MET_20  = dir.make<TH1F>("pair_vis_mass_30_40_MET_20","pair_vis_mass_30_40_MET_20", 300, 0, 300);
    pair_vis_mass_40_40 = dir.make<TH1F>("pair_vis_mass_40_40","pair_vis_mass_40_40", 300, 0, 300);
    pair_vis_mass_40_40_MET_20  = dir.make<TH1F>("pair_vis_mass_40_40_MET_20","pair_vis_mass_40_40_MET_20", 300, 0, 300);

    pair_vis_mass_mode0  = dir.make<TH1F>("pair_vis_mass_mode0","pair_vis_mass_mode0", 150, 0, 300);
    pair_vis_mass_mode1  = dir.make<TH1F>("pair_vis_mass_mode1","pair_vis_mass_mode1", 150, 0, 300);

    pair_vis_mass_mode0_EB  = dir.make<TH1F>("pair_vis_mass_mode0_EB","pair_vis_mass_mode0_EB", 150, 0, 300);
    pair_vis_mass_mode1_EB  = dir.make<TH1F>("pair_vis_mass_mode1_EB","pair_vis_mass_mode1_EB", 150, 0, 300);

    pair_vis_mass_mode0_EE  = dir.make<TH1F>("pair_vis_mass_mode0_EE","pair_vis_mass_mode0_EE", 150, 0, 300);
    pair_vis_mass_mode1_EE  = dir.make<TH1F>("pair_vis_mass_mode1_EE","pair_vis_mass_mode1_EE", 150, 0, 300);

    svfit_mode0  = dir.make<TH1F>("svfit_mode0","svfit_mode0", 150, 0, 300);
    svfit_mode1  = dir.make<TH1F>("svfit_mode1","svfit_mode1", 150, 0, 300);

    svfit_mode0_EB  = dir.make<TH1F>("svfit_mode0_EB","svfit_mode0_EB", 150, 0, 300);
    svfit_mode1_EB  = dir.make<TH1F>("svfit_mode1_EB","svfit_mode1_EB", 150, 0, 300);

    svfit_mode0_EE  = dir.make<TH1F>("svfit_mode0_EE","svfit_mode0_EE", 150, 0, 300);
    svfit_mode1_EE  = dir.make<TH1F>("svfit_mode1_EE","svfit_mode1_EE", 150, 0, 300);

    svfit_mass  = dir.make<TH1F>("svfit_mass","svfit_mass", 25, 0, 500);
    double bins[14] =       { 0., 20., 40., 60., 80., 100., 120., 140., 160., 180., 200., 250., 300., 350. };
    double bins_fine[27] =       { 0., 10, 20., 30., 40., 50., 60., 70., 80., 90., 100., 110, 120., 130., 140., 150., 160., 170., 180., 190., 200., 225., 250., 275., 300., 325., 350. };

    svfit_dc = dir.make<TH1F>("svfit_sm","svfit_sm",13, bins);

    svfit_sm_fine = dir.make<TH1F>("svfit_sm_fine","svfit_sm_fine",26, bins_fine);


    double bins_mssm_fine[32] = {0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,1000,1500}; 
    double bins_mssm[19] = {0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,1000,1500}; 

    svfit_mssm_fine = dir.make<TH1F>("svfit_mssm_fine","svfit_mssm_fine",31, bins_mssm_fine);
    svfit_mssm = dir.make<TH1F>("svfit_mssm","svfit_mssm",18, bins_mssm);

    vis_sm = dir.make<TH1F>("vis_sm","vis_sm",13, bins);
    vis_sm_fine = dir.make<TH1F>("vis_sm_fine","vis_sm_fine",26, bins_fine);
    vis_mssm_fine = dir.make<TH1F>("vis_mssm_fine","vis_mssm_fine",31, bins_mssm_fine);
    vis_mssm = dir.make<TH1F>("vis_mssm","vis_mssm",18, bins_mssm);

    // Tau specific plots
    tau_mva_iso_val  = dir.make<TH1F>("tau_mva_iso_val","tau_mva_iso_val", 50, 0.5, 1);

    // Pair-MET plots
    lep1_mvamet_mt  = dir.make<TH1F>("lep1_mvamet_mt","lep1_mvamet_mt", 80, 0, 160);
    lep1_mvamet_mt_all  = dir.make<TH1F>("lep1_mvamet_mt_all","lep1_mvamet_mt_all", 80, 0, 160);
    lep2_mvamet_mt  = dir.make<TH1F>("lep2_mvamet_mt","lep2_mvamet_mt", 80, 0, 160);
    
    lep1_mvamet_pzeta_0p85  = dir.make<TH1F>("lep1_mvamet_pzeta_0p85","lep1_mvamet_pzeta_0p85", 80, -200, 200);
    lep1_mvamet_pzeta_0p5  = dir.make<TH1F>("lep1_mvamet_pzeta_0p5","lep1_mvamet_pzeta_0p5", 80, -200, 200);

    //Jet Plots
    n_jets  = dir.make<TH1F>("n_jets","n_jets", 10, 0, 10);
    lead_jet_pt  = dir.make<TH1F>("lead_jet_pt","lead_jet_pt", 60, 0, 300);
    lead_jet_eta  = dir.make<TH1F>("lead_jet_eta","lead_jet_eta", 100, -5, 5);
    sublead_jet_pt  = dir.make<TH1F>("sublead_jet_pt","sublead_jet_pt", 60, 0, 300);
    sublead_jet_eta  = dir.make<TH1F>("sublead_jet_eta","sublead_jet_eta", 100, -5, 5);
    dijet_mass  = dir.make<TH1F>("dijet_mass","dijet_mass", 40, 0, 1000);
    dijet_delta_eta  = dir.make<TH1F>("dijet_delta_eta","dijet_delta_eta", 80, 0, 8);

    //b-jet Plots
    n_pt20jets  = dir.make<TH1F>("n_pt20jets","n_pt20jets", 10, 0, 10);
    lead_pt20jet_pt  = dir.make<TH1F>("lead_pt20jet_pt","lead_pt20jet_pt", 60, 0, 300);
    lead_pt20jet_eta  = dir.make<TH1F>("lead_pt20jet_eta","lead_pt20jet_eta", 100, -5, 5);
    sublead_pt20jet_pt  = dir.make<TH1F>("sublead_pt20jet_pt","sublead_pt20jet_pt", 60, 0, 300);
    sublead_pt20jet_eta  = dir.make<TH1F>("sublead_pt20jet_eta","sublead_pt20jet_eta", 100, -5, 5);
    n_bjets  = dir.make<TH1F>("n_bjets","n_bjets", 10, 0, 10);
    lead_bjet_pt  = dir.make<TH1F>("lead_bjet_pt","lead_bjet_pt", 60, 0, 300);
    lead_bjet_eta  = dir.make<TH1F>("lead_bjet_eta","lead_bjet_eta", 100, -5, 5);
    lead_pt20jet_btag_val  = dir.make<TH1F>("lead_pt20jet_btag_val","lead_pt20jet_btag_val", 20, 0, 1);
    sublead_pt20jet_btag_val  = dir.make<TH1F>("sublead_pt20jet_btag_val","sublead_pt20jet_btag_val", 20, 0, 1);

    //MET Plots
    mvamet_et  = dir.make<TH1F>("mvamet_et","mvamet_et", 200, 0, 200);
    mvamet_phi  = dir.make<TH1F>("mvamet_phi","mvamet_phi", 126, -3.15, 3.15);

    double beff_bins[9] = { 20., 40., 60., 80., 100., 150., 200., 300., 400.};
    beff_all = dir.make<TH1F>("beff_all","beff_all",8, beff_bins);
    beff_pass = dir.make<TH1F>("beff_pass","beff_pass",8, beff_bins);

  }

  void HttPlots::FillVertexPlots(std::vector<Vertex *> const& vertices, double wt) {
    n_good_pv_noweight->Fill(vertices.size(), 1.0);
    n_good_pv->Fill(vertices.size(), wt);
  }

  void HttPlots::FillVertexPlots(unsigned vertices, double wt) {
    n_good_pv_noweight->Fill(vertices, 1.0);
    n_good_pv->Fill(vertices, wt);
  }

  void HttPlots::FillSVFitMassPlot(double const& mass, double wt) {
    svfit_mass->Fill(mass, wt);
    svfit_dc->Fill(mass, wt);
    svfit_one_gev->Fill(mass, wt);
    svfit_sm_fine->Fill(mass, wt);
    svfit_mssm->Fill(mass, wt);
    svfit_mssm_fine->Fill(mass, wt);
  }

  void HttPlots::FillNPartons(std::vector<GenParticle *> parts, double wt) {
    bool count_jets = false;
    unsigned partons = 0;
    // std::cout << "--------New Events-----------" << std::endl;
    for (unsigned i = 0; i < parts.size(); ++i) {
      // std::cout << i << "\t" << parts[i]->pdgid() << "\t" << parts[i]->status() << "\t" << parts[i]->vector() << std::endl;
      if (parts[i]->status() != 3) continue;  // Don't care about non-status 3
      unsigned id = abs(parts[i]->pdgid());
      if (count_jets) { // If we've found the W, the subsequent particles will be the partons - count them
        if (id == 1 || id == 2 || id == 3 || id == 4 || id == 5 || id == 6 || id == 21) partons++;
      }
      if (id == 24) count_jets = true;  // Start counting partons after we find the W
    }
    npartons->Fill(partons, wt);
  }


  void HttPlots::FillVbfMvaPlots( double const& mjj,
                                  double const& dEta,
                                  double const& dPhi,
                                  double const& vDiTau,
                                  double const& vDiJet,
                                  double const& dphi_hj,
                                  double const& C1,
                                  double const& C2,
                                  double const& mva_value, double wt) {
    vbf_mjj->Fill(mjj, wt);
    vbf_dEta->Fill(dEta, wt);
    vbf_dPhi->Fill(dPhi, wt);
    vbf_vDiTau->Fill(vDiTau, wt);
    vbf_vDiJet->Fill(vDiJet, wt);
    vbf_dphi_hj->Fill(dphi_hj, wt);
    vbf_C1->Fill(C1, wt);
    vbf_C2->Fill(C2, wt);
    vbf_mva_val->Fill(mva_value, wt);
  }

  void HttPlots::FillVbfMvaPlotsDummy(double wt) {
    vbf_mjj->Fill(0., wt);
    vbf_dEta->Fill(0., wt);
    vbf_dPhi->Fill(0., wt);
    vbf_vDiTau->Fill(0., wt);
    vbf_vDiJet->Fill(0., wt);
    vbf_dphi_hj->Fill(0., wt);
    vbf_C1->Fill(0., wt);
    vbf_C2->Fill(0., wt);
    vbf_mva_val->Fill(0., wt);
  }


  void HttPlots::FillVbfMvaPlots( double const& mjj,
                                  double const& dEta,
                                  bool pass_mva,
                                  bool pass_cuts,
                                  double wt) {
    vbf_mjj->Fill(mjj, wt);
    vbf_dEta->Fill(dEta, wt);
    if (pass_mva) {
      vbf_mjj_DEta_mva_Y->Fill(mjj, dEta, wt);
    }
    if (!pass_mva && pass_cuts) {
      vbf_mjj_DEta_mva_N_cut_Y->Fill(mjj, dEta, wt);
    }
  }



  void HttPlots::FillLeptonMetPlots(CompositeCandidate const& dilepton, Met const& met, double wt) {
    Candidate const* lep1 = dilepton.GetCandidate("lepton1");
    Candidate const* lep2 = dilepton.GetCandidate("lepton2");
    lep1_pt->Fill(lep1->pt(),wt);
    lep1_eta->Fill(lep1->eta(), wt);
    lep2_pt->Fill(lep2->pt(), wt);
    lep2_eta->Fill(lep2->eta(), wt);
    double vis_mass = dilepton.vector().M();
    pair_vis_mass->Fill(vis_mass, wt);
    vis_one_gev->Fill(vis_mass, wt);

    if (lep1->pt() > 20. && lep2->pt() > 40.) {
      pair_vis_mass_20_40->Fill(vis_mass, wt);
      if (met.pt() < 20.) pair_vis_mass_20_40_MET_20->Fill(vis_mass, wt);
    }

    if (lep1->pt() > 30. && lep2->pt() > 40.) {
      pair_vis_mass_30_40->Fill(vis_mass, wt);
      if (met.pt() < 20.) pair_vis_mass_30_40_MET_20->Fill(vis_mass, wt);
    }

    if (lep1->pt() > 40. && lep2->pt() > 40.) {
      pair_vis_mass_40_40->Fill(vis_mass, wt);
      if (met.pt() < 20.) pair_vis_mass_40_40_MET_20->Fill(vis_mass, wt);
    }

    Tau const* tau = dynamic_cast<Tau const*>(lep2);
    if (tau) {
      tau_mva_iso_val->Fill(tau->GetTauID("byIsolationMVAraw"), wt);
      if (tau->decay_mode() == 0) {
        pair_vis_mass_mode0->Fill(vis_mass, wt);
        if (fabs(tau->eta()) < 1.5) {
          pair_vis_mass_mode0_EB->Fill(vis_mass, wt);
        } else {
          pair_vis_mass_mode0_EE->Fill(vis_mass, wt);
        }
      }
      if (tau->decay_mode() == 1) {
        pair_vis_mass_mode1->Fill(vis_mass, wt);
        if (fabs(tau->eta()) < 1.5) {
          pair_vis_mass_mode1_EB->Fill(vis_mass, wt);
        } else {
          pair_vis_mass_mode1_EE->Fill(vis_mass, wt);
        }
      }
    }
    lep1_mvamet_mt->Fill(MT(lep1,&met), wt);
    lep2_mvamet_mt->Fill(MT(lep2,&met), wt);
    lep1_mvamet_pzeta_0p85->Fill(PZeta(&dilepton, &met, 0.85), wt);
    lep1_mvamet_pzeta_0p5->Fill(PZeta(&dilepton, &met, 0.5), wt);
    mvamet_et->Fill(met.pt(), wt);
    mvamet_phi->Fill(met.phi(), wt);
    vis_sm->Fill(vis_mass, wt);
    vis_sm_fine->Fill(vis_mass, wt);
    vis_mssm->Fill(vis_mass, wt);
    vis_mssm_fine->Fill(vis_mass, wt);
  }

  void HttPlots::FillSVFitMassModePlots(CompositeCandidate const& dilepton, double const& mass, double wt) {
    Candidate const* lep2 = dilepton.GetCandidate("lepton2");
    Tau const* tau = dynamic_cast<Tau const*>(lep2);
    if (tau) {
      if (tau->decay_mode() == 0)     {
        svfit_mode0->Fill(mass, wt);
        if (fabs(tau->eta()) < 1.5) {
          svfit_mode0_EB->Fill(mass, wt);
        } else {
          svfit_mode0_EE->Fill(mass, wt);
        }
      }
      if (tau->decay_mode() == 1)  {
        svfit_mode1->Fill(mass, wt);
        if (fabs(tau->eta()) < 1.5) {
          svfit_mode1_EB->Fill(mass, wt);
        } else {
          svfit_mode1_EE->Fill(mass, wt);
        }
      }  
    }
  }


  void HttPlots::FillIncMT(CompositeCandidate const& dilepton, Met const& met, double wt) {
    Candidate const* lep1 = dilepton.GetCandidate("lepton1");
    lep1_mvamet_mt_all->Fill(MT(lep1,&met), wt);
  }

  void HttPlots::FillJetPlots(std::vector<PFJet *> jets, double wt) {
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30., 4.7));
    n_jets->Fill(jets.size(), wt);
    if (jets.size() >= 1) {
      lead_jet_pt->Fill(jets[0]->pt(), wt);
      lead_jet_eta->Fill(jets[0]->eta(), wt);
    }
    if (jets.size() >= 2) {
      sublead_jet_pt->Fill(jets[1]->pt(), wt);
      sublead_jet_eta->Fill(jets[1]->eta(), wt);
      dijet_mass->Fill((jets[0]->vector() + jets[1]->vector()).M(), wt);
      dijet_delta_eta->Fill(fabs(jets[0]->eta() - jets[1]->eta()), wt); 
      mjj_DEta->Fill((jets[0]->vector() + jets[1]->vector()).M(), fabs(jets[0]->eta() - jets[1]->eta()), wt);
    } 
  }

  void HttPlots::FillJetPlotsDummy(std::vector<PFJet *> jets, double wt) {
      ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30., 4.7));
      n_jets->Fill(jets.size(), wt);
      lead_jet_pt->Fill(0.0, wt);
      lead_jet_eta->Fill(0.0, wt);
      sublead_jet_pt->Fill(0.0, wt);
      sublead_jet_eta->Fill(0.0, wt);
      dijet_mass->Fill(0.0, wt);
      dijet_delta_eta->Fill(0.0, wt); 
      mjj_DEta->Fill(0.0, wt);
  }


  void HttPlots::FillPt20JetPlots(std::vector<PFJet *> jets, double wt, double btag_weight) {
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 20., 2.4));
    n_pt20jets->Fill(jets.size(), wt);
    if (jets.size() >= 1) {
      lead_pt20jet_pt->Fill(jets[0]->pt(), wt);
      lead_pt20jet_eta->Fill(jets[0]->eta(), wt);
      lead_pt20jet_btag_val->Fill(jets[0]->GetBDiscriminator("combinedSecondaryVertexBJetTags"), wt);
    } 
    if (jets.size() >= 2) {
      sublead_pt20jet_pt->Fill(jets[1]->pt(), wt);
      sublead_pt20jet_eta->Fill(jets[1]->eta(), wt);
      sublead_pt20jet_btag_val->Fill(jets[1]->GetBDiscriminator("combinedSecondaryVertexBJetTags"), wt);
    } 
    ic::erase_if(jets, boost::bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") < 0.679);
    n_bjets->Fill(jets.size(), wt);
    if (jets.size() >= 1) {
      lead_bjet_pt->Fill(jets[0]->pt(), wt * btag_weight);
      lead_bjet_eta->Fill(jets[0]->eta(), wt * btag_weight);
    } 
  }


  void HttPlots::FillPt20JetPlotsDummy(std::vector<PFJet *> jets, double wt) {
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 20., 2.4));
    n_pt20jets->Fill(jets.size(), wt);
    lead_pt20jet_pt->Fill(0.0, wt);
    lead_pt20jet_eta->Fill(0.0, wt);
    lead_pt20jet_btag_val->Fill(0.0, wt);
    sublead_pt20jet_pt->Fill(0.0, wt);
    sublead_pt20jet_eta->Fill(0.0, wt);
    sublead_pt20jet_btag_val->Fill(0.0, wt);
    ic::erase_if(jets, boost::bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") < 0.679);
    n_bjets->Fill(jets.size(), wt);    
    lead_bjet_pt->Fill(0.0, wt);
    lead_bjet_eta->Fill(0.0, wt);
  }

  void HttPlots::FillBEfficiency(std::vector<PFJet *> jets, double wt) {
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 20., 2.4));
    for (unsigned i = 0; i < jets.size(); ++i) {
      int flav = abs(jets[i]->parton_flavour());
      if (flav != 5) continue;
      beff_all->Fill(jets[i]->pt(), wt);
      if (jets[i]->GetBDiscriminator("combinedSecondaryVertexBJetTags") > 0.679) beff_pass->Fill(jets[i]->pt(), wt);
    }
  }

} //namespace
