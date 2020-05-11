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

wp = 'medium'

###############################################
## et channel ####
###############################################

w = ROOT.RooWorkspace('w')

# get fractions

loc = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/mvadm_ff_deeptauV2p1_2017_et/' % vars()

histsToWrap = [(loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets0_os_qcd' % vars(), 'et_%(wp)s_fracs_njets0_os_qcd' % vars()),
               (loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets1_os_qcd' % vars(), 'et_%(wp)s_fracs_njets1_os_qcd' % vars()),
               (loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets2_os_qcd' % vars(), 'et_%(wp)s_fracs_njets2_os_qcd' % vars()),
               (loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets0_ss_qcd' % vars(), 'et_%(wp)s_fracs_njets0_ss_qcd' % vars()),
               (loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets1_ss_qcd' % vars(), 'et_%(wp)s_fracs_njets1_ss_qcd' % vars()),
               (loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets2_ss_qcd' % vars(), 'et_%(wp)s_fracs_njets2_ss_qcd' % vars()),
               (loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets0_os_wjets' % vars(), 'et_%(wp)s_fracs_njets0_os_wjets' % vars()),
               (loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets1_os_wjets' % vars(), 'et_%(wp)s_fracs_njets1_os_wjets' % vars()),
               (loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets2_os_wjets' % vars(), 'et_%(wp)s_fracs_njets2_os_wjets' % vars()),
               (loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets0_ss_wjets' % vars(), 'et_%(wp)s_fracs_njets0_ss_wjets' % vars()),
               (loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets1_ss_wjets' % vars(), 'et_%(wp)s_fracs_njets1_ss_wjets' % vars()),
               (loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets2_ss_wjets' % vars(), 'et_%(wp)s_fracs_njets2_ss_wjets' % vars()),
               (loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets0_os_ttbar' % vars(), 'et_%(wp)s_fracs_njets0_os_ttbar' % vars()),
               (loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets1_os_ttbar' % vars(), 'et_%(wp)s_fracs_njets1_os_ttbar' % vars()),
               (loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets2_os_ttbar' % vars(), 'et_%(wp)s_fracs_njets2_os_ttbar' % vars()),
               (loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets0_ss_ttbar' % vars(), 'et_%(wp)s_fracs_njets0_ss_ttbar' % vars()),
               (loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets1_ss_ttbar' % vars(), 'et_%(wp)s_fracs_njets1_ss_ttbar' % vars()),
               (loc + 'fakefactor_fits_et_%(wp)s_2017.root:et_fracs_njets2_ss_ttbar' % vars(), 'et_%(wp)s_fracs_njets2_ss_ttbar' % vars())
              ]

for task in histsToWrap:
   wsptools.SafeWrapHist(
     w, ['expr::et_max120("min(119.9,@0)",pfmt[0])'],
     GetFromTFile(task[0]),
     name=task[1])

for i in ['qcd','wjets','ttbar']:
  w.factory('expr::et_%(wp)s_fracs_%(i)s("(@0!=0)*((@1==0)*(@2)+(@1==1)*@3+(@1>1)*(@4))+(@0==0)*((@1==0)*(@5)+(@1==1)*@6+(@1>1)*(@7))", os[1],njets[0],et_%(wp)s_fracs_njets0_os_%(i)s,et_%(wp)s_fracs_njets1_os_%(i)s,et_%(wp)s_fracs_njets2_os_%(i)s,et_%(wp)s_fracs_njets0_ss_%(i)s,et_%(wp)s_fracs_njets1_ss_%(i)s,et_%(wp)s_fracs_njets2_ss_%(i)s)' % vars())


xtrg_pt=28 # change this pT cut depending on what pT range uses cross trigger

# load all TF1's needed to define fake factors
w.factory('expr::pt_bounded100("max(min(99.9,@0),30)",pt[0])' % vars())
w.factory('expr::mvis_bounded250("min(249.9,@0)",mvis[0])' % vars())
w.factory('expr::l_pt_bounded120("min(119.9,@0)",e_pt[%(xtrg_pt)s])' % vars()) # change me for et channel!!

f = ROOT.TFile('us_fakefactors/ff_files_et_2017/uncorrected_fakefactors_et.root')
func = f.Get('rawFF_et_qcd_0jetSSloose')

loc = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/us_fakefactors/ff_files_et_2017/' % vars()

# raw fake factors

fraw = ROOT.TFile(loc+'uncorrected_fakefactors_et.root')
for uncert in ['','_unc1_up','_unc1_down','_unc2_up','_unc2_down']:
  for proc in ['qcd','w','tt']:
    if proc == 'tt':
      name = 'mc_rawFF_et_%(proc)s%(uncert)s' % vars() 
      func = fraw.Get(name) 
      wsptools.SafeWrapFunc(w, ['pt_bounded100'], func, name=None)
    else:
      for njet in ['0','1','2']:
        name = 'rawFF_et_%(proc)s_%(njet)sjet%(uncert)s' % vars()
        func = fraw.Get(name) 
        wsptools.SafeWrapFunc(w, ['pt_bounded100'], func, name=None)

# get closure corrections

fclosure = ROOT.TFile(loc+'FF_corrections_1.root')

# met closure correction for w+jets 0 jet events
name = 'closure_met_et_0jet_w'
func = fclosure.Get(name)
wsptools.SafeWrapFunc(w, ['met'], func, name=None)

w.factory('expr::closure_met_et_w("(@0==0)*@1 + (@0>0)",njets[0], closure_met_et_0jet_w)' % vars())

# get l pt closures for single and cross trigger

for proc in ['qcd','w','ttmc']:
  for x in ['30to40', '40to50', 'gt50', '30to40_xtrg', '40to50_xtrg', 'gt50_xtrg']:

    name = 'closure_lpt_taupt%(x)s_et_%(proc)s' % vars()
    func = fclosure.Get(name)
    wsptools.SafeWrapFunc(w, ['l_pt_bounded120'], func, name=None)

# mT correction and uncertainties

fcorr = ROOT.TFile(loc+'FF_QCDcorrectionOSSS.root')
for uncert in ['','_unc1_up','_unc1_down','_unc2_up','_unc2_down']:
  name = 'closure_mt_et_w%(uncert)s' % vars() 
  func = fcorr.Get(name)
  wsptools.SafeWrapFunc(w, ['mt'], func, name='closure_mt_w%(uncert)s' % vars())

# QCD correction is now flat 10% with +/- 10% uncertainty

#########

# make njets/pt binned functions
## these function can also take care of apply/not-applying corrections for OS/SS events and set the bounds on the mt correction
#
for proc in ['qcd','w']:
  for uncert in ['','_unc1_up','_unc1_down','_unc2_up','_unc2_down']:

    w.factory('expr::rawFF_et_%(proc)s%(uncert)s("(@0==0)*@1+(@0==1)*@2+(@0>1)*@3", njets[0], rawFF_et_%(proc)s_0jet%(uncert)s, rawFF_et_%(proc)s_1jet%(uncert)s, rawFF_et_%(proc)s_2jet%(uncert)s)' % vars())

for proc in ['qcd','w','ttmc']:
  w.factory('expr::closure_l_pt_et_%(proc)s("(@0<%(xtrg_pt).1f)*((@1<40)*@2 + (@1>=40 && @1<50)*@3 + (@1>=50)*@4) + (@0>=%(xtrg_pt).1f)*((@1<40)*@5 + (@1>=40 && @1<50)*@6 + (@1>=50)*@7)", l_pt_bounded120, pt, closure_lpt_taupt30to40_xtrg_et_%(proc)s, closure_lpt_taupt40to50_xtrg_et_%(proc)s, closure_lpt_tauptgt50_xtrg_et_%(proc)s, closure_lpt_taupt30to40_et_%(proc)s, closure_lpt_taupt40to50_et_%(proc)s, closure_lpt_tauptgt50_et_%(proc)s )' % vars())

# apply corrections to raw fake factors

# nominal ffs + stat uncertainties
for uncert in ['','_unc1_up','_unc1_down','_unc2_up','_unc2_down']:
  w.factory('expr::ff_et_%(wp)s_qcd%(uncert)s("@0*@1*1.1",rawFF_et_qcd%(uncert)s, closure_l_pt_et_qcd)' % vars())
  w.factory('expr::ff_et_%(wp)s_wjets%(uncert)s("@0*@1*@2*@3",rawFF_et_w%(uncert)s, closure_l_pt_et_w, closure_mt_w, closure_met_et_w)' % vars())
  w.factory('expr::ff_et_%(wp)s_ttbar%(uncert)s("@0*@1",mc_rawFF_et_tt%(uncert)s, closure_l_pt_et_ttmc)' % vars())
  if uncert != '': w.factory('expr::ff_et_%(wp)s_wjets_closure_mt%(uncert)s("@0*@1*@2*@3",rawFF_et_w, closure_l_pt_et_w, closure_mt_w%(uncert)s, closure_met_et_w)' % vars())
# systematic uncertainties
#need to split this into low and high pT!
w.factory('expr::ff_et_%(wp)s_qcd_l_pt_high_down("@0*1.1",rawFF_et_qcd)' % vars())
w.factory('expr::ff_et_%(wp)s_qcd_l_pt_high_up("@0*(1+2*(@1-1))*1.1",rawFF_et_qcd, closure_l_pt_et_qcd)' % vars())
w.factory('expr::ff_et_%(wp)s_wjets_l_pt_high_down("@0*@1*@2",rawFF_et_w, closure_mt_w, closure_met_et_w)' % vars())
w.factory('expr::ff_et_%(wp)s_wjets_l_pt_high_up("@0*(1+2*(@1-1))*@2*@3",rawFF_et_w, closure_l_pt_et_w, closure_mt_w, closure_met_et_w)' % vars())
w.factory('expr::ff_et_%(wp)s_ttbar_l_pt_high_down("@0",mc_rawFF_et_tt)' % vars())
w.factory('expr::ff_et_%(wp)s_ttbar_l_pt_high_up("@0*(1+2*(@1-1))",mc_rawFF_et_tt, closure_l_pt_et_ttmc)' % vars())

for proc in ['qcd', 'wjets', 'ttbar']:
  w.factory('expr::ff_et_%(wp)s_%(proc)s_l_pt_low_up("((@0<%(xtrg_pt)s)*1.1 + (@0>=%(xtrg_pt)s))*@1", l_pt_bounded120, ff_et_%(wp)s_%(proc)s )' % vars())
  w.factory('expr::ff_et_%(wp)s_%(proc)s_l_pt_low_down("((@0<%(xtrg_pt)s)*0.9 + (@0>=%(xtrg_pt)s))*@1", l_pt_bounded120, ff_et_%(wp)s_%(proc)s )' % vars())

w.factory('expr::ff_et_%(wp)s_qcd_osss_down("@0*@1*1.1*0.9",rawFF_et_qcd, closure_l_pt_et_qcd)' % vars())
w.factory('expr::ff_et_%(wp)s_qcd_osss_up("@0*@1*1.1*1.1",rawFF_et_qcd, closure_l_pt_et_qcd)' % vars())

w.factory('expr::ff_et_%(wp)s_wjets_et_unc1_up("@0*@1*@2*@3",rawFF_et_w, closure_l_pt_et_w, closure_mt_w_unc1_up, closure_met_et_w)' % vars())
w.factory('expr::ff_et_%(wp)s_wjets_et_unc1_down("@0*@1*@2*@3",rawFF_et_w, closure_l_pt_et_w, closure_mt_w_unc1_down, closure_met_et_w)' % vars())
w.factory('expr::ff_et_%(wp)s_wjets_et_unc2_up("@0*@1*@2*@3",rawFF_et_w, closure_l_pt_et_w, closure_mt_w_unc2_up, closure_met_et_w)' % vars())
w.factory('expr::ff_et_%(wp)s_wjets_et_unc2_down("@0*@1*@2*@3",rawFF_et_w, closure_l_pt_et_w, closure_mt_w_unc2_down, closure_met_et_w)' % vars())

# add stat uncertainties in njets bins 
for proc in ['qcd', 'wjets']:
  for uncert in ['unc1','unc2']:
    w.factory('expr::ff_et_%(wp)s_%(proc)s_%(uncert)s_njets0_up("(@0==0)*@1 + (@0!=0)*@2",njets[0], ff_et_%(wp)s_%(proc)s_%(uncert)s_up, ff_et_%(wp)s_%(proc)s)' % vars())
    w.factory('expr::ff_et_%(wp)s_%(proc)s_%(uncert)s_njets0_down("(@0==0)*@1 + (@0!=0)*@2",njets[0], ff_et_%(wp)s_%(proc)s_%(uncert)s_down, ff_et_%(wp)s_%(proc)s)' % vars())

    w.factory('expr::ff_et_%(wp)s_%(proc)s_%(uncert)s_njets1_up("(@0==1)*@1 + (@0!=1)*@2",njets[0], ff_et_%(wp)s_%(proc)s_%(uncert)s_up, ff_et_%(wp)s_%(proc)s)' % vars())
    w.factory('expr::ff_et_%(wp)s_%(proc)s_%(uncert)s_njets1_down("(@0==1)*@1 + (@0!=1)*@2",njets[0], ff_et_%(wp)s_%(proc)s_%(uncert)s_down, ff_et_%(wp)s_%(proc)s)' % vars())

    w.factory('expr::ff_et_%(wp)s_%(proc)s_%(uncert)s_njets2_up("(@0>=2)*@1 + (@0<2)*@2",njets[0], ff_et_%(wp)s_%(proc)s_%(uncert)s_up, ff_et_%(wp)s_%(proc)s)' % vars())
    w.factory('expr::ff_et_%(wp)s_%(proc)s_%(uncert)s_njets2_down("(@0>=2)*@1 + (@0<2)*@2",njets[0], ff_et_%(wp)s_%(proc)s_%(uncert)s_down, ff_et_%(wp)s_%(proc)s)' % vars())

# scale FF uncerts by fractions
w.factory('expr::ff_et_%(wp)s_us("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_wjets, ff_et_%(wp)s_qcd, ff_et_%(wp)s_ttbar)' % vars())

for i in ['up','down']:
  for u in ['unc1','unc2']:
    w.factory('expr::ff_et_%(wp)s_us_ttbar_stat_%(u)s_%(i)s("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_wjets, ff_et_%(wp)s_qcd, ff_et_%(wp)s_ttbar_%(u)s_%(i)s)' % vars())
    for njet in [0,1,2]:
      w.factory('expr::ff_et_%(wp)s_us_qcd_stat_njets%(njet)i_%(u)s_%(i)s("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_wjets, ff_et_%(wp)s_qcd_%(u)s_njets%(njet)i_%(i)s, ff_et_%(wp)s_ttbar)' % vars())
      w.factory('expr::ff_et_%(wp)s_us_wjets_stat_njets%(njet)i_%(u)s_%(i)s("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_wjets_%(u)s_njets%(njet)i_%(i)s, ff_et_%(wp)s_qcd, ff_et_%(wp)s_ttbar)' % vars())

    w.factory('expr::ff_et_%(wp)s_us_wjets_syst_mt_%(u)s_%(i)s("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_wjets_et_%(u)s_%(i)s, ff_et_%(wp)s_qcd, ff_et_%(wp)s_ttbar)' % vars())

    w.factory('expr::ff_et_%(wp)s_us_qcd_syst_osss_%(i)s("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_wjets, ff_et_%(wp)s_qcd_osss_%(i)s, ff_et_%(wp)s_ttbar)' % vars())

  w.factory('expr::ff_et_%(wp)s_us_wjets_syst_closure_low_%(i)s("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_wjets_l_pt_low_%(i)s, ff_et_%(wp)s_qcd, ff_et_%(wp)s_ttbar)' % vars())
  w.factory('expr::ff_et_%(wp)s_us_qcd_syst_closure_low_%(i)s("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_wjets, ff_et_%(wp)s_qcd_l_pt_low_%(i)s, ff_et_%(wp)s_ttbar)' % vars())
  w.factory('expr::ff_et_%(wp)s_us_ttbar_syst_closure_low_%(i)s("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_wjets, ff_et_%(wp)s_qcd, ff_et_%(wp)s_ttbar_l_pt_low_%(i)s)' % vars())

  w.factory('expr::ff_et_%(wp)s_us_wjets_syst_closure_high_%(i)s("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_wjets_l_pt_high_%(i)s, ff_et_%(wp)s_qcd, ff_et_%(wp)s_ttbar)' % vars())
  w.factory('expr::ff_et_%(wp)s_us_qcd_syst_closure_high_%(i)s("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_wjets, ff_et_%(wp)s_qcd_l_pt_high_%(i)s, ff_et_%(wp)s_ttbar)' % vars())
  w.factory('expr::ff_et_%(wp)s_us_ttbar_syst_closure_high_%(i)s("@0*@3 + @1*@4 + @2*@5", et_%(wp)s_fracs_wjets, et_%(wp)s_fracs_qcd, et_%(wp)s_fracs_ttbar, ff_et_%(wp)s_wjets, ff_et_%(wp)s_qcd, ff_et_%(wp)s_ttbar_l_pt_high_%(i)s)' % vars())

w.Print()
w.writeToFile('fakefactors_us_ws_et_lite_2017.root')
w.Delete()


###############################################
## mt channel ####
###############################################

w = ROOT.RooWorkspace('w')

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
     w, ['expr::mt_max120("min(119.9,@0)",pfmt[0])'],
     GetFromTFile(task[0]),
     name=task[1])

for i in ['qcd','wjets','ttbar']:
  w.factory('expr::mt_%(wp)s_fracs_%(i)s("(@0!=0)*((@1==0)*(@2)+(@1==1)*@3+(@1>1)*(@4))+(@0==0)*((@1==0)*(@5)+(@1==1)*@6+(@1>1)*(@7))", os[1],njets[0],mt_%(wp)s_fracs_njets0_os_%(i)s,mt_%(wp)s_fracs_njets1_os_%(i)s,mt_%(wp)s_fracs_njets2_os_%(i)s,mt_%(wp)s_fracs_njets0_ss_%(i)s,mt_%(wp)s_fracs_njets1_ss_%(i)s,mt_%(wp)s_fracs_njets2_ss_%(i)s)' % vars())


xtrg_pt=25 # change this pT cut depending on what pT range uses cross trigger

# load all TF1's needed to define fake factors
w.factory('expr::pt_bounded100("max(min(99.9,@0),30)",pt[0])' % vars())
w.factory('expr::mvis_bounded250("min(249.9,@0)",mvis[0])' % vars())
w.factory('expr::l_pt_bounded120("min(119.9,@0)",m_pt[%(xtrg_pt)s])' % vars()) # change me for et channel!!

f = ROOT.TFile('us_fakefactors/ff_files_mt_2017/uncorrected_fakefactors_mt.root')
func = f.Get('rawFF_mt_qcd_0jetSSloose')

loc = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/us_fakefactors/ff_files_mt_2017/' % vars()

# raw fake factors

fraw = ROOT.TFile(loc+'uncorrected_fakefactors_mt.root')
for uncert in ['','_unc1_up','_unc1_down','_unc2_up','_unc2_down']:
  for proc in ['qcd','w','tt']:
    if proc == 'tt':
      name = 'mc_rawFF_mt_%(proc)s%(uncert)s' % vars() 
      func = fraw.Get(name) 
      wsptools.SafeWrapFunc(w, ['pt_bounded100'], func, name=None)
    else:
      for njet in ['0','1','2']:
        name = 'rawFF_mt_%(proc)s_%(njet)sjet%(uncert)s' % vars()
        func = fraw.Get(name) 
        wsptools.SafeWrapFunc(w, ['pt_bounded100'], func, name=None)

# get closure corrections

fclosure = ROOT.TFile(loc+'FF_corrections_1.root')

# met closure correction for w+jets 0 jet events
name = 'closure_met_mt_0jet_w'
func = fclosure.Get(name)
wsptools.SafeWrapFunc(w, ['met'], func, name=None)

w.factory('expr::closure_met_mt_w("(@0==0)*@1 + (@0>0)",njets[0], closure_met_mt_0jet_w)' % vars())

# get l pt closures for single and cross trigger

for proc in ['qcd','w','ttmc']:
  for x in ['30to40', '40to50', 'gt50', '30to40_xtrg', '40to50_xtrg', 'gt50_xtrg']:

    name = 'closure_lpt_taupt%(x)s_mt_%(proc)s' % vars()
    func = fclosure.Get(name)
    wsptools.SafeWrapFunc(w, ['l_pt_bounded120'], func, name=None)

# mT correction and uncertainties

fcorr = ROOT.TFile(loc+'FF_QCDcorrectionOSSS.root')
for uncert in ['','_unc1_up','_unc1_down','_unc2_up','_unc2_down']:
  name = 'closure_mt_mt_w%(uncert)s' % vars() # second mt is the channel
  func = fcorr.Get(name)
  wsptools.SafeWrapFunc(w, ['mt'], func, name='closure_mt_w%(uncert)s' % vars())

# QCD correction is now flat 10% with +/- 10% uncertainty

#########

# make njets/pt binned functions
## these function can also take care of apply/not-applying corrections for OS/SS events and set the bounds on the mt correction
#
for proc in ['qcd','w']:
  for uncert in ['','_unc1_up','_unc1_down','_unc2_up','_unc2_down']:

    w.factory('expr::rawFF_mt_%(proc)s%(uncert)s("(@0==0)*@1+(@0==1)*@2+(@0>1)*@3", njets[0], rawFF_mt_%(proc)s_0jet%(uncert)s, rawFF_mt_%(proc)s_1jet%(uncert)s, rawFF_mt_%(proc)s_2jet%(uncert)s)' % vars())

for proc in ['qcd','w','ttmc']:
  w.factory('expr::closure_l_pt_mt_%(proc)s("(@0<%(xtrg_pt).1f)*((@1<40)*@2 + (@1>=40 && @1<50)*@3 + (@1>=50)*@4) + (@0>=%(xtrg_pt).1f)*((@1<40)*@5 + (@1>=40 && @1<50)*@6 + (@1>=50)*@7)", l_pt_bounded120, pt, closure_lpt_taupt30to40_xtrg_mt_%(proc)s, closure_lpt_taupt40to50_xtrg_mt_%(proc)s, closure_lpt_tauptgt50_xtrg_mt_%(proc)s, closure_lpt_taupt30to40_mt_%(proc)s, closure_lpt_taupt40to50_mt_%(proc)s, closure_lpt_tauptgt50_mt_%(proc)s )' % vars())

# apply corrections to raw fake factors

# nominal ffs + stat uncertainties
for uncert in ['','_unc1_up','_unc1_down','_unc2_up','_unc2_down']:
  w.factory('expr::ff_mt_%(wp)s_qcd%(uncert)s("@0*@1*1.1",rawFF_mt_qcd%(uncert)s, closure_l_pt_mt_qcd)' % vars())
  w.factory('expr::ff_mt_%(wp)s_wjets%(uncert)s("@0*@1*@2*@3",rawFF_mt_w%(uncert)s, closure_l_pt_mt_w, closure_mt_w, closure_met_mt_w)' % vars())
  w.factory('expr::ff_mt_%(wp)s_ttbar%(uncert)s("@0*@1",mc_rawFF_mt_tt%(uncert)s, closure_l_pt_mt_ttmc)' % vars())
  if uncert != '': w.factory('expr::ff_mt_%(wp)s_wjets_closure_mt%(uncert)s("@0*@1*@2*@3",rawFF_mt_w, closure_l_pt_mt_w, closure_mt_w%(uncert)s, closure_met_mt_w)' % vars())
# systematic uncertainties
#need to split this into low and high pT!
w.factory('expr::ff_mt_%(wp)s_qcd_l_pt_high_down("@0*1.1",rawFF_mt_qcd)' % vars())
w.factory('expr::ff_mt_%(wp)s_qcd_l_pt_high_up("@0*(1+2*(@1-1))*1.1",rawFF_mt_qcd, closure_l_pt_mt_qcd)' % vars())
w.factory('expr::ff_mt_%(wp)s_wjets_l_pt_high_down("@0*@1*@2",rawFF_mt_w, closure_mt_w, closure_met_mt_w)' % vars())
w.factory('expr::ff_mt_%(wp)s_wjets_l_pt_high_up("@0*(1+2*(@1-1))*@2*@3",rawFF_mt_w, closure_l_pt_mt_w, closure_mt_w, closure_met_mt_w)' % vars())
w.factory('expr::ff_mt_%(wp)s_ttbar_l_pt_high_down("@0",mc_rawFF_mt_tt)' % vars())
w.factory('expr::ff_mt_%(wp)s_ttbar_l_pt_high_up("@0*(1+2*(@1-1))",mc_rawFF_mt_tt, closure_l_pt_mt_ttmc)' % vars())

for proc in ['qcd', 'wjets', 'ttbar']:
  w.factory('expr::ff_mt_%(wp)s_%(proc)s_l_pt_low_up("((@0<%(xtrg_pt)s)*1.1 + (@0>=%(xtrg_pt)s))*@1", l_pt_bounded120, ff_mt_%(wp)s_%(proc)s )' % vars())
  w.factory('expr::ff_mt_%(wp)s_%(proc)s_l_pt_low_down("((@0<%(xtrg_pt)s)*0.9 + (@0>=%(xtrg_pt)s))*@1", l_pt_bounded120, ff_mt_%(wp)s_%(proc)s )' % vars())

w.factory('expr::ff_mt_%(wp)s_qcd_osss_down("@0*@1*1.1*0.9",rawFF_mt_qcd, closure_l_pt_mt_qcd)' % vars())
w.factory('expr::ff_mt_%(wp)s_qcd_osss_up("@0*@1*1.1*1.1",rawFF_mt_qcd, closure_l_pt_mt_qcd)' % vars())

w.factory('expr::ff_mt_%(wp)s_wjets_mt_unc1_up("@0*@1*@2*@3",rawFF_mt_w, closure_l_pt_mt_w, closure_mt_w_unc1_up, closure_met_mt_w)' % vars())
w.factory('expr::ff_mt_%(wp)s_wjets_mt_unc1_down("@0*@1*@2*@3",rawFF_mt_w, closure_l_pt_mt_w, closure_mt_w_unc1_down, closure_met_mt_w)' % vars())
w.factory('expr::ff_mt_%(wp)s_wjets_mt_unc2_up("@0*@1*@2*@3",rawFF_mt_w, closure_l_pt_mt_w, closure_mt_w_unc2_up, closure_met_mt_w)' % vars())
w.factory('expr::ff_mt_%(wp)s_wjets_mt_unc2_down("@0*@1*@2*@3",rawFF_mt_w, closure_l_pt_mt_w, closure_mt_w_unc2_down, closure_met_mt_w)' % vars())

# add stat uncertainties in njets bins 
for proc in ['qcd', 'wjets']:
  for uncert in ['unc1','unc2']:
    w.factory('expr::ff_mt_%(wp)s_%(proc)s_%(uncert)s_njets0_up("(@0==0)*@1 + (@0!=0)*@2",njets[0], ff_mt_%(wp)s_%(proc)s_%(uncert)s_up, ff_mt_%(wp)s_%(proc)s)' % vars())
    w.factory('expr::ff_mt_%(wp)s_%(proc)s_%(uncert)s_njets0_down("(@0==0)*@1 + (@0!=0)*@2",njets[0], ff_mt_%(wp)s_%(proc)s_%(uncert)s_down, ff_mt_%(wp)s_%(proc)s)' % vars())

    w.factory('expr::ff_mt_%(wp)s_%(proc)s_%(uncert)s_njets1_up("(@0==1)*@1 + (@0!=1)*@2",njets[0], ff_mt_%(wp)s_%(proc)s_%(uncert)s_up, ff_mt_%(wp)s_%(proc)s)' % vars())
    w.factory('expr::ff_mt_%(wp)s_%(proc)s_%(uncert)s_njets1_down("(@0==1)*@1 + (@0!=1)*@2",njets[0], ff_mt_%(wp)s_%(proc)s_%(uncert)s_down, ff_mt_%(wp)s_%(proc)s)' % vars())

    w.factory('expr::ff_mt_%(wp)s_%(proc)s_%(uncert)s_njets2_up("(@0>=2)*@1 + (@0<2)*@2",njets[0], ff_mt_%(wp)s_%(proc)s_%(uncert)s_up, ff_mt_%(wp)s_%(proc)s)' % vars())
    w.factory('expr::ff_mt_%(wp)s_%(proc)s_%(uncert)s_njets2_down("(@0>=2)*@1 + (@0<2)*@2",njets[0], ff_mt_%(wp)s_%(proc)s_%(uncert)s_down, ff_mt_%(wp)s_%(proc)s)' % vars())

# scale FF uncerts by fractions
w.factory('expr::ff_mt_%(wp)s_us("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_wjets, ff_mt_%(wp)s_qcd, ff_mt_%(wp)s_ttbar)' % vars())

for i in ['up','down']:
  for u in ['unc1','unc2']:
    w.factory('expr::ff_mt_%(wp)s_us_ttbar_stat_%(u)s_%(i)s("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_wjets, ff_mt_%(wp)s_qcd, ff_mt_%(wp)s_ttbar_%(u)s_%(i)s)' % vars())
    for njet in [0,1,2]:
      w.factory('expr::ff_mt_%(wp)s_us_qcd_stat_njets%(njet)i_%(u)s_%(i)s("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_wjets, ff_mt_%(wp)s_qcd_%(u)s_njets%(njet)i_%(i)s, ff_mt_%(wp)s_ttbar)' % vars())
      w.factory('expr::ff_mt_%(wp)s_us_wjets_stat_njets%(njet)i_%(u)s_%(i)s("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_wjets_%(u)s_njets%(njet)i_%(i)s, ff_mt_%(wp)s_qcd, ff_mt_%(wp)s_ttbar)' % vars())

    w.factory('expr::ff_mt_%(wp)s_us_wjets_syst_mt_%(u)s_%(i)s("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_wjets_mt_%(u)s_%(i)s, ff_mt_%(wp)s_qcd, ff_mt_%(wp)s_ttbar)' % vars())

    w.factory('expr::ff_mt_%(wp)s_us_qcd_syst_osss_%(i)s("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_wjets, ff_mt_%(wp)s_qcd_osss_%(i)s, ff_mt_%(wp)s_ttbar)' % vars())

  w.factory('expr::ff_mt_%(wp)s_us_wjets_syst_closure_low_%(i)s("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_wjets_l_pt_low_%(i)s, ff_mt_%(wp)s_qcd, ff_mt_%(wp)s_ttbar)' % vars())
  w.factory('expr::ff_mt_%(wp)s_us_qcd_syst_closure_low_%(i)s("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_wjets, ff_mt_%(wp)s_qcd_l_pt_low_%(i)s, ff_mt_%(wp)s_ttbar)' % vars())
  w.factory('expr::ff_mt_%(wp)s_us_ttbar_syst_closure_low_%(i)s("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_wjets, ff_mt_%(wp)s_qcd, ff_mt_%(wp)s_ttbar_l_pt_low_%(i)s)' % vars())

  w.factory('expr::ff_mt_%(wp)s_us_wjets_syst_closure_high_%(i)s("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_wjets_l_pt_high_%(i)s, ff_mt_%(wp)s_qcd, ff_mt_%(wp)s_ttbar)' % vars())
  w.factory('expr::ff_mt_%(wp)s_us_qcd_syst_closure_high_%(i)s("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_wjets, ff_mt_%(wp)s_qcd_l_pt_high_%(i)s, ff_mt_%(wp)s_ttbar)' % vars())
  w.factory('expr::ff_mt_%(wp)s_us_ttbar_syst_closure_high_%(i)s("@0*@3 + @1*@4 + @2*@5", mt_%(wp)s_fracs_wjets, mt_%(wp)s_fracs_qcd, mt_%(wp)s_fracs_ttbar, ff_mt_%(wp)s_wjets, ff_mt_%(wp)s_qcd, ff_mt_%(wp)s_ttbar_l_pt_high_%(i)s)' % vars())

w.Print()
w.writeToFile('fakefactors_us_ws_mt_lite_2017.root')
w.Delete()


#################################################
#### tt channel ####
#################################################

w = ROOT.RooWorkspace('w')

## load all TF1's needed to define fake factors
w.factory('expr::pt_bounded1000("max(min(999.9,@0),40)",pt[40])' % vars())
w.factory('expr::pt_bounded80("max(min(79.9,@0),30)",pt[40])' % vars())

loc = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/us_fakefactors/ff_files_tt_2017/' % vars()

# raw fake factors

fraw = ROOT.TFile(loc+'uncorrected_fakefactors_tt.root')
for uncert in ['','_linear']:
  for njet in ['0','1','2']:
    name = 'rawFF_tt_qcd_%(njet)sjet%(uncert)s' % vars()
    func = fraw.Get(name) 
    if uncert == '': wsptools.SafeWrapFunc(w, ['pt_bounded1000'], func, name=None)
    else: wsptools.SafeWrapFunc(w, ['pt_bounded80'], func, name=None)

w.factory('expr::pt_2_bounded200("max(min(199.9,@0),40)",pt_2[40])' % vars())
fcorr = ROOT.TFile(loc+'FF_corrections_1.root')
for njet in ['0','1','2']:
  name = 'closure_tau2pt_tt_qcd_%(njet)sjet' % vars()
  func = fcorr.Get(name)
  wsptools.SafeWrapFunc(w, ['pt_2_bounded200'], func, name=None)

foscorr = ROOT.TFile(loc+'FF_QCDcorrectionOSSS_tt.root')
name = 'closure_OSSS_mvis_tt_qcd' % vars()
func = foscorr.Get(name)
wsptools.SafeWrapFunc(w, ['mvis'], func, name=None)
name = 'closure_OSSS_mvis_tt_qcd_linear' % vars()
func = foscorr.Get(name)
wsptools.SafeWrapFunc(w, ['mvis'], func, name=None)

w.factory('expr::ff_tt_%(wp)s_us_raw("(@0==0)*@1 + (@0==1)*@2 + (@0>=2)*@3", njets[0], rawFF_tt_qcd_0jet, rawFF_tt_qcd_1jet, rawFF_tt_qcd_2jet)' % vars())

w.factory('expr::ff_tt_%(wp)s_us_raw_stat_up("(@0==0)*@1 + (@0==1)*@2 + (@0>=2)*@3", njets[0], rawFF_tt_qcd_0jet_linear, rawFF_tt_qcd_1jet_linear, rawFF_tt_qcd_2jet_linear)' % vars())
w.factory('expr::ff_tt_%(wp)s_us_raw_stat_down("2*@0 - @1", ff_tt_%(wp)s_us_raw, ff_tt_%(wp)s_us_raw_stat_up)' % vars())

w.factory('expr::ff_tt_%(wp)s_us_ss_closure("(@0==0)*@1 + (@0==1)*@2 + (@0>=2)*@3", njets[0], closure_tau2pt_tt_qcd_0jet, closure_tau2pt_tt_qcd_1jet, closure_tau2pt_tt_qcd_2jet)' % vars())
w.factory('expr::ff_tt_%(wp)s_us_os_closure("(@0<90)*@1 + (@0>=90)*@2", mvis, closure_OSSS_mvis_tt_qcd, closure_OSSS_mvis_tt_qcd_linear)' % vars())

w.factory('expr::ff_tt_%(wp)s_us("@1*@2*((@0!=0)*@3 + (@0==0))", os[1], ff_tt_%(wp)s_us_raw, ff_tt_%(wp)s_us_ss_closure, ff_tt_%(wp)s_us_os_closure)' % vars())
w.factory('expr::ff_tt_%(wp)s_us_stat_up("@1*@2*((@0!=0)*@3 + (@0==0))", os[1], ff_tt_%(wp)s_us_raw_stat_up, ff_tt_%(wp)s_us_ss_closure, ff_tt_%(wp)s_us_os_closure)' % vars())
w.factory('expr::ff_tt_%(wp)s_us_stat_down("@1*@2*((@0!=0)*@3 + (@0==0))", os[1], ff_tt_%(wp)s_us_raw_stat_down, ff_tt_%(wp)s_us_ss_closure, ff_tt_%(wp)s_us_os_closure)' % vars())


w.factory('expr::ff_tt_%(wp)s_us_ss_closure_up("@1*@2*@2*((@0!=0)*@3 + (@0==0))", os[1], ff_tt_%(wp)s_us_raw, ff_tt_%(wp)s_us_ss_closure, ff_tt_%(wp)s_us_os_closure)' % vars())
w.factory('expr::ff_tt_%(wp)s_us_ss_closure_down("@1*((@0!=0)*@3 + (@0==0))", os[1], ff_tt_%(wp)s_us_raw, ff_tt_%(wp)s_us_ss_closure, ff_tt_%(wp)s_us_os_closure)' % vars())

w.factory('expr::ff_tt_%(wp)s_us_qcd_syst_closure_njets0_up("(@0!=0)*@1 + (@0==0)*@2", njets, ff_tt_%(wp)s_us, ff_tt_%(wp)s_us_ss_closure_up)' % vars())
w.factory('expr::ff_tt_%(wp)s_us_qcd_syst_closure_njets0_down("(@0!=0)*@1 + (@0==0)*@2", njets, ff_tt_%(wp)s_us, ff_tt_%(wp)s_us_ss_closure_down)' % vars())

w.factory('expr::ff_tt_%(wp)s_us_qcd_syst_closure_njets1_up("(@0!=1)*@1 + (@0==1)*@2", njets, ff_tt_%(wp)s_us, ff_tt_%(wp)s_us_ss_closure_up)' % vars())
w.factory('expr::ff_tt_%(wp)s_us_qcd_syst_closure_njets1_down("(@0!=1)*@1 + (@0==1)*@2", njets, ff_tt_%(wp)s_us, ff_tt_%(wp)s_us_ss_closure_down)' % vars())

w.factory('expr::ff_tt_%(wp)s_us_qcd_syst_closure_njets2_up("(@0<2)*@1 + (@0>=2)*@2", njets, ff_tt_%(wp)s_us, ff_tt_%(wp)s_us_ss_closure_up)' % vars())
w.factory('expr::ff_tt_%(wp)s_us_qcd_syst_closure_njets2_down("(@0<2)*@1 + (@0>=2)*@2", njets, ff_tt_%(wp)s_us, ff_tt_%(wp)s_us_ss_closure_down)' % vars())

w.factory('expr::ff_tt_%(wp)s_us_qcd_stat_njets0_up("(@0!=0)*@1 + (@0==0)*@2", njets, ff_tt_%(wp)s_us, ff_tt_%(wp)s_us_stat_up)' % vars())
w.factory('expr::ff_tt_%(wp)s_us_qcd_stat_njets0_down("(@0!=0)*@1 + (@0==0)*@2", njets, ff_tt_%(wp)s_us, ff_tt_%(wp)s_us_stat_down)' % vars())

w.factory('expr::ff_tt_%(wp)s_us_qcd_stat_njets1_up("(@0!=1)*@1 + (@0==1)*@2", njets, ff_tt_%(wp)s_us, ff_tt_%(wp)s_us_stat_up)' % vars())
w.factory('expr::ff_tt_%(wp)s_us_qcd_stat_njets1_down("(@0!=1)*@1 + (@0==1)*@2", njets, ff_tt_%(wp)s_us, ff_tt_%(wp)s_us_stat_down)' % vars())

w.factory('expr::ff_tt_%(wp)s_us_qcd_stat_njets2_up("(@0<2)*@1 + (@0>=2)*@2", njets, ff_tt_%(wp)s_us, ff_tt_%(wp)s_us_stat_up)' % vars())
w.factory('expr::ff_tt_%(wp)s_us_qcd_stat_njets2_down("(@0<2)*@1 + (@0>=2)*@2", njets, ff_tt_%(wp)s_us, ff_tt_%(wp)s_us_stat_down)' % vars())

w.factory('expr::ff_tt_%(wp)s_us_qcd_syst_osss_up("(@0<90)*(@3/@2)*@1 + (@0>=90)*@1", mvis, ff_tt_%(wp)s_us, closure_OSSS_mvis_tt_qcd, closure_OSSS_mvis_tt_qcd_linear)' % vars())
w.factory('expr::ff_tt_%(wp)s_us_qcd_syst_osss_down("2*@0-@1", ff_tt_%(wp)s_us, ff_tt_%(wp)s_us_qcd_syst_osss_up)' % vars())

w.Print()
w.writeToFile('fakefactors_us_ws_tt_lite_2017.root')
w.Delete() 
