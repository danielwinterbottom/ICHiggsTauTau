# python scripts/checkSVFits.py /vols/cms/akd116/Offline/output/SM/2019/fastMTT_2018/ svfit
import uproot
import pandas as pd
import sys
import glob
import os

file_names = []
channels = ["tt"]#,"et","mt","em"]
years = ["2017"]
# subdirs = ['','TSCALE_DOWN','TSCALE_UP','TSCALE0PI_UP','TSCALE0PI_DOWN','TSCALE1PI_UP','TSCALE1PI_DOWN','TSCALE3PRONG_UP','TSCALE3PRONG_DOWN','JES_UP','JES_DOWN','MET_SCALE_UP','MET_SCALE_DOWN','MET_RES_UP','MET_RES_DOWN', 'EFAKE0PI_UP', 'EFAKE0PI_DOWN', 'EFAKE1PI_UP', 'EFAKE1PI_DOWN','MUFAKE0PI_UP','MUFAKE0PI_DOWN','MUFAKE1PI_UP','MUFAKE1PI_DOWN','METUNCL_UP','METUNCL_DOWN','MUSCALE_UP','MUSCALE_DOWN','ESCALE_UP','ESCALE_DOWN']
# subdirs = ['','TSCALE0PI_UP','TSCALE0PI_DOWN','TSCALE1PI_UP','TSCALE1PI_DOWN','TSCALE3PRONG_UP','TSCALE3PRONG_DOWN','JES_UP','JES_DOWN','MET_SCALE_UP','MET_SCALE_DOWN','MET_RES_UP','MET_RES_DOWN', 'EFAKE0PI_UP', 'EFAKE0PI_DOWN', 'EFAKE1PI_UP', 'EFAKE1PI_DOWN','MUFAKE0PI_UP','MUFAKE0PI_DOWN','MUFAKE1PI_UP','MUFAKE1PI_DOWN','METUNCL_UP','METUNCL_DOWN']
# subdirs = ['JES_UP']
# subdirs = ['']
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

for dir_ in subdirs:
    filenames = glob.glob("{}/{}/svfit_*_output.root".format(sys.argv[1],dir_))
    # svfitfiles = [x.split("svfit_")[1][0:-20] for x in filenames]
    # filenames = glob.glob("{}/{}/*.root".format(sys.argv[1],dir_))
    svfitfiles = [x.split("svfit_")[1].split("_{}".format(channels[0]))[0] for x in filenames]
    if sys.argv[2] == "svfit":
        for file_name in svfitfiles:
            for year in years:
                channels = ["mt","et","tt","em"]
                if "TSCALE" in dir_: channels = ["mt","et","tt"]
                elif "MUSCALE" in dir_: channels = ["mt","em"]
                elif "MUFAKE" in dir_: channels = ["mt"]
                elif "ESCALE" in dir_: channels = ["et","em"]
                elif "EFAKE" in dir_: channels = ["et"]

                if "EmbeddingTauTau" in file_name: channels = ["tt"]
                elif "EmbeddingMuTau" in file_name: channels = ["mt"]
                elif "EmbeddingElTau" in file_name: channels = ["et"]
                elif "EmbeddingElMu" in file_name: channels = ["em"]
                elif "\bTau" in file_name: channels = ["tt"]
                elif "SingleMuon" in file_name: channels = ["mt"]
                elif "EGamma" in file_name or "SingleElectron" in file_name: channels = ["et"]
                elif "MuonEG" in file_name: channels = ["em"]
                for channel in channels:
                    file_names.append("{}/{}/svfit_{}_{}_{}_output.root".format(sys.argv[1],dir_,file_name,channel,year))
    elif sys.argv[2] == "ntuple":
        for file_name in filenames:
            for year in years:
                for channel in channels:
                    if "_{}_{}.root".format(channel, year) in file_name:
                        file_names.append("{}".format(file_name))

missing = []
for f in file_names:
    print(f)
    # if "Embed" not in f:
    if sys.argv[2] == "svfit":
        try:
            tree = uproot.open(f)["svfit"]
            df = tree.pandas.df("svfit_mass", namedecode="utf-8")
            # print(df[df["svfit_mass"] < 0])
            if not df[df["svfit_mass"] < 0].empty:
                print("Found {} out of {} events with svfit below 0 in {}!\n".format(df[df["svfit_mass"] < 0].shape[0], df.shape[0], f))
        except IOError:
            print("not found svfit tree\n")
            missing.append(f)

    elif sys.argv[2] == "ntuple":
        try:
            tree = uproot.open(f)["ntuple"]
            df = tree.pandas.df("svfit_mass", namedecode="utf-8")
            if not df[df["svfit_mass"] < 0].empty:
                print("Found {} out of {} events with svfit below 0 in {}!\n".format(df[df["svfit_mass"] < 0].shape[0], df.shape[0], f))
        except IOError:
            print("not found ntuple tree\n")
            missing.append(f)

print("Missing\n", missing)


