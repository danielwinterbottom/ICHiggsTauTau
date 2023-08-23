import os
import json
import numpy as np


DO_SS = False
DOTAUDECAY=False


variables = [
      'pt_tt[0,10,20,30,40,60,80,100,120,160,200,280,320,400,600]',
      'm_vis(20,0,200)',
      'eta_1(30,-3,3)',
      'eta_2(30,-3,3)',
     'n_jets(4,0,4)', 
      'pt_1(20,0,200)',
     'pt_2(20,0,200)',
     'mt_1(30,0,300)',
     'mt_2(30,0,300)'
]
if DOTAUDECAY:
  TauModes = ["0","1","10","11"] #10 seems best
else: 
  TauModes =[None]


for v in variables:
  for mode in TauModes:
    v_tt = v
    extra=' --ratio_range=\"0.6,1.4\" '
    extra += ' --v2p5'
    #Factor out Run2 top weight
    extra+=' --add_wt="wt_tquark_down"'
    #extra+= ' --singletau'
    extra+= ' --folder="/vols/cms/eb921/output/output_mt_2022/"'
    if DOTAUDECAY:
      extra+= ' --sel="(mt_1<70 && m_vis>50&&m_vis<80 && tau_decay_mode_2==%s)"'%mode
      try:
          path = 'mt_plots/taumode_%s'%mode
          os.mkdir(path)
          print("Directory '%(path)s' created"% vars())
      except OSError as error:
          print("Directory '%(path)s' can not be created: %(error)s"% vars())
      extra+= ' --outputfolder="%s"'%path
    else:
      extra+= ' --sel="(mt_1<70 && m_vis>50&&m_vis<80)"'
      extra+= ' --outputfolder="mt_plots"'
    os.system('python scripts/HiggsTauTauPlot_Run3.py %(extra)s --cfg scripts/mt_plot_2022.cfg --var=\"%(v_tt)s\"' % vars() )
    if DO_SS:
      os.system('python scripts/HiggsTauTauPlot_Run3.py %(extra)s --do_ss --do_aiso --cfg scripts/mt_plot_2022.cfg --var=\"%(v_tt)s\"' % vars() )            
    