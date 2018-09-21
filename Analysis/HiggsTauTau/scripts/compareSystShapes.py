import ROOT
import UserCode.ICHiggsTauTau.plotting as plotting
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--datacard', '-d', help= 'Data card')
parser.add_argument('--systematic', '-s', help= 'Name of systematic to make the comparrison plots for')
parser.add_argument('--output', '-o', help= 'Output directory')
args = parser.parse_args()

infile = ROOT.TFile(args.datacard)


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
         
    directory.Close() 

infile.Close()

