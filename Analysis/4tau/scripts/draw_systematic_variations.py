import argparse
import ROOT
import UserCode.ICHiggsTauTau.plotting as plotting

# python scripts/draw_systematic_variations.py --datacard=/vols/cms/gu18/CH/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/shapes/2016/tt/htt_all.inputs-mssm-vs-sm-Run2016-mt_tot_puppi.root --category=tt_Nbtag0 --process=TTL --logx --x_min=100 --x_max=2000 --systematic=CMS_eff_t_highpT_100-500_2016

parser = argparse.ArgumentParser()
parser.add_argument('--datacard',help= 'insert datacard name', default='/vols/cms/gu18/CH/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/shapes/2016/tt/htt_all.inputs-mssm-vs-sm-Run2016-mt_tot_puppi.root')
parser.add_argument('--category',help= 'insert category name', default='tt_Nbtag0')
parser.add_argument('--process',help= 'insert process name', default='TTL')
parser.add_argument('--systematic',help= 'insert systematic name', default='CMS_eff_t_highpT_100-500_2016')
parser.add_argument('--logx', help= 'use a log x axis', action='store_true')
parser.add_argument('--x_label', help= 'insert label for x axis', default='m_{T}^{tot} (GeV)')
parser.add_argument('--x_min', help= 'insert mimimum x value', default=None)
parser.add_argument('--x_max', help= 'insert maximum x value', default=None)
parser.add_argument('--y_ratio_min', help= 'insert mimimum y ratio value', default="0.5")
parser.add_argument('--y_ratio_max', help= 'insert maximum y_ratio value', default="1.5")

args = parser.parse_args()

def SetEmptyBinToOne(h):
  for i in range(0,h.GetNbinsX()+2):
    if h.GetBinContent(i) == 0 and h.GetBinError(i) == 0:
      h.SetBinContent(i,1)
  return h


f = ROOT.TFile(args.datacard)
up = f.Get(args.category+'/'+args.process+'_'+args.systematic+'_up')
down = f.Get(args.category+'/'+args.process+'_'+args.systematic+'_down')
nom = f.Get(args.category+'/'+args.process)


up_over_nom = up.Clone()
nom_over_nom = nom.Clone()
down_over_nom = down.Clone()

up_over_nom.Divide(nom)
nom_over_nom.Divide(nom)
down_over_nom.Divide(nom)


up_int, nom_int, down_int = up.Integral(), nom.Integral(), down.Integral()

up_over_nom = SetEmptyBinToOne(up_over_nom)
nom_over_nom = SetEmptyBinToOne(nom_over_nom)
down_over_nom = SetEmptyBinToOne(down_over_nom)

#up_over_down = up.Clone()
#up_over_down.Divide(down)
#up_over_down = SetEmptyBinToOne(up_over_down)
#up_over_down.Print("all")

if args.x_min == None:
  x_min = up.GetBinLowEdge(0)
else:
  x_min = float(args.x_min)
if args.x_max == None:
  x_max = up.GetBinLowEdge(up.GetNbinsX()+1)
else:
  x_max = float(args.x_max)

c = ROOT.TCanvas('c','c',700,700)

pad1 = ROOT.TPad("pad1","pad1",0,0.4,1,1)
pad1.SetBottomMargin(0.03)
if args.logx: pad1.SetLogx()
pad1.Draw()
pad1.cd()


up.Draw("HIST")
up.SetStats(0)
#up.SetTitle(args.process+'_'+args.systematic)
up.SetTitle("")
up.GetXaxis().SetTitle(args.x_label)
up.SetAxisRange(x_min,x_max,'X')
up.GetYaxis().SetTitle('Events')
up.GetYaxis().SetTitleOffset(0.8)
up.GetYaxis().SetTitleSize(0.04)
up.GetXaxis().SetLabelSize(0)
up.SetLineColor(4)

down.Draw("HIST SAME")
down.SetLineColor(2)
nom.Draw("HIST SAME")
nom.SetLineColor(1)


l = ROOT.TLegend(0.65,0.7,0.88,0.88)
#l = ROOT.TLegend(0.12,0.7,0.35,0.88)
l.AddEntry(nom,"Nominal ({})".format(round(nom_int,2)),"l")
l.AddEntry(up,"Up Shift ({})".format(round(up_int,2)),"l")
l.AddEntry(down,"Down Shift ({})".format(round(down_int,2)),"l")
l.SetBorderSize(0)
l.Draw()

plotting.DrawTitle(pad1, args.category, 1, scale=0.7)
plotting.DrawTitle(pad1, args.process+'_'+args.systematic, 3, scale=0.7)


c.cd()
pad2 = ROOT.TPad("pad2","pad2",0,0,1,0.39)
pad2.SetTopMargin(0.05)
pad2.SetBottomMargin(0.2)
if args.logx: pad2.SetLogx()
pad2.Draw()
pad2.cd()

up_over_nom.SetStats(0)
up_over_nom.SetTitle("")
up_over_nom.Draw("HIST")
up_over_nom.GetXaxis().SetTitle(args.x_label)

up_over_nom.SetAxisRange(float(args.y_ratio_min),float(args.y_ratio_max),'Y')
up_over_nom.SetAxisRange(x_min,x_max,'X')
up_over_nom.GetXaxis().SetLabelSize(0.06)
if args.logx:
  up_over_nom.GetXaxis().SetMoreLogLabels()
  up_over_nom.GetXaxis().SetNoExponent()
up_over_nom.GetYaxis().SetLabelSize(0.06)
up_over_nom.GetYaxis().SetNdivisions(8)
up_over_nom.GetYaxis().SetTitle("Ratio")
up_over_nom.GetYaxis().SetTitleSize(0.06)
up_over_nom.GetYaxis().SetTitleOffset(0.5)
up_over_nom.GetXaxis().SetTitleSize(0.08)
up_over_nom.GetXaxis().SetTitleOffset(0.9)
up_over_nom.SetLineColor(4)

down_over_nom.Draw("HIST SAME")
down_over_nom.SetLineColor(2)

nom_over_nom.Draw("HIST SAME")
nom_over_nom.SetLineColor(1)

c.Update()
name = '{}_{}_syst_variation.pdf'.format(args.process,args.systematic)
c.Print(name)
c.Close()

