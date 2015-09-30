#!/usr/bin/env python
import sys
from optparse import OptionParser

parser = OptionParser()

parser.add_option("--dir", dest="crabdir",
                  help="Crab task directory")
parser.add_option("--noreport", dest="no_report",action="store_true",default=False,
                  help="Do not fetch the task report")

(options,args) = parser.parse_args()

if not options.crabdir:
  parser.error('Crab task directory not specified (--dir CRAB_TASK)')


getoutputcommand="crab getoutput --dump --xrootd "+options.crabdir
reportcommand="crab report "+options.crabdir
import subprocess 

checkproxy=subprocess.Popen("voms-proxy-info",stdout=subprocess.PIPE,stderr=subprocess.STDOUT,shell=False)
proxyexists=checkproxy.communicate()
proxywords=(list(proxyexists)[0]).split()
if proxywords[2]=="found:" :
  print "You need to generate a proxy first!"
  exit()

print options.crabdir

if not options.no_report:
  print "   Getting report for task"
  #run report command and get number of events
  rep=subprocess.Popen(reportcommand,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,shell=True)
  repout,reperr = rep.communicate()
  replines=repout.splitlines()
  for repline in replines:
      if repline[-21:]=="events have been read":
          replinebits=repline.split(" ")
          evoutstring="   "+replinebits[0]+" events have been processed"
          print evoutstring

else:
  print "    Using --noreport: straight to output list retrieval"

print "   Getting output list for task"
#run command and get output split into lines
p=subprocess.Popen(getoutputcommand,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,shell=True)
out,err = p.communicate()
lines=out.splitlines()

#open filelist with name of crabtask
linebits=lines[0].split("/")
prodname=linebits[7]
filelistname=prodname+"_"+linebits[9][5:]+".dat"
filelist=open(filelistname,'w')


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

#output diagnostic information
print "   Filelist written to "+filelistname
print "   "+str(nfilesprocessed)+" files processed"

#get prefix
xrootdstart=lines[0].find("/")
n=3
while xrootdstart>=0 and n>1:
    xrootdstart=lines[0].find("/",xrootdstart+1)
    n-=1
print "   Prefix should be: root://xrootd.grid.hep.ph.ic.ac.uk"+lines[0][xrootdstart:start]

#PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/pdunne/$PRODUCTION/MC/
#root://cms-xrd-global.cern.ch//store/user/pdunne/May13_MC/VBF_HToInv_M-125_13TeV_powheg-pythia6/crab_Powheg-Htoinv-mH125PU20bx25/150513_140650/0000/EventTree_1.root
