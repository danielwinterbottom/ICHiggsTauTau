if [ -z $1 ]
then
    echo "Usage: prepare_output_folder.sh [output/somefolder]"
    exit
fi

cd $1

cp -v *Data* TSCALE_UP/
cp -v *Data* TSCALE_DOWN/

PATHS=(
'TTJets'
'TT'
'WWJetsTo2L2Nu'
'WZJetsTo2L2Q'
'WZJetsTo3LNu'
'ZZJetsTo2L2Nu'
'ZZJetsTo2L2Q'
'ZZJetsTo4L'
'T-tW'
'Tbar-tW'
'DYJetsToTauTau'
'DYJetsToTauTauSoup'
)

for i in "${PATHS[@]}"
do
	cp -v "$i"_em_201[1,2].root TSCALE_UP/
	cp -v "$i"_em_201[1,2].root TSCALE_DOWN/
done

PATHS=(
'WJetsToLNuSoup'
'TTJets'
'TT'
'WWJetsTo2L2Nu'
'WZJetsTo2L2Q'
'WZJetsTo3LNu'
'ZZJetsTo2L2Nu'
'ZZJetsTo2L2Q'
'ZZJetsTo4L'
'T-tW'
'Tbar-tW'
)

for i in "${PATHS[@]}"
do
  cp -v "$i"_et_201[1,2].root TSCALE_UP/
  cp -v "$i"_et_201[1,2].root TSCALE_DOWN/
  cp -v "$i"_mt_201[1,2].root TSCALE_UP/
  cp -v "$i"_mt_201[1,2].root TSCALE_DOWN/
done

cd -
