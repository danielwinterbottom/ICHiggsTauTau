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

CHANNELS= ['tpzmm','tpzee']

conf_parser = argparse.ArgumentParser(
    description=__doc__,
    formatter_class=argparse.RawDescriptionHelpFormatter,
    add_help=False
    )
conf_parser.add_argument("--cfg",
                    help="Specify config file", metavar="FILE")
options, remaining_argv = conf_parser.parse_known_args()

defaults = { "channel":"tpzmm" , "outputfolder":"output", "folder":"/vols/cms/dw515/Offline/output/MSSM/TagAndProbe/" , "paramfile":"scripts/Params_2016_spring16.json","era":"mssmsummer16" }

if options.cfg:
    config = ConfigParser.SafeConfigParser()
    config.read([options.cfg])
    defaults.update(dict(config.items("Defaults")))

parser = argparse.ArgumentParser(
    parents=[conf_parser]
    )
parser.set_defaults(**defaults)
parser.add_argument("--channel", dest="channel", type=str,
    help="Tau decay channel to process.  Supported channels: %(CHANNELS)s" % vars())
parser.add_argument("--outputfolder", dest="outputfolder", type=str,
    help="Name of output folder")
parser.add_argument("--folder", dest="folder", type=str,
    help="Name of input folder")
parser.add_argument("--paramfile", dest="paramfile", type=str,
    help="Name of parameter file")
parser.add_argument("--era", dest="era", type=str,
    help="Era")

options = parser.parse_args(remaining_argv)   

print ''
print '################### Options ###################'
print 'channel           = ' + options.channel
print 'outputfolder      = ' + options.outputfolder
print 'folder            = ' + options.folder
print 'paramfile         = ' + options.paramfile
print 'era               = ' + options.era
print '###############################################'
print ''

# Add data sample names
if options.channel == 'tpzmm': 
    data_samples = ['SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF','SingleMuonG','SingleMuonHv2','SingleMuonHv3']
if  options.channel == 'tpzee': 
    data_samples = ['SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF','SingleElectronG','SingleElectronHv2','SingleElectronHv3']

# Add MC sample names   
ztt_samples = ['DYJetsToLL-LO-ext1','DYJetsToLL-LO-ext2','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO']

ROOT.TH1.SetDefaultSumw2(True)

trg_probe_1 = '(trg_probe_1)'
trg_probe_2 = '(trg_probe_2)'
id_probe_1 = '(id_probe_1)'
id_probe_2 = '(id_probe_2)'
if options.channel == 'tpzmm':
  baseline_tag1 = '(m_vis>70&&m_vis<110&&pt_1>25&&abs(eta_1)<2.1&&iso_1<0.15&&id_tag_1&&trg_tag_1)'
  baseline_tag2 = '(m_vis>70&&m_vis<110&&pt_2>25&&abs(eta_2)<2.1&&iso_2<0.15&&id_tag_2&&trg_tag_2)'
  iso_probe_1 = '(iso_1<0.15)'
  iso_probe_2 = '(iso_2<0.15)'
  idiso_probe_1 = '(id_probe_1&&iso_1<0.15)'
  idiso_probe_2 = '(id_probe_2&&iso_2<0.15)'
  trg_tag_1 = baseline_tag1+'*(iso_2<0.15&&id_tag_2)'
  trg_tag_2 = baseline_tag2+'*(iso_1<0.15&&id_tag_1)'
  id_tag_1 = baseline_tag1+'*(iso_2<0.15)'
  id_tag_2 = baseline_tag2+'*(iso_1<0.15)'
if options.channel == 'tpzee':
  baseline_tag1 = '(m_vis>70&&m_vis<110&&pt_1>25&&abs(eta_1)<2.1&&iso_1<0.1&&id_tag_1&&trg_tag_1)'
  baseline_tag2 = '(m_vis>70&&m_vis<110&&pt_2>25&&abs(eta_2)<2.1&&iso_2<0.1&&id_tag_2&&trg_tag_2)'
  iso_probe_1 = '(iso_1<0.1)'
  iso_probe_2 = '(iso_2<0.1)'
  idiso_probe_1 = '(id_probe_1&&iso_1<0.1)'
  idiso_probe_2 = '(id_probe_2&&iso_2<0.1)'
  trg_tag_1 = baseline_tag1+'*(iso_2<0.1&&id_tag_2)'
  trg_tag_2 = baseline_tag2+'*(iso_1<0.1&&id_tag_1)'
  id_tag_1 = baseline_tag1+'*(iso_2<0.1)'
  id_tag_2 = baseline_tag2+'*(iso_1<0.1)'

iso_tag_1 = baseline_tag1+'*(id_tag_2)'
iso_tag_2 = baseline_tag2+'*(id_tag_1)'
# All functions defined here

def BuildCutString(wt='', sel='', cat='', sign='os',bkg_sel=''):
    full_selection = '(1)'
    if wt != '':
        full_selection = '('+wt+')'
    if sel != '':
        full_selection += '*('+sel+')'
    if sign != '':
        full_selection += '*('+sign+')'
    if bkg_sel != '':
        full_selection += '*('+bkg_sel+')'
    if cat != '':
        full_selection += '*('+cat+')'
    return full_selection

def GetZLLNode(ana, add_name='', samples=[], plot='', wt='', sel='', cat=''):
    full_selection = BuildCutString(wt, sel, cat, 'os', '1')
    return ana.SummedFactory('ZLL'+add_name, samples, plot, full_selection)

def GetDataNode(ana, add_name='', samples=[], plot='', wt='', sel='', cat=''):
    full_selection = BuildCutString(wt, sel, cat, 'os', '1')
    return ana.SummedFactory('data'+add_name, samples, plot, full_selection)

def GenerateZLL(ana, add_name='', samples=[], plot='', wt='', sel='', cat=''):
    zll_node = GetZLLNode(ana, add_name, samples, plot, wt, sel, cat)
    ana.nodes[nodename].AddNode(zll_node)

def GenerateData(ana, add_name='', samples=[], plot='', wt='', sel='', cat=''):
    data_node = GetDataNode(ana, add_name, samples, plot, wt, sel, cat)
    ana.nodes[nodename].AddNode(data_node)
    
def RunTagAndProbePlotting(ana, wt='wt', outfile=None):
    
    if options.channel == 'tpzmm':
      idiso_pt_bins = '[10,15,20,25,30,40,50,60,80,100,200,1000]'
      idiso_eta_bins = '[0,0.9,1.2,2.1,2.4]'
      trg_pt_bins = '[10,13,16,17,18,19,20,21,22,23,24,25,26,27,28,31,34,37,40,45,50,60,70,100,200,1000]'
      trg_eta_bins = '[0,0.9,1.2,2.1,2.4]'
    
    if options.channel == 'tpzee':
      idiso_pt_bins = '[10,20,25,30,40,50,100,200,1000]'
      idiso_eta_bins = '[0,1,1.4442,1.56,2.1,2.5]'
      trg_pt_bins = '[10,20,22,24,26,28,30,40,50,100,200,1000]'
      trg_eta_bins = '[0,1,1.4442,1.56,2.1,2.5]'
    
    trg_plot_probe_1 = 'eta_1,pt_1'+trg_eta_bins+','+trg_pt_bins
    trg_plot_probe_2 = 'eta_2,pt_2'+trg_eta_bins+','+trg_pt_bins
    idiso_plot_probe_1 = 'eta_1,pt_1'+idiso_eta_bins+','+idiso_pt_bins
    idiso_plot_probe_2 = 'eta_2,pt_2'+idiso_eta_bins+','+idiso_pt_bins

    GenerateZLL(ana, '_trg_tag1_denum', ztt_samples, trg_plot_probe_2, wt, trg_tag_1, '1')
    GenerateZLL(ana, '_trg_tag2_denum', ztt_samples, trg_plot_probe_1, wt, trg_tag_2, '1')
    GenerateZLL(ana, '_trg_tag1_num', ztt_samples, trg_plot_probe_2, wt, trg_tag_1, trg_probe_2)
    GenerateZLL(ana, '_trg_tag2_num', ztt_samples, trg_plot_probe_1, wt, trg_tag_2, trg_probe_1)    
    GenerateData(ana, '_trg_tag1_denum', data_samples, trg_plot_probe_2, wt, trg_tag_1, '1')
    GenerateData(ana, '_trg_tag2_denum', data_samples, trg_plot_probe_1, wt, trg_tag_2, '1')
    GenerateData(ana, '_trg_tag1_num', data_samples, trg_plot_probe_2, wt, trg_tag_1, trg_probe_2)
    GenerateData(ana, '_trg_tag2_num', data_samples, trg_plot_probe_1, wt, trg_tag_2, trg_probe_1)
    
    GenerateZLL(ana, '_id_tag1_denum', ztt_samples, idiso_plot_probe_2, wt, id_tag_1, '1')
    GenerateZLL(ana, '_id_tag2_denum', ztt_samples, idiso_plot_probe_1, wt, id_tag_2, '1')
    GenerateZLL(ana, '_id_tag1_num', ztt_samples, idiso_plot_probe_2, wt, id_tag_1, id_probe_2)
    GenerateZLL(ana, '_id_tag2_num', ztt_samples, idiso_plot_probe_1, wt, id_tag_2, id_probe_1)    
    GenerateData(ana, '_id_tag1_denum', data_samples, idiso_plot_probe_2, wt, id_tag_1, '1')
    GenerateData(ana, '_id_tag2_denum', data_samples, idiso_plot_probe_1, wt, id_tag_2, '1')
    GenerateData(ana, '_id_tag1_num', data_samples, idiso_plot_probe_2, wt, id_tag_1, id_probe_2)
    GenerateData(ana, '_id_tag2_num', data_samples, idiso_plot_probe_1, wt, id_tag_2, id_probe_1)
    
    GenerateZLL(ana, '_iso_tag1_denum', ztt_samples, idiso_plot_probe_2, wt, iso_tag_1, '1')
    GenerateZLL(ana, '_iso_tag2_denum', ztt_samples, idiso_plot_probe_1, wt, iso_tag_2, '1')
    GenerateZLL(ana, '_iso_tag1_num', ztt_samples, idiso_plot_probe_2, wt, iso_tag_1, iso_probe_2)
    GenerateZLL(ana, '_iso_tag2_num', ztt_samples, idiso_plot_probe_1, wt, iso_tag_2, iso_probe_1)   
    GenerateData(ana, '_iso_tag1_denum', data_samples, idiso_plot_probe_2, wt, iso_tag_1, '1')
    GenerateData(ana, '_iso_tag2_denum', data_samples, idiso_plot_probe_1, wt, iso_tag_2, '1')
    GenerateData(ana, '_iso_tag1_num', data_samples, idiso_plot_probe_2, wt, iso_tag_1, iso_probe_2)
    GenerateData(ana, '_iso_tag2_num', data_samples, idiso_plot_probe_1, wt, iso_tag_2, iso_probe_1)
    
    GenerateZLL(ana, '_idiso_tag1_denum', ztt_samples, idiso_plot_probe_2, wt, baseline_tag1, '1')
    GenerateZLL(ana, '_idiso_tag2_denum', ztt_samples, idiso_plot_probe_1, wt, baseline_tag2, '1')
    GenerateZLL(ana, '_idiso_tag1_num', ztt_samples, idiso_plot_probe_2, wt, baseline_tag1, idiso_probe_2)
    GenerateZLL(ana, '_idiso_tag2_num', ztt_samples, idiso_plot_probe_1, wt, baseline_tag2, idiso_probe_1)    
    GenerateData(ana, '_idiso_tag1_denum', data_samples, idiso_plot_probe_2, wt, baseline_tag1, '1')
    GenerateData(ana, '_idiso_tag2_denum', data_samples, idiso_plot_probe_1, wt, baseline_tag2, '1')
    GenerateData(ana, '_idiso_tag1_num', data_samples, idiso_plot_probe_2, wt, baseline_tag1, idiso_probe_2)
    GenerateData(ana, '_idiso_tag2_num', data_samples, idiso_plot_probe_1, wt, baseline_tag2, idiso_probe_1)
    
    ana.Run()
    ana.nodes.Output(outfile)
    
    zll_trg_denum = outfile.Get(nodename+'/ZLL_trg_tag1_denum')
    zll_trg_denum.Add(outfile.Get(nodename+'/ZLL_trg_tag2_denum'))
    zll_trg_num = outfile.Get(nodename+'/ZLL_trg_tag1_num')
    zll_trg_num.Add(outfile.Get(nodename+'/ZLL_trg_tag2_num'))
    
    data_trg_denum = outfile.Get(nodename+'/data_trg_tag1_denum')
    data_trg_denum.Add(outfile.Get(nodename+'/data_trg_tag2_denum'))
    data_trg_num = outfile.Get(nodename+'/data_trg_tag1_num')
    data_trg_num.Add(outfile.Get(nodename+'/data_trg_tag2_num'))
    
    zll_id_denum = outfile.Get(nodename+'/ZLL_id_tag1_denum')
    zll_id_denum.Add(outfile.Get(nodename+'/ZLL_id_tag2_denum'))
    zll_id_num = outfile.Get(nodename+'/ZLL_id_tag1_num')
    zll_id_num.Add(outfile.Get(nodename+'/ZLL_id_tag2_num'))
    
    data_id_denum = outfile.Get(nodename+'/data_id_tag1_denum')
    data_id_denum.Add(outfile.Get(nodename+'/data_id_tag2_denum'))
    data_id_num = outfile.Get(nodename+'/data_id_tag1_num')
    data_id_num.Add(outfile.Get(nodename+'/data_id_tag2_num'))
    
    zll_iso_denum = outfile.Get(nodename+'/ZLL_iso_tag1_denum')
    zll_iso_denum.Add(outfile.Get(nodename+'/ZLL_iso_tag2_denum'))
    zll_iso_num = outfile.Get(nodename+'/ZLL_iso_tag1_num')
    zll_iso_num.Add(outfile.Get(nodename+'/ZLL_iso_tag2_num'))
    
    data_iso_denum = outfile.Get(nodename+'/data_iso_tag1_denum')
    data_iso_denum.Add(outfile.Get(nodename+'/data_iso_tag2_denum'))
    data_iso_num = outfile.Get(nodename+'/data_iso_tag1_num')
    data_iso_num.Add(outfile.Get(nodename+'/data_iso_tag2_num'))
    
    zll_idiso_denum = outfile.Get(nodename+'/ZLL_idiso_tag1_denum')
    zll_idiso_denum.Add(outfile.Get(nodename+'/ZLL_idiso_tag2_denum'))
    zll_idiso_num = outfile.Get(nodename+'/ZLL_idiso_tag1_num')
    zll_idiso_num.Add(outfile.Get(nodename+'/ZLL_idiso_tag2_num'))
    
    data_idiso_denum = outfile.Get(nodename+'/data_idiso_tag1_denum')
    data_idiso_denum.Add(outfile.Get(nodename+'/data_idiso_tag2_denum'))
    data_idiso_num = outfile.Get(nodename+'/data_idiso_tag1_num')
    data_idiso_num.Add(outfile.Get(nodename+'/data_idiso_tag2_num'))
    
    data_trg_num_proj   = data_trg_num.ProjectionX()
    zll_trg_num_proj    = zll_trg_num.ProjectionX()
    data_id_num_proj    = data_id_num.ProjectionX()
    zll_id_num_proj     = zll_id_num.ProjectionX()
    data_iso_num_proj   = data_iso_num.ProjectionX()
    zll_iso_num_proj    = zll_iso_num.ProjectionX()
    data_idiso_num_proj = data_idiso_num.ProjectionX()
    zll_idiso_num_proj  = zll_idiso_num.ProjectionX()
    
    data_trg_denum_proj   = data_trg_denum.ProjectionX()
    zll_trg_denum_proj    = zll_trg_denum.ProjectionX()
    data_id_denum_proj    = data_id_denum.ProjectionX()
    zll_id_denum_proj     = zll_id_denum.ProjectionX()
    data_iso_denum_proj   = data_iso_denum.ProjectionX()
    zll_iso_denum_proj    = zll_iso_denum.ProjectionX()
    data_idiso_denum_proj = data_idiso_denum.ProjectionX()
    zll_idiso_denum_proj  = zll_idiso_denum.ProjectionX()
    
    data_trg_gr   = ROOT.TGraphAsymmErrors(data_trg_num.GetNbinsX())
    data_iso_gr   = ROOT.TGraphAsymmErrors(data_iso_num.GetNbinsX())
    data_id_gr    = ROOT.TGraphAsymmErrors(data_id_num.GetNbinsX())
    data_idiso_gr = ROOT.TGraphAsymmErrors(data_idiso_num.GetNbinsX())
    mc_trg_gr   = ROOT.TGraphAsymmErrors(zll_trg_num.GetNbinsX())
    mc_iso_gr   = ROOT.TGraphAsymmErrors(zll_iso_num.GetNbinsX())
    mc_id_gr    = ROOT.TGraphAsymmErrors(zll_id_num.GetNbinsX())
    mc_idiso_gr = ROOT.TGraphAsymmErrors(zll_idiso_num.GetNbinsX())
    
    data_trg_gr.Divide(data_trg_num_proj,data_trg_denum_proj,"n")
    mc_trg_gr.Divide(zll_trg_num_proj,zll_trg_denum_proj,"n") 
    data_id_gr.Divide(data_id_num_proj,data_id_denum_proj,"n")
    mc_id_gr.Divide(zll_id_num_proj,zll_id_denum_proj,"n")
    data_iso_gr.Divide(data_iso_num_proj,data_iso_denum_proj,"n")
    mc_iso_gr.Divide(zll_iso_num_proj,zll_iso_denum_proj,"n")
    data_idiso_gr.Divide(data_idiso_num_proj,data_idiso_denum_proj,"n")
    mc_idiso_gr.Divide(zll_idiso_num_proj,zll_idiso_denum_proj,"n")
    
    if options.channel == 'tpzmm': sfs_output_name = options.outputfolder+'/muon_SFs.root'
    if options.channel == 'tpzee': sfs_output_name = options.outputfolder+'/electron_SFs.root'
    sfs_output = ROOT.TFile(sfs_output_name, 'RECREATE')
    sfs_output.cd()
    
    data_trg_gr.Write('data_trg_proj')
    mc_trg_gr.Write('mc_trg_proj')
    data_id_gr.Write('data_id_proj') 
    mc_id_gr.Write('mc_id_proj') 
    data_iso_gr.Write('data_iso_proj')
    mc_iso_gr.Write('mc_iso_proj')
    data_idiso_gr.Write('data_idiso_proj')
    mc_idiso_gr.Write('mc_idiso_proj')
    
    trg_graphs = [sfs_output.Get('data_trg_proj'),sfs_output.Get('mc_trg_proj')]
    id_graphs = [sfs_output.Get('data_id_proj'),sfs_output.Get('mc_id_proj')]
    iso_graphs = [sfs_output.Get('data_iso_proj'),sfs_output.Get('mc_iso_proj')]
    idiso_graphs = [sfs_output.Get('data_idiso_proj'),sfs_output.Get('mc_idiso_proj')]
    if options.channel == 'tpzmm': 
        x_title = 'P_{T}^{#mu} (GeV)'
        plot_name = 'muon_efficiency_'
    if options.channel == 'tpzee':
        x_title = 'P_{T}^{e} (GeV)'
        plot_name = 'electron_efficiency_'
    plotting.TagAndProbePlot(trg_graphs,['data','MC'],"",True,False,True,"0.85,1.15",True,1000,0,False,0,1,x_title, "Efficiency",0,plot_name+'trg',"trigger")
    plotting.TagAndProbePlot(id_graphs,['data','MC'],"",True,False,True,"0.85,1.15",True,1000,0,False,0,1,x_title, "Efficiency",0,plot_name+'id',"ID")
    plotting.TagAndProbePlot(iso_graphs,['data','MC'],"",True,False,True,"0.85,1.15",True,1000,0,False,0,1,x_title, "Efficiency",0,plot_name+'iso',"isolation")
    plotting.TagAndProbePlot(idiso_graphs,['data','MC'],"",True,False,True,"0.85,1.15",True,1000,0,False,0,1,x_title, "Efficiency",0,plot_name+'idiso',"ID-isolation")

    
    data_trg_num.Divide(data_trg_denum)
    zll_trg_num.Divide(zll_trg_denum)
    data_id_num.Divide(data_id_denum)
    zll_id_num.Divide(zll_id_denum)
    data_iso_num.Divide(data_iso_denum)
    zll_iso_num.Divide(zll_iso_denum)
    data_idiso_num.Divide(data_idiso_denum)
    zll_idiso_num.Divide(zll_idiso_denum)
    
    data_trg_num.SetName("trg_data")
    zll_trg_num.SetName("trg_mc")
    data_id_num.SetName("id_data")
    zll_id_num.SetName("id_mc")
    data_iso_num.SetName("iso_data")
    zll_iso_num.SetName("iso_mc")
    data_idiso_num.SetName("idiso_data")
    zll_idiso_num.SetName("idiso_mc")
    
    hists = [data_trg_num,zll_trg_num,data_id_num,zll_id_num,data_iso_num,zll_iso_num,data_idiso_num,zll_idiso_num]
    
    # set overflow pT bins to same value as last bin and write to file
    for hist in hists:
      for i in range(1,hist.GetNbinsY()+1): 
        hist.SetBinContent(hist.GetNbinsX()+1,i,hist.GetBinContent(hist.GetNbinsX(),i))
      hist.Write()

# Create output file

output_name = options.outputfolder+'/tagandprobe_'+options.channel+'.root'
outfile = ROOT.TFile(output_name, 'RECREATE')

nodename = options.channel
ana = Analysis()
ana.nodes.AddNode(ListNode(nodename))

ana.remaps = {}
if options.channel =='tpzmm': ana.remaps['SingleMuon'] = 'data_obs'
elif options.channel == 'tpzee': ana.remaps['SingleElectron'] = 'data_obs'


# Add all data files
for sample_name in data_samples:
    ana.AddSamples(options.folder+'/'+sample_name+'_'+options.channel+'*.root', 'tagandprobe', None, sample_name)

# Add all MC background files
for sample_name in ztt_samples:
    print options.folder+'/'+sample_name+'_'+options.channel+'*.root'
    ana.AddSamples(options.folder+'/'+sample_name+'_'+options.channel+'*.root', 'tagandprobe', None, sample_name)

RunTagAndProbePlotting(ana, 'wt', outfile)

 