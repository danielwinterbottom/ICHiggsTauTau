PROD=Paper_DOnly
YEAR=2012
ANA=sm
PARAMS=./scripts/"$PROD"_params_$YEAR.dat
SMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350"
MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1100,1200,1300,1400,1500"

#############################################################
#### Add scale factors to backgrounds
#############################################################
MT_INC_SHIFT="" 
MT_1JET_SHIFT=""
MT_2JET_SHIFT=""
MT_BTAG_SHIFT=""

#Add string containing "--shift_backgrounds="ZTT: ,QCD: ,W: ,ZL: ,ZJ: ,VV: ,TT: " --auto_error_band= " from output of PrintShifts.cpp for post fit plots: 

#ET_INC_SHIFT=" --draw_error_band=true"
#ET_1JET_SHIFT=" --draw_error_band=true"
#ET_2JET_SHIFT=" --draw_error_band=true"
#ET_BTAG_SHIFT=" --draw_error_band=true"
#MT_INC_SHIFT=" --draw_error_band=true"
#MT_1JET_SHIFT=" --draw_error_band=true"
#MT_2JET_SHIFT=" --draw_error_band=true"
#MT_BTAG_SHIFT=" --draw_error_band=true"
#EM_INC_SHIFT=" --draw_error_band=true"
#EM_1JET_SHIFT=" --draw_error_band=true"
#EM_2JET_SHIFT=" --draw_error_band=true"
#EM_BTAG_SHIFT=" --draw_error_band=true"

###############################################################################
#### Inclusive selection plots
###############################################################################

EXTRA=" --set_alias=sel:mt_1<45. --set_alias=w_sdb:mt_1>45. --set_alias=w_vbf_sdb:mt_1>45."

#### SVFit Mass

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --var="m_sv"["$SMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="inclusive" \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --var="m_vis"["$SMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau}^{vis} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="inclusive"\
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"

#### MET

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="inclusive" --var="met(20,0,100)" --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 $MT_INC_SHIFT  \
  --datacard="inclusive" --extra_pad=1.1 \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"

#### pt_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="inclusive" --var="pt_1(60,0,30)" $MT_INC_SHIFT \
  --x_axis_label="Muon p_{T} [GeV]" --datacard="inclusive" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"
#### pt_2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="inclusive" --var="pt_2(30,0,150)" $MT_INC_SHIFT \
  --x_axis_label="Tau p_{T} [GeV]" --datacard="inclusive" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"

#### pt_tt

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="inclusive" --var="pt_tt(30,0,300)" $MT_INC_SHIFT \
  --x_axis_label="p_{T}^{#tau#tau}" --datacard="inclusive" \
  --background_scheme="et_default" --signal_scheme="$ANA""_default"
#### pt_tt log

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="inclusive" --var="pt_tt(30,0,300)" $MT_INC_SHIFT \
  --x_axis_label="p_{T}^{#tau#tau}" --datacard="inclusive" --log_y=true \
  --background_scheme="et_default" --signal_scheme="$ANA""_default"

#### eta_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="inclusive" --var="eta_1(30,-3,3)" --extra_pad=1.6 \
  --x_axis_label="Muon #eta" --datacard="inclusive" $MT_INC_SHIFT \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"

#### eta_2


./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="inclusive" --var="eta_2(30,-3,3)" --extra_pad=1.5 \
  --x_axis_label="Tau #eta" --datacard="inclusive" $MT_INC_SHIFT \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"

#### m_2
./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="inclusive" --var="m_2(20,0,2)" --extra_pad=1.1\
  --x_axis_label="Tau Mass [GeV]" --datacard="inclusive" $MT_INC_SHIFT \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"

#### n_jets (log)


./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="inclusive" --var="n_jets(10,0,9)"  --x_axis_label="Number of Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="inclusive" $MT_INC_SHIFT \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default" \
  --custom_y_axis_min=true --y_axis_min=0.99
#### n_bjets (log)


./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="inclusive" --var="n_bjets(5,0,4)"  --x_axis_label="Number of b-tagged Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="inclusive" $MT_INC_SHIFT \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default" \
  --custom_y_axis_min=true --y_axis_min=0.99

#### n_jets

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et $EXTRA \
#  --method=8 --cat="inclusive" --var="n_jets(10,0,9)"  --x_axis_label="Number of Jets" \
#  --draw_ratio=true --datacard="inclusive" \
#  --background_scheme="et_default" --signal_scheme="$ANA""_default" \

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
#  --method=8 --cat="inclusive" --var="n_jets(10,0,9)"  --x_axis_label="Number of Jets" \
#  --draw_ratio=true --datacard="inclusive" \
#  --background_scheme="et_default" --signal_scheme="$ANA""_default" \

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
#  --method=8 --cat="inclusive" --var="n_jets(10,0,9)"  --x_axis_label="Number of Jets" \
#  --draw_ratio=true --datacard="inclusive" \
# --background_scheme="et_default" --signal_scheme="$ANA""_default" \

#### n_bjets

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et $EXTRA \
#  --method=8 --cat="inclusive" --var="n_bjets(5,0,4)"  --x_axis_label="Number of b-tagged Jets" \
#  --draw_ratio=true --datacard="inclusive" \
#  --background_scheme="et_default" --signal_scheme="$ANA""_default" \

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
#  --method=8 --cat="inclusive" --var="n_bjets(5,0,4)"  --x_axis_label="Number of b-tagged Jets" \
#  --draw_ratio=true --datacard="inclusive" \
#  --background_scheme="mt_default" --signal_scheme="$ANA""_default" \

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
#  --method=8 --cat="inclusive" --var="n_bjets(5,0,4)"  --x_axis_label="Number of b-tagged Jets" \
#  --draw_ratio=true --datacard="inclusive" \
#  --background_scheme="em_default" --signal_scheme="$ANA""_default" \

#### mt_1 / pzeta

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS \
  --method=8 --cat="inclusive" --set_alias="sel:1" --var="mt_1(40,0,160)" $MT_INC_SHIFT \
  --x_axis_label="m_{T}" --datacard="inclusive" --extra_pad=1.5 \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=150 \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"
#### n_vtx

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="inclusive" --var="n_vtx(30,0,30)" $MT_INC_SHIFT \
  --x_axis_label="Number of Vertices" --datacard="inclusive" --extra_pad=1.5 \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"

#### tau_decay_mode

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et $EXTRA \
#  --method=8 --cat="inclusive" --var="tau_decay_mode(3,0,2)" \
#  --x_axis_label="Tau Decay Mode" --datacard="inclusive" --extra_pad=1.5 \
#  --background_scheme="et_default" --signal_scheme="$ANA""_default"\
#  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" 

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
#  --method=8 --cat="inclusive" --var="tau_decay_mode(3,0,2)" \
#  --x_axis_label="Tau Decay Mode" --datacard="inclusive" --extra_pad=1.5 \
#  --background_scheme="mt_default" --signal_scheme="$ANA""_default"\
#  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" 

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:em_gf_mva>-0.5" \
#  --method=8 --cat="inclusive" --var="tau_decay_mode(3,0,2)" \
#  --x_axis_label="Tau Decay Mode" --datacard="inclusive" --extra_pad=1.5 \
#  --background_scheme="em_default" --signal_scheme="$ANA""_default" \
#  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" 

###############################################################################
#### Jet selection plots
###############################################################################

#### jpt_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="1jet" --var="jpt_1(20,0,200)" $MT_1JET_SHIFT \
  --x_axis_label="Leading Jet p_{T} [GeV]" --datacard="1jet" \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"

#### jeta_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="1jet" --var="jeta_1(25,-5,5)" --extra_pad=1.5 $MT_1JET_SHIFT \
  --x_axis_label="Leading Jet #eta [GeV]" --datacard="1jet" \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"

#### jpt_2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="twojet" --var="jpt_2(20,0,200)" $MT_2JET_SHIFT \
  --x_axis_label="Subleading Jet p_{T} [GeV]" --datacard="twojet" \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"

#### jeta_2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="1jet" --var="jeta_2(25,-5,5)" --extra_pad=1.6 $MT_2JET_SHIFT \
  --x_axis_label="Leading Jet #eta [GeV]" --datacard="1jet" \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"

#### bpt_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="btag" --var="bpt_1(20,0,200)" \
  --x_axis_label="Leading b-tagged jet p_{T} [GeV]" --datacard="btag" $MT_BTAG_SHIFT \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"

#### beta_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="btag" --var="beta_1(15,-3,3)" --extra_pad=1.7 \
  --x_axis_label="Leading b-tagged jet #eta" --datacard="btag" $MT_BTAG_SHIFT \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="twojet" --var="mjj(20,0,1000)" $MT_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="twojet" \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mtmet --paramfile=$PARAMS $EXTRA \
  --method=8 --cat="twojet" --var="jdeta(20,0,10)" $MT_2JET_SHIFT \
  --x_axis_label="#Delta#eta_{jj}" \
  --datacard="twojet" \
  --background_scheme="mt_default" --signal_scheme="$ANA""_default"
