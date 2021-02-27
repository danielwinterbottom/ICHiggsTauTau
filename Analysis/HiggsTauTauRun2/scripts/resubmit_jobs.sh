COUNT=0
TOTAL=0
for i in $(ls jobs/*.sh); do 
  if [[ $i = *"parajob_"* ]]; then continue; fi
  if [[ $i = *"hadd_"* ]]; then continue; fi 
  (( TOTAL++ ))
  job=$i
  i=${job//.sh/".log"}
  if grep -q "End of job" $i && ! grep -q "Disk quota exceeded" $i && ! grep -q "Auth failed" $i ; then
    continue;
  else 
    if ! grep -q /vols/grid/cms/setup.sh $job; then
    sed -i '/export SCRAM_ARCH/ i\
source /vols/grid/cms/setup.sh' $job
    fi
    echo $job
    (( COUNT++ ))
    if [[ $1 != "" ]]; then
      qsub -e /dev/null -o /dev/null -V -q hep.q -l h_rt=0:$1:0 -l h_vmem=24G -cwd $job
      #qsub -e /dev/null -o /dev/null -V -q hep.q -l h_rt=0:$1:0 -l h_vmem=24G -pe hep.pe 2 -cwd $job
      #qsub -e /dev/null -o /dev/null -V -q hep.q -l h_rt=0:$1:0 -cwd $job
    fi
  fi; 
done
echo $COUNT jobs out of $TOTAL failed
