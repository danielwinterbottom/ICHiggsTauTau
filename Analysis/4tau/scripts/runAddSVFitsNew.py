#python scripts/runAddSVFitsNew.py --path /vols/cms/dw515/Offline/output/SM/Mar27_2018/ --svfit_path /vols/cms/dw515/Offline/output/SM/Jan24_2018_svfit/ --year 2018 --tag svfit_mass --channel tt

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

    cwd = os.getcwd()
    os.system('mkdir -p svfitout')

    sample_list = {}
    with open("scripts/sample_list_{}.yaml".format(args.year), "r") as f:
        try:
            sample_list = yaml.safe_load(f)
        except yaml.YAMLError as exc:
            print exc

    subdirs = [""]

    subdirs = [
        '',
        'TSCALE0PI_UP','TSCALE0PI_DOWN','TSCALE1PI_UP','TSCALE1PI_DOWN',
        'TSCALE3PRONG_UP','TSCALE3PRONG_DOWN',
        'TSCALE3PRONG1PI0_DOWN','TSCALE3PRONG1PI0_UP',
        'MET_SCALE_UP','MET_SCALE_DOWN','MET_RES_UP','MET_RES_DOWN', 
        'EFAKE0PI_UP', 'EFAKE0PI_DOWN','EFAKE1PI_UP','EFAKE1PI_DOWN',
        'MUFAKE0PI_UP','MUFAKE0PI_DOWN','MUFAKE1PI_UP','MUFAKE1PI_DOWN',
        'METUNCL_UP','METUNCL_DOWN','MUSCALE_UP','MUSCALE_DOWN',
        'ESCALE_UP','ESCALE_DOWN','JESRBAL_DOWN','JESRBAL_UP',
        'JESABS_DOWN','JESABS_UP','JESABS_YEAR_DOWN','JESABS_YEAR_UP',
        'JESFLAV_DOWN','JESFLAV_UP','JESBBEC1_DOWN','JESBBEC1_UP',
        'JESBBEC1_YEAR_DOWN','JESBBEC1_YEAR_UP','JESEC2_DOWN','JESEC2_UP',
        'JESEC2_YEAR_DOWN','JESEC2_YEAR_UP','JESHF_DOWN','JESHF_UP',
        'JESHF_YEAR_DOWN','JESHF_YEAR_UP',
        'JESRELSAMP_YEAR_DOWN','JESRELSAMP_YEAR_UP',
        'JER_UP','JER_DOWN',
    ]

    qsub_command = (
        'qsub -e svfitout/ -o svfitout/ -cwd -V -l h_rt=3:0:0'
        ' -q hep.q -v input="{}",svfit_path="{}",path="{}",tag="{}",'
        'channel="{}",year="{}",extra="{}",dir="{}"'
        )


    for key, samples in sample_list.iteritems():
        for sample in samples:
            #if sample not in [
            # '', # add samples here if you only want to run specific ones
            #]: continue

            print sample

            for subdir in subdirs:
                #print("{}/{}_{}_{}.root".format(
                #    args.path, sample, args.channel, args.year
                #))
                if glob.glob("{}/{}_{}_{}.root".format(
                    args.path+'/'+subdir+'/', sample, args.channel, args.year
                    )):

                    run_command(
                        qsub_command.format(sample, args.svfit_path+'/'+subdir+'/', 
                        args.path+'/'+subdir+'/', args.tag, args.channel, args.year,subdir, cwd)
                        + ' ./scripts/batch_addSVFits.sh'
                    )
                    print( qsub_command.format(sample, args.svfit_path+'/'+subdir+'/',
                        args.path+'/'+subdir+'/', args.tag, args.channel, args.year,subdir, cwd)
                        + ' ./scripts/batch_addSVFits.sh'
                    )
                    f = open('jobs/%s_%s_%s_%s_svfit.sh' % (sample, args.channel, args.year , subdir ),'w')
                    f.write( qsub_command.format(sample, args.svfit_path+'/'+subdir+'/',
                        args.path+'/'+subdir+'/', args.tag, args.channel, args.year,subdir, cwd)
                        + ' ./scripts/batch_addSVFits.sh'
                    )
                    f.close()
                    os.system('chmod 755 jobs/%s_%s_%s_%s_svfit.sh' % (sample, args.channel, args.year , subdir ))

if __name__ == "__main__":
    args = parse_arguments()
    main(args)
