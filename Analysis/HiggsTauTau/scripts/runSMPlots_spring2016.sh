#!/bin/bash
mkdir -p SMPlots/emu
mkdir -p SMPlots/etau
mkdir -p SMPlots/mutau
mkdir -p SMPlots/tautau
declare -a cat=("inclusive")
declare -a channel=("et" "mt" "tt")
for i in "${channel[@]}"
do
  if [ $i == "em" ]; then
    back="em_default"
    folder="SMPlots/emu/"
  elif [ $i == "et" ]; then
    back="et_default"
    folder="SMPlots/etau/"
    method="12"
  elif [ $i == "mt" ]; then
    back="mt_with_zmm"
    folder="SMPlots/mutau/"
    method="12"
  elif [ $i == "tt" ]; then
    back="tt_default"
    folder="SMPlots/tautau/"
    method="8"
  else
    echo "Invalid channel"
  fi
  for j in "${cat[@]}"
  do
    if [ $j == "inclusive" ]; then
      blind="false"
    else
      blind="true"
    fi
    ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2016.cfg --channel=$i --set_alias="sel:mt_1<50." --method=$method --var="m_sv"["0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250"] --cat=$j --x_axis_label="M_{#tau#tau} [GeV]" --blind=$blind --x_blind_min=80 --x_blind_max=140 --background_scheme=$back --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001
    #mv "m_vis_"${j}_${i}"_2016.png" $folder.
    #mv "m_vis_"${j}_${i}"_2016.pdf" $folder.
    #rm "datacard_m_vis_"${j}_${i}"_2016.root" $folder.
  done
done
