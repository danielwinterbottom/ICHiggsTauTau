#!/usr/bin/env python

from optparse import OptionParser
import os

CHANNELS= ['et', 'mt', 'em']

def validate_channel(channel):
	assert channel in CHANNELS
	CHANNELS.remove(channel)

SCALES= ['0', '1', '2']

def validate_scales(scale):
	assert scale in SCALES
	SCALES.remove(scale)

def split_callback(option, opt, value, parser):
  setattr(parser.values, option.dest, value.split(','))


parser = OptionParser()
parser.add_option("-e", "--era", dest="era",
                  help="Use configuration for data ERA, e.g. Moriond, HCP or DOnly", metavar="ERA")
parser.add_option("-c", "--channels", dest="channels", type='string', action='callback',callback=split_callback,
                  help="Channels to run, any of et,mt,em")
parser.add_option("-s", "--energyscales", dest="scales", type='string', action='callback',callback=split_callback,
                  help="Run energy scale shifts, 0=central 1=down 2=up")

(options, args) = parser.parse_args()

channels = options.channels
scales =options.scales

for channel in channels:
	validate_channel(channel)
	
for scale in scales:
	validate_scales(scale)

CONFIG = 'scripts/%s_data_2012.cfg'% options.era
ERA = options.era
print 'Using config: %s'% CONFIG

JOBWRAPPER = './scripts/generate_mac_job.sh'
JOBSUBMIT = 'true'
FILELIST='filelists/Dec30_Data_53X'

for ch in channels:
	JOB='Data_%s_2012' % ch
	os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s --filelist=%(FILELIST)s_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s --output_name=%(JOB)s.root &> jobs/%(JOB)s.log" jobs/%(JOB)s.sh' % vars())
	os.system('%(JOBSUBMIT)s jobs/%(JOB)s.sh' % vars())

	if ch in ['et', 'mt']:
		for sc in scales:
			JOB='Embedded_%s_2012' % ch
			os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s --tau_scale_mode=%(sc)s --filelist=%(FILELIST)s_Embedded_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s '
				' --is_embedded=true --output_name=%(JOB)s.root &> jobs/%(JOB)s-%(sc)s.log" jobs/%(JOB)s-%(sc)s.sh' % vars())
			os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(sc)s.sh' % vars())

		JOB='Data_%s_2012' % ch
		os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s --filelist=%(FILELIST)s_Special_3_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s'
			' --special_mode=3 --output_name=%(JOB)s.root &> jobs/Special_3_%(JOB)s.log" jobs/Special_3_%(JOB)s.sh' % vars())
		os.system('%(JOBSUBMIT)s jobs/Special_3_%(JOB)s.sh' % vars())


	if ch in ['em']:
		JOB='Data_%s_2012' % ch
		os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s --filelist=%(FILELIST)s_Special_20_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
		--special_mode=20 --output_name=%(JOB)s.root &> jobs/Special_20_%(JOB)s.log" jobs/Special_20_%(JOB)s.sh' % vars())
		os.system('%(JOBSUBMIT)s jobs/Special_20_%(JOB)s.sh' % vars())

		JOB='Data_%s_2012' % ch
		os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s --filelist=%(FILELIST)s_Special_21_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
		--special_mode=21 --output_name=%(JOB)s.root &> jobs/Special_21_%(JOB)s.log" jobs/Special_21_%(JOB)s.sh' % vars())
		os.system('%(JOBSUBMIT)s jobs/Special_21_%(JOB)s.sh' % vars())

		JOB='Data_%s_2012' % ch
		os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s --filelist=%(FILELIST)s_Special_22_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
		--special_mode=22 --output_name=%(JOB)s.root &> jobs/Special_22_%(JOB)s.log" jobs/Special_22_%(JOB)s.sh' % vars())
		os.system('%(JOBSUBMIT)s jobs/Special_22_%(JOB)s.sh' % vars())

		JOB='Data_%s_2012' % ch
		os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s --filelist=%(FILELIST)s_Special_25_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
		--special_mode=23 --output_name=%(JOB)s.root &> jobs/Special_23_%(JOB)s.log" jobs/Special_23_%(JOB)s.sh' % vars())
		os.system('%(JOBSUBMIT)s jobs/Special_23_%(JOB)s.sh' % vars())

		JOB='Data_%s_2012' % ch
		os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s --filelist=%(FILELIST)s_Special_25_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
		--special_mode=24 --output_name=%(JOB)s.root &> jobs/Special_24_%(JOB)s.log" jobs/Special_24_%(JOB)s.sh' % vars())
		os.system('%(JOBSUBMIT)s jobs/Special_24_%(JOB)s.sh' % vars())

		if '0' in scales:
			JOB='Embedded_%s_2012' % ch
			os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s --mass_scale_mode=2 --filelist=%(FILELIST)s_Embedded_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
				--is_embedded=true --output_name=%(JOB)s.root &> jobs/%(JOB)s-2.log" jobs/%(JOB)s-2.sh' % vars())
			os.system('%(JOBSUBMIT)s jobs/%(JOB)s-2.sh' % vars())

		if '1' in scales:
			JOB='Embedded_%s_2012' % ch
			os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s --mass_scale_mode=1 --filelist=%(FILELIST)s_Embedded_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
				--is_embedded=true --output_name=%(JOB)s.root &> jobs/%(JOB)s-1.log" jobs/%(JOB)s-1.sh' % vars())
			os.system('%(JOBSUBMIT)s jobs/%(JOB)s-1.sh' % vars())

		if '2' in scales:
			JOB='Embedded_%s_2012' % ch
			os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s --mass_scale_mode=3 --filelist=%(FILELIST)s_Embedded_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
				--is_embedded=true --output_name=%(JOB)s.root &> jobs/%(JOB)s-3.log" jobs/%(JOB)s-3.sh' % vars())
			os.system('%(JOBSUBMIT)s jobs/%(JOB)s-3.sh' % vars())





