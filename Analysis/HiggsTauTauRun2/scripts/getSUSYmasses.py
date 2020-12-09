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

for f in files_in_dir:
  if 'SUSY' in f and channel in f:
    if 'GluGluToH' in f:
      ggh_masses.append(int(f.split('M-')[1].split(channel)[0]))
    elif 'GluGluToBBH' in f:
      if 'NLO' in f:
        bbh_nlo_masses.append(int(f.split('M-')[1].split('-NLO'+channel)[0]))
      else:
        bbh_masses.append(int(f.split('M-')[1].split(channel)[0]))

print 'ggh_masses={0}'.format(','.join(map(str, sorted(ggh_masses))))
print 'bbh_nlo_masses={0}'.format(','.join(map(str, sorted(bbh_nlo_masses))))
print 'bbh_masses={0}'.format(','.join(map(str, sorted(bbh_masses))))

    
