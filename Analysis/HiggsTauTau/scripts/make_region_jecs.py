import math
infile='input/jec/Summer16_23Sep2016HV4_DATA_UncertaintySources_AK4PFchs.txt'

def GetLinesForSource(source,data):
  first=0
  last=0
  found=False
  for i, l in enumerate(data):
    if source in l: 
      first=i+1 # +1 skips the line that has no JES uncertainty numbers 
      found=True
      continue
    if found and '[' in l and ']' in l: 
      last = i
      break
  if not found: 
    print 'Did not find source %s, not incuding this source in grouping' % source
    out=[]  
  else: out = data[first+1:last]
  return out  

blah='-5.4 -5.0 150 9.0 0.0041 0.0041 11.0 0.0037 0.0037 13.5 0.0033 0.0033 16.5 0.0029 0.0029 19.5 0.0026 0.0026 22.5 0.0024 0.0024 26.0 0.0021 0.0021 30.0 0.0018 0.0018 34.5 0.0016 0.0016 40.0 0.0013 0.0013 46.0 0.0010 0.0010 52.5 0.0007 0.0007 60.0 0.0004 0.0004 69.0 0.0000 0.0000 79.0 0.0000 0.0000 90.5 0.0000 0.0000 105.5 0.0000 0.0000 123.5 0.0000 0.0000 143.0 0.0000 0.0000 163.5 0.0000 0.0000 185.0 0.0000 0.0000 208.0 0.0002 0.0002 232.5 0.0005 0.0005 258.5 0.0007 0.0007 286.0 0.0009 0.0009 331.0 0.0011 0.0011 396.0 0.0014 0.0014 468.5 0.0016 0.0016 549.5 0.0018 0.0018 639.0 0.0020 0.0020 738.0 0.0021 0.0021 847.5 0.0023 0.0023 968.5 0.0024 0.0024 1102.0 0.0025 0.0025 1249.5 0.0027 0.0027 1412.0 0.0028 0.0028 1590.5 0.0029 0.0029 1787.0 0.0030 0.0030 2003.0 0.0031 0.0031 2241.0 0.0032 0.0032 2503.0 0.0033 0.0033 2790.5 0.0034 0.0034 3107.0 0.0035 0.0035 3455.0 0.0036 0.0036 3837.0 0.0037 0.0037 4257.0 0.0038 0.0038 4719.0 0.0038 0.0038 5226.5 0.0039 0.0039 5784.0 0.0040 0.0040 6538.0 0.0041 0.0041'


def GetValues(line):
  vals = line.split()
  eta_min = vals[0]
  eta_max = vals[1]
  nentries= vals[2]
  pt_vals = []
  shifts = []
  for i in range(3,len(vals),3):
    pt_vals.append(vals[i])
    shifts.append((vals[i+1],vals[i+2]))
  return (eta_min, eta_max, nentries, pt_vals, shifts)

def AddinQuad(a,b):
  c = []
  for i, ai in enumerate(a):
    c1 = math.sqrt(float(ai[0])**2+float(b[i][0])**2)
    c2 = math.sqrt(float(ai[1])**2+float(b[i][1])**2)
    c.append((c1,c2))
  return c

with open (infile, "r") as myfile:
    data=myfile.readlines()

groupings = {}

groupings['Eta0to5'] = ["SinglePionECAL", "SinglePionHCAL", "AbsoluteFlavMap", "AbsoluteMPFBias", "AbsoluteScale", "AbsoluteStat", "Fragmentation", "FlavorQCD", "TimePtEta", "PileUpDataMC", "RelativeFSR", "RelativeStatFSR", "PileUpPtRef", "AboluteSample"] 
groupings['Eta0to3'] = ["PileUpPtEC1", "PileUpPtBB", "RelativeJEREC1", "RelativePtEC1", "RelativeStatEC", "RelativePtBB"]
groupings['Eta3to5'] = ["RelativeStatHF", "RelativePtHF", "PileUpPtHF", "RelativeJERHF"]
groupings['EC2'] = ["PileUpPtEC2", "RelativeJEREC2", "RelativePtEC2"]


for g in groupings:
  print 'Combining JES uncertainties for group: %s' %g
  lines = {}
  for s in groupings[g]:
    lines[s] = GetLinesForSource(s,data)
  for i in range(0,len(lines[groupings[g][0]])):
    total_shifts = []
    first=True
    for s in groupings[g]:
      if len(lines[s])==0: continue
      (eta_min,eta_max,nentries,pt_vals,shifts) = GetValues(lines[s][i])  
      if first: 
        total_shifts = shifts
        first=False
      else: total_shifts = AddinQuad(total_shifts,shifts)
  break


