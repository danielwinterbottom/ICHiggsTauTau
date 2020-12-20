import ROOT
import UserCode.ICHiggsTauTau.plotting as plotting
import numpy as np
import argparse

def ScaleUncertHist(hists):
  hout = hists[0].Clone()
  for i in range(1,hout.GetNbinsX()+1):
    x=[]
    for h in hists: x.append(h.GetBinContent(i))

    umax = max(x)
    umin = min(x)
    c = (umax+umin)/2
    u = (umax-umin)/2
    hout.SetBinContent(i,c)
    hout.SetBinError(i,u)
  return hout

def PDFUncertHist(h0,hists):
  hout = h0.Clone()
  for i in range(1,hout.GetNbinsX()+1):
    x=[]
    for h in hists: x.append(h.GetBinContent(i))
    m = np.mean(x)
    u = np.sqrt(np.mean((x-m)**2)) # uncertainty is RMS
    hout.SetBinError(i,u)

  uncert = 0.
  x=[]
  for h in hists: x.append(h.Integral(2,-1))
  m = np.mean(x)
  uncert1 = np.sqrt(np.mean((x-m)**2))/h0.Integral(2,-1) # uncertainty is RMS
  x=[]
  for h in hists: x.append(h.Integral(1,1))
  m = np.mean(x)
  uncert0 = np.sqrt(np.mean((x-m)**2))/h0.Integral(1,1) # uncertainty is RMS

  return (hout, uncert0, uncert1)

parser = argparse.ArgumentParser()
parser.add_argument('--mass', '-m', help= 'Mass point to compute uncertainties for', default='125')
parser.add_argument('--year', '-y', help= 'Year to compute uncertainties for', default='2018')
args = parser.parse_args()
mass = args.mass
year = args.year

f1=ROOT.TFile('/vols/cms/dw515/Offline/output/MSSM/bbh_uncerts_%(year)s/bbh_m%(mass)s_tt_2018.root' % vars())
t1 = f1.Get('gen_ntuple')

f2=ROOT.TFile('/vols/cms/dw515/Offline/output/MSSM/bbh_uncerts_%(year)s/bbh_m%(mass)s_up_tt_2018.root' % vars())
t2 = f2.Get('gen_ntuple')
f3=ROOT.TFile('/vols/cms/dw515/Offline/output/MSSM/bbh_uncerts_%(year)s/bbh_m%(mass)s_down_tt_2018.root' % vars())
t3 = f3.Get('gen_ntuple')

h0 = ROOT.TH1D('h0','',4,0,4)
h1 = ROOT.TH1D('h1','',4,0,4)
h2 = ROOT.TH1D('h2','',4,0,4)
h3 = ROOT.TH1D('h3','',4,0,4)
h4 = ROOT.TH1D('h4','',4,0,4)
h5 = ROOT.TH1D('h5','',4,0,4)
h6 = ROOT.TH1D('h6','',4,0,4)

h0_up = ROOT.TH1D('h0_up','',4,0,4)
h0_down = ROOT.TH1D('h0_down','',4,0,4)

h10 = ROOT.TH1D('h10','',4,0,4)
h11 = ROOT.TH1D('h11','',4,0,4)
h12 = ROOT.TH1D('h12','',4,0,4)
h13 = ROOT.TH1D('h13','',4,0,4)
h14 = ROOT.TH1D('h14','',4,0,4)
h15 = ROOT.TH1D('h15','',4,0,4)
h16 = ROOT.TH1D('h16','',4,0,4)
h17 = ROOT.TH1D('h17','',4,0,4)
h18 = ROOT.TH1D('h18','',4,0,4)
h19 = ROOT.TH1D('h19','',4,0,4)
h20 = ROOT.TH1D('h20','',4,0,4)
h21 = ROOT.TH1D('h21','',4,0,4)
h22 = ROOT.TH1D('h22','',4,0,4)
h23 = ROOT.TH1D('h23','',4,0,4)
h24 = ROOT.TH1D('h24','',4,0,4)
h25 = ROOT.TH1D('h25','',4,0,4)
h26 = ROOT.TH1D('h26','',4,0,4)
h27 = ROOT.TH1D('h27','',4,0,4)
h28 = ROOT.TH1D('h28','',4,0,4)
h29 = ROOT.TH1D('h29','',4,0,4)
h30 = ROOT.TH1D('h30','',4,0,4)
h31 = ROOT.TH1D('h31','',4,0,4)
h32 = ROOT.TH1D('h32','',4,0,4)
h33 = ROOT.TH1D('h33','',4,0,4)
h34 = ROOT.TH1D('h34','',4,0,4)
h35 = ROOT.TH1D('h35','',4,0,4)
h36 = ROOT.TH1D('h36','',4,0,4)
h37 = ROOT.TH1D('h37','',4,0,4)
h38 = ROOT.TH1D('h38','',4,0,4)
h39 = ROOT.TH1D('h39','',4,0,4)
h40 = ROOT.TH1D('h40','',4,0,4)

t1.Draw("n_bjets>>h0(4,0,4)","wt",'goff')
h0 = t1.GetHistogram()
t1.Draw("n_bjets>>h1(4,0,4)","wt*wt_mur2_muf2",'goff')
h1 = t1.GetHistogram()
t1.Draw("n_bjets>>h2(4,0,4)","wt*wt_mur1_muf2",'goff')
h2 = t1.GetHistogram()
t1.Draw("n_bjets>>h3(4,0,4)","wt*wt_mur2_muf1",'goff')
h3 = t1.GetHistogram()

t1.Draw("n_bjets>>h4(4,0,4)","wt*wt_mur0p5_muf0p5",'goff')
h4 = t1.GetHistogram()
t1.Draw("n_bjets>>h5(4,0,4)","wt*wt_mur1_muf0p5",'goff')
h5 = t1.GetHistogram()
t1.Draw("n_bjets>>h6(4,0,4)","wt*wt_mur0p5_muf1",'goff')
h6 = t1.GetHistogram()

t1.Draw("n_bjets>>h10(4,0,4)","wt*wt_pdf_0",'goff')
h10 = t1.GetHistogram()
t1.Draw("n_bjets>>h11(4,0,4)","wt*wt_pdf_1",'goff')
h11 = t1.GetHistogram()
t1.Draw("n_bjets>>h12(4,0,4)","wt*wt_pdf_2",'goff')
h12 = t1.GetHistogram()
t1.Draw("n_bjets>>h13(4,0,4)","wt*wt_pdf_3",'goff')
h13 = t1.GetHistogram()
t1.Draw("n_bjets>>h14(4,0,4)","wt*wt_pdf_4",'goff')
h14 = t1.GetHistogram()
t1.Draw("n_bjets>>h15(4,0,4)","wt*wt_pdf_5",'goff')
h15 = t1.GetHistogram()
t1.Draw("n_bjets>>h16(4,0,4)","wt*wt_pdf_6",'goff')
h16 = t1.GetHistogram()
t1.Draw("n_bjets>>h17(4,0,4)","wt*wt_pdf_7",'goff')
h17 = t1.GetHistogram()
t1.Draw("n_bjets>>h18(4,0,4)","wt*wt_pdf_8",'goff')
h18 = t1.GetHistogram()
t1.Draw("n_bjets>>h19(4,0,4)","wt*wt_pdf_9",'goff')
h19 = t1.GetHistogram()
t1.Draw("n_bjets>>h20(4,0,4)","wt*wt_pdf_10",'goff')
h20 = t1.GetHistogram()
t1.Draw("n_bjets>>h21(4,0,4)","wt*wt_pdf_11",'goff')
h21 = t1.GetHistogram()
t1.Draw("n_bjets>>h22(4,0,4)","wt*wt_pdf_12",'goff')
h22 = t1.GetHistogram()
t1.Draw("n_bjets>>h23(4,0,4)","wt*wt_pdf_13",'goff')
h23 = t1.GetHistogram()
t1.Draw("n_bjets>>h24(4,0,4)","wt*wt_pdf_14",'goff')
h24 = t1.GetHistogram()
t1.Draw("n_bjets>>h25(4,0,4)","wt*wt_pdf_15",'goff')
h25 = t1.GetHistogram()
t1.Draw("n_bjets>>h26(4,0,4)","wt*wt_pdf_16",'goff')
h26 = t1.GetHistogram()
t1.Draw("n_bjets>>h27(4,0,4)","wt*wt_pdf_17",'goff')
h27 = t1.GetHistogram()
t1.Draw("n_bjets>>h28(4,0,4)","wt*wt_pdf_18",'goff')
h28 = t1.GetHistogram()
t1.Draw("n_bjets>>h29(4,0,4)","wt*wt_pdf_19",'goff')
h29 = t1.GetHistogram()
t1.Draw("n_bjets>>h30(4,0,4)","wt*wt_pdf_20",'goff')
h30 = t1.GetHistogram()
t1.Draw("n_bjets>>h31(4,0,4)","wt*wt_pdf_21",'goff')
h31 = t1.GetHistogram()
t1.Draw("n_bjets>>h32(4,0,4)","wt*wt_pdf_22",'goff')
h32 = t1.GetHistogram()
t1.Draw("n_bjets>>h33(4,0,4)","wt*wt_pdf_23",'goff')
h33 = t1.GetHistogram()
t1.Draw("n_bjets>>h34(4,0,4)","wt*wt_pdf_24",'goff')
h34 = t1.GetHistogram()
t1.Draw("n_bjets>>h35(4,0,4)","wt*wt_pdf_25",'goff')
h35 = t1.GetHistogram()
t1.Draw("n_bjets>>h36(4,0,4)","wt*wt_pdf_26",'goff')
h36 = t1.GetHistogram()
t1.Draw("n_bjets>>h37(4,0,4)","wt*wt_pdf_27",'goff')
h37 = t1.GetHistogram()
t1.Draw("n_bjets>>h38(4,0,4)","wt*wt_pdf_28",'goff')
h38 = t1.GetHistogram()
t1.Draw("n_bjets>>h39(4,0,4)","wt*wt_pdf_29",'goff')
h39 = t1.GetHistogram()
t1.Draw("n_bjets>>h40(4,0,4)","wt*wt_pdf_30",'goff')
h40 = t1.GetHistogram()

hists_pdf=[h10,h11,h12,h13,h14,h15,h16,h17,h18,h19,h20,h21,h22,h23,h24,h25,h26,h27,h28,h29,h30,h31,h32,h33,h34,h35,h36,h37,h38,h39,h40]

for h in hists_pdf: h.Scale(1./h.Integral(-1,-1))

h0.Scale(1./h0.Integral(-1,-1))

titles = ['nominal',
  '(#mu_{R},#mu_{F})=(2,2)',
  '(#mu_{R},#mu_{F})=(1,2)',
  '(#mu_{R},#mu_{F})=(2,1)',
  '(#mu_{R},#mu_{F})=(0.5,0.5)',
  '(#mu_{R},#mu_{F})=(1,0.5)',
  '(#mu_{R},#mu_{F})=(0.5,1)',
]

ninc = h0.Integral(-1,-1) 
n0 = h0.Integral(1,1) 
n1 = h0.Integral(2,-1) 

hists = [h0,h1,h2,h3,h4,h5,h6]

maxinc = ninc
max0 = n0
max1 = n1

mininc = ninc
min0 = n0
min1 = n1

for h in hists:
  h.Scale(1./h.Integral(-1,-1))
  ninc_ = h.Integral(-1,-1) 
  n0_ = h.Integral(1,1)     
  n1_ = h.Integral(2,-1)

  if ninc_>maxinc: maxinc=ninc_
  if n0_>max0:     max0=n0_
  if n1_>max1:     max1=n1_

  if ninc_<mininc: mininc=ninc_
  if n0_<min0:     min0=n0_
  if n1_<min1:     min1=n1_


scale_uncert1_up = max1-n1
scale_uncert1_down = min1-n1 

scale_uncert0_up = -scale_uncert1_up
scale_uncert0_down = -scale_uncert1_down

scale_uncert1_up/=n1
scale_uncert1_down/=n1

scale_uncert0_up/=n0
scale_uncert0_down/=n0

scale_uncert_hist = ScaleUncertHist(hists)

plotting.CompareHists([h0],
         ['nominal'],
         "bbH %(mass)s GeV" % vars(),
         True,
         False,
         False,
         "0.9,1.1",
         False,
         10000,
         0,
         False,
         10000,
         0,
         'N_{bjets}',
         'a.u.',
         0,
         False,
         'bbh_uncerts/bbH_QCDscale_uncerts_m%(mass)s_%(year)s' % vars(),
         uncert_hist=scale_uncert_hist,
         uncert_title='(#mu_{R},#mu_{F}) uncertainty')

#pdf uncerts
(pdf_uncert_hist,pdf_uncert0, pdf_uncert1) = PDFUncertHist(h0,hists)

plotting.CompareHists([h0],
         ['nominal'],
         "bbH %(mass)s GeV" % vars(),
         True,
         False,
         False,
         "0.9,1.1",
         False,
         10000,
         0,
         False,
         10000,
         0,
         'N_{bjets}',
         'a.u.',
         0,
         False,
         'bbh_uncerts/bbH_PDF_uncerts_m%(mass)s_%(year)s' % vars(),
         uncert_hist=pdf_uncert_hist,
         uncert_title='PDF uncertainty')

# hdamp uncerts

t2.Draw("n_bjets>>h0_up(4,0,4)","wt",'goff')
h0_up = t2.GetHistogram()

t3.Draw("n_bjets>>h0_down(4,0,4)","wt",'goff')
h0_down = t3.GetHistogram()

h0.Scale(1./h0.Integral(-1,-1))
h0_up.Scale(1./h0_up.Integral(-1,-1))
h0_down.Scale(1./h0_down.Integral(-1,-1))


ps_uncert0 = (max(h0.Integral(1,1), h0_up.Integral(1,1), h0_down.Integral(1,1)) - min(h0.Integral(1,1), h0_up.Integral(1,1), h0_down.Integral(1,1)))/2
ps_uncert1 = (max(h0.Integral(2,-1), h0_up.Integral(2,-1), h0_down.Integral(2,-1)) - min(h0.Integral(2,-1), h0_up.Integral(2,-1), h0_down.Integral(2,-1)))/2
ps_uncert1_up = (max(h0.Integral(2,-1), h0_up.Integral(2,-1), h0_down.Integral(2,-1)) - h0.Integral(2,-1))
ps_uncert1_down = (min(h0.Integral(2,-1), h0_up.Integral(2,-1), h0_down.Integral(2,-1)) - h0.Integral(2,-1))

ps_uncert0_up = -(ps_uncert1_up)/h0.Integral(1,1)
ps_uncert0_down = -(ps_uncert1_down)/h0.Integral(1,1)

ps_uncert1_up/=h0.Integral(2,-1)
ps_uncert1_down/=h0.Integral(2,-1)

hdamp_uncert_hist = ScaleUncertHist([h0, h0_up, h0_down])

plotting.CompareHists([h0],
         ['nominal'],
         "bbH %(mass)s GeV" % vars(), 
         True,
         False,
         False,
         "0.95,1.05",
         False,
         10000,
         0,
         False,
         10000,
         0,
         'N_{bjets}',
         'a.u.',
         0,
         False,
         'bbh_uncerts/bbH_hdamp_uncerts_m%(mass)s_%(year)s' % vars(),
         uncert_hist=hdamp_uncert_hist,
         uncert_title='hdamp uncertainty')

print 'Uncertainties for %(year)s M=%(mass)s:' % vars()

print 'hdamp uncerts (0, >0) = ((+) %.3f (-) %.4f , (+) %.3f (-) %.3f)' % (1.+ps_uncert0_up, 1.+ps_uncert0_down, 1.+ps_uncert1_up, 1.+ps_uncert1_down)
print 'QCD-scale uncerts (0, >0) = ((+) %.3f (-) %.3f , (+) %.3f (-) %.3f)' % (1.+scale_uncert0_up, 1.+scale_uncert0_down, 1.+scale_uncert1_up, 1.+scale_uncert1_down)
print 'QCD-scale + hdamp uncerts (0, >0) = ((+) %.3f (-) %.3f , (+) %.3f (-) %.3f)' % (1.+scale_uncert0_up+ps_uncert0_up, 1.+scale_uncert0_down+ps_uncert0_down, 1.+scale_uncert1_up+scale_uncert1_up, 1.+scale_uncert1_down+scale_uncert1_down)
print 'PDF uncerts (0, >0) = (%.3f, %.3f)' % (1.-pdf_uncert0, 1+pdf_uncert1)
