import ROOT
import argparse
from UserCode.ICHiggsTauTau.analysis import *

ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)

parser = argparse.ArgumentParser()
parser.add_argument('--type', '-t', help='control, tp')
parser.add_argument('--output', '-o', default='TauID_Basic', help='Output name')
parser.add_argument('--qcd_os_ss', default=2.0, type=float)
parser.add_argument('--extra', default=None)

args = parser.parse_args()

ROOT.TH1.SetDefaultSumw2(True)

ana = Analysis()
ana.remaps = {
    'SingleMuon': 'data_obs'
}
path = 'output/HTT2016Studies_Aug16/Zmm'
ana.AddSamples('%s/*.root' % path, 'Zmm')
ana.AddInfo('scripts/params_Aug16.json', scaleTo='data_obs')

ana_scale_m_hi = Analysis()
ana_scale_m_hi.remaps = ana.remaps
ana_scale_m_hi.AddSamples('%s/scale_m_hi/*.root' % path, 'Zmm', fallback='%s/*.root' % path)
ana_scale_m_hi.AddInfo('scripts/params_Aug16.json', scaleTo='data_obs')

ana_scale_m_lo = Analysis()
ana_scale_m_lo.remaps = ana.remaps
ana_scale_m_lo.AddSamples('%s/scale_m_lo/*.root' % path, 'Zmm', fallback='%s/*.root' % path)
ana_scale_m_lo.AddInfo('scripts/params_Aug16.json', scaleTo='data_obs')
# missing OS!

vv_samples = ['VVTo2L2Nu', 'WWTo1L1Nu2Q', 'WZJToLLLNu',
    'WZTo1L1Nu2Q', 'WZTo1L3Nu', 'WZTo2L2Q', 'ZZTo2L2Q', 'ZZTo4L',
    'ST_t-channel_antitop', 'ST_t-channel_top', 'ST_tW_antitop', 'ST_tW_top']


def AddMCProcs(node, v, sel, pfix=''):
    node.AddNode(ana.BasicFactory(
        'ZTT' + pfix, 'DYJetsToLL', v, sel(extra='gen_2==5')))

    node.AddNode(ana.BasicFactory(
        'ZL' + pfix, 'DYJetsToLL', v, sel(extra='gen_2<5')))

    node.AddNode(ana.BasicFactory(
        'ZJ' + pfix, 'DYJetsToLL', v, sel(extra='gen_2==6')))

    node.AddNode(ana.BasicFactory('W' + pfix, 'WJetsToLNu', v, sel()))

    node.AddNode(ana.BasicFactory('TT' + pfix, 'TT', v, sel()))

    node.AddNode(ana.SummedFactory('VV' + pfix, vv_samples, v, sel()))


def StandardMM(ana, node, v, sel, pfix='', qcd_os_ss=2.0):
    node.AddNode(BasicNode('data_obs' + pfix, 'data_obs', v, sel()))

    AddMCProcs(node)

    node.AddNode(HttQCDNode('QCD' + pfix,
        ana.BasicFactory('data_obs' + pfix, 'data_obs', v, sel(sign='!os')),
        ana.SummedFactory('backgrounds' + pfix, [], v, sel(sign='!os')),
        qcd_os_ss))
    node['QCD' + pfix].subtract_node.add_output_prefix = False
    AddMCProcs(node['QCD' + pfix].subtract_node, v, sel.copy(sign='!os'), pfix)


sel = Sel(
    sign='os',
    masswindow='m_ll>60 && m_ll<120',
    baseline='pt_1>23 && pt_2>20 && iso_1<0.15 && iso_2<0.15 && (trg_IsoMu22 || trg_IsoTkMu22))',
    wt='wt * wt_zpt * wt_top')

ana_list = [ana, ana_scale_m_hi, ana_scale_m_lo]
for var in [
        ('m_ll',  'm_ll(60,60,120)', '1'),
        ]:
    nodename = var[0]
    varname = var[1]
    dosel = sel.copy(category=var[2])
    StandardMM(ana, ana.nodes, v_pass, v_fail, dosel, probe)
    StandardMM(ana_scale_m_hi, ana_scale_m_hi.nodes, varname, dosel, pfix='_CMS_scale_mUp')
    StandardMM(ana_scale_m_lo, ana_scale_m_lo.nodes, varname, dosel, pfix='_CMS_scale_mDown')

outfile = ROOT.TFile('%s.root' % args.output, 'RECREATE')

for a in ana_list:
   a.nodes.PrintTree()
   a.compiled = False
   a.Run()
   a.nodes.Output(outfile)

outfile.Close()
