import ROOT
import os
import glob
import json
from UserCode.ICHiggsTauTau.analysis import *
from UserCode.ICHiggsTauTau.uncertainties import ufloat
from optparse import OptionParser

CHANNELS= ['et', 'mt', 'em','tt']
ANALYSIS= ['sm','mssm']

parser = OptionParser()
parser.add_option("--channel", dest="channel", type='string', default='mt',
    help="Tau decay channel to process.  Supported channels: %(CHANNELS)s" % vars())
parser.add_option("-o", "--outputfolder", dest="output_folder", type='string', default='output',
    help="Name of output folder")
parser.add_option("-i", "--inputfolder", dest="input_folder", type='string', default='/vols/cms/dw515/Offline/output/Dec08',
    help="Name of input folder")
parser.add_option("-p", "--paramfile", dest="param_file", type='string', default='scripts/Params_2016_spring16.json',
    help="Name of parameter file")
parser.add_option("-c", "--cat", dest="cat", type='string', default='inclusive',
    help="Category")
parser.add_option("-y", "--year", dest="year", type='string', default='2016',
    help="Year")
parser.add_option("-s", "--sel", dest="sel", type='string', default='',
    help="Selection")
parser.add_option("-a", "--analysis", dest="analysis", type='string', default='sm',
    help="Analysis.  Supported options: %(CHANNELS)s" % vars())
(options, args) = parser.parse_args()

print '################### Options ###################'
print 'channel      = ' + options.channel
print 'outputfolder = ' + options.output_folder
print 'inputfolder  = ' + options.input_folder
print 'paramfile    = ' + options.param_file
print 'cat          = ' + options.cat
print 'year         = ' + options.year
print 'sel          = ' + options.sel
print 'analysis     = ' + options.analysis
print '###############################################'

ROOT.TH1.SetDefaultSumw2(True)

ana = Analysis()

ana.remaps = {
    'SingleMuon': 'data_obs'
}

z_sels = {}
if options.channel == 'et':
    z_sels['ztt_sel'] = '(gen_match_2==5)'
    z_sels['zl_sel'] = '(gen_match_2<5)'
    z_sels['zj_sel'] = '(gen_match_2==6)'
elif options.channel == 'mt':
    z_sels['ztt_sel'] = '(gen_match_2==5)'
    z_sels['zl_sel'] = '(gen_match_2<5)'
    z_sels['zj_sel'] = '(gen_match_2==6)'
elif options.channel == 'tt':
    z_sels['ztt_sel'] = '(gen_match_1==5&&gen_match_2==5)'
    z_sels['zl_sel'] = '(gen_match_2<6&&gen_match_1<6&&!(gen_match_1==5&&gen_match_2==5))'
    z_sels['zj_sel'] = '(gen_match_2==6||gen_match_1==6)'
elif options.channel == 'em':
    z_sels['ztt_sel'] = '(gen_match_1>2 && gen_match_2>3)'
    z_sels['zll_sel'] = '(gen_match_1<3 || gen_match_2<4)'
    
cats = {}

if options.analysis == 'sm':
    if options.channel == 'mt':
        cats['baseline'] = '(iso_1<0.15 && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
    elif options.channel == 'et': 
        cats['baseline'] = '(iso_1<0.1  && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
elif options.analysis == 'mssm':
    if options.channel == 'mt':        
        cats['baseline'] = '(iso_1<0.15 && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
    elif options.channel == 'et':
        cats['baseline'] = '(iso_1<0.1  && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'
if options.channel == 'tt':
    cats['baseline'] = '(mva_olddm_tight_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)'
elif options.channel == 'em':
    cats['baseline'] = '(iso_1<0.15 && iso_2<0.2 && !leptonveto)'

cats['inclusive'] = '(1)'    
    
# Add data sample names
if options.channel == 'mt': 
    data_samples = ['SingleMuonB','SingleMuonC','SingleMuonD']
if options.channel == 'em': 
    data_samples = ['MuonEGB','MuonEGC','MuonEGD']
if options.channel == 'et': 
    data_samples = ['SingleElectronB','SingleElectronC','SingleElectronD']
if options.channel == 'tt': 
    data_samples = ['TauB','TauC','TauD']
    
# Add MC sample names   
ztt_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO']
vv_samples = ['T-tW', 'Tbar-tW','Tbar-t','T-t','WWTo1L1Nu2Q','WZJToLLLNu','VVTo2L2Nu','ZZTo2L2Q','ZZTo4L','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q']
wgam_samples = ['WGToLNuG','WGstarToLNuEE','WGstarToLNuMuMu']
top_samples = ['TT']
ztt_shape_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO']

if options.channel == 'em': 
    qcd_sub_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','T-tW', 'Tbar-tW', 'Tbar-t', 'T-t','WWTo1L1Nu2Q','VVTo2L2Nu', 'ZZTo4L','ZZTo2L2Q','WZJToLLLNu','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','TT','WJetsToLNu-LO','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO']
else:
    qcd_sub_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','ZZTo4L','T-tW','T-t','Tbar-tW','Tbar-t','WWTo1L1Nu2Q','VVTo2L2Nu','ZZTo2L2Q','WZJToLLLNu','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','TT','WJetsToLNu-LO','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO']
    
w_sub_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','T-tW', 'Tbar-tW', 'Tbar-t','T-t','WWTo1L1Nu2Q','VVTo2L2Nu','ZZTo4L','ZZTo2L2Q','WZJToLLLNu','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','TT']
wjets_samples = ['WJetsToLNu-LO','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO']

# Add all data and MC filenames
for sample_name in ztt_samples + data_samples + vv_samples + wgam_samples + top_samples + ztt_shape_samples + qcd_sub_samples + w_sub_samples + wjets_samples:
    ana.AddSamples(options.input_folder+'/'+sample_name+'*'+options.channel+'*.root', 'ntuple', None,sample_name)
ana.AddInfo(options.param_file, scaleTo='data_obs')

cat = '('+cats[options.cat]+')*('+cats['baseline']+')'
sel = options.sel
plot = 'm_vis(7,0,140)' # set this under option
plot = 'os(2,0,2)'
nodename = plot.split('(')[0]

def GenerateZ(ana, samples=[], plot='', wt='', sel='', cat='', z_sels={}):
    full_selection = BuildCutString(wt, sel, cat, 'os', z_sels['ztt_sel'])
    ana.nodes[nodename].AddNode(ana.SummedFactory('ZTT', ztt_samples, plot, full_selection))
    if options.channel == 'em':
        full_selection = BuildCutString(wt, sel, cat, 'os', z_sels['zll_sel'])
        ana.nodes[nodename].AddNode(ana.SummedFactory('ZLL', ztt_samples, plot, full_selection))
    else:
        full_selection = BuildCutString(wt, sel, cat, 'os', z_sels['zl_sel'])
        ana.nodes[nodename].AddNode(ana.SummedFactory('ZL', ztt_samples, plot, full_selection))
        full_selection = BuildCutString(wt, sel, cat, 'os', z_sels['zj_sel'])
        ana.nodes[nodename].AddNode(ana.SummedFactory('ZJ', ztt_samples, plot, full_selection))
        
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

ana.nodes.AddNode(ListNode(nodename))

full_selection = BuildCutString('wt', sel, cat)
print full_selection
ana.nodes[nodename].AddNode(ana.SummedFactory('data_obs', data_samples, plot, full_selection))
#ana.nodes[nodename].AddNode(ana.SummedFactory('MC', ztt_samples+top_samples+vv_samples+w_sub_samples+wjets_samples, plot, sel))

GenerateZ(ana, ztt_samples, plot, 'wt', sel, cat, z_sels)                                 

ana.Run()
ana.nodes.PrintTree()
output_name = options.output_folder+"/"+nodename+"_"+options.cat+"_"+options.channel+".root"
outfile = ROOT.TFile(output_name, 'RECREATE')

ana.nodes.Output(outfile)

