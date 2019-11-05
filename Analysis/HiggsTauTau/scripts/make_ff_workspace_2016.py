#!/usr/bin/env python
import ROOT
import imp
import json
from array import array
import numpy as np
import subprocess
import re

cmssw_base = subprocess.check_output('echo $CMSSW_BASE', shell=True).replace('\n','')

wsptools = imp.load_source('wsptools', '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/workspaceTools.py' % vars())

def GetFromTFile(str):
    f = ROOT.TFile(str.split(':')[0])
    obj = f.Get(str.split(':')[1]).Clone()
    f.Close()
    return obj

# Boilerplate
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.RooWorkspace.imp = getattr(ROOT.RooWorkspace, 'import')
ROOT.TH1.AddDirectory(0)

w = ROOT.RooWorkspace('w')

################################################
### tt channel ####
################################################

ip_sig_cut = '1'
wps = ['tight','medium','vtight']

for wp in wps:

  # get fractions

  loc = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/mvadm_ff_deeptauV2p1_2016/' % vars()
 
  histsToWrap = [(loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets0_os_qcd' % vars(), 'tt_%(wp)s_fracs_njets0_os_qcd' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets1_os_qcd' % vars(), 'tt_%(wp)s_fracs_njets1_os_qcd' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets2_os_qcd' % vars(), 'tt_%(wp)s_fracs_njets2_os_qcd' % vars()), 
                 (loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets0_ss_qcd' % vars(), 'tt_%(wp)s_fracs_njets0_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets1_ss_qcd' % vars(), 'tt_%(wp)s_fracs_njets1_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets2_ss_qcd' % vars(), 'tt_%(wp)s_fracs_njets2_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets0_os_wjets' % vars(), 'tt_%(wp)s_fracs_njets0_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets1_os_wjets' % vars(), 'tt_%(wp)s_fracs_njets1_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets2_os_wjets' % vars(), 'tt_%(wp)s_fracs_njets2_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets0_ss_wjets' % vars(), 'tt_%(wp)s_fracs_njets0_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets1_ss_wjets' % vars(), 'tt_%(wp)s_fracs_njets1_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets2_ss_wjets' % vars(), 'tt_%(wp)s_fracs_njets2_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets0_os_ttbar' % vars(), 'tt_%(wp)s_fracs_njets0_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets1_os_ttbar' % vars(), 'tt_%(wp)s_fracs_njets1_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets2_os_ttbar' % vars(), 'tt_%(wp)s_fracs_njets2_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets0_ss_ttbar' % vars(), 'tt_%(wp)s_fracs_njets0_ss_ttbar' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets1_ss_ttbar' % vars(), 'tt_%(wp)s_fracs_njets1_ss_ttbar' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2016.root:tt_fracs_njets2_ss_ttbar' % vars(), 'tt_%(wp)s_fracs_njets2_ss_ttbar' % vars())
                ]
  
  for task in histsToWrap:
     wsptools.SafeWrapHist(
       w, ['expr::m_vis_max300("min(299.9,@0)",m_vis[0])'],
       GetFromTFile(task[0]),
       name=task[1])
  
  for i in ['qcd','wjets','ttbar']:
    w.factory('expr::tt_%(wp)s_fracs_%(i)s("(@0!=0)*((@1==0)*(@2)+(@1==1)*@3+(@1>1)*(@4))+(@0==0)*((@1==0)*(@5)+(@1==1)*@6+(@1>1)*(@7))", os[1],njets[0],tt_%(wp)s_fracs_njets0_os_%(i)s,tt_%(wp)s_fracs_njets1_os_%(i)s,tt_%(wp)s_fracs_njets2_os_%(i)s,tt_%(wp)s_fracs_njets0_ss_%(i)s,tt_%(wp)s_fracs_njets1_ss_%(i)s,tt_%(wp)s_fracs_njets2_ss_%(i)s)' % vars())

  # pT dependent fits

  dm_bins = [
              'dm0',
              'dm1',
              'dm10',
              'dm11',
              'mvadm0',
              'mvadm0_sig_gt3',
              'mvadm0_sig_lt3',
              'mvadm1',
              'mvadm2',
              'mvadm10',
              'mvadm11'
  ]

  cutsmap_njets = {
                  0: '@0==0',
                  1: '@0==1',
                  2: '@0>1'
  }

  cutsmap_dm = {
              'dm0':'@1==0',
              'dm1':'@1==1',
              'dm10':'@1==10',
              'dm11':'@1==11',
              'mvadm0':'@1==0',
              'mvadm0_sig_gt3':'@1==0&&@2>=%(ip_sig_cut)s',
              'mvadm0_sig_lt3':'@1==0&&@2<%(ip_sig_cut)s',
              'mvadm1':'@1==1',
              'mvadm2':'@1==2',
              'mvadm10':'@1==10',
              'mvadm11':'@1==11'
  }

  # get all fitted functions for raw fake factors and statistical uncertainties from fit uncertainty bands
  for njet in [0,1,2]:
    for dm in dm_bins:
      func = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_2016.root:%(dm)s_njets%(njet)i_pt_1_ff_qcd_fit' % vars())
      func_str = func.GetTitle()
      params = func.GetParameters()
      for i in range(0,func.GetNpar()): func_str = func_str.replace('[%(i)i]' % vars(),'%f' % params[i])
      func_str = re.sub('x', '@0', func_str)

      w.factory('expr::pt_bounded("max(min(199.9,@0),40)",pt[0])' % vars())
      w.factory('expr::tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_fit("%(func_str)s",pt_bounded)' % vars())
 
      # get stat uncertainties
      hist_nom = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_2016.root:%(dm)s_njets%(njet)i_pt_1_ff_qcd_uncert' % vars())
      (hist_up, hist_down) = wsptools.UncertsFromHist(hist_nom)
      wsptools.SafeWrapHist(w, ['pt_bounded'], hist_nom, name='tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert_nom' % vars())
      wsptools.SafeWrapHist(w, ['pt_bounded'], hist_up, name='tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert_up' % vars())
      wsptools.SafeWrapHist(w, ['pt_bounded'], hist_down, name='tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert_down' % vars())  
 
      njets_cut = cutsmap_njets[njet] % vars() 
      dm_cut    = cutsmap_dm[dm] % vars()

      w.factory('expr::tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_up("(%(njets_cut)s&&%(dm_cut)s)*(1.+@3/@4) + ((%(njets_cut)s&&%(dm_cut)s)==0)",njets[0],mvadm[1],ipsig[0],tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert_up,tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert_nom)' % vars())
      w.factory('expr::tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_down("(%(njets_cut)s&&%(dm_cut)s)*(1.-@3/@4) + ((%(njets_cut)s&&%(dm_cut)s)==0)",njets[0],mvadm[1],ipsig[0],tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert_down,tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert_nom)' % vars())

  # make njets / dm binned functions

  w.factory('expr::ff_tt_%(wp)s_dmbins_qcd("(@0==0)*((@1==0)*@2+(@1==1)*@3+(@1==10)*@4 + (@1==11)*@5) + (@0==1)*((@1==0)*@6+(@1==1)*@7+(@1==10)*@8 + (@1==11)*@9) + (@0>1)*((@1==0)*@10+(@1==1)*@11+(@1==10)*@12 + (@1==11)*@13)", njets[0], dm[1], tt_dm0_njets0_%(wp)s_qcd_fit, tt_dm1_njets0_%(wp)s_qcd_fit, tt_dm10_njets0_%(wp)s_qcd_fit, tt_dm11_njets0_%(wp)s_qcd_fit, tt_dm0_njets1_%(wp)s_qcd_fit, tt_dm1_njets1_%(wp)s_qcd_fit, tt_dm10_njets1_%(wp)s_qcd_fit, tt_dm11_njets1_%(wp)s_qcd_fit, tt_dm0_njets2_%(wp)s_qcd_fit, tt_dm1_njets2_%(wp)s_qcd_fit, tt_dm10_njets2_%(wp)s_qcd_fit, tt_dm11_njets2_%(wp)s_qcd_fit)' % vars())

  w.factory('expr::ff_tt_%(wp)s_mvadmbins_nosig_qcd("(@0==0)*((@1==0)*@2+(@1==1)*@3+(@1==2)*@4+(@1==10)*@5+(@1==11)*@6) + (@0==1)*((@1==0)*@7+(@1==1)*@8+(@1==2)*@9+(@1==10)*@10+(@1==11)*@11) + (@0>1)*((@1==0)*@12+(@1==1)*@13+(@1==2)*@14+(@1==10)*@15+(@1==11)*@16)", njets[0], mvadm[1], tt_mvadm0_njets0_%(wp)s_qcd_fit, tt_mvadm1_njets0_%(wp)s_qcd_fit, tt_mvadm2_njets0_%(wp)s_qcd_fit, tt_mvadm10_njets0_%(wp)s_qcd_fit, tt_mvadm11_njets0_%(wp)s_qcd_fit, tt_mvadm0_njets1_%(wp)s_qcd_fit, tt_mvadm1_njets1_%(wp)s_qcd_fit, tt_mvadm2_njets1_%(wp)s_qcd_fit, tt_mvadm10_njets1_%(wp)s_qcd_fit, tt_mvadm11_njets1_%(wp)s_qcd_fit, tt_mvadm0_njets2_%(wp)s_qcd_fit, tt_mvadm1_njets2_%(wp)s_qcd_fit, tt_mvadm2_njets2_%(wp)s_qcd_fit, tt_mvadm10_njets2_%(wp)s_qcd_fit, tt_mvadm11_njets2_%(wp)s_qcd_fit)' % vars())

  w.factory('expr::ff_tt_%(wp)s_mvadmbins_qcd("(@0==0)*((@1==0&&@2<%(ip_sig_cut)s)*@3+(@1==0&&@2>=%(ip_sig_cut)s)*@4+(@1==1)*@5+(@1==2)*@6+(@1==10)*@7+(@1==11)*@8) + (@0==1)*((@1==0&&@2<3)*@9+(@1==0&&@2>=%(ip_sig_cut)s)*@10+(@1==1)*@11+(@1==2)*@12+(@1==10)*@13+(@1==11)*@14) + (@0>1)*((@1==0&&@2<3)*@15+(@1==0&&@2>=%(ip_sig_cut)s)*@16+(@1==1)*@17+(@1==2)*@18+(@1==10)*@19+(@1==11)*@20)", njets[0], mvadm[1], ipsig[0], tt_mvadm0_sig_lt3_njets0_%(wp)s_qcd_fit, tt_mvadm0_sig_gt3_njets0_%(wp)s_qcd_fit, tt_mvadm1_njets0_%(wp)s_qcd_fit, tt_mvadm2_njets0_%(wp)s_qcd_fit, tt_mvadm10_njets0_%(wp)s_qcd_fit, tt_mvadm11_njets0_%(wp)s_qcd_fit, tt_mvadm0_sig_lt3_njets1_%(wp)s_qcd_fit, tt_mvadm0_sig_gt3_njets1_%(wp)s_qcd_fit, tt_mvadm1_njets1_%(wp)s_qcd_fit, tt_mvadm2_njets1_%(wp)s_qcd_fit, tt_mvadm10_njets1_%(wp)s_qcd_fit, tt_mvadm11_njets1_%(wp)s_qcd_fit, tt_mvadm0_sig_lt3_njets2_%(wp)s_qcd_fit, tt_mvadm0_sig_gt3_njets2_%(wp)s_qcd_fit, tt_mvadm1_njets2_%(wp)s_qcd_fit, tt_mvadm2_njets2_%(wp)s_qcd_fit, tt_mvadm10_njets2_%(wp)s_qcd_fit, tt_mvadm11_njets2_%(wp)s_qcd_fit)' % vars())

  for dmtype in ['mvadmbins','mvadmbins_nosig','dmbins']:

    # get SS met closure correction

    func = GetFromTFile((loc+'/fakefactor_fits_tt_%(wp)s_2016.root:%(dmtype)s_met_ss_closure_qcd_fit' % vars()).replace('bins',''))
    func_str_pol3 = "[0] + [1]*x + [2]*x*x + [3]*x*x*x"
    params = func.GetParameters()
    for i in range(0,func.GetNpar()): func_str_pol3 = func_str_pol3.replace('[%(i)i]' % vars(),'%.9f' % params[i])
    func_str_pol3 = re.sub('x', '@0', func_str_pol3)
    w.factory('expr::met_bounded("min(99.9,@0)",met[0])' % vars())
    w.factory('expr::tt_%(dmtype)s_%(wp)s_qcd_ss_corr("(%(func_str_pol3)s)*(@1==0) + (@1>0)",met_bounded,njets[0])' % vars())


    # get OS/SS corrections and uncertainties

    func = GetFromTFile((loc+'fakefactor_fits_tt_%(wp)s_2016.root:%(dmtype)s_pt_2_os_closure_qcd_fit' % vars()).replace('bins',''))
    func_str_pol3 = "[0] + [1]*x + [2]*x*x + [3]*x*x*x"
    params = func.GetParameters()
    for i in range(0,func.GetNpar()): func_str_pol3 = func_str_pol3.replace('[%(i)i]' % vars(),'%.9f' % params[i])
    func_str_pol3 = re.sub('x', '@0', func_str_pol3)
    w.factory('expr::pt_2_bounded("max(min(139.9,@0),40.)",pt_2[0])' % vars())
    w.factory('expr::tt_%(dmtype)s_%(wp)s_qcd_corr("%(func_str_pol3)s",pt_2_bounded)' % vars())

    # apply QCD corrections - only do this for the fraction of events that come from QCD - OS events have both met and pt_2 corrections. SS events have only the MET correction
    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s("(@0!=0)*@1*(@2*@3*@4 + (1.-@3)) + (@0==0)*(@3*@4 + (1.-@3))*@1", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(wp)s_fracs_qcd, tt_%(dmtype)s_%(wp)s_qcd_ss_corr)' % vars())

    # QCD systematic is partly statistical from pt_2 os closure corrections and partly systematic from comparing data to prediction in aiso2 region in bins of dm's of both taus

    # get stat uncertainties for met ss closure correction
    hist_nom = GetFromTFile((loc+'fakefactor_fits_tt_%(wp)s_2016.root:%(dmtype)s_met_ss_closure_qcd_uncert' % vars()).replace('bins',''))
    (hist_up, hist_down) = wsptools.UncertsFromHist(hist_nom)
    wsptools.SafeWrapHist(w, ['met_bounded'], hist_nom, name='tt_%(dmtype)s_met_ss_%(wp)s_qcd_uncert_nom' % vars())
    wsptools.SafeWrapHist(w, ['met_bounded'], hist_up, name='tt_%(dmtype)s_met_ss_%(wp)s_qcd_uncert_up' % vars())
    wsptools.SafeWrapHist(w, ['met_bounded'], hist_down, name='tt_%(dmtype)s_met_ss_%(wp)s_qcd_uncert_down' % vars())

    w.factory('expr::tt_%(dmtype)s_met_ss_%(wp)s_qcd_up("(1.+@0/@1)*(@2==0) + (@2>0)",tt_%(dmtype)s_met_ss_%(wp)s_qcd_uncert_up,tt_%(dmtype)s_met_ss_%(wp)s_qcd_uncert_nom, njets[0])' % vars())
    w.factory('expr::tt_%(dmtype)s_met_ss_%(wp)s_qcd_down("(1.-@0/@1)*(@2==0) + (@2>0)",tt_%(dmtype)s_met_ss_%(wp)s_qcd_uncert_down,tt_%(dmtype)s_met_ss_%(wp)s_qcd_uncert_nom, njets[0])' % vars())

    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_stat_met_up("(@0!=0)*@1*(@2*@3*@4*@5 + (1.-@3)) + (@0==0)*(@3*@4*@5 + (1.-@3))*@1", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(wp)s_fracs_qcd, tt_%(dmtype)s_%(wp)s_qcd_ss_corr,tt_%(dmtype)s_met_ss_%(wp)s_qcd_up)' % vars())

    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_stat_met_down("(@0!=0)*@1*(@2*@3*@4*@5 + (1.-@3)) + (@0==0)*(@3*@4*@5 + (1.-@3))*@1", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(wp)s_fracs_qcd, tt_%(dmtype)s_%(wp)s_qcd_ss_corr,tt_%(dmtype)s_met_ss_%(wp)s_qcd_down)' % vars())


    # get stat uncertainties for OS/SS correction
    hist_nom = GetFromTFile((loc+'fakefactor_fits_tt_%(wp)s_2016.root:%(dmtype)s_pt_2_os_closure_qcd_uncert' % vars()).replace('bins',''))
    (hist_up, hist_down) = wsptools.UncertsFromHist(hist_nom)
    wsptools.SafeWrapHist(w, ['pt_2_bounded'], hist_nom, name='tt_%(dmtype)s_pt_2_%(wp)s_qcd_uncert_nom' % vars())
    wsptools.SafeWrapHist(w, ['pt_2_bounded'], hist_up, name='tt_%(dmtype)s_pt_2_%(wp)s_qcd_uncert_up' % vars())
    wsptools.SafeWrapHist(w, ['pt_2_bounded'], hist_down, name='tt_%(dmtype)s_pt_2_%(wp)s_qcd_uncert_down' % vars()) 

    w.factory('expr::tt_%(dmtype)s_pt_2_%(wp)s_qcd_up("1.+@0/@1",tt_%(dmtype)s_pt_2_%(wp)s_qcd_uncert_up,tt_%(dmtype)s_pt_2_%(wp)s_qcd_uncert_nom)' % vars())
    w.factory('expr::tt_%(dmtype)s_pt_2_%(wp)s_qcd_down("1.-@0/@1",tt_%(dmtype)s_pt_2_%(wp)s_qcd_uncert_down,tt_%(dmtype)s_pt_2_%(wp)s_qcd_uncert_nom)' % vars())

    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_stat_pt2_up("(@0!=0)*@1*(@2*@3*@4*@5 + (1.-@3)) + (@0==0)*(@3*@5 + (1.-@3))*@1", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(wp)s_fracs_qcd, tt_%(dmtype)s_pt_2_%(wp)s_qcd_up, tt_%(dmtype)s_%(wp)s_qcd_ss_corr)' % vars())
    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_stat_pt2_down("(@0!=0)*@1*(@2*@3*@4*@5 + (1.-@3)) + (@0==0)*(@3*@5 + (1.-@3))*@1", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(wp)s_fracs_qcd, tt_%(dmtype)s_pt_2_%(wp)s_qcd_down, tt_%(dmtype)s_%(wp)s_qcd_ss_corr)' % vars())

    # systematic uncertainty from comparing data to prediction in aiso2 region in bins of tau1 and tau2 dm's
    if dmtype != 'dmbins':
      wsptools.SafeWrapHist(w, ['mvadm','mvadm2'], GetFromTFile((loc+'fakefactor_fits_tt_%(wp)s_2016.root:%(dmtype)s_os_dm_uncert_qcd' % vars()).replace('bins','')), name='%(dmtype)s_os_dm_uncert_qcd' % vars())


      w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_syst_up("(@0!=0)*@1*(@2*@3*@4*@5 + (1.-@3)) + (@0==0)*(@3*@5 + (1.-@3))*@1", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(wp)s_fracs_qcd, %(dmtype)s_os_dm_uncert_qcd, tt_%(dmtype)s_%(wp)s_qcd_ss_corr)'  % vars())
      w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_syst_down("(@0!=0)*@1*(@2*@3*(2.-@4)*@5 + (1.-@3)) + (@0==0)*(@3*@5 + (1.-@3))*@1", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(wp)s_fracs_qcd, %(dmtype)s_os_dm_uncert_qcd, tt_%(dmtype)s_%(wp)s_qcd_ss_corr)'  % vars())

    # wjets and ttbar systematics from comparing the inclusive fakefactors derived from data for QCD events with the MC fake factors predicted in MC for wjets and ttbar

    for x in ['qcd','wjets_mc','ttbar_mc']:
      func = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_2016.root:inclusive_inclusive_pt_1_ff_%(x)s_fit' % vars())
      func_str = func.GetTitle()
      params = func.GetParameters()
      for i in range(0,func.GetNpar()): func_str = func_str.replace('[%(i)i]' % vars(),'%f' % params[i])
      func_str = re.sub('x', '@0', func_str)
      w.factory('expr::pt_bounded("max(min(199.9,@0),40)",pt[0])' % vars())
      w.factory('expr::tt_inclusive_%(wp)s_%(x)s_fit("%(func_str)s",pt_bounded)' % vars())

    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_wjets_syst_up("@0*(1.-@1 + @1*@2/@3)", ff_tt_%(wp)s_%(dmtype)s, tt_%(wp)s_fracs_wjets, tt_inclusive_%(wp)s_wjets_mc_fit, tt_inclusive_%(wp)s_qcd_fit)' % vars())
    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_wjets_syst_down("@0*(1.-@1 + @1*@3/@2)", ff_tt_%(wp)s_%(dmtype)s, tt_%(wp)s_fracs_wjets, tt_inclusive_%(wp)s_wjets_mc_fit, tt_inclusive_%(wp)s_qcd_fit)' % vars())

    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_ttbar_syst_up("@0*(1.-@1 + @1*@2/@3)", ff_tt_%(wp)s_%(dmtype)s, tt_%(wp)s_fracs_ttbar, tt_inclusive_%(wp)s_ttbar_mc_fit, tt_inclusive_%(wp)s_qcd_fit)' % vars())
    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_ttbar_syst_down("@0*(1.-@1 + @1*@3/@2)", ff_tt_%(wp)s_%(dmtype)s, tt_%(wp)s_fracs_ttbar, tt_inclusive_%(wp)s_ttbar_mc_fit, tt_inclusive_%(wp)s_qcd_fit)' % vars())

    # add statistical uncertainties 1 per njets/dm bin
    for njet in [0,1,2]:
      for dm in dm_bins:
        w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_stat_njet%(njet)i_%(dm)s_up("@0*@1",tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_up,ff_tt_%(wp)s_%(dmtype)s)' % vars())
        w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_stat_njet%(njet)i_%(dm)s_down("@0*@1",tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_down,ff_tt_%(wp)s_%(dmtype)s)' % vars())
  
w.Print()
w.writeToFile('fakefactors_ws_2016.root')
w.Delete()
