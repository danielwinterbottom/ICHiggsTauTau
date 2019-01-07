import argparse
import shlex
from subprocess import Popen, PIPE
import datetime
import os
import sys
import shutil
from multiprocessing import Pool

def transfer(var, dir_prefix):
    today = datetime.date.today()
    ftoday = today.strftime("%b%d") # format to month and date
    
    pdir = "/home/hep/akd116/public_html/CP2017/2018"
    if not os.path.isdir("{}/{}".format(pdir,ftoday)):
        os.mkdir("{}/{}".format(pdir,ftoday))
        
    shutil.copy(var, "{}/{}/{}".format(pdir,ftoday,dir_prefix))

all_processes = []

def run_command(command):
    print command
    p = Popen(shlex.split(command), stdout = PIPE, stderr = PIPE)
    all_processes.append(p)
    out, err = p.communicate()
    print out, err
    return out, err

def multi_job(command,args):
    print command
    p = Pool(8)
    p.map(command, args)


def parse_arguments():
    parser = argparse.ArgumentParser()

    parser.add_argument("--channel", default="zmm")
    parser.add_argument("--embedding", action='store_true', default=False)
    parser.add_argument("--ff", action='store_true', default=False)
    parser.add_argument("--use_2016", action='store_true', default=False)
    parser.add_argument("--extra_pad", default=0)
    parser.add_argument("--signal_scale", default=1)
    parser.add_argument("--do_HB", action='store_true', default=False)
    parser.add_argument("--do_HE", action='store_true', default=False)
    parser.add_argument("--do_HF", action='store_true', default=False)
    parser.add_argument("--do_EEnoise", action='store_true', default=False)
    parser.add_argument("--add_jes", action='store_true', default=False)
    parser.add_argument("--add_jes_corr", action='store_true', default=False)
    parser.add_argument("--add_jes_uncorr", action='store_true', default=False)
    parser.add_argument("--log_y", action='store_true', default=False)
    parser.add_argument("--cat", default="inclusive")
    parser.add_argument("--add_wt", default="")
    parser.add_argument("--add_syst", action='store_true', default=False)

    return parser.parse_args()

def main(args):
        
    extras = ""
    if args.channel == "tt":
        plot_vars = [
                "(lead_p_1-genE_pi1)/genE_pi1[-2,-1.5,-1.0,-0.5,0,0.5,1.0,1.5,2]",
                # "m_vis(25,20,250)",
                # "jpt_1(16,0,400)",
                # "jpt_2(15,0,300)",
                # "mjj(15,0,1500)",
                # "n_jets(5,0,5)",
                # "jeta_1(12,-4.7,4.7)",
                # "jeta_2(12,-4.7,4.7)",

                # "m_sv(30,0,300)",
                # "n_jets(5,0,5)",
                # "genM(30,0,300)",
                
                # "pt_tt,m_sv[0,100,170,300],[50,70,80,90,100,110,120,130,150,200,250]",
                # "pt_tt(60,0,300)",
                
                # "m_vis[0,1000]",
                # "IC_highMjj_Oct05_max_score[0.0,0.4,0.5,0.6,0.7,0.8,0.9,1.0]",
                # "IC_binary_Oct11_score(20,0.,1.)",
                # "mjj(16,0,800)",
                # "(sjdphi-gen_sjdphi)/sjdphi(12,-3.2,3.2)",
                # "m_vis(25,20,250)","pt_1(20,40,140)","pt_2(12,40,100)",
                # "eta_1(12,-2.3,2.3)","eta_2(12,-2.3,2.3)",
                # "jpt_1(17,30,200)","jpt_2(17,40,200)",
                # "jeta_1(12,-4.7,4.7)","jeta_2(12,-4.7,4.7)",
                # "met(20,0,200)","n_jets(5,0,5)",
                ]
        method = "8" if args.ff == False else "17"

        extras += " --cat {} ".format(args.cat)
        # extras += " --cat {}_highMjj ".format(args.cat)
        # extras += " --split_sm_scheme --ggh_scheme madgraph "
        # extras += ' --set_alias "inclusive:(n_jets>=2 && mjj>300 && fabs(jeta_2)>2.65 && fabs(jeta_2)<3.139)" '
        # extras += ' --ratio_range 0,2 '
        # extras += ' --ratio_range 0.3,1.7 '

    elif args.channel in ["mt","et"]:
        # plot_vars = [
        #         "sjdphi(12,-3.2,3.2)","m_vis(20,20,200)","pt_1(20,20,120)","pt_2(14,30,100)",
        #         "eta_1(12,-2.2.3.3)","eta_2(12,-2.3,2.3)",
        #         "jpt_1(17,30,200)","jpt_2(17,40,200)",
        #         "jeta_1(12,-4.7,4.7)","jeta_2(12,-4.7,4.7)","mjj(40,0,800)",
        #         "met(20,0,200)","n_jets(5,0,5)"]
        plot_vars = [
                "m_sv(30,0,300)",
                # "jpt_1(16,0,400)",
                # "jpt_2(30,0,300)",
                # "mjj(15,0,1500)",
                # "m_vis(20,20,200)",
                # "n_jets(5,0,5)",
                # "m_sv(30,0,300)",

                # "pt_tt,m_sv[0,100,150,200,250,300],[50,80,90,100,110,120,130,140,150,160,300]",
                # "pt_tt(60,0,300)",

                # "m_vis[0,1000]",
                # "IC_highMjj_Oct05_max_score[0.0,0.4,0.5,0.6,0.7,0.8,0.9,1.0]",
                # "IC_binary_Oct11_score(20,0.,1.)",
                # "met(20,0,200)",
                # "jeta_1(12,-4.7,4.7)","jeta_2(12,-4.7,4.7)",
                # "mjj(16,0,800)",
                # "sjdphi(12,-3.2,3.2)",
                # "jpt_1(17,30,200)","jpt_2(17,40,200)",
                # "n_jets(5,0,5)"
                ]
        method = "12" if args.ff == False else "17"
        extras += ' --set_alias "sel:mt_1<50" '
        # extras += ' --set_alias "inclusive:n_bjets==0" '
        # extras += ' --ratio_range 0.3,1.7 '
        # extras += ' --set_alias "inclusive:(mjj>500 && n_jets>1 && n_bjets==0)" '
        # extras += ' --set_alias "inclusive:(n_jets>=2 && mjj>300 && fabs(jeta_2)>2.65 && fabs(jeta_2)<3.139)" '

        extras += " --cat {} ".format(args.cat)
        # extras += " --cat {}_highMjj ".format(args.cat)
        # extras += ' --ratio_range 0,2 '
        # extras += " --split_sm_scheme --ggh_scheme madgraph "

    elif args.channel == "em":
        # plot_vars = ["m_vis(20,0,200)","pt_1(18,10,100)","pt_2(18,10,100)",
        #         "eta_1(12,-2.5,2.5)","eta_2(12,-2.5,2.5)",
        #         "jpt_1(17,30,200)","jpt_2(17,40,200)",
        #         "jeta_1(12,-4.7,4.7)","jeta_2(12,-4.7,4.7)","mjj(40,0,800)",
        #         "met(20,0,200)","n_jets(5,0,5)"]
        plot_vars = [
                # "IC_highMjj_Oct05_max_score[0.0,0.4,0.5,0.6,0.7,0.8,0.9,1.0]",
                # "m_vis(40,0,400)",
                "pt_tt(30,0,300)",
                # "m_sv(30,0,300)",
                # "IC_binary_Oct11_score(20,0.,1.)",
                # "n_jets(5,0,5)",
                # "sjdphi(12,-3.2,3.2)",
                # "mjj[0,50,100,150,200,250,300,400,500,600,700]",
                # "jdeta(25,0,5)",
                # "mjj[0,50,100,150,200,250,300,400,500,600,700]",
                # "n_jets(5,0,5)",
                # "jdeta(25,0,5)",
                # "mjj(16,0,800)",
                # "m_vis(20,0,200)",
                # "pt_tt(30,0,300)",
                # "pt_vis(30,0,300)",
                # "pzeta(50,-300,200)",
                # #"pt_1(28,10,150)","pt_2(28,10,150)",
                # "met(40,0,400)",
                # "jpt_1(17,30,200)","jpt_2(17,30,200)",
                # "jeta_1(12,-4.7,4.7)","jeta_2(12,-4.7,4.7)",
                ]
        method = "19"
        extras += ' --set_alias "sel:pzeta<-50" ' # to select ttbar region
        # extras += ' --ratio_range 0.3,1.7 '

        # extras += " --cat {} ".format(args.cat)
        # if args.cat in ["0jet","boosted","inclusive"]:
        #     extras += ' --set_alias "sel:pzeta>-35" ' 
        # else:
        #     extras += ' --set_alias "sel:pzeta>-10" ' # to select dijet region

        # extras += " --cat {}_highMjj ".format(args.cat)
        # extras += " --split_sm_scheme --ggh_scheme madgraph "
        # extras += ' --set_alias "sel:pzeta>-10" '

    elif args.channel == "zmm":
        plot_vars = [
                # "jeta_1,jpt_1[0.,2.65,3.139],[30,40,50,60,70,100]",
                # "jmva_1(40,-1,1)",
                # "jmva_2(40,-1,1)",
                # "pt_1(30,0,30)",
                # "pt_2(30,0,30)",
                # "jpt_1(17,30,200)",
                # "n_jets(10,0,10)",
                # "jeta_1(12,-4.7,4.7)",

                "mjj[0,50,100,150,200,250,300,400,500,600,700,800,1000]",
                # "sjdphi(20,-3.2,3.2)",
                # "jdeta(25,0,5)",
                # "jpt_2(17,30,200)",
                # "jeta_2(12,-4.7,4.7)",

                # "mjj(16,0,800)",
                # "met(40,0,400)",
                # "pt_vis(30,0,300)",

                # "jarea_1(8,0.3,0.7)",
                # "jchm_1(20,0,20)",
                # "jnm_1(20,0,20)",
                # "jchemf_1(20,0,1)",
                # "jnemf_1(20,0,1)",
                # "jchhf_1(20,0,1)",
                # "jnhf_1(20,0,1)",

                ]
        # extras += ' --ratio_range 0,2.5 '
        extras += ' --ratio_range 0.3,1.7 '
        # extras += ' --set_alias "inclusive:(m_vis>70 && m_vis<110 && n_jets==2 && fabs(jeta_1)>2.65 && fabs(jeta_1)<3.139 && pt_1>30)" '
        # extras += ' --set_alias "inclusive:(m_vis>70 && m_vis<110 && n_jets==2 && fabs(dphi_jtt)<1.5 && (jpt_1/pt_vis)>0.5 && (jpt_1/pt_vis)<1.5)" '
        extras += ' --set_alias "inclusive:(n_jets>=2)" '
        # extras += " add_wt "
        method = "8"

        # plot_vars = ["m_vis(80,40,120)","pt_1(18,10,100)","pt_2(18,10,100)",
        #         "eta_1(12,-2.5,2.5)","eta_2(12,-2.5,2.5)",
        #         "jpt_1(17,30,200)","jpt_2(17,40,200)",
        #         "jeta_1(12,-4.7,4.7)","jeta_2(12,-4.7,4.7)","mjj(40,0,800)",
        #         "met(20,0,200)","n_jets(5,0,5)"]

    if args.embedding:
        extras += " --embedding "
    if args.extra_pad != 0:
        extras += " --extra_pad {} ".format(args.extra_pad)
    if args.signal_scale != 1:
        extras += " --signal_scale {} ".format(args.signal_scale)
    if args.log_y:
        extras += " --log_y "
    
    if args.add_wt != "":
        extras += " --add_wt {} ".format(args.add_wt)

    if args.add_syst:
        extras += ' --syst_mu_scale="CMS_scale_m_13TeV" '

    if args.add_jes:
        extras += ' --syst_scale_j="CMS_scale_j_13TeV" --custom_uncerts_up_name "total_bkg_CMS_scale_j_13TeVUp" '
        extras +=' --custom_uncerts_down_name "total_bkg_CMS_scale_j_13TeVDown" --do_custom_uncerts --add_stat_to_syst '
        # plot_vars = ["mjj(16,0,800)","jpt_1(17,30,200)","jpt_2(17,30,200)","jeta_1(12,-4.7,4.7)","jeta_2(12,-4.7,4.7)",
        #         "sjdphi(12,-3.2,3.2)","n_jets(5,0,5)"]
    if args.add_jes_corr:
        extras += ' --syst_scale_j_corr="CMS_scale_j_corr_13TeV" --custom_uncerts_up_name "total_bkg_CMS_scale_j_corr_13TeVUp" '
        extras +=' --custom_uncerts_down_name "total_bkg_CMS_scale_j_corr_13TeVDown" --do_custom_uncerts --add_stat_to_syst '
    if args.add_jes_uncorr:
        extras += ' --syst_scale_j_uncorr="CMS_scale_j_uncorr_13TeV" --custom_uncerts_up_name "total_bkg_CMS_scale_j_uncorr_13TeVUp" '
        extras +=' --custom_uncerts_down_name "total_bkg_CMS_scale_j_uncorr_13TeVDown" --do_custom_uncerts --add_stat_to_syst '

    if args.do_HB:
        extras += ' --set_alias "inclusive:(fabs(jeta_1)<1.4 && fabs(jeta_2)<1.4)" '
        plot_vars = ["mjj(16,0,800)","jpt_1(17,30,200)","jpt_2(17,30,200)",
                "sjdphi(12,-3.2,3.2)","n_jets(5,0,5)"]
    if args.do_HE:
        extras += ' --set_alias "inclusive:(fabs(jeta_1)>1.3 && fabs(jeta_1)<3.0 && fabs(jeta_2)>1.3 && fabs(jeta_2)<3.0)" '
        plot_vars = ["jpt_1(17,30,200)","jpt_2(17,30,200)",
                "sjdphi(12,-3.2,3.2)","mjj(16,0,800)","n_jets(5,0,5)"]
    if args.do_HF:
        extras += ' --set_alias "inclusive:(fabs(jeta_1)>3.0 && fabs(jeta_1)<5.0 && fabs(jeta_2)>3.0 && fabs(jeta_2)<5.0)" '
        plot_vars = ["jpt_1(20,30,200)","jpt_2(20,30,200)",
                "sjdphi(12,-3.2,3.2)","mjj(20,0,1500)","n_jets(5,0,5)"]
    if args.do_EEnoise:
        extras += ' --set_alias "inclusive:(n_jets==1 && fabs(jeta_1)>2.65 && fabs(jeta_1)<3.139)" '
        # plot_vars = [
        #         "jpt_1(17,30,200)",
        #         # "jpt_2(20,0,200)",
        #         # "mjj(15,0,1500)",
        #         # "n_jets(5,0,5)"
        #         ]

    for var in plot_vars:
        if args.use_2016:
            if "sjdphi" in var or "jeta" in var:
                extras += " --extra_pad 0.55 "
            run_command(' python scripts/HiggsTauTauPlot.py --cfg ' 
                    + ' scripts/new_plot_sm_2016_NewPlotting.cfg --ratio '
                    + ' --var "{}" --channel {} '.format(var,args.channel)
                    + ' --method {} --norm_bins {} '.format(method,extras))
        # elif args.channel in ["mt","tt"]:
        #     if "sjdphi" in var or "jeta" in var:
        #         extras += " --extra_pad 0.55 "
        #     run_command(' python scripts/HiggsTauTauPlot.py --cfg ' 
        #             + ' scripts/plot_sm_2017_copy.cfg --ratio '
        #             + ' --var "{}" --channel {} '.format(var,args.channel)
        #             + ' --method {} --norm_bins {} '.format(method,extras))

        else:
            # if "sjdphi" in var or "jeta" in var:
            #     extras_n = extras+" --extra_pad 0.55 "
            #     run_command(' python scripts/HiggsTauTauPlot.py --cfg ' 
            #             + ' scripts/plot_sm_2017.cfg --ratio '
            #             + ' --var "{}" --channel {} '.format(var,args.channel)
            #             + ' --method {} --norm_bins {} '.format(method,extras_n))
            # elif "n_jets" in var:
            #     extras_n = extras+" --log_y --extra_pad 0.45 "
            #     run_command(' python scripts/HiggsTauTauPlot.py --cfg ' 
            #             + ' scripts/plot_sm_2017.cfg --ratio '
            #             + ' --var "{}" --channel {} '.format(var,args.channel)
            #             + ' --method {} --norm_bins {} '.format(method,extras_n))
            # elif "jpt_1" in var:
            #     extras_n = extras+" --log_y  --extra_pad 0.45 "
            #     run_command(' python scripts/HiggsTauTauPlot.py --cfg ' 
            #             + ' scripts/plot_sm_2017.cfg --ratio '
            #             + ' --var "{}" --channel {} '.format(var,args.channel)
            #             + ' --method {} --norm_bins {} '.format(method,extras_n))
            # else:
            if "sjdphi" in var or "jeta" in var:
                extras += " --extra_pad 0.55 "
            run_command(' python scripts/HiggsTauTauPlot.py --cfg ' 
                    + ' scripts/plot_sm_2017.cfg --ratio '
                    + ' --var "{}" --channel {} '.format(var,args.channel)
                    + ' --method {} --norm_bins {} '.format(method,extras))

if __name__ == "__main__":
    args = parse_arguments()
    try:
        main(args)
    except KeyboardInterrupt:
        for p in all_processes:
            p.kill()
