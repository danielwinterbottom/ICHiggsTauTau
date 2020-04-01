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
    parser.add_argument("--resubmit", default=False, action="store_true",
        help="Resubmit failed/incomplete jobs?")
    parser.add_argument("--to_check", default=None,
        help="Which file(regex) to resubmit")

    return parser.parse_args()

def checkJobs(year, resubmit, to_check):
    # path = "/vols/build/cms/akd116/newest/run/CMSSW_8_0_25/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/jobs"
    path = "./jobs"
    files = [
        f for f in os.listdir(path) \
        if os.path.isfile(os.path.join(path, f)) \
        and f.endswith(".log") and "{}".format(year) in f \
    ]
    if to_check is not None:
        files = [x for x in files if to_check in x]

    print("Checking era {}\n".format(year))

    # check running jobs
    # out, _ = run_command("qstat -u akd116 -xml")
    # jobs = xmltodict.parse(out)["job_info"]["job_info"]["job_list"]
    # for job in jobs:
    #     print(dict(job))
    #     break
    # assert False

    counter = 0
    for file_ in files:
        if "End of job" not in open("{}/{}".format(path, file_)).read():
            print((file_.split(".")[0]))
            counter += 1

            if resubmit:
                run_command(
                    "qsub -e /dev/null -o /dev/null -cwd -V -l h_rt=3:0:0"
                    + " -l h_vmem=24G -q hep.q -pe hep.pe 2"
                    + " jobs/{}.sh".format(file_.split(".")[0])
                )
        # if "W4JetsToLNu-LO_2017-4" in file_:
        #     break

    print("{} incomplete files".format(counter))

if __name__ == "__main__":
    checkJobs(**vars(parse_arguments()))
