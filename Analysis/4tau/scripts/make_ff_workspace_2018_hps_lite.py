#!/usr/bin/env python
import ROOT
import imp
import json
from array import array
import numpy as np
import subprocess
import re

cmssw_base = subprocess.check_output('echo $CMSSW_BASE', shell=True).replace('\n','')

wsptools = imp.load_source('wsptools', '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/workspaceTools.py' % vars())

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


################################################
### et channel ####
################################################

w = ROOT.RooWorkspace('w')

ip_sig_cut = '1.5'
wps = ['medium']

for wp in wps:

  # get fractions

  loc = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/hpsdm_ff_deeptauV2p1_2018_et/' % vars()

  histsToWrap = [(loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets0_os_qcd' % vars(), 'et_%(wp)s_fracs_njets0_os_qcd' % vars()),
                 (loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets1_os_qcd' % vars(), 'et_%(wp)s_fracs_njets1_os_qcd' % vars()),
                 (loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets2_os_qcd' % vars(), 'et_%(wp)s_fracs_njets2_os_qcd' % vars()),
                 (loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets0_ss_qcd' % vars(), 'et_%(wp)s_fracs_njets0_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets1_ss_qcd' % vars(), 'et_%(wp)s_fracs_njets1_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets2_ss_qcd' % vars(), 'et_%(wp)s_fracs_njets2_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets0_os_wjets' % vars(), 'et_%(wp)s_fracs_njets0_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets1_os_wjets' % vars(), 'et_%(wp)s_fracs_njets1_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets2_os_wjets' % vars(), 'et_%(wp)s_fracs_njets2_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets0_ss_wjets' % vars(), 'et_%(wp)s_fracs_njets0_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets1_ss_wjets' % vars(), 'et_%(wp)s_fracs_njets1_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets2_ss_wjets' % vars(), 'et_%(wp)s_fracs_njets2_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets0_os_ttbar' % vars(), 'et_%(wp)s_fracs_njets0_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets1_os_ttbar' % vars(), 'et_%(wp)s_fracs_njets1_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets2_os_ttbar' % vars(), 'et_%(wp)s_fracs_njets2_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets0_ss_ttbar' % vars(), 'et_%(wp)s_fracs_njets0_ss_ttbar' % vars()),
                 (loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets1_ss_ttbar' % vars(), 'et_%(wp)s_fracs_njets1_ss_ttbar' % vars()),
                 (loc + 'fakefactor_fits_et_%(wp)s_2018.root:et_fracs_njets2_ss_ttbar' % vars(), 'et_%(wp)s_fracs_njets2_ss_ttbar' % vars())
                ]

  for task in histsToWrap:
     wsptools.SafeWrapHist(
       w, ['expr::mt_max120("min(119.9,@0)",mt[0])'],
       GetFromTFile(task[0]),
       name=task[1])

  for i in ['qcd','wjets','ttbar']:
    w.factory('expr::et_%(wp)s_fracs_%(i)s_nom("(@0!=0)*((@1==0)*(@2)+(@1==1)*@3+(@1>1)*(@4))+(@0==0)*((@1==0)*(@5)+(@1==1)*@6+(@1>1)*(@7))", os[1],njets[0],et_%(wp)s_fracs_njets0_os_%(i)s,et_%(wp)s_fracs_njets1_os_%(i)s,et_%(wp)s_fracs_njets2_os_%(i)s,et_%(wp)s_fracs_njets0_ss_%(i)s,et_%(wp)s_fracs_njets1_ss_%(i)s,et_%(wp)s_fracs_njets2_ss_%(i)s)' % vars())

    # if the fractions are set by the user then overwrite these values
    w.factory('expr::et_%(wp)s_fracs_%(i)s("(@0>=0)*@0 + (@0<0)*@1", %(i)s_frac[-1], et_%(wp)s_fracs_%(i)s_nom)' % vars())

  # pT dependent fits

  dm_bins = [
              'dm0',
              'dm1',
              'dm10',
              'dm11',
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
  }

  # get all fitted functions for raw fake factors and statistical uncertainties from fit uncertainty bands
  for dm in dm_bins:
    for njet in ['0','1','2','0_crosstrg','1_crosstrg','2_crosstrg']:
      func_qcd = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:%(dm)s_njets%(njet)s_pt_2_ff_qcd_fit' % vars())
      func_wjets = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:%(dm)s_njets%(njet)s_pt_2_ff_wjets_fit' % vars())
      func_qcd_str = str(func_qcd.GetExpFormula('p')).replace('x','@0').replace(',false','')
      func_wjets_str = str(func_wjets.GetExpFormula('p')).replace('x','@0').replace(',false','')


      # QCD set max at 100, W+jets at 140, unless it is a cross trigger fit, then set maximum as 80
      w.factory('expr::pt_bounded140("max(min(139.9,@0),20)",pt[0])' % vars())
      w.factory('expr::pt_bounded100("max(min(99.9,@0),20)",pt[0])' % vars())
      w.factory('expr::pt_bounded80("max(min(79.9,@0),20)",pt[0])' % vars())

      if 'crosstrg' in njet:
        w.factory('expr::et_%(dm)s_njets%(njet)s_%(wp)s_qcd_fit("max(%(func_qcd_str)s,0.)",pt_bounded80)' % vars())
        w.factory('expr::et_%(dm)s_njets%(njet)s_%(wp)s_wjets_fit("max(%(func_wjets_str)s,0.)",pt_bounded80)' % vars())
      else:
        w.factory('expr::et_%(dm)s_njets%(njet)s_%(wp)s_qcd_fit("max(%(func_qcd_str)s,0.)",pt_bounded100)' % vars())
        w.factory('expr::et_%(dm)s_njets%(njet)s_%(wp)s_wjets_fit("max(%(func_wjets_str)s,0.)",pt_bounded140)' % vars())

      # get stat uncertainties
      hist_w_nom = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:%(dm)s_njets%(njet)s_pt_2_ff_wjets_uncert' % vars())
      hist_qcd_nom = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:%(dm)s_njets%(njet)s_pt_2_ff_qcd_uncert' % vars())
      (w_uncert1_up, w_uncert1_down, w_uncert2_up, w_uncert_2_down) = wsptools.SplitUncert(hist_w_nom)
      (qcd_uncert1_up, qcd_uncert1_down, qcd_uncert2_up, qcd_uncert_2_down) = wsptools.SplitUncert(hist_qcd_nom)

      if 'crosstrg' not in njet:
        wsptools.SafeWrapHist(w, ['pt_bounded100'], hist_qcd_nom, name='et_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_nom' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded100'], qcd_uncert1_up, name='et_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert1_up' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded100'], qcd_uncert2_up, name='et_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert2_up' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded140'], hist_w_nom, name='et_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_nom' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded140'], w_uncert1_up, name='et_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert1_up' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded140'], w_uncert2_up, name='et_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert2_up' % vars())
      else:
        wsptools.SafeWrapHist(w, ['pt_bounded80'], hist_qcd_nom, name='et_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_nom' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded80'], qcd_uncert1_up, name='et_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert1_up' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded80'], qcd_uncert2_up, name='et_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert2_up' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded80'], hist_w_nom, name='et_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_nom' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded80'], w_uncert1_up, name='et_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert1_up' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded80'], w_uncert2_up, name='et_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert2_up' % vars())

      njets_cut = cutsmap_njets[njet] % vars()
      dm_cut    = cutsmap_dm[dm] % vars()

    for njet in ['0','1','2']:
      njets_cut = cutsmap_njets[njet] % vars()
      njets_cut_cross = cutsmap_njets[njet+'_crosstrg'] % vars()

      w.factory('expr::et_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert1_up("(%(njets_cut)s&&%(dm_cut)s)*(@4/@5) + (%(njets_cut_cross)s&&%(dm_cut)s)*(@6/@7) + (((%(njets_cut)s||%(njets_cut_cross)s)&&%(dm_cut)s)==0)",njets[0],dm[1],ipsig[0],pass_single[1],et_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert1_up,et_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_nom, et_%(dm)s_njets%(njet)s_crosstrg_%(wp)s_qcd_uncert1_up,et_%(dm)s_njets%(njet)s_crosstrg_%(wp)s_qcd_uncert_nom)' % vars())
      w.factory('expr::et_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert2_up("(%(njets_cut)s&&%(dm_cut)s)*(@4/@5) + (%(njets_cut_cross)s&&%(dm_cut)s)*(@6/@7) + (((%(njets_cut)s||%(njets_cut_cross)s)&&%(dm_cut)s)==0)",njets[0],dm[1],ipsig[0],pass_single[1],et_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert2_up,et_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_nom, et_%(dm)s_njets%(njet)s_crosstrg_%(wp)s_qcd_uncert2_up,et_%(dm)s_njets%(njet)s_crosstrg_%(wp)s_qcd_uncert_nom)' % vars())

      w.factory('expr::et_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert1_up("(%(njets_cut)s&&%(dm_cut)s)*(@4/@5) + (%(njets_cut_cross)s&&%(dm_cut)s)*(@6/@7) + (((%(njets_cut)s||%(njets_cut_cross)s)&&%(dm_cut)s)==0)",njets[0],dm[1],ipsig[0],pass_single[1],et_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert1_up,et_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_nom, et_%(dm)s_njets%(njet)s_crosstrg_%(wp)s_wjets_uncert1_up,et_%(dm)s_njets%(njet)s_crosstrg_%(wp)s_wjets_uncert_nom)' % vars())
      w.factory('expr::et_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert2_up("(%(njets_cut)s&&%(dm_cut)s)*(@4/@5) + (%(njets_cut_cross)s&&%(dm_cut)s)*(@6/@7) + (((%(njets_cut)s||%(njets_cut_cross)s)&&%(dm_cut)s)==0)",njets[0],dm[1],ipsig[0],pass_single[1],et_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert2_up,et_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_nom, et_%(dm)s_njets%(njet)s_crosstrg_%(wp)s_wjets_uncert2_up,et_%(dm)s_njets%(njet)s_crosstrg_%(wp)s_wjets_uncert_nom)' % vars())

      w.factory('expr::et_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert1_down("2. - @0", et_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert1_up)' % vars())
      w.factory('expr::et_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert2_down("2. - @0", et_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert2_up)' % vars())
      w.factory('expr::et_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert1_down("2. - @0", et_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert1_up)' % vars())
      w.factory('expr::et_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert2_down("2. - @0", et_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert2_up)' % vars())

    # get ttbar FFs inclusive in njets and also get W data and MC SFs to be used to define uncertainties

    func_ttbar = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:%(dm)s_inclusive_pt_2_ff_ttbar_mc_fit' % vars())
    func_wjets = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:%(dm)s_inclusive_pt_2_ff_wjets_fit' % vars())
    func_wjets_mc = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:%(dm)s_inclusive_pt_2_ff_wjets_mc_fit' % vars())
    func_ttbar_str = str(func_ttbar.GetExpFormula('p')).replace('x','@0').replace(',false','')
    func_wjets_str = str(func_wjets.GetExpFormula('p')).replace('x','@0').replace(',false','')
    func_wjets_mc_str = str(func_wjets_mc.GetExpFormula('p')).replace('x','@0').replace(',false','')

    w.factory('expr::pt_bounded140("max(min(139.9,@0),20)",pt[0])' % vars())

    w.factory('expr::et_%(dm)s_inclusive_%(wp)s_ttbar_fit("max(%(func_ttbar_str)s,0.)",pt_bounded100)' % vars())
    w.factory('expr::et_%(dm)s_inclusive_%(wp)s_wjets_fit("max(%(func_wjets_str)s,0.)",pt_bounded140)' % vars())
    w.factory('expr::et_%(dm)s_inclusive_%(wp)s_wjets_mc_fit("max(%(func_wjets_mc_str)s,0.)",pt_bounded140)' % vars())

    # get stat uncertainties on ttbar
    hist_nom = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:%(dm)s_inclusive_pt_2_ff_ttbar_mc_uncert' % vars())

    (uncert1_up, uncert1_down, uncert2_up, uncert_2_down) = wsptools.SplitUncert(hist_nom)
    wsptools.SafeWrapHist(w, ['pt_bounded140'], hist_nom, name='et_%(dm)s_inclusive_%(wp)s_ttbar_uncert_nom' % vars())
    wsptools.SafeWrapHist(w, ['pt_bounded140'], uncert1_up, name='et_%(dm)s_inclusive_%(wp)s_ttbar_uncert1_hist_up' % vars())
    wsptools.SafeWrapHist(w, ['pt_bounded140'], uncert2_up, name='et_%(dm)s_inclusive_%(wp)s_ttbar_uncert2_hist_up' % vars())

    dm_cut    = (cutsmap_dm[dm] % vars()).replace('@1','@0').replace('@2','@1')

    w.factory('expr::et_%(dm)s_inclusive_%(wp)s_ttbar_uncert1_up("(%(dm_cut)s)*(@2/@3) + ((%(dm_cut)s)==0)",dm[1],ipsig[0],et_%(dm)s_inclusive_%(wp)s_ttbar_uncert1_hist_up,et_%(dm)s_inclusive_%(wp)s_ttbar_uncert_nom)' % vars())
    w.factory('expr::et_%(dm)s_inclusive_%(wp)s_ttbar_uncert2_up("(%(dm_cut)s)*(@2/@3) + ((%(dm_cut)s)==0)",dm[1],ipsig[0],et_%(dm)s_inclusive_%(wp)s_ttbar_uncert2_hist_up,et_%(dm)s_inclusive_%(wp)s_ttbar_uncert_nom)' % vars())

    w.factory('expr::et_%(dm)s_inclusive_%(wp)s_ttbar_uncert1_down("2. - @0", et_%(dm)s_inclusive_%(wp)s_ttbar_uncert1_up)' % vars())
    w.factory('expr::et_%(dm)s_inclusive_%(wp)s_ttbar_uncert2_down("2. - @0", et_%(dm)s_inclusive_%(wp)s_ttbar_uncert2_up)' % vars())

  # combine uncertainties into functions

  ff_eqn_tot = '(@0>=0.5)*((@2==0)*((@1==0)*(@3)+(@1==1)*(@4)+(@1==10)*(@5)+(@1==11)*(@6)) + (@2==1)*((@1==0)*(@7)+(@1==1)*(@8)+(@1==10)*(@9)+(@1==11)*(@10)) + (@2>1)*((@1==0)*(@11)+(@1==1)*(@12)+(@1==10)*(@13)+(@1==11)*(@14)))' % vars()

  ff_eqn_tot_cross = '(@0<0.5)*((@2==0)*((@1==0)*(@15)+(@1==1)*(@16)+(@1==10)*(@17)+(@1==11)*(@18)) + (@2==1)*((@1==0)*(@19)+(@1==1)*(@20)+(@1==10)*(@21)+(@1==11)*(@22)) + (@2>1)*((@1==0)*(@23)+(@1==1)*(@24)+(@1==10)*(@25)+(@1==11)*(@26)))' % vars()
  
  w.factory('expr::ff_et_%(wp)s_dmbins_qcd_raw("%(ff_eqn_tot)s + %(ff_eqn_tot_cross)s", pass_single[1], dm[1], njets[0], et_dm0_njets0_%(wp)s_qcd_fit, et_dm1_njets0_%(wp)s_qcd_fit, et_dm10_njets0_%(wp)s_qcd_fit, et_dm11_njets0_%(wp)s_qcd_fit, et_dm0_njets1_%(wp)s_qcd_fit, et_dm1_njets1_%(wp)s_qcd_fit, et_dm10_njets1_%(wp)s_qcd_fit, et_dm11_njets1_%(wp)s_qcd_fit, et_dm0_njets2_%(wp)s_qcd_fit, et_dm1_njets2_%(wp)s_qcd_fit, et_dm10_njets2_%(wp)s_qcd_fit, et_dm11_njets2_%(wp)s_qcd_fit, et_dm0_njets0_crosstrg_%(wp)s_qcd_fit, et_dm1_njets0_crosstrg_%(wp)s_qcd_fit, et_dm10_njets0_crosstrg_%(wp)s_qcd_fit, et_dm11_njets0_crosstrg_%(wp)s_qcd_fit, et_dm0_njets1_crosstrg_%(wp)s_qcd_fit, et_dm1_njets1_crosstrg_%(wp)s_qcd_fit, et_dm10_njets1_crosstrg_%(wp)s_qcd_fit, et_dm11_njets1_crosstrg_%(wp)s_qcd_fit, et_dm0_njets2_crosstrg_%(wp)s_qcd_fit, et_dm1_njets2_crosstrg_%(wp)s_qcd_fit, et_dm10_njets2_crosstrg_%(wp)s_qcd_fit, et_dm11_njets2_crosstrg_%(wp)s_qcd_fit )' % vars())

  w.factory(('expr::ff_et_%(wp)s_dmbins_qcd_raw("%(ff_eqn_tot)s + %(ff_eqn_tot_cross)s", pass_single[1], dm[1], njets[0], et_dm0_njets0_%(wp)s_qcd_fit, et_dm1_njets0_%(wp)s_qcd_fit, et_dm10_njets0_%(wp)s_qcd_fit, et_dm11_njets0_%(wp)s_qcd_fit, et_dm0_njets1_%(wp)s_qcd_fit, et_dm1_njets1_%(wp)s_qcd_fit, et_dm10_njets1_%(wp)s_qcd_fit, et_dm11_njets1_%(wp)s_qcd_fit, et_dm0_njets2_%(wp)s_qcd_fit, et_dm1_njets2_%(wp)s_qcd_fit, et_dm10_njets2_%(wp)s_qcd_fit, et_dm11_njets2_%(wp)s_qcd_fit, et_dm0_njets0_crosstrg_%(wp)s_qcd_fit, et_dm1_njets0_crosstrg_%(wp)s_qcd_fit, et_dm10_njets0_crosstrg_%(wp)s_qcd_fit, et_dm11_njets0_crosstrg_%(wp)s_qcd_fit, et_dm0_njets1_crosstrg_%(wp)s_qcd_fit, et_dm1_njets1_crosstrg_%(wp)s_qcd_fit, et_dm10_njets1_crosstrg_%(wp)s_qcd_fit, et_dm11_njets1_crosstrg_%(wp)s_qcd_fit, et_dm0_njets2_crosstrg_%(wp)s_qcd_fit, et_dm1_njets2_crosstrg_%(wp)s_qcd_fit, et_dm10_njets2_crosstrg_%(wp)s_qcd_fit, et_dm11_njets2_crosstrg_%(wp)s_qcd_fit )' % vars()).replace('qcd','wjets'))


  w.factory('expr::ff_et_%(wp)s_dmbins_ttbar_raw("(@0==0)*(@1)+(@0==1)*(@2)+(@0==10)*(@3)+(@0==11)*(@4)", dm[1], et_dm0_inclusive_%(wp)s_ttbar_fit, et_dm1_inclusive_%(wp)s_ttbar_fit, et_dm10_inclusive_%(wp)s_ttbar_fit, et_dm11_inclusive_%(wp)s_ttbar_fit )' % vars())

# systematic shifts for ttbar


  w.factory('expr::ff_et_%(wp)s_dmbins_ttbar_up("(@0==0)*(@1/@2)+(@0==1)*(@3/@4)+(@0==10)*(@5/@6)+(@0==11)*(@7/@8)", dm[1], et_dm0_inclusive_%(wp)s_wjets_fit,et_dm0_inclusive_%(wp)s_wjets_mc_fit,et_dm1_inclusive_%(wp)s_wjets_fit,et_dm1_inclusive_%(wp)s_wjets_mc_fit,et_dm10_inclusive_%(wp)s_wjets_fit,et_dm10_inclusive_%(wp)s_wjets_mc_fit,et_dm11_inclusive_%(wp)s_wjets_fit,et_dm11_inclusive_%(wp)s_wjets_mc_fit)' % vars())

  w.factory('expr::ff_et_%(wp)s_dmbins_ttbar_down("1/@0",ff_et_%(wp)s_dmbins_ttbar_up)' % vars())

  # get the ttbar met corrction
  # met correction
  w.factory('expr::met_var_w_bounded("max(min(3.5,@0),-3.5)",met_var_w[0])' % vars())
  func = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:met_closure_ttbar_mc_fit' % vars())
  corr_str = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
  w.factory('expr::et_%(wp)s_ttbar_met_corr("%(corr_str)s",met_var_w_bounded)' % vars())

  # apply corrections to ttbar FFs
  for dmtype in ['dm']:
    dmname = dmtype.replace('dm','dmbins')
    w.factory('expr::ff_et_%(wp)s_%(dmname)s_ttbar("@0*@1*@2", ff_et_%(wp)s_%(dmname)s_ttbar_raw, et_%(wp)s_ttbar_met_corr, ff_et_%(wp)s_%(dmname)s_ttbar_up)' % vars())

  # get W+jets corrections
  for dmtype in ['dm']:

    dmname = dmtype.replace('dm','dmbins')

    # met correction
    w.factory('expr::met_var_w_bounded("max(min(3.5,@0),-3.5)",met_var_w[0])' % vars())
    for njet in ['0','1','2']:
      func = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:%(dmtype)s_met_closure_njet%(njet)s_wjets_fit' % vars())
      corr_str = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
      w.factory('expr::et_%(dmname)s_%(wp)s_wjets_met_corr_njet%(njet)s("%(corr_str)s",met_var_w_bounded)' % vars())
    w.factory('expr::et_%(dmname)s_%(wp)s_wjets_met_corr("(@0==0)*(@1) + (@0==1)*(@2) + (@0>1)*(@3)",njets[0],et_%(dmname)s_%(wp)s_wjets_met_corr_njet0,et_%(dmname)s_%(wp)s_wjets_met_corr_njet1,et_%(dmname)s_%(wp)s_wjets_met_corr_njet2)' % vars())

    # pt correction
    w.factory('expr::e_pt_bounded90("min(@0,89.9)",e_pt[0])' % vars())
    for njet in ['0','1','2']:
      func = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:%(dmtype)s_pt_1_closure_njet%(njet)s_wjets_fit' % vars())
      corr_str = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
      w.factory('expr::et_%(dmname)s_%(wp)s_wjets_e_pt_corr_njet%(njet)s("%(corr_str)s",e_pt_bounded90)' % vars())
    w.factory('expr::et_%(dmname)s_%(wp)s_wjets_e_pt_corr("(@4<33)+(@4>33)*((@0==0)*(@1) + (@0==1)*(@2) + (@0>1)*(@3))",njets[0],et_%(dmname)s_%(wp)s_wjets_e_pt_corr_njet0,et_%(dmname)s_%(wp)s_wjets_e_pt_corr_njet1,et_%(dmname)s_%(wp)s_wjets_e_pt_corr_njet2,e_pt[0])' % vars())

    w.factory('expr::et_%(dmname)s_%(wp)s_wjets_met_corr_up("@0*@0",et_%(dmname)s_%(wp)s_wjets_met_corr)' % vars())
    w.factory('expr::et_%(dmname)s_%(wp)s_wjets_met_corr_down("@0/@0",et_%(dmname)s_%(wp)s_wjets_met_corr)' % vars())

    w.factory('expr::et_%(dmname)s_%(wp)s_wjets_e_pt_corr_up("@0*@0",et_%(dmname)s_%(wp)s_wjets_e_pt_corr)' % vars())
    w.factory('expr::et_%(dmname)s_%(wp)s_wjets_e_pt_corr_down("@0/@0",et_%(dmname)s_%(wp)s_wjets_e_pt_corr)' % vars())
 
    # mt_1 correction (this is now as a function of the W pT and njets)
    #func = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:%(dmtype)s_mt_corr_wjets_mc_fit' % vars())
    #func_mt_corr = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')

    # for now adding this by hand until evenythign is run
    w.factory('expr::WpT_bounded("min(199.99,@0)",WpT[0])' % vars())

    for njet in ['0','1','2']:
      func = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:%(dmtype)s_WpT_corr_njet%(njet)s_wjets_mc_fit' % vars())
      corr_str = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
      w.factory('expr::et_%(dmname)s_%(wp)s_wjets_mt_corr_njet%(njet)s("%(corr_str)s",WpT_bounded)' % vars())
    w.factory('expr::et_%(dmname)s_%(wp)s_wjets_mt_corr("(@4>=70)+(@4<70)*((@0==0)*(@1) + (@0==1)*(@2) + (@0>1)*(@3))",njets[0],et_%(dmname)s_%(wp)s_wjets_mt_corr_njet0,et_%(dmname)s_%(wp)s_wjets_mt_corr_njet1,et_%(dmname)s_%(wp)s_wjets_mt_corr_njet2,mt[0])' % vars())


    w.factory('expr::mvis_bounded100("min(99.9,@0)",mvis[50])' % vars())
    # since we dont apply this correction anymore use a dummy function that is always = 1
    w.factory('expr::et_%(dmname)s_%(wp)s_wjets_mvis_corr("1",mvis_bounded100)' % vars())

    # apply corrections to raw W+jets FFs
    w.factory('expr::ff_et_%(wp)s_%(dmname)s_wjets("@0*@1*@2*@3*((@5<50)*@4 +(@5>=50))", ff_et_%(wp)s_%(dmname)s_wjets_raw , et_%(dmname)s_%(wp)s_wjets_met_corr, et_%(dmname)s_%(wp)s_wjets_e_pt_corr, et_%(dmname)s_%(wp)s_wjets_mt_corr, et_%(dmname)s_%(wp)s_wjets_mvis_corr, mt)' % vars())

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_wjets_corr_stat_met_up("@0*@1*@2*@3*((@5<50)*@4 +(@5>=50))", ff_et_%(wp)s_%(dmname)s_wjets_raw , et_%(dmname)s_%(wp)s_wjets_met_corr_up, et_%(dmname)s_%(wp)s_wjets_e_pt_corr, et_%(dmname)s_%(wp)s_wjets_mt_corr, et_%(dmname)s_%(wp)s_wjets_mvis_corr, mt)' % vars())
    w.factory('expr::ff_et_%(wp)s_%(dmname)s_wjets_corr_stat_met_down("@0*@1*@2*@3*((@5<50)*@4 +(@5>=50))", ff_et_%(wp)s_%(dmname)s_wjets_raw , et_%(dmname)s_%(wp)s_wjets_met_corr_down, et_%(dmname)s_%(wp)s_wjets_e_pt_corr, et_%(dmname)s_%(wp)s_wjets_mt_corr, et_%(dmname)s_%(wp)s_wjets_mvis_corr, mt)' % vars())

    # apply systematic shifts

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_wjets_corr_stat_e_pt_up("@0*@1*@2*@3*((@5<50)*@4 +(@5>=50))", ff_et_%(wp)s_%(dmname)s_wjets_raw , et_%(dmname)s_%(wp)s_wjets_met_corr, et_%(dmname)s_%(wp)s_wjets_e_pt_corr_up, et_%(dmname)s_%(wp)s_wjets_mt_corr, et_%(dmname)s_%(wp)s_wjets_mvis_corr, mt)' % vars())
    w.factory('expr::ff_et_%(wp)s_%(dmname)s_wjets_corr_stat_e_pt_down("@0*@1*@2*@3*((@5<50)*@4 +(@5>=50))", ff_et_%(wp)s_%(dmname)s_wjets_raw , et_%(dmname)s_%(wp)s_wjets_met_corr, et_%(dmname)s_%(wp)s_wjets_e_pt_corr_down, et_%(dmname)s_%(wp)s_wjets_mt_corr, et_%(dmname)s_%(wp)s_wjets_mvis_corr, mt)' % vars())

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_wjets_corr_mt_up("@0*@1*@2*@3*@3*((@5<50)*@4 +(@5>=50))", ff_et_%(wp)s_%(dmname)s_wjets_raw , et_%(dmname)s_%(wp)s_wjets_met_corr, et_%(dmname)s_%(wp)s_wjets_e_pt_corr, et_%(dmname)s_%(wp)s_wjets_mt_corr, et_%(dmname)s_%(wp)s_wjets_mvis_corr, mt)' % vars())

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_wjets_corr_mt_down("@0*@1*@2*((@5<50)*@4 +(@5>=50))", ff_et_%(wp)s_%(dmname)s_wjets_raw , et_%(dmname)s_%(wp)s_wjets_met_corr, et_%(dmname)s_%(wp)s_wjets_e_pt_corr, et_%(dmname)s_%(wp)s_wjets_mt_corr, et_%(dmname)s_%(wp)s_wjets_mvis_corr, mt)' % vars())

  # get QCD corrections
  for dmtype in ['dm']:

    dmname = dmtype.replace('dm','dmbins')

    w.factory('expr::met_var_qcd_bounded("max(min(1.5,@0),-2.5)",met_var_qcd[0])' % vars())
    for njet in ['0','1']:
      func = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:%(dmtype)s_met_closure_njet%(njet)s_qcd_fit' % vars())
      corr_str = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
      w.factory('expr::et_%(dmname)s_%(wp)s_qcd_met_corr_njet%(njet)s("%(corr_str)s",met_var_qcd_bounded)' % vars())

    w.factory('expr::et_%(dmname)s_%(wp)s_qcd_met_corr("(@0==0)*(@1) + (@0>0)*(@2)",njets[0],et_%(dmname)s_%(wp)s_qcd_met_corr_njet0,et_%(dmname)s_%(wp)s_qcd_met_corr_njet1)' % vars())

    # pt correction
    w.factory('expr::e_pt_bounded50("min(@0,49.9)",e_pt[0])' % vars())
#    for njet in ['0','1']:
#      func = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:%(dmtype)s_pt_1_closure_njet%(njet)s_qcd_fit' % vars())
#      corr_str = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
#      w.factory('expr::et_%(dmname)s_%(wp)s_qcd_e_pt_corr_njet%(njet)s("%(corr_str)s",e_pt_bounded50)' % vars())

# new from here
    w.factory('expr::e_pt_bounded70("min(@0,69.9)",e_pt[0])' % vars())
    for njet in ['0','1']:
      for pt in ['20to30','30to40','gt40']:
        func = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:%(dmtype)s_pt_1_closure_njet%(njet)s_taupt%(pt)s_qcd_fit' % vars())
        corr_str = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
        w.factory('expr::et_%(dmname)s_%(wp)s_qcd_e_pt_corr_njet%(njet)s_taupt%(pt)s("%(corr_str)s",e_pt_bounded70)' % vars())

      w.factory('expr::et_%(dmname)s_%(wp)s_qcd_e_pt_corr_njet%(njet)s("((@0<30)*(@1) + (@0>=30&&@0<40)*(@2) + (@0>=40)*(@3))",pt[0], et_%(dmname)s_%(wp)s_qcd_e_pt_corr_njet%(njet)s_taupt20to30, et_%(dmname)s_%(wp)s_qcd_e_pt_corr_njet%(njet)s_taupt30to40, et_%(dmname)s_%(wp)s_qcd_e_pt_corr_njet%(njet)s_tauptgt40)' % vars())
# end of new

    w.factory('expr::et_%(dmname)s_%(wp)s_qcd_e_pt_corr("(@3<33)+(@3>33)*((@0==0)*(@1) + (@0>0)*(@2))",njets[0],et_%(dmname)s_%(wp)s_qcd_e_pt_corr_njet0,et_%(dmname)s_%(wp)s_qcd_e_pt_corr_njet1,e_pt[0])' % vars())

    w.factory('expr::et_%(dmname)s_%(wp)s_qcd_met_corr_up("@0*@0",et_%(dmname)s_%(wp)s_qcd_met_corr)' % vars())
    w.factory('expr::et_%(dmname)s_%(wp)s_qcd_met_corr_down("@0/@0",et_%(dmname)s_%(wp)s_qcd_met_corr)' % vars())

    w.factory('expr::et_%(dmname)s_%(wp)s_qcd_e_pt_corr_up("@0*@0",et_%(dmname)s_%(wp)s_qcd_e_pt_corr)' % vars())
    w.factory('expr::et_%(dmname)s_%(wp)s_qcd_e_pt_corr_down("@0/@0",et_%(dmname)s_%(wp)s_qcd_e_pt_corr)' % vars())

    # aiso->iso correction
    func = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:%(dmtype)s_iso_closure_qcd_fit' % vars())
    func_iso_corr = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
    w.factory('expr::iso_bounded0p5("min(0.499,@0)",e_iso[0])' % vars())
    w.factory('expr::et_%(dmname)s_%(wp)s_qcd_iso_corr("%(func_iso_corr)s",iso_bounded0p5)' % vars())

    w.factory('expr::e_pt_bounded80("min(49.99,@0)",e_pt)' % vars())
    # OS/SS correction
    for njet in ['0','1']:
      func = GetFromTFile(loc+'fakefactor_fits_et_%(wp)s_2018.root:%(dmtype)s_osss_closure_njet%(njet)s_qcd_fit' % vars())
      func_osss_corr = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
      w.factory('expr::et_%(dmname)s_%(wp)s_qcd_osss_corr_njet%(njet)s("%(func_osss_corr)s",e_pt_bounded50)' % vars())
    w.factory('expr::et_%(dmname)s_%(wp)s_qcd_osss_corr("(@0==0)*(@1)+(@0>0)*(@2)",njets[0],et_%(dmname)s_%(wp)s_qcd_osss_corr_njet0,et_%(dmname)s_%(wp)s_qcd_osss_corr_njet1)' % vars())

    # apply corrections to raw QCD FFs
    w.factory('expr::ff_et_%(wp)s_%(dmname)s_qcd("(@5!=0)*@0*@1*@2*@3*@4 + (@5==0)*@0*@1*@2*@3", ff_et_%(wp)s_%(dmname)s_qcd_raw , et_%(dmname)s_%(wp)s_qcd_met_corr, et_%(dmname)s_%(wp)s_qcd_e_pt_corr, et_%(dmname)s_%(wp)s_qcd_iso_corr, et_%(dmname)s_%(wp)s_qcd_osss_corr, os[1])' % vars())

    # apply systematic shifts

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_qcd_corr_stat_e_pt_up("(@5!=0)*@0*@1*@2*@3*@4 + (@5==0)*@0*@1*@2*@3", ff_et_%(wp)s_%(dmname)s_qcd_raw , et_%(dmname)s_%(wp)s_qcd_met_corr, et_%(dmname)s_%(wp)s_qcd_e_pt_corr_up, et_%(dmname)s_%(wp)s_qcd_iso_corr, et_%(dmname)s_%(wp)s_qcd_osss_corr, os[1])' % vars())
    w.factory('expr::ff_et_%(wp)s_%(dmname)s_qcd_corr_stat_e_pt_down("(@5!=0)*@0*@1*@2*@3*@4 + (@5==0)*@0*@1*@2*@3", ff_et_%(wp)s_%(dmname)s_qcd_raw , et_%(dmname)s_%(wp)s_qcd_met_corr, et_%(dmname)s_%(wp)s_qcd_e_pt_corr_down, et_%(dmname)s_%(wp)s_qcd_iso_corr, et_%(dmname)s_%(wp)s_qcd_osss_corr, os[1])' % vars())

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_qcd_corr_stat_met_up("(@5!=0)*@0*@1*@2*@3*@4 + (@5==0)*@0*@1*@2*@3", ff_et_%(wp)s_%(dmname)s_qcd_raw , et_%(dmname)s_%(wp)s_qcd_met_corr_up, et_%(dmname)s_%(wp)s_qcd_e_pt_corr, et_%(dmname)s_%(wp)s_qcd_iso_corr, et_%(dmname)s_%(wp)s_qcd_osss_corr, os[1])' % vars())
    w.factory('expr::ff_et_%(wp)s_%(dmname)s_qcd_corr_stat_met_down("(@5!=0)*@0*@1*@2*@3*@4 + (@5==0)*@0*@1*@2*@3", ff_et_%(wp)s_%(dmname)s_qcd_raw , et_%(dmname)s_%(wp)s_qcd_met_corr_down, et_%(dmname)s_%(wp)s_qcd_e_pt_corr, et_%(dmname)s_%(wp)s_qcd_iso_corr, et_%(dmname)s_%(wp)s_qcd_osss_corr, os[1])' % vars())

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_qcd_corr_syst_up("(@5!=0)*@0*@1*@2*@3*@3*@4*@4 + (@5==0)*@0*@1*@2*@3*@3", ff_et_%(wp)s_%(dmname)s_qcd_raw , et_%(dmname)s_%(wp)s_qcd_met_corr, et_%(dmname)s_%(wp)s_qcd_e_pt_corr, et_%(dmname)s_%(wp)s_qcd_iso_corr, et_%(dmname)s_%(wp)s_qcd_osss_corr, os[1])' % vars())

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_qcd_corr_syst_down("(@5!=0)*@0*@1*@2 + (@5==0)*@0*@1*@2", ff_et_%(wp)s_%(dmname)s_qcd_raw , et_%(dmname)s_%(wp)s_qcd_met_corr, et_%(dmname)s_%(wp)s_qcd_e_pt_corr, et_%(dmname)s_%(wp)s_qcd_iso_corr, et_%(dmname)s_%(wp)s_qcd_osss_corr, os[1])' % vars())

# scale FFs by their fractions

    w.factory('expr::ff_et_%(wp)s_%(dmname)s("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar)' % vars()) # the nominal ttbar is now shifted up by the uncertainty

# systematic uncertainties due to corrections

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_wjets_met_up("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets_corr_stat_met_up, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar)' % vars())
    w.factory('expr::ff_et_%(wp)s_%(dmname)s_wjets_met_down("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets_corr_stat_met_down, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar)' % vars())

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_wjets_l_pt_up("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets_corr_stat_e_pt_up, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar)' % vars())
    w.factory('expr::ff_et_%(wp)s_%(dmname)s_wjets_l_pt_down("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets_corr_stat_e_pt_down, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar)' % vars())

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_wjets_syst_up("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets_corr_mt_up, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar)' % vars())
    w.factory('expr::ff_et_%(wp)s_%(dmname)s_wjets_syst_down("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets_corr_mt_down, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar)' % vars())

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_qcd_met_up("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd_corr_stat_met_up, ff_et_%(wp)s_%(dmname)s_ttbar)' % vars())
    w.factory('expr::ff_et_%(wp)s_%(dmname)s_qcd_met_down("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd_corr_stat_met_down, ff_et_%(wp)s_%(dmname)s_ttbar)' % vars())

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_qcd_l_pt_up("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd_corr_stat_e_pt_up, ff_et_%(wp)s_%(dmname)s_ttbar)' % vars())
    w.factory('expr::ff_et_%(wp)s_%(dmname)s_qcd_l_pt_down("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd_corr_stat_e_pt_down, ff_et_%(wp)s_%(dmname)s_ttbar)' % vars())

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_qcd_syst_up("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd_corr_syst_up, ff_et_%(wp)s_%(dmname)s_ttbar)' % vars())
    w.factory('expr::ff_et_%(wp)s_%(dmname)s_qcd_syst_down("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd_corr_syst_down, ff_et_%(wp)s_%(dmname)s_ttbar)' % vars())

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_ttbar_syst_up("@0*@3 + @1*@4 + @2*@5*@6", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar,ff_et_%(wp)s_%(dmname)s_ttbar_up)' % vars()) # the up shift now has the correction applied twice

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_ttbar_syst_down("@0*@3 + @1*@4 + @2*@5*@6", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar, ff_et_%(wp)s_%(dmname)s_ttbar_down)' % vars()) # the down shift now has no corretion applied

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_ttbar_met_up("@0*@3 + @1*@4 + @2*@5*@6", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar,et_%(wp)s_ttbar_met_corr)' % vars())

    w.factory('expr::ff_et_%(wp)s_%(dmname)s_ttbar_met_down("@0*@3 + @1*@4 + @2*@5/@6", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar,et_%(wp)s_ttbar_met_corr)' % vars())

# produce statistical uncertainties
# merging cross trigger uncertainties to reduce number of systematics

    for dm in dm_bins:
      for njet in ['0','1','2']:

        w.factory('expr::ff_et_%(wp)s_%(dmname)s_qcd_stat_unc1_njet%(njet)s_%(dm)s_up("@0*@3 + @1*@4*@6 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar, et_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert1_up)' % vars())
        w.factory('expr::ff_et_%(wp)s_%(dmname)s_qcd_stat_unc1_njet%(njet)s_%(dm)s_down("@0*@3 + @1*@4*@6 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar, et_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert1_down)' % vars())

        w.factory('expr::ff_et_%(wp)s_%(dmname)s_qcd_stat_unc2_njet%(njet)s_%(dm)s_up("@0*@3 + @1*@4*@6 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar, et_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert2_up)' % vars())
        w.factory('expr::ff_et_%(wp)s_%(dmname)s_qcd_stat_unc2_njet%(njet)s_%(dm)s_down("@0*@3 + @1*@4*@6 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar, et_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert2_down)' % vars())

        w.factory('expr::ff_et_%(wp)s_%(dmname)s_wjets_stat_unc1_njet%(njet)s_%(dm)s_up("@0*@3*@6 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar, et_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert1_up)' % vars())
        w.factory('expr::ff_et_%(wp)s_%(dmname)s_wjets_stat_unc1_njet%(njet)s_%(dm)s_down("@0*@3*@6 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar, et_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert1_down)' % vars())

        w.factory('expr::ff_et_%(wp)s_%(dmname)s_wjets_stat_unc2_njet%(njet)s_%(dm)s_up("@0*@3*@6 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar, et_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert2_up)' % vars())
        w.factory('expr::ff_et_%(wp)s_%(dmname)s_wjets_stat_unc2_njet%(njet)s_%(dm)s_down("@0*@3*@6 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar, et_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert2_down)' % vars())

      w.factory('expr::ff_et_%(wp)s_%(dmname)s_ttbar_stat_unc1_%(dm)s_up("@0*@3 + @1*@4 + @2*@5*@6", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar, et_%(dm)s_inclusive_%(wp)s_ttbar_uncert1_up)' % vars())
      w.factory('expr::ff_et_%(wp)s_%(dmname)s_ttbar_stat_unc1_%(dm)s_down("@0*@3 + @1*@4 + @2*@5*@6", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar, et_%(dm)s_inclusive_%(wp)s_ttbar_uncert1_down)' % vars())

      w.factory('expr::ff_et_%(wp)s_%(dmname)s_ttbar_stat_unc2_%(dm)s_up("@0*@3 + @1*@4 + @2*@5*@6", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar, et_%(dm)s_inclusive_%(wp)s_ttbar_uncert2_up)' % vars())
      w.factory('expr::ff_et_%(wp)s_%(dmname)s_ttbar_stat_unc2_%(dm)s_down("@0*@3 + @1*@4 + @2*@5*@6", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_%(dmname)s_wjets, ff_et_%(wp)s_%(dmname)s_qcd, ff_et_%(wp)s_%(dmname)s_ttbar, et_%(dm)s_inclusive_%(wp)s_ttbar_uncert2_down)' % vars())

w.Print()
w.writeToFile('fakefactors_ws_et_hps_lite_2018.root')
w.Delete()


################################################
### mt channel ####
################################################

w = ROOT.RooWorkspace('w')

ip_sig_cut = '1.5'
wps = ['medium']

for wp in wps:

  # get fractions

  loc = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/hpsdm_ff_deeptauV2p1_2018_mt/' % vars()

  histsToWrap = [(loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets0_os_qcd' % vars(), 'mt_%(wp)s_fracs_njets0_os_qcd' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets1_os_qcd' % vars(), 'mt_%(wp)s_fracs_njets1_os_qcd' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets2_os_qcd' % vars(), 'mt_%(wp)s_fracs_njets2_os_qcd' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets0_ss_qcd' % vars(), 'mt_%(wp)s_fracs_njets0_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets1_ss_qcd' % vars(), 'mt_%(wp)s_fracs_njets1_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets2_ss_qcd' % vars(), 'mt_%(wp)s_fracs_njets2_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets0_os_wjets' % vars(), 'mt_%(wp)s_fracs_njets0_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets1_os_wjets' % vars(), 'mt_%(wp)s_fracs_njets1_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets2_os_wjets' % vars(), 'mt_%(wp)s_fracs_njets2_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets0_ss_wjets' % vars(), 'mt_%(wp)s_fracs_njets0_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets1_ss_wjets' % vars(), 'mt_%(wp)s_fracs_njets1_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets2_ss_wjets' % vars(), 'mt_%(wp)s_fracs_njets2_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets0_os_ttbar' % vars(), 'mt_%(wp)s_fracs_njets0_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets1_os_ttbar' % vars(), 'mt_%(wp)s_fracs_njets1_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets2_os_ttbar' % vars(), 'mt_%(wp)s_fracs_njets2_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets0_ss_ttbar' % vars(), 'mt_%(wp)s_fracs_njets0_ss_ttbar' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets1_ss_ttbar' % vars(), 'mt_%(wp)s_fracs_njets1_ss_ttbar' % vars()),
                 (loc + 'fakefactor_fits_mt_%(wp)s_2018.root:mt_fracs_njets2_ss_ttbar' % vars(), 'mt_%(wp)s_fracs_njets2_ss_ttbar' % vars())
                ]

  for task in histsToWrap:
     wsptools.SafeWrapHist(
       w, ['expr::mt_max120("min(119.9,@0)",mt[0])'],
       GetFromTFile(task[0]),
       name=task[1])

  for i in ['qcd','wjets','ttbar']:
    w.factory('expr::mt_%(wp)s_fracs_%(i)s_nom("(@0!=0)*((@1==0)*(@2)+(@1==1)*@3+(@1>1)*(@4))+(@0==0)*((@1==0)*(@5)+(@1==1)*@6+(@1>1)*(@7))", os[1],njets[0],mt_%(wp)s_fracs_njets0_os_%(i)s,mt_%(wp)s_fracs_njets1_os_%(i)s,mt_%(wp)s_fracs_njets2_os_%(i)s,mt_%(wp)s_fracs_njets0_ss_%(i)s,mt_%(wp)s_fracs_njets1_ss_%(i)s,mt_%(wp)s_fracs_njets2_ss_%(i)s)' % vars())

    # if the fractions are set by the user then overwrite these values
    w.factory('expr::mt_%(wp)s_fracs_%(i)s("(@0>=0)*@0 + (@0<0)*@1", %(i)s_frac[-1], mt_%(wp)s_fracs_%(i)s_nom)' % vars())

  # pT dependent fits

  dm_bins = [
              'dm0',
              'dm1',
              'dm10',
              'dm11',
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
  }

  # get all fitted functions for raw fake factors and statistical uncertainties from fit uncertainty bands
  for dm in dm_bins:
    for njet in ['0','1','2','0_crosstrg','1_crosstrg','2_crosstrg']:
      func_qcd = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:%(dm)s_njets%(njet)s_pt_2_ff_qcd_fit' % vars())
      func_wjets = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:%(dm)s_njets%(njet)s_pt_2_ff_wjets_fit' % vars())
      func_qcd_str = str(func_qcd.GetExpFormula('p')).replace('x','@0').replace(',false','')
      func_wjets_str = str(func_wjets.GetExpFormula('p')).replace('x','@0').replace(',false','')


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
      hist_w_nom = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:%(dm)s_njets%(njet)s_pt_2_ff_wjets_uncert' % vars())
      hist_qcd_nom = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:%(dm)s_njets%(njet)s_pt_2_ff_qcd_uncert' % vars())
      (w_uncert1_up, w_uncert1_down, w_uncert2_up, w_uncert_2_down) = wsptools.SplitUncert(hist_w_nom)
      (qcd_uncert1_up, qcd_uncert1_down, qcd_uncert2_up, qcd_uncert_2_down) = wsptools.SplitUncert(hist_qcd_nom)

      if 'crosstrg' not in njet:
        wsptools.SafeWrapHist(w, ['pt_bounded100'], hist_qcd_nom, name='mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_nom' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded100'], qcd_uncert1_up, name='mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert1_up' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded100'], qcd_uncert2_up, name='mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert2_up' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded140'], hist_w_nom, name='mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_nom' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded140'], w_uncert1_up, name='mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert1_up' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded140'], w_uncert2_up, name='mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert2_up' % vars())
      else:
        wsptools.SafeWrapHist(w, ['pt_bounded80'], hist_qcd_nom, name='mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_nom' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded80'], qcd_uncert1_up, name='mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert1_up' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded80'], qcd_uncert2_up, name='mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert2_up' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded80'], hist_w_nom, name='mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_nom' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded80'], w_uncert1_up, name='mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert1_up' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded80'], w_uncert2_up, name='mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert2_up' % vars())

      njets_cut = cutsmap_njets[njet] % vars()
      dm_cut    = cutsmap_dm[dm] % vars()

    for njet in ['0','1','2']:
      njets_cut = cutsmap_njets[njet] % vars()
      njets_cut_cross = cutsmap_njets[njet+'_crosstrg'] % vars()

      w.factory('expr::mt_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert1_up("(%(njets_cut)s&&%(dm_cut)s)*(@4/@5) + (%(njets_cut_cross)s&&%(dm_cut)s)*(@6/@7) + (((%(njets_cut)s||%(njets_cut_cross)s)&&%(dm_cut)s)==0)",njets[0],dm[1],ipsig[0],pass_single[1],mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert1_up,mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_nom, mt_%(dm)s_njets%(njet)s_crosstrg_%(wp)s_qcd_uncert1_up,mt_%(dm)s_njets%(njet)s_crosstrg_%(wp)s_qcd_uncert_nom)' % vars())
      w.factory('expr::mt_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert2_up("(%(njets_cut)s&&%(dm_cut)s)*(@4/@5) + (%(njets_cut_cross)s&&%(dm_cut)s)*(@6/@7) + (((%(njets_cut)s||%(njets_cut_cross)s)&&%(dm_cut)s)==0)",njets[0],dm[1],ipsig[0],pass_single[1],mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert2_up,mt_%(dm)s_njets%(njet)s_%(wp)s_qcd_uncert_nom, mt_%(dm)s_njets%(njet)s_crosstrg_%(wp)s_qcd_uncert2_up,mt_%(dm)s_njets%(njet)s_crosstrg_%(wp)s_qcd_uncert_nom)' % vars())

      w.factory('expr::mt_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert1_up("(%(njets_cut)s&&%(dm_cut)s)*(@4/@5) + (%(njets_cut_cross)s&&%(dm_cut)s)*(@6/@7) + (((%(njets_cut)s||%(njets_cut_cross)s)&&%(dm_cut)s)==0)",njets[0],dm[1],ipsig[0],pass_single[1],mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert1_up,mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_nom, mt_%(dm)s_njets%(njet)s_crosstrg_%(wp)s_wjets_uncert1_up,mt_%(dm)s_njets%(njet)s_crosstrg_%(wp)s_wjets_uncert_nom)' % vars())
      w.factory('expr::mt_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert2_up("(%(njets_cut)s&&%(dm_cut)s)*(@4/@5) + (%(njets_cut_cross)s&&%(dm_cut)s)*(@6/@7) + (((%(njets_cut)s||%(njets_cut_cross)s)&&%(dm_cut)s)==0)",njets[0],dm[1],ipsig[0],pass_single[1],mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert2_up,mt_%(dm)s_njets%(njet)s_%(wp)s_wjets_uncert_nom, mt_%(dm)s_njets%(njet)s_crosstrg_%(wp)s_wjets_uncert2_up,mt_%(dm)s_njets%(njet)s_crosstrg_%(wp)s_wjets_uncert_nom)' % vars())

      w.factory('expr::mt_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert1_down("2. - @0", mt_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert1_up)' % vars())
      w.factory('expr::mt_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert2_down("2. - @0", mt_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert2_up)' % vars())
      w.factory('expr::mt_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert1_down("2. - @0", mt_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert1_up)' % vars())
      w.factory('expr::mt_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert2_down("2. - @0", mt_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert2_up)' % vars())

    # get ttbar FFs inclusive in njets and also get W data and MC SFs to be used to define uncertainties

    func_ttbar = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:%(dm)s_inclusive_pt_2_ff_ttbar_mc_fit' % vars())
    func_wjets = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:%(dm)s_inclusive_pt_2_ff_wjets_fit' % vars())
    func_wjets_mc = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:%(dm)s_inclusive_pt_2_ff_wjets_mc_fit' % vars())
    func_ttbar_str = str(func_ttbar.GetExpFormula('p')).replace('x','@0').replace(',false','')
    func_wjets_str = str(func_wjets.GetExpFormula('p')).replace('x','@0').replace(',false','')
    func_wjets_mc_str = str(func_wjets_mc.GetExpFormula('p')).replace('x','@0').replace(',false','')

    w.factory('expr::pt_bounded140("max(min(139.9,@0),20)",pt[0])' % vars())

    w.factory('expr::mt_%(dm)s_inclusive_%(wp)s_ttbar_fit("max(%(func_ttbar_str)s,0.)",pt_bounded100)' % vars())
    w.factory('expr::mt_%(dm)s_inclusive_%(wp)s_wjets_fit("max(%(func_wjets_str)s,0.)",pt_bounded140)' % vars())
    w.factory('expr::mt_%(dm)s_inclusive_%(wp)s_wjets_mc_fit("max(%(func_wjets_mc_str)s,0.)",pt_bounded140)' % vars())

    # get stat uncertainties on ttbar
    hist_nom = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:%(dm)s_inclusive_pt_2_ff_ttbar_mc_uncert' % vars())

    (uncert1_up, uncert1_down, uncert2_up, uncert_2_down) = wsptools.SplitUncert(hist_nom)
    wsptools.SafeWrapHist(w, ['pt_bounded140'], hist_nom, name='mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert_nom' % vars())
    wsptools.SafeWrapHist(w, ['pt_bounded140'], uncert1_up, name='mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert1_hist_up' % vars())
    wsptools.SafeWrapHist(w, ['pt_bounded140'], uncert2_up, name='mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert2_hist_up' % vars())

    dm_cut    = (cutsmap_dm[dm] % vars()).replace('@1','@0').replace('@2','@1')

    w.factory('expr::mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert1_up("(%(dm_cut)s)*(@2/@3) + ((%(dm_cut)s)==0)",dm[1],ipsig[0],mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert1_hist_up,mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert_nom)' % vars())
    w.factory('expr::mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert2_up("(%(dm_cut)s)*(@2/@3) + ((%(dm_cut)s)==0)",dm[1],ipsig[0],mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert2_hist_up,mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert_nom)' % vars())

    w.factory('expr::mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert1_down("2. - @0", mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert1_up)' % vars())
    w.factory('expr::mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert2_down("2. - @0", mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert2_up)' % vars())

  # combine uncertainties into functions

  ff_eqn_tot = '(@0>=0.5)*((@2==0)*((@1==0)*(@3)+(@1==1)*(@4)+(@1==10)*(@5)+(@1==11)*(@6)) + (@2==1)*((@1==0)*(@7)+(@1==1)*(@8)+(@1==10)*(@9)+(@1==11)*(@10)) + (@2>1)*((@1==0)*(@11)+(@1==1)*(@12)+(@1==10)*(@13)+(@1==11)*(@14)))' % vars()

  ff_eqn_tot_cross = '(@0<0.5)*((@2==0)*((@1==0)*(@15)+(@1==1)*(@16)+(@1==10)*(@17)+(@1==11)*(@18)) + (@2==1)*((@1==0)*(@19)+(@1==1)*(@20)+(@1==10)*(@21)+(@1==11)*(@22)) + (@2>1)*((@1==0)*(@23)+(@1==1)*(@24)+(@1==10)*(@25)+(@1==11)*(@26)))' % vars()
  
  w.factory('expr::ff_mt_%(wp)s_dmbins_qcd_raw("%(ff_eqn_tot)s + %(ff_eqn_tot_cross)s", pass_single[1], dm[1], njets[0], mt_dm0_njets0_%(wp)s_qcd_fit, mt_dm1_njets0_%(wp)s_qcd_fit, mt_dm10_njets0_%(wp)s_qcd_fit, mt_dm11_njets0_%(wp)s_qcd_fit, mt_dm0_njets1_%(wp)s_qcd_fit, mt_dm1_njets1_%(wp)s_qcd_fit, mt_dm10_njets1_%(wp)s_qcd_fit, mt_dm11_njets1_%(wp)s_qcd_fit, mt_dm0_njets2_%(wp)s_qcd_fit, mt_dm1_njets2_%(wp)s_qcd_fit, mt_dm10_njets2_%(wp)s_qcd_fit, mt_dm11_njets2_%(wp)s_qcd_fit, mt_dm0_njets0_crosstrg_%(wp)s_qcd_fit, mt_dm1_njets0_crosstrg_%(wp)s_qcd_fit, mt_dm10_njets0_crosstrg_%(wp)s_qcd_fit, mt_dm11_njets0_crosstrg_%(wp)s_qcd_fit, mt_dm0_njets1_crosstrg_%(wp)s_qcd_fit, mt_dm1_njets1_crosstrg_%(wp)s_qcd_fit, mt_dm10_njets1_crosstrg_%(wp)s_qcd_fit, mt_dm11_njets1_crosstrg_%(wp)s_qcd_fit, mt_dm0_njets2_crosstrg_%(wp)s_qcd_fit, mt_dm1_njets2_crosstrg_%(wp)s_qcd_fit, mt_dm10_njets2_crosstrg_%(wp)s_qcd_fit, mt_dm11_njets2_crosstrg_%(wp)s_qcd_fit )' % vars())

  w.factory(('expr::ff_mt_%(wp)s_dmbins_qcd_raw("%(ff_eqn_tot)s + %(ff_eqn_tot_cross)s", pass_single[1], dm[1], njets[0], mt_dm0_njets0_%(wp)s_qcd_fit, mt_dm1_njets0_%(wp)s_qcd_fit, mt_dm10_njets0_%(wp)s_qcd_fit, mt_dm11_njets0_%(wp)s_qcd_fit, mt_dm0_njets1_%(wp)s_qcd_fit, mt_dm1_njets1_%(wp)s_qcd_fit, mt_dm10_njets1_%(wp)s_qcd_fit, mt_dm11_njets1_%(wp)s_qcd_fit, mt_dm0_njets2_%(wp)s_qcd_fit, mt_dm1_njets2_%(wp)s_qcd_fit, mt_dm10_njets2_%(wp)s_qcd_fit, mt_dm11_njets2_%(wp)s_qcd_fit, mt_dm0_njets0_crosstrg_%(wp)s_qcd_fit, mt_dm1_njets0_crosstrg_%(wp)s_qcd_fit, mt_dm10_njets0_crosstrg_%(wp)s_qcd_fit, mt_dm11_njets0_crosstrg_%(wp)s_qcd_fit, mt_dm0_njets1_crosstrg_%(wp)s_qcd_fit, mt_dm1_njets1_crosstrg_%(wp)s_qcd_fit, mt_dm10_njets1_crosstrg_%(wp)s_qcd_fit, mt_dm11_njets1_crosstrg_%(wp)s_qcd_fit, mt_dm0_njets2_crosstrg_%(wp)s_qcd_fit, mt_dm1_njets2_crosstrg_%(wp)s_qcd_fit, mt_dm10_njets2_crosstrg_%(wp)s_qcd_fit, mt_dm11_njets2_crosstrg_%(wp)s_qcd_fit )' % vars()).replace('qcd','wjets'))



  w.factory('expr::ff_mt_%(wp)s_dmbins_ttbar_raw("(@0==0)*(@1)+(@0==1)*(@2)+(@0==10)*(@3)+(@0==11)*(@4)", dm[1], mt_dm0_inclusive_%(wp)s_ttbar_fit, mt_dm1_inclusive_%(wp)s_ttbar_fit, mt_dm10_inclusive_%(wp)s_ttbar_fit, mt_dm11_inclusive_%(wp)s_ttbar_fit )' % vars())

# systematic shifts for ttbar

  
  w.factory('expr::ff_mt_%(wp)s_dmbins_ttbar_up("(@0==0)*(@1/@2)+(@0==1)*(@3/@4)+(@0==10)*(@5/@6)+(@0==11)*(@7/@8)", dm[1], mt_dm0_inclusive_%(wp)s_wjets_fit,mt_dm0_inclusive_%(wp)s_wjets_mc_fit,mt_dm1_inclusive_%(wp)s_wjets_fit,mt_dm1_inclusive_%(wp)s_wjets_mc_fit,mt_dm10_inclusive_%(wp)s_wjets_fit,mt_dm10_inclusive_%(wp)s_wjets_mc_fit,mt_dm11_inclusive_%(wp)s_wjets_fit,mt_dm11_inclusive_%(wp)s_wjets_mc_fit)' % vars())




  w.factory('expr::ff_mt_%(wp)s_dmbins_ttbar_down("1/@0",ff_mt_%(wp)s_dmbins_ttbar_up)' % vars())

  # get the ttbar met corrction
  # met correction
  w.factory('expr::met_var_w_bounded("max(min(3.5,@0),-3.5)",met_var_w[0])' % vars())
  func = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:met_closure_ttbar_mc_fit' % vars())
  corr_str = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
  w.factory('expr::mt_%(wp)s_ttbar_met_corr("%(corr_str)s",met_var_w_bounded)' % vars())

  # apply corrections to ttbar FFs
  for dmtype in ['dm']:
    dmname = dmtype.replace('dm','dmbins')
    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_ttbar("@0*@1*@2", ff_mt_%(wp)s_%(dmname)s_ttbar_raw, mt_%(wp)s_ttbar_met_corr, ff_mt_%(wp)s_%(dmname)s_ttbar_up)' % vars())

  # get W+jets corrections
  for dmtype in ['dm']:

    dmname = dmtype.replace('dm','dmbins')

    # met correction
    w.factory('expr::met_var_w_bounded("max(min(3.5,@0),-3.5)",met_var_w[0])' % vars())
    for njet in ['0','1','2']:
      func = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:%(dmtype)s_met_closure_njet%(njet)s_wjets_fit' % vars())
      corr_str = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
      w.factory('expr::mt_%(dmname)s_%(wp)s_wjets_met_corr_njet%(njet)s("%(corr_str)s",met_var_w_bounded)' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_wjets_met_corr("(@0==0)*(@1) + (@0==1)*(@2) + (@0>1)*(@3)",njets[0],mt_%(dmname)s_%(wp)s_wjets_met_corr_njet0,mt_%(dmname)s_%(wp)s_wjets_met_corr_njet1,mt_%(dmname)s_%(wp)s_wjets_met_corr_njet2)' % vars())

    # pt correction
    w.factory('expr::m_pt_bounded90("min(@0,89.9)",m_pt[0])' % vars())
    for njet in ['0','1','2']:
      func = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:%(dmtype)s_pt_1_closure_njet%(njet)s_wjets_fit' % vars())
      corr_str = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
      w.factory('expr::mt_%(dmname)s_%(wp)s_wjets_m_pt_corr_njet%(njet)s("%(corr_str)s",m_pt_bounded90)' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_wjets_m_pt_corr("(@4<25)+(@4>25)*((@0==0)*(@1) + (@0==1)*(@2) + (@0>1)*(@3))",njets[0],mt_%(dmname)s_%(wp)s_wjets_m_pt_corr_njet0,mt_%(dmname)s_%(wp)s_wjets_m_pt_corr_njet1,mt_%(dmname)s_%(wp)s_wjets_m_pt_corr_njet2,m_pt[0])' % vars())

    w.factory('expr::mt_%(dmname)s_%(wp)s_wjets_met_corr_up("@0*@0",mt_%(dmname)s_%(wp)s_wjets_met_corr)' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_wjets_met_corr_down("@0/@0",mt_%(dmname)s_%(wp)s_wjets_met_corr)' % vars())

    w.factory('expr::mt_%(dmname)s_%(wp)s_wjets_m_pt_corr_up("@0*@0",mt_%(dmname)s_%(wp)s_wjets_m_pt_corr)' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_wjets_m_pt_corr_down("@0/@0",mt_%(dmname)s_%(wp)s_wjets_m_pt_corr)' % vars())
 
    # mt_1 correction (this is now as a function of the W pT and njets)
    #func = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:%(dmtype)s_mt_corr_wjets_mc_fit' % vars())
    #func_mt_corr = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')

    # for now adding this by hand until evenythign is run
    w.factory('expr::WpT_bounded("min(199.99,@0)",WpT[0])' % vars())

    for njet in ['0','1','2']:
      func = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:%(dmtype)s_WpT_corr_njet%(njet)s_wjets_mc_fit' % vars())
      corr_str = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
      w.factory('expr::mt_%(dmname)s_%(wp)s_wjets_mt_corr_njet%(njet)s("%(corr_str)s",WpT_bounded)' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_wjets_mt_corr("(@4>=70)+(@4<70)*((@0==0)*(@1) + (@0==1)*(@2) + (@0>1)*(@3))",njets[0],mt_%(dmname)s_%(wp)s_wjets_mt_corr_njet0,mt_%(dmname)s_%(wp)s_wjets_mt_corr_njet1,mt_%(dmname)s_%(wp)s_wjets_mt_corr_njet2,mt[0])' % vars())


    #w.factory('expr::mt_%(dmname)s_%(wp)s_wjets_mt_corr("(@0<70)*((@1==0)*(1.12832-0.00262922*@2) + (@1==1)*(1.0664-0.00140539*@2) + (@1>1)*(1.06237-0.000952116*@2)) + (@0>70)", mt[0], njets[0], WpT_bounded )' % vars())

    #w.factory('expr::mt_bounded140("min(139.9,@0)",mt)' % vars())
    #w.factory('expr::mt_%(dmname)s_%(wp)s_wjets_mt_corr("%(func_mt_corr)s",mt_bounded140)' % vars())

    # m_vis correction
    #func = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:%(dmtype)s_mvis_corr_wjets_mc_fit' % vars())
    #func_mvis_corr = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')

    w.factory('expr::mvis_bounded100("min(99.9,@0)",mvis[50])' % vars())
    #w.factory('expr::mt_%(dmname)s_%(wp)s_wjets_mvis_corr("%(func_mvis_corr)s",mvis_bounded100)' % vars())
    # since we dont apply this correction anymore use a dummy function that is always = 1
    w.factory('expr::mt_%(dmname)s_%(wp)s_wjets_mvis_corr("1",mvis_bounded100)' % vars())

    # apply corrections to raw W+jets FFs
    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets("@0*@1*@2*@3*((@5<50)*@4 +(@5>=50))", ff_mt_%(wp)s_%(dmname)s_wjets_raw , mt_%(dmname)s_%(wp)s_wjets_met_corr, mt_%(dmname)s_%(wp)s_wjets_m_pt_corr, mt_%(dmname)s_%(wp)s_wjets_mt_corr, mt_%(dmname)s_%(wp)s_wjets_mvis_corr, mt)' % vars())

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets_corr_stat_met_up("@0*@1*@2*@3*((@5<50)*@4 +(@5>=50))", ff_mt_%(wp)s_%(dmname)s_wjets_raw , mt_%(dmname)s_%(wp)s_wjets_met_corr_up, mt_%(dmname)s_%(wp)s_wjets_m_pt_corr, mt_%(dmname)s_%(wp)s_wjets_mt_corr, mt_%(dmname)s_%(wp)s_wjets_mvis_corr, mt)' % vars())
    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets_corr_stat_met_down("@0*@1*@2*@3*((@5<50)*@4 +(@5>=50))", ff_mt_%(wp)s_%(dmname)s_wjets_raw , mt_%(dmname)s_%(wp)s_wjets_met_corr_down, mt_%(dmname)s_%(wp)s_wjets_m_pt_corr, mt_%(dmname)s_%(wp)s_wjets_mt_corr, mt_%(dmname)s_%(wp)s_wjets_mvis_corr, mt)' % vars())

    # apply systematic shifts

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets_corr_stat_m_pt_up("@0*@1*@2*@3*((@5<50)*@4 +(@5>=50))", ff_mt_%(wp)s_%(dmname)s_wjets_raw , mt_%(dmname)s_%(wp)s_wjets_met_corr, mt_%(dmname)s_%(wp)s_wjets_m_pt_corr_up, mt_%(dmname)s_%(wp)s_wjets_mt_corr, mt_%(dmname)s_%(wp)s_wjets_mvis_corr, mt)' % vars())
    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets_corr_stat_m_pt_down("@0*@1*@2*@3*((@5<50)*@4 +(@5>=50))", ff_mt_%(wp)s_%(dmname)s_wjets_raw , mt_%(dmname)s_%(wp)s_wjets_met_corr, mt_%(dmname)s_%(wp)s_wjets_m_pt_corr_down, mt_%(dmname)s_%(wp)s_wjets_mt_corr, mt_%(dmname)s_%(wp)s_wjets_mvis_corr, mt)' % vars())

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets_corr_mt_up("@0*@1*@2*@3*@3*((@5<50)*@4 +(@5>=50))", ff_mt_%(wp)s_%(dmname)s_wjets_raw , mt_%(dmname)s_%(wp)s_wjets_met_corr, mt_%(dmname)s_%(wp)s_wjets_m_pt_corr, mt_%(dmname)s_%(wp)s_wjets_mt_corr, mt_%(dmname)s_%(wp)s_wjets_mvis_corr, mt)' % vars())

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets_corr_mt_down("@0*@1*@2*((@5<50)*@4 +(@5>=50))", ff_mt_%(wp)s_%(dmname)s_wjets_raw , mt_%(dmname)s_%(wp)s_wjets_met_corr, mt_%(dmname)s_%(wp)s_wjets_m_pt_corr, mt_%(dmname)s_%(wp)s_wjets_mt_corr, mt_%(dmname)s_%(wp)s_wjets_mvis_corr, mt)' % vars())

  # get QCD corrections
  for dmtype in ['dm']:

    dmname = dmtype.replace('dm','dmbins')

    w.factory('expr::met_var_qcd_bounded("max(min(1.5,@0),-2.5)",met_var_qcd[0])' % vars())
    for njet in ['0','1']:
      func = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:%(dmtype)s_met_closure_njet%(njet)s_qcd_fit' % vars())
      corr_str = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
      w.factory('expr::mt_%(dmname)s_%(wp)s_qcd_met_corr_njet%(njet)s("%(corr_str)s",met_var_qcd_bounded)' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_qcd_met_corr("(@0==0)*(@1) + (@0>0)*(@2)",njets[0],mt_%(dmname)s_%(wp)s_qcd_met_corr_njet0,mt_%(dmname)s_%(wp)s_qcd_met_corr_njet1)' % vars())

    # pt correction
    w.factory('expr::m_pt_bounded50("min(@0,49.9)",m_pt[0])' % vars())
    for njet in ['0','1']:
      func = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:%(dmtype)s_pt_1_closure_njet%(njet)s_qcd_fit' % vars())
      corr_str = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
      w.factory('expr::mt_%(dmname)s_%(wp)s_qcd_m_pt_corr_njet%(njet)s("%(corr_str)s",m_pt_bounded50)' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_qcd_m_pt_corr("(@3<25)+(@3>25)*((@0==0)*(@1) + (@0>0)*(@2))",njets[0],mt_%(dmname)s_%(wp)s_qcd_m_pt_corr_njet0,mt_%(dmname)s_%(wp)s_qcd_m_pt_corr_njet1,m_pt[0])' % vars())

    w.factory('expr::mt_%(dmname)s_%(wp)s_qcd_met_corr_up("@0*@0",mt_%(dmname)s_%(wp)s_qcd_met_corr)' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_qcd_met_corr_down("@0/@0",mt_%(dmname)s_%(wp)s_qcd_met_corr)' % vars())

    w.factory('expr::mt_%(dmname)s_%(wp)s_qcd_m_pt_corr_up("@0*@0",mt_%(dmname)s_%(wp)s_qcd_m_pt_corr)' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_qcd_m_pt_corr_down("@0/@0",mt_%(dmname)s_%(wp)s_qcd_m_pt_corr)' % vars())

    # aiso->iso correction
    func = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:%(dmtype)s_iso_closure_qcd_fit' % vars())
    func_iso_corr = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
    w.factory('expr::iso_bounded0p5("min(0.499,@0)",m_iso[0])' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_qcd_iso_corr("%(func_iso_corr)s",iso_bounded0p5)' % vars())

    w.factory('expr::m_pt_bounded80("min(49.99,@0)",m_pt)' % vars())
    # OS/SS correction
    for njet in ['0','1']:
      func = GetFromTFile(loc+'fakefactor_fits_mt_%(wp)s_2018.root:%(dmtype)s_osss_closure_njet%(njet)s_qcd_fit' % vars())
      func_osss_corr = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
      w.factory('expr::mt_%(dmname)s_%(wp)s_qcd_osss_corr_njet%(njet)s("%(func_osss_corr)s",m_pt_bounded50)' % vars())
    w.factory('expr::mt_%(dmname)s_%(wp)s_qcd_osss_corr("(@0==0)*(@1)+(@0>0)*(@2)",njets[0],mt_%(dmname)s_%(wp)s_qcd_osss_corr_njet0,mt_%(dmname)s_%(wp)s_qcd_osss_corr_njet1)' % vars())

    # apply corrections to raw QCD FFs
    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd("(@5!=0)*@0*@1*@2*@3*@4 + (@5==0)*@0*@1*@2*@3", ff_mt_%(wp)s_%(dmname)s_qcd_raw , mt_%(dmname)s_%(wp)s_qcd_met_corr, mt_%(dmname)s_%(wp)s_qcd_m_pt_corr, mt_%(dmname)s_%(wp)s_qcd_iso_corr, mt_%(dmname)s_%(wp)s_qcd_osss_corr, os[1])' % vars())

    # apply systematic shifts

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd_corr_stat_m_pt_up("(@5!=0)*@0*@1*@2*@3*@4 + (@5==0)*@0*@1*@2*@3", ff_mt_%(wp)s_%(dmname)s_qcd_raw , mt_%(dmname)s_%(wp)s_qcd_met_corr, mt_%(dmname)s_%(wp)s_qcd_m_pt_corr_up, mt_%(dmname)s_%(wp)s_qcd_iso_corr, mt_%(dmname)s_%(wp)s_qcd_osss_corr, os[1])' % vars())
    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd_corr_stat_m_pt_down("(@5!=0)*@0*@1*@2*@3*@4 + (@5==0)*@0*@1*@2*@3", ff_mt_%(wp)s_%(dmname)s_qcd_raw , mt_%(dmname)s_%(wp)s_qcd_met_corr, mt_%(dmname)s_%(wp)s_qcd_m_pt_corr_down, mt_%(dmname)s_%(wp)s_qcd_iso_corr, mt_%(dmname)s_%(wp)s_qcd_osss_corr, os[1])' % vars())

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd_corr_stat_met_up("(@5!=0)*@0*@1*@2*@3*@4 + (@5==0)*@0*@1*@2*@3", ff_mt_%(wp)s_%(dmname)s_qcd_raw , mt_%(dmname)s_%(wp)s_qcd_met_corr_up, mt_%(dmname)s_%(wp)s_qcd_m_pt_corr, mt_%(dmname)s_%(wp)s_qcd_iso_corr, mt_%(dmname)s_%(wp)s_qcd_osss_corr, os[1])' % vars())
    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd_corr_stat_met_down("(@5!=0)*@0*@1*@2*@3*@4 + (@5==0)*@0*@1*@2*@3", ff_mt_%(wp)s_%(dmname)s_qcd_raw , mt_%(dmname)s_%(wp)s_qcd_met_corr_down, mt_%(dmname)s_%(wp)s_qcd_m_pt_corr, mt_%(dmname)s_%(wp)s_qcd_iso_corr, mt_%(dmname)s_%(wp)s_qcd_osss_corr, os[1])' % vars())

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd_corr_syst_up("(@5!=0)*@0*@1*@2*@3*@3*@4*@4 + (@5==0)*@0*@1*@2*@3*@3", ff_mt_%(wp)s_%(dmname)s_qcd_raw , mt_%(dmname)s_%(wp)s_qcd_met_corr, mt_%(dmname)s_%(wp)s_qcd_m_pt_corr, mt_%(dmname)s_%(wp)s_qcd_iso_corr, mt_%(dmname)s_%(wp)s_qcd_osss_corr, os[1])' % vars())

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd_corr_syst_down("(@5!=0)*@0*@1*@2 + (@5==0)*@0*@1*@2", ff_mt_%(wp)s_%(dmname)s_qcd_raw , mt_%(dmname)s_%(wp)s_qcd_met_corr, mt_%(dmname)s_%(wp)s_qcd_m_pt_corr, mt_%(dmname)s_%(wp)s_qcd_iso_corr, mt_%(dmname)s_%(wp)s_qcd_osss_corr, os[1])' % vars())

# scale FFs by their fractions

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar)' % vars()) # the nominal ttbar is now shifted up by the uncertainty

# systematic uncertainties due to corrections

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets_met_up("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets_corr_stat_met_up, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar)' % vars())
    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets_met_down("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets_corr_stat_met_down, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar)' % vars())

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets_l_pt_up("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets_corr_stat_m_pt_up, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar)' % vars())
    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets_l_pt_down("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets_corr_stat_m_pt_down, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar)' % vars())

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets_syst_up("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets_corr_mt_up, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar)' % vars())
    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets_syst_down("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets_corr_mt_down, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar)' % vars())

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd_met_up("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd_corr_stat_met_up, ff_mt_%(wp)s_%(dmname)s_ttbar)' % vars())
    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd_met_down("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd_corr_stat_met_down, ff_mt_%(wp)s_%(dmname)s_ttbar)' % vars())

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd_l_pt_up("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd_corr_stat_m_pt_up, ff_mt_%(wp)s_%(dmname)s_ttbar)' % vars())
    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd_l_pt_down("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd_corr_stat_m_pt_down, ff_mt_%(wp)s_%(dmname)s_ttbar)' % vars())

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd_syst_up("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd_corr_syst_up, ff_mt_%(wp)s_%(dmname)s_ttbar)' % vars())
    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd_syst_down("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd_corr_syst_down, ff_mt_%(wp)s_%(dmname)s_ttbar)' % vars())

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_ttbar_syst_up("@0*@3 + @1*@4 + @2*@5*@6", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar,ff_mt_%(wp)s_%(dmname)s_ttbar_up)' % vars()) # the up shift now has the correction applied twice

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_ttbar_syst_down("@0*@3 + @1*@4 + @2*@5*@6", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar, ff_mt_%(wp)s_%(dmname)s_ttbar_down)' % vars()) # the down shift now has no corretion applied

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_ttbar_met_up("@0*@3 + @1*@4 + @2*@5*@6", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar,mt_%(wp)s_ttbar_met_corr)' % vars())

    w.factory('expr::ff_mt_%(wp)s_%(dmname)s_ttbar_met_down("@0*@3 + @1*@4 + @2*@5/@6", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar,mt_%(wp)s_ttbar_met_corr)' % vars())

# produce statistical uncertainties
# merging cross trigger uncertainties to reduce number of systematics

    for dm in dm_bins:
      for njet in ['0','1','2']:

        w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd_stat_unc1_njet%(njet)s_%(dm)s_up("@0*@3 + @1*@4*@6 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar, mt_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert1_up)' % vars())
        w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd_stat_unc1_njet%(njet)s_%(dm)s_down("@0*@3 + @1*@4*@6 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar, mt_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert1_down)' % vars())

        w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd_stat_unc2_njet%(njet)s_%(dm)s_up("@0*@3 + @1*@4*@6 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar, mt_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert2_up)' % vars())
        w.factory('expr::ff_mt_%(wp)s_%(dmname)s_qcd_stat_unc2_njet%(njet)s_%(dm)s_down("@0*@3 + @1*@4*@6 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar, mt_%(dm)s_njets%(njet)s_comb_%(wp)s_qcd_uncert2_down)' % vars())

        w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets_stat_unc1_njet%(njet)s_%(dm)s_up("@0*@3*@6 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar, mt_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert1_up)' % vars())
        w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets_stat_unc1_njet%(njet)s_%(dm)s_down("@0*@3*@6 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar, mt_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert1_down)' % vars())

        w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets_stat_unc2_njet%(njet)s_%(dm)s_up("@0*@3*@6 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar, mt_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert2_up)' % vars())
        w.factory('expr::ff_mt_%(wp)s_%(dmname)s_wjets_stat_unc2_njet%(njet)s_%(dm)s_down("@0*@3*@6 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar, mt_%(dm)s_njets%(njet)s_comb_%(wp)s_wjets_uncert2_down)' % vars())

      w.factory('expr::ff_mt_%(wp)s_%(dmname)s_ttbar_stat_unc1_%(dm)s_up("@0*@3 + @1*@4 + @2*@5*@6", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar, mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert1_up)' % vars())
      w.factory('expr::ff_mt_%(wp)s_%(dmname)s_ttbar_stat_unc1_%(dm)s_down("@0*@3 + @1*@4 + @2*@5*@6", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar, mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert1_down)' % vars())

      w.factory('expr::ff_mt_%(wp)s_%(dmname)s_ttbar_stat_unc2_%(dm)s_up("@0*@3 + @1*@4 + @2*@5*@6", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar, mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert2_up)' % vars())
      w.factory('expr::ff_mt_%(wp)s_%(dmname)s_ttbar_stat_unc2_%(dm)s_down("@0*@3 + @1*@4 + @2*@5*@6", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_%(dmname)s_wjets, ff_mt_%(wp)s_%(dmname)s_qcd, ff_mt_%(wp)s_%(dmname)s_ttbar, mt_%(dm)s_inclusive_%(wp)s_ttbar_uncert2_down)' % vars())

w.Print()
w.writeToFile('fakefactors_ws_mt_hps_lite_2018.root')
w.Delete()


################################################
### tt channel ####
################################################

w = ROOT.RooWorkspace('w')

ip_sig_cut = '1.5'
wps = ['medium']

for wp in wps:

  # get fractions

  loc = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/hpsdm_ff_deeptauV2p1_2018_tt/' % vars()
 
  histsToWrap = [(loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets0_os_qcd' % vars(), 'tt_%(wp)s_fracs_njets0_os_qcd' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets1_os_qcd' % vars(), 'tt_%(wp)s_fracs_njets1_os_qcd' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets2_os_qcd' % vars(), 'tt_%(wp)s_fracs_njets2_os_qcd' % vars()), 
                 (loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets0_ss_qcd' % vars(), 'tt_%(wp)s_fracs_njets0_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets1_ss_qcd' % vars(), 'tt_%(wp)s_fracs_njets1_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets2_ss_qcd' % vars(), 'tt_%(wp)s_fracs_njets2_ss_qcd' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets0_os_wjets' % vars(), 'tt_%(wp)s_fracs_njets0_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets1_os_wjets' % vars(), 'tt_%(wp)s_fracs_njets1_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets2_os_wjets' % vars(), 'tt_%(wp)s_fracs_njets2_os_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets0_ss_wjets' % vars(), 'tt_%(wp)s_fracs_njets0_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets1_ss_wjets' % vars(), 'tt_%(wp)s_fracs_njets1_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets2_ss_wjets' % vars(), 'tt_%(wp)s_fracs_njets2_ss_wjets' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets0_os_ttbar' % vars(), 'tt_%(wp)s_fracs_njets0_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets1_os_ttbar' % vars(), 'tt_%(wp)s_fracs_njets1_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets2_os_ttbar' % vars(), 'tt_%(wp)s_fracs_njets2_os_ttbar' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets0_ss_ttbar' % vars(), 'tt_%(wp)s_fracs_njets0_ss_ttbar' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets1_ss_ttbar' % vars(), 'tt_%(wp)s_fracs_njets1_ss_ttbar' % vars()),
                 (loc + 'fakefactor_fits_tt_%(wp)s_2018.root:tt_fracs_njets2_ss_ttbar' % vars(), 'tt_%(wp)s_fracs_njets2_ss_ttbar' % vars())
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
  }

  # get all fitted functions for raw fake factors and statistical uncertainties from fit uncertainty bands
  for njet in [0,1,2]:
    for dm in dm_bins:
      func = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_2018.root:%(dm)s_njets%(njet)i_pt_1_ff_qcd_fit' % vars())
      func_str=str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')

      w.factory('expr::pt_bounded("max(min(169.9,@0),40)",pt[0])' % vars())
      w.factory('expr::tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_fit("max(%(func_str)s,0.)",pt_bounded)' % vars())
 
      # get stat uncertainties
      hist_nom = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_2018.root:%(dm)s_njets%(njet)i_pt_1_ff_qcd_uncert' % vars())
      (uncert1_up, uncert1_down, uncert2_up, uncert_2_down) = wsptools.SplitUncert(hist_nom)
      wsptools.SafeWrapHist(w, ['pt_bounded'], hist_nom, name='tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert_nom' % vars())
      wsptools.SafeWrapHist(w, ['pt_bounded'], uncert1_up, name='tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert1_hist_up' % vars())
      wsptools.SafeWrapHist(w, ['pt_bounded'], uncert2_up, name='tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert2_hist_up' % vars())
 
      njets_cut = cutsmap_njets[njet] % vars() 
      dm_cut    = cutsmap_dm[dm] % vars()

      w.factory('expr::tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert1_up("(%(njets_cut)s&&%(dm_cut)s)*(@3/@4) + ((%(njets_cut)s&&%(dm_cut)s)==0)",njets[0],dm[1],ipsig[0],tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert1_hist_up,tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert_nom)' % vars())
      w.factory('expr::tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert2_up("(%(njets_cut)s&&%(dm_cut)s)*(@3/@4) + ((%(njets_cut)s&&%(dm_cut)s)==0)",njets[0],dm[1],ipsig[0],tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert2_hist_up,tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert_nom)' % vars())

      w.factory('expr::tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert1_down("2. - @0", tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert1_up)' % vars())
      w.factory('expr::tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert2_down("2. - @0", tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert2_up)' % vars())

  # make njets / dm binned functions

  w.factory('expr::ff_tt_%(wp)s_dmbins_qcd("(@0==0)*((@1==0)*@2+(@1==1)*@3+(@1==10)*@4 + (@1==11)*@5) + (@0==1)*((@1==0)*@6+(@1==1)*@7+(@1==10)*@8 + (@1==11)*@9) + (@0>1)*((@1==0)*@10+(@1==1)*@11+(@1==10)*@12 + (@1==11)*@13)", njets[0], dm[1], tt_dm0_njets0_%(wp)s_qcd_fit, tt_dm1_njets0_%(wp)s_qcd_fit, tt_dm10_njets0_%(wp)s_qcd_fit, tt_dm11_njets0_%(wp)s_qcd_fit, tt_dm0_njets1_%(wp)s_qcd_fit, tt_dm1_njets1_%(wp)s_qcd_fit, tt_dm10_njets1_%(wp)s_qcd_fit, tt_dm11_njets1_%(wp)s_qcd_fit, tt_dm0_njets2_%(wp)s_qcd_fit, tt_dm1_njets2_%(wp)s_qcd_fit, tt_dm10_njets2_%(wp)s_qcd_fit, tt_dm11_njets2_%(wp)s_qcd_fit)' % vars())


  for dmtype in ['dmbins']:

# start of new

    # get SS met closure correction
    w.factory('expr::met_var_qcd_bounded("max(min(1.5,@0),-1.5)",met_var_qcd[0])' % vars())
    for njet in ['0','1','2']:
      func = GetFromTFile((loc+'/fakefactor_fits_tt_%(wp)s_2018.root:%(dmtype)s_met_ss_closure_njet%(njet)s_qcd_fit' % vars()).replace('bins',''))
      func_str_pol3 = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
      w.factory('expr::tt_%(dmtype)s_%(wp)s_qcd_ss_corr_njet%(njet)s("(%(func_str_pol3)s)",met_var_qcd_bounded)' % vars())
    w.factory('expr::tt_%(dmtype)s_%(wp)s_qcd_ss_corr("(@0==0)*(@1) + (@0==1)*(@2) + (@0>1)*(@3)",njets[0],tt_%(dmtype)s_%(wp)s_qcd_ss_corr_njet0,tt_%(dmtype)s_%(wp)s_qcd_ss_corr_njet1,tt_%(dmtype)s_%(wp)s_qcd_ss_corr_njet2)' % vars())

    # get OS/SS corrections and uncertainties
    w.factory('expr::pt_2_bounded("max(min(139.9,@0),40.)",pt_2[0])' % vars())
    for njet in ['0','1']:
      func = GetFromTFile((loc+'fakefactor_fits_tt_%(wp)s_2018.root:%(dmtype)s_pt_2_os_closure_njet%(njet)s_qcd_fit' % vars()).replace('bins',''))
      func_str_pol3 = str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
      w.factory('expr::tt_%(dmtype)s_%(wp)s_qcd_corr_njet%(njet)s("%(func_str_pol3)s",pt_2_bounded)' % vars())


    w.factory('expr::tt_%(dmtype)s_%(wp)s_qcd_corr("(@0==0)*(1.24845-0.0578994*@1) + (@0>0)*(1.15364-0.0317013*@1)",njets[0], dR[2])' % vars())

    # apply QCD corrections - OS events have both met and pt_2 corrections. SS events have only the MET correction
    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s("(@0!=0)*@1*@2*@3 + (@0==0)*@1*@3", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(dmtype)s_%(wp)s_qcd_ss_corr)' % vars())

    # QCD systematics uncertainties: from applying corrections twice vs not applying them

    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_met_up("(@0!=0)*@1*@2*@3*@3 + (@0==0)*@1*@3*@3", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(dmtype)s_%(wp)s_qcd_ss_corr)' % vars())
    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_met_down("(@0!=0)*@1*@2 + (@0==0)*@1", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(dmtype)s_%(wp)s_qcd_ss_corr)' % vars())

# QCD systematics uncertainty

    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_syst_up("(@0!=0)*@1*@2*@2*@3 + (@0==0)*@1*@3", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(dmtype)s_%(wp)s_qcd_ss_corr)' % vars())
    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_syst_down("(@0!=0)*@1*@3 + (@0==0)*@1*@3", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(dmtype)s_%(wp)s_qcd_ss_corr)' % vars())

# end of new

    for x in ['qcd','wjets_mc','ttbar_mc']:
      func = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_2018.root:inclusive_inclusive_pt_1_ff_%(x)s_fit' % vars())
      func_str = func.GetTitle()
      params = func.GetParameters()
      for i in range(0,func.GetNpar()): func_str = func_str.replace('[%(i)i]' % vars(),'%f' % params[i])
      func_str = re.sub('x', '@0', func_str)
      w.factory('expr::pt_bounded("max(min(199.9,@0),40)",pt[0])' % vars())
      w.factory('expr::tt_%(dmtype)s_inclusive_%(wp)s_%(x)s_fit("%(func_str)s",pt_bounded)' % vars())

    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_wjets_syst_up("@0*(1.-@1 + @1*@2/@3)", ff_tt_%(wp)s_%(dmtype)s, tt_%(wp)s_fracs_wjets, tt_%(dmtype)s_inclusive_%(wp)s_wjets_mc_fit, tt_%(dmtype)s_inclusive_%(wp)s_qcd_fit)' % vars())
    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_wjets_syst_down("@0*(1.-@1 + @1*@3/@2)", ff_tt_%(wp)s_%(dmtype)s, tt_%(wp)s_fracs_wjets, tt_%(dmtype)s_inclusive_%(wp)s_wjets_mc_fit, tt_%(dmtype)s_inclusive_%(wp)s_qcd_fit)' % vars())

    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_ttbar_syst_up("@0*(1.-@1 + @1*@2/@3)", ff_tt_%(wp)s_%(dmtype)s, tt_%(wp)s_fracs_ttbar, tt_%(dmtype)s_inclusive_%(wp)s_ttbar_mc_fit, tt_%(dmtype)s_inclusive_%(wp)s_qcd_fit)' % vars())
    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_ttbar_syst_down("@0*(1.-@1 + @1*@3/@2)", ff_tt_%(wp)s_%(dmtype)s, tt_%(wp)s_fracs_ttbar, tt_%(dmtype)s_inclusive_%(wp)s_ttbar_mc_fit, tt_%(dmtype)s_inclusive_%(wp)s_qcd_fit)' % vars())

    # add statistical uncertainties 1 per njets/dm bin
    for njet in [0,1,2]:
      for dm in dm_bins:
        w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_stat_unc1_njet%(njet)i_%(dm)s_up("@0*@1",tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert1_up,ff_tt_%(wp)s_%(dmtype)s)' % vars())
        w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_stat_unc1_njet%(njet)i_%(dm)s_down("@0*@1",tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert1_down,ff_tt_%(wp)s_%(dmtype)s)' % vars())
 
        w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_stat_unc2_njet%(njet)i_%(dm)s_up("@0*@1",tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert2_up,ff_tt_%(wp)s_%(dmtype)s)' % vars())
        w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_stat_unc2_njet%(njet)i_%(dm)s_down("@0*@1",tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_uncert2_down,ff_tt_%(wp)s_%(dmtype)s)' % vars())

w.Print()
w.writeToFile('fakefactors_ws_tt_hps_lite_2018.root')
w.Delete() 
