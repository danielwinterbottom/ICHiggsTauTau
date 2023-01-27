import argparse
import os

parser = argparse.ArgumentParser()
parser.add_argument('--channel',help= 'Channel to search for', default='mt')
parser.add_argument('--folder', help= 'Name of input directory', default='/vols/cms/gu18/Offline/output/MSSM/mssm_2016/')
args = parser.parse_args()

channel = '_'+args.channel+'_'
input_folder = args.folder

files_in_dir = os.listdir(input_folder)

bbh_nlo_masses = []
bbh_masses = []
ggh_masses = []
ggh_masses_powheg = []
bbh_masses_powheg = []

for f in files_in_dir:
  if 'SUSY' in f and channel in f:
    if 'GluGluToH' in f:
      if 'powheg' in f:
        ggh_masses_powheg.append(int(f.split('M-')[1].split('_powheg'+channel)[0]))
      else:
        ggh_masses.append(int(f.split('M-')[1].split(channel)[0]))
    elif 'GluGluToBBH' in f:
      if 'NLO' in f:
        bbh_nlo_masses.append(int(f.split('M-')[1].split('-NLO'+channel)[0]))
      elif 'powheg' in f:
        bbh_masses_powheg.append(int(f.split('M-')[1].split('_powheg'+channel)[0]))
      else:
        bbh_masses.append(int(f.split('M-')[1].split(channel)[0]))

print 'ggh_masses={0}'.format(','.join(map(str, sorted(ggh_masses))))
print 'bbh_nlo_masses={0}'.format(','.join(map(str, sorted(bbh_nlo_masses))))
print 'bbh_masses={0}'.format(','.join(map(str, sorted(bbh_masses))))
print 'ggh_masses_powheg={0}'.format(','.join(map(str, sorted(ggh_masses_powheg))))
print 'bbh_masses_powheg={0}'.format(','.join(map(str, sorted(bbh_masses_powheg))))


    
