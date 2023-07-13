import ROOT as R
from UserCode.ICHiggsTauTau.plotting import CompareHists
import numpy as np

file_w_qcdsub_dirname = "../output/datacard_"
file_wo_qcdsub_dirname = "../output_nosubqcd/datacard_"

variables = [
  
  ["n_jets","N_{jets}"],
  ["pt_1","p_{T}^{#tau_{1}}"],
  ["pt_2", "p_{T}^{#tau_{2}}"],
  ["mt_1", "m_{T}(p_{T}^{#tau_{1}},p_{T}^{miss})"],
  ["mt_2","m_{T}(p_{T}^{#tau_{2}},p_{T}^{miss})"],
  ["m_vis","m_{#tau#tau}"],
  ["pt_tt", "p_{T}^{#tau#tau}"]

]

selnames = ["medium_vloose_medium",
            "medium_vvvloose_medium",
            "vloose_vvvloose_medium",
            "medium_vloose_medium_vloose",
            "medium_vvvloose_medium_vvvloose",
            "vloose_vvvloose_vloose_vvvloose"
            ]

types=["SS","OppS"]

output_folder_name = "."

chi2file = open('chi2test.txt','w')

selnames_array = np.expand_dims(np.array(selnames,dtype = object),axis=1)

for var in variables:
  chi2file.write("----------- %s ------------ \n"% var[0])
  ChiList = []
  ChiList_SS = []
  for sel in selnames:
    for chargetype in types:
#--------------------------------------------------------------------
      filename = var[0]+"_inclusive_"+sel+"_tt_2018"
      if chargetype == "SS":
        file_w_qcdsub= R.TFile.Open(file_w_qcdsub_dirname+filename+"_ss.root", "READ")
      else:
        file_w_qcdsub= R.TFile.Open(file_w_qcdsub_dirname+filename+".root", "READ")
      file_w_qcdsub_dir = file_w_qcdsub.Get("tt_inclusive")
      (file_w_qcdsub_dir).cd()
      hist_w_qcdsub = file_w_qcdsub_dir.Get("QCD")
#--------------------------------------------------------------------
      if chargetype == "SS":
        file_wo_qcdsub = R.TFile.Open(file_wo_qcdsub_dirname+filename+"_ss.root", "READ")
      else:
        file_wo_qcdsub = R.TFile.Open(file_wo_qcdsub_dirname+filename+".root", "READ")
      file_wo_qcdsub_dir = file_wo_qcdsub.Get("tt_inclusive")
      (file_wo_qcdsub_dir).cd()
      hist_wo_qcdsub = file_wo_qcdsub_dir.Get("QCD")
#--------------------------------------------------------------------
      sub_hist_w_qcdsub = file_w_qcdsub_dir.Get("data_obs")
      tot_bkg_hist_w = file_w_qcdsub_dir.Get("total_bkg")
      sub_hist_w_qcdsub.Add(tot_bkg_hist_w,-1)
      sub_hist_w_qcdsub.Add(hist_w_qcdsub)
#--------------------------------------------------------------------
      p = hist_wo_qcdsub.Chi2Test(hist_w_qcdsub,"WW")
      chi2 = hist_wo_qcdsub.Chi2Test(hist_w_qcdsub,"WW CHI2")
      chi2NDF = hist_wo_qcdsub.Chi2Test(hist_w_qcdsub,"WW CHI2/NDF")
      p_obs = hist_w_qcdsub.Chi2Test(sub_hist_w_qcdsub,"WW")
      chi2_obs = hist_w_qcdsub.Chi2Test(sub_hist_w_qcdsub,"WW CHI2")
      chi2NDF_obs = hist_w_qcdsub.Chi2Test(sub_hist_w_qcdsub,"WW CHI2/NDF")      
      
      if chargetype == "SS":
        ChiList_SS.append([p,chi2,chi2NDF,p_obs,chi2_obs,chi2NDF_obs])
      else:
        ChiList.append([p,chi2,chi2NDF,p_obs,chi2_obs,chi2NDF_obs])
#--------------------------------------------------------------------      
      output_file = R.TFile(output_folder_name+"/"+var[0]+"_"+sel+"_"+chargetype+".root","RECREATE")
      if var[0] == "n_jets":
        CompareHists(hists = [hist_w_qcdsub,hist_wo_qcdsub,sub_hist_w_qcdsub],
              legend_titles = ["#splitline{Background}{Subtracted} (%s)"%chargetype,"Data Only ("+chargetype+")","OBS - Non-QCD ("+chargetype+")"],
              title="59.7 fb^{-1} (13 TeV)", extra_pad=0.2,ratio_range="0.6,1.4",
              x_title = var[1], y_title = "Events",plot_name = var[0]+"_"+sel+"_"+chargetype, output_file = output_file)
      else:
        CompareHists(hists = [hist_w_qcdsub,hist_wo_qcdsub,sub_hist_w_qcdsub],
              legend_titles = ["#splitline{Background}{Subtracted} (%s)"%chargetype,"Data Only ("+chargetype+")","OBS - Non-QCD ("+chargetype+")"],
              title="59.7 fb^{-1} (13 TeV)", extra_pad=0.2,ratio_range="0.6,1.4",
              x_title = var[1]+" (GeV)", y_title = "Events/GeV",plot_name = var[0]+"_"+sel+"_"+chargetype, output_file = output_file)

#--------------------------------------------------------------------  
      output_file.Close()
      file_w_qcdsub.Close()
      file_wo_qcdsub.Close()
#--------------------------------------------------------------------      
  ChiList_SS = np.array(ChiList_SS,dtype = object)
  ChiList = np.array(ChiList,dtype = object)
  ChiList = np.concatenate((selnames_array,ChiList), axis=1)
  ChiList_SS = np.concatenate((selnames_array,ChiList_SS), axis=1)
  ChiList = ChiList[ChiList[:,3].argsort()]
  ChiList_SS = ChiList_SS[ChiList_SS[:,3].argsort()]
  chi2file.write("------------ OppS ------------ \n")
  order = 1
  for ChiVar in ChiList:
    chi2file.write("%i Type: %s\np: %.5f, chi2: %.5f, chi2/ndf: %.5f\n"%(order,ChiVar[0],ChiVar[1],
                                                            ChiVar[2],ChiVar[3]))
    order+=1
  chi2file.write("------------- SS ------------ \n")
  order = 1
  for ChiVar in ChiList_SS:
    chi2file.write("%i Type: %s\np: %.5f, chi2: %.5f, chi2/ndf: %.5f\n"%(order,ChiVar[0],ChiVar[1],
                                                            ChiVar[2],ChiVar[3]))    
    order+=1
    
  chi2file.write("------------ Compared to BKG------------ \n")
  ChiList = ChiList[ChiList[:,-1].argsort()]
  ChiList_SS = ChiList_SS[ChiList_SS[:,-1].argsort()]
  chi2file.write("------------ OppS ------------ \n")
  order = 1
  for ChiVar in ChiList:
    chi2file.write("%i Type: %s\np: %.5f, chi2: %.5f, chi2/ndf: %.5f\n"%(order,ChiVar[0],ChiVar[4],
                                                            ChiVar[5],ChiVar[6]))  
    order+=1
  chi2file.write("------------- SS ------------ \n")
  order = 1
  for ChiVar in ChiList_SS:
    chi2file.write("%i Type: %s\np: %.5f, chi2: %.5f, chi2/ndf: %.5f\n"%(order,ChiVar[0],ChiVar[4],
                                                            ChiVar[5],ChiVar[6]))    
    order+=1
