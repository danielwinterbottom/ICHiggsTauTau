# Example usage
#
# python ./scripts/launch_example.py --filelist filelist.dat --task-name sampleX --job-mode condor --sub-opts='+JobFlavour = "workday"' --merge 2 --dry-run
#


from UserCode.ICHiggsTauTau.jobs import Jobs
import argparse

job_mgr = Jobs()
parser = argparse.ArgumentParser()

parser.add_argument('--filelist', default='')

job_mgr.attach_job_args(parser)
args = parser.parse_args()
job_mgr.set_args(args)


with open(args.filelist) as f:
    content = f.readlines()
content = [x.strip() for x in content]

for i, f in enumerate(content):
    job_mgr.job_queue.append('cmsRun config_cfg.py --input %s --output output_%s_%i.root' % (f, job_mgr.task_name, i))

job_mgr.flush_queue()

