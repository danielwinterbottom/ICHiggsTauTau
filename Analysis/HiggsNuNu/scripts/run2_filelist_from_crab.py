#!/usr/bin/env python
import sys

if (len(sys.argv)!=2):
    print "Usage ./run2_filelist_from_crab.py CRAB_TASK"
    exit()



print "Getting output for that task"
getoutputcommand="crab getoutput --dump --xrootd "+sys.argv[1]
import subprocess 

#run command and get output split into lines
p=subprocess.Popen(getoutputcommand,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,shell=True)
out,err = p.communicate()
lines=out.splitlines()


#open filelist with name of crabtask
linebits=lines[0].split("/")
filelist=open(linebits[9][5:]+".dat",'w')
    
#parse each line to get material to go into filelist
nfilesprocessed=0
for line in lines:
    if line[-5:]==".root":
        start=line.find("/")
        n=8
        while start>=0 and n>1:
            start=line.find("/",start+1)
            n-=1
        filelistcontent=line[start:]
        filelist.write(filelistcontent+"\n")
        nfilesprocessed+=1
filelist.close()
        
print "Filelist written to "+linebits[9][5:]+".dat"
print str(nfilesprocessed)+" files processed"

#get prefix
xrootdstart=lines[0].find("/")
n=3
while xrootdstart>=0 and n>1:
    xrootdstart=lines[0].find("/",xrootdstart+1)
    n-=1
print "Prefix should be: root://xrootd.grid.hep.ph.ic.ac.uk"+lines[0][xrootdstart:start]

#PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/pdunne/$PRODUCTION/MC/
#root://cms-xrd-global.cern.ch//store/user/pdunne/May13_MC/VBF_HToInv_M-125_13TeV_powheg-pythia6/crab_Powheg-Htoinv-mH125PU20bx25/150513_140650/0000/EventTree_1.root
