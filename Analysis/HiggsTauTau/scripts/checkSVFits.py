# python scripts/checkSVFits.py /vols/cms/akd116/Offline/output/SM/2019/fastMTT_2018/ svfit
import uproot
import pandas as pd
import sys
import glob
import os

file_names = []
channels = ["mt","et","tt","em"]
years = ["2016","2017","2018"]
subdirs = ['','TSCALE_DOWN','TSCALE_UP','TSCALE0PI_UP','TSCALE0PI_DOWN','TSCALE1PI_UP','TSCALE1PI_DOWN','TSCALE3PRONG_UP','TSCALE3PRONG_DOWN','JES_UP','JES_DOWN','MET_SCALE_UP','MET_SCALE_DOWN','MET_RES_UP','MET_RES_DOWN', 'EFAKE0PI_UP', 'EFAKE0PI_DOWN', 'EFAKE1PI_UP', 'EFAKE1PI_DOWN','MUFAKE0PI_UP','MUFAKE0PI_DOWN','MUFAKE1PI_UP','MUFAKE1PI_DOWN','METUNCL_UP','METUNCL_DOWN','MUSCALE_UP','MUSCALE_DOWN','ESCALE_UP','ESCALE_DOWN']
# subdirs = ['TSCALE_DOWN']

for dir_ in subdirs:
    filenames = glob.glob("{}/{}/svfit_*_output.root".format(sys.argv[1],dir_))
    for file_name in filenames:
        if 'output.root' not in file_name:
            continue
        for year in years:
            for channel in channels:
                if sys.argv[2] == "svfit":
                    if "_{}_{}_output.root".format(channel, year) in file_name:
                        # key = file_name.replace("_{}_{}_output.root".format(channel, year),"")
                        # file_names[key] = file_name
                        file_names.append(file_name)
                elif sys.argv[2] == "ntuple":
                    if "_{}_{}.root".format(channel, year) in file_name:
                        file_names.append("{}".format(file_name))

missing = []
for f in file_names:
    print(f)
    # if "Embed" not in f:
    if sys.argv[2] == "svfit":
        try:
            tree = uproot.open("{}".format(f))["svfit"]
            df = tree.pandas.df("svfit_mass", namedecode="utf-8")
            # print(df[df["svfit_mass"] < 0])
            if not df[df["svfit_mass"] < 0].empty:
                print("Found {} out of {} events with svfit below 0 in {}!\n".format(df[df["svfit_mass"] < 0].shape[0], df.shape[0], f))
        except:
            print("not found svfit tree\n")
            missing.append(f)

    elif sys.argv[2] == "ntuple":
        try:
            tree = uproot.open("{}".format(f))["ntuple"]
            df = tree.pandas.df("m_sv", namedecode="utf-8")
            if not df[df["m_sv"] < 0].empty:
                print("Found {} out of {} events with svfit below 0 in {}!\n".format(df[df["m_sv"] < 0].shape[0], df.shape[0], f))
        except:
            print("not found ntuple tree\n")
            missing.append(f)

print(missing)


