#!/usr/bin/env python
import ROOT
import imp
import json
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
ROOT.gROOT.LoadMacro("CrystalBallEfficiency.cxx+")

w = ROOT.RooWorkspace('w')

loc = 'output/tag_and_probe'

histsToWrap = [
    (loc+'/ZmmTP_Data_Fits_ID_pt_eta_bins.root:ID_pt_eta_bins',                          'm_id_data'),
    (loc+'/ZmmTP_DYJetsToLL_Fits_ID_pt_eta_bins.root:ID_pt_eta_bins',                    'm_id_mc'),
    (loc+'/ZmmTP_Data_Fits_Iso_pt_eta_bins.root:Iso_pt_eta_bins',                        'm_iso_data'),
    (loc+'/ZmmTP_DYJetsToLL_Fits_Iso_pt_eta_bins.root:Iso_pt_eta_bins',                  'm_iso_mc'),
    (loc+'/ZmmTP_Data_Fits_AIso1_pt_eta_bins.root:AIso1_pt_eta_bins',                    'm_aiso1_data'),
    (loc+'/ZmmTP_DYJetsToLL_Fits_AIso1_pt_eta_bins.root:AIso1_pt_eta_bins',              'm_aiso1_mc'),
    (loc+'/ZmmTP_Data_Fits_AIso2_pt_eta_bins.root:AIso2_pt_eta_bins',                    'm_aiso2_data'),
    (loc+'/ZmmTP_DYJetsToLL_Fits_AIso2_pt_eta_bins.root:AIso2_pt_eta_bins',              'm_aiso2_mc'),
    (loc+'/ZmmTP_Data_Fits_Trg_Iso_pt_eta_bins.root:Trg_Iso_pt_eta_bins',                'm_trg_data'),
    (loc+'/ZmmTP_Data_Fits_Trg_AIso1_pt_bins_inc_eta.root:Trg_AIso1_pt_bins_inc_eta',    'm_trg_aiso1_data'),
    (loc+'/ZmmTP_Data_Fits_Trg_AIso2_pt_bins_inc_eta.root:Trg_AIso2_pt_bins_inc_eta',    'm_trg_aiso2_data')
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


muon_trk_eff_hist = wsptools.TGraphAsymmErrorsToTH1D(GetFromTFile(loc+'/ratios.root:ratio_eta'))
wsptools.SafeWrapHist(w, ['m_eta'], muon_trk_eff_hist, name='m_trk_ratio')


## Electron and muon scale factors from the DESY group
desyloc = 'output/LeptonEfficiencies-master'

desyHistsToWrap = [
    (desyloc+'/Muon/Run2016BCD/Muon_IdIso0p15_eff.root',            'MC',   'm_idiso0p15_desy_mc'),
    (desyloc+'/Muon/Run2016BCD/Muon_IdIso0p15_eff.root',            'Data', 'm_idiso0p15_desy_data'),
    (desyloc+'/Muon/Run2016BCD/Muon_IdIso0p20_eff.root',            'MC',   'm_idiso0p20_desy_mc'),
    (desyloc+'/Muon/Run2016BCD/Muon_IdIso0p20_eff.root',            'Data', 'm_idiso0p20_desy_data'),
    (desyloc+'/Muon/Run2016BCD/Muon_IsoMu22_eff.root',              'Data', 'm_trgIsoMu22_desy_data'),
    (desyloc+'/Muon/Run2016BCD/Muon_IsoMu22_OR_TkIsoMu22_eff.root', 'Data', 'm_trgIsoMu22orTkIsoMu22_desy_data'),
]

for task in desyHistsToWrap:
    wsptools.SafeWrapHist(w, ['m_pt', 'expr::m_abs_eta("TMath::Abs(@0)",m_eta[0])'],
                          wsptools.ProcessDESYLeptonSFs(task[0], task[1], task[2]), name=task[2])
for t in ['idiso0p15_desy', 'idiso0p20_desy']:
    w.factory('expr::m_%s_ratio("@0/@1", m_%s_data, m_%s_mc)' % (t, t, t))

desyHistsToWrap = [
    (desyloc+'/Electron/Run2016BCD/Electron_IdIso0p10_eff.root',          'MC',   'e_idiso0p10_desy_mc'),
    (desyloc+'/Electron/Run2016BCD/Electron_IdIso0p10_eff.root',          'Data', 'e_idiso0p10_desy_data'),
    (desyloc+'/Electron/Run2016BCD/Electron_IdIso0p15_eff.root',          'MC',   'e_idiso0p15_desy_mc'),
    (desyloc+'/Electron/Run2016BCD/Electron_IdIso0p15_eff.root',          'Data', 'e_idiso0p15_desy_data'),
    (desyloc+'/Electron/Run2016BCD/Electron_Ele25eta2p1WPTight_eff.root', 'Data', 'e_trgEle25eta2p1WPTight_desy_data')
]

for task in desyHistsToWrap:
    wsptools.SafeWrapHist(w, ['e_pt', 'expr::e_abs_eta("TMath::Abs(@0)",e_eta[0])'],
                          wsptools.ProcessDESYLeptonSFs(task[0], task[1], task[2]), name=task[2])

for t in ['idiso0p10_desy', 'idiso0p15_desy']:
    w.factory('expr::e_%s_ratio("@0/@1", e_%s_data, e_%s_mc)' % (t, t, t))




tauloc = 'output/triggerSF-diTauICHEP2016/di-tau'

with open(tauloc+'/real_taus_cumulative.json') as jsonfile:
    pars = json.load(jsonfile)
    for wp in pars:
        x = pars[wp]
        w.factory('CrystalBallEfficiency::t_trg%s_data(t_pt[0],%g,%g,%g,%g,%g)' % (
                wp, x['m_{0}'], x['sigma'], x['alpha'], x['n'], x['norm']
            ))

w.importClassCode('CrystalBallEfficiency')

# trgfunc.Print('tree')
# trgvar = w.factory('expr::t_trg_data("0.8600521839194679 * ROOT::Math::crystalball_function(@0, 7.301872862369342, 3.00923339165087, 5.611128781906076, 38.585308272334196)", t_pt[0])')
# trgpdf = w.factory('CBShape::t_trg_pdf(t_pt[0], 38.585308272334196, 5.611128781906076, 7.301872862369342, 3.00923339165087)')


   # 63 RooCBShape::RooCBShape(const char *name, const char *title,
   # 64              RooAbsReal& _m, RooAbsReal& _m0, RooAbsReal& _sigma,
   # 65              RooAbsReal& _alpha, RooAbsReal& _n) :

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
w.writeToFile('htt_scalefactors_v2.root')
w.Delete()
