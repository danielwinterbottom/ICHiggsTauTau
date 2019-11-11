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
### mt channel ####
################################################

ip_sig_cut = '1'
wps = ['tight','medium','vtight']

for wp in wps:

  # get fractions

  loc = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/mvadm_ff_deeptauV2p1_2017_mt/' % vars()

  histsToWrap = [(loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets0_os_qcd' % vars(), 'mt_%(wp)s_fracs_njets0_os_qcd' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets1_os_qcd' % vars(), 'mt_%(wp)s_fracs_njets1_os_qcd' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets2_os_qcd' % vars(), 'mt_%(wp)s_fracs_njets2_os_qcd' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets0_ss_qcd' % vars(), 'mt_%(wp)s_fracs_njets0_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets1_ss_qcd' % vars(), 'mt_%(wp)s_fracs_njets1_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets2_ss_qcd' % vars(), 'mt_%(wp)s_fracs_njets2_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets0_os_wjets' % vars(), 'mt_%(wp)s_fracs_njets0_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets1_os_wjets' % vars(), 'mt_%(wp)s_fracs_njets1_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets2_os_wjets' % vars(), 'mt_%(wp)s_fracs_njets2_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets0_ss_wjets' % vars(), 'mt_%(wp)s_fracs_njets0_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets1_ss_wjets' % vars(), 'mt_%(wp)s_fracs_njets1_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets2_ss_wjets' % vars(), 'mt_%(wp)s_fracs_njets2_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets0_os_ttbar' % vars(), 'mt_%(wp)s_fracs_njets0_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets1_os_ttbar' % vars(), 'mt_%(wp)s_fracs_njets1_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets2_os_ttbar' % vars(), 'mt_%(wp)s_fracs_njets2_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets0_ss_ttbar' % vars(), 'mt_%(wp)s_fracs_njets0_ss_ttbar' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets1_ss_ttbar' % vars(), 'mt_%(wp)s_fracs_njets1_ss_ttbar' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2017.root:mt_fracs_njets2_ss_ttbar' % vars(), 'mt_%(wp)s_fracs_njets2_ss_ttbar' % vars())
                ]

  for task in histsToWrap:
     wsptools.SafeWrapHist(
       w, ['expr::mt_max120("min(119.9,@0)",mt[0])'],
       GetFromTFile(task[0]),
       name=task[1])

  for i in ['qcd','wjets','ttbar']:
    w.factory('expr::mt_%(wp)s_fracs_%(i)s("(@0!=0)*((@1==0)*(@2)+(@1==1)*@3+(@1>1)*(@4))+(@0==0)*((@1==0)*(@5)+(@1==1)*@6+(@1>1)*(@7))", os[1],njets[0],mt_%(wp)s_fracs_njets0_os_%(i)s,mt_%(wp)s_fracs_njets1_os_%(i)s,mt_%(wp)s_fracs_njets2_os_%(i)s,mt_%(wp)s_fracs_njets0_ss_%(i)s,mt_%(wp)s_fracs_njets1_ss_%(i)s,mt_%(wp)s_fracs_njets2_ss_%(i)s)' % vars())

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
                  '0': '@0==0&&@3>0.5',
                  '1': '@0==1&&@3>0.5',
                  '2': '@0>1&&@3>0.5',
                  '0_crosstrg': '@0==0&&@3<0.5',
                  '1_crosstrg': '@0==1&&@3<0.5',
                  '2_crosstrg': '@0>1&&@3<0.5',
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
  for dm in dm_bins:
    for njet in ['0','1','2','0_crosstrg','1_crosstrg','2_crosstrg']:
      func_qcd = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2017.root:%(dm)s_njets%(njet)s_pt_2_ff_qcd_fit' % vars())
      func_wjets = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2017.root:%(dm)s_njets%(njet)s_pt_2_ff_wjets_fit' % vars())
      func_qcd_str = str(func_qcd.GetExpFormula('p')).replace('x','@0')
      func_wjets_str = str(func_wjets.GetExpFormula('p')).replace('x','@0')


      # QCD set max at 100, W+jets at 140, unless it is a cross trigger fit, then set maximum as 80
      w.factory('expr::pt_bounded140("max(min(139.9,@0),20)",pt[0])' % vars())
      w.factory('expr::pt_bounded100("max(min(99.9,@0),20)",pt[0])' % vars())
      w.factory('expr::pt_bounded80("max(min(79.9,@0),20)",pt[0])' % vars())

      if 'crosstrg' in njet:
        w.factory('expr::mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_fit("max(%(func_qcd_str)s,0.)",pt_bounded80)' % vars())
        w.factory('expr::mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_fit("max(%(func_wjets_str)s,0.)",pt_bounded80)' % vars())
      else:
        w.factory('expr::mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_fit("max(%(func_qcd_str)s,0.)",pt_bounded100)' % vars())
        w.factory('expr::mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_fit("max(%(func_wjets_str)s,0.)",pt_bounded140)' % vars())

      # get stat uncertainties
      hist_nom = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2017.root:%(dm)s_njets%(njet)s_pt_2_ff_qcd_uncert' % vars())
      (hist_up, hist_down) = wsptools.UncertsFromHist(hist_nom,0.,2.)
      if 'crosstrg' in njet:
        wsptools.SafeWrapHist(w, ['pt_bounded100'], hist_nom, name='mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_nom' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded100'], hist_up, name='mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_up' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded100'], hist_down, name='mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_down' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded140'], hist_nom, name='mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_nom' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded140'], hist_up, name='mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_up' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded140'], hist_down, name='mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_down' % vars())
      else:
        wsptools.SafeWrapHist(w, ['pt_bounded80'], hist_nom, name='mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_nom' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded80'], hist_up, name='mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_up' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded80'], hist_down, name='mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_down' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded80'], hist_nom, name='mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_nom' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded80'], hist_up, name='mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_up' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded80'], hist_down, name='mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_down' % vars())

      njets_cut = cutsmap_njets[njet] % vars()
      dm_cut    = cutsmap_dm[dm] % vars()

      w.factory('expr::mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_up("(%(njets_cut)s&&%(dm_cut)s)*(1.+@4/@5) + ((%(njets_cut)s&&%(dm_cut)s)==0)",njets[0],mvadm[1],ipsig[0],pass_single[1],mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_up,mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_nom)' % vars())
      w.factory('expr::mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_down("(%(njets_cut)s&&%(dm_cut)s)*(1.-@4/@5) + ((%(njets_cut)s&&%(dm_cut)s)==0)",njets[0],mvadm[1],ipsig[0],pass_single[1],mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_down,mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_nom)' % vars())

      w.factory('expr::mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_up("(%(njets_cut)s&&%(dm_cut)s)*(1.+@4/@5) + ((%(njets_cut)s&&%(dm_cut)s)==0)",njets[0],mvadm[1],ipsig[0],pass_single[1],mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_up,mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_nom)' % vars())
      w.factory('expr::mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_down("(%(njets_cut)s&&%(dm_cut)s)*(1.-@4/@5) + ((%(njets_cut)s&&%(dm_cut)s)==0)",njets[0],mvadm[1],ipsig[0],pass_single[1],mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_down,mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_nom)' % vars())

    # get ttbar FFs inclusive in njets and also get W data and MC SFs to be used to define uncertainties

    func_ttbar = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2017.root:%(dm)s_inclusive_pt_2_ff_ttbar_mc_fit' % vars())
    func_wjets = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2017.root:%(dm)s_inclusive_pt_2_ff_wjets_fit' % vars())
    func_wjets_mc = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2017.root:%(dm)s_inclusive_pt_2_ff_wjets_mc_fit' % vars())
    func_ttbar_str = str(func_ttbar.GetExpFormula('p')).replace('x','@0')
    func_wjets_str = str(func_wjets.GetExpFormula('p')).replace('x','@0')
    func_wjets_mc_str = str(func_wjets_mc.GetExpFormula('p')).replace('x','@0')

    w.factory('expr::pt_bounded140("max(min(139.9,@0),20)",pt[0])' % vars())

    w.factory('expr::mt_%(dm)s_inclusive_%(wp)s_ttbar_fit("max(%(func_ttbar_str)s,0.)",pt_bounded100)' % vars())
    w.factory('expr::mt_%(dm)s_inclusive_%(wp)s_wjets_fit("max(%(func_wjets_str)s,0.)",pt_bounded140)' % vars())
    w.factory('expr::mt_%(dm)s_inclusive_%(wp)s_wjets_mc_fit("max(%(func_wjets_mc_str)s,0.)",pt_bounded140)' % vars())

    # get stat uncertainties on ttbar
    hist_nom = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2017.root:%(dm)s_inclusive_pt_2_ff_ttbar_mc_uncert' % vars())
    (hist_up, hist_down) = wsptools.UncertsFromHist(hist_nom,0.,2.)
    wsptools.SafeWrapHist(w, ['pt_bounded140'], hist_nom, name='mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert_nom' % vars())
    wsptools.SafeWrapHist(w, ['pt_bounded140'], hist_up, name='mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert_up' % vars())
    wsptools.SafeWrapHist(w, ['pt_bounded140'], hist_down, name='mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert_down' % vars())

    dm_cut    = (cutsmap_dm[dm] % vars()).replace('@1','@0').replace('@2','@1')

    w.factory('expr::mt_%(dm)s_inclusive_%(wp)s_ttbar_up("(%(dm_cut)s)*(1.+@2/@3) + ((%(dm_cut)s)==0)",mvadm[1],ipsig[0],mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert_up,mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert_nom)' % vars())
    w.factory('expr::mt_%(dm)s_inclusive_%(wp)s_ttbar_down("(%(dm_cut)s)*(1.-@2/@3) + ((%(dm_cut)s)==0)",mvadm[1],ipsig[0],mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert_down,mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert_nom)' % vars())

  # combine uncertainties into functions


  ff_eqn_tot = '(@0>=0.5)*((@2==0)*((@1==0&&@3<%(ip_sig_cut)s)*(@4)+(@1==0&&@3>=%(ip_sig_cut)s)*(@5)+(@1==1)*(@6)+(@1==2)*(@7)+(@1==10)*(@8)+(@1==11)*(@9)) + (@2==1)*((@1==0&&@3<%(ip_sig_cut)s)*(@10)+(@1==0&&@3>=%(ip_sig_cut)s)*(@11)+(@1==1)*(@12)+(@1==2)*(@13)+(@1==10)*(@14)+(@1==11)*(@15)) + (@2>1)*((@1==0&&@3<%(ip_sig_cut)s)*(@16)+(@1==0&&@3>=%(ip_sig_cut)s)*(@17)+(@1==1)*(@18)+(@1==2)*(@19)+(@1==10)*(@20)+(@1==11)*(@21)))' % vars() 

  ff_eqn_tot_cross = '(@0<0.5)*((@2==0)*((@1==0&&@3<%(ip_sig_cut)s)*(@22)+(@1==0&&@3>=%(ip_sig_cut)s)*(@23)+(@1==1)*(@24)+(@1==2)*(@25)+(@1==10)*(@26)+(@1==11)*(@27)) + (@2==1)*((@1==0&&@3<%(ip_sig_cut)s)*(@28)+(@1==0&&@3>=%(ip_sig_cut)s)*(@29)+(@1==1)*(@30)+(@1==2)*(@31)+(@1==10)*(@32)+(@1==11)*(@33)) + (@2>1)*((@1==0&&@3<%(ip_sig_cut)s)*(@34)+(@1==0&&@3>=%(ip_sig_cut)s)*(@35)+(@1==1)*(@36)+(@1==2)*(@37)+(@1==10)*(@38)+(@1==11)*(@39)))' %vars()

  w.factory('expr::ff_mt_%(wp)s_mvadmbins_qcd_raw("%(ff_eqn_tot)s + %(ff_eqn_tot_cross)s", pass_single[1], mvadm[1], njets[0], ipsig[0], mt_mvadm0_sig_lt3_njets0_%(wp)s_qcd_fit, mt_mvadm0_sig_gt3_njets0_%(wp)s_qcd_fit, mt_mvadm1_njets0_%(wp)s_qcd_fit, mt_mvadm2_njets0_%(wp)s_qcd_fit, mt_mvadm10_njets0_%(wp)s_qcd_fit, mt_mvadm11_njets0_%(wp)s_qcd_fit, mt_mvadm0_sig_lt3_njets1_%(wp)s_qcd_fit, mt_mvadm0_sig_gt3_njets1_%(wp)s_qcd_fit, mt_mvadm1_njets1_%(wp)s_qcd_fit, mt_mvadm2_njets1_%(wp)s_qcd_fit, mt_mvadm10_njets1_%(wp)s_qcd_fit, mt_mvadm11_njets1_%(wp)s_qcd_fit, mt_mvadm0_sig_lt3_njets2_%(wp)s_qcd_fit, mt_mvadm0_sig_gt3_njets2_%(wp)s_qcd_fit, mt_mvadm1_njets2_%(wp)s_qcd_fit, mt_mvadm2_njets2_%(wp)s_qcd_fit, mt_mvadm10_njets2_%(wp)s_qcd_fit, mt_mvadm11_njets2_%(wp)s_qcd_fit, mt_mvadm0_sig_lt3_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm0_sig_gt3_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm1_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm2_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm10_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm11_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm0_sig_lt3_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm0_sig_gt3_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm1_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm2_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm10_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm11_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm0_sig_lt3_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm0_sig_gt3_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm1_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm2_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm10_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm11_njets2_crosstrg_%(wp)s_qcd_fit )' % vars())

  w.factory(('expr::ff_mt_%(wp)s_mvadmbins_qcd_raw("%(ff_eqn_tot)s + %(ff_eqn_tot_cross)s", pass_single[1], mvadm[1], njets[0], ipsig[0], mt_mvadm0_sig_lt3_njets0_%(wp)s_qcd_fit, mt_mvadm0_sig_gt3_njets0_%(wp)s_qcd_fit, mt_mvadm1_njets0_%(wp)s_qcd_fit, mt_mvadm2_njets0_%(wp)s_qcd_fit, mt_mvadm10_njets0_%(wp)s_qcd_fit, mt_mvadm11_njets0_%(wp)s_qcd_fit, mt_mvadm0_sig_lt3_njets1_%(wp)s_qcd_fit, mt_mvadm0_sig_gt3_njets1_%(wp)s_qcd_fit, mt_mvadm1_njets1_%(wp)s_qcd_fit, mt_mvadm2_njets1_%(wp)s_qcd_fit, mt_mvadm10_njets1_%(wp)s_qcd_fit, mt_mvadm11_njets1_%(wp)s_qcd_fit, mt_mvadm0_sig_lt3_njets2_%(wp)s_qcd_fit, mt_mvadm0_sig_gt3_njets2_%(wp)s_qcd_fit, mt_mvadm1_njets2_%(wp)s_qcd_fit, mt_mvadm2_njets2_%(wp)s_qcd_fit, mt_mvadm10_njets2_%(wp)s_qcd_fit, mt_mvadm11_njets2_%(wp)s_qcd_fit, mt_mvadm0_sig_lt3_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm0_sig_gt3_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm1_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm2_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm10_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm11_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm0_sig_lt3_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm0_sig_gt3_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm1_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm2_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm10_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm11_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm0_sig_lt3_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm0_sig_gt3_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm1_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm2_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm10_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm11_njets2_crosstrg_%(wp)s_qcd_fit )' % vars()).replace('qcd','wjets'))

  ff_eqn_tot = '(@0>=0.5)*((@2==0)*((@1==0)*(@3)+(@1==1)*(@4)+(@1==2)*(@5)+(@1==10)*(@6)+(@1==11)*(@7)) + (@2==1)*((@1==0)*(@8)+(@1==1)*(@9)+(@1==2)*(@10)+(@1==10)*(@11)+(@1==11)*(@12)) + (@2>1)*((@1==0)*(@13)+(@1==1)*(@14)+(@1==2)*(@15)+(@1==10)*(@16)+(@1==11)*(@17)))' % vars()

  ff_eqn_tot_cross = '(@0<0.5)*((@2==0)*((@1==0)*(@18)+(@1==1)*(@19)+(@1==2)*(@20)+(@1==10)*(@21)+(@1==11)*(@22)) + (@2==1)*((@1==0)*(@23)+(@1==1)*(@24)+(@1==2)*(@25)+(@1==10)*(@26)+(@1==11)*(@27)) + (@2>1)*((@1==0)*(@28)+(@1==1)*(@29)+(@1==2)*(@30)+(@1==10)*(@31)+(@1==11)*(@32)))' % vars()
  
  w.factory('expr::ff_mt_%(wp)s_mvadmbins_nosig_qcd_raw("%(ff_eqn_tot)s + %(ff_eqn_tot_cross)s", pass_single[1], mvadm[1], njets[0], mt_mvadm0_njets0_%(wp)s_qcd_fit, mt_mvadm1_njets0_%(wp)s_qcd_fit, mt_mvadm2_njets0_%(wp)s_qcd_fit, mt_mvadm10_njets0_%(wp)s_qcd_fit, mt_mvadm11_njets0_%(wp)s_qcd_fit, mt_mvadm0_njets1_%(wp)s_qcd_fit, mt_mvadm1_njets1_%(wp)s_qcd_fit, mt_mvadm2_njets1_%(wp)s_qcd_fit, mt_mvadm10_njets1_%(wp)s_qcd_fit, mt_mvadm11_njets1_%(wp)s_qcd_fit, mt_mvadm0_njets2_%(wp)s_qcd_fit, mt_mvadm1_njets2_%(wp)s_qcd_fit, mt_mvadm2_njets2_%(wp)s_qcd_fit, mt_mvadm10_njets2_%(wp)s_qcd_fit, mt_mvadm11_njets2_%(wp)s_qcd_fit, mt_mvadm0_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm1_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm2_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm10_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm11_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm0_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm1_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm2_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm10_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm11_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm0_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm1_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm2_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm10_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm11_njets2_crosstrg_%(wp)s_qcd_fit )' % vars()) 

  w.factory(('expr::ff_mt_%(wp)s_mvadmbins_nosig_qcd_raw("%(ff_eqn_tot)s + %(ff_eqn_tot_cross)s", pass_single[1], mvadm[1], njets[0], mt_mvadm0_njets0_%(wp)s_qcd_fit, mt_mvadm1_njets0_%(wp)s_qcd_fit, mt_mvadm2_njets0_%(wp)s_qcd_fit, mt_mvadm10_njets0_%(wp)s_qcd_fit, mt_mvadm11_njets0_%(wp)s_qcd_fit, mt_mvadm0_njets1_%(wp)s_qcd_fit, mt_mvadm1_njets1_%(wp)s_qcd_fit, mt_mvadm2_njets1_%(wp)s_qcd_fit, mt_mvadm10_njets1_%(wp)s_qcd_fit, mt_mvadm11_njets1_%(wp)s_qcd_fit, mt_mvadm0_njets2_%(wp)s_qcd_fit, mt_mvadm1_njets2_%(wp)s_qcd_fit, mt_mvadm2_njets2_%(wp)s_qcd_fit, mt_mvadm10_njets2_%(wp)s_qcd_fit, mt_mvadm11_njets2_%(wp)s_qcd_fit, mt_mvadm0_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm1_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm2_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm10_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm11_njets0_crosstrg_%(wp)s_qcd_fit, mt_mvadm0_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm1_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm2_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm10_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm11_njets1_crosstrg_%(wp)s_qcd_fit, mt_mvadm0_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm1_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm2_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm10_njets2_crosstrg_%(wp)s_qcd_fit, mt_mvadm11_njets2_crosstrg_%(wp)s_qcd_fit )' % vars()).replace('qcd','wjets')) 

  ff_eqn_tot = '(@0>=0.5)*((@2==0)*((@1==0)*(@3)+(@1==1)*(@4)+(@1==10)*(@5)+(@1==11)*(@6)) + (@2==1)*((@1==0)*(@7)+(@1==1)*(@8)+(@1==10)*(@9)+(@1==11)*(@10)) + (@2>1)*((@1==0)*(@11)+(@1==1)*(@12)+(@1==10)*(@13)+(@1==11)*(@14)))' % vars()

  ff_eqn_tot_cross = '(@0<0.5)*((@2==0)*((@1==0)*(@15)+(@1==1)*(@16)+(@1==10)*(@17)+(@1==11)*(@18)) + (@2==1)*((@1==0)*(@19)+(@1==1)*(@20)+(@1==10)*(@21)+(@1==11)*(@22)) + (@2>1)*((@1==0)*(@23)+(@1==1)*(@24)+(@1==10)*(@25)+(@1==11)*(@26)))' % vars()
  
  w.factory('expr::ff_mt_%(wp)s_dmbins_qcd_raw("%(ff_eqn_tot)s + %(ff_eqn_tot_cross)s", pass_single[1], dm[1], njets[0], mt_dm0_njets0_%(wp)s_qcd_fit, mt_dm1_njets0_%(wp)s_qcd_fit, mt_dm10_njets0_%(wp)s_qcd_fit, mt_dm11_njets0_%(wp)s_qcd_fit, mt_dm0_njets1_%(wp)s_qcd_fit, mt_dm1_njets1_%(wp)s_qcd_fit, mt_dm10_njets1_%(wp)s_qcd_fit, mt_dm11_njets1_%(wp)s_qcd_fit, mt_dm0_njets2_%(wp)s_qcd_fit, mt_dm1_njets2_%(wp)s_qcd_fit, mt_dm10_njets2_%(wp)s_qcd_fit, mt_dm11_njets2_%(wp)s_qcd_fit, mt_dm0_njets0_crosstrg_%(wp)s_qcd_fit, mt_dm1_njets0_crosstrg_%(wp)s_qcd_fit, mt_dm10_njets0_crosstrg_%(wp)s_qcd_fit, mt_dm11_njets0_crosstrg_%(wp)s_qcd_fit, mt_dm0_njets1_crosstrg_%(wp)s_qcd_fit, mt_dm1_njets1_crosstrg_%(wp)s_qcd_fit, mt_dm10_njets1_crosstrg_%(wp)s_qcd_fit, mt_dm11_njets1_crosstrg_%(wp)s_qcd_fit, mt_dm0_njets2_crosstrg_%(wp)s_qcd_fit, mt_dm1_njets2_crosstrg_%(wp)s_qcd_fit, mt_dm10_njets2_crosstrg_%(wp)s_qcd_fit, mt_dm11_njets2_crosstrg_%(wp)s_qcd_fit )' % vars())

  w.factory(('expr::ff_mt_%(wp)s_dmbins_qcd_raw("%(ff_eqn_tot)s + %(ff_eqn_tot_cross)s", pass_single[1], dm[1], njets[0], mt_dm0_njets0_%(wp)s_qcd_fit, mt_dm1_njets0_%(wp)s_qcd_fit, mt_dm10_njets0_%(wp)s_qcd_fit, mt_dm11_njets0_%(wp)s_qcd_fit, mt_dm0_njets1_%(wp)s_qcd_fit, mt_dm1_njets1_%(wp)s_qcd_fit, mt_dm10_njets1_%(wp)s_qcd_fit, mt_dm11_njets1_%(wp)s_qcd_fit, mt_dm0_njets2_%(wp)s_qcd_fit, mt_dm1_njets2_%(wp)s_qcd_fit, mt_dm10_njets2_%(wp)s_qcd_fit, mt_dm11_njets2_%(wp)s_qcd_fit, mt_dm0_njets0_crosstrg_%(wp)s_qcd_fit, mt_dm1_njets0_crosstrg_%(wp)s_qcd_fit, mt_dm10_njets0_crosstrg_%(wp)s_qcd_fit, mt_dm11_njets0_crosstrg_%(wp)s_qcd_fit, mt_dm0_njets1_crosstrg_%(wp)s_qcd_fit, mt_dm1_njets1_crosstrg_%(wp)s_qcd_fit, mt_dm10_njets1_crosstrg_%(wp)s_qcd_fit, mt_dm11_njets1_crosstrg_%(wp)s_qcd_fit, mt_dm0_njets2_crosstrg_%(wp)s_qcd_fit, mt_dm1_njets2_crosstrg_%(wp)s_qcd_fit, mt_dm10_njets2_crosstrg_%(wp)s_qcd_fit, mt_dm11_njets2_crosstrg_%(wp)s_qcd_fit )' % vars()).replace('qcd','wjets'))

  w.factory('expr::ff_mt_%(wp)s_mvadmbins_ttbar("(@0==0&&@1<%(ip_sig_cut)s)*(@2)+(@0==0&&@1>=%(ip_sig_cut)s)*(@3)+(@0==1)*(@4)+(@0==2)*(@5)+(@0==10)*(@6)+(@0==11)*(@7)", mvadm[1], ipsig[0], mt_mvadm0_sig_lt3_inclusive_%(wp)s_ttbar_fit, mt_mvadm0_sig_gt3_inclusive_%(wp)s_ttbar_fit, mt_mvadm1_inclusive_%(wp)s_ttbar_fit, mt_mvadm2_inclusive_%(wp)s_ttbar_fit, mt_mvadm10_inclusive_%(wp)s_ttbar_fit, mt_mvadm11_inclusive_%(wp)s_ttbar_fit,)' % vars())

  w.factory('expr::ff_mt_%(wp)s_mvadmbins_nosig_ttbar("(@0==0)*(@1)+(@0==1)*(@2)+(@0==2)*(@3)+(@0==10)*(@4)+(@0==11)*(@5)", mvadm[1], mt_mvadm0_inclusive_%(wp)s_ttbar_fit, mt_mvadm1_inclusive_%(wp)s_ttbar_fit, mt_mvadm2_inclusive_%(wp)s_ttbar_fit, mt_mvadm10_inclusive_%(wp)s_ttbar_fit, mt_mvadm11_inclusive_%(wp)s_ttbar_fit)' % vars())

  w.factory('expr::ff_mt_%(wp)s_dmbins_ttbar("(@0==0)*(@1)+(@0==1)*(@2)+(@0==10)*(@3)+(@0==11)*(@4)", dm[1], mt_dm0_inclusive_%(wp)s_ttbar_fit, mt_dm1_inclusive_%(wp)s_ttbar_fit, mt_dm10_inclusive_%(wp)s_ttbar_fit, mt_dm11_inclusive_%(wp)s_ttbar_fit )' % vars()) 

  # get W+jets corrections
  for dmtype in ['mvadm','mvadm_nosig','dm']:

    dmname = dmtype.replace('dm','dmbins')

    # met correction
    func = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2017.root:%(dmtype)s_met_closure_wjets_fit' % vars())
    func_met_corr = str(func.GetExpFormula('p')).replace('x','@0').replace('y','@1')
  
    w.factory('expr::pt_bounded100("max(min(99.9,@0),20.)",pt[20])' % vars())
    w.factory('expr::met_bounded140("max(min(139.9,@0),20.)",met[0])' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_wjets_met_corr("(%(func_met_corr)s)*(@2<2) + (@2>1)",pt_bounded100,met_bounded140,njets[0])' % vars())
  
    # pt_1 correction
    func = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2017.root:%(dmtype)s_pt_1_closure_wjets_fit' % vars())
    func_m_pt_corr = str(func.GetExpFormula('p')).replace('x','@0').replace('y','@1')
  
    w.factory('expr::pt_bounded100("max(min(99.9,@0),20.)",pt[20])' % vars())
    w.factory('expr::m_pt_bounded140("max(min(139.9,@0),20.)",m_pt[20])' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_wjets_m_pt_corr("(%(func_m_pt_corr)s)*(@2<2) + (@2>1)",pt_bounded100,m_pt_bounded140,njets[0])' % vars())
  
    # mt_1 correction
    func = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2017.root:%(dmtype)s_mt_corr_wjets_mc_fit' % vars())
    func_mt_corr = str(func.GetExpFormula('p')).replace('x','@0')
  
    w.factory('expr::mt_bounded140("min(139.9,@0)",mt)' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_wjets_mt_corr("%(func_mt_corr)s",mt_bounded140)' % vars())

    # apply corrections to raw W+jets FFs
    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets("@0*@1*@2*@3", ff_mt_%(wp)s_%(dmname)s_wjets_raw , mt_%(dmname)s_%(wp)s_wjets_met_corr, mt_%(dmname)s_%(wp)s_wjets_m_pt_corr, mt_%(dmname)s_%(wp)s_wjets_mt_corr)' % vars())

  # get QCD corrections
  for dmtype in ['mvadm','mvadm_nosig','dm']:

    dmname = dmtype.replace('dm','dmbins')

    # met correction
    func = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2017.root:%(dmtype)s_met_closure_qcd_fit' % vars())
    func_met_corr = str(func.GetExpFormula('p')).replace('x','@0').replace('y','@1')

    w.factory('expr::pt_bounded70("max(min(69.9,@0),20.)",pt)' % vars())
    w.factory('expr::met_bounded70("min(69.9,@0)",met[0])' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_qcd_met_corr("(%(func_met_corr)s)*(@2==0) + (@2>0)",pt_bounded70,met_bounded70,njets[0])' % vars())

    # pt_1 correction
    func = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2017.root:%(dmtype)s_pt_1_closure_qcd_fit' % vars())
    func_m_pt_corr = str(func.GetExpFormula('p')).replace('x','@0').replace('y','@1')

    w.factory('expr::pt_bounded70("max(min(69.9,@0),20.)")' % vars())
    w.factory('expr::m_pt_bounded50("max(min(49.9,@0),20.)",m_pt[20])' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_qcd_m_pt_corr("(%(func_m_pt_corr)s)*(@2==0) + (@2>0)",pt_bounded70,m_pt_bounded50,njets[0])' % vars())

    # aiso->iso correction
    func = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2017.root:%(dmtype)s_iso_closure_qcd_fit' % vars())
    func_iso_corr = str(func.GetExpFormula('p')).replace('x','@0')
    w.factory('expr::iso_bounded0p5("min(0.499,@0)",m_iso[0])' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_qcd_iso_corr("%(func_iso_corr)s",iso_bounded0p5)' % vars())

    # OS/SS correction
    func = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2017.root:%(dmtype)s_osss_closure_qcd_fit' % vars())
    func_osss_corr = str(func.GetExpFormula('p')).replace('x','@0')
    w.factory('expr::m_pt_bounded80("min(79.99,@0)",m_pt)' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_qcd_osss_corr("%(func_osss_corr)s",m_pt_bounded80)' % vars())

    # apply corrections to raw QCD FFs
    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd("(@5!=0)*@0*@1*@2*@3*@4 + (@5==0)*@0*@1*@2*@3", ff_mt_%(wp)s_%(dmname)s_qcd_raw , mt_%(dmname)s_%(wp)s_qcd_met_corr, mt_%(dmname)s_%(wp)s_qcd_m_pt_corr, mt_%(dmname)s_%(wp)s_qcd_iso_corr, mt_%(dmname)s_%(wp)s_qcd_osss_corr, os[1])' % vars())

# scale FFs by their fractions

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar)' % vars())

################################################
### tt channel ####
################################################

ip_sig_cut = '1'
wps = ['tight','medium','vtight']

for wp in wps:

  # get fractions

  loc = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/mvadm_ff_deeptauV2p1_2017/' % vars()
 
  histsToWrap = [(loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets0_os_qcd' % vars(), 'tt_%(wp)s_fracs_njets0_os_qcd' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets1_os_qcd' % vars(), 'tt_%(wp)s_fracs_njets1_os_qcd' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets2_os_qcd' % vars(), 'tt_%(wp)s_fracs_njets2_os_qcd' % vars()), 
                 (loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets0_ss_qcd' % vars(), 'tt_%(wp)s_fracs_njets0_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets1_ss_qcd' % vars(), 'tt_%(wp)s_fracs_njets1_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets2_ss_qcd' % vars(), 'tt_%(wp)s_fracs_njets2_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets0_os_wjets' % vars(), 'tt_%(wp)s_fracs_njets0_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets1_os_wjets' % vars(), 'tt_%(wp)s_fracs_njets1_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets2_os_wjets' % vars(), 'tt_%(wp)s_fracs_njets2_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets0_ss_wjets' % vars(), 'tt_%(wp)s_fracs_njets0_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets1_ss_wjets' % vars(), 'tt_%(wp)s_fracs_njets1_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets2_ss_wjets' % vars(), 'tt_%(wp)s_fracs_njets2_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets0_os_ttbar' % vars(), 'tt_%(wp)s_fracs_njets0_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets1_os_ttbar' % vars(), 'tt_%(wp)s_fracs_njets1_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets2_os_ttbar' % vars(), 'tt_%(wp)s_fracs_njets2_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets0_ss_ttbar' % vars(), 'tt_%(wp)s_fracs_njets0_ss_ttbar' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets1_ss_ttbar' % vars(), 'tt_%(wp)s_fracs_njets1_ss_ttbar' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2017.root:tt_fracs_njets2_ss_ttbar' % vars(), 'tt_%(wp)s_fracs_njets2_ss_ttbar' % vars())
                ]
  
  for task in histsToWrap:
     print task
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
      func = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_2017.root:%(dm)s_njets%(njet)i_pt_1_ff_qcd_fit' % vars())
      func_str = func.GetTitle()
      params = func.GetParameters()
      for i in range(0,func.GetNpar()): func_str = func_str.replace('[%(i)i]' % vars(),'%f' % params[i])
      func_str = re.sub('x', '@0', func_str)

      w.factory('expr::pt_bounded("max(min(199.9,@0),40)",pt[0])' % vars())
      w.factory('expr::tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_fit("max(%(func_str)s,0.)",pt_bounded)' % vars())
 
      # get stat uncertainties
      hist_nom = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_2017.root:%(dm)s_njets%(njet)i_pt_1_ff_qcd_uncert' % vars())
      (hist_up, hist_down) = wsptools.UncertsFromHist(hist_nom,0.,2.)
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

    func = GetFromTFile((loc+'/fakefactor_fits_tt_%(wp)s_2017.root:%(dmtype)s_met_ss_closure_qcd_fit' % vars()).replace('bins',''))
    func_str_pol3 = "[0] + [1]*x + [2]*x*x + [3]*x*x*x"
    params = func.GetParameters()
    for i in range(0,func.GetNpar()): func_str_pol3 = func_str_pol3.replace('[%(i)i]' % vars(),'%.9f' % params[i])
    func_str_pol3 = re.sub('x', '@0', func_str_pol3)
    w.factory('expr::met_bounded("min(99.9,@0)",met[0])' % vars())
    w.factory('expr::tt_%(dmtype)s_%(wp)s_qcd_ss_corr("(%(func_str_pol3)s)*(@1==0) + (@1>0)",met_bounded,njets[0])' % vars())


    # get OS/SS corrections and uncertainties

    func = GetFromTFile((loc+'fakefactor_fits_tt_%(wp)s_2017.root:%(dmtype)s_pt_2_os_closure_qcd_fit' % vars()).replace('bins',''))
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
    hist_nom = GetFromTFile((loc+'fakefactor_fits_tt_%(wp)s_2017.root:%(dmtype)s_met_ss_closure_qcd_uncert' % vars()).replace('bins',''))
    (hist_up, hist_down) = wsptools.UncertsFromHist(hist_nom,0.,2.)
    wsptools.SafeWrapHist(w, ['met_bounded'], hist_nom, name='tt_%(dmtype)s_met_ss_%(wp)s_qcd_uncert_nom' % vars())
    wsptools.SafeWrapHist(w, ['met_bounded'], hist_up, name='tt_%(dmtype)s_met_ss_%(wp)s_qcd_uncert_up' % vars())
    wsptools.SafeWrapHist(w, ['met_bounded'], hist_down, name='tt_%(dmtype)s_met_ss_%(wp)s_qcd_uncert_down' % vars())

    w.factory('expr::tt_%(dmtype)s_met_ss_%(wp)s_qcd_up("(1.+@0/@1)*(@2==0) + (@2>0)",tt_%(dmtype)s_met_ss_%(wp)s_qcd_uncert_up,tt_%(dmtype)s_met_ss_%(wp)s_qcd_uncert_nom, njets[0])' % vars())
    w.factory('expr::tt_%(dmtype)s_met_ss_%(wp)s_qcd_down("(1.-@0/@1)*(@2==0) + (@2>0)",tt_%(dmtype)s_met_ss_%(wp)s_qcd_uncert_down,tt_%(dmtype)s_met_ss_%(wp)s_qcd_uncert_nom, njets[0])' % vars())

    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_stat_met_up("(@0!=0)*@1*(@2*@3*@4*@5 + (1.-@3)) + (@0==0)*(@3*@4*@5 + (1.-@3))*@1", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(wp)s_fracs_qcd, tt_%(dmtype)s_%(wp)s_qcd_ss_corr,tt_%(dmtype)s_met_ss_%(wp)s_qcd_up)' % vars())

    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_stat_met_down("(@0!=0)*@1*(@2*@3*@4*@5 + (1.-@3)) + (@0==0)*(@3*@4*@5 + (1.-@3))*@1", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(wp)s_fracs_qcd, tt_%(dmtype)s_%(wp)s_qcd_ss_corr,tt_%(dmtype)s_met_ss_%(wp)s_qcd_down)' % vars())


    # get stat uncertainties for OS/SS correction
    hist_nom = GetFromTFile((loc+'fakefactor_fits_tt_%(wp)s_2017.root:%(dmtype)s_pt_2_os_closure_qcd_uncert' % vars()).replace('bins',''))
    (hist_up, hist_down) = wsptools.UncertsFromHist(hist_nom,0.,2.)
    wsptools.SafeWrapHist(w, ['pt_2_bounded'], hist_nom, name='tt_%(dmtype)s_pt_2_%(wp)s_qcd_uncert_nom' % vars())
    wsptools.SafeWrapHist(w, ['pt_2_bounded'], hist_up, name='tt_%(dmtype)s_pt_2_%(wp)s_qcd_uncert_up' % vars())
    wsptools.SafeWrapHist(w, ['pt_2_bounded'], hist_down, name='tt_%(dmtype)s_pt_2_%(wp)s_qcd_uncert_down' % vars()) 

    w.factory('expr::tt_%(dmtype)s_pt_2_%(wp)s_qcd_up("1.+@0/@1",tt_%(dmtype)s_pt_2_%(wp)s_qcd_uncert_up,tt_%(dmtype)s_pt_2_%(wp)s_qcd_uncert_nom)' % vars())
    w.factory('expr::tt_%(dmtype)s_pt_2_%(wp)s_qcd_down("1.-@0/@1",tt_%(dmtype)s_pt_2_%(wp)s_qcd_uncert_down,tt_%(dmtype)s_pt_2_%(wp)s_qcd_uncert_nom)' % vars())

    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_stat_pt2_up("(@0!=0)*@1*(@2*@3*@4*@5 + (1.-@3)) + (@0==0)*(@3*@5 + (1.-@3))*@1", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(wp)s_fracs_qcd, tt_%(dmtype)s_pt_2_%(wp)s_qcd_up, tt_%(dmtype)s_%(wp)s_qcd_ss_corr)' % vars())
    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_stat_pt2_down("(@0!=0)*@1*(@2*@3*@4*@5 + (1.-@3)) + (@0==0)*(@3*@5 + (1.-@3))*@1", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(wp)s_fracs_qcd, tt_%(dmtype)s_pt_2_%(wp)s_qcd_down, tt_%(dmtype)s_%(wp)s_qcd_ss_corr)' % vars())

    # systematic uncertainty from comparing data to prediction in aiso2 region in bins of tau1 and tau2 dm's
    if dmtype != 'dmbins':
      wsptools.SafeWrapHist(w, ['mvadm','mvadm2'], GetFromTFile((loc+'fakefactor_fits_tt_%(wp)s_2017.root:%(dmtype)s_os_dm_uncert_qcd' % vars()).replace('bins','')), name='%(dmtype)s_os_dm_uncert_qcd' % vars())


      w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_syst_up("(@0!=0)*@1*(@2*@3*@4*@5 + (1.-@3)) + (@0==0)*(@3*@5 + (1.-@3))*@1", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(wp)s_fracs_qcd, %(dmtype)s_os_dm_uncert_qcd, tt_%(dmtype)s_%(wp)s_qcd_ss_corr)'  % vars())
      w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_syst_down("(@0!=0)*@1*(@2*@3*(2.-@4)*@5 + (1.-@3)) + (@0==0)*(@3*@5 + (1.-@3))*@1", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(wp)s_fracs_qcd, %(dmtype)s_os_dm_uncert_qcd, tt_%(dmtype)s_%(wp)s_qcd_ss_corr)'  % vars())

    # wjets and ttbar systematics from comparing the inclusive fakefactors derived from data for QCD events with the MC fake factors predicted in MC for wjets and ttbar

    for x in ['qcd','wjets_mc','ttbar_mc']:
      func = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_2017.root:inclusive_inclusive_pt_1_ff_%(x)s_fit' % vars())
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
w.writeToFile('fakefactors_ws_2017.root')
w.Delete()
