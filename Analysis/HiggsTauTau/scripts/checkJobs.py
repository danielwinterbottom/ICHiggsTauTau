import shlex
from subprocess import Popen, PIPE
import os 

def run_command(command):
    p = Popen(shlex.split(command), stdout=PIPE, stderr=PIPE)
    out, err = p.communicate()
    print out, err
    return out, err

path = "./jobs"
files = [f for f in os.listdir(path) \
            if os.path.isfile(os.path.join(path, f)) \
            and "2016" in f and f.endswith(".log")]

counter = 0
for file_ in files:
    if "Complete" not in open("{}/{}".format(path, file_)).read():
        print("{}".format(file_.split(".")[0]))
        # run_command("qsub -e /dev/null -o /dev/null -cwd -V -l h_rt=3:0:0 -l h_vmem=18G -q hep.q jobs/{}.sh"
        # run_command("qsub -e /dev/null -o /dev/null -cwd -V -l h_rt=3:0:0 -q hep.q jobs/{}.sh"
               # .format(file_.split(".")[0]))
        counter += 1
print "{} incomplete files".format(counter)

