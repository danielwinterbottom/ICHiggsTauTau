import os

#years = ['2016','2017']#,'2018']
years = ['2017']#,'2018']

chan = 'mt'

variables = [
  'met[0,10,20,30,40,50,60,70,80,90,100,120,140,200,400]',
  #'n_jets(4,0,4)', 
  #'n_prebjets(4,0,4)', 
  'n_deepbjets(4,0,4)', 
  'pt_1[20,25,30,35,40,45,50,55,60,65,70,80,90,100,120,140,200,400]',
  'pt_2[30,35,40,45,50,55,60,65,70,80,90,100,120,140,200,400]',
  'mt_1(20,0,200)',
  'mt_2(20,0,200)',
  'mt_lep(20,0,200)',
  'm_vis(30,0,300)',
  'mt_tot[100,110,125,140,160,175,200,225,250,280,320,350,400,450,500,560,630,710,800,890,1000]'
]

variables_qcd = [
  'met[0,10,20,30,40,50,60,70,80,90,100,120,140,200,400]',
  #'n_jets(4,0,4)', 
  #'n_prebjets(4,0,4)', 
  'n_deepbjets(4,0,4)', 
  'pt_1[20,25,30,35,40,45,50,55,60,65,70,80,90,100,120,140,200,400]',
  'pt_2[30,35,40,45,50,55,60,65,70,80,90,100,120,140,200,400]',
  'mt_1(20,0,200)',
  'mt_2(20,0,200)',
  'mt_lep(20,0,200)',
  'm_vis(30,0,300)',
  'mt_tot[20,30,40,50,60,70,80,90,100,110,120,140,160,200,250,300,350,400,500,600,800,1000]'
]

for y in years:
  for i, v in enumerate(variables):

    year = y

    extra=' --norm_bins '

    v_qcd = variables_qcd[i]
    if 'mt_tot' in v: extra='--log_x'

    extra+=' --ratio_range=\"0.6,1.4\" '

    sub1='python scripts/HiggsTauTauPlotW_2.py --channel %(chan)s --cfg scripts/plot_mssm_2017.cfg --var=\"%(v)s\"  --sel=\"mt_1>70\" --method 17 --folder /vols/cms/dw515/Offline/output/MSSM/ff_%(year)s_test/  --analysis=cpdecay --outputfolder ff_closures/%(year)s %(extra)s --norm_bins' % vars()
    sub2='python scripts/HiggsTauTauPlotQCD_2.py --channel %(chan)s --cfg scripts/plot_mssm_2017.cfg --var=\"%(v_qcd)s\"  --sel=\"mt_1<50\" --method 17 --folder /vols/cms/dw515/Offline/output/MSSM/ff_%(year)s_test/  --analysis=cpdecay --outputfolder ff_closures/%(year)s %(extra)s --norm_bins --do_ss' % vars()

    os.system(sub1)
    os.system(sub2)
 

