DIR=$1

echo cd ${PWD} > jobs/duplicates_check_para_job.sh
echo export X509_USER_PROXY=/home/hep/dw515/cms.proxy >> jobs/duplicates_check_para_job.sh
echo source /vols/grid/cms/setup.sh >> jobs/duplicates_check_para_job.sh
echo export SCRAM_ARCH=slc6_amd64_gcc481 >> jobs/duplicates_check_para_job.sh
echo eval \`scramv1 runtime -sh\` >> jobs/duplicates_check_para_job.sh
echo source /vols/cms/dw515/workareas/UL_ditau/new_JEC/CMSSW_10_2_19/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/setup_libs.sh >> jobs/duplicates_check_para_job.sh
echo ulimit -c 0 >> jobs/duplicates_check_para_job.sh

COUNT=1; for x in ${DIR}/*.root; do echo if [ \$\{SGE_TASK_ID\} == ${COUNT} ]\; then python -u scripts/check_unique_events.py -f ${x} \&\> jobs/duplicate_check_${COUNT}.out\; fi; ((COUNT++)); done >> jobs/duplicates_check_para_job.sh

qsub -e /dev/null -o /dev/null -V -q hep.q -l h_rt=0:180:0 -l h_vmem=24G -t 1-${COUNT}:1 jobs/duplicates_check_para_job.sh
