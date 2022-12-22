#python check_and_resubmit_crab_jobs.py --directory=Nov30_Data_106X_2018 --user=gu18
import argparse
import os
import time
from collections import OrderedDict

parser = argparse.ArgumentParser()
parser.add_argument("--resubmit", action='store_true',help="Resbmit jobs")
parser.add_argument('--duration',help= 'Number of minutes to run for', default='0.1')
parser.add_argument('--interval',help= 'How often it will be run', default='0')
parser.add_argument('--directory',help='location of crab folders', default='./')
parser.add_argument('--user',help='vols username', default='gu18')
args = parser.parse_args()

exit_codes_dict = {"50660":" --maxmemory=5000","50664":" --numcores=3"}

def get_status(folder,status_dict,directory):
  os.system("crab status %(directory)s/crab_%(folder)s >> crab_status_output.txt" % vars())
  crab_status_file = open('crab_status_output.txt', 'r')
  tmp_files = True
  nnl = False
  jaf = False
  for line in crab_status_file:
    if ("Jobs status" in line or nnl == True) and line != "\n":
      nnl = True
      if "finished" in line: status_dict[folder]["Finished"] = int(line.split("(")[1].split("/")[0])
      elif "running" in line: status_dict[folder]["Running"] = int(line.split("(")[1].split("/")[0])
      elif "transferring" in line: status_dict[folder]["Transferring"] = int(line.split("(")[1].split("/")[0])
      elif "failed" in line: status_dict[folder]["Failed"] = int(line.split("(")[1].split("/")[0])
      elif "idle" in line: 
        if "Idle" in status_dict[folder].keys():
          status_dict[folder]["Idle"] += int(line.split("(")[1].split("/")[0])
        else:
          status_dict[folder]["Idle"] = int(line.split("(")[1].split("/")[0])
      elif "toRetry" in line:
        if "Idle" in status_dict[folder].keys():
          status_dict[folder]["Idle"] += int(line.split("(")[1].split("/")[0])
        else:
          status_dict[folder]["Idle"] = int(line.split("(")[1].split("/")[0])
    elif "SUBMITFAILED" in line: 
      status_dict[folder]["Warnings"] = "Not submitted."
    elif "Cannot find .requestcache file" in line:
      status_dict[folder]["Warnings"] = "Not found"
      tmp_files = False
    elif line == "\n":
      nnl = False
    if "jobs failed with exit code" in line:
      if not jaf:
        status_dict[folder]["Exit Codes"] = line.split("jobs failed with exit code")[0].strip()+"x"+line.split("jobs failed with exit code")[1].strip()
        jaf = True
      else:
        status_dict[folder]["Exit Codes"] += " "+line.split("jobs failed with exit code")[0].strip()+"x"+line.split("jobs failed with exit code")[1].strip()
  os.system("rm crab_status_output.txt")
  if tmp_files:
    # Remove these files so not to clog up tmp directory
    os.system("find /tmp/ -user {} 2>&1 -name \"curlData*\" | grep -v \"Permission denied\" | xargs rm &> /dev/null".format(args.user))
    os.system("find /tmp/ -user {} 2>&1 -name \"status-cache*\" | grep -v \"Permission denied\" | xargs rm &> /dev/null".format(args.user))
  return status_dict

start_time = time.time()
while (time.time() - start_time) < float(args.duration)*60:
  print "Time",round((time.time() - start_time)/60,4),"minutes"
  # find job information
  for dir in args.directory.split(","):
    status_dict = {}
    print "Using directory", dir
    for folder in os.listdir(dir):
      folder = folder.replace("crab_","")
      if os.path.isdir(dir+'/'+"crab_"+folder) and os.path.isfile(dir+"/crab_"+folder+"/crab.log"):

        status_dict[folder] = OrderedDict([("Finished",0),("Running",0),("Transferring",0),("Failed",0),("Idle",0),("Exit Codes",""),("Percentage",0),("Warnings","")]) # set up dictionary for each folder
        status_dict = get_status(folder,status_dict,dir)
        if float(status_dict[folder]["Finished"]+status_dict[folder]["Running"]+status_dict[folder]["Transferring"]+status_dict[folder]["Failed"]+status_dict[folder]["Idle"]) == 0:
          status_dict[folder]["Percentage"] = 0
        else:
          status_dict[folder]["Percentage"] = 100*round(float(status_dict[folder]["Finished"])/float(status_dict[folder]["Finished"]+status_dict[folder]["Running"]+status_dict[folder]["Transferring"]+status_dict[folder]["Failed"]+status_dict[folder]["Idle"]),5)


    # print dictionary
    status_dict = OrderedDict(sorted(status_dict.items()))
    spacing = [70,10,10,15,10,10,30,10,20]
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
          #if not status["Exit Codes"] == "8028":
          #print status["Exit Codes"]
          for ec_key, ec_val in exit_codes_dict.items():
            if ec_key in status_val["Exit Codes"]:
              add_cond += ec_val
          os.system("crab resubmit%(add_cond)s %(dir)s/crab_%(status_key)s >> crab_resubmit.txt" % vars())
          os.system("rm crab_resubmit.txt")

  time.sleep(int(args.interval)*60)

