import shlex
from subprocess import Popen, PIPE
import os 
import sys

def run_command(command):
    p = Popen(shlex.split(command), stdout=PIPE, stderr=PIPE)
    out, err = p.communicate()
    print out, err
    return out, err

path = "./jobs"
files = [
        f for f in os.listdir(path) \
                if os.path.isfile(os.path.join(path, f)) \
                and f.endswith(".log") and "{}".format(sys.argv[1]) in f \
                # and "SingleMuo" not in f 
                # and "amcat" in f
                # and "GluGluToH" not in f
                # and "VBFH" in f
                # and "JJH" in f
            ]
print("Checking era {}\n".format(sys.argv[1]))

counter = 0
for file_ in files:
    if "Complete" not in open("{}/{}".format(path, file_)).read():# \
            # or "-nan" in open("{}/{}".format(path, file_)).read():
        # if "bad_alloc"  not in open("{}/{}".format(path, file_)).read():
        #     print("{}".format(file_.split(".")[0]))
        print("{}".format(file_.split(".")[0]))
        # run_command("qsub -e /dev/null -o /dev/null -cwd -V -l h_rt=3:0:0 -l h_vmem=24G -q hep.q jobs/{}.sh"
        # run_command("qsub -e /dev/null -o /dev/null -cwd -V -l h_rt=3:0:0 -q hep.q jobs/{}.sh"
               # .format(file_.split(".")[0]))
        counter += 1
print "{} incomplete files".format(counter)

