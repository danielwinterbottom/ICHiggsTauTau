year=2017

if year == 2017: year_ = 2018
else: year_ = year

in_file = 'scripts/signal_xs_%(year_)i.txt' % vars()

if year == 2016: evt_file = 'scripts/evt_leg%(year)i.txt' % vars()
else: evt_file = 'scripts/evt_%(year)i.txt' % vars()


xs_map = {}

file1 = open(in_file, 'r') 
Lines = file1.readlines() 
for line in Lines:
  if 'JJH' not in line: continue
  xs_map[line.split()[0]] = line.split()[-2:]

print xs_map

evt_map = {}

file2 = open(evt_file, 'r')
Lines = file2.readlines()
for line in Lines:
  if 'JJH' not in line or 'Higgs' in line: continue
  evt_map[line.split()[0]] = line.split()[1]

print evt_map

xs = 3.045966 

out=''

for x in ['0PM', '0M', '0Mf05ph0']:

  evt0 = int(evt_map['JJH%(x)sToTauTauPlusZeroJets' % vars()])
  evt1 = int(evt_map['JJH%(x)sToTauTauPlusOneJets' % vars()])
  evt2 = int(evt_map['JJH%(x)sToTauTauPlusTwoJets' % vars()])

  evt0_filt = int(evt_map['JJH%(x)sToTauTauPlusZeroJets_Filtered' % vars()])
  evt1_filt = int(evt_map['JJH%(x)sToTauTauPlusOneJets_Filtered' % vars()])
  evt2_filt = int(evt_map['JJH%(x)sToTauTauPlusTwoJets_Filtered' % vars()])

  xs0 = float(xs_map['JJH%(x)sToTauTauPlusZeroJets' % vars()][0])
  eff0 = float(xs_map['JJH%(x)sToTauTauPlusZeroJets' % vars()][1]) 

  xs1 = float(xs_map['JJH%(x)sToTauTauPlusOneJets' % vars()][0])
  eff1 = float(xs_map['JJH%(x)sToTauTauPlusOneJets' % vars()][1])  

  xs2 = float(xs_map['JJH%(x)sToTauTauPlusTwoJets' % vars()][0])
  eff2 = float(xs_map['JJH%(x)sToTauTauPlusTwoJets' % vars()][1])

  xs_nlo = xs0+xs1+xs2
  xs0_new = xs*xs0/xs_nlo
  xs1_new = xs*xs1/xs_nlo
  xs2_new = xs*xs2/xs_nlo

  if x == '0Mf05ph0':
    xs0_new*=2
    xs1_new*=2
    xs2_new*=2

  print 'JJH%(x)sToTauTauPlusZeroJets %(xs0_new).4f' % vars()
  print 'JJH%(x)sToTauTauPlusZeroJets_Filtered %(xs0_new).4f' % vars()
   
  print 'JJH%(x)sToTauTauPlusOneJets %(xs1_new).4f' % vars()
  print 'JJH%(x)sToTauTauPlusOneJets_Filtered %(xs1_new).4f' % vars()

  print 'JJH%(x)sToTauTauPlusTwoJets %(xs2_new).4f' % vars()
  print 'JJH%(x)sToTauTauPlusTwoJets_Filtered %(xs2_new).4f' % vars()

  out+='  \"JJH%(x)sToTauTauPlusZeroJets\": {\n' %vars()
  out+='    \"xs\": %(xs0_new).4f,\n' % vars()
  out+='    \"evt\": %(evt0)i\n' % vars()
  out+='  },\n'
  out+='  \"JJH%(x)sToTauTauPlusZeroJets_Filtered\": {\n' % vars()
  out+='    \"xs\": %(xs0_new).4f,\n' % vars()
  out+='    \"evt\": %(evt0_filt)i,\n' % vars() 
  out+='    \"eff\": %(eff0).4f\n' % vars()
  out+='  },\n'

  out+='  \"JJH%(x)sToTauTauPlusOneJets\": {\n' %vars()
  out+='    \"xs\": %(xs1_new).4f,\n' % vars()
  out+='    \"evt\": %(evt1)i\n' % vars()
  out+='  },\n'
  out+='  \"JJH%(x)sToTauTauPlusOneJets_Filtered\": {\n' % vars()
  out+='    \"xs\": %(xs1_new).4f,\n' % vars()
  out+='    \"evt\": %(evt1_filt)i,\n' % vars()
  out+='    \"eff\": %(eff1).4f\n' % vars()
  out+='  },\n'

  out+='  \"JJH%(x)sToTauTauPlusTwoJets\": {\n' %vars()
  out+='    \"xs\": %(xs2_new).4f,\n' % vars()
  out+='    \"evt\": %(evt2)i\n' % vars()
  out+='  },\n'
  out+='  \"JJH%(x)sToTauTauPlusTwoJets_Filtered\": {\n' % vars()
  out+='    \"xs\": %(xs2_new).4f,\n' % vars()
  out+='    \"evt\": %(evt2_filt)i,\n' % vars()
  out+='    \"eff\": %(eff2).4f\n' % vars()
  out+='  },\n'


print out
