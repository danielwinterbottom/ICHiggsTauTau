
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
      ET_INC_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:0.99199,QCD:1.19585,W:0.99341,ZL:1.04225,ZJ:1.02756,VV:0.99489,TT:0.92087 --auto_error_band=0.06920"
      MT_INC_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:0.95022,QCD:1.09791,W:0.99701,ZL:1.01870,ZJ:1.01983,VV:1.00438,TT:1.02265 --auto_error_band=0.06687"
      ET_2JET1TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:0.98507,QCD:0.99923,W:1.31871,ZL:1.00243,ZJ:0.99628,VV:0.97383,TT:0.92691 --auto_error_band=0.07302"
      MT_2JET1TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:0.99239,QCD:0.91871,W:1.07840,ZL:0.96021,ZJ:1.04372,VV:1.01142,TT:1.03975 --auto_error_band=0.06745"
      ET_2JET2TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:0.96640,QCD:1.04639,W:0.77391,ZL:1.14871,ZJ:1.18167,VV:0.89296,TT:0.92313 --auto_error_band=0.11130"
      MT_2JET2TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.07055,QCD:1.01165,W:0.62660,ZL:1.18118,ZJ:1.36275,VV:1.01456,TT:1.05884 --auto_error_band=0.09753"
    else 
      #### Fit without mass cuts. Currently using inclusive is 2jet0tag instead of 2jetinclusive
      echo "Applying 8TeV scale factors from fit without mass cuts..."
      ET_INC_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.00011,QCD:1.01007,W:1.00149,ZL:0.95309,ZJ:0.98716,VV:1.00010,TT:0.95635 --auto_error_band=0.05516"
      MT_INC_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.02294,QCD:1.07348,W:0.99610,ZL:1.00829,ZJ:1.00799,VV:1.01983,TT:1.01981 --auto_error_band=0.05172"
      ET_2JET1TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.00647,QCD:0.96742,W:1.11335,ZL:0.93699,ZJ:0.97598,VV:0.99457,TT:0.95218 --auto_error_band=0.06618"
      MT_2JET1TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.03653,QCD:0.95798,W:1.03105,ZL:1.00776,ZJ:1.00615,VV:1.04573,TT:1.04186  --auto_error_band=0.05957"
      ET_2JET2TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.02728,QCD:1.06637,W:0.86061,ZL:0.89258,ZJ:1.00026,VV:0.99326,TT:0.94251 --auto_error_band=0.09835"
      MT_2JET2TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.06519,QCD:1.01817,W:0.57583,ZL:1.05853,ZJ:1.18944,VV:1.03505,TT:1.06644 --auto_error_band=0.08198"
      fi
    fi
  fi  

################################################################################
##### Inclusive selection plots
################################################################################


#### MET

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="met(20,0,100)" --x_axis_label="E_{T}^{miss} [GeV]" \
  --norm_bins=true --datacard="2jetinclusive"$MASSCUTS"" --extra_pad=1.1 \
  --background_scheme="et_default" $ET_INC_SHIFT 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="met(20,0,100)" --x_axis_label="E_{T}^{miss} [GeV]" \
  --norm_bins=true --datacard="2jetinclusive"$MASSCUTS"" --extra_pad=1.1 \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### pt_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="pt_1(25,0,100)" \
  --x_axis_label="Electron p_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" $ET_INC_SHIFT
  
./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="pt_1(25,0,100)" \
  --x_axis_label="Muon p_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### pt_2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="pt_2(25,0,100)" \
  --x_axis_label="Tau p_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="pt_2(25,0,100)" \
  --x_axis_label="Tau p_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### pt_tt

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### pt_tt log

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" --log_y=true --draw_ratio=true \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="pt_tt(30,0,300)" \
  --x_axis_label="p_{T}^{#tau#tau} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" --log_y=true --draw_ratio=true \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### eta_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="eta_1(30,-3,3)" --extra_pad=1.9 \
  --x_axis_label="Electron #eta" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="eta_1(30,-3,3)" --extra_pad=1.9 \
  --x_axis_label="Muon #eta" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### eta_2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="eta_2(30,-3,3)" --extra_pad=2.0 \
  --x_axis_label="Tau #eta" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="eta_2(30,-3,3)" --extra_pad=1.8 \
  --x_axis_label="Tau #eta" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### m_2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="m_2(20,0,2)" --extra_pad=1.1 \
  --x_axis_label="Tau Mass [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="m_2(20,0,2)" --extra_pad=1.1\
  --x_axis_label="Tau Mass [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --cat="pt_2>30. && tau_decay_mode==1" --var="m_2(20,0,2)" --extra_pad=1.5 \
#  --x_axis_label="Tau Mass [GeV]" --datacard="1prong" \
#  --background_scheme="et_default" $ET_INC_SHIFT

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --cat="pt_2>30. && tau_decay_mode==1" --var="m_2(20,0,2)" --extra_pad=1.5\
#  --x_axis_label="Tau Mass [GeV]" --datacard="1prong" \
#  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --cat="pt_2>30. && tau_decay_mode==10" --var="m_2(20,0,2)" --extra_pad=1.5 \
#  --x_axis_label="Tau Mass [GeV]" --datacard="3prong" \
#  --background_scheme="et_default" $ET_INC_SHIFT

#./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --cat="pt_2>30. && tau_decay_mode==10" --var="m_2(20,0,2)" --extra_pad=1.5 \
#  --x_axis_label="Tau Mass [GeV]" --datacard="3prong" \
#  --background_scheme="mt_with_zmm" $MT_INC_SHIFT

#### n_jets (log)

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="n_jets_csv(10,-0.5,9.5)"  --x_axis_label="Number of Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" $ET_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="n_jets_csv(10,-0.5,9.5)"  --x_axis_label="Number of Jets" \
  --draw_ratio=true --log_y=true --extra_pad=5 --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

#### n_bjets (log)

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="n_bjets_csv(9,-0.5,8.5)"  --x_axis_label="Number of b-tagged Jets" \
  --draw_ratio=true --log_y=true --extra_pad=500 --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" $ET_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="n_bjets_csv(9,-0.5,8.5)"  --x_axis_label="Number of b-tagged Jets" \
  --draw_ratio=true --log_y=true --extra_pad=500 --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT \
  --custom_y_axis_min=true --y_axis_min=0.99

#### mt_1 / pzeta

MT_BINS="mt_1(40,0,160)"
if [ "$MASSCUTS" = "MassCuts" ]
then
  MT_BINS="mt_1(20,0,160)"
fi
./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et  \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --set_alias="sel:1" --var=$MT_BINS \
  --x_axis_label="m_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" --extra_pad=1.5 $ET_BAND_ONLY \
  --background_scheme="et_default" $ET_INC_SHIFT

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --set_alias="sel:1" --var=$MT_BINS \
  --x_axis_label="m_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" --extra_pad=1.5 $MT_BAND_ONLY \
  --background_scheme="mt_with_zmm" $MT_INC_SHIFT 


#### n_vtx

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="n_vtx(30,0,30)" $ET_INC_SHIFT \
  --x_axis_label="Number of Vertices" --datacard="2jetinclusive"$MASSCUTS"" --extra_pad=1.8 \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="n_vtx(30,0,30)" $MT_INC_SHIFT \
  --x_axis_label="Number of Vertices" --datacard="2jetinclusive"$MASSCUTS"" --extra_pad=1.8 \
  --background_scheme="mt_with_zmm"


#### tau_decay_mode

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="tau_decay_mode(3,-4,11)" $ET_INC_SHIFT \
  --x_axis_label="Tau Decay Mode" --datacard="2jetinclusive"$MASSCUTS"" --extra_pad=2.0 \
  --background_scheme="et_default"\
  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="tau_decay_mode(3,-4,11)" $MT_INC_SHIFT \
  --x_axis_label="Tau Decay Mode" --datacard="2jetinclusive"$MASSCUTS"" --extra_pad=2.0 \
  --background_scheme="mt_with_zmm"\
  --x_axis_bin_labels="1 Prong 0 #pi^{0}:1 Prong 1 #pi^{0}:3 Prong" 


###############################################################################
#### Jet selection plots
###############################################################################


#### jet_csvpt_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="jet_csvpt_1(20,0,200)" $ET_INC_SHIFT \
  --x_axis_label="Leading selected jet p_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" --extra_pad=1.3

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="jet_csvpt_1(20,0,200)" \
  --x_axis_label="Leading selected jet p_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" $MT_INC_SHIFT \
  --background_scheme="mt_with_zmm" --extra_pad=1.3

#### jet_csveta_1
BETA_BINS="jet_csveta_1(10,-3,3)"
if [ "$YEAR" = "2012" ]
then
  BETA_BINS="jet_csveta_1(15,-3,3)"
fi
./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var=$BETA_BINS --extra_pad=2.0 $ET_INC_SHIFT \
  --x_axis_label="Leading selected jet #eta" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var=$BETA_BINS --extra_pad=2.0 \
  --x_axis_label="Leading selected jet #eta" --datacard="2jetinclusive"$MASSCUTS"" $MT_INC_SHIFT \
  --background_scheme="mt_with_zmm"

#### jet_csvpt_2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="jet_csvpt_2(20,0,200)" $ET_INC_SHIFT \
  --x_axis_label="Subleading selected jet p_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" --extra_pad=1.3

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="jet_csvpt_2(20,0,200)" \
  --x_axis_label="Subleading selected jet p_{T} [GeV]" --datacard="2jetinclusive"$MASSCUTS"" $MT_INC_SHIFT \
  --background_scheme="mt_with_zmm" --extra_pad=1.3

#### jet_csveta_2
BETA_BINS="jet_csveta_2(10,-3,3)"
if [ "$YEAR" = "2012" ]
then
  BETA_BINS="jet_csveta_2(15,-3,3)"
fi
./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var=$BETA_BINS --extra_pad=2.0 $ET_INC_SHIFT \
  --x_axis_label="Subleading selected jet #eta" --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default"

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var=$BETA_BINS --extra_pad=2.0 \
  --x_axis_label="Subleading selected jet #eta" --datacard="2jetinclusive"$MASSCUTS"" $MT_INC_SHIFT \
  --background_scheme="mt_with_zmm"

#### jet_csvcsv_1

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="jet_csvbcsv_1(25,0,1)" \
  --x_axis_label="Leading selected jet CSV" --datacard="2jetinclusive"$MASSCUTS""  $ET_INC_SHIFT \
  --background_scheme="et_default" --extra_pad=1.2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="jet_csvbcsv_1(25,0,1)" \
  --x_axis_label="Leading selected jet CSV" --datacard="2jetinclusive"$MASSCUTS""  $MT_INC_SHIFT \
  --background_scheme="mt_with_zmm" --extra_pad=1.2

#### jet_csvcsv_2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="jet_csvbcsv_2(25,0,1)" \
  --x_axis_label="Subleading selected jet CSV" --datacard="2jetinclusive"$MASSCUTS""  $ET_INC_SHIFT \
  --background_scheme="et_default" --extra_pad=1.2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="jet_csvbcsv_2(25,0,1)" \
  --x_axis_label="Subleading selected jet CSV" --datacard="2jetinclusive"$MASSCUTS""  $MT_INC_SHIFT \
  --background_scheme="mt_with_zmm" --extra_pad=1.2


#### jet_csvjdeta

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="jet_csv_deta(20,0,10)" $ET_INC_SHIFT \
  --x_axis_label="#Delta#eta_{jj}" \
  --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="et_default" --extra_pad=1.2

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="2jetinclusive"$MASSCUTS"" --var="jet_csv_deta(20,0,10)" $MT_INC_SHIFT \
  --x_axis_label="#Delta#eta_{jj}" \
  --datacard="2jetinclusive"$MASSCUTS"" \
  --background_scheme="mt_with_zmm" --extra_pad=1.2


