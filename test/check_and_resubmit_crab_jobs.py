import argparse
import os
import time

parser = argparse.ArgumentParser()
parser.add_argument("--resubmit", action='store_true',help="Resbmit jobs")
parser.add_argument("--multiple_directories", action='store_true',help="Will look for directory with crab directories directory/crab_directory")
parser.add_argument('--duration',help= 'Number of minutes to run for', default='1')
parser.add_argument('--interval',help= 'How often it will be run', default='0')
parser.add_argument('--directory',help='location of crab folders', default='./')
args = parser.parse_args()

exit_codes_dict = {"50660":" --maxmemory=5000","50664":" --numcores=3"}

def get_status(folder,status_dict,mult_dir,directory):
  if mult_dir:
    os.system("crab status %(folder)s/crab_%(folder)s >> crab_status_output.txt" % vars())
  else:
    os.system("crab status %(directory)s/crab_%(folder)s >> crab_status_output.txt" % vars())
  crab_status_file = open('crab_status_output.txt', 'r')
  nnl = False
  jaf = False
  for line in crab_status_file:
    if ("Jobs status" in line or nnl == True) and line != "\n":
      nnl = True
      if "finished" in line: status_dict[folder]["Finished"] = int(line.split("(")[1].split("/")[0])
      elif "running" in line: status_dict[folder]["Running"] = int(line.split("(")[1].split("/")[0])
      elif "transferring" in line: status_dict[folder]["Transferring"] = int(line.split("(")[1].split("/")[0])
      elif "failed" in line: status_dict[folder]["Failed"] = int(line.split("(")[1].split("/")[0])
      elif "idle" in line: status_dict[folder]["Idle"] = int(line.split("(")[1].split("/")[0])
    elif line == "\n":
      nnl = False
    if "jobs failed with exit code" in line:
      if not jaf:
        status_dict[folder]["Exit Codes"] = line.split("jobs failed with exit code")[0].strip()+"x"+line.split("jobs failed with exit code")[1].strip()
        jaf = True
      else:
        status_dict[folder]["Exit Codes"] += " "+line.split("jobs failed with exit code")[0].strip()+"x"+line.split("jobs failed with exit code")[1].strip()


  os.system("rm crab_status_output.txt")
  return status_dict

start_time = time.time()
while (time.time() - start_time) < int(args.duration)*60:
  print "Time",round((time.time() - start_time)/60,4),"minutes"
  # find job information
  status_dict = {}
  if args.multiple_directories:
    for folder in os.listdir("./"):
      if os.path.isdir(folder) and os.path.isfile(folder+"/crab_"+folder+"/crab.log"):

        status_dict[folder] = {"Finished":0,"Running":0,"Transferring":0,"Failed":0,"Idle":0,"Exit Codes":""} # set up dictionary for each folder
        status_dict = get_status(folder,status_dict,args.multiple_directories,args.directory)
        status_dict[folder]["Percentage"] = 100*round(float(status_dict[folder]["Finished"])/float(status_dict[folder]["Finished"]+status_dict[folder]["Running"]+status_dict[folder]["Transferring"]+status_dict[folder]["Failed"]+status_dict[folder]["Idle"]),5)
  else:
    for folder in os.listdir(args.directory):
      folder = folder.replace("crab_","")
      if os.path.isdir(args.directory+'/'+"crab_"+folder) and os.path.isfile(args.directory+"/crab_"+folder+"/crab.log"):

        status_dict[folder] = {"Finished":0,"Running":0,"Transferring":0,"Failed":0,"Idle":0,"Exit Codes":""} # set up dictionary for each folder
        status_dict = get_status(folder,status_dict,args.multiple_directories,args.directory)
        status_dict[folder]["Percentage"] = 100*round(float(status_dict[folder]["Finished"])/float(status_dict[folder]["Finished"]+status_dict[folder]["Running"]+status_dict[folder]["Transferring"]+status_dict[folder]["Failed"]+status_dict[folder]["Idle"]),5)


  # print dictionary
  spacing = [70,10,10,10,10,10,15,10]
  headings = status_dict[status_dict.keys()[0]].keys()
  headings2 = ["Filename"] + headings
  sep = "+"
  for i in range(0,len(headings2)): sep += "{}+".format("-"*(spacing[i]+3))
  print sep
  string = "| "
  for i in range(0,len(headings2)): string += "{} {} | ".format(headings2[i], " "*(spacing[i]-len(headings2[i])))
  print string
  print sep
  for key, val in status_dict.items():
    entries = [key]
    for i in headings: entries.append(str(val[i]))
    string = "| "
    for i in range(0,len(entries)): string += "{} {} | ".format(entries[i], " "*(spacing[i]-len(entries[i])))
    print string
  print sep

  # resubmit jobs
  if args.resubmit:
    for status_key, status_val in status_dict.items():
      add_cond = ''
      if status_val["Failed"] > 0:
        print "- Resubmiting %(status_key)s" % vars()
        for ec_key, ec_val in exit_codes_dict.items():
          if ec_key in status_val["Exit Codes"]:
            add_cond += ec_val
        if args.multiple_directories:
          os.system("crab resubmit%(add_cond)s %(status_key)s/crab_%(status_key)s >> crab_resubmit.txt" % vars())
        else:
          directory = args.directory
          os.system("crab resubmit%(add_cond)s %(directory)s/crab_%(status_key)s >> crab_resubmit.txt" % vars())
        os.system("rm crab_resubmit.txt")

  time.sleep(int(args.interval)*60)

