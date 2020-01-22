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

  infile = ROOT.TFile('zmm_ip_hists_2018_new.root')
#  for x in ['ip_bs_rx','ip_bs_ry','ip_bs_rz','ip_bs_new_drx','ip_bs_new_dry','ip_bs_new_drz']:
  for x in ['ip_bs_rx','ip_bs_ry','ip_bs_rz','ip_bs_new_drx','ip_bs_new_dry','ip_bs_new_drz','ip_bs_rx_eta0p0to0p9','ip_bs_ry_eta0p0to0p9','ip_bs_rz_eta0p0to0p9','ip_bs_rx_eta0p9to1p2','ip_bs_ry_eta0p9to1p2','ip_bs_rz_eta0p9to1p2','ip_bs_rx_eta1p2to2p1','ip_bs_ry_eta1p2to2p1','ip_bs_rz_eta1p2to2p1','ip_bs_rz_eta1p2to2p1_ext']:
    if 'qoverp' in x:
      var = wsp.factory('ip_%(x)s[0,-0.04,0.04]' % vars()) 
    elif '_rz' in x:
      if 'ext' in x: var = wsp.factory('ip_%(x)s[0,-0.04,0.04]' % vars())
      else: var = wsp.factory('ip_%(x)s[0,-0.02,0.02]' % vars()) 
    elif 'drz' in x:
      var = wsp.factory('ip_%(x)s[0,-0.03,0.03]' % vars())
    elif 'drx' in x or 'dry' in x:
      var = wsp.factory('ip_%(x)s[0,-0.006,0.006]' % vars())
    else: 
      var = wsp.factory('ip_%(x)s[0,-0.005,0.005]' % vars())
      #wsp.var('ip_%(x)s').setRange(-0.005,0.005)
    #var = wsp.var('ip_%(x)s')    

    h_mc = infile.Get('%(x)s_mc' % vars())
    h_data = infile.Get('%(x)s_data' % vars())
    h_embed = infile.Get('%(x)s_embed' % vars())  

    h_data.Rebin(8)
    #h_data.Rebin(4) 
    #h_mc.Rebin(8) 
    #h_embed.Rebin(4) 

 
    dat = wsp.imp(ROOT.RooDataHist('obs_%(x)s' % vars(), '', ROOT.RooArgList(var),
                  ROOT.RooFit.Index(wsp.factory('cat[data,mc]')),
                  ROOT.RooFit.Import('data', h_data),
                  ROOT.RooFit.Import('mc', h_mc)))
  
  
    model_hist = ROOT.RooDataHist('model_hist_%(x)s' % vars(),'',ROOT.RooArgList(var),h_mc)
    model = ROOT.RooHistPdf ('model_pdf_%(x)s' % vars(), '', ROOT.RooArgSet(var), model_hist, 0)
    wsp.imp(model)

    model_hist_embed = ROOT.RooDataHist('model_hist_%(x)s_embed' % vars(),'',ROOT.RooArgList(var),h_embed)
    model_embed = ROOT.RooHistPdf ('model_pdf_%(x)s_embed' % vars(), '', ROOT.RooArgSet(var), model_hist_embed, 0)
    wsp.imp(model_embed) 
 
    wsp.Print()
  
    pdf_args = [ ]
 
    ForceSym = False
    if 'eta1p2to2p1' in x and 'rz' in x: ForceSym = True
 
    if not doGaus:
  
      pdf_args.extend(
              [

                  "Voigtian::voigtian_%(x)s(ip_%(x)s, mean_v_%(x)s[-0.01,0.01], width_v_%(x)s[0,0,0.1], sigma_v_%(x)s[0,0.,0.1])" % vars(),
                  "FFTConvPdf::model_data_%(x)s(ip_%(x)s,voigtian_%(x)s,model_pdf_%(x)s)" % vars(),
                  "expr::sigma_v_%(x)s_embed('sigma_v_%(x)s_embed_p0+sigma_v_%(x)s_embed_p1*fabs(ip_%(x)s)', ip_%(x)s,  sigma_v_%(x)s_embed_p0[0,0.,0.1], sigma_v_%(x)s_embed_p1[0,-1,1])" % vars(), 
                  #"Voigtian::voigtian_%(x)s_embed(ip_%(x)s, mean_v_%(x)s_embed[-0.01,0.01], width_v_%(x)s_embed[0,0,0.1], sigma_v_%(x)s_embed[0,0.,0.1])" % vars(),
                  "Voigtian::voigtian_%(x)s_embed(ip_%(x)s, mean_v_%(x)s_embed[-0.01,0.01], width_v_%(x)s_embed[0,0,0.1], sigma_v_%(x)s_embed)" % vars(),
                  "FFTConvPdf::model_data_%(x)s_embed(ip_%(x)s,voigtian_%(x)s_embed,model_pdf_%(x)s_embed)" % vars(),
              ]
          )
  
  
    else:
      if ('_drx' in x or '_dry' in x or '_drz' in x): 
        pdf_args.extend(
                [
                   # #"expr::sigma_g_%(x)s_1('sigma_g_%(x)s_p0_1+sigma_g_%(x)s_p1_1*fabs(ip_%(x)s-mean_g_%(x)s)', ip_%(x)s,  sigma_g_%(x)s_p0_1[0,0.,0.1], sigma_g_%(x)s_p1_1[0,-1,1],mean_g_%(x)s[0])" % vars(),
                   # "expr::sigma_g_%(x)s_1('sigma_g_%(x)s_p0_1+sigma_g_%(x)s_p1_1*fabs(ip_%(x)s-mean_g_%(x)s)', ip_%(x)s,  sigma_g_%(x)s_p0_1[0,0.,0.1], sigma_g_%(x)s_p1_1[0,-1,1],mean_g_%(x)s[0,-0.1,0.1])" % vars(),
                   # "expr::sigma_g_%(x)s_2('sigma_g_%(x)s_p0_1+sigma_g_%(x)s_p1_1*fabs(-ip_%(x)s-mean_g_%(x)s)', ip_%(x)s,  sigma_g_%(x)s_p0_1, sigma_g_%(x)s_p1_1,mean_g_%(x)s)" % vars(),
                   # #"expr::sigma_g_%(x)s_1('sigma_g_%(x)s_p0_1+sigma_g_%(x)s_p1_1*fabs(ip_%(x)s-mean_g_%(x)s)', ip_%(x)s,  sigma_g_%(x)s_p0_1[0,0.,0.1], sigma_g_%(x)s_p1_1[0,-1,1],mean_g_%(x)s[0])" % vars(),
                   # #"expr::sigma_g_%(x)s_2('sigma_g_%(x)s_p0_2+sigma_g_%(x)s_p1_2*fabs(ip_%(x)s-mean_g_%(x)s)', ip_%(x)s,  sigma_g_%(x)s_p0_2[0,0.,0.1], sigma_g_%(x)s_p1_2[0,-1,1],mean_g_%(x)s[0,-0.01,0.01])" % vars(),

                   # #"expr::sigma_g_%(x)s('(ip_%(x)s>=mean_g_%(x)s)*sigma_g_%(x)s_1+(ip_%(x)s<mean_g_%(x)s)*sigma_g_%(x)s_2', ip_%(x)s,  sigma_g_%(x)s_1, sigma_g_%(x)s_2,mean_g_%(x)s)" % vars(),
                   # "Gaussian::gaus_%(x)s(ip_%(x)s, mean_g_%(x)s, sigma_g_%(x)s_1)" % vars(),
                   # "Gaussian::gausother_%(x)s(-ip_%(x)s, mean_g_%(x)s, sigma_g_%(x)s_2)" % vars(),
                   # "FFTConvPdf::gaus2_%(x)s(ip_%(x)s,gaus_%(x)s,gausother_%(x)s)" % vars(),
                   # "FFTConvPdf::model_data_%(x)s(ip_%(x)s,gaus_%(x)s,model_pdf_%(x)s)" % vars(),
                   # #"expr::model_data('numTot*model_data_%(x)s(ip_%(x)s_1',numTot[1,0,1e10],model_data_%(x)s(ip_%(x)s_1)" % vars(),


                   # #"expr::sigma_g_%(x)s_1_embed('sigma_g_%(x)s_p0_1_embed+sigma_g_%(x)s_p1_1_embed*fabs(ip_%(x)s-mean_g_%(x)s_embed)', ip_%(x)s,  sigma_g_%(x)s_p0_1_embed[0,0,1], sigma_g_%(x)s_p1_1_embed[0,-1,1],mean_g_%(x)s_embed[0])" % vars(),
                   # "expr::sigma_g_%(x)s_1_embed('sigma_g_%(x)s_p0_1_embed+sigma_g_%(x)s_p1_1_embed*fabs(ip_%(x)s-mean_g_%(x)s_embed)', ip_%(x)s,  sigma_g_%(x)s_p0_1_embed[0,-1,1], sigma_g_%(x)s_p1_1_embed[0,-1,1],mean_g_%(x)s_embed[0,-0.1,0.1])" % vars(),
                   # #"expr::sigma_g_%(x)s_2_embed('sigma_g_%(x)s_p0_1_embed+sigma_g_%(x)s_p1_1_embed*fabs(-ip_%(x)s-mean_g_%(x)s_embed)', ip_%(x)s,  sigma_g_%(x)s_p0_1_embed, sigma_g_%(x)s_p1_1_embed,mean_g_%(x)s_embed)" % vars(),
                   # #"expr::sigma_g_%(x)s_1_embed('sigma_g_%(x)s_p0_1_embed+sigma_g_%(x)s_p1_1_embed*fabs(ip_%(x)s-mean_g_%(x)s_embed)', ip_%(x)s,  sigma_g_%(x)s_p0_1_embed[0,0,1], sigma_g_%(x)s_p1_1_embed[0,-1,1],mean_g_%(x)s_embed[0])" % vars(),
                   # #"expr::sigma_g_%(x)s_2_embed('sigma_g_%(x)s_p0_2_embed+sigma_g_%(x)s_p1_2_embed*fabs(ip_%(x)s-mean_g_%(x)s_embed)', ip_%(x)s,  sigma_g_%(x)s_p0_2_embed[0,0.,1], sigma_g_%(x)s_p1_2_embed[0,-1,1],mean_g_%(x)s_embed[0,-3,3])" % vars(),
                   # #"expr::sigma_g_%(x)s_embed('(ip_%(x)s>=mean_g_%(x)s_embed)*sigma_g_%(x)s_1_embed+(ip_%(x)s<mean_g_%(x)s_embed)*sigma_g_%(x)s_2_embed', ip_%(x)s,  sigma_g_%(x)s_1_embed, sigma_g_%(x)s_2_embed,mean_g_%(x)s_embed)" % vars(),
                   # "Gaussian::gaus_%(x)s_embed(ip_%(x)s, mean_g_%(x)s_embed, sigma_g_%(x)s_1_embed)" % vars(),
                   # "Gaussian::gausother_%(x)s_embed(-ip_%(x)s, mean_g_%(x)s_embed, sigma_g_%(x)s_2_embed)" % vars(),
                   # "FFTConvPdf::gaus2_%(x)s_embed(ip_%(x)s,gausother_%(x)s_embed,gaus_%(x)s_embed)" % vars(),
                   # "FFTConvPdf::model_data_%(x)s_embed(ip_%(x)s,gaus_%(x)s_embed,model_pdf_%(x)s_embed)" % vars(),

                    "expr::sigma_g_%(x)s_1('sigma_g_%(x)s_p0_1+sigma_g_%(x)s_p1_1*fabs(ip_%(x)s-mean_g_%(x)s)', ip_%(x)s,  sigma_g_%(x)s_p0_1[0,0.,0.1], sigma_g_%(x)s_p1_1[0,-1,1],mean_g_%(x)s[0,-0.01,0.01])" % vars(),
                    "expr::sigma_g_%(x)s_2('sigma_g_%(x)s_p0_1+sigma_g_%(x)s_p1_1*fabs(ip_%(x)s+mean_g_%(x)s)', ip_%(x)s,  sigma_g_%(x)s_p0_1, sigma_g_%(x)s_p1_1,mean_g_%(x)s)" % vars(),
                    #"expr::sigma_g_%(x)s_1('sigma_g_%(x)s_p0_1+sigma_g_%(x)s_p1_1*fabs(ip_%(x)s-mean_g_%(x)s)', ip_%(x)s,  sigma_g_%(x)s_p0_1[0,0.,0.1], sigma_g_%(x)s_p1_1[0,-1,1],mean_g_%(x)s[0])" % vars(),
                    #"expr::sigma_g_%(x)s_2('sigma_g_%(x)s_p0_2+sigma_g_%(x)s_p1_2*fabs(ip_%(x)s-mean_g_%(x)s)', ip_%(x)s,  sigma_g_%(x)s_p0_2[0,0.,0.1], sigma_g_%(x)s_p1_2[0,-1,1],mean_g_%(x)s[0,-0.01,0.01])" % vars(),

                    #"expr::sigma_g_%(x)s('(ip_%(x)s>=mean_g_%(x)s)*sigma_g_%(x)s_1+(ip_%(x)s<mean_g_%(x)s)*sigma_g_%(x)s_2', ip_%(x)s,  sigma_g_%(x)s_1, sigma_g_%(x)s_2,mean_g_%(x)s)" % vars(),
                    "Gaussian::gaus_%(x)s(ip_%(x)s, mean_g_%(x)s, sigma_g_%(x)s_1)" % vars(),
                    "Gaussian::gausother_%(x)s(ip_%(x)s, -mean_g_%(x)s, sigma_g_%(x)s_2)" % vars(),
                    "FFTConvPdf::gaus2_%(x)s(ip_%(x)s,gaus_%(x)s,gausother_%(x)s)" % vars(),
                    "FFTConvPdf::model_data_%(x)s(ip_%(x)s,gaus2_%(x)s,model_pdf_%(x)s)" % vars(),
                    #"expr::model_data('numTot*model_data_%(x)s(ip_%(x)s_1',numTot[1,0,1e10],model_data_%(x)s(ip_%(x)s_1)" % vars(),


                    "expr::sigma_g_%(x)s_1_embed('sigma_g_%(x)s_p0_1_embed+sigma_g_%(x)s_p1_1_embed*fabs(ip_%(x)s-mean_g_%(x)s_embed)', ip_%(x)s,  sigma_g_%(x)s_p0_1_embed[0,0,1], sigma_g_%(x)s_p1_1_embed[0,-1,1],mean_g_%(x)s_embed[0,-0.1,0.1])" % vars(),
                    "expr::sigma_g_%(x)s_2_embed('sigma_g_%(x)s_p0_1_embed+sigma_g_%(x)s_p1_1_embed*fabs(ip_%(x)s+mean_g_%(x)s_embed)', ip_%(x)s,  sigma_g_%(x)s_p0_1_embed, sigma_g_%(x)s_p1_1_embed,mean_g_%(x)s_embed)" % vars(),
                    #"expr::sigma_g_%(x)s_1_embed('sigma_g_%(x)s_p0_1_embed+sigma_g_%(x)s_p1_1_embed*fabs(ip_%(x)s-mean_g_%(x)s_embed)', ip_%(x)s,  sigma_g_%(x)s_p0_1_embed[0,0,1], sigma_g_%(x)s_p1_1_embed[0,-1,1],mean_g_%(x)s_embed[0])" % vars(),
                    #"expr::sigma_g_%(x)s_2_embed('sigma_g_%(x)s_p0_2_embed+sigma_g_%(x)s_p1_2_embed*fabs(ip_%(x)s-mean_g_%(x)s_embed)', ip_%(x)s,  sigma_g_%(x)s_p0_2_embed[0,0.,1], sigma_g_%(x)s_p1_2_embed[0,-1,1],mean_g_%(x)s_embed[0,-3,3])" % vars(),
                    #"expr::sigma_g_%(x)s_embed('(ip_%(x)s>=mean_g_%(x)s_embed)*sigma_g_%(x)s_1_embed+(ip_%(x)s<mean_g_%(x)s_embed)*sigma_g_%(x)s_2_embed', ip_%(x)s,  sigma_g_%(x)s_1_embed, sigma_g_%(x)s_2_embed,mean_g_%(x)s_embed)" % vars(),
                    "Gaussian::gaus_%(x)s_embed(ip_%(x)s, mean_g_%(x)s_embed, sigma_g_%(x)s_1_embed)" % vars(),
                    "Gaussian::gausother_%(x)s_embed(ip_%(x)s, -mean_g_%(x)s_embed, sigma_g_%(x)s_2_embed)" % vars(),
                    "FFTConvPdf::gaus2_%(x)s_embed(ip_%(x)s,gausother_%(x)s_embed,gaus_%(x)s_embed)" % vars(),
                    "FFTConvPdf::model_data_%(x)s_embed(ip_%(x)s,gaus2_%(x)s_embed,model_pdf_%(x)s_embed)" % vars(),

 
                ]
            )

      else:

        if ForceSym:
          pdf_args.extend(
                  [
                      "expr::sigma_g_%(x)s('sigma_g_%(x)s_p0_1+sigma_g_%(x)s_p1_1*fabs(ip_%(x)s-mean_g_%(x)s)', ip_%(x)s,  sigma_g_%(x)s_p0_1[0,0.,0.1], sigma_g_%(x)s_p1_1[0,-1,1],mean_g_%(x)s[0,-0.01,0.01])" % vars(),
 
                      "Gaussian::gaus_%(x)s(ip_%(x)s, mean_g_%(x)s, sigma_g_%(x)s)" % vars(),
                      "FFTConvPdf::model_data_%(x)s(ip_%(x)s,gaus_%(x)s,model_pdf_%(x)s)" % vars(),
 
                      "expr::sigma_g_%(x)s_embed('sigma_g_%(x)s_p0_1_embed+sigma_g_%(x)s_p1_1_embed*fabs(ip_%(x)s-mean_g_%(x)s_embed)', ip_%(x)s,  sigma_g_%(x)s_p0_1_embed[0,0,1], sigma_g_%(x)s_p1_1_embed[0,-1,1],mean_g_%(x)s_embed[0,-3,3])" % vars(),
                      "Gaussian::gaus_%(x)s_embed(ip_%(x)s, mean_g_%(x)s_embed, sigma_g_%(x)s_embed)" % vars(),
                      "FFTConvPdf::model_data_%(x)s_embed(ip_%(x)s,gaus_%(x)s_embed,model_pdf_%(x)s_embed)" % vars(),
 
                  ]
              )

        else: 
          pdf_args.extend(
                  [
                      "expr::sigma_g_%(x)s_1('sigma_g_%(x)s_p0_1+sigma_g_%(x)s_p1_1*fabs(ip_%(x)s-mean_g_%(x)s)', ip_%(x)s,  sigma_g_%(x)s_p0_1[0,0.,0.1], sigma_g_%(x)s_p1_1[0,-1,1],mean_g_%(x)s[0,-0.01,0.01])" % vars(),
                      "expr::sigma_g_%(x)s_2('sigma_g_%(x)s_p0_2+sigma_g_%(x)s_p1_2*fabs(ip_%(x)s-mean_g_%(x)s)', ip_%(x)s,  sigma_g_%(x)s_p0_2[0,0.,0.1], sigma_g_%(x)s_p1_2[0,-1,1],mean_g_%(x)s[0,-0.01,0.01])" % vars(),
    
                      "expr::sigma_g_%(x)s('(ip_%(x)s>=mean_g_%(x)s)*sigma_g_%(x)s_1+(ip_%(x)s<mean_g_%(x)s)*sigma_g_%(x)s_2', ip_%(x)s,  sigma_g_%(x)s_1, sigma_g_%(x)s_2,mean_g_%(x)s)" % vars(),
                      "expr::sigma_g_%(x)s('sigma_g_%(x)s_p0+sigma_g_%(x)s_p1*fabs(ip_%(x)s-mean_g_%(x)s)', ip_%(x)s,  sigma_g_%(x)s_p0[0,0.,0.1], sigma_g_%(x)s_p1[0,-1,1],mean_g_%(x)s[0,-0.01,0.01])" % vars(),
                      "Gaussian::gaus_%(x)s(ip_%(x)s, mean_g_%(x)s, sigma_g_%(x)s)" % vars(),
                      "FFTConvPdf::model_data_%(x)s(ip_%(x)s,gaus_%(x)s,model_pdf_%(x)s)" % vars(),
    
                      "expr::sigma_g_%(x)s_1_embed('sigma_g_%(x)s_p0_1_embed+sigma_g_%(x)s_p1_1_embed*fabs(ip_%(x)s-mean_g_%(x)s_embed)', ip_%(x)s,  sigma_g_%(x)s_p0_1_embed[0,0,1], sigma_g_%(x)s_p1_1_embed[0,-1,1],mean_g_%(x)s_embed[0,-3,3])" % vars(),
                      "expr::sigma_g_%(x)s_2_embed('sigma_g_%(x)s_p0_2_embed+sigma_g_%(x)s_p1_2_embed*fabs(ip_%(x)s-mean_g_%(x)s_embed)', ip_%(x)s,  sigma_g_%(x)s_p0_2_embed[0,0.,1], sigma_g_%(x)s_p1_2_embed[0,-1,1],mean_g_%(x)s_embed[0,-3,3])" % vars(),
                      "expr::sigma_g_%(x)s_embed('(ip_%(x)s>=mean_g_%(x)s_embed)*sigma_g_%(x)s_1_embed+(ip_%(x)s<mean_g_%(x)s_embed)*sigma_g_%(x)s_2_embed', ip_%(x)s,  sigma_g_%(x)s_1_embed, sigma_g_%(x)s_2_embed,mean_g_%(x)s_embed)" % vars(),
                      "Gaussian::gaus_%(x)s_embed(ip_%(x)s, mean_g_%(x)s_embed, sigma_g_%(x)s_embed)" % vars(),
                      "FFTConvPdf::model_data_%(x)s_embed(ip_%(x)s,gaus_%(x)s_embed,model_pdf_%(x)s_embed)" % vars(),
     
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
                           #ROOT.RooFit.Range(0,0.01),
                           #ROOT.RooFit.Range(-0.01,0),
                           ROOT.RooFit.Optimize(False),
                           ROOT.RooFit.Minimizer("Minuit2", "Migrad"),
                           ROOT.RooFit.Offset(True),
                           ROOT.RooFit.Extended(True),
                           ROOT.RooFit.SumW2Error(True),
                           ROOT.RooFit.PrintLevel(-1),
    )
    
    fitres = wsp.pdf("model_%(x)s" % vars()).fitTo(wsp.data('obs_%(x)s' % vars()),
                                    #ROOT.RooFit.Range(0,0.01),
                                    #ROOT.RooFit.Range(-0.01,0),
                                    ROOT.RooFit.Minimizer("Minuit2", "Migrad"),
                                    ROOT.RooFit.Optimize(False),
                                    ROOT.RooFit.Offset(True),
                                    ROOT.RooFit.Extended(True),
                                    ROOT.RooFit.PrintLevel(-1),
                                    ROOT.RooFit.SumW2Error(True), 
                                    ROOT.RooFit.Save(),
    )
    fitres.Print()

    if not doGaus or True:
      extra=''
      if doGaus: extra = '_gaus'
      canv = ROOT.TCanvas('canv', '', 0, 0, 700, 400)

      splitData = wsp.data('obs_%(x)s' % vars()).split(wsp.cat('cat'))
      xframe = wsp.var("ip_%(x)s" % vars()).frame()
      width = (wsp.var("ip_%(x)s" % vars()).getMax() - wsp.var("ip_%(x)s" % vars()).getMin()) / splitData.At(0).numEntries()
      wsp.pdf('model_data_%(x)s' % vars()).plotOn(xframe,ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('model_data_%(x)s' % vars()).plotOn(xframe,ROOT.RooFit.LineColor(ROOT.kRed),ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('model_pdf_%(x)s' % vars()).plotOn(xframe,ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('model_pdf_%(x)s' % vars()).plotOn(xframe,ROOT.RooFit.LineColor(ROOT.kBlue),ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      splitData.At(0).plotOn(xframe, ROOT.RooFit.Name("Data"),ROOT.RooFit.MarkerStyle(7))
      xframe.Draw()

      canv.Print('data_mc_fit_%(x)s%(extra)s_new.pdf' % vars())

    if not doGaus:

      splitData = wsp.data('obs_%(x)s' % vars()).split(wsp.cat('cat'))
      xframe = wsp.var("ip_%(x)s" % vars()).frame()
      width = (wsp.var("ip_%(x)s" % vars()).getMax() - wsp.var("ip_%(x)s" % vars()).getMin()) / splitData.At(1).numEntries()
      wsp.pdf('voigtian_%(x)s' % vars()).plotOn(xframe,ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('voigtian_%(x)s' % vars()).plotOn(xframe,ROOT.RooFit.LineColor(ROOT.kBlue),ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      #splitData.At(1).plotOn(xframe, ROOT.RooFit.Name("MC"),ROOT.RooFit.MarkerStyle(7))
      xframe.Draw()

      canv.Print('mc_smear_%(x)s_new.pdf' % vars())

    else: 

      splitData = wsp.data('obs_%(x)s' % vars()).split(wsp.cat('cat'))
      xframe = wsp.var("ip_%(x)s" % vars()).frame()
      width = (wsp.var("ip_%(x)s" % vars()).getMax() - wsp.var("ip_%(x)s" % vars()).getMin()) / splitData.At(1).numEntries()
      wsp.pdf('gaus_%(x)s' % vars()).plotOn(xframe,ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('gaus_%(x)s' % vars()).plotOn(xframe,ROOT.RooFit.LineColor(ROOT.kBlue),ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))

      #if ('_drx' in x or '_dry' in x or '_drz' in x):
      #  wsp.pdf('gausother_%(x)s' % vars()).plotOn(xframe,ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      #  wsp.pdf('gausother_%(x)s' % vars()).plotOn(xframe,ROOT.RooFit.LineColor(ROOT.kRed),ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))

      #splitData.At(1).plotOn(xframe, ROOT.RooFit.Name("MC"),ROOT.RooFit.MarkerStyle(7))
      xframe.Draw()

      canv.Print('mc_smear_%(x)s_gaus_new.pdf' % vars())


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

    if not doGaus or True:
      extra=''
      if doGaus: extra = '_gaus'

      canv = ROOT.TCanvas('canv', '', 0, 0, 700, 400)

      splitData = wsp.data('obs_%(x)s' % vars()).split(wsp.cat('cat'))
      xframe = wsp.var("ip_%(x)s" % vars()).frame()
      width = (wsp.var("ip_%(x)s" % vars()).getMax() - wsp.var("ip_%(x)s" % vars()).getMin()) / splitData.At(0).numEntries()
      wsp.pdf('model_data_%(x)s_embed' % vars()).plotOn(xframe,ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('model_data_%(x)s_embed' % vars()).plotOn(xframe,ROOT.RooFit.LineColor(ROOT.kRed),ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('model_pdf_%(x)s_embed' % vars()).plotOn(xframe,ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('model_pdf_%(x)s_embed' % vars()).plotOn(xframe,ROOT.RooFit.LineColor(ROOT.kBlue),ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      splitData.At(0).plotOn(xframe, ROOT.RooFit.Name("Data"),ROOT.RooFit.MarkerStyle(7))
      xframe.Draw()

      canv.Print('data_embed_fit_%(x)s%(extra)s_new.pdf' % vars())

    if not doGaus:

      splitData = wsp.data('obs_%(x)s' % vars()).split(wsp.cat('cat'))
      xframe = wsp.var("ip_%(x)s" % vars()).frame()
      width = (wsp.var("ip_%(x)s" % vars()).getMax() - wsp.var("ip_%(x)s" % vars()).getMin()) / splitData.At(1).numEntries()
      wsp.pdf('voigtian_%(x)s_embed' % vars()).plotOn(xframe,ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('voigtian_%(x)s_embed' % vars()).plotOn(xframe,ROOT.RooFit.LineColor(ROOT.kBlue),ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      #splitData.At(1).plotOn(xframe, ROOT.RooFit.Name("MC"),ROOT.RooFit.MarkerStyle(7))
      xframe.Draw()
 
      canv.Print('embed_smear_%(x)s_new.pdf' % vars())

    else:

      splitData = wsp.data('obs_%(x)s' % vars()).split(wsp.cat('cat'))
      xframe = wsp.var("ip_%(x)s" % vars()).frame()
      width = (wsp.var("ip_%(x)s" % vars()).getMax() - wsp.var("ip_%(x)s" % vars()).getMin()) / splitData.At(1).numEntries()
      wsp.pdf('gaus_%(x)s_embed' % vars()).plotOn(xframe,ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      wsp.pdf('gaus_%(x)s_embed' % vars()).plotOn(xframe,ROOT.RooFit.LineColor(ROOT.kBlue),ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      #splitData.At(1).plotOn(xframe, ROOT.RooFit.Name("MC"),ROOT.RooFit.MarkerStyle(7))

      #if ('_drx' in x or '_dry' in x or '_drz' in x):
      #  wsp.pdf('gausother_%(x)s_embed' % vars()).plotOn(xframe,ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      #  wsp.pdf('gausother_%(x)s_embed' % vars()).plotOn(xframe,ROOT.RooFit.LineColor(ROOT.kRed),ROOT.RooFit.Normalization(h_data.Integral(-1,-1),ROOT.RooAbsReal.NumEvent))
      xframe.Draw()

      canv.Print('embed_smear_%(x)s_gaus_new.pdf' % vars())

  wsfile.cd()
  wsp.Write()


wsfile = ROOT.TFile('smear_ws_2018_new.root','RECREATE')
#FitWorkspace('test',outfile,outfile,wsfile)
FitWorkspace('test',outfile,outfile,wsfile,doGaus=True)

fout = ROOT.TFile('smear_file_2018_new6.root','RECREATE')

#f = new TF1("f","TMath::Voigt(x-mean, sigma, width)",-0.01,0.01)

func = 'TMath::Voigt(x-mean, sigma, width)'
func_gaus = 'TMath::Gaus(x,mean,(x>=mean)*(sigma0_1+fabs(x-mean)*sigma1_1)+(x<mean)*(sigma0_2+fabs(x-mean)*sigma1_2))'
func_gaus_sym = 'TMath::Gaus(x,mean,sigma0_1+fabs(x-mean)*sigma1_1)'

#for x in ['ip_bs_rx','ip_bs_ry','ip_bs_rz','ip_bs_new_drx','ip_bs_new_dry','ip_bs_new_drz']:
for x in ['ip_bs_rx','ip_bs_ry','ip_bs_rz','ip_bs_new_drx','ip_bs_new_dry','ip_bs_new_drz','ip_bs_rx_eta0p0to0p9','ip_bs_ry_eta0p0to0p9','ip_bs_rz_eta0p0to0p9','ip_bs_rx_eta0p9to1p2','ip_bs_ry_eta0p9to1p2','ip_bs_rz_eta0p9to1p2','ip_bs_rx_eta1p2to2p1','ip_bs_ry_eta1p2to2p1','ip_bs_rz_eta1p2to2p1','ip_bs_rz_eta1p2to2p1_ext']:
  for i in ['','_embed']:
    wsp = wsfile.Get('wsp')
   
    ForceSym = False
    if 'eta1p2to2p1' in x and 'rz' in x: ForceSym = True
 
    x_range = 0.006
    if '_rz' in x or '_drz' in x: x_range = 0.03

    if '_drx' in x or '_dry' in x or '_drz' in x:
      wsp = wsfile.Get('wsp_gaus')
      m = '%.4e' % wsp.var('mean_g_%(x)s%(i)s' % vars()).getVal()
      s0_1 = '%.4e' % wsp.var('sigma_g_%(x)s_p0_1%(i)s' % vars()).getVal()
      s1_1 = '%.4e' % wsp.var('sigma_g_%(x)s_p1_1%(i)s' % vars()).getVal()

      f = ROOT.TF1(x+i,func_gaus_sym.replace('mean',m).replace('sigma0_1',s0_1).replace('sigma1_1',s1_1),-x_range,x_range)
      f.Write()

      print x+i
      print func_gaus_sym.replace('mean',m).replace('sigma0_1',s0_1).replace('sigma1_1',s1_1)

    else:
      wsp = wsfile.Get('wsp_gaus')
      m = '%.4e' % wsp.var('mean_g_%(x)s%(i)s' % vars()).getVal()    
      s0_1 = '%.4e' % wsp.var('sigma_g_%(x)s_p0_1%(i)s' % vars()).getVal()
      s1_1 = '%.4e' % wsp.var('sigma_g_%(x)s_p1_1%(i)s' % vars()).getVal()
      if not ForceSym:
        s0_2 = '%.4e' % wsp.var('sigma_g_%(x)s_p0_2%(i)s' % vars()).getVal()
        s1_2 = '%.4e' % wsp.var('sigma_g_%(x)s_p1_2%(i)s' % vars()).getVal()
      else:
        s0_2 = '%.4e' % wsp.var('sigma_g_%(x)s_p0_1%(i)s' % vars()).getVal()
        s1_2 = '%.4e' % wsp.var('sigma_g_%(x)s_p1_1%(i)s' % vars()).getVal()
 
      f = ROOT.TF1(x+i,func_gaus.replace('mean',m).replace('sigma0_1',s0_1).replace('sigma1_1',s1_1).replace('sigma0_2',s0_2).replace('sigma1_2',s1_2),-x_range,x_range)
      f.Write()

      print x+i
      print func_gaus.replace('mean',m).replace('sigma0_1',s0_1).replace('sigma1_1',s1_1).replace('sigma0_2',s0_2).replace('sigma1_2',s1_2)

fout.Close()
wsfile.Close()
