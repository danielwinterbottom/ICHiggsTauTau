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

wps = ['tight']

for wp in wps:

  # get fractions

  loc = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/mvadm_ff_deeptauV2p1_2018_newaiso2_ipsig/' % vars()
 
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
     wsptools.SafeWrapHist(
       w, ['expr::m_vis_max300("min(300,@0)",m_vis[0])'],
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

  for njet in [0,1,2]:
    for dm in dm_bins:
      func = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_2018.root:%(dm)s_njets%(njet)i_pt_1_ff_qcd_fit' % vars())
      func_str = func.GetTitle()
      params = func.GetParameters()
      for i in range(0,func.GetNpar()): func_str = func_str.replace('[%(i)i]' % vars(),'%f' % params[i])
      func_str = re.sub('x', '@0', func_str)

      w.factory('expr::pt_bounded("max(min(200,@0),40)",pt[0])' % vars())
      w.factory('expr::tt_%(dm)s_njets%(njet)i_%(wp)s_qcd_fit("%(func_str)s",pt_bounded)' % vars())

  # make njets / dm binned functions

  w.factory('expr::ff_tt_%(wp)s_dmbins_qcd("(@0==0)*((@1==0)*@2+(@1==1)*@3+(@1==10)*@4 + (@1==11)*@5) + (@0==1)*((@1==0)*@6+(@1==1)*@7+(@1==10)*@8 + (@1==11)*@9) + (@0>1)*((@1==0)*@10+(@1==1)*@11+(@1==10)*@12 + (@1==11)*@13)", njets[0], dm[1], tt_dm0_njets0_%(wp)s_qcd_fit, tt_dm1_njets0_%(wp)s_qcd_fit, tt_dm10_njets0_%(wp)s_qcd_fit, tt_dm11_njets0_%(wp)s_qcd_fit, tt_dm0_njets1_%(wp)s_qcd_fit, tt_dm1_njets1_%(wp)s_qcd_fit, tt_dm10_njets1_%(wp)s_qcd_fit, tt_dm11_njets1_%(wp)s_qcd_fit, tt_dm0_njets2_%(wp)s_qcd_fit, tt_dm1_njets2_%(wp)s_qcd_fit, tt_dm10_njets2_%(wp)s_qcd_fit, tt_dm11_njets2_%(wp)s_qcd_fit)' % vars())

  w.factory('expr::ff_tt_%(wp)s_mvadmbins_nosig_qcd("(@0==0)*((@1==0)*@2+(@1==1)*@3+(@1==2)*@4+(@1==10)*@5+(@1==11)*@6) + (@0==1)*((@1==0)*@7+(@1==1)*@8+(@1==2)*@9+(@1==10)*@10+(@1==11)*@11) + (@0>1)*((@1==0)*@12+(@1==1)*@13+(@1==2)*@14+(@1==10)*@15+(@1==11)*@16)", njets[0], mvadm[1], tt_mvadm0_njets0_%(wp)s_qcd_fit, tt_mvadm1_njets0_%(wp)s_qcd_fit, tt_mvadm2_njets0_%(wp)s_qcd_fit, tt_mvadm10_njets0_%(wp)s_qcd_fit, tt_mvadm11_njets0_%(wp)s_qcd_fit, tt_mvadm0_njets1_%(wp)s_qcd_fit, tt_mvadm1_njets1_%(wp)s_qcd_fit, tt_mvadm2_njets1_%(wp)s_qcd_fit, tt_mvadm10_njets1_%(wp)s_qcd_fit, tt_mvadm11_njets1_%(wp)s_qcd_fit, tt_mvadm0_njets2_%(wp)s_qcd_fit, tt_mvadm1_njets2_%(wp)s_qcd_fit, tt_mvadm2_njets2_%(wp)s_qcd_fit, tt_mvadm10_njets2_%(wp)s_qcd_fit, tt_mvadm11_njets2_%(wp)s_qcd_fit)' % vars())

  w.factory('expr::ff_tt_%(wp)s_mvadmbins_qcd("(@0==0)*((@1==0&&@2<3)*@3+(@1==0&&@2>=3)*@4+(@1==1)*@5+(@1==2)*@6+(@1==10)*@7+(@1==11)*@8) + (@0==1)*((@1==0&&@2<3)*@9+(@1==0&&@2>=3)*@10+(@1==1)*@11+(@1==2)*@12+(@1==10)*@13+(@1==11)*@14) + (@0>1)*((@1==0&&@2<3)*@15+(@1==0&&@2>=3)*@16+(@1==1)*@17+(@1==2)*@18+(@1==10)*@19+(@1==11)*@20)", njets[0], mvadm[1], ipsig[0], tt_mvadm0_sig_lt3_njets0_%(wp)s_qcd_fit, tt_mvadm0_sig_gt3_njets0_%(wp)s_qcd_fit, tt_mvadm1_njets0_%(wp)s_qcd_fit, tt_mvadm2_njets0_%(wp)s_qcd_fit, tt_mvadm10_njets0_%(wp)s_qcd_fit, tt_mvadm11_njets0_%(wp)s_qcd_fit, tt_mvadm0_sig_lt3_njets1_%(wp)s_qcd_fit, tt_mvadm0_sig_gt3_njets1_%(wp)s_qcd_fit, tt_mvadm1_njets1_%(wp)s_qcd_fit, tt_mvadm2_njets1_%(wp)s_qcd_fit, tt_mvadm10_njets1_%(wp)s_qcd_fit, tt_mvadm11_njets1_%(wp)s_qcd_fit, tt_mvadm0_sig_lt3_njets2_%(wp)s_qcd_fit, tt_mvadm0_sig_gt3_njets2_%(wp)s_qcd_fit, tt_mvadm1_njets2_%(wp)s_qcd_fit, tt_mvadm2_njets2_%(wp)s_qcd_fit, tt_mvadm10_njets2_%(wp)s_qcd_fit, tt_mvadm11_njets2_%(wp)s_qcd_fit)' % vars())

  # os/ss corrections
  for dm in dm_bins:
    func = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_2018.root:%(dm)s_os_closure_qcd_fit' % vars())
    func_str = "[0] + [1]*x"
    params = func.GetParameters()
    for i in range(0,func.GetNpar()): func_str = func_str.replace('[%(i)i]' % vars(),'%f' % params[i])
    func_str
    func_str = re.sub('x', '@0', func_str)
    w.factory('expr::mvis_bounded("min(300,@0)",mvis[0])' % vars())
    w.factory('expr::tt_%(dm)s_%(wp)s_qcd_corr("%(func_str)s",mvis_bounded)' % vars())
 
  w.factory('expr::tt_dmbins_%(wp)s_qcd_corr("(@0==0)*@1+(@0==1)*@2+(@0==10)*@3+(@0==11)*@4", dm[1], tt_dm0_%(wp)s_qcd_corr,tt_dm1_%(wp)s_qcd_corr,tt_dm10_%(wp)s_qcd_corr,tt_dm11_%(wp)s_qcd_corr)' % vars())

  w.factory('expr::tt_mvadmbins_nosig_%(wp)s_qcd_corr("(@0==0)*@1+(@0==1)*@2+(@0==2)*@3+(@0==10)*@4+(@0==11)*@5", mvadm[1], tt_mvadm0_%(wp)s_qcd_corr,tt_mvadm1_%(wp)s_qcd_corr,tt_mvadm2_%(wp)s_qcd_corr,tt_mvadm10_%(wp)s_qcd_corr,tt_mvadm11_%(wp)s_qcd_corr)' % vars())

  
  for dmtype in ['mvadmbins','mvadmbins_nosig','dmbins']:
    # apply QCD corrections - only do this for the fraction of events that come from QCD
    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s("(@0!=0)*@1*(@2*@3 + (1.-@3)) + (@0==0)*@1", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(wp)s_fracs_qcd)' % vars())
    # add QCD systematics defined by not applying the OS/SS correction and applying the correction twice -> this might be a convervative approach but represents the extent of our knowladge about the QCD OS/SS differences
     
    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_syst_up("(@0!=0)*@1*(@2*@2*@3 + (1.-@3)) + (@0==0)*@1", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(wp)s_fracs_qcd)' % vars())
    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_qcd_syst_down("(@0!=0)*@1 + (@0==0)*@1", os[1], ff_tt_%(wp)s_%(dmtype)s_qcd, tt_%(dmtype)s_%(wp)s_qcd_corr, tt_%(wp)s_fracs_qcd)' % vars())

    # wjets and ttbar systematics from comparing the inclusive fakefactors derived from data for QCD events with the MC fake factors predicted in MC for wjets and ttbar

    for x in ['qcd','wjets_mc','ttbar_mc']:
      func = GetFromTFile(loc+'fakefactor_fits_tt_%(wp)s_2018.root:inclusive_inclusive_pt_1_ff_%(x)s_fit' % vars())
      func_str = func.GetTitle()
      params = func.GetParameters()
      for i in range(0,func.GetNpar()): func_str = func_str.replace('[%(i)i]' % vars(),'%f' % params[i])
      func_str = re.sub('x', '@0', func_str)
      w.factory('expr::pt_bounded("max(min(200,@0),40)",pt[0])' % vars())
      w.factory('expr::tt_inclusive_%(wp)s_%(x)s_fit("%(func_str)s",pt_bounded)' % vars())

    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_wjets_syst_up("@0*(1.-@1 + @1*@2/@3)", ff_tt_%(wp)s_%(dmtype)s, tt_%(wp)s_fracs_wjets, tt_inclusive_%(wp)s_wjets_mc_fit, tt_inclusive_%(wp)s_qcd_fit)' % vars())
    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_wjets_syst_down("@0*(1.-@1 + @1*@3/@2)", ff_tt_%(wp)s_%(dmtype)s, tt_%(wp)s_fracs_wjets, tt_inclusive_%(wp)s_wjets_mc_fit, tt_inclusive_%(wp)s_qcd_fit)' % vars())

    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_ttbar_syst_up("@0*(1.-@1 + @1*@2/@3)", ff_tt_%(wp)s_%(dmtype)s, tt_%(wp)s_fracs_ttbar, tt_inclusive_%(wp)s_ttbar_mc_fit, tt_inclusive_%(wp)s_qcd_fit)' % vars())
    w.factory('expr::ff_tt_%(wp)s_%(dmtype)s_ttbar_syst_down("@0*(1.-@1 + @1*@3/@2)", ff_tt_%(wp)s_%(dmtype)s, tt_%(wp)s_fracs_ttbar, tt_inclusive_%(wp)s_ttbar_mc_fit, tt_inclusive_%(wp)s_qcd_fit)' % vars())

w.Print()
w.writeToFile('fakefactors_ws_2018.root')
w.Delete()
