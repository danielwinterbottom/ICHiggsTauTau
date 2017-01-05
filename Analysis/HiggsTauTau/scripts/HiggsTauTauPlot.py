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

top_sels = {}
top_sels['ttt_sel'] = z_sels['ztt_sel']
top_sels['ttj_sel'] = '!('+z_sels['ztt_sel']+')'

vv_sels = {}
vv_sels['vvt_sel'] = z_sels['ztt_sel']
vv_sels['vvj_sel'] = '!('+z_sels['ztt_sel']+')'
    
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

cats['w_os'] = 'os'
cats['w_sdb'] = 'mt_1>70.'
cats['w_sdb_os'] = 'os'
    
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
    #qcd_sub_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','T-tW', 'Tbar-tW', 'Tbar-t', 'T-t','WWTo1L1Nu2Q','VVTo2L2Nu', 'ZZTo4L','ZZTo2L2Q','WZJToLLLNu','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','TT','WJetsToLNu-LO','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO']
    qcd_sub_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','T-tW', 'Tbar-tW', 'Tbar-t', 'T-t','WWTo1L1Nu2Q','VVTo2L2Nu', 'ZZTo4L','ZZTo2L2Q','WZJToLLLNu','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','TT']
else:
    #qcd_sub_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','ZZTo4L','T-tW','T-t','Tbar-tW','Tbar-t','WWTo1L1Nu2Q','VVTo2L2Nu','ZZTo2L2Q','WZJToLLLNu','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','TT','WJetsToLNu-LO','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO']
    qcd_sub_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','ZZTo4L','T-tW','T-t','Tbar-tW','Tbar-t','WWTo1L1Nu2Q','VVTo2L2Nu','ZZTo2L2Q','WZJToLLLNu','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','TT']
    
w_sub_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','T-tW', 'Tbar-tW', 'Tbar-t','T-t','WWTo1L1Nu2Q','VVTo2L2Nu','ZZTo4L','ZZTo2L2Q','WZJToLLLNu','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','TT']
wjets_samples = ['WJetsToLNu-LO','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO']

# Add all data and MC filenames
for sample_name in ztt_samples + data_samples + vv_samples + wgam_samples + top_samples + ztt_shape_samples + qcd_sub_samples + w_sub_samples + wjets_samples:
    ana.AddSamples(options.input_folder+'/'+sample_name+'_'+options.channel+'*.root', 'ntuple', None, sample_name)
ana.AddInfo(options.param_file, scaleTo='data_obs')

cat = '('+cats[options.cat]+')*('+cats['baseline']+')'
sel = options.sel
#plot = 'm_vis(7,0,140)' # set this under option
plot = 'os(2,0,2)'
nodename = plot.split('(')[0]

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

def GenerateZ(ana, samples=[], plot='', wt='', sel='', cat='', z_sels={}):
    full_selection = BuildCutString(wt, sel, cat, 'os', z_sels['ztt_sel'])
    ana.nodes[nodename].AddNode(ana.SummedFactory('ZTT', samples, plot, full_selection))
    if options.channel == 'em':
        full_selection = BuildCutString(wt, sel, cat, 'os', z_sels['zll_sel'])
        ana.nodes[nodename].AddNode(ana.SummedFactory('ZLL', samples, plot, full_selection))
    else:
        full_selection = BuildCutString(wt, sel, cat, 'os', z_sels['zl_sel'])
        ana.nodes[nodename].AddNode(ana.SummedFactory('ZL', samples, plot, full_selection))
        full_selection = BuildCutString(wt, sel, cat, 'os', z_sels['zj_sel'])
        ana.nodes[nodename].AddNode(ana.SummedFactory('ZJ', samples, plot, full_selection))
        
def GenerateTop(ana, samples=[], plot='', wt='', sel='', cat='', top_sels={}):
  full_selection = BuildCutString(wt, sel, cat, 'os', top_sels['ttt_sel'])
  ana.nodes[nodename].AddNode(ana.SummedFactory('TTT', samples, plot, full_selection))
  full_selection = BuildCutString(wt, sel, cat, 'os', top_sels['ttj_sel'])
  ana.nodes[nodename].AddNode(ana.SummedFactory('TTJ', samples, plot, full_selection))

def GenerateVV(ana, samples=[], plot='', wt='', sel='', cat='', vv_sels={}):
  full_selection = BuildCutString(wt, sel, cat, 'os', vv_sels['vvt_sel'])
  ana.nodes[nodename].AddNode(ana.SummedFactory('VVT', samples, plot, full_selection))
  full_selection = BuildCutString(wt, sel, cat, 'os', vv_sels['vvj_sel'])
  ana.nodes[nodename].AddNode(ana.SummedFactory('VVJ', samples, plot, full_selection))
  
def GenerateWG(ana, samples=[], plot='', wt='', sel='', cat='', wg_sel=''):
  full_selection = BuildCutString(wt, sel, cat, 'os', wg_sel)
  ana.nodes[nodename].AddNode(ana.SummedFactory('WGam', samples, plot, full_selection))

def GetWNode(ana, name='W', samples=[], data=[], sub_samples=[], plot='', wt='', sel='', cat='', method=8, qcd_factor=1.18, get_os=True):
  if get_os:
      OSSS = 'os'
  else:
      OSSS = '!os'
  full_selection = BuildCutString(wt, sel, cat, OSSS, '')
  shape_cat = cat
  if method == 14:
      shape_cat = '(n_jets<=1 && n_loose_bjets>=1)*('+cats['baseline']+')'
  shape_selection = BuildCutString(wt, sel, shape_cat, 'os', '')
  
  if method in [8, 9, 15]:
      w_node = ana.SummedFactory(name, samples, plot, full_selection)
  elif method in [10, 11]:
      control_sel = cats['w_sdb']+' && '+ OSSS
      w_control_full_selection = BuildCutString('wt', control_sel, cat, OSSS)
      w_node = HttWNode(name,
        ana.SummedFactory('data_obs', data, plot, w_control_full_selection),
        ana.SummedFactory('backgrounds', sub_samples, plot, w_control_full_selection),
        ana.SummedFactory('w_control', samples, plot, w_control_full_selection),
        ana.SummedFactory('w_signal', samples, plot, full_selection),
        ana.SummedFactory('w_shape', samples, plot, shape_selection))
  elif method in [12, 13, 14, 16]:
      if method == 16:
          cat_nobtag = '(n_jets <=1 && n_lowpt_jets>=1)*('+cats['baseline']+')'
          full_selection = BuildCutString(wt, sel, cat_nobtag, OSSS, '')
          ss_selection = BuildCutString(wt, '', cat_nobtag, '!os', '')
          os_selection = BuildCutString(wt, '', cat_nobtag, 'os', '')
          control_sel = cats['w_sdb']
          w_control_full_selection_os = BuildCutString('wt', control_sel, cat_nobtag)
          w_control_full_selection_ss = BuildCutString('wt', control_sel, cat_nobtag, '!os')
          btag_extrap_sel_num = BuildCutString(wt, sel, cat, OSSS, '')
          btag_extrap_sel_den = BuildCutString(wt, sel, cat_nobtag, OSSS, '')
              
          btag_extrap_num_node = ana.SummedFactory('btag', samples, plot, btag_extrap_sel_num)
          btag_extrap_den_node = ana.SummedFactory('no_btag', samples, plot, btag_extrap_sel_den)
          
      else:
          ss_selection = BuildCutString(wt, '', cat, '!os', '')
          os_selection = BuildCutString(wt, '', cat, 'os', '')
          control_sel = cats['w_sdb']
          w_control_full_selection_os = BuildCutString('wt', control_sel, cat)
          w_control_full_selection_ss = BuildCutString('wt', control_sel, cat, '!os')
          btag_extrap_num_node = None
          btag_extrap_den_node = None
          
      w_node = HttWOSSSNode(name,
        ana.SummedFactory('data_os_obs', data, plot, w_control_full_selection_os),
        ana.SummedFactory('backgrounds_os', sub_samples, plot, w_control_full_selection_os),
        ana.SummedFactory('data_ss_obs', data_samples, plot, w_control_full_selection_ss),
        ana.SummedFactory('backgrounds_ss', sub_samples, plot, w_control_full_selection_ss),
        ana.SummedFactory('w_control', samples, plot, w_control_full_selection_os),
        ana.SummedFactory('w_signal', samples, plot, full_selection),
        ana.SummedFactory('w_os', samples, plot, os_selection),
        ana.SummedFactory('w_ss', samples, plot, ss_selection),
        ana.SummedFactory('w_shape', samples, plot, shape_selection),
        qcd_factor,
        get_os,
        btag_extrap_num_node,
        btag_extrap_den_node) 
  return w_node

def GenerateW(ana, name='W', samples=[], data=[], sub_samples=[], plot='', wt='', sel='', cat='', method=8, qcd_factor=1.18, get_os=True):
  w_node = GetWNode(ana, name, samples, data, sub_samples, plot, wt, sel, cat, method, qcd_factor, get_os)
  ana.nodes[nodename].AddNode(w_node)
      
def GenerateQCD(ana, data=[], qcd_sub_samples=[], w_sub_samples=[], plot='', wt='', sel='', cat='', method=8, qcd_factor=1.18, get_os=True):
    tt_qcd_norm = '(mva_olddm_medium_1>0.5 && mva_olddm_loose_2>0.5 &&mva_olddm_vtight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)'
    if options.channel == 'et' or options.channel == 'mt':
        ttqcdcat = '('+cats[options.cat]+')*(iso_1<0.1 && antiele_2 && antimu_2 && !leptonveto)*(tau_decay_mode_2!=6&&tau_decay_mode_2!=5)'
    elif options.channel == 'tt':
        ttqcdcat = '('+cats[options.cat]+')*(antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)'
        
    qcd_sdb_sel = '(!os && ' + sel + ')'
    w_extrp_sdb_sel = '(!os && '+ cats['w_sdb'] + ')'
    w_extrp_sig_sel = '(!os && ' + sel + ')'
    w_sdb_sel = '(!os && ' + cats['w_sdb'] + ')'
    w_sdb_sel_osss = cats['w_sdb']
    qcd_cat = cat
    qcd_sdb_cat = cat
    qcd_extrap_sel = '(!os && ' + sel + ')'

    if options.channel == 'tt' and method == 8:
        if get_os: 
            qcd_sdb_sel =  '(os && ' + sel + ')'
        qcd_sdb_cat = ttqcdcat + tt_qcd_norm 
    
    if options.channel != 'tt': 
        qcd_os_ss_factor = qcd_factor
        weight = wt
            
        if method == 15:
            qcd_os_ss_factor = 1
            if get_os:
                weight = wt+'wt_em_qcd'
        
        full_selection = BuildCutString(weight, qcd_sdb_sel, qcd_cat, '')
        subtract_node = ana.SummedFactory('subtract_node', qcd_sub_samples, plot, full_selection)
        w_node = GetWNode(ana, 'Wss', wjets_samples, data_samples, w_sub_samples, plot, 'wt', sel, cat, method, 1.18, False)
        subtract_node.AddNode(w_node)
    
    ana.nodes[nodename].AddNode(HttQCDNode('QCD',
        ana.SummedFactory('data_ss', data, plot, full_selection),
        subtract_node,
        qcd_os_ss_factor))
        
    

ana.nodes.AddNode(ListNode(nodename))

full_selection = BuildCutString('wt', sel, cat)
ana.nodes[nodename].AddNode(ana.SummedFactory('data_obs', data_samples, plot, full_selection))

GenerateZ(ana, ztt_samples, plot, 'wt', sel, cat, z_sels)                                 
GenerateTop(ana, top_samples, plot, 'wt', sel, cat, top_sels)  
GenerateVV(ana, vv_samples, plot, 'wt', sel, cat, vv_sels)  
if options.channel == 'em':
    GenerateWG(ana, wgam_samples, plot, 'wt', sel, cat, '')
GenerateW(ana, 'W', wjets_samples, data_samples, w_sub_samples, plot, 'wt', sel, cat, 10)

if options.channel == 'em':
    GenerateQCD(ana, data_samples, qcd_sub_samples, [], plot, 'wt', sel, cat, 8)
else:
    GenerateQCD(ana, data_samples, qcd_sub_samples, w_sub_samples, plot, 'wt', sel, cat, 12)

ana.Run()
ana.nodes.PrintTree()
output_name = options.output_folder+"/"+nodename+"_"+options.cat+"_"+options.channel+".root"
outfile = ROOT.TFile(output_name, 'RECREATE')

ana.nodes.Output(outfile)

