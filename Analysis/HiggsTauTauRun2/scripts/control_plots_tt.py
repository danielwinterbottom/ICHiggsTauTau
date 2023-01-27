import os

years = ['2018','2017','2016']
#years = ['2017']

variables = [

  'met[0,10,20,30,40,50,60,70,80,90,100,120,140,200,400]',
  'n_jets(4,0,4)', 
  'pt_1[40,45,50,55,60,65,70,80,90,100,120,140,200,400]',
  'pt_2[40,45,50,55,60,65,70,80,90,100,120,140,200,400]',
  'mt_1(30,0,300)',
  'mt_2(30,0,300)',
  'mt_lep(30,0,300)',
  'm_vis(50,0,500)',
  'met_dphi_1(20,0,3.1415927)',
  'met_dphi_2(20,0,3.1415927)',
  'mt_tot[50,60,70,80,100,120,140,160,180,200,250,300,350,400,500,700,900,3900]', 
  'dphi(20,0,3.1415927)',
  'n_deepbjets(4,0,4)', 
]


for y in years:
  for v in variables:

    yp = y

    v_tt = v


    if y != '2018':
      extra=' --norm_bins --extra_pad=0.4 --singletau --add_wt=\'wt_tau_trg_mssm*wt_tau_id_mssm*wt_prefire\' '
    else:
      extra=' --norm_bins --extra_pad=0.4 --singletau --add_wt=\'wt_tau_trg_mssm*wt_tau_id_mssm\' '
 
    if 'mt_tot' in v: extra='--log_y --log_x'

    extra+=' --ratio_range=\"0.6,1.4\" '
 
    # add systematic bands
    extra += ' --do_custom_uncerts --add_stat_to_syst --syst_tau_trg_diff="trg_syst_*" --syst_tau_id_diff="id_syst_*"  --do_ff_systs --syst_tquark="syst_ttbar_pt"  --syst_embedding_tt="syst_embed_tt" ' #--syst_tau_scale_grouped="tes_*group" '

    os.system('python scripts/HiggsTauTauPlot.py --cfg=scripts/plot_mssm_%(yp)s.cfg --method=17 --channel=tt --var=\"%(v_tt)s\"   --outputfolder=mssm_control/ %(extra)s --embedding ' % vars())
    os.system('python scripts/HiggsTauTauPlot.py --cfg=scripts/plot_mssm_%(yp)s.cfg --method=17 --channel=tt --var=\"%(v_tt)s\"   --outputfolder=mssm_control/ %(extra)s --datacard=mc ' % vars())

