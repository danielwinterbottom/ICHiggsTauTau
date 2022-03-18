import ROOT
import json
from array import array

channel = "mt"
year = "2017"
wp = "medium"
alt_wp = "vvvloose"

file_ext = '_%(channel)s_%(year)s.root' % vars()

if year == '2018':
  lumi = 58826.8469
  params_file = 'scripts/params_mssm_2018.json'
  input_folder = '/vols/cms/dw515/Offline/output/MSSM/trg_check_2018_v5/'

  if channel == "mt":
    crosstrg_pt = 25
    data_files = [
      'SingleMuonA',
      'SingleMuonB',
      'SingleMuonC',
      'SingleMuonD',
      'TauA',
      'TauB',
      'TauC',
      'TauD'
    ]
  elif channel == "et":
    crosstrg_pt = 33
    data_files = [
      'EGammaA',
      'EGammaB',
      'EGammaC',
      'EGammaD',
      'TauA',
      'TauB',
      'TauC',
      'TauD'
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
  input_folder = '/vols/cms/gu18/Offline/output/MSSM/mssm_2017_v4/'

  if channel == "mt":
    crosstrg_pt = 25
    data_files = [
      'SingleMuonB',
      'SingleMuonC',
      'SingleMuonD',
      'SingleMuonE',
      'SingleMuonF',
      'TauB',
      'TauC',
      'TauD',
      'TauE',
      'TauF'
    ]
  elif channel == "et":
    crosstrg_pt = 28
    data_files = [
      'SingleElectronB',
      'SingleElectronC',
      'SingleElectronD',
      'SingleElectronE',
      'SingleElectronF',
      'TauB',
      'TauC',
      'TauD',
      'TauE',
      'TauF'
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
  input_folder = '/vols/cms/gu18/Offline/output/MSSM/mssm_2016_v4/'

  if channel == "mt":
    crosstrg_pt = 23
    data_files = [
      'SingleMuonB',
      'SingleMuonC',
      'SingleMuonD',
      'SingleMuonE',
      'SingleMuonF',
      'SingleMuonG',
      'SingleMuonH',
      'TauB',
      'TauC',
      'TauD',
      'TauE',
      'TauF',
      'TauG',
      'TauH'
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
      'SingleElectronH',
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

with open(params_file) as jsonfile:
  params = json.load(jsonfile)

if year == "2016":
  m_lowpt=23
  e_lowpt=26
  t_highpt=120
  t_lowpt_mt=25
  t_lowpt_et=25
elif year == "2017":
  m_lowpt=25
  e_lowpt=28
  t_highpt=180
  t_lowpt_mt=32
  t_lowpt_et=35
elif year == "2018":
  m_lowpt=25
  e_lowpt=33
  t_highpt=180
  t_lowpt_mt=32
  t_lowpt_et=35

if channel == 'mt':
  baseline_iso_pass = '(iso_1<0.15 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && !leptonveto && pt_2>30 && ((trg_mutaucross&&pt_2>%(t_lowpt_mt)s&&pt_2<%(t_highpt)s&&fabs(eta_2)<2.1&&pt_1<%(m_lowpt)s)||(trg_singlemuon&&pt_1>=%(m_lowpt)s)||(trg_singletau_2&&pt_2>=%(t_highpt)s&&fabs(eta_2)<2.1)))' % vars()
elif channel == 'et':
  baseline_iso_pass = '(iso_1<0.15 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && pt_2>30 && ((trg_etaucross&&pt_2>%(t_lowpt_et)s&&pt_2<%(t_highpt)s&&fabs(eta_2)<2.1&&pt_1<%(e_lowpt)s)||(trg_singleelectron&&pt_1>=%(e_lowpt)s)||(trg_singletau_2&&pt_2>=%(t_highpt)s&&fabs(eta_2)<2.1)))' % vars()
        
baseline_iso_fail = baseline_iso_pass.replace('deepTauVsJets_%(wp)s_2>0.5' % vars(),'deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_%(alt_wp)s_2>0.5' % vars())
baseline_aiso_pass = baseline_iso_pass.replace('iso_1<0.15','iso_1<0.5 && iso_1>0.25')
baseline_aiso_fail = baseline_iso_fail.replace('iso_1<0.15','iso_1<0.5 && iso_1>0.25')

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
      if h.GetBinError(i)/h.GetBinContent(i) > 1.:
        print 'error>1 for, ', h.GetName(), 'bin=', i
        h.SetBinContent(i,0)
        h.SetBinError(i,0)
  return h

def DrawHists(var_input, cuts, name, input_folder, file_ext,doOS=False,add_wt='1',doQCD=True,doW=True,doMC=True,doTT=True,doIso=True,fullMT=False,lowMT=False,qcdMT='50'):
  add_wt = "(" + add_wt + "*wt_tau_trg_mssm*wt_tau_id_mssm)"
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
    # draw data for QCD
    if not doIso:  qcd_cuts = 'mt_1<%(qcdMT)s&&iso_1>0.05' % vars()
    else: qcd_cuts = 'mt_1<%(qcdMT)s' % vars()
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
    # draw data for W+jets
    for i in data_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'((n_deepbjets==0)*(%(cuts)s)*((os==1)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')

      h = t.GetHistogram()
      data_w.Add(h)

    # draw all (non-QCD) backgrounds to be subtracted from data for W estimation
    # only subtract non j->tau fake component
    for i in other_files+ttbar_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'wt*((n_deepbjets==0)*(%(cuts)s)*((os==1)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')
      h = t.GetHistogram()
      scale = lumi*params[i]['xs']/params[i]['evt']
      h.Scale(scale)
      bkgs_w.Add(h)


    # draw data for QCD subtraction
    for i in data_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'((n_deepbjets==0)*(%(cuts)s)*((os==0)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')

      h = t.GetHistogram()
      w_qcd_sub.Add(h)

    # draw MC for QCD subtraction
    for i in other_files+ttbar_files+wjets_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'wt*((n_deepbjets==0)*(%(cuts)s)*((os==0)))*(%(mt_cut)s)*(%(add_wt)s)' % vars(),'goff')
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

def PlotDistributionComparison(var,x_label,dist_1,dist_1_name,dist_2,dist_2_name,output_folder,save_name):
  # ratio is dist_1/dist_2
  bins = array('f', map(float,var.split('[')[1].split(']')[0].split(',')))
  var = var.split('[')[0]

  dist_2_ratio = dist_2.Clone()
  dist_2_ratio.Divide(dist_2)

  dist_1_ratio = dist_1.Clone()
  dist_1_ratio.Divide(dist_2)

  c = ROOT.TCanvas('c','c',1400,700)

  pad1 = ROOT.TPad("pad1","pad1",0,0.3,1,1)
  pad1.SetBottomMargin(0.03)
  if 'mt_tot' in var or 'm_vis' in var:
    pad1.SetLogx()
  pad1.Draw()
  pad1.cd()

  dist_2.Draw("BAR")
  dist_2.SetStats(0)
  dist_2.SetFillColor(38)
  dist_2.GetXaxis().SetTitle(x_label)
  dist_2.GetYaxis().SetTitle('Events')
  dist_2.GetYaxis().SetTitleOffset(0.8)
  dist_2.GetYaxis().SetTitleSize(0.04)
  dist_2.GetXaxis().SetLabelSize(0)

  dist_1.Draw("E SAME")
  dist_1.SetMarkerColor(1)
  dist_1.SetLineColor(1)
  dist_1.SetMarkerStyle(19)

  l = ROOT.TLegend(0.65,0.75,0.9,0.9);
  l.AddEntry(dist_1,dist_1_name,"lep")
  l.AddEntry(dist_2,dist_2_name,"f");
  l.Draw()

  c.cd()
  pad2 = ROOT.TPad("pad2","pad2",0,0,1,0.28)
  pad2.SetTopMargin(0)
  pad2.SetBottomMargin(0.2)
  if 'mt_tot' in var or 'm_vis' in var:
    pad2.SetLogx()
  pad2.Draw()
  pad2.cd()

  dist_2_ratio.SetFillColor(16)
  dist_2_ratio.Draw("E2")
  dist_2_ratio.SetAxisRange(0.6,1.4,'Y')
  dist_2_ratio.SetStats(0)
  dist_2_ratio.GetXaxis().SetLabelSize(0.08)
  dist_2_ratio.GetYaxis().SetLabelSize(0.08)
  dist_2_ratio.GetXaxis().SetTitle(x_label)
  dist_2_ratio.GetYaxis().SetTitle("Obs/Pred")
  dist_2_ratio.GetYaxis().SetTitleSize(0.1)
  dist_2_ratio.GetYaxis().SetTitleOffset(0.2)
  dist_2_ratio.GetXaxis().SetTitleSize(0.1)
  dist_2_ratio.GetXaxis().SetTitleOffset(0.9)

  if 'mt_tot' in var or 'm_vis' in var:
    dist_2_ratio.GetXaxis().SetMoreLogLabels()
    dist_2_ratio.GetXaxis().SetNoExponent()

  line = ROOT.TLine(min(bins),1,max(bins),1)
  line.SetLineWidth(2)
  line.SetLineStyle(2)
  line.SetLineColor(1)
  line.Draw("same")

  dist_1_ratio.Draw("E SAME")
  dist_1_ratio.SetMarkerColor(1)
  dist_1_ratio.SetLineColor(1)
  dist_1_ratio.SetMarkerStyle(19)

  c.Update()
  name = '%(output_folder)s/%(var)s_%(save_name)s_dist.pdf' % vars()
  c.Print(name)
  c.Close()

var = 'mt_tot[20,30,40,50,60,70,80,90,100,110,125,140,160,175,200,225,250,280,320,350,400,450,500,560,630,710,800,890,1000]'
(_,dist_obs,_,_) = DrawHists(var, baseline_iso_pass, 'dist_obs', input_folder, file_ext,doOS=False,add_wt='1',doQCD=False,doW=True,doMC=False,doTT=False,fullMT=False,lowMT=True)
(_,dist_pred,_,_) = DrawHists(var, baseline_iso_fail, 'dist_pred', input_folder, file_ext,doOS=False,add_wt='wt_ff_mssm_wjets_1',doQCD=False,doW=True,doMC=False,doTT=False,fullMT=False,lowMT=True)
PlotDistributionComparison(var,'m_{T}^{tot} (GeV)',dist_obs,'Observed',dist_pred,'Predicted','./','output')


