import os
import json
import numpy as np

DO_ZPT_CONTROL = False
DO_CONTROL = False
DO_SS = False

FOLDER = "/vols/cms/ia2318/outputs/MSSM/2022-preEE_031023_zmm"
#ZPTFOLDER = "/vols/cms/eb921/test_output/output_zmm_Zpt_2022/"

if DO_ZPT_CONTROL or DO_CONTROL:
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
else:
  variables = [
    'pt_tt[0,10,20,30,40,60,80,100,120,160,200,280,320,400,600]',
  ]
first_column = np.concatenate((np.arange(50, 91,10),np.arange(100, 181,20),[200,300,400,600,800]),axis=None)
second_column = np.concatenate((np.arange(60, 101,10),np.arange(120, 201,20),[300,400,600,800,"inf"]),axis=None)
if DO_ZPT_CONTROL or DO_CONTROL:
  mass_selections = [None]
else:
  mass_selections = np.column_stack([first_column, second_column])



for v in variables:
  v_tt = v
  for sel in mass_selections:
    extra=' --ratio_range=\"0.6,1.4\" '
    extra += ' --v2p5'
    #Factor out wt_dysoup if it is wrongly not 1
    #extra+=' --add_wt="1/wt_dysoup"'
    if DO_ZPT_CONTROL:
      extra+= ' --folder="%s"'%ZPTFOLDER
      extra+= ' --extra_name=mvis50_Zpt'
      extra+= ' --sel="(m_vis>50)"'
    else:
      extra+= ' --folder="%s"'%FOLDER
      if DO_CONTROL:
        extra+= ' --extra_name=mvis50'
        extra+= ' --sel="(m_vis>50)"'
      else:
        extra+= ' --extra_name=mvis%sto%s'%(sel[0],sel[1])
        if sel[1] == "inf":
          extra += ' --sel="(m_vis>%s)"'%(sel[0])
        else:
          extra += ' --sel="(m_vis>%s && m_vis<%s)"'%(sel[0], sel[1])
    os.system('python scripts/HiggsTauTauPlot_Run3.py %(extra)s --cfg scripts/zpt_plot_2022.cfg --var=\"%(v_tt)s\"' % vars() )
    if DO_SS:
      os.system('python scripts/HiggsTauTauPlot_Run3.py %(extra)s --do_ss --cfg scripts/zpt_plot_2022.cfg --var=\"%(v_tt)s\"' % vars() )            
    