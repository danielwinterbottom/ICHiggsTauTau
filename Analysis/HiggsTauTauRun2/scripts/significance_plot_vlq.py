import ROOT
from array import array
import math
import UserCode.ICHiggsTauTau.plotting as plotting

scenario = {
            "betaRd33_0":"#beta_{R}^{b#tau} = 0 (best fit)",
            "betaRd33_minus1":"#beta_{R}^{b#tau} = -1 (best fit)",
            "betaRd33_0_offdiag0":"#beta_{R}^{b#tau} = 0 (#beta_{L}^{s#mu}=#beta_{L}^{s#tau}=#beta_{L}^{b#mu}=0)",
             }

colour = {
           "betaRd33_0":1,
           "betaRd33_minus1":2,
           "betaRd33_0_offdiag0":3,
           }

limit_dict = {}
first_pass = True
for key, val in scenario.items():

  file_loc_sig = "/vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/analysis/1801_%(key)s/datacards_vector_leptoquark/combined/cmb/" % vars()
  
  file_loc_p = "/vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/analysis/1801_%(key)s/datacards_vector_leptoquark/combined/cmb/" % vars()
  
  masses = [500,1000,2000,3000,4000,5000]
  
  # change so in bin center
  
  limit_dict[key] = {} 

  limit_dict[key]["p_value"] = ROOT.TGraph()
  
  for m in masses:
    f = ROOT.TFile(file_loc_p+"/higgsCombine.pvalue.Significance.mH%(m)i.root" % vars())
    t = f.Get('limit')
    for event in t:
      limit_dict[key]["p_value"].SetPoint(limit_dict[key]["p_value"].GetN(),float(m)/1000,event.limit)

  if first_pass:
    c = ROOT.TCanvas('c','c',700,700)
    c.SetLogy()

    limit_dict[key]["p_value"].Draw("")
    limit_dict[key]["p_value"].SetLineWidth(2)
    limit_dict[key]["p_value"].GetXaxis().SetTitleSize(0.04)
    limit_dict[key]["p_value"].GetXaxis().SetTitle('M_{U} (GeV)')
    limit_dict[key]["p_value"].GetYaxis().SetTitle('Local p-value')
    limit_dict[key]["p_value"].GetYaxis().SetTitleOffset(1.2)
    limit_dict[key]["p_value"].SetLineColor(colour[key])
    limit_dict[key]["p_value"].SetMarkerColor(colour[key])
    #limit_dict[key]["p_value"].SetMarkerStyle(15)
    limit_dict[key]["p_value"].SetMaximum(1)
    limit_dict[key]["p_value"].SetMinimum(0.001)
    limit_dict[key]["p_value"].GetXaxis().SetRangeUser(float(masses[0])/1000,float(masses[-1])/1000)


    l = ROOT.TLegend(0.42,0.15,0.87,0.4)
    l.SetBorderSize(0)
    l.SetTextSize(0.04)
    l.AddEntry(limit_dict[key]["p_value"],val,'l')

    first_pass = False
  else:
    limit_dict[key]["p_value"].SetLineWidth(2)
    limit_dict[key]["p_value"].SetLineColor(colour[key])
    limit_dict[key]["p_value"].SetMarkerColor(colour[key])
    #limit_dict[key]["p_value"].SetMarkerStyle(15)
    limit_dict[key]["p_value"].Draw("same")

    c.Update()
    l.AddEntry(limit_dict[key]["p_value"],val,'l')

l.Draw()

y = [ROOT.Math.normal_cdf_c(1.0),ROOT.Math.normal_cdf_c(2.0)]
line = []
latex = []
for i in range(0,len(y)):
  line.append(ROOT.TLine(float(masses[0])/1000,y[i],float(masses[-1])/1000,y[i]))
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
  latex[i].DrawLatex(1.01-r,-b+((1-math.log(y[i])/math.log(0.001)*(1-t-b))),str(i+1) + "#sigma")

plotting.DrawTitle(c, '138 fb^{-1} (13 TeV)' % vars(), 3, scale=0.7)
plotting.DrawCMSLogo(c, 'CMS', '', 1, 0.045, 0.05, 1.0, '', 0.5)

c.Print('significance_plot.pdf' % vars())
