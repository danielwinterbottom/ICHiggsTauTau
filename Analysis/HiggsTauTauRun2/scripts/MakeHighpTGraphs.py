import ROOT
from array import array
import ctypes
ROOT.gROOT.SetBatch(1)

bins=[100.,120.,140.,180.,220.,260.,500.]
bins=[100.,120.,140.,180.,220.,280.,500.]
min_pt=140

rescale=False
#rescale=True

def GetDataMCW(f):
  bkg_procs =['h_jfakes','h_lfakes']
  data=f.Get('h_data' % vars())
  for b in bkg_procs: data.Add(f.Get('%(b)s'%vars()),-1)
  bkg=f.Get('h_sig')
  bkg.Add(f.Get('h_tau'))

  return data, bkg

def GetDataMC(f, dirname):
  #bkg_procs =['TTJ','TTL','VVJ','VVL','QCD','W','ZJ','ZL']
  #bkg_procs =['TTJ','TTL','VVJ','VVL','W','ZJ','ZL']
  bkg_procs =['TTJ','VVJ','QCD','W','ZJ','ZL']
  data=f.Get('%(dirname)s/data_obs' % vars())
  for b in bkg_procs: data.Add(f.Get('%(dirname)s/%(b)s'%vars()),-1) 
  bkg=f.Get('%(dirname)s/ZTT' % vars())
  bkg.Add(f.Get('%(dirname)s/TTL' % vars()))
  bkg.Add(f.Get('%(dirname)s/VVL' % vars()))

  bin_array = array('d',bins)
  bkg=bkg.Rebin(len(bin_array)-1,'',bin_array)
  data=data.Rebin(len(bin_array)-1,'',bin_array)

  return data, bkg

def findAvepT(infile,dirname):
    '''Find the average pT of the tau in each pT bin'''
    directory = infile.Get(dirname)
    name='ZTT'
    histo = directory.Get(name).Clone()
    if isinstance(histo, ROOT.TH1):
        bin_means = []
        bin_rmss = []
        for i in range(len(bins)-1):
          histo.GetXaxis().SetRangeUser(bins[i],bins[i+1])
          #print bins[i], bins[i+1], histo.GetMean(), histo.GetRMS()
          bin_means.append(round(histo.GetMean(),1))
          bin_rmss.append(round(histo.GetRMS(),1))
        print(bin_means)
        return bin_means, bin_rmss

def Plot(f, h, name='test', output_folder='highpT_closure', title='',func=None,g=None):
  x_title='p_{T} (GeV)'
  c1 = ROOT.TCanvas()
  f.GetXaxis().SetTitle(x_title)
  f.GetYaxis().SetTitle('data/MC')
  if rescale: f.GetYaxis().SetTitle('SF/flat-SF')
  else: f.GetYaxis().SetTitle('data/MC')
  f.SetTitle(title)
  f.SetMinimum(0.4)
  f.SetMaximum(1.6)
  f.SetLineColor(ROOT.kBlack)
  f.SetMarkerColor(ROOT.kBlack)
  f.SetMarkerStyle(20)
  #c1.SetTicks(1)
  f.Draw('ape')
  h.SetStats(0)
  h.SetFillColor(ROOT.kBlue-10)
  h.SetLineColor(ROOT.kRed)
  h.SetLineWidth(1)
  h.Draw("e3 same")
  if func: func.Draw('same')
  f.Draw("pe same")
  leg=ROOT.TLegend(0.1,0.8,0.9,0.9)
  leg.SetFillStyle(0)
  leg.SetNColumns(2)
  leg.SetBorderSize(0)
  leg.AddEntry(f, 'W*#rightarrow#tau#nu','pe')
#  leg.AddEntry(h, 'W*#rightarrow#tau#nu fit','lf')
  if g is not None: 
    leg.SetNColumns(3)
    g.SetMarkerStyle(22)
    g.Draw("pe same")
    leg.AddEntry(g, 'Z#rightarrow#tau#tau','pe')
  leg.AddEntry(h, 'Fit','lf')
  leg.Draw()
  if rescale: name+='_rescaled'
  c1.Print(output_folder+'/%(name)s.pdf' % vars())

if rescale: fout = ROOT.TFile('highpt_fit_graphs_rescaled.root','RECREATE')
else: fout = ROOT.TFile('highpt_fit_graphs.root','RECREATE')
#wp_upper='Medium'
#wp_VSe_upper='VVLoose'

for wp_VSe_upper in ['VVLoose','Tight']:
  for wp_upper in ['Loose','Medium','Tight','VTight']:

    print '\n VSjet WP=%(wp_upper)s, VSele = %(wp_VSe_upper)s' % vars()

    wp=wp_upper.lower()
    wp_VSe=wp_VSe_upper.lower()

    f = ROOT.TFile('HighpTChecks_%(wp_VSe)sVSe_Apr26_finepTbins/Run2/mt/datacard_pt_2_inclusive_mTLt65_%(wp)s_highpT_mt_Run2.root' % vars())
    f2 = ROOT.TFile('HighPT_pt/pT_%(wp_upper)sVsJet_TightVsMu_%(wp_VSe_upper)sVsE.root' % vars())
    
    dirname = 'mt_inclusive_mTLt65_%(wp)s_highpT' % vars()
    
    bin_means, bin_rmss = findAvepT(f,dirname)
    
    data, mc = GetDataMC(f,dirname)
    
    data.Divide(mc)    

    g = ROOT.TGraphErrors()
    
    for i in range(0,data.GetNbinsX()):
      x = bin_means[i]
      if x< min_pt: continue 
      #x_e = bin_rmss[i]
      x_e = 0.
      y= data.GetBinContent(i+1) 
      y_e= data.GetBinError(i+1) 
    
      n = g.GetN()
      g.SetPoint(n,x,y)
      g.SetPointError(n,x_e,y_e)
       
    if rescale:
      func0 = ROOT.TF1('func0','[0]',min_pt,500)
      g.Fit(func0)
      p0=func0.GetParameter(0)
      x=ctypes.c_double(0.)
      y=ctypes.c_double(0.)
      for i in range(0,g.GetN()):
        g.GetPoint(i,x,y)
        g.SetPoint(i,x,y.value/p0)
      g.GetListOfFunctions().Remove(g.GetFunction("func0"))
 
    # now get W* graph
    
    data_w, mc_w = GetDataMCW(f2)
    
    data_w.Divide(mc_w)
    
    g2 = ROOT.TGraphErrors()
    
    for i in range(0,data_w.GetNbinsX()):
      x = data_w.GetXaxis().GetBinCenter(i+1)
      if x<min_pt: continue 
      x_e = 0.
      y= data_w.GetBinContent(i+1)
      y_e= data_w.GetBinError(i+1)
      n = g2.GetN()
 
      g2.SetPoint(n,x,y)
      g2.SetPointError(n,x_e,y_e)

    if rescale:
      func0 = ROOT.TF1('func0','[0]',min_pt,500)
      g2.Fit(func0)
      p0=func0.GetParameter(0)
      x=ctypes.c_double(0.)
      y=ctypes.c_double(0.)
      for i in range(0,g2.GetN()):
        g2.GetPoint(i,x,y)
        g2.SetPoint(i,x,y.value/p0)
      g2.GetListOfFunctions().Remove(g2.GetFunction("func0"))

    
    fout.cd()   
 
    g.SetLineColor(ROOT.kBlue)
    g.SetMarkerColor(ROOT.kBlue)
    
    g2.SetLineColor(ROOT.kRed)
    g2.SetMarkerColor(ROOT.kRed)
    
    g.Write('graph_%(wp)sVSjet_%(wp_VSe)sVSe_ZTauTau' % vars())
    g2.Write('graph_%(wp)sVSjet_%(wp_VSe)sVSe_WTauNu' % vars())
    mg=ROOT.TMultiGraph()
    mg.Add(g)
    mg.Add(g2)
    mg.Write('multigraph_%(wp)sVSjet_%(wp_VSe)sVSe' % vars())
    
    
    func0 = ROOT.TF1('func0','[0]',min_pt,500)
    func1 = ROOT.TF1('func1','[0]+[1]*(x-%s)' % min_pt,min_pt,500)
    
    mg.Fit(func0,'R')
    print 'Func0 chi2, NDF, p-value: ', func0.GetChisquare(), func0.GetNDF(), func0.GetProb()
    fix=1.
    fix=func0.GetParameter(0)
    func1.SetParameter(0,fix)
    func1.FixParameter(0,fix)
    mg.Fit(func1,'R')
    print 'Func1 chi2, NDF, p-value: ', func1.GetChisquare(), func1.GetNDF(), func1.GetProb()
    func1.Write('func_%(wp)sVSjet_%(wp_VSe)sVSe' % vars())
   
    h_uncert=ROOT.TH1D('h_uncert_%(wp)sVSjet_%(wp_VSe)sVSe' % vars(),'',10000,min_pt,496)
    ROOT.TVirtualFitter.GetFitter().GetConfidenceIntervals(h_uncert, 0.68)
    h_uncert.Write('h_uncert_%(wp)sVSjet_%(wp_VSe)sVSe' % vars())
    Plot(g2, h_uncert,name='high_pt_uncert_%(wp)sVSjet_%(wp_VSe)sVSe' % vars(), title='%(wp_upper)sVSjet,%(wp_VSe_upper)sVSe' % vars(),func=func1,g=g)
 
    m_val = abs(func1.GetParameter(1))+abs(func1.GetParError(1))
    func_uncert = ROOT.TF1('uncert_func_%(wp)sVSjet_%(wp_VSe)sVSe' % vars(),'1.+(x>=140.)*(x-%s)*%.6f' % (min_pt,m_val),100,500)
    
    func_uncert.Write('uncert_func_%(wp)sVSjet_%(wp_VSe)sVSe' % vars())
    
    print func_uncert
    
    print 'Uncert @ 140 GeV  = %.3f' % func_uncert.Eval(140.)
    print 'Uncert @ 300 GeV  = %.3f' % func_uncert.Eval(300.)
    print 'Uncert @ 500 GeV  = %.3f' % func_uncert.Eval(500.)
    print 'Uncert @ 1000 GeV = %.3f' % func_uncert.Eval(1000.)
