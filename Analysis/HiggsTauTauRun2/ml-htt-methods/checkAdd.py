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

    parser.add_argument("path", default="",
        help="What path to run for")
    parser.add_argument("--to_check", default=None,
        help="Which file(regex) to resubmit")

    return parser.parse_args()

def checkJobs(path, to_check):

    #print os.listdir(path)

    files = [
        f for f in os.listdir(path) \
        if os.path.isfile(os.path.join(path, f)) \
        and f.endswith(".log") \
    ]
    if to_check is not None:
        files = [x for x in files if to_check in x]

    print("Checking path {}\n".format(path))

    counter = 0
    for file_ in files:
        #print file_
        #print "{}/{}".format(path, file_)
        #if "Closed file" not in open("{}/{}".format(path, file_)).read():
        if "Closed file" not in open("{}/{}".format(path, file_)).read():
           
            print((file_.split(".")[0]))
            counter += 1

    print("{} incomplete files".format(counter))


if __name__ == "__main__":
    checkJobs(**vars(parse_arguments()))

