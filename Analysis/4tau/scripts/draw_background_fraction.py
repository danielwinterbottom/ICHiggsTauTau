import ROOT
from array import array
from collections import OrderedDict
import argparse
import copy
import json
import seaborn as sns
sns.color_palette

ROOT.gROOT.SetBatch(1)
ROOT.Math.MinimizerOptions.SetDefaultTolerance(1)

parser = argparse.ArgumentParser()
parser.add_argument('--channel', help= 'Name of channel', default='mmtt')
parser.add_argument('--sel', help= 'Selection to place over baseline', default='1')
parser.add_argument('--var', help= 'Variable to use', default='pt_3[20,40,60,80,100,150,200,300]')
parser.add_argument('--xlabel', help= 'x label to use', default='p_{T}^{3} (GeV)')
parser.add_argument('--replace', help= 'Replace axis labels', default='')
parser.add_argument('--split', help= 'Add extra splits', default='')
parser.add_argument('--logx', help= 'Use logx',  action='store_true')
parser.add_argument('--title_left', help= 'Add title left to channel', default='')
parser.add_argument('--output_name', help= 'Name of output file', default='')
args = parser.parse_args()


def DrawHists(var_input, cuts, name, input_folder, file_ext, files, params, lumi, add_wt="1"):
  var = var_input.split('[')[0]
  bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
  hout = ROOT.TH1D('hout','',len(bins)-1, bins)

  hist = hout.Clone()
  hist.SetName(name)

  for i in files:
    f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
    t = f.Get('ntuple')
    h = hout.Clone()
    h.SetName('h')
    t.Draw('%(var)s>>h' % vars(),'wt*(%(cuts)s)*(%(add_wt)s)' % vars(),'goff')
    if params != None:
      scale = lumi*params[i]['xs']/params[i]['evt']
      h.Scale(scale)
    h = t.GetHistogram()
    hist.Add(h)
  return hist

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

def DrawTitle(pad, text, align, textOffset=0.2,textSize=0.6,x=0,y=0):
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
        latex.DrawLatex(l, y_off, text)
    elif align == 2:
        latex.SetTextAlign(21)
        latex.DrawLatex(l + (1 - l - r) * 0.5, y_off, text)
    elif align == 3:
        latex.SetTextAlign(31)
        latex.DrawLatex(1 - r, y_off, text)
    else:
        latex.DrawLatex(x,y,text)
    pad_backup.cd()

mc_datasets = {
  "ztt": {
    "2016-preVFP": ['DYJetsToLL-LO','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10to50-LO'],
    "2016-postVFP": ['DYJetsToLL-LO','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10to50-LO'],
    "2017": ['DYJetsToLL-LO','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10to50-LO'],
    "2018": ['DYJetsToLL-LO','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10to50-LO'],
  },
  "top": {
    "2016-preVFP": ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic'],
    "2016-postVFP": ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic'],
    "2017": ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic'],
    "2018": ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic'],
  },
  "vv": {
    "2016-preVFP": ['WZTo1L1Nu2Q','WZTo3LNu','WZTo3LNu','WWTo2L2Nu','ZZTo2L2Nu','ZZTo4L','T-tW', 'Tbar-tW','Tbar-t','T-t'],
    "2016-postVFP": ['WZTo3LNu','WZTo3LNu','WWTo2L2Nu','ZZTo2L2Nu','ZZTo4L','T-tW', 'Tbar-tW','Tbar-t','T-t'],
    "2017": ['T-tW', 'Tbar-tW','Tbar-t','T-t','WZTo3LNu','WWTo2L2Nu','ZZTo2L2Nu','ZZTo4L'],
    "2018": ['WZTo1L1Nu2Q','WZTo3LNu','WWTo1L1Nu2Q','WWTo2L2Nu','ZZTo2L2Nu','ZZTo4L','Tbar-t','Tbar-tW','T-t','T-tW'],
  },
  "vvv": {
    "2016-preVFP": ['WWZ','WWZ-ext1','WZZ','WZZ-ext1','WWW','WWW-ext1','ZZZ','ZZZ-ext1'],
    "2016-postVFP": ['WWZ','WWZ-ext1','WZZ','WZZ-ext1','WWW','WWW-ext1','ZZZ','ZZZ-ext1'],
    "2017": ['WWZ','WWZ-ext1','WZZ','WZZ-ext1','WWW','WWW-ext1','ZZZ','ZZZ-ext1'],
    "2018": ['WWZ','WWZ-ext1','WZZ','WZZ-ext1','WWW','WWW-ext1','ZZZ','ZZZ-ext1'],
  },
  "wjets": {
    "2016-preVFP": ['WJetsToLNu-LO' ,'W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets_WToLNu','EWKWPlus2Jets_WToLNu'],
    "2016-postVFP": ['WJetsToLNu-LO' ,'W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets_WToLNu','EWKWPlus2Jets_WToLNu'],
    "2017": ['WJetsToLNu-LO' ,'W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets_WToLNu','EWKWPlus2Jets_WToLNu'],
    "2018": ['WJetsToLNu-LO' ,'W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets_WToLNu','EWKWPlus2Jets_WToLNu'],
  },
  "wgam": {
    "2016-preVFP": ['WGToLNuG'],
    "2016-postVFP": ['WGToLNuG'],
    "2017": ['WGToLNuG'],
    "2018": ['WGToLNuG'],
  },
}

datasets = {
 "tttt": {
   "2016-preVFP": ['TauB','TauC','TauD','TauE','TauF'],
   "2016-postVFP": ['TauF','TauG','TauH'],
   "2017": ['TauB','TauC','TauD','TauE','TauF'],
   "2018": ['TauA','TauB','TauC','TauD'],
 },
 "ttt": {
   "2016-preVFP": ['TauB','TauC','TauD','TauE','TauF'],
   "2016-postVFP": ['TauF','TauG','TauH'],
   "2017": ['TauB','TauC','TauD','TauE','TauF'],
   "2018": ['TauA','TauB','TauC','TauD'],
 },
 "ettt": {
   "2016-preVFP": ['TauB','TauC','TauD','TauE','TauF','SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF'],
   "2016-postVFP": ['TauF','TauG','TauH','SingleElectronF','SingleElectronG','SingleElectronH'],
   "2017": ['TauB','TauC','TauD','TauE','TauF','SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF'],
   "2018": ['TauA','TauB','TauC','TauD','EGammaA','EGammaB','EGammaC','EGammaD'],
 },
 "eett": {
   "2016-preVFP": ['TauB','TauC','TauD','TauE','TauF','SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF'],
   "2016-postVFP": ['TauF','TauG','TauH','SingleElectronF','SingleElectronG','SingleElectronH'],
   "2017": ['TauB','TauC','TauD','TauE','TauF','SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF'],
   "2018": ['TauA','TauB','TauC','TauD','EGammaA','EGammaB','EGammaC','EGammaD'],
 },
 "mttt": {
   "2016-preVFP": ['TauB','TauC','TauD','TauE','TauF','SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF'],
   "2016-postVFP": ['TauF','TauG','TauH','SingleMuonF','SingleMuonG','SingleMuonH'],
   "2017": ['TauB','TauC','TauD','TauE','TauF','SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF'],
   "2018": ['TauA','TauB','TauC','TauD','SingleMuonA','SingleMuonB','SingleMuonC','SingleMuonD'],
 },
 "mmtt": {
   "2016-preVFP": ['TauB','TauC','TauD','TauE','TauF','SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF'],
   "2016-postVFP": ['TauF','TauG','TauH','SingleMuonF','SingleMuonG','SingleMuonH'],
   "2017": ['TauB','TauC','TauD','TauE','TauF','SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF'],
   "2018": ['TauA','TauB','TauC','TauD','SingleMuonA','SingleMuonB','SingleMuonC','SingleMuonD'],
 },
 "emtt": {
   "2016-preVFP": ['TauB','TauC','TauD','TauE','TauF','SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF','SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF'],
   "2016-postVFP": ['TauF','TauG','TauH','SingleMuonF','SingleMuonG','SingleMuonH','SingleElectronF','SingleElectronG','SingleElectronH'],
   "2017": ['TauB','TauC','TauD','TauE','TauF','SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF','SingleElectronB', 'SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF'],
   "2018": ['TauA','TauB','TauC','TauD','SingleMuonA','SingleMuonB','SingleMuonC','SingleMuonD','EGammaA','EGammaB','EGammaC','EGammaD'],
 },
}

selections = {
  "tttt": "(deepTauVsJets_iso_1>0.1 && deepTauVsJets_iso_2>0.1 && deepTauVsJets_iso_3>0.1 && deepTauVsJets_iso_4>0.1 && deepTauVsMu_vloose_1>0.5 && deepTauVsEle_vvloose_1>0.5 && deepTauVsMu_vloose_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_vloose_3>0.5 && deepTauVsEle_vvloose_3>0.5 && deepTauVsMu_vloose_4>0.5 && deepTauVsEle_vvloose_4>0.5 && ((trg_doubletau_12==1) || (trg_doubletau_13==1) || (trg_doubletau_14==1) || (trg_doubletau_23==1) || (trg_doubletau_24==1) || (trg_doubletau_34==1)))",
  "ttt": "(deepTauVsJets_iso_1>0.1 && deepTauVsJets_iso_2>0.1 && deepTauVsJets_iso_3>0.1 && deepTauVsMu_vloose_1>0.5 && deepTauVsEle_vvloose_1>0.5 && deepTauVsMu_vloose_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_vloose_3>0.5 && deepTauVsEle_vvloose_3>0.5 && ((trg_doubletau_12==1) || (trg_doubletau_13==1) || (trg_doubletau_23==1)))",
  "ettt": "(deepTauVsJets_iso_2>0.1 && deepTauVsJets_iso_3>0.1 && deepTauVsJets_iso_4>0.1 && iso_1<0.15 && deepTauVsMu_vloose_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_vloose_3>0.5 && deepTauVsEle_vvloose_3>0.5 && deepTauVsMu_vloose_4>0.5 && deepTauVsEle_vvloose_4>0.5 && ((trg_singleelectron_1==1) || (trg_doubletau_23==1) || (trg_doubletau_24==1) || (trg_doubletau_34==1)) && (((isTau==1) && ((trg_singleelectron_1==1)))==0))",
  "eett": "(deepTauVsJets_iso_3>0.1 && deepTauVsJets_iso_4>0.1 && iso_1<0.15 && iso_2<0.15 && deepTauVsMu_vloose_3>0.5 && deepTauVsEle_vvloose_3>0.5 && deepTauVsMu_vloose_4>0.5 && deepTauVsEle_vvloose_4>0.5 && ((trg_singleelectron_1==1) || (trg_singleelectron_2==1) || (trg_doubletau_34==1)) && (((isTau==1) && ((trg_singleelectron_1==1) || (trg_singleelectron_2==1)))==0))",
  "mttt": "(deepTauVsJets_iso_2>0.1 && deepTauVsJets_iso_3>0.1 && deepTauVsJets_iso_4>0.1 && iso_1<0.15 && deepTauVsMu_vloose_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_vloose_3>0.5 && deepTauVsEle_vvloose_3>0.5 && deepTauVsMu_vloose_4>0.5 && deepTauVsEle_vvloose_4>0.5 && ((trg_singlemuon_1==1) || (trg_doubletau_23==1) || (trg_doubletau_24==1) || (trg_doubletau_34==1)) && (((isTau==1) && ((trg_singlemuon_1==1)))==0))",
  "mmtt": "(deepTauVsJets_iso_3>0.1 && deepTauVsJets_iso_4>0.1 && iso_1<0.15 && iso_2<0.15 && deepTauVsMu_vloose_3>0.5 && deepTauVsEle_vvloose_3>0.5 && deepTauVsMu_vloose_4>0.5 && deepTauVsEle_vvloose_4>0.5 && ((trg_singlemuon_1==1) || (trg_singlemuon_2==1) || (trg_doubletau_34==1)) && (((isTau==1) && ((trg_singlemuon_1==1) || (trg_singlemuon_2==1)))==0))",
  "emtt": "(deepTauVsJets_iso_3>0.1 && deepTauVsJets_iso_4>0.1 && iso_1<0.15 && iso_2<0.15 && deepTauVsMu_vloose_3>0.5 && deepTauVsEle_vvloose_3>0.5 && deepTauVsMu_vloose_4>0.5 && deepTauVsEle_vvloose_4>0.5 && ((trg_singleelectron_1==1) || (trg_singlemuon_2==1) || (trg_doubletau_34==1)) && ((((isTau==1) && (trg_singlemuon_2==1))==0) && (((isSingleElectron==1) && ((trg_singlemuon_1==1) || (trg_doubletau_34==1)))==0)))",
}

param_files = {
  "2016-preVFP": "scripts/params_UL_2016_preVFP.json",
  "2016-postVFP": "scripts/params_UL_2016_postVFP.json",
  "2017": "scripts/params_UL_2017.json",
  "2018": "scripts/params_UL_2018.json",
}

lumis = {
  "2016-preVFP": 19520,
  "2016-postVFP": 16810,
  "2017": 41530,
  "2018": 59830,
}

var = args.var
folder = '/vols/cms/gu18/Offline/output/4tau/1012' 

mc_files = ["ztt","top","vv","wjets"]

replace = {}
if args.replace != "":
 for i in args.replace.split(","):
   replace[i.split(":")[0]] = i.split(":")[1]

split_dict = OrderedDict()
if args.split != "":
 for i in args.split.split(","):
   split_dict["_"+i.split(":")[0]] = i.split(":")[1]
else:
  split_dict[""] = "(1)"


mc_temp = {}
mc_hist = OrderedDict()
for ind, year in enumerate(["2016-preVFP","2016-postVFP","2017","2018"]):

  print year

  with open(param_files[year]) as jsonfile:
    params = json.load(jsonfile)

  data_temp = DrawHists(var, "((" + args.sel + ") && (" + selections[args.channel] + "))", "data", folder, "_"+args.channel+"_"+year.replace("-","_")+".root", datasets[args.channel][year], None, None, add_wt="1")
  for mc_file in mc_files:
    for k,v in split_dict.iteritems():
      mc_temp[mc_file+k] = DrawHists(var, "((" + v + ") && (" + args.sel + ") && (" + selections[args.channel] + "))", mc_file+k, folder, "_"+args.channel+"_"+year.replace("-","_")+".root", mc_datasets[mc_file][year], params, lumis[year], add_wt="1") 

  if ind == 0: 
    data_hist = copy.deepcopy(data_temp)
    for mc_file in mc_files:  
      for k,v in split_dict.iteritems():
        mc_hist[mc_file+k] = copy.deepcopy(mc_temp[mc_file+k])
  else:
    data_hist.Add(copy.deepcopy(data_temp))
    for mc_file in mc_files:  
      for k,v in split_dict.iteritems():
        mc_hist[mc_file+k].Add(copy.deepcopy(mc_temp[mc_file+k]))

qcd_hist = copy.deepcopy(data_hist)
qcd_hist.SetName("qcd")
for k, v in mc_hist.iteritems():
  qcd_hist.Add(v,-1)

# If qcd is negative
for i in range(0,qcd_hist.GetNbinsX()+1):
  if qcd_hist.GetBinContent(i) < 0:
    qcd_hist.SetBinContent(i,0)

hs = ROOT.THStack("hs","")
palette_tmp = list(sns.color_palette("Set2", len(mc_hist)+1))
# drop if integral small
palette = [palette_tmp[0]]
total_hist = copy.deepcopy(qcd_hist)
ind = 0
for k, v in mc_hist.iteritems():
  ind += 1
  if v.Integral() < 0.01*data_hist.Integral():
    print "deleting", k
    del mc_hist[k]
  else:
    palette.append(palette_tmp[ind])
    total_hist.Add(v)

for k, v in mc_hist.iteritems(): mc_hist[k].Divide(total_hist.Clone())
qcd_hist.Divide(total_hist.Clone())

for k, v in mc_hist.iteritems(): v.Print("all")
qcd_hist.Print("all")

c = ROOT.TCanvas('c','c',700,700)
if args.logx: c.SetLogx()
new_bins = array('f', map(float,var.split("[")[1].split("]")[0].split(",")))
dummy_hist = ROOT.TH1D("dummy_hist","",len(new_bins)-1, new_bins)
dummy_hist.SetStats(0)
if args.logx:
  dummy_hist.GetXaxis().SetMoreLogLabels()
  dummy_hist.GetXaxis().SetNoExponent()
dummy_hist.GetXaxis().SetTitle(args.xlabel)
dummy_hist.GetXaxis().SetTitleOffset(1.2)
dummy_hist.GetYaxis().SetTitle('Fraction')
dummy_hist.SetMaximum(1.5)
dummy_hist.Draw()

qcd_hist.SetFillColor(ROOT.TColor.GetColor(palette[0][0],palette[0][1],palette[0][2]))
qcd_hist.SetLineColor(1)
hs.Add(qcd_hist,"hist")
ind = 1
for k, v in mc_hist.iteritems():
  mc_hist[k].SetFillColor(ROOT.TColor.GetColor(palette[ind][0],palette[ind][1],palette[ind][2]))
  mc_hist[k].SetLineColor(1)
  hs.Add(mc_hist[k],"hist")
  ind += 1
hs.Draw("same")

c.RedrawAxis()
l = ROOT.TLegend(0.55,0.65,0.85,0.88)
l.SetBorderSize(0)
l.SetTextSize(0.03)
l.AddEntry(qcd_hist,'QCD','f')
for k, v in mc_hist.iteritems(): 
  if k in replace.keys():
    l.AddEntry(mc_hist[k],replace[k],'f')
  else:
    l.AddEntry(mc_hist[k],k,'f')
l.Draw()
DrawCMSLogo(c, 'CMS', 'Preliminary', 11, 0.045, 0.05, 1.0, '', 0.6)
if args.title_left == "":
  DrawTitle(c, args.channel.replace("e","e_{}^{}").replace("m","#mu_{}^{}").replace("t","#tau_{h}^{}"), 1, textSize=0.4)
else:
  DrawTitle(c, args.channel.replace("e","e_{}^{}").replace("m","#mu_{}^{}").replace("t","#tau_{h}^{}") + " - " + args.title_left , 1, textSize=0.4)
DrawTitle(c, '138 fb^{-1} (13 TeV)' % vars(), 3, textSize=0.4)

c.Print(args.output_name+'.pdf')
