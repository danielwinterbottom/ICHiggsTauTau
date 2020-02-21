import ROOT

chan='et'

do_highmt=False
do_ss=False
do_aiso=True

filename='output/datacard_w_frac_score_vs_qcd_frac_score_m12_%(chan)s_2018.root' % vars()
if do_highmt: filename ='output/datacard_w_frac_score_vs_qcd_frac_score_m12_highmt_%(chan)s_2018.root' % vars()
if do_ss: filename ='output/datacard_w_frac_score_vs_qcd_frac_score_m12_ss_%(chan)s_2018.root' % vars()
if do_aiso: filename ='output/datacard_w_frac_score_vs_qcd_frac_score_m12_aiso_%(chan)s_2018.root' % vars()

print filename

extra=''
if do_ss: extra='_ss'
if do_aiso: extra='_aiso'
if do_highmt: extra='_highmt'

f = ROOT.TFile(filename)

w = f.Get('%(chan)s_m12%(extra)s/W' % vars())
w.Add(f.Get('%(chan)s_m12%(extra)s/ZJ' % vars()))
w.Add(f.Get('%(chan)s_m12%(extra)s/VVJ' % vars()))
qcd = f.Get('%(chan)s_m12%(extra)s/data_obs' % vars())
ttbar = f.Get('%(chan)s_m12%(extra)s/TTJ' % vars())
real = f.Get('%(chan)s_m12%(extra)s/ZTT' % vars())
real.Add(f.Get('%(chan)s_m12%(extra)s/TTT' % vars()))
real.Add(f.Get('%(chan)s_m12%(extra)s/VVT' % vars()))
real.Add(f.Get('%(chan)s_m12%(extra)s/ZL' % vars()))


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
c1.SetRightMargin(0.12)
w.SetStats(0)
w.GetXaxis().SetTitle('QCD score')
w.GetYaxis().SetTitle('W score')
w.SetTitle('')
w.GetYaxis().SetTitleOffset(1.4)
w.Draw('colz')
c1.Print('w_mva_%(chan)s_2018%(extra)s.pdf' % vars())

qcd.SetStats(0)
qcd.GetXaxis().SetTitle('QCD score')
qcd.GetYaxis().SetTitle('W score')
qcd.SetTitle('')
qcd.GetYaxis().SetTitleOffset(1.4)
qcd.Draw('colz')
c1.Print('qcd_mva_%(chan)s_2018%(extra)s.pdf' % vars())

ttbar.SetStats(0)
ttbar.GetXaxis().SetTitle('QCD score')
ttbar.GetYaxis().SetTitle('W score')
ttbar.SetTitle('')
ttbar.GetYaxis().SetTitleOffset(1.4)
ttbar.Draw('colz')
#c1.SetLogz()
c1.Print('ttbar_mva_fracs_%(chan)s_2018%(extra)s.pdf' % vars())

fout = ROOT.TFile('mva_fract_%(chan)s_2018%(extra)s.root' % vars(),'RECREATE')
w.Write('W')
qcd.Write('QCD')
ttbar.Write('ttbar')
check.Write('check')

