import uproot
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.metrics import roc_curve

tree_d  = uproot.open("/vols/cms/akd116/Offline/output/SM/2019/Feb12_2016/DYJetsToLL-LO-ext1_tt_2016.root")["ntuple"]
tree_d2 = uproot.open("/vols/cms/akd116/Offline/output/SM/2019/Feb12_2016/DYJetsToLL-LO-ext2_tt_2016.root")["ntuple"]
# tree_w  = uproot.open("/vols/cms/akd116/Offline/output/SM/2019/Feb07_2016/WJetsToLNu-LO-ext_tt_2016.root")["ntuple"]

df_d = tree_d.pandas.df([
        "gen_match_1","dpfTauV0_iso_1","deepTauVsJets_iso_1","dpfTauV1_iso_1",
        "deepTauVsEle_iso_1","iso_1","wt"],
        namedecode="utf-8")
df_d2 = tree_d2.pandas.df([
        "gen_match_1","dpfTauV0_iso_1","deepTauVsJets_iso_1","dpfTauV1_iso_1",
        "deepTauVsEle_iso_1","iso_1","wt"],
        namedecode="utf-8")
df_d = pd.concat([df_d, df_d2])
# df_w = tree_w.pandas.df(["gen_match_1","dpfTauV0_iso_1","deepTauVsJets_iso_1","iso_1","wt"], namedecode="utf-8")

# df_d = df_d.replace(3,6)
df_d = df_d[df_d["gen_match_1"]>=5]
df_d = df_d.replace(5,1)
df_d = df_d.replace(6,0)

# df_w = df_w[df_w["gen_match_1"]>=5]
# df_w = df_w.replace(5,1)
# df_w = df_w.replace(6,0)

print(len(df_d[df_d["gen_match_1"]==1]))
print(len(df_d[df_d["gen_match_1"]==0]))

df_d = df_d[df_d["dpfTauV0_iso_1"] > -1]

bkgEffDPF, sigEffDPF, _   = roc_curve(df_d["gen_match_1"],
        df_d["dpfTauV0_iso_1"], sample_weight=df_d["wt"])
bkgEffDeep, sigEffDeep, _ = roc_curve(df_d["gen_match_1"], 
        df_d["deepTauVsJets_iso_1"], sample_weight=df_d["wt"])
bkgEffOld, sigEffOld, _   = roc_curve(df_d["gen_match_1"],
        df_d["iso_1"], sample_weight=df_d["wt"])
bkgEffDeepEle, sigEffDeepEle, _ = roc_curve(df_d["gen_match_1"],
        df_d["deepTauVsEle_iso_1"], sample_weight=df_d["wt"])

# fig, (axtop, axbot) = plt.subplots(
#         nrows=1, ncols=1, sharex=True, 
#         gridspec_kw={'height_ratios': [3, 1],
#                      'wspace': 0.1,
#                      'hspace': 0.1},
#         figsize = (4.8, 6),
#         )

fig, axtop = plt.subplots()

axtop.plot(bkgEffOld, sigEffOld)
axtop.plot(bkgEffDPF, sigEffDPF)
axtop.plot(bkgEffDeep, sigEffDeep)
# axtop.plot(bkgEffDeepEle, sigEffDeepEle)

axtop.legend(("MVA POG","DPFTau","DeepTauVsJets"))
# axtop.legend(("MVA POG","DPFTau","DeepTauVsJets","DeepTauVsEle"))
axtop.text(0.01, 1, r'$\mathbf{CMS}\ \mathit{Simulation}$',
        ha='left', va='bottom', transform=axtop.transAxes, fontsize=12)
axtop.text(0.99, 1, r'$35.9\ \mathrm{fb}^{-1}(13\ \mathrm{TeV})$',
        ha='right', va='bottom', transform=axtop.transAxes, fontsize=12)

axtop.set_ylabel(r"$\tau_{h}$ ID Efficiency")
axtop.set_xlabel("Mid-ID Prob.")
fig.savefig("TauIdRoc_tt_lead.pdf")

