## Examples

mass=125
et_input=limit_area/LIMITS-130924-sm/hww-bg/et
mt_input=limit_area/LIMITS-130924-sm/hww-bg/mt_only
em_input=limit_area/LIMITS-130924-sm/hww-bg/em
tt_input=limit_area/LIMITS-130924-sm/hww-bg/tt
ignore_corrs="false"
lumi_title_7tev="CMS Preliminary, #sqrt{s} = 7 TeV, L = 4.9 fb^{-1}"
lumi_title_8tev="CMS Preliminary, #sqrt{s} = 8 TeV, L = 19.8 fb^{-1}"
blind="--blind=false --x_blind_min=100 --x_blind_max=160"
cfg="scripts/new_plot_sm_2012.cfg"

POSTFITS=(
	'false'
	'true'
	)
POSTFITS_LABEL=(
	'prefit'
	'postfit'
	)

POISSON=(
	'false'
	'true'
	)
POISSON_LABEL=(
	''
	'_poisson'
	)


CATS=(
	'All Categories'
	'0-Jet'
	'1-Jet'
	'VBF'
	)
CATSREBIN=(
	'true'
	'false'
	'false'
	'true'
	)
CATSLABEL=(
	'all'
	'0jet'
	'1jet'
	'vbf'
	)
CATS_ET=(
	'.'
	'[12]'
	'[345]'
	'[67]'
	)
CATS_MT=(
	'.'
	'[12]'
	'[345]'
	'[67]'
	)
CATS_EM=(
	'.'
	'[01]'
	'[23]'
	'[45]'
	)
CATS_TT=(
	'.'
	'[23]'
	'[23]'
	'[45]'
	)

for ((j=0 ; j<${#CATS[*]} ; j++))
do
	for ((i=0 ; i<${#POISSON[*]} ; i++))
	do
		for ((k=0 ; k<${#POSTFITS[*]} ; k++))
		do

		###################################### mu-tau 8TeV
		./bin/SOBPlot --cfg=$cfg \
		 --datacard_path=$et_input/$mass \
		 --datacard_regex="htt_.._"${CATS_ET[$j]}"_.TeV.txt" \
		 --root_file_path=$et_input/common \
		 --root_file_regex="htt.*root" \
		 --pulls_file=$et_input/$mass/out/mlfit.txt \
		 --signal_mass=$mass --postfit=${POSTFITS[$k]} --mssm=false --norm_bins=true \
		 --title_left="CMS Preliminary, 4.9 fb^{-1} at 7TeV, 19.7 fb^{-1} at 8TeV" \
		 $blind --rebin_to_vbf=${CATSREBIN[$j]} \
		 --background_scheme=all --signal_scheme=sm_default --draw_error_band=true \
		 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		 --extra_pad=1.2 --split_zll=true \
		 --text1="e#tau_{h}" --text2="${CATS[$j]}" \
		 --output="sob_et_"${CATSLABEL[$j]}"_"${POSTFITS_LABEL[$k]}${POISSON_LABEL[$i]} \
		 --ignore_corrs=$ignore_corrs --poisson_errors=${POISSON[$i]}

		./bin/SOBPlot --cfg=$cfg \
		 --datacard_path=$mt_input/$mass \
		 --datacard_regex="htt_.._"${CATS_MT[$j]}"_.TeV.txt" \
		 --root_file_path=$mt_input/common \
		 --root_file_regex="htt.*root" \
		 --pulls_file=$mt_input/$mass/out/mlfit.txt \
		 --signal_mass=$mass --postfit=${POSTFITS[$k]} --mssm=false --norm_bins=true \
		 --title_left="CMS Preliminary, 4.9 fb^{-1} at 7TeV, 19.7 fb^{-1} at 8TeV" \
		 $blind --rebin_to_vbf=${CATSREBIN[$j]} \
		 --background_scheme=all --signal_scheme=sm_default --draw_error_band=true \
		 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		 --extra_pad=1.2 --split_zll=true \
		 --text1="#mu#tau_{h}" --text2="${CATS[$j]}" \
		 --output="sob_mt_"${CATSLABEL[$j]}"_"${POSTFITS_LABEL[$k]}${POISSON_LABEL[$i]} \
		 --ignore_corrs=$ignore_corrs --poisson_errors=${POISSON[$i]}		 

		./bin/SOBPlot --cfg=$cfg \
		 --datacard_path=$em_input/$mass \
		 --datacard_regex="htt_.._"${CATS_EM[$j]}"_.TeV.txt" \
		 --root_file_path=$em_input/common \
		 --root_file_regex="htt.*root" \
		 --pulls_file=$em_input/$mass/out/mlfit.txt \
		 --signal_mass=$mass --postfit=${POSTFITS[$k]} --mssm=false --norm_bins=true \
		 --title_left="CMS Preliminary, 4.9 fb^{-1} at 7TeV, 19.7 fb^{-1} at 8TeV" \
		 $blind --rebin_to_vbf=${CATSREBIN[$j]} \
		 --background_scheme=all --signal_scheme=sm_default --draw_error_band=true \
		 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		 --extra_pad=1.2 --split_zll=false \
		 --text1="e#mu" --text2="${CATS[$j]}" \
		 --output="sob_em_"${CATSLABEL[$j]}"_"${POSTFITS_LABEL[$k]}${POISSON_LABEL[$i]} \
		 --ignore_corrs=$ignore_corrs --poisson_errors=${POISSON[$i]}		 
		done
	done
done

CATS=(
	'All Categories'
	'1-Jet'
	'VBF'
	)
CATSREBIN=(
	'true'
	'false'
	'true'
	)
CATSLABEL=(
	'all'
	'1jet'
	'vbf'
	)
CATS_TT=(
	'.'
	'[01]'
	'[2]'
	)

for ((j=0 ; j<${#CATS[*]} ; j++))
do
	for ((i=0 ; i<${#POISSON[*]} ; i++))
	do
		for ((k=0 ; k<${#POSTFITS[*]} ; k++))
		do

		###################################### mu-tau 8TeV
		./bin/SOBPlot --cfg=$cfg \
		 --datacard_path=$tt_input/$mass \
		 --datacard_regex="htt_.._"${CATS_TT[$j]}"_.TeV.txt" \
		 --root_file_path=$tt_input/common \
		 --root_file_regex="htt.*root" \
		 --pulls_file=$tt_input/$mass/out/mlfit.txt \
		 --signal_mass=$mass --postfit=${POSTFITS[$k]} --mssm=false --norm_bins=true \
		 --title_left="CMS Preliminary, 19.7 fb^{-1} at 8TeV" \
		 $blind --rebin_to_vbf=${CATSREBIN[$j]} \
		 --background_scheme=all --signal_scheme=sm_default --draw_error_band=true \
		 --x_axis_label="m_{#tau#tau} [GeV]" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]" \
		 --extra_pad=1.2 --split_zll=false \
		 --text1="#tau_{h}#tau_{h}" --text2="${CATS[$j]}" \
		 --output="sob_tt_"${CATSLABEL[$j]}"_"${POSTFITS_LABEL[$k]}${POISSON_LABEL[$i]} \
		 --ignore_corrs=$ignore_corrs --poisson_errors=${POISSON[$i]}
		done
	done
done


