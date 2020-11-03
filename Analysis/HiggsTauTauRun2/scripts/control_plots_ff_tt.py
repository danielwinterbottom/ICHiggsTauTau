import os

#years = ['2016','2017']#,'2018']
years = ['2016']#,'2018']

variables = [
  'met[0,10,20,30,40,50,60,70,80,90,100,120,140,200,400]',
  'n_jets(4,0,4)', 
  'n_prebjets(4,0,4)', 
  'n_deepbjets(4,0,4)', 
  'pt_1[40,45,50,55,60,65,70,80,90,100,120,140,200,400]',
  'pt_2[40,45,50,55,60,65,70,80,90,100,120,140,200,400]',
  'mt_1(20,0,200)',
  'mt_2(20,0,200)',
  'mt_lep(20,0,200)',
  'm_vis(50,0,500)',
  'mt_tot[80,100,120,140,160,180,200,250,300,350,400,500,700,3900]'
]


for y in years:
  for v in variables:

    yp = y

    v_tt = v

    extra=' --norm_bins '

    if 'mt_tot' in v: extra='--log_y --log_x'

    extra+=' --ratio_range=\"0.6,1.4\" '
 
    os.system('python scripts/HiggsTauTauPlot.py --cfg=scripts/plot_mssm_%(yp)s.cfg --method=17 --channel=tt --var=\"%(v_tt)s\"   --outputfolder=ff_control/ %(extra)s --do_ss ' % vars())

