import os
import json
import stat
from math import ceil
from functools import partial
from multiprocessing import Pool


JOB_PREFIX = """#!/bin/sh
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

    def attach_job_args(self, group):
        group.add_argument('--job-mode', default=self.job_mode, choices=[
                           'interactive', 'script', 'lxbatch', 'NAF'], help='Task execution mode')
        group.add_argument('--task-name', default=self.task_name,
                           help='Task name, used for job script and log filenames for batch system tasks')
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

    def set_args(self, args):
        self.args = args
        self.job_mode = self.args.job_mode
        self.task_name = self.args.task_name
        self.parallel = self.args.parallel
        self.merge = self.args.merge
        self.dry_run = self.args.dry_run
        self.bopts = self.args.sub_opts
        self.memory = self.args.memory

    def file_len(self, fname):
        with open(fname) as f:
            for i, l in enumerate(f):
                pass
            return i + 1

    def add_filelist_split_jobs(self, prog, cfg, files_per_job):
        if 'filelist' in cfg:
            filelist = cfg['filelist']
            nfiles = self.file_len(filelist)
        elif 'filelists' in cfg:
            nfiles = 0
            for f in cfg['filelists']:
                nfiles += self.file_len(f)
        njobs = int(ceil(float(nfiles)/float(files_per_job)))
        output_name = cfg['output']
        for n in xrange(njobs):
            # output_name = cfg['output']
            pos = output_name.rfind('.root')
            # Replace .root with _JOB.root in the output filename
            cfg['output'] = output_name[:pos] + '_%i.root' % n
            cfg['file_offset'] = n
            cfg['file_step'] = njobs
            cmd = """%s '%s'""" % (prog, json.dumps(cfg))
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
            for i, command in enumerate(commands):
                tee = 'tee' if i == 0 else 'tee -a'
                log_part = '\n'
                if do_log: log_part = ' 2>&1 | %s ' % tee + logname + log_part
                # if command.startswith('combine') or command.startswith('pushd'):
                #     text_file.write(
                #         'eval ' + command + log_part)
                # else:
                #     text_file.write(command)
                text_file.write(command + log_part)
        st = os.stat(fname)
        os.chmod(fname, st.st_mode | stat.S_IEXEC)
        print 'Created job script: %s' % script_filename

    def flush_queue(self):
        if self.job_mode == 'interactive':
            pool = Pool(processes=self.parallel)
            result = pool.map(
                partial(run_command, self.dry_run), self.job_queue)
        script_list = []
        if self.job_mode in ['script', 'lxbatch', 'NAF']:
            for i, j in enumerate(range(0, len(self.job_queue), self.merge)):
                script_name = 'job_%s_%i.sh' % (self.task_name, i)
                # each job is given a slice from the list of combine commands of length 'merge'
                # we also keep track of the files that were created in case submission to a
                # batch system was also requested
                self.create_job_script(
                    self.job_queue[j:j + self.merge], script_name, self.job_mode == 'script')
                script_list.append(script_name)
        if self.job_mode == 'lxbatch':
            for script in script_list:
                full_script = os.path.abspath(script)
                logname = full_script.replace('.sh', '_%J.log')
                run_command(self.dry_run, 'bsub -o %s %s %s' % (logname, self.bopts, full_script))
        if self.job_mode == 'NAF':
            for script in script_list:
                full_script = os.path.abspath(script)
                logname = full_script.replace('.sh', '_%J.log')
                run_command(self.dry_run, 'qsub -o %s %s %s' % (logname, self.bopts, full_script))
        del self.job_queue[:]
