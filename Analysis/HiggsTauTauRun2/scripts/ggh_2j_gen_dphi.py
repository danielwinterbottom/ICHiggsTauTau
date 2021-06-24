import ROOT
from UserCode.ICHiggsTauTau.plotting import CompareHists

#ROOT.gROOT.Reset()
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.gStyle.Reset()

dir='/vols/cms/dw515/Offline/output/SM/gen_output_2018/'
ext='_mt_2018.root'
import json

ps_samples = [
  'JJH0MToTauTauPlusOneJets_Filtered',
  'JJH0MToTauTauPlusTwoJets_Filtered',
  'JJH0MToTauTauPlusZeroJets_Filtered',
]

mm_samples = [
  'JJH0Mf05ph0ToTauTauPlusOneJets_Filtered',
  'JJH0Mf05ph0ToTauTauPlusTwoJets_Filtered',
  'JJH0Mf05ph0ToTauTauPlusZeroJets_Filtered',
]

sm_samples = [
  'JJH0PMToTauTauPlusOneJets_Filtered',
  'JJH0PMToTauTauPlusTwoJets_Filtered',
  'JJH0PMToTauTauPlusZeroJets_Filtered',
]

def GetCrossWeight(samp):
    filename='scripts/params_2018.json'
    with open(filename, 'r') as f:
        info = json.load(f)

    lumi = info['SingleMuon']['lumi']
    xs = info[samp]['xs']
    evt = info[samp]['evt']
    eff = info[samp]['eff']
    print '(lumi, xs, evt, eff) = (%f,%f,%f,%f)' % (lumi, xs, evt, eff)
    return xs*lumi*eff/evt

def DrawHist(samples, name):
  hist = ROOT.TH1D(name, '', 40,-3.1416,3.1415)
  for s in samples:
    w = GetCrossWeight(s)
    f = ROOT.TFile(dir+s+ext)
    t = f.Get('gen_ntuple')
    print '-------'
    print s
    print t.GetEntries()
    h = ROOT.TH1D('h_'+s,'',40,-3.1416,3.1415)
    print h.GetName()
    t.Draw('sjdphi>>h_%(s)s' % vars(),'wt*(n_jets>1&&mjj>500)','goff')
    h = t.GetHistogram()
    h.Scale(w)
    print h.GetName()
    print h.Integral()
    #if s == samples[0]:
    #  hist=h.Clone()
    #else: 
    hist.Add(h)

  hist.SetName(name)
  return hist

print 'doing SM:'
h_sm = DrawHist(sm_samples,'h_sm')
print 'doing PS:'
h_ps = DrawHist(ps_samples,'h_ps')
print 'doing MM:'
h_mm = DrawHist(mm_samples,'h_mm')

#h_mm.Scale(-0.5)
#h_mm.Add(h_sm)
#h_mm.Add(h_ps)

h_mm_old = h_mm.Clone()

for i in range(1,h_mm.GetNbinsX()+1):
  b = h_mm_old.GetBinContent(h_mm.GetNbinsX()+1-i)
  h_mm.SetBinContent(i,b)

print h_sm.Integral(), h_ps.Integral(), h_mm.Integral()

CompareHists([h_sm,h_ps,h_mm],
         ['f_{a3}^{ggH}=0','f_{a3}^{ggH}=1', 'f_{a3}^{ggH}=0.5'],
         "",
         False, # ratio
         False,
         False,
         "0,3.",
         False,
         3.2,
         -3.2,
         False,
         100, #ymax
         0, #ymin
         '#Delta#varphi_{jj}',
         "a.u.",
         0.06,
         True,
         'gen_deltaphi_jj',
         "")
         #"#mu#tau_{h}")
