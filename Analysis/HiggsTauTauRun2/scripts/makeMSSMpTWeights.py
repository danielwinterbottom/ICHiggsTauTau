import ROOT
from array import array
import math
from collections import OrderedDict
import UserCode.ICHiggsTauTau.plotting as plotting

dir_2016 = '/vols/cms/dw515/Offline/output/MSSM/ggh_reweighting_2016/'
dir_2018 = '/vols/cms/dw515/Offline/output/MSSM/ggh_reweighting_2018/'

masses_ph     = [60, 80, 100, 120, 125, 130, 140, 160, 180, 200, 250, 300, 350, 400, 450, 500, 600, 700, 800, 900, 1000, 1200, 1400, 1600, 1800, 2000, 2300, 2600, 2900, 3200, 3500]

masses_ph_alt  = [350, 400, 450, 500, 600, 700, 800, 900]

fout = ROOT.TFile('mssm_pt_weight_inputs.root', 'RECREATE')

bins = array('d',[0,10,20,30,40,50,60,70,80,90,100,120,140,160,180,200,250,300,400,500])
cross_sections = OrderedDict()

alpha=math.pi/4
beta = math.atan(50.)
Y_H_t = math.sin(alpha)/math.sin(beta)
Y_H_b = math.cos(alpha)/math.cos(beta)

beta = math.atan(15.)
Y_h_t = math.cos(alpha)/math.sin(beta)
Y_h_b = -math.sin(alpha)/math.cos(beta)
Y_A_t = 1./math.tan(beta)
Y_A_b = math.tan(beta)

def GetPythiaHist(file_name, hist_name):
  ROOT.TH1.AddDirectory(True)
  f = ROOT.TFile(file_name)
  t = f.Get('gen_ntuple') 
  h = ROOT.TH1D(hist_name,'',len(bins)-1, bins)
  t.Draw('HpT>>%(hist_name)s' % vars(), 'wt*1.0001','goff')
  h = t.GetHistogram()
  h.Scale(1./h.Integral(-1,-1))
  h.SetDirectory(0)
  ROOT.TH1.AddDirectory(False)
  f.Close()
  return h

def GetPowhegHists(file_name, hist_name, contrib, DoUncerts=True):
  hists = []
  ROOT.TH1.AddDirectory(True)
  f = ROOT.TFile(file_name)
  t = f.Get('gen_ntuple')
  nevents = t.GetEntries('wt>0') - t.GetEntries('wt<0')

  if DoUncerts: uncerts = ['','_scale_up','_scale_down']
  else: uncerts = ['']

  for u in uncerts:

    if contrib == 'tb': weight = '(wt_X_tb-wt_X_t-wt_X_b)' % vars()
    else: weight = 'wt_X_%(contrib)s' % vars()
    if 'up' in u:   weight+='*min(max(wt_mur2_muf2,-10.),10.)'
    if 'down' in u: weight+='*min(max(wt_mur0p5_muf0p5,-10.),10.)'

    #if 'up' in u:   weight+='*wt_mur2_muf2'
    #if 'down' in u: weight+='*wt_mur0p5_muf0p5'

    h_h = ROOT.TH1D('h_'+hist_name+u,'',len(bins)-1, bins)
    h_H = ROOT.TH1D('H_'+hist_name+u,'',len(bins)-1, bins)
    h_A = ROOT.TH1D('A_'+hist_name+u,'',len(bins)-1, bins)

    t.Draw('HpT>>h_%(hist_name)s%(u)s' % vars(), 'wt_lhe_nominal*'+weight.replace('X','h'),'goff')
    h_h = t.GetHistogram()
    hists.append(h_h.Clone())

    t.Draw('HpT>>H_%(hist_name)s%(u)s' % vars(), 'wt_lhe_nominal*'+weight.replace('X','H'),'goff')
    h_H = t.GetHistogram()
    hists.append(h_H.Clone())

    t.Draw('HpT>>A_%(hist_name)s%(u)s' % vars(), 'wt_lhe_nominal*'+weight.replace('X','A'),'goff')
    h_A = t.GetHistogram()
    hists.append(h_A.Clone())

  for h in hists:
    h.Scale(1./nevents)
    if 'scale' in h.GetName() or 'hdamp' in h.GetName():
      h0 = fout.Get(str(h.GetName()).replace('scale_up','unscaled').replace('scale_down','unscaled').replace('hdamp_up','unscaled').replace('hdamp_down','unscaled'))
      h = FixBins(h,h0)
    name = h.GetName()
    xs = h.Integral(-1,-1) 
    cross_sections[name] = xs
    fout.cd()
    h.Write(h.GetName()+'_unscaled') 
    h.Scale(1./h.Integral(-1,-1))
    h.SetDirectory(0)

  ROOT.TH1.AddDirectory(False)
  f.Close()
  return hists

def TotalUnc(h0, hists=[]):
  hout = h0.Clone()
  hout.SetName(h0.GetName()+'_uncerts_total')
  for i in range(1,h0.GetNbinsX()+1):
    x0 = h0.GetBinContent(i)
    uncerts_up = [0.]
    uncerts_down = [0.]
    for h in hists:
      x = h.GetBinContent(i)
      if x>x0: uncerts_up.append(x-x0)
      if x<x0: uncerts_down.append(x0-x)
    up = 0.
    down = 0.
    for u in uncerts_up: up+=u**2
    for u in uncerts_down: down+=u**2
    up = up**.5
    down = down**.5

    c = (x0+up + x0-down)/2
    u = (up+down)/2
    hout.SetBinContent(i,c)
    hout.SetBinError(i,u)
  return hout

def FixBins(h,h0):
  # this function is used to fix bins for uncertainty variations which are very large/small due to an extreme ststistical uncertainty
  # such bins are set equal to the nominal histogram 
  for i in range(1,h.GetNbinsX()+1):
    c_old = h.GetBinContent(i)
    e_old = h.GetBinError(i)
    c = h0.GetBinContent(i)
    e = h0.GetBinError(i)
    if (abs(c_old/c)>2. or abs(c_old/c)<0.5) and abs(e_old/c_old) > 1.:
    #if abs(e_old/c_old) > 1.:
      h.SetBinContent(i,c)
      h.SetBinError(i,c)
  return h

for m in set(masses_ph):
  file_name_2018 = '%(dir_2018)s/SUSYGluGluToHToTauTau_M-%(m)s_powheg_tt_2018And2017.root' % vars()
  hist_name_2018 = 'ref_m%(m)i' % vars()
  h1 = GetPythiaHist(file_name_2018, hist_name_2018)
  fout.cd()
  h1.Write()

for m in set(masses_ph):

  file_name_2016 = '%(dir_2016)s/SUSYGluGluToHToTauTau_M-%(m)i_powheg_tt_2016.root' % vars()
  hist_name_2016 = 'ref_m%(m)i_2016' % vars()
  h2 = GetPythiaHist(file_name_2016, hist_name_2016)
  fout.cd()
  h2.Write()

for m in masses_ph:

  for c in ['t', 'b', 'tb']:

    file_name_2018 = '%(dir_2018)s/ggh_m%(m)i_%(c)s_tt_2018.root' % vars()
    if c == 'tb': hist_name_2018 = 'm%(m)i_i' % vars()
    else: hist_name_2018 = 'm%(m)i_%(c)s' % vars()
    hists = GetPowhegHists(file_name_2018, hist_name_2018, c)
    fout.cd()
    for h in hists: h.Write()

    file_name_2018 = '%(dir_2018)s/ggh_m%(m)i_%(c)s_up_tt_2018.root' % vars()
    if c == 'tb': hist_name_2018 = 'm%(m)i_i_hdamp_up' % vars()
    else: hist_name_2018 = 'm%(m)i_%(c)s_hdamp_up' % vars()
    hists = GetPowhegHists(file_name_2018, hist_name_2018, c, False)
    fout.cd()
    for h in hists: h.Write()

    file_name_2018 = '%(dir_2018)s/ggh_m%(m)i_%(c)s_down_tt_2018.root' % vars()
    if c == 'tb': hist_name_2018 = 'm%(m)i_i_hdamp_down' % vars()
    else: hist_name_2018 = 'm%(m)i_%(c)s_hdamp_down' % vars()
    hists = GetPowhegHists(file_name_2018, hist_name_2018, c, False)
    fout.cd()
    for h in hists: h.Write()


for m in masses_ph_alt:
  c = 'tb'

  file_name_2018 = '%(dir_2018)s/ggh_m%(m)i_%(c)s_alt_tt_2018.root' % vars()
  hist_name_2018 = 'm%(m)i_i_alt' % vars()
  hists = GetPowhegHists(file_name_2018, hist_name_2018, c)
  fout.cd()
  for h in hists: h.Write()

  file_name_2018 = '%(dir_2018)s/ggh_m%(m)i_%(c)s_alt_up_tt_2018.root' % vars()
  hist_name_2018 = 'm%(m)i_i_alt_hdamp_up' % vars()
  hists = GetPowhegHists(file_name_2018, hist_name_2018, c, False)
  fout.cd()
  for h in hists: h.Write()

  file_name_2018 = '%(dir_2018)s/ggh_m%(m)i_%(c)s_alt_down_tt_2018.root' % vars()
  hist_name_2018 = 'm%(m)i_i_alt_hdamp_down' % vars()
  hists = GetPowhegHists(file_name_2018, hist_name_2018, c, False)
  fout.cd()
  for h in hists: h.Write()


out=''
for name in cross_sections:
  xs = cross_sections[name]
  scale = 1.
  if name.startswith('A'):
    if '_t_' in name or name.endswith('_t'): scale = 1./(Y_A_t**2)
    if '_b_' in name or name.endswith('_b'): scale = 1./(Y_A_b**2)
    if '_i_' in name or name.endswith('_i'): scale = 1./(Y_A_t*Y_A_b)
  if name.startswith('H'):
    if '_t_' in name or name.endswith('_t'): scale = 1./(Y_H_t**2)
    if '_b_' in name or name.endswith('_b'): scale = 1./(Y_H_b**2)
    if '_i_' in name or name.endswith('_i'): scale = 1./(Y_H_t*Y_H_b)
  if name.startswith('h'):
    if '_t_' in name or name.endswith('_t'): scale = 1./(Y_h_t**2)
    if '_b_' in name or name.endswith('_b'): scale = 1./(Y_h_b**2)
    if '_i_' in name or name.endswith('_i'): scale = 1./(Y_h_t*Y_h_b)

  scaled_xs = xs*scale

  out+='%(name)s  %(scaled_xs)s  %(xs)s  %(scale)s\n' % vars()

print out

with open("mssm_pt_weight_input_xs.txt", "w") as out_file:
    out_file.write(out)

fout.Close()

exit()

# checks:
#1. the h i template for 125 GeV has +ve cross section when you expect negative interference in SM - this could be because Yb*Yt for generated sample is also negative - check!! 
