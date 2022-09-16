import ROOT
import matplotlib.pyplot as plt
from matplotlib.pyplot import figure
import matplotlib.patches as mpatches

plt.rcParams["figure.figsize"] = (8,4)
plt.rcParams['font.size'] = 5.0
fig1, ax1 = plt.subplots(2,5)
backgrounds_and_mapping = {"jetFakes":"JetFakes", "EMB":"Embedding", "ZL":r"Z$\rightarrow$ll", "TTL":r"$t\bar{t}$", "VVL":"Diboson", "wFakes":"JetFakes", "QCD":"QCD", "W":"W"}
colour_mapping = {'JetFakes':'#ff9999','Embedding':'#66b3ff',r"Z$\rightarrow$ll":'#99ff99',r"$t\bar{t}$":'#ffcc99','Diboson':'#f58142','QCD':'#E036B7','W':'#14720B','Other':'#4D9190'}

ind1 = 0
ind2 = 0
for cat_name in ["no b-tag", "b-tag"]:
  for ch in ["tt","et","mt","em"]:
    if cat_name == "no b-tag":
      if ch == "et":
        title = r'$e\tau_{h}$ no b-tag'
        categories = ["et_Nbtag0_MTLt40","et_Nbtag0_MT40To70"]
      elif ch == "mt":
        title = r'$\mu\tau_{h}$ no b-tag'
        categories = ["mt_Nbtag0_MTLt40","et_Nbtag0_MT40To70"]
      elif ch == "tt":
        title = r'$\tau_{h}\tau_{h}$ no b-tag'
        categories = ["tt_Nbtag0"]
      elif ch == "em":
        title = r'$e\mu$ no b-tag'
        categories = ["em_Nbtag0_DZetaGt30","em_Nbtag0_DZetam10To30","em_Nbtag0_DZetam35Tom10"]
    elif cat_name == "b-tag":
      if ch == "et":
        title = r'$e\tau_{h}$ b-tag'
        categories = ["et_NbtagGt1_MTLt40","et_NbtagGt1_MT40To70"]
      elif ch == "mt":
        title = r'$\mu\tau_{h}$ b-tag'
        categories = ["mt_NbtagGt1_MTLt40","et_NbtagGt1_MT40To70"]
      elif ch == "tt":
        title = r'$\tau_{h}\tau_{h}$ b-tag'
        categories = ["tt_NbtagGt1"]
      elif ch == "em":
        title = r'$e\mu$ b-tag'
        categories = ["em_NbtagGt1_DZetaGt30","em_NbtagGt1_DZetam10To30","em_NbtagGt1_DZetam35Tom10"]


    datacards = ["/vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/shapes/2018/%(ch)s/htt_all.inputs-mssm-vs-sm-Run2018-mt_tot_puppi.root" % vars()]

    # Setup dictionary
    
    i = {}
    
    for key, val in backgrounds_and_mapping.items(): i[val] = 0.
    
    
    # Get Integrals
    
    f = {}
    tot = 0.
    	
    for dc in datacards:

      f[dc+"_"+ch] = ROOT.TFile(dc)
      
      for k in f[dc+"_"+ch].GetListOfKeys():
      
        cat = k.ReadObj()
        if cat.GetName() in categories:
    
          for hist in cat.GetListOfKeys():
   
            if hist.GetName() in backgrounds_and_mapping.keys():
              h =  hist.ReadObj()
              i[backgrounds_and_mapping[hist.GetName()]] +=  h.Integral()
              tot += h.Integral()
            
    
    # Put entries in list
    
    labels = []
    values = []
    other = 0.
    
    for key, val in i.items():
      if val/tot > 0.05:
    
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
  
    names = [str(round(i*100,1))+"%" for i in list1] 
    #ax1[ind1][ind2].pie(list1, colors=colors, autopct='%1.1f%%',startangle=140, textprops={'fontsize': 5})
    ax1[ind1][ind2].pie(list1, colors=colors)
    ax1[ind1][ind2].set_title(title,fontsize=10)
    ax1[ind1][ind2].axis('equal')
    #plt.show()
    ind2 += 1
  ind2 = 0
  ind1 += 1



# Draw legend
leg = []
for k,v in colour_mapping.items():
  if k != "Other" and k != "W":
    leg.append(mpatches.Patch(color=v, label=k))
leg.append(mpatches.Patch(color=colour_mapping["Other"], label="Remaining\ncontributions"))
ax1[0][4].legend(handles=leg,prop={'size': 10},loc=2,frameon=False)
ax1[0][4].axis('off')
ax1[1][4].axis('off')



plt.savefig("bkg_percentage.pdf")
