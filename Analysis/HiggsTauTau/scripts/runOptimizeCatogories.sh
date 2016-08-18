#!/bin/bash
#
export channel=$1
export dirname=$2
cd /vols/cms/dw515/Offline/CMSSW_8_0_9/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau
source /vols/grid/cms/setup.sh
eval `scramv1 runtime -sh`
export count=1
IFS=$'\n'
export num=${SGE_TASK_ID}
while read line; do    
  if [ $count == $num ]; then
    export alias="&& "$line
  fi
  ((count++))
done < $dirname/cutsInputTemp_$channel.txt
./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2016.cfg --channel=$channel --method=8 --var="m_vis"["10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250"] --cat=inclusive --x_axis_label="M_{#tau#tau} [GeV]" --blind=true --x_blind_min=30 --x_blind_max=200 --background_scheme=mt_with_zmm --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001 --set_alias="sel:(mt_1<40. "$alias")" --supress_output=true --sOverb_output_name=$dirname/outputTemp_$num.txt
paste <(cat $dirname/outputTemp_$num.txt) <(echo $alias) > $dirname/output_$num.txt 
rm $dirname/outputTemp_$num.txt
