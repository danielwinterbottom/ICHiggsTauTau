#!/usr/bin/env python
from optparse import OptionParser, OptionGroup

## set up the option parser
parser = OptionParser(usage="usage: %prog [options]",
                      description="Script to fit the tails to functions compute shape high and low nuisances and propagate them to the analysis")
parser.add_option("-i"  ,"--input", dest="input", default="test.root", type="string", help="Input file where to find the signal histograms (or workspaces). [Default: test.root]")
parser.add_option("-b"  ,"--background", dest="background", default="TT QCD EWK",      type="string",   help="Template to change [TT QCD EWK]")
parser.add_option("-n"  ,"--name",       dest="name",       default="shift",   type="string",   help="shift")
parser.add_option("-e"  ,"--energy",     dest="energy",     default="8TeV",    type="string",   help="Energy [Default: 8 TeV]")
parser.add_option("-c"  ,"--channel",    dest="channel",    default="tt",      type="string",   help="channel [Default: tt]")
parser.add_option("-k"  ,"--categories", dest="categories", default="8 9",     type="string",   help="Categories [Default: 8 9 ]")
parser.add_option("-r"  ,"--range",      dest="first",      default="200",     type="int",      help="Beginning of fit range [Default: 200]")
parser.add_option("-l"  ,"--rangelast",  dest="last",      default="1500",     type="int",      help="End of fit range [Default: 1500]")
parser.add_option("-f"  ,"--fitmodel",   dest="fitmodel",   default="0",       type="int",      help="Fit model 0(exp(m/(a+b*m)) 1(exp(a*m*pow(b)) [Default:0]")
parser.add_option("-o"  ,"--varbin",     dest="varbin",     default=False,action="store_true",  help="Use variable binned fits    [Default: False]")
parser.add_option("-m"  ,"--rebin",      dest="rebin",      default=False,action="store_true",  help="Rebin histogram to data_obs [Default: False]")
parser.add_option("-s"  ,"--setup",      dest="setup",      default="HiggsAnalysis/HiggsToTauTau/setup",    type="string",  help="Setup Directory : HiggsAnalysis/HiggsToTauTau/setup")
parser.add_option("-v"  ,"--verbose",    dest="verbose",    default=False, action="store_true", help="increase verbosity. [Default: False]")

# check number of arguments; in case print usage
(options, args) = parser.parse_args()
#if len(args) < 1 :
#    parser.print_usage()
#    exit(1)

import os
import re

print " You are running with configuration: "
print "-------------------------------------"
print " input      : ", options.input
print " Template   : ", options.background
print " energy     : ", options.energy
print " channel    : ", options.channel
print " categories : ", options.categories

from HiggsAnalysis.HiggsToTauTau.utils import parseArgs 

## mapping out signals
channelName = {
    "mm" : "mumu",
    "em" : "emu",
    "et" : "eleTau",
    "mt" : "muTau",
    "tt" : "tauTau"
    }

print " channel : ",  channelName[options.channel]
## add shift Nuisance (ignore the VBF Option right now)
os.system(r"cp {CMSSW_BASE}/src/HiggsAnalysis/HiggsToTauTau/macros/rootlogon.C .".format(CMSSW_BASE=os.environ.get("CMSSW_BASE")))

os.system("cp %s %s.bak"      %  (options.setup+'/'+options.channel+'/'+options.input,options.setup+'/'+options.channel+'/'+options.input))
for cat in options.categories.split() :
    for bkg in options.background.split() : 
        os.system(r"root -l -b -q rootlogon.C {CMSSW_BASE}/src/HiggsAnalysis/HiggsToTauTau/macros/addFitNuisanceBiasStudy.C+\(\"{FILENAME}\"\,\"{CHANNEL}\"\,\"{BKG}\"\,\"{ENERGY}\"\,\"{NAME}\"\,\"{CATEGORY}\"\,{FIRST}\,{LAST}\,{FITMODEL}\,{VARBIN}\,{REBIN}\)".format(
            CMSSW_BASE=os.environ.get("CMSSW_BASE"), FILENAME=options.setup+'/'+options.channel+'/'+options.input,CHANNEL=channelName[options.channel],BKG=bkg,ENERGY=options.energy,NAME=options.name,CATEGORY=cat,FIRST=options.first,LAST=options.last,FITMODEL=options.fitmodel,VARBIN=str(options.varbin).lower(),REBIN=str(options.rebin).lower()))
        os.system("rm %s"      %  (options.setup+'/'+options.channel+'/'+options.input))
        os.system("mv Output.root %s" %  (options.setup+'/'+options.channel+'/'+options.input))

for cat in options.categories.split() :
    if cat == ' ' :
        continue
## add Nuisance to the conf
    datacard=options.setup+'/'+options.channel+'/unc-mssm-'+options.energy+'-0'+cat+'.vals'
    print "datacard: ",datacard
    old = open(datacard, 'r')
    new = open("%s-tmp.txt" % datacard[0:datacard.rfind('.txt')], 'w')
    for line in old :
        words = line.split()
        new.write(line )
        if len(words) < 2 :
            continue
        if '#' in words[0] :
            continue
        #if ('_scale_t_' or '_scale_e_') in words[2] :
        if  words[2].find('_scale_t_')>-1 or words[2].find('_scale_e_')>-1 :
            new_words    = words 
            new_words[1] = options.background.replace(' ',',').replace("_fine_binning", "")
            new_words[2] = 'CMS_'+options.name+'1_' + channelName[options.channel] + '_' + options.energy
            new_line1     = '                      '.join(new_words)
            new_words[2] = 'CMS_'+options.name+'2_' + channelName[options.channel] + '_' + options.energy
            new_line2     = '                      '.join(new_words)
            new.write(new_line1+'\n')
            new.write(new_line2+'\n')
    old.close()
    new.close()
    os.system("mv %s-tmp.txt %s" % (datacard[0:datacard.rfind('.txt')], datacard))           

## add Nuisance to the vals
    datacard=options.setup+'/'+options.channel+'/unc-mssm-'+options.energy+'-0'+cat+'.conf'
    print "datacard: ",datacard
    old = open(datacard, 'r')
    new = open("%s-tmp.txt" % datacard[0:datacard.rfind('.txt')], 'w')
    for line in old :
        new.write(line )
        words = line.split()
        if len(words) < 2 :
            continue
        if '#' in words[0] :
            continue
        #if  ('_scale_t_' or '_scale_e_') in words[0] :
        if  words[0].find('_scale_t_')>-1 or words[0].find('_scale_e_')>-1 :
            new_words    = words 
            new_words[0] = 'CMS_'+options.name+'1_' + channelName[options.channel] + '_' + options.energy
            new_line1     = '                      '.join(new_words)
            new_words[0] = 'CMS_'+options.name+'2_' + channelName[options.channel] + '_' + options.energy
            new_line2     = '                      '.join(new_words)
            new.write(new_line1+'\n')
            new.write(new_line2+'\n')
    new.close()
    old.close()
    os.system("mv %s-tmp.txt %s" % (datacard[0:datacard.rfind('.txt')], datacard))           

