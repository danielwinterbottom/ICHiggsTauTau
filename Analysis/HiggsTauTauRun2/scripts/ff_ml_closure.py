import ROOT
import json
from array import array

output_folder = '.'
variables = {   
                'mt_tot[20,30,40,50,60,70,80,90,100,110,125,140,160,175,200,225,250,280,320,350,400,450,500,560,630,710,800,890,1000]':'m_{T}^{tot}/GeV',
                'm_vis[20,30,40,50,60,70,80,90,100,110,125,140,160,175,200,225,250,280,320,350,400,450,500,560,630,710,800,890,1000]':'m_{vis}/GeV',
                'pt_1[20,25,30,35,40,45,50,55,60,65,70,80,90,100,120,140,200,400]':'p_{T}^{#mu}/GeV',
                'pt_2[30,35,40,45,50,55,60,65,70,80,90,100,120,140,200,400]':'p_{T}^{#tau_{h}}/GeV',
                'met[10,20,30,40,50,60,70,80,90,100,120,140,200,400]': 'MET/GeV',
                'eta_2[-2.8,-2.6,-2.4,-2.2,-2.0,-1.8,-1.6,-1.4,-1.2,-1.0,-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8]':'eta_2'
             }

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
  h.SetDirectory(0);  
  return h

def PlotDistributionComparison(var,x_label,dist_0,dist_0_name,dist_1,dist_1_name,dist_2,dist_2_name,output_folder,save_name):
  # ratio is dist_1/dist_2
  bins = array('f', map(float,var.split('[')[1].split(']')[0].split(',')))
  var = var.split('[')[0]

  dist_2_ratio = dist_2.Clone()
  dist_2_ratio.Divide(dist_2)

  dist_1_ratio = dist_1.Clone()
  dist_1_ratio.Divide(dist_2)

  dist_0_ratio = dist_0.Clone()
  dist_0_ratio.Divide(dist_2)

  c = ROOT.TCanvas('c','c',600,600)

  pad1 = ROOT.TPad("pad1","pad1",0,0.3,1,1)
  pad1.SetBottomMargin(0.03)
  #pad1.SetLogx()
  pad1.Draw()
  pad1.cd()

  dist_2.Draw("BAR")
  dist_2.SetStats(0)
  dist_2.SetFillColor(38)
  dist_2.GetXaxis().SetTitle(x_label)
  dist_2.GetYaxis().SetTitle('Events')
  dist_2.GetYaxis().SetTitleOffset(1.2)
  dist_2.GetYaxis().SetTitleSize(0.04)
  dist_2.GetXaxis().SetLabelSize(0)

  dist_1.Draw("E SAME")
  dist_1.SetMarkerColor(1)
  dist_1.SetLineColor(1)
  dist_1.SetMarkerStyle(19)

  dist_0.Draw("E SAME")
  dist_0.SetMarkerColor(2)
  dist_0.SetLineColor(2)
  dist_0.SetMarkerStyle(19)


  l = ROOT.TLegend(0.65,0.75,0.9,0.9);
  l.AddEntry(dist_0,dist_0_name,"lep")
  l.AddEntry(dist_1,dist_1_name,"lep")
  l.AddEntry(dist_2,dist_2_name,"f");
  l.Draw()

  c.cd()
  pad2 = ROOT.TPad("pad2","pad2",0,0,1,0.28)
  pad2.SetTopMargin(0)
  pad2.SetBottomMargin(0.2)
  #pad2.SetLogx()
  pad2.Draw()
  pad2.cd()

  dist_0_ratio.Draw("E")
  dist_0_ratio.SetMarkerColor(2)
  dist_0_ratio.SetLineColor(2)
  dist_0_ratio.SetMarkerStyle(19)

  dist_1_ratio.Draw("E")
  dist_1_ratio.SetMarkerColor(1)
  dist_1_ratio.SetLineColor(1)
  dist_1_ratio.SetMarkerStyle(19)


  ratio_line = ROOT.TLine(dist_2.GetBinLowEdge(1),1,dist_2.GetBinLowEdge(dist_2.GetNbinsX()+1),1)
  ratio_line.SetLineStyle(2)
  # Change to just line
  #dist_2_ratio.SetFillColor(16)
  dist_2_ratio.SetAxisRange(0.6,1.4,'Y')
  dist_2_ratio.SetStats(0)
  dist_2_ratio.GetXaxis().SetLabelSize(0.08)
  dist_2_ratio.GetYaxis().SetLabelSize(0.08)
  dist_2_ratio.GetXaxis().SetTitle(x_label)
  dist_2_ratio.GetYaxis().SetTitle("Obs/Pred")
  dist_2_ratio.GetYaxis().SetTitleSize(0.1)
  dist_2_ratio.GetYaxis().SetTitleOffset(0.4)
  dist_2_ratio.GetXaxis().SetTitleSize(0.1)
  dist_2_ratio.GetXaxis().SetTitleOffset(0.9)
  #dist_2_ratio.GetXaxis().SetMoreLogLabels()
  #dist_2_ratio.GetXaxis().SetNoExponent()

  dist_2_ratio.Draw("E2")
  dist_1_ratio.Draw("E same")
  dist_1_ratio.SetMarkerColor(1)
  dist_0_ratio.Draw("E same")
  dist_0_ratio.SetMarkerColor(2)
  dist_2_ratio.SetFillColorAlpha(16,0.8)
  ratio_line.Draw("l same")
  #dist_1_ratio.SetLineColor(1)
  #dist_1_ratio.SetMarkerStyle(19)

  c.Update()
  name = '%(output_folder)s/%(var)s_%(save_name)s_dist.pdf' % vars()
  c.Print(name)
  c.Close()



for channel in ["et"]:
  for year in ["2018"]:
    file_ext = "_%(channel)s_%(year)s.root" % vars()
    file_name = "WJets_ff_ml"
    input_folder = "./"
    selection = "(1)"

    for var_input in variables:
      med_pass = DrawHist(var_input, "(deepTauVsJets_medium_2 == 1 && %(selection)s)" % vars(), input_folder,file_name, file_ext ,add_wt='1')
      med_fail = DrawHist(var_input, "(deepTauVsJets_medium_2 == 0 && %(selection)s)" % vars(), input_folder,file_name, file_ext ,add_wt='wt_ff_ml')
      med_fail.Print("all")
      med_fail_base = DrawHist(var_input, "(deepTauVsJets_medium_2 == 0 && %(selection)s)" % vars(), input_folder,file_name, file_ext ,add_wt='wt_ff_mssm_wjets_1')
      PlotDistributionComparison(var_input,var_input.split('[')[0],med_fail_base,"MSSM F_{F} x Fail", med_fail,"ML F_{F} x Fail",med_pass,"Pass",output_folder,'ff_ml_closure_%(channel)s_%(year)s' % vars())



