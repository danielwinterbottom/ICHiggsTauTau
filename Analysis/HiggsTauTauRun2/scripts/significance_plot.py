import ROOT
from array import array
import math
import UserCode.ICHiggsTauTau.plotting as plotting


#file_loc_p = "/vols/cms/gu18/MSSMCombine/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/analysis/cmb_ind/datacards_bsm-model-indep/combined/cmb/"
file_loc_p = "/vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/analysis/limits_0511_betaRd33_0/datacards_vector_leptoquark/combined/cmb/"

#masses = [60,80,100,120,125,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000,1200,1400,1600,1800,2000,2300,2600,2900,3200,3500]
masses = [500,1000,2000,3000,4000,5000]

# change so in bin center
new_masses = [1.5*masses[0]-0.5*masses[1]]
for i in range(0,len(masses)-1):
  new_masses.append((masses[i+1]+masses[i])/2)
new_masses.append(1.5*masses[-1]-0.5*masses[-2]) 


bins = array('f', map(float,new_masses))

limit_dict = {"gg":{},"bb":{}}
limit_dict["gg"]["p_value"] = ROOT.TH1D("gg_p_value",'',len(bins)-1, bins)
limit_dict["bb"]["p_value"] = ROOT.TH1D("bb_p_value",'',len(bins)-1, bins)


for proc in ["gg","bb"]:
  i = 1
  for m in masses:
    #f = ROOT.TFile(file_loc_p+"/higgsCombine.%(proc)sH.Significance.mH%(m)i.root" % vars())
    f = ROOT.TFile(file_loc_p+"/higgsCombine.significance.Significance.mH%(m)i.root" % vars())
    t = f.Get('limit')
    for event in t:
      limit_dict[proc]["p_value"].SetBinContent(i,event.limit)


c = ROOT.TCanvas('c','c',700,700)
c.SetLogx()
c.SetLogy()

limit_dict["gg"]["p_value"].Draw("hist lp+")
limit_dict["gg"]["p_value"].SetStats(0)
limit_dict["gg"]["p_value"].SetLineWidth(2)
limit_dict["gg"]["p_value"].GetXaxis().SetMoreLogLabels()
limit_dict["gg"]["p_value"].GetXaxis().SetNoExponent()
limit_dict["gg"]["p_value"].GetXaxis().SetTitleSize(0.04)
limit_dict["gg"]["p_value"].GetXaxis().SetTitle('m_{#phi} (GeV)')
limit_dict["gg"]["p_value"].GetYaxis().SetTitle('Local p-value')
limit_dict["gg"]["p_value"].GetYaxis().SetTitleOffset(1.2)
limit_dict["gg"]["p_value"].SetLineColor(4)
limit_dict["gg"]["p_value"].SetMarkerColor(4)
limit_dict["gg"]["p_value"].SetMarkerStyle(15)
limit_dict["gg"]["p_value"].SetMaximum(1)
limit_dict["gg"]["p_value"].SetMinimum(0.0001)
limit_dict["gg"]["p_value"].GetXaxis().SetRangeUser(masses[0],masses[-1])

limit_dict["bb"]["p_value"].Draw("hist lp same")
limit_dict["bb"]["p_value"].SetLineWidth(2)
limit_dict["bb"]["p_value"].SetLineColor(2)
limit_dict["bb"]["p_value"].SetMarkerColor(2)
limit_dict["bb"]["p_value"].SetMarkerStyle(15)


c.Update()
l = ROOT.TLegend(0.75,0.15,0.87,0.3)
l.SetBorderSize(0)
l.SetTextSize(0.04)
l.AddEntry(limit_dict["gg"]["p_value"],'gg#phi','l')
l.AddEntry(limit_dict["bb"]["p_value"],'bb#phi','l')
l.Draw()

y = [ROOT.Math.normal_cdf_c(1.0),ROOT.Math.normal_cdf_c(2.0),ROOT.Math.normal_cdf_c(3.0)]
line = []
latex = []
for i in range(0,len(y)):
  line.append(ROOT.TLine(masses[0],y[i],masses[-1],y[i]))
  line[i].SetLineWidth(2)
  line[i].SetLineStyle(2)
  line[i].SetLineColor(13)
  line[i].Draw()

  latex.append(ROOT.TLatex())
  latex[i].SetNDC()
  latex[i].SetTextAngle(0)
  latex[i].SetTextAlign(12)
  latex[i].SetTextSize(0.04)
  latex[i].SetTextColor(13)
  t = c.GetTopMargin()
  b = c.GetBottomMargin()
  r = c.GetRightMargin()
  latex[i].DrawLatex(1.01-r,-b+((1-math.log(y[i])/math.log(0.0001)*(1-t-b))),str(i+1) + "#sigma")

plotting.DrawTitle(c, '138 fb^{-1} (13 TeV)' % vars(), 3, scale=0.7)
plotting.DrawCMSLogo(c, 'CMS', '', 1, 0.045, 0.05, 1.0, '', 0.5)

c.Print('significance_plot.pdf' % vars())
