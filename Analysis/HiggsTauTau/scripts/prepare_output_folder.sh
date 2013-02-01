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
	cp -v "$i"_em_2012.root TSCALE_UP/
	cp -v "$i"_em_2012.root TSCALE_DOWN/
done

cd -
