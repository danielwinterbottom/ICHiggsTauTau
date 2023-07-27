import os
import json

variables = [

   'n_jets(4,0,4)', 
    'pt_1(20,0,200)',
   'pt_2(20,0,200)',
    'mt_1(30,0,300)',
    'mt_2(30,0,300)',
   'pt_tt(20,0,200)',
  'm_vis(20,0,200)'
]

#Selections in the control region (QCD)
selnames = [
            ["medium_vloose_medium","deepTauVsJets_medium_1<0.5 && deepTauVsJets_vloose_1>0.5 && deepTauVsJets_medium_2>0.5"],
            # ["medium_vvvloose_medium","deepTauVsJets_medium_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_medium_2>0.5"],
            # ["vloose_vvvloose_medium","deepTauVsJets_vloose_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_medium_2>0.5"],
            ["medium_vloose_medium_vloose","deepTauVsJets_medium_1<0.5 && deepTauVsJets_vloose_1>0.5 && deepTauVsJets_medium_2<0.5 && deepTauVsJets_vloose_2>0.5"],
            ["medium_vvvloose_medium_vvvloose","deepTauVsJets_medium_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_medium_2<0.5 && deepTauVsJets_vvvloose_2>0.5"],
            # ["vloose_vvvloose_vloose_vvvloose","deepTauVsJets_vloose_1<0.5 && deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_vloose_2<0.5 && deepTauVsJets_vvvloose_2>0.5"],
            ]

DO_SS = True #produce same-sign files
years = ["UL_2018"]#"UL_2018","2022"
triggernames_2022 = [["trg_doubletau && pt_1>40 && pt_2>40","trg_doubletau"],["(trg_doubletau && pt_1>40 && pt_2>40) || ((trg_doubletau_plusjet60 || trg_doubletau_plusjet75) && trg60jet_pt>65)","trg_both"]]#pt>35 base cut
versions = ["v2p5"] #"v2p1","v2p5"


json_name= 'input/tau_SF_strings_dm_binned_mediumvsjet_vvloosevsele_split_fit.json'
with open(json_name) as json_file:
  sf_data = json.load(json_file)
sf=sf_data['mediumvsjet_vvloosevsele']['2018']
sf+='*%s' % sf.replace('_2','_1')

for yr in years:
  for v in variables:
    v_tt = v
    for sel in selnames:
      if yr == "UL_2018":
        triggernames = [["trg_doubletau && pt_1>40 && pt_2>40","trg_doubletau"]]
        add_cond = '--add_wt=\'%(sf)s*1/wt_tau_id_dm\'' % vars()#'--add_wt=\'wt_prefire*%(sf)s/wt_tau_id_dm*%(trg_sf)s\'' % vars()
      else:
        triggernames = triggernames_2022
        add_cond = ''
        
      for trigname in triggernames:
        for version in versions:
          extra=' --ratio_range=\"0.6,1.4\" '  
          if v[0] != 'm':   
            extra += ' --set_alias=baseline:"(deepTauVsJets_medium_1>0.5 && deepTauVsJets_medium_2>0.5 && leptonveto==0 && (%s) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2 && m_vis<100)"'%trigname[0]
            
            extra += ' --set_alias=tt_qcd_norm:"(%s && leptonveto==0 && (%s) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2 && m_vis<100)"'%(sel[1],trigname[0])
          else:
            extra += ' --set_alias=baseline:"(deepTauVsJets_medium_1>0.5 && deepTauVsJets_medium_2>0.5 && leptonveto==0 && (%s) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2)"'%trigname[0]
          
            extra += ' --set_alias=tt_qcd_norm:"(%s && leptonveto==0 && (%s) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2)"'%(sel[1],trigname[0])
          
          if version =="v2p5":
            extra += ' --v2p5'
            extra+= ' --extra_name=%s_%s_v2p5'%(sel[0],trigname[1])
            os.system('python scripts/HiggsTauTauPlot_Run3.py %(extra)s %(add_cond)s --cfg scripts/plot_%(yr)s.cfg --var=\"%(v_tt)s\"' % vars() )
            if DO_SS:
              os.system('python scripts/HiggsTauTauPlot_Run3.py %(extra)s %(add_cond)s --do_ss --cfg scripts/plot_%(yr)s.cfg --var=\"%(v_tt)s\"' % vars() )            
          elif yr=="UL_2018":
            extra+= ' --extra_name=%s_%s_v2p1'%(sel[0],trigname[1])
            os.system('python scripts/HiggsTauTauPlot_Run3.py %(extra)s %(add_cond)s --cfg scripts/plot_%(yr)s.cfg --var=\"%(v_tt)s\"' % vars() )
            if DO_SS:
              os.system('python scripts/HiggsTauTauPlot_Run3.py %(extra)s %(add_cond)s --do_ss --cfg scripts/plot_%(yr)s.cfg --var=\"%(v_tt)s\"' % vars() ) 
    

