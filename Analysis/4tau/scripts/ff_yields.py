import ROOT

channel = "mmtt"
#channel = "emtt"

cat = "{}_z_control_nobtag".format(channel)
#cat = "{}_2l2t_sig_nobtag".format(channel)

print "output/mt_tot_ff_full_{}_all.root".format(cat)
f = ROOT.TFile("output/mt_tot_ff_full_{}_all.root".format(cat))

j3 = f.Get(cat+"/jetFakes3").Integral()
j4 = f.Get(cat+"/jetFakes4").Integral()
j34 = abs(f.Get(cat+"/jetFakes34").Integral())

j3up = f.Get(cat+"/jetFakes3_{}_all_non_closureUp".format(channel)).Integral()
j4up = f.Get(cat+"/jetFakes4_{}_all_non_closureUp".format(channel)).Integral()
j34up = abs(f.Get(cat+"/jetFakes34_{}_all_non_closureUp".format(channel)).Integral())

j3down = f.Get(cat+"/jetFakes3_{}_all_non_closureDown".format(channel)).Integral()
j4down = f.Get(cat+"/jetFakes4_{}_all_non_closureDown".format(channel)).Integral()
j34down = abs(f.Get(cat+"/jetFakes34_{}_all_non_closureDown".format(channel)).Integral())


d = 1
j3 = round(j3,d)
j4 = round(j4,d)
j34 = round(j34,d)
j3up = round(j3up,d)
j4up = round(j4up,d)
j34up = round(j34up,d)
j3down = round(j3down,d)
j4down = round(j4down,d)
j34down = round(j34down,d)

print "{}^{{+{}}}_{{-{}}} + {}^{{+{}}}_{{-{}}} - ({}^{{+{}}}_{{-{}}}) = {}^{{+{}}}_{{-{}}}".format(j3,j3up-j3,j3-j3down,j4,j4up-j4,j4-j4down,j34,j34up-j34,j34-j34down,j3+j4-j34,j3up+j4up-j34up - (j3+j4-j34), (j3+j4-j34) - (j3down+j4down-j34down))

print "{}^{{+{}}}_{{-{}}} - ({}^{{+{}}}_{{-{}}}) = {}^{{+{}}}_{{-{}}}".format(j3+j4,j3up+j4up-j3-j4,j3+j4-j3down-j4down,j34,j34up-j34,j34-j34down,j3+j4-j34,j3up+j4up-j34up - (j3+j4-j34), (j3+j4-j34) - (j3down+j4down-j34down))
