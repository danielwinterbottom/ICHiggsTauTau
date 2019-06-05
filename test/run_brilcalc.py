import sys
import shlex
from subprocess import Popen, PIPE
from multiprocessing import Pool

def parse_arguments():
    parser.add_argument(
        "--file_", default="", help="File.")
    return parser.parse_args()

def run_command(command):
    print(command)
    p = Popen(shlex.split(command), stdout = PIPE, stderr = PIPE)
    return p.communicate()

def load_files(file_):
    with open(file_) as f:
        lines = f.read().splitlines()
    return lines

def compute_lumi(line):
    run_command("brilcalc lumi -i " 
            + "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18"
            + "/13TeV/PromptReco/Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.txt"
            + " --hltpath {0}_v* -u /fb -o {0}.txt".format(line))

if __name__ == "__main__":
    
    lines = load_files(sys.argv[1])
    print(lines)
 
    pool = Pool(processes=4)
    results = [pool.apply_async(compute_lumi, args=(line,)) for line in lines]
    output = [p.get() for p in results]
    
    print(output)

