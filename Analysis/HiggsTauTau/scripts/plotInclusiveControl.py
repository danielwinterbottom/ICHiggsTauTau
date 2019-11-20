# Example:
# python scripts/plotInclusiveControl.py --era 2018 --channel tt --embedding --ff --signal_scale 50

# to run for different categories (rather than just inclusive)
# for cat in inclusive dijet; do python scripts/plotInclusiveControl.py --era 2018 --channel tt --embedding --ff --signal_scale 50 --cat ${cat}; done

# or to run with several channels:
# for ch in tt mt et em; do python scripts/plotInclusiveControl.py --era 2018 --channel ${ch} --embedding --ff --signal_scale 50; done


from __future__ import print_function
import argparse
import numpy as np
import shlex
from subprocess import Popen, PIPE

all_processes = []

def run_command(command):
    print(command)
    p = Popen(shlex.split(command), stdout = PIPE, stderr = PIPE)
    all_processes.append(p)
    out, err = p.communicate()
    print(out,err)
    return out, err

def parse_arguments():
    parser = argparse.ArgumentParser()

    parser.add_argument("--analysis", default="cpprod", help="cpprod or cpdecays")
    parser.add_argument("--channel", default="tt")
    parser.add_argument("--embedding", action='store_true', default=False)
    parser.add_argument("--ff", action='store_true', default=False)
    parser.add_argument("--era", default="2016")
    parser.add_argument("--extra_pad", default=0)
    parser.add_argument("--signal_scale", default=1)
    parser.add_argument("--log_y", action='store_true', default=False)
    parser.add_argument("--cat", default="inclusive")

    return parser.parse_args()

def main(args):
        
    plot_vars = []
    extras = ""
    if args.channel == "tt":

        plot_vars = [

            # taus/leptons
            # "m_vis(25,20,250)",
            "m_sv(25,50,300)",
            "pt_tt(30,0,300)",
            "pt_1(20,40,140)",
            "pt_2(12,40,100)",
            "eta_1(12,-2.3,2.3)",
            "eta_2(12,-2.3,2.3)",
            "tau_decay_mode_2(11,0,11)",
            "tau_decay_mode_1(11,0,11)",
            "mva_dm_2(11,0,11)",
            "mva_dm_1(11,0,11)",
            "met(20,0,200)",

            # jets
            "sjdphi(12,-3.2,3.2)",
            "n_jets(5,0,5)",
            "mjj(15,0,1500)",
            "jdeta(25,0,5)",
            "jeta_1(12,-4.7,4.7)",
            "jeta_2(12,-4.7,4.7)",
            "jpt_1(17,30,200)",
            "jpt_2(17,40,200)",

            # bjets
            "n_bjets(3,0,3)",
            "bpt_1(17,30,200)",
            "bpt_2(17,30,200)",
            "beta_1(12,-4.7,4.7)",
            "beta_2(12,-4.7,4.7)",

            ]
        method = "8" if args.ff == False else "17"

        extras += " --cat {} ".format(args.cat)
        extras += " --outputfolder output/{} ".format(args.analysis)
        # extras += " --split_sm_scheme  --ggh_scheme tauspinner "

    elif args.channel in ["mt","et"]:
        # plot_vars = [
        plot_vars = [

            # taus/leptons
            "m_vis(20,20,200)",
            "m_sv(25,50,300)",
            "pt_tt(30,0,300)",
            "pt_1(20,20,120)",
            "pt_2(14,30,100)",
            "eta_1(12,-2.3,2.3)",
            "eta_2(12,-2.3,2.3)",
            "tau_decay_mode_2(11,0,11)",
            "mva_dm_2(11,0,11)",
            "met(20,0,200)",

            # jets
            "sjdphi(12,-3.2,3.2)",
            "n_jets(5,0,5)",
            "mjj(15,0,1500)",
            "jdeta(25,0,5)",
            "jeta_1(12,-4.7,4.7)",
            "jeta_2(12,-4.7,4.7)",
            "jpt_1(17,30,200)",
            "jpt_2(17,40,200)",

            # bjets
            "n_bjets(3,0,3)",
            "bpt_1(17,30,200)",
            "bpt_2(17,30,200)",
            "beta_1(12,-4.7,4.7)",
            "beta_2(12,-4.7,4.7)",

            ]
        method = "12" if args.ff == False else "17"
        extras += ' --set_alias "sel:mt_1<50" '
        extras += " --cat {} ".format(args.cat)
        extras += " --outputfolder output/{} ".format(args.analysis)
        # extras += " --split_sm_scheme "

    elif args.channel == "em":
        plot_vars = [

            # taus/leptons
            "m_vis(20,20,200)",
            "m_sv(25,50,300)",
            "pt_tt(30,0,300)",
            "pt_1(18,10,100)",
            "pt_2(18,10,100)",
            "eta_1(12,-2.3,2.3)",
            "eta_2(12,-2.3,2.3)",
            "met(20,0,200)",

            # jets
            "sjdphi(12,-3.2,3.2)",
            "n_jets(5,0,5)",
            "mjj(15,0,1500)",
            "jdeta(25,0,5)",
            "jeta_1(12,-4.7,4.7)",
            "jeta_2(12,-4.7,4.7)",
            "jpt_1(17,30,200)",
            "jpt_2(17,40,200)",

            # bjets
            "n_bjets(3,0,3)",
            "bpt_1(17,30,200)",
            "bpt_2(17,30,200)",
            "beta_1(12,-4.7,4.7)",
            "beta_2(12,-4.7,4.7)",

            ]
        method = "19"
        # extras += ' --set_alias "sel:pzeta<-50" ' # to select ttbar region
        extras += ' --set_alias "sel:pzeta>-35" '
        extras += " --cat {} ".format(args.cat)
        extras += " --outputfolder output/{} ".format(args.analysis)

        # if args.cat in ["0jet","boosted","inclusive"]:
        #     extras += ' --set_alias "sel:pzeta>-35" ' 
        # else:
        #     extras += ' --set_alias "sel:pzeta>-10" ' # to select dijet region

        # extras += " --split_sm_scheme  "

    if args.analysis == "cpdecays":
        for num in range(7):
            plot_vars.append("aco_angle_{}(14,0,6.28319)".format(num))

    if args.embedding:
        extras += " --embedding "
    if args.extra_pad != 0:
        extras += " --extra_pad {} ".format(args.extra_pad)
    if args.signal_scale != 1:
        extras += " --signal_scale {} ".format(args.signal_scale)
    if args.log_y:
        extras += " --log_y "
    
    if args.era == "2018":
        config = " scripts/plot_{}_2018.cfg ".format(args.analysis)
    elif args.era == "2017":
        config = " scripts/plot_{}_2017.cfg ".format(args.analysis)
    elif args.era == "2016":
        config = " scripts/plot_{}_2016.cfg ".format(args.analysis)

    for var in plot_vars:
        custom_extras = ""
        pad_extra = ""
        print(var)
        if "sjdphi" in var or "eta" in var or "aco" in var and not "(" in var:
            pad_extra = " --extra_pad 0.55 "

        if var.split("(")[0] in ["jeta_1","jpt_1"]:
            custom_extras = ' --set_alias "inclusive:(n_jets>=1)" '
        elif var.split("(")[0] in ["jdeta","jpt_2","jeta_2","mjj","sjdphi"]:
            custom_extras = ' --set_alias "inclusive:(n_jets>=2)" '
        # elif var.split("(")[0] in ["pt_tt",]:
        #     custom_extras = ' --log_y '

        run_command(' python scripts/HiggsTauTauPlot.py '
                + ' --cfg {} --ratio '.format(config)
                + ' --var "{}" --channel {} '.format(var,args.channel)
                + ' --method {} --norm_bins {} {} {} '.format(method,extras,custom_extras,pad_extra))

if __name__ == "__main__":
    args = parse_arguments()
    try:
        main(args)
    except KeyboardInterrupt:
        for p in all_processes:
            p.kill()
