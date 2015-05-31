#!/usr/bin/env python
import sys

if (len(sys.argv)!=2):
    print "Usage ./job_checker.py CRAB_TASK"
    exit()



statcommand="crab status "+sys.argv[1]
resubcommand="crab resubmit "+sys.argv[1]
import subprocess 

print sys.argv[1]

#run report command and get number of events
stat=subprocess.Popen(statcommand,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,shell=True)
statout,staterr = stat.communicate()
print statout
statlines=statout.splitlines()
for statline in statlines:
    failed=False
    if "failed" in statline:
        if not failed:
            failed=True
            print "\033[93m Do you want to resubmit? \033[0m"
            response=raw_input()
            if response=="y":
                p=subprocess.Popen(resubcommand,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,shell=True)
                out,err = p.communicate()
                print out
            elif response=="n":
                print "Not resubmitting"
            else:
                print "You must say yes or no"
            break

#PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/pdunne/$PRODUCTION/MC/
#root://cms-xrd-global.cern.ch//store/user/pdunne/May13_MC/VBF_HToInv_M-125_13TeV_powheg-pythia6/crab_Powheg-Htoinv-mH125PU20bx25/150513_140650/0000/EventTree_1.root
