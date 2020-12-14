import ROOT
import re
import os
import time
import json
from array import array



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

def DrawHistsForFractions(var_input, cuts, name, input_folder, file_ext):
  var = var_input.split('[')[0]
  bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
  hout = ROOT.TH1D('hout','',len(bins)-1, bins)
  gen_extra='(gen_match_2==6)'
  data = hout.Clone()
  data.SetName(name+'_qcd')
  wjets = hout.Clone()
  wjets.SetName(name+'_wjets')
  ttbar = hout.Clone()
  ttbar.SetName(name+'_ttbar')
  bkgs = hout.Clone()
  bkgs.SetName(name+'_bkgs')

  # draw data
  for i in data_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'(%(cuts)s)' % vars(),'goff')
    h = t.GetHistogram()
    data.Add(h)

  # draw wjets when the tau candidates is a jet fake - this includes VV and DY events also
  for i in wjets_files+other_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'wt*(%(cuts)s)*(%(gen_extra)s)' % vars(),'goff')
    h = t.GetHistogram()
    scale = lumi*params[i]['xs']/params[i]['evt']
    h.Scale(scale)
    wjets.Add(h)

  # draw ttbar when the tau candidates is a jet fake 
  for i in ttbar_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'wt*(%(cuts)s)*(%(gen_extra)s)' % vars(),'goff')
    h = t.GetHistogram()
    scale = lumi*params[i]['xs']/params[i]['evt']
    h.Scale(scale)
    ttbar.Add(h)

  # draw all backgrounds with no jet fake
  for i in other_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'wt*(%(cuts)s)' % vars(),'goff')
    h = t.GetHistogram()
    scale = lumi*params[i]['xs']/params[i]['evt']
    h.Scale(scale)
    bkgs.Add(h)
  # need to add also the ttbar and W+jet events without any fakes
  for i in wjets_files+ttbar_files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'wt*(%(cuts)s)*((%(gen_extra)s)==0)' % vars(),'goff')
    h = t.GetHistogram()
    scale = lumi*params[i]['xs']/params[i]['evt']
    h.Scale(scale)
    bkgs.Add(h)

  # subtract other processes to get QCD
  data.Add(bkgs,-1); data.Add(wjets,-1); data.Add(ttbar,-1)

  # fix any negative bins to 0 
  for i in range(1,data.GetNbinsX()+1):
    if data.GetBinContent(i) < 0: data.SetBinContent(i,0.)
    if ttbar.GetBinContent(i) < 0: ttbar.SetBinContent(i,0.)
    if wjets.GetBinContent(i) < 0: wjets.SetBinContent(i,0.)

  return (data, wjets, ttbar)

years = ['2016','2017','2018']
channels = ['mt','et']

wp = 'medium'
alt_wp = 'vvvloose'

for year in years:
  for channel in channels:
    file_ext = '_%(channel)s_%(year)s.root' % vars()
    if year == '2018':
      lumi = 58826.8469
      params_file = 'scripts/params_mssm_2018.json'
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
      elif channel == "tt":
        data_files = [
          'TauB',
          'TauC',
          'TauD',
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
      elif channel == "tt":
        data_files = [
          'TauB',
          'TauC',
          'TauD',
          'TauE',
          'TauF',
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
      elif channel == "tt":
        data_files = [
          'TauB',
          'TauC',
          'TauD',
          'TauE',
          'TauF',
          'TauG',
          'TauH'
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
    if channel == "mt" or channel == "et":
      leading_tau = '2'
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
    elif channel == "tt":
      leading_tau = '1'
      baseline_iso_pass = 'deepTauVsJets_%(wp)s_1>0.5 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau' % vars()  
      baseline_iso_fail = 'deepTauVsJets_%(wp)s_1<0.5 && deepTauVsJets_%(alt_wp)s_1>0.5 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_1 && deepTauVsMu_vloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_2 && leptonveto==0 && trg_doubletau' % vars()
    
    
    if channel == "mt":
      channel_string = "#tau_{#mu}#tau_{h}"
    elif channel == "et":
      channel_string = "#tau_{e}#tau_{h}"
    elif channel == "tt":
      channel_string = "#tau_{h}#tau_{h}"
    
    
    if year == "2016":
      lumi_string = "35.9 fb^{-1}"
    elif year == "2017":
      lumi_string = "41.5 fb^{-1}"
    elif year == "2018":
      lumi_string = "59.7 fb^{-1}"
    
    ana_cats = {
      'tightmt_nbjets0':'n_deepbjets==0 && mt_1<40',
      'tightmt_nbjets1':'n_deepbjets>0 && mt_1<40',
      'loosemt_nbjets0':'n_deepbjets==0 && mt_1>40 && mt_1<70',
      'loosemt_nbjets1':'n_deepbjets>0 && mt_1>40 && mt_1<70',
      }
    
    for cat_name,cat_cut in ana_cats.items():
      var= 'mt_tot[0,10,20,30,40,50,60,70,80,90,100,120,140,160,180,200,250,300,350,400,500,700,900,1100,3000]'
      cuts = '(%(baseline_iso_fail)s)*(%(cat_cut)s)' % vars()
      name = '%(channel)s_fracs_%(cat_name)s' % vars()
      qcd_os, wjets_os, ttbar_os = DrawHistsForFractions(var, '%(cuts)s*(os==1)' % vars(), name+'_os', input_folder, file_ext)
      #qcd_ss, wjets_ss, ttbar_ss = DrawHistsForFractions(var, '%(cuts)s*(os==0)' % vars(), name+'_ss', input_folder, file_ext)
      total_os = qcd_os.Clone(); total_os.Add(wjets_os); total_os.Add(ttbar_os) 
      #total_ss = qcd_ss.Clone(); total_ss.Add(wjets_ss); total_ss.Add(ttbar_ss)
      qcd_os.Divide(total_os)
      wjets_os.Divide(total_os)
      ttbar_os.Divide(total_os)
      #qcd_ss.Divide(total_ss)
      #wjets_ss.Divide(total_ss)
      #ttbar_ss.Divide(total_ss)
    
    
      c = ROOT.TCanvas('c','c',700,700)
      c.SetLogx()
      bins = array('f', map(float,var.split('[')[1].split(']')[0].split(',')))
      dummy_hist = ROOT.TH1D("dummy_hist","",len(bins)-1, bins)
      dummy_hist.SetStats(0)
      dummy_hist.GetXaxis().SetMoreLogLabels()
      dummy_hist.GetXaxis().SetNoExponent()
      dummy_hist.GetXaxis().SetTitle('m_{T}^{tot} (GeV)')
      dummy_hist.GetXaxis().SetTitleOffset(1.2)
      dummy_hist.GetYaxis().SetTitle('Fraction')
      dummy_hist.SetMaximum(1.3)
      dummy_hist.Draw()
    
      hs = ROOT.THStack("hs","")
      qcd_os.SetFillColor(ROOT.TColor.GetColor(250,202,255))
      qcd_os.SetLineColor(1)
      wjets_os.SetFillColor(ROOT.TColor.GetColor(222,90,106))
      wjets_os.SetLineColor(1)
      ttbar_os.SetFillColor(ROOT.TColor.GetColor(155,152,204))
      ttbar_os.SetLineColor(1)
      hs.Add(qcd_os,"hist")
      hs.Add(wjets_os,"hist")
      hs.Add(ttbar_os,"hist")
      hs.Draw("same")
    
      c.RedrawAxis()
      l = ROOT.TLegend(0.65,0.75,0.85,0.88)
      l.SetBorderSize(0)
      l.SetTextSize(0.03)
      l.AddEntry(qcd_os,'QCD','f')
      l.AddEntry(wjets_os,'W + Jets','f')
      l.AddEntry(ttbar_os,'t#bar{t}','f')
      l.Draw()
    
      DrawCMSLogo(c, 'CMS', 'Preliminary', 11, 0.045, 0.05, 1.0, '', 0.6)
      DrawTitle(c, '%(channel_string)s' % vars(), 1, textSize=0.4)
      DrawTitle(c, '%(year)s: %(lumi_string)s (13 TeV)' % vars(), 3, textSize=0.4)
    
      c.Print('ff_fraction_%(cat_name)s_%(channel)s_%(year)s.pdf' % vars())
      c.Close()

