import ROOT
import json
from array import array

output_folder = '.'
variables = {   
                'mt_tot[20,30,40,50,60,70,80,90,100,110,125,140,160,175,200,225,250,280,320,350,400,450,500,560,630,710,800,890,1000,1200,1500,1900,2400,3000,4000]':'m_{T}^{tot}/GeV',
             }

params_file = "scripts/params_vlq_2018_gu4.json"
with open(params_file) as jsonfile:
  params = json.load(jsonfile)

def DrawHist(var_input, cuts, input_folder,file_name, file_ext ,add_wt='1'):
  var = var_input.split('[')[0]
  bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
  hout = ROOT.TH1D('hout','',len(bins)-1, bins)

  f = ROOT.TFile('%(input_folder)s/%(file_name)s%(file_ext)s' % vars())
  t = f.Get('ntuple')
  h = hout.Clone()
  h.SetName('h')
  t.Draw('%(var)s>>h' % vars(),'wt*wt_tau_trg_mssm*wt_tau_id_mssm*(%(add_wt)s)*(%(cuts)s)' % vars(),'goff')
  h = t.GetHistogram()
  scale = params[file_name]['xs']/params[file_name]['evt']
  h.Scale(scale)
  h.SetDirectory(0);  
  return h

def PlotDistributionComparison(var,x_label,dist_0,dist_0_name,dist_1,dist_1_name,output_folder,save_name,logx=False):
  # ratio is dist_1/dist_2
  bins = array('f', map(float,var.split('[')[1].split(']')[0].split(',')))
  var = var.split('[')[0]

  dist_1_ratio = dist_1.Clone()
  dist_1_ratio.Divide(dist_0)

  dist_0_ratio = dist_0.Clone()
  dist_0_ratio.Divide(dist_0)

  c = ROOT.TCanvas('c','c',600,600)

  pad1 = ROOT.TPad("pad1","pad1",0,0.3,1,1)
  pad1.SetBottomMargin(0.03)
  if logx: pad1.SetLogx()
  pad1.Draw()
  pad1.cd()

  dist_0.Draw("E SAME")
  dist_0.SetMarkerColor(2)
  dist_0.SetLineColor(2)
  dist_0.SetMarkerStyle(19)
  dist_0.SetStats(0)
  dist_0.SetFillColor(38)
  dist_0.GetXaxis().SetTitle(x_label)
  dist_0.GetYaxis().SetTitle('Events')
  dist_0.GetYaxis().SetTitleOffset(1.2)
  dist_0.GetYaxis().SetTitleSize(0.04)
  dist_0.GetXaxis().SetLabelSize(0)


  dist_1.Draw("E SAME")
  dist_1.SetMarkerColor(1)
  dist_1.SetLineColor(1)
  dist_1.SetMarkerStyle(19)


  l = ROOT.TLegend(0.65,0.75,0.9,0.9);
  l.AddEntry(dist_0,dist_0_name,"lep")
  l.AddEntry(dist_1,dist_1_name,"lep")
  l.Draw()

  c.cd()
  pad2 = ROOT.TPad("pad2","pad2",0,0,1,0.28)
  pad2.SetTopMargin(0)
  pad2.SetBottomMargin(0.2)
  if logx: pad2.SetLogx()
  pad2.Draw()
  pad2.cd()

  dist_0_ratio.Draw("E")
  dist_0_ratio.SetMarkerColor(2)
  dist_0_ratio.SetLineColor(2)
  dist_0_ratio.SetMarkerStyle(19)
  dist_0_ratio.SetAxisRange(0.6,1.4,'Y')
  dist_0_ratio.SetStats(0)
  dist_0_ratio.GetXaxis().SetLabelSize(0.08)
  dist_0_ratio.GetYaxis().SetLabelSize(0.08)
  dist_0_ratio.GetXaxis().SetTitle(x_label)
  dist_0_ratio.GetYaxis().SetTitle("Obs/Pred")
  dist_0_ratio.GetYaxis().SetTitleSize(0.1)
  dist_0_ratio.GetYaxis().SetTitleOffset(0.4)
  dist_0_ratio.GetXaxis().SetTitleSize(0.1)
  dist_0_ratio.GetXaxis().SetTitleOffset(0.9)
  if logx:
    dist_0_ratio.GetXaxis().SetMoreLogLabels()
    dist_0_ratio.GetXaxis().SetNoExponent()

  dist_1_ratio.SetMarkerColor(1)
  dist_1_ratio.SetLineColor(1)
  dist_1_ratio.SetMarkerStyle(19)

  ratio_line = ROOT.TLine(dist_0.GetBinLowEdge(1),1,dist_0.GetBinLowEdge(dist_0.GetNbinsX()+1),1)

  dist_0_ratio.Draw("E")
  dist_1_ratio.Draw("E same")
  ratio_line.Draw("l same")
  #dist_1_ratio.SetLineColor(1)
  #dist_1_ratio.SetMarkerStyle(19)

  c.Update()
  name = '%(output_folder)s/%(var)s_%(save_name)s_dist.pdf' % vars()
  c.Print(name)
  c.Close()



for channel in ["tt"]:
  for year in ["2018"]:
    file_ext = "_%(channel)s_%(year)s.root" % vars()

    file_name_2 = "VectorLQToTauTau_betaRd33_0_mU4_gU1"
    file_name_1 = "VectorLQToTauTau_betaRd33_minus1_mU4_gU1"

    input_folder = "/vols/cms/gu18/Offline/output/MSSM/vlq_2018_v6/"
    selection = "(deepTauVsJets_medium_1>0.5 && deepTauVsJets_medium_2>0.5 && leptonveto==0 && (trg_doubletau || (pt_1>180 && trg_singletau_1) || (pt_2>180 && trg_singletau_2)) && deepTauVsEle_vvloose_1 && deepTauVsEle_vvloose_2 && deepTauVsMu_vloose_1 && deepTauVsMu_vloose_2)"

    for var_input in variables:
      hist1 = DrawHist(var_input, "(%(selection)s)" % vars(), input_folder,file_name_1, file_ext ,add_wt='1')
      #hist2 = DrawHist(var_input, "(%(selection)s)" % vars(), input_folder,file_name_2, file_ext ,add_wt='wt_vlq_betaR33_minus1')
      hist2 = DrawHist(var_input, "(%(selection)s)" % vars(), input_folder,file_name_2, file_ext ,add_wt='1')
      PlotDistributionComparison(var_input,var_input.split('[')[0],hist1,"#beta_{R}^{33}=-1", hist2,"Reweighted #beta_{R}^{33}=0",output_folder,'betaRd33_reweight_closure_%(channel)s_%(year)s' % vars(),logx=True)



