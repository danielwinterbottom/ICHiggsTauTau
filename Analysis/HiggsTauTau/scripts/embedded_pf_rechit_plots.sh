# FILE_MC="output/Paper_2013_Moriond/DYJetsToTauTauSoup_mt_2012.root"
# FILE_PF="output/Paper_2013_Moriond/Embedded_mt_2012.root"
# FILE_RH="output/Paper_2013_Moriond/EmbeddedRecHit_mt_2012.root"

FILE_MC="./DYJetsToTauTauSoup_mt_2012.root"
FILE_PF="./Embedded_mt_2012.root"
FILE_PFOLD="./EmbeddedOld_mt_2012.root"
FILE_RH="./RecHitEmbedded_mt_2012.root"

 	PLOT="m_vis"
  ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
 	--x_axis_title="Visible Mass [GeV]" --norm_mode=3  --big_label="#mu#tau_{h}" \
 	--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
 	--outname="embedding_comp_mt_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=5


 	PLOT="n_vtx"
  ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
 	--x_axis_title="Number of Vertices" --norm_mode=3  --big_label="#mu#tau_{h}" \
 	--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
 	--outname="embedding_comp_mt_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=1

 	# PLOT="m_sv"
  # ./bin/PlotCompare  \
  # -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  # -p "pf:PF Embedded:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  # -p "rh:RecHit Embedded:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
 	# --x_axis_title="Number of Vertices" --norm_mode=3  --big_label="#mu#tau_{h}" \
 	# --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
 	# --outname="embedding_comp_mt_$PLOT.pdf" --ratios="pf/mc/2:rh/mc/4" --rebin=5

	PLOT="mt_1"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os/:$PLOT:-1:0:4" \
	--x_axis_title="m_{T} [GeV]" --norm_mode=3  --big_label="#mu#tau_{h}" \
	--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
	--outname="embedding_comp_mt_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="pt_1"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="Muon p_{T} [GeV]" --norm_mode=3  --big_label="#mu#tau_{h}" \
	--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
	--outname="embedding_comp_mt_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="pt_2"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="Tau p_{T} [GeV]" --norm_mode=3  --big_label="#mu#tau_{h}" \
	--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
	--outname="embedding_comp_mt_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="eta_1"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="Muon #eta" --norm_mode=3  --big_label="#mu#tau_{h}" \
	--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
	--outname="embedding_comp_mt_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="eta_2"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="Tau #eta" --norm_mode=3  --big_label="#mu#tau_{h}" \
	--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
	--outname="embedding_comp_mt_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="met"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="MVA MET [GeV]" --norm_mode=3  --big_label="#mu#tau_{h}" \
	--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
	--outname="embedding_comp_mt_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="m_2"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="Tau Mass [GeV]" --norm_mode=3  --big_label="#mu#tau_{h}" \
	--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=100 \
	--outname="embedding_comp_mt_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="n_jets"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="Number of jets" --norm_mode=3  --big_label="#mu#tau_{h}" \
	--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=100 \
	--outname="embedding_comp_mt_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="jpt_1"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:1jet_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:1jet_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:1jet_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:1jet_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="Leading Jet p_{T} [GeV]" --norm_mode=3  --big_label="#mu#tau_{h}" \
	--custom_x_axis_range=true --x_axis_min=20 --x_axis_max=200 \
	--outname="embedding_comp_mt_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="jeta_1"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:1jet_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:1jet_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:1jet_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:1jet_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="Leading Jet #eta" --norm_mode=3  --big_label="#mu#tau_{h}" \
	--custom_x_axis_range=true --x_axis_min=-5 --x_axis_max=5 \
	--outname="embedding_comp_mt_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=1





	FILE_MC="./DYJetsToTauTauSoup_et_2012.root"
	FILE_PF="./Embedded_et_2012.root"
	FILE_PFOLD="./EmbeddedOld_et_2012.root"
	FILE_RH="./RecHitEmbedded_et_2012.root"

# FILE_MC="output/Paper_2013_Moriond/DYJetsToTauTauSoup_et_2012.root"
# FILE_PF="output/Paper_2013_Moriond/Embedded_et_2012.root"
# FILE_RH="output/Paper_2013_Moriond/EmbeddedRecHit_et_2012.root"

 	PLOT="m_vis"
  ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
 	--x_axis_title="Visible Mass [GeV]" --norm_mode=3  --big_label="e#tau_{h}" \
 	--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
 	--outname="embedding_comp_et_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=5


 	PLOT="n_vtx"
  ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
 	--x_axis_title="Number of Vertices" --norm_mode=3  --big_label="e#tau_{h}" \
 	--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
 	--outname="embedding_comp_et_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=1

 	# PLOT="m_sv"
  # ./bin/PlotCompare  \
  # -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  # -p "pf:PF Embedded:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  # -p "rh:RecHit Embedded:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
 	# --x_axis_title="Number of Vertices" --norm_mode=3  --big_label="e#tau_{h}" \
 	# --custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
 	# --outname="embedding_comp_et_$PLOT.pdf" --ratios="pf/mc/2:rh/mc/4" --rebin=5

	PLOT="mt_1"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os/:$PLOT:-1:0:4" \
	--x_axis_title="m_{T} [GeV]" --norm_mode=3  --big_label="e#tau_{h}" \
	--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
	--outname="embedding_comp_et_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="pt_1"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="Electron p_{T} [GeV]" --norm_mode=3  --big_label="e#tau_{h}" \
	--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
	--outname="embedding_comp_et_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="pt_2"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="Tau p_{T} [GeV]" --norm_mode=3  --big_label="e#tau_{h}" \
	--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
	--outname="embedding_comp_et_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="eta_1"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="Electron #eta" --norm_mode=3  --big_label="e#tau_{h}" \
	--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
	--outname="embedding_comp_et_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="eta_2"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="Tau #eta" --norm_mode=3  --big_label="e#tau_{h}" \
	--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 \
	--outname="embedding_comp_et_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="met"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="MVA MET [GeV]" --norm_mode=3  --big_label="e#tau_{h}" \
	--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
	--outname="embedding_comp_et_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="m_2"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="Tau Mass [GeV]" --norm_mode=3  --big_label="e#tau_{h}" \
	--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=100 \
	--outname="embedding_comp_et_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="n_jets"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:inclusive_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:inclusive_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:inclusive_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:inclusive_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="Number of jets" --norm_mode=3  --big_label="e#tau_{h}" \
	--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=100 \
	--outname="embedding_comp_et_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="jpt_1"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:1jet_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:1jet_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:1jet_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:1jet_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="Leading Jet p_{T} [GeV]" --norm_mode=3  --big_label="e#tau_{h}" \
	--custom_x_axis_range=true --x_axis_min=20 --x_axis_max=200 \
	--outname="embedding_comp_et_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=2

	PLOT="jeta_1"
 ./bin/PlotCompare  \
  -p "mc:MC:$FILE_MC:1jet_os_sel/:$PLOT:-1:1:1" \
  -p "pf:PF Prompt:$FILE_PF:1jet_os_sel/:$PLOT:-1:0:2" \
  -p "pfold:PF ReReco:$FILE_PFOLD:1jet_os_sel/:$PLOT:-1:0:30" \
  -p "rh:RecHit ReReco:$FILE_RH:1jet_os_sel/:$PLOT:-1:0:4" \
	--x_axis_title="Leading Jet #eta" --norm_mode=3  --big_label="e#tau_{h}" \
	--custom_x_axis_range=true --x_axis_min=-5 --x_axis_max=5 \
	--outname="embedding_comp_et_$PLOT.pdf" --ratios="pf/mc/2:pfold/mc/30:rh/mc/4" --rebin=1


# ./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_et_2012.root"  \
# 										--file2="output/Paper_2013_Moriond/RecHit_et_2012.root" \
# 		--plot_folder="inclusive_os_sel/" --plot="m_vis" --lumi1=-1 --lumi2=-1 --label1=Embedded --label2=MC \
# 		--x_axis_title="Visible Mass [GeV]" --norm_mode=3  --channel=et \
# 		--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150
#
# ./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_et_2012.root"  \
# 										--file2="output/Paper_2013_Moriond/RecHit_et_2012.root" \
# 		--plot_folder="inclusive_os_sel/" --plot="m_sv" --lumi1=-1 --lumi2=-1 --label1=Embedded --label2=MC \
# 		--x_axis_title="m_{#tau#tau} [GeV]" --norm_mode=3  --channel=et \
# 		--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150
#
# ./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_et_2012.root"  \
# 										--file2="output/Paper_2013_Moriond/RecHit_et_2012.root" \
# 		--plot_folder="inclusive_os/" --plot="mt_1" --lumi1=-1 --lumi2=-1 --label1=Embedded --label2=MC \
# 		--x_axis_title="m_{T} [GeV]" --norm_mode=3  --channel=et \
# 		--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2
#
# ./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_et_2012.root"  \
# 										--file2="output/Paper_2013_Moriond/RecHit_et_2012.root" \
# 		--plot_folder="inclusive_os_sel/" --plot="pt_1" --lumi1=-1 --lumi2=-1 --label1=Embedded --label2=MC \
# 		--x_axis_title="Electron p_{T} [GeV]" --norm_mode=3  --channel=et \
# 		--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2
#
# ./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_et_2012.root"  \
# 										--file2="output/Paper_2013_Moriond/RecHit_et_2012.root" \
# 		--plot_folder="inclusive_os_sel/" --plot="pt_2" --lumi1=-1 --lumi2=-1 --label1=Embedded --label2=MC \
# 		--x_axis_title="Tau p_{T} [GeV]" --norm_mode=3  --channel=et \
# 		--custom_x_axis_range=true --x_axis_min=10 --x_axis_max=80 --rebin=2
#
# ./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_et_2012.root"  \
# 										--file2="output/Paper_2013_Moriond/RecHit_et_2012.root" \
# 		--plot_folder="inclusive_os_sel/" --plot="eta_1" --lumi1=-1 --lumi2=-1 --label1=Embedded --label2=MC \
# 		--x_axis_title="Electron #eta" --norm_mode=3  --channel=et \
# 		--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2
#
# ./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_et_2012.root"  \
# 										--file2="output/Paper_2013_Moriond/RecHit_et_2012.root" \
# 		--plot_folder="inclusive_os_sel/" --plot="eta_2" --lumi1=-1 --lumi2=-1 --label1=Embedded --label2=MC \
# 		--x_axis_title="Tau #eta" --norm_mode=3  --channel=et \
# 		--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2
#
# ./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_et_2012.root"  \
# 										--file2="output/Paper_2013_Moriond/RecHit_et_2012.root" \
# 		--plot_folder="inclusive_os_sel/" --plot="met" --lumi1=-1 --lumi2=-1 --label1=Embedded --label2=MC \
# 		--x_axis_title="MVA MET [GeV]" --norm_mode=3  --channel=et \
# 		--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 --rebin=2
#
# ./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_et_2012.root"  \
# 										--file2="output/Paper_2013_Moriond/RecHit_et_2012.root" \
# 		--plot_folder="inclusive_os_sel/" --plot="n_jets" --lumi1=-1 --lumi2=-1 --label1=Embedded --label2=MC \
# 		--x_axis_title="Jet Multiplicity" --norm_mode=3  --channel=et \
# 		--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2
#
# ./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_et_2012.root"  \
# 										--file2="output/Paper_2013_Moriond/RecHit_et_2012.root" \
# 		--plot_folder="inclusive_os_sel/" --plot="jpt_1" --lumi1=-1 --lumi2=-1 --label1=Embedded --label2=MC \
# 		--x_axis_title="Leading Jet p_{T} [GeV]" --norm_mode=3  --channel=et \
# 		--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2
#
# ./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_et_2012.root"  \
# 										--file2="output/Paper_2013_Moriond/RecHit_et_2012.root" \
# 		--plot_folder="inclusive_os_sel/" --plot="jeta_1" --lumi1=-1 --lumi2=-1 --label1=Embedded --label2=MC \
# 		--x_axis_title="Leading Jet #eta" --norm_mode=3  --channel=et \
# 		--custom_x_axis_range=true --x_axis_min=-5 --x_axis_max=5 --rebin=2
#
#
#
#
#./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_em_2012.root"  \
#										--file2="output/Paper_2013_Moriond/RecHit_em_2012.root" \
#		--plot_folder="inclusive_os_sel/" --plot="m_vis" --lumi1=-1 --lumi2=7.3 --label1=Embedded --label2=MC \
#		--x_axis_title="Visible Mass [GeV]" --norm_mode=3  --channel=em \
#		--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150
#
#./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_em_2012.root"  \
#										--file2="output/Paper_2013_Moriond/RecHit_em_2012.root" \
#		--plot_folder="inclusive_os_sel/" --plot="m_sv" --lumi1=-1 --lumi2=7.3 --label1=Embedded --label2=MC \
#		--x_axis_title="m_{#tau#tau} [GeV]" --norm_mode=3  --channel=em \
#		--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150
#
#./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_em_2012.root"  \
#										--file2="output/Paper_2013_Moriond/RecHit_em_2012.root" \
#		--plot_folder="inclusive_os/" --plot="pzeta" --lumi1=-1 --lumi2=7.3 --label1=Embedded --label2=MC \
#		--x_axis_title="#slash{P}_{#zeta} - 0.85 #times P_{#zeta}^{vis}" --norm_mode=3  --channel=em \
#		--custom_x_axis_range=true --x_axis_min=-100 --x_axis_max=150 --rebin=1
#
#./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_em_2012.root"  \
#										--file2="output/Paper_2013_Moriond/RecHit_em_2012.root" \
#		--plot_folder="inclusive_os_sel/" --plot="pt_1" --lumi1=-1 --lumi2=7.3 --label1=Embedded --label2=MC \
#		--x_axis_title="Electron p_{T} [GeV]" --norm_mode=3  --channel=em \
#		--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2
#
#./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_em_2012.root"  \
#										--file2="output/Paper_2013_Moriond/RecHit_em_2012.root" \
#		--plot_folder="inclusive_os_sel/" --plot="pt_2" --lumi1=-1 --lumi2s=7.3 --label1=Embedded --label2=MC \
#		--x_axis_title="Muon p_{T} [GeV]" --norm_mode=3  --channel=em \
#		--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2
#
#./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_em_2012.root"  \
#										--file2="output/Paper_2013_Moriond/RecHit_em_2012.root" \
#		--plot_folder="inclusive_os_sel/" --plot="eta_1" --lumi1=-1 --lumi2=7.3 --label1=Embedded --label2=MC \
#		--x_axis_title="Electron #eta" --norm_mode=3  --channel=em \
#		--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2
#
#./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_em_2012.root"  \
#										--file2="output/Paper_2013_Moriond/RecHit_em_2012.root" \
#		--plot_folder="inclusive_os_sel/" --plot="eta_2" --lumi1=-1 --lumi2=7.3 --label1=Embedded --label2=MC \
#		--x_axis_title="Muon #eta" --norm_mode=3  --channel=em \
#		--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2
#
#./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_em_2012.root"  \
#										--file2="output/Paper_2013_Moriond/RecHit_em_2012.root" \
#		--plot_folder="inclusive_os_sel/" --plot="met" --lumi1=-1 --lumi2=7.3 --label1=Embedded --label2=MC \
#		--x_axis_title="MVA MET [GeV]" --norm_mode=3  --channel=em \
#		--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 --rebin=2
#
#./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_em_2012.root"  \
#										--file2="output/Paper_2013_Moriond/RecHit_em_2012.root" \
#		--plot_folder="inclusive_os_sel/" --plot="n_jets" --lumi1=-1 --lumi2=7.3 --label1=Embedded --label2=MC \
#		--x_axis_title="Jet Multiplicity" --norm_mode=3  --channel=em \
#		--custom_x_axis_range=false --x_axis_min=0 --x_axis_max=150 --rebin=2
#
#./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_em_2012.root"  \
#										--file2="output/Paper_2013_Moriond/RecHit_em_2012.root" \
#		--plot_folder="inclusive_os_sel/" --plot="jpt_1" --lumi1=-1 --lumi2=7.3 --label1=Embedded --label2=MC \
#		--x_axis_title="Leading Jet p_{T} [GeV]" --norm_mode=3  --channel=em \
#		--custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 --rebin=2
#
#./bin/DataCompare  	--file1="output/Paper_2013_Moriond/Embedded_em_2012.root"  \
#										--file2="output/Paper_2013_Moriond/RecHit_em_2012.root" \
#		--plot_folder="inclusive_os_sel/" --plot="jeta_1" --lumi1=-1 --lumi2=7.3 --label1=Embedded --label2=MC \
#		--x_axis_title="Leading Jet #eta" --norm_mode=3  --channel=em \
#		--custom_x_axis_range=true --x_axis_min=-5 --x_axis_max=5 --rebin=2
