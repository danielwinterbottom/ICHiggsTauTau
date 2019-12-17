import ROOT
import os
import glob
import json
from UserCode.ICHiggsTauTau.analysis import *
from UserCode.ICHiggsTauTau.uncertainties import ufloat
from optparse import OptionParser
import argparse
import ConfigParser
import UserCode.ICHiggsTauTau.plotting as plotting
from collections import OrderedDict
import copy

ROOT.RooWorkspace.imp = getattr(ROOT.RooWorkspace, 'import')
ROOT.TH1.AddDirectory(0)
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.RooMsgService.instance().setGlobalKillBelow(ROOT.RooFit.WARNING)

outfile = ROOT.TFile('reso_fits.root','RECREATE')
dat=None
      
  
def FitWorkspace(name,infile,outfile,wsfile,doGaus=False):

  if doGaus: wsp = ROOT.RooWorkspace('wsp_gaus', '')
  else: wsp = ROOT.RooWorkspace('wsp', '')
  var = wsp.factory('ip[0,-0.01,0.01]')


  infile = ROOT.TFile('zmm_ip_hists_2018_new.root')

  for x in ['ip_nx','ip_ny','ip_nz','ip_bs_nx','ip_bs_ny','ip_bs_nz']:

    h_mc = infile.Get('%(x)s_mc' % vars())
    h_data = infile.Get('%(x)s_data' % vars())
    h_embed = infile.Get('%(x)s_embed' % vars())  

    h_data.Rebin(8)
  
    dat = wsp.imp(ROOT.RooDataHist('obs_%(x)s' % vars(), '', ROOT.RooArgList(var),
                  ROOT.RooFit.Index(wsp.factory('cat[data,mc]')),
                  ROOT.RooFit.Import('data', h_data),
                  ROOT.RooFit.Import('mc', h_mc)))
  
  
    model_hist = ROOT.RooDataHist('model_hist','',ROOT.RooArgList(var),h_mc)
    model = ROOT.RooHistPdf ('model_pdf_%(x)s' % vars(), '', ROOT.RooArgSet(var), model_hist, 0)
    wsp.imp(model)

    model_hist_embed = ROOT.RooDataHist('model_hist_embed','',ROOT.RooArgList(var),h_embed)
    model_embed = ROOT.RooHistPdf ('model_pdf_%(x)s_embed' % vars(), '', ROOT.RooArgSet(var), model_hist_embed, 0)
    wsp.imp(model_embed) 
 
    wsp.Print()
  
    pdf_args = [ ]
  
    if not doGaus:
  
      pdf_args.extend(
              [

                  "Voigtian::voigtian_%(x)s(ip, mean_v_%(x)s[-0.01,0.01], width_v_%(x)s[0,0,0.1], sigma_v_%(x)s[0,0.,0.1])" % vars(),
                  "FFTConvPdf::model_data_%(x)s(ip,voigtian_%(x)s,model_pdf_%(x)s)" % vars(),
                  "Voigtian::voigtian_%(x)s_embed(ip, mean_v_%(x)s_embed[-0.01,0.01], width_v_%(x)s_embed[0,0,0.1], sigma_v_%(x)s_embed[0,0.,0.1])" % vars(),
                  "FFTConvPdf::model_data_%(x)s_embed(ip,voigtian_%(x)s_embed,model_pdf_%(x)s_embed)" % vars(),
              ]
          )
  
  
    else:
  
      pdf_args.extend(
              [
                  "Gaussian::gaus_%(x)s(ip, mean_g_%(x)s[0,-0.01,0.01], sigma_g_%(x)s[0.001,0,0.1])" % vars(),
                  "FFTConvPdf::model_data_%(x)s(ip,gaus_%(x)s,model_pdf_%(x)s)" % vars(),
                  "Gaussian::gaus_%(x)s_embed(ip, mean_g_%(x)s_embed[0,-0.01,0.01], sigma_g_%(x)s_embed[0.001,0,0.1])" % vars(),
                  "FFTConvPdf::model_data_%(x)s_embed(ip,gaus_%(x)s_embed,model_pdf_%(x)s_embed)" % vars(), 
 
              ]
          )
  
  
    for arg in pdf_args: wsp.factory(arg)
  
   
    model_args = [
        "cat[data,mc]",
        "SIMUL::model_%(x)s(cat,data=model_data_%(x)s,mc=model_pdf_%(x)s)" % vars(),
        "SIMUL::model_%(x)s_embed(cat,data=model_data_%(x)s_embed,mc=model_pdf_%(x)s_embed)" % vars()
    ]
    for arg in model_args:
      wsp.factory(arg)

    # fit for MC  
   
    wsp.pdf("model_%(x)s" % vars()).fitTo(wsp.data('obs_%(x)s' % vars()),
                           ROOT.RooFit.Optimize(False),
                           ROOT.RooFit.Minimizer("Minuit2", "Migrad"),
                           ROOT.RooFit.Offset(True),
                           ROOT.RooFit.Extended(True),
                           ROOT.RooFit.SumW2Error(True),
                           ROOT.RooFit.PrintLevel(-1),
    )
    
    fitres = wsp.pdf("model_%(x)s" % vars()).fitTo(wsp.data('obs_%(x)s' % vars()),
                                    ROOT.RooFit.Minimizer("Minuit2", "Migrad"),
                                    ROOT.RooFit.Optimize(False),
                                    ROOT.RooFit.Offset(True),
                                    ROOT.RooFit.Extended(True),
                                    ROOT.RooFit.PrintLevel(-1),
                                    ROOT.RooFit.SumW2Error(True), 
                                    ROOT.RooFit.Save(),
    )
    fitres.Print()

    if not doGaus:
      canv = ROOT.TCanvas('canv', '', 0, 0, 700, 400)

      splitData = wsp.data('obs_%(x)s' % vars()).split(wsp.cat('cat'))
      xframe = wsp.var("ip").frame()
      width = (wsp.var("ip").getMax() - wsp.var("ip").getMin()) / splitData.At(0).numEntries()
      wsp.pdf('model_data_%(x)s' % vars()).plotOn(xframe,ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('model_data_%(x)s' % vars()).plotOn(xframe,ROOT.RooFit.LineColor(ROOT.kRed),ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('model_pdf_%(x)s' % vars()).plotOn(xframe,ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('model_pdf_%(x)s' % vars()).plotOn(xframe,ROOT.RooFit.LineColor(ROOT.kBlue),ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      splitData.At(0).plotOn(xframe, ROOT.RooFit.Name("Data"),ROOT.RooFit.MarkerStyle(7))
      xframe.Draw()

      canv.Print('data_mc_fit_%(x)s.pdf' % vars())

      splitData = wsp.data('obs_%(x)s' % vars()).split(wsp.cat('cat'))
      xframe = wsp.var("ip").frame()
      width = (wsp.var("ip").getMax() - wsp.var("ip").getMin()) / splitData.At(1).numEntries()
      wsp.pdf('voigtian_%(x)s' % vars()).plotOn(xframe,ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('voigtian_%(x)s' % vars()).plotOn(xframe,ROOT.RooFit.LineColor(ROOT.kBlue),ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      #splitData.At(1).plotOn(xframe, ROOT.RooFit.Name("MC"),ROOT.RooFit.MarkerStyle(7))
      xframe.Draw()

      canv.Print('mc_smear_%(x)s.pdf' % vars())

    # fit for embed

    wsp.pdf("model_%(x)s_embed" % vars()).fitTo(wsp.data('obs_%(x)s' % vars()),
                           ROOT.RooFit.Optimize(False),
                           ROOT.RooFit.Minimizer("Minuit2", "Migrad"),
                           ROOT.RooFit.Offset(True),
                           ROOT.RooFit.Extended(True),
                           ROOT.RooFit.SumW2Error(True),
                           ROOT.RooFit.PrintLevel(-1),
    )

    fitres = wsp.pdf("model_%(x)s_embed" % vars()).fitTo(wsp.data('obs_%(x)s' % vars()),
                                    ROOT.RooFit.Minimizer("Minuit2", "Migrad"),
                                    ROOT.RooFit.Optimize(False),
                                    ROOT.RooFit.Offset(True),
                                    ROOT.RooFit.Extended(True),
                                    ROOT.RooFit.PrintLevel(-1),
                                    ROOT.RooFit.SumW2Error(True),
                                    ROOT.RooFit.Save(),
    )
    fitres.Print()

    if not doGaus:
      canv = ROOT.TCanvas('canv', '', 0, 0, 700, 400)

      splitData = wsp.data('obs_%(x)s' % vars()).split(wsp.cat('cat'))
      xframe = wsp.var("ip").frame()
      width = (wsp.var("ip").getMax() - wsp.var("ip").getMin()) / splitData.At(0).numEntries()
      wsp.pdf('model_data_%(x)s_embed' % vars()).plotOn(xframe,ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('model_data_%(x)s_embed' % vars()).plotOn(xframe,ROOT.RooFit.LineColor(ROOT.kRed),ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('model_pdf_%(x)s_embed' % vars()).plotOn(xframe,ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('model_pdf_%(x)s_embed' % vars()).plotOn(xframe,ROOT.RooFit.LineColor(ROOT.kBlue),ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      splitData.At(0).plotOn(xframe, ROOT.RooFit.Name("Data"),ROOT.RooFit.MarkerStyle(7))
      xframe.Draw()

      canv.Print('data_embed_fit_%(x)s.pdf' % vars())

      splitData = wsp.data('obs_%(x)s' % vars()).split(wsp.cat('cat'))
      xframe = wsp.var("ip").frame()
      width = (wsp.var("ip").getMax() - wsp.var("ip").getMin()) / splitData.At(1).numEntries()
      wsp.pdf('voigtian_%(x)s_embed' % vars()).plotOn(xframe,ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('voigtian_%(x)s_embed' % vars()).plotOn(xframe,ROOT.RooFit.LineColor(ROOT.kBlue),ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      #splitData.At(1).plotOn(xframe, ROOT.RooFit.Name("MC"),ROOT.RooFit.MarkerStyle(7))
      xframe.Draw()
 
      canv.Print('embed_smear_%(x)s.pdf' % vars())

  wsfile.cd()
  wsp.Write()


wsfile = ROOT.TFile('smear_ws_2018_new.root','RECREATE')
FitWorkspace('test',outfile,outfile,wsfile)
FitWorkspace('test',outfile,outfile,wsfile,doGaus=True)
wsfile.Close()



