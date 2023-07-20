import os

variables = [

  # 'n_jets(4,0,4)', 
  #  'pt_1(20,0,200)',
  #  'pt_2(20,0,200)',
  #  'mt_1(30,0,300)',
  #  'mt_2(30,0,300)',
  # 'pt_tt(20,0,200)',
   'm_vis(20,0,200)'
]

#Selections in the control region (QCD)
selnames = [
            ["medium_vloose_medium","deepTauVsJets_medium_1<0.5 && deepTauVsJets_vloose_1>0.5 && deepTauVsJets_medium_2>0.5"],
            ["medium_vvvloose_medium","deepTauVsJets_medium_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_medium_2>0.5"],
            ["vloose_vvvloose_medium","deepTauVsJets_vloose_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_medium_2>0.5"],
            ["medium_vloose_medium_vloose","deepTauVsJets_medium_1<0.5 && deepTauVsJets_vloose_1>0.5 && deepTauVsJets_medium_2<0.5 && deepTauVsJets_vloose_2>0.5"],
            ["medium_vvvloose_medium_vvvloose","deepTauVsJets_medium_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_medium_2<0.5 && deepTauVsJets_vvvloose_2>0.5"],
            ["vloose_vvvloose_vloose_vvvloose","deepTauVsJets_vloose_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_vloose_2<0.5 && deepTauVsJets_vvvloose_2>0.5"],
            ]

DO_SS = True #produce same-sign files
years = ["UL_2018"]#"UL_2018","2022"
triggernames_2022 = ["trg_doubletau","trg_doubletau_plusjet60","trg_doubletau_plusjet75"]

for yr in years:
  for v in variables:
    v_tt = v
    for sel in selnames:
      extra=' --ratio_range=\"0.6,1.4\" '
      #extra += ' --tt_qcd_sel_num=%i'%sel
      if yr == "UL_2018":
        triggernames = ["trg_doubletau"]
      else:
        triggernames = triggernames_2022
        
      for trigname in triggernames:
        extra+= ' --extra_name=%s_%s'%(sel[0],trigname)   
        if v[0] != 'm':   
          extra += ' --set_alias=baseline:"(deepTauVsJets_medium_1>0.5 && deepTauVsJets_medium_2>0.5 && leptonveto==0 && (%s && pt_2>40) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2 && m_vis<100)"'%trigname
        else:
          extra += ' --set_alias=baseline:"(deepTauVsJets_medium_1>0.5 && deepTauVsJets_medium_2>0.5 && leptonveto==0 && (%s && pt_2>40) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2)"'%trigname
        
        extra += ' --set_alias=tt_qcd_norm:"(%s && leptonveto==0 && (%s && pt_2>40) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2)"'%(sel[1],trigname)
        
        os.system('python scripts/HiggsTauTauPlot.py %(extra)s --cfg scripts/plot_%(yr)s.cfg --var=\"%(v_tt)s\"' % vars() )
        if DO_SS:
          os.system('python scripts/HiggsTauTauPlot.py %(extra)s --do_ss --cfg scripts/plot_%(yr)s.cfg --var=\"%(v_tt)s\"' % vars() )
    

