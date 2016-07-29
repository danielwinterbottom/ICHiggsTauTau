#!/usr/bin/env python
import ROOT
import imp
from array import array
wsptools = imp.load_source('wsptools', 'scripts/workspaceTools.py')


def GetFromTFile(str):
    f = ROOT.TFile(str.split(':')[0])
    obj = f.Get(str.split(':')[1]).Clone()
    f.Close()
    return obj

# Boilerplate
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.RooWorkspace.imp = getattr(ROOT.RooWorkspace, 'import')
ROOT.TH1.AddDirectory(0)


w = ROOT.RooWorkspace('w')

loc = 'output/tag_and_probe/'

histsToWrap = [
    (loc+'ZmmTP_Data_Fits_ID_pt_eta_bins.root:ID_pt_eta_bins',                          'm_id_data'),
    (loc+'ZmmTP_DYJetsToLL_Fits_ID_pt_eta_bins.root:ID_pt_eta_bins',                    'm_id_mc'),
    (loc+'ZmmTP_Data_Fits_Iso_pt_eta_bins.root:Iso_pt_eta_bins',                        'm_iso_data'),
    (loc+'ZmmTP_DYJetsToLL_Fits_Iso_pt_eta_bins.root:Iso_pt_eta_bins',                  'm_iso_mc'),
    (loc+'ZmmTP_Data_Fits_AIso1_pt_eta_bins.root:AIso1_pt_eta_bins',                    'm_aiso1_data'),
    (loc+'ZmmTP_DYJetsToLL_Fits_AIso1_pt_eta_bins.root:AIso1_pt_eta_bins',              'm_aiso1_mc'),
    (loc+'ZmmTP_Data_Fits_AIso2_pt_eta_bins.root:AIso2_pt_eta_bins',                    'm_aiso2_data'),
    (loc+'ZmmTP_DYJetsToLL_Fits_AIso2_pt_eta_bins.root:AIso2_pt_eta_bins',              'm_aiso2_mc'),
    (loc+'ZmmTP_Data_Fits_Trg_Iso_pt_eta_bins.root:Trg_Iso_pt_eta_bins',                'm_trg_data'),
    (loc+'ZmmTP_Data_Fits_Trg_AIso1_pt_bins_inc_eta.root:Trg_AIso1_pt_bins_inc_eta',    'm_trg_aiso1_data'),
    (loc+'ZmmTP_Data_Fits_Trg_AIso2_pt_bins_inc_eta.root:Trg_AIso2_pt_bins_inc_eta',    'm_trg_aiso2_data')
]

for task in histsToWrap:
    wsptools.SafeWrapHist(w, ['m_pt', 'expr::m_abs_eta("TMath::Abs(@0)",m_eta[0])'],
                          GetFromTFile(task[0]), name=task[1])


wsptools.MakeBinnedCategoryFuncMap(w, 'm_iso', [0., 0.15, 0.25, 0.50],
                                   'm_iso_binned_data', ['m_iso_data', 'm_aiso1_data', 'm_aiso2_data'])
wsptools.MakeBinnedCategoryFuncMap(w, 'm_iso', [0., 0.15, 0.25, 0.50],
                                   'm_iso_binned_mc', ['m_iso_mc', 'm_aiso1_mc', 'm_aiso2_mc'])
wsptools.MakeBinnedCategoryFuncMap(w, 'm_iso', [0., 0.15, 0.25, 0.50],
                                   'm_trg_binned_data', ['m_trg_data', 'm_trg_aiso1_data', 'm_trg_aiso2_data'])

for t in ['id', 'iso', 'aiso1', 'aiso2', 'iso_binned']:
    w.factory('expr::m_%s_ratio("@0/@1", m_%s_data, m_%s_mc)' % (t, t, t))



# fn = w.function('m_iso_binned_ratio').functor(ROOT.RooArgList(w.argSet('m_pt,m_eta,m_iso')))
# for pars in [
#     (20., 0.5, 0.1),
#     (20., 0.5, 0.15),
#     (20., 0.5, 0.20),
#     (20., 0.5, 0.25),
#     (20., 0.5, 0.35)]:
#     print (pars[0], pars[1], pars[2], fn.eval(array('d', [pars[0], pars[1], pars[2]])))

# out = ROOT.TFile('debug.root', 'RECREATE')
# out.Close()

w.Print()
w.writeToFile('htt_scalefactors_v1.root')
w.Delete()
