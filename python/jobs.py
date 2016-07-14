import os
import json
import stat
import copy
import glob
from math import ceil
from functools import partial
from multiprocessing import Pool

JOB_PREFIX = """#!/bin/sh
set -o pipefail
cd %(CMSSW_BASE)s/src
export SCRAM_ARCH=%(SCRAM_ARCH)s
eval `scramv1 runtime -sh`
cd %(PWD)s
""" % ({
    'CMSSW_BASE': os.environ['CMSSW_BASE'],
    'SCRAM_ARCH': os.environ['SCRAM_ARCH'],
    'PWD': os.environ['PWD']
})

JOB_NAF_PREFIX = """#!/bin/sh
set -o pipefail
source /cvmfs/cms.cern.ch/cmsset_default.sh
cd %(CMSSW_BASE)s/src
export SCRAM_ARCH=%(SCRAM_ARCH)s
eval `scramv1 runtime -sh`
cd %(PWD)s
""" % ({
    'CMSSW_BASE': os.environ['CMSSW_BASE'],
    'SCRAM_ARCH': os.environ['SCRAM_ARCH'],
    'PWD': os.environ['PWD']
})

ERROR_CAPTURE = """
function error_exit
{
    mv %s %s.$1
    exit $1
}

trap 'error_exit $?' ERR
"""


def run_command(dry_run, command):
    if not dry_run:
        print '>> ' + command
        return os.system(command)
    else:
        print '[DRY-RUN]: ' + command


class Jobs:
    description = 'Simple job submission system'

    def __init__(self):
        self.job_queue = []
        self.args = None
        self.job_mode = 'interactive'
        self.parallel = 1
        self.merge = 1
        self.task_name = 'combine_task'
        self.dry_run = False
        self.bopts = ''  # batch submission options
        self.tracking = False
        self.task_dir = ''

    def attach_job_args(self, group):
        group.add_argument('--job-mode', default=self.job_mode, choices=[
                           'interactive', 'script', 'lxbatch', 'NAF', 'ts'], help='Task execution mode')
        group.add_argument('--task-name', default=self.task_name,
                           help='Task name, used for job script and log filenames for batch system tasks')
        group.add_argument('--dir', default=self.task_dir,
                           help='Area for creating the job scripts')
        group.add_argument('--parallel', type=int, default=self.parallel,
                           help='Number of jobs to run in parallel [only affects interactive job-mode]')
        group.add_argument('--merge', type=int, default=self.merge,
                           help='Number of jobs to run in a single script [only affects batch submission]')
        group.add_argument('--dry-run', action='store_true',
                           help='Print commands to the screen but do not run them')
        group.add_argument('--sub-opts', default=self.bopts,
                           help='Options for batch submission')
        group.add_argument('--memory', type=int,
                           help='Request memory for job [MB]')
        group.add_argument('--tracking', nargs='?', default=False, const='short',
                           help='Track job status (if applicable)')

    def set_args(self, args):
        self.args = args
        self.job_mode = self.args.job_mode
        self.task_name = self.args.task_name
        self.parallel = self.args.parallel
        self.merge = self.args.merge
        self.dry_run = self.args.dry_run
        self.bopts = self.args.sub_opts
        self.memory = self.args.memory
        self.tracking = self.args.tracking
        self.task_dir = self.args.dir
        # if self.dry_run:
        #     self.tracking = False

    def file_len(self, fname):
        with open(fname) as f:
            for i, l in enumerate(f):
                pass
            return i + 1

    def add_filelist_split_jobs(self, prog, cfg, files_per_job, output_cfgs):
        if 'filelist' in cfg:
            filelist = cfg['filelist']
            nfiles = self.file_len(filelist)
        elif 'filelists' in cfg:
            nfiles = 0
            for f in cfg['filelists']:
                nfiles += self.file_len(f)
        if files_per_job <= 0:
            njobs = 1
        else:
            njobs = int(ceil(float(nfiles)/float(files_per_job)))
        for n in xrange(njobs):
            final_cfg = copy.deepcopy(cfg)
            for item in output_cfgs:
                filename, extension = os.path.splitext(cfg[item])
                final_cfg[item] = filename + ('_%i' % n) + extension
            final_cfg['file_offset'] = n
            final_cfg['file_step'] = njobs
            cmd = """%s '%s'""" % (prog, json.dumps(final_cfg))
            self.job_queue.append(cmd)
            # print cmd

    def create_job_script(self, commands, script_filename, do_log=False):
        fname = script_filename
        logname = script_filename.replace('.sh', '.log')
        with open(fname, "w") as text_file:
            if self.job_mode == 'NAF':
                text_file.write(JOB_NAF_PREFIX)
            else:
                text_file.write(JOB_PREFIX)
            if self.tracking:
                full_path = os.path.abspath(script_filename)
                text_file.write('mv %s %s\n' % (
                        full_path.replace('.sh', '.status.submitted'),
                        full_path.replace('.sh', '.status.running')
                    ))
                text_file.write(ERROR_CAPTURE % (
                        full_path.replace('.sh', '.status.running'),
                        full_path.replace('.sh', '.status.error')
                    ))
            for i, command in enumerate(commands):
                tee = 'tee' if i == 0 else 'tee -a'
                log_part = '\n'
                if do_log: log_part = ' 2>&1 | %s ' % tee + logname + log_part
                text_file.write(command + log_part)
                if self.tracking:
                    full_path = os.path.abspath(script_filename)
                    text_file.write('mv %s %s\n' % (
                            full_path.replace('.sh', '.status.running'),
                            full_path.replace('.sh', '.status.done')
                        ))
        st = os.stat(fname)
        os.chmod(fname, st.st_mode | stat.S_IEXEC)
        print 'Created job script: %s' % script_filename
        if self.tracking and not self.dry_run:
            trackname = script_filename.replace('.sh', '.status.created')
            open(trackname, 'a').close()

    def read_job_status(self, script_filename):
        full_path = os.path.abspath(script_filename)
        status_files = glob.glob(full_path.replace('.sh', '.status.*'))
        statuses = []
        for status_f in status_files:
            pos = status_f.rfind('status')
            status = (status_f[pos:]).split('.')[1:]
            statuses.append(status)
        # print statuses
        return statuses

    def flush_queue(self):
        if self.job_mode == 'interactive':
            pool = Pool(processes=self.parallel)
            result = pool.map(
                partial(run_command, self.dry_run), self.job_queue)
        script_list = []
        status_result = {}
        njobs = 0
        if self.job_mode in ['script', 'lxbatch', 'NAF', 'ts']:
            for i, j in enumerate(range(0, len(self.job_queue), self.merge)):
                njobs += 1
                script_name = 'job_%s_%i.sh' % (self.task_name, i)
                if self.task_dir is not '':
                    script_name = os.path.join(self.task_dir, script_name)
                status = 'unknown'
                if self.tracking:
                    statuses = self.read_job_status(script_name)
                    if len(statuses) == 0:
                        status = 'new'
                    elif len(statuses) == 1:
                        status = statuses[0][0]
                    else:
                        status = 'confused'
                    if status not in status_result:
                       status_result[status] = []
                    status_result[status].append(script_name)
                    if len(statuses) > 0:
                        # print '%s appears to already be in progress, skipping...' % script_name
                        continue

                # each job is given a slice from the list of combine commands of length 'merge'
                # we also keep track of the files that were created in case submission to a
                # batch system was also requested
                self.create_job_script(
                    self.job_queue[j:j + self.merge], script_name, self.job_mode in ['script', 'ts'])
                script_list.append(script_name)
        if self.job_mode == 'lxbatch':
            for script in script_list:
                full_script = os.path.abspath(script)
                logname = full_script.replace('.sh', '_%J.log')
                if self.tracking and not self.dry_run:
                    os.rename(full_script.replace('.sh', '.status.created'), full_script.replace('.sh', '.status.submitted'))
                run_command(self.dry_run, 'bsub -o %s %s %s' % (logname, self.bopts, full_script))
        if self.job_mode == 'NAF':
            for script in script_list:
                full_script = os.path.abspath(script)
                logname = full_script.replace('.sh', '_$JOB_ID.log')
                if self.tracking and not self.dry_run:
                    os.rename(full_script.replace('.sh', '.status.created'), full_script.replace('.sh', '.status.submitted'))
                run_command(self.dry_run, """qsub -j y -o '%s' %s %s""" % (logname, self.bopts, full_script))
        if self.job_mode == 'ts':
            for script in script_list:
                full_script = os.path.abspath(script)
                if self.tracking and not self.dry_run:
                    os.rename(full_script.replace('.sh', '.status.created'), full_script.replace('.sh', '.status.submitted'))
                run_command(self.dry_run, 'ts bash -c "eval %s"' % (full_script))
        if self.job_mode in ['lxbatch', 'NAF', 'ts'] and self.tracking:
            print '>> Status summary'
            for status in status_result:
                counter = '[%i/%i]' % (len(status_result[status]), njobs)
                print '%20s %10s' % (status, counter)
                if self.tracking == 'long':
                    for f in status_result[status]:
                        print ' '*20 + '%s' % f



        del self.job_queue[:]
