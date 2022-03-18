import ROOT
import json
from array import array

wp = 'medium'
alt_wp = 'vvvloose'
#years = ['2016','2017','2018']
#channels = ['mt','et']
years = ['2018']
channels = ['et']
output_folder = '.'
variables = {   #'newmet*cos(newmet_dphi_2)/pt_2[-10,-7,-5,-3.500,-3.275,-3.050,-2.825,-2.600,-2.375,-2.150,-1.925,-1.700,-1.475,-1.250,-1.025,-0.800,-0.575,-0.350,-0.125,0.100,0.325,0.550,0.775,1.000,2,4,6,10]:met[10,20,30,40,50,60,70,80,90,100,120,140,200,400]'
                #'mt_tot[20,30,40,50,60,70,80,90,100,110,125,140,160,175,200,225,250,280,320,350,400,450,500,560,630,710,800,890,1000]':'m_{T}^{tot}/GeV',
                #'m_vis[20,30,40,50,60,70,80,90,100,110,125,140,160,175,200,225,250,280,320,350,400,450,500,560,630,710,800,890,1000]':'m_{vis}/GeV',
                #'pt_1[20,25,30,35,40,45,50,55,60,65,70,80,90,100,120,140,200,400]':'p_{T}^{#mu}/GeV',
                'pt_2[30,35,40,45,50,55,60,65,70,80,90,100,120,140,200,400]':'p_{T}^{#tau_{h}}/GeV',
                #'met[10,20,30,40,50,60,70,80,90,100,120,140,200,400]': 'MET/GeV'
             }

def ZeroNegativeBins(h):
  # if a bin of h1 is equal to 0 ir negative then remove it
  # also remove non zero bins with > 100% errors
  for i in range(1,h.GetNbinsX()+1):
    if h.GetBinContent(i) <= 0:
      h.SetBinContent(i,0)
      h.SetBinError(i,0)
  return h

def DrawHists(var_input, cuts, name, input_folder, file_ext ,add_wt='1',doMC=True,doTT=True,fullMT=False,lowMT=False):
  add_wt = "(" + add_wt + "*wt_tau_trg_mssm*wt_tau_id_mssm)"
  if ':' in var_input:
    #pass 2D inputs like x[],y[]
    var_input1 = var_input.split(':')[0]
    var_input2 = var_input.split(':')[1]
    var1 = var_input1.split('[')[0]
    var2 = var_input2.split('[')[0]

    bins1 = array('f', map(float,var_input1.split('[')[1].split(']')[0].split(',')))
    bins2 = array('f', map(float,var_input2.split('[')[1].split(']')[0].split(',')))
    hout = ROOT.TH2D('hout','',len(bins1)-1, bins1,len(bins2)-1, bins2)
    hout.GetXaxis().SetTitle(var1)
    hout.GetYaxis().SetTitle(var2)
    var='%(var2)s:%(var1)s' % vars()
  else:
    var = var_input.split('[')[0]
    bins = array('f', map(float,var_input.split('[')[1].split(']')[0].split(',')))
    hout = ROOT.TH1D('hout','',len(bins)-1, bins)
  gen_extra='(gen_match_2==6)'

  wjets = hout.Clone()
  wjets.SetName(name+'_wjets_mc')
  ttbar = hout.Clone()
  ttbar.SetName(name+'_ttbar_mc')


  if fullMT: mt_cut='1'
  elif lowMT: mt_cut='mt_1<70'
  else: mt_cut='mt_1>70'

  if doMC:
    # draw wjets when tau candidate is a jet fake
    for i in wjets_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s)*((os==1)))*(%(mt_cut)s)*(%(gen_extra)s)*(%(add_wt)s)' % vars(),'goff')
      h = t.GetHistogram()
      #scale = lumi*params[i]['xs']/params[i]['evt']
      #h.Scale(scale)
      wjets.Add(h)

    wjets = ZeroNegativeBins(wjets)

  if doTT:
    # draw ttbar when tau candidate is a jet fake 
    for i in ttbar_files:
      f = ROOT.TFile('%(input_folder)s/%(i)s%(file_ext)s' % vars())
      t = f.Get('ntuple')
      h = hout.Clone()
      h.SetName('h')
      t.Draw('%(var)s>>h' % vars(),'wt*((%(cuts)s))*(os==1)*(mt_1<70)*(%(gen_extra)s)*(%(add_wt)s)' % vars(),'goff')
      h = t.GetHistogram()
      scale = lumi*params[i]['xs']/params[i]['evt']
      h.Scale(scale)
      ttbar.Add(h)

    ttbar = ZeroNegativeBins(ttbar)

  return (wjets, ttbar)

def PlotDistributionComparison(var,x_label,dist_1,dist_1_name,dist_2,dist_2_name,output_folder,save_name):
  # ratio is dist_1/dist_2
  bins = array('f', map(float,var.split('[')[1].split(']')[0].split(',')))
  var = var.split('[')[0]

  dist_2_ratio = dist_2.Clone()
  dist_2_ratio.Divide(dist_2)

  dist_1_ratio = dist_1.Clone()
  dist_1_ratio.Divide(dist_2)

  c = ROOT.TCanvas('c','c',600,600)

  pad1 = ROOT.TPad("pad1","pad1",0,0.3,1,1)
  pad1.SetBottomMargin(0.03)
  pad1.SetLogx()
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

  l = ROOT.TLegend(0.65,0.75,0.9,0.9);
  l.AddEntry(dist_1,dist_1_name,"lep")
  l.AddEntry(dist_2,dist_2_name,"f");
  l.Draw()

  c.cd()
  pad2 = ROOT.TPad("pad2","pad2",0,0,1,0.28)
  pad2.SetTopMargin(0)
  pad2.SetBottomMargin(0.2)
  pad2.SetLogx()
  pad2.Draw()
  pad2.cd()

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
  dist_2_ratio.GetXaxis().SetMoreLogLabels()
  dist_2_ratio.GetXaxis().SetNoExponent()

  dist_2_ratio.Draw("E2")
  dist_1_ratio.Draw("E same")
  dist_1_ratio.SetMarkerColor(1)
  dist_2_ratio.SetFillColorAlpha(16,0.8)
  ratio_line.Draw("l same")
  #dist_1_ratio.SetLineColor(1)
  #dist_1_ratio.SetMarkerStyle(19)

  c.Update()
  name = '%(output_folder)s/%(var)s_%(save_name)s_dist.pdf' % vars()
  c.Print(name)
  c.Close()

json_name = 'scripts/ff_strings.json'
with open(json_name) as json_file:
  ff_dict = json.load(json_file)



for year in years:
  for channel in channels:
    for var_input,x_label in variables.items():
#    for var_input in variables:

      if year == '2018':
        lumi = 58826.8469
        params_file = 'scripts/params_mssm_2018.json'
        #input_folder = '/vols/cms/gu18/Offline/output/MSSM/mssm_2018_v8/'
        input_folder='./'

        ttbar_files = [
          'TTTo2L2Nu',
          'TTToHadronic',
          'TTToSemiLeptonic'
        ]

        wjets_files = [
          'WJets_ff_ml',
          #'W1JetsToLNu-LO',
          #'W2JetsToLNu-LO',
          #'W3JetsToLNu-LO',
          #'W4JetsToLNu-LO',
          #'WJetsToLNu-LO',
          #'EWKWMinus2Jets',
          #'EWKWPlus2Jets'
        ]

      elif year == "2017":
        lumi = 41530.
        params_file = 'scripts/params_mssm_2017.json'
        input_folder = '/vols/cms/gu18/Offline/output/MSSM/mssm_2017_v6/'

        ttbar_files = [
          'TTTo2L2Nu', 
          'TTToHadronic', 
          'TTToSemiLeptonic'
        ] 
  
        wjets_files = [
          'WJetsToLNu-LO',
          'WJetsToLNu-LO-ext',
          'W1JetsToLNu-LO',
          'W2JetsToLNu-LO',
          'W3JetsToLNu-LO',
          'W4JetsToLNu-LO',
          'EWKWMinus2Jets',
          'EWKWPlus2Jets'
        ]


      elif year == "2016":
        lumi = 35920.
        params_file = 'scripts/params_mssm_2016.json'
        input_folder = '/vols/cms/gu18/Offline/output/MSSM/mssm_2016_v6/'

        ttbar_files = [
          'TT'
        ]

        wjets_files = [
          'WJetsToLNu-LO', 
          'WJetsToLNu-LO-ext',
          'W1JetsToLNu-LO',
          'W2JetsToLNu-LO',
          'W2JetsToLNu-LO-ext',
          'W3JetsToLNu-LO',
          'W3JetsToLNu-LO-ext',
          'W4JetsToLNu-LO',
          'W4JetsToLNu-LO-ext1',
          'W4JetsToLNu-LO-ext2', 
          'EWKWMinus2Jets_WToLNu',
          'EWKWMinus2Jets_WToLNu-ext1',
          'EWKWMinus2Jets_WToLNu-ext2',
          'EWKWPlus2Jets_WToLNu',
          'EWKWPlus2Jets_WToLNu-ext1',
          'EWKWPlus2Jets_WToLNu-ext2'
        ]


      file_ext = '_%(channel)s_%(year)s.root' % vars()

      # read params from json
      with open(params_file) as jsonfile:
        params = json.load(jsonfile)

      if year == "2016":
        m_lowpt=23
        e_lowpt=26
        t_highpt=120
        t_lowpt_mt=25
        t_lowpt_et=25
      elif year == "2017":
        m_lowpt=25
        e_lowpt=28
        t_highpt=180
        t_lowpt_mt=32
        t_lowpt_et=35
      elif year == "2018":
        m_lowpt=25
        e_lowpt=33
        t_highpt=180
        t_lowpt_mt=32
        t_lowpt_et=35

      if channel == 'mt':
        baseline_iso_pass = '(iso_1<0.15 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && !leptonveto && pt_2>30 && ((trg_mutaucross&&pt_2>%(t_lowpt_mt)s&&pt_2<%(t_highpt)s&&fabs(eta_2)<2.1&&pt_1<%(m_lowpt)s)||(trg_singlemuon&&pt_1>=%(m_lowpt)s)||(trg_singletau_2&&pt_2>=%(t_highpt)s&&fabs(eta_2)<2.1)))' % vars()
      elif channel == 'et':
        baseline_iso_pass = '(iso_1<0.15 && deepTauVsJets_%(wp)s_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && pt_2>30 && ((trg_etaucross&&pt_2>%(t_lowpt_et)s&&pt_2<%(t_highpt)s&&fabs(eta_2)<2.1&&pt_1<%(e_lowpt)s)||(trg_singleelectron&&pt_1>=%(e_lowpt)s)||(trg_singletau_2&&pt_2>=%(t_highpt)s&&fabs(eta_2)<2.1)))' % vars()
        
      baseline_iso_fail = baseline_iso_pass.replace('deepTauVsJets_%(wp)s_2>0.5' % vars(),'deepTauVsJets_%(wp)s_2<0.5 && deepTauVsJets_%(alt_wp)s_2>0.5' % vars())
   
      sel = '(1)'

      baseline_iso_pass = "deepTauVsJets_medium_2>0.5"
      baseline_iso_fail = "deepTauVsJets_medium_2<0.5"


      print baseline_iso_fail 
      # Begin making histograms
      (w_mc_dr_pass,_) =  DrawHists(var_input, '(' + sel + ' && ' + baseline_iso_pass + ')', 'w_mc_dr_pass', input_folder, file_ext,add_wt='1',doMC=True,doTT=False,fullMT=True,lowMT=False)
      w_mc_dr_pass.GetEntries()
      w_mc_dr_pass.Print("all")
      #c = ROOT.TCanvas('c','c',600,600)
      #c.SetLogy()
      #w_mc_dr_pass.SetStats(0)
      #w_mc_dr_pass.GetYaxis().SetMoreLogLabels()
      #w_mc_dr_pass.GetYaxis().SetNoExponent()
      #w_mc_dr_pass.SetMarkerStyle(7)
      #w_mc_dr_pass.Draw()
      #c.Print("scan.pdf")
      (w_mc_dr_ff_fail,_) = DrawHists(var_input, '(' + sel + ' && ' + baseline_iso_fail + ')', 'w_mc_dr_ff_fail', input_folder, file_ext,add_wt="wt_ff_mssm_wjets_1",doMC=True,doTT=False,fullMT=True,lowMT=False)
      w_mc_dr_ff_fail.Print("all")
      PlotDistributionComparison(var_input,x_label,w_mc_dr_pass,"Pass",w_mc_dr_ff_fail,"F_{F} x Fail",output_folder,'w_mc_dr_{}_{}'.format(channel,year))

      #(w_mc_ar_pass,_) =  DrawHists(var_input, baseline_iso_pass, 'w_mc_ar_pass', input_folder, file_ext,add_wt='1',doMC=True,doTT=False,fullMT=False,lowMT=True)
      #(w_mc_ar_ff_fail,_) = DrawHists(var_input, baseline_iso_fail, 'w_mc_ar_ff_fail', input_folder, file_ext,add_wt=ff_dict[channel][year]['wjets_mc'],doMC=True,doTT=False,fullMT=False,lowMT=True)
      #PlotDistributionComparison(var_input,x_label,w_mc_ar_pass,"Pass",w_mc_ar_ff_fail,"F_{F} x Fail",output_folder,'w_mc_ar_{}_{}'.format(channel,year))

      #(_,ttbar_mc_pass) =  DrawHists(var_input, '(' + sel + ' && ' + baseline_iso_pass + ')', 'ttbar_mc_pass', input_folder, file_ext,add_wt='1',doMC=False,doTT=True,fullMT=False,lowMT=False)
      #(_,ttbar_mc_ff_fail) = DrawHists(var_input, '(' + sel + ' && ' + baseline_iso_fail + ')', 'ttbar_mc_ff_fail', input_folder, file_ext,add_wt=ff_dict[channel][year]['ttbar_mc'],doMC=False,doTT=True,fullMT=False,lowMT=False)
      #PlotDistributionComparison(var_input,x_label,ttbar_mc_pass,"Pass",ttbar_mc_ff_fail,"F_{F} x Fail",output_folder,'ttbar_mc_{}_{}'.format(channel,year))


