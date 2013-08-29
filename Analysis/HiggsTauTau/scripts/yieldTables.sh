
mass=160
tanb=8
et_input=output/mloutput_0829/et
mt_input=output/mloutput_0829/mt
em_input=output/mloutput_0829/em
tt_input=output/mloutput_0829/tt
mm_input=output/mloutput_0829/mm
postfit="true"
no_correlations="false"
no_shapes="false"
params="scripts/Paper_params_2011.dat,scripts/Paper_params_2012.dat"


./bin/YieldTable --channel=mt --columns="B-Tag:9,No B-Tag:8" --eras="7TeV,8TeV" \
	--datacard_path=$mt_input/$mass --root_file_path=$mt_input/common \
	--pulls_file=$mt_input/160/out/mlfit.txt \
	--signal_mass=$mass --tanb=$tanb --postfit=$postfit --mssm=true \
	--era_params="$params" --no_correlations=$no_correlations --no_shapes=$no_shapes

./bin/YieldTable --channel=et --columns="B-Tag:9,No B-Tag:8" --eras="7TeV,8TeV" \
	--datacard_path=$et_input/$mass --root_file_path=$et_input/common \
	--pulls_file=$et_input/160/out/mlfit.txt \
	--signal_mass=$mass --tanb=$tanb --postfit=$postfit --mssm=true \
	--era_params="$params" --no_correlations=$no_correlations --no_shapes=$no_shapes

./bin/YieldTable --channel=em --columns="B-Tag:9,No B-Tag:8" --eras="7TeV,8TeV" \
	--datacard_path=$em_input/$mass --root_file_path=$em_input/common \
	--pulls_file=$em_input/160/out/mlfit.txt \
	--signal_mass=$mass --tanb=$tanb --postfit=$postfit --mssm=true \
	--era_params="$params" --no_correlations=$no_correlations --no_shapes=$no_shapes

./bin/YieldTable --channel=mm --columns="B-Tag:9,No B-Tag:8" --eras="7TeV,8TeV" \
	--datacard_path=$mm_input/$mass --root_file_path=$mm_input/common \
	--pulls_file=$mm_input/160/out/mlfit.txt \
	--signal_mass=$mass --tanb=$tanb --postfit=$postfit --mssm=true \
	--era_params="$params" --no_correlations=$no_correlations --no_shapes=$no_shapes

./bin/YieldTable --channel=tt --columns="B-Tag:9,No B-Tag:8" --eras="8TeV" \
	--datacard_path=$tt_input/$mass --root_file_path=$tt_input/common \
	--pulls_file=$tt_input/160/out/mlfit.txt \
	--signal_mass=$mass --tanb=$tanb --postfit=$postfit --mssm=true \
	--era_params="scripts/Paper_params_2012.dat" --no_correlations=$no_correlations --no_shapes=$no_shapes