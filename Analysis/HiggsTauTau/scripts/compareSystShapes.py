import ROOT
import UserCode.ICHiggsTauTau.plotting as plotting
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--datacard', '-d', help= 'Data card')
parser.add_argument('--systematic', '-s', help= 'Name of systematic to make the comparrison plots for')
parser.add_argument('--output', '-o', help= 'Output directory')
args = parser.parse_args()

infile = ROOT.TFile(args.datacard)

chan='mt'
if 'htt_tt.' in args.datacard: chan='tt'
if 'htt_et.' in args.datacard: chan='et'
if 'htt_em.' in args.datacard: chan='em'

to_print=[]

for key in infile.GetListOfKeys():
  if isinstance(infile.Get(key.GetName()),ROOT.TDirectory):
    dirname=key.GetName()
    directory = infile.Get(dirname)
    for dirkey in directory.GetListOfKeys():
        name = dirkey.GetName()
        if 'norm' in name and 'jetFakes' in name: continue
        if args.systematic in name and 'Up' in name:
          histo_up = directory.Get(name)
          if isinstance(histo_up,ROOT.TH1D) or  isinstance(histo_up,ROOT.TH1F): 
            histo_nom = directory.Get(name.replace('_'+args.systematic+'Up',''))
            histo_down = directory.Get(name.replace('Up','Down'))
            if isinstance(histo_up,ROOT.TDirectory): continue 

            plot_name = '%s/systs_%s_%s' % (args.output, dirname, name.replace('Up',''))

            plotting.CompareSysts([histo_nom,histo_up,histo_down],
               plot_name,
               dirname+"_"+name.replace('Up','')) 

            proc=name.replace('_'+args.systematic+'Up','')
            noPrint=False
            if '0jet' in dirname: binnum=1
            elif 'boosted' in dirname and 'dijet' not in dirname: binnum=2
            elif 'dijet_loosemjj_lowboost' in dirname: binnum=3
            elif 'dijet_loosemjj_boosted' in dirname: binnum=4
            elif 'dijet_tightmjj_lowboost' in dirname: binnum=5
            elif 'dijet_tightmjj_boosted' in dirname: binnum=6
            else: noPrint = True
            if '_jhu_' in proc or '_ph_' in proc or '_total_bkg_' in proc or '_ZTT_' in proc or 'plus' in proc or 'minus' in proc: noPrint=True
            if histo_nom.Integral() > 0 and not noPrint:
              up = histo_up.Integral()/histo_nom.Integral()
              down = histo_down.Integral()/histo_nom.Integral()
              to_print.append('({\"%s\"}, {%i}, {\"%s\"}, %.3f, %.3f)' % (chan, binnum, proc, down, up))
         
    directory.Close() 

infile.Close()

for i in to_print: print i
