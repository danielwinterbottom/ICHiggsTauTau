import ROOT
import UserCode.ICHiggsTauTau.plotting as plot
import math
import sys
import os

ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.TH1.AddDirectory(False)

def getHistogram(fname, histname, dirname=''):
  if dirname == '' :
    histogram = fname.Get(histname)
  else :
    histogram = fname.Get('%(dirname)s/%(histname)s'%vars())
  if isinstance(histogram, ROOT.TH1):
    return histogram
  else:
    return None


def createAxisHists(n,src,xmin=0,xmax=499):
  result = []
  for i in range(0,n):
    res = src.Clone()
    res.Reset()
    res.SetTitle("")
    res.SetName("axis%(i)d"%vars())
    res.SetAxisRange(xmin,xmax)
    res.SetStats(0)
    result.append(res)
  return result

denominator_file = ROOT.TFile("output/datacard_jet_pt_denominator_coarse_em_2016-extra.root")
numerator_file_med = ROOT.TFile("output/datacard_jet_pt_numerator_medium_coarse_em_2016-extra.root")

denom_data = getHistogram(denominator_file,"data_obs","em_inclusive")
denom_VV = getHistogram(denominator_file,"VV","em_inclusive")
denom_TT = getHistogram(denominator_file,"TT","em_inclusive")
denom_ZLL = getHistogram(denominator_file,"ZLL","em_inclusive")
denom_W = getHistogram(denominator_file,"W","em_inclusive")

num_data = getHistogram(numerator_file_med,"data_obs","em_inclusive")
num_VV = getHistogram(numerator_file_med,"VV","em_inclusive")
num_TT = getHistogram(numerator_file_med,"TT","em_inclusive")
num_ZLL = getHistogram(numerator_file_med,"ZLL","em_inclusive")
num_W = getHistogram(numerator_file_med,"W","em_inclusive")


denom_data.Add(denom_VV,-1)
denom_data.Add(denom_W,-1)
denom_data.Add(denom_ZLL,-1)

num_data.Add(num_VV, -1)
num_data.Add(num_W, -1)
num_data.Add(num_ZLL, -1)

num_data.Divide(denom_data)
num_TT.Divide(denom_TT)

num_data.SetMarkerStyle(20)
num_data.SetMarkerColor(1)
num_data.SetLineColor(1)

num_TT.SetLineColor(ROOT.kGreen+3)

c2 = ROOT.TCanvas()
c2.cd()

pads=plot.TwoPadSplit(0.29,0.01,0.01)
pads[0].cd()
pads[0].SetLogy(1)
axish = createAxisHists(2,num_data,num_data.GetXaxis().GetXmin(),num_data.GetXaxis().GetXmax()-0.01)
axish[1].GetYaxis().SetTitle("Obs/MC")
axish[1].GetXaxis().SetTitle("jet p_{T} [GeV]")
axish[1].GetYaxis().SetNdivisions(8)
axish[0].GetXaxis().SetTitleSize(0)
axish[0].GetXaxis().SetLabelSize(0)
axish[0].GetYaxis().SetTitle("Misidentification rate")
axish[0].GetYaxis().SetRangeUser(0.0001,1)
axish[0].Draw()
num_data.DrawCopy("E0PLSAME")
num_TT.DrawCopy("E0LSAME")

legend = plot.PositionedLegend(0.3,0.1,3,0.03)
legend.SetTextFont(42)
legend.SetTextSize(0.025)
legend.SetFillColor(0)
legend.AddEntry(num_data,"Observed","P")
legend.AddEntry(num_TT,"MC","L")
legend.Draw("same")

plot.DrawCMSLogo(pads[0], 'CMS', 'Preliminary', 11, 0.045,0.05,1.0,'',1.0)
plot.DrawTitle(pads[0],"35.9 fb^{-1}",3)

ratio_data_hist = plot.MakeRatioHist(num_data,num_TT,True,True)
ratio_data_hist_fit = plot.MakeRatioHist(num_data,num_TT,True,True)
ratio_TT_hist = plot.MakeRatioHist(num_TT,num_TT,False,False)


pads[1].cd()
pads[1].SetGrid(0,1)
axish[1].Draw("axis")
axish[1].SetMinimum(0.5)
axish[1].SetMaximum(1.5)
#ratio_W_hist.DrawCopy("LSAME")
ratio_data_hist.DrawCopy("PSAME")
pads[1].RedrawAxis("G")
func = ROOT.TF1("func","pol1",30,140)
x = ratio_data_hist.Fit("func","R0+")
plotfn = ratio_data_hist.GetFunction("func")
#plotfn.Draw("LSAME")
pads[1].RedrawAxis("G")


#x = ratio_data_hist.Fit("expo","0S")
#y = x.Get()
#y.Print()


c2.SaveAs("tt_jetfakes_nofit.pdf")



