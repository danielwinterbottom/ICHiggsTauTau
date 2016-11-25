import ROOT
import math
import sys
import os
import numpy as np
import fnmatch
import json
ROOT.gROOT.SetBatch(ROOT.kTRUE)

singleelectronB = "/vols/cms/amd12/CMSSW_7_6_3/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/July21-TandP/SingleElectronB_tpzee_2016.root"
singleelectronC = "/vols/cms/amd12/CMSSW_7_6_3/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/July21-TandP/SingleElectronC_tpzee_2016.root"
singleelectronD = "/vols/cms/amd12/CMSSW_7_6_3/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/July21-TandP/SingleElectronD_tpzee_2016.root"

fileeleB = ROOT.TFile.Open(singleelectronB)
fileeleC = ROOT.TFile.Open(singleelectronC)
fileeleD = ROOT.TFile.Open(singleelectronD)

treeeleB = fileeleB.Get("ntuple")
treeeleC = fileeleC.Get("ntuple")
treeeleD = fileeleD.Get("ntuple")

xbins = [10.,13.,16.,19.,22.,25.,28.,31.,34.,37.,40.,45.,50.,60.,70.,100.,1000.]

ele2Beta1p48Num = ROOT.TH1D("ele2Beta1p48Num","ele2Beta1p48Num",16,np.array(xbins))
ele2Beta1p48Denom = ROOT.TH1D("ele2Beta1p48Denom","ele2Beta1p48Denom",16,np.array(xbins))
ele1Beta1p48Num = ROOT.TH1D("ele1Beta1p48Num","ele1Beta1p48Num",16,np.array(xbins))
ele1Beta1p48Denom = ROOT.TH1D("ele1Beta1p48Denom","ele1Beta1p48Denom",16,np.array(xbins))
ele2Ceta1p48Num = ROOT.TH1D("ele2Ceta1p48Num","ele2Ceta1p48Num",16,np.array(xbins))
ele2Ceta1p48Denom = ROOT.TH1D("ele2Ceta1p48Denom","ele2Ceta1p48Denom",16,np.array(xbins))
ele1Ceta1p48Num = ROOT.TH1D("ele1Ceta1p48Num","ele1Ceta1p48Num",16,np.array(xbins))
ele1Ceta1p48Denom = ROOT.TH1D("ele1Ceta1p48Denom","ele1Ceta1p48Denom",16,np.array(xbins))
ele2Deta1p48Num = ROOT.TH1D("ele2Deta1p48Num","ele2Deta1p48Num",16,np.array(xbins))
ele2Deta1p48Denom = ROOT.TH1D("ele2Deta1p48Denom","ele2Deta1p48Denom",16,np.array(xbins))
ele1Deta1p48Num = ROOT.TH1D("ele1Deta1p48Num","ele1Deta1p48Num",16,np.array(xbins))
ele1Deta1p48Denom = ROOT.TH1D("ele1Deta1p48Denom","ele1Deta1p48Denom",16,np.array(xbins))
ele2Beta1p48Num.Sumw2()
ele2Beta1p48Denom.Sumw2()
ele1Beta1p48Num.Sumw2()
ele1Beta1p48Denom.Sumw2()
ele2Ceta1p48Num.Sumw2()
ele2Ceta1p48Denom.Sumw2()
ele1Ceta1p48Num.Sumw2()
ele1Ceta1p48Denom.Sumw2()
ele2Deta1p48Num.Sumw2()
ele2Deta1p48Denom.Sumw2()
ele1Deta1p48Num.Sumw2()
ele1Deta1p48Denom.Sumw2()


ele2Betagt1p48Num = ROOT.TH1D("ele2Betagt1p48Num","ele2Betagt1p48Num",16,np.array(xbins))
ele2Betagt1p48Denom = ROOT.TH1D("ele2Betagt1p48Denom","ele2Betagt1p48Denom",16,np.array(xbins))
ele1Betagt1p48Num = ROOT.TH1D("ele1Betagt1p48Num","ele1Betagt1p48Num",16,np.array(xbins))
ele1Betagt1p48Denom = ROOT.TH1D("ele1Betagt1p48Denom","ele1Betagt1p48Denom",16,np.array(xbins))
ele2Cetagt1p48Num = ROOT.TH1D("ele2Cetagt1p48Num","ele2Cetagt1p48Num",16,np.array(xbins))
ele2Cetagt1p48Denom = ROOT.TH1D("ele2Cetagt1p48Denom","ele2Cetagt1p48Denom",16,np.array(xbins))
ele1Cetagt1p48Num = ROOT.TH1D("ele1Cetagt1p48Num","ele1Cetagt1p48Num",16,np.array(xbins))
ele1Cetagt1p48Denom = ROOT.TH1D("ele1Cetagt1p48Denom","ele1Cetagt1p48Denom",16,np.array(xbins))
ele2Detagt1p48Num = ROOT.TH1D("ele2Detagt1p48Num","ele2Detagt1p48Num",16,np.array(xbins))
ele2Detagt1p48Denom = ROOT.TH1D("ele2Detagt1p48Denom","ele2Detagt1p48Denom",16,np.array(xbins))
ele1Detagt1p48Num = ROOT.TH1D("ele1Detagt1p48Num","ele1Detagt1p48Num",16,np.array(xbins))
ele1Detagt1p48Denom = ROOT.TH1D("ele1Detagt1p48Denom","ele1Detagt1p48Denom",16,np.array(xbins))
ele2Betagt1p48Num.Sumw2()
ele2Betagt1p48Denom.Sumw2()
ele1Betagt1p48Num.Sumw2()
ele1Betagt1p48Denom.Sumw2()
ele2Cetagt1p48Num.Sumw2()
ele2Cetagt1p48Denom.Sumw2()
ele1Cetagt1p48Num.Sumw2()
ele1Cetagt1p48Denom.Sumw2()
ele2Detagt1p48Num.Sumw2()
ele2Detagt1p48Denom.Sumw2()
ele1Detagt1p48Num.Sumw2()
ele1Detagt1p48Denom.Sumw2()


#SingleEleTrigger
singleele2Beta1p48Num = ROOT.TH1D("singleele2Beta1p48Num","singleele2Beta1p48Num",16,np.array(xbins))
singleele2Beta1p48Denom = ROOT.TH1D("singleele2Beta1p48Denom","singleele2Beta1p48Denom",16,np.array(xbins))
singleele1Beta1p48Num = ROOT.TH1D("singleele1Beta1p48Num","singleele1Beta1p48Num",16,np.array(xbins))
singleele1Beta1p48Denom = ROOT.TH1D("singleele1Beta1p48Denom","singleele1Beta1p48Denom",16,np.array(xbins))
singleele2Ceta1p48Num = ROOT.TH1D("singleele2Ceta1p48Num","singleele2Ceta1p48Num",16,np.array(xbins))
singleele2Ceta1p48Denom = ROOT.TH1D("singleele2Ceta1p48Denom","singleele2Ceta1p48Denom",16,np.array(xbins))
singleele1Ceta1p48Num = ROOT.TH1D("singleele1Ceta1p48Num","singleele1Ceta1p48Num",16,np.array(xbins))
singleele1Ceta1p48Denom = ROOT.TH1D("singleele1Ceta1p48Denom","singleele1Ceta1p48Denom",16,np.array(xbins))
singleele2Deta1p48Num = ROOT.TH1D("singleele2Deta1p48Num","singleele2Deta1p48Num",16,np.array(xbins))
singleele2Deta1p48Denom = ROOT.TH1D("singleele2Deta1p48Denom","singleele2Deta1p48Denom",16,np.array(xbins))
singleele1Deta1p48Num = ROOT.TH1D("singleele1Deta1p48Num","singleele1Deta1p48Num",16,np.array(xbins))
singleele1Deta1p48Denom = ROOT.TH1D("singleele1Deta1p48Denom","singleele1Deta1p48Denom",16,np.array(xbins))
singleele2Beta1p48Num.Sumw2()
singleele2Beta1p48Denom.Sumw2()
singleele1Beta1p48Num.Sumw2()
singleele1Beta1p48Denom.Sumw2()
singleele2Ceta1p48Num.Sumw2()
singleele2Ceta1p48Denom.Sumw2()
singleele1Ceta1p48Num.Sumw2()
singleele1Ceta1p48Denom.Sumw2()
singleele2Deta1p48Num.Sumw2()
singleele2Deta1p48Denom.Sumw2()
singleele1Deta1p48Num.Sumw2()
singleele1Deta1p48Denom.Sumw2()


singleele2Betagt1p48Num = ROOT.TH1D("singleele2Betagt1p48Num","singleele2Betagt1p48Num",16,np.array(xbins))
singleele2Betagt1p48Denom = ROOT.TH1D("singleele2Betagt1p48Denom","singleele2Betagt1p48Denom",16,np.array(xbins))
singleele1Betagt1p48Num = ROOT.TH1D("singleele1Betagt1p48Num","singleele1Betagt1p48Num",16,np.array(xbins))
singleele1Betagt1p48Denom = ROOT.TH1D("singleele1Betagt1p48Denom","singleele1Betagt1p48Denom",16,np.array(xbins))
singleele2Cetagt1p48Num = ROOT.TH1D("singleele2Cetagt1p48Num","singleele2Cetagt1p48Num",16,np.array(xbins))
singleele2Cetagt1p48Denom = ROOT.TH1D("singleele2Cetagt1p48Denom","singleele2Cetagt1p48Denom",16,np.array(xbins))
singleele1Cetagt1p48Num = ROOT.TH1D("singleele1Cetagt1p48Num","singleele1Cetagt1p48Num",16,np.array(xbins))
singleele1Cetagt1p48Denom = ROOT.TH1D("singleele1Cetagt1p48Denom","singleele1Cetagt1p48Denom",16,np.array(xbins))
singleele2Detagt1p48Num = ROOT.TH1D("singleele2Detagt1p48Num","singleele2Detagt1p48Num",16,np.array(xbins))
singleele2Detagt1p48Denom = ROOT.TH1D("singleele2Detagt1p48Denom","singleele2Detagt1p48Denom",16,np.array(xbins))
singleele1Detagt1p48Num = ROOT.TH1D("singleele1Detagt1p48Num","singleele1Detagt1p48Num",16,np.array(xbins))
singleele1Detagt1p48Denom = ROOT.TH1D("singleele1Detagt1p48Denom","singleele1Detagt1p48Denom",16,np.array(xbins))
singleele2Betagt1p48Num.Sumw2()
singleele2Betagt1p48Denom.Sumw2()
singleele1Betagt1p48Num.Sumw2()
singleele1Betagt1p48Denom.Sumw2()
singleele2Cetagt1p48Num.Sumw2()
singleele2Cetagt1p48Denom.Sumw2()
singleele1Cetagt1p48Num.Sumw2()
singleele1Cetagt1p48Denom.Sumw2()
singleele2Detagt1p48Num.Sumw2()
singleele2Detagt1p48Denom.Sumw2()
singleele1Detagt1p48Num.Sumw2()
singleele1Detagt1p48Denom.Sumw2()





treeeleB.Draw("pt_2>>ele2Beta1p48Denom","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<1.48&&tag_trigger_match_1>0.5")
treeeleB.Draw("pt_1>>ele1Beta1p48Denom","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<1.48&&tag_trigger_match_2>0.5")
treeeleB.Draw("pt_2>>ele2Beta1p48Num","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<1.48&&tag_trigger_match_1>0.5&&probe_trigger_match_2")
treeeleB.Draw("pt_1>>ele1Beta1p48Num","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<1.48&&tag_trigger_match_2>0.5&&probe_trigger_match_1")
treeeleC.Draw("pt_2>>ele2Ceta1p48Denom","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<1.48&&tag_trigger_match_1>0.5")
treeeleC.Draw("pt_1>>ele1Ceta1p48Denom","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<1.48&&tag_trigger_match_2>0.5")
treeeleC.Draw("pt_2>>ele2Ceta1p48Num","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<1.48&&tag_trigger_match_1>0.5&&probe_trigger_match_2")
treeeleC.Draw("pt_1>>ele1Ceta1p48Num","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<1.48&&tag_trigger_match_2>0.5&&probe_trigger_match_1")
treeeleD.Draw("pt_2>>ele2Deta1p48Denom","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<1.48&&tag_trigger_match_1>0.5")
treeeleD.Draw("pt_1>>ele1Deta1p48Denom","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<1.48&&tag_trigger_match_2>0.5")
treeeleD.Draw("pt_2>>ele2Deta1p48Num","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<1.48&&tag_trigger_match_1>0.5&&probe_trigger_match_2")
treeeleD.Draw("pt_1>>ele1Deta1p48Num","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<1.48&&tag_trigger_match_2>0.5&&probe_trigger_match_1")


treeeleB.Draw("pt_2>>ele2Betagt1p48Denom","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.48&&tag_trigger_match_1>0.5")
treeeleB.Draw("pt_1>>ele1Betagt1p48Denom","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_1)>1.48&&tag_trigger_match_2>0.5")
treeeleB.Draw("pt_2>>ele2Betagt1p48Num","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.48&&tag_trigger_match_1>0.5&&probe_trigger_match_2")
treeeleB.Draw("pt_1>>ele1Betagt1p48Num","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_1)>1.48&&tag_trigger_match_2>0.5&&probe_trigger_match_1")
treeeleC.Draw("pt_2>>ele2Cetagt1p48Denom","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.48&&tag_trigger_match_1>0.5")
treeeleC.Draw("pt_1>>ele1Cetagt1p48Denom","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_1)>1.48&&tag_trigger_match_2>0.5")
treeeleC.Draw("pt_2>>ele2Cetagt1p48Num","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.48&&tag_trigger_match_1>0.5&&probe_trigger_match_2")
treeeleC.Draw("pt_1>>ele1Cetagt1p48Num","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_2)>1.48&&tag_trigger_match_2>0.5&&probe_trigger_match_1")
treeeleD.Draw("pt_2>>ele2Detagt1p48Denom","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.48&&tag_trigger_match_1>0.5")
treeeleD.Draw("pt_1>>ele1Detagt1p48Denom","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_1)>1.48&&tag_trigger_match_2>0.5")
treeeleD.Draw("pt_2>>ele2Detagt1p48Num","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.48&&tag_trigger_match_1>0.5&&probe_trigger_match_2")
treeeleD.Draw("pt_1>>ele1Detagt1p48Num","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_2)>1.48&&tag_trigger_match_2>0.5&&probe_trigger_match_1")





ele2Beta1p48Num.Add(ele1Beta1p48Num)
ele2Beta1p48Num.Add(ele1Ceta1p48Num)
ele2Beta1p48Num.Add(ele2Ceta1p48Num)
ele2Beta1p48Num.Add(ele1Deta1p48Num)
ele2Beta1p48Num.Add(ele2Deta1p48Num)
ele2Beta1p48Denom.Add(ele1Beta1p48Denom)
ele2Beta1p48Denom.Add(ele1Ceta1p48Denom)
ele2Beta1p48Denom.Add(ele2Ceta1p48Denom)
ele2Beta1p48Denom.Add(ele1Deta1p48Denom)
ele2Beta1p48Denom.Add(ele2Deta1p48Denom)
ele2Beta1p48Num.Divide(ele2Beta1p48Denom)
ele2Beta1p48Num.SetName("Ele24_eta1p48")

ele2Betagt1p48Num.Add(ele1Betagt1p48Num)
ele2Betagt1p48Num.Add(ele1Cetagt1p48Num)
ele2Betagt1p48Num.Add(ele2Cetagt1p48Num)
ele2Betagt1p48Num.Add(ele1Detagt1p48Num)
ele2Betagt1p48Num.Add(ele2Detagt1p48Num)
ele2Betagt1p48Denom.Add(ele1Betagt1p48Denom)
ele2Betagt1p48Denom.Add(ele1Cetagt1p48Denom)
ele2Betagt1p48Denom.Add(ele2Cetagt1p48Denom)
ele2Betagt1p48Denom.Add(ele1Detagt1p48Denom)
ele2Betagt1p48Denom.Add(ele2Detagt1p48Denom)
ele2Betagt1p48Num.Divide(ele2Betagt1p48Denom)
ele2Betagt1p48Num.SetName("Ele24_etagt1p48")

treeeleB.Draw("pt_2>>singleele2Beta1p48Denom","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<1.48&&tag_trigger_match_1>0.5")
treeeleB.Draw("pt_1>>singleele1Beta1p48Denom","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<1.48&&tag_trigger_match_2>0.5")
treeeleB.Draw("pt_2>>singleele2Beta1p48Num","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<1.48&&tag_trigger_match_1>0.5&&tag_trigger_match_2")
treeeleB.Draw("pt_1>>singleele1Beta1p48Num","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<1.48&&tag_trigger_match_2>0.5&&tag_trigger_match_1")
treeeleC.Draw("pt_2>>singleele2Ceta1p48Denom","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<1.48&&tag_trigger_match_1>0.5")
treeeleC.Draw("pt_1>>singleele1Ceta1p48Denom","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<1.48&&tag_trigger_match_2>0.5")
treeeleC.Draw("pt_2>>singleele2Ceta1p48Num","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<1.48&&tag_trigger_match_1>0.5&&tag_trigger_match_2")
treeeleC.Draw("pt_1>>singleele1Ceta1p48Num","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<1.48&&tag_trigger_match_2>0.5&&tag_trigger_match_1")
treeeleD.Draw("pt_2>>singleele2Deta1p48Denom","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<1.48&&tag_trigger_match_1>0.5")
treeeleD.Draw("pt_1>>singleele1Deta1p48Denom","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<1.48&&tag_trigger_match_2>0.5")
treeeleD.Draw("pt_2>>singleele2Deta1p48Num","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<1.48&&tag_trigger_match_1>0.5&&tag_trigger_match_2")
treeeleD.Draw("pt_1>>singleele1Deta1p48Num","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<1.48&&tag_trigger_match_2>0.5&&tag_trigger_match_1")


treeeleB.Draw("pt_2>>singleele2Betagt1p48Denom","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.48&&tag_trigger_match_1>0.5")
treeeleB.Draw("pt_1>>singleele1Betagt1p48Denom","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_1)>1.48&&tag_trigger_match_2>0.5")
treeeleB.Draw("pt_2>>singleele2Betagt1p48Num","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.48&&tag_trigger_match_1>0.5&&tag_trigger_match_2")
treeeleB.Draw("pt_1>>singleele1Betagt1p48Num","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_1)>1.48&&tag_trigger_match_2>0.5&&tag_trigger_match_1")
treeeleC.Draw("pt_2>>singleele2Cetagt1p48Denom","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.48&&tag_trigger_match_1>0.5")
treeeleC.Draw("pt_1>>singleele1Cetagt1p48Denom","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_1)>1.48&&tag_trigger_match_2>0.5")
treeeleC.Draw("pt_2>>singleele2Cetagt1p48Num","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.48&&tag_trigger_match_1>0.5&&tag_trigger_match_2")
treeeleC.Draw("pt_1>>singleele1Cetagt1p48Num","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_2)>1.48&&tag_trigger_match_2>0.5&&tag_trigger_match_1")
treeeleD.Draw("pt_2>>singleele2Detagt1p48Denom","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.48&&tag_trigger_match_1>0.5")
treeeleD.Draw("pt_1>>singleele1Detagt1p48Denom","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_1)>1.48&&tag_trigger_match_2>0.5")
treeeleD.Draw("pt_2>>singleele2Detagt1p48Num","pt_1>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1<0.1&&iso_2>0.2&&iso_2<0.5&&abs(eta_1)<2.1&&abs(eta_2)<2.1&&abs(eta_2)>1.48&&tag_trigger_match_1>0.5&&tag_trigger_match_2")
treeeleD.Draw("pt_1>>singleele1Detagt1p48Num","pt_2>26&&m_vis>75&&m_vis<105&&id_1>0.5&&id_2>0.5&&iso_1>0.2&&iso_1<0.5&&iso_2<0.1&&abs(eta_2)<2.1&&abs(eta_1)<2.1&&abs(eta_2)>1.48&&tag_trigger_match_2>0.5&&tag_trigger_match_1")


singleele2Beta1p48Num.Add(singleele1Beta1p48Num)
singleele2Beta1p48Num.Add(singleele1Ceta1p48Num)
singleele2Beta1p48Num.Add(singleele2Ceta1p48Num)
singleele2Beta1p48Num.Add(singleele1Deta1p48Num)
singleele2Beta1p48Num.Add(singleele2Deta1p48Num)
singleele2Beta1p48Denom.Add(singleele1Beta1p48Denom)
singleele2Beta1p48Denom.Add(singleele1Ceta1p48Denom)
singleele2Beta1p48Denom.Add(singleele2Ceta1p48Denom)
singleele2Beta1p48Denom.Add(singleele1Deta1p48Denom)
singleele2Beta1p48Denom.Add(singleele2Deta1p48Denom)
singleele2Beta1p48Num.Divide(singleele2Beta1p48Denom)
singleele2Beta1p48Num.SetName("Ele25_eta1p48")

singleele2Betagt1p48Num.Add(singleele1Betagt1p48Num)
singleele2Betagt1p48Num.Add(singleele1Cetagt1p48Num)
singleele2Betagt1p48Num.Add(singleele2Cetagt1p48Num)
singleele2Betagt1p48Num.Add(singleele1Detagt1p48Num)
singleele2Betagt1p48Num.Add(singleele2Detagt1p48Num)
singleele2Betagt1p48Denom.Add(singleele1Betagt1p48Denom)
singleele2Betagt1p48Denom.Add(singleele1Cetagt1p48Denom)
singleele2Betagt1p48Denom.Add(singleele2Cetagt1p48Denom)
singleele2Betagt1p48Denom.Add(singleele1Detagt1p48Denom)
singleele2Betagt1p48Denom.Add(singleele2Detagt1p48Denom)
singleele2Betagt1p48Num.Divide(singleele2Betagt1p48Denom)
singleele2Betagt1p48Num.SetName("Ele25_etagt1p48")


etabins = [0,1.48,2.1]

Ele25_Data_Eff = ROOT.TH2D("Ele25_Data_Eff","Ele25_Data_Eff",2,np.array(etabins),16,np.array(xbins))
Ele24_Data_Eff = ROOT.TH2D("Ele24_Data_Eff","Ele24_Data_Eff",2,np.array(etabins),16,np.array(xbins))

for i in range(1,17):
  print i
  Ele24_Data_Eff.SetBinContent(1,i,ele2Beta1p48Num.GetBinContent(i))  
  Ele24_Data_Eff.SetBinContent(2,i,ele2Betagt1p48Num.GetBinContent(i))  
  Ele25_Data_Eff.SetBinContent(1,i,singleele2Beta1p48Num.GetBinContent(i))  
  Ele25_Data_Eff.SetBinContent(2,i,singleele2Betagt1p48Num.GetBinContent(i))  


outfile = ROOT.TFile("EletrigEfficienciesAntiIso0p2to0p5.root","RECREATE")
ele2Beta1p48Num.Write()
ele2Betagt1p48Num.Write()
singleele2Beta1p48Num.Write()
singleele2Betagt1p48Num.Write()
Ele24_Data_Eff.Write()
Ele25_Data_Eff.Write()

outfile.Close()


