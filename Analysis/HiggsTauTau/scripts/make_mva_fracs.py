import ROOT

f = ROOT.TFile('output/datacard_w_frac_score_vs_qcd_frac_score_inclusive_mt_2018.root')

w = f.Get('mt_inclusive/W')
w.Add(f.Get('mt_inclusive/ZJ'))
w.Add(f.Get('mt_inclusive/VVJ'))
qcd = f.Get('mt_inclusive/data_obs')
ttbar = f.Get('mt_inclusive/TTJ')
real = f.Get('mt_inclusive/ZTT')
real.Add(f.Get('mt_inclusive/TTT'))
real.Add(f.Get('mt_inclusive/VVT'))

qcd.Add(w,-1)
qcd.Add(ttbar,-1)
qcd.Add(real,-1)

def FixNegBins (h):
  for i in range(1, h.GetNbinsX()+1):
    for j in range(1, h.GetNbinsY()+1):
      if h.GetBinContent(i,j)<0: h.SetBinContent(i,j,0.) 
  return h


w = FixNegBins(w)
ttbar = FixNegBins(ttbar)
qcd = FixNegBins(qcd)

total = w.Clone()
total.Add(qcd)
total.Add(ttbar)

w.Divide(total)
qcd.Divide(total)
ttbar.Divide(total)

check = w.Clone()
check.Add(qcd)
check.Add(ttbar)

c1 = ROOT.TCanvas('c1','',600,600)
w.SetStats(0)
w.GetXaxis().SetTitle('QCD score')
w.GetYaxis().SetTitle('W score')
w.SetTitle('')
w.Draw('colz')
c1.Print('w_mva_fracs.pdf')

qcd.SetStats(0)
qcd.GetXaxis().SetTitle('QCD score')
qcd.GetYaxis().SetTitle('W score')
qcd.SetTitle('')
qcd.Draw('colz')
c1.Print('qcd_mva_fracs.pdf')

ttbar.SetStats(0)
ttbar.GetXaxis().SetTitle('QCD score')
ttbar.GetYaxis().SetTitle('W score')
ttbar.SetTitle('')
ttbar.Draw('colz')
#c1.SetLogz()
c1.Print('ttbar_mva_fracs.pdf')

fout = ROOT.TFile('mva_fract_mt_2018.root','RECREATE')
w.Write('W')
qcd.Write('QCD')
ttbar.Write('ttbar')
check.Write('check')

