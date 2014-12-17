
if (( "$#" != "5" ))
then
  echo "Usage: new_plot_control.sh [PROD] [YEAR] [ANALYSIS] [POSTFIT] [MASSCUTS]"
  exit
fi


PROD=$1
YEAR=$2
ANA=$3
POSTFIT=$4
MASSCUTS=$5
PARAMS=./scripts/"$PROD"_params_$YEAR.dat
MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350"
MSSMBINS2JET2TAG="0,20,40,60,80,100,120,140,160,180,200,250,300,350"
#MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1100,1200,1300,1400,1500"
#MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,1000,1500"


#############################################################
#### Add scale factors to backgrounds
#############################################################

ET_INC_SHIFT=""
MT_INC_SHIFT="" 

if [ "$POSTFIT" -gt "0" ]
then
  if [ "$YEAR" = "2012" ]
  then
    if [ "$MASSCUTS" = "MassCuts" ]
    then
      #### Fit with mass cuts. Currently using inclusive is 2jet0tag instead of 2jetinclusive
      echo "Applying 8TeV scale factors from fit with mass cuts..."
      ET_INC_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.01970,QCD:1.17660,W:0.99940,ZL:0.96756,ZJ:0.95566,VV:1.01669,TT:0.99316 --auto_error_band=0.06727"
      MT_INC_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:0.92984,QCD:1.12305,W:1.00935,ZL:1.03194,ZJ:1.04850,VV:1.00393,TT:0.98038 --auto_error_band=0.07119"
      ET_2JET1TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.01320,QCD:1.03198,W:1.17044,ZL:0.91733,ZJ:0.90368,VV:0.96910,TT:0.98011 --auto_error_band=0.07346"
      MT_2JET1TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:0.94978,QCD:0.89195,W:1.17516,ZL:0.92646,ZJ:1.07546,VV:0.96415,TT:1.00571 --auto_error_band=0.06622"
      ET_2JET2TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:0.99650,QCD:1.02488,W:0.79428,ZL:0.84512,ZJ:0.99514,VV:1.01535,TT:0.97939 --auto_error_band=0.08461"
      MT_2JET2TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.00320,QCD:0.99097,W:0.71665,ZL:1.50946,ZJ:1.63310,VV:1.00079,TT:1.01360 --auto_error_band=0.06697"
    else 
      #### Fit without mass cuts. Currently using inclusive is 2jet0tag instead of 2jetinclusive
      echo "Applying 8TeV scale factors from fit without mass cuts..."
      ET_INC_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.01366,QCD:0.97748,W:1.00108,ZL:0.95615,ZJ:0.98109,VV:1.01176,TT:0.98128 --auto_error_band=0.05771"
      MT_INC_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.01768,QCD:1.08211,W:1.00280,ZL:1.00015,ZJ:1.01735,VV:1.00626,TT:1.00226 --auto_error_band=0.05778"
      ET_2JET1TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.02383,QCD:1.01152,W:1.01323,ZL:0.94096,ZJ:0.98001,VV:1.01558,TT:0.96018 --auto_error_band=0.06507"
      MT_2JET1TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.01831,QCD:0.94604,W:1.08328,ZL:1.00462,ZJ:1.00413,VV:1.01385,TT:1.01881 --auto_error_band=0.05808"
      ET_2JET2TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.04447,QCD:1.04843,W:0.86100,ZL:0.88403,ZJ:1.00093,VV:1.01007,TT:0.95473 --auto_error_band=0.07434"
      MT_2JET2TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.04089,QCD:1.02251,W:0.61596,ZL:1.13150,ZJ:1.17439,VV:1.01572,TT:1.03848 --auto_error_band=0.05866"
      fi
    fi
  fi  

################################################################################
##### Inclusive selection plots
################################################################################


#### MET

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="met(20,0,100)" --x_axis_label="E_{T}^{miss} [GeV]" \
  --norm_bins=true --datacard="2jetinclusive"$MASSCUTS"" --extra_pad=1.1 \
  --background_scheme="et_default" $ET_INC_SHIFT 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="met(20,0,100)" --x_axis_label="E_{T}^{miss} [GeV]" \
  --norm_bins=true --datacard="2jetinclusive"$MASSCUTS"" --extra_pad=1.1 \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### pt_1

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="pt_1(25,0,100)" \
  --x_axis_label="Electron p_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" $ET_INC_SHIFT
  
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="pt_1(25,0,100)" \
  --x_axis_label="Muon p_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### pt_2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="pt_2(25,0,100)" \
  --x_axis_label="Tau p_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="pt_2(25,0,100)" \
  --x_axis_label="Tau p_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### pt_tt

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### pt_tt log

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" --log_y=true --draw_ratio=true \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" --log_y=true --draw_ratio=true \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### eta_1

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="eta_1(30,-3,3)" --extra_pad=1.9 \
  --x_axis_label="Electron #eta" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="eta_1(30,-3,3)" --extra_pad=1.9 \
  --x_axis_label="Muon #eta" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### eta_2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="eta_2(30,-3,3)" --extra_pad=2.0 \
  --x_axis_label="Tau #eta" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="eta_2(30,-3,3)" --extra_pad=1.8 \
  --x_axis_label="Tau #eta" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### m_2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="m_2(20,0,2)" --extra_pad=1.1 \
  --x_axis_label="Tau Mass [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="m_2(20,0,2)" --extra_pad=1.1\
  --x_axis_label="Tau Mass [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --cat="pt_2>30. && tau_decay_mode==1" --var="m_2(20,0,2)" --extra_pad=1.5 \
#  --x_axis_label="Tau Mass [GeV]" --datacard="1prong" \
#  --background_scheme="et_default" $ET_INC_SHIFT

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --cat="pt_2>30. && tau_decay_mode==1" --var="m_2(20,0,2)" --extra_pad=1.5\
#  --x_axis_label="Tau Mass [GeV]" --datacard="1prong" \
#  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --cat="pt_2>30. && tau_decay_mode==10" --var="m_2(20,0,2)" --extra_pad=1.5 \
#  --x_axis_label="Tau Mass [GeV]" --datacard="3prong" \
#  --background_scheme="et_default" $ET_INC_SHIFT

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --cat="pt_2>30. && tau_decay_mode==10" --var="m_2(20,0,2)" --extra_pad=1.5 \
#  --x_axis_label="Tau Mass [GeV]" --datacard="3prong" \
#  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### n_jets (log)

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="n_jets(10,-0.5,9.5)"  --x_axis_label="Number of Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" $ET_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="n_jets(10,-0.5,9.5)"  --x_axis_label="Number of Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

#### n_prebjets (log)

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="n_prebjets_SF(9,-0.5,8.5)"  --x_axis_label="Number of b-tagged Jets" \
  --draw_ratio=true --log_y=true --extra_pad=500 --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" $ET_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="n_prebjets_SF(9,-0.5,8.5)"  --x_axis_label="Number of b-tagged Jets" \
  --draw_ratio=true --log_y=true --extra_pad=500 --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

#### mt_1 / pzeta

MT_BINS="mt_1(40,0,160)"
if [ "$MASSCUTS" = "MassCuts" ]
then
  MT_BINS="mt_1(20,0,160)"
fi
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et  \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --set_alias="sel:1" --var=$MT_BINS \
  --x_axis_label="m_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" --extra_pad=1.1 $ET_BAND_ONLY \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --set_alias="sel:1" --var=$MT_BINS \
  --x_axis_label="m_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" --extra_pad=1.1 $MT_BAND_ONLY \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT 


#### n_vtx

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="n_vtx(30,0,30)" $ET_INC_SHIFT \
  --x_axis_label="Number of Vertices" --datacard="2jetinclusive"$MASSCUTS"" --extra_pad=1.8 \
  --background_scheme="et_default"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="n_vtx(30,0,30)" $MT_INC_SHIFT \
  --x_axis_label="Number of Vertices" --datacard="2jetinclusive"$MASSCUTS"" --extra_pad=1.8 \
  --background_scheme="mt_with_zmm"


#### tau_decay_mode

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="tau_decay_mode(3,-4,11)" $ET_INC_SHIFT \
  --x_axis_label="Tau Decay Mode" --datacard="2jetinclusive"$MASSCUTS"" --extra_pad=2.0 \
  --background_scheme="et_default"\
  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="tau_decay_mode(3,-4,11)" $MT_INC_SHIFT \
  --x_axis_label="Tau Decay Mode" --datacard="2jetinclusive"$MASSCUTS"" --extra_pad=2.0 \
  --background_scheme="mt_with_zmm"\
  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" 


###############################################################################
#### Jet selection plots
###############################################################################


#### prebjetpt_1

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="prebjetpt_1(20,0,200)" $ET_INC_SHIFT \
  --x_axis_label="Leading selected jet p_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" --extra_pad=1.3

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="prebjetpt_1(20,0,200)" \
  --x_axis_label="Leading selected jet p_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" $MT_INC_SHIFT \
  --background_scheme="mt_with_zmm" --extra_pad=1.3

#### prebjeteta_1
BETA_BINS="prebjeteta_1(10,-3,3)"
if [ "$YEAR" = "2012" ]
then
  BETA_BINS="prebjeteta_1(15,-3,3)"
fi
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var=$BETA_BINS --extra_pad=2.0 $ET_INC_SHIFT \
  --x_axis_label="Leading selected jet #eta" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var=$BETA_BINS --extra_pad=2.0 \
  --x_axis_label="Leading selected jet #eta" --datacard="2jetinclusive"$MASSCUTS"" $MT_INC_SHIFT \
  --background_scheme="mt_with_zmm"

#### prebjetpt_2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="prebjetpt_2(20,0,200)" $ET_INC_SHIFT \
  --x_axis_label="Subleading selected jet p_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" --extra_pad=1.3

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="prebjetpt_2(20,0,200)" \
  --x_axis_label="Subleading selected jet p_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" $MT_INC_SHIFT \
  --background_scheme="mt_with_zmm" --extra_pad=1.3

#### prebjeteta_2
BETA_BINS="prebjeteta_2(10,-3,3)"
if [ "$YEAR" = "2012" ]
then
  BETA_BINS="prebjeteta_2(15,-3,3)"
fi
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var=$BETA_BINS --extra_pad=2.0 $ET_INC_SHIFT \
  --x_axis_label="Subleading selected jet #eta" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default"

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var=$BETA_BINS --extra_pad=2.0 \
  --x_axis_label="Subleading selected jet #eta" --datacard="2jetinclusive"$MASSCUTS"" $MT_INC_SHIFT \
  --background_scheme="mt_with_zmm"

#### prebjetcsv_1

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="prebjetbcsv_1(25,0,1)" \
  --x_axis_label="Leading selected jet CSV" --datacard="2jetinclusive"$MASSCUTS""  $ET_INC_SHIFT \
  --background_scheme="et_default" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="prebjetbcsv_1(25,0,1)" \
  --x_axis_label="Leading selected jet CSV" --datacard="2jetinclusive"$MASSCUTS""  $MT_INC_SHIFT \
  --background_scheme="mt_with_zmm" --extra_pad=1.2

#### prebjetcsv_2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="prebjetbcsv_2(25,0,1)" \
  --x_axis_label="Subleading selected jet CSV" --datacard="2jetinclusive"$MASSCUTS""  $ET_INC_SHIFT \
  --background_scheme="et_default" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="prebjetbcsv_2(25,0,1)" \
  --x_axis_label="Subleading selected jet CSV" --datacard="2jetinclusive"$MASSCUTS""  $MT_INC_SHIFT \
  --background_scheme="mt_with_zmm" --extra_pad=1.2


#### prebjetjdeta

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="prebjet_deta(20,0,10)" $ET_INC_SHIFT \
  --x_axis_label="#Delta#eta_{jj}" \
  --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="prebjet_deta(20,0,10)" $MT_INC_SHIFT \
  --x_axis_label="#Delta#eta_{jj}" \
  --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" --extra_pad=1.2


