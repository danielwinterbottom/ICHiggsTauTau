## HiggsTauTau CP ML methods
Repository for creating training datasets from ROOT files 
and then train on with an algorithm of choice (XGBoost, keras, sklearn etc) / or implement others.

### Git instructions

`git@github.com:danielwinterbottom/ml-htt-methods.git`
`git checkout urops` 
 

### Train
TO DO

### Annotate

To annotate files ROOT files with trained (XBG) model follow these steps:

- choose the year you are running for and directory you are adding the BDT scores for

  `YEAR=2018`
  `DIR=/vols/cms/dw515/masters_polvec_test/CMSSW_10_2_19/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/outputTTMT/`

- make directroies and generate your job scripts 

  `./generate_parajob.sh $YEAR $DIR`

  `mkdir err && mkdir out`

  `mkdir filelist/tmp_${YEAR}_split/ && mkdir filelist/tmp_${YEAR}_split/tt/`

  `cd filelist/tmp_${era}_split/tt/ && python ../../../geteratejobs.py --filelist=../../full_tt_${YEAR}.txt --dir=$DIR && cd ../../../`

- submit the jobs to the batch

  `max=$(ls filelist/tmp_${YEAR}_split/tt/x* | wc -l)`
  `qsub -e err/ -o out/ -cwd -V -l h_rt=3:0:0 -q hep.q -t 1-${max}:1 batch_annotate_parajob_${YEAR}.sh`

