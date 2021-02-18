#for m in 3200 130 900 1800 140 400 160 2600 300 1200 180 700 1600 800 450 200 80 120 2900 600 90 1500 350 2000 100 110 1400 250 2300; do python scripts/makeMSSMpTWeightsPlots.py -m ${m}; done

import ROOT
import UserCode.ICHiggsTauTau.plotting as plotting
import argparse


parser = argparse.ArgumentParser()
parser.add_argument('--mass', '-m', help= 'Mass to draw')
args = parser.parse_args()

masses_ph_alt  = [350, 400, 450, 500, 600, 700, 800, 900]
#masses_ph_alt=[]

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


def MakePlot(m,x,extra):
    fout = ROOT.TFile('mssm_pt_weight_inputs.root')
    fout.cd()
    h_pythia = fout.Get('ref_m%(m)s' % vars())
    h_t = fout.Get('%(x)s_m%(m)s_t' % vars())
    h_b = fout.Get('%(x)s_m%(m)s_b' % vars())
    h_i = fout.Get('%(x)s_m%(m)s_i%(extra)s' % vars())

    h_t_scale_up = fout.Get('%(x)s_m%(m)s_t_scale_up' % vars())
    h_b_scale_up = fout.Get('%(x)s_m%(m)s_b_scale_up' % vars())
    h_i_scale_up = fout.Get('%(x)s_m%(m)s_i%(extra)s_scale_up' % vars())

    h_t_scale_down = fout.Get('%(x)s_m%(m)s_t_scale_down' % vars())
    h_b_scale_down = fout.Get('%(x)s_m%(m)s_b_scale_down' % vars())
    h_i_scale_down = fout.Get('%(x)s_m%(m)s_i%(extra)s_scale_down' % vars())

    h_t_hdamp_up = fout.Get('%(x)s_m%(m)s_t_hdamp_up' % vars())
    h_b_hdamp_up = fout.Get('%(x)s_m%(m)s_b_hdamp_up' % vars())
    h_i_hdamp_up = fout.Get('%(x)s_m%(m)s_i%(extra)s_hdamp_up' % vars())
    h_t_hdamp_down = fout.Get('%(x)s_m%(m)s_t_hdamp_down' % vars())
    h_b_hdamp_down = fout.Get('%(x)s_m%(m)s_b_hdamp_down' % vars())
    h_i_hdamp_down = fout.Get('%(x)s_m%(m)s_i%(extra)s_hdamp_down' % vars())


    u_t =  TotalUnc(h_t, [h_t_scale_up, h_t_scale_down, h_t_hdamp_up, h_t_hdamp_down])
    u_b =  TotalUnc(h_b, [h_b_scale_up, h_b_scale_down, h_b_hdamp_up, h_b_hdamp_down])
    u_i =  TotalUnc(h_i, [h_i_scale_up, h_i_scale_down, h_i_hdamp_up, h_i_hdamp_down])

    plotting.CompareHists([h_pythia, h_t, h_b, h_i],
             ['Reference','Top-only','Bottom-only','Interference'],
             "gg%(x)s: %(m)s GeV" % vars(),
             True,
             False,
             False,
             "-0.5,3.",
             False,
             10000,
             0,
             False,
             10000,
             0,
             'p_{T} (GeV)',
             'a.u.',
             0.1,
             False,
             'ggh_reweighting_plots/powheg_%(x)s_m%(m)s' % vars(),
             uncert_hist=[None,u_t,u_b,u_i],
             uncert_title=[None,'Top-only Unc.','Bottom-only Unc.', 'Interference Unc.'],

             ReweightPlot=True
             )
    fout.Close()
    fout.IsA().Destructor(fout)



m=args.mass

extra=''
if int(m) in masses_ph_alt: extra='_alt'

for x in ['h', 'H', 'A']:
  extra_=''
  if x == 'A': extra_ = extra 
  MakePlot(m,x,extra_)

