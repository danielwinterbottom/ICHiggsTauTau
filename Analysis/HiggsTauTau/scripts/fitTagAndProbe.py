import argparse
import ROOT
import os
import CombineHarvester.CombineTools.plotting as plot

ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.RooMsgService.instance().setGlobalKillBelow(ROOT.RooFit.WARNING)

parser = argparse.ArgumentParser()
parser.add_argument('input')
parser.add_argument('--sig-model', default='DoubleVCorr')
parser.add_argument('--bkg-model', default='Exponential')
parser.add_argument('--title', default='Muon ID Efficiency')
parser.add_argument('--postfix', default='')
parser.add_argument('--plot-dir', '-p', default='./')
parser.add_argument('--bin-replace', default=None) #(100,2.3,80,2.3)
args = parser.parse_args()


plot.ModTDRStyle(width=1200, l=0.35, r=0.15)
# Apparently I don't need to do this...
# ROOT.gSystem.Load('lib/libICHiggsTauTau.so')


if args.plot_dir != '':
    os.system('mkdir -p %s' % args.plot_dir)

ROOT.RooWorkspace.imp = getattr(ROOT.RooWorkspace, 'import')
ROOT.TH1.AddDirectory(0)

filename = args.input.split(':')[0]
name = args.input.split(':')[1]

infile = ROOT.TFile(filename)
wsp = infile.Get('wsp_'+name)

pdf_args = [ ]
if args.sig_model == 'DoubleVCorr':
    pdf_args.extend(
            [
                "Voigtian::signal1Pass(m_ll, mean1[90,80,100], width[2.495], sigma1[2,1,3])",
                "Voigtian::signal2Pass(m_ll, mean2[90,80,100], width,        sigma2[4,2,10])",
                "SUM::signalPass(vFrac[0.8,0,1]*signal1Pass, signal2Pass)",
                "Voigtian::signal1Fail(m_ll, mean1[90,80,100], width[2.495], sigma1[2,1,3])",
                "Voigtian::signal2Fail(m_ll, mean2[90,80,100], width,        sigma2[4,2,10])",
                "SUM::signalFail(vFrac[0.8,0,1]*signal1Fail, signal2Fail)",
            ]
        )
elif args.sig_model == 'DoubleVUncorr':
    pdf_args.extend(
            [
                "Voigtian::signal1Pass(m_ll, mean1p[90,80,100], widthp[2.495], sigma1p[2,1,3])",
                "Voigtian::signal2Pass(m_ll, mean2p[90,80,100], widthp,        sigma2p[4,2,10])",
                "SUM::signalPass(vFracp[0.8,0,1]*signal1Pass, signal2Pass)",
                "Voigtian::signal1Fail(m_ll, mean1f[90,80,100], widthf[2.495], sigma1f[2,1,3])",
                "Voigtian::signal2Fail(m_ll, mean2f[90,80,100], widthf,        sigma2f[4,2,10])",
                "SUM::signalFail(vFracf[0.8,0,1]*signal1Fail, signal2Fail)"
            ]
        )
else:
    raise RuntimeError('Chosen --sig-model %s not supported' % args.sig_model)

if args.bkg_model == 'Exponential':
    pdf_args.extend(
            [
                "Exponential::backgroundPass(m_ll, lp[-0.1,-1,0.1])",
                "Exponential::backgroundFail(m_ll, lf[-0.1,-1,0.1])"
            ]
        )
elif args.bkg_model == 'CMSShape':
    pdf_args.extend(
            [
                "RooCMSShape::backgroundPass(m_ll, alphaPass[70,60,90], betaPass[0.001,0,0.1], gammaPass[0.001,0,0.1], peak[90])",
                "RooCMSShape::backgroundFail(m_ll, alphaFail[70,60,90], betaFail[0.001,0,0.1], gammaFail[0.001,0,0.1], peak[90])",
            ]
        )
elif args.bkg_model == 'Chebychev':
    pdf_args.extend(
            [
                "RooChebychev::backgroundPass(m_ll, {a0p[0.25,0,0.5], a1p[-0.25,-1,0.1],a2p[0.,-0.25,0.25]})",
                "RooChebychev::backgroundFail(m_ll, {a0f[0.25,0,0.5], a1f[-0.25,-1,0.1],a2f[0.,-0.25,0.25]})",
            ]
        )
else:
    raise RuntimeError('Chosen --bkg-model %s not supported' % args.bkg_model)

for arg in pdf_args:
    wsp.factory(arg)

model_args = [
    "expr::nSignalPass('efficiency*fSigAll*numTot',efficiency[0,1], fSigAll[0.9,0,1],numTot[1,0,1e10])",
    "expr::nSignalFail('(1-efficiency)*fSigAll*numTot',efficiency,fSigAll,numTot)",
    "expr::nBkgPass('effBkg*(1-fSigAll)*numTot',effBkg[0.9,0,1],fSigAll,numTot)",
    "expr::nBkgFail('(1-effBkg)*(1-fSigAll)*numTot',effBkg,fSigAll,numTot)",
    "SUM::passing(nSignalPass*signalPass,nBkgPass*backgroundPass)",
    "SUM::failing(nSignalFail*signalFail,nBkgFail*backgroundFail)",
    "cat[fail,pass]",
    "SIMUL::model(cat,fail=failing,pass=passing)"
]
for arg in model_args:
    wsp.factory(arg)

hist = infile.Get(name)
bin_cfg = {
    'name': hist.GetName(),
    'binvar_x': hist.GetXaxis().GetTitle(),
    'binvar_y': hist.GetYaxis().GetTitle()
}

bins = []

for i in xrange(1, hist.GetNbinsX()+1):
    for j in xrange(1, hist.GetNbinsY()+1):
        bins.append((i, j,
                     hist.GetXaxis().GetBinLowEdge(i),
                     hist.GetXaxis().GetBinUpEdge(i),
                     hist.GetYaxis().GetBinLowEdge(j),
                     hist.GetYaxis().GetBinUpEdge(j)
                    ))

res = []

for b in bins:
    dat = '%s>=%g && %s<%g && %s>=%g && %s<%g' % (
            bin_cfg['binvar_x'], b[2],
            bin_cfg['binvar_x'], b[3],
            bin_cfg['binvar_y'], b[4],
            bin_cfg['binvar_y'], b[5],
            )
    label = '%s.%g_%g.%s.%g_%g' % (
            bin_cfg['binvar_x'], b[2], b[3],
            bin_cfg['binvar_y'], b[4], b[5]
            )
    label = label.replace('(', '_')
    label = label.replace(')', '_')

    # Set the initial yield and efficiency values
    yield_tot = wsp.data(dat).sumEntries()
    yield_pass = wsp.data(dat).sumEntries("cat==cat::pass")
    wsp.var("numTot").setVal(yield_tot)
    wsp.var("efficiency").setVal(yield_pass/yield_tot)

    # wsp.pdf("model").fitTo(wsp.data(dat),
    #                        ROOT.RooFit.Minimizer("Minuit2", "Scan"),
    #                        ROOT.RooFit.Offset(True),
    #                        ROOT.RooFit.Extended(True),
    #                        ROOT.RooFit.PrintLevel(-1))

    wsp.pdf("model").fitTo(wsp.data(dat),
                           ROOT.RooFit.Minimizer("Minuit2", "Migrad"),
                           ROOT.RooFit.Offset(True),
                           ROOT.RooFit.Extended(True),
                           ROOT.RooFit.PrintLevel(-1))

    fitres = wsp.pdf("model").fitTo(wsp.data(dat),
                                    ROOT.RooFit.Minimizer("Minuit2", "Migrad"),
                                    ROOT.RooFit.Offset(True),
                                    ROOT.RooFit.Extended(True),
                                    ROOT.RooFit.PrintLevel(-1),
                                    ROOT.RooFit.Save())
                           #ROOT.RooFit.Minos())

    fitres.Print()

    res.append((dat, wsp.var('efficiency').getVal(), wsp.var('efficiency').getError()))

    hist.SetBinContent(b[0], b[1], wsp.var('efficiency').getVal())
    hist.SetBinError(b[0], b[1], wsp.var('efficiency').getError())

    canv = ROOT.TCanvas('%s' % (label), "%s" % (label))
    pad_left = ROOT.TPad('left', '', 0., 0., 0.5, 1.)
    pad_left.Draw()
    pad_right = ROOT.TPad('right', '', 0.5, 0., 1., 1.)
    pad_right.Draw()
    pads = [pad_left, pad_right]

    latex = ROOT.TLatex()
    latex.SetNDC()

    ROOT.TGaxis.SetExponentOffset(-0.08, -0.02)

    splitData = wsp.data(dat).split(wsp.cat('cat'))
    xframe = wsp.var("m_ll").frame(ROOT.RooFit.Title("Passing"))
    width = (wsp.var("m_ll").getMax() - wsp.var("m_ll").getMin()) / splitData.At(1).numEntries()
    splitData.At(1).plotOn(xframe)
    wsp.pdf("passing").plotOn(xframe,
                              ROOT.RooFit.Slice(wsp.cat('cat'), "pass"),
                              ROOT.RooFit.LineColor(ROOT.kBlue))
    wsp.pdf("passing").plotOn(xframe,
                              ROOT.RooFit.Slice(wsp.cat('cat'), "pass"),
                              ROOT.RooFit.Components('backgroundPass'),
                              ROOT.RooFit.LineStyle(ROOT.kDashed),
                              ROOT.RooFit.LineColor(ROOT.kBlue))
    pads[0].cd()
    xframe.Draw()
    axis = plot.GetAxisHist(pads[0])
    plot.Set(axis.GetXaxis().SetTitle('m_{tag-probe} (GeV)'))
    plot.Set(axis.GetYaxis().SetTitle('Events / %g GeV' % width))
    plot.DrawTitle(pads[0], 'Pass Region', 1)
    latex.SetTextSize(0.03)
    latex.DrawLatex(0.5, 0.89, args.title)
    latex.DrawLatex(0.5, 0.84, 'p_{T}: [%g, %g] GeV #eta: [%g, %g]' % (b[2], b[3], b[4], b[5]))
    latex.SetTextFont(42)
    latex.DrawLatex(0.5, 0.79, '#varepsilon = %.4f #pm %.4f' % (wsp.var('efficiency').getVal(), wsp.var('efficiency').getError()))

    xframe2 = wsp.var("m_ll").frame(ROOT.RooFit.Title("Failing"))
    splitData.At(0).plotOn(xframe2)
    wsp.pdf("failing").plotOn(xframe2,
                              ROOT.RooFit.Slice(wsp.cat('cat'), "fail"),
                              ROOT.RooFit.LineColor(ROOT.kRed))
    wsp.pdf("failing").plotOn(xframe2,
                              ROOT.RooFit.Slice(wsp.cat('cat'), "fail"),
                              ROOT.RooFit.Components('backgroundFail'),
                              ROOT.RooFit.LineStyle(ROOT.kDashed),
                              ROOT.RooFit.LineColor(ROOT.kRed))
    pads[1].cd()
    xframe2.Draw()
    axis = plot.GetAxisHist(pads[1])
    plot.Set(axis.GetXaxis().SetTitle('m_{tag-probe} (GeV)'))
    plot.Set(axis.GetYaxis().SetTitle('Events / %g GeV' % width))
    plot.DrawTitle(pads[1], '12.9 fb^{-1} (13 TeV)', 3)
    plot.DrawTitle(pads[1], 'Fail Region', 1)

    canv.Print('%s/%s.png' % (args.plot_dir, canv.GetName()))
    canv.Print('%s/%s.pdf' % (args.plot_dir, canv.GetName()))

if args.bin_replace is not None:
    replacements = args.bin_replace.split(':')
    for rep in replacements:
        bins = [float(x) for x in rep.split(',')]
        dest_bin_x = hist.GetXaxis().FindFixBin(bins[0])
        dest_bin_y = hist.GetYaxis().FindFixBin(bins[1])
        src_bin_x = hist.GetXaxis().FindFixBin(bins[2])
        src_bin_y = hist.GetYaxis().FindFixBin(bins[3])
        dest_val, dest_err = hist.GetBinContent(dest_bin_x, dest_bin_y), hist.GetBinError(dest_bin_x, dest_bin_y)
        src_val, src_err = hist.GetBinContent(src_bin_x, src_bin_y), hist.GetBinError(src_bin_x, src_bin_y)
        print 'Replacing content of bin %g,%g (%g +/- %g) with %g,%g (%g +/- %g)' % (dest_bin_x, dest_bin_y, dest_val, dest_err, src_bin_x, src_bin_y, src_val, src_err)
        hist.SetBinContent(dest_bin_x, dest_bin_y, src_val)
        hist.SetBinError(dest_bin_x, dest_bin_y, src_err)

outfile = ROOT.TFile(filename.replace('.root', '_Fits_%s%s.root' % (name, args.postfix)), 'RECREATE')
hist.Write()

for i in xrange(1, hist.GetNbinsY()+1):
    slice = hist.ProjectionX('%s_projx_%i' % (hist.GetName(), i), i, i)
    slice.Write()
    gr = ROOT.TGraphAsymmErrors(slice)
    gr.SetName('gr_'+slice.GetName())
    gr.Write()

outfile.Close()
wsp.Delete()
