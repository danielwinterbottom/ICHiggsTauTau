#!/bin/bash
#
export channel=$1
export dirname=$2
export mva=$3
export mt=$4

if [ "$channel" == "tt" ]; then
  export method="8"
  if [ "$mva" == "loose" ]; then
    export mva_string='--set_alias=baseline:(mva_olddm_loose_1>0.5 && mva_olddm_loose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)' 
    export qcd_string='--set_alias=tt_qcd_norm:(mva_olddm_vloose_1>0.5 && mva_olddm_vloose_2>0.5 && mva_olddm_loose_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)'
  elif [ "$mva" == "medium" ]; then
    export mva_string='--set_alias=baseline:(mva_olddm_medium_1>0.5 && mva_olddm_medium_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)'
    export qcd_string='--set_alias=tt_qcd_norm:(mva_olddm_loose_1>0.5 && mva_olddm_loose_2>0.5 &&mva_olddm_medium_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)'
  elif [ "$mva" == "tight" ]; then
    export mva_string='--set_alias=baseline:(mva_olddm_tight_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)'
    export qcd_string='--set_alias=tt_qcd_norm:(mva_olddm_medium_1>0.5 && mva_olddm_loose_2>0.5 &&mva_olddm_tight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)'
  elif [ "$mva" == "vtight" ]; then
    export mva_string='--set_alias=baseline:(mva_olddm_vtight_1>0.5 && mva_olddm_vtight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)'
    export qcd_string='--set_alias=tt_qcd_norm:(mva_olddm_tight_1>0.5 && mva_olddm_loose_2>0.5 &&mva_olddm_vtight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)'
  fi
else
  export method="12"
  export qcd_string=" "
  if [ "$mva" == "loose" ]; then
    export mva_string='--set_alias=baseline:(iso_1<0.1 && mva_olddm_loose_2>0.5 && antiele_2 && antimu_2 && leptonveto==0)'
  elif [ "$mva" == "medium" ]; then
    export mva_string='--set_alias=baseline:(iso_1<0.1 && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && leptonveto==0)'
  elif [ "$mva" == "tight" ]; then
    export mva_string='--set_alias=baseline:(iso_1<0.1 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0)'
  elif [ "$mva" == "vtight" ]; then
    export mva_string='--set_alias=baseline:(iso_1<0.1 && mva_olddm_vtight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0)'
  fi
fi

if [ "$mt" \< 0 ]; then
  export mt_string="mt_1>-1"
else
  export mt_string="mt_1<"$mt
fi

export iso_mt_string=$mva" && "$mt_string

if [ "$channel" == "et" ]; then
  export back="et_default"
elif [ "$channel" == "mt" ]; then
  export back="mt_with_zmm"
elif [ "$channel" == "tt" ]; then
  export back="tt_default"
fi

cd /vols/cms/dw515/Offline/CMSSW_8_0_9/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau
source /vols/grid/cms/setup.sh
eval `scramv1 runtime -sh`
export count=1
IFS=$'\n'
export num=${SGE_TASK_ID}
while read line; do    
  if [ $count == $num ]; then
    export alias=$line
  fi
  ((count++))
done < $dirname/cutsInputTemp_$channel.txt
export alias="1"
./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2016.cfg --channel=$channel --method=$method --var='m_vis'['10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250'] --cat=variable_cat --x_axis_label='M_{#tau#tau} [GeV]' --blind=true --x_blind_min=30 --x_blind_max=200 --background_scheme=$back --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001 --set_alias='sel:('$mt_string')' $mva_string $qcd_string --set_alias='variable_cat:('$alias')' --supress_output=true --sOverb_output_name=$dirname/outputTemp_$mva"_mt"$mt"_"$num.txt
paste <(cat $dirname/outputTemp_$mva'_mt'$mt'_'$num.txt) <(echo ' && '$iso_mt_string' && '$alias) > $dirname/output_$mva'_mt'$mt'_'$num.txt
rm $dirname/outputTemp_$mva'_mt'$mt'_'$num.txt
