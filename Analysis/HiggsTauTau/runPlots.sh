#!/bin/bash
declare -a cat=("inclusive" "vbf" "1jet_high_highhpt", "1jet_high_lowhpt", "1jet_low", "1jet_medium", "0jet_low", "0jet_medium", "0jet_high")
declare -a channel=("et" "mt" "tt")
for i in "${channel[@]}"
  if [$i="et"]; then
    back="et_default"
    folder=SMPlots/etau
  elif [$i="mt"]; then
    back="mt_with_zmm"
    folder=SMPlots/mutau
  elif [$i="tt"] then;
    back="tt_default"
    folder=SMPlots/tautau
  do
    for j in "${cat[@]}"
    blind="true"
    if [$j="inclusive"]; then
      blind="false"
    do
      ./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2015.cfg --channel=$i --set_alias="sel:mt_1<30." --method=8 --var="m_sv"["50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250"] --cat=$j --datacard=$j --x_axis_label="M_{#tau#tau} [GeV]" --blind=$blind --x_blind_min=10 --x_blind_max=2000 --background_scheme=$back --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001 --folder=$folder
    done
  done
