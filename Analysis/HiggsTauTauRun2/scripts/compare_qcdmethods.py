import ROOT as R
from UserCode.ICHiggsTauTau.plotting import CompareHists

file_w_qcdsub_name = "../output/datacard_m_vis_inclusive_tt_2018_ss.root"
file_wo_qcdsub_name = "../output_nosubqcd/datacard_m_vis_inclusive_nosubqcd_tt_2018_ss.root"

output_folder_name = "."
output_plotname = "QCD_ss_compare"


file_w_qcdsub= R.TFile.Open(file_w_qcdsub_name, "READ")
file_w_qcdsub_dir = file_w_qcdsub.Get("tt_inclusive")
(file_w_qcdsub_dir).cd()
# if(file_w_qcdsub.IsZombie()):
#   print("Error")
hist_w_qcdsub = file_w_qcdsub_dir.Get("QCD")

file_wo_qcdsub = R.TFile.Open(file_wo_qcdsub_name, "READ")
(file_wo_qcdsub_name, "READ")
file_wo_qcdsub_dir = file_wo_qcdsub.Get("tt_inclusive")
(file_wo_qcdsub_dir).cd()
hist_wo_qcdsub = file_wo_qcdsub_dir.Get("QCD")


output_file = R.TFile(output_folder_name+"/"+output_plotname+".root","RECREATE")
c1 = R.TCanvas()
CompareHists(hists = [hist_w_qcdsub,hist_wo_qcdsub],
             legend_titles = ["Background Subtracted (SS)","Data Only (SS)"],title="59.7 fb^{-1} (13 TeV)", extra_pad=0.2,ratio_range="0.6,1.4",
             x_title = "m_{#tau#tau} (GeV)", y_title = "Events/GeV",plot_name = output_plotname, output_file = output_file)