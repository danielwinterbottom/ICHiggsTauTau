#!/bin/bash
mkdir -p SMPlots/emu
mkdir -p SMPlots/etau
mkdir -p SMPlots/mutau
mkdir -p SMPlots/tautau
declare -a cat=("inclusive" "vbf" "1jet_high_highhpt" "1jet_high_lowhpt" "1jet_low" "1jet_medium" "0jet_low" "0jet_medium" "0jet_high" "1jet_high_highhpt_incbveto" "1jet_high_lowhpt_incbveto" "1jet_low_incbveto" "1jet_medium_incbveto" "vbf_incbveto" "vbf_hadhad")
declare -a channel=("et" "mt" "tt")
for i in "${channel[@]}"
do
  if [ $i == "em" ]; then
    back="em_default"
    folder="SMPlots/emu/"
  elif [ $i == "et" ]; then
    back="et_default"
    folder="SMPlots/etau/"
  elif [ $i == "mt" ]; then
    back="mt_with_zmm"
    folder="SMPlots/mutau/"
  elif [ $i == "tt" ]; then
    back="tt_default"
    folder="SMPlots/tautau/"
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
    ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2016.cfg --channel=$i --set_alias="sel:mt_1<40." --method=8 --var="m_vis"["10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250"] --cat=$j --datacard=$j --x_axis_label="M_{#tau#tau} [GeV]" --blind=$blind --x_blind_min=30 --x_blind_max=200 --background_scheme=$back --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001
    mv "m_vis_"${j}_${i}"_2016.png" $folder.
    mv "m_vis_"${j}_${i}"_2016.pdf" $folder.
    mv "datacard_m_vis_"${j}_${i}"_2016.root" $folder.
  done
done
#./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2016.cfg --channel=mt --set_alias="sel:mt_1<40." --method=8 --var="m_vis"["10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250"] --cat=vbf --datacard=vbf --x_axis_label="M_{#tau#tau} [GeV]" --blind=true --x_blind_min=30 --x_blind_max=200 --background_scheme=mt_with_zmm --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001
