#!/bin/bash
# This script generates a file containing a list of diferent cut strings for different category options. A submission script is generated that runs each of those strings over the IC batch returning the AMS as an output file. The channel, number of VBF categories (max 2), number of 1 jet categories (max 3), number of 0 jet categories (max 2) should be passed to the script as input parameters [1] [2] [3] and [4] respectivly. Input parameter [5] should be set to 1 to run the generated submission script.   

export channel=$1
export vbf_cats=$2
export onejet_cats=$3
export zerojet_cats=$4
export submit=$5
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

if [ "$zerojet_cats" == 1 ]; then
  export dirname6=$dirname0"/0Jet/1Cat/"
  mkdir -p $dirname6
elif [ "$zerojet_cats" == 2 ]; then
  export dirname6=$dirname0"/0Jet/2Cat/tight"
  export dirname7=$dirname0"/0Jet/2Cat/loose"
  mkdir -p $dirname6
  mkdir -p $dirname7
fi


export total_jobs=0
rm $dirname0/submitJobs.sh
echo "#!/bin/bash" >> $dirname0/submitJobs.sh
echo "cd /vols/cms/dw515/Offline/CMSSW_8_0_9/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau" >> $dirname0/submitJobs.sh
echo "source /vols/grid/cms/setup.sh" >> $dirname0/submitJobs.sh
echo "eval \`scramv1 runtime -sh\`" >> $dirname0/submitJobs.sh


declare -a tau_id=( "medium" )
declare -a dEta=( 3 4 )
declare -a Mjj=( 400 500 600 700 800 900 )
declare -a HPt=( 0 40 80 100 120 140 )
declare -a HPt_1jet=( 0 40 80 100 120 140 160 180 )
declare -a taupt=( 0 30 40 50 )
declare -a taupt_vbf=( 20 30 )
declare -a taupt_0jet=( 0 5 10 15 20 25 30 35 40 45 50 55 60 )

if [ "$channel" == "tt" ]; then
  declare -a mt=( -1 )
  declare -a taupt_vbf=( 40 50 )
  declare -a taupt=( 40 50 60 )
  declare -a tau_id=( "tight" )
else
  declare -a mt=( 50 )
fi

if [ "$channel" == "et" ]; then
  declare -a tau_id=( "tight" )
  declare -a HPt=( 0 40 80 100 120 140)
  declare -a HPt_1jet=( 0 40 80 120 160 180)
  declare -a taupt=( 0 30 40 50 )
fi

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
taupt_vbf_2=("${taupt_vbf[@]}")
taupt_0jet_2=("${taupt_0jet[@]}")
met_2=("${met[@]}")

export line_count=0
export line_count2=0
export line_count3=0
for i in "${dEta[@]}"; do for j in "${Mjj[@]}"; do for k in "${HPt[@]}"; do for l in "${taupt_vbf[@]}"; do
  export output_line="n_jets>=2 && n_jetsingap==0 && n_bjets==0 && jdeta>="$i" && mjj>="$j" && pt_tt>="$k" && pt_1>="$l
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

          if [ \( "$x" -gt "$x2" -a "$y" -ge "$y2" -a "$z" -ge "$z2" \) -o \( "$x" -ge "$x2" -a "$y" -gt "$y2" -a "$z" -ge "$z2" \) -o \( "$x" -ge "$x2" -a "$y" -ge "$y2" -a "$z" -gt "$z2" \) ]; then
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
  
done; done; done; done;

for i in "${tau_id[@]}"; do for j in "${mt[@]}"; do
   export split=$((line_count/75000+1))
   for ((k=1; k<=$split; k++)); do
     export minjob=$(((k-1)*75000+1))
     export maxjob=$((k*75000))
     if [ "$maxjob" -ge "$line_count" ]; then
       export maxjob=$line_count
     fi
     echo "qsub -q hep.q -l h_rt=0:5:0 -t $minjob-"$maxjob":1 scripts/runOptimizeCategories.sh "$channel" "$dirname1" "$i" "$j >> $dirname0/submitJobs.sh
   done
   ((total_jobs+=line_count))
done; done

if [ "$onejet_cats" -ge 1 ]; then
  for x in "${tau_id[@]}"; do for y in "${mt[@]}"; do
     export split=$((line_count2/75000+1))
     for ((k=1; k<=$split; k++)); do
       export minjob=$(((k-1)*75000+1))
       export maxjob=$((k*75000))
       if [ "$maxjob" -ge "$line_count2" ]; then
         export maxjob=$line_count2
       fi
       echo "qsub -q hep.q -l h_rt=0:5:0 -t $minjob-"$maxjob":1 scripts/runOptimizeCategories.sh "$channel" "$dirname3" "$x" "$y >> $dirname0/submitJobs.sh
     done
     ((total_jobs+=line_count2))
  done; done
fi

if [ "$onejet_cats" -ge 2 ]; then
  for x in "${tau_id[@]}"; do for y in "${mt[@]}"; do
     export split=$((line_count3/75000+1))
     for ((k=1; k<=$split; k++)); do
       export minjob=$(((k-1)*75000+1))
       export maxjob=$((k*75000))
       if [ "$maxjob" -ge "$line_count3" ]; then
         export maxjob=$line_count3
       fi
       echo "qsub -q hep.q -l h_rt=0:5:0 -t $minjob-"$maxjob":1 scripts/runOptimizeCategories.sh "$channel" "$dirname4" "$x" "$y >> $dirname0/submitJobs.sh
     done
     ((total_jobs+=line_count3))
  done; done
fi

if [ "$vbf_cats" == 2 ]; then
  export line_count=0
  for i in "${dEta[@]}"; do for j in "${Mjj[@]}"; do for k in "${HPt[@]}"; do for l in "${taupt_vbf[@]}"; do
    for i2 in "${dEta_2[@]}"; do for j2 in "${Mjj_2[@]}"; do for k2 in "${HPt_2[@]}"; do for l2 in "${taupt_vbf_2[@]}"; do
      if [ \( "$i" -gt "$i2" -a "$j" -ge "$j2" -a "$k" -ge "$k2" -a "$l" -ge "$l2" \) -o \( "$j" -gt "$j2" -a "$i" -ge "$i2" -a "$k" -ge "$k2" -a "$l" -ge "$l2" \) -o \( "$k" -gt "$k2" -a "$i" -ge "$i2" -a "$j" -ge "$j2" -a "$l" -ge "$l2" \) -o \( "$l" -gt "$l2" -a "$k" -ge "$k2" -a "$i" -ge "$i2" -a "$j" -ge "$j2" \) ]; then
        export output_line_tight="!(n_jets>=2 && n_jetsingap==0 && n_bjets==0 && jdeta>="$i" && mjj>="$j" && pt_tt>="$k" && pt_1>="$l")"
        export output_line_loose="n_jets>=2 && n_jetsingap==0 && n_bjets==0 && jdeta>="$i2" && mjj>="$j2" && pt_tt>="$k2" && pt_1>="$l2
        export output_line=$output_line_loose" && "$output_line_tight
        echo $output_line >> $dirname2/cutsInputTemp_$channel.txt
        ((line_count++))
      fi
    done; done; done; done
  done; done; done; done
  
  for i in "${tau_id[@]}"; do for j in "${mt[@]}"; do
     export split=$((line_count/75000+1))
     for ((k=1; k<=$split; k++)); do
       export minjob=$(((k-1)*75000+1))
       export maxjob=$((k*75000))
       if [ "$maxjob" -ge "$line_count" ]; then
         export maxjob=$line_count
       fi
       echo "qsub -q hep.q -l h_rt=0:5:0 -t $minjob-"$maxjob":1 scripts/runOptimizeCategories.sh "$channel" "$dirname2" "$i" "$j >> $dirname0/submitJobs.sh
     done
     ((total_jobs+=line_count))
  done; done
fi

if [ "$zerojet_cats" -ge 1 ]; then
  
  export line_count=0
  for i in "${taupt_0jet[@]}"; do 
    export output_line_tight="n_jets==0 && pt_1>="$i
    export output_line=$output_line_tight
    echo $output_line >> $dirname6/cutsInputTemp_$channel.txt
    ((line_count++))
  done
  
  for i in "${tau_id[@]}"; do for j in "${mt[@]}"; do
     export split=$((line_count/75000+1))
     for ((k=1; k<=$split; k++)); do
       export minjob=$(((k-1)*75000+1))
       export maxjob=$((k*75000))
       if [ "$maxjob" -ge "$line_count" ]; then
         export maxjob=$line_count
       fi
       echo "qsub -q hep.q -l h_rt=0:5:0 -t $minjob-"$maxjob":1 scripts/runOptimizeCategories.sh "$channel" "$dirname6" "$i" "$j >> $dirname0/submitJobs.sh
     done
     ((total_jobs+=line_count))
  done; done
fi
  
if [ "$zerojet_cats" == 2 ]; then

  export line_count=0
  for i in "${taupt_0jet[@]}"; do 
    for i2 in "${taupt_0jet_2[@]}"; do
      if [ "$i" -gt "$i2" ]; then
        if [ "$channel" == "tt" ]; then
          export output_line_tight="!(n_jets==0 && pt_1>="$i")"
          export output_line_loose="n_jets==0 && pt_1>="$i2
        else
          export output_line_tight="!(n_jets==0 && pt_2>="$i")"
          export output_line_loose="n_jets==0 && pt_2>="$i2
        fi 
          export output_line=$output_line_loose" && "$output_line_tight
          echo $output_line >> $dirname7/cutsInputTemp_$channel.txt
        ((line_count++))
      fi
    done
  done
  
  for i in "${tau_id[@]}"; do for j in "${mt[@]}"; do
     export split=$((line_count/75000+1))
     for ((k=1; k<=$split; k++)); do
       export minjob=$(((k-1)*75000+1))
       export maxjob=$((k*75000))
       if [ "$maxjob" -ge "$line_count" ]; then
         export maxjob=$line_count
       fi
       echo "qsub -q hep.q -l h_rt=0:5:0 -t $minjob-"$maxjob":1 scripts/runOptimizeCategories.sh "$channel" "$dirname7" "$i" "$j >> $dirname0/submitJobs.sh
     done
     ((total_jobs+=line_count))
  done; done
fi

echo "#Total Jobs = "$total_jobs >> $dirname0/submitJobs.sh
chmod 755 $dirname0/submitJobs.sh

if [ "$submit" == 1 ]; then
  ./$dirname0/submitJobs.sh
fi
