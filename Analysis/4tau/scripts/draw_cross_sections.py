import ROOT
import json
import numpy as np
from array import array
import UserCode.ICHiggsTauTau.plotting as plotting

def InterpolationFromBinEdges(x,y,x1,x2,y1,y2,f11,f12,f21,f22):
  #fact = 1/((float(x2-x1))*(float(y2-y1)))
  #mat1 = [[float(x2-x),float(x-x1)]]
  #mat2 = [[f11,f12],[f21,f22]]
  #mat3 = [[float(y2-y)],[float(y-y1)]]

  #mat12 = [[mat1[0][0]*mat2[0][0] + mat1[0][1]*mat2[0][1],mat1[0][0]*mat2[1][0] + mat1[0][1]*mat2[1][1]]]
  #mat123 = mat12[0][0]*mat3[0][0] + mat12[0][1]*mat3[0][0]

  #return fact*mat123

  t1 = ((float(y2)-float(y))/(float(y2)-float(y1)))
  t2 = ((float(x2)-float(x))/(float(x2)-float(x1)))*f11
  t3 = ((float(x)-float(x1))/(float(x2)-float(x1)))*f21
  t4 = ((float(y)-float(y1))/(float(y2)-float(y1)))
  t5 = ((float(x2)-float(x))/(float(x2)-float(x1)))*f12
  t6 = ((float(x)-float(x1))/(float(x2)-float(x1)))*f22

  return (t1*(t2+t3)) + (t4*(t5+t6))

def find_closest(numbers, target):
  numbers_sorted = sorted(numbers)
  closest_higher = numbers[-1]
  closest_lower = None
  for number in numbers_sorted:
    if number >= target:
      closest_higher = number
      break
    closest_lower = number
  return closest_lower, closest_higher

params_file = "scripts/params_UL_2018.json"

with open(params_file) as jsonfile: 
  params = json.load(jsonfile)

bins_A = []
bins_phi = []
for key, val in params.items():
  if "Zstar" not in key: continue
  A = int(key.split("A")[1].split("To")[0])
  phi = int(key.split("phi")[1].split("A")[0])
#  print key, val["xs"]
  if A not in bins_A: bins_A.append(A)
  if phi not in bins_phi: bins_phi.append(phi)

 
bins_A = sorted(bins_A)
bins_phi = sorted(bins_phi)

bins = 1000
new_bins_A = list(np.linspace(bins_A[0],bins_A[-1],num=bins))
new_bins_phi = list(np.linspace(bins_phi[0],bins_phi[-1],num=bins))


hout = ROOT.TH2D('hout','',len(new_bins_phi)-1, array('f', map(float,new_bins_phi)),len(new_bins_A)-1, array('f', map(float,new_bins_A)))

for bphi in range(1,hout.GetNbinsX()+1):
  for bA in range(1,hout.GetNbinsY()+1):
    x = hout.GetXaxis().GetBinCenter(bphi)
    y = hout.GetYaxis().GetBinCenter(bA)
    x1, x2 = find_closest(bins_phi,x) 
    y1, y2 = find_closest(bins_A,y)

    f11 = params["ZstarTophi{}A{}To4Tau".format(x1,y1)]["xs"]
    f12 = params["ZstarTophi{}A{}To4Tau".format(x1,y2)]["xs"]
    f21 = params["ZstarTophi{}A{}To4Tau".format(x2,y1)]["xs"]
    f22 = params["ZstarTophi{}A{}To4Tau".format(x2,y2)]["xs"]

    #print x, x1, x2, y, y1, y2, f11, f12, f21, f22, InterpolationFromBinEdges(x,y,x1,x2,y1,y2,f11,f12,f21,f22)

    hout.SetBinContent(bphi,bA,1000.0*InterpolationFromBinEdges(x,y,x1,x2,y1,y2,f11,f12,f21,f22))
  

c = ROOT.TCanvas('c','c',900,700)
c.SetLeftMargin(0.15)
c.SetRightMargin(0.25)
c.SetBottomMargin(0.15)
c.SetLogz()
hout.Draw("COLZ")
hout.SetStats(0)
hout.GetZaxis().SetMoreLogLabels()
hout.GetZaxis().SetNoExponent()
hout.GetXaxis().SetTitle("m_{#phi} (GeV)")
hout.GetYaxis().SetTitle("m_{A} (GeV)")
hout.GetZaxis().SetTitle("Production Cross Section (fb)")

plotting.DrawTitle(c, "13 TeV", 3,scale=0.75)


hout.GetXaxis().SetNdivisions(5)
hout.GetYaxis().SetNdivisions(5)
hout.GetZaxis().SetNdivisions(5)


hout.GetXaxis().SetTitleSize(0.05)
hout.GetXaxis().SetTitleOffset(1.1)

hout.GetYaxis().SetTitleSize(0.05)
#hout.GetYaxis().SetTitleOffset(0.9)

hout.GetZaxis().SetTitleSize(0.05)
hout.GetZaxis().SetTitleOffset(1.2)

c.Print('cross_sections.pdf')
c.Print('cross_sections.png')
