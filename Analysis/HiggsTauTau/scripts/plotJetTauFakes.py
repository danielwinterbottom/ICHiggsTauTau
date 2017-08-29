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

denominator_file = ROOT.TFile("output/wmnu_denum.root")
numerator_file_med = ROOT.TFile("output/wmnu_num.root")

denom_data = getHistogram(denominator_file,"data_obs","wmnu_inclusive")
denom_VV = getHistogram(denominator_file,"VV","wmnu_inclusive")
denom_TT = getHistogram(denominator_file,"TT","wmnu_inclusive")
denom_ZLL = getHistogram(denominator_file,"ZLL","wmnu_inclusive")
denom_W = getHistogram(denominator_file,"W","wmnu_inclusive")

num_data = getHistogram(numerator_file_med,"data_obs","wmnu_inclusive")
num_VV = getHistogram(numerator_file_med,"VV","wmnu_inclusive")
num_TT = getHistogram(numerator_file_med,"TT","wmnu_inclusive")
num_ZLL = getHistogram(numerator_file_med,"ZLL","wmnu_inclusive")
num_W = getHistogram(numerator_file_med,"W","wmnu_inclusive")

num_W_up = getHistogram(numerator_file_med,"W_JESUp","wmnu_inclusive")
num_W_down = getHistogram(numerator_file_med,"W_JESDown","wmnu_inclusive")
denom_W_up = getHistogram(denominator_file,"W_JESUp","wmnu_inclusive")
denom_W_down = getHistogram(denominator_file,"W_JESDown","wmnu_inclusive")

num_sub_up=num_data.Clone()
num_sub_down=num_data.Clone()
denum_sub_up=denom_data.Clone()
denum_sub_down=denom_data.Clone()

num_sub_up.Add(num_VV,-1.2)
num_sub_up.Add(num_TT,-1.2)
num_sub_up.Add(num_ZLL,-1.2)

num_sub_down.Add(num_VV,-0.8)
num_sub_down.Add(num_TT,-0.8)
num_sub_down.Add(num_ZLL,-0.8)

denum_sub_up.Add(denom_VV,-1.2)
denum_sub_up.Add(denom_TT,-1.2)
denum_sub_up.Add(denom_ZLL,-1.2)

denum_sub_down.Add(denom_VV,-0.8)
denum_sub_down.Add(denom_TT,-0.8)
denum_sub_down.Add(denom_ZLL,-0.8)

num_sub_up.Divide(denum_sub_up)
num_sub_down.Divide(denum_sub_down)


denom_data.Add(denom_VV,-1)
denom_data.Add(denom_TT,-1)
denom_data.Add(denom_ZLL,-1)

num_data.Add(num_VV, -1)
num_data.Add(num_TT, -1)
num_data.Add(num_ZLL, -1)

num_data.Divide(denom_data)
num_W.Divide(denom_W)

num_W_up.Divide(denom_W_up)
num_W_down.Divide(denom_W_down)

#error_hist=num_data.Clone()
#set num_data bins error to the stat error added in quadrature with the subtraction error
#for i in range(1,error_hist.GetNbinsX()+1):
#    stat_error = error_hist.GetBinError(i)
#    sub_up_error = num_sub_up.GetBin

num_sub_up.SetLineColor(ROOT.kRed)
num_sub_up.SetMarkerColor(ROOT.kRed)
num_sub_down.SetLineColor(ROOT.kGreen-2)
num_sub_down.SetMarkerColor(ROOT.kGreen-2)
num_sub_up.SetMarkerStyle(0)
num_sub_down.SetMarkerStyle(0)
num_sub_up.SetLineWidth(2)
num_sub_down.SetLineWidth(2)



num_data.SetMarkerStyle(20)
num_data.SetMarkerColor(1)
num_data.SetLineColor(1)

num_W.SetLineColor(ROOT.kBlue)
num_W.SetMarkerColor(ROOT.kBlue)

data_num_W_up = num_data.Clone()
data_num_W_down = num_data.Clone()

data_num_W_up.SetLineColor(ROOT.kRed)
data_num_W_up.SetMarkerColor(ROOT.kRed)
data_num_W_down.SetLineColor(ROOT.kGreen-2)
data_num_W_down.SetMarkerColor(ROOT.kGreen-2)
data_num_W_up.SetMarkerStyle(0)
data_num_W_down.SetMarkerStyle(0)
data_num_W_up.SetLineWidth(2)
data_num_W_down.SetLineWidth(2)

c2 = ROOT.TCanvas()
c2.cd()

pads=plot.TwoPadSplit(0.29,0.01,0.01)
pads[0].cd()
pads[0].SetLogy(1)
axish = createAxisHists(2,num_data,num_data.GetXaxis().GetXmin(),num_data.GetXaxis().GetXmax()-0.01)
axish[1].GetYaxis().SetTitle("Obs/MC")
axish[1].GetXaxis().SetTitle("jet p_{T} (GeV)")
axish[1].GetYaxis().SetNdivisions(8)
axish[0].GetXaxis().SetTitleSize(0)
axish[0].GetXaxis().SetLabelSize(0)
axish[0].GetYaxis().SetTitle("Misidentification rate")
axish[0].GetYaxis().SetRangeUser(0.0001,1)
axish[0].Draw()
num_data.DrawCopy("PLSAME")
#num_sub_down.DrawCopy("LSAME")
#num_sub_up.DrawCopy("LSAME")
num_W.DrawCopy("E0LSAME")

legend = plot.PositionedLegend(0.3,0.1,3,0.03)
legend.SetTextFont(42)
legend.SetTextSize(0.025)
legend.SetFillColor(0)
legend.AddEntry(num_data,"Observation","P")
#legend.AddEntry(num_sub_up,"MC bkg up","l")
#legend.AddEntry(num_sub_down,"MC bkg down","l")
#legend.AddEntry(data_num_W_up,"JES up","l")
#legend.AddEntry(data_num_W_down,"JES down","l")
legend.AddEntry(num_W,"MC","L")
legend.Draw("same")

plot.DrawCMSLogo(pads[0], 'CMS', 'Preliminary', 11, 0.045,0.05,1.0,'',1.0)
plot.DrawTitle(pads[0],"35.9 fb^{-1}",3)

ratio_data_hist = plot.MakeRatioHist(num_data,num_W,True,True)
ratio_data_hist_sub_down = plot.MakeRatioHist(num_sub_down,num_W,True,True)
ratio_data_hist_sub_up = plot.MakeRatioHist(num_sub_up,num_W,True,True)
ratio_data_hist_jes_down = plot.MakeRatioHist(data_num_W_down,num_W_down,True,True)
ratio_data_hist_jes_up = plot.MakeRatioHist(data_num_W_up,num_W_up,True,True)
#ratio_data_hist = num_data.Clone()
#ratio_data_hist.Divide(num_W)
ratio_data_hist_fit = plot.MakeRatioHist(num_data,num_W,True,True)
ratio_W_hist = plot.MakeRatioHist(num_W,num_W,False,False)


pads[1].cd()
pads[1].SetGrid(0,1)
axish[1].Draw("axis")
axish[1].SetMinimum(0.5)
axish[1].SetMaximum(1.5)
#ratio_W_hist.DrawCopy("E0LSAME")

ratio_data_hist.DrawCopy("PSAME")
#ratio_data_hist_sub_down.DrawCopy("histSAME")
#ratio_data_hist_sub_up.DrawCopy("histSAME")
#ratio_data_hist_jes_down.DrawCopy("histSAME") 
#ratio_data_hist_jes_up.DrawCopy("histSAME")
pads[1].RedrawAxis("G")
func = ROOT.TF1("func","pol1",40,200)
#func.FixParameter(0,1.25)
x = ratio_data_hist.Fit("func","R0+")
plotfn = ratio_data_hist.GetFunction("func")

#plotfn.Draw("LSAME")
pads[1].RedrawAxis("G")


#x = ratio_data_hist.Fit("expo","0S")
#y = x.Get()
#y.Print()


c2.SaveAs("W_jettaufake.pdf")



