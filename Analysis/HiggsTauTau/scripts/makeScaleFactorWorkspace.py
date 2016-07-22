#!/usr/bin/env python
import ROOT

# Boilerplate
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.RooWorkspace.imp = getattr(ROOT.RooWorkspace, 'import')
ROOT.TH1.AddDirectory(0)


def SafeWrapHist(wsp, binvars, hist, name=None, bound=True):
    # Use the histogram name for this function unless a new name has
    # been specified
    if name is None:
        name = hist.GetName()
    # Bit of technical RooFit thing:
    # We want to use two sets of x and y variables. The first set will be
    # named specifically for the histogram in question. When we create the
    # RooDataHist RooFit will adjust the ranges and binning of these
    # RooRealVars to match the input histogram. We'll store these in
    # 'h_arglist'. The second set will contain the variables that are actually
    # used for looking up values in the RooHistFunc that will wrap around the
    # RooDataHist. These variables can be used for different RooHistFuncs with
    # different binnings and/or ranges in each, and could even be defined as
    # functions of other variables. We'll store these in 'f_arglist'.
    h_arglist = ROOT.RooArgList()
    f_arglist = ROOT.RooArgList()
    # Keep track of the relevant histogram axes (for the 1D, 2D or 3D cases)
    axes = [hist.GetXaxis()]
    if len(binvars) >= 2:
        axes.append(hist.GetYaxis())
    if len(binvars) >= 3:
        axes.append(hist.GetZaxis())
    for i, var in enumerate(binvars):
        # If the axis variable doesn't exist in the workspace already we need
        # to create it
        if not wsp.arg(var):
            # Check if the user has defined a function here
            if var.startswith('expr::'):
                funcarg = wsp.factory(var)
                var = funcarg.GetName()
            # otherwise create a normal variable
            else:
                wsp.factory('%s[0]' % var)
        # If the bound option is set we first pass the axis variables through
        # a function that adjusts the value to the xaxis minimum or maximum if
        # the user has set a value outside the axis range
        if bound:
            f_arglist.add(wsp.factory('expr::%s_range_%s("TMath::Range(%f,%f,@0)", %s)' % (
                name, var,
                axes[i].GetBinLowEdge(1),
                axes[i].GetBinUpEdge(axes[i].GetNbins()),
                var)))
        else:
            f_arglist.add(wsp.arg(var))
        # Now create the histogram-specific binning variable
        h_arglist.add(wsp.factory('%s_binningvar_%s[0]' % (name, var)))
    # First create the RooDataHist
    rdh = ROOT.RooDataHist(name, hist.GetTitle(), h_arglist, hist)
    # Then the RooHistFunc - it will create a 1->1 correspondence between the lookup variables
    # (f_arglist) and the variables that defined the histogram binning (h_arglist)
    rhf = ROOT.RooHistFunc(name, hist.GetTitle(), f_arglist, h_arglist, rdh)
    # Finally we can import it into the workspace
    wsp.imp(rhf)

file = ROOT.TFile('input/scale_factors/Muon_SF_spring16temp.root')
hists = []
hists.append(file.Get('Mu22_Data_Eff'))
hists.append(file.Get('Mu19_Data_Eff'))
file.Close()


ws = ROOT.RooWorkspace('ws')

for hist in hists:
    # Here we are saying that the x-axis lookup is done by taking the the abs
    # value of m_eta first, and the y-axis lookup is just done directly with
    # the m_pt variable
    SafeWrapHist(ws, ['expr::m_abs_eta("TMath::Abs(@0)",m_eta[0])', 'm_pt'], hist)


ws.Print()
ws.writeToFile('input/scale_factors/Muon_SF_spring16temp_workspace.root')
# I don't know why I need to delete this but it segfaults if I don't
ws.Delete()
