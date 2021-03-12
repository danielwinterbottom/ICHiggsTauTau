#!/usr/bin/env python
import ROOT
import imp
import json
from array import array
import numpy as np
import subprocess
import re
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--year','-y', help= 'year', type=str, default='2017')
parser.add_argument('--input','-i', help= 'input', type=str, default='./')
parser.add_argument('--output','-o', help= 'output', type=str, default='./')

args = parser.parse_args()

year = args.year
input = args.input
output = args.output

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
### tt channel ####
################################################

w = ROOT.RooWorkspace('w')

wp = 'medium'

# get fractions

loc = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/%(input)s/' % vars()

histsToWrap = [(loc + 'fakefactor_fits_tt_%(wp)s_%(year)s.root:tt_fracs_n_deepbjets0_os_qcd' % vars(), 'tt_%(wp)s_fracs_nbjets0_os_qcd' % vars()),
               (loc + 'fakefactor_fits_tt_%(wp)s_%(year)s.root:tt_fracs_n_deepbjets1_os_qcd' % vars(), 'tt_%(wp)s_fracs_nbjets1_os_qcd' % vars()),
               (loc + 'fakefactor_fits_tt_%(wp)s_%(year)s.root:tt_fracs_n_deepbjets0_ss_qcd' % vars(), 'tt_%(wp)s_fracs_nbjets0_ss_qcd' % vars()),
               (loc + 'fakefactor_fits_tt_%(wp)s_%(year)s.root:tt_fracs_n_deepbjets1_ss_qcd' % vars(), 'tt_%(wp)s_fracs_nbjets1_ss_qcd' % vars()),
               (loc + 'fakefactor_fits_tt_%(wp)s_%(year)s.root:tt_fracs_n_deepbjets0_os_wjets' % vars(), 'tt_%(wp)s_fracs_nbjets0_os_wjets' % vars()),
               (loc + 'fakefactor_fits_tt_%(wp)s_%(year)s.root:tt_fracs_n_deepbjets1_os_wjets' % vars(), 'tt_%(wp)s_fracs_nbjets1_os_wjets' % vars()),
               (loc + 'fakefactor_fits_tt_%(wp)s_%(year)s.root:tt_fracs_n_deepbjets0_ss_wjets' % vars(), 'tt_%(wp)s_fracs_nbjets0_ss_wjets' % vars()),
               (loc + 'fakefactor_fits_tt_%(wp)s_%(year)s.root:tt_fracs_n_deepbjets1_ss_wjets' % vars(), 'tt_%(wp)s_fracs_nbjets1_ss_wjets' % vars()),
               (loc + 'fakefactor_fits_tt_%(wp)s_%(year)s.root:tt_fracs_n_deepbjets0_os_ttbar' % vars(), 'tt_%(wp)s_fracs_nbjets0_os_ttbar' % vars()),
               (loc + 'fakefactor_fits_tt_%(wp)s_%(year)s.root:tt_fracs_n_deepbjets1_os_ttbar' % vars(), 'tt_%(wp)s_fracs_nbjets1_os_ttbar' % vars()),
               (loc + 'fakefactor_fits_tt_%(wp)s_%(year)s.root:tt_fracs_n_deepbjets0_ss_ttbar' % vars(), 'tt_%(wp)s_fracs_nbjets0_ss_ttbar' % vars()),
               (loc + 'fakefactor_fits_tt_%(wp)s_%(year)s.root:tt_fracs_n_deepbjets1_ss_ttbar' % vars(), 'tt_%(wp)s_fracs_nbjets1_ss_ttbar' % vars()),
              ]


w.factory('expr::pt_bounded("max(min(399.9,@0),40)",pt[0])' % vars())

for task in histsToWrap:
   wsptools.SafeWrapHist(
     w, ['pt_bounded'],
     GetFromTFile(task[0]),
     name=task[1])

for i in ['qcd','wjets','ttbar']:
  w.factory('expr::tt_fracs_%(i)s("(@0!=0)*((@1==0)*(@2)+(@1>=1)*@3)+(@0==0)*((@1==0)*(@4)+(@1>=1)*@5)", os[1],nbjets[0],tt_%(wp)s_fracs_nbjets0_os_%(i)s,tt_%(wp)s_fracs_nbjets1_os_%(i)s,tt_%(wp)s_fracs_nbjets0_ss_%(i)s,tt_%(wp)s_fracs_nbjets1_ss_%(i)s)' % vars())

# pT dependent fits

jetpt_bins = [
            'jet_pt_low',
            'jet_pt_med',
            'jet_pt_high',
]


cutsmap_njets = {
                0: '@0==0',
                1: '@0>=1',
}

cutsmap_jetpt = {
            'jet_pt_low':'@1<1.25*@2',
            'jet_pt_med':'@1>=1.25*@2&&@1<1.5*@2',
            'jet_pt_high':'@1>=1.5*@2',
}

# get all fitted functions for raw fake factors and statistical uncertainties from fit uncertainty bands
for njet in [0,1]:
  for jetpt in jetpt_bins:

    name='%(jetpt)s_%(njet)ijet_pt_1_ff_qcd' % vars()

    short_name='%(jetpt)s_%(njet)ijet' % vars()

    func = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_%(year)s.root:%(name)s_fit' % vars())
    func_str=str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')

    func_alt = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_%(year)s_forcepol1.root:%(name)s_fit' % vars())
    func_str_alt=str(func_alt.GetExpFormula('p')).replace('x','@0').replace(',false','')

    hist = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_%(year)s.root:%(name)s' % vars())

    high_pt_val = hist.GetBinContent(hist.FindBin(300.))
    high_pt_uncert = hist.GetBinError(hist.FindBin(300.))
    if high_pt_val > 0.:
      high_pt_uncert /= high_pt_val
    else: high_pt_uncert=1.

    if 'high' in jetpt:
      w.factory('expr::tt_%(name)s_fit("max(%(func_str)s,0.)",pt_bounded)' % vars())
      w.factory('expr::tt_%(name)s_fit_alt_func_up("max(%(func_str_alt)s,0.)/@1",pt_bounded, tt_%(name)s_fit)' % vars())
    else:
      w.factory('expr::tt_%(name)s_fit("max((%(func_str)s)*(@0<200.) + (@0>=200.)*%(high_pt_val).5f,0.)",pt_bounded)' % vars())
      print 'expr::tt_%(name)s_fit("max((%(func_str)s)*(@0<200.) + (@0>=200.)*%(high_pt_val).5f,0.)",pt_bounded)' % vars()
      w.factory('expr::tt_%(name)s_fit_alt_func_up("max((%(func_str_alt)s)*(@0<200.) + (@0>=200.)*%(high_pt_val).5f,0.)/@1",pt_bounded, tt_%(name)s_fit)' % vars())
 

    # get stat uncertainties
    hist_nom = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_%(year)s.root:%(name)s_uncert' % vars())
    (uncert1_up, uncert1_down, uncert2_up, uncert_2_down) = wsptools.SplitUncert(hist_nom)
    wsptools.SafeWrapHist(w, ['pt_bounded'], hist_nom, name='tt_%(name)s_uncert_nom' % vars())
    wsptools.SafeWrapHist(w, ['pt_bounded'], uncert1_up, name='tt_%(name)s_uncert1_hist_up' % vars())
    wsptools.SafeWrapHist(w, ['pt_bounded'], uncert2_up, name='tt_%(name)s_uncert2_hist_up' % vars())

    njets_cut = cutsmap_njets[njet] % vars() 
    jetpt_cut    = cutsmap_jetpt[jetpt] % vars()

    if 'high' in jetpt:
      w.factory('expr::tt_%(short_name)s_uncert1_up("(%(njets_cut)s&&%(jetpt_cut)s)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s)==0)",njets[0],jetpt[40],pt_bounded,tt_%(name)s_uncert1_hist_up,tt_%(name)s_uncert_nom)' % vars())
      w.factory('expr::tt_%(short_name)s_uncert2_up("(%(njets_cut)s&&%(jetpt_cut)s)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s)==0)",njets[0],jetpt[40],pt_bounded,tt_%(name)s_uncert2_hist_up,tt_%(name)s_uncert_nom)' % vars())
      w.factory('expr::tt_%(short_name)s_uncert3_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2>200)*1.5 + ((%(njets_cut)s&&%(jetpt_cut)s&&@2>200)==0)",njets[0],jetpt[40],pt_bounded)' % vars())
    else:
      w.factory('expr::tt_%(short_name)s_uncert1_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2<200)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s&&@2<200)==0)",njets[0],jetpt[40],pt_bounded,tt_%(name)s_uncert1_hist_up,tt_%(name)s_uncert_nom)' % vars())
      w.factory('expr::tt_%(short_name)s_uncert2_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2<200)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s&&@2<200)==0)",njets[0],jetpt[40],pt_bounded,tt_%(name)s_uncert2_hist_up,tt_%(name)s_uncert_nom)' % vars())
      w.factory('expr::tt_%(short_name)s_uncert3_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2>200)*(1.+%(high_pt_uncert).5f) + ((%(njets_cut)s&&%(jetpt_cut)s&&@2>200)==0)",njets[0],jetpt[40],pt_bounded)' % vars())

    w.factory('expr::tt_%(short_name)s_uncert1_down("2.-@0", tt_%(short_name)s_uncert1_up)' % vars())
    w.factory('expr::tt_%(short_name)s_uncert2_down("2.-@0", tt_%(short_name)s_uncert2_up)' % vars())
    w.factory('expr::tt_%(short_name)s_uncert3_down("2.-@0", tt_%(short_name)s_uncert3_up)' % vars())

# make njets / jetpt binned functions

w.factory('expr::ff_tt_qcd("(@0==0)*((@1<1.25*@2)*@3 + (@1>=1.25*@2&&@1<1.5*@2)*@4 + (@1>=1.5*@2)*@5) + (@0>0)*((@1<1.25*@2)*@6 + (@1>=1.25*@2&&@1<1.5*@2)*@7 + (@1>=1.5*@2)*@8)", njets[0], jetpt[40], pt_bounded, tt_jet_pt_low_0jet_pt_1_ff_qcd_fit, tt_jet_pt_med_0jet_pt_1_ff_qcd_fit, tt_jet_pt_high_0jet_pt_1_ff_qcd_fit, tt_jet_pt_low_1jet_pt_1_ff_qcd_fit, tt_jet_pt_med_1jet_pt_1_ff_qcd_fit, tt_jet_pt_high_1jet_pt_1_ff_qcd_fit, pt_2[40], met[30])' % vars())
w.factory('expr::ff_tt_qcd_alt_func_up("(@0==0)*((@1<1.25*@2)*@3 + (@1>=1.25*@2&&@1<1.5*@2)*@4 + (@1>=1.5*@2)*@5) + (@0>0)*((@1<1.25*@2)*@6 + (@1>=1.25*@2&&@1<1.5*@2)*@7 + (@1>=1.5*@2)*@8)", njets[0], jetpt[40], pt_bounded, tt_jet_pt_low_0jet_pt_1_ff_qcd_fit_alt_func_up, tt_jet_pt_med_0jet_pt_1_ff_qcd_fit_alt_func_up, tt_jet_pt_high_0jet_pt_1_ff_qcd_fit_alt_func_up, tt_jet_pt_low_1jet_pt_1_ff_qcd_fit_alt_func_up, tt_jet_pt_med_1jet_pt_1_ff_qcd_fit_alt_func_up, tt_jet_pt_high_1jet_pt_1_ff_qcd_fit_alt_func_up)' % vars())
w.factory('expr::ff_tt_qcd_alt_func_down("2-@0",ff_tt_qcd_alt_func_up)')

# get closure corrections

# this one just for an uncertainty
func_ss_pt_2 = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_%(year)s.root:ss_pt_2_closure_qcd_fit' % vars())
func_ss_pt_2_str=str(func_ss_pt_2.GetExpFormula('p')).replace('x','min(@0,140.)').replace(',false','')
w.factory('expr::tt_qcd_ss_pt_2_uncert("max(%(func_ss_pt_2_str)s,0.)",pt_2[40])' % vars())

# we also introduce a met uncertainty due to non-closure of met in 2018. For 2017 and 2016 this is very small so we take the 2018 fits for all years
w.factory('expr::tt_qcd_ss_met_uncert("0.986185+0.000606797*min(@0,200.)",met[30])' % vars())

for nbjet in [0,1]:
  w.factory('expr::dR_bounded("min(max(0.5,@0),4.5)",dR[3])' % vars())

  func_ss = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_%(year)s.root:ss_closure_nbjet%(nbjet)i_qcd_fit' % vars())
  func_ss_str=str(func_ss.GetExpFormula('p')).replace('x','@0').replace(',false','')
  func_os = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_%(year)s.root:os_closure_nbjet%(nbjet)i_qcd_fit' % vars())
  func_os_str=str(func_os.GetExpFormula('p')).replace('x','@0').replace(',false','')
  func_os_alt = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_%(year)s.root:os_closure_nbjet%(nbjet)i_alt_qcd_fit' % vars())
  func_os_alt_str=str(func_os_alt.GetExpFormula('p')).replace('x','@0').replace(',false','')
  hist_os_flat = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_%(year)s.root:os_closure_nbjet%(nbjet)i_flat' % vars())
  val_flat = hist_os_flat.GetBinContent(1)

  w.factory('expr::tt_qcd_ss_nbjets%(nbjet)i_correction("max(%(func_ss_str)s,0.)",dR_bounded)' % vars())
  w.factory('expr::tt_qcd_os_nbjets%(nbjet)i_dR_correction("max(%(func_os_str)s,0.)",dR_bounded)' % vars())
  w.factory('expr::tt_qcd_os_nbjets%(nbjet)i_pt_correction("max(%(func_os_alt_str)s,0.)/%(val_flat).5f",pt_bounded)' % vars())

  w.factory('expr::tt_qcd_os_nbjets%(nbjet)i_correction("@0*@1",tt_qcd_os_nbjets%(nbjet)i_dR_correction, tt_qcd_os_nbjets%(nbjet)i_pt_correction)' % vars())

  # get stat uncertainties

  hist_ss_nom = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_%(year)s.root:ss_closure_nbjet%(nbjet)i_qcd_uncert' % vars())
  hist_os_nom = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_%(year)s.root:os_closure_nbjet%(nbjet)i_qcd_uncert' % vars())
  hist_os_alt_nom = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_%(year)s.root:os_closure_nbjet%(nbjet)i_alt_qcd_uncert' % vars())

  #multiply os by ss so we only have 1 uncertainty for dR correction 
  hist_os_nom.Multiply(hist_ss_nom)

  (uncert1_up, uncert1_down, uncert2_up, uncert_2_down) = wsptools.SplitUncert(hist_ss_nom)

  wsptools.SafeWrapHist(w, ['dR_bounded'], hist_ss_nom, name='tt_qcd_ss_nbjets%(nbjet)i_dR_correction_nom' % vars())
  wsptools.SafeWrapHist(w, ['dR_bounded'], uncert1_up, name='tt_qcd_ss_nbjets%(nbjet)i_dR_correction_uncert1_hist_up' % vars())
  wsptools.SafeWrapHist(w, ['dR_bounded'], uncert2_up, name='tt_qcd_ss_nbjets%(nbjet)i_dR_correction_uncert2_hist_up' % vars())

  (uncert1_up, uncert1_down, uncert2_up, uncert_2_down) = wsptools.SplitUncert(hist_os_nom)

  wsptools.SafeWrapHist(w, ['dR_bounded'], hist_os_nom, name='tt_qcd_os_nbjets%(nbjet)i_dR_correction_nom' % vars())
  wsptools.SafeWrapHist(w, ['dR_bounded'], uncert1_up, name='tt_qcd_os_nbjets%(nbjet)i_dR_correction_uncert1_hist_up' % vars())
  wsptools.SafeWrapHist(w, ['dR_bounded'], uncert2_up, name='tt_qcd_os_nbjets%(nbjet)i_dR_correction_uncert2_hist_up' % vars())

  (uncert1_up, uncert1_down, uncert2_up, uncert_2_down) = wsptools.SplitUncert(hist_os_alt_nom)

  wsptools.SafeWrapHist(w, ['pt_bounded'], hist_os_alt_nom, name='tt_qcd_os_nbjets%(nbjet)i_pt_correction_nom' % vars())
  wsptools.SafeWrapHist(w, ['pt_bounded'], uncert1_up, name='tt_qcd_os_nbjets%(nbjet)i_pt_correction_uncert1_hist_up' % vars())
  wsptools.SafeWrapHist(w, ['pt_bounded'], uncert2_up, name='tt_qcd_os_nbjets%(nbjet)i_pt_correction_uncert2_hist_up' % vars())

  w.factory('expr::tt_qcd_ss_nbjets%(nbjet)i_dR_correction_uncert1_up("@0/@1", tt_qcd_ss_nbjets%(nbjet)i_dR_correction_uncert1_hist_up,tt_qcd_ss_nbjets%(nbjet)i_dR_correction_nom)' % vars())
  w.factory('expr::tt_qcd_os_nbjets%(nbjet)i_dR_correction_uncert1_up("@0/@1", tt_qcd_os_nbjets%(nbjet)i_dR_correction_uncert1_hist_up,tt_qcd_os_nbjets%(nbjet)i_dR_correction_nom)' % vars())
  w.factory('expr::tt_qcd_os_nbjets%(nbjet)i_pt_correction_uncert1_up("@0/@1", tt_qcd_os_nbjets%(nbjet)i_pt_correction_uncert1_hist_up,tt_qcd_os_nbjets%(nbjet)i_pt_correction_nom)' % vars())

  w.factory('expr::tt_qcd_ss_nbjets%(nbjet)i_dR_correction_uncert2_up("@0/@1", tt_qcd_ss_nbjets%(nbjet)i_dR_correction_uncert2_hist_up,tt_qcd_ss_nbjets%(nbjet)i_dR_correction_nom)' % vars())
  w.factory('expr::tt_qcd_os_nbjets%(nbjet)i_dR_correction_uncert2_up("@0/@1", tt_qcd_os_nbjets%(nbjet)i_dR_correction_uncert2_hist_up,tt_qcd_os_nbjets%(nbjet)i_dR_correction_nom)' % vars())
  w.factory('expr::tt_qcd_os_nbjets%(nbjet)i_pt_correction_uncert2_up("@0/@1", tt_qcd_os_nbjets%(nbjet)i_pt_correction_uncert2_hist_up,tt_qcd_os_nbjets%(nbjet)i_pt_correction_nom)' % vars())

w.factory('expr::tt_qcd_ss_correction("(@0==0)*@1 + (@0>0)*@2",nbjets[0], tt_qcd_ss_nbjets0_correction, tt_qcd_ss_nbjets1_correction )' % vars())
w.factory('expr::tt_qcd_os_correction("(@0==0)*@1 + (@0>0)*@2",nbjets[0], tt_qcd_os_nbjets0_correction, tt_qcd_os_nbjets1_correction )' % vars())

for i in [1,2]:

  w.factory('expr::tt_qcd_correction_dR_uncert%(i)i_up("((@0==0)*@2 + (@0>0)*@3)*(@1!=0) + ((@0==0)*@4 + (@0>0)*@5)*(@1==0) ",nbjets[0], os[1], tt_qcd_os_nbjets0_dR_correction_uncert%(i)i_up, tt_qcd_os_nbjets1_dR_correction_uncert%(i)i_up, tt_qcd_ss_nbjets0_dR_correction_uncert%(i)i_up, tt_qcd_ss_nbjets1_dR_correction_uncert%(i)i_up )' % vars())
  w.factory('expr::tt_qcd_correction_pt_uncert%(i)i_up("((@0==0)*@2 + (@0>0)*@3)*(@1!=0) + 1.*(@1==0) ",nbjets[0], os[1], tt_qcd_os_nbjets0_pt_correction_uncert%(i)i_up, tt_qcd_os_nbjets1_pt_correction_uncert%(i)i_up )' % vars())

  w.factory('expr::tt_qcd_correction_dR_uncert%(i)i_down("2.-@0", tt_qcd_correction_dR_uncert%(i)i_up)' % vars())
  w.factory('expr::tt_qcd_correction_pt_uncert%(i)i_down("2.-@0", tt_qcd_correction_pt_uncert%(i)i_up)' % vars())

# get final fake factor
w.factory('expr::ff_total("@0*@1*((@2!=0)*@3 + (@2==0))", ff_tt_qcd, tt_qcd_ss_correction, os[1], tt_qcd_os_correction)' % vars())

#uncertainty due to alternative fitted functions
w.factory('expr::ff_total_syst_alt_func_up("@0*@1", ff_tt_qcd_alt_func_up, ff_total)' % vars())
w.factory('expr::ff_total_syst_alt_func_down("@0*@1", ff_tt_qcd_alt_func_down, ff_total)' % vars())

# uncertainty on pt_2
w.factory('expr::ff_total_qcd_syst_pt_2_up("@0*@1", ff_total, tt_qcd_ss_pt_2_uncert)' % vars())
w.factory('expr::ff_total_qcd_syst_pt_2_down("@0*(2-@1)", ff_total, tt_qcd_ss_pt_2_uncert)' % vars())

# uncertainty on met
w.factory('expr::ff_total_qcd_syst_met_up("@0*@1", ff_total, tt_qcd_ss_met_uncert)' % vars())
w.factory('expr::ff_total_qcd_syst_met_down("@0*(2-@1)", ff_total, tt_qcd_ss_met_uncert)' % vars())

# statistical uncertainties on measured fake factors
## add statistical uncertainties 1 per njets/jetpt bin
for njet in [0,1]:
  for jetpt in jetpt_bins:
      short_name='%(jetpt)s_%(njet)ijet' % vars()
      for i in [1,2,3]:
        w.factory('expr::ff_total_qcd_stat_njet%(njet)i_%(jetpt)s_unc%(i)i_up("@0*@1",ff_total, tt_%(short_name)s_uncert%(i)i_up)' % vars())
        w.factory('expr::ff_total_qcd_stat_njet%(njet)i_%(jetpt)s_unc%(i)i_down("@0*@1",ff_total, tt_%(short_name)s_uncert%(i)i_down)' % vars())

# systematic uncertainty from applying os/ss correction twice or not applying it 
w.factory('expr::ff_total_qcd_syst_up("@0*@1*((@2!=0)*@3*@3 + (@2==0))", ff_tt_qcd, tt_qcd_ss_correction, os[1], tt_qcd_os_correction)' % vars())
w.factory('expr::ff_total_qcd_syst_down("@0*@1", ff_tt_qcd, tt_qcd_ss_correction, os[1])' % vars())

# statistical uncertainties on dR and pt closure corrections
for i in [1,2]:
  w.factory('expr::ff_total_qcd_stat_dR_unc%(i)i_up("@0*@1", ff_total, tt_qcd_correction_dR_uncert%(i)i_up)' % vars())
  w.factory('expr::ff_total_qcd_stat_dR_unc%(i)i_down("@0*@1", ff_total, tt_qcd_correction_dR_uncert%(i)i_down)' % vars())

  w.factory('expr::ff_total_qcd_stat_pt_unc%(i)i_up("@0*@1", ff_total, tt_qcd_correction_pt_uncert%(i)i_up)' % vars())
  w.factory('expr::ff_total_qcd_stat_pt_unc%(i)i_down("@0*@1", ff_total, tt_qcd_correction_pt_uncert%(i)i_down)' % vars())

# systematics for wjet and ttbar by scaling the FF by 20% and 40%
w.factory('expr::ff_total_wjets_syst_up("@0*(1+@1*0.2)", ff_total, tt_fracs_wjets)' % vars())
w.factory('expr::ff_total_wjets_syst_down("@0*(1-@1*0.2)", ff_total, tt_fracs_wjets)' % vars())
w.factory('expr::ff_total_ttbar_syst_up("@0*(1+@1*0.4)", ff_total, tt_fracs_ttbar)' % vars())
w.factory('expr::ff_total_ttbar_syst_down("@0*(1-@1*0.4)", ff_total, tt_fracs_ttbar)' % vars())

w.Print()
w.writeToFile('%(output)s/fakefactors_ws_tt_mssm_%(year)s_v3.root' % vars())
w.Delete() 
