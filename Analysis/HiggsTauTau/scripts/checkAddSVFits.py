# python scripts/checkAddSVFits.py 2016
import shlex
from subprocess import Popen, PIPE
import os 
import sys
import argparse
# import xmltodict

def run_command(command):
    p = Popen(shlex.split(command), stdout=PIPE, stderr=PIPE)
    out, err = p.communicate()
    print out, err
    return out, err

def parse_arguments():
    parser = argparse.ArgumentParser()

    parser.add_argument("year", default="2016",
        help="What year to run for")
    parser.add_argument("--to_check", default=None,
        help="Which file(regex) to resubmit")

    return parser.parse_args()

def checkJobs(year, to_check):
    path = "./jobs"
    files = [
        f for f in os.listdir(path) \
        if os.path.isfile(os.path.join(path, f)) \
        and f.endswith("svfit.log") and "{}".format(year) in f \
    ]
    if to_check is not None:
        files = [x for x in files if to_check in x]

    print("Checking era {}\n".format(year))

    counter = 0
    for file_ in files:
        if "Closed file" not in open("{}/{}".format(path, file_)).read():
            print((file_.split(".")[0]))
            counter += 1

            if resubmit:
                run_command(
                    "qsub -e /dev/null -o /dev/null -cwd -V -l h_rt=3:0:0"
                    + " -l h_vmem=24G -q hep.q -pe hep.pe 2"
                    + " jobs/{}.sh".format(file_.split(".")[0])

    print("{} incomplete files".format(counter))

if __name__ == "__main__":
    checkJobs(**vars(parse_arguments()))
