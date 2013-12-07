tanb=8
mass=125
et_input=limit_area/LIMITS-130924-sm/hww-bg/et
mt_input=bbb-mlfit/cmb
em_input=limit_area/LIMITS-130924-sm/hww-bg/em
tt_input=limit_area/LIMITS-130924-sm/hww-bg/tt
postfit="true"
no_correlations="false"
no_shapes="false"
params="scripts/Paper_params_2011.dat,scripts/Paper_params_2012.dat"


PTTH="p_{T}^{#tau_{h}}"

./bin/YieldTable --channel=mt --columns="0-Jet:1+2,1-Jet:3+4+5,VBF:6+7" --eras="7TeV,8TeV" \
	--datacard_path=$mt_input/$mass --root_file_path=$mt_input/common \
	--pulls_file=$mt_input/$mass/out/mlfit.txt \
	--signal_mass=$mass --tanb=$tanb --postfit=$postfit --mssm=false \
	--era_params="$params" --no_correlations=$no_correlations --no_shapes=$no_shapes

./bin/YieldTable --channel=mt --columns="0-Jet Medium:1,0-Jet High:2,1-Jet Medium:3,1-Jet High:4,1-Jet High Boost:5,VBF Loose:5,VBF Tight:6" --eras="7TeV,8TeV" \
	--datacard_path=$mt_input/$mass --root_file_path=$mt_input/common \
	--pulls_file=$mt_input/$mass/out/mlfit.txt \
	--signal_mass=$mass --tanb=$tanb --postfit=$postfit --mssm=false \
	--era_params="$params" --no_correlations=$no_correlations --no_shapes=$no_shapes


# ./bin/YieldTable --channel=et --columns="B-Tag:9,No B-Tag:8" --eras="7TeV,8TeV" \
# 	--datacard_path=$et_input/$mass --root_file_path=$et_input/common \
# 	--pulls_file=$et_input/$mass/out/mlfit.txt \
# 	--signal_mass=$mass --tanb=$tanb --postfit=$postfit --mssm=true \
# 	--era_params="$params" --no_correlations=$no_correlations --no_shapes=$no_shapes

# ./bin/YieldTable --channel=em --columns="B-Tag:9,No B-Tag:8" --eras="7TeV,8TeV" \
# 	--datacard_path=$em_input/$mass --root_file_path=$em_input/common \
# 	--pulls_file=$em_input/$mass/out/mlfit.txt \
# 	--signal_mass=$mass --tanb=$tanb --postfit=$postfit --mssm=true \
# 	--era_params="$params" --no_correlations=$no_correlations --no_shapes=$no_shapes

# ./bin/YieldTable --channel=mm --columns="B-Tag:9,No B-Tag:8" --eras="7TeV,8TeV" \
# 	--datacard_path=$mm_input/$mass --root_file_path=$mm_input/common \
# 	--pulls_file=$mm_input/$mass/out/mlfit.txt \
# 	--signal_mass=$mass --tanb=$tanb --postfit=$postfit --mssm=true \
# 	--era_params="$params" --no_correlations=$no_correlations --no_shapes=$no_shapes

# ./bin/YieldTable --channel=tt --columns="B-Tag:9,No B-Tag:8" --eras="8TeV" \
# 	--datacard_path=$tt_input/$mass --root_file_path=$tt_input/common \
# 	--pulls_file=$tt_input/$mass/out/mlfit.txt \
# 	--signal_mass=$mass --tanb=$tanb --postfit=$postfit --mssm=true \
# 	--era_params="scripts/Paper_params_2012.dat" --no_correlations=$no_correlations --no_shapes=$no_shapes