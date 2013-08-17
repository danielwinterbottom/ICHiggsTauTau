#!/bin/sh

# Rename standard control plots produced by the plot_control_2011.sh
# plot_control_2012.sh scripts to those required for an analysis note.
# This script expects all plots to be located under the current working
# directory, and that the folder output/an_plots exists.

YEARS=(
'2011'
'2012'
)

FMTS=(
'pdf'
'png'
)

for i in "${YEARS[@]}"
do
  for j in "${FMTS[@]}"
  do
    mv -v eta_1_inclusive_et_$i.$j output/an_plots/eta_1_eTau_$i.$j
    mv -v eta_1_inclusive_mt_$i.$j output/an_plots/eta_1_muTau_$i.$j
    mv -v eta_2_inclusive_et_$i.$j output/an_plots/eta_2_eTau_$i.$j
    mv -v eta_2_inclusive_mt_$i.$j output/an_plots/eta_2_muTau_$i.$j
    mv -v jdeta_twojet_et_$i.$j output/an_plots/jdeta_eTau_$i.$j
    mv -v jdeta_twojet_mt_$i.$j output/an_plots/jdeta_muTau_$i.$j
    mv -v jeta_1_1jet_et_$i.$j output/an_plots/jeta_1_eTau_$i.$j
    mv -v jeta_1_1jet_mt_$i.$j output/an_plots/jeta_1_muTau_$i.$j
    mv -v jeta_2_twojet_et_$i.$j output/an_plots/jeta_2_eTau_$i.$j
    mv -v jeta_2_twojet_mt_$i.$j output/an_plots/jeta_2_muTau_$i.$j
    mv -v jpt_1_1jet_et_$i.$j output/an_plots/jpt_1_eTau_$i.$j
    mv -v jpt_1_1jet_mt_$i.$j output/an_plots/jpt_1_muTau_$i.$j
    mv -v jpt_2_twojet_et_$i.$j output/an_plots/jpt_2_eTau_$i.$j
    mv -v jpt_2_twojet_mt_$i.$j output/an_plots/jpt_2_muTau_$i.$j
    mv -v m_2_inclusive_et_$i.$j output/an_plots/m_2_eTau_$i.$j
    mv -v m_2_inclusive_mt_$i.$j output/an_plots/m_2_muTau_$i.$j
    mv -v m_sv_inclusive_et_$i.$j output/an_plots/m_sv_eTau_$i.$j
    mv -v m_sv_inclusive_mt_$i.$j output/an_plots/m_sv_muTau_$i.$j
    mv -v m_vis_inclusive_et_$i.$j output/an_plots/mvis_eTau_$i.$j
    mv -v m_vis_inclusive_mt_$i.$j output/an_plots/mvis_muTau_$i.$j
    mv -v m_sv_inclusive_et_"$i"_log.$j output/an_plots/m_sv_eTau_"$i"_log.$j
    mv -v m_sv_inclusive_mt_"$i"_log.$j output/an_plots/m_sv_muTau_"$i"_log.$j
    mv -v m_vis_inclusive_et_"$i"_log.$j output/an_plots/mvis_eTau_"$i"_log.$j
    mv -v m_vis_inclusive_mt_"$i"_log.$j output/an_plots/mvis_muTau_"$i"_log.$j
    mv -v met_inclusive_et_$i.$j output/an_plots/mvamet_eTau_$i.$j
    mv -v met_inclusive_mt_$i.$j output/an_plots/mvamet_muTau_$i.$j
    mv -v mjj_twojet_et_$i.$j output/an_plots/mjj_eTau_$i.$j
    mv -v mjj_twojet_mt_$i.$j output/an_plots/mjj_muTau_$i.$j
    mv -v mt_1_inclusive_et_$i.$j output/an_plots/mt_1_eTau_$i.$j
    mv -v mt_1_inclusive_mt_$i.$j output/an_plots/mt_1_muTau_$i.$j
    mv -v n_bjets_inclusive_et_"$i"_log.$j output/an_plots/nbtags_eTau_"$i"_log.$j
    mv -v n_bjets_inclusive_mt_"$i"_log.$j output/an_plots/nbtags_muTau_"$i"_log.$j
    mv -v n_jets_inclusive_et_"$i"_log.$j output/an_plots/njets_eTau_"$i"_log.$j
    mv -v n_jets_inclusive_mt_"$i"_log.$j output/an_plots/njets_muTau_"$i"_log.$j
    mv -v n_vtx_inclusive_et_$i.$j output/an_plots/nvtx_eTau_$i.$j
    mv -v n_vtx_inclusive_mt_$i.$j output/an_plots/nvtx_muTau_$i.$j
    mv -v pt_1_inclusive_et_$i.$j output/an_plots/pt_1_eTau_$i.$j
    mv -v pt_1_inclusive_mt_$i.$j output/an_plots/pt_1_muTau_$i.$j
    mv -v pt_2_inclusive_et_$i.$j output/an_plots/pt_2_eTau_$i.$j
    mv -v pt_2_inclusive_mt_$i.$j output/an_plots/pt_2_muTau_$i.$j
    mv -v pt_tt_inclusive_et_$i.$j output/an_plots/pt_tt_eTau_$i.$j
    mv -v pt_tt_inclusive_mt_$i.$j output/an_plots/pt_tt_muTau_$i.$j
    mv -v pt_tt_inclusive_et_"$i"_log.$j output/an_plots/pt_tt_eTau_"$i"_log.$j
    mv -v pt_tt_inclusive_mt_"$i"_log.$j output/an_plots/pt_tt_muTau_"$i"_log.$j
 
    
    mv -v eta_1_inclusive_em_$i.$j output/an_plots/eta_1_emu_$i.$j
    mv -v eta_2_inclusive_em_$i.$j output/an_plots/eta_2_emu_$i.$j
    mv -v jdeta_twojet_em_$i.$j output/an_plots/jdeta_emu_$i.$j
    mv -v jeta_1_1jet_em_$i.$j output/an_plots/jeta_1_emu_$i.$j
    mv -v jeta_2_twojet_em_$i.$j output/an_plots/jeta_2_emu_$i.$j
    mv -v jpt_1_1jet_em_$i.$j output/an_plots/jpt_1_emu_$i.$j
    mv -v jpt_2_twojet_em_$i.$j output/an_plots/jpt_2_emu_$i.$j
    mv -v m_sv_inclusive_em_$i.$j output/an_plots/m_sv_emu_$i.$j
    mv -v m_vis_inclusive_em_$i.$j output/an_plots/mvis_emu_$i.$j
    mv -v m_sv_inclusive_em_"$i"_log.$j output/an_plots/m_sv_emu_"$i"_log.$j
    mv -v m_vis_inclusive_em_"$i"_log.$j output/an_plots/mvis_emu_"$i"_log.$j
    mv -v met_inclusive_em_$i.$j output/an_plots/mvamet_emu_$i.$j
    mv -v mjj_twojet_em_$i.$j output/an_plots/mjj_emu_$i.$j
    mv -v n_bjets_inclusive_em_"$i"_log.$j output/an_plots/nbtags_emu_"$i"_log.$j
    mv -v n_jets_inclusive_em_"$i"_log.$j output/an_plots/njets_emu_"$i"_log.$j
    mv -v n_vtx_inclusive_em_$i.$j output/an_plots/nvtx_emu_$i.$j
    mv -v pt_1_inclusive_em_$i.$j output/an_plots/pt_1_emu_$i.$j
    mv -v pt_2_inclusive_em_$i.$j output/an_plots/pt_2_emu_$i.$j
    mv -v pt_tt_inclusive_em_$i.$j output/an_plots/pt_tt_emu_$i.$j
    mv -v pt_tt_inclusive_em_"$i"_log.$j output/an_plots/pt_tt_emu_"$i"_log.$j
    
    mv -v em_gf_mva_inclusive_em_"$i".$j output/an_plots/mva_emu_"$i".$j
    mv -v em_gf_mva_inclusive_em_"$i"_log.$j output/an_plots/mva_emu_"$i"_log.$j
    mv -v emu_dphi_inclusive_em_"$i".$j output/an_plots/dphi_emu_"$i".$j
    mv -v emu_dxy_1_inclusive_em_"$i"_log.$j output/an_plots/d0_1_emu_"$i"_log.$j
    mv -v mt_ll_inclusive_em_"$i".$j output/an_plots/mt_ll_emu_"$i".$j
    mv -v pzeta_inclusive_em_"$i".$j output/an_plots/pzeta_emu_"$i".$j
    mv -v pzetavis_inclusive_em_"$i".$j output/an_plots/pzetavis_emu_"$i".$j
    mv -v pzetamiss_inclusive_em_"$i".$j output/an_plots/pzetamiss_emu_"$i".$j

    mv -v tau_decay_mode_inclusive_et_$i.$j output/an_plots/tau_decay_mode_eTau_$i.$j
    mv -v tau_decay_mode_inclusive_mt_$i.$j output/an_plots/tau_decay_mode_muTau_$i.$j
    
    mv -v bcsv_1_prebtag_em_$i.$j output/an_plots/bcsv_emu_$i.$j
    mv -v bcsv_1_prebtag_et_$i.$j output/an_plots/bcsv_eTau_$i.$j
    mv -v bcsv_1_prebtag_mt_$i.$j output/an_plots/bcsv_muTau_$i.$j
    mv -v bpt_1_btag_em_$i.$j output/an_plots/bpt_emu_$i.$j
    mv -v beta_1_btag_em_$i.$j output/an_plots/beta_1_emu_$i.$j
    mv -v bpt_1_btag_mt_$i.$j output/an_plots/bpt_eTau_$i.$j
    mv -v beta_1_btag_mt_$i.$j output/an_plots/beta_1_eTau_$i.$j
    mv -v bpt_1_btag_et_$i.$j output/an_plots/bpt_muTau_$i.$j
    mv -v beta_1_btag_et_$i.$j output/an_plots/beta_1_muTau_$i.$j
  done
done

