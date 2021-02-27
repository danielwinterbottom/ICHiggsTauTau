import ROOT
import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--crab_output', help= 'Name of crab output directory', default='Sep18_Data_102X_2018')
args = parser.parse_args()

crab_output = args.crab_output

crab_output_files = os.listdir(crab_output)

# Set up dictionary
event_numbers = {}
for crab_output_file in crab_output_files:
  event_numbers[crab_output_file] = {'DAS':0,'ntuple':0}


# Find DAS and ntuple event numbers
for crab_output_file in crab_output_files:
  print "Processing", crab_output_file
  f = open("{}/{}/crab.log".format(crab_output,crab_output_file))
  for line in f:
    if 'config.Data.inputDataset' in line:
      das_filename = line.split("'")[1]
    elif 'config.Data.outLFNDirBase' in line:
      dcache_directory = line.split("'")[1][1:]
    elif 'Task name' in line:
      date_stamp = line.split(':')[4].strip()
      break
  f.close()

  # DAS
  os.system('dasgoclient --query "summary dataset={}" >> das_output.txt'.format(das_filename))
  df = open("das_output.txt")
  event_numbers[crab_output_file]['DAS'] = int(df.read().split('"num_event":')[1].split(',')[0])
  df.close()
  os.system('rm das_output.txt')

  # ntuple
  dcache_cmd = "uberftp sedsk53.grid.hep.ph.ic.ac.uk 'ls -r {}/{}/{}/{}'".format(dcache_directory,das_filename.split("/")[1],crab_output_file,date_stamp)
  os.system('{} >> dcache_files.txt'.format(dcache_cmd))
  nf = open("dcache_files.txt")
  file_list = []
  for dcache_file in nf:
    if '.root' in dcache_file:
      file_list.append(dcache_file.split(' ')[-1])
  nf.close()
  os.system('rm dcache_files.txt')
  for i in file_list:
    rf =  ROOT.TFile.Open('root://gfe02.grid.hep.ph.ic.ac.uk:1097/{}'.format(i).strip())
    ep = rf.Get('icEventProducer')
    t = ep.Get('EventTree')
    event_numbers[crab_output_file]['ntuple'] += int(t.GetEntries())
    rf.Close()

# Print output
print "Number of events dcache/DAS:"
for key,val in event_numbers.items():
  print key.replace('crab_',""),val['ntuple'],'/',val['DAS'],'=',100*float(val['ntuple'])/float(val['DAS']),"%" 
  

