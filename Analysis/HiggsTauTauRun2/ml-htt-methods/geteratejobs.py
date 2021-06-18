import ROOT
from optparse import OptionParser
import os


parser = OptionParser()
parser.add_option("--filelist")
parser.add_option("--directory")

(options, args) = parser.parse_args()

count=1

dir_ = options.directory

maxperjobs=300000

with open(options.filelist) as fp: 
    for line in fp:
        # if 'UncorrelatedDecay' not in line:
        #     continue
        l = line.strip()
        #if True not in [x in l for x in [
        #  'ZHToTauTauUncorrelatedDecay_Filtered',
        #  'WplusHToTauTauUncorrelatedDecay_Filtered',
        #  'WminusHToTauTauUncorrelatedDecay_Filtered',
        #  'VBFHToTauTauUncorrelatedDecay_Filtered',
        #  'GluGluHToTauTauUncorrelatedDecay_Filtered',

        #]]: continue

        print l
        if '.root' not in l: continue
        try:
            if os.path.isfile('%(dir_)s/%(l)s'%vars()):
              f = ROOT.TFile('%(dir_)s/%(l)s'%vars())
              t = f.Get('ntuple')
              entries=t.GetEntries()
              nsplit=0
              for i in range(0,entries,maxperjobs):
                outF = open("x%(count)i" % vars(), "w")
                out_line = '%(l)s %(nsplit)i' % vars() 
                nsplit+=1
                outF.write(out_line)
                outF.close() 
                count+=1
        except AttributeError:
            print("Not found this one, might not be needed for this sample")

