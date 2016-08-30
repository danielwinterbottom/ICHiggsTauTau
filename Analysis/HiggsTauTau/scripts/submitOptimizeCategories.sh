#!/bin/bash
#

export channel=$1
export vbf_cats=$2
export onejet_cats=$3
export submit=$4
IFS='%'

export now=$(date +"%Y_%m_%d_%H_%M_%S")

export dirname0=$channel"_output/"$now
mkdir -p $dirname0

if [ "$vbf_cats" == 1 ]; then
  export dirname1=$dirname0"/2Jet/1Cat"
  mkdir -p $dirname1
elif [ "$vbf_cats" == 2 ]; then
  dirname1=$dirname0"/2Jet/2Cat/tight"
  dirname2=$dirname0"/2Jet/2Cat/loose"
  mkdir -p $dirname1
  mkdir -p $dirname2
fi

if [ "$onejet_cats" == 1 ]; then
  export dirname3=$dirname0"/1Jet/1Cat"
  mkdir -p $dirname3
elif [ "$onejet_cats" == 2 ]; then
  export dirname3=$dirname0"/1Jet/2Cat/tight"
  export dirname4=$dirname0"/1Jet/2Cat/loose"
  mkdir -p $dirname3
  mkdir -p $dirname4
elif [ "$onejet_cats" == 3 ]; then
  export dirname3=$dirname0"/1Jet/2Cat/tight"
  export dirname4=$dirname0"/1Jet/2Cat/medium"
  export dirname5=$dirname0"/1Jet/2Cat/loose"
  mkdir -p $dirname3
  mkdir -p $dirname4
  mkdir -p $dirname5
fi

export total_jobs=0
rm $dirname0/submitJobs.sh
echo "#!/bin/bash" >> $dirname0/submitJobs.sh
echo "cd /vols/cms/dw515/Offline/CMSSW_8_0_9/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau" >> $dirname0/submitJobs.sh
echo "source /vols/grid/cms/setup.sh" >> $dirname0/submitJobs.sh
echo "eval \`scramv1 runtime -sh\`" >> $dirname0/submitJobs.sh

if [ "$channel" == "tt" ]; then
  declare -a mt=( -1 )
else
  declare -a mt=( 50 )
fi
declare -a tau_id=( "tight" )
declare -a dEta=( 3 4 )
declare -a Mjj=( 400 500 600 700 800 )
declare -a HPt=( 0 50 100 150 )
declare -a HPt_1jet=( 0 50 100 150 )
declare -a taupt=( 0 30 40 )

if [ "$channel" == "et" ]; then
  declare -a met=( 0 20 30 40 )
else
  declare -a met=( 0 )
fi

dEta_2=("${dEta[@]}")
Mjj_2=("${Mjj[@]}")
HPt_2=("${HPt[@]}")
HPt_1jet_2=("${HPt_1jet[@]}")
taupt_2=("${taupt[@]}")
met_2=("${met[@]}")

export line_count=0
export line_count2=0
export line_count3=0
for i in "${dEta[@]}"; do for j in "${Mjj[@]}"; do for k in "${HPt[@]}"; do
  export output_line="n_jets>=2 && n_jetsingap==0 && n_bjets==0 && jdeta>="$i" && mjj>="$j" && pt_tt>="$k
  echo $output_line >> $dirname1/cutsInputTemp_$channel.txt
  ((line_count++))
  
  if [ "$onejet_cats" -ge 1 ]; then
    for x in "${HPt_1jet[@]}"; do for y in "${taupt[@]}"; do for z in "${met}"; do
      if [ "$channel" == "tt" ]; then
        export output_line2="n_jets>=1 && n_bjets==0 && pt_tt>="$x" && pt_1>="$y" && met>="$z
      else
        export output_line2="n_jets>=1 && n_bjets==0 && pt_tt>="$x" && pt_2>="$y" && met>="$z
      fi
      echo $output_line2" && !("$output_line")" >> $dirname3/cutsInputTemp_$channel.txt
      ((line_count2++))
    done; done; done
    
    if [ "$onejet_cats" -ge 2 ]; then
      for x in "${HPt_1jet[@]}"; do for y in "${taupt[@]}"; do for z in "${met[@]}"; do
        for x2 in "${HPt_1jet_2[@]}"; do for y2 in "${taupt_2[@]}"; do for z2 in ${met_2}; do

          if [ \( "$x" \< "$x2" -o ${#HPt_1jet[@]} \> 2 \) -a \( "$y" \> "$y2" -o ${#taupt[@]} \< 2 \) -a \( "$z" \> "$z2" -o ${#met[@]} \< 2 \) ]; then
            if [ "$channel" == "tt" ]; then
              export output_line2_tight="!(n_jets>=1 && n_bjets==0 && pt_tt>="$x" && pt_1>="$y" && met>="$z")"
              export output_line2_loose="n_jets>=1 && n_bjets==0 && pt_tt>="$x2" && pt_1>="$y2" && met>="$z2
            else
              export output_line2_tight="!(n_jets>=1 && n_bjets==0 && pt_tt>="$x" && pt_2>="$y" && met>="$z")"
              export output_line2_loose="n_jets>=1 && n_bjets==0 && pt_tt>="$x2" && pt_2>="$y2" && met>="$z2
            fi
            export output_line2=$output_line2_loose" && "$output_line2_tight
            echo $output_line2" && !("$output_line")" >> $dirname4/cutsInputTemp_$channel.txt
            ((line_count3++))
          fi    
        done; done; done
      done; done; done
    
    fi
  fi
  
done; done; done;

for i in "${tau_id[@]}"; do for j in "${mt[@]}"; do
   echo "qsub -q hep.q -l h_rt=0:5:0 -t 1-"$line_count":1 scripts/runOptimizeCategories.sh "$channel" "$dirname1" "$i" "$j >> $dirname0/submitJobs.sh
   ((total_jobs+=line_count))
done; done

for x in "${tau_id[@]}"; do for y in "${mt[@]}"; do
   echo "qsub -q hep.q -l h_rt=0:5:0 -t 1-"$line_count2":1 scripts/runOptimizeCategories.sh "$channel" "$dirname3" "$x" "$y >> $dirname0/submitJobs.sh
   ((total_jobs+=line_count2))
done; done

for x in "${tau_id[@]}"; do for y in "${mt[@]}"; do
   echo "qsub -q hep.q -l h_rt=0:5:0 -t 1-"$line_count3":1 scripts/runOptimizeCategories.sh "$channel" "$dirname4" "$x" "$y >> $dirname0/submitJobs.sh
   ((total_jobs+=line_count3))
done; done

if [ "$vbf_cats" == 2 ]; then
  export line_count=0
  for i in "${dEta[@]}"; do for j in "${Mjj[@]}"; do for k in "${HPt[@]}"; do
    for i2 in "${dEta_2[@]}"; do for j2 in "${Mjj_2[@]}"; do for k2 in "${HPt_2[@]}"; do
      if [ \( "$i" \> "$i2" -o ${#dEta[@]} \< 2 \) -a \( "$j" \> "$j2" -o ${#Mjj[@]} \< 2 \) -a \( "$k" \> "$k2" -o ${#HPt[@]} \< 2 \) ]; then
        export output_line_tight="!(n_jets>=2 && n_jetsingap==0 && n_bjets==0 && jdeta>="$i" && mjj>="$j" && pt_tt>="$k")"
        export output_line_loose="n_jets>=2 && n_jetsingap==0 && n_bjets==0 && jdeta>="$i2" && mjj>="$j2" && pt_tt>="$k2
        export output_line=$output_line_loose" && "$output_line_tight
        echo $output_line >> $dirname2/cutsInputTemp_$channel.txt
        ((line_count++))
      fi
    done; done; done
  done; done; done
  
  for i in "${tau_id[@]}"; do for j in "${mt[@]}"; do
     echo "qsub -q hep.q -l h_rt=0:5:0 -t 1-"$line_count":1 scripts/runOptimizeCategories.sh "$channel" "$dirname2" "$i" "$j >> $dirname0/submitJobs.sh
     ((total_jobs+=line_count))
  done; done
fi

echo "#Total Jobs = "$total_jobs >> $dirname0/submitJobs.sh
chmod 755 $dirname0/submitJobs.sh

if [ "$submit" == 1 ]; then
  ./$dirname0/submitJobs.sh
fi
