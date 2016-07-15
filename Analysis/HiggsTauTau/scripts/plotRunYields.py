#!/usr/bin/env python
import ROOT
import argparse
import json
import math
import bisect
import CombineHarvester.CombineTools.plotting as plot

# Boilerplate
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
plot.ModTDRStyle(width=2000, height=1000, b=0.17)

LS_TIME = 23.310779  # seconds
MINBIAS_XS = 71000000000.0  # pb
ROTATION_RATE = 11245.613
# from https://github.com/xiezhen/brilws/blob/master/brilws/lumiParameters.py

parser = argparse.ArgumentParser()

parser.add_argument('--yields', default=None)
parser.add_argument('--lumi', default=None)
parser.add_argument('--beam', default=None)
parser.add_argument('--trg', default=None)
parser.add_argument('--json', default=None)
parser.add_argument('--min-lumi', default=0., type=float)
parser.add_argument('--output', default='yield_per_run')

args = parser.parse_args()

if args.lumi is not None:
    with open(args.lumi) as f:
        lumi_lines = [line.split(',') for line in f.read().splitlines() if not line.startswith('#')]

lumi_dict = {}
nls_dict = {}
for line in lumi_lines:
    run = int(line[0].split(':')[0])
    lumi_dict[run] = float(line[-1])/ 1E6 # 1E6 to go from ub-1 to pb-1
    nls_dict[run] = float(line[2])

if args.beam is not None:
    with open(args.beam) as f:
        beam_lines = [line.split(',') for line in f.read().splitlines() if not line.startswith('#')]

if args.trg is not None:
    with open(args.trg) as f:
        trg_lines = [line.split(',') for line in f.read().splitlines() if not line.startswith('#')]

trg_info = []
for line in trg_lines:
    if len(line) >= 2 and line[1] == 'None':
        continue
    trg_info.append(((int(line[0]), int(line[1])), line[2:]))

trg_info.sort(key=lambda x: x[0])
for x in trg_info:
    print x

trg_keys = [x[0] for x in trg_info]

prescale_ok = {}
prescale_l1_ok = {}

if args.json is not None:
    with open(args.json) as jsonfile:
        cert_json = json.load(jsonfile)
    for run in cert_json:
        prescale_ok[run] = True
        prescale_l1_ok[run] = True
        for ls in cert_json[run]:
            print 'Checking %s, %s' % (run,ls)
            idx_lo = bisect.bisect_right(trg_keys, (int(run), int(ls[0])))
            idx_hi = bisect.bisect_right(trg_keys, (int(run), int(ls[1])))
            if idx_lo > 0:
                idx_lo -= 1
            if idx_hi > 0:
                idx_hi -= 1
            for info in trg_info[idx_lo:idx_hi+1]:

# ((274344, 1), ['0', '0', 'HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v4/1', 'OR', 'L1_DoubleIsoTau30er/0 L1_DoubleIsoTau29er/0 L1_DoubleIsoTau26er/0 L1_DoubleIsoTau27er/0 L1_DoubleIsoTau32er/0 L1_DoubleIsoTau28er/0'])
                # Need to figure out what our prescale situation is:
                # if int(info[1][1]) != 1:
                #     prescale_ok[run] = False
                # print info
                hlt_prescale = int(info[1][2][-1])
                # print hlt_prescale
                l1_prescales = [int(x[-1]) for x in info[1][-1].split(' ')]
                # print l1_prescales
                l1_all_unprescaled = True
                l1_at_least_one_unprescaled = (1 in l1_prescales)
                l1_all_unprescaled = all(x == 1 for x in l1_prescales)
                if hlt_prescale != 1 or not l1_at_least_one_unprescaled:
                    prescale_ok[run] = False
                if hlt_prescale == 1 and (l1_at_least_one_unprescaled and not l1_all_unprescaled):
                    prescale_l1_ok[run] = False
        if not prescale_ok[run]:
            print 'Trigger is prescaled in run %s' % run


nbx_dict = {}
for line in beam_lines:
    run = int(line[1])
    nbx_dict[run] = float(line[-1])


runs = sorted(x for x in lumi_dict.keys() if lumi_dict[x] > args.min_lumi)

yield_dict = {}
if args.yields is not None:
    yields = args.yields.split(':')

    with open(yields[0]) as jsonfile:
        yield_dict_tmp = json.load(jsonfile)
    for key in yields[1:]:
        yield_dict_tmp = yield_dict_tmp[key]
    for run, rate in yield_dict_tmp.iteritems():
        yield_dict[int(run)] = rate

yield_per_run = {}
yield_per_run_err = {}
avgpu_per_run = {}
for run in lumi_dict:
    if run in yield_dict:
        yield_per_run[run] = yield_dict[run] / lumi_dict[run]
        yield_per_run_err[run] = math.sqrt(yield_dict[run]) / lumi_dict[run]
    else:
        print 'Warning, run %i not found in yield dictionary!' % run
        yield_per_run[run] = 0.
        yield_per_run_err[run] = 0.
    avglumi = lumi_dict[run] / (nls_dict[run] * LS_TIME)
    avgpu_per_run[run] = avglumi/nbx_dict[run]*MINBIAS_XS/ROTATION_RATE


yield_hist = ROOT.TH1F('yield_hist', '', len(runs), 0, len(runs))
lumi_hist = ROOT.TH1F('lumi_hist', '', len(runs), 0, len(runs))
npu_hist = ROOT.TH1F('npu_hist', '', len(runs), 0, len(runs))

for i, run in enumerate(runs):
    yield_hist.SetBinContent(i+1, yield_per_run[run])
    yield_hist.SetBinError(i+1, yield_per_run_err[run])
    yield_hist.GetXaxis().SetBinLabel(i+1, str(run))

    lumi_hist.SetBinContent(i+1, lumi_dict[run])
    lumi_hist.SetBinError(i+1, 0)
    lumi_hist.GetXaxis().SetBinLabel(i+1, str(run))

    npu_hist.SetBinContent(i+1, avgpu_per_run[run])
    npu_hist.SetBinError(i+1, 0)
    npu_hist.GetXaxis().SetBinLabel(i+1, str(run))

canv = ROOT.TCanvas(args.output, args.output)
pads = plot.MultiRatioSplit([0.4, 0.2], [0.01, 0.01], [0.01, 0.01])

bad_trigger = ROOT.TBox()
bad_trigger.SetFillColorAlpha(ROOT.kRed, 0.3)

some_presc_l1 = ROOT.TBox()
some_presc_l1.SetFillColorAlpha(ROOT.kYellow, 0.3)


boxes = []
pads[0].cd()
plot.Set(pads[0], Grid=(0, 1), Tickx=1, Ticky=1)
plot.Set(yield_hist, LineWidth=2, MarkerSize=2, LineColor=ROOT.kRed, MarkerColor=ROOT.kRed)
plot.Set(yield_hist.GetXaxis(), Title='', LabelSize=0)
plot.Set(yield_hist.GetYaxis(), Title='Yield / pb^{-1}')
yield_hist.Draw('P0')
plot.FixBothRanges(pads[0], plot.GetAxisHist(pads[0]).GetMinimum(), 0.05, plot.GetAxisHist(pads[0]).GetMaximum(), 0.05)
for i, run in enumerate(runs):
    if not prescale_ok[str(run)]:
        boxes.append(bad_trigger.DrawBox(float(i), plot.GetAxisHist(pads[0]).GetMinimum(), float(i+1), plot.GetAxisHist(pads[0]).GetMaximum()))
    elif not prescale_l1_ok[str(run)]:
        boxes.append(some_presc_l1.DrawBox(float(i), plot.GetAxisHist(pads[0]).GetMinimum(), float(i+1), plot.GetAxisHist(pads[0]).GetMaximum()))

pads[1].cd()

plot.Set(pads[1], Grid=(0, 1), Tickx=1, Ticky=1)
plot.Set(npu_hist, LineWidth=2, MarkerSize=2, LineColor=ROOT.kGreen-2, MarkerColor=ROOT.kGreen-2)
plot.Set(npu_hist.GetXaxis(), Title='', LabelSize=0)
plot.Set(npu_hist.GetYaxis(), Title='Avg. PU')
npu_hist.GetYaxis().CenterTitle(True)
npu_hist.Draw('P')
plot.FixBothRanges(pads[1], plot.GetAxisHist(pads[1]).GetMinimum(), 0.05, plot.GetAxisHist(pads[1]).GetMaximum(), 0.05)
print (plot.GetAxisHist(pads[1]).GetMinimum(), plot.GetAxisHist(pads[1]).GetMaximum())
for i, run in enumerate(runs):
    if not prescale_ok[str(run)]:
        boxes.append(bad_trigger.DrawBox(float(i), plot.GetAxisHist(pads[1]).GetMinimum(), float(i+1), plot.GetAxisHist(pads[1]).GetMaximum()))
    elif not prescale_l1_ok[str(run)]:
        boxes.append(some_presc_l1.DrawBox(float(i), plot.GetAxisHist(pads[1]).GetMinimum(), float(i+1), plot.GetAxisHist(pads[1]).GetMaximum()))


pads[2].cd()
bad_trigger = ROOT.TBox()
bad_trigger.SetFillColorAlpha(ROOT.kRed, 0.3)
plot.Set(pads[2], Grid=(0, 1), Tickx=1, Ticky=1)
plot.Set(lumi_hist, LineWidth=2, MarkerSize=2, LineColor=ROOT.kBlue, MarkerColor=ROOT.kBlue)
plot.Set(lumi_hist.GetXaxis(), Title='Run')
plot.Set(lumi_hist.GetYaxis(), Title='Lumi (pb^{-1})')
lumi_hist.GetXaxis().LabelsOption('v1')
lumi_hist.GetYaxis().CenterTitle(True)
lumi_hist.Draw('P')
plot.FixBothRanges(pads[2], 0.00, 0.00, plot.GetAxisHist(pads[2]).GetMaximum(), 0.05)
for i, run in enumerate(runs):
    if not prescale_ok[str(run)]:
        boxes.append(bad_trigger.DrawBox(float(i), plot.GetAxisHist(pads[2]).GetMinimum(), float(i+1), plot.GetAxisHist(pads[2]).GetMaximum()))
    elif not prescale_l1_ok[str(run)]:
        boxes.append(some_presc_l1.DrawBox(float(i), plot.GetAxisHist(pads[2]).GetMinimum(), float(i+1), plot.GetAxisHist(pads[2]).GetMaximum()))

tot_lumi = sum(lumi_dict.values()) / 1000.  # fb-1 for now

right_label = '%.1f fb^{-1} (13 TeV)' % tot_lumi
if args.min_lumi > 0.:
    right_label += ', runs with L > %g pb^{-1}' % args.min_lumi

plot.DrawTitle(pads[0], yields[-1], 1)
plot.DrawTitle(pads[0], right_label, 3)

legend = ROOT.TLegend(0.1, 0.01, 0.7, 0.07, '', 'NBNDC')
legend.AddEntry(bad_trigger, 'Prescaled in run', 'F')
legend.AddEntry(some_presc_l1, 'Partial L1 prescale', 'F')
legend.SetNColumns(2)
legend.Draw()


canv.Print('.png')
canv.Print('.pdf')
