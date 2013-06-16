# #LABEL="Inclusive"
SM_BINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350"
MSSM_BINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,1000,1500"
WIDE_SM_BINS="0,20,40,60,80,100,120,140,160,180,200,250,300,350"
WIDE_MSSM_BINS="0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,1000,1500"
LABEL='CMS Preliminary, #sqrt{s} = 8 TeV, L = 19.4 fb^{-1}'
Y_AXIS_TITLE='dN/dm_{#tau#tau} [1/GeV]'

########## et SM
./bin/HiggsTauTauPlot4 \
	--cfg=scripts/new_plot_2012.cfg \
	--channel=et \
	--method=8 \
	--cat="inclusive" \
	--datacard="inclusive" \
	--var="m_vis[$SM_BINS]" --norm_bins=true \
  --background_scheme="et_default" \
  --signal_scheme="sm_default" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
  --blind=true --x_blind_min=100 --x_blind_max=160

./bin/HiggsTauTauPlot4 \
	--cfg=scripts/new_plot_2012.cfg \
	--channel=et \
	--method=5 \
	--cat="vbf" \
	--datacard="vbf" \
	--var="m_vis[$WIDE_SM_BINS]" --norm_bins=true \
  --background_scheme="et_default" \
  --signal_scheme="sm_default" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
  --blind=true --x_blind_min=100 --x_blind_max=160

./bin/HiggsTauTauPlot4 \
	--cfg=scripts/new_plot_2012.cfg \
	--channel=et \
	--method=0 \
	--cat="0jet_low" \
	--datacard="0jet_low" \
	--var="m_vis[$SM_BINS]" --norm_bins=true \
  --background_scheme="et_default" \
  --signal_scheme="sm_default" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
  --blind=true --x_blind_min=100 --x_blind_max=160

./bin/HiggsTauTauPlot4 \
	--cfg=scripts/new_plot_2012.cfg \
	--channel=et \
	--method=1 \
	--cat="0jet_high" \
	--datacard="0jet_high" \
	--var="m_vis[$SM_BINS]" --norm_bins=true \
  --background_scheme="et_default" \
  --signal_scheme="sm_default" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
  --blind=true --x_blind_min=100 --x_blind_max=160

./bin/HiggsTauTauPlot4 \
	--cfg=scripts/new_plot_2012.cfg \
	--channel=et \
	--method=2 \
	--cat="1jet_low" \
	--datacard="boost_low" \
	--var="m_vis[$SM_BINS]" --norm_bins=true \
  --background_scheme="et_default" \
  --signal_scheme="sm_default" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
  --blind=true --x_blind_min=100 --x_blind_max=160

./bin/HiggsTauTauPlot4 \
	--cfg=scripts/new_plot_2012.cfg \
	--channel=et \
	--method=3 \
	--cat="1jet_high" \
	--datacard="boost_high" \
	--var="m_vis[$SM_BINS]" --norm_bins=true \
  --background_scheme="et_default" \
  --signal_scheme="sm_default" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
  --blind=true --x_blind_min=100 --x_blind_max=160 \
  --set_alias="w_shape_sel:mt_1<20."

 hadd -f htt_et.inputs-sm-8TeV-mvis-new.root datacard_*.root
 rm datacard_*.root

########## mt SM
#./bin/HiggsTauTauPlot4 \
#	--cfg=scripts/new_plot_2012.cfg \
#	--channel=mt \
#	--method=8 \
#	--cat="inclusive" \
#	--datacard="inclusive" \
#	--var="m_vis[$SM_BINS]" --norm_bins=true \
#  --background_scheme="mt_with_zmm" \
#  --signal_scheme="sm_default" \
#  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
#  --blind=true --x_blind_min=100 --x_blind_max=160
#
#./bin/HiggsTauTauPlot4 \
#	--cfg=scripts/new_plot_2012.cfg \
#	--channel=mt \
#	--method=5 \
#	--cat="vbf" \
#	--datacard="vbf" \
#	--var="m_vis[$WIDE_SM_BINS]" --norm_bins=true \
#  --background_scheme="mt_with_zmm" \
#  --signal_scheme="sm_default" \
#  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
#  --blind=true --x_blind_min=100 --x_blind_max=160
#
#./bin/HiggsTauTauPlot4 \
#	--cfg=scripts/new_plot_2012.cfg \
#	--channel=mt \
#	--method=0 \
#	--cat="0jet_low" \
#	--datacard="0jet_low" \
#	--var="m_vis[$SM_BINS]" --norm_bins=true \
#  --background_scheme="mt_with_zmm" \
#  --signal_scheme="sm_default" \
#  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
#  --blind=true --x_blind_min=100 --x_blind_max=160
#
#./bin/HiggsTauTauPlot4 \
#	--cfg=scripts/new_plot_2012.cfg \
#	--channel=mt \
#	--method=1 \
#	--cat="0jet_high" \
#	--datacard="0jet_high" \
#	--var="m_vis[$SM_BINS]" --norm_bins=true \
#  --background_scheme="mt_with_zmm" \
#  --signal_scheme="sm_default" \
#  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
#  --blind=true --x_blind_min=100 --x_blind_max=160
#
#./bin/HiggsTauTauPlot4 \
#	--cfg=scripts/new_plot_2012.cfg \
#	--channel=mt \
#	--method=2 \
#	--cat="1jet_low" \
#	--datacard="boost_low" \
#	--var="m_vis[$SM_BINS]" --norm_bins=true \
#  --background_scheme="mt_with_zmm" \
#  --signal_scheme="sm_default" \
#  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
#  --blind=true --x_blind_min=100 --x_blind_max=160 \
#  --syst_qcd_shape="CMS_htt_QCDShape_mutau_boost_low_8TeV" \
#
#./bin/HiggsTauTauPlot4 \
#	--cfg=scripts/new_plot_2012.cfg \
#	--channel=mt \
#	--method=3 \
#	--cat="1jet_high" \
#	--datacard="boost_high" \
#	--var="m_vis[$SM_BINS]" --norm_bins=true \
#  --background_scheme="mt_with_zmm" \
#  --signal_scheme="sm_default" \
#  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
#  --blind=true --x_blind_min=100 --x_blind_max=160 \
#  --set_alias="w_shape_sel:mt_1<20."
#
# hadd -f htt_mt.inputs-sm-8TeV-mvis-new.root datacard_*.root
# rm datacard_*.root

########## em SM
#./bin/HiggsTauTauPlot4 \
#	--cfg=scripts/new_plot_2012.cfg \
#	--channel=em \
#	--method=8 \
#	--cat="inclusive" \
#	--datacard="inclusive" \
#	--var="m_vis[$SM_BINS]" --norm_bins=true \
#  --background_scheme="em_default" \
#  --signal_scheme="sm_default" \
#  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
#  --blind=true --x_blind_min=100 --x_blind_max=160
#
#./bin/HiggsTauTauPlot4 \
#	--cfg=scripts/new_plot_2012.cfg \
#	--channel=em \
#	--method=5 \
#	--cat="vbf" \
#	--datacard="vbf" \
#	--var="m_vis[$WIDE_SM_BINS]" --norm_bins=true \
#  --background_scheme="em_default" \
#  --signal_scheme="sm_default" \
#  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
#  --blind=true --x_blind_min=100 --x_blind_max=160
#
#./bin/HiggsTauTauPlot4 \
#	--cfg=scripts/new_plot_2012.cfg \
#	--channel=em \
#	--method=0 \
#	--cat="0jet_low" \
#	--datacard="0jet_low" \
#	--var="m_vis[$SM_BINS]" --norm_bins=true \
#  --background_scheme="em_default" \
#  --signal_scheme="sm_default" \
#  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
#  --blind=true --x_blind_min=100 --x_blind_max=160
#
#./bin/HiggsTauTauPlot4 \
#	--cfg=scripts/new_plot_2012.cfg \
#	--channel=em \
#	--method=1 \
#	--cat="0jet_high" \
#	--datacard="0jet_high" \
#	--var="m_vis[$SM_BINS]" --norm_bins=true \
#  --background_scheme="em_default" \
#  --signal_scheme="sm_default" \
#  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
#  --blind=true --x_blind_min=100 --x_blind_max=160
#
#./bin/HiggsTauTauPlot4 \
#	--cfg=scripts/new_plot_2012.cfg \
#	--channel=em \
#	--method=2 \
#	--cat="1jet_low" \
#	--datacard="boost_low" \
#	--var="m_vis[$SM_BINS]" --norm_bins=true \
#  --background_scheme="em_default" \
#  --signal_scheme="sm_default" \
#  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
#  --blind=true --x_blind_min=100 --x_blind_max=160
#
#./bin/HiggsTauTauPlot4 \
#	--cfg=scripts/new_plot_2012.cfg \
#	--channel=em \
#	--method=3 \
#	--cat="1jet_high" \
#	--datacard="boost_high" \
#	--var="m_vis[$SM_BINS]" --norm_bins=true \
#  --background_scheme="em_default" \
#  --signal_scheme="sm_default" \
#  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
#  --blind=true --x_blind_min=100 --x_blind_max=160
#
# hadd -f htt_em.inputs-sm-8TeV-mvis-new.root datacard_*.root
# rm datacard_*.root

# ./bin/HiggsTauTauPlot4 \
#	--cfg=scripts/new_plot_2012.cfg \
#	--channel=mt \
#	--method=8 \
#	--cat="inclusive" \
#	--datacard="inclusive" \
#	--var="m_vis[$MSSM_BINS]" --norm_bins=true \
#  --background_scheme="mt_with_zmm" \
#  --signal_scheme="mssm_default" \
#  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
#  --blind=true --x_blind_min=100 --x_blind_max=160 \
#  --mssm_masses="80,90,100,110,120,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000" \
#  --draw_signal_mass="160"
#
# ./bin/HiggsTauTauPlot4 \
#	--cfg=scripts/new_plot_2012.cfg \
#	--channel=mt \
#	--method=11 \
#	--cat="nobtag" \
#	--datacard="nobtag" \
#	--var="m_vis[$MSSM_BINS]" --norm_bins=true \
#  --background_scheme="mt_with_zmm" \
#  --signal_scheme="mssm_default" \
#  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
#  --blind=true --x_blind_min=100 --x_blind_max=160 \
#  --mssm_masses="80,90,100,110,120,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000" \
#  --draw_signal_mass="160"
# 
#  ./bin/HiggsTauTauPlot4 \
#	--cfg=scripts/new_plot_2012.cfg \
#	--channel=mt \
#	--method=12 \
#	--cat="btag" \
#	--datacard="btag" \
#	--var="m_vis[$WIDE_MSSM_BINS]" --norm_bins=true \
#  --background_scheme="mt_with_zmm" \
#  --signal_scheme="mssm_default" \
#  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" \
#  --blind=true --x_blind_min=100 --x_blind_max=160 \
#  --mssm_masses="80,90,100,110,120,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000" \
#  --draw_signal_mass="160" \
#  --sub_ztt_top_frac=0.015
# hadd -f htt_mt.inputs-mssm-8TeV-mvis-new.root datacard_*.root
# rm datacard_*.root

#./bin/PostFitPlot --channel=mt --selection="btag:9" --eras="8TeV" \
#  --cfg=scripts/new_plot_2012.cfg \
# 	--datacard_path=output/mssm-3hits-loose/mt/160/ \
# 	--root_file_path=output/mssm-3hits-loose/mt/common/ \
# 	--pulls_file=output/mssm-3hits-loose/mt/160/out/mlfit.txt \
# 	--draw_signal_mass=160 --postfit=false --mssm=true --log_y=true --draw_signal_tanb=20 \
# 	--blind=true --x_blind_min=100 --x_blind_max=1500 \
# 	--norm_bins=true --extra_pad=5  \
# 	--background_scheme="mt_with_zmm_zll" \
# 	--signal_scheme="mssm_default" \
# 	--x_axis_label="M_{#tau#tau} [GeV]" \
# 	--draw_error_band=true \
# 	--title_left="$LABEL" \
# 	--y_axis_label="$Y_AXIS_TITLE"
#
#
#
#	./bin/PostFitPlot2 --channel=mt --selection="btag:9" --eras="8TeV" \
#	 --datacard_path=output/mssm-3hits-loose/mt/160/ \
#	 --root_file_path=output/mssm-3hits-loose/mt/common/ \
#	 --pulls_file=output/mssm-3hits-loose/mt/160/out/mlfit.txt \
#	 --signal_mass=160 --postfit=false --mssm=true --log_y=true --tanb=20 \
#	 --title_left="CMS Preliminary, #sqrt{s} = 8 TeV, L = 19.4 fb^{-1}" \
#	 --blind=true --x_blind_min=100 --x_blind_max=1500
  # --syst_tau_scale="CMS_scale_t_mutau_8TeV" \
  # --add_sm_background="125" \
  # --syst_qcd_shape="CMS_htt_QCDShape_mutau_boost_low_8TeV" \
  # --sub_ztt_top_frac=0.015 \
  # --fix_empty_bins="QCD,ZL,ZJ,W" \
  # --fix_empty_hists="ggH.*,qqH.*,VH.*,bbH.*"

  # --syst_fakes_shape="CMS_htt_FakeShape_em_0jet_high_8TeV"
  

# ./bin/HiggsTauTauPlot4 \
# 	--cfg=scripts/new_plot_2012.cfg \
# 	--channel=mt \
# 	--method=8 \
# 	--cat="twojet" \
# 	--var="mjj(40,0,400)" --norm_bins=false \
# 	--datacard="muTau_inclusive" \
#   --plot_name="mjj_mt" \
#   --background_scheme="mt_with_zmm" \
#   --x_axis_label="M_{jj} [GeV]"

# ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_2012.cfg --channel=mt \
#   --method=8 --cat="inclusive" --var="met(25,0,100)" --x_axis_label="E_{T}^{miss} [GeV]" \
#   --extra_pad=1 --plot_name="met_mt_2012" --background_scheme="mt_with_zmm" \
#   --signal_scheme="sm_default" --log_y=false --custom_y_axis_min=false --y_axis_min=1.0
#   
