#!/usr/bin/env python
import ROOT

file = ROOT.TFile('input/scale_factors/Muon_SF_spring16temp_workspace.root')
ws = file.Get('ws')

for pt, eta in [(20, 0.1), (22, 0.1), (0, 0.1), (3000, 0.7), (300, 1.8), (300, -1.8)]:
    ws.var('m_pt').setVal(pt)
    ws.var('m_eta').setVal(eta)
    print 'pT = %f, eta = %f, eff(Mu19) = %f, eff(Mu22) = %f' % (
        pt, eta,
        ws.function('Mu19_Data_Eff').getVal(),
        ws.function('Mu22_Data_Eff').getVal())
