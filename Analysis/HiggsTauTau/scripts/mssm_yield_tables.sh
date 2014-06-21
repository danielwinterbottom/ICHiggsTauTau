tanb=8
mass=160
et_input=output/mssm-paper-cmb
mt_input=output/mssm-paper-cmb
em_input=output/mssm-paper-cmb
mm_input=output/mssm-paper-cmb
tt_input=output/mssm-paper-cmb
postfit="true"

./bin/MSSMYieldTable --channel=mt --header="$\\Pgm\\Pgt_{h}$ channel" \
  --columns "No B-Tag:8" "B-Tag:9" --eras "7TeV:4935" "8TeV:19712" \
  -d $mt_input/$mass/tmp_8.00.txt -s $mt_input/$mass/htt_mt_*.txt \
  -f $mt_input/$mass/out/mlfit.root:fit_s \
  --signal_mass=$mass --tanb=$tanb --postfit=$postfit &> mt_yields.tex

./bin/MSSMYieldTable --channel=et --header="$\\Pe\\Pgt_{h}$ channel" \
  --columns "No B-Tag:8" "B-Tag:9" --eras "7TeV:4935" "8TeV:19712" \
  -d $et_input/$mass/tmp_8.00.txt -s $et_input/$mass/htt_et_*.txt \
  -f $et_input/$mass/out/mlfit.root:fit_s \
  --signal_mass=$mass --tanb=$tanb --postfit=$postfit &> et_yields.tex

./bin/MSSMYieldTable --channel=em --header="$\\Pe\\Pgm$ channel" \
  --columns "No B-Tag:8" "B-Tag:9" --eras "7TeV:4980" "8TeV:19712" \
  -d $em_input/$mass/tmp_8.00.txt -s $em_input/$mass/htt_em_*.txt \
  -f $em_input/$mass/out/mlfit.root:fit_s \
  --signal_mass=$mass --tanb=$tanb --postfit=$postfit &> em_yields.tex

./bin/MSSMYieldTable --channel=mm --header="$\\Pgm\\Pgm$ channel" \
  --columns "No B-Tag:8" "B-Tag:9" --eras "7TeV:4980" "8TeV:19712" \
  -d $mm_input/$mass/tmp_8.00.txt -s $mm_input/$mass/htt_mm_*.txt \
  -f $mm_input/$mass/out/mlfit.root:fit_s \
  --signal_mass=$mass --tanb=$tanb --postfit=$postfit &> mm_yields.tex

./bin/MSSMYieldTable --channel=tt --header="$\\Pgt_{h}\\Pgt_{h}$ channel" \
  --columns "No B-Tag:8" "B-Tag:9" --eras "8TeV:18400" \
  -d $tt_input/$mass/tmp_8.00.txt -s $tt_input/$mass/htt_tt_*.txt \
  -f $tt_input/$mass/out/mlfit.root:fit_s \
  --signal_mass=$mass --tanb=$tanb --postfit=$postfit &> tt_yields.tex
