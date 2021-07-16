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
parser.add_argument('--channel','-c', help= 'channel', type=str, default='mt')
parser.add_argument('--input','-i', help= 'input', type=str, default='./')
parser.add_argument('--output','-o', help= 'output', type=str, default='./')

args = parser.parse_args()

year = args.year
channel = args.channel
input = args.input
output = args.output


def ZeroNegativeBinContent(h):
  # if a bin of h1 is equal to 0 ir negative then remove it
  # also remove non zero bins with > 100% errors
  for i in range(1,h.GetNbinsX()+1):
    if h.GetBinContent(i) <= 0:
      h.SetBinContent(i,0.0001)
  return h


crosstrg_pt = 0
if channel == 'mt':
  if year == '2016':
    crosstrg_pt = 23
  else:
    crosstrg_pt = 25
if channel == 'et':
  if year == '2016':
    crosstrg_pt = 25
  elif year == '2017':
    crosstrg_pt = 28
  elif year == '2018':
    crosstrg_pt = 33


cmssw_base = subprocess.check_output('echo $CMSSW_BASE', shell=True).replace('\n','')

print '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/workspaceTools.py' % vars()
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

w = ROOT.RooWorkspace('w')

wp = 'medium'

# get fractions

loc = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/%(input)s/' % vars()

#histsToWrap = [(loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_nbjets0_os_qcd' % vars(),   'lt_fracs_nbjets0_os_qcd' % vars()),
#               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_nbjets1_os_qcd' % vars(),   'lt_fracs_nbjets1_os_qcd' % vars()),
#               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_nbjets0_ss_qcd' % vars(),   'lt_fracs_nbjets0_ss_qcd' % vars()),
#               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_nbjets1_ss_qcd' % vars(),   'lt_fracs_nbjets1_ss_qcd' % vars()),
#               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_nbjets0_os_wjets' % vars(), 'lt_fracs_nbjets0_os_wjets' % vars()),
#               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_nbjets1_os_wjets' % vars(), 'lt_fracs_nbjets1_os_wjets' % vars()),
#               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_nbjets0_ss_wjets' % vars(), 'lt_fracs_nbjets0_ss_wjets' % vars()),
#               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_nbjets1_ss_wjets' % vars(), 'lt_fracs_nbjets1_ss_wjets' % vars()),
#               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_nbjets0_os_ttbar' % vars(), 'lt_fracs_nbjets0_os_ttbar' % vars()),
#               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_nbjets1_os_ttbar' % vars(), 'lt_fracs_nbjets1_os_ttbar' % vars()),
#               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_nbjets0_ss_ttbar' % vars(), 'lt_fracs_nbjets0_ss_ttbar' % vars()),
#               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_nbjets1_ss_ttbar' % vars(), 'lt_fracs_nbjets1_ss_ttbar' % vars()),
#              ]

histsToWrap = [(loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_tightmt_nbjets0_os_qcd' % vars(),   'lt_fracs_tightmt_nbjets0_os_qcd' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_tightmt_nbjets1_os_qcd' % vars(),   'lt_fracs_tightmt_nbjets1_os_qcd' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_tightmt_nbjets0_ss_qcd' % vars(),   'lt_fracs_tightmt_nbjets0_ss_qcd' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_tightmt_nbjets1_ss_qcd' % vars(),   'lt_fracs_tightmt_nbjets1_ss_qcd' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_tightmt_nbjets0_os_wjets' % vars(), 'lt_fracs_tightmt_nbjets0_os_wjets' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_tightmt_nbjets1_os_wjets' % vars(), 'lt_fracs_tightmt_nbjets1_os_wjets' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_tightmt_nbjets0_ss_wjets' % vars(), 'lt_fracs_tightmt_nbjets0_ss_wjets' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_tightmt_nbjets1_ss_wjets' % vars(), 'lt_fracs_tightmt_nbjets1_ss_wjets' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_tightmt_nbjets0_os_ttbar' % vars(), 'lt_fracs_tightmt_nbjets0_os_ttbar' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_tightmt_nbjets1_os_ttbar' % vars(), 'lt_fracs_tightmt_nbjets1_os_ttbar' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_tightmt_nbjets0_ss_ttbar' % vars(), 'lt_fracs_tightmt_nbjets0_ss_ttbar' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_tightmt_nbjets1_ss_ttbar' % vars(), 'lt_fracs_tightmt_nbjets1_ss_ttbar' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_loosemt_nbjets0_os_qcd' % vars(),   'lt_fracs_loosemt_nbjets0_os_qcd' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_loosemt_nbjets1_os_qcd' % vars(),   'lt_fracs_loosemt_nbjets1_os_qcd' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_loosemt_nbjets0_ss_qcd' % vars(),   'lt_fracs_loosemt_nbjets0_ss_qcd' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_loosemt_nbjets1_ss_qcd' % vars(),   'lt_fracs_loosemt_nbjets1_ss_qcd' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_loosemt_nbjets0_os_wjets' % vars(), 'lt_fracs_loosemt_nbjets0_os_wjets' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_loosemt_nbjets1_os_wjets' % vars(), 'lt_fracs_loosemt_nbjets1_os_wjets' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_loosemt_nbjets0_ss_wjets' % vars(), 'lt_fracs_loosemt_nbjets0_ss_wjets' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_loosemt_nbjets1_ss_wjets' % vars(), 'lt_fracs_loosemt_nbjets1_ss_wjets' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_loosemt_nbjets0_os_ttbar' % vars(), 'lt_fracs_loosemt_nbjets0_os_ttbar' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_loosemt_nbjets1_os_ttbar' % vars(), 'lt_fracs_loosemt_nbjets1_os_ttbar' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_loosemt_nbjets0_ss_ttbar' % vars(), 'lt_fracs_loosemt_nbjets0_ss_ttbar' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_loosemt_nbjets1_ss_ttbar' % vars(), 'lt_fracs_loosemt_nbjets1_ss_ttbar' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_control_nbjets0_os_qcd' % vars(),   'lt_fracs_control_nbjets0_os_qcd' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_control_nbjets1_os_qcd' % vars(),   'lt_fracs_control_nbjets1_os_qcd' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_control_nbjets0_ss_qcd' % vars(),   'lt_fracs_control_nbjets0_ss_qcd' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_control_nbjets1_ss_qcd' % vars(),   'lt_fracs_control_nbjets1_ss_qcd' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_control_nbjets0_os_wjets' % vars(), 'lt_fracs_control_nbjets0_os_wjets' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_control_nbjets1_os_wjets' % vars(), 'lt_fracs_control_nbjets1_os_wjets' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_control_nbjets0_ss_wjets' % vars(), 'lt_fracs_control_nbjets0_ss_wjets' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_control_nbjets1_ss_wjets' % vars(), 'lt_fracs_control_nbjets1_ss_wjets' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_control_nbjets0_os_ttbar' % vars(), 'lt_fracs_control_nbjets0_os_ttbar' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_control_nbjets1_os_ttbar' % vars(), 'lt_fracs_control_nbjets1_os_ttbar' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_control_nbjets0_ss_ttbar' % vars(), 'lt_fracs_control_nbjets0_ss_ttbar' % vars()),
               (loc + 'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(channel)s_fracs_control_nbjets1_ss_ttbar' % vars(), 'lt_fracs_control_nbjets1_ss_ttbar' % vars()),
              ]


#for task in histsToWrap:
#   wsptools.SafeWrapHist(
#     w, ['expr::mt_max120("min(119.9,@0)",mt[0])'],
#     GetFromTFile(task[0]),
#     name=task[1])

#for i in ['qcd','wjets','ttbar']:
  #w.factory('expr::lt_fracs_%(i)s_nom("(@0!=0)*((@1==0)*(@2)+(@1>=1)*@3)+(@0==0)*((@1==0)*(@4)+(@1>=1)*@5)", os[1],nbjets[0],lt_fracs_nbjets0_os_%(i)s,lt_fracs_nbjets1_os_%(i)s,lt_fracs_nbjets0_ss_%(i)s,lt_fracs_nbjets1_ss_%(i)s)' % vars())

  # if the fractions are set by the user then overwrite these values
  #w.factory('expr::lt_fracs_%(i)s("(@0>=0)*@0 + (@0<0)*@1", %(i)s_frac[-1], lt_fracs_%(i)s_nom)' % vars())

for task in histsToWrap:
   print task[0]
   wsptools.SafeWrapHist(
     w, ['expr::mt_tot_max3000("min(2999.9,@0)",mt_tot[0])'],
     GetFromTFile(task[0]),
     name=task[1])


for i in ['qcd','wjets','ttbar']:
  w.factory('expr::lt_fracs_%(i)s_nom_os_nbjets0("(@0!=0)*( (@1==0 && @2<40)*(@3)+ (@1==0 && @2>=40 && @2<70)*(@4) + (@1==0 && @2>=70)*(@5) )",os[1],nbjets[0],mt[0],lt_fracs_tightmt_nbjets0_os_%(i)s,lt_fracs_loosemt_nbjets0_os_%(i)s,lt_fracs_control_nbjets0_os_%(i)s)' % vars())
  w.factory('expr::lt_fracs_%(i)s_nom_os_nbjets1("(@0!=0)*( (@1>0 && @2<40)*(@3)+ (@1>0 && @2>=40 && @2<70)*(@4) + (@1>0 && @2>=70)*(@5) )",os[1],nbjets[0],mt[0],lt_fracs_tightmt_nbjets1_os_%(i)s,lt_fracs_loosemt_nbjets1_os_%(i)s,lt_fracs_control_nbjets1_os_%(i)s)' % vars())
  w.factory('expr::lt_fracs_%(i)s_nom_ss_nbjets0("(@0==0)*( (@1==0 && @2<40)*(@3)+ (@1==0 && @2>=40 && @2<70)*(@4) + (@1==0 && @2>=70)*(@5) )",os[1],nbjets[0],mt[0],lt_fracs_tightmt_nbjets0_os_%(i)s,lt_fracs_loosemt_nbjets0_os_%(i)s,lt_fracs_control_nbjets0_os_%(i)s)' % vars())
  w.factory('expr::lt_fracs_%(i)s_nom_ss_nbjets1("(@0==0)*( (@1>0 && @2<40)*(@3)+ (@1>0 && @2>=40 && @2<70)*(@4) + (@1>0 && @2>=70)*(@5) )",os[1],nbjets[0],mt[0],lt_fracs_tightmt_nbjets1_os_%(i)s,lt_fracs_loosemt_nbjets1_os_%(i)s,lt_fracs_control_nbjets1_os_%(i)s)' % vars())
  w.factory('expr::lt_fracs_%(i)s_nom("@0 + @1 + @2 + @3", lt_fracs_%(i)s_nom_os_nbjets0, lt_fracs_%(i)s_nom_os_nbjets1, lt_fracs_%(i)s_nom_ss_nbjets0, lt_fracs_%(i)s_nom_ss_nbjets1)' % vars())

  # if the fractions are set by the user then overwrite these values
  w.factory('expr::lt_fracs_%(i)s("(@0>=0)*@0 + (@0<0)*@1", %(i)s_frac[-1], lt_fracs_%(i)s_nom)' % vars())


w.factory('expr::pt_bounded("max(min(599.9,@0),20)",pt[30])' % vars())
w.factory('expr::pt_bounded140("max(min(139.9,@0),20)",pt[30])' % vars())

# pT dependent fits

jetpt_bins = [
            'jet_pt_low',
            'jet_pt_med',
            'jet_pt_high',
]


cutsmap_njets = {
                -1: '@0>=0',
                0: '@0==0',
                1: '@0>=1',
}

cutsmap_jetpt = {
            'jet_pt_low':'@1<1.25*@2',
            'jet_pt_med':'@1>=1.25*@2&&@1<1.5*@2',
            'jet_pt_high':'@1>=1.5*@2',
}

# get all fitted functions for raw fake factors and statistical uncertainties from fit uncertainty bands

for p in ['qcd','wjets','ttbar']:

  if p == 'ttbar': 
    njets_loop = [-1]
  else:
    njets_loop = [0,1]

  for njet in njets_loop:
    for jetpt in jetpt_bins:
 
      if p == 'ttbar':
        name='%(jetpt)s_inclusive_pt_2_ff_%(p)s_mc' % vars()
        short_name='%(jetpt)s_ttbar' % vars()
        func = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(name)s_fit' % vars())
        func_str=str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
        nparams = func.GetNpar()

        w.factory('expr::lt_%(name)s_fit("max(%(func_str)s,0.)",pt_bounded)' % vars())

      else:
        name='%(jetpt)s_%(njet)ijet_pt_2_ff_%(p)s' % vars()
        short_name='%(jetpt)s_%(njet)ijet_%(p)s' % vars()

        for mc_shift in ["Up","Down",""]:
          func = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s%(mc_shift)s.root:%(name)s_fit' % vars())
          func_str=str(func.GetExpFormula('p')).replace('x','@0').replace(',false','')
          nparams = func.GetNpar()

          w.factory('expr::lt_%(name)s_fit%(mc_shift)s("max(%(func_str)s,0.)",pt_bounded)' % vars())

      #func_alt = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s_forcepol1.root:%(name)s_fit' % vars())
      func_alt = GetFromTFile(loc.replace(input.split('/')[0],input.split('/')[0]+'_pol1')+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(name)s_fit' % vars())
      func_str_alt=str(func_alt.GetExpFormula('p')).replace('x','@0').replace(',false','') 
      alt_syst_max = '600.'
      if '>=140' in func_str: alt_syst_max = '140.'
      elif '>=200' in func_str: alt_syst_max = '200.' 
      w.factory('expr::lt_%(name)s_fit_alt_func_up("(@0<%(alt_syst_max)s)*(max(%(func_str_alt)s,0.)/@1) + (@0>=%(alt_syst_max)s)",pt_bounded,lt_%(name)s_fit)' % vars())
   
 
      if p == 'wjets' and njet==1:
        # if wjets and njets=1 we need to also get the function bounded at 140 for ttbar corrections and the corresponding MC function
        #lt_jet_pt_low_1jet_wjets_pt140_fit
        w.factory('expr::lt_%(name)s_pt140_fit("max(%(func_str)s,0.)",pt_bounded140)' % vars()) 
        name_mc = name.replace('wjets','wjets_mc')
        func_mc = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(name_mc)s_fit' % vars())
        func_str_mc=str(func_mc.GetExpFormula('p')).replace('x','@0').replace(',false','')
        w.factory('expr::lt_%(name_mc)s_pt140_fit("max(%(func_str_mc)s,0.)",pt_bounded140)' % vars()) 

      # get stat uncertainties
      hist_nom = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(name)s_uncert' % vars())
      if p == 'ttbar':
        # for the ttbar the FF will be corrected by the ratio of Wdata/Wmc
        # for Wdata we will correlated properly the statistical uncertainties with the W FFs
        # for Wmc to reduce the number of statistical uncertainties we divide ttbar by Wmc and use the corresponding uncertainty band as the uncertainty on ttbar
        wmc_name='%(jetpt)s_1jet_pt_2_ff_wjets_mc' % vars()
        hist_nom_2 = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:%(wmc_name)s_uncert' % vars())
        firstflatbin = hist_nom_2.FindBin(140)
        maxval = hist_nom_2.GetBinContent(firstflatbin -1)
        maxerr = hist_nom_2.GetBinError(firstflatbin -1)
        for i in range(firstflatbin,hist_nom_2.GetNbinsX()+1):
          hist_nom_2.SetBinContent(i, maxval) 
          hist_nom_2.SetBinError(i, maxerr)
        hist_nom.Divide(hist_nom_2)    
      (uncert1_up, uncert1_down, uncert2_up, uncert_2_down) = wsptools.SplitUncert(hist_nom)
      wsptools.SafeWrapHist(w, ['pt_bounded'], hist_nom, name='lt_%(name)s_uncert_nom' % vars())
      wsptools.SafeWrapHist(w, ['pt_bounded'], uncert1_up, name='lt_%(name)s_uncert1_hist_up' % vars())
      wsptools.SafeWrapHist(w, ['pt_bounded'], uncert2_up, name='lt_%(name)s_uncert2_hist_up' % vars())

      njets_cut = cutsmap_njets[njet] % vars() 
      jetpt_cut    = cutsmap_jetpt[jetpt] % vars()
 
      if p == 'wjets' and njet==1:
        # if wjets and njets=1 we need to also get the function bounded at 140 for ttbar corrections
        wsptools.SafeWrapHist(w, ['pt_bounded140'], hist_nom,   name='lt_%(name)s_pt140_uncert_nom' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded140'], uncert1_up, name='lt_%(name)s_pt140_uncert1_hist_up' % vars())
        wsptools.SafeWrapHist(w, ['pt_bounded140'], uncert2_up, name='lt_%(name)s_pt140_uncert2_hist_up' % vars())

        w.factory('expr::lt_%(short_name)s_pt140_uncert1_up("(%(jetpt_cut)s)*(@3/@4) + ((%(jetpt_cut)s)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_pt140_uncert1_hist_up,lt_%(name)s_pt140_uncert_nom)' % vars())
        w.factory('expr::lt_%(short_name)s_pt140_uncert2_up("(%(jetpt_cut)s)*(@3/@4) + ((%(jetpt_cut)s)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_pt140_uncert2_hist_up,lt_%(name)s_pt140_uncert_nom)' % vars())

        w.factory('expr::lt_%(short_name)s_pt140_uncert1_down("2.-@0", lt_%(short_name)s_pt140_uncert1_up)' % vars())
        w.factory('expr::lt_%(short_name)s_pt140_uncert2_down("2.-@0", lt_%(short_name)s_pt140_uncert2_up)' % vars())
 
      if nparams <= 4 and ("Landau" in func_str or "Erf" in func_str): 
        w.factory('expr::lt_%(short_name)s_uncert1_up("(%(njets_cut)s&&%(jetpt_cut)s)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert1_hist_up,lt_%(name)s_uncert_nom)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert2_up("(%(njets_cut)s&&%(jetpt_cut)s)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert2_hist_up,lt_%(name)s_uncert_nom)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert3_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2>200)*1.5 + ((%(njets_cut)s&&%(jetpt_cut)s&&@2>200)==0)",njets[0],jetpt[40],pt_bounded)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert4_up("1",njets[0],jetpt[40],pt_bounded)' % vars()) # for convinience always add a uncert4 even if it is not needed (=1)
      elif nparams == 6 and ("Landau" in func_str or "Erf" in func_str):
        w.factory('expr::lt_%(short_name)s_uncert1_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2<140)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s&&@2<140)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert1_hist_up,lt_%(name)s_uncert_nom)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert2_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2<140)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s&&@2<140)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert2_hist_up,lt_%(name)s_uncert_nom)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert3_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2>=140&&@2<200)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s&&@2>=140&&@2<200)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert2_hist_up,lt_%(name)s_uncert_nom)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert4_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2>=200)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s&&@2>=200)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert2_hist_up,lt_%(name)s_uncert_nom)' % vars())

      elif nparams == 5 and ("Landau" in func_str or "Erf" in func_str):
        # for nparams 5 we have 2 possibilities: either the 140-200 and >200 bins have been merged with a single uncertainty or they are not merged and we have a single uncertainty only for the >200 bin
        # we need to determine whether or not the 140-200 and >200 bins are merged or not
        merged = '>=140' in func_str
#<ROOT.TF1 object ("jet_pt_med_inclusive_pt_2_ff_ttbar_mc_fit") at 0x96dcc60>
        if merged: minpt = '140'
        else: minpt = '200'
        w.factory('expr::lt_%(short_name)s_uncert1_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2<%(minpt)s)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s&&@2<%(minpt)s)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert1_hist_up,lt_%(name)s_uncert_nom)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert2_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2<%(minpt)s)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s&&@2<%(minpt)s)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert2_hist_up,lt_%(name)s_uncert_nom)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert3_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2>=%(minpt)s)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s&&@2>=%(minpt)s)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert2_hist_up,lt_%(name)s_uncert_nom)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert4_up("1",njets[0],jetpt[40],pt_bounded)' % vars()) # for convinience always add a uncert4 even if it is not needed (=1)
      elif nparams <= 2 and not ("Landau" in func_str or "Erf" in func_str): # for pol1
        w.factory('expr::lt_%(short_name)s_uncert1_up("(%(njets_cut)s&&%(jetpt_cut)s)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert1_hist_up,lt_%(name)s_uncert_nom)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert2_up("(%(njets_cut)s&&%(jetpt_cut)s)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert2_hist_up,lt_%(name)s_uncert_nom)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert3_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2>200)*1.5 + ((%(njets_cut)s&&%(jetpt_cut)s&&@2>200)==0)",njets[0],jetpt[40],pt_bounded)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert4_up("1",njets[0],jetpt[40],pt_bounded)' % vars()) # for convinience always add a uncert4 even if it is not needed (=1)
      elif nparams == 4 and not ("Landau" in func_str or "Erf" in func_str): # for pol1
        w.factory('expr::lt_%(short_name)s_uncert1_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2<140)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s&&@2<140)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert1_hist_up,lt_%(name)s_uncert_nom)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert2_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2<140)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s&&@2<140)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert2_hist_up,lt_%(name)s_uncert_nom)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert3_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2>=140&&@2<200)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s&&@2>=140&&@2<200)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert2_hist_up,lt_%(name)s_uncert_nom)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert4_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2>=200)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s&&@2>=200)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert2_hist_up,lt_%(name)s_uncert_nom)' % vars())

      elif nparams == 3 and not ("Landau" in func_str or "Erf" in func_str): # for pol1
        # for nparams 5 we have 2 possibilities: either the 140-200 and >200 bins have been merged with a single uncertainty or they are not merged and we have a single uncertainty only for the >200 bin
        # we need to determine whether or not the 140-200 and >200 bins are merged or not
        merged = '>=140' in func_str
#<ROOT.TF1 object ("jet_pt_med_inclusive_pt_2_ff_ttbar_mc_fit") at 0x96dcc60>
        if merged: minpt = '140'
        else: minpt = '200'
        w.factory('expr::lt_%(short_name)s_uncert1_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2<%(minpt)s)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s&&@2<%(minpt)s)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert1_hist_up,lt_%(name)s_uncert_nom)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert2_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2<%(minpt)s)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s&&@2<%(minpt)s)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert2_hist_up,lt_%(name)s_uncert_nom)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert3_up("(%(njets_cut)s&&%(jetpt_cut)s&&@2>=%(minpt)s)*(@3/@4) + ((%(njets_cut)s&&%(jetpt_cut)s&&@2>=%(minpt)s)==0)",njets[0],jetpt[40],pt_bounded,lt_%(name)s_uncert2_hist_up,lt_%(name)s_uncert_nom)' % vars())
        w.factory('expr::lt_%(short_name)s_uncert4_up("1",njets[0],jetpt[40],pt_bounded)' % vars()) # for convinience always add a uncert4 even if it is not needed (=1)



      w.factory('expr::lt_%(short_name)s_uncert1_down("2.-@0", lt_%(short_name)s_uncert1_up)' % vars())
      w.factory('expr::lt_%(short_name)s_uncert2_down("2.-@0", lt_%(short_name)s_uncert2_up)' % vars())
      w.factory('expr::lt_%(short_name)s_uncert3_down("2.-@0", lt_%(short_name)s_uncert3_up)' % vars())
      w.factory('expr::lt_%(short_name)s_uncert4_down("2.-@0", lt_%(short_name)s_uncert4_up)' % vars())

# make njets / jetpt binned functions

for p in ['wjets','qcd']:

  for mc_shift in ["Up","Down",""]:
    w.factory('expr::ff_lt_%(p)s_raw%(mc_shift)s("(@0==0)*((@1<1.25*@2)*@3 + (@1>=1.25*@2&&@1<1.5*@2)*@4 + (@1>=1.5*@2)*@5) + (@0>0)*((@1<1.25*@2)*@6 + (@1>=1.25*@2&&@1<1.5*@2)*@7 + (@1>=1.5*@2)*@8)", njets[0], jetpt[40], pt_bounded, lt_jet_pt_low_0jet_pt_2_ff_%(p)s_fit%(mc_shift)s, lt_jet_pt_med_0jet_pt_2_ff_%(p)s_fit%(mc_shift)s, lt_jet_pt_high_0jet_pt_2_ff_%(p)s_fit%(mc_shift)s, lt_jet_pt_low_1jet_pt_2_ff_%(p)s_fit%(mc_shift)s, lt_jet_pt_med_1jet_pt_2_ff_%(p)s_fit%(mc_shift)s, lt_jet_pt_high_1jet_pt_2_ff_%(p)s_fit%(mc_shift)s)' % vars())

  w.factory('expr::ff_lt_%(p)s_alt_func_up("(@0==0)*((@1<1.25*@2)*@3 + (@1>=1.25*@2&&@1<1.5*@2)*@4 + (@1>=1.5*@2)*@5) + (@0>0)*((@1<1.25*@2)*@6 + (@1>=1.25*@2&&@1<1.5*@2)*@7 + (@1>=1.5*@2)*@8)", njets[0], jetpt[40], pt_bounded, lt_jet_pt_low_0jet_pt_2_ff_%(p)s_fit_alt_func_up, lt_jet_pt_med_0jet_pt_2_ff_%(p)s_fit_alt_func_up, lt_jet_pt_high_0jet_pt_2_ff_%(p)s_fit_alt_func_up, lt_jet_pt_low_1jet_pt_2_ff_%(p)s_fit_alt_func_up, lt_jet_pt_med_1jet_pt_2_ff_%(p)s_fit_alt_func_up, lt_jet_pt_high_1jet_pt_2_ff_%(p)s_fit_alt_func_up)' % vars())
  w.factory('expr::ff_lt_%(p)s_alt_func_down("2-@0",ff_lt_%(p)s_alt_func_up)' % vars())

w.factory('expr::ff_lt_ttbar_raw("(@0>=0)*((@1<1.25*@2)*@3 + (@1>=1.25*@2&&@1<1.5*@2)*@4 + (@1>=1.5*@2)*@5)", njets[0], jetpt[40], pt_bounded, lt_jet_pt_low_inclusive_pt_2_ff_ttbar_mc_fit, lt_jet_pt_med_inclusive_pt_2_ff_ttbar_mc_fit, lt_jet_pt_high_inclusive_pt_2_ff_ttbar_mc_fit)' % vars())

w.factory('expr::ff_lt_ttbar_alt_func_up("(@0>=0)*((@1<1.25*@2)*@3 + (@1>=1.25*@2&&@1<1.5*@2)*@4 + (@1>=1.5*@2)*@5)", njets[0], jetpt[40], pt_bounded, lt_jet_pt_low_inclusive_pt_2_ff_ttbar_mc_fit_alt_func_up, lt_jet_pt_med_inclusive_pt_2_ff_ttbar_mc_fit_alt_func_up, lt_jet_pt_high_inclusive_pt_2_ff_ttbar_mc_fit_alt_func_up)' % vars())
w.factory('expr::ff_lt_ttbar_alt_func_down("2-@0",ff_lt_ttbar_alt_func_up)')

# apply qcd corrections

w.factory('expr::met_var_qcd_bounded("@0",met_var_qcd[0])' % vars())
w.factory('expr::met_var_w_bounded("@0",met_var_w[0])' % vars())
#w.factory('expr::l_pt_bounded100("min(@0,99.9)",l_pt[20])' % vars())
w.factory('expr::l_pt_bounded100("min(@0,199.9)",l_pt[20])' % vars()) # hacky change
w.factory('expr::l_pt_bounded140("min(@0,139.9)",l_pt[20])' % vars())
w.factory('expr::l_pt_bounded160("min(@0,159.9)",l_pt[20])' % vars())
#w.factory('expr::l_pt_bounded200("min(@0,199.9)",l_pt[20])' % vars())
w.factory('expr::l_pt_bounded200("min(@0,299.9)",l_pt[20])' % vars()) # hacky change
w.factory('expr::l_pt_bounded250("min(@0,249.9)",l_pt[20])' % vars())
w.factory('expr::iso_bounded("min(@0,0.5)",iso[0])' % vars())

for njet in [0,1]:
  for mc_shift in ["Up","Down",""]:
    func_ss = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s%(mc_shift)s.root:met_%(njet)ijet_closure_qcd_fit' % vars())
    func_ss_str=str(func_ss.GetExpFormula('p')).replace('x','@0').replace(',false','')
  
    func_os_1 = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s%(mc_shift)s.root:pt_1_nbjets%(njet)i_dr_to_ar_aiso_closure_qcd_fit' % vars())
    func_os_str_1=str(func_os_1.GetExpFormula('p')).replace('x','@0').replace(',false','')

    w.factory('expr::lt_qcd_ss_njets%(njet)i_correction%(mc_shift)s("max(%(func_ss_str)s,0.)",met_var_qcd_bounded)' % vars())
    w.factory('expr::lt_qcd_os_nbjets%(njet)i_correction%(mc_shift)s("max(%(func_os_str_1)s,0.)",l_pt_bounded100)' % vars())

  # get stat uncertainties

  print 'met_%(njet)ijet_closure_qcd_uncert' % vars()
  hist_ss_nom = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:met_%(njet)ijet_closure_qcd_uncert' % vars())
  hist_os_nom_1 = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:pt_1_nbjets%(njet)i_dr_to_ar_aiso_closure_qcd_uncert' % vars())

  hist_ss_nom = ZeroNegativeBinContent(hist_ss_nom)

  #low_pt = hist_os_nom_1.GetBinContent(1) # might need a low pT correction for below the cross trigger threshold!
  (uncert1_up, uncert1_down, uncert2_up, uncert_2_down) = wsptools.SplitUncert(hist_ss_nom)

  wsptools.SafeWrapHist(w, ['met_var_qcd_bounded'], hist_ss_nom, name='lt_qcd_ss_njets%(njet)i_correction_nom' % vars())
  wsptools.SafeWrapHist(w, ['met_var_qcd_bounded'], uncert1_up, name='lt_qcd_ss_njets%(njet)i_correction_uncert1_hist_up' % vars())
  wsptools.SafeWrapHist(w, ['met_var_qcd_bounded'], uncert2_up, name='lt_qcd_ss_njets%(njet)i_correction_uncert2_hist_up' % vars())

  (uncert1_up, uncert1_down, uncert2_up, uncert_2_down) = wsptools.SplitUncert(hist_os_nom_1)

  wsptools.SafeWrapHist(w, ['l_pt_bounded100'], hist_os_nom_1, name='lt_qcd_os_nbjets%(njet)i_correction_nom' % vars())
  wsptools.SafeWrapHist(w, ['l_pt_bounded100'], uncert1_up,     name='lt_qcd_os_nbjets%(njet)i_correction_uncert1_hist_up' % vars())
  wsptools.SafeWrapHist(w, ['l_pt_bounded100'], uncert2_up,     name='lt_qcd_os_nbjets%(njet)i_correction_uncert2_hist_up' % vars())
  
  w.factory('expr::lt_qcd_ss_njets%(njet)i_correction_uncert1_up("@0/@1", lt_qcd_ss_njets%(njet)i_correction_uncert1_hist_up, lt_qcd_ss_njets%(njet)i_correction_nom)' % vars())
  w.factory('expr::lt_qcd_ss_njets%(njet)i_correction_uncert2_up("@0/@1", lt_qcd_ss_njets%(njet)i_correction_uncert2_hist_up, lt_qcd_ss_njets%(njet)i_correction_nom)' % vars())

  w.factory('expr::lt_qcd_os_nbjets%(njet)i_correction_uncert1_up("@0/@1", lt_qcd_os_nbjets%(njet)i_correction_uncert1_hist_up, lt_qcd_os_nbjets%(njet)i_correction_nom)' % vars())
  w.factory('expr::lt_qcd_os_nbjets%(njet)i_correction_uncert2_up("@0/@1", lt_qcd_os_nbjets%(njet)i_correction_uncert2_hist_up, lt_qcd_os_nbjets%(njet)i_correction_nom)' % vars())

# iso>0.05 corrections and uncertainties
#iso_inclusive_closure_qcd_alt_fit
for mc_shift in ["Up","Down",""]:
  func_iso_alt = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s%(mc_shift)s.root:iso_inclusive_closure_qcd_alt_fit' % vars())
  func_iso_alt_str=str(func_iso_alt.GetExpFormula('p')).replace('x','@0').replace(',false','')

  func_iso = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s%(mc_shift)s.root:iso_inclusive_closure_qcd_fit' % vars())
  func_iso_str=str(func_iso.GetExpFormula('p')).replace('x','@0').replace(',false','')
  hist_iso = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s%(mc_shift)s.root:iso_inclusive_closure_qcd_uncert' % vars())

  w.factory('expr::lt_iso_correction%(mc_shift)s("max(%(func_iso_str)s,0.)",iso_bounded )' % vars())

(uncert1_up, uncert1_down, uncert2_up, uncert_2_down) = wsptools.SplitUncert(hist_iso)

wsptools.SafeWrapHist(w, ['iso_bounded'], hist_iso, name='lt_qcd_iso_correction_nom' % vars())
wsptools.SafeWrapHist(w, ['iso_bounded'], uncert1_up,     name='lt_qcd_iso_correction_uncert1_hist_up' % vars())
wsptools.SafeWrapHist(w, ['iso_bounded'], uncert2_up,     name='lt_qcd_iso_correction_uncert2_hist_up' % vars())

w.factory('expr::lt_iso_correction_uncert1_up("@0/@1",lt_qcd_iso_correction_uncert1_hist_up, lt_qcd_iso_correction_nom)' % vars())
w.factory('expr::lt_iso_correction_uncert2_up("@0/@1",lt_qcd_iso_correction_uncert2_hist_up, lt_qcd_iso_correction_nom)' % vars())
w.factory('expr::lt_iso_correction_syst_up("max(%(func_iso_alt_str)s,0.)/@1",iso_bounded, lt_iso_correction)' % vars())

w.factory('expr::lt_iso_correction_uncert1_down("2.-@0", lt_iso_correction_uncert1_up)' % vars())
w.factory('expr::lt_iso_correction_uncert2_down("2.-@0", lt_iso_correction_uncert2_up)' % vars())
w.factory('expr::lt_iso_correction_syst_down("2.-@0", lt_iso_correction_syst_up)' % vars())

# add these as systs further down!

######

#pt closure

for mc_shift in ["Up","Down",""]:
  hist_l_pt = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s%(mc_shift)s.root:l_pt_closure_qcd' % vars())
  l_pt_low = hist_l_pt.GetBinContent(1)
  l_pt_high = hist_l_pt.GetBinContent(2)
  l_pt_low_uncert = hist_l_pt.GetBinError(1)
  l_pt_high_uncert = hist_l_pt.GetBinError(2)

  w.factory('expr::lt_qcd_ss_correction%(mc_shift)s("((@0==0)*@1 + (@0>0)*@2)*((@3<%(crosstrg_pt)s)*%(l_pt_low).5f + (@3>=%(crosstrg_pt)s)*%(l_pt_high).5f)*@4",njets[0], lt_qcd_ss_njets0_correction%(mc_shift)s, lt_qcd_ss_njets1_correction%(mc_shift)s, l_pt_bounded200, lt_iso_correction%(mc_shift)s )' % vars())

  w.factory('expr::lt_qcd_os_correction%(mc_shift)s("(@0==0)*@1 + (@0>0)*@2", nbjets[0], lt_qcd_os_nbjets0_correction%(mc_shift)s, lt_qcd_os_nbjets1_correction%(mc_shift)s)' % vars())

w.factory('expr::lt_qcd_ss_correction_l_pt_uncert1_up("((@0==0)*@1 + (@0>0)*@2)*((@3<%(crosstrg_pt)s)*(%(l_pt_low).5f+%(l_pt_low_uncert).5f) + (@3>=%(crosstrg_pt)s)*(%(l_pt_high).5f))*@4/@5",njets[0], lt_qcd_ss_njets0_correction, lt_qcd_ss_njets1_correction, l_pt_bounded200, lt_iso_correction, lt_qcd_ss_correction)' % vars())

w.factory('expr::lt_qcd_ss_correction_l_pt_uncert1_down("((@0==0)*@1 + (@0>0)*@2)*((@3<%(crosstrg_pt)s)*(%(l_pt_low).5f-%(l_pt_low_uncert).5f) + (@3>=%(crosstrg_pt)s)*(%(l_pt_high).5f))*@4/@5",njets[0], lt_qcd_ss_njets0_correction, lt_qcd_ss_njets1_correction, l_pt_bounded200, lt_iso_correction, lt_qcd_ss_correction)' % vars())

w.factory('expr::lt_qcd_ss_correction_l_pt_uncert2_up("((@0==0)*@1 + (@0>0)*@2)*((@3<%(crosstrg_pt)s)*(%(l_pt_low).5f) + (@3>=%(crosstrg_pt)s)*(%(l_pt_high).5f+%(l_pt_high_uncert).5f))*@4/@5",njets[0], lt_qcd_ss_njets0_correction, lt_qcd_ss_njets1_correction, l_pt_bounded200, lt_iso_correction, lt_qcd_ss_correction)' % vars())
w.factory('expr::lt_qcd_ss_correction_l_pt_uncert2_down("((@0==0)*@1 + (@0>0)*@2)*((@3<%(crosstrg_pt)s)*(%(l_pt_low).5f) + (@3>=%(crosstrg_pt)s)*(%(l_pt_high).5f-%(l_pt_high_uncert).5f))*@4/@5",njets[0], lt_qcd_ss_njets0_correction, lt_qcd_ss_njets1_correction, l_pt_bounded200, lt_iso_correction, lt_qcd_ss_correction)' % vars())


for i in [1,2]:

  w.factory('expr::lt_qcd_ss_correction_njets0_uncert%(i)i_up("(@0==0)*@1 + (@0!=0)", njets[0], lt_qcd_ss_njets0_correction_uncert%(i)i_up)' % vars())
  w.factory('expr::lt_qcd_ss_correction_njets1_uncert%(i)i_up("(@0==0) + (@0!=0)*@1", njets[0], lt_qcd_ss_njets1_correction_uncert%(i)i_up)' % vars())

  w.factory('expr::lt_qcd_os_correction_uncert%(i)i_up("(@0!=0)*((@1==0)*@2 + (@1>0)*@3) + (@0==0) ",os[1], nbjets[0], lt_qcd_os_nbjets0_correction_uncert%(i)i_up, lt_qcd_os_nbjets1_correction_uncert%(i)i_up)' % vars())

  w.factory('expr::lt_qcd_ss_correction_njets0_uncert%(i)i_down("2.-@0", lt_qcd_ss_correction_njets0_uncert%(i)i_up)' % vars())
  w.factory('expr::lt_qcd_ss_correction_njets1_uncert%(i)i_down("2.-@0", lt_qcd_ss_correction_njets1_uncert%(i)i_up)' % vars())
  w.factory('expr::lt_qcd_os_correction_uncert%(i)i_down("2.-@0", lt_qcd_os_correction_uncert%(i)i_up)' % vars())


w.factory('expr::ff_lt_qcd_syst_met_closure_up("@0*@1*@1*((@2!=0)*@3 + (@2==0))", ff_lt_qcd_raw, lt_qcd_ss_correction, os[1], lt_qcd_os_correction, met_var_w_bounded, mt[0])' % vars())
w.factory('expr::ff_lt_qcd_syst_met_closure_down("@0*((@1!=0)*@2 + (@1==0))", ff_lt_qcd_raw, os[1], lt_qcd_os_correction, met_var_w_bounded, mt[0])' % vars())


# get final qcd fake factor
for mc_shift in ["Up","Down",""]:
  w.factory('expr::ff_lt_qcd%(mc_shift)s("@0*@1*((@2!=0)*@3 + (@2==0))", ff_lt_qcd_raw%(mc_shift)s, lt_qcd_ss_correction%(mc_shift)s, os[1], lt_qcd_os_correction%(mc_shift)s, met_var_w_bounded, mt[0])' % vars())

qcd_systs=[]

qcd_systs.append('qcd_syst_met_closure')

# statistical uncertainties on measured qcd fake factors
# add statistical uncertainties 1 per njets/jetpt bin
for njet in [0,1]:
  for jetpt in jetpt_bins:
      short_name='%(jetpt)s_%(njet)ijet_qcd' % vars()
      for i in [1,2,3]:
        w.factory('expr::ff_lt_qcd_stat_njet%(njet)i_%(jetpt)s_unc%(i)i_up("@0*@1",ff_lt_qcd, lt_%(short_name)s_uncert%(i)i_up)' % vars())
        w.factory('expr::ff_lt_qcd_stat_njet%(njet)i_%(jetpt)s_unc%(i)i_down("@0*@1",ff_lt_qcd, lt_%(short_name)s_uncert%(i)i_down)' % vars())
        qcd_systs.append('qcd_stat_njet%(njet)i_%(jetpt)s_unc%(i)i' % vars())

# systematic uncertainty from applying os/ss correction twice or not applying it 
w.factory('expr::ff_lt_qcd_syst_up("@0*@1*((@2!=0)*@3*@3 + (@2==0))", ff_lt_qcd_raw, lt_qcd_ss_correction, os[1], lt_qcd_os_correction)' % vars())
w.factory('expr::ff_lt_qcd_syst_down("@0*@1", ff_lt_qcd_raw, lt_qcd_ss_correction, os[1])' % vars())
qcd_systs.append('qcd_syst')

## statistical uncertainties on os and ss closure corrections
for i in [1,2]:
#lt_qcd_os_correction_uncert%(i)i_up
  w.factory('expr::ff_lt_qcd_stat_ss_njets0_unc%(i)i_up("@0*@1", ff_lt_qcd, lt_qcd_ss_correction_njets0_uncert%(i)i_up)' % vars())
  w.factory('expr::ff_lt_qcd_stat_ss_njets0_unc%(i)i_down("@0*@1", ff_lt_qcd, lt_qcd_ss_correction_njets0_uncert%(i)i_down)' % vars())
  w.factory('expr::ff_lt_qcd_stat_ss_njets1_unc%(i)i_up("@0*@1", ff_lt_qcd, lt_qcd_ss_correction_njets1_uncert%(i)i_up)' % vars())
  w.factory('expr::ff_lt_qcd_stat_ss_njets1_unc%(i)i_down("@0*@1", ff_lt_qcd, lt_qcd_ss_correction_njets1_uncert%(i)i_down)' % vars())
  qcd_systs.append('qcd_stat_ss_njets0_unc%(i)i' % vars())
  qcd_systs.append('qcd_stat_ss_njets1_unc%(i)i' % vars())

  w.factory('expr::ff_lt_qcd_stat_os_unc%(i)i_up("@0*@1", ff_lt_qcd, lt_qcd_os_correction_uncert%(i)i_up)' % vars())
  w.factory('expr::ff_lt_qcd_stat_os_unc%(i)i_down("@0*@1", ff_lt_qcd, lt_qcd_os_correction_uncert%(i)i_down)' % vars())
  qcd_systs.append('qcd_stat_os_unc%(i)i' % vars())

  w.factory('expr::ff_lt_qcd_stat_l_pt_unc%(i)i_up("@0*@1", ff_lt_qcd, lt_qcd_ss_correction_l_pt_uncert%(i)i_up)' % vars())
  w.factory('expr::ff_lt_qcd_stat_l_pt_unc%(i)i_down("@0*@1", ff_lt_qcd, lt_qcd_ss_correction_l_pt_uncert%(i)i_down)' % vars())
  qcd_systs.append('qcd_stat_l_pt_unc%(i)i' % vars())

  w.factory('expr::ff_lt_qcd_stat_iso_unc%(i)i_up("@0*@1", ff_lt_qcd, lt_iso_correction_uncert%(i)i_up)' % vars())
  w.factory('expr::ff_lt_qcd_stat_iso_unc%(i)i_down("@0*@1", ff_lt_qcd, lt_iso_correction_uncert%(i)i_down)' % vars())
  qcd_systs.append('qcd_stat_iso_unc%(i)i' % vars())


w.factory('expr::ff_lt_qcd_syst_iso_up("@0*@1", ff_lt_qcd, lt_iso_correction_syst_up)' % vars())
w.factory('expr::ff_lt_qcd_syst_iso_down("@0*@1", ff_lt_qcd, lt_iso_correction_syst_down)' % vars())
qcd_systs.append('qcd_syst_iso' % vars())

print 'qcd systematics:'
print qcd_systs

#################

# apply wjets corrections

#met in njets bins, pt in njets bins and low and high pT region, extrap in nbjets loose and tight mt (vs met)

for njet in [0,1]:
  for mc_shift in ["Up","Down",""]:
    func_met = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s%(mc_shift)s.root:met_%(njet)ijet_closure_wjets_fit' % vars())
    func_met_str=str(func_met.GetExpFormula('p')).replace('x','@0').replace(',false','')

    func_pt = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s%(mc_shift)s.root:l_pt_%(njet)ijet_closure_wjets_fit' % vars())
    func_pt_str=str(func_pt.GetExpFormula('p')).replace('x','@0').replace(',false','')

    func_extrap_1 = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:pt_1_nbjets%(njet)i_tightmt_dr_to_ar_closure_wjets_mc_fit' % vars())
    hist_extrap_1 = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:pt_1_nbjets%(njet)i_tightmt_dr_to_ar_closure_wjets_mc' % vars())
    func_extrap_str_1=str(func_extrap_1.GetExpFormula('p')).replace('x','@0').replace(',false','')

    func_extrap_2 = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:pt_1_nbjets%(njet)i_loosemt_dr_to_ar_closure_wjets_mc_fit' % vars())
    hist_extrap_2 = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:pt_1_nbjets%(njet)i_loosemt_dr_to_ar_closure_wjets_mc' % vars())
    func_extrap_str_2=str(func_extrap_2.GetExpFormula('p')).replace('x','@0').replace(',false','')

    lowpt_extrap_1 = hist_extrap_1.GetBinContent(1)
    lowpt_extrap_2 = hist_extrap_2.GetBinContent(1)

    if lowpt_extrap_1<=0: lowpt_extrap_1 = 1.
    if lowpt_extrap_2<=0: lowpt_extrap_2 = 1.

    lowpt_corr = 1.
    if not (channel == 'et' and year == '2016'):
      hist_l_pt = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s%(mc_shift)s.root:l_pt_%(njet)ijet_closure_wjets' % vars())
      lowpt_corr = hist_l_pt.GetBinContent(1)
 
    if lowpt_corr<=0: lowpt_corr = 1.


    w.factory('expr::lt_wjets_met_njets%(njet)i_correction%(mc_shift)s("max(%(func_met_str)s,0.)",met_var_w_bounded)' % vars())
    if njet ==0: w.factory('expr::lt_wjets_l_pt_njets%(njet)i_correction%(mc_shift)s("max(%(func_pt_str)s,0.)",l_pt_bounded100)' % vars())
    else: w.factory('expr::lt_wjets_l_pt_njets%(njet)i_correction%(mc_shift)s("max(%(func_pt_str)s,0.)",l_pt_bounded200)' % vars())
    w.factory('expr::lt_wjets_low_l_pt_njets%(njet)i_correction%(mc_shift)s("%(lowpt_corr)s",l_pt_bounded200)' % vars())
  w.factory('expr::lt_wjets_extrap_nbjets%(njet)i_tightmt_correction("(@0<%(crosstrg_pt)s)*%(lowpt_extrap_1).5f + (@0>=%(crosstrg_pt)s)*max(%(func_extrap_str_1)s,0.)",l_pt_bounded200)' % vars())
  w.factory('expr::lt_wjets_extrap_nbjets%(njet)i_loosemt_correction("(@0<%(crosstrg_pt)s)*%(lowpt_extrap_2).5f + (@0>=%(crosstrg_pt)s)*max(%(func_extrap_str_2)s,0.)",l_pt_bounded200)' % vars())

  # get stat uncertainties

  hist_met = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:met_%(njet)ijet_closure_wjets_uncert' % vars())
  hist_pt = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:l_pt_%(njet)ijet_closure_wjets_uncert' % vars())
  hist_extrap_1 = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:pt_1_nbjets%(njet)i_tightmt_dr_to_ar_closure_wjets_mc_uncert' % vars())
  hist_extrap_2 = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:pt_1_nbjets%(njet)i_loosemt_dr_to_ar_closure_wjets_mc_uncert' % vars())

  (uncert1_up, uncert1_down, uncert2_up, uncert_2_down) = wsptools.SplitUncert(hist_met)

 
  wsptools.SafeWrapHist(w, ['met_var_w_bounded'], hist_met, name='lt_wjets_met_njets%(njet)i_correction_nom' % vars())
  wsptools.SafeWrapHist(w, ['met_var_w_bounded'], uncert1_up, name='lt_wjets_met_njets%(njet)i_correction_uncert1_hist_up' % vars())
  wsptools.SafeWrapHist(w, ['met_var_w_bounded'], uncert2_up, name='lt_wjets_met_njets%(njet)i_correction_uncert2_hist_up' % vars())

  (uncert1_up, uncert1_down, uncert2_up, uncert_2_down) = wsptools.SplitUncert(hist_pt)

  if njet ==0:
    wsptools.SafeWrapHist(w, ['l_pt_bounded100'], hist_pt, name='lt_wjets_l_pt_njets%(njet)i_correction_nom' % vars())
    wsptools.SafeWrapHist(w, ['l_pt_bounded100'], uncert1_up, name='lt_wjets_l_pt_njets%(njet)i_correction_uncert1_hist_up' % vars())
    wsptools.SafeWrapHist(w, ['l_pt_bounded100'], uncert2_up, name='lt_wjets_l_pt_njets%(njet)i_correction_uncert2_hist_up' % vars())
  else:
    wsptools.SafeWrapHist(w, ['l_pt_bounded200'], hist_pt, name='lt_wjets_l_pt_njets%(njet)i_correction_nom' % vars())
    wsptools.SafeWrapHist(w, ['l_pt_bounded200'], uncert1_up, name='lt_wjets_l_pt_njets%(njet)i_correction_uncert1_hist_up' % vars())
    wsptools.SafeWrapHist(w, ['l_pt_bounded200'], uncert2_up, name='lt_wjets_l_pt_njets%(njet)i_correction_uncert2_hist_up' % vars())

  (uncert1_up, uncert1_down, uncert2_up, uncert_2_down) = wsptools.SplitUncert(hist_extrap_1)

  wsptools.SafeWrapHist(w, ['l_pt_bounded200'], hist_extrap_1,  name='lt_wjets_extrap_nbjets%(njet)i_tightmt_correction_nom' % vars())
  wsptools.SafeWrapHist(w, ['l_pt_bounded200'], uncert1_up,     name='lt_wjets_extrap_nbjets%(njet)i_tightmt_correction_uncert1_hist_up' % vars())
  wsptools.SafeWrapHist(w, ['l_pt_bounded200'], uncert2_up,     name='lt_wjets_extrap_nbjets%(njet)i_tightmt_correction_uncert2_hist_up' % vars())

  wsptools.SafeWrapHist(w, ['l_pt_bounded200'], hist_extrap_2,  name='lt_wjets_extrap_nbjets%(njet)i_loosemt_correction_nom' % vars())
  wsptools.SafeWrapHist(w, ['l_pt_bounded200'], uncert1_up,     name='lt_wjets_extrap_nbjets%(njet)i_loosemt_correction_uncert1_hist_up' % vars())
  wsptools.SafeWrapHist(w, ['l_pt_bounded200'], uncert2_up,     name='lt_wjets_extrap_nbjets%(njet)i_loosemt_correction_uncert2_hist_up' % vars())

  w.factory('expr::lt_wjets_met_njets%(njet)i_correction_uncert1_up("@0/@1", lt_wjets_met_njets%(njet)i_correction_uncert1_hist_up, lt_wjets_met_njets%(njet)i_correction_nom)' % vars())
  w.factory('expr::lt_wjets_met_njets%(njet)i_correction_uncert2_up("@0/@1", lt_wjets_met_njets%(njet)i_correction_uncert2_hist_up, lt_wjets_met_njets%(njet)i_correction_nom)' % vars())

  w.factory('expr::lt_wjets_l_pt_njets%(njet)i_correction_uncert1_up("@0/@1", lt_wjets_l_pt_njets%(njet)i_correction_uncert1_hist_up, lt_wjets_l_pt_njets%(njet)i_correction_nom)' % vars())
  w.factory('expr::lt_wjets_l_pt_njets%(njet)i_correction_uncert2_up("@0/@1", lt_wjets_l_pt_njets%(njet)i_correction_uncert2_hist_up, lt_wjets_l_pt_njets%(njet)i_correction_nom)' % vars())

  for x in ['loose', 'tight']:
    w.factory('expr::lt_wjets_extrap_nbjets%(njet)i_%(x)smt_correction_uncert1_up("@0/@1", lt_wjets_extrap_nbjets%(njet)i_%(x)smt_correction_uncert1_hist_up, lt_wjets_extrap_nbjets%(njet)i_%(x)smt_correction_nom)' % vars())
    w.factory('expr::lt_wjets_extrap_nbjets%(njet)i_%(x)smt_correction_uncert2_up("@0/@1", lt_wjets_extrap_nbjets%(njet)i_%(x)smt_correction_uncert2_hist_up, lt_wjets_extrap_nbjets%(njet)i_%(x)smt_correction_nom)' % vars())

for mc_shift in ["Up","Down",""]:
  w.factory('expr::lt_wjets_dr_correction%(mc_shift)s("((@0==0)*@1 + (@0>0)*@2)*((@3<%(crosstrg_pt)s)*((@0==0)*@4 + (@0>0)*@5) + (@3>=%(crosstrg_pt)s)*((@0==0)*@6 + (@0>0)*@7))",njets[0], lt_wjets_met_njets0_correction%(mc_shift)s, lt_wjets_met_njets1_correction%(mc_shift)s, l_pt_bounded200, lt_wjets_low_l_pt_njets0_correction%(mc_shift)s, lt_wjets_low_l_pt_njets1_correction%(mc_shift)s, lt_wjets_l_pt_njets0_correction%(mc_shift)s, lt_wjets_l_pt_njets1_correction%(mc_shift)s )' % vars())
w.factory('expr::lt_wjets_extrap_correction("(@0>=70) + (@0<70&&@0>=40)*((@1==0)*@2 + (@1>0)*@3) + (@0<40)*((@1==0)*@4 + (@1>0)*@5)", mt[0], nbjets[0], lt_wjets_extrap_nbjets0_loosemt_correction, lt_wjets_extrap_nbjets1_loosemt_correction, lt_wjets_extrap_nbjets0_tightmt_correction, lt_wjets_extrap_nbjets1_tightmt_correction )' % vars())

for i in [1,2]:

  w.factory('expr::lt_wjets_met_correction_njets0_uncert%(i)i_up("(@0==0)*@1 + (@0!=0)", njets[0], lt_wjets_met_njets0_correction_uncert%(i)i_up)' % vars())
  w.factory('expr::lt_wjets_met_correction_njets1_uncert%(i)i_up("(@0==0) + (@0!=0)*@1", njets[0], lt_wjets_met_njets1_correction_uncert%(i)i_up)' % vars())
  w.factory('expr::lt_wjets_l_pt_correction_njets0_uncert%(i)i_up("((@0==0)*@1 + (@0!=0))*(@2>=%(crosstrg_pt)s) + (@2<%(crosstrg_pt)s)", njets[0], lt_wjets_l_pt_njets0_correction_uncert%(i)i_up, l_pt_bounded200)' % vars())
  w.factory('expr::lt_wjets_l_pt_correction_njets1_uncert%(i)i_up("((@0==0) + (@0!=0)*@1)*(@2>=%(crosstrg_pt)s) + (@2<%(crosstrg_pt)s)", njets[0], lt_wjets_l_pt_njets1_correction_uncert%(i)i_up, l_pt_bounded200)' % vars())

  w.factory('expr::lt_wjets_extrap_correction_uncert%(i)i_up("(@0>=70) + (@0<70&&@0>=40)*((@1==0)*@2 + (@1>0)*@3) + (@0<40)*((@1==0)*@4 + (@1>0)*@5) ",mt[0], nbjets[0], lt_wjets_extrap_nbjets0_loosemt_correction_uncert%(i)i_up, lt_wjets_extrap_nbjets1_loosemt_correction_uncert%(i)i_up, lt_wjets_extrap_nbjets0_tightmt_correction_uncert%(i)i_up, lt_wjets_extrap_nbjets1_tightmt_correction_uncert%(i)i_up)' % vars())

  w.factory('expr::lt_wjets_met_correction_njets0_uncert%(i)i_down("2.-@0", lt_wjets_met_correction_njets0_uncert%(i)i_up)' % vars())
  w.factory('expr::lt_wjets_met_correction_njets1_uncert%(i)i_down("2.-@0", lt_wjets_met_correction_njets1_uncert%(i)i_up)' % vars())
  w.factory('expr::lt_wjets_l_pt_correction_njets0_uncert%(i)i_down("2.-@0", lt_wjets_l_pt_correction_njets0_uncert%(i)i_up)' % vars())
  w.factory('expr::lt_wjets_l_pt_correction_njets1_uncert%(i)i_down("2.-@0", lt_wjets_l_pt_correction_njets1_uncert%(i)i_up)' % vars())
  w.factory('expr::lt_wjets_extrap_correction_uncert%(i)i_down("2.-@0", lt_wjets_extrap_correction_uncert%(i)i_up)' % vars())

w.factory('expr::ff_lt_wjets_dr_closure_syst_up("@0*@1*@1*@2", ff_lt_wjets_raw, lt_wjets_dr_correction, lt_wjets_extrap_correction, os[1], met_var_qcd_bounded, iso_bounded)' % vars())
w.factory('expr::ff_lt_wjets_dr_closure_syst_down("@0*@1", ff_lt_wjets_raw, lt_wjets_extrap_correction, os[1], met_var_qcd_bounded, iso_bounded)' % vars())

# get final wjets fake factor
for mc_shift in ["Up","Down",""]:
  w.factory('expr::ff_lt_wjets%(mc_shift)s("@0*@1*@2", ff_lt_wjets_raw%(mc_shift)s, lt_wjets_dr_correction%(mc_shift)s, lt_wjets_extrap_correction, os[1], met_var_qcd_bounded, iso_bounded)' % vars())

wjets_systs=[]

# systematic uncertainty for wjets dr closure corrections applied twice or not at all
w.factory('expr::ff_lt_wjets_syst_met_closure_up("@0*(((@1==0)*@2*@2 + (@1>0)*@3*@3)*((@4<%(crosstrg_pt)s)*((@1==0)*@5 + (@1>0)*@6) + (@4>=%(crosstrg_pt)s)*((@1==0)*@7 + (@1>0)*@8)))*@9",ff_lt_wjets_raw,njets[0], lt_wjets_met_njets0_correction, lt_wjets_met_njets1_correction, l_pt_bounded200, lt_wjets_low_l_pt_njets0_correction, lt_wjets_low_l_pt_njets1_correction, lt_wjets_l_pt_njets0_correction, lt_wjets_l_pt_njets1_correction,lt_wjets_extrap_correction)' % vars())
w.factory('expr::ff_lt_wjets_syst_met_closure_down("@0*(((@1==0)*@2/@2 + (@1>0)*@3/@3)*((@4<%(crosstrg_pt)s)*((@1==0)*@5 + (@1>0)*@6) + (@4>=%(crosstrg_pt)s)*((@1==0)*@7 + (@1>0)*@8)))*@9",ff_lt_wjets_raw,njets[0], lt_wjets_met_njets0_correction, lt_wjets_met_njets1_correction, l_pt_bounded200, lt_wjets_low_l_pt_njets0_correction, lt_wjets_low_l_pt_njets1_correction, lt_wjets_l_pt_njets0_correction, lt_wjets_l_pt_njets1_correction,lt_wjets_extrap_correction)' % vars())

w.factory('expr::ff_lt_wjets_syst_l_pt_closure_up("@0*(((@1==0)*@2 + (@1>0)*@3)*((@4<%(crosstrg_pt)s)*((@1==0)*@5*@5 + (@1>0)*@6*@6) + (@4>=%(crosstrg_pt)s)*((@1==0)*@7*@7 + (@1>0)*@8*@8)))*@9",ff_lt_wjets_raw,njets[0], lt_wjets_met_njets0_correction, lt_wjets_met_njets1_correction, l_pt_bounded200, lt_wjets_low_l_pt_njets0_correction, lt_wjets_low_l_pt_njets1_correction, lt_wjets_l_pt_njets0_correction, lt_wjets_l_pt_njets1_correction,lt_wjets_extrap_correction)' % vars())
w.factory('expr::ff_lt_wjets_syst_l_pt_closure_down("@0*(((@1==0)*@2 + (@1>0)*@3)*((@4<%(crosstrg_pt)s)*((@1==0)*@5/@5 + (@1>0)*@6/@6) + (@4>=%(crosstrg_pt)s)*((@1==0)*@7/@7 + (@1>0)*@8/@8)))*@9",ff_lt_wjets_raw,njets[0], lt_wjets_met_njets0_correction, lt_wjets_met_njets1_correction, l_pt_bounded200, lt_wjets_low_l_pt_njets0_correction, lt_wjets_low_l_pt_njets1_correction, lt_wjets_l_pt_njets0_correction, lt_wjets_l_pt_njets1_correction,lt_wjets_extrap_correction)' % vars())

wjets_systs.append('wjets_syst_met_closure')
wjets_systs.append('wjets_syst_l_pt_closure')

# statistical uncertainties on measured wjets fake factors
# add statistical uncertainties 1 per njets/jetpt bin
for njet in [0,1]:
  for jetpt in jetpt_bins:
      short_name='%(jetpt)s_%(njet)ijet_wjets' % vars()
      for i in [1,2,3,4]:
        # possibly up to 4 uncerts per bin but in some cases uncert4 is a dummy
        w.factory('expr::ff_lt_wjets_stat_njet%(njet)i_%(jetpt)s_unc%(i)i_up("@0*@1",ff_lt_wjets, lt_%(short_name)s_uncert%(i)i_up)' % vars())
        w.factory('expr::ff_lt_wjets_stat_njet%(njet)i_%(jetpt)s_unc%(i)i_down("@0*@1",ff_lt_wjets, lt_%(short_name)s_uncert%(i)i_down)' % vars())
        wjets_systs.append('wjets_stat_njet%(njet)i_%(jetpt)s_unc%(i)i' % vars())

# systematic uncertainty from applying os/ss correction twice or not applying it 
w.factory('expr::ff_lt_wjets_syst_up("@0*@1*@2*@2", ff_lt_wjets_raw, lt_wjets_dr_correction, lt_wjets_extrap_correction)' % vars())
w.factory('expr::ff_lt_wjets_syst_down("@0*@1", ff_lt_wjets_raw, lt_wjets_dr_correction)' % vars())
wjets_systs.append('wjets_syst')

## statistical uncertainties on met, pt_1 and high->low mT extrap closure corrections
for i in [1,2]:
  w.factory('expr::ff_lt_wjets_stat_met_njets0_unc%(i)i_up("@0*@1", ff_lt_wjets, lt_wjets_met_correction_njets0_uncert%(i)i_up)' % vars())
  w.factory('expr::ff_lt_wjets_stat_met_njets0_unc%(i)i_down("@0*@1", ff_lt_wjets, lt_wjets_met_correction_njets0_uncert%(i)i_down)' % vars())
  w.factory('expr::ff_lt_wjets_stat_met_njets1_unc%(i)i_up("@0*@1", ff_lt_wjets, lt_wjets_met_correction_njets1_uncert%(i)i_up)' % vars())
  w.factory('expr::ff_lt_wjets_stat_met_njets1_unc%(i)i_down("@0*@1", ff_lt_wjets, lt_wjets_met_correction_njets1_uncert%(i)i_down)' % vars())

  wjets_systs.append('wjets_stat_met_njets0_unc%(i)i' % vars())
  wjets_systs.append('wjets_stat_met_njets1_unc%(i)i' % vars())

  w.factory('expr::ff_lt_wjets_stat_l_pt_njets0_unc%(i)i_up("@0*@1", ff_lt_wjets,   lt_wjets_l_pt_correction_njets0_uncert%(i)i_up)' % vars())
  w.factory('expr::ff_lt_wjets_stat_l_pt_njets0_unc%(i)i_down("@0*@1", ff_lt_wjets, lt_wjets_l_pt_correction_njets0_uncert%(i)i_down)' % vars())
  w.factory('expr::ff_lt_wjets_stat_l_pt_njets1_unc%(i)i_up("@0*@1", ff_lt_wjets,   lt_wjets_l_pt_correction_njets1_uncert%(i)i_up)' % vars())
  w.factory('expr::ff_lt_wjets_stat_l_pt_njets1_unc%(i)i_down("@0*@1", ff_lt_wjets, lt_wjets_l_pt_correction_njets1_uncert%(i)i_down)' % vars())

  wjets_systs.append('wjets_stat_l_pt_njets0_unc%(i)i' % vars())
  wjets_systs.append('wjets_stat_l_pt_njets1_unc%(i)i' % vars())

  w.factory('expr::ff_lt_wjets_stat_extrap_unc%(i)i_up("@0*@1", ff_lt_wjets,   lt_wjets_extrap_correction_uncert%(i)i_up)' % vars())
  w.factory('expr::ff_lt_wjets_stat_extrap_unc%(i)i_down("@0*@1", ff_lt_wjets, lt_wjets_extrap_correction_uncert%(i)i_down)' % vars())
  wjets_systs.append('wjets_stat_extrap_unc%(i)i' % vars())

  # adding extrapolation systematic uncertainties for W+jets
  # come from using Z->mumu + jet->tauh events to check high->low mT extrapolations in data (Ersatz method)
  # uncertainty comes from difference between mT regions added to statistical uncertainties (uncerts are mainly statistical though)
  # uncertainty is doubled for btag categories because we can't perform a dedicated measurment due to lack of stats

  w.factory('expr::wjets_extrap_uncert("(@1==0)*((@0<40)*0.1+(@0>=40&&@0<70)*0.04) + (@1>0)*((@0<40)*0.2+(@0>=40&&@0<70)*0.08)", mt[0], nbjets[0])' % vars())
  w.factory('expr::ff_lt_wjets_syst_extrap_up("@0*(1.+@1)", ff_lt_wjets, wjets_extrap_uncert)' % vars())
  w.factory('expr::ff_lt_wjets_syst_extrap_down("@0*(1.-@1)", ff_lt_wjets, wjets_extrap_uncert)' % vars())

  wjets_systs.append('wjets_syst_extrap' % vars())

print 'wjets systematics:'
print wjets_systs

# apply ttbar corrections

func_met_1 = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:met_closure_ttbar_mc_fit' % vars())
func_met_str_1=str(func_met_1.GetExpFormula('p')).replace('x','@0').replace(',false','')

func_l_pt_1 = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:l_pt_tightmT_closure_ttbar_mc_fit' % vars())
func_l_pt_str_1=str(func_l_pt_1.GetExpFormula('p')).replace('x','@0').replace(',false','')

func_l_pt_2 = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:l_pt_loosemT_closure_ttbar_mc_fit' % vars())
func_l_pt_str_2=str(func_l_pt_2.GetExpFormula('p')).replace('x','@0').replace(',false','')

lowpt_corr_1 = 1.
lowpt_corr_2 = 1.
if not (channel == 'et' and year == '2016'):
  hist_l_pt_1 = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:l_pt_tightmT_closure_ttbar_mc' % vars())
  hist_l_pt_2 = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:l_pt_loosemT_closure_ttbar_mc' % vars())
  lowpt_corr_1 = hist_l_pt_1.GetBinContent(1)
  lowpt_corr_2 = hist_l_pt_2.GetBinContent(1)

if lowpt_corr_1<=0: lowpt_corr_1 = 1.
if lowpt_corr_2<=0: lowpt_corr_2 = 1.

w.factory('expr::lt_ttbar_met_correction("max(%(func_met_str_1)s,0.)",met_var_w_bounded)' % vars())
# note the l_pt ones are actually just for uncertainties
w.factory('expr::lt_ttbar_l_pt_tightmt_correction("max((%(func_l_pt_str_1)s)*(@0>=%(crosstrg_pt)s) + %(lowpt_corr_1).5f*(@0<%(crosstrg_pt)s),0.)",l_pt_bounded200)' % vars())
w.factory('expr::lt_ttbar_l_pt_loosemt_correction("max((%(func_l_pt_str_2)s)*(@0>=%(crosstrg_pt)s) + %(lowpt_corr_2).5f*(@0<%(crosstrg_pt)s),0.)",l_pt_bounded200)' % vars())

# get stat uncertainties

hist_met_nom_1 = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:met_closure_ttbar_mc_uncert' % vars())

(uncert1_up, uncert1_down, uncert2_up, uncert_2_down) = wsptools.SplitUncert(hist_met_nom_1)

wsptools.SafeWrapHist(w, ['met_var_w_bounded'], hist_met_nom_1, name='lt_ttbar_met_correction_nom' % vars())
wsptools.SafeWrapHist(w, ['met_var_w_bounded'], uncert1_up,     name='lt_ttbar_met_correction_uncert1_hist_up' % vars())
wsptools.SafeWrapHist(w, ['met_var_w_bounded'], uncert2_up,     name='lt_ttbar_met_correction_uncert2_hist_up' % vars())

w.factory('expr::lt_ttbar_met_correction_uncert1_up("@0/@1", lt_ttbar_met_correction_uncert1_hist_up, lt_ttbar_met_correction_nom)' % vars())
w.factory('expr::lt_ttbar_met_correction_uncert2_up("@0/@1", lt_ttbar_met_correction_uncert2_hist_up, lt_ttbar_met_correction_nom)' % vars())

hist_l_pt_nom_1 = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:l_pt_tightmT_closure_ttbar_mc_uncert' % vars())
hist_l_pt_nom_2 = GetFromTFile(loc+'fakefactor_fits_%(channel)s_%(wp)s_%(year)s.root:l_pt_loosemT_closure_ttbar_mc_uncert' % vars())

(uncert1_up, uncert1_down, uncert2_up, uncert_2_down) = wsptools.SplitUncert(hist_l_pt_nom_1)

wsptools.SafeWrapHist(w, ['l_pt_bounded200'], hist_l_pt_nom_1, name='lt_ttbar_tightmt_l_pt_correction_nom' % vars())
wsptools.SafeWrapHist(w, ['l_pt_bounded200'], uncert1_up,     name='lt_ttbar_tightmt_l_pt_correction_uncert1_hist_up' % vars())
wsptools.SafeWrapHist(w, ['l_pt_bounded200'], uncert2_up,     name='lt_ttbar_tightmt_l_pt_correction_uncert2_hist_up' % vars())

(uncert1_up, uncert1_down, uncert2_up, uncert_2_down) = wsptools.SplitUncert(hist_l_pt_nom_2)

wsptools.SafeWrapHist(w, ['l_pt_bounded200'], hist_l_pt_nom_2, name='lt_ttbar_loosemt_l_pt_correction_nom' % vars())
wsptools.SafeWrapHist(w, ['l_pt_bounded200'], uncert1_up,     name='lt_ttbar_loosemt_l_pt_correction_uncert1_hist_up' % vars())
wsptools.SafeWrapHist(w, ['l_pt_bounded200'], uncert2_up,     name='lt_ttbar_loosemt_l_pt_correction_uncert2_hist_up' % vars())

w.factory('expr::lt_ttbar_l_pt_tightmt_correction_uncert1_up("@0/@1", lt_ttbar_tightmt_l_pt_correction_uncert1_hist_up, lt_ttbar_tightmt_l_pt_correction_nom)' % vars())
w.factory('expr::lt_ttbar_l_pt_tightmt_correction_uncert2_up("@0/@1", lt_ttbar_tightmt_l_pt_correction_uncert2_hist_up, lt_ttbar_tightmt_l_pt_correction_nom)' % vars())
w.factory('expr::lt_ttbar_l_pt_loosemt_correction_uncert1_up("@0/@1", lt_ttbar_loosemt_l_pt_correction_uncert1_hist_up, lt_ttbar_loosemt_l_pt_correction_nom)' % vars())
w.factory('expr::lt_ttbar_l_pt_loosemt_correction_uncert2_up("@0/@1", lt_ttbar_loosemt_l_pt_correction_uncert2_hist_up, lt_ttbar_loosemt_l_pt_correction_nom)' % vars())

w.factory('expr::lt_ttbar_l_pt_correction("(@0<40)*@1 + (@0>=40)*@2",mt[0], lt_ttbar_l_pt_tightmt_correction, lt_ttbar_l_pt_loosemt_correction )' % vars())

for i in [1,2]:
  w.factory('expr::lt_ttbar_met_correction_uncert%(i)i_down("2.-@0", lt_ttbar_met_correction_uncert%(i)i_up)' % vars())

  w.factory('expr::lt_ttbar_l_pt_correction_uncert%(i)i_up("(@0<40)*@1 + (@0>=40)*@2", mt[0], lt_ttbar_l_pt_tightmt_correction_uncert%(i)i_up, lt_ttbar_l_pt_loosemt_correction_uncert%(i)i_up)' % vars())
  w.factory('expr::lt_ttbar_l_pt_correction_uncert%(i)i_down("2.-@0", lt_ttbar_l_pt_correction_uncert%(i)i_up)' % vars())

w.factory('expr::lt_ttbar_datamc_correction("(@0>=0)*((@1<1.25*@2)*@3/@4 + (@1>=1.25*@2&&@1<1.5*@2)*@5/@6 + (@1>=1.5*@2)*@7/@8)", njets[0], jetpt[40], pt_bounded, lt_jet_pt_low_1jet_pt_2_ff_wjets_pt140_fit, lt_jet_pt_low_1jet_pt_2_ff_wjets_mc_pt140_fit, lt_jet_pt_med_1jet_pt_2_ff_wjets_pt140_fit, lt_jet_pt_med_1jet_pt_2_ff_wjets_mc_pt140_fit, lt_jet_pt_high_1jet_pt_2_ff_wjets_pt140_fit, lt_jet_pt_high_1jet_pt_2_ff_wjets_mc_pt140_fit)' % vars())

# get final ttbar fake factor
w.factory('expr::ff_lt_ttbar("@0*@1*@2*@3", ff_lt_ttbar_raw, lt_ttbar_met_correction, lt_ttbar_datamc_correction, lt_ttbar_l_pt_correction, os[1], l_pt_bounded200, nbjets[0], met_var_qcd_bounded, iso_bounded)' % vars())

ttbar_systs=[]

# systematic for ttbar dr closure correction applied twice or not all
w.factory('expr::ff_lt_ttbar_syst_met_closure_up("@0*@1*@1*@2*@3", ff_lt_ttbar_raw, lt_ttbar_met_correction, lt_ttbar_datamc_correction, lt_ttbar_l_pt_correction, os[1], l_pt_bounded200, nbjets[0], met_var_qcd_bounded, iso_bounded)' % vars())
w.factory('expr::ff_lt_ttbar_syst_met_closure_down("@0*@1*@2", ff_lt_ttbar_raw, lt_ttbar_datamc_correction, lt_ttbar_l_pt_correction, os[1], l_pt_bounded200, nbjets[0], met_var_qcd_bounded, iso_bounded)' % vars())

w.factory('expr::ff_lt_ttbar_syst_l_pt_closure_up("@0*@1*@2*@3*@3", ff_lt_ttbar_raw, lt_ttbar_met_correction, lt_ttbar_datamc_correction, lt_ttbar_l_pt_correction, os[1], l_pt_bounded200, nbjets[0], met_var_qcd_bounded, iso_bounded)' % vars())
w.factory('expr::ff_lt_ttbar_syst_l_pt_closure_down("@0*@1*@2", ff_lt_ttbar_raw, lt_ttbar_met_correction, lt_ttbar_datamc_correction, os[1], l_pt_bounded200, nbjets[0], met_var_qcd_bounded, iso_bounded)' % vars())


ttbar_systs.append('ttbar_syst_met_closure')
ttbar_systs.append('ttbar_syst_l_pt_closure')

# statistical uncertainties on measured ttbar and wjets fake factors
# add statistical uncertainties 1 per jetpt bin
for jetpt in jetpt_bins:
  short_name_wjets='%(jetpt)s_1jet_wjets' % vars()
  short_name='%(jetpt)s_ttbar' % vars()
  for i in [1,2,3]:
    if i < 3:
      # the ttbar fake factors depend on the W+jets 1jet FFs due to data to MC correction so need to correlate the statistical uncertainties for these
      w.factory('expr::ff_lt_ttbar_wjets_stat_njet1_%(jetpt)s_unc%(i)i_up("@0*@1",ff_lt_ttbar, lt_%(short_name_wjets)s_pt140_uncert%(i)i_up)' % vars())
      w.factory('expr::ff_lt_ttbar_wjets_stat_njet1_%(jetpt)s_unc%(i)i_down("@0*@1",ff_lt_ttbar, lt_%(short_name_wjets)s_pt140_uncert%(i)i_down)' % vars())
      ttbar_systs.append('ttbar_wjets_stat_njet1_%(jetpt)s_unc%(i)i' % vars())

    w.factory('expr::ff_lt_ttbar_stat_%(jetpt)s_unc%(i)i_up("@0*@1",ff_lt_ttbar, lt_%(short_name)s_uncert%(i)i_up)' % vars())
    w.factory('expr::ff_lt_ttbar_stat_%(jetpt)s_unc%(i)i_down("@0*@1",ff_lt_ttbar, lt_%(short_name)s_uncert%(i)i_down)' % vars())
    ttbar_systs.append('ttbar_stat_%(jetpt)s_unc%(i)i' % vars())

# systematic uncertainty from applying mc->data correction twice or not applying it 
w.factory('expr::ff_lt_ttbar_syst_up("@0*@1*@2*@2*@3", ff_lt_ttbar_raw, lt_ttbar_met_correction, lt_ttbar_datamc_correction, lt_ttbar_l_pt_correction)' % vars())
w.factory('expr::ff_lt_ttbar_syst_down("@0*@1*@2", ff_lt_ttbar_raw, lt_ttbar_met_correction, lt_ttbar_l_pt_correction)' % vars())
ttbar_systs.append('ttbar_syst')

# systematic uncertainty from lepton pT non-closure
#w.factory('expr::ff_lt_ttbar_l_pt_syst_up("@0*@1*@2*@3", ff_lt_ttbar_raw, lt_ttbar_met_correction, lt_ttbar_datamc_correction, lt_ttbar_l_pt_correction)' % vars())
#w.factory('expr::ff_lt_ttbar_l_pt_syst_down("@0*@1*@2/@3", ff_lt_ttbar_raw, lt_ttbar_met_correction, lt_ttbar_datamc_correction, lt_ttbar_l_pt_correction)' % vars())
#ttbar_systs.append('ttbar_l_pt_syst')

# statistical uncertainties on met correction
for i in [1,2]:
  w.factory('expr::ff_lt_ttbar_stat_met_unc%(i)i_up("@0*@1", ff_lt_ttbar, lt_ttbar_met_correction_uncert%(i)i_up)' % vars())
  w.factory('expr::ff_lt_ttbar_stat_met_unc%(i)i_down("@0*@1", ff_lt_ttbar, lt_ttbar_met_correction_uncert%(i)i_down)' % vars())
  w.factory('expr::ff_lt_ttbar_stat_l_pt_unc%(i)i_up("@0*@1", ff_lt_ttbar, lt_ttbar_l_pt_correction_uncert%(i)i_up)' % vars())
  w.factory('expr::ff_lt_ttbar_stat_l_pt_unc%(i)i_down("@0*@1", ff_lt_ttbar, lt_ttbar_l_pt_correction_uncert%(i)i_down)' % vars())

  ttbar_systs.append('ttbar_stat_met_unc%(i)i' % vars())
  ttbar_systs.append('ttbar_stat_l_pt_unc%(i)i' % vars())

print 'ttbar systematics:'
print ttbar_systs

# now put all fake factors together using fractions
w.factory('expr::ff_total("(@0*@1 + @2*@3 + @4*@5)/(@1+@3+@5)", ff_lt_qcd, lt_fracs_qcd, ff_lt_wjets, lt_fracs_wjets, ff_lt_ttbar, lt_fracs_ttbar)' % vars())
# define uncertainties on fractions by varying them by 10%
w.factory('expr::ff_total_wjets_frac_up("(@0*@1 + @2*@3*1.1 + @4*@5)/(@1 + @3*1.1 + @5)", ff_lt_qcd, lt_fracs_qcd, ff_lt_wjets, lt_fracs_wjets, ff_lt_ttbar, lt_fracs_ttbar)' % vars())
w.factory('expr::ff_total_wjets_frac_down("(@0*@1 + @2*@3*0.9 + @4*@5)/(@1 + @3*0.9 + @5)", ff_lt_qcd, lt_fracs_qcd, ff_lt_wjets, lt_fracs_wjets, ff_lt_ttbar, lt_fracs_ttbar)' % vars())
w.factory('expr::ff_total_ttbar_frac_up("(@0*@1 + @2*@3 + @4*@5*1.1)/(@1 + @3 + @5*1.1)", ff_lt_qcd, lt_fracs_qcd, ff_lt_wjets, lt_fracs_wjets, ff_lt_ttbar, lt_fracs_ttbar)' % vars())
w.factory('expr::ff_total_ttbar_frac_down("(@0*@1 + @2*@3 + @4*@5*0.9)/(@1 + @3 + @5*0.9)", ff_lt_qcd, lt_fracs_qcd, ff_lt_wjets, lt_fracs_wjets, ff_lt_ttbar, lt_fracs_ttbar)' % vars())


w.factory('expr::ff_total_syst_alt_func_up("(@0*@1 + @2*@3 + @4*@5)/(@1+@3+@5)*@6", ff_lt_qcd_alt_func_up, lt_fracs_qcd, ff_lt_wjets_alt_func_up, lt_fracs_wjets, ff_lt_ttbar_alt_func_up, lt_fracs_ttbar, ff_total)' % vars())
w.factory('expr::ff_total_syst_alt_func_down("(@0*@1 + @2*@3 + @4*@5)/(@1+@3+@5)*@6", ff_lt_qcd_alt_func_down, lt_fracs_qcd, ff_lt_wjets_alt_func_down, lt_fracs_wjets, ff_lt_ttbar_alt_func_down, lt_fracs_ttbar, ff_total)' % vars())

# uncertainty for cross triggered part
w.factory('expr::ff_total_low_pt_up("(@0*@1 + @2*@3 + @4*@5)/(@1+@3+@5)*(1.2*(@6<%(crosstrg_pt)s)+(@6>=%(crosstrg_pt)s))", ff_lt_qcd, lt_fracs_qcd, ff_lt_wjets, lt_fracs_wjets, ff_lt_ttbar, lt_fracs_ttbar, l_pt_bounded200)' % vars())
w.factory('expr::ff_total_low_pt_down("(@0*@1 + @2*@3 + @4*@5)/(@1+@3+@5)*(0.8*(@6<%(crosstrg_pt)s)+(@6>=%(crosstrg_pt)s))", ff_lt_qcd, lt_fracs_qcd, ff_lt_wjets, lt_fracs_wjets, ff_lt_ttbar, lt_fracs_ttbar, l_pt_bounded200)' % vars())

# add qcd systematics
for s in qcd_systs:   
  w.factory('expr::ff_total_%(s)s_up("(@0*@1 + @2*@3 + @4*@5)/(@1+@3+@5)", ff_lt_%(s)s_up, lt_fracs_qcd, ff_lt_wjets, lt_fracs_wjets, ff_lt_ttbar, lt_fracs_ttbar)' % vars())
  w.factory('expr::ff_total_%(s)s_down("(@0*@1 + @2*@3 + @4*@5)/(@1+@3+@5)", ff_lt_%(s)s_down, lt_fracs_qcd, ff_lt_wjets, lt_fracs_wjets, ff_lt_ttbar, lt_fracs_ttbar)' % vars())

# add ttbar systs except those from w+jets FF statistics
for s in ttbar_systs:
  if 'wjets' in s: continue
  w.factory('expr::ff_total_%(s)s_up("(@0*@1 + @2*@3 + @4*@5)/(@1+@3+@5)", ff_lt_qcd, lt_fracs_qcd, ff_lt_wjets, lt_fracs_wjets, ff_lt_%(s)s_up, lt_fracs_ttbar)' % vars())
  w.factory('expr::ff_total_%(s)s_down("(@0*@1 + @2*@3 + @4*@5)/(@1+@3+@5)", ff_lt_qcd, lt_fracs_qcd, ff_lt_wjets, lt_fracs_wjets, ff_lt_%(s)s_down, lt_fracs_ttbar)' % vars())

# add wjets systs including correlated part with ttbar corrections
for s in wjets_systs:
  if 'wjets_stat_njet1_jet_pt_' in s and ('unc1' in s or 'unc2' in s):
    w.factory('expr::ff_total_%(s)s_up("(@0*@1 + @2*@3 + @4*@5)/(@1+@3+@5)", ff_lt_qcd, lt_fracs_qcd, ff_lt_%(s)s_up, lt_fracs_wjets, ff_lt_ttbar_%(s)s_up, lt_fracs_ttbar)' % vars())
    w.factory('expr::ff_total_%(s)s_down("(@0*@1 + @2*@3 + @4*@5)/(@1+@3+@5)", ff_lt_qcd, lt_fracs_qcd, ff_lt_%(s)s_down, lt_fracs_wjets, ff_lt_ttbar_%(s)s_down, lt_fracs_ttbar)' % vars())
  else:
    w.factory('expr::ff_total_%(s)s_up("(@0*@1 + @2*@3 + @4*@5)/(@1+@3+@5)", ff_lt_qcd, lt_fracs_qcd, ff_lt_%(s)s_up, lt_fracs_wjets, ff_lt_ttbar, lt_fracs_ttbar)' % vars())
    w.factory('expr::ff_total_%(s)s_down("(@0*@1 + @2*@3 + @4*@5)/(@1+@3+@5)", ff_lt_qcd, lt_fracs_qcd, ff_lt_%(s)s_down, lt_fracs_wjets, ff_lt_ttbar, lt_fracs_ttbar)' % vars())

# add subtracted MC shifts
w.factory('expr::ff_total_wjets_syst_bkg_up("(@0*@1 + @2*@3 + @4*@5)/(@1+@3+@5)", ff_lt_qcd, lt_fracs_qcd, ff_lt_wjetsUp, lt_fracs_wjets, ff_lt_ttbar, lt_fracs_ttbar)' % vars())
w.factory('expr::ff_total_wjets_syst_bkg_down("(@0*@1 + @2*@3 + @4*@5)/(@1+@3+@5)", ff_lt_qcd, lt_fracs_qcd, ff_lt_wjetsDown, lt_fracs_wjets, ff_lt_ttbar, lt_fracs_ttbar)' % vars())
w.factory('expr::ff_total_qcd_syst_bkg_up("(@0*@1 + @2*@3 + @4*@5)/(@1+@3+@5)", ff_lt_qcdUp, lt_fracs_qcd, ff_lt_wjets, lt_fracs_wjets, ff_lt_ttbar, lt_fracs_ttbar)' % vars())
w.factory('expr::ff_total_qcd_syst_bkg_down("(@0*@1 + @2*@3 + @4*@5)/(@1+@3+@5)", ff_lt_qcdDown, lt_fracs_qcd, ff_lt_wjets, lt_fracs_wjets, ff_lt_ttbar, lt_fracs_ttbar)' % vars())

w.Print()
w.writeToFile('%(output)s/fakefactors_ws_%(channel)s_mssm_%(year)s_v5.root' % vars())
w.Delete() 

