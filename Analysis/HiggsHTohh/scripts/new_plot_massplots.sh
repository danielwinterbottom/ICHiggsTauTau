
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
HIGGSMASSBINS="0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000"
#MSSMBINS="0,20,40,60,80,100,120,140,160,180,200,250,300,350"
#MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1100,1200,1300,1400,1500"
#MSSMBINS="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,1000,1500"

#### SVFit Mass MSSM

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="inclusive"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="sasha" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="sasha"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="anyjet_jpt1_20_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_20_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="anyjet_jpt1_30_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_30_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="anyjet_jpt1_40_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="anyjet_jpt1_40_jpt2_30_twomed-tag" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_30_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="inclusive"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="sasha" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="sasha"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="anyjet_jpt1_20_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_20_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="anyjet_jpt1_30_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_30_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="anyjet_jpt1_40_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="anyjet_jpt1_40_jpt2_30_twomed-tag" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_30_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="inclusive"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="em_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="sasha" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="sasha"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="em_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="anyjet_jpt1_20_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_20_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="em_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="anyjet_jpt1_30_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_30_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="em_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="anyjet_jpt1_40_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="em_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --var="m_sv"["$MSSMBINS"] --cat="anyjet_jpt1_40_jpt2_30_twomed-tag" \
  --x_axis_label="M_{#tau#tau} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_30_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=100 --x_blind_max=150 \
  --background_scheme="em_default" 

#### mjj_h REQUIRES HIGGS 4 VECTOR FROM SVFIT

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="inclusive" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="inclusive"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=false --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="et_default" 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="sasha" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="sasha"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="et_default" 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_20_jpt2_20_twomed-tag" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="anyjet_jpt1_20_jpt2_20_twomed-tag"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="et_default" 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_30_jpt2_20_twomed-tag" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="anyjet_jpt1_30_jpt2_20_twomed-tag"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="et_default" 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_40_jpt2_20_twomed-tag" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_20_twomed-tag"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="et_default" 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_40_jpt2_30_twomed-tag" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_30_twomed-tag"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="et_default" 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="inclusive" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="inclusive"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=false --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="mt_with_zmm" 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="sasha" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="sasha"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="mt_with_zmm" 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_20_jpt2_20_twomed-tag" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="anyjet_jpt1_20_jpt2_20_twomed-tag"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="mt_with_zmm" 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_30_jpt2_20_twomed-tag" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="anyjet_jpt1_30_jpt2_20_twomed-tag"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="mt_with_zmm" 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_40_jpt2_20_twomed-tag" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_20_twomed-tag"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="mt_with_zmm" 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_40_jpt2_30_twomed-tag" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_30_twomed-tag"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="mt_with_zmm" 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="inclusive" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="inclusive"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=false --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="em_default" 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="sasha" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="sasha"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="em_default" 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_20_jpt2_20_twomed-tag" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="anyjet_jpt1_20_jpt2_20_twomed-tag"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="em_default" 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_30_jpt2_20_twomed-tag" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="anyjet_jpt1_30_jpt2_20_twomed-tag"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="em_default" 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_40_jpt2_20_twomed-tag" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_20_twomed-tag"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="em_default" 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
#  --method=8 --var="mjj_h"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_40_jpt2_30_twomed-tag" \
#  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
#  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_30_twomed-tag"  \
#  --custom_y_axis_min=true --y_axis_min=0.0099 \
#  --blind=true --x_blind_min=200 --x_blind_max=600 \
#  --background_scheme="em_default" 

#### mjj_tt

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="inclusive"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="sasha" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="sasha"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_20_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_20_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_30_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_30_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_40_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_40_jpt2_30_twomed-tag" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_30_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="et_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="inclusive"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="sasha" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="sasha"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_20_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_20_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_30_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_30_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_40_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_40_jpt2_30_twomed-tag" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_30_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="mt_with_zmm" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="inclusive" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="inclusive"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=false --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="em_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="sasha" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="sasha"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="em_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_20_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_20_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="em_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_30_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_30_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="em_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_40_jpt2_20_twomed-tag" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_20_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="em_default" 

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --var="mjj_tt"["$HIGGSMASSBINS"] --cat="anyjet_jpt1_40_jpt2_30_twomed-tag" \
  --x_axis_label="M_{#tau#tau+jj} [GeV]" $ET_INC_SHIFT \
  --norm_bins=true --datacard="anyjet_jpt1_40_jpt2_30_twomed-tag"  \
  --custom_y_axis_min=true --y_axis_min=0.0099 \
  --blind=true --x_blind_min=200 --x_blind_max=600 \
  --background_scheme="em_default" 


##mjj


./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="sasha" --var="prebjet_mjj(30,0,600)" $ET_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --norm_bins=true --datacard="sasha" \
  --blind=true --x_blind_min=80 --x_blind_max=160 \
  --background_scheme="et_default" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="anyjet_jpt1_20_jpt2_20_twomed-tag" --norm_bins=true --var="prebjet_mjj(30,0,600)" $ET_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="anyjet_jpt1_20_jpt2_20_twomed-tag" \
  --blind=true --x_blind_min=80 --x_blind_max=160 \
  --background_scheme="et_default" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="anyjet_jpt1_30_jpt2_20_twomed-tag" --norm_bins=true --var="prebjet_mjj(30,0,600)" $ET_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="anyjet_jpt1_30_jpt2_20_twomed-tag" \
  --blind=true --x_blind_min=80 --x_blind_max=160 \
  --background_scheme="et_default" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="anyjet_jpt1_40_jpt2_20_twomed-tag" --norm_bins=true --var="prebjet_mjj(30,0,600)" $ET_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="anyjet_jpt1_40_jpt2_20_twomed-tag" \
  --blind=true --x_blind_min=80 --x_blind_max=160 \
  --background_scheme="et_default" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
  --method=8 --cat="anyjet_jpt1_40_jpt2_30_twomed-tag" --norm_bins=true --var="prebjet_mjj(30,0,600)" $ET_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="anyjet_jpt1_40_jpt2_30_twomed-tag" \
  --blind=true --x_blind_min=80 --x_blind_max=160 \
  --background_scheme="et_default" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="sasha" --var="prebjet_mjj(30,0,600)" --norm_bins=true $ET_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="sasha" \
  --blind=true --x_blind_min=80 --x_blind_max=160 \
  --background_scheme="mt_with_zmm" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="anyjet_jpt1_20_jpt2_20_twomed-tag" --norm_bins=true --var="prebjet_mjj(30,0,600)" $ET_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="anyjet_jpt1_20_jpt2_20_twomed-tag" \
  --blind=true --x_blind_min=80 --x_blind_max=160 \
  --background_scheme="mt_with_zmm" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="anyjet_jpt1_30_jpt2_20_twomed-tag" --norm_bins=true --var="prebjet_mjj(30,0,600)" $ET_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="anyjet_jpt1_30_jpt2_20_twomed-tag" \
  --blind=true --x_blind_min=80 --x_blind_max=160 \
  --background_scheme="mt_with_zmm" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="anyjet_jpt1_40_jpt2_20_twomed-tag" --norm_bins=true --var="prebjet_mjj(30,0,600)" $ET_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="anyjet_jpt1_40_jpt2_20_twomed-tag" \
  --blind=true --x_blind_min=80 --x_blind_max=160 \
  --background_scheme="mt_with_zmm" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
  --method=8 --cat="anyjet_jpt1_40_jpt2_30_twomed-tag" --norm_bins=true --var="prebjet_mjj(30,0,600)" $ET_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="anyjet_jpt1_40_jpt2_30_twomed-tag" \
  --blind=true --x_blind_min=80 --x_blind_max=160 \
  --background_scheme="mt_with_zmm" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="sasha" --var="prebjet_mjj(30,0,600)" --norm_bins=true $ET_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="sasha" \
  --blind=true --x_blind_min=80 --x_blind_max=160 \
  --background_scheme="em_default" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="anyjet_jpt1_20_jpt2_20_twomed-tag" --var="prebjet_mjj(30,0,600)" --norm_bins=true $ET_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="anyjet_jpt1_20_jpt2_20_twomed-tag" \
  --blind=true --x_blind_min=80 --x_blind_max=160 \
  --background_scheme="em_default" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="anyjet_jpt1_30_jpt2_20_twomed-tag" --var="prebjet_mjj(30,0,600)" --norm_bins=true $ET_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="anyjet_jpt1_30_jpt2_20_twomed-tag" \
  --blind=true --x_blind_min=80 --x_blind_max=160 \
  --background_scheme="em_default" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="anyjet_jpt1_40_jpt2_20_twomed-tag" --var="prebjet_mjj(30,0,600)" --norm_bins=true $ET_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="anyjet_jpt1_40_jpt2_20_twomed-tag" \
  --blind=true --x_blind_min=80 --x_blind_max=160 \
  --background_scheme="em_default" --extra_pad=1.2

./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=em --set_alias="sel:pzeta>-30" \
  --method=8 --cat="anyjet_jpt1_40_jpt2_30_twomed-tag" --var="prebjet_mjj(30,0,600)" --norm_bins=true $ET_2JET_SHIFT \
  --x_axis_label="M_{jj} [GeV]" --datacard="anyjet_jpt1_40_jpt2_20_twomed-tag" \
  --blind=true --x_blind_min=80 --x_blind_max=160 \
  --background_scheme="em_default" --extra_pad=1.2


#### pt_1

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --cat="inclusive" --var="pt_1(25,0,100)" \
#  --x_axis_label="Electron p_{T} [GeV]" --datacard="inclusive" \
#  --background_scheme="et_default" $ET_INC_SHIFT 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --cat="inclusive" --var="pt_1(25,0,100)" \
#  --x_axis_label="Muon p_{T} [GeV]" --datacard="inclusive" \
#  --background_scheme="mt_with_zmm" $ET_INC_SHIFT 
  
#### pt_2

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=et --set_alias="sel:mt_1<30." \
#  --method=8 --cat="inclusive" --var="pt_2(50,0,100)" \
#  --x_axis_label="p_{T}(#tau_{h}) [GeV]" --datacard="inclusive" \
#  --background_scheme="et_default" $ET_INC_SHIFT 

#./bin/HiggsHTohhPlot --cfg=scripts/new_plot_"$ANA"_"$YEAR".cfg --channel=mt --set_alias="sel:mt_1<30." \
#  --method=8 --cat="inclusive" --var="pt_2(50,0,100)" \
#  --x_axis_label="p_{T}(#tau_{h}) [GeV]" --datacard="inclusive" \
#  --background_scheme="mt_with_zmm" $ET_INC_SHIFT 

