import ROOT
import os
import glob
import json
from UserCode.ICHiggsTauTau.analysis import *
from UserCode.ICHiggsTauTau.uncertainties import ufloat
from optparse import OptionParser

CHANNELS= ['et', 'mt', 'em','tt']
ANALYSIS= ['sm','mssm']
METHODS= [8 ,9, 10, 11, 12 , 13, 14, 15, 16]

parser = OptionParser()
parser.add_option("--channel", dest="channel", type='string', default='mt',
    help="Tau decay channel to process.  Supported channels: %(CHANNELS)s" % vars())
parser.add_option("-o", "--outputfolder", dest="output_folder", type='string', default='output',
    help="Name of output folder")
parser.add_option("-i", "--inputfolder", dest="input_folder", type='string', default='/vols/cms/dw515/Offline/output/MSSM/Jan11/',
    help="Name of input folder")
parser.add_option("-p", "--paramfile", dest="param_file", type='string', default='scripts/Params_2016_spring16.json',
    help="Name of parameter file")
parser.add_option("-c", "--cat", dest="cat", type='string', default='inclusive',
    help="Category")
parser.add_option("-y", "--year", dest="year", type='string', default='2016',
    help="Year")
parser.add_option("-s", "--sel", dest="sel", type='string', default='(1)',
    help="Selection")
parser.add_option("-a", "--analysis", dest="analysis", type='string', default='sm',
    help="Analysis.  Supported options: %(CHANNELS)s" % vars())
parser.add_option("-v", "--var", dest="var", type='string', default='m_vis(7,0,140)',
    help="Variable to plot")
parser.add_option("-m", "--method", dest="method", type='int', default=8,
    help="Method.  Supported options: %(METHODS)s" % vars())
parser.add_option("--signalmasses", dest="signalmasses", type='string', default='125',
    help="Comma seperated list of signal masses.")
parser.add_option("-r", "--qcd_os_ss_ratio", dest="qcd_os_ss_ratio", type='float', default=-1,
    help="QCD OS/SS ratio")
parser.add_option("--sm_bkg", dest="sm_bkg", default=False,
    help="Add SM Higgs background for MSSM")
parser.add_option("--syst_tau_scale", dest="syst_tau_scale", type='string', default='',
    help="If this string is set then the systematic shift due to tau energy scale is performed with the set string appended to the resulting histogram name")
parser.add_option("--syst_eff_t", dest="syst_eff_t", type='string', default='',
    help="If this string is set then the systematic shift due to tau ID is performed with the set string appended to the resulting histogram name")
parser.add_option("--syst_tquark", dest="syst_tquark", type='string', default='',
    help="If this string is set then the top-quark weight systematic is performed with the set string appended to the resulting histogram name")
parser.add_option("--syst_zwt", dest="syst_zwt", type='string', default='',
    help="If this string is set then the z-reweighting systematic is performed with the set string appended to the resulting histogram name")
parser.add_option("--syst_w_fake_rate", dest="syst_w_fake_rate", type='string', default='',
    help="If this string is set then the W+jets fake-rate systematic is performed with the set string appended to the resulting histogram name")
parser.add_option("--syst_scale_j", dest="syst_scale_j", type='string', default='',
    help="If this string is set then the jet scale systematic is performed with the set string appended to the resulting histogram name")
parser.add_option("--syst_eff_b", dest="syst_eff_b", type='string', default='',
    help="If this string is set then the b-tag efficiency systematic is performed with the set string appended to the resulting histogram name")
parser.add_option("--syst_fake_b", dest="syst_fake_b", type='string', default='',
    help="If this string is set then the b-tag fake-rate systematic is performed with the set string appended to the resulting histogram name")
(options, args) = parser.parse_args()


print ''
print '################### Options ###################'
print 'channel          = ' + options.channel
print 'outputfolder     = ' + options.output_folder
print 'inputfolder      = ' + options.input_folder
print 'paramfile        = ' + options.param_file
print 'cat              = ' + options.cat
print 'year             = ' + options.year
print 'sel              = ' + options.sel
print 'analysis         = ' + options.analysis
print 'var              = ' + options.var
print 'method           ='  ,  options.method
print 'signalmasses     = ' +  options.signalmasses
print 'qcd_os_ss_ratio  ='  ,  options.qcd_os_ss_ratio
print 'sm_bkg           ='  ,  options.sm_bkg
print 'syst_tau_scale   ='  ,  options.syst_tau_scale
print 'syst_eff_t       ='  ,  options.syst_eff_t
print 'syst_tquark      ='  ,  options.syst_tquark
print 'syst_zwt         ='  ,  options.syst_zwt
print 'syst_w_fake_rate ='  ,  options.syst_w_fake_rate
print 'syst_scale_j     ='  ,  options.syst_scale_j
print 'syst_eff_b       ='  ,  options.syst_eff_b
print 'syst_fake_b      ='  ,  options.syst_fake_b
print '###############################################'
print ''
        
if options.qcd_os_ss_ratio > 0:
    qcd_os_ss_ratio = options.qcd_os_ss_ratio
else:
    if options.channel == 'et':
        qcd_os_ss_ratio = 1.02
    elif options.channel == 'mt':
        qcd_os_ss_ratio = 1.18
    else:
        qcd_os_ss_ratio = 1.0

# Get array of signal masses to process        
        
masses = options.signalmasses.split(',')

ROOT.TH1.SetDefaultSumw2(True)

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

def GenerateZTT(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', z_sels={}):
    full_selection = BuildCutString(wt, sel, cat, 'os', z_sels['ztt_sel'])
    ana.nodes[nodename].AddNode(ana.SummedFactory('ZTT'+add_name, samples, plot, full_selection))

def GenerateZLL(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', z_sels={}, doZL=True, doZJ=True):
    if options.channel == 'em':
        full_selection = BuildCutString(wt, sel, cat, 'os', z_sels['zll_sel'])
        ana.nodes[nodename].AddNode(ana.SummedFactory('ZLL'+add_name, samples, plot, full_selection))
    else:
        if doZL:
            full_selection = BuildCutString(wt, sel, cat, 'os', z_sels['zl_sel'])
            ana.nodes[nodename].AddNode(ana.SummedFactory('ZL'+add_name, samples, plot, full_selection))
        if doZJ:
            full_selection = BuildCutString(wt, sel, cat, 'os', z_sels['zj_sel'])
            ana.nodes[nodename].AddNode(ana.SummedFactory('ZJ'+add_name, samples, plot, full_selection))
        
def GenerateTop(ana, add_name='', samples=[], plot='', wt='', sel='', cat='', top_sels={}, doTTT=True, doTTJ=True):
  if doTTT:
      full_selection = BuildCutString(wt, sel, cat, 'os', top_sels['ttt_sel'])
      ana.nodes[nodename].AddNode(ana.SummedFactory('TTT'+add_name, samples, plot, full_selection))
  if doTTJ:
      full_selection = BuildCutString(wt, sel, cat, 'os', top_sels['ttj_sel'])
      ana.nodes[nodename].AddNode(ana.SummedFactory('TTJ'+add_name, samples, plot, full_selection))

def GenerateVV(ana, add_name ='', samples=[], plot='', wt='', sel='', cat='', vv_sels={}, doVVT=True, doVVJ=True): 
  if doVVT:
      full_selection = BuildCutString(wt, sel, cat, 'os', vv_sels['vvt_sel'])
      ana.nodes[nodename].AddNode(ana.SummedFactory('VVT'+add_name, samples, plot, full_selection))
  if doVVJ:
      full_selection = BuildCutString(wt, sel, cat, 'os', vv_sels['vvj_sel'])
      ana.nodes[nodename].AddNode(ana.SummedFactory('VVJ'+add_name, samples, plot, full_selection))
  
def GetWGNode(ana, add_name='', samples=[], plot='', wt='', sel='', cat=''):
  full_selection = BuildCutString(wt, sel, cat, 'os')
  wg_node = ana.SummedFactory('WGam'+add_name, samples, plot, full_selection)
  return wg_node

def GetWNode(ana, name='W', samples=[], data=[], sub_samples=[], plot='', wt='', sel='', cat='', method=8, qcd_factor=qcd_os_ss_ratio, get_os=True):
  if get_os:
      OSSS = 'os'
  else:
      OSSS = '!os'
  full_selection = BuildCutString(wt, sel, cat, OSSS, '')
  shape_cat = cat
  if method == 14:
      shape_cat = '(n_jets<=1 && n_loose_bjets>=1)*('+cats['baseline']+')'
  shape_selection = BuildCutString(wt, sel, shape_cat, OSSS, '')
  
  if method in [8, 9, 15]:
      w_node = ana.SummedFactory(name, samples, plot, full_selection)
  elif method in [10, 11]:
      control_sel = cats['w_sdb']+' && '+ OSSS
      w_control_full_selection = BuildCutString('wt', control_sel, cat, OSSS)
      if shape_selection == full_selection:
          w_shape = None
      else:    
          w_shape = ana.SummedFactory('w_shape', samples, plot, shape_selection)
      w_node = HttWNode(name,
        ana.SummedFactory('data_obs', data, plot, w_control_full_selection),
        ana.SummedFactory('backgrounds', sub_samples, plot, w_control_full_selection),
        ana.SummedFactory('w_control', samples, plot, w_control_full_selection),
        ana.SummedFactory('w_signal', samples, plot, full_selection),
        w_shape)
  elif method in [12, 13, 14, 16]:
      if method == 16:
          cat_nobtag = '(n_jets <=1 && n_lowpt_jets>=1)*('+cats['baseline']+')'
          full_selection = BuildCutString(wt, sel, cat_nobtag, OSSS)
          ss_selection = BuildCutString(wt, '', cat_nobtag, '!os', '')
          os_selection = BuildCutString(wt, '', cat_nobtag, 'os', '')
          control_sel = cats['w_sdb']
          w_control_full_selection = BuildCutString('wt', control_sel, cat_nobtag, OSSS)
          w_control_full_selection_os = BuildCutString('wt', control_sel, cat_nobtag)
          w_control_full_selection_ss = BuildCutString('wt', control_sel, cat_nobtag, '!os')
          btag_extrap_sel_num = BuildCutString(wt, sel, cat, OSSS, '')
          btag_extrap_sel_den = BuildCutString(wt, sel, cat_nobtag, OSSS, '')
              
          btag_extrap_num_node = ana.SummedFactory('btag', samples, plot, btag_extrap_sel_num)
          btag_extrap_den_node = ana.SummedFactory('no_btag', samples, plot, btag_extrap_sel_den)
          
      else:
          full_selection = BuildCutString(wt, sel, cat, OSSS)
          ss_selection = BuildCutString(wt, '', cat, '!os', '')
          os_selection = BuildCutString(wt, '', cat, 'os', '')
          control_sel = cats['w_sdb']
          w_control_full_selection = BuildCutString('wt', control_sel, cat, OSSS)
          w_control_full_selection_os = BuildCutString('wt', control_sel, cat)
          w_control_full_selection_ss = BuildCutString('wt', control_sel, cat, '!os')
          btag_extrap_num_node = None
          btag_extrap_den_node = None
      
      if shape_selection == full_selection:
          w_shape = None
      else:    
          w_shape = ana.SummedFactory('w_shape', samples, plot, shape_selection)
      w_node = HttWOSSSNode(name,
        ana.SummedFactory('data_os_obs', data, plot, w_control_full_selection_os),
        ana.SummedFactory('backgrounds_os', sub_samples, plot, w_control_full_selection_os),
        ana.SummedFactory('data_ss_obs', data_samples, plot, w_control_full_selection_ss),
        ana.SummedFactory('backgrounds_ss', sub_samples, plot, w_control_full_selection_ss),
        ana.SummedFactory('w_control', samples, plot, w_control_full_selection),
        ana.SummedFactory('w_signal', samples, plot, full_selection),
        ana.SummedFactory('w_os', samples, plot, os_selection),
        ana.SummedFactory('w_ss', samples, plot, ss_selection),
        w_shape,
        qcd_factor,
        get_os,
        btag_extrap_num_node,
        btag_extrap_den_node) 
  return w_node

def GenerateW(ana, name='W', add_name='', samples=[], data=[], sub_samples=[], wg_samples=[], plot='', wt='', sel='', cat='', method=8, qcd_factor=qcd_os_ss_ratio, get_os=True):
  w_node_name = name  
  if options.channel == 'em':
      w_total_node = SummedNode('W'+add_name)
      w_total_node.AddNode(GetWGNode(ana, add_name, wg_samples, plot, wt, sel, cat))
      ana.nodes[nodename].AddNode(GetWGNode(ana, add_name, wg_samples, plot, wt, sel, cat))
      w_node_name = name+'J'
  ana.nodes[nodename].AddNode(GetWNode(ana, w_node_name+add_name, samples, data, sub_samples, plot, wt, sel, cat, method, qcd_factor, get_os))
  if options.channel == 'em':
      w_total_node.AddNode(GetWNode(ana, w_node_name+add_name, samples, data, sub_samples, plot, wt, sel, cat, method, qcd_factor, get_os))
      ana.nodes[nodename].AddNode(w_total_node)
      
def GenerateQCD(ana, add_name='', data=[], qcd_sub_samples=[], w_sub_samples=[], plot='', wt='', sel='', cat='', method=8, qcd_factor=qcd_os_ss_ratio, get_os=True):
    tt_qcd_norm = cats['tt_qcd_norm']
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
    
    shape_node = None
    
    if options.channel != 'tt':
        
        if method in [9, 11, 13, 16]:
            if method == 16:
                shape_cat = cats[options.cat]
            else:
                shape_cat = '('+cats[options.cat]+')*('+cats['qcd_loose_shape']+')'
            shape_selection = BuildCutString('wt', qcd_sdb_sel, shape_cat, '')
            shape_node = ana.SummedFactory('shape', data, plot, shape_selection)
        if method in [10, 12, 14]:
            if method == 14:
                shape_cat = '(n_jets<=1 && n_loose_bjets>=1)*('+cats['baseline']+')'
            else: 
                shape_cat = cat
            shape_selection = BuildCutString('wt', qcd_sdb_sel, shape_cat, '')
            bkg_shape = ana.SummedFactory('bkg_shape', qcd_sub_samples, plot, shape_selection)
            bkg_shape.AddNode(GetWNode(ana, 'W_shape', wjets_samples, data_samples, w_sub_samples, plot, 'wt', sel, shape_cat, method, qcd_os_ss_ratio, False))
            shape_node = SubtractNode('shape', ana.SummedFactory('data_shape', data, plot, shape_selection), bkg_shape)
        
        if options.channel == 'em':
            qcd_os_ss_factor = 1
        else:
            qcd_os_ss_factor = qcd_factor
        weight = wt
        if method == 15:
            qcd_os_ss_factor = 1
            if get_os:
                weight = wt+'*wt_em_qcd'
        
        full_selection = BuildCutString(weight, qcd_sdb_sel, qcd_cat, '')
        subtract_node = ana.SummedFactory('subtract_node', qcd_sub_samples, plot, full_selection)
        w_node = GetWNode(ana, 'Wss', wjets_samples, data_samples, w_sub_samples, plot, weight, sel, cat, method, qcd_os_ss_ratio, False)
        subtract_node.AddNode(w_node)
        
        ana.nodes[nodename].AddNode(HttQCDNode('QCD'+add_name,
          ana.SummedFactory('data_ss', data, plot, full_selection),
          subtract_node,
          qcd_os_ss_factor,
          shape_node))
        
    else:
        OSSS = False
        if method == 8:
            if get_os: 
                qcd_sdb_sel =  '(os && ' + sel + ')'
                OSSS = True
            qcd_sdb_cat = ttqcdcat+'*'+tt_qcd_norm 
            
        shape_cat = '('+cats[options.cat]+')*('+cats['tt_qcd_norm']+')'
        shape_selection = BuildCutString('wt', qcd_sdb_sel, shape_cat, '')
        bkg_shape = ana.SummedFactory('bkg_shape', qcd_sub_samples, plot, shape_selection)
        bkg_shape.AddNode(GetWNode(ana, 'W_shape', wjets_samples, data_samples, w_sub_samples, plot, 'wt', sel, shape_cat, method, qcd_os_ss_ratio, OSSS))
        shape_node = SubtractNode('shape', ana.SummedFactory('data_shape', data, plot, shape_selection), bkg_shape)
            
        full_selection = BuildCutString('wt', qcd_sdb_sel, qcd_sdb_cat, '')
        subtract_node = ana.SummedFactory('subtract_node', qcd_sub_samples, plot, full_selection)
        if method == 8:
            w_node = GetWNode(ana, 'Wos', wjets_samples, data_samples, w_sub_samples, plot, 'wt', qcd_sdb_sel, qcd_sdb_cat, method, qcd_os_ss_ratio, get_os)
        else:
            w_node = GetWNode(ana, 'Wss', wjets_samples, data_samples, w_sub_samples, plot, 'wt', sel, qcd_sdb_cat, method, qcd_os_ss_ratio, False)
        subtract_node.AddNode(w_node)
        num_selection = BuildCutString('wt', sel, qcd_cat, '!os')
        den_selection = BuildCutString('wt', sel, qcd_sdb_cat, '!os')

        num_node = SubtractNode('ratio_num',
                     ana.SummedFactory('data_num', data, plot, num_selection),
                     ana.SummedFactory('bkg_num', qcd_sub_samples, plot, num_selection))
        w_num_node = GetWNode(ana, 'W_num', wjets_samples, data_samples, w_sub_samples, plot, 'wt', sel, qcd_cat, method, qcd_os_ss_ratio, False)
        num_node = SubtractNode('ratio_num',
                     num_node,
                     w_num_node)
        den_node = SubtractNode('ratio_den',
                     ana.SummedFactory('data_den', data, plot, den_selection),
                     ana.SummedFactory('bkg_den', qcd_sub_samples, plot, den_selection))
        w_den_node = GetWNode(ana, 'W_den', wjets_samples, data_samples, w_sub_samples, plot, 'wt', sel, qcd_sdb_cat, method, qcd_os_ss_ratio, False)
        den_node = SubtractNode('ratio_den',
                     den_node,
                     w_den_node) 
        
        ana.nodes[nodename].AddNode(HttQCDNode('QCD'+add_name,
          ana.SummedFactory('data_ss', data, plot, full_selection),
          subtract_node,
          1,
          shape_node,
          num_node,
          den_node))
        
def GenerateSMSignal(ana, add_name='', plot='', masses=['125'], wt='', sel='', cat='', sm_bkg = False):
    full_selection = BuildCutString(wt, sel, cat, 'os')
    for mass in masses:  
        if sm_bkg:
            add_str = '_SM'+mass
        else:
            add_str = mass
        for key in sm_samples:
            sample_name = sm_samples[key]+'_M-'+mass
            ana.nodes[nodename].AddNode(ana.BasicFactory(key+add_str+add_name, sample_name, plot, full_selection))
            
def GenerateMSSMSignal(ana, add_name='', plot='', masses = ['700'], wt='', sel='', cat='', do_ggH=True, do_bbH=True):
    full_selection = BuildCutString(wt, sel, cat, 'os')
    for mass in masses:
        for key in mssm_samples:
            if key == 'ggH' and not do_ggH:
                continue
            if key == 'bbH' and not do_bbH:
                continue
            sample_name = mssm_samples[key]+'_M-'+mass
            ana.nodes[nodename].AddNode(ana.BasicFactory(key+mass+add_name, sample_name, plot, full_selection))
        
def GenerateHhhSignal(ana, add_name='', plot='', masses = ['700'], wt='', sel='', cat=''):
    full_selection = BuildCutString(wt, sel, cat, 'os')
    for mass in masses:
        for key in Hhh_samples:
            sample_name = Hhh_samples[key]+'_M-'+mass
            ana.nodes[nodename].AddNode(ana.BasicFactory(key+mass+add_name, sample_name, plot, full_selection))
        
def PrintSummary(nodename='', data_strings=['data_obs'], add_name=''):
    print ''
    print '################### Summary ###################'
    nodes = ana.nodes[nodename].SubNodes()
    bkg_total = ufloat(0.000000001,0.000000001)
    sig_total = ufloat(0.000000001,0.000000001)
    for node in nodes:
        per_err = node.shape.rate.s/node.shape.rate.n
        print node.name.ljust(10) , ("%.2f" % node.shape.rate.n).ljust(10), '+/-'.ljust(5), ("%.2f" % node.shape.rate.s).ljust(7), "(%.4f)" % per_err
        if True in [node.name.find(sig) != -1 for sig in signal_samples.keys()]:
            sig_total += node.shape.rate
        elif node.name not in data_strings:
            bkg_total += node.shape.rate
    per_err = bkg_total.s/bkg_total.n
    print 'Total bkg'.ljust(10) , ("%.2f" % bkg_total.n).ljust(10), '+/-'.ljust(5), ("%.2f" % bkg_total.s).ljust(7), "(%.4f)" % per_err
    per_err = sig_total.s/sig_total.n
    print 'Total sig'.ljust(10) , ("%.2f" % sig_total.n).ljust(10), '+/-'.ljust(5), ("%.2f" % sig_total.s).ljust(7), "(%.4f)" % per_err
    print '###############################################'
    print ''
    
def RunPlotting(ana, cat='', sel='', add_name='', wt='wt', samples_to_skip=[]):
    doTTJ = 'TTJ' not in samples_to_skip
    doTTT = 'TTT' not in samples_to_skip
    doVVJ = 'VVJ' not in samples_to_skip
    doVVT = 'VVT' not in samples_to_skip

    if 'ZTT' not in samples_to_skip:
        GenerateZTT(ana, add_name, ztt_samples, plot, wt, sel, cat, z_sels)                                
    if 'ZLL' not in samples_to_skip:
        GenerateZLL(ana, add_name, ztt_samples, plot, wt, sel, cat, z_sels)
    if 'TT' not in samples_to_skip:    
        GenerateTop(ana, add_name, top_samples, plot, wt, sel, cat, top_sels, doTTT, doTTJ)  
    if 'VV' not in samples_to_skip:
        GenerateVV(ana, add_name, vv_samples, plot, wt, sel, cat, vv_sels, doVVT, doVVJ)  
    if 'W' not in samples_to_skip:
        GenerateW(ana, 'W', add_name, wjets_samples, data_samples, w_sub_samples, wgam_samples, plot, wt, sel, cat, options.method)
    if 'QCD' not in samples_to_skip:
        GenerateQCD(ana, add_name, data_samples, qcd_sub_samples, w_sub_samples, plot, wt, sel, cat, options.method)
    if 'signal' not in samples_to_skip:
        if options.analysis == 'sm':
            GenerateSMSignal(ana, add_name, plot, masses, wt, sel, cat)
        elif options.analysis == 'mssm':
            GenerateMSSMSignal(ana, add_name, plot, masses, wt, sel, cat)
            if options.sm_bkg:
                GenerateSMSignal(ana, add_name, plot, ['125'],  wt, sel, cat, options.sm_bkg)  
        elif options.analysis == 'Hhh':
            GenerateHhhSignal(ana, add_name, plot, masses, wt, sel, cat)

# Category dictionary 

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
cats['tt_qcd_norm'] = '(mva_olddm_tight_1>0.5 && mva_olddm_medium_2>0.5 &&mva_olddm_tight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)'
cats['qcd_loose_shape'] = '(iso_1>0.2 && iso_1<0.5 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto)'

# Additional selections to seperate MC samples by gen flags

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
    qcd_sub_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','T-tW', 'Tbar-tW', 'Tbar-t', 'T-t','WWTo1L1Nu2Q','VVTo2L2Nu', 'ZZTo4L','ZZTo2L2Q','WZJToLLLNu','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','TT']
else:
    qcd_sub_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','ZZTo4L','T-tW','T-t','Tbar-tW','Tbar-t','WWTo1L1Nu2Q','VVTo2L2Nu','ZZTo2L2Q','WZJToLLLNu','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','TT']
    
w_sub_samples = ['DYJetsToLL-LO-ext','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO','T-tW', 'Tbar-tW', 'Tbar-t','T-t','WWTo1L1Nu2Q','VVTo2L2Nu','ZZTo4L','ZZTo2L2Q','WZJToLLLNu','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','TT']
wjets_samples = ['WJetsToLNu-LO','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO']

sm_samples = { 'ggH' : 'GluGluHToTauTau', 'qqH' : 'VBFHToTauTau', 'WplusH' : 'WplusHToTauTau', 'WminusH' : 'WminusHToTauTau', 'ZH' : 'ZHToTauTau', 'TTH' : 'TTHToTauTau' }
mssm_samples = { 'ggH' : 'SUSYGluGluToHToTauTau', 'bbH' : 'SUSYGluGluToBBHToTauTau' }
Hhh_samples = { 'ggH' : 'GluGluToRadionToHHTo2B2Tau' }

# set systematics: first index sets folder name contaning systematic samples, second index sets string to be appended to output histograms, third index specifies the weight to be applied , 4th lists samples that should be skipped

systematics = { 'default' : ('','', 'wt', []) }
if options.syst_tau_scale != '':
    systematics['scale_t_up'] = ('TSCALE_UP' , '_'+options.syst_tau_scale+'UP', 'wt', [])
    systematics['scale_t_down'] = ('TSCALE_DOWN' , '_'+options.syst_tau_scale+'DOWN', 'wt', [])
if options.syst_eff_t != '':
    systematics['syst_eff_t_up'] = ('' , '_'+options.syst_eff_t+'UP', 'wt*wt_tau_id_up', ['ZLL','VVJ','TTJ','QCD','W'] )
    systematics['syst_eff_t_down'] = ('' , '_'+options.syst_eff_t+'DOWN', 'wt*wt_tau_id_down', ['ZLL','VVJ','TTJ','QCD','W'])
if options.syst_tquark != '':
    systematics['syst_tquark_up'] = ('' , '_'+options.syst_tquark+'UP', 'wt*wt_tquark_up', ['ZTT','ZLL','VV','QCD','W','signal'])
    systematics['syst_tquark_down'] = ('' , '_'+options.syst_tquark+'DOWN', 'wt*wt_tquark_down', ['ZTT','ZLL','VV','QCD','W', 'signal'])    
if options.syst_zwt != '':
    systematics['syst_zwt_up'] = ('' , '_'+options.syst_zwt+'UP', 'wt*wt_zpt_up', ['ZLL','VV','TT','QCD','W','signal'])
    systematics['syst_zwt_down'] = ('' , '_'+options.syst_zwt+'DOWN', 'wt*wt_zpt_down', ['ZLL','VV','TT','QCD','W','signal'])
if options.syst_w_fake_rate != '':
    systematics['syst_w_fake_rate_up'] = ('' , '_'+options.syst_w_fake_rate+'UP', 'wt*wt_tau_fake_up', ['ZTT','ZLL','VV','TT','QCD','signal'])
    systematics['syst_w_fake_rate_down'] = ('' , '_'+options.syst_w_fake_rate+'DOWN', 'wt*wt_tau_fake_down', ['ZTT','ZLL','VV','TT','QCD','signal'])
if options.syst_scale_j != '':
    systematics['syst_scale_j_up'] = ('JES_UP' , '_'+options.syst_scale_j+'UP', 'wt', [])
    systematics['syst_scale_j_down'] = ('JES_DOWN' , '_'+options.syst_scale_j+'DOWN', 'wt', [])
if options.syst_eff_b != '':
    systematics['syst_eff_b_up'] = ('BTAG_UP' , '_'+options.syst_eff_b+'UP', 'wt', [])
    systematics['syst_scale_j_down'] = ('BTAG_DOWN' , '_'+options.syst_eff_b+'DOWN', 'wt', [])
if options.syst_fake_b != '':
    systematics['syst_fake_b_up'] = ('BFAKE_UP' , '_'+options.syst_fake_b+'UP', 'wt', [])
    systematics['syst_fake_b_down'] = ('BFAKE_DOWN' , '_'+options.syst_fake_b+'DOWN', 'wt', [])

print systematics

for systematic in systematics:
    
    print "Processing:", systematic
    print ""
    
    add_folder_name = systematics[systematic][0]
    add_name = systematics[systematic][1]
    weight = systematics[systematic][2]
    samples_to_skip = systematics[systematic][3]
    
    ana = Analysis()
    
    ana.remaps = {
        'SingleMuon': 'data_obs'
    }#need to make sure this is set correctly for all channels!!!!
    
    mc_input_folder_name = options.input_folder
    if add_folder_name != '':
        mc_input_folder_name += '/'+add_folder_name
        
    # Add all data files
    for sample_name in data_samples:
        ana.AddSamples(options.input_folder+'/'+sample_name+'_'+options.channel+'*.root', 'ntuple', None, sample_name)
    
    # Add all MC background files
    for sample_name in ztt_samples + vv_samples + wgam_samples + top_samples + ztt_shape_samples + qcd_sub_samples + w_sub_samples + wjets_samples:
        ana.AddSamples(mc_input_folder_name+'/'+sample_name+'_'+options.channel+'*.root', 'ntuple', None, sample_name)
     
    # Add all MC signal files
    
    if options.analysis == 'sm':
        signal_samples = sm_samples
    elif options.analysis == 'mssm':
        signal_samples = mssm_samples
    elif options.analysis == 'Hhh':
        signal_samples = Hhh_samples
    
    for mass in masses:
        for samp in signal_samples:
            sample_name = signal_samples[samp]+'_M-'+mass
            ana.AddSamples(mc_input_folder_name+'/'+sample_name+'_'+options.channel+'*.root', 'ntuple', None, sample_name)
    
    if options.sm_bkg and options.analysis == 'mssm':
        for samp in sm_samples:
            sample_name = sm_samples[samp]+'_M-125'
            ana.AddSamples(mc_input_folder_name+'/'+sample_name+'_'+options.channel+'*.root', 'ntuple', None, sample_name)
            
    ana.AddInfo(options.param_file, scaleTo='data_obs')
    
    cat = '('+cats[options.cat]+')*('+cats['baseline']+')'
    sel = options.sel
    plot = options.var
    nodename = options.channel+'_'+options.cat
    
    ana.nodes.AddNode(ListNode(nodename))
    
    # Add data only for default
    if systematic == 'default':
        full_selection = BuildCutString('wt', sel, cat)
        ana.nodes[nodename].AddNode(ana.SummedFactory('data_obs', data_samples, plot, full_selection))
            
    #Run default plot        
    RunPlotting(ana, cat, sel, add_name, weight, samples_to_skip)
    
    ana.Run()
    
    PrintSummary(nodename, ['data_obs'], add_name)
    
    output_name = options.output_folder+"/"+nodename+"_"+options.cat+"_"+options.channel+".root"
    outfile = ROOT.TFile(output_name, 'RECREATE')
    ana.nodes.Output(outfile)
    
    # add histograms to get totals for backgrounds split into real/fake taus and make a total backgrounds histogram
    if systematic is 'default':
        outfile.cd('analysis/'+nodename)
        nodes = ana.nodes[nodename].SubNodes()
        for i in ['TT', 'VV', 'Z']:
            sum_hist = ana.nodes[nodename].nodes['data_obs'].shape.hist.Clone()
            sum_hist.Reset()
            j = 'T'
            outname = i
            if i is 'Z':
                outname = 'ZLL'
                j = 'L'
            if i+'J' or i+j in [node.name for node in nodes]:
                sum_hist.Add(ana.nodes[nodename].nodes[i+'J'].shape.hist.Clone())
                sum_hist.Add(ana.nodes[nodename].nodes[i+j].shape.hist.Clone())
            sum_hist.SetName(outname)
            sum_hist.Write()
        
        total_bkg = ana.nodes[nodename].nodes['data_obs'].shape.hist.Clone()
        total_bkg.Reset()
        for node in nodes:
            if True not in [node.name.find(sig) != -1 for sig in signal_samples.keys()] and node.name != 'data_obs':
                total_bkg.Add(ana.nodes[nodename].nodes[node.name].shape.hist.Clone())
        total_bkg.SetName('total_bkg')
        total_bkg.Write()
        outfile.cd()
        
    if systematic is 'syst_tquark_up' or systematic is 'syst_tquark_down':
        outfile.cd('analysis/'+nodename)
        sum_hist = ana.nodes[nodename].nodes['TTT'+add_name].shape.hist.Clone()
        sum_hist.Add(ana.nodes[nodename].nodes['TTJ'+add_name].shape.hist.Clone())
        sum_hist.SetName('TT'+add_name)
        sum_hist.Write()
        outfile.cd()
        
    

