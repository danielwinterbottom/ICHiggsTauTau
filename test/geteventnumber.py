import ROOT
import glob, os
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-d", type="string", dest="date")
parser.add_option("-i", type="string", dest="input_dir")
parser.add_option("-o", type="string", dest="output")

options, args = parser.parse_args()

print options

prefix = "root://gfe02.grid.hep.ph.ic.ac.uk:1097/store/user/dwinterb/Mar11_MC_80X/"
test_line = ""
a = ['amcatnlo']
full_string=""

for f in glob.glob(options.input_dir+"/"+options.date+"*MC*.dat"):
    c = ROOT.TChain("icEventProducer/EventTree")
    for line in open(f, 'r'):
        line = line.rstrip()
        test_line = line
        c.Add(prefix+line)
        
    out_string = f.split('_', 3)[-1]
    out_string = out_string.split('.', 1)[0]
    
    amc_add_string=""  
    if any(x in test_line for x in a): amc_add_string=" [aMC@NLO]"
    entries=c.GetEntries()
    print out_string, entries
    full_string+=out_string+" "+str(entries)+amc_add_string+"\n"
    
f1 = open(options.output, 'w')
f1.write(full_string)
f1.close()
