year=2018

for ch in et mt;
do
  for i in wjets_control nobtag_tightmt nobtag_loosemt btag_tightmt btag_loosemt;
  do
    eval "python scripts/HiggsTauTauPlot.py --cfg=scripts/plot_mssm_${year}.cfg --channel=${ch} --method=17 --var='m_vis[100,110,125,140,160,175,200,225,250,280,320,350,400,450,500,560,630,710,800,890,1000,1200]' --embedding --log_x --singletau --add_wt='wt_tau_trg_mssm*wt_tau_id_mssm*wt_prefire' --ratio --cat=$i --datacard=$i --ggh_masses='' --bbh_nlo_masses=''"
  done

  eval "python scripts/HiggsTauTauPlot.py --cfg=scripts/plot_mssm_${year}.cfg --channel=${ch} --method=17 --var='mt_tot[100,110,125,140,160,175,200,225,250,280,320,350,400,450,500,560,630,710,800,890,1000,1200]' --embedding --log_x --singletau --add_wt='wt_tau_trg_mssm*wt_tau_id_mssm*wt_prefire' --ratio --cat=wjets_control --datacard=wjets_control --ggh_masses='' --bbh_nlo_masses=''"
done

