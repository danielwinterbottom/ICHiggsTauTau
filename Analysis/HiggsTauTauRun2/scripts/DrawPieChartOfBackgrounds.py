import ROOT
import matplotlib.pyplot as plt

#datacards = ["/vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/shapes/2018/et/htt_all.inputs-mssm-vs-sm-Run2018-mt_tot_puppi.root"]

#title = r'$e\tau_{h}$ no b-tag'
#categories = ["et_Nbtag0_MTLt40","et_NbtagG0_MT40To70"]

#title = r'$e\tau_{h}$ b-tag' 
#categories = ["et_NbtagGt1_MTLt40","et_NbtagGt1_MT40To70"]


#datacards = ["/vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/shapes/2018/mt/htt_all.inputs-mssm-vs-sm-Run2018-mt_tot_puppi.root"]

#title = r'$\mu\tau_{h}$ no b-tag'
#categories = ["mt_Nbtag0_MTLt40","et_NbtagG0_MT40To70"]

#title = r'$\mu\tau_{h}$ b-tag'
#categories = ["mt_NbtagGt1_MTLt40","et_NbtagGt1_MT40To70"]


#datacards = ["/vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/shapes/2018/tt/htt_all.inputs-mssm-vs-sm-Run2018-mt_tot_puppi.root"]

#title = r'$\tau_{h}\tau_{h}$ no b-tag'
#categories = ["tt_Nbtag0"]

#title = r'$\tau_{h}\tau_{h}$ b-tag'
#categories = ["tt_NbtagGt1"]


datacards = ["/vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/shapes/2018/em/htt_all.inputs-mssm-vs-sm-Run2018-mt_tot_puppi.root"]

#title = r'$e\mu$ no b-tag'
#categories = ["em_Nbtag0_DZetaGt30","em_Nbtag0_DZetam10To30","em_Nbtag0_DZetam35Tom10"]

title = r'$e\mu$ b-tag'
categories = ["em_NbtagGt1_DZetaGt30","em_NbtagGt1_DZetam10To30","em_NbtagGt1_DZetam35Tom10"]



backgrounds_and_mapping = {"jetFakes":"JetFakes", "EMB":"Embedding", "ZL":r"Z$\rightarrow$ll", "TTL":r"$t\bar{t}$", "VVL":"Diboson", "wFakes":"JetFakes", "QCD":"QCD", "W":"W"}

colour_mapping = {'JetFakes':'#ff9999','Embedding':'#66b3ff',r"Z$\rightarrow$ll":'#99ff99',r"$t\bar{t}$":'#ffcc99','Diboson':'#f58142','QCD':'#E036B7','W':'#14720B','Other':'#4D9190'}

# Setup dictionary

i = {}

for key, val in backgrounds_and_mapping.items(): i[val] = 0.


# Get Integrals

f = {}
tot = 0.
	
for dc in datacards:

  f[dc] = ROOT.TFile(dc)
  
  for k in f[dc].GetListOfKeys():
  
    cat = k.ReadObj()

    if cat.GetName() in categories:

      for hist in cat.GetListOfKeys():

        if hist.GetName() in backgrounds_and_mapping.keys():

          h =  hist.ReadObj()
          i[backgrounds_and_mapping[h.GetName()]] +=  h.Integral()
          tot += h.Integral()
        

# Put entries in list

labels = []
values = []
other = 0.

for key, val in i.items():

  if val/tot > 0.01:

    labels.append(key)
    values.append(val/tot)

  else:
    
    other += val/tot

if other != 0.:
  labels.append("Other")
  values.append(other)

list1, list2 = zip(*sorted(zip(values, labels)))


# Set colours

colors = []

for i in list2: colors.append(colour_mapping[i])

# Set colours

#plt.pie(values,labels=labels,autopct='%1.1f%%')
#plt.title(title,fontsize=20)
#plt.axis('equal')
#plt.show()
#plt.savefig("bkg_percentage.pdf")

fig1, ax1 = plt.subplots()
ax1.pie(list1, labels=list2, colors=colors, autopct='%1.1f%%',startangle=140)
plt.title(title,fontsize=20)
ax1.axis('equal')
#plt.show()
plt.savefig("bkg_percentage.pdf")
