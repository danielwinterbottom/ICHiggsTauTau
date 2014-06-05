
if (( "$#" != "4" ))
then
  echo "Usage: new_plot_control.sh [PROD] [YEAR] [ANALYSIS] [POSTFIT]"
  exit
fi


PROD=$1
YEAR=$2
ANA=$3
POSTFIT=$4
PARAMS=./scripts/"$PROD"_params_$YEAR.dat
MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350"
#MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1100,1200,1300,1400,1500"
#MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,1000,1500"

################################################################################
##### Inclusive selection plots
################################################################################


#### MET

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="met(20,0,100)" --x_axis_label="E_{T}^{miss} [GeV]" \
  --norm_bins=true --datacard="2jetinclusive" --extra_pad=1.1 \
  --background_scheme="et_default" $ET_INC_SHIFT 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="met(20,0,100)" --x_axis_label="E_{T}^{miss} [GeV]" \
  --norm_bins=true --datacard="2jetinclusive" --extra_pad=1.1 \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="met(20,0,100)" --x_axis_label="E_{T}^{miss} [GeV]" \
  --custom_x_axis_range=true --x_axis_min=0 --x_axis_max=100 \
  --norm_bins=true --datacard="2jetinclusive" --extra_pad=1.1 \
  --background_scheme="em_default" $EM_INC_SHIFT

#### pt_1

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="pt_1(25,0,100)" \
  --x_axis_label="Electron p_{T} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="et_default" $ET_INC_SHIFT
  
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="pt_1(25,0,100)" \
  --x_axis_label="Muon p_{T} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="pt_1(50,0,100)" \
  --x_axis_label="Electron p_{T} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="em_default" $EM_INC_SHIFT

#### pt_2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="pt_2(50,0,100)" \
  --x_axis_label="p_{T}(#tau_{h}) [GeV]" --datacard="2jetinclusive" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="pt_2(50,0,100)" \
  --x_axis_label="p_{T}(#tau_{h}) [GeV]" --datacard="2jetinclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="pt_2(50,0,100)" \
  --x_axis_label="Muon p_{T} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="em_default" $EM_INC_SHIFT

#### pt_tt

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="em_default" $EM_INC_SHIFT

#### pt_tt log

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="2jetinclusive" --log_y=true --draw_ratio=true \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="2jetinclusive" --log_y=true --draw_ratio=true \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="2jetinclusive" --log_y=true --draw_ratio=true \
  --background_scheme="em_default" $EM_INC_SHIFT

#### eta_1

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="eta_1(30,-3,3)" --extra_pad=1.9 \
  --x_axis_label="Electron #eta" --datacard="2jetinclusive" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="eta_1(30,-3,3)" --extra_pad=1.9 \
  --x_axis_label="Muon #eta" --datacard="2jetinclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30"\
  --method=8 --cat="2jetinclusive" --var="eta_1(30,-3,3)" --extra_pad=1.9 \
  --x_axis_label="Electron #eta" --datacard="2jetinclusive" \
  --background_scheme="em_default" $EM_INC_SHIFT

#### eta_2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="eta_2(30,-3,3)" --extra_pad=2.0 \
  --x_axis_label="Tau #eta" --datacard="2jetinclusive" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="eta_2(30,-3,3)" --extra_pad=1.8 \
  --x_axis_label="Tau #eta" --datacard="2jetinclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30"\
  --method=8 --cat="2jetinclusive" --var="eta_2(30,-3,3)" --extra_pad=1.8 \
  --x_axis_label="Muon #eta" --datacard="2jetinclusive" \
  --background_scheme="em_default" $EM_INC_SHIFT

#### m_2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="m_2(20,0,2)" --extra_pad=1.1 \
  --x_axis_label="Tau Mass [GeV]" --datacard="2jetinclusive" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="m_2(20,0,2)" --extra_pad=1.1\
  --x_axis_label="Tau Mass [GeV]" --datacard="2jetinclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="pt_2>30. && tau_decay_mode==1" --var="m_2(20,0,2)" --extra_pad=1.5 \
  --x_axis_label="Tau Mass [GeV]" --datacard="1prong" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="pt_2>30. && tau_decay_mode==1" --var="m_2(20,0,2)" --extra_pad=1.5\
  --x_axis_label="Tau Mass [GeV]" --datacard="1prong" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="pt_2>30. && tau_decay_mode==10" --var="m_2(20,0,2)" --extra_pad=1.5 \
  --x_axis_label="Tau Mass [GeV]" --datacard="3prong" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="pt_2>30. && tau_decay_mode==10" --var="m_2(20,0,2)" --extra_pad=1.5 \
  --x_axis_label="Tau Mass [GeV]" --datacard="3prong" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### n_jets (log)

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="n_jets(10,-0.5,9.5)"  --x_axis_label="Number of Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="2jetinclusive" \
  --background_scheme="et_default" $ET_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="n_jets(10,-0.5,9.5)"  --x_axis_label="Number of Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="2jetinclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="n_jets(10,-0.5,9.5)"  --x_axis_label="Number of Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="2jetinclusive" \
  --background_scheme="em_default" $EM_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

#### n_bjets (log)

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="n_bjets(5,-0.5,4.5)"  --x_axis_label="Number of b-tagged Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="2jetinclusive" \
  --background_scheme="et_default" $ET_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="n_bjets(5,-0.5,4.5)"  --x_axis_label="Number of b-tagged Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="2jetinclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="n_bjets(5,-0.5,4.5)"  --x_axis_label="Number of b-tagged Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="2jetinclusive" \
  --background_scheme="em_default" $EM_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

#### n_prebjets (log)

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="n_prebjets(9,-0.5,8.5)"  --x_axis_label="Number of prebtag Jets" \
  --draw_ratio=true --log_y=true --extra_pad=500 --datacard="2jetinclusive" \
  --background_scheme="et_default" $ET_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="n_prebjets(9,-0.5,8.5)"  --x_axis_label="Number of prebtag Jets" \
  --draw_ratio=true --log_y=true --extra_pad=500 --datacard="2jetinclusive" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="n_prebjets(9,-0.5,8.5)"  --x_axis_label="Number of prebtag Jets" \
  --draw_ratio=true --log_y=true --extra_pad=500 --datacard="2jetinclusive" \
  --background_scheme="em_default" $EM_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

#### mt_1 / pzeta

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et  \
  --method=8 --cat="2jetinclusive" --set_alias="sel:1" --var="mt_1(40,0,160)" \
  --x_axis_label="m_{T} [GeV]" --datacard="2jetinclusive" --extra_pad=1.1 $ET_BAND_ONLY \
  --background_scheme="et_default"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt \
  --method=8 --cat="2jetinclusive" --set_alias="sel:1" --var="mt_1(40,0,160)" \
  --x_axis_label="m_{T} [GeV]" --datacard="2jetinclusive" --extra_pad=1.1 $MT_BAND_ONLY \
  --background_scheme="mt_with_zmm"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="2jetinclusive" --var="pzeta(48,-100,100)" --extra_pad=1.5 $EM_BAND_ONLY \
  --x_axis_label="D_{#zeta} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="em_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="2jetinclusive" --var="pzetavis(25,0,50)"  $EM_BAND_ONLY \
  --x_axis_label="P_{#zeta}^{vis} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="em_default" --extra_pad=1.2

 ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="2jetinclusive" --var="pzetamiss(25,-100,100)"  $EM_BAND_ONLY \
  --x_axis_label="#slash{P}_{#zeta} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="em_default" --extra_pad=2.0

 ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="2jetinclusive" --var="em_gf_mva(40,-1,1)" --extra_pad=1. $EM_BAND_ONLY \
  --x_axis_label="BDT output" --datacard="2jetinclusive" \
  --background_scheme="em_default"
 
 ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="2jetinclusive" --var="em_gf_mva(40,-1,1)" --extra_pad=100. $EM_BAND_ONLY \
  --x_axis_label="BDT output" --datacard="2jetinclusive" --log_y=true --draw_ratio=true \
  --background_scheme="em_default"
 
 ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="2jetinclusive" --var="em_gf_mva_1(40,-1,1)" --extra_pad=1. $EM_BAND_ONLY \
  --x_axis_label="BDT 1 output" --datacard="2jetinclusive" \
  --background_scheme="em_default"
 
 ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="2jetinclusive" --var="em_gf_mva_1(40,-1,1)" --extra_pad=100. $EM_BAND_ONLY \
  --x_axis_label="BDT 1 output" --datacard="2jetinclusive" --log_y=true --draw_ratio=true \
  --background_scheme="em_default"
 
 ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="2jetinclusive" --var="em_gf_mva_2(40,-1,1)" --extra_pad=1. $EM_BAND_ONLY \
  --x_axis_label="BDT 2 output" --datacard="2jetinclusive" \
  --background_scheme="em_default"
 
 ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="2jetinclusive" --var="em_gf_mva_2(40,-1,1)" --extra_pad=100. $EM_BAND_ONLY \
  --x_axis_label="BDT 2 output" --datacard="2jetinclusive" --log_y=true --draw_ratio=true \
  --background_scheme="em_default"
  
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="2jetinclusive" --var="mt_ll(40,0,160)" $EM_BAND_ONLY \
  --x_axis_label="m_{T}(ll) [GeV]" --datacard="2jetinclusive" \
  --background_scheme="em_default" --extra_pad=1.3

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="2jetinclusive" --var="emu_dphi(40,0,3.15)" --extra_pad=1.5 $EM_BAND_ONLY \
  --x_axis_label="#Delta#phi_{e,#mu}" --datacard="2jetinclusive" \
  --background_scheme="em_default"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:1" \
  --method=8 --cat="2jetinclusive" --var="emu_dxy_1(40,-0.02,0.02)" --extra_pad=100 --log_y=true --draw_ratio=true \
  --x_axis_label="Electron d_{0}^{vtx} [cm]" --datacard="2jetinclusive" $EM_BAND_ONLY \
  --background_scheme="em_default"

#### n_vtx

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="n_vtx(30,0,30)" $ET_INC_SHIFT \
  --x_axis_label="Number of Vertices" --datacard="2jetinclusive" --extra_pad=1.8 \
  --background_scheme="et_default"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="n_vtx(30,0,30)" $MT_INC_SHIFT \
  --x_axis_label="Number of Vertices" --datacard="2jetinclusive" --extra_pad=1.8 \
  --background_scheme="mt_with_zmm"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="n_vtx(30,0,30)" $EM_INC_SHIFT \
  --x_axis_label="Number of Vertices" --datacard="2jetinclusive" --extra_pad=1.8 \
  --background_scheme="em_default"

#### tau_decay_mode

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="tau_decay_mode(3,-4,11)" $ET_INC_SHIFT \
  --x_axis_label="Tau Decay Mode" --datacard="2jetinclusive" --extra_pad=2.0 \
  --background_scheme="et_default"\
  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="tau_decay_mode(3,-4,11)" $MT_INC_SHIFT \
  --x_axis_label="Tau Decay Mode" --datacard="2jetinclusive" --extra_pad=2.0 \
  --background_scheme="mt_with_zmm"\
  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" 


###############################################################################
#### Jet selection plots
###############################################################################

#### jpt_1

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="jpt_1(20,0,200)" $ET_1JET_SHIFT \
  --x_axis_label="Leading Jet p_{T} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="et_default" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="jpt_1(20,0,200)" $MT_1JET_SHIFT \
  --x_axis_label="Leading Jet p_{T} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="mt_with_zmm" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="jpt_1(20,0,200)" $EM_1JET_SHIFT \
  --x_axis_label="Leading Jet p_{T} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="em_default" --extra_pad=1.2

#### jeta_1

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="jeta_1(20,-5,5)" --extra_pad=2.0 $ET_1JET_SHIFT \
  --x_axis_label="Leading Jet #eta [GeV]" --datacard="2jetinclusive" \
  --background_scheme="et_default"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="jeta_1(20,-5,5)" --extra_pad=1.9 $MT_1JET_SHIFT \
  --x_axis_label="Leading Jet #eta [GeV]" --datacard="2jetinclusive" \
  --background_scheme="mt_with_zmm"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="jeta_1(20,-5,5)" --extra_pad=1.9 $EM_1JET_SHIFT \
  --x_axis_label="Leading Jet #eta [GeV]" --datacard="2jetinclusive" \
  --background_scheme="em_default"

#### jpt_2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="jpt_2(20,0,200)" $ET_2JET_SHIFT \
  --x_axis_label="Subleading Jet p_{T} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="et_default" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="jpt_2(20,0,200)" $MT_2JET_SHIFT \
  --x_axis_label="Subleading Jet p_{T} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="mt_with_zmm" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="jpt_2(20,0,200)" $EM_2JET_SHIFT \
  --x_axis_label="Subleading Jet p_{T} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="em_default" --extra_pad=1.2

#### jeta_2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="jeta_2(20,-5,5)" --extra_pad=1.9 $ET_2JET_SHIFT \
  --x_axis_label="Subleading Jet #eta [GeV]" --datacard="2jetinclusive" \
  --background_scheme="et_default"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="jeta_2(20,-5,5)" --extra_pad=1.9 $MT_2JET_SHIFT \
  --x_axis_label="Subleading Jet #eta [GeV]" --datacard="2jetinclusive" \
  --background_scheme="mt_with_zmm"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="jeta_2(20,-5,5)" --extra_pad=1.9 $EM_2JET_SHIFT \
  --x_axis_label="Subleading Jet #eta [GeV]" --datacard="2jetinclusive" \
  --background_scheme="em_default"

#### bpt_1

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetMoreThan1tag" --var="bpt_1(20,0,200)" $ET_BTAG_SHIFT \
  --x_axis_label="Leading b-tagged jet p_{T} [GeV]" --datacard="2jetMoreThan1tag" \
  --background_scheme="et_default" --extra_pad=1.3

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetMoreThan1tag" --var="bpt_1(20,0,200)" \
  --x_axis_label="Leading b-tagged jet p_{T} [GeV]" --datacard="2jetMoreThan1tag" $MT_BTAG_SHIFT \
  --background_scheme="mt_with_zmm" --extra_pad=1.3

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetMoreThan1tag" --var="bpt_1(20,0,200)" $EM_BTAG_SHIFT \
  --x_axis_label="Leading b-tagged jet p_{T} [GeV]" --datacard="2jetMoreThan1tag" \
  --background_scheme="em_default" --extra_pad=1.3

#### beta_1
BETA_BINS="beta_1(10,-3,3)"
if [ "$YEAR" = "2012" ]
then
  BETA_BINS="beta_1(15,-3,3)"
fi
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetMoreThan1tag" --var=$BETA_BINS --extra_pad=2.0 $ET_BTAG_SHIFT \
  --x_axis_label="Leading b-tagged jet #eta" --datacard="2jetMoreThan1tag" \
  --background_scheme="et_default"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetMoreThan1tag" --var=$BETA_BINS --extra_pad=2.0 \
  --x_axis_label="Leading b-tagged jet #eta" --datacard="2jetMoreThan1tag" $MT_BTAG_SHIFT \
  --background_scheme="mt_with_zmm"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetMoreThan1tag" --var=$BETA_BINS --extra_pad=2.0 \
  --x_axis_label="Leading b-tagged jet #eta" --datacard="2jetMoreThan1tag" $EM_BTAG_SHIFT \
  --background_scheme="em_default"

#### bpt_2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetMoreThan2tag" --var="bpt_2(20,0,200)" $ET_BTAG_SHIFT \
  --x_axis_label="Subleading b-tagged jet p_{T} [GeV]" --datacard="2jetMoreThan2tag" \
  --background_scheme="et_default" --extra_pad=1.3

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetMoreThan2tag" --var="bpt_2(20,0,200)" \
  --x_axis_label="Subleading b-tagged jet p_{T} [GeV]" --datacard="2jetMoreThan2tag" $MT_BTAG_SHIFT \
  --background_scheme="mt_with_zmm" --extra_pad=1.3

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetMoreThan2tag" --var="bpt_2(20,0,200)" $EM_BTAG_SHIFT \
  --x_axis_label="Subleading b-tagged jet p_{T} [GeV]" --datacard="2jetMoreThan2tag" \
  --background_scheme="em_default" --extra_pad=1.3

#### beta_2
BETA_BINS="beta_2(10,-3,3)"
if [ "$YEAR" = "2012" ]
then
  BETA_BINS="beta_2(15,-3,3)"
fi
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetMoreThan2tag" --var=$BETA_BINS --extra_pad=2.0 $ET_BTAG_SHIFT \
  --x_axis_label="Subleading b-tagged jet #eta" --datacard="2jetMoreThan2tag" \
  --background_scheme="et_default"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetMoreThan2tag" --var=$BETA_BINS --extra_pad=2.0 \
  --x_axis_label="Subleading b-tagged jet #eta" --datacard="2jetMoreThan2tag" $MT_BTAG_SHIFT \
  --background_scheme="mt_with_zmm"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetMoreThan2tag" --var=$BETA_BINS --extra_pad=2.0 \
  --x_axis_label="Subleading b-tagged jet #eta" --datacard="2jetMoreThan2tag" $EM_BTAG_SHIFT \
  --background_scheme="em_default"



#### prebjetpt_1

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="prebjetpt_1(20,0,200)" $ET_BTAG_SHIFT \
  --x_axis_label="Leading selected jet p_{T} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="et_default" --extra_pad=1.3

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="prebjetpt_1(20,0,200)" \
  --x_axis_label="Leading selected jet p_{T} [GeV]" --datacard="2jetinclusive" $MT_BTAG_SHIFT \
  --background_scheme="mt_with_zmm" --extra_pad=1.3

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="prebjetpt_1(20,0,200)" $EM_BTAG_SHIFT \
  --x_axis_label="Leading selected jet p_{T} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="em_default" --extra_pad=1.3

#### prebjeteta_1
BETA_BINS="prebjeteta_1(10,-3,3)"
if [ "$YEAR" = "2012" ]
then
  BETA_BINS="prebjeteta_1(15,-3,3)"
fi
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var=$BETA_BINS --extra_pad=2.0 $ET_BTAG_SHIFT \
  --x_axis_label="Leading selected jet #eta" --datacard="2jetinclusive" \
  --background_scheme="et_default"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var=$BETA_BINS --extra_pad=2.0 \
  --x_axis_label="Leading selected jet #eta" --datacard="2jetinclusive" $MT_BTAG_SHIFT \
  --background_scheme="mt_with_zmm"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var=$BETA_BINS --extra_pad=2.0 \
  --x_axis_label="Leading selected jet #eta" --datacard="2jetinclusive" $EM_BTAG_SHIFT \
  --background_scheme="em_default"

#### prebjetpt_2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="prebjetpt_2(20,0,200)" $ET_BTAG_SHIFT \
  --x_axis_label="Subleading selected jet p_{T} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="et_default" --extra_pad=1.3

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="prebjetpt_2(20,0,200)" \
  --x_axis_label="Subleading selected jet p_{T} [GeV]" --datacard="2jetinclusive" $MT_BTAG_SHIFT \
  --background_scheme="mt_with_zmm" --extra_pad=1.3

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="prebjetpt_2(20,0,200)" $EM_BTAG_SHIFT \
  --x_axis_label="Subleading selected jet p_{T} [GeV]" --datacard="2jetinclusive" \
  --background_scheme="em_default" --extra_pad=1.3

#### prebjeteta_2
BETA_BINS="prebjeteta_2(10,-3,3)"
if [ "$YEAR" = "2012" ]
then
  BETA_BINS="prebjeteta_2(15,-3,3)"
fi
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var=$BETA_BINS --extra_pad=2.0 $ET_BTAG_SHIFT \
  --x_axis_label="Subleading selected jet #eta" --datacard="2jetinclusive" \
  --background_scheme="et_default"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var=$BETA_BINS --extra_pad=2.0 \
  --x_axis_label="Subleading selected jet #eta" --datacard="2jetinclusive" $MT_BTAG_SHIFT \
  --background_scheme="mt_with_zmm"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var=$BETA_BINS --extra_pad=2.0 \
  --x_axis_label="Subleading selected jet #eta" --datacard="2jetinclusive" $EM_BTAG_SHIFT \
  --background_scheme="em_default"

#### prebjetcsv_1

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="prebjetbcsv_1(25,0,1)" \
  --x_axis_label="Leading selected jet CSV" --datacard="2jetinclusive"  $ET_BTAG_SHIFT \
  --background_scheme="et_default" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="prebjetbcsv_1(25,0,1)" \
  --x_axis_label="Leading selected jet CSV" --datacard="2jetinclusive"  $MT_BTAG_SHIFT \
  --background_scheme="mt_with_zmm" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="prebjetbcsv_1(25,0,1)" \
  --x_axis_label="Leading selected jet CSV" --datacard="2jetinclusive"  $EM_BTAG_SHIFT \
  --background_scheme="em_default" --extra_pad=1.2

#### prebjetcsv_2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="prebjetbcsv_2(25,0,1)" \
  --x_axis_label="Subleading selected jet CSV" --datacard="2jetinclusive"  $ET_BTAG_SHIFT \
  --background_scheme="et_default" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="prebjetbcsv_2(25,0,1)" \
  --x_axis_label="Subleading selected jet CSV" --datacard="2jetinclusive"  $MT_BTAG_SHIFT \
  --background_scheme="mt_with_zmm" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="prebjetbcsv_2(25,0,1)" \
  --x_axis_label="Subleading selected jet CSV" --datacard="2jetinclusive"  $EM_BTAG_SHIFT \
  --background_scheme="em_default" --extra_pad=1.2


#### prebjetjdeta

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="prebjet_deta(20,0,10)" $ET_2JET_SHIFT \
  --x_axis_label="#Delta#eta_{jj}" \
  --datacard="2jetinclusive" \
  --background_scheme="et_default" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive" --var="prebjet_deta(20,0,10)" $MT_2JET_SHIFT \
  --x_axis_label="#Delta#eta_{jj}" \
  --datacard="2jetinclusive" \
  --background_scheme="mt_with_zmm" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="2jetinclusive" --var="prebjet_deta(20,0,10)" $EM_2JET_SHIFT \
  --x_axis_label="#Delta#eta_{jj}" \
  --datacard="2jetinclusive" \
  --background_scheme="em_default" --extra_pad=1.2
