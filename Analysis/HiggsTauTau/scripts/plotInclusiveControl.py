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
import multiprocessing as mp

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
    # parser.add_argument("--eta_lo", default=-5.)
    # parser.add_argument("--eta_hi", default=5.)
    parser.add_argument("--ncores", type=int, default=4)
    parser.add_argument("--jer_regions", action='store_true', default=False)


    return parser.parse_args()

def main(args):

    plot_vars = []
    extras = ""
    if args.channel == "tt":

        plot_vars = [

            # taus/leptons
            # "m_vis(25,20,250)",
            # # "m_sv(25,50,300)",
            # "svfit_mass(25,50,300)",
            "svfit_mass_err(10,0,100)",
            # "pt_tt(30,0,300)",
            # "pt_1(20,40,140)",
            # "pt_2(12,40,100)",
            # "eta_1(12,-2.3,2.3)",
            # "eta_2(12,-2.3,2.3)",
            # "tau_decay_mode_2(11,0,11)",
            # "tau_decay_mode_1(11,0,11)",
            # "mva_dm_2(11,0,11)",
            # "mva_dm_1(11,0,11)",
            # "met(20,0,200)",

            # # jets
            # "sjdphi(12,-3.2,3.2)",
            # "n_jets(5,0,5)",
            # "mjj(15,0,1500)",
            # "jdeta(25,0,5)",
            # "jeta_1(12,-4.7,4.7)",
            # "jeta_2(12,-4.7,4.7)",
            # "jpt_1(17,30,200)",
            # "jpt_2(17,40,200)",

            # "svfit_mass_test(25,50,300)",

            # bjets
            # "n_bjets(3,0,3)",
            # "n_btag(3,0,3)",
            # "n_loose_btag(3,0,3)",
            # "bpt_1(17,30,200)",
            # "bpt_2(17,30,200)",
            # "beta_1(12,-4.7,4.7)",
            # "beta_2(12,-4.7,4.7)",

            # MVA things
            # "IC_Nov13_tauspinner_v1_max_score,aco_angle_1[0.,0.7,0.8,0.9],(14,0,6.28319)",
            # "IC_Nov13_tauspinner_v1_max_score,aco_angle_5[0.,0.7,0.8,0.9],(14,0,6.28319)",

            ]
        method = "8" if args.ff == False else "17"

        extras += " --cat {} ".format(args.cat)
        extras += ' --ratio_range 0.01,1.99 '
        # extras += " --split_sm_scheme  --ggh_scheme tauspinner "
        extras += " --add_wt (wt_tau_trg_mvadm) "

    elif args.channel in ["mt","et"]:
        # plot_vars = [
        plot_vars = [

            # taus/leptons
            # "m_vis(20,20,200)",
            # "m_sv(25,50,300)",
            # "svfit_mass_test(25,50,300)",
            # "pt_tt(30,0,300)",
            # "pt_1(20,20,120)",
            # "pt_2(14,30,100)",
            # "eta_1(12,-2.3,2.3)",
            # "eta_2(12,-2.3,2.3)",
            # # "tau_decay_mode_2(11,0,11)",
            # # "mva_dm_2(11,0,11)",
            # "met(20,0,200)",

            # jets
            "sjdphi(12,-3.2,3.2)",
            "sjdphi_smear(12,-3.2,3.2)",
            # "n_jets(5,0,5)",
            # "mjj(15,0,1500)",
            # "jdeta(25,0,5)",
            # "jeta_1(12,-4.7,4.7)",
            # "jeta_2(12,-4.7,4.7)",
            # "jpt_1(17,30,200)",
            # "jpt_2(17,40,200)",
            # "mt_1(16,0,160)",

            # # bjets
            # "n_bjets(3,0,3)",
            # "bpt_1(17,30,200)",
            # "bpt_2(17,30,200)",
            # "beta_1(12,-4.7,4.7)",
            # "beta_2(12,-4.7,4.7)",

            ]
        method = "12" if args.ff == False else "17"
        extras += ' --set_alias "sel:(mt_1<50)" '
        # extras += ' --set_alias "inclusive:(n_bjets==0)" '
        extras += " --cat {} ".format(args.cat)
        # extras += " --split_sm_scheme "
        # extras += ' --set_alias "inclusive:(n_jets>=1 && fabs(jeta_1)>2.65 && fabs(jeta_1)<3.139)" '

    elif args.channel == "em":
        plot_vars = [

            # # taus/leptons
            # "m_vis(20,20,200)",
            # "m_sv(25,50,300)",
            # "pt_tt(30,0,300)",
            # "pt_1(18,10,100)",
            # "pt_2(18,10,100)",
            # "eta_1(12,-2.3,2.3)",
            # "eta_2(12,-2.3,2.3)",
            # "met(20,0,200)",

            # # jets
            # "sjdphi(12,-3.2,3.2)",
            # "n_jets(5,0,5)",
            # "mjj(15,0,1500)",
            # "jdeta(25,0,5)",
            # "jeta_1(12,-4.7,4.7)",
            # "jeta_2(12,-4.7,4.7)",
            # "jpt_1(17,30,200)",
            # "jpt_2(17,40,200)",

            # # bjets
            "n_btag(3,0,3)",
            # "n_loose_btag(3,0,3)",
            # "bpt_1(17,30,200)",
            # "bpt_2(17,30,200)",
            # "beta_1(12,-4.7,4.7)",
            # "beta_2(12,-4.7,4.7)",

            # "pzeta(20,-100,100)",
            # "svfit_mass_test(25,50,300)",

            ]
        method = "19"
        # extras += ' --set_alias "sel:pzeta<-50" ' # to select ttbar region
        extras += ' --set_alias "sel:(pzeta>-35)" '
        # extras += ' --set_alias "inclusive:(n_bjets==0)" '
        # extras += ' --set_alias "inclusive:(wt_btag>0)" '
        # extras += ' --set_alias "inclusive:(n_btag==0 && n_loose_btag==0)" '
        extras += " --cat {} ".format(args.cat)
        # extras += " --add_wt (1/wt_btag) "

        # if args.cat in ["0jet","boosted","inclusive"]:
        #     extras += ' --set_alias "sel:pzeta>-35" ' 
        # else:
        #     extras += ' --set_alias "sel:pzeta>-10" ' # to select dijet region

        # extras += " --split_sm_scheme  "
        
    elif args.channel in ["zmm","zee"]:
        plot_vars = [
            # "jeta_1,jpt_1[0.,2.65,3.139],[30,40,50,60,70,100]",
            # "jpt_1,jnemf_1[30,35,40,45,50,75],(20,0,0.95)",
            # "jpt_1,jlrm_1[30,40,50,75,100],(20,-1,1)",
            # "jeta_1,jpt_1[0.,3.,3.5],[30,40,50,60,70,100]",
            # "jeta_2,jpt_2[0.,3.,3.5],[30,40,50,60,70,100]",
            # "jmva_1(40,-1,1)",
            # "jmva_2(40,-1,1)",
            # "pt_tt(40,0,400)",
            # "m_vis(50,70,110)",
            # "pt_1(18,10,100)",
            # "pt_2(18,10,100)",
            # "jpt_1(14,30,100)",
            # "jlrm_1(20,-1,1)",
            # "n_jets(4,0,4)",
            # "n_lowpt_jets(7,0,7)",
            # "jeta_1(12,-4.7,4.7)",
            # "m_sv(20,0,200)",

            # "mjj[0,50,100,150,200,250,300,400,500,600,700,800,1000]",
            # "sjdphi(20,-3.2,3.2)",
            # "jdeta(25,0,5)",
            # "jpt_2(17,30,200)",
            # "jeta_2(12,-4.7,4.7)",

            # "mjj(16,0,1000)",
            # "met(40,0,200)",
            # "pt_vis(30,0,300)",

            # "((dphi_jtt<0.)*(dphi_jtt+{0})+(dphi_jtt>0)*(dphi_jtt-{0}))(41,-1., 1.)".format(np.pi),
            "shifted_dphi_jtt(81,-3.142,3.142)",
            # "shifted_dphi_jtt_smear(81,-3.142,3.142)",
            # "smear_jet_delta(21,-0.5,0.5)",
            # "dphi_jtt_smear(81,-3.142,3.142)",
            # "dphi_jtt(81,-3.142,3.142)",
            # "smear_jet_delta(41,-1.,1.)",
            # "((dphi_jtt<0.)*(dphi_jtt+2*{0})+(dphi_jtt>0)*(dphi_jtt)-{0})(41,-1., 1.)".format(np.pi),
            # "dphi_jtt_smear(101,-3.142,3.142)",
            # "((dphi_jtt_smear<0.)*(dphi_jtt_smear+2*{0})+(dphi_jtt_smear>0)*(dphi_jtt_smear)-{0})(41,-1., 1.)".format(np.pi),

            # "residual_pt(20,0,200)",
            # "residual_phi(20,-3.142,3.142)",
            # "dphi_residual_tt(81,-3.142,3.142)",
            # "gen_jpt_1(14,30,100)",
            # "gen_jphi_1(20,-3.142,3.142)",
            # "jdphi_gen_reco(50,-0.1,0.1)",
            # "jpt20_sum(14,30,100)",
            # "shifted_dphi_j20tt(81,-3.142,3.142)",
            # "shifted_dphi_j20tt_smear(81,-3.142,3.142)",

            ]
        # extras += ' --set_alias "inclusive:(m_vis>70 && m_vis<110 )" '
        extras += ('--set_alias "inclusive:('
            'm_vis>70 && m_vis<110 && pt_tt>50'
            ' && (jpt_1/pt_tt)>0.8 && (jpt_1/pt_tt)<1.2'
            # ' && n_jets==1'
            ' && n_lowpt_jets==1'
            # ' && fabs(jeta_1>{}) && fabs(jeta_1<{}) '.format(args.eta_lo, args.eta_hi)
            ')" ')
        # extras += " --datacard {}_dynlo ".format(args.cat)
        method = "8"


    ## Some extra stuff
    # if args.analysis == "cpdecays":
    #     for num in range(7):
    #         plot_vars.append("aco_angle_{}(14,0,6.28319)".format(num))

    # extras += " --outputfolder output/{}/smear_etafine_ptinc_v5/ ".format(args.analysis)
    extras += " --outputfolder output/{}/ ".format(args.analysis)

    if args.ff and args.embedding:
        extras += " --datacard {}_ff_embed ".format(args.cat)
    elif args.ff:
        extras += " --datacard {}_ff ".format(args.cat)
    elif args.embedding:
        extras += " --datacard {}_embed ".format(args.cat)
    # extras += " --split_sm_scheme --ggh_scheme madgraph "

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
        config = " scripts/plot_{}_leg2016.cfg ".format(args.analysis)

    commands = []

    if args.jer_regions:
        for var in plot_vars:
            custom_extras = ""
            alt_extras = ""
            pad_extra = ""
            pre_eta = "0.0"
            pre_pt = "30"
            eta_high = ["1.1", "2.1", "3.0", "5.0",]
            # eta_high = ["3.0", "5.0",]
            pt_high = ["60", "10000",]
            # for (pt, eta) in [(pt_high, eta_high) for pt_high in pts_high for eta_high in etas_high]:
            for pt in pt_high:
                for eta in eta_high:
                    alt_extras = ""
                    alt_extras += ('--set_alias "inclusive:('
                        'm_vis>70 && m_vis<110 && pt_tt>50'
                        ' && (jpt_1/pt_tt)>0.8 && (jpt_1/pt_tt)<1.2'
                        ' && n_jets==1'
                        ' && fabs(jeta_1>{}) && fabs(jeta_1<{}) '
                        ' && jpt_1>{} && jpt_1<{} '
                        ')" '.format(pre_eta, eta, pre_pt, pt))
                    alt_extras += " --datacard shifted_dphi_jtt_etalow{}p{}_ptlow{} ".format(str(pre_eta).split(".")[0], str(pre_eta).split(".")[1], pre_pt)

                    print(var)
                    print(pre_pt, pt, pre_eta, eta)
                    if "pzeta" in var or "sjdphi" in var or "eta" in var or "aco" in var and not "(" in var:
                        pad_extra = " --extra_pad 0.55 "

                    # if var.split("(")[0] in ["jeta_1","jpt_1"]:
                    #     if args.channel in ["mt","et","em"]:
                    #         custom_extras = ' --set_alias "inclusive:(n_jets>=1 && n_bjets==0)" '
                    #     else:
                    #         custom_extras = ' --set_alias "inclusive:(n_jets>=1)" '
                    # elif var.split("(")[0] in ["jdeta","jpt_2","jeta_2","mjj","sjdphi"]:
                        # if args.channel in ["mt","et","em"]:
                            # custom_extras = ' --set_alias "inclusive:(n_jets>=2 && n_bjets==0)" '
                        # else:
                        #     custom_extras = ' --set_alias "inclusive:(n_jets>=2)" '
                    if var.split("(")[0] in ["n_bjets", "n_btag", "n_loose_btag"]:
                        custom_extras = ' --set_alias "inclusive:(1)" '
                    elif var.split("(")[0] in ["bpt_1","beta_1"]:
                        custom_extras = ' --set_alias "inclusive:(n_bjets>=1)" '
                    elif var.split("(")[0] in ["bpt_2","beta_2"]:
                        custom_extras = ' --set_alias "inclusive:(n_bjets>=2)" '
                    elif var.split("(")[0] in ["mt_1"]:
                        custom_extras = ' --set_alias "sel:(1)" --set_alias "inclusive:(1)" '
                    
                    # elif var.split("(")[0] in ["pt_tt",]:
                    #     custom_extras = ' --log_y '

                    commands.append([
                        'python scripts/HiggsTauTauPlot.py '
                        + ' --cfg {} --ratio '.format(config)
                        + ' --var "{}" --channel {} '.format(var,args.channel)
                        + ' --method {}  {} {} {} '.format(method,extras,custom_extras,pad_extra)
                        + ' {} '.format(alt_extras)
                    ])

                    # now set the low boundaries to the last high edge
                    pre_eta = eta
                    if pre_eta == "5.0":
                        pre_eta = "0.0"
                pre_pt = pt
                if pre_pt == "10000":
                    pre_pt = "30";

    else:
        for var in plot_vars:
            custom_extras = ""
            pad_extra = ""
            print(var)
            if "pzeta" in var or "sjdphi" in var or "eta" in var or "aco" in var and not "(" in var:
                pad_extra = " --extra_pad 0.55 "

            if var.split("(")[0] in ["jeta_1","jpt_1"]:
                if args.channel in ["mt","et","em"]:
                    custom_extras = ' --set_alias "inclusive:(n_jets>=1 && n_bjets==0)" '
                else:
                    custom_extras = ' --set_alias "inclusive:(n_jets>=1)" '
            elif var.split("(")[0] in ["jdeta","jpt_2","jeta_2","mjj","sjdphi","sjdphi_smear"]:
                if args.channel in ["mt","et","em"]:
                    custom_extras = ' --set_alias "inclusive:(n_jets>=2 && n_bjets==0)" '
                else:
                    custom_extras = ' --set_alias "inclusive:(n_jets>=2)" '
            if var.split("(")[0] in ["n_bjets"]:
                custom_extras = ' --set_alias "inclusive:(1)" '
            elif var.split("(")[0] in ["bpt_1","beta_1"]:
                custom_extras = ' --set_alias "inclusive:(n_bjets>=1)" '
            elif var.split("(")[0] in ["bpt_2","beta_2"]:
                custom_extras = ' --set_alias "inclusive:(n_bjets>=2)" '
            elif var.split("(")[0] in ["mt_1"]:
                custom_extras = ' --set_alias "sel:(1)" --set_alias "inclusive:(1)" '
            
            # elif var.split("(")[0] in ["pt_tt",]:
            #     custom_extras = ' --log_y '

            commands.append([
                'python scripts/HiggsTauTauPlot.py '
                + ' --cfg {} --ratio '.format(config)
                + ' --var "{}" --channel {} '.format(var,args.channel)
                + ' --method {}  {} {} {} '.format(method,extras,custom_extras,pad_extra)
            ])

    pool = mp.Pool(processes=args.ncores)
    results = []
    try:
        for cmd in commands:
            results.append(pool.apply_async(run_command, cmd))
        [r.get() for r in results]
    except KeyboardInterrupt:
        pool.terminate()
    pool.close()


if __name__ == "__main__":
    args = parse_arguments()
    try:
        main(args)
    except KeyboardInterrupt:
        for p in all_processes:
            p.kill()
