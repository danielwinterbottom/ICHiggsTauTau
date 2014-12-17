
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
HIGGSMASSBINS="110,130,150,170,190,210,230,250,270,290,310,330,350,370,390,410,430,450,500,550,600,650,700,750,800,850,900"
KINFITMASSBINS="200,250,270,290,310,330,350,370,390,410,430,450,500,550,600,650,700"
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


#### svfit mass with cut on m_bb

if [ "$MASSCUTS" = "MassCuts" ]
then
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="2jet0tagSFMassCutMbb" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSFMassCutMbb" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="2jet0tagSFMassCutMbb" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSFMassCutMbb" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="et_default" 
  
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=24 --var="m_sv"["$MSSMBINS"] --cat="2jet1tagSFMassCutMbb" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCutMbb" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=24 --var="m_sv"["$MSSMBINS"] --cat="2jet1tagSFMassCutMbb" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCutMbb" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=27 --var="m_sv"["$MSSMBINS2JET2TAG"] --cat="2jet2tagSFMassCutMbb" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSFMassCutMbb" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=27 --var="m_sv"["$MSSMBINS2JET2TAG"] --cat="2jet2tagSFMassCutMbb" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSFMassCutMbb" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="et_default"
else
#### svfit mass

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=8 --var="m_sv"["$MSSMBINS"] --cat="2jet0tagSF" \
    --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
    --norm_bins=true --datacard="2jet0tagSF" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
    --custom_y_axis_min=true --y_axis_min=0.0099 \
    --blind=true --x_blind_min=100 --x_blind_max=150 \
    --background_scheme="mt_with_zmm" 

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=8 --var="m_sv"["$MSSMBINS"] --cat="2jet0tagSF" \
    --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
    --norm_bins=true --datacard="2jet0tagSF" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
    --custom_y_axis_min=true --y_axis_min=0.0099 \
    --blind=true --x_blind_min=100 --x_blind_max=150 \
    --background_scheme="et_default" 
      
  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=24 --var="m_sv"["$MSSMBINS"] --cat="2jet1tagSF" \
    --x_axis_label="M_{#tau#tau} [GeV]" $MT_2JET1TAG_SHIFT \
    --norm_bins=true --datacard="2jet1tagSF" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
    --custom_y_axis_min=true --y_axis_min=0.0099 \
    --blind=true --x_blind_min=100 --x_blind_max=150 \
    --background_scheme="mt_with_zmm" 

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=24 --var="m_sv"["$MSSMBINS"] --cat="2jet1tagSF" \
    --x_axis_label="M_{#tau#tau} [GeV]" $ET_2JET1TAG_SHIFT \
    --norm_bins=true --datacard="2jet1tagSF" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
    --custom_y_axis_min=true --y_axis_min=0.0099 \
    --blind=true --x_blind_min=100 --x_blind_max=150 \
    --background_scheme="et_default" 

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=27 --var="m_sv"["$MSSMBINS2JET2TAG"] --cat="2jet2tagSF" \
    --x_axis_label="M_{#tau#tau} [GeV]" $MT_2JET2TAG_SHIFT \
    --norm_bins=true --datacard="2jet2tagSF" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
    --custom_y_axis_min=true --y_axis_min=0.0099 \
    --blind=true --x_blind_min=100 --x_blind_max=150 \
    --background_scheme="mt_with_zmm" --fix_negative_bins="QCD"

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=27 --var="m_sv"["$MSSMBINS2JET2TAG"] --cat="2jet2tagSF" \
    --x_axis_label="M_{#tau#tau} [GeV]" $ET_2JET2TAG_SHIFT \
    --norm_bins=true --datacard="2jet2tagSF" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
    --custom_y_axis_min=true --y_axis_min=0.0099 \
    --blind=true --x_blind_min=100 --x_blind_max=150 \
    --background_scheme="et_default" --fix_negative_bins="QCD"
fi


#### m_bb with mass cut on m_tautau
if [ "$MASSCUTS" = "MassCuts" ]
then
  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=8 --cat="2jet0tagSFMassCutMtt" --var="prebjet_mjj(30,0,600)" $ET_INC_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet0tagSFMassCutMtt" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="et_default" --extra_pad=1.2

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=8 --cat="2jet0tagSFMassCutMtt" --var="prebjet_mjj(30,0,600)" $MT_INC_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet0tagSFMassCutMtt" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="mt_with_zmm" --extra_pad=1.2

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=24 --cat="2jet1tagSFMassCutMtt" --var="prebjet_mjj(30,0,600)" $ET_2JET1TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet1tagSFMassCutMtt" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="et_default" --extra_pad=1.2

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=24 --cat="2jet1tagSFMassCutMtt" --var="prebjet_mjj(30,0,600)" $MT_2JET1TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet1tagSFMassCutMtt" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="mt_with_zmm" --extra_pad=1.2

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=27 --cat="2jet2tagSFMassCutMtt" --var="prebjet_mjj(15,0,600)" $ET_2JET2TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet2tagSFMassCutMtt" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="et_default" --extra_pad=1.2 --fix_negative_bins="QCD"

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=27 --cat="2jet2tagSFMassCutMtt" --var="prebjet_mjj(15,0,600)" $MT_2JET2TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet2tagSFMassCutMtt" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="mt_with_zmm" --extra_pad=1.2 --fix_negative_bins="QCD"
else
#### m_bb

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=8 --cat="2jet0tagSF" --var="prebjet_mjj(30,0,600)" $ET_INC_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet0tagSF" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="et_default" --extra_pad=1.2

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=8 --cat="2jet0tagSF" --var="prebjet_mjj(30,0,600)" $MT_INC_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet0tagSF" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="mt_with_zmm" --extra_pad=1.2

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=24 --cat="2jet1tagSF" --var="prebjet_mjj(30,0,600)" $ET_2JET1TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet1tagSF" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="et_default" --extra_pad=1.2

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=24 --cat="2jet1tagSF" --var="prebjet_mjj(30,0,600)" $MT_2JET1TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet1tagSF" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="mt_with_zmm" --extra_pad=1.2

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=27 --cat="2jet2tagSF" --var="prebjet_mjj(15,0,600)" $ET_2JET2TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet2tagSF" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="et_default" --extra_pad=1.2 --fix_negative_bins="QCD"

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=27 --cat="2jet2tagSF" --var="prebjet_mjj(15,0,600)" $MT_2JET2TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet2tagSF" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="mt_with_zmm" --extra_pad=1.2 --fix_negative_bins="QCD"

fi


#### m_ttbb with mass cuts
if [ "$MASSCUTS" = "MassCuts" ]
then

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=14 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet0tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=14 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet0tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=15 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet1tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=15 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet1tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $MT_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=16 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet2tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default"  --fix_negative_bins="QCD"


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=16 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet2tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $MT_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm"  --fix_negative_bins="QCD"


else
#### m_ttbb
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet0tagSF" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet0tagSF" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=24 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet1tagSF" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=24 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet1tagSF" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $MT_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=27 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet2tagSF" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default"  --fix_negative_bins="QCD"


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=27 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet2tagSF" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $MT_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm"  --fix_negative_bins="QCD"

fi

#### m_H with mass cuts
if [ "$MASSCUTS" = "MassCuts" ]
then

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=14 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet0tagSFMassCuts" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=14 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet0tagSFMassCuts" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=15 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet1tagSFMassCuts" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $ET_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=15 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet1tagSFMassCuts" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $MT_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=16 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet2tagSFMassCuts" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $ET_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=16 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet2tagSFMassCuts" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $MT_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

else
#### m_H
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet0tagSF" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet0tagSF" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=24 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet1tagSF" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $ET_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=24 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet1tagSF" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $MT_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=27 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet2tagSF" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $ET_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default"  --fix_negative_bins="QCD"


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=27 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet2tagSF" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $MT_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm"  --fix_negative_bins="QCD"

fi

#### chi2 from kinematic fit
if [ "$MASSCUTS" = "MassCuts" ]
then
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=14 --var="m_H_hh_chi2(25,0,100)" --cat="2jet0tagSFMassCuts" --y_axis_label="Events"\
  --x_axis_label="chi2" $ET_INC_SHIFT \
  --norm_bins=false --datacard="2jet0tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=14 --var="m_H_hh_chi2(25,0,100)" --cat="2jet0tagSFMassCuts" --y_axis_label="Events"\
  --x_axis_label="chi2" $MT_INC_SHIFT \
  --norm_bins=false --datacard="2jet0tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=15 --var="m_H_hh_chi2(25,0,100)" --cat="2jet1tagSFMassCuts" --y_axis_label="Events"\
  --x_axis_label="chi2" $ET_2JET1TAG_SHIFT \
  --norm_bins=false --datacard="2jet1tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=15 --var="m_H_hh_chi2(25,0,100)" --cat="2jet1tagSFMassCuts" --y_axis_label="Events"\
  --x_axis_label="chi2" $MT_2JET1TAG_SHIFT \
  --norm_bins=false --datacard="2jet1tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=16 --var="m_H_hh_chi2(25,0,100)" --cat="2jet2tagSFMassCuts" --y_axis_label="Events"\
  --x_axis_label="chi2" $ET_2JET2TAG_SHIFT \
  --norm_bins=false --datacard="2jet2tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=16 --var="m_H_hh_chi2(25,0,100)" --cat="2jet2tagSFMassCuts" --y_axis_label="Events"\
  --x_axis_label="chi2" $MT_2JET2TAG_SHIFT \
  --norm_bins=false --datacard="2jet2tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="mt_with_zmm" 
else
./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_H_hh_chi2(25,0,100)" --cat="2jet0tagSF" --y_axis_label="Events"\
  --x_axis_label="chi2" $ET_INC_SHIFT \
  --norm_bins=false --datacard="2jet0tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_H_hh_chi2(25,0,100)" --cat="2jet0tagSF" --y_axis_label="Events"\
  --x_axis_label="chi2" $MT_INC_SHIFT \
  --norm_bins=false --datacard="2jet0tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=24 --var="m_H_hh_chi2(25,0,100)" --cat="2jet1tagSF" --y_axis_label="Events"\
  --x_axis_label="chi2" $ET_2JET1TAG_SHIFT \
  --norm_bins=false --datacard="2jet1tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=24 --var="m_H_hh_chi2(25,0,100)" --cat="2jet1tagSF" --y_axis_label="Events"\
  --x_axis_label="chi2" $MT_2JET1TAG_SHIFT \
  --norm_bins=false --datacard="2jet1tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=27 --var="m_H_hh_chi2(25,0,100)" --cat="2jet2tagSF" --y_axis_label="Events"\
  --x_axis_label="chi2" $ET_2JET2TAG_SHIFT \
  --norm_bins=false --datacard="2jet2tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=27 --var="m_H_hh_chi2(25,0,100)" --cat="2jet2tagSF" --y_axis_label="Events"\
  --x_axis_label="chi2" $MT_2JET2TAG_SHIFT \
  --norm_bins=false --datacard="2jet2tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="mt_with_zmm" 
fi
