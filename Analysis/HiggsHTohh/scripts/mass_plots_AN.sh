
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
      ET_INC_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.06262,QCD:0.93680,W:1.00077,ZL:1.06344,ZJ:1.02953,VV:1.03175,TT:1.01239 --auto_error_band=0.02852"
      ET_BAND_ONLY="  --draw_error_band=true  --auto_error_band=0.02852"
      MT_INC_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.00527,QCD:0.96815,W:0.92797,ZL:1.05419,ZJ:1.04077,VV:0.98635,TT:0.94947 --auto_error_band=0.02292"
      MT_BAND_ONLY="  --draw_error_band=true  --auto_error_band=0.02292"
      ET_2JET1TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.05585,QCD:0.94913,W:1.07497,ZL:1.05302,ZJ:1.02125,VV:1.01510,TT:0.92967 --auto_error_band=0.04717"
      MT_2JET1TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.00871,QCD:0.91516,W:0.98349,ZL:1.06099,ZJ:1.01715,VV:0.99253,TT:0.93628 --auto_error_band=0.03822"
      ET_2JET2TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.03365,QCD:0.88208,W:1.0,ZL:1.05302,ZJ:1.00923,VV:1.01510,TT:0.92967 --auto_error_band=0.07460"
      MT_2JET2TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:0.99419,QCD:0.94554,W:1.0,ZL:1.06099,ZJ:1.00678,VV:0.99253,TT:0.93628 --auto_error_band=0.05843"
    else 
      #### Fit without mass cuts. Currently using inclusive is 2jet0tag instead of 2jetinclusive
      echo "Applying 8TeV scale factors from fit without mass cuts..."
      ET_INC_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.03797,QCD:0.90932,W:0.99442,ZL:1.03668,ZJ:1.03099,VV:1.01245,TT:0.98581 --auto_error_band=0.01629"
      ET_BAND_ONLY="  --draw_error_band=true  --auto_error_band=0.02852"
      MT_INC_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.05551,QCD:0.93496,W:0.96594,ZL:1.02465,ZJ:1.02279,VV:1.05501,TT:0.99297 --auto_error_band=0.01253"
      MT_BAND_ONLY="  --draw_error_band=true  --auto_error_band=0.02292"
      ET_2JET1TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.04572,QCD:0.90898,W:1.04110,ZL:1.01646,ZJ:1.03378,VV:0.98023,TT:0.85394 --auto_error_band=0.03197"
      MT_2JET1TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.05989,QCD:0.89754,W:1.07921,ZL:1.02365,ZJ:0.99739,VV:1.05510,TT:0.90401 --auto_error_band=0.02318"
      ET_2JET2TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.02991,QCD:0.90704,W:0.94865,ZL:1.01646,ZJ:1.02002,VV:0.98023,TT:0.85394 --auto_error_band=0.04985"
      MT_2JET2TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.04566,QCD:0.99538,W:1.0,ZL:1.02365,ZJ:1.00331,VV:1.05510,TT:0.90401 --auto_error_band=0.03746"
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
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="2jet1tagSFMassCutMbb" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCutMbb" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="2jet1tagSFMassCutMbb" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCutMbb" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS2JETS2TAG"] --cat="2jet2tagSFMassCutMbb" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSFMassCutMbb" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS2JET2TAG"] --cat="2jet2tagSFMassCutMbb" \
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
    --method=8 --var="m_sv"["$MSSMBINS"] --cat="2jet1tagSF" \
    --x_axis_label="M_{#tau#tau} [GeV]" $MT_2JET1TAG_SHIFT \
    --norm_bins=true --datacard="2jet1tagSF" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
    --custom_y_axis_min=true --y_axis_min=0.0099 \
    --blind=true --x_blind_min=100 --x_blind_max=150 \
    --background_scheme="mt_with_zmm" 

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=8 --var="m_sv"["$MSSMBINS"] --cat="2jet1tagSF" \
    --x_axis_label="M_{#tau#tau} [GeV]" $ET_2JET1TAG_SHIFT \
    --norm_bins=true --datacard="2jet1tagSF" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
    --custom_y_axis_min=true --y_axis_min=0.0099 \
    --blind=true --x_blind_min=100 --x_blind_max=150 \
    --background_scheme="et_default" 

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=8 --var="m_sv"["$MSSMBINS2JET2TAG"] --cat="2jet2tagSF" \
    --x_axis_label="M_{#tau#tau} [GeV]" $MT_2JET1TAG_SHIFT \
    --norm_bins=true --datacard="2jet2tagSF" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
    --custom_y_axis_min=true --y_axis_min=0.0099 \
    --blind=true --x_blind_min=100 --x_blind_max=150 \
    --background_scheme="mt_with_zmm" --fix_negative_bins="QCD"

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=8 --var="m_sv"["$MSSMBINS2JET2TAG"] --cat="2jet2tagSF" \
    --x_axis_label="M_{#tau#tau} [GeV]" $ET_2JET1TAG_SHIFT \
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
    --method=8 --cat="2jet1tagSFMassCutMtt" --var="prebjet_mjj(30,0,600)" $ET_2JET1TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet1tagSFMassCutMtt" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="et_default" --extra_pad=1.2

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=8 --cat="2jet1tagSFMassCutMtt" --var="prebjet_mjj(30,0,600)" $MT_2JET1TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet1tagSFMassCutMtt" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="mt_with_zmm" --extra_pad=1.2

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=8 --cat="2jet2tagSFMassCutMtt" --var="prebjet_mjj(15,0,600)" $ET_2JET2TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet2tagSFMassCutMtt" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="et_default" --extra_pad=1.2 --fix_negative_bins="QCD"

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=8 --cat="2jet2tagSFMassCutMtt" --var="prebjet_mjj(15,0,600)" $MT_2JET2TAG_SHIFT \
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
    --method=8 --cat="2jet1tagSF" --var="prebjet_mjj(30,0,600)" $ET_2JET1TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet1tagSF" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="et_default" --extra_pad=1.2

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=8 --cat="2jet1tagSF" --var="prebjet_mjj(30,0,600)" $MT_2JET1TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet1tagSF" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="mt_with_zmm" --extra_pad=1.2

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=8 --cat="2jet2tagSF" --var="prebjet_mjj(15,0,600)" $ET_2JET2TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet2tagSF" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="et_default" --extra_pad=1.2 --fix_negative_bins="QCD"

  ./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=8 --cat="2jet2tagSF" --var="prebjet_mjj(15,0,600)" $MT_2JET2TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet2tagSF" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=true --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="mt_with_zmm" --extra_pad=1.2 --fix_negative_bins="QCD"

fi


#### m_ttbb with mass cuts
if [ "$MASSCUTS" = "MassCuts" ]
then

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet0tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet0tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=24 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet1tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=24 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet1tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $MT_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=27 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet2tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default"  --fix_negative_bins="QCD"


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=27 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet2tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
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
  --method=15 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet0tagSFMassCuts" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=16 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet1tagSFMassCuts" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $ET_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=14 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet1tagSFMassCuts" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $MT_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=15 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet2tagSFMassCuts" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
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
