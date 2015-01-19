
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
      ET_INC_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.00855,QCD:1.18834,W:1.00239,ZL:0.96883,ZJ:0.96042,VV:1.00071,TT:0.96051 --auto_error_band=0.06626"
      MT_INC_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:0.93468,QCD:1.11541,W:1.00498,ZL:1.03907,ZJ:1.04238,VV:1.01803,TT:1.03997 --auto_error_band=0.06765"
      ET_2JET1TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:0.99853,QCD:1.02430,W:1.16519,ZL:0.91895,ZJ:0.90445,VV:0.95126,TT:0.96103 --auto_error_band=0.08715"
      MT_2JET1TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:0.96287,QCD:0.89908,W:1.17610,ZL:0.93839,ZJ:1.07402,VV:0.99802,TT:1.05708 --auto_error_band=0.08071"
      ET_2JET2TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:0.97541,QCD:0.98141,W:0.77511,ZL:0.86009,ZJ:0.99583,VV:0.99784,TT:0.95665 --auto_error_band=0.10982"
      MT_2JET2TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.03280,QCD:1.00855,W:0.63720,ZL:1.44467,ZJ:1.62335,VV:1.02247,TT:1.07278 --auto_error_band=0.09702"
    else 
      #### Fit without mass cuts. Currently using inclusive is 2jet0tag instead of 2jetinclusive
      echo "Applying 8TeV scale factors from fit without mass cuts..."
      ET_INC_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.00641,QCD:0.98875,W:1.00561,ZL:0.95730,ZJ:0.98783,VV:1.00324,TT:0.96238 --auto_error_band=0.05581"
      MT_INC_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.02150,QCD:1.07399,W:0.99922,ZL:1.00747,ZJ:1.01138,VV:1.00671,TT:1.02479 --auto_error_band=0.05524"
      ET_2JET1TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.01437,QCD:1.00917,W:1.01645,ZL:0.93944,ZJ:0.97583,VV:0.99819,TT:0.95777 --auto_error_band=0.07744"
      MT_2JET1TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.01993,QCD:0.95018,W:1.06923,ZL:1.00906,ZJ:1.00569,VV:1.02804,TT:1.04886 --auto_error_band=0.07038"
      ET_2JET2TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.03829,QCD:1.04448,W:0.85095,ZL:0.89490,ZJ:1.00431,VV:0.99785,TT:0.94808 --auto_error_band=0.09952"
      MT_2JET2TAG_SHIFT="  --draw_error_band=true --shift_backgrounds=ZTT:1.03070,QCD:1.01704,W:0.55213,ZL:1.09945,ZJ:1.20458,VV:1.02121,TT:1.07438 --auto_error_band=0.08462"
      fi
    fi
  fi  




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
