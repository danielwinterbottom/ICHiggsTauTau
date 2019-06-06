import uproot
import pandas as pd
import sys
import os

file_names = []
channels = ['et','mt','tt','em','zmm']
years = ['2016','2017']
for file_name in os.listdir(sys.argv[1]):
    if '.root' not in file_name:
        continue
    for year in years:
        for channel in channels:
            if "_{}_{}_output.root".format(channel, year) in file_name:
                # key = file_name.replace("_{}_{}_output.root".format(channel, year),"")
                # file_names[key] = file_name
                file_names.append(file_name)

for f in file_names:
    print(f)
    if "Embed" not in f:
        tree = uproot.open("{}/{}".format(sys.argv[1], f))["svfit"]
        df = tree.pandas.df("svfit_mass", namedecode="utf-8")
        # print(df[df["svfit_mass"] < 0])
        if not df[df["svfit_mass"] < 0].empty:
            print("Found {} out of {} events with svfit below 0 in {}!"
                    .format(df[df["svfit_mass"] < 0].shape[0], df.shape[0], f))

