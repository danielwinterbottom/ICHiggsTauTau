for i in $(ls jobs/*.log); do 
  if grep -q "Post-analysis" $i; then 
    continue; 
  else 
    job=$i
    job=${job//.log/".sh"}
    if ! grep -q /vols/grid/cms/setup.sh $job; then
    sed -i '/export SCRAM_ARCH/ i\
source /vols/grid/cms/setup.sh' $job
    fi
    echo $job
    qsub -e /dev/null -o /dev/null -V -q hep.q -l h_rt=0:$1:0 -cwd $job
  fi; 
done
