import os

variables = [

  #'n_jets(4,0,4)' 
  # 'pt_1(20,0,200)',
  # 'pt_2(20,0,200)',
   #'mt_1(30,0,300)'
  # 'mt_2(30,0,300)',
  # 'm_vis(20,0,200)',
  # 'pt_tt(20,0,200)'
]

selnames = ["medium_vloose_medium",
            "medium_vvvloose_medium",
            "vloose_vvvloose_medium",
            "medium_vloose_medium_vloose",
            "medium_vvvloose_medium_vvvloose",
            "vloose_vvvloose_vloose_vvvloose"]

for v in variables:
  v_tt = v
  for sel in range(0,6):
    extra=' --ratio_range=\"0.6,1.4\" '
    extra+= '--extra_name=%s'%selnames[sel]
    extra += ' --tt_qcd_sel_num=%i'%sel

  #if 'mt_tot' in v: extra+='--log_y --log_x'
  

  # add systematic bands
  #extra += ' --do_custom_uncerts --add_stat_to_syst --syst_tau_trg_diff="trg_syst_*" --syst_tau_id_diff="id_syst_*"  --do_ff_systs --syst_tquark="syst_ttbar_pt"  --syst_embedding_tt="syst_embed_tt" ' #--syst_tau_scale_grouped="tes_*group" '
    os.system('python scripts/HiggsTauTauPlot.py %(extra)s --cfg scripts/plot_UL_2018_nosubqcd.cfg --no_qcd_subtract --var=\"%(v_tt)s\"' % vars ())
    os.system('python scripts/HiggsTauTauPlot.py %(extra)s --cfg scripts/plot_UL_2018.cfg --var=\"%(v_tt)s\"' % vars() )
    os.system('python scripts/HiggsTauTauPlot.py %(extra)s --do_ss --cfg scripts/plot_UL_2018_nosubqcd.cfg --no_qcd_subtract --var=\"%(v_tt)  s\"' % vars())
    os.system('python scripts/HiggsTauTauPlot.py %(extra)s --do_ss --cfg scripts/plot_UL_2018.cfg --var=\"%(v_tt)s\"' % vars() )
  
  #os.system('python scripts/HiggsTauTauPlot.py --cfg=scripts/plot_UL_%(yp)s.cfg --var=\"%(v_tt)s\"   --outputfolder=controlOutput/ %(extra)s --embedding ' % vars())
  #os.system('python scripts/HiggsTauTauPlot.py --cfg=scripts/plot_UL_%(yp)s.cfg --var=\"%(v_tt)s\"   --outputfolder=controlOutput/ %(extra)s --datacard=mc ' % vars())

