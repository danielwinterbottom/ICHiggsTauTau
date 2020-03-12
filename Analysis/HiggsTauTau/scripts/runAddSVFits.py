# python scripts/runAddSVFits.py --path /vols/cms/akd116/Offline/output/SM/2019/CP_2018_v5/ --svfit_path /vols/cms/akd116/Offline/output/SM/2019/2018_nominalSVFit/ --year 2018 --tag svfit_mass --channel tt

import glob
import yaml
import argparse
import os
import shlex
from subprocess import Popen, PIPE

JOBWRAPPER      = './scripts/generate_job.sh'
JOBSUBMIT       = './scripts/submit_ic_batch_job.sh "hep.q -l h_rt=0:180:0"'

def run_command(command):
    print command
    p = Popen(shlex.split(command), stdout = PIPE, stderr = PIPE)
    out, err = p.communicate()
    print out, err
    return out, err

def parse_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument("--path", default="./output/",
            help="Where are the trees to be annotated stored?")
    parser.add_argument("--svfit_path", default="./output/",
            help="Where are the trees to be annotated stored?")
    parser.add_argument("--year", default="2016",
            help="What year to annotate?")
    parser.add_argument("--tag", default="test_batch",
            help="What to call the new branch")
    parser.add_argument("--channel", default="mt",
            help="What channel?")

    return parser.parse_args()

def main(args):

    sample_list = {}
    with open("scripts/sample_list_{}.yaml".format(args.year), "r") as f:
        try:
            sample_list = yaml.safe_load(f)
        except yaml.YAMLError as exc:
            print exc

    subdirs = [""]

#    qsub_command = (
#        'qsub -e /dev/null -o /dev/null -cwd -V -l h_rt=3:0:0'
#        ' -q hep.q -v input="{}",svfit_path="{}",path="{}",tag="{}",'
#        'channel="{}",year="{}"'
#        )

    qsub_command = (
        'qsub -e svfitout/ -o svfitout/ -cwd -V -l h_rt=3:0:0'
        ' -q hep.q -v input="{}",svfit_path="{}",path="{}",tag="{}",'
        'channel="{}",year="{}"'
        )


    for key, samples in sample_list.iteritems():
        for sample in samples:
            # if sample in [
            #     "WWToLNuQQ",]:

            for subdir in subdirs:
                #print("{}/{}_{}_{}.root".format(
                #    args.path, sample, args.channel, args.year
                #))
                if glob.glob("{}/{}_{}_{}.root".format(
                    args.path, sample, args.channel, args.year
                    )):

                    run_command(
                        qsub_command.format(sample, args.svfit_path, 
                        args.path, args.tag, args.channel, args.year)
                        + ' ./scripts/batch_addSVFits.sh'
                    )
                    print( qsub_command.format(sample, args.svfit_path,
                        args.path, args.tag, args.channel, args.year)
                        + ' ./scripts/batch_addSVFits.sh'
)

if __name__ == "__main__":
    args = parse_arguments()
    main(args)
