import CombineHarvester.CombineTools.plotting as plot
import ROOT
import argparse
# import sys
import os
from copy import deepcopy

ROOT.TH1.SetDefaultSumw2(True)
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
plot.ModTDRStyle()


def Getter(file, hist, width=False):
    h = file.Get(hist)
    if h == None:
        return None
    if width and len(h.GetXaxis().GetXbins()) > 0:
        h.Scale(1., 'width')
    return h

CHANNELS = {
    'et': 'e_{}#tau_{h}',
    'mt': '#mu_{}#tau_{h}',
    'em': 'e#mu',
    'tt': '#tau_{h}#tau_{h}',
    'mm': '#mu#mu'
}

LAYOUTS = {
    "generic": [
        # ('QCD', {
        #     'entries': ['QCD'],
        #     'legend': 'QCD multijet',
        #     'color': ROOT.TColor.GetColor(250, 202, 255)
        # }
        # ),
        ('VV', {
            'entries': ['VV'],
            'legend': 'Diboson',
            'color': ROOT.TColor.GetColor(222, 90, 106)
        }
        ),
        ('TT', {
            'entries': ['TT'],
            'legend': 't#bar{t}',
            'color': ROOT.TColor.GetColor(155, 152, 204)
        }
        ),
        ('ZLL', {
            'entries': ['ZLL'],
            'legend': 'Z#rightarrowll',
            'color': ROOT.TColor.GetColor(100, 192, 232)
        }
        ),
        # ('ZTT', {
        #     'entries': ['ZTT'],
        #     'legend': 'Z#rightarrow#tau#tau',
        #     'color': ROOT.TColor.GetColor(248, 206, 104)
        # }
        # )
    ],
    "mt": [
        ('QCD', {
            'entries': ['QCD'],
            'legend': 'QCD multijet',
            'color': ROOT.TColor.GetColor(250, 202, 255)
        }
        ),
        ('TT', {
            'entries': ['TT'],
            'legend': 't#bar{t}',
            'color': ROOT.TColor.GetColor(155, 152, 204)
        }
        ),
        ('EWK', {
            'entries': ['VV', 'W'],
            'legend': 'Electroweak',
            'color': ROOT.TColor.GetColor(222, 90, 106)
        }
        ),
        ('ZLL', {
            'entries': ['ZL', 'ZJ'],
            'legend': 'Z#rightarrowll',
            'color': ROOT.TColor.GetColor(100, 192, 232)
        }
        ),
        ('ZTT', {
            'entries': ['ZTT'],
            'legend': 'Z#rightarrow#tau#tau',
            'color': ROOT.TColor.GetColor(248, 206, 104)
        }
        )
    ],
    "mm": [
        ('QCD', {
            'entries': ['QCD'],
            'legend': 'Misidentified #mu',
            'color': ROOT.TColor.GetColor(250, 202, 255)
        }),
        ('TT', {
            'entries': ['TT'],
            'legend': 't#bar{t}',
            'color': ROOT.TColor.GetColor(155, 152, 204)
        }),
        ('EWK', {
            'entries': ['VV', 'W'],
            'legend': 'Electroweak',
            'color': ROOT.TColor.GetColor(222, 90, 106)
        }),
        ('ZTT', {
            'entries': ['ZTT'],
            'legend': 'Z#rightarrow#tau#tau',
            'color': ROOT.TColor.GetColor(248, 206, 104)
        }),
        ('ZLL', {
            'entries': ['ZLL'],
            'legend': 'Z#rightarrow#mu#mu',
            'color': ROOT.TColor.GetColor(100, 192, 232)
        })
    ]
}

# customise the layouts for each channel
# LAYOUTS['mt'] = deepcopy(LAYOUTS['generic'])
LAYOUTS['et'] = deepcopy(LAYOUTS['generic'])
LAYOUTS['tt'] = deepcopy(LAYOUTS['generic'])


parser = argparse.ArgumentParser()
parser.add_argument('--input', '-i', help='Output of PostFitShapes or PostFitShapesFromWorkspace, specified as FILE:BIN')
parser.add_argument('--output', '-o', default='', help='Output name')
parser.add_argument('--channel', '-c', default='mt', choices=['mt', 'et', 'em', 'tt', 'mm', 'generic'], help='Channel')
parser.add_argument('--x-title', default='', help='x-axis variable, without GeV')
parser.add_argument('--logy', action='store_true')
parser.add_argument('--y-min', type=float, default=1)
parser.add_argument('--title', default='')

args = parser.parse_args()


filename, folder = args.input.split(':')
file = ROOT.TFile(filename)

target_list = []

if folder[-1] == '*':
    print folder[:-1]
    ROOT.gDirectory.cd(folder[:-1])
    for name in ROOT.gDirectory.GetListOfKeys():
        if name.GetClassName() == 'TDirectoryFile':
            target_list.append(folder.replace('*', name.GetName()))
    print target_list
else:
    target_list.append(folder)


if args.output is None:
    args.output = folder
else:
    if args.output[-1] == '/' and not os.path.exists(args.output):
        os.makedirs(args.output)

for target in target_list:
    if len(target_list) == 1:
        output = args.output if args.output != '' else target.split('/')[-1]
    else:
        output = args.output + target.split('/')[-1]

    if args.logy:
        output += '_logy'
    # Canvas and pads
    canv = ROOT.TCanvas(output, output)
    pads = plot.TwoPadSplit(0.27, 0.01, 0.01)

    # Get the data and create axis hist
    h_data = Getter(file, '%s/data_obs' % target, True)
    if isinstance(h_data, ROOT.TH2):
        print 'TH2: aborting!'
        continue

    h_axes = [h_data.Clone() for x in pads]
    for h in h_axes:
        # h.GetXaxis().SetLimits(2.1,200)
        h.Reset()

    build_h_tot = True
    h_tot = Getter(file, '%s/TotalProcs' % target, True)
    if h_tot is not None:
        build_h_tot = False
        print '>> Taking background uncertainty from TotalProcs hist'

    x_title = args.x_title
    units = ''

    if x_title == '' and h_data.GetXaxis().GetTitle() != '':
        x_title = h_data.GetXaxis().GetTitle()

    if ':' in x_title:
        units = x_title.split(':')[1]
        x_title = x_title.split(':')[0]

    plot.StandardAxes(h_axes[1].GetXaxis(), h_axes[0].GetYaxis(), x_title, units)
    h_axes[0].Draw()

    # A dict to keep track of the hists
    h_store = {}

    layout = LAYOUTS[args.channel]

    stack = ROOT.THStack()
    legend = ROOT.TLegend(0.67, 0.86 - 0.04*len(layout), 0.90, 0.91, '', 'NBNDC')

    # h_tot = None

    for ele in layout:
        info = ele[1]
        hist = Getter(file, '%s/%s' % (target, info['entries'][0]), True)
        plot.Set(hist, FillColor=info['color'], Title=info['legend'])
        if len(info['entries']) > 1:
            for other in info['entries'][1:]:
                hist.Add(Getter(file, '%s/%s' % (target, other), True))
        h_store[ele[0]] = hist
        if build_h_tot:
            if h_tot is None:
                h_tot = hist.Clone()
            else:
                h_tot.Add(hist)
        stack.Add(hist)

    h_tot.SetFillColor(plot.CreateTransparentColor(12, 0.3))
    h_tot.SetMarkerSize(0)

    legend.AddEntry(h_data, 'Observed', 'PL')
    for ele in reversed(layout):
        legend.AddEntry(h_store[ele[0]], '', 'F')
    bkg_uncert_label = 'Stat. Uncertainty'
    if not build_h_tot:
        bkg_uncert_label = 'Uncertainty'
    legend.AddEntry(h_tot, bkg_uncert_label, 'F')

    stack.Draw('HISTSAME')
    h_tot.Draw("E2SAME")
    h_data.Draw('E0SAME')

    if args.logy:
        h_axes[0].SetMinimum(args.y_min)
        pads[0].SetLogy(True)

    plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.35)
    legend.Draw()
    # plot.FixBoxPadding(pads[0], legend, 0.05)

    # Do the ratio plot
    pads[1].cd()
    pads[1].SetGrid(0, 1)
    h_axes[1].Draw()

    r_data = plot.MakeRatioHist(h_data, h_tot, True, False)
    r_tot = plot.MakeRatioHist(h_tot, h_tot, True, False)
    r_tot.Draw('E2SAME')
    r_data.Draw('SAME')

    plot.SetupTwoPadSplitAsRatio(
        pads, plot.GetAxisHist(
            pads[0]), plot.GetAxisHist(pads[1]), 'Obs/Exp', True, 0.61, 1.39)

    # Go back and tidy up the axes and frame
    pads[0].cd()
    pads[0].GetFrame().Draw()
    pads[0].RedrawAxis()

    # CMS logo
    plot.DrawCMSLogo(pads[0], 'CMS', 'Internal', 11, 0.045, 0.05, 1.0, '', 1.0)
    plot.DrawTitle(pads[0], '12.9 fb^{-1} (13 TeV)', 3)

    latex = ROOT.TLatex()
    plot.Set(latex, NDC=None, TextFont=42, TextSize=0.03)
    latex.DrawLatex(0.20, 0.75, args.title)
    # plot.DrawTitle(pads[0], args.title, 1)

    # ... and we're done
    canv.Print('.png')
    canv.Print('.pdf')


