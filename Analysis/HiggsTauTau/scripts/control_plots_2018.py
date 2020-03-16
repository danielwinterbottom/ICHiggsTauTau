import os

#years = ['2016','2017','2018']
years = ['2018']

variables = [
  #'dR(20,0,5)',
  #'rho(20,0,50)',
  #'met_dphi_1(20,0,3.2)',
  #'met_dphi_2(20,0,3.2)',
  #'dphi(20,0,3.2)',
  'met(20,0,100)',
  'n_jets(4,0,4)', 
  'pt_1(16,20,100)',
  'pt_2(16,20,100)',
  'pt_tt(20,0,200)',
  'pt_vis(15,0,150)',
  'm_vis(20,0,200)',
  'mva_dm_1(12,0,12)',
  'mva_dm_2(12,0,12)',
  'mjj(20,0,500)',
  'jdeta(20,0,5)',
  "dijetpt(20,0,200)",
  'mt_1(10,0,50)',
  'jpt_1(16,20,100)',
  'jpt_2(16,20,100)',
]


for y in years:
  for v in variables:

    yp = y
    if y == '2016': yp='leg2016'

    if 'pt_1' in v: v_tt = 'pt_1(22,20,150)'
    if 'm_vis' in v: v_tt = 'm_vis(30,0,300)'
    else: v_tt = v

    extra='--set_alias=\"inclusive:(1)\"'
    if 'jpt_1' in v: extra='--set_alias=\"inclusive:(n_jets>0)\"'
    if 'jpt_2' in v or 'mjj' in v or 'jdeta' in v  or 'dijetpt' in v: extra='--set_alias=\"inclusive:(n_jets>1)\"'

    extra+=' --ratio_range=\"0.6,1.4\" '

    extra+=' --add_wt=\"wt_tau_trg_mvadm*((wt_tau_id_mvadm<=0)+(wt_tau_id_mvadm>0)*wt_tau_id_mvadm)\"'  

    print 'python scripts/HiggsTauTauPlot.py --cfg=scripts/plot_ff_%(yp)s.cfg --method=17 --channel=tt --var=\"%(v_tt)s\" --sm_masses=\"\" --draw_signal_mass=\"\" --outputfolder=controlnew %(extra)s --embedding --datacard=\"new_sf_embed\" ' % vars()
 
    os.system('python scripts/HiggsTauTauPlot.py --cfg=scripts/plot_ff_%(yp)s.cfg --method=17 --channel=tt --var=\"%(v_tt)s\" --sm_masses=\"\" --draw_signal_mass=\"\" --outputfolder=controlnew %(extra)s --embedding --datacard=\"new_sf_embed\" ' % vars())
    os.system('python scripts/HiggsTauTauPlot.py --cfg=scripts/plot_ff_%(yp)s.cfg --method=17 --channel=tt --var=\"%(v_tt)s\" --sm_masses=\"\" --draw_signal_mass=\"\" --outputfolder=controlnew %(extra)s --datacard=\"new_sf_mc\" ' % vars())


    extra='--set_alias=\"inclusive:(n_bjets==0)\"'
    if 'jpt_1' in v: extra='--set_alias=\"inclusive:(n_bjets==0&&n_jets>0)\"'
    if 'jpt_2' in v or 'mjj' in v or 'jdeta' in v or 'dijetpt' in v: extra='--set_alias=\"inclusive:(n_bjets==0&&n_jets>1)\"'

    extra+=' --ratio_range=\"0.6,1.4\" '

    extra+=' --add_wt=\"wt_btag*wt_tau_trg_mvadm*((wt_tau_id_mvadm<=0)+(wt_tau_id_mvadm>0)*wt_tau_id_mvadm)\"'   

    os.system('python scripts/HiggsTauTauPlot.py --cfg=scripts/plot_ff_%(yp)s.cfg --method=17 --channel=mt --var=\"%(v)s\"  --sm_masses=\"\" --draw_signal_mass=\"\"  --sel=\"mt_1<50\" --outputfolder=controlnew %(extra)s --embedding --datacard=\"new_sf_embed\" ' % vars()) 
    print 'python scripts/HiggsTauTauPlot.py --cfg=scripts/plot_ff_%(yp)s.cfg --method=17 --channel=mt --var=\"%(v)s\"  --sm_masses=\"\" --draw_signal_mass=\"\"  --sel=\"mt_1<50\" --outputfolder=controlnew %(extra)s --embedding --datacard=\"new_sf_embed\" ' % vars()
    os.system('python scripts/HiggsTauTauPlot.py --cfg=scripts/plot_ff_%(yp)s.cfg --method=17 --channel=mt --var=\"%(v)s\"  --sm_masses=\"\" --draw_signal_mass=\"\"  --sel=\"mt_1<50\" --outputfolder=controlnew %(extra)s --datacard=\"new_sf_mc\" ' % vars()) 


#    os.system('python scripts/HiggsTauTauPlotQCD_2.py --cfg=scripts/plot_ff_%(yp)s.cfg --method=17 --channel=mt --var=\"%(v)s\" --do_ss --sm_masses=\"\" --draw_signal_mass=\"\" --set_alias=\"baseline:({baseline}&&iso_1>0.05)\" --sel=\"mt_1<50\" --outputfolder=ff_closure_decays %(extra)s' % vars())

#    os.system('python scripts/HiggsTauTauPlotW_2.py --cfg=scripts/plot_ff_%(yp)s.cfg --method=17 --channel=et --var=\"%(v)s\" --sm_masses=\"\" --draw_signal_mass=\"\"  --sel=\"mt_1>70\" --outputfolder=ff_closure_decays %(extra)s' % vars())

#    os.system('python scripts/HiggsTauTauPlotQCD_2.py --cfg=scripts/plot_ff_%(yp)s.cfg --method=17 --channel=et --var=\"%(v)s\"  --do_ss --sm_masses=\"\" --draw_signal_mass=\"\" --set_alias=\"baseline:({baseline}&&iso_1>0.05)\" --sel=\"mt_1<50\" --outputfolder=ff_closure_decays %(extra)s' % vars())
