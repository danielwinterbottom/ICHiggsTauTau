import ROOT
import glob
import sys
# import json
from array import array
import UserCode.ICHiggsTauTau.analysis as analysis

ROOT.RooWorkspace.imp = getattr(ROOT.RooWorkspace, 'import')
ROOT.TH1.AddDirectory(0)


bin_cfgs = [
    {
        'name': 'ID_pt_bins_inc_eta',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22',
        'probe': 'id_p',
        'binvar_x': 'pt_p',
        'bins_x': [10., 15., 20., 25., 30., 40., 50., 60., 80., 100., 200., 1000.],
        'binvar_y': 'eta_p',
        'bins_y': [-2.4, 2.4]
    },
    {
        'name': 'ID_pt_eta_bins',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22',
        'probe': 'id_p',
        'binvar_x': 'pt_p',
        'bins_x': [10., 15., 20., 25., 30., 40., 50., 60., 80., 100., 200., 1000.],
        'binvar_y': 'abs(eta_p)',
        'bins_y': [0, 0.9, 1.2, 2.1, 2.4]
    },
    {
        'name': 'IDTrk_pt_bins_inc_eta',
        'var': 'm_ll(50,75,125)',
        'tag': '(!muon_p) && trg_t_IsoMu22',
        'probe': 'id_p',
        'binvar_x': 'pt_p',
        'bins_x': [10., 15., 20., 25., 30., 40., 50., 60., 80., 100., 200., 1000.],
        'binvar_y': 'eta_p',
        'bins_y': [-2.4, 2.4]
    },
    {
        'name': 'IDTrk_pt_eta_bins',
        'var': 'm_ll(50,75,125)',
        'tag': '(!muon_p) && trg_t_IsoMu22',
        'probe': 'id_p',
        'binvar_x': 'pt_p',
        'bins_x': [10., 15., 20., 25., 30., 40., 50., 60., 80., 100., 200., 1000.],
        'binvar_y': 'abs(eta_p)',
        'bins_y': [0, 0.9, 1.2, 2.1, 2.4]
    },
    {
        'name': 'Iso_pt_bins_inc_eta',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p',
        'probe': 'iso_p < 0.15',
        'binvar_x': 'pt_p',
        'bins_x': [10., 15., 20., 25., 30., 40., 50., 60., 80., 100., 200., 1000.],
        'binvar_y': 'eta_p',
        'bins_y': [-2.4, 2.4]
    },
    {
        'name': 'Iso_pt_eta_bins',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p',
        'probe': 'iso_p < 0.15',
        'binvar_x': 'pt_p',
        'bins_x': [10., 15., 20., 25., 30., 40., 50., 60., 80., 100., 200., 1000.],
        'binvar_y': 'abs(eta_p)',
        'bins_y': [0, 0.9, 1.2, 2.1, 2.4]
    },
    {
        'name': 'AIso1_pt_bins_inc_eta',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p',
        'probe': 'iso_p >= 0.15 && iso_p < 0.25',
        'binvar_x': 'pt_p',
        'bins_x': [10., 15., 20., 25., 30., 40., 50., 60., 80., 100., 1000.],
        'binvar_y': 'eta_p',
        'bins_y': [-2.4, 2.4]
    },
    {
        'name': 'AIso1_pt_eta_bins',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p',
        'probe': 'iso_p >= 0.15 && iso_p < 0.25',
        'binvar_x': 'pt_p',
        'bins_x': [10., 15., 20., 25., 30., 40., 50., 60., 80., 100., 1000.],
        'binvar_y': 'abs(eta_p)',
        'bins_y': [0, 0.9, 1.2, 2.1, 2.4]
    },
    {
        'name': 'AIso2_pt_bins_inc_eta',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p',
        'probe': 'iso_p >= 0.25 && iso_p < 0.50',
        'binvar_x': 'pt_p',
        'bins_x': [10., 15., 20., 25., 30., 40., 50., 60., 80., 100., 1000.],
        'binvar_y': 'eta_p',
        'bins_y': [-2.4, 2.4]
    },
    {
        'name': 'AIso2_pt_eta_bins',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p',
        'probe': 'iso_p >= 0.25 && iso_p < 0.50',
        'binvar_x': 'pt_p',
        'bins_x': [10., 15., 20., 25., 30., 40., 50., 60., 80., 100., 1000.],
        'binvar_y': 'abs(eta_p)',
        'bins_y': [0, 0.9, 1.2, 2.1, 2.4]
    },
    {
        'name': 'Trg_Iso_pt_bins_inc_eta',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p && iso_p < 0.15',
        'probe': 'trg_p_IsoMu22',
        'binvar_x': 'pt_p',
        'bins_x': [20., 21., 22., 23., 24., 25., 30., 40., 50., 60., 80., 100., 200., 1000.],
        'binvar_y': 'eta_p',
        'bins_y': [-2.4, 2.4]
    },
    {
        'name': 'Trg_Iso_pt_eta_bins',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p && iso_p < 0.15',
        'probe': 'trg_p_IsoMu22',
        'binvar_x': 'pt_p',
        'bins_x': [20., 21., 22., 23., 24., 25., 30., 40., 50., 60., 80., 100., 200., 1000.],
        'binvar_y': 'abs(eta_p)',
        'bins_y': [0, 0.9, 1.2, 2.1, 2.4]
    },
    {
        'name': 'TrgMT_Iso_pt_bins_inc_eta',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && trg_t_IsoMu19Tau && id_p && iso_p < 0.15',
        'probe': 'trg_p_IsoMu19Tau',
        'binvar_x': 'pt_p',
        'bins_x': [17., 18., 19., 20., 21., 22., 25., 30., 40., 50., 60., 80., 100., 200., 1000.],
        'binvar_y': 'eta_p',
        'bins_y': [-2.4, 2.4]
    },
    {
        'name': 'TrgMT_Iso_pt_eta_bins',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && trg_t_IsoMu19Tau && id_p && iso_p < 0.15',
        'probe': 'trg_p_IsoMu19Tau',
        'binvar_x': 'pt_p',
        'bins_x': [17., 18., 19., 20., 21., 22., 25., 30., 40., 50., 60., 80., 100., 200., 1000.],
        'binvar_y': 'abs(eta_p)',
        'bins_y': [0, 0.9, 1.2, 2.1, 2.4]
    },
    {
        'name': 'TrgMTL1_Iso_pt_bins_inc_eta',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p && iso_p < 0.15',
        'probe': 'trg_p_IsoMu19TauL1',
        'binvar_x': 'pt_p',
        'bins_x': [17., 18., 19., 20., 21., 22., 25., 30., 40., 50., 60., 80., 100., 200., 1000.],
        'binvar_y': 'eta_p',
        'bins_y': [-2.4, 2.4]
    },
    {
        'name': 'TrgMTL1_Iso_pt_eta_bins',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p && iso_p < 0.15',
        'probe': 'trg_p_IsoMu19TauL1',
        'binvar_x': 'pt_p',
        'bins_x': [17., 18., 19., 20., 21., 22., 25., 30., 40., 50., 60., 80., 100., 200., 1000.],
        'binvar_y': 'abs(eta_p)',
        'bins_y': [0, 0.9, 1.2, 2.1, 2.4]
    },
    {
        'name': 'TrgOR_Iso_pt_bins_inc_eta',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p && iso_p < 0.15',
        'probe': 'trg_p_IsoMu22 || trg_p_IsoTkMu22',
        'binvar_x': 'pt_p',
        'bins_x': [20., 21., 22., 23., 24., 25., 30., 40., 50., 60., 80., 100., 200., 1000.],
        'binvar_y': 'eta_p',
        'bins_y': [-2.4, 2.4]
    },
    {
        'name': 'TrgOR_Iso_pt_eta_bins',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p && iso_p < 0.15',
        'probe': 'trg_p_IsoMu22 || trg_p_IsoTkMu22',
        'binvar_x': 'pt_p',
        'bins_x': [20., 21., 22., 23., 24., 25., 30., 40., 50., 60., 80., 100., 200., 1000.],
        'binvar_y': 'abs(eta_p)',
        'bins_y': [0, 0.9, 1.2, 2.1, 2.4]
    },
    {
        'name': 'TrgOR3_Iso_pt_bins_inc_eta',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p && iso_p < 0.15',
        'probe': 'trg_p_IsoMu22 || trg_p_IsoTkMu22 || trg_p_PFTau120',
        'binvar_x': 'pt_p',
        'bins_x': [20., 21., 22., 23., 24., 25., 30., 40., 50., 60., 80., 100., 200., 1000.],
        'binvar_y': 'eta_p',
        'bins_y': [-2.4, 2.4]
    },
    {
        'name': 'TrgOR3_Iso_pt_eta_bins',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p && iso_p < 0.15',
        'probe': 'trg_p_IsoMu22 || trg_p_IsoTkMu22 || trg_p_PFTau120',
        'binvar_x': 'pt_p',
        'bins_x': [20., 21., 22., 23., 24., 25., 30., 40., 50., 60., 80., 100., 200., 1000.],
        'binvar_y': 'abs(eta_p)',
        'bins_y': [0, 0.9, 1.2, 2.1, 2.4]
    },
    {
        'name': 'Trg_AIso1_pt_bins_inc_eta',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p && iso_p >= 0.15 && iso_p < 0.25',
        'probe': 'trg_p_IsoMu22',
        'binvar_x': 'pt_p',
        'bins_x': [20., 21., 22., 23., 24., 25., 30., 40., 50., 60., 80., 100., 1000.],
        'binvar_y': 'eta_p',
        'bins_y': [-2.4, 2.4]
    },
    {
        'name': 'Trg_AIso2_pt_bins_inc_eta',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p && iso_p >= 0.25 && iso_p < 0.50',
        'probe': 'trg_p_IsoMu22',
        'binvar_x': 'pt_p',
        'bins_x': [20., 21., 22., 23., 24., 25., 30., 40., 50., 60., 80., 100., 1000.],
        'binvar_y': 'eta_p',
        'bins_y': [-2.4, 2.4]
    },
    {
        'name': 'TrgOR_AIso1_pt_bins_inc_eta',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p && iso_p >= 0.15 && iso_p < 0.25',
        'probe': 'trg_p_IsoMu22 || trg_p_IsoTkMu22',
        'binvar_x': 'pt_p',
        'bins_x': [20., 21., 22., 23., 24., 25., 30., 40., 50., 60., 80., 100., 1000.],
        'binvar_y': 'eta_p',
        'bins_y': [-2.4, 2.4]
    },
    {
        'name': 'TrgOR_AIso2_pt_bins_inc_eta',
        'var': 'm_ll(50,75,125)',
        'tag': 'muon_p && trg_t_IsoMu22 && id_p && iso_p >= 0.25 && iso_p < 0.50',
        'probe': 'trg_p_IsoMu22 || trg_p_IsoTkMu22',
        'binvar_x': 'pt_p',
        'bins_x': [20., 21., 22., 23., 24., 25., 30., 40., 50., 60., 80., 100., 1000.],
        'binvar_y': 'eta_p',
        'bins_y': [-2.4, 2.4]
    },
]

drawlist = []
andable = set()

for cfg in bin_cfgs:
    cfg['hist'] = ROOT.TH2D(cfg['name'], cfg['name'],
                            len(cfg['bins_x'])-1, array('d', cfg['bins_x']),
                            len(cfg['bins_y'])-1, array('d', cfg['bins_y']))
    hist = cfg['hist']
    hist.GetXaxis().SetTitle(cfg['binvar_x'])
    hist.GetYaxis().SetTitle(cfg['binvar_y'])

    cfg['bins'] = []

    for i in xrange(1, hist.GetNbinsX()+1):
        for j in xrange(1, hist.GetNbinsY()+1):
            cfg['bins'].append('%s>=%g && %s<%g && %s>=%g && %s<%g' % (
                cfg['binvar_x'], hist.GetXaxis().GetBinLowEdge(i),
                cfg['binvar_x'], hist.GetXaxis().GetBinUpEdge(i),
                cfg['binvar_y'], hist.GetYaxis().GetBinLowEdge(j),
                cfg['binvar_y'], hist.GetYaxis().GetBinUpEdge(j),
                ))
            andable.add('%s>=%g' % (cfg['binvar_x'], hist.GetXaxis().GetBinLowEdge(i)))
            andable.add('%s<%g' % (cfg['binvar_x'], hist.GetXaxis().GetBinUpEdge(i)))
            andable.add('%s>=%g' % (cfg['binvar_y'], hist.GetYaxis().GetBinLowEdge(j)))
            andable.add('%s<%g' % (cfg['binvar_y'], hist.GetYaxis().GetBinUpEdge(j)))

    for b in cfg['bins']:
        drawlist.append((cfg['var'], '((%s) && !(%s) && (%s)) * wt' % (b, cfg['probe'], cfg['tag'])))
        drawlist.append((cfg['var'], '((%s) && (%s) && (%s)) * wt' % (b, cfg['probe'], cfg['tag'])))
        andable.add(cfg['probe'])
        andable.add(cfg['tag'])


trees = {
    'DYJetsToLL': analysis.TTreeEvaluator('ZmmTP', 'output/HTT2016Studies_Aug16/ZmmTP/DYJetsToLL.root'),
    'Data': analysis.TTreeEvaluator('ZmmTP', 'output/HTT2016Studies_Aug16/ZmmTP/SingleMuon.root')
}


# sys.exit(0)

for sample in trees:
    outfile = ROOT.TFile('ZmmTP_%s.root' % sample, 'RECREATE')
    hists = trees[sample].Draw(drawlist, compiled=True)

    i = 0
    for cfg in bin_cfgs:
        wsp = ROOT.RooWorkspace('wsp_'+cfg['name'], '')
        var = wsp.factory('m_ll[100,75,125]')

        outfile.cd()
        outfile.mkdir(cfg['name'])
        ROOT.gDirectory.cd(cfg['name'])

        for b in cfg['bins']:
            hists[2*i].SetName(b+':fail')
            hists[2*i+1].SetName(b+':pass')
            hists[2*i].Write()
            hists[2*i+1].Write()
            dat = wsp.imp(ROOT.RooDataHist(b, '', ROOT.RooArgList(var),
                          ROOT.RooFit.Index(wsp.factory('cat[fail,pass]')),
                          ROOT.RooFit.Import('fail', hists[2*i]),
                          ROOT.RooFit.Import('pass', hists[2*i+1]))
                          )
            i += 1
        outfile.cd()
        wsp.Write()
        cfg['hist'].Write()
        wsp.Delete()

    outfile.Close()
