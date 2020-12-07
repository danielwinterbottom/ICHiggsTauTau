import ROOT
import re
import os
import time
import json
from array import array

year = '2016'
channel = 'et'

wp = 'medium'
alt_wp = 'vvvloose'
file_ext = '_%(channel)s_%(year)s.root' % vars()


def DrawTitle(pad, text, align, textOffset=0.2,textSize=0.6):
    pad_backup = ROOT.gPad
    pad.cd()
    t = pad.GetTopMargin()
    l = pad.GetLeftMargin()
    r = pad.GetRightMargin()

    pad_ratio = (float(pad.GetWh()) * pad.GetAbsHNDC()) / \
        (float(pad.GetWw()) * pad.GetAbsWNDC())
    if pad_ratio < 1.:
        pad_ratio = 1.

    latex = ROOT.TLatex()
    latex.SetNDC()
    latex.SetTextAngle(0)
    latex.SetTextColor(ROOT.kBlack)
    latex.SetTextFont(42)
    latex.SetTextSize(textSize * t * pad_ratio)

    y_off = 1 - t + textOffset * t
    if align == 1:
        latex.SetTextAlign(11)
    if align == 1:
        latex.DrawLatex(l, y_off, text)
    if align == 2:
        latex.SetTextAlign(21)
    if align == 2:
        latex.DrawLatex(l + (1 - l - r) * 0.5, y_off, text)
    if align == 3:
        latex.SetTextAlign(31)
    if align == 3:
        latex.DrawLatex(1 - r, y_off, text)
    pad_backup.cd()


def DrawCMSLogo(pad, cmsText, extraText, iPosX, relPosX, relPosY, relExtraDY, extraText2='', cmsTextSize=0.8):
    """Blah
    
    Args:
        pad (TYPE): Description
        cmsText (TYPE): Description
        extraText (TYPE): Description
        iPosX (TYPE): Description
        relPosX (TYPE): Description
        relPosY (TYPE): Description
        relExtraDY (TYPE): Description
        extraText2 (str): Description
        cmsTextSize (float): Description
    
    Returns:
        TYPE: Description
    """
    pad.cd()
    cmsTextFont = 62  # default is helvetic-bold

    writeExtraText = len(extraText) > 0
    writeExtraText2 = len(extraText2) > 0
    extraTextFont = 52

    # text sizes and text offsets with respect to the top frame
    # in unit of the top margin size
    lumiTextOffset = 0.2
    # cmsTextSize = 0.8
    # float cmsTextOffset    = 0.1;  // only used in outOfFrame version

    # ratio of 'CMS' and extra text size
    extraOverCmsTextSize = 0.76

    outOfFrame = False
    if iPosX / 10 == 0:
        outOfFrame = True

    alignY_ = 3
    alignX_ = 2
    if (iPosX / 10 == 0):
        alignX_ = 1
    if (iPosX == 0):
        alignX_ = 1
    if (iPosX == 0):
        alignY_ = 1
    if (iPosX / 10 == 1):
        alignX_ = 1
    if (iPosX / 10 == 2):
        alignX_ = 2
    if (iPosX / 10 == 3):
        alignX_ = 3
    # if (iPosX == 0): relPosX = 0.14
    align_ = 10 * alignX_ + alignY_

    l = pad.GetLeftMargin()
    t = pad.GetTopMargin()
    r = pad.GetRightMargin()
    b = pad.GetBottomMargin()

    latex = ROOT.TLatex()
    latex.SetNDC()
    latex.SetTextAngle(0)
    latex.SetTextColor(ROOT.kBlack)

    extraTextSize = extraOverCmsTextSize * cmsTextSize
    pad_ratio = (float(pad.GetWh()) * pad.GetAbsHNDC()) / \
        (float(pad.GetWw()) * pad.GetAbsWNDC())
    if (pad_ratio < 1.):
        pad_ratio = 1.
    if outOfFrame:
        latex.SetTextFont(cmsTextFont)
        latex.SetTextAlign(11)
        latex.SetTextSize(cmsTextSize * t * pad_ratio)
        latex.DrawLatex(l, 1 - t + lumiTextOffset * t, cmsText)

    posX_ = 0
    if iPosX % 10 <= 1:
        posX_ = l + relPosX * (1 - l - r)
    elif (iPosX % 10 == 2):
        posX_ = l + 0.5 * (1 - l - r)
    elif (iPosX % 10 == 3):
        posX_ = 1 - r - relPosX * (1 - l - r)

    posY_ = 1 - t - relPosY * (1 - t - b)
    if not outOfFrame:
        latex.SetTextFont(cmsTextFont)
        latex.SetTextSize(cmsTextSize * t * pad_ratio)
        latex.SetTextAlign(align_)
        latex.DrawLatex(posX_, posY_, cmsText)
        if writeExtraText:
            latex.SetTextFont(extraTextFont)
            latex.SetTextAlign(align_)
            latex.SetTextSize(extraTextSize * t * pad_ratio)
            latex.DrawLatex(
                posX_, posY_ - relExtraDY * cmsTextSize * t, extraText)
            if writeExtraText2:
                latex.DrawLatex(
                    posX_, posY_ - 1.8 * relExtraDY * cmsTextSize * t, extraText2)
    elif writeExtraText:
        if iPosX == 0:
            posX_ = l + relPosX * (1 - l - r)
            posY_ = 1 - t + lumiTextOffset * t
        latex.SetTextFont(extraTextFont)
        latex.SetTextSize(extraTextSize * t * pad_ratio)
        latex.SetTextAlign(align_)
        latex.DrawLatex(posX_, posY_, extraText)


def DrawHists(var_input, cuts, name, input_folder, file_ext,doOS=False,add_wt='1',doQCD=True,doW=True,doMC=True,doTT=True,doIso=True,fullMT=False,lowMT=False,qcdMT='50'):
  if ':' in var_input:
    #pass 2D inputs like x[],y[]
    var_input1 = var_input.split(':')[0]
    var_input2 = var_input.split(':')[1]
    var1 = var_input1.split('[')[0]
    var2 = var_input2.split('[')[0]

    bins1 = array('f', map(float,var_input1.split('[')[1].split(']')[0].split(',')))
    bins2 = array('f', map(float,var_input2.split('[')[1].split(']')[0].split(',')))
    hout = ROOT.TH2D('hout','',len(bins1)-1, bins1,len(bins2)-1, bins2)
    hout.GetXaxis().SetTitle(var1)
    hout.GetYaxis().SetTitle(var2)
    var='%(var2)s:%(var1)s' % vars()
  else:
    var = var_input.split('[')[0]
    bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
    hout = ROOT.TH1D('hout','',len(bins)-1, bins)
  gen_extra='(gen_match_2==6)'

  data_qcd = hout.Clone()
  data_qcd.SetName(name+'_qcd')
  data_w = hout.Clone()
  data_w.SetName(name+'_wjets')
  wjets = hout.Clone()
  wjets.SetName(name+'_wjets_mc')
  ttbar = hout.Clone()
  ttbar.SetName(name+'_ttbar_mc')
  bkgs_qcd = hout.Clone()
  bkgs_qcd.SetName(name+'_bkgs_qcd')
  bkgs_w = hout.Clone()
  bkgs_w.SetName(name+'_bkgs_wjets')
  w_qcd_sub = hout.Clone()
  w_qcd_sub.SetName(name+'_qcd_bkgs_wjets')

  w_qcd_sub_mc = hout.Clone()
  w_qcd_sub_mc.SetName(name+'_qcd_mc_bkgs_wjets')

  if doQCD:
    print "Drawing QCD distribution:"
    # draw data for QCD
    if not doIso:  qcd_cuts = 'mt_1<%(qcdMT)s&&iso_1>0.05' % vars()
    else: qcd_cuts = 'mt_1<%(qcdMT)s' % vars()
    print "- Drawing data."
    for i in data_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      if not doOS:
        t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==0)))*(%(qcd_cuts)s)*(%(add_wt)s)' % vars(),'goff')
      else:
        t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==1)))*(%(qcd_cuts)s)*(%(add_wt)s)' % vars(),'goff')

      h = t.GetHistogram()
      data_qcd.Add(h)

    # draw all backgrounds to be subtracted from data for QCD estimation
    # only subtract non j->tau fake component
    print "- Drawing subtracted background."
    for i in other_files+wjets_files+ttbar_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      if not doOS:
        t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==0)))*(%(qcd_cuts)s)*(%(add_wt)s)' % vars(),'goff')
      else:
        t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==1)))*(%(qcd_cuts)s)*(%(add_wt)s)' % vars(),'goff')
      h = t.GetHistogram()
      scale = lumi*params[i]['xs']/params[i]['evt']
      h.Scale(scale)
      bkgs_qcd.Add(h)

    bkgs_qcd = ZeroNegativeBins(bkgs_qcd)
    data_qcd.Add(bkgs_qcd,-1)
    data_qcd = ZeroNegativeBins(data_qcd)

  if fullMT: mt_cut='1'
  elif lowMT: mt_cut='mt_1<70'
  else: mt_cut='mt_1>70'
  if doW:
    print "Drawing W + Jets distribution:"
    # draw data for W+jets
    for i in data_files:
      print "- Drawing data."
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'((n_deepbjets==0)*(%(cuts)s)*((os==1)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')
      #t.Draw('%(var)s>>h' % vars(),'((%(cuts)s)*((os==1)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')

      h = t.GetHistogram()
      data_w.Add(h)

    # draw all (non-QCD) backgrounds to be subtracted from data for W estimation
    # only subtract non j->tau fake component
    print "- Drawing subtracted MC."
    for i in other_files+ttbar_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'wt*((n_deepbjets==0)*(%(cuts)s)*((os==1)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')
      #t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==1)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')
      h = t.GetHistogram()
      scale = lumi*params[i]['xs']/params[i]['evt']
      h.Scale(scale)
      bkgs_w.Add(h)


    # draw data for QCD subtraction
    print "- Drawing QCD from data."
    for i in data_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'((n_deepbjets==0)*(%(cuts)s)*((os==0)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')
      #t.Draw('%(var)s>>h' % vars(),'((%(cuts)s)*((os==0)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')

      h = t.GetHistogram()
      w_qcd_sub.Add(h)

    # draw MC for QCD subtraction
    print "- Drawing background for QCD subtraction."
    for i in other_files+ttbar_files+wjets_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'wt*((n_deepbjets==0)*(%(cuts)s)*((os==0)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')
      #t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==0)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')
      h = t.GetHistogram()
      scale = lumi*params[i]['xs']/params[i]['evt']
      h.Scale(scale)
      w_qcd_sub_mc.Add(h)

    w_qcd_sub_mc = ZeroNegativeBins(w_qcd_sub_mc)
    w_qcd_sub.Add(w_qcd_sub_mc,-1)
    w_qcd_sub = ZeroNegativeBins(w_qcd_sub)
    # scale by OS/SS ratio
    w_qcd_sub.Scale(1.1)

    bkgs_w = ZeroNegativeBins(bkgs_w)
    data_w.Add(bkgs_w,-1)
    data_w.Add(w_qcd_sub,-1)
    data_w = ZeroNegativeBins(data_w)

  if doMC:
    print "Drawing W + jets MC"
    # draw wjets when tau candidate is a jet fake
    for i in wjets_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==1)))*(%(mt_cut)s)*(%(gen_extra)s)*(%(add_wt)s)' % vars(),'goff')
      h = t.GetHistogram()
      scale = lumi*params[i]['xs']/params[i]['evt']
      h.Scale(scale)
      wjets.Add(h)

    wjets = ZeroNegativeBins(wjets)

  if doTT:
    print "Drawing ttbar MC"
    # draw ttbar when tau candidate is a jet fake 
    for i in ttbar_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s))*(os==1)*(mt_1<70)*(%(gen_extra)s)*(%(add_wt)s)' % vars(),'goff')
      h = t.GetHistogram()
      scale = lumi*params[i]['xs']/params[i]['evt']
      h.Scale(scale)
      ttbar.Add(h)

    ttbar = ZeroNegativeBins(ttbar)

  return (data_qcd, data_w, wjets, ttbar)

def ZeroNegativeBins(h):
  # if a bin of h1 is equal to 0 ir negative then remove it
  # also remove non zero bins with > 100% errors
  for i in range(1,h.GetNbinsX()+1):
    if h.GetBinContent(i) <= 0:
      h.SetBinContent(i,0)
      h.SetBinError(i,0)
  return h

def ZeroLargeErrorBins(h):
  # remove non zero bins with > 100% errors
  for i in range(1,h.GetNbinsX()+1):
    if h.GetBinContent(i) != 0:
      if h.GetBinError(i)/h.GetBinContent(i) > 0.5:
        print 'error>1 for, ', h.GetName(), 'bin=', i
        h.SetBinContent(i,0)
        h.SetBinError(i,0)
  return h


def CalculateFakeFactors(num,denum):
  name = num.GetName().replace('_iso','_ff')
  ff = num.Clone()
  ff.SetName(name)
  ff.Divide(denum)
  ff = ZeroLargeErrorBins(ff)
  return ff

if year == '2018':
  lumi = 58826.8469
  params_file = 'scripts/params_2018.json'
  input_folder = '/vols/cms/gu18/Offline/output/MSSM/mssm_2018/'

  if channel == "mt":
    crosstrg_pt = 25
    data_files = [
      'SingleMuonA',
      'SingleMuonB',
      'SingleMuonC',
      'SingleMuonD'
    ]
  elif channel == "et":
    crosstrg_pt = 33
    data_files = [
      'EGammaA',
      'EGammaB',
      'EGammaC',
      'EGammaD'
    ]

  ttbar_files = [
    'TTTo2L2Nu',
    'TTToHadronic',
    'TTToSemiLeptonic'
  ]

  wjets_files = [
    'W1JetsToLNu-LO',
    'W2JetsToLNu-LO',
    'W3JetsToLNu-LO',
    'W4JetsToLNu-LO',
    'WJetsToLNu-LO',
    'EWKWMinus2Jets',
    'EWKWPlus2Jets'
  ]

  other_files = [
    'EWKZ2Jets',
    'T-tW-ext1',
    'T-t',
    'DY1JetsToLL-LO',
    'DY2JetsToLL-LO',
    'DY3JetsToLL-LO',
    'DY4JetsToLL-LO',
    'DYJetsToLL-LO',
    'DYJetsToLL_M-10-50-LO',
    'Tbar-tW-ext1',
    'Tbar-t',
    'WWToLNuQQ',
    'WWTo2L2Nu',
    'WZTo1L3Nu',
    'WZTo2L2Q',
    'WZTo3LNu-ext1',
    'WZTo3LNu',
    'ZZTo2L2Nu-ext1',
    'ZZTo2L2Nu-ext2',
    'ZZTo2L2Q',
    'ZZTo4L-ext',
    'ZZTo4L'
  ]

elif year == "2017":
  lumi = 41530.
  params_file = 'scripts/params_mssm_2017.json'
  input_folder = '/vols/cms/gu18/Offline/output/MSSM/mssm_2017/'

  if channel == "mt":
    crosstrg_pt = 25
    data_files = [
      'SingleMuonB',
      'SingleMuonC',
      'SingleMuonD',
      'SingleMuonE',
      'SingleMuonF'
    ]
  elif channel == "et":
    crosstrg_pt = 28
    data_files = [
      'SingleElectronB',
      'SingleElectronC',
      'SingleElectronD',
      'SingleElectronE',
      'SingleElectronF',
    ]
 
  ttbar_files = [
    'TTTo2L2Nu', 
    'TTToHadronic', 
    'TTToSemiLeptonic'
  ] 
  
  wjets_files = [
    'WJetsToLNu-LO',
    'WJetsToLNu-LO-ext',
    'W1JetsToLNu-LO',
    'W2JetsToLNu-LO',
    'W3JetsToLNu-LO',
    'W4JetsToLNu-LO',
    'EWKWMinus2Jets',
    'EWKWPlus2Jets'
  ]

  other_files = [
    'DYJetsToLL-LO',
    'DYJetsToLL-LO-ext1',
    'DY1JetsToLL-LO',
    'DY1JetsToLL-LO-ext',
    'DY2JetsToLL-LO',
    'DY2JetsToLL-LO-ext',
    'DY3JetsToLL-LO',
    'DY3JetsToLL-LO-ext',
    'DY4JetsToLL-LO',
    'DYJetsToLL_M-10-50-LO',
    'DYJetsToLL_M-10-50-LO-ext1',
    'T-tW', 
    'Tbar-tW',
    'Tbar-t',
    'T-t',
    'WWToLNuQQ',
    'WZTo2L2Q',
    'WZTo1L1Nu2Q',
    'WZTo1L3Nu',
    'WZTo3LNu',
    'ZZTo2L2Nu',
    'WWTo2L2Nu',
    'ZZTo2L2Q',
    'ZZTo4L-ext',
    'ZZTo4L',
    'EWKZ2Jets'
  ]

elif year == "2016":
  lumi = 35920.
  params_file = 'scripts/params_mssm_2016.json'
  input_folder = '/vols/cms/gu18/Offline/output/MSSM/mssm_2016/'

  if channel == "mt":
    crosstrg_pt = 23
    data_files = [
      'SingleMuonB',
      'SingleMuonC',
      'SingleMuonD',
      'SingleMuonE',
      'SingleMuonF',
      'SingleMuonG',
      'SingleMuonH'
    ]
  elif channel == "et":
    crosstrg_pt = 25
    data_files = [
      'SingleElectronB',
      'SingleElectronC',
      'SingleElectronD',
      'SingleElectronE',
      'SingleElectronF',
      'SingleElectronG',
      'SingleElectronH'
    ]

  ttbar_files = [
    'TT'
  ]

  wjets_files = [
    'WJetsToLNu-LO', 
    'WJetsToLNu-LO-ext',
    'W1JetsToLNu-LO',
    'W2JetsToLNu-LO',
    'W2JetsToLNu-LO-ext',
    'W3JetsToLNu-LO',
    'W3JetsToLNu-LO-ext',
    'W4JetsToLNu-LO',
    'W4JetsToLNu-LO-ext1',
    'W4JetsToLNu-LO-ext2', 
    'EWKWMinus2Jets_WToLNu',
    'EWKWMinus2Jets_WToLNu-ext1',
    'EWKWMinus2Jets_WToLNu-ext2',
    'EWKWPlus2Jets_WToLNu',
    'EWKWPlus2Jets_WToLNu-ext1',
    'EWKWPlus2Jets_WToLNu-ext2'
  ]

  other_files = [
    'DYJetsToLL-LO-ext1',
    'DYJetsToLL-LO-ext2',
    'DY1JetsToLL-LO',
    'DY2JetsToLL-LO',
    'DY3JetsToLL-LO',
    'DY4JetsToLL-LO',
    'DYJetsToLL_M-10-50-LO', 
    'T-tW', 
    'Tbar-tW',
    'Tbar-t','T-t',
    'WWTo1L1Nu2Q',
    'WZJToLLLNu',
    'VVTo2L2Nu',
    'VVTo2L2Nu-ext1',
    'ZZTo2L2Q',
    'ZZTo4L-amcat',
    'WZTo2L2Q',
    'WZTo1L3Nu',
    'WZTo1L1Nu2Q',
    'EWKZ2Jets_ZToLL',
    'EWKZ2Jets_ZToLL-ext1',
    'EWKZ2Jets_ZToLL-ext2'
  ]
  

# read params from json
with open(params_file) as jsonfile:
  params = json.load(jsonfile)

# Basline Selection
iso = 'iso_1<0.15'
anti_iso = 'iso_1<0.5 && iso_1>0.25'
deeptau_jet_pass = 'deepTauVsJets_%(wp)s_2>0.5' % vars()
deeptau_jet_fail = 'deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_%(alt_wp)s_2>0.5' % vars()
if channel == "mt":
  deeptau_lep = 'deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5'
  if year == "2016":
    crosstrg = '((trg_mutaucross&&pt_1<%(crosstrg_pt)s&&pt_2>25)||(trg_singlemuon&&pt_1>%(crosstrg_pt)s))' % vars()
  else: crosstrg = '((trg_mutaucross&&pt_1<%(crosstrg_pt)s&&pt_2>32)||(trg_singlemuon&&pt_1>%(crosstrg_pt)s))' % vars()
elif channel == "et":
  deeptau_lep = "deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5"
  if year == "2016":
    crosstrg = 'trg_singleelectron'
  else:
    crosstrg = "((trg_etaucross&&pt_1<%(crosstrg_pt)s&&pt_2>35)||(trg_singleelectron&&pt_1>%(crosstrg_pt)s))" % vars() 
other_sel = 'leptonveto==0 && pt_2>30 && wt<2'


baseline_iso_pass = '(%(iso)s && %(deeptau_jet_pass)s && %(deeptau_lep)s && %(crosstrg)s && %(other_sel)s)' % vars()
baseline_iso_fail = '(%(iso)s && %(deeptau_jet_fail)s && %(deeptau_lep)s && %(crosstrg)s && %(other_sel)s)' % vars()

var_input = 'jet_pt_2[30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300,310,320,330,340,350]:pt_2[30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300,310,320,330,340,350]'


(data_qcd_pass, data_w_pass, wjets_pass, ttbar_pass) = DrawHists(var_input, baseline_iso_pass, 'pass', input_folder, file_ext,doOS=False,add_wt='1',doQCD=True,doW=True,doMC=False,doTT=True,doIso=False,fullMT=False,lowMT=False,qcdMT='50')
(data_qcd_fail, data_w_fail, wjets_fail, ttbar_fail) = DrawHists(var_input, baseline_iso_fail, 'fail', input_folder, file_ext,doOS=False,add_wt='1',doQCD=True,doW=True,doMC=False,doTT=True,doIso=False,fullMT=False,lowMT=False,qcdMT='50')


ff_data_qcd = ZeroNegativeBins(ZeroLargeErrorBins(CalculateFakeFactors(data_qcd_pass,data_qcd_fail)))
ff_data_w = ZeroNegativeBins(ZeroLargeErrorBins(CalculateFakeFactors(data_w_pass,data_w_fail)))
ff_wjets = ZeroNegativeBins(ZeroLargeErrorBins(CalculateFakeFactors(wjets_pass,wjets_fail)))
ff_ttbar = ZeroNegativeBins(ZeroLargeErrorBins(CalculateFakeFactors(ttbar_pass,ttbar_fail))) 

### plot params ###
min_y_bin = 30.
max_x_bin = 350.

if channel == "mt":
  channel_string = "#mu#tau_{h}"
elif channel == "et":
  channel_string = "e#tau_{h}"

if year == "2016":
  lumi_string = "35.9 fb^{-1}"
elif year == "2017":
  lumi_string = "41.5 fb^{-1}"
elif year == "2018":
  lumi_string = "59.7 fb^{-1}"

### qcd plot ###

c_qcd = ROOT.TCanvas('c_qcd','c_qcd',700,700)
c_qcd.SetRightMargin(0.15)
ff_data_qcd.Draw('colz')
ff_data_qcd.SetStats(0)
ff_data_qcd.SetMinimum(0)
ff_data_qcd.SetMaximum(0.4)
ff_data_qcd.GetYaxis().SetTitle('Tau-p_{T} (GeV)')
ff_data_qcd.GetXaxis().SetTitle('Jet-p_{T} (GeV)')
ff_data_qcd.GetYaxis().SetTitleOffset(1.4)
ff_data_qcd.GetZaxis().SetTitle('F_{F}')
ff_data_qcd.GetZaxis().SetTitleOffset(1.4)

c_qcd.Update()

line1 = ROOT.TLine(1.25*min_y_bin,min_y_bin,max_x_bin,max_x_bin/1.25)
line1.SetLineColor(2)
line1.SetLineWidth(2)
line1.SetLineStyle(2)
line1.Draw()

line2 = ROOT.TLine(1.5*min_y_bin,min_y_bin,max_x_bin,max_x_bin/1.5)
line2.SetLineColor(1)
line2.SetLineWidth(2)
line2.SetLineStyle(2)
line2.Draw()

l = ROOT.TLegend(0.45,0.75,0.65,0.88)
l.SetBorderSize(0)
l.SetTextSize(0.03)
l.AddEntry(line1,'Jet-p_{T}/Tau-p_{T}=1.25','l')
l.AddEntry(line2,'Jet-p_{T}/Tau-p_{T}=1.5','l')
l.Draw()

DrawCMSLogo(c_qcd, 'CMS', 'Preliminary', 11, 0.045, 0.05, 1.0, '', 0.6)
DrawTitle(c_qcd, '%(channel_string)s QCD DR' % vars(), 1, textSize=0.3)
DrawTitle(c_qcd, '%(year)s: %(lumi_string)s (13 TeV)' % vars(), 3, textSize=0.3)

c_qcd.Print('ff_colz_qcd_%(channel)s_%(year)s.pdf' % vars())


### w plot ###

c_w = ROOT.TCanvas('c_w','c_w',700,700)
c_w.SetRightMargin(0.15)
ff_data_w.Draw('colz')
ff_data_w.SetStats(0)
ff_data_w.SetMinimum(0)
ff_data_w.SetMaximum(0.4)
ff_data_w.GetYaxis().SetTitle('Tau-p_{T} (GeV)')
ff_data_w.GetXaxis().SetTitle('Jet-p_{T} (GeV)')
ff_data_w.GetYaxis().SetTitleOffset(1.4)
ff_data_w.GetZaxis().SetTitle('F_{F}')
ff_data_w.GetZaxis().SetTitleOffset(1.4)

c_w.Update()

line1 = ROOT.TLine(1.25*min_y_bin,min_y_bin,max_x_bin,max_x_bin/1.25)
line1.SetLineColor(2)
line1.SetLineWidth(2)
line1.SetLineStyle(2)
line1.Draw()

line2 = ROOT.TLine(1.5*min_y_bin,min_y_bin,max_x_bin,max_x_bin/1.5)
line2.SetLineColor(1)
line2.SetLineWidth(2)
line2.SetLineStyle(2)
line2.Draw()

l = ROOT.TLegend(0.45,0.75,0.65,0.88)
l.SetBorderSize(0)
l.SetTextSize(0.03)
l.AddEntry(line1,'Jet-p_{T}/Tau-p_{T}=1.25','l')
l.AddEntry(line2,'Jet-p_{T}/Tau-p_{T}=1.5','l')
l.Draw()

DrawCMSLogo(c_w, 'CMS', 'Preliminary', 11, 0.045, 0.05, 1.0, '', 0.6)
DrawTitle(c_w, '%(channel_string)s W + Jets DR' % vars(), 1, textSize=0.3)
DrawTitle(c_w, '%(year)s: %(lumi_string)s (13 TeV)' % vars(), 3, textSize=0.3)

c_w.Print('ff_colz_wjets_%(channel)s_%(year)s.pdf' % vars())


### ttbar plot ###

c_tt = ROOT.TCanvas('c_tt','c_tt',700,700)
c_tt.SetLeftMargin(0.15)
ff_ttbar.SetStats(0)
ff_ttbar.Draw('')
ff_ttbar.GetXaxis().SetTitle('p_{T} (GeV)')
ff_ttbar.GetXaxis().SetTitle('n_{jets}')
ff_ttbar.GetYaxis().SetTitleOffset(1.8)
ff_ttbar.GetYaxis().SetTitle('F_{F}')

c_tt.SetRightMargin(0.15)
ff_ttbar.Draw('colz')
ff_ttbar.SetStats(0)
ff_ttbar.SetMinimum(0)
ff_ttbar.SetMaximum(0.4)
ff_ttbar.GetYaxis().SetTitle('Tau-p_{T} (GeV)')
ff_ttbar.GetXaxis().SetTitle('Jet-p_{T} (GeV)')
ff_ttbar.GetYaxis().SetTitleOffset(1.4)
ff_ttbar.GetZaxis().SetTitle('F_{F}')
ff_ttbar.GetZaxis().SetTitleOffset(1.4)

c_tt.Update()

line1 = ROOT.TLine(1.25*min_y_bin,min_y_bin,max_x_bin,max_x_bin/1.25)
line1.SetLineColor(2)
line1.SetLineWidth(2)
line1.SetLineStyle(2)
line1.Draw()

line2 = ROOT.TLine(1.5*min_y_bin,min_y_bin,max_x_bin,max_x_bin/1.5)
line2.SetLineColor(1)
line2.SetLineWidth(2)
line2.SetLineStyle(2)
line2.Draw()

l = ROOT.TLegend(0.45,0.75,0.65,0.88)
l.SetBorderSize(0)
l.SetTextSize(0.03)
l.AddEntry(line1,'Jet-p_{T}/Tau-p_{T}=1.25','l')
l.AddEntry(line2,'Jet-p_{T}/Tau-p_{T}=1.5','l')
l.Draw()

DrawCMSLogo(c_tt, 'CMS', 'Preliminary', 11, 0.045, 0.05, 1.0, '', 0.6)
DrawTitle(c_tt, '%(channel_string)s t#bar{t} DR' % vars(), 1, textSize=0.3)
DrawTitle(c_tt, '%(year)s: %(lumi_string)s (13 TeV)' % vars(), 3, textSize=0.3)

c_tt.Print('ff_colz_ttbar_%(channel)s_%(year)s.pdf' % vars())


