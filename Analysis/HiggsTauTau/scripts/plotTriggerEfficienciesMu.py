import ROOT
import math
import sys
import os
import numpy as np
import fnmatch
import json
ROOT.gROOT.SetBatch(ROOT.kTRUE)

singlemuonB = "/vols/cms/amd12/CMSSW_7_6_3/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/July08-TandP/SingleMuonB_tpzmm_2016.root"
singlemuonC = "/vols/cms/amd12/CMSSW_7_6_3/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/July08-TandP/SingleMuonC_tpzmm_2016.root"

filemuB = ROOT.TFile.Open(singlemuonB)
filemuC = ROOT.TFile.Open(singlemuonC)

treemuB = filemuB.Get("ntuple")
treemuC = filemuC.Get("ntuple")

xbins = [10.,13.,16.,19.,22.,25.,28.,31.,34.,37.,40.,45.,50.,60.,70.,100.,1000.]

mu2Beta0p9Num = ROOT.TH1D("mu2Beta0p9Num","mu2Beta0p9Num",16,np.array(xbins))
mu2Beta0p9Denom = ROOT.TH1D("mu2Beta0p9Denom","mu2Beta0p9Denom",16,np.array(xbins))
mu1Beta0p9Num = ROOT.TH1D("mu1Beta0p9Num","mu1Beta0p9Num",16,np.array(xbins))
mu1Beta0p9Denom = ROOT.TH1D("mu1Beta0p9Denom","mu1Beta0p9Denom",16,np.array(xbins))
mu2Ceta0p9Num = ROOT.TH1D("mu2Ceta0p9Num","mu2Ceta0p9Num",16,np.array(xbins))
mu2Ceta0p9Denom = ROOT.TH1D("mu2Ceta0p9Denom","mu2Ceta0p9Denom",16,np.array(xbins))
mu1Ceta0p9Num = ROOT.TH1D("mu1Ceta0p9Num","mu1Ceta0p9Num",16,np.array(xbins))
mu1Ceta0p9Denom = ROOT.TH1D("mu1Ceta0p9Denom","mu1Ceta0p9Denom",16,np.array(xbins))

mu2Beta0p9to1p2Num = ROOT.TH1D("mu2Beta0p9to1p2Num","mu2Beta0p9to1p2Num",16,np.array(xbins))
mu2Beta0p9to1p2Denom = ROOT.TH1D("mu2Beta0p9to1p2Denom","mu2Beta0p9to1p2Denom",16,np.array(xbins))
mu1Beta0p9to1p2Num = ROOT.TH1D("mu1Beta0p9to1p2Num","mu1Beta0p9to1p2Num",16,np.array(xbins))
mu1Beta0p9to1p2Denom = ROOT.TH1D("mu1Beta0p9to1p2Denom","mu1Beta0p9to1p2Denom",16,np.array(xbins))
mu2Ceta0p9to1p2Num = ROOT.TH1D("mu2Ceta0p9to1p2Num","mu2Ceta0p9to1p2Num",16,np.array(xbins))
mu2Ceta0p9to1p2Denom = ROOT.TH1D("mu2Ceta0p9to1p2Denom","mu2Ceta0p9to1p2Denom",16,np.array(xbins))
mu1Ceta0p9to1p2Num = ROOT.TH1D("mu1Ceta0p9to1p2Num","mu1Ceta0p9to1p2Num",16,np.array(xbins))
mu1Ceta0p9to1p2Denom = ROOT.TH1D("mu1Ceta0p9to1p2Denom","mu1Ceta0p9to1p2Denom",16,np.array(xbins))

mu2Beta1p2Num = ROOT.TH1D("mu2Beta1p2Num","mu2Beta1p2Num",16,np.array(xbins))
mu2Beta1p2Denom = ROOT.TH1D("mu2Beta1p2Denom","mu2Beta1p2Denom",16,np.array(xbins))
mu1Beta1p2Num = ROOT.TH1D("mu1Beta1p2Num","mu1Beta1p2Num",16,np.array(xbins))
mu1Beta1p2Denom = ROOT.TH1D("mu1Beta1p2Denom","mu1Beta1p2Denom",16,np.array(xbins))
mu2Ceta1p2Num = ROOT.TH1D("mu2Ceta1p2Num","mu2Ceta1p2Num",16,np.array(xbins))
mu2Ceta1p2Denom = ROOT.TH1D("mu2Ceta1p2Denom","mu2Ceta1p2Denom",16,np.array(xbins))
mu1Ceta1p2Num = ROOT.TH1D("mu1Ceta1p2Num","mu1Ceta1p2Num",16,np.array(xbins))
mu1Ceta1p2Denom = ROOT.TH1D("mu1Ceta1p2Denom","mu1Ceta1p2Denom",16,np.array(xbins))

#SingleMuonTrigger
singlemu2Beta0p9Num = ROOT.TH1D("singlemu2Beta0p9Num","singlemu2Beta0p9Num",16,np.array(xbins))
singlemu2Beta0p9Denom = ROOT.TH1D("singlemu2Beta0p9Denom","singlemu2Beta0p9Denom",16,np.array(xbins))
singlemu1Beta0p9Num = ROOT.TH1D("singlemu1Beta0p9Num","singlemu1Beta0p9Num",16,np.array(xbins))
singlemu1Beta0p9Denom = ROOT.TH1D("singlemu1Beta0p9Denom","singlemu1Beta0p9Denom",16,np.array(xbins))
singlemu2Ceta0p9Num = ROOT.TH1D("singlemu2Ceta0p9Num","singlemu2Ceta0p9Num",16,np.array(xbins))
singlemu2Ceta0p9Denom = ROOT.TH1D("singlemu2Ceta0p9Denom","singlemu2Ceta0p9Denom",16,np.array(xbins))
singlemu1Ceta0p9Num = ROOT.TH1D("singlemu1Ceta0p9Num","singlemu1Ceta0p9Num",16,np.array(xbins))
singlemu1Ceta0p9Denom = ROOT.TH1D("singlemu1Ceta0p9Denom","singlemu1Ceta0p9Denom",16,np.array(xbins))

singlemu2Beta0p9to1p2Num = ROOT.TH1D("singlemu2Beta0p9to1p2Num","singlemu2Beta0p9to1p2Num",16,np.array(xbins))
singlemu2Beta0p9to1p2Denom = ROOT.TH1D("singlemu2Beta0p9to1p2Denom","singlemu2Beta0p9to1p2Denom",16,np.array(xbins))
singlemu1Beta0p9to1p2Num = ROOT.TH1D("singlemu1Beta0p9to1p2Num","singlemu1Beta0p9to1p2Num",16,np.array(xbins))
singlemu1Beta0p9to1p2Denom = ROOT.TH1D("singlemu1Beta0p9to1p2Denom","singlemu1Beta0p9to1p2Denom",16,np.array(xbins))
singlemu2Ceta0p9to1p2Num = ROOT.TH1D("singlemu2Ceta0p9to1p2Num","singlemu2Ceta0p9to1p2Num",16,np.array(xbins))
singlemu2Ceta0p9to1p2Denom = ROOT.TH1D("singlemu2Ceta0p9to1p2Denom","singlemu2Ceta0p9to1p2Denom",16,np.array(xbins))
singlemu1Ceta0p9to1p2Num = ROOT.TH1D("singlemu1Ceta0p9to1p2Num","singlemu1Ceta0p9to1p2Num",16,np.array(xbins))
singlemu1Ceta0p9to1p2Denom = ROOT.TH1D("singlemu1Ceta0p9to1p2Denom","singlemu1Ceta0p9to1p2Denom",16,np.array(xbins))

singlemu2Beta1p2Num = ROOT.TH1D("singlemu2Beta1p2Num","singlemu2Beta1p2Num",16,np.array(xbins))
singlemu2Beta1p2Denom = ROOT.TH1D("singlemu2Beta1p2Denom","singlemu2Beta1p2Denom",16,np.array(xbins))
singlemu1Beta1p2Num = ROOT.TH1D("singlemu1Beta1p2Num","singlemu1Beta1p2Num",16,np.array(xbins))
singlemu1Beta1p2Denom = ROOT.TH1D("singlemu1Beta1p2Denom","singlemu1Beta1p2Denom",16,np.array(xbins))
singlemu2Ceta1p2Num = ROOT.TH1D("singlemu2Ceta1p2Num","singlemu2Ceta1p2Num",16,np.array(xbins))
singlemu2Ceta1p2Denom = ROOT.TH1D("singlemu2Ceta1p2Denom","singlemu2Ceta1p2Denom",16,np.array(xbins))
singlemu1Ceta1p2Num = ROOT.TH1D("singlemu1Ceta1p2Num","singlemu1Ceta1p2Num",16,np.array(xbins))
singlemu1Ceta1p2Denom = ROOT.TH1D("singlemu1Ceta1p2Denom","singlemu1Ceta1p2Denom",16,np.array(xbins))






treemuB.Draw("pt_2>>mu2Beta0p9Denom","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)<0.9&&tag_trigger_match_1>0.5")
treemuB.Draw("pt_1>>mu1Beta0p9Denom","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<0.9&&tag_trigger_match_2>0.5")
treemuB.Draw("pt_2>>mu2Beta0p9Num","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)<0.9&&tag_trigger_match_1>0.5&&probe_trigger_match_2")
treemuB.Draw("pt_1>>mu1Beta0p9Num","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<0.9&&tag_trigger_match_2>0.5&&probe_trigger_match_1")
treemuC.Draw("pt_2>>mu2Ceta0p9Denom","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)<0.9&&tag_trigger_match_1>0.5")
treemuC.Draw("pt_1>>mu1Ceta0p9Denom","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<0.9&&tag_trigger_match_2>0.5")
treemuC.Draw("pt_2>>mu2Ceta0p9Num","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)<0.9&&tag_trigger_match_1>0.5&&probe_trigger_match_2")
treemuC.Draw("pt_1>>mu1Ceta0p9Num","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<0.9&&tag_trigger_match_2>0.5&&probe_trigger_match_1")

treemuB.Draw("pt_2>>mu2Beta0p9to1p2Denom","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)>0.9&&abs(eta_2)<1.2&&tag_trigger_match_1>0.5")
treemuB.Draw("pt_1>>mu1Beta0p9to1p2Denom","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)>0.9&&abs(eta_1)<1.2&&tag_trigger_match_2>0.5")
treemuB.Draw("pt_2>>mu2Beta0p9to1p2Num","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)>0.9&&abs(eta_2)<1.2&&tag_trigger_match_1>0.5&&probe_trigger_match_2")
treemuB.Draw("pt_1>>mu1Beta0p9to1p2Num","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)>0.9&&abs(eta_1)<1.2&&tag_trigger_match_2>0.5&&probe_trigger_match_1")
treemuC.Draw("pt_2>>mu2Ceta0p9to1p2Denom","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)>0.9&&abs(eta_2)<1.2&&tag_trigger_match_1>0.5")
treemuC.Draw("pt_1>>mu1Ceta0p9to1p2Denom","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<1.2&&abs(eta_1)>0.9&&tag_trigger_match_2>0.5")
treemuC.Draw("pt_2>>mu2Ceta0p9to1p2Num","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)>0.9&&abs(eta_2)<1.2&&tag_trigger_match_1>0.5&&probe_trigger_match_2")
treemuC.Draw("pt_1>>mu1Ceta0p9to1p2Num","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<1.2&&abs(eta_1)>0.9&&tag_trigger_match_2>0.5&&probe_trigger_match_1")

treemuB.Draw("pt_2>>mu2Beta1p2Denom","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.2&&tag_trigger_match_1>0.5")
treemuB.Draw("pt_1>>mu1Beta1p2Denom","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_1)>1.2&&tag_trigger_match_2>0.5")
treemuB.Draw("pt_2>>mu2Beta1p2Num","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.2&&tag_trigger_match_1>0.5&&probe_trigger_match_2")
treemuB.Draw("pt_1>>mu1Beta1p2Num","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_1)>1.2&&tag_trigger_match_2>0.5&&probe_trigger_match_1")
treemuC.Draw("pt_2>>mu2Ceta1p2Denom","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.2&&tag_trigger_match_1>0.5")
treemuC.Draw("pt_1>>mu1Ceta1p2Denom","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_1)>1.2&&tag_trigger_match_2>0.5")
treemuC.Draw("pt_2>>mu2Ceta1p2Num","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.2&&tag_trigger_match_1>0.5&&probe_trigger_match_2")
treemuC.Draw("pt_1>>mu1Ceta1p2Num","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_2)>1.2&&tag_trigger_match_2>0.5&&probe_trigger_match_1")



mu2Beta0p9Num.Add(mu1Beta0p9Num)
mu2Beta0p9Num.Add(mu1Ceta0p9Num)
mu2Beta0p9Num.Add(mu2Ceta0p9Num)
mu2Beta0p9Denom.Add(mu1Beta0p9Denom)
mu2Beta0p9Denom.Add(mu1Ceta0p9Denom)
mu2Beta0p9Denom.Add(mu2Ceta0p9Denom)
mu2Beta0p9Num.Divide(mu2Beta0p9Denom)
mu2Beta0p9Num.SetName("Mu19_eta0p9")

mu2Beta0p9to1p2Num.Add(mu1Beta0p9to1p2Num)
mu2Beta0p9to1p2Num.Add(mu1Ceta0p9to1p2Num)
mu2Beta0p9to1p2Num.Add(mu2Ceta0p9to1p2Num)
mu2Beta0p9to1p2Denom.Add(mu1Beta0p9to1p2Denom)
mu2Beta0p9to1p2Denom.Add(mu1Ceta0p9to1p2Denom)
mu2Beta0p9to1p2Denom.Add(mu2Ceta0p9to1p2Denom)
mu2Beta0p9to1p2Num.Divide(mu2Beta0p9to1p2Denom)
mu2Beta0p9to1p2Num.SetName("Mu19_eta0p9to1p2")

mu2Beta1p2Num.Add(mu1Beta1p2Num)
mu2Beta1p2Num.Add(mu1Ceta1p2Num)
mu2Beta1p2Num.Add(mu2Ceta1p2Num)
mu2Beta1p2Denom.Add(mu1Beta1p2Denom)
mu2Beta1p2Denom.Add(mu1Ceta1p2Denom)
mu2Beta1p2Denom.Add(mu2Ceta1p2Denom)
mu2Beta1p2Num.Divide(mu2Beta1p2Denom)
mu2Beta1p2Num.SetName("Mu19_eta1p2")

treemuB.Draw("pt_2>>singlemu2Beta0p9Denom","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)<0.9&&tag_trigger_match_1>0.5")
treemuB.Draw("pt_1>>singlemu1Beta0p9Denom","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<0.9&&tag_trigger_match_2>0.5")
treemuB.Draw("pt_2>>singlemu2Beta0p9Num","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)<0.9&&tag_trigger_match_1>0.5&&tag_trigger_match_2")
treemuB.Draw("pt_1>>singlemu1Beta0p9Num","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<0.9&&tag_trigger_match_2>0.5&&tag_trigger_match_1")
treemuC.Draw("pt_2>>singlemu2Ceta0p9Denom","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)<0.9&&tag_trigger_match_1>0.5")
treemuC.Draw("pt_1>>singlemu1Ceta0p9Denom","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<0.9&&tag_trigger_match_2>0.5")
treemuC.Draw("pt_2>>singlemu2Ceta0p9Num","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)<0.9&&tag_trigger_match_1>0.5&&tag_trigger_match_2")
treemuC.Draw("pt_1>>singlemu1Ceta0p9Num","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<0.9&&tag_trigger_match_2>0.5&&tag_trigger_match_1")

treemuB.Draw("pt_2>>singlemu2Beta0p9to1p2Denom","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)>0.9&&abs(eta_2)<1.2&&tag_trigger_match_1>0.5")
treemuB.Draw("pt_1>>singlemu1Beta0p9to1p2Denom","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)>0.9&&abs(eta_1)<1.2&&tag_trigger_match_2>0.5")
treemuB.Draw("pt_2>>singlemu2Beta0p9to1p2Num","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)>0.9&&abs(eta_2)<1.2&&tag_trigger_match_1>0.5&&tag_trigger_match_2")
treemuB.Draw("pt_1>>singlemu1Beta0p9to1p2Num","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)>0.9&&abs(eta_1)<1.2&&tag_trigger_match_2>0.5&&tag_trigger_match_1")
treemuC.Draw("pt_2>>singlemu2Ceta0p9to1p2Denom","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)>0.9&&abs(eta_2)<1.2&&tag_trigger_match_1>0.5")
treemuC.Draw("pt_1>>singlemu1Ceta0p9to1p2Denom","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<1.2&&abs(eta_1)>0.9&&tag_trigger_match_2>0.5")
treemuC.Draw("pt_2>>singlemu2Ceta0p9to1p2Num","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)>0.9&&abs(eta_2)<1.2&&tag_trigger_match_1>0.5&&tag_trigger_match_2")
treemuC.Draw("pt_1>>singlemu1Ceta0p9to1p2Num","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<1.2&&abs(eta_1)>0.9&&tag_trigger_match_2>0.5&&tag_trigger_match_1")

treemuB.Draw("pt_2>>singlemu2Beta1p2Denom","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.2&&tag_trigger_match_1>0.5")
treemuB.Draw("pt_1>>singlemu1Beta1p2Denom","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_1)>1.2&&tag_trigger_match_2>0.5")
treemuB.Draw("pt_2>>singlemu2Beta1p2Num","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.2&&tag_trigger_match_1>0.5&&tag_trigger_match_2")
treemuB.Draw("pt_1>>singlemu1Beta1p2Num","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_1)>1.2&&tag_trigger_match_2>0.5&&tag_trigger_match_1")
treemuC.Draw("pt_2>>singlemu2Ceta1p2Denom","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.2&&tag_trigger_match_1>0.5")
treemuC.Draw("pt_1>>singlemu1Ceta1p2Denom","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_1)>1.2&&tag_trigger_match_2>0.5")
treemuC.Draw("pt_2>>singlemu2Ceta1p2Num","pt_1>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.2&&tag_trigger_match_1>0.5&&tag_trigger_match_2")
treemuC.Draw("pt_1>>singlemu1Ceta1p2Num","pt_2>23&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.15&&iso_2<0.15&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_2)>1.2&&tag_trigger_match_2>0.5&&tag_trigger_match_1")



singlemu2Beta0p9Num.Add(singlemu1Beta0p9Num)
singlemu2Beta0p9Num.Add(singlemu1Ceta0p9Num)
singlemu2Beta0p9Num.Add(singlemu2Ceta0p9Num)
singlemu2Beta0p9Denom.Add(singlemu1Beta0p9Denom)
singlemu2Beta0p9Denom.Add(singlemu1Ceta0p9Denom)
singlemu2Beta0p9Denom.Add(singlemu2Ceta0p9Denom)
singlemu2Beta0p9Num.Divide(singlemu2Beta0p9Denom)
singlemu2Beta0p9Num.SetName("Mu22_eta0p9")

singlemu2Beta0p9to1p2Num.Add(singlemu1Beta0p9to1p2Num)
singlemu2Beta0p9to1p2Num.Add(singlemu1Ceta0p9to1p2Num)
singlemu2Beta0p9to1p2Num.Add(singlemu2Ceta0p9to1p2Num)
singlemu2Beta0p9to1p2Denom.Add(singlemu1Beta0p9to1p2Denom)
singlemu2Beta0p9to1p2Denom.Add(singlemu1Ceta0p9to1p2Denom)
singlemu2Beta0p9to1p2Denom.Add(singlemu2Ceta0p9to1p2Denom)
singlemu2Beta0p9to1p2Num.Divide(singlemu2Beta0p9to1p2Denom)
singlemu2Beta0p9to1p2Num.SetName("Mu22_eta0p9to1p2")

singlemu2Beta1p2Num.Add(singlemu1Beta1p2Num)
singlemu2Beta1p2Num.Add(singlemu1Ceta1p2Num)
singlemu2Beta1p2Num.Add(singlemu2Ceta1p2Num)
singlemu2Beta1p2Denom.Add(singlemu1Beta1p2Denom)
singlemu2Beta1p2Denom.Add(singlemu1Ceta1p2Denom)
singlemu2Beta1p2Denom.Add(singlemu2Ceta1p2Denom)
singlemu2Beta1p2Num.Divide(singlemu2Beta1p2Denom)
singlemu2Beta1p2Num.SetName("Mu22_eta1p2")


etabins = [0,0.9,1.2,2.1]

Mu19_Data_Eff = ROOT.TH2D("Mu19_Data_Eff","Mu19_Data_Eff",3,np.array(etabins),16,np.array(xbins))
Mu22_Data_Eff = ROOT.TH2D("Mu22_Data_Eff","Mu22_Data_Eff",3,np.array(etabins),16,np.array(xbins))

for i in range(1,17):
  print i
  Mu19_Data_Eff.SetBinContent(1,i,mu2Beta0p9Num.GetBinContent(i))  
  Mu19_Data_Eff.SetBinContent(2,i,mu2Beta0p9to1p2Num.GetBinContent(i))  
  Mu19_Data_Eff.SetBinContent(3,i,mu2Beta1p2Num.GetBinContent(i))  
  Mu22_Data_Eff.SetBinContent(1,i,singlemu2Beta0p9Num.GetBinContent(i))  
  Mu22_Data_Eff.SetBinContent(2,i,singlemu2Beta0p9to1p2Num.GetBinContent(i))  
  Mu22_Data_Eff.SetBinContent(3,i,singlemu2Beta1p2Num.GetBinContent(i))  


outfile = ROOT.TFile("MutrigEfficiencies.root","RECREATE")
mu2Beta0p9Num.Write()
mu2Beta0p9to1p2Num.Write()
mu2Beta1p2Num.Write()
singlemu2Beta0p9Num.Write()
singlemu2Beta0p9to1p2Num.Write()
singlemu2Beta1p2Num.Write()
Mu19_Data_Eff.Write()
Mu22_Data_Eff.Write()

outfile.Close()


