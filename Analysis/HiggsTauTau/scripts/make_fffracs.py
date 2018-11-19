import  ROOT
import argparse
import CombineHarvester.CombineTools.plotting as plot

parser = argparse.ArgumentParser()
parser.add_argument('--channel',default='mt',help='Channel')
parser.add_argument('--do2D',default=False,action='store_true')
args = parser.parse_args()

cats = ['0jet', 'boosted', 'dijet_loosemjj_lowboost', 'dijet_loosemjj_boosted', 'dijet_tightmjj_lowboost', 'dijet_tightmjj_boosted']
if args.channel == 'tt': cats = ['0jet_1', 'boosted_1', 'dijet_loosemjj_lowboost_1', 'dijet_loosemjj_boosted_1', 'dijet_tightmjj_lowboost_1', 'dijet_tightmjj_boosted_1', '0jet_2', 'boosted_2', 'dijet_loosemjj_lowboost_2', 'dijet_loosemjj_boosted_2', 'dijet_tightmjj_lowboost_2', 'dijet_tightmjj_boosted_2']

file_name = 'htt_%s.inputs-sm-13TeV-2D.root' % args.channel
f = ROOT.TFile(file_name)

fout = ROOT.TFile('ff_fracs_new_%s.root' % args.channel,'RECREATE')

for cat in cats:

  prefit_dir = '%s_%s' % (args.channel, cat)
  
  total = f.Get('%s/data_obs' % prefit_dir)
  ztt = f.Get('%s/EmbedZTT' % prefit_dir)
  vvt = f.Get('%s/VVT' % prefit_dir)
  ttt = f.Get('%s/TTT' % prefit_dir)
  zl = f.Get('%s/ZL' % prefit_dir)
  if args.channel == 'tt': extra = f.Get('%s/W_realextra' % prefit_dir)
  real = ztt
  real.Add(vvt)
  real.Add(ttt)
  real.Add(zl)
  if args.channel == 'tt': real.Add(extra)
  fakes = total.Clone()
  fakes.Add(real,-1)

  qcd = f.Get('%s/QCD' % prefit_dir)
  w = f.Get('%s/W' % prefit_dir)
  dy = f.Get('%s/ZJ' % prefit_dir)
  vv = f.Get('%s/VVJ' % prefit_dir)
  tt = f.Get('%s/TTJ' % prefit_dir)

  if args.channel != 'tt': w.Add(dy)
  w.Add(vv)

  for i in range(1,qcd.GetNbinsX()+2):
    cont =  qcd.GetBinContent(i)
    if cont < 0:
      qcd.SetBinContent(i,0)
      tot = total.GetBinContent(i)
      tot_new = tot - cont
      total.SetBinContent(i,tot_new)
      fakes.SetBinContent(i,fakes.GetBinContent(i)-cont)

  for i in range(1,w.GetNbinsX()+2):
    cont =  w.GetBinContent(i)
    if cont < 0:
      w.SetBinContent(i,0)
      tot = total.GetBinContent(i)
      tot_new = tot - cont
      total.SetBinContent(i,tot_new)
      fakes.SetBinContent(i,fakes.GetBinContent(i)-cont)

  for i in range(1,tt.GetNbinsX()+2):
    cont =  tt.GetBinContent(i)
    if cont < 0:
      tt.SetBinContent(i,0)
      tot = total.GetBinContent(i)
      tot_new = tot - cont
      total.SetBinContent(i,tot_new)
      fakes.SetBinContent(i,fakes.GetBinContent(i)-cont)

  for i in range(1,real.GetNbinsX()+2):
    cont =  real.GetBinContent(i)
    if cont < 0:
      real.SetBinContent(i,0)
      tot = total.GetBinContent(i)
      tot_new = tot - cont
      total.SetBinContent(i,tot_new)

  if args.channel == 'tt':
    for i in range(1,dy.GetNbinsX()+2):
      cont =  dy.GetBinContent(i)
      if cont < 0:
        dy.SetBinContent(i,0)
        tot = total.GetBinContent(i)
        tot_new = tot - cont
        total.SetBinContent(i,tot_new)
        fakes.SetBinContent(i,fakes.GetBinContent(i)-cont)

  w.SetName('bin_%s_W_fracs' % cat)
  qcd.SetName('bin_%s_QCD_fracs' % cat)
  tt.SetName('bin_%s_TT_fracs' % cat)
  dy.SetName('bin_%s_DY_fracs' % cat)

  w_rtotal = w.Clone()
  qcd_rtotal = qcd.Clone()
  tt_rtotal = tt.Clone()
  dy_rtotal = dy.Clone()
  
  w_rtotal.SetName('bin_%s_W_tot_fracs' % cat)
  qcd_rtotal.SetName('bin_%s_QCD_tot_fracs' % cat)
  tt_rtotal.SetName('bin_%s_TT_tot_fracs' % cat)
  dy_rtotal.SetName('bin_%s_DY_tot_fracs' % cat)
  
  w.Divide(fakes)
  qcd.Divide(fakes)
  tt.Divide(fakes)
  dy.Divide(fakes)

  w_rtotal.Divide(total)
  qcd_rtotal.Divide(total)
  tt_rtotal.Divide(total)
  dy_rtotal.Divide(total)
  real.Divide(total)


  w.Write()
  qcd.Write()
  tt.Write()
  w_rtotal.Write()
  qcd_rtotal.Write()
  tt_rtotal.Write()
  if args.channel == 'tt':
    dy.Write()
    dy_rtotal.Write()


  if not args.do2D:  
    plot.ModTDRStyle(width=600, height=600, t=0.1, b=0.12, l=0.16, r=0.04)
    c1 = ROOT.TCanvas()
    c1.SetTicks(1)
    w_rtotal.SetFillColor(ROOT.TColor.GetColor(222,90,106))
    qcd_rtotal.SetFillColor(ROOT.TColor.GetColor(250,202,255))
    tt_rtotal.SetFillColor(ROOT.TColor.GetColor(155,152,204))
    real.SetFillColor(ROOT.TColor.GetColor(248,206,104))
    dy_rtotal.SetFillColor(ROOT.TColor.GetColor(100,192,232))
    w_rtotal.SetLineColor(ROOT.kBlack)
    qcd_rtotal.SetLineColor(ROOT.kBlack)
    tt_rtotal.SetLineColor(ROOT.kBlack)
    real.SetLineColor(ROOT.kBlack)
    dy.SetLineColor(ROOT.kBlack)
    
    hs = ROOT.THStack()

    hs.Add(w_rtotal)
    hs.Add(qcd_rtotal)
    hs.Add(tt_rtotal)
    if args.channel == 'tt': hs.Add(dy_rtotal)
    hs.Add(real)

    hs.Draw('hist')
    hs.GetXaxis().SetTitle('bin')
    hs.GetYaxis().SetTitle('Background Fraction')
    c1.Update()
    legend = ROOT.TLegend(0.2, 0.92, 0.95, 0.97, '', 'NBNDC')
    if args.channel == 'tt': legend.SetNColumns(5)
    else: legend.SetNColumns(4)
    legend.SetTextSize(0.04)
    legend.SetColumnSeparation(0.0)
    legend.AddEntry(w_rtotal, 'Electroweak',"f")
    legend.AddEntry(qcd_rtotal, 'QCD',"f")
    legend.AddEntry(tt_rtotal, 't#bar{t}',"f")
    if args.channel == 'tt': legend.AddEntry(dy_rtotal, 'DY',"f")
    legend.AddEntry(real, 'e/#mu/#tau#rightarrow#tau_{h}',"f")  
    legend.Draw()

    c1.Print('fffracs_bin_%s_%s.pdf' %(cat,args.channel)) 
