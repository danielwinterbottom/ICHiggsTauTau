
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
#HIGGSMASSBINS="110,130,150,170,190,210,230,250,270,290,310,330,350,370,390,410,430,450,500,550,600,650,700,750,800,850,900"
HIGGSMASSBINS="110,130,150,170,190,210,230,250,270,290,310,330,350,370,390,410,430,450,500,550,600"
KINFITMASSBINS="200,250,270,290,310,330,350,370,390,410,430,450,500,550,600,650,700"
#MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1100,1200,1300,1400,1500"
#MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,1000,1500"


#############################################################
#### Add scale factors to backgrounds
#############################################################

ET_INC_SHIFT=""
ET_2JET1TAG_SHIFT=""
ET_2JET2TAG_SHIFT=""
MT_INC_SHIFT="" 
MT_2JET1TAG_SHIFT="" 
MT_2JET2TAG_SHIFT="" 

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




#### svfit mass with cut on m_bb

if [ "$MASSCUTS" = "MassCuts" ]
then
./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=28 --var="m_sv"["$MSSMBINS"] --cat="2jet0tagSFMassCutMbb" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSFMassCutMbb" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=28 --var="m_sv"["$MSSMBINS"] --cat="2jet0tagSFMassCutMbb" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSFMassCutMbb" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="et_default" 
  
./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=24 --var="m_sv"["$MSSMBINS"] --cat="2jet1tagSFMassCutMbb" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCutMbb" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=24 --var="m_sv"["$MSSMBINS"] --cat="2jet1tagSFMassCutMbb" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCutMbb" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="et_default" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=27 --var="m_sv"["$MSSMBINS2JET2TAG"] --cat="2jet2tagSFMassCutMbb" \
  --x_axis_label="M_{#tau#tau} [GeV]" $MT_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSFMassCutMbb" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=27 --var="m_sv"["$MSSMBINS2JET2TAG"] --cat="2jet2tagSFMassCutMbb" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSFMassCutMbb" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="et_default"
else
#### svfit mass

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=28 --var="m_sv"["$MSSMBINS"] --cat="2jet0tagSF" \
    --x_axis_label="M_{#tau#tau} [GeV]" $MT_INC_SHIFT \
    --norm_bins=true --datacard="2jet0tagSF" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
    --custom_y_axis_min=true --y_axis_min=0.0099 \
    --blind=false --x_blind_min=100 --x_blind_max=150 \
    --background_scheme="mt_with_zmm" 

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=28 --var="m_sv"["$MSSMBINS"] --cat="2jet0tagSF" \
    --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
    --norm_bins=true --datacard="2jet0tagSF" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
    --custom_y_axis_min=true --y_axis_min=0.0099 \
    --blind=false --x_blind_min=100 --x_blind_max=150 \
    --background_scheme="et_default" 
      
  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=24 --var="m_sv"["$MSSMBINS"] --cat="2jet1tagSF" \
    --x_axis_label="M_{#tau#tau} [GeV]" $MT_2JET1TAG_SHIFT \
    --norm_bins=true --datacard="2jet1tagSF" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
    --custom_y_axis_min=true --y_axis_min=0.0099 \
    --blind=false --x_blind_min=100 --x_blind_max=150 \
    --background_scheme="mt_with_zmm" 

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=24 --var="m_sv"["$MSSMBINS"] --cat="2jet1tagSF" \
    --x_axis_label="M_{#tau#tau} [GeV]" $ET_2JET1TAG_SHIFT \
    --norm_bins=true --datacard="2jet1tagSF" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
    --custom_y_axis_min=true --y_axis_min=0.0099 \
    --blind=false --x_blind_min=100 --x_blind_max=150 \
    --background_scheme="et_default" 

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=27 --var="m_sv"["$MSSMBINS2JET2TAG"] --cat="2jet2tagSF" \
    --x_axis_label="M_{#tau#tau} [GeV]" $MT_2JET2TAG_SHIFT \
    --norm_bins=true --datacard="2jet2tagSF" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
    --custom_y_axis_min=true --y_axis_min=0.0099 \
    --blind=false --x_blind_min=100 --x_blind_max=150 \
    --background_scheme="mt_with_zmm" --fix_negative_bins="QCD"

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=27 --var="m_sv"["$MSSMBINS2JET2TAG"] --cat="2jet2tagSF" \
    --x_axis_label="M_{#tau#tau} [GeV]" $ET_2JET2TAG_SHIFT \
    --norm_bins=true --datacard="2jet2tagSF" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
    --custom_y_axis_min=true --y_axis_min=0.0099 \
    --blind=false --x_blind_min=100 --x_blind_max=150 \
    --background_scheme="et_default" --fix_negative_bins="QCD"
fi


#### m_bb with mass cut on m_tautau
if [ "$MASSCUTS" = "MassCuts" ]
then
  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=28 --cat="2jet0tagSFMassCutMtt" --var="jet_csv_mjj(30,0,600)" $ET_INC_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet0tagSFMassCutMtt" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=false --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="et_default" --extra_pad=0.2

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=28 --cat="2jet0tagSFMassCutMtt" --var="jet_csv_mjj(30,0,600)" $MT_INC_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet0tagSFMassCutMtt" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=false --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="mt_with_zmm" --extra_pad=0.2

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=24 --cat="2jet1tagSFMassCutMtt" --var="jet_csv_mjj(30,0,600)" $ET_2JET1TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet1tagSFMassCutMtt" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=false --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="et_default" --extra_pad=0.2

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=24 --cat="2jet1tagSFMassCutMtt" --var="jet_csv_mjj(30,0,600)" $MT_2JET1TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet1tagSFMassCutMtt" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=false --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="mt_with_zmm" --extra_pad=0.2

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=27 --cat="2jet2tagSFMassCutMtt" --var="jet_csv_mjj(15,0,600)" $ET_2JET2TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet2tagSFMassCutMtt" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=false --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="et_default" --extra_pad=0.2 --fix_negative_bins="QCD"

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=27 --cat="2jet2tagSFMassCutMtt" --var="jet_csv_mjj(15,0,600)" $MT_2JET2TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet2tagSFMassCutMtt" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=false --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="mt_with_zmm" --extra_pad=0.2 --fix_negative_bins="QCD"
else
#### m_bb

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=28 --cat="2jet0tagSF" --var="jet_csv_mjj(30,0,600)" $ET_INC_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet0tagSF" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=false --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="et_default" --extra_pad=0.2

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=28 --cat="2jet0tagSF" --var="jet_csv_mjj(30,0,600)" $MT_INC_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet0tagSF" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=false --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="mt_with_zmm" --extra_pad=0.2

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=24 --cat="2jet1tagSF" --var="jet_csv_mjj(30,0,600)" $ET_2JET1TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet1tagSF" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=false --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="et_default" --extra_pad=0.2

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=24 --cat="2jet1tagSF" --var="jet_csv_mjj(30,0,600)" $MT_2JET1TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet1tagSF" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=false --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="mt_with_zmm" --extra_pad=0.2

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
    --method=27 --cat="2jet2tagSF" --var="jet_csv_mjj(15,0,600)" $ET_2JET2TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet2tagSF" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=false --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="et_default" --extra_pad=0.2 --fix_negative_bins="QCD"

  ./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
    --method=27 --cat="2jet2tagSF" --var="jet_csv_mjj(15,0,600)" $MT_2JET2TAG_SHIFT \
    --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="2jet2tagSF" --y_axis_label="dN/dm_{jj} [1/GeV]"\
    --blind=false --x_blind_min=80 --x_blind_max=160 \
    --background_scheme="mt_with_zmm" --extra_pad=0.2 --fix_negative_bins="QCD"

fi


#### m_ttbb with mass cuts
if [ "$MASSCUTS" = "MassCuts" ]
then

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=14 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet0tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=14 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet0tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=15 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet1tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=15 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet1tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $MT_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=16 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet2tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default"  --fix_negative_bins="QCD"


./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=16 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet2tagSFMassCuts" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $MT_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm"  --fix_negative_bins="QCD"


else
#### m_ttbb
./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=28 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet0tagSF" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=28 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet0tagSF" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=24 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet1tagSF" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=24 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet1tagSF" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $MT_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=27 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet2tagSF" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default"  --fix_negative_bins="QCD"


./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=27 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="2jet2tagSF" --y_axis_label="dN/dm_{#tau#tau+jj} [1/GeV]"\
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $MT_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm"  --fix_negative_bins="QCD"

fi

#### m_H with mass cuts
if [ "$MASSCUTS" = "MassCuts" ]
then

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=14 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet0tagSFMassCuts" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=14 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet0tagSFMassCuts" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=15 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet1tagSFMassCuts" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $ET_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=15 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet1tagSFMassCuts" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $MT_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=16 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet2tagSFMassCuts" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $ET_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=16 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet2tagSFMassCuts" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $MT_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

else
#### m_H
./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=28 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet0tagSF" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=28 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet0tagSF" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $MT_INC_SHIFT \
  --norm_bins=true --datacard="2jet0tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=24 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet1tagSF" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $ET_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=24 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet1tagSF" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $MT_2JET1TAG_SHIFT \
  --norm_bins=true --datacard="2jet1tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=27 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet2tagSF" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $ET_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="et_default"  --fix_negative_bins="QCD"


./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=27 --var="m_H_hh"["$KINFITMASSBINS"] --cat="2jet2tagSF" --y_axis_label="dN/dm_{H}^{kinfit} [1/GeV]"\
  --x_axis_label="M_{H}^{kinfit} [GeV]" $MT_2JET2TAG_SHIFT \
  --norm_bins=true --datacard="2jet2tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=400 \
  --background_scheme="mt_with_zmm"  --fix_negative_bins="QCD"

fi

#### chi2 from kinematic fit
if [ "$MASSCUTS" = "MassCuts" ]
then
./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=14 --var="m_H_hh_chi2(25,0,100)" --cat="2jet0tagSFMassCuts" --y_axis_label="Events"\
  --x_axis_label="chi2" $ET_INC_SHIFT \
  --norm_bins=false --datacard="2jet0tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="et_default" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=14 --var="m_H_hh_chi2(25,0,100)" --cat="2jet0tagSFMassCuts" --y_axis_label="Events"\
  --x_axis_label="chi2" $MT_INC_SHIFT \
  --norm_bins=false --datacard="2jet0tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=15 --var="m_H_hh_chi2(25,0,100)" --cat="2jet1tagSFMassCuts" --y_axis_label="Events"\
  --x_axis_label="chi2" $ET_2JET1TAG_SHIFT \
  --norm_bins=false --datacard="2jet1tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="et_default" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=15 --var="m_H_hh_chi2(25,0,100)" --cat="2jet1tagSFMassCuts" --y_axis_label="Events"\
  --x_axis_label="chi2" $MT_2JET1TAG_SHIFT \
  --norm_bins=false --datacard="2jet1tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=16 --var="m_H_hh_chi2(25,0,100)" --cat="2jet2tagSFMassCuts" --y_axis_label="Events"\
  --x_axis_label="chi2" $ET_2JET2TAG_SHIFT \
  --norm_bins=false --datacard="2jet2tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="et_default" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=16 --var="m_H_hh_chi2(25,0,100)" --cat="2jet2tagSFMassCuts" --y_axis_label="Events"\
  --x_axis_label="chi2" $MT_2JET2TAG_SHIFT \
  --norm_bins=false --datacard="2jet2tagSFMassCuts"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="mt_with_zmm" 
else
./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=28 --var="m_H_hh_chi2(25,0,100)" --cat="2jet0tagSF" --y_axis_label="Events"\
  --x_axis_label="chi2" $ET_INC_SHIFT \
  --norm_bins=false --datacard="2jet0tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="et_default" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=28 --var="m_H_hh_chi2(25,0,100)" --cat="2jet0tagSF" --y_axis_label="Events"\
  --x_axis_label="chi2" $MT_INC_SHIFT \
  --norm_bins=false --datacard="2jet0tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=24 --var="m_H_hh_chi2(25,0,100)" --cat="2jet1tagSF" --y_axis_label="Events"\
  --x_axis_label="chi2" $ET_2JET1TAG_SHIFT \
  --norm_bins=false --datacard="2jet1tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="et_default" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=24 --var="m_H_hh_chi2(25,0,100)" --cat="2jet1tagSF" --y_axis_label="Events"\
  --x_axis_label="chi2" $MT_2JET1TAG_SHIFT \
  --norm_bins=false --datacard="2jet1tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=27 --var="m_H_hh_chi2(25,0,100)" --cat="2jet2tagSF" --y_axis_label="Events"\
  --x_axis_label="chi2" $ET_2JET2TAG_SHIFT \
  --norm_bins=false --datacard="2jet2tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="et_default" 

./bin/HiggsTauTauPlot4 --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=27 --var="m_H_hh_chi2(25,0,100)" --cat="2jet2tagSF" --y_axis_label="Events"\
  --x_axis_label="chi2" $MT_2JET2TAG_SHIFT \
  --norm_bins=false --datacard="2jet2tagSF"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=0 --x_blind_max=10 \
  --background_scheme="mt_with_zmm" 
fi
