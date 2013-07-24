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
    cp -v eta_1_inclusive_et_$i.$j output/an_plots/eta_1_eTau_$i.$j
    cp -v eta_1_inclusive_mt_$i.$j output/an_plots/eta_1_muTau_$i.$j
    cp -v eta_2_inclusive_et_$i.$j output/an_plots/eta_2_eTau_$i.$j
    cp -v eta_2_inclusive_mt_$i.$j output/an_plots/eta_2muTau_$i.$j
    cp -v jdeta_twojet_et_$i.$j output/an_plots/jdeta_eTau_$i.$j
    cp -v jdeta_twojet_mt_$i.$j output/an_plots/jdeta_muTau_$i.$j
    cp -v jeta_1_1jet_et_$i.$j output/an_plots/jeta_1_eTau_$i.$j
    cp -v jeta_1_1jet_mt_$i.$j output/an_plots/jeta_1_muTau_$i.$j
    cp -v jeta_2_twojet_et_$i.$j output/an_plots/jeta_2_eTau_$i.$j
    cp -v jeta_2_twojet_mt_$i.$j output/an_plots/jeta_2_muTau_$i.$j
    cp -v jpt_1_1jet_et_$i.$j output/an_plots/jpt_1_eTau_$i.$j
    cp -v jpt_1_1jet_mt_$i.$j output/an_plots/jpt_1_muTau_$i.$j
    cp -v jpt_2_twojet_et_$i.$j output/an_plots/jpt_2_eTau_$i.$j
    cp -v jpt_2_twojet_mt_$i.$j output/an_plots/jpt_2_muTau_$i.$j
    cp -v m_2_inclusive_et_$i.$j output/an_plots/m_2_eTau_$i.$j
    cp -v m_2_inclusive_mt_$i.$j output/an_plots/m_2_muTau_$i.$j
    cp -v m_sv_inclusive_et_$i.$j output/an_plots/m_sv_eTau_$i.$j
    cp -v m_sv_inclusive_mt_$i.$j output/an_plots/m_sv_muTau_$i.$j
    cp -v m_vis_inclusive_et_$i.$j output/an_plots/mvis_eTau_$i.$j
    cp -v m_vis_inclusive_mt_$i.$j output/an_plots/mvis_muTau_$i.$j
    cp -v met_inclusive_et_$i.$j output/an_plots/mvamet_eTau_$i.$j
    cp -v met_inclusive_mt_$i.$j output/an_plots/mvamet_muTau_$i.$j
    cp -v mjj_twojet_et_$i.$j output/an_plots/mjj_eTau_$i.$j
    cp -v mjj_twojet_mt_$i.$j output/an_plots/mjj_muTau_$i.$j
    cp -v mt_1_inclusive_et_$i.$j output/an_plots/mt_1_eTau_$i.$j
    cp -v mt_1_inclusive_mt_$i.$j output/an_plots/mt_1_muTau_$i.$j
    cp -v n_bjets_inclusive_et_"$i"_log.$j output/an_plots/nbtags_eTau_"$i"_log.$j
    cp -v n_bjets_inclusive_mt_"$i"_log.$j output/an_plots/nbtags_muTau_"$i"_log.$j
    cp -v n_jets_inclusive_et_"$i"_log.$j output/an_plots/njets_eTau_"$i"_log.$j
    cp -v n_jets_inclusive_mt_"$i"_log.$j output/an_plots/njets_muTau_"$i"_log.$j
    cp -v n_vtx_inclusive_et_$i.$j output/an_plots/nvtx_eTau_$i.$j
    cp -v n_vtx_inclusive_mt_$i.$j output/an_plots/nvtx_muTau_$i.$j
    cp -v pt_1_inclusive_et_$i.$j output/an_plots/pt_1_eTau_$i.$j
    cp -v pt_1_inclusive_mt_$i.$j output/an_plots/pt_1_muTau_$i.$j
    cp -v pt_2_inclusive_et_$i.$j output/an_plots/pt_2_eTau_$i.$j
    cp -v pt_2_inclusive_mt_$i.$j output/an_plots/pt_2_muTau_$i.$j
    cp -v pt_tt_inclusive_et_$i.$j output/an_plots/pt_tt_eTau_$i.$j
    cp -v pt_tt_inclusive_mt_$i.$j output/an_plots/pt_tt_muTau_$i.$j
    cp -v pt_tt_inclusive_et_"$i"_log.$j output/an_plots/pt_tt_eTau_"$i"_log.$j
    cp -v pt_tt_inclusive_mt_"$i"_log.$j output/an_plots/pt_tt_muTau_"$i"_log.$j
#    cp -v tau_decay_mode_inclusive_et_$i.$j output/an_plots/tau_decay_mode_eTau_$i.$j
#    cp -v tau_decay_mode_inclusive_mt_$i.$j output/an_plots/tau_decay_mode_muTau_$i.$j
  done
done

